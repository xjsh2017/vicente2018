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

#include "DataEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)//����4786����

/////////////////////////////////////////////////////////////////////////////
// CDataEngine
//##ModelId=49B87BA9003F
/****************************************************
Date:2011/10/17  Author:LYH
������:   CDataEngine	
����ֵ:   	
���ܸ�Ҫ: 
����: CDBEngine * pDBEngine	
*****************************************************/
CDataEngine::CDataEngine( CDBEngine* pDBEngine )
{
	m_pDBEngine = pDBEngine;
	m_sDownDir = "";
	m_sLogDir = "";

	m_NetList.RemoveAll();		//��������
	m_StationMap.RemoveAll();	//��վ����
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
 �� �� ����BuildNetList()
 ���ܸ�Ҫ����ȡ���ݿ��е�������, ���ɵ�������, ������������
 �� �� ֵ: �����ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA900AC
BOOL CDataEngine::BuildNetList()
{
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_NET_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//����
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
					if(m_nLoadDataModel == 1)
					{
						pObj->InitLock();
						pObj->LoadData(m_pDBEngine);
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
	MYASSERT_BOOL(m_pDBEngine);

	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BAY_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BREAKER_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_BUS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_CAPACITOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_GEN_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

		//�齨��ѯ����
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;

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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_PMS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_REACTOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_SWITCH_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult =m_pDBEngine->XJSelectData(EX_STTP_INFO_TRANS_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
		//�齨��ѯ����
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

		//����
		//�豸����
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
			WriteLog("CDataEngine::BuildSecList, ��ѯʧ��");
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
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	
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
	LONGLONG startTime = 0;
	//�������վ��ɫ������ص����б�
	if (m_bLoadNet)
	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("�����������...");
		startTime = StartCount();
		//���������, ��վ, ������һ���豸, ����������豸, ������뱣�������
		if(!BuildNetList())
		{
			WriteLog("����������ݳ���!", XJ_LOG_LV1);
		}
		EndCount("�����������ʱ��", startTime);
	}
	
	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("���볧վ����...");
	startTime = StartCount();
	if(!BuildStationList())
	{
		WriteLog("���볧վ���ݳ���!", XJ_LOG_LV1);
	}
	EndCount("���볧վ����ʱ��", startTime);

	if(m_nLoadDataModel == 0)
	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("����������...");
		startTime = StartCount();
		if(!BuildBayList())
		{
			WriteLog("���������ݳ���!", XJ_LOG_LV1);
		}
		EndCount("����������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("���뿪������...");
		startTime = StartCount();
		if(!BuildBreakList())
		{
			WriteLog("���뿪�����ݳ���!", XJ_LOG_LV1);
		}
		EndCount("���뿪������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("����ĸ������...");
		startTime = StartCount();
		if(!BuildBusList())
		{
			WriteLog("����ĸ�����ݳ���!", XJ_LOG_LV1);
		}
		EndCount("����ĸ������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("�������������...");
		startTime = StartCount();
		if(!BuildCapList())
		{
			WriteLog("������������ݳ���!", XJ_LOG_LV1);
		}
		EndCount("�������������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("���뷢�������...");
		startTime = StartCount();
		if(!BuildGenList())
		{
			WriteLog("���뷢������ݳ���!", XJ_LOG_LV1);
		}
		EndCount("���뷢�������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("������·����...");
		startTime = StartCount();
		if(!BuildLineList())
		{
			WriteLog("����·�����ݳ���!", XJ_LOG_LV1);
		}
		EndCount("����·������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("����翹������...");
		startTime = StartCount();
		if(!BuildReactorList())
		{
			WriteLog("����翹�����ݳ���!", XJ_LOG_LV1);
		}
		EndCount("����翹������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("���뵶բ����...");
		startTime = StartCount();
		if(!BuildSwitchList())
		{
			WriteLog("���뵶բ���ݳ���!", XJ_LOG_LV1);
		}
		EndCount("���뵶բ����ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("�����ѹ������...");
		startTime = StartCount();
		if(!BuildTransList())
		{
			WriteLog("�����ѹ�����ݳ���!", XJ_LOG_LV1);
		}
		EndCount("�����ѹ������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("���뱣�����������...");
		startTime = StartCount();
		if(!BuildPmsList())
		{
			WriteLog("���뱣����������ݳ���!", XJ_LOG_LV1);
		}
		EndCount("���뱣�����������ʱ��", startTime);
		
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("��������豸����...");
		startTime = StartCount();
		if(!BuildSecList())
		{
			WriteLog("��������豸���ݳ���!", XJ_LOG_LV1);
		}
		EndCount("��������豸����ʱ��", startTime);
		
		if(m_bLoadCPUInfo)
		{
			if(m_pSplashWnd != NULL)
				m_pSplashWnd->AddDetailString("����CPU����...");
			startTime = StartCount();
			if(!BuildCpuList())
			{
				WriteLog("����CPU���ݳ���!", XJ_LOG_LV1);
			}
			EndCount("����CPU����ʱ��", startTime);
		}
		
		if(m_bLoadZoneInfo)
		{
			if(m_pSplashWnd != NULL)
				m_pSplashWnd->AddDetailString("���붨ֵ��������...");
			startTime = StartCount();
			if(!BuildZoneList())
			{
				WriteLog("���붨ֵ���ų���!", XJ_LOG_LV1);
			}
		
			EndCount("���붨ֵ��������ʱ��", startTime);
		}
	}

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("����ͨ������������...");
	startTime = StartCount();
	if(!BuildUniversalActChr())
	{
		WriteLog("����ͨ�����������ݳ���", XJ_LOG_LV1);
	}
	EndCount("����ͨ������������ʱ��", startTime);

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("���볧վͨ��ԭ������...");
	if(!BuildStationStatusConfig())
	{
		WriteLog("���볧վͨ��ԭ�����ݳ���", XJ_LOG_LV1);
	}	

	if(m_pSplashWnd != NULL)
		m_pSplashWnd->AddDetailString("����ͨѶ��ʱ����...");
	GetFuncTimeOut();
	WriteLog("���빦��ͨ�ų�ʱ�������", XJ_LOG_LV1);

	if(m_bLoad20162Config)
 	{
		if(m_pSplashWnd != NULL)
			m_pSplashWnd->AddDetailString("����20162��������...");
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
			nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_STATION_STATUS_CONFIG, sql, sError, pMemset);
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
	for(pos = m_StationMap.GetStartPosition(); pos != NULL; )
	{
		m_StationMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_StationMap.RemoveAll();

	//����
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

	//���ͨ������ֵ
	for(int i = 0; i < m_arrUniversalActChr.GetSize(); i++)
	{
		delete m_arrUniversalActChr[i];
	}
	m_arrUniversalActChr.RemoveAll();


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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
				nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_CHARCTOR_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
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
		nResult = m_pDBEngine->XJSelectData(EX_STTP_INFO_FUN_TIMEOUT_CFG, sql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//�齨��ѯ����
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
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//�齨��ѯ����
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
	MYASSERT(m_pDBEngine);
	//clear
	ClearRefreshSec();

	//�齨��ѯ����
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
	MYASSERT(m_pDBEngine);
	//�齨��ѯ���
	CString strSQL;
	//------------------------
	strSQL.Format("select * from tb_pt_station_status A JOIN (select station_id,Reserve1,max(Time) Time from tb_pt_station_status  group by station_id,Reserve1) B ON A.station_id = B.station_id and A.Time = B.Time");
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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

	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT_BOOL(m_pDBEngine);
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
	
	int nResult;
	try
	{
		nResult = m_pDBEngine->XJExecuteSql(MutiSql, sError, pMemset);
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
	MYASSERT(m_pDBEngine);
	//�齨��ѯ����
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
		WriteLog("CDataEngine::Load20162Config, ��ѯʧ��");
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
������:    BuildZoneListOld
����ֵ:   BOOL
���ܸ�Ҫ:  ������ֵ��������,��tb_secdev_zone_base��ȡ����
************************************/
BOOL CDataEngine::BuildZoneListOld()
{
	MYASSERT_BOOL(m_pDBEngine);
	BOOL bReturn = TRUE;
	
	//�齨��ѯ����
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
void CDataEngine::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType, CString sStationID/*=""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetPriDeviceList(listDevice, nDeviceType);
			}
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
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, CString sPriID /*= ""*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetSecList(listDevice, sPriID, nSecType);
			}
		}
	}
	else
	{
		//���ҵ���Ӧ��վ
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, sPriID, nSecType);
	}
}

/****************************************************
Date:2011/12/30  Author:LYH
������:   GetSecList	
����ֵ:   void	
���ܸ�Ҫ: 
����: DEVICE_LIST & listDevice	
����: int nPriType	
����: int nSecType	
����: CString sStaionID	
����: CString sNetID	
*****************************************************/
void CDataEngine::GetSecList( DEVICE_LIST& listDevice, int nPriType /*= 0*/, int nSecType /*= TYPE_SEC*/, CString sStationID /*= ""*/, CString sNetID /*= ""*/ )
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
			{
				if(sNetID.IsEmpty() || pStation->m_sNetID == sNetID)
					pStation->GetSecList(listDevice, nPriType, nSecType);
			}
		}
	}
	else
	{
		//���ҵ���Ӧ��վ
		pStation = (CStationObj*)FindDevice(sStationID, TYPE_STATION);
		if(pStation != NULL)
			pStation->GetSecList(listDevice, nPriType, nSecType);
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
Date:2011/10/17  Author:LYH
������:   GetDownDir	
����ֵ:   CString	
���ܸ�Ҫ: 
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
������:   LoadDownDir	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CDataEngine::LoadDownDir()
{
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//��ѯcomtrade_path
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
		WriteLog("CXJBrowserApp::LoadDownDir, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sDownDir = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("CXJBrowserApp::LoadDownDir, ·��Ϊ:%s", m_sDownDir);
			WriteLog(str);
		}
		else
		{
			WriteLog("�Ҳ���ϵͳ����·��");
		}

	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadDownDir, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if(m_sDownDir != "")
	{
		//���п��ܵ�/������"\\"
		m_sDownDir.Replace("/", "\\");
		if(m_sDownDir.Right(1)!="\\")
			m_sDownDir += "\\";
	}
}

/****************************************************
Date:2011/10/19  Author:LYH
������:   SetSecCommStatus	
����ֵ:   void	
���ܸ�Ҫ: ���ö����豸ͨ��״̬
����: CSecObj * pSec	ָ�������豸
����: int nStatus	ָ��״̬
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
������:   SetSecRunStatus	
����ֵ:   void	
���ܸ�Ҫ: ���ö����豸����״̬
����: CSecObj * pSec	ָ�������豸
����: int nStatus	ָ��״̬
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
������:   GetSysLogPath	
����ֵ:   CString	��־�ļ�·��
���ܸ�Ҫ: ȡ����־�ļ�·��
*****************************************************/
CString CDataEngine::LoadSysLogPath()
{
	CString sReturn = "";
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//��ѯcomtrade_path
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
		WriteLog("CXJBrowserApp::LoadSysLogPath, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			sReturn = pMemset->GetValue((UINT)0);
		}
		else
		{
			WriteLog("�Ҳ���ϵͳ����·��");
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
������:   GetSysLogPath	
����ֵ:   CString	��־�ļ�·��
���ܸ�Ҫ: ȡ����־�ļ�·��
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
		//���п��ܵ�"\\"������/
		sReturn.Replace("\\", "/");
		if(sReturn.Right(1)!="/")
			sReturn += "/";
		//����ģ����
		sReturn += sProjectName;
		sReturn += "/";
	}
	return sReturn;
}

