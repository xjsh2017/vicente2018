// qptsetstatetable.cpp : implementation file
//

#include "StdAfx.h"
#include "qptsetstatetable.h"

#include "XJStoreDefine.h"
#include "XJTagOutStore.h"
#include "XJBrowser.h"

const char* NAME_TB_SYS_CFG = "tb_sys_config";
const char* NAME_TB_PT_SOFTBOARD_DEF = "tb_pt_softboard_def";

const char* SAVE_PATH_TB_PT_SOFTBOARD_DEF = "c:/tb_pt_softboard_def";

const char* TAGOUT_KEYNAME = "STATE_TAGOUT";
const char* PTVALVSET_KEYNAME = "STATE_PTSET_VALV";
const char* PTZONESET_KEYNAME = "STATE_PTSET_ZONE";
const char* PTSOFTSET_KEYNAME = "STATE_PTSET_SOFT";

const int COL_WORKFLOW_TAGOUT_ID = 1;
const int COL_WORKFLOW_STATE_ID = 2;
const int COL_WORKFLOW_USERGROUP_ID = 4;
const int COL_WORKFLOW_ENABLE = 3;
const int COL_WORKFLOW_USER_ID = 5;

const int COL_TAGOUT_LASTTYPE = 7;

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
					val << "子状态机：" << GetTagOutReasonName(nTagOutType);
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
					val << "装置挂牌状态机";
					SetFieldValue(iRow, "note", val);
				}
			}
		}
		
		Save();
		//Save("c:/tb_sys_config.txt");
		//AfxMessageBox("Added.");
	}else{
		//AfxMessageBox("Already existed.");
	}

	return ( iRow > 0 ? TRUE : FALSE);
}

QByteArray QPTSetStateTable::GetTagOutValue(int iCol)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;

	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	return val.GetFieldValue(1, iCol);
}

void QPTSetStateTable::SetTagOutValue(int iCol, QByteArray &s)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, iCol, s);
	
	SetFieldValue(keyVals, "Value", val);
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

int	QPTSetStateTable::GetLastType()
{	
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	
	return val.GetFieldValue(1, COL_TAGOUT_LASTTYPE).toInt();
}

void QPTSetStateTable::SetLastType(int nType)
{
	QByteArrayMatrix keyVals;
	keyVals << TAGOUT_KEYNAME;
	
	QByteArrayMatrix val = GetFieldValue(keyVals, "Value");
	val.SetFieldValue(1, COL_TAGOUT_LASTTYPE, QByteArray::number(nType));
	
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

	if (val.GetFieldValue(1, 4).isEmpty())
		return -1;
	
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
	
	if (val.GetFieldValue(1, 5).isEmpty())
		return -1;
	
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

	if (val.GetFieldValue(1, 5).isEmpty())
		return -1;
	
	return val.GetFieldValue(1, 6).toInt();
}

int QPTSetStateTable::GetTagOutRowIdx(int nTagOutType/* = XJ_TAGOUT_UNDEFINE*/)
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
	
	return GetRowIndex(keyvals);
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

QByteArray QPTSetStateTable::GetWorkFlowUserID(int nTagOutType, int nStateID)
{
	QByteArray s;

	if (-1 == nTagOutType)
		nTagOutType = GetTagOutReasonTypeByState(nStateID);
	if (nTagOutType == 0)
		return s;

	QByteArrayMatrix &flow = GetWorkFlow(nTagOutType);

	for (int i = 1; i <= flow.GetRowCount(); i++){
		int nFlowStateID = flow.GetFieldValue(i, COL_WORKFLOW_STATE_ID).toInt();
		if (nStateID != nFlowStateID)
			continue;
		
		return flow.GetFieldValue(i, COL_WORKFLOW_USER_ID);
		break;
	}

	return s;
}

void QPTSetStateTable::SetWorkFlowUserID(int nTagOutType, int nStateID, const char* pszUserID)
{
	QByteArray s;
	
	if (-1 == nTagOutType)
		nTagOutType = GetTagOutReasonTypeByState(nStateID);
	if (nTagOutType == 0)
		return;

	QByteArrayMatrix &flow = GetWorkFlow(nTagOutType);
	int nTagOutRowIdx = GetTagOutRowIdx(nTagOutType);
	
	for (int i = 1; i <= flow.GetRowCount(); i++){
		int nFlowStateID = flow.GetFieldValue(i, COL_WORKFLOW_STATE_ID).toInt();
		if (nStateID != nFlowStateID)
			continue;
		
		flow.SetFieldValue(i, COL_WORKFLOW_USER_ID, pszUserID);
		
		SetFieldValue(nTagOutRowIdx, "reverse1", flow);
		break;
	}
}

void QPTSetStateTable::SetWorkFlowUserID(int nTagOutType, int nStateID, QByteArray &userID)
{
	SetWorkFlowUserID(nTagOutType, nStateID, userID.constData());
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
	
	// 功能组ID, 功能ID, 是否可见, 用户组，用户ID;...;..
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
	if (nTagOutType == XJ_TAGOUT_UNDEFINE){
		nTagOutType = GetLastType();
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
	if (QByteArray(pszTagOutReason).trimmed() == QByteArray("定值修改")){
		return XJ_TAGOUT_PTVALVSET;
	}else if (QByteArray(pszTagOutReason).trimmed() == QByteArray("定值区切换")){
		return XJ_TAGOUT_PTZONESET;
	}else if (QByteArray(pszTagOutReason).trimmed() == QByteArray("软压板投退")){
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
		return QByteArray("定值修改");
	case XJ_TAGOUT_PTZONESET:
		return QByteArray("定值区切换");
	case XJ_TAGOUT_PTSOFTSET:
		return QByteArray("软压板投退");
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

	// 添加工作进度
	QByteArrayMatrix slog = AddLog(XJ_OPER_UNHANGOUT, pszUserID);

	// 添加历史日志
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_UNHANGOUT, slog.GetFieldValue(1, 1).constData(), pszUserID);

	SetWorkFlowUserID(GetType(), XJ_OPER_UNHANGOUT, pszUserID);
	SetLastType(GetType());

	SetType(XJ_TAGOUT_UNDEFINE);
	SetStateID(XJ_OPER_UNHANGOUT);
	SetCPUID(0);
	SetZoneID(0);
	SetFlags(1);	// 通知相应操作员的界面恢复原值
	//RevertModify();	 // 数据库恢复原值
	
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
	SetWorkFlowUserID(nType, XJ_OPER_HANGOUT, pszUserID);
	
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

	SetWorkFlowUserID(GetType(), XJ_OPER_PTVALVSET_STATE_2, pszUserID);

	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_2, pszUserID);
	
	Save();

	if (m_pData_Valv)
		m_pData_Valv->ReLoad(GetPTID(), nCPU, nZone, arrModifyList, arrSetting);
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_2
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_3(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_3);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTVALVSET_STATE_3, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_3, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_3
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_4(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_4);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTVALVSET_STATE_4, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_4, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_4
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSet_State_5(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTVALVSET_STATE_5);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTVALVSET_STATE_5, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTVALVSET_STATE_5, pszUserID);
	
	Save();

	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTVALVSET_STATE_5
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_ZONE_STATE_2(int nCPU, int nZone, const char *pszUserID
										  , QByteArray &oldZoneValue, QByteArray &newZoneValue)
{
	SetStateID(XJ_OPER_PTZONESET_STATE_2);
// 	SetCPUID(nCPU);
// 	SetZoneID(nZone);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTZONESET_STATE_2, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTZONESET_STATE_2, pszUserID);
	
	Save();
	
	if (m_pData_Zone)
		m_pData_Zone->ReLoad(GetPTID(), nCPU, nZone, oldZoneValue, newZoneValue);
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTZONESET_STATE_2
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_ZONE_STATE_3(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTZONESET_STATE_3);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTZONESET_STATE_3, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTZONESET_STATE_3, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTZONESET_STATE_3
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_ZONE_STATE_4(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTZONESET_STATE_4);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTZONESET_STATE_4, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTZONESET_STATE_4, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTZONESET_STATE_4
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_ZONE_STATE_5(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTZONESET_STATE_5);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTZONESET_STATE_5, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTZONESET_STATE_5, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTZONESET_STATE_5
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_SOFT_STATE_2(int nCPU, const char *pszUserID
											   , const MODIFY_LIST &arrModifyList
								, const PT_SOFTBOARD_LIST &arrSoftBoard)
{
	SetStateID(XJ_OPER_PTSOFTSET_STATE_2);
	SetCPUID(nCPU);
	//SetZoneID(nZone);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTSOFTSET_STATE_2, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTSOFTSET_STATE_2, pszUserID);
	
	Save();
	
	if (m_pData_Soft)
		m_pData_Soft->ReLoad(GetPTID(), nCPU, arrModifyList, arrSoftBoard);
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTSOFTSET_STATE_2
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_SOFT_STATE_3(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTSOFTSET_STATE_3);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTSOFTSET_STATE_3, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTSOFTSET_STATE_3, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTSOFTSET_STATE_3
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_SOFT_STATE_4(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTSOFTSET_STATE_4);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTSOFTSET_STATE_4, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTSOFTSET_STATE_4, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTSOFTSET_STATE_4
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::Next_PTSET_SOFT_STATE_5(const char *pszUserID)
{
	SetStateID(XJ_OPER_PTSOFTSET_STATE_5);
	
	SetWorkFlowUserID(GetType(), XJ_OPER_PTSOFTSET_STATE_5, pszUserID);
	
	QByteArrayMatrix slog = AddLog(XJ_OPER_PTSOFTSET_STATE_5, pszUserID);
	
	Save();
	
	CXJTagOutStore::GetInstance()->AddNewManOperator(XJ_OPER_PTSOFTSET_STATE_5
		, slog.GetFieldValue(1, 1).constData(), pszUserID);
}

void QPTSetStateTable::RevertTo_PTSet_State_1(int nFrom_PTSetStateID, const char* pszUserID, QByteArray &strMs)
{
	SetStateID(XJ_OPER_HANGOUT);

	int nPTSetType = GetType();
	QByteArrayMatrix log = GetLog(XJ_OPER_HANGOUT);
	
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
	
	SetFieldValue(keyVals, "reverse3", log);
	Save();

	switch (nTagOutType){
	case XJ_TAGOUT_PTVALVSET:
		{
			m_pData_Valv->RevertModifiy();
			m_pData_Valv->Save();
		}
		break;
	case XJ_TAGOUT_PTZONESET:
		{
			m_pData_Zone->RevertModifiy();
			m_pData_Zone->Save();
		}
		break;
	case XJ_TAGOUT_PTSOFTSET:
		{
			m_pData_Soft->RevertModifiy();
			m_pData_Soft->Save();
		}
		break;
	default:;
	}

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

BOOL QPTSetDataTable::ReLoad(QByteArray &pt_id)
{
	BOOL bReturn = FALSE;
	
	LoadInfo("tb_pt_setting_def");
	//LoadDataAll();
	
	QByteArray baPTID = pt_id;
	if (baPTID.isEmpty())
		baPTID = m_pState->GetPTID();
	//if (!m_card.GetPTID().isEmpty()){
	if (1){
		QByteArray baSQL;
		baSQL << "SELECT * FROM tb_pt_setting_def WHERE pt_id IN ('" 
			//<< "由由BH51"
			<< baPTID
			<< "')";
		LoadData(baSQL);
		//Save("c:/tb_pt_setting_def.txt");
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

BOOL QPTSetDataTable::ReLoad(QByteArray &pt_id, int nCPU, int nZone, const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting)
{
	BOOL bReturn = FALSE;
	
	ReLoad(pt_id);
	
	int nCount = arrSetting.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SETTING *pts_data = (PT_SETTING*)arrSetting.GetAt(i);
		
		int nSettingID = atoi(pts_data->ID);
		QByteArrayMatrix keyvals;
		keyvals << pt_id << "," << nCPU << "," << nSettingID;
		
		QByteArray val = pts_data->Value.GetBuffer(0);
		if (val.isEmpty())
			continue;
		
		SetFieldValue(keyvals, "reserve3", val);
	}
	
	
	nCount = arrModifyList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);
		
		int nSettingID = sttpData->id;
		int nCPUID = sttpData->nCpu;
		QByteArrayMatrix keyvals;
		keyvals << pt_id << "," << nCPUID << "," << nSettingID;
		
		QByteArrayMatrix val = GetFieldValue(keyvals, "reserve3");
		val << ", " << sttpData->str_value.c_str();
		
		SetFieldValue(keyvals, "reserve3", val);
	}
	
	return TRUE;
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

BOOL QPTSetDataTable::RevertModifiy()	// 修改值列清空保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(i, "reserve3");
		QByteArray &old = val.GetFieldValue(1, 1);
		
		SetFieldValue(i, "reserve3", old);
	}

	return TRUE;
}

BOOL QPTSetDataTable::SaveModify()	// 修改值替换原值保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(1, "reserve3");
		QByteArray Val1 = val.GetFieldValue(1, 1);
		QByteArray Val2 = val.GetFieldValue(1, 2);

		QByteArray newVal;
		if (!Val2.isEmpty()){
			newVal = Val2;
		}else{
			newVal = Val1;
		}
			
		SetFieldValue(i, "reserve3", newVal);
	}
	
	return TRUE;
}

void QPTSetDataTable::UnitTest_01()
{
	QByteArrayMatrix keyVals;
	
	ReLoad();
	
	keyVals << "由由BH51" << keyVals.GetDelimCol()
		<< 2 << keyVals.GetDelimCol()
		<< 1010;
	
	SetFieldValue(keyVals, "reserve3", "20,19.8");
	SetFieldValue("由由BH51,2,1011", "reserve3", "10.6");
	SetFieldValue("由由BH51,2,1014", "reserve3", "30,52.3");
	Save("c:/tb_pt_setting_def.txt");
}


////////////////////////////////////////////////////////////
// QPTZoneDataTable
//
QPTZoneDataTable::QPTZoneDataTable()
{
}

QPTZoneDataTable::~QPTZoneDataTable()
{
	
}

BOOL QPTZoneDataTable::ReLoad(QByteArray &pt_id)
{
	BOOL bReturn = FALSE;

	if (!m_pState){
		AfxMessageBox("no QPTSetStateTable instance assigned to QPTZoneDataTable");
		return FALSE;
	}
	
	LoadInfo("tb_pt_zone_def");
	//LoadDataAll();
	
	QByteArray baPTID = pt_id;
	if (baPTID.isEmpty())
		baPTID = m_pState->GetPTID();
	//if (!m_card.GetPTID().isEmpty()){
	if (1){
		QByteArray baSQL;
		baSQL << "SELECT * FROM tb_pt_zone_def WHERE pt_id IN ('" 
			//<< "由由BH51"
			<< baPTID
			<< "')";
		LoadData(baSQL);
		//AfxMessageBox("loaded!");
		Save("c:/tb_pt_zone_def.txt");
	}
	
	return bReturn;
}

BOOL QPTZoneDataTable::ReLoad(QByteArray &pt_id, int nCPU, int nZone
							  , QByteArray &oldZoneValue
							  , QByteArray &newZoneValue)
{
	BOOL bReturn = FALSE;
	
	ReLoad(pt_id);

	QByteArray chagVal;
	chagVal << oldZoneValue << "," << newZoneValue;
	SetFieldValue(1, "reserve3", chagVal);
	Save("c:/tb_pt_zone_def.txt");
	
	return TRUE;
}

BOOL QPTZoneDataTable::Save(const char *pszFilePath)
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

BOOL QPTZoneDataTable::RevertModifiy()	// 修改值列清空保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(i, "reserve3");
		QByteArray &old = val.GetFieldValue(1, 1);
		
		SetFieldValue(i, "reserve3", old);
	}

	return TRUE;
}

BOOL QPTZoneDataTable::SaveModify()	// 修改值替换原值保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(1, "reserve3");
		QByteArray Val1 = val.GetFieldValue(1, 1);
		QByteArray Val2 = val.GetFieldValue(1, 2);

		QByteArray newVal;
		if (!Val2.isEmpty()){
			newVal = Val2;
		}else{
			newVal = Val1;
		}
			
		SetFieldValue(i, "reserve3", newVal);
	}
	
	return TRUE;
}

void QPTZoneDataTable::UnitTest_01()
{
	QByteArrayMatrix keyVals;
	
	ReLoad();
	
	keyVals << "由由BH51" << keyVals.GetDelimCol()
		<< 2 << keyVals.GetDelimCol()
		<< 60001;
	
	SetFieldValue(keyVals, "reserve3", "1,5");
	Save("c:/tb_pt_zone_def.txt");
}


////////////////////////////////////////////////////////////
// QPTSoftDataTable
//
QPTSoftDataTable::QPTSoftDataTable()
{
}

QPTSoftDataTable::~QPTSoftDataTable()
{
	
}

BOOL QPTSoftDataTable::ReLoad(QByteArray &pt_id)
{
	BOOL bReturn = FALSE;

	if (!m_pState){
		AfxMessageBox("no QPTSetStateTable instance assigned to QPTSoftDataTable");
		return FALSE;
	}
	
	LoadInfo(NAME_TB_PT_SOFTBOARD_DEF);
	//LoadDataAll();
	
	QByteArray baPTID = pt_id;
	if (baPTID.isEmpty())
		baPTID = m_pState->GetPTID();
	//if (!m_card.GetPTID().isEmpty()){
	if (1){
		QByteArray baSQL;
		baSQL << "SELECT * FROM " << NAME_TB_PT_SOFTBOARD_DEF 
			<< " WHERE pt_id IN ('" 
			//<< "由由BH51"
			<< baPTID
			<< "')";
		LoadData(baSQL);
		//AfxMessageBox("loaded!");
		Save(SAVE_PATH_TB_PT_SOFTBOARD_DEF);
	}
	
	return bReturn;
}

BOOL QPTSoftDataTable::ReLoad(QByteArray &pt_id, int nCPU
							, const MODIFY_LIST &arrModifyList, const PT_SOFTBOARD_LIST &arrSoftBoard)
{
	BOOL bReturn = FALSE;
	
	ReLoad(pt_id);

	int nCount = arrSoftBoard.GetSize();
	for (int i = 0; i < nCount; i++){
		PT_SOFTBOARD *pts_data = (PT_SOFTBOARD*)arrSoftBoard.GetAt(i);
		
		int nID = atoi(pts_data->ID);
		QByteArrayMatrix keyvals;
		keyvals << pt_id << "," << nCPU << "," << nID;
		
		QByteArray val = pts_data->Value.GetBuffer(0);
		if (val.isEmpty())
			continue;
		
		SetFieldValue(keyvals, "reserve3", val);
	}
	
	nCount = arrModifyList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);
		
		int nID = sttpData->id;
		int nCPUID = sttpData->nCpu;
		QByteArrayMatrix keyvals;
		keyvals << pt_id << "," << nCPUID << "," << nID;
		
		QByteArrayMatrix val = GetFieldValue(keyvals, "reserve3");
		val << ", " << sttpData->str_value.c_str();
		
		SetFieldValue(keyvals, "reserve3", val);
	}

	Save(SAVE_PATH_TB_PT_SOFTBOARD_DEF);
	
	return TRUE;
}

BOOL QPTSoftDataTable::Save(const char *pszFilePath)
{
	BOOL bReturn = FALSE;
	
	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);
	
	return bReturn;
}

BOOL QPTSoftDataTable::RevertModifiy()	// 修改值列清空保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(i, "reserve3");
		QByteArray &old = val.GetFieldValue(1, 1);
		
		SetFieldValue(i, "reserve3", old);
	}

	return TRUE;
}

BOOL QPTSoftDataTable::SaveModify()	// 修改值替换原值保存
{
	BOOL bReturn = FALSE;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArrayMatrix val = GetFieldValue(1, "reserve3");
		QByteArray Val1 = val.GetFieldValue(1, 1);
		QByteArray Val2 = val.GetFieldValue(1, 2);

		QByteArray newVal;
		if (!Val2.isEmpty()){
			newVal = Val2;
		}else{
			newVal = Val1;
		}
			
		SetFieldValue(i, "reserve3", newVal);
	}
	
	return TRUE;
}

void QPTSoftDataTable::UnitTest_01()
{
	QByteArrayMatrix keyVals;
	
	ReLoad();
	
	keyVals << "由由BH51" << keyVals.GetDelimCol()
		<< 2 << keyVals.GetDelimCol()
		<< 40005;
	
	SetFieldValue(keyVals, "reserve3", "0,1");
	Save(SAVE_PATH_TB_PT_SOFTBOARD_DEF);
}