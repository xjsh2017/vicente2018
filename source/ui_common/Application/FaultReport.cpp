// FaultReport.cpp : implementation file
//

#include "FaultReport.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CFaultReport

/****************************************************
Date:2011/10/17  Author:LYH
函数名:   CFaultReport	
返回值:   	
功能概要: 
参数: CDataEngine * pDataEngine	
*****************************************************/
CFaultReport::CFaultReport( CDataEngine* pDataEngine ) : CXJNotify(pDataEngine)
{
	/** @brief           故障包ID*/
	m_nID = -1;
	/** @brief           归档名称*/
	m_sName = "";
	/** @brief           一次设备ID*/
	m_sPriDeviceID = "";
	/** @brief           一次设备指针*/
	m_pPriDevice = NULL;
	/** @brief           故障测距*/
	m_sFaultDistance = "";
	/** @brief           故障类型*/
	m_nFaultType = 0;
	/** @brief           厂站1ID*/
	m_sStation1ID = "";
	/** @brief           厂站2ID*/
	m_sStation2ID = "";
	/** @brief           事件等级*/
	m_nEventLevel = 1;
	m_nmsFaultTime = 0;

	m_pDecisionReport = NULL;
	m_bLoadDecision = TRUE;

	/** @brief           厂站1指针*/
	m_pStation1 = NULL;
	/** @brief           厂站2指针*/
	m_pStation2 = NULL;
	/** @brief           一次设备指针*/
	m_pPriDevice = NULL;

	m_bLoadDetail = FALSE;

	m_bIsSaved = 0;

	m_listChr.RemoveAll();
	m_listSign.RemoveAll();
	m_listAlarm.RemoveAll();
	m_listPTOSC.RemoveAll();
	m_listWROSC.RemoveAll();
	m_listOSCList.RemoveAll();

	m_listSec.RemoveAll();
	/** @brief           零时动作信号量数组*/
	m_listTempSign.RemoveAll();
	/** @brief           零时故障量数组*/
	m_listTempChr.RemoveAll();
	/** @brief           零时保护录波数组*/
	m_listTempPTOSC.RemoveAll();
	m_pTempSec = NULL;

	m_bShowReturn = TRUE;
	m_bFilterDistance = FALSE;

	CTime tmMin(1971, 1, 1, 1, 1, 1);
	m_tmFaultTime = tmMin;
}

//##ModelId=49B87B9A005E
CFaultReport::~CFaultReport()
{
	if(m_pDecisionReport != NULL)
	{
		delete m_pDecisionReport;
		m_pDecisionReport = NULL;
	}
	//清除详细信息
	ClearDetail();
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：从数据库载入故障包信息
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	数据集指针
		   Param2
**************************************************************/
//##ModelId=49B87B9A010E
BOOL CFaultReport::LoadFromDB( CMemSet* pMemset )
{
	// 判断数据有效性
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0); //ID
	m_nID = atoi(str);
	m_sName = pMemset->GetValue(1); //故障包名称
	m_sPriDeviceID = pMemset->GetValue(2); //一次设备ID
	str = pMemset->GetValue(3); //故障时间
	m_tmFaultTime = StringToTime14(str);
	m_sFaultDistance = pMemset->GetValue(4); //故障测距
	str = pMemset->GetValue(5); //故障类型
	m_nFaultType = atoi(str);
	m_sStation1ID = pMemset->GetValue(6); //厂站1ID
	m_sStation2ID = pMemset->GetValue(7); //厂站2ID
	str = pMemset->GetValue(8); 
	m_nEventLevel = atoi(str); //事件等级
	str = pMemset->GetValue(13);
	m_nmsFaultTime = atoi(str); //故障时间毫秒值
	str = pMemset->GetValue(9);
	m_bIsSaved = atoi(str);  //是否存档
    m_sFaultReason = pMemset->GetValue(11);
	str = pMemset->GetValue(12);
	m_tmCreateTime = StringToTime14(str);
	

	if(!BuildReleation())
	{
		WriteLog("CFaultReport::LoadFromDB 载入数据失败");
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildReleation()
 功能概要：建立与设备的关联
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0119
BOOL CFaultReport::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	//判断数据有效性
	/*
	if(m_sPriDeviceID == "")
		return FALSE;
		*/
	//不知道一次设备类型, 只能都查找了
	//线路
	m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_LINE);
	if(m_pPriDevice == NULL)
	{
		//变压器
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_TRANS);
	}
	if(m_pPriDevice == NULL)
	{
		//母线
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BUS);
	}
	if(m_pPriDevice == NULL)
	{
		//开关
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BREAK);
	}
	if(m_pPriDevice == NULL)
	{
		//电容器
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_CAP);
	}
	if(m_pPriDevice == NULL)
	{
		//电抗器
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_REACTOR);
	}
	if(m_pPriDevice == NULL)
	{
		//发电机
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_GEN);
	}
	if(m_pPriDevice == NULL)
	{
		//刀闸
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_SWITCH);
	}
	if(m_pPriDevice == NULL)
	{
		//保护管理机
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_PMS);
	}
	if(m_pPriDevice == NULL)
	{
		//间隔
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BAY);
	}

	//查找不到
//	MYASSERT_BOOL(m_pPriDevice);
	m_pStation1 = (CStationObj*)m_pDataEngine->FindDevice(m_sStation1ID, TYPE_STATION);
	m_pStation2 = (CStationObj*)m_pDataEngine->FindDevice(m_sStation2ID, TYPE_STATION);
	return TRUE;
}

/*************************************************************
 函 数 名：LoadDetail()
 功能概要：载入故障报告详细信息
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	是否强制重新载入
		   Param2
**************************************************************/
//##ModelId=49B87B9A011A
BOOL CFaultReport::LoadDetail( BOOL bForce /*= FALSE*/ )
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	//已被载入, 且不重新载入
	if(m_bLoadDetail && !bForce)
		return TRUE;
	//清除之前的信息
	ClearDetail();

	BOOL bReturn = TRUE;

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//根据 MainRecID查找所有字段
	CString str;		
	//条件
	
	//MainRecID
	Condition condition2;
	str.Format("MAINRECID = %d", m_nID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("order by PTID, FaultStartTime");
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CFaultReport::LoadDetail, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CFaultReport::LoadDetail, 读取到%d条事件记录", nCount);
		WriteLog(sLog, XJ_LOG_LV3);
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CFaultEvent* pEvent = new CFaultEvent(m_pDataEngine);
			if(!pEvent->LoadFromDB(pMemset))
			{
				delete pEvent;
				pMemset->MoveNext();
				continue;
			}
			
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//增加特征量中故障序号和故障测距==0的过滤
				int nEventdef = atoi(pEvent->m_sEventDef);
				if(m_bFilterNULL)
				{
					
					if(nEventdef == 94) //故障序号
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				if(m_bFilterDistance)
				{
					if(nEventdef == 4) //故障测距
					{
						int nEventConnect = atoi(pEvent->m_sEventContent);
						if(nEventConnect == 0)
						{
							delete pEvent;
							pMemset->MoveNext();
							continue;
						}
					}
				}
				//增加III类事件过滤
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listChr.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
			{
				if(!m_bShowReturn)
				{
					//去掉返回信息
					if(pEvent->IsReturnEvent())
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				//增加III类事件过滤
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listSign.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
			{
				if(!m_bShowReturn)
				{
					//去掉返回信息
					if(pEvent->IsReturnEvent())
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				//增加III类事件过滤
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listAlarm.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
			{
				if(pEvent->m_pSec == NULL)
				{
					delete pEvent;
					pMemset->MoveNext();
					continue;
				}
				if(pEvent->m_pSec->m_nType == TYPE_PROTECT)
					m_listPTOSC.Add(pEvent);
				else
					m_listWROSC.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_OSCLIST)
			{
				m_listOSCList.Add(pEvent);
			}
			//加入到二次设备
			//AddSec(pEvent->m_pSec);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CFaultReport::LoadDetail,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	m_bLoadDetail = TRUE;

	CString strLog;
	strLog.Format("CFaultReport::LoadDetail,故障量%d条,信号量%d条,保护录波%d条,录波器录波%d条", m_listChr.GetSize(), m_listSign.GetSize(), m_listPTOSC.GetSize(), m_listWROSC.GetSize());
	WriteLog(strLog);

	//排序
	SortActionSign();
	//载入决策信息
	if(m_bLoadDecision)
		LoadDecision();

	return bReturn;
}

/*************************************************************
 函 数 名：ClearDetail()
 功能概要：清除详细信息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0128
void CFaultReport::ClearDetail()
{
	//清空各链表
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		delete pEvent;
	}
	m_listChr.RemoveAll();
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
	//告警事件
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//保护录波
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		delete pEvent;
	}
	m_listPTOSC.RemoveAll();
	//录波器录波
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		delete pEvent;
	}
	m_listWROSC.RemoveAll();
	//录波列表
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		delete pEvent;
	}
	m_listOSCList.RemoveAll();

	m_listTempChr.RemoveAll();
	m_listTempSign.RemoveAll();
	m_listTempPTOSC.RemoveAll();
	m_pTempSec = NULL;

	//二次设备列表
	m_listSec.RemoveAll();
}

/*************************************************************
 函 数 名：GetFaultType()
 功能概要：得到故障类型的文字描述
 返 回 值: void
 参    数：param1	故障类型文字描述
		   Param2
**************************************************************/
//##ModelId=49B87B9A0129
CString CFaultReport::GetFaultTypeString(int nFaultType)
{
	CString strReturn = "";
	switch(nFaultType)
	{
	case 0:
		strReturn = "";
		break;
	case 1:
		strReturn = "A相接地";
		break;
	case 2:
		strReturn = "B相接地";
		break;
	case 3:
		strReturn = "C相接地";
		break;
	case 4:
		strReturn = "AB两相短路";
		break;
	case 5:
		strReturn = "BC两相短路";
		break;
	case 6:
		strReturn = "CA两相短路";
		break;
	case 7:
		strReturn = "AB两相短路接地";
		break;
	case 8:
		strReturn = "BC两相短路接地";
		break;
	case 9:
		strReturn = "CA两相短路接地";
		break;
	case 10:
		strReturn = "ABC三相短路";
		break;
	case 11:
		strReturn = "ABC三相短路接地";
		break;
	default:
		strReturn = "未知";
		break;
	}
	return strReturn;
}


BOOL CFaultReport::SaveToDB()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	//查找最新的num条记录
	//组建查询条件
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//指定ID,更新CaseName, PriDeviceID, FaultTime, FaultLocation, FaultType, STATION1_ID, STATION2_ID
	
	CString str;	
	//字段
	//CaseName
	BLOB_FIELD Field1;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field1, "CaseName", EX_STTP_DATA_TYPE_STRING, m_sName);
 
	//PriDeviceID
	BLOB_FIELD Field2;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field2, "PriDeviceID", EX_STTP_DATA_TYPE_STRING, m_sPriDeviceID);

	//FaultTime
	BLOB_FIELD Field3;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field3, "FaultTime", EX_STTP_DATA_TYPE_TIME, m_tmFaultTime.Format("%Y%m%d%H%M%S"));

	//FaultLocation
	BLOB_FIELD Field4;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field4, "FaultLocation", EX_STTP_DATA_TYPE_STRING, m_sFaultDistance);

	//FaultType
	BLOB_FIELD Field5;
	str.Format("%d", m_nFaultType);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field5, "FaultType", EX_STTP_DATA_TYPE_INT, str);

	//STATION1_ID
    BLOB_FIELD Field6;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field6, "STATION1_ID", EX_STTP_DATA_TYPE_STRING, m_sStation1ID);

	//STATION2_ID
	BLOB_FIELD Field7;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field7, "STATION2_ID", EX_STTP_DATA_TYPE_STRING, m_sStation2ID);

	//REVERSE1
	BLOB_FIELD Field9;
	str.Format("%d", m_bIsSaved);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field9, "REVERSE1", EX_STTP_DATA_TYPE_INT, str);

    //REVERSE3
    BLOB_FIELD Field11;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field11, "REVERSE3", EX_STTP_DATA_TYPE_STRING, m_sFaultReason);


	//FaultMsTime
    BLOB_FIELD Field13;
	CString strMs;
	strMs.Format("%d", m_nmsFaultTime);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field13, "FaultMsTime", EX_STTP_DATA_TYPE_INT, strMs);
	
	//条件
	//id
	BLOB_CONDITION condition2;
	str.Format("ID = %d", m_nID);
	m_pDataEngine->GetDBEngine()->SetBlobCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJUpdateBlogData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
		if (nResult)
		{
			WriteLog("CFaultReport::SaveToDB 更新结果成功\n");
		}
	}
	catch (...)
	{
		WriteLog("CFaultReport::SaveToDB, 更新失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("CFaultReport::SaveToDB,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;

	return bReturn;
}


/*************************************************************
 CreateDate: 13:1:2010     Author:LYH
 函 数 名: MultiSaveEvent()
 功能概要: 批量保存事件
 返 回 值: 保存成功0,删除数据失败返回1,保存数据失败返回2
 参    数: param1 
		   Param2 
**************************************************************/
int CFaultReport::MultiSaveEvent()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	if(!DeleteAllEventFromDB())
	{
		//删除失败
		WriteLog("CFaultReport::MultiSaveEvent, 删除旧数据失败");
		return 1;
	}
	//循环所有选择项
	STTP_FULL_DATA sttpData;
	//清空结构
	zero_sttp_full_data(sttpData);
	int i = 0;
	//动作特征值
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return FALSE;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//告警信号量
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//保护录波
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//录波器录波
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//录波列表
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}

	STTP_DBDATA_RECORD sttpDB;
	sttpDB.condition_list.clear();
	sttpDB.field_list.clear();

	//MainRecID
	STTP_DBDATA_UNIT field;
	CString str;
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "MainRecID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//保护ID
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "PTID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventType
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventType";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultStartTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultStartTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultReceiveTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultReceiveTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventDef
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventDef";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventContent
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "EventContent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//SignalTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "SignalTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//IsKeyEvent
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "IsKeyEvent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);

	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nReturn = 0;
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJInsertMutiData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sttpData, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::MultiSaveEvent, 保存失败");
		delete[] sError;
		return 2;
	}
	
	if(nResult == 1)
	{
		//成功
		nReturn = 0;
	}
	else
		nReturn = 2;

	zero_sttp_full_data(sttpData);
	
	delete[] sError;
	if(nReturn == 0) //成功
	{
		WriteLog("CFaultReport::MultiSaveEvent, 保存成功");
	}
	else
	{
		WriteLog("CFaultReport::MultiSaveEvent, 保存失败");
	}
	return nReturn;	
}

/*************************************************************
 函 数 名：DeleteAllEventFromDB()
 功能概要：从数据库中删除所有事件
 返 回 值: 删除成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL CFaultReport::DeleteAllEventFromDB()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置条件
	Condition con1;
	CString str;
	str.Format("MainRecID=%d", m_nID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, con1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::DeleteAllEventFromDB, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文中读取故障报告属性,(20151)
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	STTP报文数据结构
		   param2
**************************************************************/
BOOL CFaultReport::LoadFromSTTP( STTP_FULL_DATA& sttpData )
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20151)
	{
		CString str;
		str.Format("CFaultReport::LoadFromSTTP, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	//所属一次设备ID
	m_sPriDeviceID = sttpData.sttp_body.ch_HandlerName;
	//厂站1
	m_sStation1ID = sttpData.sttp_body.ch_station_id;
	//厂站2
	m_sStation2ID = sttpData.sttp_body.ch_pt_id;
	//信息发生毫秒时间值
	m_nmsFaultTime = sttpData.sttp_body.nFlag;
	//信息包编号
	CString str;
	str = sttpData.sttp_body.ch_check_code;
	m_nID = atoi(str);
	//信息包的名称
	m_sName = XJToMS(sttpData.sttp_body.strMessage);
	//故障时间
	CString sTime = sttpData.sttp_body.ch_time_15_BIT1;
	m_tmFaultTime = StringToTime14(sTime);
	//故障测距值
	m_sFaultDistance = sttpData.sttp_body.ch_version;
	//故障类型
	str = XJToMS(sttpData.sttp_body.strRelativePath);
	m_nFaultType = GetFaultTypeInt(str);
	//故障时的毫秒值
	m_nmsFaultTime = sttpData.sttp_body.nFlag;
	m_tmCreateTime = CTime::GetCurrentTime();
//	CString strTime = FindCreateTime(m_sName);
//	m_tmFaultTime = StringToTime14(strTime);

	m_bLoadDetail = FALSE;

	//关联设备
	if(!BuildReleation())
	{
		WriteLog("CFaultReport::LoadFromSTTP, 关联到设备失败");
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：GetFaultTypeInt()
 功能概要：将故障类型从字符型转为整型
 返 回 值: 故障类型标识
 参    数：param1	故障类型字符串
		   Param2
**************************************************************/
int CFaultReport::GetFaultTypeInt( CString sType )
{
	// A/B/C/AB/BC/AC/ABG/ACG/BCG/ABC/ABCG
	sType.MakeUpper();
	if ((sType == "AG") || (sType == "AN") || (sType == "A"))
	{
		//"A相接地";
		return 1;
	}
	if ((sType == "BG") || (sType == "BN") || (sType == "B"))
	{
		//"B相接地";
		return 2;
	}
	if ((sType == "CG") || (sType == "CN") || (sType == "C"))
	{
		//"C相接地";
		return 3;
	}
	if ((sType == "AB") || (sType == "BA"))
	{
		//"AB两相短路";
		return 4;
	}
	if ((sType == "BC") || (sType == "CB"))
	{
		//"BC两相短路";
		return 5;
	}
	if ((sType == "CA") || (sType == "AC"))
	{
		//"CA两相短路";
		return 6;
	}
	if ((sType == "ABG") || (sType == "ABN") || (sType == "BAG") || (sType == "BAN"))
	{
		//"AB两相短路接地";
		return 7;
	}
	if ((sType == "BCG") || (sType == "BCN") || (sType == "CBG") || (sType == "CBN"))
	{
		//"BC两相短路接地";
		return 8;
	}
	if ((sType == "CAG") || (sType == "CAN") || (sType == "ACG") || (sType == "ACN"))
	{
		//"CA两相短路接地";
		return 9;
	}
	if (sType == "ABC")
	{
		//"ABC三相短路";
		return 10;
	}
	if ((sType == "ABCG") || (sType == "ABCN"))
	{
		//"ABC三相接地";
		return 11;
	}	
	else
	{
		//"未知故障";
		return 0;
	}
	return 0;
}

/*************************************************************
 函 数 名：SortActionSign()
 功能概要：信号量时间和毫秒值排序动作信号列表
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CFaultReport::SortActionSign()
{
	//是否有信息
	if(m_listSign.GetSize() < 1)
		return;
	BOOL bSortPTID = FALSE; //是否需要按PTID排序
	//按时间排
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		for(int k = 1; k < m_listSign.GetSize()-i; k++)
		{
			CFaultEvent* pEvent1 = (CFaultEvent*)m_listSign.GetAt(k-1);
			CFaultEvent* pEvent2 = (CFaultEvent*)m_listSign.GetAt(k);
			if(CompareSign(pEvent1, pEvent2) > 0)
			{
				//前面比后面大, 将前面的后移
				m_listSign.SetAt(k, pEvent1);
				m_listSign.SetAt(k-1, pEvent2);
			}
			if(pEvent1->m_sSecID != pEvent2->m_sSecID)
				bSortPTID = TRUE;
		}
	}

	//查找所有保护
	m_listSec.RemoveAll();
	//动作特征量
	for(int i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
	//动作信号量
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}

	//保护录波
	for(int i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
}

/*************************************************************
 函 数 名：CompareSign()
 功能概要：比较两个故障事件的动作信号时时间大小
 返 回 值: 1>2返回>0, 1=2返回0, 1<2返回<0
 参    数：param1	故障事件1
		   Param2	故障事件2
**************************************************************/
int CFaultReport::CompareSign( CFaultEvent* pEvent1, CFaultEvent* pEvent2 )
{
	//检查数据合法性
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	//新比较时间
	if(pEvent1->m_tmSign == pEvent2->m_tmSign)
	{
		//时间相等,比较毫秒
		if(pEvent1->m_nmsSign > pEvent2->m_nmsSign)
			return 1;
		if(pEvent1->m_nmsSign == pEvent2->m_nmsSign)
			return 0;
		if(pEvent1->m_nmsSign < pEvent2->m_nmsSign)
			return -1;
	}
	if(pEvent1->m_tmSign > pEvent2->m_tmSign)
		return 1;
	if(pEvent1->m_tmSign < pEvent2->m_tmSign)
		return -1;
	return 0;
}

/*************************************************************
 函 数 名：GetFaultTypeLetter()
 功能概要：将故障类型从整形转为字母形式
 返 回 值: 故障类型字符串
 参    数：param1	故障类型代码
		   Param2
**************************************************************/
//##ModelId=49B87B9A014A
CString CFaultReport::GetFaultTypeLetter( int nType )
{
	CString sType = "";
	switch(nType)
	{
	case 1:
		sType = "A"; //"A相接地";
		break;
	case 2:
		sType = "B"; //"B相接地";
		break;
	case 3:
		sType = "C"; //"C相接地";
		break;
	case 4:
		sType = "AB"; //"AB两相短路";
		break;
	case 5:
		sType = "BC"; //"BC两相短路";
		break;
	case 6:
		sType = "AC"; //"CA两相短路";
		break;
	case 7:
		sType = "ABG"; //"AB两相短路接地";
		break;
	case 8:
		sType = "BCG"; //"BC两相短路接地";
		break;
	case 9:
		sType = "ACG"; ////"CA两相短路接地";
		break;
	case 10:
		sType = "ABC"; //"ABC三相短路";
		break;
	case 11:
		sType = "ABCG"; //"ABC三相接地";
		break;
	default:
		sType = "";
		break;
	}
	return sType;
}

/*************************************************************
 函 数 名：AddSec()
 功能概要：将设备加入到二次设备数组
 返 回 值: 设备指针
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A015B
void CFaultReport::AddSec( CDeviceObj* pObj )
{
	MYASSERT(pObj);
	//查找是否已经在数组中
	BOOL bFind = FALSE;
	int nLastEquel = -1;
	for(int i = 0; i < m_listSec.GetSize(); i++)
	{
		CDeviceObj* pDevice = (CDeviceObj*)m_listSec.GetAt(i);
		if(pDevice == pObj)
		{
			bFind = TRUE;
			break;
		}
		if(pObj->m_sStationID == pDevice->m_sStationID)
			nLastEquel = i;
	}
	//加入到数组
	if(!bFind)
	{
		if(pObj->m_nType == TYPE_PROTECT)
		{
			if(nLastEquel == m_listSec.GetUpperBound() || nLastEquel < 0)
				m_listSec.Add(pObj);
			else
				m_listSec.InsertAt(nLastEquel, pObj);
		}
	}
}

/*************************************************************
 函 数 名：BuildTempList()
 功能概要：指定二次设备建立零时数组
 返 回 值: 指定二次设备
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0168
void CFaultReport::BuildTempList( CSecObj* pSec )
{
	if(pSec == NULL)
		return;

	m_listTempChr.RemoveAll();
	m_listTempSign.RemoveAll();
	m_listTempPTOSC.RemoveAll();

	m_pTempSec = pSec;
		
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempChr.Add(pEvent);
		}
	}

	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempSign.Add(pEvent);
		}
	}
	
	//保护录波
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempPTOSC.Add(pEvent);
		}
	}

	CString str;
	str.Format("CFaultReport::BuildTempList,保护%s,故障量%d条,信号量%d条,录波%d条", pSec->m_sName, m_listTempChr.GetSize(), m_listTempSign.GetSize(), m_listTempPTOSC.GetSize());
	WriteLog(str);
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetNotifyType	
返回值:   int	
功能概要: 
*****************************************************/
int CFaultReport::GetNotifyType()
{
	return XJ_NOTIFY_FAULT;
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetDescribe	
返回值:   CString	事件描述字符串
功能概要: 获得事件描述
*****************************************************/
CString CFaultReport::GetDescribe()
{
	return "大家好,我是故障报告!";
}

/****************************************************
Date:2011/11/8  Author:LYH
函数名:   GetSimpleDescribe	
返回值:   CString	事件简要描述字符串
功能概要: 获得事件简要描述
*****************************************************/
CString CFaultReport::GetSimpleDescribe()
{
	CString str,strTemp;
	str = "故障报告:";
	strTemp.Format("[%s] ",m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S"));
	str += strTemp;
	strTemp = "";
	if(m_pPriDevice != NULL)
		strTemp = m_pPriDevice->m_sName;
	str += strTemp;
	str += " ";
	strTemp.Format("[%s]故障 ", GetFaultTypeString(m_nFaultType));
	str += strTemp;

	if(m_pStation1 != NULL)
		str += "[厂站1:" + m_pStation1->m_sName+"]";
	if(m_pStation2 != NULL)
		str += "[厂站2:" + m_pStation2->m_sName+"]";

	return str;
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   LoadDecision	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CFaultReport::LoadDecision()
{
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据 MainRecID查找所有字段
	CString str;		
	//条件

	//MainRecID
	Condition condition2;
	str.Format("MainRecID = %d", m_nID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);

	CMemSet* pMemset;
	pMemset = new CMemSet;

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_ANALYFAULT_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CFaultReport::LoadDecision, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CFaultReport::LoadDecision, 读取到%d条事件记录", nCount);
		WriteLog(sLog, XJ_LOG_LV3);
		if(nCount == 1)
		{
			if(m_pDecisionReport == NULL)
				m_pDecisionReport = new CDecisionReport(m_pDataEngine);
			m_pDecisionReport->LoadFromDB(pMemset);
		}
		else
		{
			CString sLog;
			sLog.Format("CFaultReport::LoadDecision, 记录过多，%d,无效", nCount);
			WriteLog(sLog, XJ_LOG_LV3);
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(m_pDecisionReport != NULL)
		{
			//把某些属性告诉辅助决策
			m_pDecisionReport->m_sFaultDistance = m_sFaultDistance;
			m_pDecisionReport->m_sFaultPhase = GetFaultTypeString(m_nFaultType);
			if(m_pPriDevice != NULL)
				m_pDecisionReport->m_sFaultDevice = m_pPriDevice->m_sName;
		}
	}
	delete[] sError;
	delete pMemset;
	return TRUE;
}



