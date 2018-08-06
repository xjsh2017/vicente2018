// qptsetstatetable.cpp : implementation file
//

#include "stdafx.h"
#include "qptsetstatetable.h"

#include "XJStoreDefine.h"
#include "XJPTSetStore.h"
#include "XJBrowser.h"

const char* PTSET_KEYNAME = "PTSET_STATE";

///////////////////////////////////////////////////////////////////
//  QPTSetCard1
// 
QPTSetCard1::QPTSetCard1()
: QByteArrayMatrix("", ";", ",")
{
	
}

QPTSetCard1::QPTSetCard1(const char *pszIn)
: QByteArrayMatrix(pszIn, ";", ",")
{
	
}

QPTSetCard1::QPTSetCard1(const char *pszIn, const char *delim_row, const char *delim_col)
: QByteArrayMatrix(pszIn, delim_row, delim_col)
{
	
}

QByteArray QPTSetCard1::FWrite(const char *pszFilePath)
{
	QByteArray out;

#define endl "\n"

	out << "[ " << GetTime().data() << " ]" << endl;
	out << "----------------------------" << endl;
	out << "| Value    | [" << constData() << "]" << endl;
	out << "| Len      |  " << count() << endl;
	out << "----------------------------" << endl;
	out << "| Type     | " << GetType() << endl;
	out << "| State ID | " << GetStateID() << endl;
	out << "| PT    ID | " << GetPTID().data() << endl;
	out << "| CPU   ID | " << GetCPUID() << endl;
	out << "| Zone  ID | " << GetZoneID() << endl;
	out << "| Flags    | " << GetFlags() << endl;
	out << "----------------------------" << endl;

	if (NULL == pszFilePath || QByteArray(pszFilePath).isEmpty()){
		cout << out.constData() << endl;
	}else{
		if (NULL == pszFilePath)
			return out;

		fstream fs;
		fs.open(pszFilePath, std::fstream::out/* | std::fstream::app*/);
		//FWrite(fs);

		fs.write(out.data(), out.count());
	}

	return out;
}


int	QPTSetCard1::GetType()
{
	return GetFieldValue(1, 1).toInt();
}

void QPTSetCard1::SetType(int nType)
{
	SetFieldValue(1, 1, QByteArray::number(nType));
}

int	QPTSetCard1::GetStateID()
{
	return GetFieldValue(1, 2).toInt();
}

void QPTSetCard1::SetStateID(int nID)
{
	SetFieldValue(1, 2, QByteArray::number(nID));
}

QByteArray QPTSetCard1::GetPTID()
{
	return GetFieldValue(1, 3);
}

void QPTSetCard1::SetPTID(const char *pt_id)
{
	SetFieldValue(1, 3, QByteArray(pt_id));
}

int QPTSetCard1::GetCPUID()
{
	return GetFieldValue(1, 4).toInt();
}

void QPTSetCard1::SetCPUID(int nCPUID)
{
	SetFieldValue(1, 4, QByteArray::number(nCPUID));
}

int QPTSetCard1::GetZoneID()
{
	return GetFieldValue(1, 5).toInt();
}

void QPTSetCard1::SetZoneID(int nZoneID)
{
	SetFieldValue(1, 5, QByteArray::number(nZoneID));
}

int QPTSetCard1::GetFlags()
{
	return GetFieldValue(1, 6).toInt();
}

void QPTSetCard1::SetFlags(int nFlags)
{
	SetFieldValue(1, 6, QByteArray::number(nFlags));
}


/////////////////////////////////////////////////////////////
// QLogTable1
//
QLogTable1::QLogTable1()
	: QByteArrayMatrix("", ";", ",")
{

}

QLogTable1::QLogTable1(const char *pszIn)
	: QByteArrayMatrix(pszIn, ";", ",")
{

}

QLogTable1::QLogTable1(const char *pszIn, const char *delim_row, const char *delim_col)
	: QByteArrayMatrix(pszIn, delim_row, delim_col)
{

}

int QLogTable1::GetRecordCount()
{
	return GetRows();
}

QByteArray	QLogTable1::GetRecord(int iRow)
{
	return GetRowData(iRow);
}

QByteArray& QLogTable1::Revert(int iRow)
{
	int from = 0;
	int end = count();
	
	int rows = GetRecordCount();
	
	if (iRow < 1 || iRow > rows)
		return *this;
	
	for (int i = 1; i <= iRow; i++){
		from = indexOf(GetDelimRow(), from);
		
		if (i != iRow)
			from = from + 1;
	}

	return remove(from, end - from);
}

QByteArray& QLogTable1::Insert(char *pszRecord, int iLen)
{
	QByteArray byte(GetDelimRow());
	byte += pszRecord;
	return append(byte);
}

QByteArray QLogTable1::FWrite(const char *pszFilePath)
{
	QByteArray out;

	out << "[ " << GetTime().data() << " ]" << endl;
	out << "----------------------------" << endl;
	out << "| Value    | [" << constData() << "]" << endl;
	out << "| Len      |  " << count() << endl;
	out << "----------------------------" << endl;
	out << "| Record Count     | " << GetRecordCount() << endl;
	out << "| Field Count      | " << GetFieldCount() << endl;
	out << "----------------------------" << endl;
	out << QByteArrayMatrix::FWrite(pszFilePath) << endl;

	if (NULL == pszFilePath || QByteArray(pszFilePath).isEmpty()){
		cout << out.constData() << endl;
	}else{
		if (NULL == pszFilePath)
			return out;
		
		fstream fs;
		fs.open(pszFilePath, std::fstream::out/* | std::fstream::app*/);
		//FWrite(fs);
		
		fs.write(out.data(), out.count());
	}
	
	return out;
}

/////////////////////////////////////////////////////////////////////////////
// QPTSetStateTable
//
QPTSetStateTable::QPTSetStateTable()
{
}

QPTSetStateTable::~QPTSetStateTable()
{	
}

BOOL QPTSetStateTable::ReLoad()
{
	BOOL bReturn = FALSE;
	
	LoadInfo("tb_sys_config");
	LoadDataAll();
	//Save("c:/tb_sys_config.txt");
	
	return bReturn;
}

BOOL QPTSetStateTable::Save(const char *pszFilePath)
{
	BOOL bReturn = FALSE;
	
	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);
	
	return bReturn;
}

int	QPTSetStateTable::GetType()
{	
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;

	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");

	return val.GetFieldValue(1, 1).toInt();
}

void QPTSetStateTable::SetType(int nType)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 1, QByteArray::number(nType));

	SetFieldValue(keyVals, "Value", val);
}

int	QPTSetStateTable::GetStateID()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");

	return val.GetFieldValue(1, 2).toInt();
}

void QPTSetStateTable::SetStateID(int nID)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 2, QByteArray::number(nID));
	
	SetFieldValue(keyVals, "Value", val);
}

QByteArray QPTSetStateTable::GetPTID()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 3);
}

void QPTSetStateTable::SetPTID(const char *pt_id)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 3, QByteArray(pt_id));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetCPUID()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 4).toInt();
}

void QPTSetStateTable::SetCPUID(int nCPUID)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 4, QByteArray::number(nCPUID));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetZoneID()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 5).toInt();
}

void QPTSetStateTable::SetZoneID(int nZoneID)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 5, QByteArray::number(nZoneID));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetFlags()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 6).toInt();
}

void QPTSetStateTable::SetFlags(int nFlags)
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 6, QByteArray::number(nFlags));
	
	SetFieldValue(keyVals, "Value", val);
}

QByteArray QPTSetStateTable::GetRunnerUserID()
{
	QByteArray s;

	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_HANGOUT, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArray QPTSetStateTable::GetOperUserID()
{
	QByteArray s;
	
	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTSET_STATE_2, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArray QPTSetStateTable::GetMonitorUserID()
{
	QByteArray s;
	
	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTSET_STATE_3, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArrayMatrix QPTSetStateTable::GetWorkFlow()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	return GetFieldValue(keyVals, "reverse1");
}

QByteArrayMatrix QPTSetStateTable::GetDefaultWorkFlow()
{
	QByteArrayMatrix flow;
	
	// 功能ID, 是否可见, 用户组，用户ID;...;..
	// 102,1,101,run1;204,1;205,1;206,1;207,1;101,1
	if (1){
		flow += QByteArray::number(XJ_OPER_HANGOUT) + ",1," + QByteArray::number(XJ_USERGROUP_RUNNER) + ",;";
		flow += QByteArray::number(XJ_OPER_PTSET_STATE_2) + ",1," + QByteArray::number(XJ_USERGROUP_OPERATOR) + ",;";
		flow += QByteArray::number(XJ_OPER_PTSET_STATE_3) + ",1," + QByteArray::number(XJ_USERGROUP_MONITOR) + ",;";
		flow += QByteArray::number(XJ_OPER_PTSET_STATE_4) + ",1," + QByteArray::number(XJ_USERGROUP_RUNNER) + ",;";
		flow += QByteArray::number(XJ_OPER_PTSET_STATE_5) + ",1," + QByteArray::number(XJ_USERGROUP_OPERATOR) + ",;";
		flow += QByteArray::number(XJ_OPER_UNHANGOUT) + ",0," + QByteArray::number(XJ_USERGROUP_RUNNER) + ",";
	}
	return flow;
}

QByteArrayMatrix QPTSetStateTable::GetLogs()
{
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	return GetFieldValue(keyVals, "reverse3");
}

QByteArrayMatrix QPTSetStateTable::AddLog(int nStateID, const char *pszUserID, int nAddType/* = 0*/)
{
	QByteArrayMatrix s;
	
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	QByteArrayMatrix logs = GetFieldValue(keyVals, "reverse3");

	s.SetDelimRow(logs.GetDelimRow());
	s.SetDelimCol(logs.GetDelimCol());

	if (nAddType != 0){
		s << GetTime() << s.GetDelimCol() 
			<< pszUserID << s.GetDelimCol() 
			<< nStateID;

		SetFieldValue(keyVals, "reverse3", s);

		return s;
	}

	if (logs.contains(QByteArray::number(nStateID))){
		int iRowLog = -1;
		s = GetLog(nStateID, iRowLog);
		if (iRowLog == -1)
			return s;

		logs.SetFieldValue(iRowLog, 1, GetTime());
		logs.SetFieldValue(iRowLog, 2, QByteArray(pszUserID));
	}else{
		s << GetTime() << logs.GetDelimCol() 
			<< pszUserID << logs.GetDelimCol() 
			<< nStateID;
		logs.AppendRow(s);
	}

	SetFieldValue(keyVals, "reverse3", logs);

	return s;
}

QByteArrayMatrix QPTSetStateTable::GetLog(int nStateID)
{
	QByteArrayMatrix s;
	
	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix logs = GetFieldValue(keyVals, "reverse3");
	for (int i = 1; i <= logs.GetRowCount(); i++){
		int nRowStateID = logs.GetFieldValue(i, 3).toInt();
		if (nRowStateID != nStateID)
			continue;
		
		return logs.GetRow(i);
		break;
	}
	
	return s;
}

QByteArrayMatrix QPTSetStateTable::GetLog(int nStateID, int &iRow)
{
	QByteArrayMatrix s;

	QByteArrayMatrix keyVals;
	keyVals << PTSET_KEYNAME;
	
	QByteArrayMatrix logs = GetFieldValue(keyVals, "reverse3");
	for (int i = 1; i <= logs.GetRowCount(); i++){
		int nRowStateID = logs.GetFieldValue(i, 3).toInt();
		if (nRowStateID != nStateID)
			continue;
		
		iRow = i;
		return logs.GetRow(i);
		break;
	}

	return s;
}

int	QPTSetStateTable::GetHangoutReasonType(const char* pszHangoutReason)
{
	if (QByteArray(pszHangoutReason).trimmed() == QByteArray("定值修改")){
		return XJ_OPER_PTSET;
	}else if (QByteArray(pszHangoutReason).trimmed() == QByteArray("定值区修改")){
		return XJ_OPER_PTZONESET;
	}else if (QByteArray(pszHangoutReason).trimmed() == QByteArray("软压板修改")){
		return XJ_OPER_PTSOFTSET;
	}
	
	return 0;
}

QByteArray	QPTSetStateTable::GetHangoutReasonName(int nHangoutReasonType)
{
	switch (nHangoutReasonType){
	case XJ_OPER_PTSET:
		return QByteArray("定值修改");
	case XJ_OPER_PTZONESET:
		return QByteArray("定值区修改");
	case XJ_OPER_PTSOFTSET:
		return QByteArray("软压板修改");
	}
	
	return QByteArray("");
}

BOOL QPTSetStateTable::Next(int nNextStateID, const char* szUserID, int nFlag)
{
	ReLoad();
	
	int nCurrentStateID = GetStateID();
	if (nNextStateID <= nCurrentStateID){
		return TRUE;
	}
	
	SetStateID(nNextStateID);
	SetFlags(nFlag);
	
	char szLog[256] = {0};
	sprintf(szLog, "%s,%s,%d"
		, GetTime().constData()
		, szUserID
		, nNextStateID);
	//log.Insert(szLog);
	
	return Save();
}

BOOL QPTSetStateTable::Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag)
{
	if (nNextStateID > 1){
		return Next(nNextStateID, szUserID);
	}
	
	ReLoad();
	
	SetType(0);
	SetFlags(nFlag);
	SetCPUID(0);
	SetZoneID(0);
	
	if (nNextStateID == 1){
		SetStateID(1);
		SetPTID(szPTID);
		
		char szLog[256] = {0};
		sprintf(szLog, "%s,%s,%d"
			, GetTime().constData()
			, szUserID
			, 1);
		//log.FRead(szLog);
	}else{
		SetStateID(0);
	}
	
	return Save();
}

BOOL QPTSetStateTable::Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag)
{
	ReLoad();

	SetStateID(nNextStateID);
	SetCPUID(nCPUID);
	SetZoneID(nZoneID);
	
	char szLog[256] = {0};
	QByteArray curTime = GetTime();
	sprintf(szLog, "%s,%s,%d"
		, curTime.constData()
		, szUserID
		, nNextStateID);
	//log.Insert(szLog);
	
	//AddNewManOperator(nNextStateID, curTime.constData(), szUserID);
	
	return Save();
}

void QPTSetStateTable::Next_0(const char *pszUserID)
{
	if (NULL == pszUserID)
		return;
	
	SetType(0);
	SetStateID(XJ_OPER_UNHANGOUT);
	SetCPUID(0);
	SetZoneID(0);
	SetFlags(1);	// 通知相应操作员的界面恢复原值
	//RevertModify();	 // 数据库恢复原值
	
	// 添加工作进度
	QByteArrayMatrix slog = AddLog(XJ_OPER_UNHANGOUT, pszUserID);

	Save();
	
	// 添加历史日志
	CXJPTSetStore::GetInstance()->AddNewManOperator(XJ_OPER_UNHANGOUT, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_1(const char *pszUserID, const char *pt_id, const char * pszHangoutReason)
{
	if (NULL == pt_id || NULL == pszUserID)
		return;

	int nType = GetHangoutReasonType(pszHangoutReason);
	
	SetType(nType);
	SetStateID(XJ_OPER_HANGOUT);
	SetPTID(pt_id);
	SetCPUID(0);
	SetZoneID(0);
	SetFlags(0);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_HANGOUT, pszUserID, 1);

	Save();
	
	CXJPTSetStore::GetInstance()->AddNewManOperator(XJ_OPER_HANGOUT, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_2(int nCPU, int nZone, const char *pszUserID
									   , const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting)
{
	SetStateID(XJ_OPER_PTSET_STATE_2);
	SetCPUID(nCPU);
	SetZoneID(nZone);

	QByteArrayMatrix slog = AddLog(XJ_OPER_PTSET_STATE_2, pszUserID);
	
	Save();

	m_pData->ReLoad(arrModifyList, arrSetting);
	
	CXJPTSetStore::GetInstance()->AddNewManOperator(XJ_OPER_PTSET_STATE_2
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}


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
			//<< "由由BH51"
			<< m_pState->GetPTID()
			<< "')";
		LoadData(baSQL);
		Save("c:/tb_pt_setting_def.txt");
	}
	
	return bReturn;
}

BOOL QPTSetDataTable::ReLoad(const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting)
{
	BOOL bReturn = FALSE;

	ReLoad();

	int nCount = arrSetting.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SETTING *pts_data = (PT_SETTING*)arrSetting.GetAt(i);
		
		int nSettingID = atoi(pts_data->ID);
		int nCPUID = m_pState->GetCPUID();
		QByteArrayMatrix keyvals;
		keyvals << m_pState->GetPTID() << "," << nCPUID << "," << nSettingID;
		
		QByteArray val = pts_data->Value.GetBuffer(0);

		SetFieldValue(keyvals, "reserve3", val);
	}
		
		
	nCount = arrModifyList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);

		int nSettingID = sttpData->id;
		int nCPUID = sttpData->nCpu;
		QByteArrayMatrix keyvals;
		keyvals << m_pState->GetPTID() << "," << nCPUID << "," << nSettingID;

		QByteArrayMatrix val = GetFieldValue(keyvals, "reserve3");
		val << ", " << sttpData->str_value.c_str();

		SetFieldValue(keyvals, "reserve3", val);
	}

	return bReturn;
}

BOOL QPTSetDataTable::Save(const char *pszFilePath)
{
	BOOL bReturn = FALSE;
	
// 	QByteArrayMatrix keyVals;
// 	keyVals << "由由BH51" << keyVals.GetDelimCol()
// 		<< 2 << keyVals.GetDelimCol()
// 		<< 1010;
// 	SetFieldValue(keyVals, "reserve3", "20,19.8");
	
	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);
	
	return bReturn;
}

