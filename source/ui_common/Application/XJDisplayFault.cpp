#include "XJDisplayFault.h"
#include "FaultEvent.h"

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   CXJDisplayFault	
返回值:   	
功能概要: 
参数: CFaultReport * pReport	
*****************************************************/
CXJDisplayFault::CXJDisplayFault( CFaultReport* pReport )
{
	m_pReport = pReport;
	m_nFaultActionTime = 1;
	m_bShowReturn = TRUE;
	m_sMultiPtName = "";
	m_sSinglePtName = "";
	m_sCurrentPtName = "";
	m_sPath = "";
	m_bFilterDistance = FALSE;
	m_bFilterIII = FALSE;
	m_bFilterNULL = FALSE;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   ~CXJDisplayFault	
返回值:   	
功能概要: 
*****************************************************/
CXJDisplayFault::~CXJDisplayFault()
{

}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   TranslateSingleKey	
返回值:   CString	关键字代表的值
功能概要: 解释单个关键字代表的意义
参数: CString sKey	关键字内容
*****************************************************/
CString CXJDisplayFault::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	MYASSERT_STRING(m_pReport);
	CString sReturn = "";
	int nFind = -1;
	//判断是否有.GetCount()方法
	nFind = sKey.Find(".GetCount()", 0);
	if(nFind > -1)
		return GetValue_getcount(sKey);
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//没有[, 为普通关键字
		//普通关键字
		if(sKey == "$DEVICE_NAME$") //二次设备
		{
			if(m_pReport->m_listSec.GetSize() == 1)
			{
				CSecObj* pObj = (CSecObj*)m_pReport->m_listSec.GetAt(0);
				if(pObj != NULL)
					return pObj->m_sName;
			}
			return "";
		}
		if(sKey == "$FAULT_NAME$")
			return m_pReport->m_sName;
		if(sKey == "$FAULT_DISTANCE$") //故障测距
			return m_pReport->m_sFaultDistance;
		if(sKey == "$FAULT_TYPE$") //故障类型
			return m_pReport->GetFaultTypeString(m_pReport->m_nFaultType);
		if(sKey == "$FAULT_TIME$") //故障时间
			return m_pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		if(sKey == "$FAULT_TIME_MS$")//故障时间毫秒值
		{
			sReturn.Format("%03d", m_pReport->m_nmsFaultTime);
			return sReturn;
		}
		if(sKey == "$FAULT_STATION1$") //故障厂站1
		{
			if(m_pReport->m_pStation1 != NULL)
				return m_pReport->m_pStation1->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_STATION2$") //故障厂站2
		{
			if(m_pReport->m_pStation2 != NULL)
				return m_pReport->m_pStation2->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_DEVICE$") //一次设备
		{
			if(m_pReport->m_pPriDevice != NULL)
				return m_pReport->m_pPriDevice->m_sName;
			else
				return "";
		}
	}
	else
	{
		int nFind2 = sKey.Find('[', nFind+1);
		if(nFind2 == -1)
		{
			//只有一个[
			return GetValue_OneLayer(sKey);
		}
		else
		{
			nFind = sKey.Find('[', nFind2 + 1);
			if(nFind == -1)
			{
				//只有两个
				return GetValue_TwoLayer(sKey);
			}
		}
	}
	return "";
}

/*************************************************************
 函 数 名：GetValue_getcount()
 功能概要：得到关键字getcount的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8401F7
CString CXJDisplayFault::GetValue_getcount( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找是否有[
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//由单个关键字组成
		if(sKey == "$DEVICE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listSec.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ACTION_CHR_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listChr.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ACTION_SIGN_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listSign.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ALARM_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listAlarm.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_PT_OSC_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listPTOSC.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_WR_OSC_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWROSC.GetSize());
			return sReturn;
		}
	}
	else
	{
		//得到头
		CString strHead = sKey.Left(nFind);
		if(strHead == "$DEVICE_LIST")
		{
			//得到索引
			int nFind1 = sKey.Find(']', nFind);
			if(nFind1 == -1)
				return "";
			CString sIndex = sKey.Mid(nFind+1, nFind1 - nFind -1);
			int nIndex = atoi(sIndex);
			//得到索引对应的设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return "";
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return "";
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return "";
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			//得到后段的关键字 // $DEVICE_LIST[1]::FAULT_PT_OSC_LIST.GetCount()$
			nFind = sKey.ReverseFind(':');
			if(nFind == -1)
				return "";
			CString strTail = sKey.Right(sKey.GetLength() - nFind - 1);
			if(strTail == "FAULT_ACTION_CHR_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempChr.GetSize());
				return sReturn;
			}
			if(strTail == "FAULT_ACTION_SIGN_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempSign.GetSize());
				return sReturn;
			}
			if(strTail == "FAULT_PT_OSC_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempPTOSC.GetSize());
				return sReturn;
			}
		}
	}
	return sReturn;
}

/*************************************************************
 函 数 名：GetValue_OneLayer()
 功能概要：得到一层深度关键字的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B840204
CString CXJDisplayFault::GetValue_OneLayer( CString sKey )
{
	//$DEVICE_LIST[11]::DEVICE_NAME$
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
	if(strHead == "$DEVICE_LIST")
	{
		//设备列表, 取得设备
		if(m_pReport->m_listSec.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
			return "";
		CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
		if(pSec == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pSec->m_sName;
		}
		if(strTail == "DEVICE_STATION$")
		{
			if(pSec->m_pStation != NULL)
				return pSec->m_pStation->m_sName;
			return "";
		}
		if(strTail == "DEVICE_OWNER$")
		{
			if(pSec->m_pOwner != NULL)
				return pSec->m_pOwner->m_sName;
			return "";
		}
		if(strTail == "EVENT_TIME1_FULL$")
		{
			//得到第一个动作信息的时间
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			if(m_pReport->m_listTempSign.GetSize() < 1)
				return "";
			CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listTempSign.GetAt(0);
			if(pEvent == NULL)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
	}
	else
	{
		CFaultEvent* pEvent = NULL;
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//动作特征值
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return "";
			}

			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//动作信号
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_LIST")
		{
			//告警
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//保护录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//录波器录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(pEvent == NULL)
			return "";
		if(strTail == "EVENT_TIME1_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME1$")
		{
			sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:时%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME1_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2$")
		{
			sReturn = pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_SEC_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_sName;
		}
		if(strTail == "EVENT_STATION_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_pStation->m_sName;
		}
		if(strTail == "EVENT_DEF$")
		{
			return pEvent->m_sEventDefName;
		}
		if(strTail == "EVENT_CONTENT$")
		{
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//特征量
				int nEventDef = atoi(pEvent->m_sEventDef);
				if(nEventDef == 2)
				{
					//故障类型
					CString strFaultType = pEvent->GetFaultType(pEvent->m_sEventContent);
					return strFaultType;
				}
				CString str = pEvent->m_sEventContent;
				if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
					str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
				return str;
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
			{
				//动作信号或告警
				if(pEvent->m_sEventContent == "1")
				{
					return "动作";
				}
				else if(pEvent->m_sEventContent == "0")
				{
					return "返回";
				}
			}
			return pEvent->m_sEventContent;
		}
		if(strTail == "OSC_FILE_NAME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			CString str = pEvent->m_sEventDef;
			//得到文件名
			str.Replace("/", "\\");
			int nRFind = str.ReverseFind('\\');
			if(nRFind >= 0)
				str.Delete(0, nRFind+1);
			return str;
		}
		if(strTail == "OSC_FILE_TIME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_RELETIVE_TIME$")
		{
			//判断是否事件
			if(pEvent->m_nType != XJ_FAULT_EVENT_CHR && pEvent->m_nType != XJ_FAULT_EVENT_SING && pEvent->m_nType != XJ_FAULT_EVENT_ALARM)
			{
				return "";
			}
	
			int nOrgMs = 0;
			CTime tmStartTime;
			if(m_nFaultActionTime == 0)
			{
				nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
				tmStartTime = m_pReport->m_tmFaultTime;
			}
			else if(m_nFaultActionTime == 1)
			{
				if((pEvent->m_tmStart == pEvent->m_tmSign) &&(pEvent->m_nmsStart == pEvent->m_nmsSign))
				{
					nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
					tmStartTime = m_pReport->m_tmFaultTime;
				}
				else
				{
					nOrgMs = pEvent->m_nmsStart;
					tmStartTime = pEvent->m_tmStart;
				}
			}
			//int nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
			CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;
			LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
			//转为毫秒值
			LONG nMs = nSEC* 1000;
			LONG nCurMs = pEvent->m_nmsSign + nMs;
			//计算差值
			LONG nDis = nCurMs - nOrgMs;
			sReturn.Format("%d 毫秒", nDis);
			return sReturn;
		}
	}
	return "";
}

/*************************************************************
 函 数 名：GetValue_TwoLayer()
 功能概要：得到两层深度关键字的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B840206
CString CXJDisplayFault::GetValue_TwoLayer( CString sKey )
{
	CString strLog;
	strLog.Format("CXJDisplayFault::GetValue_TwoLayer, key is %s", sKey);
	WriteLog(strLog);
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	if(strHead != "$DEVICE_LIST")
		return "";
	//设备列表, 取得设备
	if(m_pReport->m_listSec.GetSize() < 1)
		return "";
	if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
		return "";
	CSecObj* pSec = NULL;
	pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
	if(pSec == NULL)
		return "";
	//建立零时事件列表
	if(pSec != m_pReport->m_pTempSec)
	{
		//组建零时数组
		m_pReport->BuildTempList(pSec);
	}
	//取得中间的关键字
	int nFind3 = sKey.Find('[', nFind2);
	if(nFind3 == -1)
		return "";
	int nFind4 = sKey.Find(']', nFind3);
	if(nFind4 == -1)
		return "";
	//得到索引
	strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
	nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);

	//取得尾部
	CString strTail = sKey.Right(sKey.GetLength() - (nFind4 + 3));
	if(strTail == "DEVICE_NAME$")
	{
		return pSec->m_sName;
	}
	if(strTail == "DEVICE_STATION$")
	{
		if(pSec->m_pStation != NULL)
			return pSec->m_pStation->m_sName;
		else
			return "";
	}

	//得到事件
	CFaultEvent* pEvent = NULL;
	CFaultEvent* pEventFirst = NULL; //第一条事件,用来计算相对时间
	if(strMid == "FAULT_ACTION_CHR_LIST")
	{
		//动作特征值
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempChr[0];
	}
	if(strMid == "FAULT_ACTION_SIGN_LIST")
	{
		//动作信号
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempSign.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempSign[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempSign[0];
	}
	if(strMid == "FAULT_PT_OSC_LIST")
	{
		//保护录波
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempPTOSC.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSC[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempPTOSC[0];
	}
	if(pEvent == NULL)
		return "";
	
	if(strTail == "DEVICE_STATION$")
	{
		if(pSec->m_pStation != NULL)
			return pSec->m_pStation->m_sName;
		else
			return "";
	}
	if(strTail == "EVENT_TIME1_FULL$")
	{
		sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME1$")
	{
		sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:时%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME1_MS$")
	{
		sReturn.Format("%d", pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_FULL$")
	{
		sReturn.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2$")
	{
		sReturn = pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_MS$")
	{
		sReturn.Format("%d", pEvent->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_SEC_NAME$")
	{
		if(pEvent->m_pSec != NULL)
			return pEvent->m_pSec->m_sName;
	}
	if(strTail == "EVENT_STATION_NAME$")
	{
		if(pEvent->m_pSec != NULL)
			return pEvent->m_pSec->m_pStation->m_sName;
	}
	if(strTail == "EVENT_DEF$")
	{
		return pEvent->m_sEventDefName;
	}
	if(strTail == "EVENT_CONTENT$")
	{
		if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征量
			int nEventDef = atoi(pEvent->m_sEventDef);
			if(nEventDef == 2)
			{
				//故障类型
				CString strFaultType = pEvent->GetFaultType(pEvent->m_sEventContent);
				return strFaultType;
			}
			CString str = pEvent->m_sEventContent;
			if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
				str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
			return str;
		}
		if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//动作信号或告警
			if(pEvent->m_sEventContent == "1")
			{
				return "动作";
			}
			else if(pEvent->m_sEventContent == "0")
			{
				return "返回";
			}
		}
		return pEvent->m_sEventContent;
	}
	if(strTail == "OSC_FILE_NAME$")
	{
		//录波简报才有
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		//得到文件名
		CString str = pEvent->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
			str.Delete(0, nRFind+1);
		return str;
	}
	if(strTail == "OSC_FILE_TIME$")
	{
		//录波简报才有
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_RELETIVE_TIME$")
	{
		//判断是否事件
		if(pEvent->m_nType != XJ_FAULT_EVENT_CHR && pEvent->m_nType != XJ_FAULT_EVENT_SING && pEvent->m_nType != XJ_FAULT_EVENT_ALARM)
		{
			return "";
		}
		if(pEventFirst == NULL)
			return "";

		int nOrgMs = 0;
		CTime tmStartTime;
		if(m_nFaultActionTime == 0)
		{
			nOrgMs = pEventFirst->m_nmsSign; //原始毫秒值
			tmStartTime = pEventFirst->m_tmSign;
		}
		else if(m_nFaultActionTime == 1)
		{
			if((pEvent->m_tmStart == pEvent->m_tmSign) &&(pEvent->m_nmsStart == pEvent->m_nmsSign))
			{
				nOrgMs = pEventFirst->m_nmsStart; //原始毫秒值
				tmStartTime = pEventFirst->m_tmStart;
			}
			else
			{
				nOrgMs = pEvent->m_nmsStart;
				tmStartTime = pEvent->m_tmStart;
			}
		}
		CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;
		
		LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
		//转为毫秒值
		LONG nMs = nSEC* 1000;
		LONG nCurMs = pEvent->m_nmsSign + nMs;
		//计算差值
		LONG nDis = nCurMs - nOrgMs;
		sReturn.Format("%d 毫秒", nDis);
		return sReturn;
	}
	
	return "";
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   InitDisplay	
返回值:   void	
功能概要: 初始化显示
*****************************************************/
CSize CXJDisplayFault::InitDisplay()
{
// 	if(m_pReport == NULL)
// 		return CSize(0,0);
	if(pReportDoc == NULL)
		return CSize(0,0);

	//载入详细信息
	if(m_pReport != NULL)
	{
		if(m_pReport->m_bLoadDetail == FALSE)
		{
			m_pReport->m_bShowReturn = m_bShowReturn;
			m_pReport->m_bFilterDistance = m_bFilterDistance;
			m_pReport->m_bFilterIII = m_bFilterIII;
			m_pReport->m_bFilterNULL = m_bFilterNULL;
			m_pReport->LoadDetail();
		}
		else
		{
			if(m_pReport->m_bShowReturn != m_bShowReturn ||
				m_pReport->m_bFilterDistance != m_bFilterDistance ||
				m_pReport->m_bFilterIII != m_bFilterIII ||
				m_pReport->m_bFilterNULL != m_bFilterNULL)
			{
				m_pReport->m_bShowReturn = m_bShowReturn;
				m_pReport->m_bFilterDistance = m_bFilterDistance;
				m_pReport->m_bFilterIII = m_bFilterIII;
				m_pReport->m_bFilterNULL = m_bFilterNULL;
				m_pReport->LoadDetail(TRUE);
			}
		}
	}

	//判断应该显示什么模板
	CString sFile = m_sSinglePtName;
	if(m_pReport != NULL && m_pReport->m_listSec.GetSize() > 1)
	{
		sFile = m_sMultiPtName;
	}

	CString sDir = m_sPath + sFile;

	//打开模板文件
	if(sFile != m_sCurrentPtName)
	{
		pReportDoc->OpenReportFileEx(sDir, this);
		m_sCurrentPtName = sFile;
	}

	CSize szDoc(0, 0);
	szDoc = pReportDoc->FillValueData();
	return szDoc;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   SetTemplateFilePath	
返回值:   void	
功能概要: 
参数: CString sPath	
参数: CString sSingle	
参数: CString sMulti	
*****************************************************/
void CXJDisplayFault::SetTemplateFilePath( CString sPath, CString sSingle, CString sMulti )
{
	m_sPath = sPath;
	m_sSinglePtName = sSingle;
	m_sMultiPtName = sMulti;
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   SetFaultReport	
返回值:   void	
功能概要: 
参数: CFaultReport * pReport	
*****************************************************/
void CXJDisplayFault::SetFaultReport( CFaultReport* pReport )
{
	m_pReport = pReport;
}

/****************************************************
Date:2011/11/22  Author:LYH
函数名:   HitTestOscEvent	
返回值:   int	
功能概要: 
参数: CPoint point	
参数: CString & strPath	
*****************************************************/
int CXJDisplayFault::HitTestOscEvent( CPoint point, CString& strPath )
{
	MYASSERT_INT(pReportDoc);

	int nHit = pReportDoc->GetHitElement(point);
	if(nHit < 0)
		return -1;

	CString strValueCode = "";
	strValueCode = pReportDoc->GetValueCode(nHit);
	if(strValueCode == "")
		return -1;
	//得到对应的事件
	CFaultEvent* pEvent = GetEventByKey(strValueCode);
	if(pEvent != NULL)
	{
		//是录波简报时才操作
		if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
		{
			CString sPath = pEvent->m_sEventDef;

			//判断是否有后缀名
			if(sPath.Find(".cfg") == -1 && sPath.Find(".dat") == -1 && sPath.Find(".CFG") == -1 && sPath.Find(".DAT") == -1)
			{
				//加上CFG后缀
				sPath += ".cfg";
			}
			strPath = sPath;
		}
		else
			return -1;
	}
	else 
		return -1;
	return nHit;
}

/*************************************************************
 函 数 名：GetEventByKey()
 功能概要：由关键字得到对应的故障事件, 是对应$OSC_FILE_NAME$的才返回指针, 其它都为NULL
 返 回 值: 故障事件指针
 参    数：param1 关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8401E4
CFaultEvent* CXJDisplayFault::GetEventByKey( CString sKey )
{
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return NULL;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return NULL;
	//解释每个关键字, 查找事件, 记录尾部字符
	CStringArray arrTail;
	arrTail.RemoveAll();
	CFaultEvent* pReturnEvent = NULL;
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sTail;
		CFaultEvent* pEvent = GetEventBySignleKey(arrKey.GetAt(i), sTail);
		if(pEvent != NULL)
		{
			pReturnEvent = pEvent;
			if(sTail == "$OSC_FILE_NAME$")
				arrTail.Add(sTail);
		}
	}
	//查找完毕
	if(pReturnEvent == NULL)
		return NULL;
	if(arrTail.GetSize() < 1)
	{
		//没有尾部关键字
		return NULL;
	}
	return pReturnEvent;
}

/*************************************************************
 函 数 名：GetEventBySignleKey()
 功能概要：由单个关键字找到对应的故障事件
 返 回 值: 故障事件指针
 参    数：param1	关键字
		   Param2	尾部关键字
**************************************************************/
//##ModelId=49B87B8401E6
CFaultEvent* CXJDisplayFault::GetEventBySignleKey( CString sKey, CString & sTail)
{
	//判断数据有效性
	if(sKey == "")
		return NULL;
	if(m_pReport == NULL)
		return NULL;
	sTail = "";
	CFaultEvent* pEvent = NULL;
	int nFind = -1;
	//判断是否带有[ ]运算符
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//带有[]运算符
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//没找到结束括号
			return NULL;
		}
		int nFind3 = sKey.Find('[', nFind2);
		if(nFind3 != -1)
		{
			//有两层
			int nFind4 = sKey.Find(']', nFind3);
			if(nFind4 == -1)
				return NULL;
			//得到索引
			CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
			int nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			//得到头
			CString strHead = sKey.Left(nFind);
			if(strHead != "$DEVICE_LIST")
				return NULL;
			//设备列表, 取得设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return NULL;
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return NULL;
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return NULL;
			//建立零时事件列表
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			
			//得到索引
			strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
			nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);
			
			//得到事件
			CFaultEvent* pEvent = NULL;
			if(strMid == "FAULT_ACTION_CHR_LIST")
			{
				//动作特征值
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
			}
			if(strMid == "FAULT_ACTION_SIGN_LIST")
			{
				//动作信号
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempSign.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempSign[nIndex];
			}
			if(strMid == "FAULT_PT_OSC_LIST")
			{
				//保护录波
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempPTOSC.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSC[nIndex];
			}
			//取得尾部
			CString str = sKey.Right(sKey.GetLength() - (nFind4 + 3));
			sTail = "$";
			sTail += str;
			return pEvent;
		}
		//取得索引号
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		//取得头部
		CString strHead = sKey.Left(nFind);
		//取得事件
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//动作特征值
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//动作信号
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_ LIST")
		{
			//告警
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//保护录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//录波器录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(pEvent == NULL)
			return NULL;
		//取得尾部字符
		sTail = "$";
		sTail += sKey.Right(sKey.GetLength() - (nFind2 + 3));
	}
	return pEvent;
}

/****************************************************
Date:2011/11/22  Author:LYH
函数名:   GetOscFullPath	
返回值:   CString	
功能概要: 
参数: CSecObj * pSec	
参数: CString strFileName	
*****************************************************/
CString CXJDisplayFault::GetOscFullPath( CSecObj* pSec, CString strFileName )
{
	return "";
}
