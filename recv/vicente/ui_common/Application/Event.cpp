// Event.cpp : implementation file
//

#include "Event.h"

#pragma warning(disable : 4996)//屏蔽4786错误
/////////////////////////////////////////////////////////////////////////////
// CSTEvent
//##ModelId=49B87B9A02DE
CXJEvent::CXJEvent(CDataEngine* pDataEngine) : CXJNotify(pDataEngine)
{
	m_pSec = NULL;

	//common
	m_lID			= -1;
	m_nIsAck		= 0;
	m_IsDebug		= 1;
	m_sMsg			= "";
	m_nms			= 0;
	m_nms2			= 0;
	m_sFaultType	= "";
	//releation
	m_sSecName		= "";
	m_sSecID		= "";
	m_sDeviceId		= "";
	m_sDeviceName	= "";
	m_sStationName	= "";
	m_sNetName		= "";
	m_nProperty		= 2;

	m_bLoadDetail	= FALSE;
	m_nIsReturn = 0;

	CTime tm(1971, 1, 1, 1, 1, 1);
	m_tmTime2 = tm;
	m_tmTime = tm;
	m_nms2 = 0;

	m_ActChrs.RemoveAll();
	m_Signals.RemoveAll();
}

//##ModelId=49B87B9A038A
CXJEvent::~CXJEvent()
{
	Clear();
}



/*************************************************************
 函 数 名：BuildReleation()
 功能概要：建立事件与设备的关系,包括找到二次设备指针,填充一次设备ID,名称,厂站ID,厂站名,地区名
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   二次设备类型
**************************************************************/
//##ModelId=49B87B9C0109
BOOL CXJEvent::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	
	//先查找二次设备指针
	m_pSec = (CSecObj*)m_pDataEngine->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL)
		return FALSE;
	//二次设备名称
	m_sSecName = m_pSec->m_sName;

	if(m_pSec->m_pOwner != NULL && m_pSec->m_nType == TYPE_PROTECT)
	{
		//一次设备ID,名称
		m_sDeviceId = m_pSec->m_pOwner->m_sID;
		m_sDeviceName = m_pSec->m_pOwner->m_sName;
	}
	//厂站ID
	m_sStationId = m_pSec->m_sStationID;
	if(m_pSec->m_pStation != NULL)
	{
		m_sStationName = m_pSec->m_pStation->m_sName;
		CNetObj * pNet = (CNetObj*)m_pDataEngine->FindDevice(m_pSec->m_pStation->m_sNetID, TYPE_NET);
		if(pNet != NULL)
		{
			//地区名称
			m_sNetName = pNet->m_sName;
		}
	}

	return TRUE;
}

/*************************************************************
 函 数 名：Clear()
 功能概要：清除点表配置
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9C0271
BOOL CXJEvent::Clear()
{
	for(int i = 0; i < m_ActChrs.GetSize(); i++)
	{
		delete m_ActChrs[i];
	}
	m_ActChrs.RemoveAll();

	for(int i = 0; i < m_Signals.GetSize(); i++)
	{
		delete m_Signals[i];
	}
	m_Signals.RemoveAll();

	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：在数据库中查询新事件的ID
 返 回 值: 查询成功返回TRUE,失败返回FALSE(有结果且仅有一个结果视为查询成功)
**************************************************************/
//##ModelId=49B87B9C01B5
BOOL CXJEvent::QueryNewEventID()
{
	return TRUE;
}

/*************************************************************
 函 数 名: IsReturnEvent()
 功能概要: 判断是否"返回"事件
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CXJEvent::IsReturnEvent()
{
	//没有信号量
	if(m_Signals.GetSize() < 1)
		return FALSE;
	BOOL bReturn = TRUE;
	for(int i = 0; i < m_Signals.GetSize(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_Signals.GetAt(i);
		if(pSign == NULL)
			continue;
		if(pSign->nValue == 1)
		{
			bReturn = FALSE;
			break;
		}
	}
	return bReturn;
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetNotifyType	
返回值:   int	
功能概要: 
*****************************************************/
int CXJEvent::GetNotifyType()
{
	return XJ_NOTIFY_EVENT;
}

/****************************************************
Date:2011/11/15  Author:LYH
函数名:   GetAlarmType	
返回值:   CString	
功能概要: 
*****************************************************/
CString CXJEvent::GetAlarmType()
{
	//分析信号号, 取最高级信号量做为事件的故障类型
	int nAlarmType = m_nProperty;

	CString str;
	switch(nAlarmType)
	{
	case 0:
		str = "I";
		break;
	case 1:
		str = "II";
		break;
	case 2:
		str = "III";
		break;
	default:
		str = "III";
	}
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CSTEventPTAlarm
//##ModelId=49B87B9C039A
CXJEventAlarm::CXJEventAlarm(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType=XJ_EVENT_ALARM; 
	m_lAlarmID = -1;
	m_nValue = -1;
	m_nCount = 0;
}

//##ModelId=49B87B9D00DA
CXJEventAlarm::~CXJEventAlarm()
{

}

/*************************************************************
 函 数 名：Ack()
 功能概要：告警事件确认(到TB_PT_ALARM修改Is_Ack字段),TRUE-成功
 返 回 值: TRUE-成功, FALSE-失败
**************************************************************/
//##ModelId=49B87B9D0290
BOOL CXJEventAlarm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;

	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段

		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventAlarm::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventAlarm::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}

	//写日志到输出窗口
	CString str;
	if(bReturn)
		str.Format("告警事件[%d]已被确认",m_lID);
	else
		str.Format("告警事件[%d]确认时出错",m_lID);
	WriteLog(str);

	return bReturn;	
}

/*
 *  @brief   	CSTEventAlarm::GetDescribe	 得到告警事件描述 
 *  @return 	CString 告警事件描述
 */
//##ModelId=49B87B9D031C
CString CXJEventAlarm::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";

	str += "厂站ID: " + m_sStationName + "\r\n";  //厂站名
	str += "一次设备名: " + m_sDeviceName + "\r\n"; //一次设备名称
	str += "二次设备名: " + m_sSecName + "\r\n"; //二次设备名称

	if (m_pSec != NULL)
	{
		//显示保护类型
		str += "保护类型: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("是否已确认: %s", (m_nIsAck==0)?"未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";

	str += "*******************************************\r\n";
	strTemp.Format("以下是告警事件信息:\r\n %s", m_sMsg);
	str += strTemp;
	
	return str;
}

/*
 *  @brief   	CSTEventAlarm::Clone	 复制告警事件对象
 *  @return 	CSTEvent* 复制生成的对象
 */
//##ModelId=49B87B9D03B9
CXJEvent* CXJEventAlarm::Clone()
{
	CXJEventAlarm* pEvent=new CXJEventAlarm(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;
	pEvent->m_nProperty		= m_nProperty;
	pEvent->m_bLoadDetail	= m_bLoadDetail;
	//special
	pEvent->m_nEventType	= XJ_EVENT_ALARM	;
	pEvent->m_lAlarmID		= m_lAlarmID;
	pEvent->m_nValue		= m_nValue	;
	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;
	
	//是否调试信息
	pEvent->m_IsDebug		= m_IsDebug;
	//是否返回信息
	pEvent->m_nIsReturn = m_nIsReturn;

	//二次设备指针
	pEvent->m_pSec = m_pSec;

    //信号量拷贝
	PT_Signal * pSignal;
	for(int i = 0; i < m_Signals.GetSize(); i ++)
	{
		pSignal = new PT_Signal;
		*pSignal = *m_Signals[i];
		pEvent->m_Signals.Add(pSignal);	
	}

	return pEvent;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_alarm
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9E0157
BOOL CXJEventAlarm::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0);//事件ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //消息内容 
		m_sSecID = pMemSet->GetValue(2); //保护ID
		str = pMemSet->GetValue(3); //告警ID
		m_lAlarmID = atol(str);
		str = pMemSet ->GetValue(4); //源的值
		m_nValue = atoi(str);
		str = pMemSet->GetValue(5); //是否已确认
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(6); //发生时间
		//时间转换
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(7); //发生时间毫秒值
		m_nms = atoi(str);
		str = pMemSet->GetValue(8); //完成时间毫秒值
		m_nms2 = atoi(str);
		str = pMemSet->GetValue(9); //信息接收时间
		m_tmTime2 = StringToTime(str);
		str = pMemSet->GetValue(10); //是否调试信息
		m_IsDebug = atoi(str);
		str = pMemSet->GetValue(12); //事件属性
		m_nProperty = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventAlarm::LoadFromDB, 填入数据失败", XJ_LOG_LV3);
	}

	//建立与设备的关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	//是否返回信息
	m_nIsReturn = IsReturnEvent();

	//解析FData
// 	if(!TranslateFData())
// 	{
// 		WriteLogEx("CXJEventAlarm::LoadFromDB, 解析FData失败", XJ_LOG_LV3);
// 		return FALSE;
// 	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文中读取告警事件属性,(20018)
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	STTP报文数据结构
		   param2	指定是报文数据中的第几个数据项
**************************************************************/
//##ModelId=49B87B9E01E4
BOOL CXJEventAlarm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20018)
	{
		CString str;
		str.Format("CXJEventAlarm::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	
	//判断对应保护
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventAlarm::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}

	//是否调试信息
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	CString str;
	str.Format("%d", sttpData.sttp_head.uMsgLengthType);
	WriteLog("CXJEventAlarm::LoadFromSttp sttpData.sttp_head.uMsgLengthType = " + str, 3);
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//解析告警信号量
	if(!TranslateSttpData(sttpData, nIndex))
	{
		//解析失败
		WriteLog("CXJEventAlarm::LoadFromSttp, 解析动作数据失败", XJ_LOG_LV3);
		return FALSE;
	}

	//是否返回信息
	m_nIsReturn = IsReturnEvent();

	return TRUE;
}

/*************************************************************
 函 数 名：TranslateFData()
 功能概要：解释FData字段, 并填充信号量列表
 返 回 值: 返回值说明
**************************************************************/
//##ModelId=49B87B9D0186
BOOL CXJEventAlarm::TranslateFData()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	m_bLoadDetail = TRUE;
	if (m_Signals.GetSize() > 0)//已经翻译出来了
		return TRUE;
	if(m_pSec == NULL)
	{
		return FALSE;
	}

	if(m_pSec != NULL)
	{
		if(m_pSec->m_arrAlarmSignal.GetSize() == 0) //如果没有初始化过
		{
			m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
		}
	}

	//清空之前的
	Clear();

	PT_Signal *pSignalConfig = GetSignalConfig(m_lAlarmID);
	if(pSignalConfig != NULL)
	{
		PT_Signal *pSignal = new PT_Signal;
		*pSignal  = *pSignalConfig;
		
		pSignal->nValue = m_nValue; //value
		CString strTemp;
		strTemp.Format("%s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"), m_nms);
		pSignal->strTime = strTemp; //time
		
		m_Signals.Add(pSignal);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：TranslateSttpData()
 功能概要：解析告警事件报文信号量数据
 返 回 值: 解析成功返回TURE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据中的第几个数据项
**************************************************************/
//##ModelId=49B87B9D0203
BOOL CXJEventAlarm::TranslateSttpData(STTP_FULL_DATA& sttpData, int nIndex)
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	if (m_Signals.GetSize() > 0)//已经翻译出来了
		return TRUE;
	if(m_pSec == NULL)
	{
		return FALSE;
	}

	//清空之前的
	Clear();

	//信号量数量
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//错误的索引值
		return FALSE;
	}

	if(m_pSec != NULL)
	{
		if(m_pSec->m_arrAlarmSignal.GetSize() == 0) //如果没有初始化过
		{
			m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
		}
	}

	PT_Signal *pSignal = new PT_Signal;
	pSignal->nID = sttpData.sttp_body.variant_member.value_data[nIndex].id;
	//查找信号量信息
	PT_Signal *pSignalConfig = GetSignalConfig(pSignal->nID);
	if(pSignalConfig == NULL)
	{
		//未找到,delete
		delete pSignal;
		pSignal = NULL;
		return FALSE;
	}

	//复制配置
	*pSignal  = *pSignalConfig;

	//值
	CString strValue = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	pSignal->nValue = atoi(strValue);
	//设置事件值
	m_lAlarmID = pSignal->nID;
	m_nValue = pSignal->nValue;
	//时间,转换格式
	CString strTime = sttpData.sttp_body.variant_member.value_data[nIndex].ch_Time;
	int nms;
	CTime tm = StringToTimeSttp15(strTime, nms);
	CString strTemp;
	strTemp.Format("%s.%03d",tm.Format("%Y-%m-%d %H:%M:%S"), nms);
	//设置事件时间
	m_tmTime = tm;
	m_nms = nms;
	//设置信号量时间
	pSignal->strTime = strTemp;

	//设置msg
	m_sMsg.Format("%s %s", pSignal->strName, (pSignal->nValue == 1)?"动作":"返回");

	//设置事件属性
	m_nProperty = pSignal->nEventType;
	CString strOut;
	strOut.Format("此事件为%s类事件", GetAlarmType());
	WriteLog(strOut, 3);

	m_Signals.Add(pSignal);

	//没有信号量的告警事件认为是错误的
	if(m_Signals.GetSize() <= 0)
		return FALSE;

	return TRUE;
}

/*************************************************************
 函 数 名：GetSignalConfig()
 功能概要：得到指定的信号量
 返 回 值: 找到ID所指的信号量则返回信号量, 未找到返回NULL
 参    数：param1   信号量ID
**************************************************************/
//##ModelId=49B87B9E00DA
PT_Signal* CXJEventAlarm::GetSignalConfig(int nID)
{
	if(m_pSec == NULL)
		return NULL;
	PT_Signal *pSignal;
	pSignal = NULL;
	for (int i = 0; i < m_pSec->m_arrAlarmSignal.GetSize(); i ++)
	{
		pSignal = (PT_Signal *)m_pSec->m_arrAlarmSignal[i];
		if (nID == pSignal->nID)
		{
			return pSignal;
		}
	}
	return NULL;
}

/*************************************************************
 函 数 名：GetAlarmType()
 功能概要：取得告警事件类别
 返 回 值: 告警事件类另
**************************************************************/
//##ModelId=49B87B9E005D
CString CXJEventAlarm::GetAlarmType()
{
	//分析信号号, 取最高级信号量做为事件的故障类型
	int nAlarmType = m_nProperty;
	
	CString str;
	switch(nAlarmType)
	{
	case 0:
		str = "I";
		break;
	case 1:
		str = "II";
		break;
	case 2:
		str = "III";
		break;
	default:
		str = "III";
	}
	return str;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9E0261
BOOL CXJEventAlarm::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,ms,alaram_id,value查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//curtime
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("CURTIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//ms
	Condition condition4;
	str.Format("MS = %d", m_nms);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);
	
	//alarm_id
	Condition condition5;
	str.Format("ALARM_ID = %d", m_lAlarmID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition5, str);
	
	//value
	Condition condition6;
	str.Format("VALUE = %d", m_nValue);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition6, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventAlarm::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventAlarm::QueryNewEventID,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CXJEventAlarm::IsReturnEvent()
{
	if(m_nValue == 0)
		return TRUE;
	return FALSE;
}

/****************************************************
Date:2011/11/18  Author:LYH
函数名:   GetSimpleDescribe	
返回值:   CString	
功能概要: 
*****************************************************/
CString CXJEventAlarm::GetSimpleDescribe()
{
	//common
	CString str,strTemp;
	str = "告警:";
	strTemp.Format("[%s.%03d] ",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp;
	str += m_sMsg;
	str += "\r\n";
	str += "[" + m_sStationName+"]";
	str += "[" + m_sDeviceName+"]";
	str += "[" + m_sSecName+"]";    

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CSTEventPTAction
//##ModelId=49B87B9E034B
CXJEventAction::CXJEventAction(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_ACTION; 
	m_FData = "";
	m_nCount = 0;
}

//##ModelId=49B87B9F003E
CXJEventAction::~CXJEventAction()
{

}

/*************************************************************
 函 数 名：Ack()
 功能概要：动作事件确认(到TB_PT_ACTION修改Is_Ack字段)
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9F00BB
BOOL CXJEventAction::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	try
	{
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventAction::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventAction::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}

	//写日志到输出窗口
	CString str;
	if(bReturn)
		str.Format("保护动作事件[%d]已被确认",m_lID);
	else
		str.Format("保护动作事件[%d]确认时出错",m_lID);
	WriteLog(str);

	return bReturn;
}
	
/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87B9F0196
CXJEvent* CXJEventAction::Clone()
{
	CXJEventAction* pEvent=new CXJEventAction(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_sFaultType	= m_sFaultType;
	pEvent->m_nProperty		= m_nProperty;
	pEvent->m_bLoadDetail	= m_bLoadDetail;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;
	//是否返回信息
	pEvent->m_nIsReturn = m_nIsReturn;

	//special
	m_nEventType			= XJ_EVENT_ACTION;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_FData			= m_FData		;

	//保护装置指针
	pEvent->m_pSec = m_pSec;
	
	//拷贝特征量与信号量
	PT_Signal * pSignal;
	PT_ActionCharacter * pCharacter;
	for(int i = 0 ; i < m_ActChrs.GetSize(); i++)
	{
		pCharacter = new PT_ActionCharacter;
		*pCharacter = *m_ActChrs[i];
		pEvent->m_ActChrs.Add(pCharacter);
	}
	for(int i = 0; i < m_Signals.GetSize(); i ++)
	{
		pSignal = new PT_Signal;
		*pSignal = *m_Signals[i];
		pEvent->m_Signals.Add(pSignal);	
	}
	return pEvent;
}


/*************************************************************
 函 数 名：GetDescribe()
 功能概要：得到动作事件详细描述
 返 回 值: 格式化的描述字符串
**************************************************************/
//##ModelId=49B87B9F0128
CString CXJEventAction::GetDescribe()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("动作事件ID: %d ", m_lID);
	str += strTemp + "\r\n";

	str += "厂站名:   " + m_sStationName  + "\r\n";
	str += "一次设备名:   " + m_sDeviceName  + "\r\n";
	str += "二次设备名:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "设备类型: " + m_pSec->m_sModel + "\r\n";
	else
		str += "设备类型: 未知\r\n";
	//故障类型
	strTemp.Format("故障类型: %s ", GetFaultType(m_sFaultType));
	str += strTemp + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("装置启动时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	strTemp.Format("信息接收时间: %s.%03d",m_tmTime2.Format("%Y-%m-%d %H:%M:%S"),m_nms2);
	str += strTemp + "\r\n";
 
	//ToDo: 解释Msg，格式ex: 2002-01-07 11:59:59.785  REL521 V2.0 AB  
	str += "*******************************************\r\n";
	if(!TranslateFData())	//从FData中获得特征值列表和开关量列表
		return str;

	//增加筛选功能 add by yinzhehong 20051230
    strTemp.Empty();
    for(i=0; i< m_Signals.GetSize(); i++)
	{
		pSignal = (PT_Signal *)m_Signals[i];
		nEventType = pSignal->nEventType;
		strValue.Format("%25s  %20s(%12s)\t%3s\r\n",pSignal->strTime,pSignal->strName,pSignal->strCodeName,pSignal->nValue ==1 ? "动作":"返回");
		strTemp += strValue;
		nCount++;		   
	}
	strValue.Format("以下是信号量列表信息:(共%d个)\r\n", nCount);
	str += strValue;
	str += strTemp;

	str += "*******************************************\r\n";
	strTemp.Format("以下是采样值信息:(共%d个)\r\n", m_ActChrs.GetSize());
	str += strTemp;
	for(i=0;i<m_ActChrs.GetSize();i++)
	{
		//2008-04-29 加入单位显示
		strTemp.Format("%s\t:%s %s\r\n",m_ActChrs[i]->strName,m_ActChrs[i]->strValue,m_ActChrs[i]->strUnits);
		str += strTemp;
	}
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：TranslateFData()
 功能概要：解析FData内容，并设置 m_ActChrs m_Signals
 返 回 值: 解析成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9F01F4
BOOL CXJEventAction::TranslateFData()
{
	m_bLoadDetail = TRUE;
    if(m_FData.IsEmpty())
	{
		return TRUE;
	}

	if(m_pSec == NULL)
		return FALSE;
	
	if ((m_ActChrs.GetSize() >0) || (m_Signals.GetSize() >0))//已经翻译出来了
		return TRUE;
	if (!RefreshConfig())
		return FALSE;

	int i = 0;
	//删除原来的	
	Clear();

	//创建新的
	int nPos, nCount, nErrAC, nErrS;
	CString strData = m_FData;
	//CString strData = "1 ,94 ,2827,1 ,18 ,1,2010-09-14 15:28:42.502";
	CString strTemp;
	BOOL bSucAC = TRUE, bSucS = TRUE;
	try
	{
		//特征量数量
		nPos = strData.Find(',', 0);
		nCount = _ttoi(strData.Left(nPos));

		//各个特征值
		strData.Delete(0, nPos+1);
		for( i = 0; i < nCount; i++ )
		{
			PT_ActionCharacter *pAC = new PT_ActionCharacter;

			//ID
			nPos = strData.Find(',',0);
			pAC->nID = _ttoi(strData.Left(nPos));
			strData.Delete(0, nPos+1);

			//取得特征量定义
			PT_ActionCharacter *pACConfig = GetActChrsConfig(pAC->nID);
			if (pACConfig == NULL)
			{
				//查找不到特征量定义
				//到通用特征量定义中去查
				pACConfig = GetActChrsFromUniversal(pAC ->nID);
				if(pACConfig == NULL)
				{
					bSucAC = FALSE;
					nErrAC = pAC->nID;
					nPos = strData.Find(',',0);
					strData.Delete(0, nPos+1);
					//清除
					delete pAC;
					pAC = NULL;
					continue;
				}
			}

			//设置特征量定义, 直接复制
			*pAC = *pACConfig;

			//Value
			nPos = strData.Find(',',0);
			CString sValue;
			if (nPos != -1)
				sValue = strData.Left(nPos);
			else
				sValue = strData;
			pAC->strValue = ComplexExchange(sValue);
			strData.Delete(0,nPos+1);

			m_ActChrs.Add(pAC);
		}

		//信号量数量
		nPos = strData.Find(',',0);
		nCount = _ttoi(strData.Left(nPos));

		//各个信号量
		strData.Delete(0, nPos+1);
		for( i = 0; i < nCount; i++ )
		{
			PT_Signal *pSignal = new PT_Signal;
			//ID
			nPos = strData.Find(',',0);
			pSignal->nID = _ttoi(strData.Left(nPos));
			strData.Delete(0, nPos+1);

			//取得信号量定义
			PT_Signal *pSignalConfig = GetSignalConfig(pSignal->nID);
			if (pSignalConfig == NULL)
			{
				bSucS = FALSE;
				nErrS = pSignal->nID;
				nPos = strData.Find(',',0);
				strData.Delete(0, nPos+1);
				nPos = strData.Find(',',0);
				strData.Delete(0, nPos+1);
				////删除新new出来的空间
				delete pSignal;
				pSignal = NULL;
				////
				continue;
			}

			//设置信号量定义
			*pSignal = *pSignalConfig;//直接拷贝全部内容
			
			//Value                     修改值
			nPos = strData.Find(',',0);
			pSignal->nValue = _ttoi(strData.Left(nPos));
			strData.Delete(0,nPos+1);

			//DateTime
			nPos = strData.Find(',',0); //时间
			if(nPos>0)
			{
				pSignal->strTime = strData.Left(nPos);
				strData.Delete(0, nPos+1);
			}
			else//最后一个
				pSignal->strTime = strData;
			
			m_Signals.Add(pSignal);
		}
	}
	catch(...)
	{
	//	return FALSE;
	}

	//在此做筛选
	if ((m_ActChrs.GetSize() > 0)||(m_Signals.GetSize() > 0)) //分析过程出错仍然将结果放出来
		return TRUE;
	else
	{
		//没有特征量和信号量的事件认为是错误事件
		return FALSE;
	}
}

//##ModelId=49B87B9F0261
BOOL CXJEventAction::TranslateSttpData(STTP_FULL_DATA& sttpData)
{
	if ((m_ActChrs.GetSize() >0) || (m_Signals.GetSize() >0))//已经翻译出来了
		return TRUE;
	if (!RefreshConfig())
		return FALSE;
	
	int i = 0;
	//删除原来的	
	Clear();

	int nPro = 2;
	//特征量个数
	int nCount = sttpData.sttp_body.variant_member.value_data2.size();
	//循环读取各特征量
	for(i = 0; i < nCount; i++)
	{
		//新建特征量对象
		PT_ActionCharacter* pAC = new PT_ActionCharacter;
		//ID
		pAC->nID = sttpData.sttp_body.variant_member.value_data2[i].id;
		//取得特征量定义
		PT_ActionCharacter *pACConfig = GetActChrsConfig(pAC->nID);
		if (pACConfig == NULL)
		{
			//查找不到特征量定义
			//到通用特征量定义中去查
			pACConfig = GetActChrsFromUniversal(pAC ->nID);
			if(pACConfig == NULL)
			{
				//清除
				delete pAC;
				pAC = NULL;
				continue;
			}
		}		
		//设置特征量定义, 直接复制
		*pAC = *pACConfig;
		
		//Value
		string str = sttpData.sttp_body.variant_member.value_data2[i].str_value;
		CString sValue = XJToMS(str);
		pAC->strValue = ComplexExchange(sValue);
		//设置事件故障类型
		if(pAC->nID == 2)
		{
			m_sFaultType = pAC->strValue;
		}
		//设置事件等级
		if(pAC->nEventType < nPro)
			nPro = pAC->nEventType;
		//加入到链表
		m_ActChrs.Add(pAC);
	}

	
	//信息号
	nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nCount > 0)
		nPro = 2;
	//循环读取各信号量
	for(i = 0; i < nCount; i++)
	{
		//新建信号量对象
		PT_Signal* pSN = new PT_Signal;
		//ID
		pSN->nID = sttpData.sttp_body.variant_member.value_data[i].id;
		//取得信号量定义
		PT_Signal * pSignalConfig = GetSignalConfig(pSN->nID);
		if(pSignalConfig == NULL)
		{
			//清空
			delete pSN;
			pSN = NULL;
			continue;
		}

		//设置信号量
		*pSN = *pSignalConfig;

		//设置值
		//string要转化为CString
		string str = sttpData.sttp_body.variant_member.value_data[i].str_value;
		CString sValue = XJToMS(str);
		pSN->nValue = atoi(sValue);
		//设置时间
		pSN->strTime = "";
		pSN->strTime = sttpData.sttp_body.variant_member.value_data[i].ch_Time;
		CTime tm;
		int nms = 0;
		tm = StringToTimeSttp15(pSN->strTime, nms);
		pSN->strTime.Format("%s.%03d", tm.Format("%Y-%m-%d %H:%M:%S"), nms);

		//设置事件等级
		if(pSN->nEventType < nPro)
			nPro = pSN->nEventType;

		if(i == 0)
		{
			//设置事件显示
			m_sMsg.Format("%s %s", pSN->strName, (pSN->nValue == 1)?"动作":"返回");
		}

		//加入到链表
		m_Signals.Add(pSN);
	}

	//如果没有信号量，则以第一个故障之为描述
	if(m_Signals.GetSize() <= 0)
	{
		for(i = 0; i < m_ActChrs.GetSize(); i++)
		{
			PT_ActionCharacter* pAC = (PT_ActionCharacter*)m_ActChrs.GetAt(i);
			if(pAC == NULL)
				continue;
			if(pAC->nID == 94) //故障序号
				continue;
			//设置事件显示
			if(pAC->strUnits == "")
				m_sMsg.Format("%s %s", pAC->strName, pAC->strValue);
			else
				m_sMsg.Format("%s %s(%s)", pAC->strName, pAC->strValue, pAC->strUnits);
			break;
		}
	}

	//设置事件属性
	m_nProperty = nPro;
	CString strOut;
	strOut.Format("此事件为%s类事件", GetAlarmType());
	WriteLog(strOut, 3);

	if ((m_ActChrs.GetSize() > 0)||(m_Signals.GetSize() > 0)) //分析过程出错仍然将结果放出来
		return TRUE;
	else
	{
		//没有特征量和信号量的事件认为是错误事件
		return FALSE;
	}
}

/*************************************************************
 函 数 名：GetSignalConfig()
 功能概要：根据指定ID得到指定的信号量
 返 回 值: 查找到指定的信号量则返回信号量对象, 未查找到返回NULL
 参    数：param1   信号量ID
**************************************************************/
//##ModelId=49B87BA00128
PT_Signal * CXJEventAction::GetSignalConfig(int nID)
{ 
	if(m_pSec == NULL)
		return NULL;
	PT_Signal *pSignal;
	pSignal = NULL;
	for (int i = 0; i < m_pSec->m_arrActionSignal.GetSize(); i ++)
	{
		pSignal = (PT_Signal *)m_pSec->m_arrActionSignal[i];
		if (nID == pSignal->nID)
		{
			return pSignal;
		}
	}
	return NULL;
}

/*************************************************************
 函 数 名：GetActChrsConfig()
 功能概要：根据指定ID得到指定的特征量
 返 回 值: 查找到指定的特征量则返回特征量对象, 未查找到返回NULL
 参    数：param1   特征量ID
**************************************************************/
//##ModelId=49B87BA0007D
PT_ActionCharacter * CXJEventAction::GetActChrsConfig(int nID)
{
	if(m_pSec == NULL)
		return NULL;

	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < m_pSec->m_arrActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) m_pSec->m_arrActChr[i];
		if (nID == pActChr->nID)
		{
			return pActChr;
		}
	}

	return NULL;
}

/*************************************************************
 函 数 名：GetActChrsFromUniversal()
 功能概要：从通用特征量中查找特征量定义
 返 回 值: 查找到则返回特征量指针, 未找到返回NULL
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
//##ModelId=49B87BA000DA
PT_ActionCharacter * CXJEventAction::GetActChrsFromUniversal(int nID)
{
	MYASSERT_BOOL(m_pDataEngine);

	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < m_pDataEngine->m_arrUniversalActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) m_pDataEngine->m_arrUniversalActChr[i];
		if (nID == pActChr->nID)
		{
			return pActChr;
		}
	}
	
	return NULL;
}
	
/*************************************************************
 函 数 名：RefreshConfig()
 功能概要：刷新对应二次设备的信号量列表
 返 回 值: 刷新成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9F03B9
BOOL CXJEventAction::RefreshConfig()
{
	if(m_pSec == NULL)
		return FALSE;
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
	return TRUE;
}

/*************************************************************
 函 数 名：GetAlarmType()
 功能概要：取得故障类型,I,II,III类
 返 回 值: 故障类型
**************************************************************/
//##ModelId=49B87B9F02BF
CString CXJEventAction::GetAlarmType()
{
	int nAlarmType = m_nProperty;

	CString str;
	switch(nAlarmType)
	{
	case 0:
		str = "I";
		break;
	case 1:
		str = "II";
		break;
	case 2:
		str = "III";
		break;
	default:
		str = "III";
	}
	return str;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_action
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9F030D
BOOL CXJEventAction::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //msg
		m_sSecID = pMemSet->GetValue(2); //pt_id
		str = pMemSet->GetValue(3); //是否已确认
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(4); //装置启动时间
		//m_tmTime1 此处需要时间转换
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(5); //启动时间毫秒值
		m_nms = atoi(str);
		str = pMemSet->GetValue(6); //结束时间毫秒值
		m_nms2 = atoi(str);
		str = pMemSet->GetValue(8); //子站系统收到事件的时间,我们视为结束时间
		//m_tmTime2 此处需要时间转换
		m_tmTime2 = StringToTime(str);
		m_sFaultType = pMemSet->GetValue(10); //故障类型
		str = pMemSet->GetValue(11); //是否调试
		m_IsDebug = atoi(str);
		//---------------------------------------------------------------
		m_FData = pMemSet->GetValue(12); //故障数据
		str = pMemSet->GetValue(14); //事件属性
		m_nProperty = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventAction::LoadFromDB, 填入数据失败");
		return FALSE;
	}
	

	//创建关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	//解析FData
	if(!TranslateFData())
	{
		WriteLog("CXJEventAction::LoadFromDB, 解析FData失败");
		return FALSE;
	}

	//是否返回信息
	m_nIsReturn = IsReturnEvent();

	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从STTP报文中读取信息填充到本事件对象中,(20022)
 返 回 值: 读取信息成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据中的第几个数据项
**************************************************************/
//##ModelId=49B87B9F036B
BOOL CXJEventAction::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20022)
	{
		CString str;
		str.Format("CXJEventAction::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}

	//判断对应保护
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventAction::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//装置启动时间
	CString sTime;
	sTime = sttpData.sttp_body.ch_time_15_BIT1;
	//将时间字符串转为CTime类型
	m_tmTime = StringToTimeSttp15(sTime, m_nms);

	//动作完成时间
	sTime = sttpData.sttp_body.ch_time_15_BIT2;
	m_tmTime2 = StringToTimeSttp15(sTime, m_nms2);

	//是否调试信息
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//解析动作数据
	if(!TranslateSttpData(sttpData))
	{
		//解析失败
		WriteLog("CXJEventAction::LoadFromSttp, 解析动作数据失败");
		return FALSE;
	}

	//是否返回信息
	m_nIsReturn = IsReturnEvent();
	
	WriteLog("CXJEventAction::LoadFromSttp, 结束");
	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件在数据库中的ID(因为新事件是由STTP报文解析来的,不知道在数据库中的ID)
 返 回 值: 查询成功返回TRUE,失败返回FALSE(有结果且仅有一个结果视为查询成功)
**************************************************************/
//##ModelId=49B87BA0002E
BOOL CXJEventAction::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;

	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,ms,recvtime,ms2查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//curtime
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("CURTIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//ms
	Condition condition4;
	str.Format("MS = %d", m_nms);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);

	//recvtime
	Condition condition5;
	strTime = m_tmTime2.Format("%Y-%m-%d %H:%M:%S");
	str.Format("RECVTIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition5, str);
	
	//ms2
	Condition condition6;
	str.Format("MS2 = %d", m_nms2);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition6, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventAction::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventAction::QueryNewEventID,查询失败,原因为%s", sError);
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
Date:2011/11/8  Author:LYH
函数名:   GetSimpleDescribe	
返回值:   CString	事件简要描述字符串
功能概要: 获得事件简要描述
*****************************************************/
CString CXJEventAction::GetSimpleDescribe()
{
	//common
	CString str,strTemp;
	str = "动作:";
	strTemp.Format("[%s.%03d] ",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp;
	str += m_sMsg;
	str += "\r\n";
	str += "[" + m_sStationName+"]";
	str += "[" + m_sDeviceName+"]";
	str += "[" + m_sSecName+"]";    

	return str;
}

//////////////////////////////////////////////////////////////////////////
//CXJEventFault 事故报告

//////////////////////////////////////////////////////////////////////////
//CXJEventDI	开关量
//##ModelId=49B87BA001D4
CXJEventDI::CXJEventDI(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_DI;
	m_nCPU = 0;
	m_nDIID = 0;
	m_nValue = 0;
	m_sDIName = "";
}

//##ModelId=49B87BA00261
CXJEventDI::~CXJEventDI()
{

}

/*************************************************************
 函 数 名：Ack()
 功能概要：开关量变位事件确认(到tb_pt_change_di修改Is_Ack字段)
 返 回 值: TRUE-成功, FALSE-失败
**************************************************************/
//##ModelId=49B87BA002B2
BOOL CXJEventDI::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	
	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_CHANGE_DI_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventDI::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventDI::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;		
	}
	catch(...)
	{
	}
	//写日志到输出窗口
	CString str;
	if(bReturn)
		str.Format("开关量事件[%d]已被确认",m_lID);
	else
		str.Format("开关量事件[%d]确认时出错",m_lID);
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA002FD
CXJEvent* CXJEventDI::Clone()
{
	CXJEventDI* pEvent=new CXJEventDI(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_DI;
	pEvent->m_nCPU			= m_nCPU		;
	pEvent->m_nDIID			= m_nDIID		;
	pEvent->m_nValue		= m_nValue		;
	pEvent->m_sDIName		= m_sDIName		;

	//保护装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_change_di
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA0037A
BOOL CXJEventDI::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //描述
		m_sDIName = m_sMsg; //在MSG字段中保存开关量定义的描述
		m_sSecID = pMemSet->GetValue(2); //保护ID
		str = pMemSet->GetValue(3); //CPU号
		m_nCPU = atoi(str);
		str = pMemSet->GetValue(4); //开关量ID
		m_nDIID = atoi(str);
		str = pMemSet->GetValue(5); //是否确认
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(6); //采样值
		m_nValue = atoi(str);
		str = pMemSet->GetValue(7); //采样时间
		//time exchange
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(8); //采样时间毫秒值
		m_nms = atoi(str);
		str = pMemSet->GetValue(9); //调试?
		m_IsDebug = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventDI::LoadFromDB, 填入数据失败");
		return FALSE;
	}
	
	//建立与设备的关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventDI::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog);
		return FALSE;
	}
	//取得开关量定义
//	GetDIName();
	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取开关量变位信息,（20010）
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA003B9
BOOL CXJEventDI::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20010)
	{
		CString str;
		str.Format("CXJEventDI::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//判断对应保护
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventDI::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//是否调试信息
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//非法索引
		return FALSE;
	}

	//cpu
	m_nCPU = sttpData.sttp_body.nCpu;

	//时间
	CString strTime = sttpData.sttp_body.ch_time_15_BIT1;
	m_tmTime = StringToTimeSttp15(strTime, m_nms);

	//开关量ID
	m_nDIID = sttpData.sttp_body.variant_member.value_data[nIndex].id;

	//采样值
	CString strValue = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	m_nValue = atoi(strValue);

	//取得开关量定义
	GetDIName();

	return TRUE;
}

/*************************************************************
 函 数 名：GetDIName()
 功能概要：根据,二次设备ID, CPU号, 开关量ID,从数据库中查询得到开关量名称
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA1000F
BOOL CXJEventDI::GetDIName()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	//组建查询条件

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	CString str;

	//要查询的字段
	//开关量Name
	Field Name;
	bzero(&Name, sizeof(Field));
	str = "Name";
	strncpy(Name.FieldName, str, str.GetLength());
	Name.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Name);

	//条件
	Condition PT_ID;
	bzero(&PT_ID, sizeof(Condition));
	str.Format("pt_id = '%s'", m_sSecID);
	strncpy(PT_ID.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(PT_ID);

	Condition CPU;
	bzero(&CPU, sizeof(Condition));
	str.Format("cpu_code = %d", m_nCPU);
	strncpy(CPU.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(CPU);

	Condition DI;
	bzero(&DI, sizeof(Condition));
	str.Format("di_id = %d", m_nDIID);
	strncpy(DI.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(DI);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	int nResult;
	nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sDIName = pMemset->GetValue((UINT)0);
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventDI::GetDIName,查询失败,原因为%s", sError);
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
 函 数 名：QueryNewEventID()
 功能概要：查询新事件的ID
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA1004E
BOOL CXJEventDI::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,ms,di_id,value,cpu查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//time
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("TIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//ms
	Condition condition4;
	str.Format("MS = %d", m_nms);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);
	
	//alarm_id
	Condition condition5;
	str.Format("DI_ID = %d", m_nDIID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition5, str);
	
	//value
	Condition condition6;
	str.Format("VALUE = %d", m_nValue);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition6, str);

	//cpu
	Condition condition7;
	str.Format("CPU = %d", m_nCPU);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition7, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_CHANGE_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventDI::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventDI::QueryNewEventID,查询失败,原因为%s", sError);
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
 函 数 名：GetDescribe()
 功能概要：得到事件详细描述
 返 回 值: 事件描述字符串
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA0033C
CString CXJEventDI::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("开关量变位事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站ID: " + m_sStationName + "\r\n";  //厂站名
	str += "一次设备名: " + m_sDeviceName + "\r\n"; //一次设备名称
	str += "二次设备名: " + m_sSecName + "\r\n"; //二次设备名称
	
	if (m_pSec != NULL)
	{
		//显示保护类型
		str += "保护类型: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("是否已确认: %s", (m_nIsAck==0)?"未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("事件: %s %s", m_sDIName, (m_nValue == 1)?"动作":"返回");
	str += strTemp + "\r\n";
	
	return str;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventOSC	录波
//##ModelId=49B87BA100FC
CXJEventOSC::CXJEventOSC(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_OSC;
	m_sFaultDistance = "";
	m_sFileName		= "";
	m_nHaveFault	= 0;
	m_sFilePath		= "";
	m_sTriggerType	= "";
	m_nRecloseTime  = 0;
	m_nDownloaded	= 0;
	m_nFileSize		= 0;
	m_nFileNum		= 0;
	m_nDataSrc		= 0;
	m_nOffset		= 0;
}

//##ModelId=49B87BA10167
CXJEventOSC::~CXJEventOSC()
{

}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA101E4
CXJEvent* CXJEventOSC::Clone()
{
	CXJEventOSC* pEvent=new CXJEventOSC(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_OSC;
	pEvent->m_sFaultDistance = m_sFaultDistance;
	pEvent->m_sFileName		= m_sFileName;
	pEvent->m_nHaveFault	= m_nHaveFault;
	pEvent->m_sFilePath		= m_sFilePath;
	pEvent->m_sTriggerType	= m_sTriggerType;
	pEvent->m_nRecloseTime  = m_nRecloseTime;

	//装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
函 数 名：LoadFromDB()
功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
调用此函数之前的查询需查询所字段,即用select * from tb_pt_osc_report
返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA10242
BOOL CXJEventOSC::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //二次设备ID
		m_sFileName = pMemSet->GetValue(2); //录波数据文件名
		//去除后缀名
		int nFind = m_sFileName.ReverseFind('.');
		if(nFind > -1)
		{
			m_sFileName = m_sFileName.Left(nFind);
		}
		str = pMemSet ->GetValue(3); //录波器启动时间
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //启动时间毫秒值
		//如果大于三位，去除后三位
		if(str.GetLength() > 3)
		{
			str.Delete(str.GetLength() - 3, 3);
		}
		m_nms = atoi(str);
		m_sFilePath = pMemSet->GetValue(5); //存放路径
		str = pMemSet->GetValue(6); //是否有故障
		m_nHaveFault = atoi(str);
		m_sFaultType = pMemSet->GetValue(7); //故障类型
		m_sTriggerType = pMemSet->GetValue(8); //跳闸相别
		str = pMemSet->GetValue(9); //重合闸时间
		m_nRecloseTime = atoi(str);
		str = pMemSet->GetValue(10); //故障测距
		m_sFaultDistance = ComplexExchange(str);
		str = pMemSet->GetValue(14); //是否已确认
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(16); //是否调试信息
		m_IsDebug = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventOSC::LoadFromDB, 填入数据失败");
		return FALSE;
	}
	
	//建立与设备和关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_osc_report
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA10242
BOOL CXJEventOSC::LoadFromDB_(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //二次设备ID
		m_sFileName = pMemSet->GetValue(2); //录波数据文件名
		//去除后缀名
		int nFind = m_sFileName.ReverseFind('.');
		if(nFind > -1)
		{
			m_sFileName = m_sFileName.Left(nFind);
		}
		str = pMemSet ->GetValue(3); //录波器启动时间
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //启动时间毫秒值
		//如果大于三位，去除后三位
		if(str.GetLength() > 3)
		{
			str.Delete(str.GetLength() - 3, 3);
		}
		m_nms = atoi(str);
		m_sFilePath = pMemSet->GetValue(5); //存放路径
		str = pMemSet->GetValue(6);
		m_nFileSize = atoi(str);
		str = pMemSet->GetValue(7); //是否有故障
		m_nHaveFault = atoi(str);
		m_sFaultType = pMemSet->GetValue(8); //故障类型
		m_sTriggerType = pMemSet->GetValue(9); //跳闸相别
		str = pMemSet->GetValue(10); //重合闸时间
		m_nRecloseTime = atoi(str);
		str = pMemSet->GetValue(11); //故障测距
		m_sFaultDistance = ComplexExchange(str);
		str = pMemSet->GetValue(14); //是否已确认
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(16); //是否调试信息
		m_IsDebug = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventOSC::LoadFromDB, 填入数据失败");
		return FALSE;
	}

	//建立与设备和关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	return TRUE;
}
/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取录波事件信息,
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA10280
BOOL CXJEventOSC::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20132)
	{
		CString str;
		str.Format("CXJEventOSC::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//判断对应装置ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventOSC::LoadFromSttp, 找不到对应装置, 装置ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//是否调试信息
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//解析报文
	//时间
	CString sTime = sttpData.sttp_body.variant_member.file_data.ch_time;
	CString strTemp = "";
	strTemp.Format("录波时间为:%s", sTime);
	WriteLog(strTemp);
	m_tmTime = StringToTimeSttp26(sTime, m_nms);
	//文件名
	m_sFileName = XJToMS(sttpData.sttp_body.variant_member.file_data.strFileName);
	//
	m_sFileName.Replace("/", "\\");
	m_sFilePath = m_sFileName;

	//分离路径和文件名
	int nFind = m_sFileName.ReverseFind('\\');
	if(nFind > -1)
	{
		m_sFilePath = m_sFileName.Left(nFind); //路径
		m_sFileName.Delete(0, nFind+1); //名称
	}

	//去除后缀名
	nFind = m_sFileName.ReverseFind('.');
	if(nFind > -1)
	{
		m_sFileName = m_sFileName.Left(nFind);
	}
	//是否有故障
	m_nHaveFault = sttpData.sttp_body.nFlag;
	//故障类型
	m_sFaultType = sttpData.sttp_body.ch_version;
	m_sFaultType.TrimLeft();
	m_sFaultType.TrimRight();
	//跳闸相别
	m_sTriggerType = sttpData.sttp_body.ch_check_code;
	m_sTriggerType.TrimLeft();
	m_sTriggerType.TrimRight();
	//重合闸时间
	m_nRecloseTime = sttpData.sttp_body.nSource;
	//故障测距
	CString str = sttpData.sttp_body.ch_HandlerName;
	str.TrimLeft();
	str.TrimRight();
	m_sFaultDistance = ComplexExchange(str);
	
	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件在数据库中的ID
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA102DE
BOOL CXJEventOSC::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,ms查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//curtime
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("TRIGGER_TIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//filename
	Condition condition4;
	str.Format("FILENAME = '%s'", m_sFileName);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventOSC::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventOSC::QueryNewEventID,查询失败,原因为%s", sError);
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
 函 数 名：Ack()
 功能概要：确认事件,修改数据库表中的确认字段
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA101C5
BOOL CXJEventOSC::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	
	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_OSC_LIST, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventOSC::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventOSC::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;		
	}
	catch(...)
	{
	}
	//写日志到输出窗口
	CString str;
	if(bReturn)
		str.Format("录波报告事件[%d]已被确认",m_lID);
	else
		str.Format("录波报告事件[%d]确认时出错",m_lID);
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取录波事件信息,20026
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA102AF
BOOL CXJEventOSC::LoadFromSttpForPT( STTP_FULL_DATA& sttpData, int nIndex )
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20026)
	{
		CString str;
		str.Format("CXJEventOSC::LoadFromSttpForPT, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//判断对应装置ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventOSC::LoadFromSttpForPT, 找不到对应装置, 装置ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}
	
	//解析报文
	//时间
	CString sTime = sttpData.sttp_body.variant_member.file_list[nIndex].ch_time;
	m_tmTime = StringToTimeSttp26(sTime, m_nms);
	//文件名
	m_sFileName = XJToMS(sttpData.sttp_body.variant_member.file_list[nIndex]. strFielName);
	m_sFileName.Replace("\\", "/");
	m_sFileName.Replace("\\", "/");
	while(m_sFileName.Left(1) == "/")
	{
		m_sFileName.Delete(1);
	}
	//文件大小
	m_nFileSize = sttpData.sttp_body.variant_member.file_list[nIndex].nFileSize;
	//文件编号
	m_nFileNum = sttpData.sttp_body.variant_member.file_list[nIndex]. nFileNO;
	//数据源
	m_nDataSrc = sttpData.sttp_body.nSource;
	//文件路径
	m_sFilePath = XJToMS(sttpData.sttp_body.strRelativePath);
	m_sFilePath.Replace("\\", "/");
	if(m_sFilePath.Right(1) != "/")
		m_sFilePath += "/";

	//判断是否已下载
	m_nDownloaded = IsDownloaded();

	return TRUE;
}

/*************************************************************
 函 数 名：IsDownloaded()
 功能概要：判断当前录波列表是否已下载
 返 回 值: 0-未下载. 1-以下载. 2-部分下载
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1030D
int CXJEventOSC::IsDownloaded()
{
	CString strFilePath = GetFilePath();
	//检查文件是否存在
	CFileStatus   status;	   
	if(CFile::GetStatus(strFilePath ,status) != TRUE)
	{
		//未找到文件, 查找是否有.tmp零时文件
		CString str = GetFilePathNoSuff();
		str += ".tmp";
		if(CFile::GetStatus(str, status) == TRUE)
		{
			//找到此文件
			m_nOffset = (int)(status.m_size);
			return 2;
		}
	}
	else
		return 1;
	return 0;
}

/*************************************************************
 函 数 名：GetFilePath()
 功能概要：得到本地录波文件全路径
 返 回 值: 本地录波文件全路径
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1032C
CString CXJEventOSC::GetFilePath()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//得到基址
	CString strDownDir = m_pDataEngine->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\保护\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\录波器\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//加上文件名
	strFullPath += m_sFileName;
	//加上.cfg后缀
	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 函 数 名：GetDescribe()
 功能概要：得到事件详细描述
 返 回 值: 事件描述字符串
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA10222
CString CXJEventOSC::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("录波事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站ID: " + m_sStationName + "\r\n";  //厂站名
	str += "一次设备名: " + m_sDeviceName + "\r\n"; //一次设备名称
	str += "二次设备名: " + m_sSecName + "\r\n"; //二次设备名称
	
	if (m_pSec != NULL)
	{
		//显示保护类型
		str += "保护类型: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("是否已确认: %s", (m_nIsAck==0)?"未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("录波文件名称: %s", m_sFileName);
	str += strTemp + "\r\n";

	strTemp.Format("录波文件大小: %d", m_nFileSize);
	str += strTemp + "\r\n";
	
	strTemp.Format("故障类型: %s", GetFaultType(m_sFaultType));
	str += strTemp + "\r\n";

	strTemp.Format("跳闸相别: %s", GetTriggerType(m_sTriggerType));
	str += strTemp + "\r\n";

	strTemp.Format("故障测距: %s", m_sFaultDistance);
	str += strTemp + "\r\n";
	
	return str;
}

/*************************************************************
 函 数 名：GetDownloadString()
 功能概要：得到是否下载的文字描述
 返 回 值: 是否下载的文字描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA10399
CString CXJEventOSC::GetDownloadString()
{
	if(m_nDownloaded == 0)
		return "未下载";
	else if(m_nDownloaded == 1)
		return "已下载";
	else if(m_nDownloaded == 2)
		return "部分下载";
	else if(m_nDownloaded == -1)
		return "下载失败";
	else if(m_nDownloaded == -2)
		return "取消下载";
	return "";
}

/*************************************************************
 函 数 名：GetFilePathNoSuff()
 功能概要：得到不带后缀名的路径
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1034B
CString CXJEventOSC::GetFilePathNoSuff()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//得到基址
	CString strDownDir = m_pDataEngine->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\保护\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\录波器\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//加上文件名
	CString strName = m_sFileName;
	strName.Replace("/", "\\");
	int nFind = strName.ReverseFind('\\');
	if(nFind > -1)
	{
		//带了路径的文件名, 则只取文件名部分
		strName = strName.Right(strName.GetLength() - nFind - 1);
	}
	strFullPath += strName;
	//加上.cfg后缀
//	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 函 数 名：GetFileRelatePath()
 功能概要：得到不带后缀名的相对路径
 返 回 值: 相对路径
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1037A
CString CXJEventOSC::GetFileRelatePath()
{
	if(m_pSec == NULL)
		return "";
	//组织路径
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = m_pSec->m_sStationID + "\\保护\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = m_pSec->m_sStationID + "\\录波器\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//加上文件名
	CString strName = m_sFileName;
	strName.Replace("/", "\\");
	int nFind = strName.ReverseFind('\\');
	if(nFind > -1)
	{
		//带了路径的文件名, 则只取文件名部分
		strName = strName.Right(strName.GetLength() - nFind - 1);
	}
	strFullPath += strName;
	//加上.cfg后缀
	//	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 函 数 名: GetFileDir()
 功能概要: 取得文件目录
 返 回 值: 文件目录
 参    数: param1 
		   Param2 
**************************************************************/
CString CXJEventOSC::GetFileDir()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//得到基址
	CString strDownDir = m_pDataEngine->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\保护\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\录波器\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";

	return strFullPath;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventPTComm 保护通讯状态
//##ModelId=49B87BA20011
CXJEventPTComm::CXJEventPTComm(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_PTCOMM;
}

//##ModelId=49B87BA2004E
CXJEventPTComm::~CXJEventPTComm()
{

}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA2008C
CXJEvent* CXJEventPTComm::Clone()
{
	CXJEventPTComm* pEvent=new CXJEventPTComm(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_PTCOMM;
	pEvent->m_nStatus		= m_nStatus		;

	//装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_commu_status
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA200CB
BOOL CXJEventPTComm::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //二次设备ID
		str = pMemSet->GetValue(2); //状态
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //时间
		//m_tmTime  exchange time
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //是否已确认
		m_nIsAck = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventPTComm::LoadFromDB, 填入数据失败");
		return FALSE;
	}

	//建立与设备的关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取保护通讯状态信息,(20002)
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA200EA
BOOL CXJEventPTComm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20002)
	{
		CString str;
		str.Format("CXJEventPTComm::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}

	//判断索引
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//非法索引
		return FALSE;
	}
	
	//判断对应装置ID
	m_sSecID = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventPTComm::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//状态
	m_nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].id;

	//时间
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件的ID
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA20109
BOOL CXJEventPTComm::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,status查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//curtime
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("CURTIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//status
	Condition condition4;
	str.Format("STATUS = %d", m_nStatus);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventPTComm::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventPTComm::QueryNewEventID,查询失败,原因为%s", sError);
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
 函 数 名：Ack()
 功能概要：确认事件, 数据库表中IS_ACK字段改为1
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA2007D
BOOL CXJEventPTComm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	
	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventPTComm::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventPTComm::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}
	CString str;
	if(bReturn)
	{
		str.Format("保护通讯状态变位事件[%d]已被确认",m_lID);
	}
	else
	{
		str.Format("保护通讯状态变位事件[%d]确认时出错",m_lID);
	}
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 函 数 名：GetDescribe()
 功能概要：得到事件详细描述
 返 回 值: 事件描述字符串
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA200AB
CString CXJEventPTComm::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("通讯状态变位事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站ID: " + m_sStationName + "\r\n";  //厂站名
	str += "一次设备名: " + m_sDeviceName + "\r\n"; //一次设备名称
	str += "二次设备名: " + m_sSecName + "\r\n"; //二次设备名称
	
	if (m_pSec != NULL)
	{
		//显示保护类型
		str += "保护类型: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("是否已确认: %s", (m_nIsAck==0)?"未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("通讯状态: %s", (m_nStatus == 0)?"正常":"断开");
	str += strTemp + "\r\n";
	
	return str;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventPTRun	保护运行状态
//##ModelId=49B87BA20169
CXJEventPTRun::CXJEventPTRun(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_PTRUN;
}

//##ModelId=49B87BA20196
CXJEventPTRun::~CXJEventPTRun()
{

}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA201C5
CXJEvent* CXJEventPTRun::Clone()
{
	CXJEventPTRun* pEvent=new CXJEventPTRun(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_PTRUN;
	pEvent->m_nStatus		= m_nStatus		;

	//装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_run_status
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA201F4
BOOL CXJEventPTRun::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //二次设备ID
		str = pMemSet->GetValue(2); //状态
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //时间
		//m_tmTime  exchange time
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //是否已确认
		m_nIsAck = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventPTRun::LoadFromDB, 填入数据失败");
		return FALSE;
	}

	//建立与设备的关联关系
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取保护运行状态信息,(20079)
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA20203
BOOL CXJEventPTRun::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CXJEventPTRun::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//判断对应装置ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventPTRun::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//状态
	m_nStatus = sttpData.sttp_body.nStatus;

	//时间
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件在数据库中的ID
 返 回 值: 查询成功返回TRUE,失败返回FALSE
**************************************************************/
//##ModelId=49B87BA20222
BOOL CXJEventPTRun::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,curtime,status查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition2;
	str.Format("PT_ID = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//curtime
	Condition condition3;
	CString strTime = m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	str.Format("CURTIME = '%s'", strTime);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//status
	Condition condition4;
	str.Format("STATUS = %d", m_nStatus);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventPTRun::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventPTRun::QueryNewEventID,查询失败,原因为%s", sError);
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
 函 数 名：Ack()
 功能概要：确认事件,改变数据表中的确认字段
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA201B6
BOOL CXJEventPTRun::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	
	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_PT_RUN_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventPTRun::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventPTRun::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}
	CString str;
	if(bReturn)
	{
		str.Format("保护运行状态变位事件[%d]已被确认",m_lID);
	}
	else
	{
		str.Format("保护运行状态变位事件[%d]确认时出错",m_lID);
	}
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 函 数 名：GetDescribe()
 功能概要：得到事件详细描述
 返 回 值: 事件描述字符串
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA201D4
CString CXJEventPTRun::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("运行状态变位事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站ID: " + m_sStationName + "\r\n";  //厂站名
	str += "一次设备名: " + m_sDeviceName + "\r\n"; //一次设备名称
	str += "二次设备名: " + m_sSecName + "\r\n"; //二次设备名称
	
	if (m_pSec != NULL)
	{
		//显示保护类型
		str += "保护类型: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("是否已确认: %s", (m_nIsAck==0)?"未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	CString str1 = "";
	str1 = GetRunStatusString(m_nStatus);

	strTemp.Format("运行状态: %s", str1);
	str += strTemp + "\r\n";
	
	return str;
}

/****************************************************
Date:2011/10/17  Author:LYH
函数名:   GetRunStatusString	
返回值:   CString	
功能概要: 
参数: int nStatus	
*****************************************************/
CString CXJEventPTRun::GetRunStatusString( int nStatus )
{
	CString sReturn = "";
	switch(nStatus)
	{
	case 0:
		sReturn = "检修";
		break;
	case 1:
		sReturn = "停运";
		break;
	case 2:
		sReturn = "投运";
		break;
	case 3:
		sReturn = "未接入";
		break;
	case 4:
		sReturn = "调试(对码表)";
		break;
	default:
		sReturn = "未知";
	}
	return sReturn;
}

//////////////////////////////////////////////////////////////////////////
//CXJEventSysAlarm 系统异常告警
//##ModelId=49B87BA202D0
CXJEventSysAlarm::CXJEventSysAlarm(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_SYSALARM;
	m_sDetail = "";
	m_arrDetail.RemoveAll();
}

//##ModelId=49B87BA202EE
CXJEventSysAlarm::~CXJEventSysAlarm()
{
	for(int i = 0; i < m_arrDetail.GetSize(); i++)
	{
		SYSALARM* sa = (SYSALARM*)m_arrDetail.GetAt(i);
		if(sa == NULL)
			continue;
		delete sa;
	}
	m_arrDetail.RemoveAll();
}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA2032C
CXJEvent* CXJEventSysAlarm::Clone()
{
	CXJEventSysAlarm* pEvent=new CXJEventSysAlarm(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_sFaultType	= m_sFaultType;

	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;

	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;

	//special
	pEvent->m_nEventType	= XJ_EVENT_SYSALARM;
	pEvent->m_nType			= m_nType		;
	pEvent->m_sDetailMsg	= m_sDetailMsg;
	pEvent->m_sCPU			= m_sCPU;
	pEvent->m_sZone			= m_sZone;
	pEvent->m_sDetail		= m_sDetail;

	//装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_sys_alarm
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA2033C
BOOL CXJEventSysAlarm::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sStationId = pMemSet->GetValue(1); //站ID
		m_sSecID = pMemSet->GetValue(2); //PT_ID
		str = pMemSet->GetValue(3); //类型
		m_nType = atoi(str);
		m_sMsg = pMemSet->GetValue(4); //告警描述
		str = pMemSet->GetValue(6);	//时间
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(9); //已确认
		m_nIsAck = atoi(str);
		m_sDetailMsg = pMemSet->GetValue(10); //详细描述
		m_sCPU = pMemSet->GetValue(11); //CPU号
		m_sZone = pMemSet->GetValue(12); //定值区号
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventSysAlarm::LoadFromDB, 填入数据失败");
		return FALSE;
	}
	//关联设备
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, 建立与设备%s的关联关系失败", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
	}
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取系统告警信息,
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA2034B
BOOL CXJEventSysAlarm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//检查是否20155报文
	if(sttpData.sttp_head.uMsgID != 20155)
		return FALSE;
	try
	{
		//PT_ID
		m_sSecID = sttpData.sttp_body.ch_pt_id;
		//zone
		m_sZone.Format("%d", sttpData.sttp_body.nZone);
		//cpu
		m_sCPU.Format("%d", sttpData.sttp_body.nCpu);
		//type
		m_nType = sttpData.sttp_body.nEventType;
		//简单描述
		m_sMsg = GetMsg();
		//项数
		int nCount = sttpData.sttp_body.variant_member.value_data.size();
		CString str = "";
		str.Format("%d", nCount);
		//循环组织详细描述语句
		for(int i = 0; i < nCount; i++)
		{
			CString strTemp = "";
			//id
			strTemp.Format("%d", sttpData.sttp_body.variant_member.value_data[i].id);
			//加入到语句
			str += ",";
			str += strTemp;
			//old value
			strTemp = XJToMS(sttpData.sttp_body.variant_member.value_data[i].str_value);
			//加入到语句
			str += ",";
			str += strTemp;
			//new value
			strTemp = XJToMS(sttpData.sttp_body.variant_member.value_data[i].str_reserved);
			//加入到语句
			str += ",";
			str += strTemp;
		}
		m_sDetailMsg = str;
		m_tmTime = CTime::GetCurrentTime();
		if(!BuildReleation())
		{
			return FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJEventSysAlarm::LoadFromSttp 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件在数据库中的ID
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA2036B
BOOL CXJEventSysAlarm::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////根据PT_ID,Cpu_code,type,Zone_code查找id
	
	CString str;	
	//字段
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//Host_name
	Condition condition2;
	str.Format("Host_name = '%s'", m_pSec->m_sID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
	
	//Cpu_code
	Condition condition3;
	str.Format("Cpu_code = %s", m_sCPU);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);
	
	//Type
	Condition condition4;
	str.Format("Type = %d", m_nType);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition4, str);

	//发生时间
	Condition condition5;
	str.Format("start_time = '%s'", m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition5, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_SYS_ALARM_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventSysAlarm::QueryNewEventID, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//查找到ID
			CString str;
			str = pMemset->GetValue((UINT)0);
			m_lID = atoi(str);
		}
		else
		{
			bReturn = FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventSysAlarm::QueryNewEventID,查询失败,原因为%s", sError);
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
 函 数 名：Ack()
 功能概要：确认事件,改变数据表中的确认字段
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA20321
BOOL CXJEventSysAlarm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//已经确认过
		return FALSE;
	
	try
	{
		//去数据库修改
		if(m_lID < 0)
		{
			//新事件, 还不知道事件ID,查找ID先
			if(!QueryNewEventID())
			{
				//查找不到ID不确认
				return FALSE;
			}
		}
		//到数据库修改Is_Ack字段
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//指定ID,更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//条件
		//id
		Condition condition2;
		str.Format("ID = %d", m_lID);
		m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = m_pDataEngine->GetDBEngine()->XJUpdateData(EX_STTP_INFO_SYS_ALARM_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJEventSysAlarm::Ack, 更新失败");
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
			m_nIsAck = 1;
		}
		else
		{
			CString str;
			str.Format("CXJEventSysAlarm::Ack,更新失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}
	CString str;
	if(bReturn)
	{
		str.Format("系统告警事件[%d]已被确认",m_lID);
	}
	else
	{
		str.Format("系统告警事件[%d]确认时出错",m_lID);
	}
	WriteLog(str);
	return bReturn;
}

/*************************************************************
 函 数 名：GetDescribe()
 功能概要：获得事件详细描述
 返 回 值: 详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2035B
CString CXJEventSysAlarm::GetDescribe()
{
//	1－定值区号；2－软压板；3－与前一次对比或子站直接上送的定值变化; 4-与基准值对比定值变化; 5-子站配置变化
	if(m_nType == 1)
	{
		//定值区号
		return GetDescribeZone();
	}
	else if(m_nType == 2)
	{
		//软压板变化
		return GetDescribeSoftBoard();
	}
	else if(m_nType == 3)
	{
		//定值变化
		return GetDescribeSetting();
	}
	else if(m_nType == 4)
	{
		//与基准值对比定值变化
		return GetDescribeOrder();
	}
	else if(m_nType == 5)
	{
		//子站配置变化
		return GetDescribeSubStation();
	}
	return "";
}

/*************************************************************
 函 数 名：GetDescribeSubStation()
 功能概要：取得子站配置变化详细描述
 返 回 值: 子站配置变化详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2037A
CString CXJEventSysAlarm::GetDescribeSubStation()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("系统告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站名:   " + m_sStationName  + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "以下是子站配置变化信息:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：TranslateDetailMsg()
 功能概要：解析事件详细描述保存在m_sDetail中
 返 回 值: 
 参    数：param1	事件类型
		   Param2
**************************************************************/
//##ModelId=49B87BA2038D
void CXJEventSysAlarm::TranslateDetailMsg( int nType )
{
	if(nType == 1)
	{
		//定值区号变化信息
		TranslateZone();
		return;
	}
	else if(nType == 2)
	{
		//软压板变化信息
		TranslateSoftBoard();
	}
	else if(m_nType == 3)
	{
		//定值变化
		TranslateSetting();
	}
	else if(m_nType == 4)
	{
		//与基准值对比定值变化
		TranslateOrder();
	}
	else if(m_nType == 5)
	{
		//子站配置变化
		TranslateSubStation();
	}
}

/*************************************************************
 函 数 名：TranslateSubStation()
 功能概要：解析子站配置变化的详细描述
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2039A
void CXJEventSysAlarm::TranslateSubStation()
{
	if(m_sDetail != "")
	{
		//表示已解析过, 不再解析
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//没有详细信息
		return;
	}

	int nCount = 0;
	int nPos = -1;
	try
	{
		//查找第一个逗号, 得到项目数
		nPos = m_sDetailMsg.Find(',', 0);
		if(nPos == -1)
			return;
		//项目数
		nCount = atoi(m_sDetailMsg.Left(nPos));
		if(nCount < 1)
			return;
		//循环读取所有项目
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";
		//项数
		str.Format("(共%d项)\r\n", nCount);
		CString strData = m_sDetailMsg;
		for(int i = 0; i < nCount; i++)
		{
			strData.Delete(0, nPos1 + 1);
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到项目
			CString strTemp = strData.Left(nPos1);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
	}
	catch (...)
	{
		WriteLog("解析子站配置变化的详细描述发生错误", 3);
	}
}

/*************************************************************
 函 数 名：TranslateSetting()
 功能概要：解析定值变化的详细描述
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203A9
void CXJEventSysAlarm::TranslateSetting()
{
	if(m_sDetail != "")
	{
		//表示已解析过, 不再解析
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//没有详细信息
		return;
	}
	if(m_pSec == NULL)
		return;

	//清空定值配置
	ClearSettingConfig();
	//读入定值配置
	GetSettingConfig(m_pSec->m_sID, atoi(m_sCPU), atoi(m_sZone));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//查找第一个逗号, 得到项目数
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//项目数
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//循环读取所有项目
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//项数
		str.Format("(共%d项)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到定值ID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//得到此定值ID的描述
			CString strName = GetSettingDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;

			//旧值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到旧值
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;

			//新值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//得到新值
				strTemp = strData.Left(nPos1);
				nPos2 = nPos1;
				strData.Delete(0, nPos2 + 1);
			}
			sa->sNewValue = strTemp;

			m_arrDetail.Add(sa);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
		ClearSettingConfig();
	}
	catch (...)
	{
		WriteLog("解析定值变化的详细描述发生错误", 3);
	}
}

/*************************************************************
 函 数 名：GetSettingDes()
 功能概要：得到指定定值ID的定值描述
 返 回 值: 定值描述
 参    数：param1	定值ID
		   Param2	
**************************************************************/
//##ModelId=49B87BA203BA
CString CXJEventSysAlarm::GetSettingDes(CString sID)
{
	if(arrSetting.GetSize() < 1)
		return "";
	for(int i = 0; i < arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)arrSetting.GetAt(i);
		if(pts->ID == sID)
		{
			return pts->Name;
		}
	}
	return "";
}

/*************************************************************
 函 数 名：TranslateZone()
 功能概要：解析定值区号变化详细描述
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203CB
void CXJEventSysAlarm::TranslateZone()
{
	if(m_sDetail != "")
	{
		//表示已解析过, 不再解析
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//没有详细信息
		return;
	}
	if(m_pSec == NULL)
		return;
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//查找第一个逗号, 得到项目数
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//项目数
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//循环读取所有项目
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//项数
		str.Format("(共%d项)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到定值ID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			
// 			CString strName = GetSettingDes(m_pSec->m_sID, atoi(strTemp));
// 			str += strName;
// 			str += "\t";
			SYSALARM* sa = new SYSALARM;
			sa->sName = strTemp;
			
			//旧值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到旧值
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//新值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//得到新值
				strTemp = strData.Left(nPos1);
				nPos2 = nPos1;
				strData.Delete(0, nPos2 + 1);
			}
			sa->sNewValue = strTemp;

			m_arrDetail.Add(sa);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
	}
	catch (...)
	{
		WriteLog("解析定值区变化的详细描述发生错误", 3);
	}
}

/*************************************************************
 函 数 名：GetDescribeSetting()
 功能概要：取得定值变化的详细描述
 返 回 值: 定值变化详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2037B
CString CXJEventSysAlarm::GetDescribeSetting()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("系统告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站名:   " + m_sStationName  + "\r\n";
	str += "一次设备名:   " + m_sDeviceName  + "\r\n";
	str += "二次设备名:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "设备类型: " + m_pSec->m_sModel + "\r\n";
	else
		str += "设备类型: 未知\r\n";
	str += "CPU号:	" + m_sCPU + "\r\n";
	str += "定值区号:	" + m_sZone + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "以下是定值变化信息:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：TranslateSoftBoard()
 功能概要：解析软压板变化的详细描述
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203CC
void CXJEventSysAlarm::TranslateSoftBoard()
{
	if(m_sDetail != "")
	{
		//表示已解析过, 不再解析
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//没有详细信息
		return;
	}
	if(m_pSec == NULL)
		return;

	//清除配置
	ClearSoftBoardConfig();
	//读取配置
	GetSoftBoardConfig(m_pSec->m_sID, atoi(m_sCPU));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//查找第一个逗号, 得到项目数
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//项目数
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//循环读取所有项目
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//项数
		str.Format("(共%d项)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到定值ID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//得到此定值ID的描述
			CString strName = GetSoftBoardDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;
			
			//旧值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到旧值
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//新值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//得到新值
				strTemp = strData.Left(nPos1);
				nPos2 = nPos1;
				strData.Delete(0, nPos2 + 1);
			}
			sa->sNewValue = strTemp;

			m_arrDetail.Add(sa);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
		//清除配置
		ClearSoftBoardConfig();
	}
	catch (...)
	{
		WriteLog("解析软压板变化的详细描述发生错误", 3);
	}
}

/*************************************************************
 函 数 名：GetSoftBoardDes()
 功能概要：得到指定软压板ID的定值描述
 返 回 值: 软压板描述
 参    数：Param1	软压板ID
**************************************************************/
//##ModelId=49B87BA203D8
CString CXJEventSysAlarm::GetSoftBoardDes(CString sID)
{
	if(arrSoftBoard.GetSize() < 1)
		return "";
	for(int i = 0; i < arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* pts = (PT_SOFTBOARD*)arrSoftBoard.GetAt(i);
		if(pts->ID == sID)
		{
			return pts->Name;
		}
	}
	return "";
}

/*************************************************************
 函 数 名：GetDescribeSoftBoard()
 功能概要：取得软压板变化的详细描述
 返 回 值: 软压板变化的详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038A
CString CXJEventSysAlarm::GetDescribeSoftBoard()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("系统告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站名:   " + m_sStationName  + "\r\n";
	str += "一次设备名:   " + m_sDeviceName  + "\r\n";
	str += "二次设备名:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "设备类型: " + m_pSec->m_sModel + "\r\n";
	else
		str += "设备类型: 未知\r\n";
	str += "CPU号:	" + m_sCPU + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "以下是软压板变化信息:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：GetDescribeZone()
 功能概要：取得定值区变化的详细描述
 返 回 值: 定值区变化的详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038B
CString CXJEventSysAlarm::GetDescribeZone()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("系统告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站名:   " + m_sStationName  + "\r\n";
	str += "一次设备名:   " + m_sDeviceName  + "\r\n";
	str += "二次设备名:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "设备类型: " + m_pSec->m_sModel + "\r\n";
	else
		str += "设备类型: 未知\r\n";
	str += "CPU号:	" + m_sCPU + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "以下是定值区变化信息:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：GetDescribeOrder()
 功能概要：取得与基准值对比定值变化的详细描述
 返 回 值: 详细描述
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038C
CString CXJEventSysAlarm::GetDescribeOrder()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("系统告警事件ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "厂站名:   " + m_sStationName  + "\r\n";
	str += "一次设备名:   " + m_sDeviceName  + "\r\n";
	str += "二次设备名:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "设备类型: " + m_pSec->m_sModel + "\r\n";
	else
		str += "设备类型: 未知\r\n";
	str += "CPU号:	" + m_sCPU + "\r\n";
	str += "定值区号:	" + m_sZone + "\r\n";
	
	strTemp.Format("是否已确认: %s", (m_nIsAck==0) ? "未确认" : "已确认" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("发生时间: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "以下是与基准值对比定值变化信息:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 函 数 名：TranslateOrder()
 功能概要：解析与基准值对比定值的详细描述
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203B9
void CXJEventSysAlarm::TranslateOrder()
{
	if(m_sDetail != "")
	{
		//表示已解析过, 不再解析
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//没有详细信息
		return;
	}
	if(m_pSec == NULL)
		return;

	//清空定值配置
	ClearSettingConfig();
	//读入定值配置
	GetSettingConfig(m_pSec->m_sID, atoi(m_sCPU), atoi(m_sZone));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//查找第一个逗号, 得到项目数
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//项目数
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//循环读取所有项目
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";
		//项数
		str.Format("(共%d项)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到定值ID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//得到此定值ID的描述
			CString strName = GetSettingDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;
			
			//旧值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//得到旧值
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//新值
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//得到新值
				strTemp = strData.Left(nPos1);
				nPos2 = nPos1;
				strData.Delete(0, nPos2 + 1);
			}
			sa->sNewValue = strTemp;

			m_arrDetail.Add(sa);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
		ClearSettingConfig();
	}
	catch (...)
	{
		WriteLog("解析与基准值相比定值变化的详细描述发生错误", 3);
	}
}

/*************************************************************
 函 数 名：GetMsg()
 功能概要：得到简单描述信息
 返 回 值: 简单描述信息
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30001
CString CXJEventSysAlarm::GetMsg()
{
	CString str = "";
	if(m_nType == 1)
	{
		str = "定值区号变化";
	}
	else if(m_nType == 2)
	{
		str = "软压板变化";
	}
	else if(m_nType == 3)
	{
		str = "与前一次对比或子站直接上送的定值变化";
	}
	else if(m_nType == 4)
	{
		str = "与基准值对比定值变化";
	}
	else if(m_nType == 5)
	{
		str = "子站配置变化";
	}
	return str;
}

/*************************************************************
 函 数 名：GetSettingConfig()
 功能概要：读取指定保护指定CPU下的所有定值定义
 返 回 值: 
 参    数：param1	指定保护
		   Param2	指定CPU号
		   param3	指定定值区号
**************************************************************/
//##ModelId=49B87BA203BC
void CXJEventSysAlarm::GetSettingConfig( CString PT_ID, int nCPU, int nZone)
{
	MYASSERT(m_pDataEngine);
	MYASSERT(m_pDataEngine->GetDBEngine());

	//与数据平台访问DLL的连接可用
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//SETTING_ID
	Field field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, field1, "setting_id", EX_STTP_DATA_TYPE_INT);

	Field field2;
	m_pDataEngine->GetDBEngine()->SetField(sql, field2, "name", EX_STTP_DATA_TYPE_STRING);

	CString str;
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", PT_ID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition1, str);
	
	//CPU
	Condition condition3;
	str.Format("cpu_code = %d", nCPU);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);

	//zone 2009-5-5 去除定值区号的指定
// 	Condition condition4;
// 	str.Format("zone=%d", nZone);
// 	pApp->m_DBEngine.SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventSysAlarm::GetSettingConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//创建定值对象
			PT_SETTING * pts = new PT_SETTING;
			//为了简单,只保存ID和NAME
			CString str;
			pts->ID = pMemset->GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset->GetValue(1); //NAME
			
			arrSetting.Add(pts);
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventSysAlarm::GetSettingConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
}

/*************************************************************
 函 数 名：ClearSettingConfig()
 功能概要：清除定值定义
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30002
void CXJEventSysAlarm::ClearSettingConfig()
{
	for(int i = 0; i < arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)arrSetting.GetAt(i);
		delete pts;
	}
	arrSetting.RemoveAll();
}

/*************************************************************
 函 数 名：GetSoftBoardConfig()
 功能概要：得到指定保护指定CPU下的软压板定义
 返 回 值: 
 参    数：param1	保护ID
		   Param2	CPU号
**************************************************************/
//##ModelId=49B87BA203DA
void CXJEventSysAlarm::GetSoftBoardConfig( CString PT_ID, int nCPU )
{
	MYASSERT(m_pDataEngine);
	MYASSERT(m_pDataEngine->GetDBEngine());
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code查找SOFT_ID,NAME
		
	CString str;
		
	//字段
	//SOFT_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "SOFT_ID";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);

	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", PT_ID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CXJEventSysAlarm::GetSoftBoardConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			PT_SOFTBOARD * softboard = new PT_SOFTBOARD;
			softboard->ID = pMemset->GetValue((UINT)0); //DI_ID
			softboard->Name = pMemset->GetValue(1); //NAME
			arrSoftBoard.Add(softboard);
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CXJEventSysAlarm::GetSoftBoardConfig,查询失败,原因为%s", sError);
		WriteLog(str);
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 函 数 名：ClearSoftBoardConfig()
 功能概要：清除软压板定义
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30003
void CXJEventSysAlarm::ClearSoftBoardConfig()
{
	for(int i = 0; i < arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* pts = (PT_SOFTBOARD*)arrSoftBoard.GetAt(i);
		delete pts;
	}
	arrSoftBoard.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////
//CXJEventComm	主子站通讯状态事件
//##ModelId=49B87BA30030
CXJEventComm::CXJEventComm(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_COMM;
	m_nStatus = 0;
}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制事件对象
 返 回 值: 复制生成的事件对象
**************************************************************/
//##ModelId=49B87BA30042
CXJEvent* CXJEventComm::Clone()
{
	CXJEventComm* pEvent=new CXJEventComm(m_pDataEngine);
	//common
	pEvent->m_lID			= m_lID		;
	pEvent->m_nIsAck		= m_nIsAck	;
	pEvent->m_sMsg			= m_sMsg	;
	pEvent->m_tmTime		= m_tmTime	;
	pEvent->m_nms			= m_nms		;
	pEvent->m_sFaultType	= m_sFaultType;
	
	//releation
	pEvent->m_sSecName		= m_sSecName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_sDeviceId		= m_sDeviceId;
	pEvent->m_sDeviceName	= m_sDeviceName	;
	pEvent->m_sStationName	= m_sStationName;
	pEvent->m_sNetName		= m_sNetName	;
	
	//是否调试信息
	pEvent->m_IsDebug = m_IsDebug;
	
	//special
	m_nEventType			= XJ_EVENT_COMM;
	pEvent->m_nStatus		= m_nStatus		;
	
	//装置指针
	pEvent->m_pSec = m_pSec;
	
	return pEvent;	
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from tb_pt_station_status
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA30044
BOOL CXJEventComm::LoadFromDB( CMemSet * pMemSet )
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //事件ID
		m_lID = atol(str);
		m_sStationName = pMemSet->GetValue(1); //站名
		str = pMemSet->GetValue(2); //通讯状态
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //时间
		m_tmTime = StringToTime(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventComm::LoadFromDB, 填入数据失败");
		return FALSE;
	}	
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：从一条STTP报文读取主子站通讯变位信息,
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 STTP报文数据结构
		   param2 指定是报文数据的第几个数据项
**************************************************************/
//##ModelId=49B87BA30047
BOOL CXJEventComm::LoadFromSttp( STTP_FULL_DATA& sttpData, int nIndex )
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20144)
	{
		CString str;
		str.Format("CXJEventComm::LoadFromSttp, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//判断对应装置ID
	m_sStationId = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//创建关联关系失败
		CString str;
		str.Format("CXJEventComm::LoadFromSttp, 找不到对应保护, 保护ID为%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}
	
	//状态
	m_nStatus = sttpData.sttp_body.nStatus;
	
	//时间
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildReleation()
 功能概要：建立事件与设备的关系,重载父类实现, 这里只需要找到厂站的信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   二次设备类型
**************************************************************/
//##ModelId=49B87BA3004B
BOOL CXJEventComm::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);

	//厂站ID
	m_sStationId = m_pSec->m_sStationID;
	CStationObj * pStation = (CStationObj*)m_pDataEngine->FindDevice(m_sStationId, TYPE_STATION);
	if(pStation != NULL)
	{
		//厂站名称
		m_sStationName = pStation->m_sName;
		CNetObj * pNet = (CNetObj*)m_pDataEngine->FindDevice(pStation->m_sNetID, TYPE_NET);
		if(pNet != NULL)
		{
			//地区名称
			m_sNetName = pNet->m_sName;
		}
	}
	else
		return FALSE;
	
	return TRUE;
}

/****************************************************
Date:2011/10/18  Author:LYH
函数名:   ~CXJEventComm	
返回值:   	
功能概要: 
*****************************************************/
CXJEventComm::~CXJEventComm()
{

}
