// qptsetstatetable.cpp : implementation file
//

#include "stdafx.h"
#include "qptsetstatetable.h"

#include "XJStoreDefine.h"
#include "XJTagOutStore.h"
#include "XJBrowser.h"

const char* TAGOUT_KEYNAME = "TAGOUT_STATE";
const char* PTVALVSET_KEYNAME = "PTVALVSET_STATE";
const char* PTZONESET_KEYNAME = "PTZONESET_STATE";
const char* PTSOFTSET_KEYNAME = "PTSOFTSET_STATE";

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

BOOL QPTSetStateTable::Check(int nTagOutType)
{
	QByteArray val;

	QByteArrayMatrix keyvals = PTVALVSET_KEYNAME;
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
	
	int iRow = GetRowIndex(keyvals);
	if (iRow < 1){
		iRow = AddRow(keyvals);
		if (iRow > 0){
			val.clear();
			switch (nTagOutType){
			case XJ_TAGOUT_PTVALVSET:
			case XJ_TAGOUT_PTZONESET:
			case XJ_TAGOUT_PTSOFTSET:
				{
					val << "��״̬����" << GetTagOutReasonName(nTagOutType);
					SetFieldValue(iRow, "note", val);
					SetFieldValue(iRow, "reverse1", GetDefaultWorkFlow(nTagOutType));
				}
				break;
			default:
				{
					val.clear();
					val << XJ_TAGOUT_UNDEFINE << ","
						<< XJ_OPER_UNHANGOUT << ",,0,0,0";
					SetFieldValue(iRow, "Value", val);
					val.clear();
					val << "װ�ù���״̬��";
					SetFieldValue(iRow, "note", val);
				}
			}
		}

		Save("c:/tb_sys_config.txt");
		//AfxMessageBox("Added.");
	}else{
		//AfxMessageBox("Already existed.");
	}

	return ( iRow > 0 ? TRUE : FALSE);
}

int	QPTSetStateTable::GetType()
{	
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;

	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");

	return val.GetFieldValue(1, 1).toInt();
}

void QPTSetStateTable::SetType(int nType)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 1, QByteArray::number(nType));

	SetFieldValue(keyVals, "Value", val);
}

int	QPTSetStateTable::GetStateID()
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");

	return val.GetFieldValue(1, 2).toInt();
}

void QPTSetStateTable::SetStateID(int nID)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 2, QByteArray::number(nID));
	
	SetFieldValue(keyVals, "Value", val);
}

QByteArray QPTSetStateTable::GetPTID()
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 3);
}

void QPTSetStateTable::SetPTID(const char *pt_id)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 3, QByteArray(pt_id));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetCPUID()
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 4).toInt();
}

void QPTSetStateTable::SetCPUID(int nCPUID)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 4, QByteArray::number(nCPUID));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetZoneID()
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 5).toInt();
}

void QPTSetStateTable::SetZoneID(int nZoneID)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, 5, QByteArray::number(nZoneID));
	
	SetFieldValue(keyVals, "Value", val);
}

int QPTSetStateTable::GetFlags()
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, 6).toInt();
}

void QPTSetStateTable::SetFlags(int nFlags)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
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
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTVALVSET_STATE_2, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArray QPTSetStateTable::GetMonitorUserID()
{
	QByteArray s;
	
	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTVALVSET_STATE_3, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArray QPTSetStateTable::GetStateUserID(int nStateID)
{
	QByteArray s;

	QByteArrayMatrix &flow = GetWorkFlow();
	
	
	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTVALVSET_STATE_3, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArray QPTSetStateTable::GetWorkFlowStep(int nStateID)
{
	QByteArray s;
	
	
	
	int iRow = -1;
	QByteArrayMatrix &log = GetLog(XJ_OPER_PTVALVSET_STATE_3, iRow);
	if (iRow == -1)
		return s;
	
	return log.GetFieldValue(1, 2);
}

QByteArrayMatrix QPTSetStateTable::GetWorkFlow(int nTagOutType)
{
	QByteArrayMatrix keyVals;

	if (nTagOutType == - 1){
		nTagOutType = GetType();
	}
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		keyVals << PTVALVSET_KEYNAME;
		break;
	case XJ_TAGOUT_PTZONESET:
		keyVals << PTZONESET_KEYNAME;
		break;
	case XJ_TAGOUT_PTSOFTSET:
		keyVals << PTSOFTSET_KEYNAME;
		break;
	default:
		keyVals << TAGOUT_KEYNAME;
	}
	
	
	return GetFieldValue(keyVals, "reverse1");
}

QByteArrayMatrix QPTSetStateTable::GetDefaultWorkFlow(int nTagOutType)
{
	QByteArrayMatrix flow;
	
	// ������ID, ����ID, �Ƿ�ɼ�, �û��飬�û�ID;...;..
	// 102,1,101,run1;204,1;205,1;206,1;207,1;101,1
	/*
	100,102,1,101,;
	201,204,1,102,;201,205,1,103,;201,206,1,101,;201,207,1,102,;
	301,204,1,102,;301,205,1,103,;301,206,1,101,;301,207,1,102,;
	401,204,1,102,;401,205,1,103,;401,206,1,101,;401,207,1,102,;
	100,101,0,101,
	*/
	
	flow << XJ_TAGOUT_UNDEFINE << "," << XJ_OPER_HANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",;";
	
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		flow 
			//<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_HANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",;"
			<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_PTVALVSET_STATE_2 << ",1," << XJ_USERGROUP_OPERATOR << ",;"
			<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_PTVALVSET_STATE_3 << ",1," << XJ_USERGROUP_MONITOR << ",;"
			<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_PTVALVSET_STATE_4 << ",1," << XJ_USERGROUP_RUNNER << ",;"
			<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_PTVALVSET_STATE_5 << ",1," << XJ_USERGROUP_OPERATOR << ",;";
			//<< XJ_TAGOUT_PTVALVSET << "," << XJ_OPER_UNHANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",";
		break;
	case XJ_TAGOUT_PTZONESET:
		flow 
			//<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_HANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",;" 
			<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_PTZONESET_STATE_2 << ",1," << XJ_USERGROUP_OPERATOR << ",;"
			<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_PTZONESET_STATE_3 << ",1," << XJ_USERGROUP_MONITOR << ",;"
			<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_PTZONESET_STATE_4 << ",0," << XJ_USERGROUP_RUNNER << ",;"
			<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_PTZONESET_STATE_5 << ",1," << XJ_USERGROUP_OPERATOR << ",;";
			//<< XJ_TAGOUT_PTZONESET << "," << XJ_OPER_UNHANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",";
		break;
	case XJ_TAGOUT_PTSOFTSET:
		flow 
			//<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_HANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",;" 
			<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_PTSOFTSET_STATE_2 << ",1," << XJ_USERGROUP_OPERATOR << ",;"
			<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_PTSOFTSET_STATE_3 << ",1," << XJ_USERGROUP_MONITOR << ",;"
			<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_PTSOFTSET_STATE_4 << ",0," << XJ_USERGROUP_RUNNER << ",;"
			<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_PTSOFTSET_STATE_5 << ",1," << XJ_USERGROUP_OPERATOR << ",;";
			//<< XJ_TAGOUT_PTSOFTSET << "," << XJ_OPER_UNHANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",";
		break;
	}
	flow << XJ_TAGOUT_UNDEFINE << "," << XJ_OPER_UNHANGOUT << ",1," << XJ_USERGROUP_RUNNER << ",";

	return flow;
}

QByteArrayMatrix QPTSetStateTable::GetLogs()
{
	QByteArrayMatrix keyVals;

	int nTagOutType = GetType();
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		keyVals << PTVALVSET_KEYNAME;
		break;
	case XJ_TAGOUT_PTZONESET:
		keyVals << PTZONESET_KEYNAME;
		break;
	case XJ_TAGOUT_PTSOFTSET:
		keyVals << PTSOFTSET_KEYNAME;
		break;
	default:
		keyVals << TAGOUT_KEYNAME;
	}
	
	return GetFieldValue(keyVals, "reverse3");
}

QByteArrayMatrix QPTSetStateTable::AddLog(int nStateID, const char *pszUserID, int nAddType/* = 0*/)
{
	QByteArrayMatrix s;
	
	QByteArrayMatrix keyVals;
	int nTagOutType = GetType();
	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		keyVals << PTVALVSET_KEYNAME;
		break;
	case XJ_TAGOUT_PTZONESET:
		keyVals << PTZONESET_KEYNAME;
		break;
	case XJ_TAGOUT_PTSOFTSET:
		keyVals << PTSOFTSET_KEYNAME;
		break;
	default:
		keyVals << TAGOUT_KEYNAME;
	}
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
	
	QByteArrayMatrix logs = GetLogs();
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

	QByteArrayMatrix logs = GetLogs();
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

int	QPTSetStateTable::GetTagOutReasonTypeByState(int nStateID)
{
	QByteArray &reason = GetTagOutReasonNameByState(nStateID);
	return GetTagOutReasonType(reason.constData());
}

int	QPTSetStateTable::GetTagOutReasonType(const char* pszTagOutReason)
{
	if (QByteArray(pszTagOutReason).trimmed() == QByteArray("��ֵ�޸�")){
		return XJ_TAGOUT_PTVALVSET;
	}else if (QByteArray(pszTagOutReason).trimmed() == QByteArray("��ֵ���л�")){
		return XJ_TAGOUT_PTZONESET;
	}else if (QByteArray(pszTagOutReason).trimmed() == QByteArray("��ѹ��Ͷ��")){
		return XJ_TAGOUT_PTSOFTSET;
	}
	
	return 0;
}

QByteArray	QPTSetStateTable::GetTypeName()
{
	int nTagOutType = GetType();
	return GetTagOutReasonName(nTagOutType);
}

QByteArray	QPTSetStateTable::GetTagOutReasonName(int nHangoutReasonType)
{
	switch (nHangoutReasonType){
	case XJ_TAGOUT_PTVALVSET:
		return QByteArray("��ֵ�޸�");
	case XJ_TAGOUT_PTZONESET:
		return QByteArray("��ֵ���л�");
	case XJ_TAGOUT_PTSOFTSET:
		return QByteArray("��ѹ��Ͷ��");
	}
	
	return QByteArray("");
}

QByteArray	QPTSetStateTable::GetTagOutReasonNameByState(int nStateID)
{
	if (nStateID >= XJ_TAGOUT_PTVALVSET && nStateID <= XJ_OPER_PTVALVSET_STATE_5)
		return GetTagOutReasonName(XJ_TAGOUT_PTVALVSET);

	if (nStateID >= XJ_TAGOUT_PTZONESET && nStateID <= XJ_OPER_PTZONESET_STATE_5)
		return GetTagOutReasonName(XJ_TAGOUT_PTZONESET);

	if (nStateID >= XJ_TAGOUT_PTSOFTSET && nStateID <= XJ_OPER_PTSOFTSET_STATE_5)
		return GetTagOutReasonName(XJ_TAGOUT_PTSOFTSET);
	
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

	// ���ӹ�������
	QByteArrayMatrix slog = AddLog(XJ_OPER_UNHANGOUT, pszUserID);

	// ������ʷ��־
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_UNHANGOUT, slog.GetFieldValue(1, 1).constData(), pszUserID);
	
	SetType(XJ_TAGOUT_UNDEFINE);
	SetStateID(XJ_OPER_UNHANGOUT);
	SetCPUID(0);
	SetZoneID(0);
	SetFlags(1);	// ֪ͨ��Ӧ����Ա�Ľ���ָ�ԭֵ
	//RevertModify();	 // ���ݿ�ָ�ԭֵ
	
	Save();
}

void QPTSetStateTable::Next_1(const char *pszUserID, const char *pt_id, const char * pszHangoutReason)
{
	if (NULL == pt_id || NULL == pszUserID)
		return;

	int nType = GetTagOutReasonType(pszHangoutReason);
	
	SetType(nType);
	SetStateID(XJ_OPER_HANGOUT);
	SetPTID(pt_id);
	SetCPUID(0);
	SetZoneID(0);
	SetFlags(0);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_HANGOUT, pszUserID, 1);

	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_HANGOUT, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_2(int nCPU, int nZone, const char *pszUserID
									   , const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_2);
	SetCPUID(nCPU);
	SetZoneID(nZone);

	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_2, pszUserID);
	
	Save();

	m_pData->ReLoad(arrModifyList, arrSetting);
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_2
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_3(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_3);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_3, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_3
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_4(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_4);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_4, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_4
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_5(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_5);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_5, pszUserID);
	
	Save();

	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_5
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::RevertTo_PTSet_State_1(int nFrom_PTSetStateID, const char* pszUserID, QByteArray &strMs)
{
	SetStateID(XJ_OPER_HANGOUT);

	int nPTSetType = GetType();
	QByteArrayMatrix log = GetLog(XJ_OPER_HANGOUT);
	
	QByteArrayMatrix keyVals;
	keyVals << PTVALVSET_KEYNAME;
	
	SetFieldValue(keyVals, "reverse3", log);
	Save();

	CXJTagOutStore::GetInstance()->AddNewManOperator(nFrom_PTSetStateID
		, GetTime().constData(), pszUserID, OPER_FAILD, strMs.constData());
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
			//<< "����BH51"
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
// 	keyVals << "����BH51" << keyVals.GetDelimCol()
// 		<< 2 << keyVals.GetDelimCol()
// 		<< 1010;
// 	SetFieldValue(keyVals, "reserve3", "20,19.8");
	
	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);
	
	return bReturn;
}
