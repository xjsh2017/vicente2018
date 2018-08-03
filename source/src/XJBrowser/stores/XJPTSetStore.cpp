// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJPTSetStore.h"

#include "XJBrowser.h"
#include "qptsetcard.h"

const char* PTSET_KEYNAME = "PTSET_STATE";

////////////////////////////////////////////////////////////

class CXJPTSetStorePrivate 
{
public:
	CXJPTSetStorePrivate();
    ~CXJPTSetStorePrivate();
	
	QPTSetCard		m_card;
	QLogTable		m_log;

	PT_SETTING_DATA_LIST m_arrPTSet;	

	BOOL	ReloadState();
	BOOL	SaveState();
	int		CheckState();

	void	ClearStore();
	BOOL	ReLoadStore();
	BOOL	SaveStore();
	BOOL	RevertStore();
};

CXJPTSetStorePrivate::CXJPTSetStorePrivate()
{
}

CXJPTSetStorePrivate::~CXJPTSetStorePrivate()
{
	ClearStore();
}

BOOL CXJPTSetStorePrivate::ReloadState()
{
	if (CheckState() < 0){
		return FALSE;
	}

	QPTSetCard &card = m_card;
	QLogTable &log = m_log;

	CXJBrowserApp *pApp = (CXJBrowserApp *)AfxGetApp();
	CDBEngine &dbEngine = pApp->m_DBEngine;

	// Get 
	CString	str;

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//value
	Field fld0;
	dbEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	//reverse1
	Field fld1;
	dbEngine.SetField(sql, fld1, "reverse3", EX_STTP_DATA_TYPE_STRING);
	
	//条件
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
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJPTSetStorePrivate::ReLoadState, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		return FALSE;
	}

	// 查询成功
	if(NULL != pMemset && 1 == nResult)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJPTSetStorePrivate::ReLoadState, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		char * val = NULL;
		if(nCount > 0)
		{
			val = pMemset->GetValue((UINT)0);
			card.FRead(val);

			val = pMemset->GetValue((UINT)1);
			log.FRead(val);
			
			//card.FWrite(1, "c:/data.txt");
			//str.Format(" value: [%s]\n log: [%s]", card.data(), log.data());
			//AfxMessageBox(str);
		}
	}
	else
	{
		CString str;
		str.Format("CXJPTSetStorePrivate::ReLoadState, 查询失败, 原因为：%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);

		return FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return TRUE;
}

BOOL CXJPTSetStorePrivate::SaveState()
{
	BOOL bReturn = FALSE;

	if (CheckState() < 0){
		return FALSE;
	}

	QPTSetCard &card = m_card;
	QLogTable &log = m_log;

    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	CString str;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
    // 更新定值修改状态机
	// 组建查询条件
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
	
	//条件
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
			str.Format("CXJPTSetStorePrivate::SaveState, 更新失败, 原因为：%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			
			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJPTSetStorePrivate::SaveState, 更新失败");
		AfxMessageBox(sError);

		DELETE_POINTERS(sError);
		
		return FALSE;
	}
	
	DELETE_POINTERS(sError);

	return bReturn;
}

int CXJPTSetStorePrivate::CheckState()
{
	int nReturn = -1;

    CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//value
	Field fld0;
	dbEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	
	//条件
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
			//AfxMessageBox("CXJPTSetStore::CheckStore, 查询失败");
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJPTSetStore::CheckStore, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		sError = NULL;

		nReturn = -1;
	}

	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CXJPTSetStore::CheckStore, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
			return 0;

		// 不存在记录 PTSET_KEYNAME
		//组建查询条件
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//设置字段
		//挂牌状态关键字
		Field fld1;
		dbEngine.SetField(sql1, fld1, "keyname", EX_STTP_DATA_TYPE_STRING, PTSET_KEYNAME);
		Field fld2;
		dbEngine.SetField(sql1, fld2, "value", EX_STTP_DATA_TYPE_STRING, "0,0,,0,0,0");
		
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
			WriteLog("CXJPTSetStore::CheckStore, 保存失败");
			delete[] sError;
			sError = NULL;
			
			nReturn = 10;
		}
	}
	else
	{
		CString str;
		str.Format("CXJPTSetStore::CheckStore, 查询失败, 原因为%s", sError);
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

void CXJPTSetStorePrivate::ClearStore()
{
	for(int i = 0; i < m_arrPTSet.GetSize(); i++)
	{
		PT_SETTING_DATA * data = (PT_SETTING_DATA*)m_arrPTSet.GetAt(i);
		DELETE_POINTER(data->pts);
		DELETE_POINTER(data);
	}
	m_arrPTSet.RemoveAll();
}

BOOL CXJPTSetStorePrivate::ReLoadStore()
{
	ClearStore();
	
	QPTSetCard &card = m_card;
	QLogTable &log = m_log;
	
	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;

	WriteLog("CXJPTSetStorePrivate::ReLoadStore, 查询开始", XJ_LOG_LV3);

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	//字段
	//Setting_ID
	Field Field1;
	dbEngine.SetField(sql, Field1, "Setting_ID", EX_STTP_DATA_TYPE_INT);

	//name
	Field Field2;
	dbEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);

	//code_name
	Field Field3;
	dbEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);

	//vpickList
	Field Field4;
	dbEngine.SetField(sql, Field4, "vpicklist", EX_STTP_DATA_TYPE_STRING);

	//unit
	Field Field5;
	dbEngine.SetField(sql, Field5, "unit", EX_STTP_DATA_TYPE_STRING);

	//s_precision
	Field Field6;
	dbEngine.SetField(sql, Field6, "s_precision", EX_STTP_DATA_TYPE_STRING);

	//datatype
	Field Field7;
	dbEngine.SetField(sql, Field7, "datatype", EX_STTP_DATA_TYPE_INT);

	//minvalue
	Field Field8;
	dbEngine.SetField(sql, Field8, "minvalue", EX_STTP_DATA_TYPE_FLOAT);

	//maxvalue
	Field Field9;
	dbEngine.SetField(sql, Field9, "maxvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field10;
	dbEngine.SetField(sql, Field10, "stepvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field11;
	dbEngine.SetField(sql, Field11, "103group", EX_STTP_DATA_TYPE_INT);
	
	//stepvalue
	Field Field12;
	dbEngine.SetField(sql, Field12, "103item", EX_STTP_DATA_TYPE_INT);

	//stepvalue
	Field Field13;
	dbEngine.SetField(sql, Field13, "61850ref", EX_STTP_DATA_TYPE_INT);

	Field Field14;
	dbEngine.SetField(sql, Field14, "RESERVE1", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	dbEngine.SetField(sql, Field15, "RESERVE3", EX_STTP_DATA_TYPE_STRING);

	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", card.GetPTID().constData());
	dbEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %d", card.GetCPUID());
	dbEngine.SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("ZONE = %d", card.GetZoneID());
	dbEngine.SetCondition(sql, condition3, str);

	//按Setting_ID大小排序
	Condition condition4;
	str.Format("order by SETTING_ID");
	dbEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //非where条件

	CMemSet mem;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = dbEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &mem);
	}
	catch (...)
	{
		WriteLogEx("CXJPTSetStorePrivate::ReLoadStore, 查询失败");
		return FALSE;
	}

	if(1 == nResult)
	{
		mem.MoveFirst();
		int nCount = mem.GetMemRowNum();

		CString str;
		str.Format("CXJPTSetStorePrivate::ReLoadStore,查询到%d条记录", nCount);
		WriteLog(str, XJ_LOG_LV3);

		//EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			//创建定值对象
			PT_SETTING * pts = new PT_SETTING;
			PT_SETTING_DATA *data = new PT_SETTING_DATA;
			data->pts = pts;

			//setting_id,NAME,CODE_NAME,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = mem.GetValue((UINT)0); //Setting_ID
			pts->Name = mem.GetValue(1); //NAME
			pts->CodeName = mem.GetValue(2); //Code_Name
			pts->VPickList = mem.GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = mem.GetValue(4); //unit
			pts->Precision = mem.GetValue(5); //s_precision
			str = mem.GetValue(6); //datatype
			pts->DataType = atoi(str);
			str = mem.GetValue(7);
			pts->minValue = str;//atof(str);
			str = mem.GetValue(8);
			pts->maxValue = str;//atof(str);
			str = mem.GetValue(9);
			pts->stepValue = str;//atof(str);
			str = mem.GetValue(10);
			pts->Group = atoi(str);
			str = mem.GetValue(11);
			pts->Item = atoi(str);
			str = mem.GetValue(12);
			pts->nRefType = 1;
			if(!str.IsEmpty())
			{
				str.MakeUpper();
				if(str.Find("$SP$", 0) != -1)
					pts->nRefType = 0;
			}
			else
			{
				CString sGroupName;// = GetGroupName(pts->Group);
				if(!sGroupName.IsEmpty())
				{
					if(sGroupName.Find( StringFromID(IDS_COMMON_PARAMETER), 0) != -1)
						pts->nRefType = 0;
				}
			}
			str = mem.GetValue(13);
			pts->ntimeMStoS = 0;
			if(!str.IsEmpty())
			{
				pts->ntimeMStoS = atoi(str);
			}

			QMatrixByteArray val = mem.GetValue(14);
			if (val.count(',') == 0){
				data->reserve1 = val;
				data->reserve3 = "";
			}else{
				data->reserve1 = val.GetFiled(1, 1);
				data->reserve3 = val.GetFiled(1, 2);
			}

			m_arrPTSet.Add(data);
					
			mem.MoveNext();
		}
	}
	else
	{
		str.Format("CXJPTSetStorePrivate::ReLoadStore,查询失败,原因为%s", sError);
		WriteLogEx(str);
	}

	str.Format("m_arrPTSet.size() = %d", m_arrPTSet.GetSize());
	//AfxMessageBox(str);

	WriteLog("CXJPTSetStorePrivate::ReLoadStore,查询结束", XJ_LOG_LV3);

	return TRUE;
}

BOOL CXJPTSetStorePrivate::SaveStore()
{
	if(m_arrPTSet.GetSize() < 0)
		return FALSE;
	
	BOOL bReturn = FALSE;

	QPTSetCard &card = m_card;
	QLogTable &log = m_log;
	
	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	
	char * sError = NULL;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	for(int i = 0; i < m_arrPTSet.GetSize(); i++)
	{
		CMemSet	mem;
		int nResult;

		PT_SETTING_DATA* data = (PT_SETTING_DATA*)m_arrPTSet.GetAt(i);
		PT_SETTING* pts = data->pts;
		
		CString strSQL;
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '%s' WHERE pt_id= '%s' AND cpu_code = '%d' AND setting_id = '%s'"
			, QByteArray(data->reserve1 + "," + data->reserve3).constData()
			, card.GetPTID().constData()
			, card.GetCPUID()
			, pts->ID);

		WriteLog(strSQL);
		//AfxMessageBox(strSQL);
		
		//进行查询
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = dbEngine.XJExecuteSql(MutiSql, sError, &mem);
		}
		catch (...)
		{
			str.Format("CXJPTSetStorePrivate::SaveStore, 更新失败");
			WriteLog(str);
			DELETE_POINTERS(sError);
			//AfxMessageBox(str);
			
			return FALSE;
		}
		
		if(nResult == 1)
		{	
			str.Format("CXJPTSetStorePrivate::SaveStore, 更新成功");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJPTSetStorePrivate::SaveStore, 更新失败, 原因为%s", sError);
			WriteLog(str);
			//AfxMessageBox(str);
		}
		
		mem.FreeData(true);
	}
	
	DELETE_POINTERS(sError);
	
	return TRUE;
	
}

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

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

BOOL CXJPTSetStore::ReLoad()
{
	if (NULL == d_ptr)
		return FALSE;

	return d_ptr->ReloadState();
}

BOOL CXJPTSetStore::ReLoadStore()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->ReLoadStore();
}

BOOL CXJPTSetStore::Save()
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->SaveState();
}

int CXJPTSetStore::Check()
{
	if (NULL == d_ptr)
		return -2;
	
	return d_ptr->CheckState();
}



QPTSetCard* CXJPTSetStore::GetCard()
{
	if (NULL == d_ptr)
		return NULL;

	return &(d_ptr->m_card);
}

QLogTable* CXJPTSetStore::GetLog()
{
	if (NULL == d_ptr)
		return NULL;
	
	return &(d_ptr->m_log);
}

PT_SETTING_DATA_LIST& CXJPTSetStore::GetStoreData()
{
	assert (NULL != d_ptr);

	return d_ptr->m_arrPTSet;
}

BOOL CXJPTSetStore::Next(int nNextStateID, const char* szUserID, int nFlag)
{
	ReLoad();
	
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	
	QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(GetCard()));
	QLogTable &log = *(reinterpret_cast<QLogTable *>(GetLog()));
	
	int nCurrentStateID = card.GetStateID();
	if (nNextStateID <= nCurrentStateID){
		return TRUE;
	}
	
	card.SetStateID(nNextStateID);
	card.SetFlags(nFlag);
	
	char szLog[256] = {0};
	sprintf(szLog, "%s,%s,%d"
		, GetTime().constData()
		, szUserID
		, nNextStateID);
	log.Insert(szLog);
	
	return Save();
}

BOOL CXJPTSetStore::Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag)
{
	if (nNextStateID > 1){
		return Next(nNextStateID, szUserID);
	}
	
	ReLoad();
	QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(GetCard()));
	QLogTable &log = *(reinterpret_cast<QLogTable *>(GetLog()));
	
	card.SetType(0);
	card.SetFlags(nFlag);
	card.SetCPUID(0);
	card.SetZoneID(0);

	if (nNextStateID == 1){
		card.SetStateID(1);
		card.SetPTID(szPTID);
		
		char szLog[256] = {0};
		sprintf(szLog, "%s,%s,%d"
			, GetTime().constData()
			, szUserID
			, 1);
		log.FRead(szLog);
	}else{
		card.SetStateID(0);
	}

	return Save();
}

BOOL CXJPTSetStore::Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag)
{
	ReLoad();
	QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(GetCard()));
	QLogTable &log = *(reinterpret_cast<QLogTable *>(GetLog()));

	card.SetStateID(nNextStateID);
	card.SetCPUID(nCPUID);
	card.SetZoneID(nZoneID);

	char szLog[256] = {0};
	QByteArray curTime = GetTime();
	sprintf(szLog, "%s,%s,%d"
		, curTime.constData()
		, szUserID
		, nNextStateID);
	log.Insert(szLog);

	AddNewManOperator(nNextStateID, curTime.constData(), szUserID);

	return Save();
}

BOOL CXJPTSetStore::SaveRecallToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting)
{
	if (NULL == d_ptr)
		return FALSE;

	if (!d_ptr->ReLoadStore())
		return false;

	map<CString, PT_SETTING*> mapPTSet;
	
	int nCount = arrSetting.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		PT_SETTING * pts = (PT_SETTING*)arrSetting.GetAt(i);
		mapPTSet.insert(std::make_pair(pts->ID, pts));
	}

	nCount = d_ptr->m_arrPTSet.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SETTING_DATA *pts_data = (PT_SETTING_DATA*)d_ptr->m_arrPTSet.GetAt(i);
		if (mapPTSet.count(pts_data->pts->ID) == 1){
			pts_data->pts->Value = mapPTSet[pts_data->pts->ID]->Value;
			pts_data->reserve1 = mapPTSet[pts_data->pts->ID]->Value.GetBuffer(0);
		}
	}

	d_ptr->SaveStore();

	return TRUE;
}

BOOL CXJPTSetStore::SaveModifyToDB(CString &sPTID, MODIFY_LIST &arrModifyList)
{
	if (NULL == d_ptr)
		return FALSE;
	
	if (!d_ptr->ReLoadStore())
		return false;
	
	map<CString, STTP_DATA*> mapModify;
	
	int nCount = arrModifyList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);

		mapModify.insert(make_pair(strID, sttpData));
	}

	nCount = d_ptr->m_arrPTSet.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SETTING_DATA *pts_data = (PT_SETTING_DATA*)d_ptr->m_arrPTSet.GetAt(i);
		if (mapModify.count(pts_data->pts->ID) == 1){
			pts_data->reserve3 = mapModify[pts_data->pts->ID]->str_value.c_str();
		}
	}

	d_ptr->SaveStore();
	
	return TRUE;
}

BOOL CXJPTSetStore::RevertModify()
{
	BOOL bReturn = FALSE;

	if (!d_ptr->ReLoadStore())
		return false;

	int nCount = d_ptr->m_arrPTSet.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SETTING_DATA *pts_data = (PT_SETTING_DATA*)d_ptr->m_arrPTSet.GetAt(i);
		pts_data->reserve3 = "";
	}

	d_ptr->SaveStore();

	return bReturn;
}

void CXJPTSetStore::AddNewManOperator(int nStateID, const char* szTime, CString sUserID)
{
	if(NULL == d_ptr)
		return;

	QPTSetCard &card = d_ptr->m_card;
	QLogTable &log = d_ptr->m_log;

	CString FunID = GetFuncID(nStateID);
	CString Act = card.GetPTID().constData();
	CString strTime = szTime;
	CString strMsg;

	CString str;
	switch (nStateID){
	case 0:
		strMsg.Format("用户[%s]以运行员身份取消挂牌成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_UNHANGOUT, OPER_SUCCESS, -1);
		break;
	case 1:
		strMsg.Format("用户[%s]以运行员身份挂牌成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_HANGOUT, OPER_SUCCESS, -1);
		break;
	case 2:
		strMsg.Format("用户[%s]以操作员身份修改核对定值成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_2, OPER_SUCCESS, -1);
		break;
	case 3:
		strMsg.Format("用户[%s]以监视员身份验证定值修改成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_3, OPER_SUCCESS, -1);
		break;
	case 4:
		strMsg.Format("用户[%s]以运行员身份验证定值修改成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_4, OPER_SUCCESS, -1);
		break;
	case 5:
		strMsg.Format("用户[%s]以操作员身份执行定值修改成功", sUserID);
		WriteLog(strMsg);
		AddNewManOperator(FunID, Act, strTime, strMsg, sUserID, XJ_OPER_PTSET_STATE_4, OPER_SUCCESS, -1);
		break;
	}
}

void CXJPTSetStore::AddNewManOperator( CString FunID, CString Act, CString strTime, CString strMsg
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

	QPTSetCard &card = d_ptr->m_card;
	QLogTable &log = d_ptr->m_log;
	
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

CString	CXJPTSetStore::GetFuncID(int nStateID)
{
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();

	CString strUserGroupName;
	switch (nStateID)
	{
	case 1: case 4: case 0:
		strUserGroupName = GetUserGroupNameByID(StringFromID(IDS_USERGROUP_RUNNER));
		break;
	case 2: case 5:
		strUserGroupName = GetUserGroupNameByID(StringFromID(IDS_USERGROUP_OPERATOR));
		break;
	case 3:
		strUserGroupName = GetUserGroupNameByID(StringFromID(IDS_USERGROUP_MONITOR));
		break;
	}
	
	CString c_dz_mod_state[] = {
			//StringFromID(IDS_DZ_MOD_STATE_0),
				"取消挂牌",
			StringFromID(IDS_DZ_MOD_STATE_1),
			StringFromID(IDS_DZ_MOD_STATE_2),
			StringFromID(IDS_DZ_MOD_STATE_3),
			StringFromID(IDS_DZ_MOD_STATE_4),
 			StringFromID(IDS_DZ_MOD_STATE_5)
	};
	
	CString str;
	str.Format("%s: %s", strUserGroupName, c_dz_mod_state[nStateID]);

	return str;
}

CString CXJPTSetStore::GetUserGroupNameByID(CString sUserGroupID)
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
	
	//字段
	//NAME
	Field fld0;
	dbEngine.SetField(sql, fld0, "NAME", EX_STTP_DATA_TYPE_STRING);
	
	//条件
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
		WriteLog("CXJPTSetStore::GetUserGroupNameByID, 查询失败", XJ_LOG_LV3);
		
		DELETE_POINTERS(sError);
		
		return sReturn;
	}
	
	// 查询成功
	if(1 == nResult)
	{
		int nCount = mem.GetMemRowNum();
		
		str.Format("CXJPTSetStore::GetUserGroupNameByID, 读取到%d条数据", nCount);
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
		str.Format("CXJPTSetStore::GetUserGroupNameByID, 查询失败, 原因为：%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		sReturn = "";	
	}
	DELETE_POINTERS(sError);
	mem.FreeData(true);
    
	return sReturn;
}
