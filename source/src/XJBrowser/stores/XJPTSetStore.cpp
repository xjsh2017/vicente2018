// DBEngine.cpp : implementation file
//

#include "stdafx.h"

#include "XJBrowser.h"

#include "XJPTSetStore.h"
#include "core/qptsetcard.h"


class CXJPTSetStorePrivate 
{
public:
	CXJPTSetStorePrivate() {}
    ~CXJPTSetStorePrivate() {}
	
	CPTSetCard		m_card;
	CMemTable		m_log;
};

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

CXJPTSetStore *CXJPTSetStore::m_pInstance = NULL;

const char* PTSET_KEYNAME = "PTSET_STATE";

CXJPTSetStore *CXJPTSetStore::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CXJPTSetStore;
		m_pInstance->Register(m_pInstance);
	}
	return m_pInstance;
}

void CXJPTSetStore::ReleaseInstance()
{
	m_pInstance->UnRegister(m_pInstance);
	DELETE_POINTER(m_pInstance);
}

CXJPTSetStore::CXJPTSetStore()
	: d_ptr(new CXJPTSetStorePrivate)
{
}

CXJPTSetStore::~CXJPTSetStore()
{
	DELETE_POINTER(d_ptr);
}

/****************************************************
Date:2011/12/20  Author:LYH
������:   AddNewManOperator	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString FunID	
����: CString Act	
����: CString strMsg	
����: CString strUser	
����: int nOperType	
����: int nOperResult	
*****************************************************/
void CXJPTSetStore::AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult , int num)
{

}
void CXJPTSetStore::AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult /*= 0*/, int num /*= -1*/ )
{
	AddNewManOperator(FunID, Act, strMsg, "", nOperType, nOperResult, num);
}

CXJPTSetStore* CXJPTSetStore::GetStore()
{
	GetInstance();
	if (CheckStore() < 0 || NULL == d_ptr){
		return NULL;
	}

	CXJPTSetStore *pReturn = this;
	CPTSetCard &card = d_ptr->m_card;
	CMemTable &log = d_ptr->m_log;

	// Get 
    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	CString str;

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//value
	Field fld0;
	dbEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	//reverse1
	Field fld1;
	dbEngine.SetField(sql, fld1, "reverse3", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//USER_ID
	Condition cond0;
	str.Format("keyname = '%s'", PTSET_KEYNAME);
	dbEngine.SetCondition(sql, cond0, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = dbEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult){
			return NULL;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::GetPTSetModState, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		return NULL;
	}

	// ��ѯ�ɹ�
	if(NULL != pMemset)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJBrowserApp::GetPTSetModState, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		char * val = NULL;
		if(nCount > 0)
		{
			val = pMemset->GetValue((UINT)0);
			card.FRead(val, strlen(val));

			val = pMemset->GetValue((UINT)1);
			log.FRead(val, strlen(val));
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::GetPTSetModState, ��ѯʧ��, ԭ��Ϊ��%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);

		return NULL;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return pReturn;
}

BOOL CXJPTSetStore::SaveStore()
{
	BOOL bReturn = FALSE;

	if (NULL == d_ptr)
		return FALSE;

	CPTSetCard &card = d_ptr->m_card;
	CMemTable &log = d_ptr->m_log;

    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	CString str;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	// 
	//str.Format("%s", m_pStoreData->GetValue());
	//AfxMessageBox(str);
	
	CTime tm;
	CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	
    // ���¶�ֵ�޸�״̬��
	// �齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	// value
	Field fld0;
	str.Format("%s", card.GetValue());
	dbEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING, str);
	// reverse3
	Field fld1;
	str.Format("%s", log.GetValue());
	dbEngine.SetField(sql, fld1, "reverse3", EX_STTP_DATA_TYPE_STRING, str);
	
	//����
	//keyname
	Condition cond0;
	str.Format("keyname = '%s'", PTSET_KEYNAME);
	dbEngine.SetCondition(sql, cond0, str);
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult; 
	try
	{
		nResult = dbEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);
		
		if(1 == nResult){
			bReturn = TRUE;
		}else{
			str.Format("CXJBrowserApp::NextPTSetModState, ����ʧ��, ԭ��Ϊ��%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			
			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::NextPTSetModState, ����ʧ��");
		AfxMessageBox(sError);

		DELETE_POINTERS(sError);
		
		return FALSE;
	}
	
	DELETE_POINTERS(sError);

	return bReturn;
}

int CXJPTSetStore::CheckStore()
{
	int nReturn = -1;

    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//value
	Field fld0;
	pApp->m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//keyname
	Condition condition1;
	str.Format("keyname = '%s'", PTSET_KEYNAME);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult)
		{
			nReturn = -1;
			//AfxMessageBox("CXJPTSetStore::CheckStore, ��ѯʧ��");
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJPTSetStore::CheckStore, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		sError = NULL;

		nReturn = -1;
	}

	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CXJPTSetStore::CheckStore, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
			return 0;

		// �����ڼ�¼ DZ_MOD_STATE
		//�齨��ѯ����
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//�����ֶ�
		//����״̬�ؼ���
		str = "DZ_MOD_STATE";
		Field fld1;
		pApp->m_DBEngine.SetField(sql1, fld1, "keyname", EX_STTP_DATA_TYPE_STRING, str);
		Field fld2;
		pApp->m_DBEngine.SetField(sql1, fld2, "value", EX_STTP_DATA_TYPE_STRING, "");
		
		memset(sError, '\0', MAXMSGLEN);
		
		//int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_TBSYSCONFIG, sql1, sError);

			if(nResult == 1)
			{
				//�ɹ�
				nReturn = 11;
			}
		}
		catch (...)
		{
			WriteLog("CXJPTSetStore::CheckStore, ����ʧ��");
			delete[] sError;
			sError = NULL;
			
			nReturn = 10;
		}
	}
	else
	{
		CString str;
		str.Format("CXJPTSetStore::CheckStore, ��ѯʧ��, ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);

		//AfxMessageBox(str);
		nReturn = -1;	
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}
