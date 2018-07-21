#include "XJDisplayDecision.h"

/****************************************************
Date:2011/11/4  Author:LYH
������:   CXJDisplayDecision	
����ֵ:   	
���ܸ�Ҫ: 
����: CDecisionReport * pReport	
*****************************************************/
CXJDisplayDecision::CXJDisplayDecision( CDecisionReport* pReport )
{
	m_pReport = pReport;
	m_bLoadTemplateFile = FALSE;
	m_nFaultActionTime = 0;
}

/****************************************************
Date:2011/11/4  Author:LYH
������:   ~CXJDisplayDecision	
����ֵ:   	
���ܸ�Ҫ: 
*****************************************************/
CXJDisplayDecision::~CXJDisplayDecision()
{

}

/****************************************************
Date:2011/11/4  Author:LYH
������:   TranslateSingleKey	
����ֵ:   CString	�ؼ��ִ����ֵ
���ܸ�Ҫ: ���͵����ؼ��ִ��������
����: CString sKey	�ؼ�������
*****************************************************/
CString CXJDisplayDecision::TranslateSingleKey( CString sKey )
{
	//�ж�������Ч��
	if(sKey == "")
		return "";
	MYASSERT_STRING(m_pReport);
	CString sReturn = "";
	int nFind = -1;
	//�ж��Ƿ���.GetCount()����
	nFind = sKey.Find(".GetCount()", 0);
	if(nFind > -1)
		return GetValue_getcount(sKey);
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//û��[, Ϊ��ͨ�ؼ���
		//��ͨ�ؼ���
		if(sKey == "$FAULT_DEVICE$")//����Ԫ��
			return m_pReport->m_sFaultDevice;
		if(sKey == "$FAULT_TYPE$")//������λ
			return m_pReport->m_sFaultPhase;
		if(sKey == "$FAULT_DISTANCE$")//���ϲ��
		{
			int nDistance = atoi(m_pReport->m_sFaultDistance);
			if(nDistance == 0)
				sReturn = "��վδ����";
			else
				sReturn.Format("%s km", m_pReport->m_sFaultDistance);
			return sReturn;
		}
		if(sKey == "$DECISION_VOLTAGE$") //��ѹ�ȼ�
		{
			sReturn.Format("%dkV", m_pReport->m_nVoltage);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTTYPE$")//��������
			return m_pReport->m_sFaultType;
		if(sKey == "$DECISION_FAULTREASON$") //����ԭ��
			return m_pReport->m_sFaultReason;
		if(sKey == "$DECISION_FIRSTTIME$") //���ι��ϳ���ʱ��
		{
			sReturn.Format("%d ��", m_pReport->m_nFirstTime);
			return sReturn;
		}
		if(sKey == "$DECISION_SECONDTIME$") //����ʱ��
		{
			sReturn.Format("%d ��", m_pReport->m_nSecondTime);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTCURRENT$")//���ϵ���
		{
			if(m_pReport->m_fFaultCurrent > -0.001 && m_pReport->m_fFaultCurrent < 0.001)
				sReturn = "��վδ����";
			else
				sReturn.Format("%.2f A", m_pReport->m_fFaultCurrent);
			return sReturn;
		}
		if(sKey == "$DECISION_FAULTDESCRIBE$") //�������
		{
			return m_pReport->m_sFaultDescribe;
		}
	}
	else
	{
		int nFind2 = sKey.Find('[', nFind+1);
		if(nFind2 == -1)
		{
			//ֻ��һ��[
			return GetValue_OneLayer(sKey);
		}
		else
		{
			nFind = sKey.Find('[', nFind2 + 1);
			if(nFind == -1)
			{
				//ֻ������
				return GetValue_TwoLayer(sKey);
			}
		}
	}
	return "";
}

/*************************************************************
 �� �� ����GetValue_getcount()
 ���ܸ�Ҫ���õ��ؼ���getcount��ֵ
 �� �� ֵ: ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B8401F7
CString CXJDisplayDecision::GetValue_getcount( CString sKey )
{
	//�ж�������Ч��
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//�����Ƿ���[
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//�ɵ����ؼ������
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
		//�ɵ����ؼ������
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
			//�õ�����
			int nFind1 = sKey.Find(']', nFind);
			if(nFind1 == -1)
				return "";
			CString sIndex = sKey.Mid(nFind+1, nFind1 - nFind -1);
			int nIndex = atoi(sIndex);
			//�õ�������Ӧ���豸
			if(m_pReport->m_listSec.GetSize() < 1)
				return "";
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return "";
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return "";
			if(pSec != m_pReport->m_pTempSec)
			{
				//�齨��ʱ����
				m_pReport->BuildTempList(pSec);
			}
			//�õ���εĹؼ��� // $DEVICE_LIST[1]::FAULT_PT_OSC_LIST.GetCount()$
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
 �� �� ����GetValue_OneLayer()
 ���ܸ�Ҫ���õ�һ����ȹؼ��ֵ�ֵ
 �� �� ֵ: ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B840204
CString CXJDisplayDecision::GetValue_OneLayer( CString sKey )
{
	//$DEVICE_LIST[11]::DEVICE_NAME$
	//�ж�������Ч��
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//����[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//�õ�����
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//�õ�ͷ,β
	CString strHead = sKey.Left(nFind);
	CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
	if(strHead == "$DECISION_WAVE_LIST")
	{
		//¼��
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
		//¼��ȱ�����
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
		//������Ϊ����
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
		//����ȱ����Ϣ���
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
		//��·����Ϊ����
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
		//�豸�б�, ȡ���豸
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
 �� �� ����GetValue_TwoLayer()
 ���ܸ�Ҫ���õ�������ȹؼ��ֵ�ֵ
 �� �� ֵ: ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B840206
CString CXJDisplayDecision::GetValue_TwoLayer( CString sKey )
{
	CString strLog;
	strLog.Format("CXJDisplayDecision::GetValue_TwoLayer, key is %s", sKey);
	WriteLog(strLog);
	//�ж�������Ч��
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//����[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//�õ�����
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//�õ�ͷ,β
	CString strHead = sKey.Left(nFind);
	if(strHead != "$DEVICE_LIST")
		return "";
	//�豸�б�, ȡ���豸
	if(m_pReport->m_listSec.GetSize() < 1)
		return "";
	if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
		return "";
	CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
	if(pSec == NULL)
		return "";
	//������ʱ�¼��б�
	if(pSec != m_pReport->m_pTempSec)
	{
		//�齨��ʱ����
		m_pReport->BuildTempList(pSec);
	}
	//ȡ���м�Ĺؼ���
	int nFind3 = sKey.Find('[', nFind2);
	if(nFind3 == -1)
		return "";
	int nFind4 = sKey.Find(']', nFind3);
	if(nFind4 == -1)
		return "";
	//�õ�����
	strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
	nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);

	//ȡ��β��
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

	//�õ��¼�
	CFaultEvent* pEvent = NULL;
	CFaultEvent* pEventFirst = NULL; //��һ���¼�,�����������ʱ��
	
	if(strMid == "FAULT_ACTION_SIGN_LIST")
	{
		//�����ź�
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
		sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:ʱ%M:%S");
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
			//������
			int nEventDef = atoi(pEvent->m_sEventDef);
			if(nEventDef == 2)
			{
				//��������
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
			//�����źŻ�澯
			if(pEvent->m_sEventContent == "1")
			{
				return "����";
			}
			else if(pEvent->m_sEventContent == "0")
			{
				return "����";
			}
		}
		return pEvent->m_sEventContent;
	}
	if(strTail == "OSC_FILE_NAME$")
	{
		//¼���򱨲���
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		//�õ��ļ���
		CString str = pEvent->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
			str.Delete(0, nRFind+1);
		return str;
	}
	if(strTail == "OSC_FILE_TIME$")
	{
		//¼���򱨲���
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_RELETIVE_TIME$")
	{
		//�ж��Ƿ��¼�
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
			nOrgMs = pEventFirst->m_nmsSign; //ԭʼ����ֵ
			tmStartTime = pEventFirst->m_tmSign;
		}
		else if(nFaultActionTime == 1)
		{
			if((pEvent->m_tmStart == pEvent->m_tmSign) &&(pEvent->m_nmsStart == pEvent->m_nmsSign))
			{
				nOrgMs = pEventFirst->m_nmsStart; //ԭʼ����ֵ
				tmStartTime = pEventFirst->m_tmStart;
			}
			else
			{
				nOrgMs = pEvent->m_nmsStart;
				tmStartTime = pEvent->m_tmStart;
			}
		}
		CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;

		LONG nSEC = tmSpan.GetTotalSeconds(); //�������
		//תΪ����ֵ
		LONG nMs = nSEC* 1000;
		LONG nCurMs = pEvent->m_nmsSign + nMs;
		//�����ֵ
		LONG nDis = nCurMs - nOrgMs;
		sReturn.Format("%d ����", nDis);
		return sReturn;
	}

	return "";
}

/****************************************************
Date:2011/11/4  Author:LYH
������:   InitDisplay	
����ֵ:   void	
���ܸ�Ҫ: ��ʼ����ʾ
*****************************************************/
CSize CXJDisplayDecision::InitDisplay()
{
// 	if(m_pReport == NULL)
// 		return CSize(0,0);
	if(pReportDoc == NULL)
		return CSize(0,0);
	CString sDir = m_sPath + m_sPtName;

	//��ģ���ļ�
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
������:   SetTemplateFilePath	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sPath	
����: CString sPtName	
*****************************************************/
void CXJDisplayDecision::SetTemplateFilePath( CString sPath, CString sPtName )
{
	m_sPath = sPath;
	m_sPtName = sPtName;
}

/****************************************************
Date:2011/11/17  Author:LYH
������:   SetDecisionReport	
����ֵ:   void	
���ܸ�Ҫ: 
����: CDecisionReport * pReport	
*****************************************************/
void CXJDisplayDecision::SetDecisionReport( CDecisionReport* pReport )
{
	m_pReport = pReport;
}
