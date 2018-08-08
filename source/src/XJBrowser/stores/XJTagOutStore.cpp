// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJTagOutStore.h"

#include "XJBrowser.h"
#include "qptsetstatetable.h"
#include "XJUserStore.h"

////////////////////////////////////////////////////////////

class CXJTagOutStorePrivate 
{
public:
	CXJTagOutStorePrivate();
    ~CXJTagOutStorePrivate();
	
	/** @brief           挂牌状态机*/
	QPTSetStateTable	m_state;

	/** @brief           定值修改数据表*/
	QPTSetDataTable		m_data_PTSet;

	BOOL		ReLoadState();
	int			CheckState(int nTagOutType);
	BOOL		Check(int nTagOutType);
	
public:	
	BOOL		Save();
	BOOL		SaveState(const char *pszFilePath = NULL);
	BOOL		SaveData(const char *pszFilePath = NULL);
	BOOL		ReLoadData();

};



////////////////////////////////////////////////////////////
// CXJTagOutStorePrivate
//
CXJTagOutStorePrivate::CXJTagOutStorePrivate()
{
	m_data_PTSet.m_pState = &m_state;
	m_state.m_pData = &m_data_PTSet;
}

CXJTagOutStorePrivate::~CXJTagOutStorePrivate()
{
}

BOOL CXJTagOutStorePrivate::ReLoadState()
{
	BOOL bReturn = FALSE;

	return m_state.ReLoad();
}

BOOL CXJTagOutStorePrivate::Save()
{
	return TRUE;
}

BOOL CXJTagOutStorePrivate::SaveState(const char *pszFilePath/* = NULL*/)
{
	return m_state.Save(pszFilePath);
}

BOOL CXJTagOutStorePrivate::SaveData(const char *pszFilePath/* = NULL*/)
{
	return m_data_PTSet.Save(pszFilePath);
}

BOOL CXJTagOutStorePrivate::Check(int nTagOutType)
{
	return m_state.Check(nTagOutType);
}
/*
 *  @brief   	CheckStore	 检查状态机配置情况
 *  @param 		void
 *  @return 	int		
						 -1: 配置不存在，创建失败;
						  0: 配置检查失败;
						  1: 配置存在;
						  2: 配置不存在，创建成功	
 */
int CXJTagOutStorePrivate::CheckState(int nTagOutType)
{
	int nReturn = 0;

    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	QByteArray tagKeyname = PTVALVSET_KEYNAME;
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		tagKeyname = PTVALVSET_KEYNAME;
		break;
	case XJ_TAGOUT_PTZONESET:
		tagKeyname = PTZONESET_KEYNAME;
		break;
	case XJ_TAGOUT_PTSOFTSET:
		tagKeyname = PTSOFTSET_KEYNAME;
		break;
	default:
		tagKeyname = TAGOUT_KEYNAME;
	}
	
	//条件
	//keyname
	Condition condition1;
	str.Format("keyname = '%s'", tagKeyname);
	dbEngine.SetCondition(sql, condition1, str);
		AfxMessageBox(tagKeyname.constData());
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = dbEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJTagOutStorePrivate::CheckState, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		sError = NULL;

		nReturn = 0;
	}

	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CXJTagOutStorePrivate::CheckState, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		AfxMessageBox(str);
		
		if(nCount > 0)
			return 1;

		// 不存在记录
		//组建查询条件
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//设置字段
		//挂牌状态关键字
		Field fld0;
		dbEngine.SetField(sql1, fld0, "keyname", EX_STTP_DATA_TYPE_STRING, tagKeyname.constData());

		switch (nTagOutType){
		case XJ_TAGOUT_PTVALVSET:
		case XJ_TAGOUT_PTZONESET:
		case XJ_TAGOUT_PTSOFTSET:
			{
				Field fld1;
				//str.Format("%d,%d,,0,0,0", XJ_TAGOUT_UNDEFINE, XJ_OPER_UNHANGOUT);
				str.Format("子状态机：%s", m_state.GetTagOutReasonName(nTagOutType).constData());
				dbEngine.SetField(sql1, fld1, "note", EX_STTP_DATA_TYPE_STRING, str);
				Field fld2;
				dbEngine.SetField(sql1, fld2, "reverse1", EX_STTP_DATA_TYPE_STRING
						, m_state.GetDefaultWorkFlow(nTagOutType).constData());
			}
			break;
		default:
			{
				Field fld1;
				str.Format("%d,%d,,0,0,0", XJ_TAGOUT_UNDEFINE, XJ_OPER_UNHANGOUT);
				dbEngine.SetField(sql1, fld1, "value", EX_STTP_DATA_TYPE_STRING, str);
				Field fld2;
				str.Format("装置挂牌状态机");
				dbEngine.SetField(sql1, fld2, "note", EX_STTP_DATA_TYPE_STRING, str);
			}
		}
		
		memset(sError, '\0', MAXMSGLEN);
		
		//int nResult;
		try
		{
			nResult = dbEngine.XJInsertData(EX_STTP_INFO_TBSYSCONFIG, sql1, sError);

			if(nResult == 1)
			{
				//成功
				nReturn = 11;
			}
		}
		catch (...)
		{
			WriteLog("CXJTagOutStore::CheckStore, 保存失败");
			delete[] sError;
			sError = NULL;
			
			nReturn = 10;
		}
	}
	else
	{
		CString str;
		str.Format("CXJTagOutStore::CheckStore, 查询失败, 原因为%s", sError);
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
// CXJTagOutStore
//
CXJTagOutStore *CXJTagOutStore::m_pInstance = NULL;

CXJTagOutStore *CXJTagOutStore::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CXJTagOutStore;
		m_pInstance->Register(m_pInstance);
	}
	return m_pInstance;
}

void CXJTagOutStore::ReleaseInstance()
{
	m_pInstance->UnRegister(m_pInstance);
	DELETE_POINTER(m_pInstance);
}

CXJTagOutStore::CXJTagOutStore()
	: d_ptr(new CXJTagOutStorePrivate)
{
}

CXJTagOutStore::~CXJTagOutStore()
{
	DELETE_POINTER(d_ptr);
}

BOOL CXJTagOutStore::ReLoadState()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->ReLoadState();
}

BOOL CXJTagOutStore::Save()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->Save();
}

BOOL CXJTagOutStore::SaveState(const char *pszFilePath/* = NULL*/)
{	
	if (NULL == d_ptr)
		return FALSE;

	return d_ptr->SaveState(pszFilePath);
}

BOOL CXJTagOutStore::SaveData(const char *pszFilePath/* = NULL*/)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->SaveData(pszFilePath);
}

BOOL CXJTagOutStore::Check()
{
	if (NULL == d_ptr)
		return TRUE;
	
	d_ptr->Check(XJ_TAGOUT_UNDEFINE);
 	d_ptr->Check(XJ_TAGOUT_PTVALVSET);
 	d_ptr->Check(XJ_TAGOUT_PTZONESET);
 	d_ptr->Check(XJ_TAGOUT_PTSOFTSET);

	return TRUE;
}

QPTSetStateTable* CXJTagOutStore::GetState()
{
	if (NULL == d_ptr)
		return NULL;
	
	return &(d_ptr->m_state);
}

QPTSetDataTable* CXJTagOutStore::GetPTSetData()
{
	if (NULL == d_ptr)
		return NULL;
	
	return &(d_ptr->m_data_PTSet);
}


BOOL CXJTagOutStore::RevertModify()
{
	BOOL bReturn = FALSE;

	return bReturn;
}

void CXJTagOutStore::AddNewManOperator(int nStateID, const char* szTime, CString sUserID, int nOperType, CString strMsg)
{
	if(NULL == d_ptr)
		return;

	CString FunID = GetFuncID(nStateID);
	CString Act = d_ptr->m_state.GetPTID().constData();
	CString strTime = szTime;

	CString str;
	if (strMsg.IsEmpty())
	switch (nStateID){
	case XJ_OPER_UNHANGOUT:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以运行员身份取消挂牌成功", sUserID);
		else
			strMsg.Format("用户[%s]以运行员身份取消挂牌失败", sUserID);
		break;
	case XJ_OPER_HANGOUT:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以运行员身份挂牌成功", sUserID);
		else
			strMsg.Format("用户[%s]以运行员身份挂牌失败", sUserID);
		break;
	case XJ_OPER_PTVALVSET_STATE_2:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份修改核对定值成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份修改核对定值失败", sUserID);
		break;
	case XJ_OPER_PTVALVSET_STATE_3:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以监视员身份验证定值修改成功", sUserID);
		else
			strMsg.Format("用户[%s]以监视员身份验证定值修改失败", sUserID);
		break;
	case XJ_OPER_PTVALVSET_STATE_4:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以运行员身份验证定值修改成功", sUserID);
		else
			strMsg.Format("用户[%s]以运行员身份验证定值修改失败", sUserID);
		break;
	case XJ_OPER_PTVALVSET_STATE_5:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份执行定值修改成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份执行定值修改失败", sUserID);
		break;
	case XJ_OPER_PTZONESET_STATE_2:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份进行定值区切换修改核对成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份进行定值区切换修改核对失败", sUserID);
		break;
	case XJ_OPER_PTZONESET_STATE_3:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以监视员身份进行定值区切换监护成功", sUserID);
		else
			strMsg.Format("用户[%s]以监视员身份进行定值区切换监护失败", sUserID);
		break;
	case XJ_OPER_PTZONESET_STATE_4:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以运行员身份进行定值区切换验证成功", sUserID);
		else
			strMsg.Format("用户[%s]以运行员身份进行定值区切换验证失败", sUserID);
		break;
	case XJ_OPER_PTZONESET_STATE_5:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份执行定值区切换成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份执行定值区切换失败", sUserID);
		break;
	case XJ_OPER_PTSOFTSET_STATE_2:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份进行软压板投退修改核对成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份进行软压板投退修改核对失败", sUserID);
		break;
	case XJ_OPER_PTSOFTSET_STATE_3:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以监视员身份进行软压板投退监护成功", sUserID);
		else
			strMsg.Format("用户[%s]以监视员身份进行软压板投退监护失败", sUserID);
		break;
	case XJ_OPER_PTSOFTSET_STATE_4:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以运行员身份进行软压板投退验证成功", sUserID);
		else
			strMsg.Format("用户[%s]以运行员身份进行软压板投退验证失败", sUserID);
		break;
	case XJ_OPER_PTSOFTSET_STATE_5:
		if (OPER_SUCCESS == nOperType)
			strMsg.Format("用户[%s]以操作员身份执行软压板投退成功", sUserID);
		else
			strMsg.Format("用户[%s]以操作员身份执行软压板投退失败", sUserID);
		break;
	}
	WriteLog(strMsg);
	AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, nStateID, nOperType, -1);
}

void CXJTagOutStore::AddNewManOperator( CString FunID, CString Act, CString strTime, CString strMsg
									  , CString strUser, int nOperType, int nOperResult , int num)
{
	if(NULL == d_ptr)
		return;

	/*获得机器名，以便进行人工操作日志记录*/
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1; 
	char strComputer[MAX_COMPUTERNAME_LENGTH+1]; 
	memset( strComputer, 0, sizeof(strComputer) );
	::GetComputerName( strComputer, &nSize );

	CString sComputer = strComputer;
	
	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	if (strTime.IsEmpty()){
		//取得当前时间
		CTime tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	}
	
	//组织SQL语句
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//指定KeyName,更新Value
		
	//字段
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
		WriteLog("保存操作日志到数据库失败");
		return;
	}
	if(nResult == 1)
	{
		
	}
	else
	{
		CString str;
		str.Format("保存操作日志到数据库失败,原因为%s", sError);
		WriteLog(str);
	}
}

QByteArray CXJTagOutStore::GetFuncID(int nStateID)
{
	QByteArray baReturn;

	CXJUserStore *pUserStore = CXJUserStore::GetInstance();

	QByteArray baUserGroupName;
	// 默认用户组
	switch (nStateID)
	{
	case XJ_OPER_UNHANGOUT: case XJ_OPER_HANGOUT: 
	case XJ_OPER_PTVALVSET_STATE_4:
	case XJ_OPER_PTZONESET_STATE_4:
	case XJ_OPER_PTSOFTSET_STATE_4:
		//strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_RUNNER));
		baUserGroupName = pUserStore->GetUserGroupName(XJ_USERGROUP_RUNNER);
		break;
	case XJ_OPER_PTVALVSET_STATE_2: 
	case XJ_OPER_PTZONESET_STATE_2: 
	case XJ_OPER_PTSOFTSET_STATE_2: 
	case XJ_OPER_PTVALVSET_STATE_5:
	case XJ_OPER_PTZONESET_STATE_5: 
	case XJ_OPER_PTSOFTSET_STATE_5: 
		//strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_OPERATOR));
		baUserGroupName = pUserStore->GetUserGroupName(XJ_USERGROUP_OPERATOR);
		break;
	case XJ_OPER_PTVALVSET_STATE_3:
	case XJ_OPER_PTZONESET_STATE_3: 
	case XJ_OPER_PTSOFTSET_STATE_3: 
		//strUserGroupName = GetUserTypeName(StringFromID(IDS_USERGROUP_MONITOR));
		baUserGroupName = pUserStore->GetUserGroupName(XJ_USERGROUP_MONITOR);
		break;
	}
	// 设定组
	QByteArrayMatrix &flow = d_ptr->m_state.GetWorkFlow();
	for (int i = 1; i <= flow.GetRows(); i++){
		int nPTSetStateID = flow.GetFieldValue(i, 1).toInt();
		if (nPTSetStateID == nStateID){
			int nUserGroupType = flow.GetFieldValue(i, 3).toInt();
			if (0 != nUserGroupType)
				baUserGroupName = pUserStore->GetUserGroupName(nUserGroupType);
			break;
		}
	}

	const char* szState[] = {
			"挂牌",
			"取消挂牌",
			"修改核对",
			"监护",
			"验证",
			"执行完成"
	};

	map<int, QByteArray> mapFunc;
	mapFunc.insert(make_pair(XJ_OPER_HANGOUT, szState[0]));
	mapFunc.insert(make_pair(XJ_OPER_UNHANGOUT, szState[1]));

	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_5, szState[5]));
	
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_5, szState[5]));
	
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_5, szState[5]));

	//QByteArray opReasonTypeName = d_ptr->m_state.GetHangoutReasonName();
	QByteArray opReasonTypeName = d_ptr->m_state.GetTagOutReasonNameByState(nStateID);
	if (opReasonTypeName.isEmpty())
		opReasonTypeName = d_ptr->m_state.GetTypeName();

	baReturn << opReasonTypeName << " | "
		<< baUserGroupName << ": "
		<< mapFunc[nStateID];

	return baReturn;
}

QByteArray CXJTagOutStore::GetSubFuncID(int nStateID)
{
	QByteArray baReturn;
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	
	const char* szState[] = {
		"挂牌",
			"取消挂牌",
			"修改核对",
			"监护",
			"验证",
			"执行完成"
	};
	
	map<int, QByteArray> mapFunc;
	mapFunc.insert(make_pair(XJ_OPER_HANGOUT, szState[0]));
	mapFunc.insert(make_pair(XJ_OPER_UNHANGOUT, szState[1]));
	
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTVALVSET_STATE_5, szState[5]));
	
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTZONESET_STATE_5, szState[5]));
	
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_2, szState[2]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_3, szState[3]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_4, szState[4]));
	mapFunc.insert(make_pair(XJ_OPER_PTSOFTSET_STATE_5, szState[5]));
	
	//QByteArray opReasonTypeName = d_ptr->m_state.GetHangoutReasonName();
	QByteArray opReasonTypeName = d_ptr->m_state.GetTagOutReasonNameByState(nStateID);
	if (opReasonTypeName.isEmpty())
		opReasonTypeName = d_ptr->m_state.GetTypeName();
	
	baReturn << mapFunc[nStateID];
	
	return baReturn;
}

int CXJTagOutStore::GetTagOutRowIdx(int nTagOutType/* = XJ_TAGOUT_UNDEFINE*/)
{
	QByteArrayMatrix keyvals;
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		keyvals = PTVALVSET_KEYNAME;
		break;
	case XJ_TAGOUT_PTZONESET:
		keyvals = PTZONESET_KEYNAME;
		break;
	case XJ_TAGOUT_PTSOFTSET:
		keyvals = PTSOFTSET_KEYNAME;
		break;
	default:
		keyvals = TAGOUT_KEYNAME;
	}

	if (NULL == d_ptr)
		return -1;

	return d_ptr->m_state.GetRowIndex(keyvals);
}