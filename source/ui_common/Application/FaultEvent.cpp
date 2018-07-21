// FaultEvent.cpp : implementation file
//

#include "FaultEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaultEvent

IMPLEMENT_DYNCREATE(CFaultEvent, CObject)

//##ModelId=49B87B9A01C7
CFaultEvent::CFaultEvent()
{
	m_strUnit = "";
	/** @brief           故障报告事件类型*/
	m_nType = 0;
	/** @brief           在数据中的ID*/
	m_nID = 0;
	m_nMainRecID = 0;
	/** @brief           二次设备ID*/
	m_sSecID = "";
	/** @brief           事件定义*/
	m_sEventDef = "";
	/** @brief           事件值*/
	m_sEventContent = "";
	/** @brief           事件定义名称*/
	m_sEventDefName = "";
	m_nmsStart = 0;
	m_nmsReceive = 0;
	m_nmsSign = 0;
	m_nCPU = -1;
	m_sDir = _T("");

	CTime tm(1972, 1, 1, 1, 1,1);
	m_tmStart = tm;
	m_tmSign = tm;
	m_tmReceiveTime  = tm;

	m_pSec = NULL;

	m_bSelected = FALSE;
	m_nNum = -1;
}

/****************************************************
Date:2011/10/17  Author:LYH
函数名:   CFaultEvent	
返回值:   	
功能概要: 
参数: CDataEngine * pDataEngine	
*****************************************************/
CFaultEvent::CFaultEvent( CDataEngine* pDataEngine )
{
	m_pDataEngine = pDataEngine;

	m_strUnit = "";
	/** @brief           故障报告事件类型*/
	m_nType = 0;
	/** @brief           在数据中的ID*/
	m_nID = 0;
	m_nMainRecID = 0;
	/** @brief           二次设备ID*/
	m_sSecID = "";
	/** @brief           事件定义*/
	m_sEventDef = "";
	/** @brief           事件值*/
	m_sEventContent = "";
	/** @brief           事件定义名称*/
	m_sEventDefName = "";
	m_nmsStart = 0;
	m_nmsReceive = 0;
	m_nmsSign = 0;
	m_nCPU = -1;
	m_sDir = _T("");

	CTime tm(1972, 1, 1, 1, 1,1);
	m_tmStart = tm;
	m_tmSign = tm;
	m_tmReceiveTime  = tm;

	m_pSec = NULL;

	m_bSelected = FALSE;
	m_nNum = -1;
}

//##ModelId=49B87B9A01C8
CFaultEvent::~CFaultEvent()
{
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：从数据库中读取事件信息
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数：param1	数据集指针
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FA
BOOL CFaultEvent::LoadFromDB( CMemSet* pMemset )
{
	//判断数据有效性
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0);
	m_nID = atoi(str); //ID
	str = pMemset->GetValue(1);
	m_nMainRecID = atoi(str); //MainRecID
	m_sSecID = pMemset->GetValue(2); //PTID
	str = pMemset->GetValue(3); 
	m_nType = atoi(str);//事件类型
	str = pMemset->GetValue(4);//启动时间
	if(m_nType < XJ_FAULT_EVENT_OSCREPORT)
	{
		//动作或告警
		m_tmStart = StringToTimeSttp15(str, m_nmsStart);
	}
	else
	{
		//录波简报或录波列表
		m_tmStart = StringToTimeSttp26(str, m_nmsStart);
	}
	str = pMemset->GetValue(5);//接收时间
	if(m_nType < XJ_FAULT_EVENT_OSCREPORT)
	{
		//动作或告警
		m_tmReceiveTime = StringToTimeSttp15(str, m_nmsReceive);
	}
	else
	{
		//录波简报或录波列表
		m_tmReceiveTime = StringToTimeSttp26(str, m_nmsReceive);
	}
	m_sEventDef = pMemset->GetValue(6);//事件描述
	str = pMemset->GetValue(7); //事件值
	m_sEventContent = ComplexExchange(str); //可能有是复数的情况
	str = pMemset->GetValue(8); //信号时间
	if(m_nType == XJ_FAULT_EVENT_SING)
	{
		//动作信号
		m_tmSign = StringToTimeSttp15(str, m_nmsSign);
	}

	//关联设备
	if(!BuildReleation())
	{
		WriteLog("CFaultEvent::LoadFromDB, 关联到设备失败");
		return FALSE;
	}
	//查找定义名称
	if(!FindEventDefName())
	{
		CString str;
		str.Format("CFaultEvent::LoadFromDB, 事件无相关信息, 事件类型为%d", m_nType);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildReleation()
 功能概要：创建与设备的关联
 返 回 值: 能关联到设备返回TRUE, 不能关联到设备返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FC
BOOL CFaultEvent::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	//判断数据有效性
	if(m_sSecID == "")
		return FALSE;
	//查找设备;
	m_pSec = (CSecObj*)m_pDataEngine->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL) //查找不到设备,失败
		return FALSE;
	return TRUE;
}

/*************************************************************
 函 数 名：FindEventDefName()
 功能概要：查找事件定义的名称
 返 回 值: 查找成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FD
BOOL CFaultEvent::FindEventDefName()
{
	//判断数据有效性
	if(m_pSec == NULL)
		return FALSE;
	if(m_sEventDef == "")
		return FALSE;
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	//刷新保护配置
	m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
	int nEventDef = atoi(m_sEventDef);
	if(m_nType == XJ_FAULT_EVENT_CHR)
	{
		//动作特征量, 先在保护中找
		PT_ActionCharacter* ptChr = GetActChrsConfig(nEventDef);
		if(ptChr == NULL)
		{
			//找不到, 在通用中找
			ptChr = GetActChrsFromUniversal(nEventDef);
		}
		if(ptChr == NULL)
			return FALSE;
		m_sEventDefName = ptChr->strName;
		m_strUnit = ptChr->strUnits;
	}
	if(m_nType == XJ_FAULT_EVENT_SING)
	{
		PT_Signal * pts = GetSignalConfig(nEventDef);
		if(pts == NULL)
			return FALSE;
		m_sEventDefName = pts->strName;
	}
	if(m_nType == XJ_FAULT_EVENT_ALARM)
	{
		PT_Signal * pts = GetAlarmConfig(nEventDef);
		if(pts == NULL)
			return FALSE;
		m_sEventDefName = pts->strName;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：GetSignalConfig()
 功能概要：根据指定ID得到指定的信号量
 返 回 值: 查找到指定的信号量则返回信号量对象, 未查找到返回NULL
 参    数：param1   信号量ID
**************************************************************/
//##ModelId=49B87B9A0209
PT_Signal * CFaultEvent::GetSignalConfig(int nID)
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
//##ModelId=49B87B9A0205
PT_ActionCharacter * CFaultEvent::GetActChrsConfig(int nID)
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
//##ModelId=49B87B9A0207
PT_ActionCharacter * CFaultEvent::GetActChrsFromUniversal(int nID)
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
 函 数 名：MultiSaveData()
 功能概要：组织批量插入数据
 返 回 值: 组织成功返回TRUE, 失败返回FALSE
 参    数：param1	数据记录
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FE
BOOL CFaultEvent::MultiSaveData( STTP_DBDATA_RECORD& sttpDB )
{
	try
	{
		//MainRecID
		STTP_DBDATA_UNIT field;
		CString str;
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nMainRecID);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//保护ID
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sSecID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventType
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nType);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//FaultStartTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		if(m_nType < 4)
		{
			str.Format("%s%03d", m_tmStart.Format("%y%m%d%H%M%S"), m_nmsStart);
		}
		else if(m_nType >= 4)
		{
			str.Format("%s.%03d", m_tmStart.Format("%Y-%m-%d %H:%M:%S"), m_nmsStart);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//FaultReceiveTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		if(m_nType < 4)
		{
			str.Format("%s%03d", m_tmReceiveTime.Format("%y%m%d%H%M%S"), m_nmsReceive);
		}
		else if(m_nType >= 4)
		{
			str.Format("%s.%03d", m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), m_nmsReceive);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventDef
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sEventDef;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventContent
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sEventContent;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//SignalTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		if(m_nType < 4)
		{
			str.Format("%s%03d", m_tmSign.Format("%y%m%d%H%M%S"), m_nmsSign);
		}
		else if(m_nType >= 4)
		{
			str = "";
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//SignalTime
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str = "0";
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
	}
	catch (...)
	{
		WriteLog("CFaultEvent::MultiSaveData, 出错", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：Clone()
 功能概要：复制故障事件
 返 回 值: 复制生成的故障事件
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0204
CFaultEvent* CFaultEvent::Clone()
{
	CFaultEvent* pEvent = new CFaultEvent;
	pEvent->m_pDataEngine	= m_pDataEngine;
	pEvent->m_nID			= m_nID;
	pEvent->m_nMainRecID	= m_nMainRecID;
	pEvent->m_nmsReceive	= m_nmsReceive;
	pEvent->m_nmsSign		= m_nmsSign;
	pEvent->m_nmsStart		= m_nmsStart;
	pEvent->m_nType			= m_nType;
	pEvent->m_pSec			= m_pSec;
	pEvent->m_sEventContent	= m_sEventContent;
	pEvent->m_sEventDef		= m_sEventDef;
	pEvent->m_sEventDefName	= m_sEventDefName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_tmReceiveTime	= m_tmReceiveTime;
	pEvent->m_tmSign		= m_tmSign;
	pEvent->m_tmStart		= m_tmStart;
	pEvent->m_strUnit		= m_strUnit;
	pEvent->m_nCPU			= m_nCPU;
	pEvent->m_nNum			= m_nNum;
	pEvent->m_sDir          = m_sDir;
	pEvent->m_sOldDir       = m_sOldDir;
	return pEvent;
}

/*************************************************************
 函 数 名：GetAlarmConfig()
 功能概要：得到指定的告警告信号量
 返 回 值: 信号量对象
 参    数：param1	信号量ID
		   Param2
**************************************************************/
//##ModelId=49B87B9A020B
PT_Signal * CFaultEvent::GetAlarmConfig( int nID )
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
 函 数 名: GetFaultType()
 功能概要: 将故障类型解释成文字
 返 回 值: 解释后的文字
 参    数: param1 故障类型
		   Param2 
**************************************************************/
CString CFaultEvent::GetFaultType( CString sType )
{
	CString strTemp;
	sType.MakeUpper();
	if ((sType == "AG") || (sType == "AN") || (sType == "A"))
	{
		strTemp = "A相接地";
	}
	else if ((sType == "BG") || (sType == "BN") || (sType == "B"))
	{
		strTemp = "B相接地";
	}
	else if ((sType == "CG") || (sType == "CN") || (sType == "C"))
	{
		strTemp = "C相接地";
	}
	else if ((sType == "AB") || (sType == "BA"))
	{
		strTemp = "AB两相短路";
	}
	else if ((sType == "BC") || (sType == "CB"))
	{
		strTemp = "BC两相短路";
	}
	else if ((sType == "CA") || (sType == "AC"))
	{
		strTemp = "CA两相短路";
	}
	else if ((sType == "ABG") || (sType == "ABN") || (sType == "BAG") || (sType == "BAN"))
	{
		strTemp = "AB两相短路接地";
	}
	else if ((sType == "BCG") || (sType == "BCN") || (sType == "CBG") || (sType == "CBN"))
	{
		strTemp = "BC两相短路接地";
	}
	else if ((sType == "CAG") || (sType == "CAN") || (sType == "ACG") || (sType == "ACN"))
	{
		strTemp = "CA两相短路接地";
	}
	else if (sType == "ABC")
	{
		strTemp = "ABC三相短路";
	}
	else if ((sType == "ABCG") || (sType == "ABCN"))
	{
		strTemp = "ABC三相接地";
	}	
	else
	{
		strTemp = sType;
	}
	
	return strTemp;
}

/*************************************************************
 函 数 名: IsReturnEvent()
 功能概要: 是否返回信息
 返 回 值: 是返回信息返回TRUE,不是返回信息返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CFaultEvent::IsReturnEvent()
{
	int nValue = atoi(m_sEventContent);
	if(nValue == 0)
		return TRUE;
	return FALSE;
}

/****************************************************
Date:2011/10/14  Author:LYH
函数名:   GetEventProperty	
返回值:   int	事件等级,默认返回0，0-I类，1-II类，2-III类
功能概要: 取得事件等级
*****************************************************/
int CFaultEvent::GetEventProperty()
{
	//判断数据有效性
	if(m_pSec == NULL)
		return 0;
	if(m_sEventDef == "")
		return 0;
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	int nProperty = 0;
	//刷新保护配置
	m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
	int nEventDef = atoi(m_sEventDef);
	if(m_nType == XJ_FAULT_EVENT_CHR)
	{
		//动作特征量, 先在保护中找
		PT_ActionCharacter* ptChr = GetActChrsConfig(nEventDef);
		if(ptChr == NULL)
		{
			//找不到, 在通用中找
			ptChr = GetActChrsFromUniversal(nEventDef);
		}
		if(ptChr == NULL)
			return 0;
		nProperty = ptChr->nEventType;
	}
	if(m_nType == XJ_FAULT_EVENT_SING)
	{
		PT_Signal * pts = GetSignalConfig(nEventDef);
		if(pts == NULL)
			return 0;
		nProperty = pts->nEventType;
	}
	if(m_nType == XJ_FAULT_EVENT_ALARM)
	{
		PT_Signal * pts = GetAlarmConfig(nEventDef);
		if(pts == NULL)
			return 0;
		nProperty = pts->nEventType;
	}
	return nProperty;
}