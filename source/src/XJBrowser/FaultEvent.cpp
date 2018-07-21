// FaultEvent.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
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
	
	m_sReceiveTime = "";
	m_sSignaltime = "";
	m_nWaveFileNo = 0;
	m_sWaveDescribe = "";
	m_sReverse2 = "";
	m_sReverse3 = "";
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

	if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
	{
		//录波简报或录波列表
		m_tmStart = StringToTimeSttp26(str, m_nmsStart);
	}
	else
	{
		m_tmStart = StringToTimeSttp15(str, m_nmsStart);
	}

	str = pMemset->GetValue(5);//接收时间
	
	if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
	{
		//录波简报或录波列表
		m_tmReceiveTime = StringToTimeSttp26(str, m_nmsReceive);
	}
	else if(m_nType == XJ_FAULT_EVENT_OSCPARSE)
	{
		m_sReceiveTime = str;
	}
	else
	{
		m_tmReceiveTime = StringToTimeSttp15(str, m_nmsReceive);
	}
	m_sEventDef = pMemset->GetValue(6);//事件描述
	str = pMemset->GetValue(7); //事件值
	m_sEventContent = ComplexExchange(str); //可能有是复数的情况
	str = pMemset->GetValue(8); //信号时间
	if(m_nType == XJ_FAULT_EVENT_SING||m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		m_tmSign = StringToTimeSttp15(str, m_nmsSign);
	}
	else
		m_sSignaltime = str;

	m_nWaveFileNo = atoi(pMemset->GetValue(9));//wavefileno
	m_sWaveDescribe = pMemset->GetValue(10);
	m_sReverse2 = pMemset->GetValue(13);
	m_sReverse3 = pMemset->GetValue(14);

	//关联设备
	if(!BuildReleation() && (m_nType != XJ_FAULT_EVENT_WAVEDISTANCE && m_nType != XJ_FAULT_EVENT_DISTANCE))
	{
		WriteLog("CFaultEvent::LoadFromDB, 关联到设备失败");
		return FALSE;
	}
	if(m_pSec != NULL && m_nType == XJ_FAULT_EVENT_OSCREPORT)
	{
		m_sEventDef.Replace("/", "\\");
		DelPostfixFromPath(m_sEventDef);
		if(m_sEventDef.Find("\\", 0) == -1)
		{
			if(m_pSec->m_nType == TYPE_PROTECT)
				m_sEventDef = m_pSec->m_sStationID + "\\保护\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\"+m_sEventDef;
			else if(m_pSec->m_nType == TYPE_WAVEREC)
				m_sEventDef = m_pSec->m_sStationID + "\\录波器\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\"+m_sEventDef;
		}
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
	//判断数据有效性
	if(m_sSecID == "")
		return FALSE;
	//查找设备
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	m_pSec = (CSecObj*)pData->FindDevice(m_sSecID, TYPE_SEC);
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
	if(m_nType == XJ_FAULT_EVENT_WAVEDISTANCE || m_nType == XJ_FAULT_EVENT_OSCPARSE || m_nType == XJ_FAULT_EVENT_DISTANCE)
		return TRUE;

	if(m_pSec == NULL)
		return FALSE;
	if(m_sEventDef == "")
		return FALSE;
	
	if(m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		m_sEventDefName = QueryDIName(m_sSecID, atoi(m_sEventDef), m_nWaveFileNo);
		return TRUE;
	}
	//刷新保护配置
	m_pSec->RefreshConfig();
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
 函 数 名：GetActChrsFromUniversal()
 功能概要：从通用特征量中查找特征量定义
 返 回 值: 查找到则返回特征量指针, 未找到返回NULL
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
//##ModelId=49B87B9A0207
PT_ActionCharacter * CFaultEvent::GetActChrsFromUniversal(int nID)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp ->GetDataEngine();
	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < pData->m_arrUniversalActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) pData->m_arrUniversalActChr[i];
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
		
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			str.Format("%s.%03d", m_tmStart.Format("%Y-%m-%d %H:%M:%S"), m_nmsStart);
		}
		else
		{
			str.Format("%s%03d", m_tmStart.Format("%y%m%d%H%M%S"), m_nmsStart);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//FaultReceiveTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			str.Format("%s.%03d", m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), m_nmsReceive);
		}
		else if(m_nType == XJ_FAULT_EVENT_OSCPARSE)
		{
			str = m_sReceiveTime;
		}
		else
		{
			str.Format("%s%03d", m_tmReceiveTime.Format("%y%m%d%H%M%S"), m_nmsReceive);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventDef
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sEventDef;
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			//修改路径斜杠
			str.Replace("\\", "/");
		}
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
		if(m_nType == XJ_FAULT_EVENT_SING||m_nType == XJ_FAULT_EVENT_OSCPARSE||m_nType == XJ_FAULT_EVENT_DICHANGE)
		{
			str.Format("%s%03d", m_tmSign.Format("%y%m%d%H%M%S"), m_nmsSign);
		}
		else
		{
			str = m_sSignaltime;
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//wavefileno
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nWaveFileNo);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//wavedescribe
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sWaveDescribe;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//reverse2
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sReverse2;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//reverse3
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sReverse3;
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
	pEvent->m_sReceiveTime	= m_sReceiveTime;
	pEvent->m_sSignaltime	= m_sSignaltime;
	pEvent->m_nWaveFileNo	= m_nWaveFileNo;
	pEvent->m_sWaveDescribe	= m_sWaveDescribe;
	pEvent->m_sReverse2		= m_sReverse2;
	pEvent->m_sReverse3		= m_sReverse3;
	return pEvent;
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
		strTemp = StringFromID(IDS_FAULTTYPE_A);
	}
	else if ((sType == "BG") || (sType == "BN") || (sType == "B"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_B);
	}
	else if ((sType == "CG") || (sType == "CN") || (sType == "C"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_C);
	}
	else if ((sType == "AB") || (sType == "BA"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_AB);
	}
	else if ((sType == "BC") || (sType == "CB"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_BC);
	}
	else if ((sType == "CA") || (sType == "AC"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_CA);
	}
	else if ((sType == "ABG") || (sType == "ABN") || (sType == "BAG") || (sType == "BAN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABN);
	}
	else if ((sType == "BCG") || (sType == "BCN") || (sType == "CBG") || (sType == "CBN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_BCN);
	}
	else if ((sType == "CAG") || (sType == "CAN") || (sType == "ACG") || (sType == "ACN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_CAN);
	}
	else if (sType == "ABC")
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABC);
	}
	else if ((sType == "ABCG") || (sType == "ABCN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABCN);
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
	int nProperty = 0;
	//刷新保护配置
	m_pSec->RefreshConfig();
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