/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   17:15
	filename: 	F:\vicente\Code\src\XJBrowser\DataEngine.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	DataEngine
	file ext:	cpp
	author:		LYH
	
	purpose:	数据引擎
*********************************************************************/
// DataEngine.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DataEngine.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString virtualStationID = "vStation";
CString virtualStationName = "";

BOOL bAddVirtual = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CDataEngine
//##ModelId=49B87BA9003F
CDataEngine::CDataEngine()
{
	m_NetList.RemoveAll();		//地区链表
	m_StationMap.RemoveAll();	//厂站链表
	m_AllSEC.RemoveAll();
	m_arrMark.RemoveAll();
	m_SepStationMap.RemoveAll();
	
	m_arrUniversalActChr.RemoveAll();

	virtualStationName = StringFromID(IDS_MODEL_VIRTUALSTATION);

	/** @brief           保护召唤定值超时时间(单位:秒)*/
	m_nPTSettingTimeOut = 240;
	/** @brief           录波器召唤定值超时时间(单位:秒)*/
	m_nWRSettingTimeOut = 500;
	/** @brief           保护召唤模拟量超时时间(单位:秒)*/
	m_nPTAiTimeOut = 120;
	/** @brief           保护召唤开关量超时时间(单位:秒)*/
	m_nPTDiTimeOut = 120;
	/** @brief           保护召唤软压板超时时间(单位:秒)*/
	m_nPTSoftBoardTimeOut = 120;
	/** @brief           保护召唤定值区号超时时间(单位:秒)*/
	m_nPTZoneTimeOut = 120;
	/** @brief           召唤装置时间超时时间(单位:秒)*/
	m_nDevTimeTimeOut = 120;
	/** @brief           召唤装置基本信息超时时间(单位:秒)*/
	m_nDevGeneralTimeOut = 120;
	/** @brief           定值修改超时时间(单位:秒)*/
	m_nModifySettingTimeOut = 120;
	/** @brief           定值区切换超时时间(单位:秒)*/
	m_nChangeZoneTimeOut = 120;
	/** @brief           软压板切换超时时间(单位:秒)*/
	m_nChangeSBTimeOut = 120;
	/** @brief           装置对时超时时间(单位:秒)*/
	m_nVerifyTimeTimeOut = 120;
	/** @brief           信号复归超时时间(单位:秒)*/
	m_nSignalResetTimeOut = 120;
	/** @brief           远方启动超时时间(单位:秒)*/
	m_nRemoteStartTimeOut = 120;
	/** @brief           召唤录波文件超时时间(单位:秒)*/
	m_nWavFileTimeOut = 120;
	/** @brief           召唤录波列表超时时间(单位:秒)*/
	m_nWavFileListTimeOut = 120;
	/** @brief           召唤历史事件超时时间(单位:秒)*/
	m_nHistoryEventTimeOut = 120;
	/** @brief           召唤通用文件超时时间(单位:秒)*/
	m_nCommFileTimeOut = 120;
	/** @brief           召唤通用文件列表超时时间(单位:秒)*/
	m_nCommFileListTimeOut = 120;
	/** @brief           召唤通用文件列表超时时间(单位:秒)*/
	m_nCommStatusTimeOut = 120;
	m_pMemset = new CMemSet;
	m_pThread = NULL;
	m_mapServiceDepartment.RemoveAll();
	::InitializeCriticalSection(&m_criSection);
}

//##ModelId=49B87BA90179
CDataEngine::~CDataEngine()
{
	MYDELETE(m_pMemset);
	::DeleteCriticalSection(&m_criSection);
	ClearRefreshSec();
	Clear();
}
/////////////////////////////////////////////////////////////////////////////
// CDataEngine
/*************************************************************
 函 数 名：BuildNetList()
 功能概要：读取数据库中地区数据, 生成地区对象, 建立地区链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA900AC
BOOL CDataEngine::BuildNetList()
{
	ClearNetList();
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(id in(select area_id from tb_dev_station_base where station_id in(select station_id from tb_user_related where user_id='%s')))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}

		Condition con1;
		pApp->m_DBEngine.SetCondition( sql, con1, "order by dis_seq ASC", EX_STTP_WHERE_ABNORMALITY);
		
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_NET_CFG, sql, sError, pMemset);
			if( !nResult )
			{
				sql.Conditionlist.clear();
				nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_NET_CFG, sql, sError, pMemset);
			}
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildNetList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildNetList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CNetObj * pObj = new CNetObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,加入到链表
					m_NetList.AddTail(pObj);
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildNetList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	return bReturn;
}

/*************************************************************
 函 数 名：BuildStationList()
 功能概要：读取数据库中厂站数据, 生成厂站对象, 建立厂站链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA900BB
BOOL CDataEngine::BuildStationList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//条件
		Condition con1;
		CString str;
		str="(TYPE=1 or TYPE=4 or TYPE=7 or TYPE=8 or TYPE=9 or TYPE=113 or TYPE=114)";
		pApp->m_DBEngine.SetCondition(sql, con1, str);

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}

		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildStationList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildStationList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CStationObj * pObj = new CStationObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,加入到链表
					m_StationMap.SetAt(pObj->m_sID, pObj);
					if(g_LoadDataModel == 1)
					{
						pObj->InitLock();
						pObj->LoadData();
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildStationList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildBayList()
 功能概要：读取数据库中间隔数据, 生成间隔对象, 建立间隔链表
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
//##ModelId=49B87BA900EA
BOOL CDataEngine::BuildBayList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}

		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BAY_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildBayList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildBayList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CBayObj * pObj = new CBayObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_BAY, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBayList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildBreakList()
 功能概要：读取数据库中开关数据, 生成开关对象, 建立开关链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA900DB
BOOL CDataEngine::BuildBreakList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BREAKER_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildBreakList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			//查询成功
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildBreakList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CBreakObj * pObj = new CBreakObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_BREAK, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBreakList, 查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildBusList()
 功能概要：读取数据库中母线数据, 生成母线对象, 建立母线链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA900DA
BOOL CDataEngine::BuildBusList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BUS_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildBusList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildBusList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CBusObj * pObj = new CBusObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_BUS, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBusList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildCapList()
 功能概要：读取数据库中电容器数据, 生成电容器对象, 建立电容器链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA9010A
BOOL CDataEngine::BuildCapList()
{
	
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("station_id in(select station_id from tb_user_related where user_id='%s')", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
		
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CAPACITOR_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildCapList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildCapList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CCapObj * pObj = new CCapObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_CAP, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildCapList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildGenList()
 功能概要：读取数据库中发电机数据, 生成发电机对象, 建立发电机链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90129
BOOL CDataEngine::BuildGenList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_GEN_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildGenList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildGenList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CGenObj * pObj = new CGenObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_GEN, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildGenList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildLineList()
 功能概要：读取数据库中线路数据, 生成路线对象, 建立路线链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90109
BOOL CDataEngine::BuildLineList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "line_id", EX_STTP_DATA_TYPE_STRING);
		
		Field Field2;
		pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
		
		Field Field3;
		pApp->m_DBEngine.SetField(sql, Field3, "code", EX_STTP_DATA_TYPE_INT);
		
		Field Field4;
		pApp->m_DBEngine.SetField(sql, Field4, "run_status", EX_STTP_DATA_TYPE_INT);

		Field Field41;
		pApp->m_DBEngine.SetField(sql, Field41, "voltage", EX_STTP_DATA_TYPE_INT);
		
		Field Field5;
		pApp->m_DBEngine.SetField(sql, Field5, "103group", EX_STTP_DATA_TYPE_INT);
		
		Field Field6;
		pApp->m_DBEngine.SetField(sql, Field6, "103item", EX_STTP_DATA_TYPE_INT);

		Field Field7;
		pApp->m_DBEngine.SetField(sql, Field7, "station_id", EX_STTP_DATA_TYPE_STRING);

		Field Field8;
		pApp->m_DBEngine.SetField(sql, Field8, "bay_id", EX_STTP_DATA_TYPE_STRING);

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_LINE_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildLineList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildLineList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CLineObj * pObj = new CLineObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_LINE, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildLineList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildPmsList()
 功能概要：读取数据库中保护管理机数据, 生成保护管理机对象,建立保护管理机链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90139
BOOL CDataEngine::BuildPmsList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PMS_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildPmsList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildPmsList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CPmsObj * pObj = new CPmsObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_PMS, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildPmsList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildReactorList()
 功能概要：读取数据库中电抗器数据, 生成电抗器对象, 建立电抗器链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90119
BOOL CDataEngine::BuildReactorList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_REACTOR_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildReactorList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildReactorList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CReactorObj * pObj = new CReactorObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_REACTOR, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildReactorList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildSwitvchList()
 功能概要：读取数据库中刀闸数据, 生成刀闸对象, 建立刀闸链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90138
BOOL CDataEngine::BuildSwitchList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}
	
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SWITCH_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildSwitchList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildSwitchList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CSwitchObj * pObj = new CSwitchObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_SWITCH, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildSwitchList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：BuildTransList()
 功能概要：读取数据库中变压器数据, 生成变压器对象, 建立变压器链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90128
BOOL CDataEngine::BuildTransList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		if( !pApp->MgrAllStations())
		{
			Condition con2;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con2, str);
		}

		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TRANS_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildTransList, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildTransList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CTransObj * pObj = new CTransObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,找到对应厂站,加入到链表
					CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
					if(pStation != NULL)
					{
						if(!pStation->AddDevice(TYPE_TRANS, pObj))
						{
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//对应厂站为空
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildTransList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildSecList()
 功能概要：读取数据库中二次设备数据, 生成二次设备对象, 建立保护链表,录波器链表,及其它二次设备链表
 返 回 值: 建立成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90148
BOOL CDataEngine::BuildSecList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

// 		Field Field1;
// 		pApp->m_DBEngine.SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field2;
// 		pApp->m_DBEngine.SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field3;
// 		pApp->m_DBEngine.SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field4;
// 		pApp->m_DBEngine.SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field41;
// 		pApp->m_DBEngine.SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field5;
// 		pApp->m_DBEngine.SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
// 		
// 		Field Field6;
// 		pApp->m_DBEngine.SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field7;
// 		pApp->m_DBEngine.SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field8;
// 		pApp->m_DBEngine.SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);
// 
// 		Field Field9;
// 		pApp->m_DBEngine.SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field10;
// 		pApp->m_DBEngine.SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field11;
// 		pApp->m_DBEngine.SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field12;
// 		pApp->m_DBEngine.SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field13;
// 		pApp->m_DBEngine.SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field14;
// 		pApp->m_DBEngine.SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);
// 
// 		Field Field15;
// 		pApp->m_DBEngine.SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field16;
// 		pApp->m_DBEngine.SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field17;
// 		pApp->m_DBEngine.SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field18;
// 		pApp->m_DBEngine.SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field19;
// 		pApp->m_DBEngine.SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field20;
// 		pApp->m_DBEngine.SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field21;
// 		pApp->m_DBEngine.SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field22;
// 		pApp->m_DBEngine.SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);
// 
// 		Field Field23;
// 		pApp->m_DBEngine.SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);
// 
// 		Field Field24;
// 		pApp->m_DBEngine.SetField(sql, Field24, "ip_a", EX_STTP_DATA_TYPE_STRING);
// 		
// 		Field Field25;
// 		pApp->m_DBEngine.SetField(sql, Field25, "port_a", EX_STTP_DATA_TYPE_INT);
// 		
// 		Field Field26;
// 		pApp->m_DBEngine.SetField(sql, Field26, "ip_b", EX_STTP_DATA_TYPE_STRING);
// 		
// 		Field Field27;
// 		pApp->m_DBEngine.SetField(sql, Field27, "port_b", EX_STTP_DATA_TYPE_INT);
// 
// 		Field Field28;
// 		pApp->m_DBEngine.SetField(sql, Field28, "orderstrategy", EX_STTP_DATA_TYPE_INT);
// 		
// 		Field Field29;
// 		pApp->m_DBEngine.SetField(sql, Field29, "suspendstatus", EX_STTP_DATA_TYPE_INT);
// 		
// 		Field Field30;
// 		pApp->m_DBEngine.SetField(sql, Field30, "suspendreason", EX_STTP_DATA_TYPE_STRING);


		//条件
		//设备类型
		Condition condition1;
		bzero(&condition1, sizeof(Condition));
		CString str;
		str.Format("type > 0");
		if(g_SecType >= 0)
			str.Format("type = %d", g_SecType);
		strncpy(condition1.ConditionContent, str, str.GetLength());
		sql.Conditionlist.push_back(condition1);

		if( !pApp->MgrAllStations())
		{
			Condition con3;
			CString str;
			str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
			pApp->m_DBEngine.SetCondition( sql, con3, str);
		}

		Condition con2;
		pApp->m_DBEngine.SetCondition(sql, con2, "order by name asc", EX_STTP_WHERE_ABNORMALITY);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SECDEV_CFG, sql, sError, m_pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildSecList, 查询失败");
			delete[] sError;
			delete m_pMemset;
			return FALSE;
		}
		if(m_pMemset != NULL && nResult == 1)
		{
			//m_pThread = AfxBeginThread(BuildSecListThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
			//m_pThread->ResumeThread();
			
			CMemSet* pMemset = m_pMemset;
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildSecList, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CSecObj * pObj = new CSecObj;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,加入到链表
					if(pObj->m_nType == TYPE_PROTECT)
					{
						//读取数据成功,找到对应厂站,加入到链表
						CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
						if(pStation != NULL)
						{
							pObj->m_pStation = pStation;
							if(!pStation->AddDevice(TYPE_PROTECT, pObj))
							{
								delete pObj;
								pObj = NULL;
							}
						}
						else
						{
							//对应厂站为空
							delete pObj;
							pObj = NULL;
						}
					}
					else if(pObj->m_nType == TYPE_WAVEREC)
					{
						//读取数据成功,找到对应厂站,加入到链表
						CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
						if(pStation != NULL)
						{
							pObj->m_pStation = pStation;
							if(!pStation->AddDevice(TYPE_WAVEREC, pObj))
							{
								delete pObj;
								pObj = NULL;
							}
						}
						else
						{
							//对应厂站为空
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//读取数据成功,找到对应厂站,加入到链表
						CStationObj * pStation = (CStationObj*)FindDevice(pObj->m_sStationID, TYPE_STATION);
						if(pStation != NULL)
						{
							pObj->m_pStation = pStation;
							if(!pStation->AddDevice(TYPE_SEC, pObj))
							{
								delete pObj;
								pObj = NULL;
							}
						}
						else
						{
							//对应厂站为空
							delete pObj;
							pObj = NULL;
						}
						if(pObj != NULL && pObj->m_nSecType == 21 )
						{
							//加入虚拟测距站
							CString vsID;
							vsID.Format("%s%s", pObj->m_pStation->m_sNetID, virtualStationID);
							
							CDeviceObj* pVirtualStation = FindDevice(vsID, TYPE_STATION);
							if( !pVirtualStation )
							{
								CString vsName;
								vsName.Format("%s%s", pObj->m_pStation->m_sNetID, virtualStationName);
								AddVirtualStation(vsID, vsName, pObj->m_pStation->m_sNetID);
							}
						}
					}
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildSecList,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete m_pMemset;
		sError = NULL;
		m_pMemset = NULL;
	
	return bReturn;
}


/*************************************************************
 函 数 名：LoadData()
 功能概要：调用各设备链表Build方法
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA9015B
BOOL CDataEngine::LoadData()
{
	LONGLONG startTime = StartCount();

	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_STATIONREASON) );
	if(!BuildStationStatusConfig())
	{
		WriteLog("载入厂站通断原因数据出错");
	}	

	//如果是子站角色无需加载地区列表
	if (g_role != MODE_SUB)
	{
		//先载入地区, 厂站, 再载入一次设备, 后载入二次设备, 最后载入保护管理机
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_AREA) );
		if(!BuildNetList())
		{
			WriteLog("载入地区数据出错!");
		}
	}
	EndCount("载入地区数据时间", startTime);
	
	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_STATION) );

	startTime = StartCount();
	LoadServiceDepartmentFromDB();
	EndCount("载入检修单位数据时间", startTime);

	startTime = StartCount();
	if(!BuildStationList())
	{
		WriteLog("载入厂站数据出错!");
	}
	EndCount("载入厂站数据时间", startTime);

	if(g_role == MODE_SEP)
	{
		//载入分站信息
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SEPSTATION));
		if(!BuildSepStationList())
			WriteLog("载入分站站数据出错!");
	}

	if(g_LoadDataModel == 0)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BAY));
		startTime = StartCount();
		if(!BuildBayList())
		{
			WriteLog("载入间隔数据出错!");
		}
		EndCount("载入间隔数据时间", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_MANAGER));
		startTime = StartCount();
		if(!LoadManagedepartmentfromDB())
		{
			WriteLog("载入厂站调管单位出错!");
		}
		EndCount("载入间隔数据时间", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_MAINTENANCE));
		startTime = StartCount();
		if(!LoadMaintaindepartmentfromDB())
		{
			WriteLog("载入厂站运维单位出错!");
		}
		EndCount("载入间隔数据时间", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SUSPENDREASON));
		startTime = StartCount();
		if(!LoadMarkInfofromDB())
		{
			WriteLog("载入挂牌原因信息出错!");
		}
		EndCount("载入挂牌原因数据时间", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BREAKER));
		startTime = StartCount();
		if(!BuildBreakList())
		{
			WriteLog("载入开关数据出错!");
		}
		EndCount("载入开关数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BUS));
		startTime = StartCount();
		if(!BuildBusList())
		{
			WriteLog("载入母线数据出错!");
		}
		EndCount("载入母线数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_CAP));
		startTime = StartCount();
		if(!BuildCapList())
		{
			WriteLog("载入电容器数据出错!");
		}
		EndCount("载入电容器数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_GEN));
		startTime = StartCount();
		if(!BuildGenList())
		{
			WriteLog("载入发电机数据出错!");
		}
		EndCount("载入发电机数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_LINE));
		startTime = StartCount();
		if(!BuildLineList())
		{
			WriteLog("载入路线数据出错!");
		}
		EndCount("载入路线数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_REACTOR));
		startTime = StartCount();
		if(!BuildReactorList())
		{
			WriteLog("载入电抗器数据出错!");
		}
		EndCount("载入电抗器数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SWITCH));
		startTime = StartCount();
		if(!BuildSwitchList())
		{
			WriteLog("载入刀闸数据出错!");
		}
		EndCount("载入刀闸数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_TRANS));
		startTime = StartCount();
		if(!BuildTransList())
		{
			WriteLog("载入变压器数据出错!");
		}
		EndCount("载入变压器数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_PMS));
		startTime = StartCount();
		if(!BuildPmsList())
		{
			WriteLog("载入保护管理机数据出错!");
		}
		EndCount("载入保护管理机数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SEC));
		startTime = StartCount();
		if(!BuildSecList())
		{
			WriteLog("载入二次设备数据出错!");
		}
		EndCount("载入二次设备数据时间", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_CPU));
		startTime = StartCount();
		if(!BuildCpuList())
		{
			WriteLog("载入CPU数据出错!");
		}
		EndCount("载入CPU数据时间", startTime);
		
		startTime = StartCount();
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_ZONE));
		if(!BuildZoneList())
		{
			WriteLog("载入定值区号出错!");
		}
		
		EndCount("载入定值区号数据时间", startTime);
	}

	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_UNICHR));
	startTime = StartCount();
	if(!BuildUniversalActChr())
	{
		WriteLog("载入通用特征量数据出错");
	}
	EndCount("载入通用特征量数据时间", startTime);

	GetFuncTimeOut();
	WriteLog("读入功能通信超时设置完毕");

	LoadSituationAndDisposeFromDB();
	WriteLog("读入巡检情况说明和处理意见配置完毕");

/*	startTime = StartCount();
	QueryHasSettingConfig();
	WriteLog("查询各二次设备定值配置数完毕");
	EndCount("查询各二次设备定值配置数时间", startTime);

	startTime = StartCount();
	QueryHasDigitalConfig();
	WriteLog("查询各二次设备开关量配置数完毕");
	EndCount("查询各二次设备开关量配置数时间", startTime);

	startTime = StartCount();
	QueryHasSoftBoardConfig();
	WriteLog("查询各二次设备软压板配置数完毕");
	EndCount("查询各二次设备软压板配置数时间", startTime);

	startTime = StartCount();
	QueryHasAnalogConfig();
	WriteLog("查询各二次设备模拟量配置数完毕");
	EndCount("查询各二次设备模拟量配置数时间", startTime);*/

	if( g_PTShowNote )
	{
		startTime = StartCount();
		QueryPTNoteCount();
		WriteLog("查询各二次设备备注数完毕");
		EndCount("查询各二次设备备注数时间", startTime);
	}

	if (g_role != MODE_SUB)
	{
		Load20162Config();
	}

	return TRUE;
}

/*************************************************************
 函 数 名：FindDevice()
 功能概要：查找设备
 返 回 值: 查找成功返回设备指针, 失败返回NULL
 参    数：param1   设备ID
           Param2   设备类型
**************************************************************/
//##ModelId=49B87BA90167
CDeviceObj* CDataEngine::FindDevice(CString DeviceID, int DeviceType, CString StationID /*= ""*/)
{
	//根据设备类型到不同的设备链表中查找
	//去空格
// 	DeviceID.TrimLeft();
// 	DeviceID.TrimRight();
	CDeviceObj * pDevice = NULL;
	CDeviceObj * pObj = NULL;
	POSITION pos;
	CString sID;
	switch(DeviceType)
	{
	case TYPE_NET: //地区
		for(pos = m_NetList.GetHeadPosition(); pos != NULL;)
		{
			pObj = (CDeviceObj*)m_NetList.GetNext(pos);
			if(pObj->m_sID == DeviceID)
			{
				pDevice = pObj;
				break;
			}
		}
		break;
	case TYPE_STATION: //厂站
		m_StationMap.Lookup(DeviceID, (CObject*&)pDevice);
		break;
	default:
		if(StationID == "")
		{
			//其它设备,遍历所有厂站下的此类设备链表
			for(pos = m_StationMap.GetStartPosition(); pos != NULL;)
			{
				CStationObj* pStation = NULL;
				m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
				if(pStation != NULL)
				{
					pObj = NULL;
					pObj = pStation->FindDevice(DeviceID, DeviceType);
					if(pObj != NULL)
					{
						pDevice = pObj;
						break;
					}
				}
			}
		}
		else
		{
			//查找厂站
			CStationObj* pStation = (CStationObj*)FindDevice(StationID, TYPE_STATION);
			if(pStation != NULL)
			{
				pObj = NULL;
				pObj = pStation->FindDevice(DeviceID, DeviceType);
				if(pObj != NULL)
				{
					pDevice = pObj;
					break;
				}
			}
		}
		break;
	}
	
	return pDevice;
}


/*************************************************************
 函 数 名：BuildProtectDeviceRelation()
 功能概要：建立厂站通断原因表
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CDataEngine::BuildStationStatusConfig()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CONFIG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildStationStatusConfig, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildStationStatusConfig, 读取到%d条数据", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//创建对象
				CString strId = pMemset->GetValue((UINT)0);
				int id = atoi(strId);
				CString strReason = pMemset->GetValue((UINT)1);
				m_StationStatusConfigMap.SetAt(id, strReason);
				
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildStationStatusConfig,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：Clear()
 功能概要：清除数据
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA90158
void CDataEngine::Clear()
{
	POSITION pos = NULL;
	CDeviceObj * pObj = NULL;
	CString sID;

	//分站
	ClearSepStationList();

	//厂站
	for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
	{
		m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_StationMap.RemoveAll();

	//地区
	ClearNetList();

	for (pos = m_arrMark.GetHeadPosition();pos != NULL;)
	{
		pObj = (CMark*)m_arrMark.GetNext(pos);
		if(pObj != NULL)
		{
			delete pObj;
			pObj = NULL;
		}
	}
	m_arrMark.RemoveAll();

	//清除通用特征值
	for(int i = 0; i < m_arrUniversalActChr.GetSize(); i++)
	{
		delete m_arrUniversalActChr[i];
	}
	m_arrUniversalActChr.RemoveAll();


	for(pos = m_mapServiceDepartment.GetStartPosition(); pos != NULL; )
	{
		CDepartment * pDep = NULL;
		WORD nID = 0;
		m_mapServiceDepartment.GetNextAssoc(pos, (WORD)nID, (CObject*&)pDep);
		if(pDep != NULL)
			delete pDep;
	}
	m_mapServiceDepartment.RemoveAll();

	//清楚厂站通断原因表
	m_StationStatusConfigMap.RemoveAll();
}

/*************************************************************
 函 数 名：BuildUniversalActChr()
 功能概要：创建通用的动作特征量
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA90159
BOOL CDataEngine::BuildUniversalActChr()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//与数据平台访问DLL的连接可用
		
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		CString str;
		
		//字段
		//cha_id
		Field Field2;
		bzero(&Field2, sizeof(Field));
		str = "cha_id";
		strncpy(Field2.FieldName, str, str.GetLength());
		Field2.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field2);

		//name
		Field Field3;
		bzero(&Field3, sizeof(Field));
		str = "Name";
		strncpy(Field3.FieldName, str, str.GetLength());
		Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
		sql.Fieldlist.push_back(Field3);

		//code_name
		Field Field4;
		bzero(&Field4, sizeof(Field));
		str = "code_name";
		strncpy(Field4.FieldName, str, str.GetLength());
		Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
		sql.Fieldlist.push_back(Field4);

		//Property
		Field Field5;
		bzero(&Field5, sizeof(Field));
		str = "Property";
		strncpy(Field5.FieldName, str, str.GetLength());
		Field5.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field5);

		//Notes(单位)
		Field Field6;
		bzero(&Field6, sizeof(Field));
		str = "Notes";
		strncpy(Field6.FieldName, str, str.GetLength());
		Field6.FieldType = EX_STTP_DATA_TYPE_STRING;
		sql.Fieldlist.push_back(Field6);
		
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
			int nResult;
			try
			{
				nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CHARCTOR_CFG, sql, sError, pMemset);
			}
			catch (...)
			{
				WriteLog("CCDataEngine::BuildUniversalActChr, 查询失败");
				delete[] sError;
				delete pMemset;
				return FALSE;
			}
			if(pMemset != NULL && nResult == 1)
			{
				pMemset->MoveFirst();
				int nCount = pMemset->GetMemRowNum();
				if(nCount > 0)
				{
					CString str;
					str.Format("CDataEngine::BuildUniversalActChr, 读取到%d条数据", nCount);
					WriteLog(str, 3);
				}
				if(nCount == 0)
				{
					CString str;
					str.Format("没有通用动作特征量信息");
					WriteLog(str);
				}
				for(int i = 0; i < nCount; i++)
				{
					//创建对象
					PT_ActionCharacter * sign = new PT_ActionCharacter;
					sign->strUnits = "";
					CString str;
					str = pMemset->GetValue((UINT)0); //action_id
					sign->nID = atoi(str);
					sign->strName = pMemset->GetValue(1); //name
					sign->strCodeName = pMemset->GetValue(2); //code_name
					str = pMemset->GetValue(3); //事件类型
					sign->nEventType = atoi(str);
					sign->strUnits = pMemset->GetValue(4); //单位

					//单位去空格
					sign->strUnits.TrimLeft();
					sign->strUnits.TrimRight();

					m_arrUniversalActChr.Add(sign);

					pMemset->MoveNext();
				}
			}
			else
			{	CString str;
				str.Format("CDataEngine::BuildUniversalActChr,查询失败,原因为%s", sError);
				WriteLog(str);
				bReturn = FALSE;
			}
		
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：FindDeviceBy103()
 功能概要：根据103组号条目号查找设备
 返 回 值: 设备指针
 参    数：param1	103组号条目号.格式:组号#条目号
		   Param2	设备类型
		   Param3	厂站ID
**************************************************************/
//##ModelId=49B87BA9016A
CDeviceObj * CDataEngine::FindDeviceBy103( CString str103, int DeviceType, CString strStaionID )
{
	//一定要指定厂站ID, 因为有可能不同厂站有相同的组号条目号
	if(strStaionID == "")
		return NULL;
	//查找厂站
	CStationObj* pStation = (CStationObj*)FindDevice(strStaionID, TYPE_STATION);
	if(pStation != NULL)
	{
		return pStation->FindDeviceBy103(str103, DeviceType);
	}
	
	return NULL;
}


/*
 *  @brief   	FindDeviceBy103	 根据uri查找设备 
 *  @param 		[In]a param of Type  CString uri
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @return 	CDeviceObj * 设备指针
 */
	//##ModelId=49B87BA6035B
CDeviceObj * CDataEngine::FindDeviceByUri(CString strUri, CString strStaionID)
{
	//一定要指定厂站ID, 因为有可能不同厂站有相同的组号条目号
	if(strStaionID == "")
		return NULL;
	//查找厂站
	CStationObj* pStation = (CStationObj*)FindDevice(strStaionID, TYPE_STATION);
	if(pStation != NULL)
		return pStation->FindDeviceByUri(strUri);
	
	return NULL;
}

/*************************************************************
 函 数 名：GetFuncTimeOut()
 功能概要：从数据库中读取各通信功能超时时间
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA9015A
BOOL CDataEngine::GetFuncTimeOut()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FUN_TIMEOUT_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::GetFuncTimeOut, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount ==0)
		{
			CString str;
			str.Format("CDataEngine::GetFuncTimeOut, 没有超时数据");
			WriteLog(str);
		}
		//记录数据
		if(nCount > 0)
		{
			int nOffset = 10;
			CString str;
			str = pMemset->GetValue((UINT)1);
			m_nPTSettingTimeOut = atoi(str) + nOffset; //保护召唤定值
			str = pMemset->GetValue(2);
			m_nWRSettingTimeOut = atoi(str) + nOffset; //录波器召唤定值
			str = pMemset->GetValue(5);
			m_nPTAiTimeOut = atoi(str) + nOffset; //召唤保护模拟量
			str = pMemset->GetValue(6);
			m_nPTDiTimeOut = atoi(str) + nOffset; //召唤保护开关量
			str = pMemset->GetValue(7);
			m_nPTSoftBoardTimeOut = atoi(str) + nOffset; //召唤保护软压板
			str = pMemset->GetValue(8);
			m_nPTZoneTimeOut = atoi(str) + nOffset; //召唤保护定值区号
			str = pMemset->GetValue(9);
			m_nDevTimeTimeOut = atoi(str) + nOffset; //召唤装置时间
			str = pMemset->GetValue(10);
			m_nDevGeneralTimeOut = atoi(str) + nOffset; //召唤装置基本信息
			str = pMemset->GetValue(11);
			m_nModifySettingTimeOut = atoi(str) + nOffset; //定值修改
			str = pMemset->GetValue(12);
			m_nChangeZoneTimeOut = atoi(str) + nOffset; //定值区切换
			str = pMemset->GetValue(13);
			m_nChangeSBTimeOut = atoi(str) + nOffset; //软压板切换
			str = pMemset->GetValue(14);
			m_nVerifyTimeTimeOut = atoi(str) + nOffset; //装置对时
			str = pMemset->GetValue(15);
			m_nSignalResetTimeOut = atoi(str) + nOffset; //信号复归
			str = pMemset->GetValue(16);
			m_nRemoteStartTimeOut = atoi(str) + nOffset; //远方启动
			str= pMemset->GetValue(17);
			m_nHistoryEventTimeOut = atoi(str) + nOffset; //历史事件
			str = pMemset->GetValue(18);
			m_nWavFileTimeOut = atoi(str) + nOffset; //下载录波文件
			str = pMemset->GetValue(19);
			m_nWavFileListTimeOut = atoi(str) + nOffset;//召唤录波列表
			str = pMemset->GetValue(20);
			m_nCommFileTimeOut = atoi(str) + nOffset; //召唤通用文件
			str = pMemset->GetValue(21);
			m_nCommFileListTimeOut = atoi(str) + nOffset; //召唤通用文件列表
			str = pMemset->GetValue(27);
			m_nCommStatusTimeOut = atoi(str) + nOffset; //召唤装置运行状态

		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::GetFuncTimeOut, 查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildCpuList()
 功能概要：建立CPU链表
 返 回 值: 建立成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA90149
BOOL CDataEngine::BuildCpuList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	Field field1;
	pApp->m_DBEngine.SetField(sql, field1, "pt_id", EX_STTP_DATA_TYPE_STRING);

	Field field2;
	pApp->m_DBEngine.SetField(sql, field2, "cpu_code", EX_STTP_DATA_TYPE_INT);

	Field field3;
	pApp->m_DBEngine.SetField(sql, field3, "cpu_des", EX_STTP_DATA_TYPE_STRING);

	Field field31;
	pApp->m_DBEngine.SetField(sql, field31, "cpu_ver", EX_STTP_DATA_TYPE_STRING);

	Field field4;
	pApp->m_DBEngine.SetField(sql, field4, "from_zone", EX_STTP_DATA_TYPE_INT);

	Field field5;
	pApp->m_DBEngine.SetField(sql, field5, "to_zone", EX_STTP_DATA_TYPE_INT);

	if( !pApp->MgrAllStations())
	{
		Condition con2;
		CString str;
		str.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition( sql, con2, str);
	}

	Condition con1;
	pApp->m_DBEngine.SetCondition(sql, con1, "order by pt_id, cpu_code asc", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_CPU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::BuildCpuList, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}

	if(pMemset != NULL && 1 == nResult)
	{
	//	MYASSERT_BOOL(m_pThread);
	//	WaitForSingleObject(m_pThread, INFINITE);
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::BuildCpuList, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			CSecCPU * cpu = new CSecCPU;
			if(cpu->Load(pMemset))
			{
				//读取成功
				CSecObj* pSec = (CSecObj*)FindDevice(cpu->PT_ID, TYPE_SEC);
				if(pSec == NULL)
				{
					//查找不到二次设备
					delete cpu;
					cpu = NULL;
				}
				else
				{
					pSec->m_arrCPU.Add(cpu);
				}
			}
			else
			{
				delete cpu;
				cpu = NULL;
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::BuildCpuList,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：BuildZoneList()
 功能概要：建立定值区号链表
 返 回 值: 建立成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA9014A
BOOL CDataEngine::BuildZoneList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;

	if( !pApp->MgrAllStations())
	{
		Condition con2;
		CString str;
		str.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition( sql, con2, str);
	}

	Condition con1;
	pApp->m_DBEngine.SetCondition(sql, con1, "order by pt_id, cpu_code, zone_id asc", EX_STTP_WHERE_ABNORMALITY);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::BuildZoneList, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CDataEngine::BuildZoneList, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			/*
			CString  PT_ID;//保护ID
			int		cpu; //cpu号
			int		code; //定值区号
			CString zone_name;
			CString code_name;
			int     group;
			int     item;
			int     minValue;
			int     maxValue;
			int     stepValue;
			int     zoneValue;
			*/


			//创建
			PT_ZONE * ptz = new PT_ZONE;
			CString str;
			ptz->PT_ID = pMemset->GetValue((UINT)0); //PT_ID
			str = pMemset->GetValue(1); //CPU_Code
			ptz->cpu = atoi(str);
			str = pMemset->GetValue(2);//Zone_CODE
			ptz->code = atoi(str);
			ptz->zone_name = pMemset->GetValue(3);
			ptz->code_name = pMemset->GetValue(4);
			str = pMemset->GetValue(5);
			ptz->group = atoi(str);
			str = pMemset->GetValue(6);
			ptz->item = atoi(str);
			str = pMemset->GetValue(7);
			ptz->minValue = atoi(str);
			str = pMemset->GetValue(8);
			ptz->maxValue = atoi(str);
			str = pMemset->GetValue(9);
			ptz->stepValue = atoi(str);
			str = pMemset->GetValue(12);
			ptz->zoneValue = atoi(str);

			/*
			str = pMemset->GetValue(3);//Zone_State
			ptz->state = atoi(str);
			*/

			//查找二次设备
			CSecObj* pSec = (CSecObj*)FindDevice(ptz->PT_ID, TYPE_SEC);
			if(pSec == NULL)
			{
				delete ptz;
				ptz = NULL;
			}
			else
			{
				if(!pSec->AddZone(ptz))
				{
					delete ptz;
					ptz = NULL;
				}
				if (ptz != NULL && ptz->zone_name.Find("编辑") != -1)
				{
					pSec->m_bNewDevice = true;
				}
			}
						
			pMemset->MoveNext();
		}
	}
	else
	{
		
		CString str;
		str.Format("CSecCPU::GetZone,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

void CDataEngine::ClearRefreshSec()
{
	POSITION pos = m_AllSEC.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pSec = (CSecObj*)m_AllSEC.GetNext(pos);
		if(pSec == NULL)
			continue;
		delete pSec;
	}
	m_AllSEC.RemoveAll();
}

/*************************************************************
 函 数 名: LoadAllSec()
 功能概要: 载入所有二次设备,用于刷新设备状态
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDataEngine::LoadAllSec()
{
	//clear
	ClearRefreshSec();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

// 	Field Field1;
// 	pApp->m_DBEngine.SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field2;
// 	pApp->m_DBEngine.SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field3;
// 	pApp->m_DBEngine.SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field4;
// 	pApp->m_DBEngine.SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);
// 
// 	Field Field41;
// 	pApp->m_DBEngine.SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field5;
// 	pApp->m_DBEngine.SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field6;
// 	pApp->m_DBEngine.SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field7;
// 	pApp->m_DBEngine.SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field8;
// 	pApp->m_DBEngine.SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);
// 	
// 	Field Field9;
// 	pApp->m_DBEngine.SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field10;
// 	pApp->m_DBEngine.SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field11;
// 	pApp->m_DBEngine.SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field12;
// 	pApp->m_DBEngine.SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field13;
// 	pApp->m_DBEngine.SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field14;
// 	pApp->m_DBEngine.SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field15;
// 	pApp->m_DBEngine.SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field16;
// 	pApp->m_DBEngine.SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field17;
// 	pApp->m_DBEngine.SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field18;
// 	pApp->m_DBEngine.SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field19;
// 	pApp->m_DBEngine.SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field20;
// 	pApp->m_DBEngine.SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field21;
// 	pApp->m_DBEngine.SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field22;
// 	pApp->m_DBEngine.SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);
// 	
// 	Field Field23;
// 	pApp->m_DBEngine.SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);
// 
// 	Field Field28;
// 	pApp->m_DBEngine.SetField(sql, Field28, "orderstrategy", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field29;
// 	pApp->m_DBEngine.SetField(sql, Field29, "suspendstatus", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field30;
// 		pApp->m_DBEngine.SetField(sql, Field30, "suspendreason", EX_STTP_DATA_TYPE_STRING);

	if( !pApp->MgrAllStations())
	{
		Condition con3;
		CString str;
		str.Format("station_id in(select station_id from tb_user_related where user_id='%s')", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition( sql, con3, str);
	}
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SECDEV_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadAllSec, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::LoadAllSec, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CSecObj * pObj = new CSecObj;
			if(pObj->Load(pMemset))
			{
				//读取数据成功,加入到链表
				m_AllSEC.AddTail(pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::LoadAllSec,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 函 数 名: RefreshDevStatus()
 功能概要: 刷新设备状态
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDataEngine::RefreshDevStatus()
{
	POSITION pos = m_AllSEC.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pSec = (CSecObj*)m_AllSEC.GetNext(pos);
		if(pSec == NULL)
			continue;
		//查找在程序内部对应的设备
		CStationObj* pStation = (CStationObj*)FindDevice(pSec->m_sStationID, TYPE_STATION);
		if(pStation == NULL)
			continue;
		CSecObj* pObj = (CSecObj*)pStation->FindDevice(pSec->m_sID, pSec->m_nType);
		if(pObj == NULL)
			continue;
		//刷新设备状态
		pObj->RefreshStatus(pSec);
	}
}

/*************************************************************
 函 数 名: QueryStationStatus()
 功能概要: 查询厂站状态
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDataEngine::QueryStationStatus()
{
	//组建查询语句
	CString strSQL;
	//------------------------
	strSQL.Format("select * from tb_pt_station_status A JOIN (select station_id,Reserve1,max(Time) Time from tb_pt_station_status  group by station_id,Reserve1) B ON A.station_id = B.station_id and A.Time = B.Time");
//	strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryStationStatus, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryStationStatus, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strStationID = pMemset->GetValue(1);
			CString strStatus = pMemset->GetValue(2);
			CString strTime = pMemset->GetValue(3);
			CString strReason = pMemset->GetValue(4);
			CString strChannel = pMemset->GetValue(5);
			CString strReasonTime = pMemset->GetValue(6);
			
			//查找对应的厂站
			CStationObj* pObj = (CStationObj*)FindDevice(strStationID, TYPE_STATION);
			if(pObj != NULL)
			{
				pObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pObj->m_nIPFlag);
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryStationStatus,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 CreateDate: 19:1:2010     Author:LYH
 函 数 名: QueryHasSettingConfig()
 功能概要: 查询各二次设备是否有定值配置
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasSettingConfig()
{
	//组建查询语句
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_setting_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryHasSettingConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryHasSettingConfig, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strStation = pMemset->GetValue((UINT)0);
			CString strPTID = pMemset->GetValue(1);
			CString strCPU = pMemset->GetValue(2);
			CString strCount = pMemset->GetValue(3);
			int nDefsCount = atoi(strCount);
			{
				//查找对应的二次设备
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//查找CPU
					CSecCPU* pCpu = pObj->FindCpu(atoi(strCPU));
					if(pCpu != NULL)
						pCpu->m_nSettingDefs = nDefsCount;
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryHasSettingConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return TRUE;
}

/*************************************************************
 CreateDate: 19:1:2010     Author:LYH
 函 数 名: QueryHasDigitalConfig()
 功能概要: 查询各二次设备是否有开关量配置
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasDigitalConfig()
{
	//组建查询语句
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_di_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryHasDigitalConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryHasDigitalConfig, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strStation = pMemset->GetValue((UINT)0);
			CString strPTID = pMemset->GetValue(1);
			CString strCPU = pMemset->GetValue(2);
			CString strCount = pMemset->GetValue(3);
			int nDefsCount = atoi(strCount);
			{
				//查找对应的二次设备
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//查找CPU
					CSecCPU* pCpu = pObj->FindCpu(atoi(strCPU));
					if(pCpu != NULL)
						pCpu->m_nDigitalDefs = nDefsCount;
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryHasDigitalConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

/*************************************************************
 CreateDate: 19:1:2010     Author:LYH
 函 数 名: QueryHasSoftBoardConfig()
 功能概要: 查询各二次设备是否有软压板配置
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasSoftBoardConfig()
{
	//组建查询语句
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_softboard_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryHasSoftBoardConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryHasSoftBoardConfig, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strStation = pMemset->GetValue((UINT)0);
			CString strPTID = pMemset->GetValue(1);
			CString strCPU = pMemset->GetValue(2);
			CString strCount = pMemset->GetValue(3);
			int nDefsCount = atoi(strCount);
			{
				//查找对应的二次设备
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//查找CPU
					CSecCPU* pCpu = pObj->FindCpu(atoi(strCPU));
					if(pCpu != NULL)
						pCpu->m_nSoftBoardDefs = nDefsCount;
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryHasSoftBoardConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

/*************************************************************
 CreateDate: 19:1:2010     Author:LYH
 函 数 名: QueryHasAnalogConfig()
 功能概要: 查询各二次设备是否有模拟量配置
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasAnalogConfig()
{
	//组建查询语句
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_ai_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryHasAnalogConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryHasAnalogConfig, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strStation = pMemset->GetValue((UINT)0);
			CString strPTID = pMemset->GetValue(1);
			CString strCPU = pMemset->GetValue(2);
			CString strCount = pMemset->GetValue(3);
			int nDefsCount = atoi(strCount);
			{
				//查找对应的二次设备
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//查找CPU
					CSecCPU* pCpu = pObj->FindCpu(atoi(strCPU));
					if(pCpu != NULL)
						pCpu->m_nAnalogDefs = nDefsCount;
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryHasAnalogConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}


void CDataEngine::Load20162Config()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "reverse2", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTENCONFIG_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::Load20162Config, 查询失败");
		delete[] sError;
		delete m_pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			g_bSend20162 = atoi(pMemset->GetValue((UINT)0));
		}
	}
				
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

}

/************************************
Date:2011/6/17  Author:LYH
函数名:    BuildZoneListOld
返回值:   BOOL
功能概要:  建立定值区号链表,从tb_secdev_zone_base读取数据
************************************/
BOOL CDataEngine::BuildZoneListOld()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	Condition con1;
	pApp->m_DBEngine.SetCondition(sql, con1, "order by pt_id, cpu_code, zone_code asc", EX_STTP_WHERE_ABNORMALITY);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ZONE_BASE_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::BuildZoneList, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CDataEngine::BuildZoneList, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			PT_ZONE * ptz = new PT_ZONE;
			CString str;
			ptz->PT_ID = pMemset->GetValue((UINT)0); //PT_ID
			str = pMemset->GetValue(1); //CPU_Code
			ptz->cpu = atoi(str);
			str = pMemset->GetValue(2);//Zone_CODE
			ptz->code = atoi(str);
			str = pMemset->GetValue(3);//Zone_State
			ptz->state = atoi(str);
			
			//查找二次设备
			CSecObj* pSec = (CSecObj*)FindDevice(ptz->PT_ID, TYPE_SEC);
			if(pSec == NULL)
			{
				delete ptz;
				ptz = NULL;
			}
			else
			{
				if(!pSec->AddZone(ptz))
				{
					delete ptz;
					ptz = NULL;
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		
		CString str;
		str.Format("CSecCPU::GetZone,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    GetPriDeviceList
返回值:   void
功能概要: 取得一次设备链表 
参数: DEVICE_LIST & listDevice 用于保存一次设备的链表
参数: int nDeviceType 指定要查找的一次设备类型，-1表示所有类型
参数: CString sStationID 指定所属厂站ID，为空时表示所有厂站
************************************/
void CDataEngine::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType, CString sStationID/*=""*/ )
{
	CStationObj* pStation = NULL;
	CString sID;
	POSITION pos = NULL;
	if(sStationID.IsEmpty())
	{
		//遍历所有厂站
		for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
		{
			m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
			if(pStation != NULL)
				pStation->GetPriDeviceList(listDevice, nDeviceType);
		}
	}
	else
	{
		//先找到对应厂站
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetPriDeviceList(listDevice, nDeviceType);
	}
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    GetSecList
返回值:   void
功能概要: 取得二次设备链表
参数: DEVICE_LIST & listDevice 用于保存二次设备的链表
参数: CString sPriID 指定一次设备，为空时表示所有
参数: int nSecType 指定要查找的二次设备类型，-1或TYPE_SEC表示所有类型
参数: CString sStationID 指定所属厂站ID，为空时表示所有厂站
************************************/
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, CString sPriID /*= ""*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, int nDetailKind/* = -1 */ )
{
	CStationObj* pStation = NULL;
	CString sID;
	POSITION pos = NULL;
	if(sStationID.IsEmpty())
	{
		//遍历所有厂站
		for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
		{
			m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
			if(pStation != NULL)
				pStation->GetSecList(listDevice, sPriID, nSecType, nDetailKind);
		}
	}
	else
	{
		//先找到对应厂站
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, sPriID, nSecType, nDetailKind);
	}
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    GetStationList
返回值:   void
功能概要: 取得厂站链表
参数: DEVICE_LIST & listDevice 用于保存厂站的链表
参数: CString sNetID 指定地区ID，为空时表示所有地区
************************************/
void CDataEngine::GetStationList( DEVICE_LIST& listDevice, CString sNetID /*= ""*/ )
{
	CString sID;
	CStationObj* pStation = NULL;
	POSITION pos = NULL;
	for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
	{
		m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
		if(pStation != NULL && (pStation->m_sNetID == sNetID || sNetID.IsEmpty()))
			listDevice.AddTail(pStation);
	}
	
}

/****************************************************
Date:2012/6/15  Author:LYH
函数名:   BuildSepStationList	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CDataEngine::BuildSepStationList()
{
	ClearSepStationList();
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件
	Condition con1;
	CString str;
	str="TYPE=6";
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::BuildSepStationList, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::BuildSepStationList, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CStationObj * pObj = new CStationObj;
			if(pObj->Load(pMemset))
			{
				//读取数据成功,加入到链表
				m_SepStationMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::BuildSepStationList,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/****************************************************
Date:2012/6/15  Author:LYH
函数名:   FindSepByStation	
返回值:   CDeviceObj*	
功能概要: 
参数: CString sStationID	
*****************************************************/
CDeviceObj* CDataEngine::FindSepByStation( CString sStationID )
{
	CStationObj* pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
	if(pStation == NULL)
		return NULL;
	CNetObj* pNet = (CNetObj*)FindDevice(pStation->m_sNetID, TYPE_NET);
	if(pNet == NULL)
		return NULL;
	CStationObj* pObj = NULL;
	m_SepStationMap.Lookup(pNet->m_sLocalID, (CObject*&)pObj);
	return pObj;
}

/****************************************************
Date:2012/6/15  Author:LYH
函数名:   FindSepStation	
返回值:   CDeviceObj*	
功能概要: 
参数: CString sSepID	
*****************************************************/
CDeviceObj* CDataEngine::FindSepStation( CString sSepID )
{
	CStationObj* pObj = NULL;
	m_SepStationMap.Lookup(sSepID, (CObject*&)pObj);
	return pObj;
}

/****************************************************
Date:2012/11/2  Author:LY
		 函数名:   LoadManagedepartmentfromDB	
		 返回值:   BOOL	
		 功能概要: 查询所属调管单位
		 参数: 
*****************************************************/
BOOL CDataEngine::LoadManagedepartmentfromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//设置条件
// 	Condition Condition0;
 	CString str;
// 	str.Format("ID = '%s'", pObj->m_sID);
// 	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	// 	Condition Condition1;
	// 	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC", EX_STTP_WHERE_ABNORMALITY);
	// 	
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TB_RESPONSDEPART_DEF, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadManagedepartmentfromDB, 查询失败");
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
			//循环所有
			
			POSITION pos = listDevice.GetHeadPosition();
			while(pos != NULL)
			{
				CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
				if (pObj->m_nManagedepartment == atoi(pMemset.GetValue((UINT)0)))
				{
					pObj->m_sManagedepartment = pMemset.GetValue(1);
				}
			}
			pMemset.MoveNext();		
		}
		
	}
	else
	{
		str.Format("CDataEngine::LoadManagedepartmentfromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
	//	pMemset = NULL;
	return bReturn;
}
/****************************************************
Date:2012/11/2  Author:LY
		 函数名:   LoadManagedepartmentfromDB	
		 返回值:   BOOL	
		 功能概要: 查询所属调管单位
		 参数: 
*****************************************************/
BOOL CDataEngine::LoadMaintaindepartmentfromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//设置条件
// 	Condition Condition0;
 	CString str;
// 	str.Format("ID = '%s'", pObj->m_sID);
// 	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	// 	Condition Condition1;
	// 	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC", EX_STTP_WHERE_ABNORMALITY);
	// 	
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TB_MAINTAINDEPART_DEF, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadMaintaindepartmentfromDB, 查询失败");
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
			//循环所有
			
			POSITION pos = listDevice.GetHeadPosition();
			while(pos != NULL)
			{
				CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
				if (pObj->m_nMaintaindepartment == atoi(pMemset.GetValue((UINT)0)))
				{
					pObj->m_sMaintaindepartment = pMemset.GetValue(1);
				}
			}
			pMemset.MoveNext();		
		}
		
	}
	else
	{
		str.Format("CDataEngine::LoadMaintaindepartmentfromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
	//	pMemset = NULL;
	return bReturn;
}
/****************************************************

Date:2012/11/2  Author:LY
	 函数名:   LoadMarkInfofromDB	
	 返回值:   BOOL	
	 功能概要: 查询所属调管单位
	 参数: 
	 *****************************************************/
BOOL CDataEngine::LoadMarkInfofromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "note", EX_STTP_DATA_TYPE_STRING);
	
	
	//设置条件
	// 	Condition Condition0;
	CString str;
	// 	str.Format("ID = '%s'", pObj->m_sID);
	// 	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	// 	Condition Condition1;
	// 	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC", EX_STTP_WHERE_ABNORMALITY);
	// 	
	CMemSet * pMemset = NULL;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TB_SUSPEND_REASON_DEF, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadMarkInfofromDB, 查询失败");
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
				//创建对象
				CMark * pObj = new CMark;
				if(pObj->Load(pMemset))
				{
					//读取数据成功,加入到链表
					m_arrMark.AddTail(pObj);
				}
				else
				{
					//读取数据失败, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();	
		}
		
	}
	else
	{
		str.Format("CDataEngine::LoadMarkInfofromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return bReturn;
}

/****************************************************
Date:2012/12/11  Author:LYH
函数名:   LoadServiceDepartmentFromDB	
返回值:   BOOL	
功能概要: 查询检修单位,建立检修单位MAP表
*****************************************************/
BOOL CDataEngine::LoadServiceDepartmentFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet  pMemset;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TB_SERVICEDEPART_DEF, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadServiceDepartmentFromDB, 查询失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CString sID = pMemset.GetValue((UINT)0);
			CString sName = pMemset.GetValue(1);
			CDepartment* pObj = new CDepartment(atoi(sID), sName);
			if(pObj != NULL)
				m_mapServiceDepartment.SetAt(pObj->GetID(), (CObject*)pObj);
			pMemset.MoveNext();	
		}
		
	}
	else
	{
		CString str;
		str.Format("CDataEngine::LoadServiceDepartmentFromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	sError = NULL;
	return bReturn;
}

/****************************************************
Date:2012/12/11  Author:LYH
函数名:   GetServiceDepartmentName	
返回值:   CString	取得检修单位名称
功能概要: 取得检修单位名称
参数: int nID	检修单位ID
*****************************************************/
CString CDataEngine::GetServiceDepartmentName( int nID )
{
	CDepartment* pObj = NULL;
	m_mapServiceDepartment.Lookup(nID, (CObject*&)pObj);
	if(pObj != NULL)
		return pObj->GetName();
	return "";
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   ClearNetList	
返回值:   void	
功能概要: 清空地区链表
*****************************************************/
void CDataEngine::ClearNetList()
{
	POSITION pos;
	for(pos = m_NetList.GetHeadPosition(); pos != NULL;)
	{
		CDeviceObj* pObj = (CDeviceObj*)m_NetList.GetNext(pos);
		if(pObj != NULL)
		{
			delete pObj;
			pObj = NULL;
		}
	}
	m_NetList.RemoveAll();
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   ClearSepStationList	
返回值:   void	
功能概要: 清空分站链表
*****************************************************/
void CDataEngine::ClearSepStationList()
{
	POSITION pos = NULL;
	CDeviceObj* pObj = NULL;
	CString sID = "";
	for(pos = m_SepStationMap.GetStartPosition(); pos != NULL; )
	{
		m_SepStationMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_SepStationMap.RemoveAll();
}


/****************************************************
Date:2013/6/4  Author:LYH
函数名:   StationInit	
返回值:   CStationObj*	厂站对象
功能概要: 厂站初始化. 如果nType==STATIONINIT_DELETE,本函数只将指定厂站移除出链表，并不会删除对象本身，对象本身需调用函数进行删除
参数: CString sStationID 厂站ID	
参数: int nType	初始化类型
*****************************************************/
CStationObj* CDataEngine::StationInit( CString sStationID, int nType )
{
	if (g_role != MODE_SUB)
	{
		BuildNetList();
	}
	if(g_role == MODE_SEP)
		BuildSepStationList();
	CStationObj* pStation = NULL;
	pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
	switch(nType)
	{
	case STATIONINIT_CHANGE:
	case STATIONINIT_CREATE:
		if(pStation != NULL)
		{
			pStation = LoadStationFromDB(sStationID, pStation);
		}
		else
		{
			pStation = LoadStationFromDB(sStationID, NULL);
			if(pStation != NULL)
			{
				pStation->LoadStatusFromDB(0);
				pStation->LoadStatusFromDB(1);
			}
		}
		break;

	case STATIONINIT_DELETE:
		if(pStation != NULL)
		{
			m_StationMap.RemoveKey(pStation->m_sID);
		}
		break;
	}
	return pStation;
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   LoadStationFromDB	
返回值:   CStationObj*	厂站对象
功能概要: 从数据库载入指定厂站的信息
参数: CString sStationID	指定要载入的厂站ID
参数: CStationObj * pStation	保存厂站信息的对象，如果为NULL，函数将新建厂站对象并在返回值中返回
*****************************************************/
CStationObj* CDataEngine::LoadStationFromDB( CString sStationID, CStationObj* pStation )
{
	CStationObj* pNew = pStation;
	if(sStationID.IsEmpty())
		return pNew;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件
	Condition con1;
	CString str;
	str="(TYPE=1 or TYPE=4 or TYPE=7 or TYPE=8 or TYPE=9)";
	pApp->m_DBEngine.SetCondition(sql, con1, str);

	Condition con2;
	str.Format("station_id='%s'", sStationID);
	pApp->m_DBEngine.SetCondition(sql, con2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::BuildStationList, 查询失败");
		delete[] sError;
		delete pMemset;
		return pNew;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		
		if(nCount == 1)
		{
			//创建对象
			if(pNew == NULL)
				pNew = new CStationObj;
			if(pNew->Load(pMemset))
			{
				//读取数据成功,加入到链表
				if(pStation == NULL)
					m_StationMap.SetAt(pNew->m_sID, pNew);
				pNew->InitLock();
				//pNew->LoadData();
				pNew->BuildData();
			}
			else
			{
				//读取数据失败, delete
				if(pStation == NULL)
				{
					delete pNew;
					pNew = NULL;
				}
			}
			pMemset->MoveNext();
		}
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return pNew;
}

CStationObj* CDataEngine::AddVirtualStation( CString stationID, CString stationName, CString netID )
{
	CStationObj* pObj = new CStationObj;
	pObj->m_sID = stationID;
	pObj->m_sName = stationName;
	pObj->m_sNetID = netID;
	pObj->m_IsDistanceVirtual = TRUE;

	m_StationMap.SetAt(pObj->m_sID, pObj);

	return pObj;
}

BOOL CDataEngine::LoadSituationAndDisposeFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "code_type", EX_STTP_DATA_TYPE_INT);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet  pMemset;
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(153, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::LoadSituationAndDisposeFromDB, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			int codetype = atoi( pMemset.GetValue((UINT)1));
			switch( codetype )
			{
			case 1: //情况说明
				{
					int id = atoi( pMemset.GetValue((UINT)0));
					CString name = pMemset.GetValue((UINT)2);
					m_arrSituation.InsertAt( id, name );
				}
				break;
			case 2: //处理意见
				{
					int id = atoi( pMemset.GetValue((UINT)0));
					CString name = pMemset.GetValue((UINT)2);
					m_arrDispose.InsertAt( id, name );
				}
				break;
			}
			pMemset.MoveNext();	
		}
		
	}
	else
	{
		CString str;
		str.Format("CDataEngine::LoadSituationAndDisposeFromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	return bReturn;
}

BOOL CDataEngine::QueryPTNoteCount()
{
	//组建查询语句
	CString strSQL;
	strSQL.Format("select pt_id, count(*) from TB_PT_NOTE_DATA group by pt_id");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDataEngine::QueryPTNoteCount, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::QueryPTNoteCount, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strPTID = pMemset->GetValue((UINT)0);
			int count = atoi(pMemset->GetValue(1));
				//查找对应的二次设备
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC);
				if(pObj != NULL)
				{
					pObj->m_nNoteCount = count;
				}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryPTNoteCount,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CDataEngine::QueryPTNotes(CString ptid, CPtrArray& arr)
{
	BOOL bReturn = TRUE;	
	
	WriteLogEx("CDataEngine::QueryPTNotes, 开始查询");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//取得数据库内有历史数据的Time列表
	CString str;
	
	//字段
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", ptid);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//按CURTIME大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by UPDATE_TIME DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);
				
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(162, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CDataEngine::QueryPTNotes, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CDataEngine::QueryPTNotes,查询到%d条", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		
		for(int i = 0; i < nCount; i++)
		{
			PT_NOTE* note = new PT_NOTE;
			CString str;
			note->id = pMemset.GetValue((UINT)0);
			note->pt_id = pMemset.GetValue(1);
			note->note_name = pMemset.GetValue(2);
			note->note_content = pMemset.GetValue(3);
			CString sTime = pMemset.GetValue(4);
			note->create_time = StringToTime( sTime);
			sTime = pMemset.GetValue(5);
			note->update_time = StringToTime( sTime);
			
			arr.Add(note);
			
			pMemset.MoveNext();
		}
		
	}
	else
	{
		CString str;
		str.Format("CDataEngine::QueryPTNotes,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	if(!bReturn)
		return FALSE;
	
	WriteLogEx("CDataEngine::QueryPTNotes, 查询结束");
	
	return bReturn;
}

void CDataEngine::GetSortStationList(DEVICE_LIST& listDevice, CString sNetID /*= ""*/)
{
	CTypedPtrArray<CPtrArray , CDeviceObj*> arr;
	GetStationList( listDevice, sNetID );
	POSITION pos = listDevice.GetHeadPosition();
	while( pos != NULL )
	{
		CDeviceObj* dev = listDevice.GetNext(pos);
		arr.Add(dev);
	}
	//冒泡排序
	
	int ncount = arr.GetSize();
	int i, j;
	CDeviceObj *station1, *station2;
	for( i= 0; i < ncount -1; i++ )
	{
		for( j=i+1; j < ncount; j++ )
		{
			station1 = arr.GetAt(i); 
			station2 = arr.GetAt(j);
			CString name1 = station1->m_sName;
			CString name2 = station2->m_sName;
			//2017-6-22 排序时不再去除电压等级信息
			/*name1.MakeLower();
			name2.MakeLower();
			int nfind = name1.Find("kv", 0);
			if( nfind >= 0 ){
				name1.Delete(0, nfind+2);
			}
			nfind = name2.Find("kv", 0);
			if( nfind >= 0 ){
				name2.Delete(0, nfind+2);
			}*/
			if( strcmp(name1, name2) > 0 ){
				arr.SetAt(i, station2);
				arr.SetAt(j, station1);
			}
		}
	}

	listDevice.RemoveAll();
	for( i = 0; i < arr.GetSize(); i++ )
	{
		CDeviceObj* dev = arr.GetAt(i);
		listDevice.AddTail( dev );
	}
}

BOOL CDataEngine::RefreshStationStatus()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件
	Condition con1;
	CString str;
	str="(TYPE=1 or TYPE=4 or TYPE=7 or TYPE=8 or TYPE=9 or TYPE=113 or TYPE=114)";
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	if( !pApp->MgrAllStations())
	{
		Condition con2;
		CString str;
		str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition( sql, con2, str);
	}
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::RefreshStationStatus, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDataEngine::RefreshStationStatus, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString id = pMemset->GetValue(int(1)); //厂站ID
			CStationObj* pStation = NULL;
			m_StationMap.Lookup(id, (CObject *&)pStation);
			if( pStation != NULL ){
				//通道0
				CString str = pMemset->GetRealDataValue("ch0_commu_status");
				int status = atoi(str);
				str = pMemset->GetRealDataValue("ch0_commustatus_time");
				CTime tm = StringToTime( str);
				str = pMemset->GetRealDataValue("ch0_commu_reason");
				int reason = atoi(str);
				pStation->SetStatus(status, tm, reason, 0, tm, pStation->m_nIPFlag);
				
				//通道1
				str = pMemset->GetRealDataValue("ch1_commu_status");
				status = atoi(str);
				str = pMemset->GetRealDataValue("ch1_commustatus_time");
				tm = StringToTime( str);
				str = pMemset->GetRealDataValue("ch1_commu_reason");
				reason = atoi(str);
				pStation->SetStatus(status, tm, reason, 1, tm, pStation->m_nIPFlag);			
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::RefreshStationStatus,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}
