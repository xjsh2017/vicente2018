// FaultReport.cpp : implementation file
//

#include "DecisionReport.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/


/****************************************************
Date:2011/11/17  Author:LYH
函数名:   CDecisionAppraise	
返回值:   	
功能概要: 
参数: CDataEngine * pDataEngine	
参数: CString sDeviceID	
*****************************************************/
CDecisionAppraise::CDecisionAppraise( CDataEngine* pDataEngine, CString sDeviceID,CString sContent, int nDeviceType )
{
	m_pDataEngine = pDataEngine;
	m_sDeviceID = sDeviceID;
	m_sContent = sContent;
	m_nDeviceType = nDeviceType;
	m_pDevice = NULL;
	BuildReleation();
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   ~CDecisionAppraise	
返回值:   	
功能概要: 
*****************************************************/
CDecisionAppraise::~CDecisionAppraise()
{

}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   GetDeviceName	
返回值:   CString	
功能概要: 
*****************************************************/
CString CDecisionAppraise::GetDeviceName()
{
	if(m_pDevice != NULL)
		return m_pDevice->m_sName;
	return "";
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   GetAppraise	
返回值:   CString	
功能概要: 
*****************************************************/
CString CDecisionAppraise::GetAppraise()
{
	CString sReturn = "";
	int nContent = atoi(m_sContent);
	switch(nContent)
	{
	case 0:
		sReturn = "缺少";
		break;
	case 1:
		sReturn = "正常";
		break;
	case 2:
		sReturn = "误动";
		break;
	case 3:
		sReturn = "拒动";
		break;
	}
	return sReturn;
}

/****************************************************
Date:2011/11/17  Author:LYH
函数名:   BuildReleation	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CDecisionAppraise::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	m_pDevice = m_pDataEngine->FindDevice(m_sDeviceID, m_nDeviceType);
	return TRUE;
}

/****************************************************
Date:2011/12/15  Author:LYH
函数名:   GetAppraiseInt	
返回值:   int	
功能概要: 
*****************************************************/
int CDecisionAppraise::GetAppraiseInt()
{
	return atoi(m_sContent);
}



/****************************************************
Date:2011/10/17  Author:LYH
函数名:   CDecisionReport	
返回值:   	
功能概要: 
参数: CDataEngine * pDataEngine	
*****************************************************/
CDecisionReport::CDecisionReport( CDataEngine* pDataEngine ) : CXJNotify(pDataEngine)
{
	/** @brief           故障包ID*/
	m_nID = -1;
	m_listSec.RemoveAll();

	/** @brief			故障元件名*/
	m_sFaultDevice = "";
	/** @brief			故障相位*/
	m_sFaultPhase = "";
	/** @brief			故障测距*/
	m_sFaultDistance = "";
}

//##ModelId=49B87B9A005E
CDecisionReport::~CDecisionReport()
{
	m_listSec.RemoveAll();
	ClearDetail();
	ClearWaveDescribe();
	ClearProtectJudge();
	ClearBreakJudge();
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：从数据库载入信息
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	数据集指针
		   Param2
**************************************************************/
//##ModelId=49B87B9A010E
BOOL CDecisionReport::LoadFromDB( CMemSet* pMemset )
{
	// 判断数据有效性
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0); //ID
	m_nID = atoi(str);
	str = pMemset->GetValue(1); //对应故障包
	m_nFaultID = atoi(str);
	str = pMemset->GetValue(2); //一次设备ID
	m_nVoltage = atoi(str);
	m_sFaultType = pMemset->GetValue(3); //故障类型
	m_sFaultReason = pMemset->GetValue(4); //故障原因
	str = pMemset->GetValue(5); //初次故障持续时间
	m_nFirstTime = atoi(str);
	str = pMemset->GetValue(6); //重合闸不成功时的故障持续时间
	m_nSecondTime = atoi(str);
	str = pMemset->GetValue(7); //故障电流
	m_fFaultCurrent = atof(str);
	m_sFaultDescribe = pMemset->GetValue(8); //故障情况
	m_sWaveDescribe = pMemset->GetValue(10); //录波情况
	m_sProtectJudge = pMemset->GetValue(12); //保护设备评价
    m_sBreakJudge = pMemset->GetValue(13); //断路器行为评价
	m_sFaultAdvise = pMemset->GetValue(14);//对策建议
	
	if(!BuildReleation())
	{
		WriteLog("CDecisionReport::LoadFromDB 载入数据失败");
		return FALSE;
	}

	LoadDetail();

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
BOOL CDecisionReport::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	ParseWaveDescribe(m_sWaveDescribe);
	ParseProtectJudge(m_sProtectJudge);
	ParseBreakJudge(m_sBreakJudge);
	
	return TRUE;
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetNotifyType	
返回值:   int	
功能概要: 
*****************************************************/
int CDecisionReport::GetNotifyType()
{
	return XJ_NOTIFY_DECISION;
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetDescribe	
返回值:   CString	事件描述字符串
功能概要: 获得事件描述
*****************************************************/
CString CDecisionReport::GetDescribe()
{
	return "!";
}

/****************************************************
Date:2011/11/8  Author:LYH
函数名:   GetSimpleDescribe	
返回值:   CString	事件简要描述字符串
功能概要: 获得事件简要描述
*****************************************************/
CString CDecisionReport::GetSimpleDescribe()
{
	CString str,strTemp;
	str = "辅助决策:";

	return str;
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ParseWaveDescribe	
返回值:   void	
功能概要: 
参数: CString sStr	
*****************************************************/
void CDecisionReport::ParseWaveDescribe( CString sStr )
{
	ClearWaveDescribe();
	//格式: PTID,0;PTID;1
	CString strDes = sStr;
	int nFind = strDes.Find(";", 0);
	while(nFind != -1)
	{
		CString sTemp = strDes.Left(nFind);
		strDes.Delete(0, nFind+1);
		nFind = strDes.Find(";", 0);
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 == -1)
			continue;
		CString sPTID = sTemp.Left(nFind1);
		sTemp.Delete(0, nFind1+1);
		CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, sTemp, TYPE_SEC);
		m_listWave.Add(pAppr);
		if(pAppr->GetAppraiseInt() == 0)
			m_listWaveLack.Add(pAppr);
	}
	if(!strDes.IsEmpty())
	{
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 != -1)
		{
			CString sPTID = strDes.Left(nFind1);
			strDes.Delete(0, nFind1+1);
			CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, strDes, TYPE_SEC);
			m_listWave.Add(pAppr);
			if(pAppr->GetAppraiseInt() == 0)
				m_listWaveLack.Add(pAppr);
		}
	}
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ClearWaveDescribe	
返回值:   void	
功能概要: 
*****************************************************/
void CDecisionReport::ClearWaveDescribe()
{
	m_listWaveLack.RemoveAll();
	for(int i = 0; i < m_listWave.GetSize(); i++)
	{
		CDecisionAppraise* pAppr = (CDecisionAppraise*)m_listWave.GetAt(i);
		if(pAppr != NULL)
			delete pAppr;
	}
	m_listWave.RemoveAll();
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ClearProtectJudge	
返回值:   void	
功能概要: 
*****************************************************/
void CDecisionReport::ClearProtectJudge()
{
	m_listProtectLack.RemoveAll();

	for(int i = 0; i < m_listProtect.GetSize(); i++)
	{
		CDecisionAppraise* pAppr = (CDecisionAppraise*)m_listProtect.GetAt(i);
		if(pAppr != NULL)
			delete pAppr;
	}
	m_listProtect.RemoveAll();
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ClearBreakJudge	
返回值:   void	
功能概要: 
*****************************************************/
void CDecisionReport::ClearBreakJudge()
{
	for(int i = 0; i < m_listBreak.GetSize(); i++)
	{
		CDecisionAppraise* pAppr = (CDecisionAppraise*)m_listBreak.GetAt(i);
		if(pAppr != NULL)
			delete pAppr;
	}
	m_listBreak.RemoveAll();
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ParseProtectJudge	
返回值:   void	
功能概要: 
参数: CString sStr	
*****************************************************/
void CDecisionReport::ParseProtectJudge( CString sStr )
{
	ClearProtectJudge();
	//格式: PTID,0;PTID;1
	CString strDes = sStr;
	int nFind = strDes.Find(";", 0);
	while(nFind != -1)
	{
		CString sTemp = strDes.Left(nFind);
		strDes.Delete(0, nFind+1);
		nFind = strDes.Find(";", 0);
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 == -1)
			continue;
		CString sPTID = sTemp.Left(nFind1);
		sTemp.Delete(0, nFind1+1);
		CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, sTemp, TYPE_SEC);
		m_listProtect.Add(pAppr);
		if(pAppr->GetAppraiseInt() == 0)
			m_listProtectLack.Add(pAppr);
	}
	if(!strDes.IsEmpty())
	{
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 != -1)
		{
			CString sPTID = strDes.Left(nFind1);
			strDes.Delete(0, nFind1+1);
			CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, strDes, TYPE_SEC);
			m_listProtect.Add(pAppr);
			if(pAppr->GetAppraiseInt() == 0)
				m_listProtectLack.Add(pAppr);
		}
	}
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ParseBreakJudge	
返回值:   void	
功能概要: 
参数: CString sStr	
*****************************************************/
void CDecisionReport::ParseBreakJudge( CString sStr )
{
	ClearBreakJudge();
	//格式: PTID,0;PTID;1
	CString strDes = sStr;
	int nFind = strDes.Find(";", 0);
	while(nFind != -1)
	{
		CString sTemp = strDes.Left(nFind);
		strDes.Delete(0, nFind+1);
		nFind = strDes.Find(";", 0);
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 == -1)
			continue;
		CString sPTID = sTemp.Left(nFind1);
		sTemp.Delete(0, nFind1+1);
		CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, sTemp, TYPE_SEC);
		m_listBreak.Add(pAppr);
	}
	if(!strDes.IsEmpty())
	{
		int nFind1 = strDes.Find(",", 0);
		if(nFind1 != -1)
		{
			CString sPTID = strDes.Left(nFind1);
			strDes.Delete(0, nFind1+1);
			CDecisionAppraise* pAppr = new CDecisionAppraise(m_pDataEngine, sPTID, strDes, TYPE_SEC);
			m_listBreak.Add(pAppr);
		}
	}
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   LoadDetail	
返回值:   BOOL	
功能概要: 
参数: BOOL bForce	
*****************************************************/
BOOL CDecisionReport::LoadDetail( BOOL bForce /*= FALSE*/ )
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

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
	str.Format("reverse1 = %d", m_nID);
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
		WriteLog("CDecisionReport::LoadDetail, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CDecisionReport::LoadDetail, 读取到%d条事件记录", nCount);
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

			if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
			{
				m_listSign.Add(pEvent);
			}
			else
			{
				delete pEvent;
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDecisionReport::LoadDetail,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	CString strLog;
	strLog.Format("CDecisionReport::LoadDetail,信号量%d条", m_listSign.GetSize());
	WriteLog(strLog);

	//排序
	SortActionSign();

	return bReturn;
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   ClearDetail	
返回值:   void	
功能概要: 
*****************************************************/
void CDecisionReport::ClearDetail()
{
	m_pTempSec = NULL;
	m_listTempSign.RemoveAll();
	int i = 0;
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   SortActionSign	
返回值:   void	
功能概要: 
*****************************************************/
void CDecisionReport::SortActionSign()
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
	
	//动作信号量
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
}

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   CompareSign	
返回值:   int	
功能概要: 
参数: CFaultEvent * pEvent1	
参数: CFaultEvent * pEvent2	
*****************************************************/
int CDecisionReport::CompareSign( CFaultEvent* pEvent1, CFaultEvent* pEvent2 )
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

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   AddSec	
返回值:   void	
功能概要: 
参数: CDeviceObj * pObj	
*****************************************************/
void CDecisionReport::AddSec( CDeviceObj* pObj )
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

/****************************************************
Date:2011/11/24  Author:LYH
函数名:   BuildTempList	
返回值:   void	
功能概要: 
参数: CSecObj * pSec	
*****************************************************/
void CDecisionReport::BuildTempList( CSecObj* pSec )
{
	m_pTempSec = pSec;

	int i = 0;

	//动作信号量
	m_listTempSign.RemoveAll();
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempSign.Add(pEvent);
		}
	}
}

