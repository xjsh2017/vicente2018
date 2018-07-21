#include "XJDisplayDecision.h"

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   CXJDisplayDecision	
返回值:   	
功能概要: 
参数: CDecisionReport * pReport	
*****************************************************/
CXJDisplayDecision::CXJDisplayDecision( CDecisionReport* pReport )
{
	m_pReport = pReport;
	m_bLoadTemplateFile = FALSE;
	m_nFaultActionTime = 0;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   ~CXJDisplayDecision	
返回值:   	
功能概要: 
*****************************************************/
CXJDisplayDecision::~CXJDisplayDecision()
{

}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   TranslateSingleKey	
返回值:   CString	关键字代表的值
功能概要: 解释单个关键字代表的意义
参数: CString sKey	关键字内容
*****************************************************/
CString CXJDisplayDecision::TranslateSingleKey( CString sKey )
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
		if(sKey == "$FAULT_DEVICE$")//故障元件
			return m_pReport->m_sFaultDevice;
		if(sKey == "$FAULT_TYPE$")//故障相位
			return m_pReport->m_sFaultPhase;
		if(sKey == "$FAULT_DISTANCE$")//故障测距
		{
			int nDistance = atoi(m_pReport->m_sFaultDistance);
			if(nDistance == 0)
				sReturn = "子站未上送";
			else
				sReturn.Format("%s km", m_pReport->m_sFaultDistance);
			return sReturn;
		}
		if(sKey == "$DECISION_VOLTAGE$") //电压等级
		{
			sReturn.Format("%dkV", m_pReport->m_nVoltage);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTTYPE$")//故障类型
			return m_pReport->m_sFaultType;
		if(sKey == "$DECISION_FAULTREASON$") //故障原因
			return m_pReport->m_sFaultReason;
		if(sKey == "$DECISION_FIRSTTIME$") //初次故障持续时间
		{
			sReturn.Format("%d 秒", m_pReport->m_nFirstTime);
			return sReturn;
		}
		if(sKey == "$DECISION_SECONDTIME$") //故障时间
		{
			sReturn.Format("%d 秒", m_pReport->m_nSecondTime);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTCURRENT$")//故障电流
		{
			if(m_pReport->m_fFaultCurrent > -0.001 && m_pReport->m_fFaultCurrent < 0.001)
				sReturn = "子站未上送";
			else
				sReturn.Format("%.2f A", m_pReport->m_fFaultCurrent);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTDESCRIBE$") //故障情况
		{
			return m_pReport->m_sFaultDescribe;
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
CString CXJDisplayDecision::GetValue_getcount( CString sKey )
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
		if(sKey == "$DECISION_WAVE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWave.GetSize());
			return sReturn;
		}
		if(sKey == "$DECISION_PROTECT_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listProtect.GetSize());
			return sReturn;
		}
		if(sKey == "$DECISION_BREAK_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listBreak.GetSize());
			return sReturn;
		}
		//由单个关键字组成
		if(sKey == "$DEVICE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listSec.GetSize());
			return sReturn;
		}
		if(sKey == "$DECISION_WAVE_LACK_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWaveLack.GetSize());
			return sReturn;
		}
		if(sKey == "$DECISION_PROTECT_LACK_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listProtectLack.GetSize());
			return sReturn;
		}
		
	}
	else
	{
		// $DEVICE_LIST[1]::FAULT_PT_OSC_LIST.GetCount()$
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
			
			if(strTail == "FAULT_ACTION_SIGN_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempSign.GetSize());
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
CString CXJDisplayDecision::GetValue_OneLayer( CString sKey )
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
	if(strHead == "$DECISION_WAVE_LIST")
	{
		//录波
		if(m_pReport->m_listWave.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listWave.GetSize())
			return "";
		CDecisionAppraise* pObj = (CDecisionAppraise*)m_pReport->m_listWave.GetAt(nIndex);
		if(pObj == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->GetDeviceName();
		}
		if(strTail == "DECISION_JUDGE$")
		{
			return pObj->GetAppraise();
		}
	}
	else if(strHead == "$DECISION_WAVE_LACK_LIST")
	{
		//录波缺少情况
		if(m_pReport->m_listWaveLack.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listWaveLack.GetSize())
			return "";
		CDecisionAppraise* pObj = (CDecisionAppraise*)m_pReport->m_listWaveLack.GetAt(nIndex);
		if(pObj == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->GetDeviceName();
		}
	}
	else if(strHead == "$DECISION_PROTECT_LIST")
	{
		//保护行为评价
		if(m_pReport->m_listProtect.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listProtect.GetSize())
			return "";
		CDecisionAppraise* pObj = (CDecisionAppraise*)m_pReport->m_listProtect.GetAt(nIndex);
		if(pObj == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->GetDeviceName();
		}
		if(strTail == "DECISION_JUDGE$")
		{
			return pObj->GetAppraise();
		}
	}
	else if(strHead == "$DECISION_PROTECT_LACK_LIST")
	{
		//保护缺少信息情况
		if(m_pReport->m_listProtectLack.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listProtectLack.GetSize())
			return "";
		CDecisionAppraise* pObj = (CDecisionAppraise*)m_pReport->m_listProtectLack.GetAt(nIndex);
		if(pObj == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->GetDeviceName();
		}
	}
	else if(strHead == "$DECISION_BREAK_LIST")
	{
		//断路器行为评价
		if(m_pReport->m_listBreak.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listBreak.GetSize())
			return "";
		CDecisionAppraise* pObj = (CDecisionAppraise*)m_pReport->m_listBreak.GetAt(nIndex);
		if(pObj == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->GetDeviceName();
		}
		if(strTail == "DECISION_JUDGE$")
		{
			return pObj->GetAppraise();
		}
	}
	else if(strHead == "$DEVICE_LIST")
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
CString CXJDisplayDecision::GetValue_TwoLayer( CString sKey )
{
	CString strLog;
	strLog.Format("CXJDisplayDecision::GetValue_TwoLayer, key is %s", sKey);
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
	CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
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
		if(pEvent->m_pSec != NULL)
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

		int nFaultActionTime = m_nFaultActionTime;
		if(nFaultActionTime == 0)
		{
			nOrgMs = pEventFirst->m_nmsSign; //原始毫秒值
			tmStartTime = pEventFirst->m_tmSign;
		}
		else if(nFaultActionTime == 1)
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
CSize CXJDisplayDecision::InitDisplay()
{
// 	if(m_pReport == NULL)
// 		return CSize(0,0);
	if(pReportDoc == NULL)
		return CSize(0,0);
	CString sDir = m_sPath + m_sPtName;

	//打开模板文件
	if(!m_bLoadTemplateFile)
	{
		pReportDoc->OpenReportFileEx(sDir, this);
		m_bLoadTemplateFile = TRUE;
	}

	CSize szDoc(0, 0);
	szDoc = pReportDoc->FillValueData();
	return szDoc;
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   SetTemplateFilePath	
返回值:   void	
功能概要: 
参数: CString sPath	
参数: CString sPtName	
*****************************************************/
void CXJDisplayDecision::SetTemplateFilePath( CString sPath, CString sPtName )
{
	m_sPath = sPath;
	m_sPtName = sPtName;
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   SetDecisionReport	
返回值:   void	
功能概要: 
参数: CDecisionReport * pReport	
*****************************************************/
void CXJDisplayDecision::SetDecisionReport( CDecisionReport* pReport )
{
	m_pReport = pReport;
}
