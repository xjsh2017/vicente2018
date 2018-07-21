/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   17:15
	filename: 	F:\vicente\Code\src\XJBrowser\DataEngine.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	DataEngine
	file ext:	cpp
	author:		LYH
	
	purpose:	��������
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
	m_NetList.RemoveAll();		//��������
	m_StationMap.RemoveAll();	//��վ����
	m_AllSEC.RemoveAll();
	m_arrMark.RemoveAll();
	m_SepStationMap.RemoveAll();
	
	m_arrUniversalActChr.RemoveAll();

	virtualStationName = StringFromID(IDS_MODEL_VIRTUALSTATION);

	/** @brief           �����ٻ���ֵ��ʱʱ��(��λ:��)*/
	m_nPTSettingTimeOut = 240;
	/** @brief           ¼�����ٻ���ֵ��ʱʱ��(��λ:��)*/
	m_nWRSettingTimeOut = 500;
	/** @brief           �����ٻ�ģ������ʱʱ��(��λ:��)*/
	m_nPTAiTimeOut = 120;
	/** @brief           �����ٻ���������ʱʱ��(��λ:��)*/
	m_nPTDiTimeOut = 120;
	/** @brief           �����ٻ���ѹ�峬ʱʱ��(��λ:��)*/
	m_nPTSoftBoardTimeOut = 120;
	/** @brief           �����ٻ���ֵ���ų�ʱʱ��(��λ:��)*/
	m_nPTZoneTimeOut = 120;
	/** @brief           �ٻ�װ��ʱ�䳬ʱʱ��(��λ:��)*/
	m_nDevTimeTimeOut = 120;
	/** @brief           �ٻ�װ�û�����Ϣ��ʱʱ��(��λ:��)*/
	m_nDevGeneralTimeOut = 120;
	/** @brief           ��ֵ�޸ĳ�ʱʱ��(��λ:��)*/
	m_nModifySettingTimeOut = 120;
	/** @brief           ��ֵ���л���ʱʱ��(��λ:��)*/
	m_nChangeZoneTimeOut = 120;
	/** @brief           ��ѹ���л���ʱʱ��(��λ:��)*/
	m_nChangeSBTimeOut = 120;
	/** @brief           װ�ö�ʱ��ʱʱ��(��λ:��)*/
	m_nVerifyTimeTimeOut = 120;
	/** @brief           �źŸ��鳬ʱʱ��(��λ:��)*/
	m_nSignalResetTimeOut = 120;
	/** @brief           Զ��������ʱʱ��(��λ:��)*/
	m_nRemoteStartTimeOut = 120;
	/** @brief           �ٻ�¼���ļ���ʱʱ��(��λ:��)*/
	m_nWavFileTimeOut = 120;
	/** @brief           �ٻ�¼���б�ʱʱ��(��λ:��)*/
	m_nWavFileListTimeOut = 120;
	/** @brief           �ٻ���ʷ�¼���ʱʱ��(��λ:��)*/
	m_nHistoryEventTimeOut = 120;
	/** @brief           �ٻ�ͨ���ļ���ʱʱ��(��λ:��)*/
	m_nCommFileTimeOut = 120;
	/** @brief           �ٻ�ͨ���ļ��б�ʱʱ��(��λ:��)*/
	m_nCommFileListTimeOut = 120;
	/** @brief           �ٻ�ͨ���ļ��б�ʱʱ��(��λ:��)*/
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
 �� �� ����BuildNetList()
 ���ܸ�Ҫ����ȡ���ݿ��е�������, ���ɵ�������, ������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA900AC
BOOL CDataEngine::BuildNetList()
{
	ClearNetList();
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildNetList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildNetList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CNetObj * pObj = new CNetObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,���뵽����
					m_NetList.AddTail(pObj);
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildNetList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildStationList()
 ���ܸ�Ҫ����ȡ���ݿ��г�վ����, ���ɳ�վ����, ������վ����
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA900BB
BOOL CDataEngine::BuildStationList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//����
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
			WriteLog("CDataEngine::BuildStationList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildStationList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CStationObj * pObj = new CStationObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,���뵽����
					m_StationMap.SetAt(pObj->m_sID, pObj);
					if(g_LoadDataModel == 1)
					{
						pObj->InitLock();
						pObj->LoadData();
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildStationList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildBayList()
 ���ܸ�Ҫ����ȡ���ݿ��м������, ���ɼ������, �����������
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
//##ModelId=49B87BA900EA
BOOL CDataEngine::BuildBayList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildBayList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildBayList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CBayObj * pObj = new CBayObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBayList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildBreakList()
 ���ܸ�Ҫ����ȡ���ݿ��п�������, ���ɿ��ض���, ������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA900DB
BOOL CDataEngine::BuildBreakList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildBreakList, ��ѯʧ��");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			//��ѯ�ɹ�
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount >= 0)
			{
				CString str;
				str.Format("CDataEngine::BuildBreakList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CBreakObj * pObj = new CBreakObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBreakList, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildBusList()
 ���ܸ�Ҫ����ȡ���ݿ���ĸ������, ����ĸ�߶���, ����ĸ������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA900DA
BOOL CDataEngine::BuildBusList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildBusList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildBusList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CBusObj * pObj = new CBusObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildBusList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildCapList()
 ���ܸ�Ҫ����ȡ���ݿ��е���������, ���ɵ���������, ��������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA9010A
BOOL CDataEngine::BuildCapList()
{
	
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildCapList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildCapList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CCapObj * pObj = new CCapObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildCapList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildGenList()
 ���ܸ�Ҫ����ȡ���ݿ��з��������, ���ɷ��������, �������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90129
BOOL CDataEngine::BuildGenList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildGenList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildGenList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CGenObj * pObj = new CGenObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildGenList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildLineList()
 ���ܸ�Ҫ����ȡ���ݿ�����·����, ����·�߶���, ����·������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90109
BOOL CDataEngine::BuildLineList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildLineList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildLineList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CLineObj * pObj = new CLineObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildLineList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildPmsList()
 ���ܸ�Ҫ����ȡ���ݿ��б������������, ���ɱ������������,�����������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90139
BOOL CDataEngine::BuildPmsList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildPmsList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildPmsList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CPmsObj * pObj = new CPmsObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildPmsList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildReactorList()
 ���ܸ�Ҫ����ȡ���ݿ��е翹������, ���ɵ翹������, �����翹������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90119
BOOL CDataEngine::BuildReactorList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildReactorList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildReactorList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CReactorObj * pObj = new CReactorObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildReactorList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildSwitvchList()
 ���ܸ�Ҫ����ȡ���ݿ��е�բ����, ���ɵ�բ����, ������բ����
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90138
BOOL CDataEngine::BuildSwitchList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildSwitchList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildSwitchList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CSwitchObj * pObj = new CSwitchObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildSwitchList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildTransList()
 ���ܸ�Ҫ����ȡ���ݿ��б�ѹ������, ���ɱ�ѹ������, ������ѹ������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90128
BOOL CDataEngine::BuildTransList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildTransList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildTransList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CTransObj * pObj = new CTransObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
						//��Ӧ��վΪ��
						delete pObj;
						pObj = NULL;
					}
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildTransList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildSecList()
 ���ܸ�Ҫ����ȡ���ݿ��ж����豸����, ���ɶ����豸����, ������������,¼��������,�����������豸����
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90148
BOOL CDataEngine::BuildSecList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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


		//����
		//�豸����
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
			WriteLog("CDataEngine::BuildSecList, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildSecList, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
				CSecObj * pObj = new CSecObj;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,���뵽����
					if(pObj->m_nType == TYPE_PROTECT)
					{
						//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
							//��Ӧ��վΪ��
							delete pObj;
							pObj = NULL;
						}
					}
					else if(pObj->m_nType == TYPE_WAVEREC)
					{
						//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
							//��Ӧ��վΪ��
							delete pObj;
							pObj = NULL;
						}
					}
					else
					{
						//��ȡ���ݳɹ�,�ҵ���Ӧ��վ,���뵽����
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
							//��Ӧ��վΪ��
							delete pObj;
							pObj = NULL;
						}
						if(pObj != NULL && pObj->m_nSecType == 21 )
						{
							//����������վ
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
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();
			}
		}
		else
		{
			CString str;
			str.Format("CDataEngine::BuildSecList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����LoadData()
 ���ܸ�Ҫ�����ø��豸����Build����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA9015B
BOOL CDataEngine::LoadData()
{
	LONGLONG startTime = StartCount();

	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_STATIONREASON) );
	if(!BuildStationStatusConfig())
	{
		WriteLog("���볧վͨ��ԭ�����ݳ���");
	}	

	//�������վ��ɫ������ص����б�
	if (g_role != MODE_SUB)
	{
		//���������, ��վ, ������һ���豸, ����������豸, ������뱣�������
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_AREA) );
		if(!BuildNetList())
		{
			WriteLog("����������ݳ���!");
		}
	}
	EndCount("�����������ʱ��", startTime);
	
	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_STATION) );

	startTime = StartCount();
	LoadServiceDepartmentFromDB();
	EndCount("������޵�λ����ʱ��", startTime);

	startTime = StartCount();
	if(!BuildStationList())
	{
		WriteLog("���볧վ���ݳ���!");
	}
	EndCount("���볧վ����ʱ��", startTime);

	if(g_role == MODE_SEP)
	{
		//�����վ��Ϣ
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SEPSTATION));
		if(!BuildSepStationList())
			WriteLog("�����վվ���ݳ���!");
	}

	if(g_LoadDataModel == 0)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BAY));
		startTime = StartCount();
		if(!BuildBayList())
		{
			WriteLog("���������ݳ���!");
		}
		EndCount("����������ʱ��", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_MANAGER));
		startTime = StartCount();
		if(!LoadManagedepartmentfromDB())
		{
			WriteLog("���볧վ���ܵ�λ����!");
		}
		EndCount("����������ʱ��", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_MAINTENANCE));
		startTime = StartCount();
		if(!LoadMaintaindepartmentfromDB())
		{
			WriteLog("���볧վ��ά��λ����!");
		}
		EndCount("����������ʱ��", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SUSPENDREASON));
		startTime = StartCount();
		if(!LoadMarkInfofromDB())
		{
			WriteLog("�������ԭ����Ϣ����!");
		}
		EndCount("�������ԭ������ʱ��", startTime);

		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BREAKER));
		startTime = StartCount();
		if(!BuildBreakList())
		{
			WriteLog("���뿪�����ݳ���!");
		}
		EndCount("���뿪������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_BUS));
		startTime = StartCount();
		if(!BuildBusList())
		{
			WriteLog("����ĸ�����ݳ���!");
		}
		EndCount("����ĸ������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_CAP));
		startTime = StartCount();
		if(!BuildCapList())
		{
			WriteLog("������������ݳ���!");
		}
		EndCount("�������������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_GEN));
		startTime = StartCount();
		if(!BuildGenList())
		{
			WriteLog("���뷢������ݳ���!");
		}
		EndCount("���뷢�������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_LINE));
		startTime = StartCount();
		if(!BuildLineList())
		{
			WriteLog("����·�����ݳ���!");
		}
		EndCount("����·������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_REACTOR));
		startTime = StartCount();
		if(!BuildReactorList())
		{
			WriteLog("����翹�����ݳ���!");
		}
		EndCount("����翹������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SWITCH));
		startTime = StartCount();
		if(!BuildSwitchList())
		{
			WriteLog("���뵶բ���ݳ���!");
		}
		EndCount("���뵶բ����ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_TRANS));
		startTime = StartCount();
		if(!BuildTransList())
		{
			WriteLog("�����ѹ�����ݳ���!");
		}
		EndCount("�����ѹ������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_PMS));
		startTime = StartCount();
		if(!BuildPmsList())
		{
			WriteLog("���뱣����������ݳ���!");
		}
		EndCount("���뱣�����������ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_SEC));
		startTime = StartCount();
		if(!BuildSecList())
		{
			WriteLog("��������豸���ݳ���!");
		}
		EndCount("��������豸����ʱ��", startTime);
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_CPU));
		startTime = StartCount();
		if(!BuildCpuList())
		{
			WriteLog("����CPU���ݳ���!");
		}
		EndCount("����CPU����ʱ��", startTime);
		
		startTime = StartCount();
		
		g_wndWait.AddDetailString( StringFromID(IDS_LOADING_ZONE));
		if(!BuildZoneList())
		{
			WriteLog("���붨ֵ���ų���!");
		}
		
		EndCount("���붨ֵ��������ʱ��", startTime);
	}

	g_wndWait.AddDetailString( StringFromID(IDS_LOADING_UNICHR));
	startTime = StartCount();
	if(!BuildUniversalActChr())
	{
		WriteLog("����ͨ�����������ݳ���");
	}
	EndCount("����ͨ������������ʱ��", startTime);

	GetFuncTimeOut();
	WriteLog("���빦��ͨ�ų�ʱ�������");

	LoadSituationAndDisposeFromDB();
	WriteLog("����Ѳ�����˵���ʹ�������������");

/*	startTime = StartCount();
	QueryHasSettingConfig();
	WriteLog("��ѯ�������豸��ֵ���������");
	EndCount("��ѯ�������豸��ֵ������ʱ��", startTime);

	startTime = StartCount();
	QueryHasDigitalConfig();
	WriteLog("��ѯ�������豸���������������");
	EndCount("��ѯ�������豸������������ʱ��", startTime);

	startTime = StartCount();
	QueryHasSoftBoardConfig();
	WriteLog("��ѯ�������豸��ѹ�����������");
	EndCount("��ѯ�������豸��ѹ��������ʱ��", startTime);

	startTime = StartCount();
	QueryHasAnalogConfig();
	WriteLog("��ѯ�������豸ģ�������������");
	EndCount("��ѯ�������豸ģ����������ʱ��", startTime);*/

	if( g_PTShowNote )
	{
		startTime = StartCount();
		QueryPTNoteCount();
		WriteLog("��ѯ�������豸��ע�����");
		EndCount("��ѯ�������豸��ע��ʱ��", startTime);
	}

	if (g_role != MODE_SUB)
	{
		Load20162Config();
	}

	return TRUE;
}

/*************************************************************
 �� �� ����FindDevice()
 ���ܸ�Ҫ�������豸
 �� �� ֵ: ���ҳɹ������豸ָ��, ʧ�ܷ���NULL
 ��    ����param1   �豸ID
           Param2   �豸����
**************************************************************/
//##ModelId=49B87BA90167
CDeviceObj* CDataEngine::FindDevice(CString DeviceID, int DeviceType, CString StationID /*= ""*/)
{
	//�����豸���͵���ͬ���豸�����в���
	//ȥ�ո�
// 	DeviceID.TrimLeft();
// 	DeviceID.TrimRight();
	CDeviceObj * pDevice = NULL;
	CDeviceObj * pObj = NULL;
	POSITION pos;
	CString sID;
	switch(DeviceType)
	{
	case TYPE_NET: //����
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
	case TYPE_STATION: //��վ
		m_StationMap.Lookup(DeviceID, (CObject*&)pDevice);
		break;
	default:
		if(StationID == "")
		{
			//�����豸,�������г�վ�µĴ����豸����
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
			//���ҳ�վ
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
 �� �� ����BuildProtectDeviceRelation()
 ���ܸ�Ҫ��������վͨ��ԭ���
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
BOOL CDataEngine::BuildStationStatusConfig()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//�齨��ѯ����
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
			WriteLog("CDataEngine::BuildStationStatusConfig, ��ѯʧ��");
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
				str.Format("CDataEngine::BuildStationStatusConfig, ��ȡ��%d������", nCount);
				WriteLog(str);
			}
			for(int i = 0; i < nCount; i++)
			{
				//��������
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
			str.Format("CDataEngine::BuildStationStatusConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Clear()
 ���ܸ�Ҫ���������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA90158
void CDataEngine::Clear()
{
	POSITION pos = NULL;
	CDeviceObj * pObj = NULL;
	CString sID;

	//��վ
	ClearSepStationList();

	//��վ
	for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
	{
		m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_StationMap.RemoveAll();

	//����
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

	//���ͨ������ֵ
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

	//�����վͨ��ԭ���
	m_StationStatusConfigMap.RemoveAll();
}

/*************************************************************
 �� �� ����BuildUniversalActChr()
 ���ܸ�Ҫ������ͨ�õĶ���������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA90159
BOOL CDataEngine::BuildUniversalActChr()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		//������ƽ̨����DLL�����ӿ���
		
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		CString str;
		
		//�ֶ�
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

		//Notes(��λ)
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
				WriteLog("CCDataEngine::BuildUniversalActChr, ��ѯʧ��");
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
					str.Format("CDataEngine::BuildUniversalActChr, ��ȡ��%d������", nCount);
					WriteLog(str, 3);
				}
				if(nCount == 0)
				{
					CString str;
					str.Format("û��ͨ�ö�����������Ϣ");
					WriteLog(str);
				}
				for(int i = 0; i < nCount; i++)
				{
					//��������
					PT_ActionCharacter * sign = new PT_ActionCharacter;
					sign->strUnits = "";
					CString str;
					str = pMemset->GetValue((UINT)0); //action_id
					sign->nID = atoi(str);
					sign->strName = pMemset->GetValue(1); //name
					sign->strCodeName = pMemset->GetValue(2); //code_name
					str = pMemset->GetValue(3); //�¼�����
					sign->nEventType = atoi(str);
					sign->strUnits = pMemset->GetValue(4); //��λ

					//��λȥ�ո�
					sign->strUnits.TrimLeft();
					sign->strUnits.TrimRight();

					m_arrUniversalActChr.Add(sign);

					pMemset->MoveNext();
				}
			}
			else
			{	CString str;
				str.Format("CDataEngine::BuildUniversalActChr,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����FindDeviceBy103()
 ���ܸ�Ҫ������103�����Ŀ�Ų����豸
 �� �� ֵ: �豸ָ��
 ��    ����param1	103�����Ŀ��.��ʽ:���#��Ŀ��
		   Param2	�豸����
		   Param3	��վID
**************************************************************/
//##ModelId=49B87BA9016A
CDeviceObj * CDataEngine::FindDeviceBy103( CString str103, int DeviceType, CString strStaionID )
{
	//һ��Ҫָ����վID, ��Ϊ�п��ܲ�ͬ��վ����ͬ�������Ŀ��
	if(strStaionID == "")
		return NULL;
	//���ҳ�վ
	CStationObj* pStation = (CStationObj*)FindDevice(strStaionID, TYPE_STATION);
	if(pStation != NULL)
	{
		return pStation->FindDeviceBy103(str103, DeviceType);
	}
	
	return NULL;
}


/*
 *  @brief   	FindDeviceBy103	 ����uri�����豸 
 *  @param 		[In]a param of Type  CString uri
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	CDeviceObj * �豸ָ��
 */
	//##ModelId=49B87BA6035B
CDeviceObj * CDataEngine::FindDeviceByUri(CString strUri, CString strStaionID)
{
	//һ��Ҫָ����վID, ��Ϊ�п��ܲ�ͬ��վ����ͬ�������Ŀ��
	if(strStaionID == "")
		return NULL;
	//���ҳ�վ
	CStationObj* pStation = (CStationObj*)FindDevice(strStaionID, TYPE_STATION);
	if(pStation != NULL)
		return pStation->FindDeviceByUri(strUri);
	
	return NULL;
}

/*************************************************************
 �� �� ����GetFuncTimeOut()
 ���ܸ�Ҫ�������ݿ��ж�ȡ��ͨ�Ź��ܳ�ʱʱ��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA9015A
BOOL CDataEngine::GetFuncTimeOut()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
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
		WriteLog("CDataEngine::GetFuncTimeOut, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount ==0)
		{
			CString str;
			str.Format("CDataEngine::GetFuncTimeOut, û�г�ʱ����");
			WriteLog(str);
		}
		//��¼����
		if(nCount > 0)
		{
			int nOffset = 10;
			CString str;
			str = pMemset->GetValue((UINT)1);
			m_nPTSettingTimeOut = atoi(str) + nOffset; //�����ٻ���ֵ
			str = pMemset->GetValue(2);
			m_nWRSettingTimeOut = atoi(str) + nOffset; //¼�����ٻ���ֵ
			str = pMemset->GetValue(5);
			m_nPTAiTimeOut = atoi(str) + nOffset; //�ٻ�����ģ����
			str = pMemset->GetValue(6);
			m_nPTDiTimeOut = atoi(str) + nOffset; //�ٻ�����������
			str = pMemset->GetValue(7);
			m_nPTSoftBoardTimeOut = atoi(str) + nOffset; //�ٻ�������ѹ��
			str = pMemset->GetValue(8);
			m_nPTZoneTimeOut = atoi(str) + nOffset; //�ٻ�������ֵ����
			str = pMemset->GetValue(9);
			m_nDevTimeTimeOut = atoi(str) + nOffset; //�ٻ�װ��ʱ��
			str = pMemset->GetValue(10);
			m_nDevGeneralTimeOut = atoi(str) + nOffset; //�ٻ�װ�û�����Ϣ
			str = pMemset->GetValue(11);
			m_nModifySettingTimeOut = atoi(str) + nOffset; //��ֵ�޸�
			str = pMemset->GetValue(12);
			m_nChangeZoneTimeOut = atoi(str) + nOffset; //��ֵ���л�
			str = pMemset->GetValue(13);
			m_nChangeSBTimeOut = atoi(str) + nOffset; //��ѹ���л�
			str = pMemset->GetValue(14);
			m_nVerifyTimeTimeOut = atoi(str) + nOffset; //װ�ö�ʱ
			str = pMemset->GetValue(15);
			m_nSignalResetTimeOut = atoi(str) + nOffset; //�źŸ���
			str = pMemset->GetValue(16);
			m_nRemoteStartTimeOut = atoi(str) + nOffset; //Զ������
			str= pMemset->GetValue(17);
			m_nHistoryEventTimeOut = atoi(str) + nOffset; //��ʷ�¼�
			str = pMemset->GetValue(18);
			m_nWavFileTimeOut = atoi(str) + nOffset; //����¼���ļ�
			str = pMemset->GetValue(19);
			m_nWavFileListTimeOut = atoi(str) + nOffset;//�ٻ�¼���б�
			str = pMemset->GetValue(20);
			m_nCommFileTimeOut = atoi(str) + nOffset; //�ٻ�ͨ���ļ�
			str = pMemset->GetValue(21);
			m_nCommFileListTimeOut = atoi(str) + nOffset; //�ٻ�ͨ���ļ��б�
			str = pMemset->GetValue(27);
			m_nCommStatusTimeOut = atoi(str) + nOffset; //�ٻ�װ������״̬

		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::GetFuncTimeOut, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildCpuList()
 ���ܸ�Ҫ������CPU����
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA90149
BOOL CDataEngine::BuildCpuList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
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
		WriteLog("CDataEngine::BuildCpuList, ��ѯʧ��");
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
			str.Format("CDataEngine::BuildCpuList, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			CSecCPU * cpu = new CSecCPU;
			if(cpu->Load(pMemset))
			{
				//��ȡ�ɹ�
				CSecObj* pSec = (CSecObj*)FindDevice(cpu->PT_ID, TYPE_SEC);
				if(pSec == NULL)
				{
					//���Ҳ��������豸
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
		str.Format("CDataEngine::BuildCpuList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BuildZoneList()
 ���ܸ�Ҫ��������ֵ��������
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA9014A
BOOL CDataEngine::BuildZoneList()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
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
		WriteLog("CDataEngine::BuildZoneList, ��ѯʧ��");
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
			str.Format("CDataEngine::BuildZoneList, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			/*
			CString  PT_ID;//����ID
			int		cpu; //cpu��
			int		code; //��ֵ����
			CString zone_name;
			CString code_name;
			int     group;
			int     item;
			int     minValue;
			int     maxValue;
			int     stepValue;
			int     zoneValue;
			*/


			//����
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

			//���Ҷ����豸
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
				if (ptz != NULL && ptz->zone_name.Find("�༭") != -1)
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
		str.Format("CSecCPU::GetZone,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: LoadAllSec()
 ���ܸ�Ҫ: �������ж����豸,����ˢ���豸״̬
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDataEngine::LoadAllSec()
{
	//clear
	ClearRefreshSec();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
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
		WriteLog("CDataEngine::LoadAllSec, ��ѯʧ��");
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
			str.Format("CDataEngine::LoadAllSec, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CSecObj * pObj = new CSecObj;
			if(pObj->Load(pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				m_AllSEC.AddTail(pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::LoadAllSec,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 �� �� ��: RefreshDevStatus()
 ���ܸ�Ҫ: ˢ���豸״̬
 �� �� ֵ: 
 ��    ��: param1 
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
		//�����ڳ����ڲ���Ӧ���豸
		CStationObj* pStation = (CStationObj*)FindDevice(pSec->m_sStationID, TYPE_STATION);
		if(pStation == NULL)
			continue;
		CSecObj* pObj = (CSecObj*)pStation->FindDevice(pSec->m_sID, pSec->m_nType);
		if(pObj == NULL)
			continue;
		//ˢ���豸״̬
		pObj->RefreshStatus(pSec);
	}
}

/*************************************************************
 �� �� ��: QueryStationStatus()
 ���ܸ�Ҫ: ��ѯ��վ״̬
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDataEngine::QueryStationStatus()
{
	//�齨��ѯ���
	CString strSQL;
	//------------------------
	strSQL.Format("select * from tb_pt_station_status A JOIN (select station_id,Reserve1,max(Time) Time from tb_pt_station_status  group by station_id,Reserve1) B ON A.station_id = B.station_id and A.Time = B.Time");
//	strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryStationStatus, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryStationStatus, ��ȡ��%d������", nCount);
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
			
			//���Ҷ�Ӧ�ĳ�վ
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
		str.Format("CDataEngine::QueryStationStatus,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 CreateDate: 19:1:2010     Author:LYH
 �� �� ��: QueryHasSettingConfig()
 ���ܸ�Ҫ: ��ѯ�������豸�Ƿ��ж�ֵ����
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasSettingConfig()
{
	//�齨��ѯ���
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_setting_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryHasSettingConfig, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryHasSettingConfig, ��ȡ��%d������", nCount);
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
				//���Ҷ�Ӧ�Ķ����豸
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//����CPU
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
		str.Format("CDataEngine::QueryHasSettingConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: QueryHasDigitalConfig()
 ���ܸ�Ҫ: ��ѯ�������豸�Ƿ��п���������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasDigitalConfig()
{
	//�齨��ѯ���
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_di_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryHasDigitalConfig, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryHasDigitalConfig, ��ȡ��%d������", nCount);
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
				//���Ҷ�Ӧ�Ķ����豸
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//����CPU
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
		str.Format("CDataEngine::QueryHasDigitalConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: QueryHasSoftBoardConfig()
 ���ܸ�Ҫ: ��ѯ�������豸�Ƿ�����ѹ������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasSoftBoardConfig()
{
	//�齨��ѯ���
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_softboard_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryHasSoftBoardConfig, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryHasSoftBoardConfig, ��ȡ��%d������", nCount);
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
				//���Ҷ�Ӧ�Ķ����豸
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//����CPU
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
		str.Format("CDataEngine::QueryHasSoftBoardConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: QueryHasAnalogConfig()
 ���ܸ�Ҫ: ��ѯ�������豸�Ƿ���ģ��������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CDataEngine::QueryHasAnalogConfig()
{
	//�齨��ѯ���
	CString strSQL;
	strSQL.Format("SELECT b.station_id,a.pt_id,a.cpu_code,COUNT(*) FROM tb_pt_ai_def a INNER JOIN tb_secdev_base b ON a.pt_id=b.pt_id GROUP BY b.station_id,a.pt_id,a.cpu_code ORDER BY b.station_id");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryHasAnalogConfig, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryHasAnalogConfig, ��ȡ��%d������", nCount);
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
				//���Ҷ�Ӧ�Ķ����豸
				CSecObj* pObj = (CSecObj*)FindDevice(strPTID, TYPE_SEC, strStation);
				if(pObj != NULL)
				{
					//����CPU
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
		str.Format("CDataEngine::QueryHasAnalogConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
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
	//�齨��ѯ����
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
		WriteLog("CDataEngine::Load20162Config, ��ѯʧ��");
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
������:    BuildZoneListOld
����ֵ:   BOOL
���ܸ�Ҫ:  ������ֵ��������,��tb_secdev_zone_base��ȡ����
************************************/
BOOL CDataEngine::BuildZoneListOld()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
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
		WriteLog("CDataEngine::BuildZoneList, ��ѯʧ��");
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
			str.Format("CDataEngine::BuildZoneList, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			PT_ZONE * ptz = new PT_ZONE;
			CString str;
			ptz->PT_ID = pMemset->GetValue((UINT)0); //PT_ID
			str = pMemset->GetValue(1); //CPU_Code
			ptz->cpu = atoi(str);
			str = pMemset->GetValue(2);//Zone_CODE
			ptz->code = atoi(str);
			str = pMemset->GetValue(3);//Zone_State
			ptz->state = atoi(str);
			
			//���Ҷ����豸
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
		str.Format("CSecCPU::GetZone,��ѯʧ��,ԭ��Ϊ%s", sError);
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
������:    GetPriDeviceList
����ֵ:   void
���ܸ�Ҫ: ȡ��һ���豸���� 
����: DEVICE_LIST & listDevice ���ڱ���һ���豸������
����: int nDeviceType ָ��Ҫ���ҵ�һ���豸���ͣ�-1��ʾ��������
����: CString sStationID ָ��������վID��Ϊ��ʱ��ʾ���г�վ
************************************/
void CDataEngine::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType, CString sStationID/*=""*/ )
{
	CStationObj* pStation = NULL;
	CString sID;
	POSITION pos = NULL;
	if(sStationID.IsEmpty())
	{
		//�������г�վ
		for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
		{
			m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
			if(pStation != NULL)
				pStation->GetPriDeviceList(listDevice, nDeviceType);
		}
	}
	else
	{
		//���ҵ���Ӧ��վ
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetPriDeviceList(listDevice, nDeviceType);
	}
}

/************************************
Date:2011/7/21  Author:LYH
������:    GetSecList
����ֵ:   void
���ܸ�Ҫ: ȡ�ö����豸����
����: DEVICE_LIST & listDevice ���ڱ�������豸������
����: CString sPriID ָ��һ���豸��Ϊ��ʱ��ʾ����
����: int nSecType ָ��Ҫ���ҵĶ����豸���ͣ�-1��TYPE_SEC��ʾ��������
����: CString sStationID ָ��������վID��Ϊ��ʱ��ʾ���г�վ
************************************/
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, CString sPriID /*= ""*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, int nDetailKind/* = -1 */ )
{
	CStationObj* pStation = NULL;
	CString sID;
	POSITION pos = NULL;
	if(sStationID.IsEmpty())
	{
		//�������г�վ
		for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
		{
			m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pStation);
			if(pStation != NULL)
				pStation->GetSecList(listDevice, sPriID, nSecType, nDetailKind);
		}
	}
	else
	{
		//���ҵ���Ӧ��վ
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, sPriID, nSecType, nDetailKind);
	}
}

/************************************
Date:2011/7/21  Author:LYH
������:    GetStationList
����ֵ:   void
���ܸ�Ҫ: ȡ�ó�վ����
����: DEVICE_LIST & listDevice ���ڱ��泧վ������
����: CString sNetID ָ������ID��Ϊ��ʱ��ʾ���е���
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
������:   BuildSepStationList	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CDataEngine::BuildSepStationList()
{
	ClearSepStationList();
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����
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
		WriteLog("CDataEngine::BuildSepStationList, ��ѯʧ��");
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
			str.Format("CDataEngine::BuildSepStationList, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CStationObj * pObj = new CStationObj;
			if(pObj->Load(pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				m_SepStationMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::BuildSepStationList,��ѯʧ��,ԭ��Ϊ%s", sError);
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
������:   FindSepByStation	
����ֵ:   CDeviceObj*	
���ܸ�Ҫ: 
����: CString sStationID	
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
������:   FindSepStation	
����ֵ:   CDeviceObj*	
���ܸ�Ҫ: 
����: CString sSepID	
*****************************************************/
CDeviceObj* CDataEngine::FindSepStation( CString sSepID )
{
	CStationObj* pObj = NULL;
	m_SepStationMap.Lookup(sSepID, (CObject*&)pObj);
	return pObj;
}

/****************************************************
Date:2012/11/2  Author:LY
		 ������:   LoadManagedepartmentfromDB	
		 ����ֵ:   BOOL	
		 ���ܸ�Ҫ: ��ѯ�������ܵ�λ
		 ����: 
*****************************************************/
BOOL CDataEngine::LoadManagedepartmentfromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)
	
	//�����ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//��������
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
		WriteLog("CDataEngine::LoadManagedepartmentfromDB, ��ѯʧ��");
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
			//ѭ������
			
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
		str.Format("CDataEngine::LoadManagedepartmentfromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
		 ������:   LoadManagedepartmentfromDB	
		 ����ֵ:   BOOL	
		 ���ܸ�Ҫ: ��ѯ�������ܵ�λ
		 ����: 
*****************************************************/
BOOL CDataEngine::LoadMaintaindepartmentfromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)
	
	//�����ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//��������
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
		WriteLog("CDataEngine::LoadMaintaindepartmentfromDB, ��ѯʧ��");
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
			//ѭ������
			
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
		str.Format("CDataEngine::LoadMaintaindepartmentfromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
	 ������:   LoadMarkInfofromDB	
	 ����ֵ:   BOOL	
	 ���ܸ�Ҫ: ��ѯ�������ܵ�λ
	 ����: 
	 *****************************************************/
BOOL CDataEngine::LoadMarkInfofromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)
	
	//�����ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "note", EX_STTP_DATA_TYPE_STRING);
	
	
	//��������
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
		WriteLog("CDataEngine::LoadMarkInfofromDB, ��ѯʧ��");
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
				//��������
				CMark * pObj = new CMark;
				if(pObj->Load(pMemset))
				{
					//��ȡ���ݳɹ�,���뵽����
					m_arrMark.AddTail(pObj);
				}
				else
				{
					//��ȡ����ʧ��, delete
					delete pObj;
					pObj = NULL;
				}
				pMemset->MoveNext();	
		}
		
	}
	else
	{
		str.Format("CDataEngine::LoadMarkInfofromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
������:   LoadServiceDepartmentFromDB	
����ֵ:   BOOL	
���ܸ�Ҫ: ��ѯ���޵�λ,�������޵�λMAP��
*****************************************************/
BOOL CDataEngine::LoadServiceDepartmentFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�����ֶ�
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
		WriteLog("CDataEngine::LoadServiceDepartmentFromDB, ��ѯʧ��");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			//��������
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
		str.Format("CDataEngine::LoadServiceDepartmentFromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	sError = NULL;
	return bReturn;
}

/****************************************************
Date:2012/12/11  Author:LYH
������:   GetServiceDepartmentName	
����ֵ:   CString	ȡ�ü��޵�λ����
���ܸ�Ҫ: ȡ�ü��޵�λ����
����: int nID	���޵�λID
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
������:   ClearNetList	
����ֵ:   void	
���ܸ�Ҫ: ��յ�������
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
������:   ClearSepStationList	
����ֵ:   void	
���ܸ�Ҫ: ��շ�վ����
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
������:   StationInit	
����ֵ:   CStationObj*	��վ����
���ܸ�Ҫ: ��վ��ʼ��. ���nType==STATIONINIT_DELETE,������ֻ��ָ����վ�Ƴ�������������ɾ������������������ú�������ɾ��
����: CString sStationID ��վID	
����: int nType	��ʼ������
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
������:   LoadStationFromDB	
����ֵ:   CStationObj*	��վ����
���ܸ�Ҫ: �����ݿ�����ָ����վ����Ϣ
����: CString sStationID	ָ��Ҫ����ĳ�վID
����: CStationObj * pStation	���泧վ��Ϣ�Ķ������ΪNULL���������½���վ�����ڷ���ֵ�з���
*****************************************************/
CStationObj* CDataEngine::LoadStationFromDB( CString sStationID, CStationObj* pStation )
{
	CStationObj* pNew = pStation;
	if(sStationID.IsEmpty())
		return pNew;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����
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
		WriteLog("CDataEngine::BuildStationList, ��ѯʧ��");
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
			//��������
			if(pNew == NULL)
				pNew = new CStationObj;
			if(pNew->Load(pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				if(pStation == NULL)
					m_StationMap.SetAt(pNew->m_sID, pNew);
				pNew->InitLock();
				//pNew->LoadData();
				pNew->BuildData();
			}
			else
			{
				//��ȡ����ʧ��, delete
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
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�����ֶ�
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
		WriteLog("CDataEngine::LoadSituationAndDisposeFromDB, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			//��������
			int codetype = atoi( pMemset.GetValue((UINT)1));
			switch( codetype )
			{
			case 1: //���˵��
				{
					int id = atoi( pMemset.GetValue((UINT)0));
					CString name = pMemset.GetValue((UINT)2);
					m_arrSituation.InsertAt( id, name );
				}
				break;
			case 2: //�������
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
		str.Format("CDataEngine::LoadSituationAndDisposeFromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	return bReturn;
}

BOOL CDataEngine::QueryPTNoteCount()
{
	//�齨��ѯ���
	CString strSQL;
	strSQL.Format("select pt_id, count(*) from TB_PT_NOTE_DATA group by pt_id");
	WriteLog(strSQL);
	
	//���в�ѯ
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
		WriteLog("CDataEngine::QueryPTNoteCount, ��ѯʧ��");
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
			str.Format("CDataEngine::QueryPTNoteCount, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString strPTID = pMemset->GetValue((UINT)0);
			int count = atoi(pMemset->GetValue(1));
				//���Ҷ�Ӧ�Ķ����豸
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
		str.Format("CDataEngine::QueryPTNoteCount,��ѯʧ��,ԭ��Ϊ%s", sError);
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
	
	WriteLogEx("CDataEngine::QueryPTNotes, ��ʼ��ѯ");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ȡ�����ݿ�������ʷ���ݵ�Time�б�
	CString str;
	
	//�ֶ�
	
	//����
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", ptid);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//��CURTIME��С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by UPDATE_TIME DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
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
		WriteLogEx("CDataEngine::QueryPTNotes, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CDataEngine::QueryPTNotes,��ѯ��%d��", nCount);
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
		str.Format("CDataEngine::QueryPTNotes,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	if(!bReturn)
		return FALSE;
	
	WriteLogEx("CDataEngine::QueryPTNotes, ��ѯ����");
	
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
	//ð������
	
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
			//2017-6-22 ����ʱ����ȥ����ѹ�ȼ���Ϣ
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
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����
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
		WriteLog("CDataEngine::RefreshStationStatus, ��ѯʧ��");
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
			str.Format("CDataEngine::RefreshStationStatus, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CString id = pMemset->GetValue(int(1)); //��վID
			CStationObj* pStation = NULL;
			m_StationMap.Lookup(id, (CObject *&)pStation);
			if( pStation != NULL ){
				//ͨ��0
				CString str = pMemset->GetRealDataValue("ch0_commu_status");
				int status = atoi(str);
				str = pMemset->GetRealDataValue("ch0_commustatus_time");
				CTime tm = StringToTime( str);
				str = pMemset->GetRealDataValue("ch0_commu_reason");
				int reason = atoi(str);
				pStation->SetStatus(status, tm, reason, 0, tm, pStation->m_nIPFlag);
				
				//ͨ��1
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
		str.Format("CDataEngine::RefreshStationStatus,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}
