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
������:   CDecisionAppraise	
����ֵ:   	
���ܸ�Ҫ: 
����: CDataEngine * pDataEngine	
����: CString sDeviceID	
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
������:   ~CDecisionAppraise	
����ֵ:   	
���ܸ�Ҫ: 
*****************************************************/
CDecisionAppraise::~CDecisionAppraise()
{

}

/****************************************************
Date:2011/11/17  Author:LYH
������:   GetDeviceName	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CDecisionAppraise::GetDeviceName()
{
	if(m_pDevice != NULL)
		return m_pDevice->m_sName;
	return "";
}

/****************************************************
Date:2011/11/17  Author:LYH
������:   GetAppraise	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CDecisionAppraise::GetAppraise()
{
	CString sReturn = "";
	int nContent = atoi(m_sContent);
	switch(nContent)
	{
	case 0:
		sReturn = "ȱ��";
		break;
	case 1:
		sReturn = "����";
		break;
	case 2:
		sReturn = "��";
		break;
	case 3:
		sReturn = "�ܶ�";
		break;
	}
	return sReturn;
}

/****************************************************
Date:2011/11/17  Author:LYH
������:   BuildReleation	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CDecisionAppraise::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	m_pDevice = m_pDataEngine->FindDevice(m_sDeviceID, m_nDeviceType);
	return TRUE;
}

/****************************************************
Date:2011/12/15  Author:LYH
������:   GetAppraiseInt	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CDecisionAppraise::GetAppraiseInt()
{
	return atoi(m_sContent);
}



/****************************************************
Date:2011/10/17  Author:LYH
������:   CDecisionReport	
����ֵ:   	
���ܸ�Ҫ: 
����: CDataEngine * pDataEngine	
*****************************************************/
CDecisionReport::CDecisionReport( CDataEngine* pDataEngine ) : CXJNotify(pDataEngine)
{
	/** @brief           ���ϰ�ID*/
	m_nID = -1;
	m_listSec.RemoveAll();

	/** @brief			����Ԫ����*/
	m_sFaultDevice = "";
	/** @brief			������λ*/
	m_sFaultPhase = "";
	/** @brief			���ϲ��*/
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
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ�������ݿ�������Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݼ�ָ��
		   Param2
**************************************************************/
//##ModelId=49B87B9A010E
BOOL CDecisionReport::LoadFromDB( CMemSet* pMemset )
{
	// �ж�������Ч��
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0); //ID
	m_nID = atoi(str);
	str = pMemset->GetValue(1); //��Ӧ���ϰ�
	m_nFaultID = atoi(str);
	str = pMemset->GetValue(2); //һ���豸ID
	m_nVoltage = atoi(str);
	m_sFaultType = pMemset->GetValue(3); //��������
	m_sFaultReason = pMemset->GetValue(4); //����ԭ��
	str = pMemset->GetValue(5); //���ι��ϳ���ʱ��
	m_nFirstTime = atoi(str);
	str = pMemset->GetValue(6); //�غ�բ���ɹ�ʱ�Ĺ��ϳ���ʱ��
	m_nSecondTime = atoi(str);
	str = pMemset->GetValue(7); //���ϵ���
	m_fFaultCurrent = atof(str);
	m_sFaultDescribe = pMemset->GetValue(8); //�������
	m_sWaveDescribe = pMemset->GetValue(10); //¼�����
	m_sProtectJudge = pMemset->GetValue(12); //�����豸����
    m_sBreakJudge = pMemset->GetValue(13); //��·����Ϊ����
	m_sFaultAdvise = pMemset->GetValue(14);//�Բ߽���
	
	if(!BuildReleation())
	{
		WriteLog("CDecisionReport::LoadFromDB ��������ʧ��");
		return FALSE;
	}

	LoadDetail();

	return TRUE;
}

/*************************************************************
 �� �� ����BuildReleation()
 ���ܸ�Ҫ���������豸�Ĺ���
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
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
������:   GetNotifyType	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CDecisionReport::GetNotifyType()
{
	return XJ_NOTIFY_DECISION;
}

/****************************************************
Date:2011/10/31  Author:LYH
������:   GetDescribe	
����ֵ:   CString	�¼������ַ���
���ܸ�Ҫ: ����¼�����
*****************************************************/
CString CDecisionReport::GetDescribe()
{
	return "!";
}

/****************************************************
Date:2011/11/8  Author:LYH
������:   GetSimpleDescribe	
����ֵ:   CString	�¼���Ҫ�����ַ���
���ܸ�Ҫ: ����¼���Ҫ����
*****************************************************/
CString CDecisionReport::GetSimpleDescribe()
{
	CString str,strTemp;
	str = "��������:";

	return str;
}

/****************************************************
Date:2011/11/24  Author:LYH
������:   ParseWaveDescribe	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sStr	
*****************************************************/
void CDecisionReport::ParseWaveDescribe( CString sStr )
{
	ClearWaveDescribe();
	//��ʽ: PTID,0;PTID;1
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
������:   ClearWaveDescribe	
����ֵ:   void	
���ܸ�Ҫ: 
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
������:   ClearProtectJudge	
����ֵ:   void	
���ܸ�Ҫ: 
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
������:   ClearBreakJudge	
����ֵ:   void	
���ܸ�Ҫ: 
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
������:   ParseProtectJudge	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sStr	
*****************************************************/
void CDecisionReport::ParseProtectJudge( CString sStr )
{
	ClearProtectJudge();
	//��ʽ: PTID,0;PTID;1
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
������:   ParseBreakJudge	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sStr	
*****************************************************/
void CDecisionReport::ParseBreakJudge( CString sStr )
{
	ClearBreakJudge();
	//��ʽ: PTID,0;PTID;1
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
������:   LoadDetail	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: BOOL bForce	
*****************************************************/
BOOL CDecisionReport::LoadDetail( BOOL bForce /*= FALSE*/ )
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	//���֮ǰ����Ϣ
	ClearDetail();

	BOOL bReturn = TRUE;

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//���� MainRecID���������ֶ�
	CString str;		
	//����

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
		WriteLog("CDecisionReport::LoadDetail, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CDecisionReport::LoadDetail, ��ȡ��%d���¼���¼", nCount);
		WriteLog(sLog, XJ_LOG_LV3);
		for(int i = 0; i < nCount; i++)
		{
			//��������
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
		str.Format("CDecisionReport::LoadDetail,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	CString strLog;
	strLog.Format("CDecisionReport::LoadDetail,�ź���%d��", m_listSign.GetSize());
	WriteLog(strLog);

	//����
	SortActionSign();

	return bReturn;
}

/****************************************************
Date:2011/11/24  Author:LYH
������:   ClearDetail	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CDecisionReport::ClearDetail()
{
	m_pTempSec = NULL;
	m_listTempSign.RemoveAll();
	int i = 0;
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
}

/****************************************************
Date:2011/11/24  Author:LYH
������:   SortActionSign	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CDecisionReport::SortActionSign()
{
	//�Ƿ�����Ϣ
	if(m_listSign.GetSize() < 1)
		return;
	BOOL bSortPTID = FALSE; //�Ƿ���Ҫ��PTID����
	//��ʱ����
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		for(int k = 1; k < m_listSign.GetSize()-i; k++)
		{
			CFaultEvent* pEvent1 = (CFaultEvent*)m_listSign.GetAt(k-1);
			CFaultEvent* pEvent2 = (CFaultEvent*)m_listSign.GetAt(k);
			if(CompareSign(pEvent1, pEvent2) > 0)
			{
				//ǰ��Ⱥ����, ��ǰ��ĺ���
				m_listSign.SetAt(k, pEvent1);
				m_listSign.SetAt(k-1, pEvent2);
			}
			if(pEvent1->m_sSecID != pEvent2->m_sSecID)
				bSortPTID = TRUE;
		}
	}

	//�������б���
	m_listSec.RemoveAll();
	
	//�����ź���
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
}

/****************************************************
Date:2011/11/24  Author:LYH
������:   CompareSign	
����ֵ:   int	
���ܸ�Ҫ: 
����: CFaultEvent * pEvent1	
����: CFaultEvent * pEvent2	
*****************************************************/
int CDecisionReport::CompareSign( CFaultEvent* pEvent1, CFaultEvent* pEvent2 )
{
	//������ݺϷ���
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	//�±Ƚ�ʱ��
	if(pEvent1->m_tmSign == pEvent2->m_tmSign)
	{
		//ʱ�����,�ȽϺ���
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
������:   AddSec	
����ֵ:   void	
���ܸ�Ҫ: 
����: CDeviceObj * pObj	
*****************************************************/
void CDecisionReport::AddSec( CDeviceObj* pObj )
{
	MYASSERT(pObj);
	//�����Ƿ��Ѿ���������
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
	//���뵽����
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
������:   BuildTempList	
����ֵ:   void	
���ܸ�Ҫ: 
����: CSecObj * pSec	
*****************************************************/
void CDecisionReport::BuildTempList( CSecObj* pSec )
{
	m_pTempSec = pSec;

	int i = 0;

	//�����ź���
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

