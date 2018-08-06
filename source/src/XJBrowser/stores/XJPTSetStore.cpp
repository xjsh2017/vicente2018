// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJPTSetStore.h"

#include "XJBrowser.h"
#include "qptsetstatetable.h"

////////////////////////////////////////////////////////////

class QPTSetDataTable : public QMemTable
{
public:
	QPTSetDataTable();
	~QPTSetDataTable();
	
public:	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);

};

class CXJPTSetStorePrivate 
{
public:
	CXJPTSetStorePrivate();
    ~CXJPTSetStorePrivate();
	
	/** @brief           ����״̬��*/
	QPTSetStateTable	m_state;

	/** @brief           ��ֵ�޸����ݱ�*/
	QPTSetDataTable		m_data_PTSet;

	BOOL		ReLoadState();
	int			CheckState();
	
public:	
	BOOL		Save(const char *pszFilePath = NULL);

	BOOL		ReloadData();
};

////////////////////////////////////////////////////////////
// QPTSetDataTable
//
QPTSetDataTable::QPTSetDataTable()
{
}

QPTSetDataTable::~QPTSetDataTable()
{
	
}

BOOL QPTSetDataTable::ReLoad()
{
	BOOL bReturn = FALSE;
	
	LoadInfo("tb_pt_setting_def");
	//LoadDataAll();

	//if (!m_card.GetPTID().isEmpty()){
	if (1){
		QByteArray baSQL;
		baSQL << "SELECT * FROM tb_pt_setting_def WHERE pt_id IN ('" 
			<< "����BH51"
			//<< m_card.GetPTID()
			<< "')";
		LoadData(baSQL);
		Save("c:/tb_pt_setting_def.txt");
	}
	
	return bReturn;
}

BOOL QPTSetDataTable::Save(const char *pszFilePath)
{
	BOOL bReturn = FALSE;
	
	QByteArrayMatrix keyVals;
	keyVals << "����BH51" << keyVals.GetDelimCol()
		<< 2 << keyVals.GetDelimCol()
		<< 1010;
	SetFieldValue(keyVals, "reserve3", "20,19.8");
	
	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);
	
	return bReturn;
}


////////////////////////////////////////////////////////////
// CXJPTSetStorePrivate
//
CXJPTSetStorePrivate::CXJPTSetStorePrivate()
{
}

CXJPTSetStorePrivate::~CXJPTSetStorePrivate()
{
}

BOOL CXJPTSetStorePrivate::ReLoadState()
{
	BOOL bReturn = FALSE;

	return m_state.ReLoad();
}

BOOL CXJPTSetStorePrivate::Save(const char *pszFilePath/* = NULL*/)
{
	return m_data_PTSet.Save(pszFilePath);
}

int CXJPTSetStorePrivate::CheckState()
{
	int nReturn = -1;

    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	QByteArrayMatrix& flow = m_state.GetWorkFlow();

	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//value
	Field fld0;
	dbEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//keyname
	Condition condition1;
	str.Format("keyname = '%s'", PTSET_KEYNAME);
	dbEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = dbEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
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

		// �����ڼ�¼ PTSET_KEYNAME
		//�齨��ѯ����
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//�����ֶ�
		//����״̬�ؼ���
		Field fld0;
		dbEngine.SetField(sql1, fld0, "keyname", EX_STTP_DATA_TYPE_STRING, PTSET_KEYNAME);
		Field fld1;
		str.Format("0,%d,,0,0,0", XJ_OPER_UNHANGOUT);
		dbEngine.SetField(sql1, fld1, "value", EX_STTP_DATA_TYPE_STRING, str);
		Field fld2;
		dbEngine.SetField(sql1, fld2, "reverse1", EX_STTP_DATA_TYPE_STRING, m_state.GetDefaultWorkFlow().constData());
		
		memset(sError, '\0', MAXMSGLEN);
		
		//int nResult;
		try
		{
			nResult = dbEngine.XJInsertData(EX_STTP_INFO_TBSYSCONFIG, sql1, sError);

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


/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore
//
CXJPTSetStore *CXJPTSetStore::m_pInstance = NULL;

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

BOOL CXJPTSetStore::ReLoadState()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->ReLoadState();
}

BOOL CXJPTSetStore::Save()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->m_state.Save();
}

BOOL CXJPTSetStore::Save(const char *pszFilePath/* = NULL*/)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->Save(pszFilePath);
}

int CXJPTSetStore::Check()
{
	if (NULL == d_ptr)
		return -2;
	
	return d_ptr->CheckState();
}

QPTSetStateTable* CXJPTSetStore::GetState()
{
	if (NULL == d_ptr)
		return NULL;
	
	return &(d_ptr->m_state);
}


BOOL CXJPTSetStore::RevertModify()
{
	BOOL bReturn = FALSE;

	return bReturn;
}

void CXJPTSetStore::AddNewManOperator(int nStateID, const char* szTime, CString sUserID)
{
	if(NULL == d_ptr)
		return;

	CString FunID = GetFuncID(nStateID);
	CString Act = d_ptr->m_state.GetPTID().constData();
	CString strTime = szTime;
	CString strMsg;

	CString str;
	switch (nStateID){
	case XJ_OPER_UNHANGOUT:
		strMsg.Format("�û�[%s]������Ա���ȡ�����Ƴɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_UNHANGOUT, OPER_SUCCESS, -1);
		break;
	case XJ_OPER_HANGOUT:
		strMsg.Format("�û�[%s]������Ա��ݹ��Ƴɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_HANGOUT, OPER_SUCCESS, -1);
		break;
	case XJ_OPER_PTSET_STATE_2:
		strMsg.Format("�û�[%s]�Բ���Ա����޸ĺ˶Զ�ֵ�ɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_2, OPER_SUCCESS, -1);
		break;
	case XJ_OPER_PTSET_STATE_3:
		strMsg.Format("�û�[%s]�Լ���Ա�����֤��ֵ�޸ĳɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_3, OPER_SUCCESS, -1);
		break;
	case XJ_OPER_PTSET_STATE_4:
		strMsg.Format("�û�[%s]������Ա�����֤��ֵ�޸ĳɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_4, OPER_SUCCESS, -1);
		break;
	case XJ_OPER_PTSET_STATE_5:
		strMsg.Format("�û�[%s]�Բ���Ա���ִ�ж�ֵ�޸ĳɹ�", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_5, OPER_SUCCESS, -1);
		break;
	}
}

void CXJPTSetStore::AddNewManOperator( CString FunID, CString Act, CString strTime, CString strMsg
									  , CString strUser, int nOperType, int nOperResult , int num)
{
	if(NULL == d_ptr)
		return;

	/*��û��������Ա�����˹�������־��¼*/
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1; 
	char strComputer[MAX_COMPUTERNAME_LENGTH+1]; 
	memset( strComputer, 0, sizeof(strComputer) );
	::GetComputerName( strComputer, &nSize );

	CString sComputer = strComputer;
	
	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	if (strTime.IsEmpty()){
		//ȡ�õ�ǰʱ��
		CTime tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	}
	
	//��֯SQL���
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//ָ��KeyName,����Value
		
	//�ֶ�
	//Msg
	BLOB_FIELD Field1;
	dbEngine.SetBlobField(sql, Field1, "Msg", EX_STTP_DATA_TYPE_STRING, strMsg);
	
	//UserName
	BLOB_FIELD Field2;
	CString sUser = strUser;
	if(strUser.IsEmpty())
		sUser = pApp->m_User.m_strUSER_ID;
	dbEngine.SetBlobField(sql, Field2, "UserName", EX_STTP_DATA_TYPE_STRING, sUser);
	
	//Computer
	BLOB_FIELD Field3;
	dbEngine.SetBlobField(sql, Field3, "Computer", EX_STTP_DATA_TYPE_STRING, sComputer);
	
	//Func
	BLOB_FIELD Field4;
	dbEngine.SetBlobField(sql, Field4, "Func", EX_STTP_DATA_TYPE_STRING, FunID);
	
	//Act
	BLOB_FIELD Field5;
	dbEngine.SetBlobField(sql, Field5, "Act", EX_STTP_DATA_TYPE_STRING, Act);
	
	//Time
	BLOB_FIELD Field6;
	dbEngine.SetBlobField(sql, Field6, "Time", EX_STTP_DATA_TYPE_TIME, strTime);
	
	if(g_OperTableType == 1)
	{
		//OperResult
		str.Format("%d", nOperResult);
		BLOB_FIELD Field7;
		dbEngine.SetBlobField(sql, Field7, "OperResult", EX_STTP_DATA_TYPE_INT, str);
		
		//OperType
		BLOB_FIELD Field8;
		str.Format("%d", nOperType);
		dbEngine.SetBlobField(sql, Field8, "OperType", EX_STTP_DATA_TYPE_INT, str);
		
		//opernum
		BLOB_FIELD Field9;
		str.Format("%d", num);
		dbEngine.SetBlobField(sql, Field9, "Reserve1", EX_STTP_DATA_TYPE_STRING, str);
	}
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = dbEngine.XJInsertBlobData(EX_STTP_INFO_OPERATION_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("���������־�����ݿ�ʧ��");
		return;
	}
	if(nResult == 1)
	{
		
	}
	else
	{
		CString str;
		str.Format("���������־�����ݿ�ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
}

CString	CXJPTSetStore::GetFuncID(int nStateID)
{
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();

	CString strUserGroupName;
	// Ĭ���û���
	switch (nStateID)
	{
	case XJ_OPER_UNHANGOUT: case XJ_OPER_HANGOUT: case XJ_OPER_PTSET_STATE_4:
		strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_RUNNER));
		break;
	case XJ_OPER_PTSET_STATE_2: case XJ_OPER_PTSET_STATE_5:
		strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_OPERATOR));
		break;
	case XJ_OPER_PTSET_STATE_3:
		strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_MONITOR));
		break;
	}

	// �趨��
	QByteArrayMatrix &flow = d_ptr->m_state.GetWorkFlow();
	for (int i = 1; i <= flow.GetRows(); i++){
		int nPTSetStateID = flow.GetFieldValue(i, 1).toInt();
		if (nPTSetStateID == nStateID){
			int nUserType = flow.GetFieldValue(i, 3).toInt();
			if (0 != nUserType)
				strUserGroupName = GetUserTypeName(nUserType);
			break;
		}
	}

	map<int, CString> mapFunc;
	mapFunc.insert(make_pair(XJ_OPER_UNHANGOUT, CString("ȡ������")));
	mapFunc.insert(make_pair(XJ_OPER_HANGOUT, StringFromID(IDS_DZ_MOD_STATE_1)));
	mapFunc.insert(make_pair(XJ_OPER_PTSET_STATE_2, StringFromID(IDS_DZ_MOD_STATE_2)));
	mapFunc.insert(make_pair(XJ_OPER_PTSET_STATE_3, StringFromID(IDS_DZ_MOD_STATE_3)));
	mapFunc.insert(make_pair(XJ_OPER_PTSET_STATE_4, StringFromID(IDS_DZ_MOD_STATE_4)));
	mapFunc.insert(make_pair(XJ_OPER_PTSET_STATE_5, StringFromID(IDS_DZ_MOD_STATE_5)));
	
	CString str;
	str.Format("%s: %s", strUserGroupName, mapFunc[nStateID]);

	return str;
}

CString	CXJPTSetStore::GetUserTypeName(int nUserType)
{
	CString sReturn;
	switch (nUserType){
	case  XJ_USERGROUP_RUNNER:
		return GetUserTypeName(StringFromID(IDS_USERGROUP_RUNNER));
	case XJ_USERGROUP_OPERATOR:
		return GetUserTypeName(StringFromID(IDS_USERGROUP_OPERATOR));
	case XJ_USERGROUP_MONITOR:
		return GetUserTypeName(StringFromID(IDS_USERGROUP_MONITOR));
	}

	return sReturn;
}

CString CXJPTSetStore::GetUserTypeName(CString sUserGroupID)
{
	CString sReturn;
	
	if (NULL == d_ptr || sUserGroupID.IsEmpty())
		return sReturn;

	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//NAME
	Field fld0;
	dbEngine.SetField(sql, fld0, "NAME", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//USER_ID
	Condition cond0;
	str.Format("GROUP_ID = '%s'", sUserGroupID);
	dbEngine.SetCondition(sql, cond0, str);
	
	CMemSet mem;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = dbEngine.XJSelectData(EX_STTP_INFO_SYS_GROUP_CFG, sql, sError, &mem);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJPTSetStore::GetUserTypeName, ��ѯʧ��", XJ_LOG_LV3);
		
		DELETE_POINTERS(sError);
		
		return sReturn;
	}
	
	// ��ѯ�ɹ�
	if(1 == nResult)
	{
		int nCount = mem.GetMemRowNum();
		
		str.Format("CXJPTSetStore::GetUserTypeName, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
			CString strVal = mem.GetValue((UINT)0);
			if (!strVal.IsEmpty()){
				sReturn = strVal;
			}
		}
	}
	else
	{
		CString str;
		str.Format("CXJPTSetStore::GetUserTypeName, ��ѯʧ��, ԭ��Ϊ��%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		sReturn = "";	
	}
	DELETE_POINTERS(sError);
	mem.FreeData(true);
    
	return sReturn;
}
