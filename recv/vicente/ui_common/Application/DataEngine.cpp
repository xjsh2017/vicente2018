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

#include "DataEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)//屏蔽4786错误

/////////////////////////////////////////////////////////////////////////////
// CDataEngine
//##ModelId=49B87BA9003F
/****************************************************
Date:2011/10/17  Author:LYH
函数名:   CDataEngine	
返回值:   	
功能概要: 
参数: CDBEngine * pDBEngine	
*****************************************************/
CDataEngine::CDataEngine( CDBEngine* pDBEngine )
{
	m_pDBEngine = pDBEngine;
	m_sDownDir = "";
	m_sLogDir = "";

	m_NetList.RemoveAll();		//地区链表
	m_StationMap.RemoveAll();	//厂站链表
	m_AllSEC.RemoveAll();

	m_arrUniversalActChr.RemoveAll();

	m_pSplashWnd = NULL;

	m_nSecType = -1;
	m_nLoadDataModel = 0;
	m_bLoadNet = TRUE;
	m_bLoad20162Config = FALSE;
	m_bSend20162 = FALSE;
	m_bLoadCPUInfo = TRUE;
	m_bLoadZoneInfo = TRUE;

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

	::InitializeCriticalSection(&m_criSection);
}

//##ModelId=49B87BA90179
CDataEngine::~CDataEngine()
{
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_NET_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//条件
		Condition con1;
		CString str;
		str="TYPE=1 or TYPE=4";
		m_pDBEngine->SetCondition(sql, con1, str);
		
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_STATION_CFG, sql, sError, pMemset);
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
					if(m_nLoadDataModel == 1)
					{
						pObj->InitLock();
						pObj->LoadData(m_pDBEngine);
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
	MYASSERT_BOOL(m_pDBEngine);

	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BAY_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BREAKER_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BUS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_CAPACITOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_GEN_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		Field Field1;
		m_pDBEngine->SetField(sql, Field1, "line_id", EX_STTP_DATA_TYPE_STRING);
		
		Field Field2;
		m_pDBEngine->SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
		
		Field Field3;
		m_pDBEngine->SetField(sql, Field3, "code", EX_STTP_DATA_TYPE_INT);
		
		Field Field4;
		m_pDBEngine->SetField(sql, Field4, "run_status", EX_STTP_DATA_TYPE_INT);

		Field Field41;
		m_pDBEngine->SetField(sql, Field41, "voltage", EX_STTP_DATA_TYPE_INT);
		
		Field Field5;
		m_pDBEngine->SetField(sql, Field5, "103group", EX_STTP_DATA_TYPE_INT);
		
		Field Field6;
		m_pDBEngine->SetField(sql, Field6, "103item", EX_STTP_DATA_TYPE_INT);

		Field Field7;
		m_pDBEngine->SetField(sql, Field7, "station_id", EX_STTP_DATA_TYPE_STRING);

		Field Field8;
		m_pDBEngine->SetField(sql, Field8, "bay_id", EX_STTP_DATA_TYPE_STRING);


		
		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_LINE_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_PMS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_REACTOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_SWITCH_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult =m_pDBEngine->XJSelectData(EX_STTP_INFO_TRANS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		Field Field1;
		m_pDBEngine->SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);

		Field Field2;
		m_pDBEngine->SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);

		Field Field3;
		m_pDBEngine->SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);

		Field Field4;
		m_pDBEngine->SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);

		Field Field41;
		m_pDBEngine->SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);

		Field Field5;
		m_pDBEngine->SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
		
		Field Field6;
		m_pDBEngine->SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);

		Field Field7;
		m_pDBEngine->SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);

		Field Field8;
		m_pDBEngine->SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);

		Field Field9;
		m_pDBEngine->SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);

		Field Field10;
		m_pDBEngine->SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);

		Field Field11;
		m_pDBEngine->SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);

		Field Field12;
		m_pDBEngine->SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);

		Field Field13;
		m_pDBEngine->SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);

		Field Field14;
		m_pDBEngine->SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);

		Field Field15;
		m_pDBEngine->SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);

		Field Field16;
		m_pDBEngine->SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);

		Field Field17;
		m_pDBEngine->SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);

		Field Field18;
		m_pDBEngine->SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);

		Field Field19;
		m_pDBEngine->SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);

		Field Field20;
		m_pDBEngine->SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);

		Field Field21;
		m_pDBEngine->SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);

		Field Field22;
		m_pDBEngine->SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);

		Field Field23;
		m_pDBEngine->SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);

		//条件
		//设备类型
		Condition condition1;
		bzero(&condition1, sizeof(Condition));
		CString str;
		str.Format("type > 0");
		if(m_nSecType >= 0)
			str.Format("type = %d", m_nSecType);
		strncpy(condition1.ConditionContent, str, str.GetLength());
		sql.Conditionlist.push_back(condition1);

		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_SECDEV_CFG, sql, sError, pMemset);
		}
		catch (CException* e)
		{
			e->Delete();
			WriteLog("CDataEngine::BuildSecList, 查询失败");
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
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
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
	LONGLONG startTime = 0;
	//如果是子站角色无需加载地区列表
	if (m_bLoadNet)
	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入地区数据...");
		startTime = StartCount();
		//先载入地区, 厂站, 再载入一次设备, 后载入二次设备, 最后载入保护管理机
		if(!BuildNetList())
		{
			WriteLog("载入地区数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入地区数据时间", startTime);
	}
	
	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("载入厂站数据...");
	startTime = StartCount();
	if(!BuildStationList())
	{
		WriteLog("载入厂站数据出错!", XJ_LOG_LV1);
	}
	EndCount("载入厂站数据时间", startTime);

	if(m_nLoadDataModel == 0)
	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入间隔数据...");
		startTime = StartCount();
		if(!BuildBayList())
		{
			WriteLog("载入间隔数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入间隔数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入开关数据...");
		startTime = StartCount();
		if(!BuildBreakList())
		{
			WriteLog("载入开关数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入开关数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入母线数据...");
		startTime = StartCount();
		if(!BuildBusList())
		{
			WriteLog("载入母线数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入母线数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入电容器数据...");
		startTime = StartCount();
		if(!BuildCapList())
		{
			WriteLog("载入电容器数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入电容器数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入发电机数据...");
		startTime = StartCount();
		if(!BuildGenList())
		{
			WriteLog("载入发电机数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入发电机数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入线路数据...");
		startTime = StartCount();
		if(!BuildLineList())
		{
			WriteLog("载入路线数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入路线数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入电抗器数据...");
		startTime = StartCount();
		if(!BuildReactorList())
		{
			WriteLog("载入电抗器数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入电抗器数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入刀闸数据...");
		startTime = StartCount();
		if(!BuildSwitchList())
		{
			WriteLog("载入刀闸数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入刀闸数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入变压器数据...");
		startTime = StartCount();
		if(!BuildTransList())
		{
			WriteLog("载入变压器数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入变压器数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入保护管理机数据...");
		startTime = StartCount();
		if(!BuildPmsList())
		{
			WriteLog("载入保护管理机数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入保护管理机数据时间", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入二次设备数据...");
		startTime = StartCount();
		if(!BuildSecList())
		{
			WriteLog("载入二次设备数据出错!", XJ_LOG_LV1);
		}
		EndCount("载入二次设备数据时间", startTime);
		
		if(m_bLoadCPUInfo)
		{
			if(m_pSplashWnd != NULL)
				m_pSplashWnd->AddDetailString("载入CPU数据...");
			startTime = StartCount();
			if(!BuildCpuList())
			{
				WriteLog("载入CPU数据出错!", XJ_LOG_LV1);
			}
			EndCount("载入CPU数据时间", startTime);
		}
		
		if(m_bLoadZoneInfo)
		{
			if(m_pSplashWnd != NULL)
				m_pSplashWnd->AddDetailString("载入定值区号数据...");
			startTime = StartCount();
			if(!BuildZoneList())
			{
				WriteLog("载入定值区号出错!", XJ_LOG_LV1);
			}
		
			EndCount("载入定值区号数据时间", startTime);
		}
	}

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("载入通用特征量数据...");
	startTime = StartCount();
	if(!BuildUniversalActChr())
	{
		WriteLog("载入通用特征量数据出错", XJ_LOG_LV1);
	}
	EndCount("载入通用特征量数据时间", startTime);

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("载入厂站通断原因数据...");
	if(!BuildStationStatusConfig())
	{
		WriteLog("载入厂站通断原因数据出错", XJ_LOG_LV1);
	}	

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("载入通讯超时数据...");
	GetFuncTimeOut();
	WriteLog("读入功能通信超时设置完毕", XJ_LOG_LV1);

	if(m_bLoad20162Config)
 	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("载入20162配置数据...");
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_STATION_STATUS_CONFIG, sql, sError, pMemset);
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

	//厂站
	for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
	{
		m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_StationMap.RemoveAll();

	//地区
	for(pos = m_NetList.GetHeadPosition(); pos != NULL;)
	{
		pObj = (CDeviceObj*)m_NetList.GetNext(pos);
		if(pObj != NULL)
		{
			delete pObj;
			pObj = NULL;
		}
	}
	m_NetList.RemoveAll();

	//清除通用特征值
	for(int i = 0; i < m_arrUniversalActChr.GetSize(); i++)
	{
		delete m_arrUniversalActChr[i];
	}
	m_arrUniversalActChr.RemoveAll();


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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
				nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_CHARCTOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_FUN_TIMEOUT_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	Field field1;
	m_pDBEngine->SetField(sql, field1, "pt_id", EX_STTP_DATA_TYPE_STRING);

	Field field2;
	m_pDBEngine->SetField(sql, field2, "cpu_code", EX_STTP_DATA_TYPE_INT);

	Field field3;
	m_pDBEngine->SetField(sql, field3, "cpu_des", EX_STTP_DATA_TYPE_STRING);

	Field field31;
	m_pDBEngine->SetField(sql, field31, "cpu_ver", EX_STTP_DATA_TYPE_STRING);

	Field field4;
	m_pDBEngine->SetField(sql, field4, "from_zone", EX_STTP_DATA_TYPE_INT);

	Field field5;
	m_pDBEngine->SetField(sql, field5, "to_zone", EX_STTP_DATA_TYPE_INT);

	Condition con1;
	m_pDBEngine->SetCondition(sql, con1, "order by pt_id, cpu_code asc", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_PT_CPU_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;

	Condition con1;
	m_pDBEngine->SetCondition(sql, con1, "order by pt_id, cpu_code, zone_id asc", EX_STTP_WHERE_ABNORMALITY);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
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
	MYASSERT(m_pDBEngine);
	//clear
	ClearRefreshSec();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	Field Field1;
	m_pDBEngine->SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field2;
	m_pDBEngine->SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	m_pDBEngine->SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);
	
	Field Field4;
	m_pDBEngine->SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);

	Field Field41;
	m_pDBEngine->SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);
	
	Field Field5;
	m_pDBEngine->SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	m_pDBEngine->SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);
	
	Field Field7;
	m_pDBEngine->SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);
	
	Field Field8;
	m_pDBEngine->SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field9;
	m_pDBEngine->SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	m_pDBEngine->SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);
	
	Field Field11;
	m_pDBEngine->SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	m_pDBEngine->SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);
	
	Field Field13;
	m_pDBEngine->SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field14;
	m_pDBEngine->SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	m_pDBEngine->SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);
	
	Field Field16;
	m_pDBEngine->SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field17;
	m_pDBEngine->SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);
	
	Field Field18;
	m_pDBEngine->SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);
	
	Field Field19;
	m_pDBEngine->SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);
	
	Field Field20;
	m_pDBEngine->SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);
	
	Field Field21;
	m_pDBEngine->SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);
	
	Field Field22;
	m_pDBEngine->SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);
	
	Field Field23;
	m_pDBEngine->SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);

	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult =m_pDBEngine->XJSelectData(EX_STTP_INFO_SECDEV_CFG, sql, sError, pMemset);
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
	MYASSERT(m_pDBEngine);
	//组建查询语句
	CString strSQL;
	//------------------------
	strSQL.Format("select * from tb_pt_station_status A JOIN (select station_id,Reserve1,max(Time) Time from tb_pt_station_status  group by station_id,Reserve1) B ON A.station_id = B.station_id and A.Time = B.Time");
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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

	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT(m_pDBEngine);
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field1;
	m_pDBEngine->SetField(sql, Field1, "reverse2", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_FAULTSHORTENCONFIG_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::Load20162Config, 查询失败");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_bSend20162 = atoi(pMemset->GetValue((UINT)0));
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	Condition con1;
	m_pDBEngine->SetCondition(sql, con1, "order by pt_id, cpu_code, zone_code asc", EX_STTP_WHERE_ABNORMALITY);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_ZONE_BASE_CFG, sql, sError, pMemset);
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
void CDataEngine::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType, CString sStationID/*=""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetPriDeviceList(listDevice, nDeviceType);
			}
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
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, CString sPriID /*= ""*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetSecList(listDevice, sPriID, nSecType);
			}
		}
	}
	else
	{
		//先找到对应厂站
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, sPriID, nSecType);
	}
}

/****************************************************
Date:2011/12/30  Author:LYH
函数名:   GetSecList	
返回值:   void	
功能概要: 
参数: DEVICE_LIST & listDevice	
参数: int nPriType	
参数: int nSecType	
参数: CString sStaionID	
参数: CString sNetID	
*****************************************************/
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, int nPriType /*= 0*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetSecList(listDevice, nPriType, nSecType);
			}
		}
	}
	else
	{
		//先找到对应厂站
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, nPriType, nSecType);
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
Date:2011/10/17  Author:LYH
函数名:   GetDownDir	
返回值:   CString	
功能概要: 
*****************************************************/
CString CDataEngine::GetDownDir()
{
	if(m_sDownDir == "")
	{
		LoadDownDir();
	}
	return m_sDownDir;
}

/****************************************************
Date:2011/10/18  Author:LYH
函数名:   LoadDownDir	
返回值:   void	
功能概要: 
*****************************************************/
void CDataEngine::LoadDownDir()
{
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//查询comtrade_path
	Field Field1;
	m_pDBEngine->SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);

	Condition con1;
	m_pDBEngine->SetCondition(sql, con1, "KeyName='DownDir'");

	CMemSet* pMemset;
	pMemset = new CMemSet;

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadDownDir, 查询失败");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sDownDir = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("CXJBrowserApp::LoadDownDir, 路径为:%s", m_sDownDir);
			WriteLog(str);
		}
		else
		{
			WriteLog("找不到系统下载路径");
		}

	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadDownDir, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if(m_sDownDir != "")
	{
		//把有可能的/都换成"\\"
		m_sDownDir.Replace("/", "\\");
		if(m_sDownDir.Right(1)!="\\")
			m_sDownDir += "\\";
	}
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   SetSecCommStatus	
返回值:   void	
功能概要: 设置二次设备通信状态
参数: CSecObj * pSec	指定二次设备
参数: int nStatus	指定状态
*****************************************************/
void CDataEngine::SetSecCommStatus( CSecObj* pSec, int nStatus )
{
	MYASSERT(pSec);
	pSec->SetCommStatus(nStatus);
	CStationObj* pStation = (CStationObj*)FindDevice(pSec->m_sStationID, TYPE_STATION);
	MYASSERT(pStation);
	pStation->SecCommChange(pSec, nStatus);
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   SetSecRunStatus	
返回值:   void	
功能概要: 设置二次设备运行状态
参数: CSecObj * pSec	指定二次设备
参数: int nStatus	指定状态
*****************************************************/
void CDataEngine::SetSecRunStatus( CSecObj* pSec, int nStatus )
{
	MYASSERT(pSec);
	pSec->SetRunStatus(nStatus);
	CStationObj* pStation = (CStationObj*)FindDevice(pSec->m_sStationID, TYPE_STATION);
	MYASSERT(pStation);
	pStation->SecRunChange(pSec, nStatus);
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   GetSysLogPath	
返回值:   CString	日志文件路径
功能概要: 取得日志文件路径
*****************************************************/
CString CDataEngine::LoadSysLogPath()
{
	CString sReturn = "";
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//查询comtrade_path
	Field Field1;
	m_pDBEngine->SetField(sql, Field1, "log_path", EX_STTP_DATA_TYPE_STRING);

	CMemSet* pMemset;
	pMemset = new CMemSet;

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	int nResult;
	try
	{
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_LOCAL_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadSysLogPath, 查询失败");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			sReturn = pMemset->GetValue((UINT)0);
		}
		else
		{
			WriteLog("找不到系统下载路径");
		}
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return sReturn;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   GetSysLogPath	
返回值:   CString	日志文件路径
功能概要: 取得日志文件路径
*****************************************************/
CString CDataEngine::GetSysLogPath(CString sProjectName, CString sLocalPath)
{
	if(m_sLogDir == "")
	{
		m_sLogDir = LoadSysLogPath();
		if(m_sLogDir.Left(1) == ".")
			m_sLogDir = sLocalPath+m_sLogDir;
	}
	CString sReturn = m_sLogDir;
	if(sReturn != "")
	{
		//把有可能的"\\"都换成/
		sReturn.Replace("\\", "/");
		if(sReturn.Right(1)!="/")
			sReturn += "/";
		//加上模块名
		sReturn += sProjectName;
		sReturn += "/";
	}
	return sReturn;
}

