#include "XJDisplayFault.h"
#include "FaultEvent.h"

/****************************************************
Date:2011/11/4  Author:LYH
������:   CXJDisplayFault	
����ֵ:   	
���ܸ�Ҫ: 
����: CFaultReport * pReport	
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
������:   ~CXJDisplayFault	
����ֵ:   	
���ܸ�Ҫ: 
*****************************************************/
CXJDisplayFault::~CXJDisplayFault()
{

}

/****************************************************
Date:2011/11/4  Author:LYH
������:   TranslateSingleKey	
����ֵ:   CString	�ؼ��ִ����ֵ
���ܸ�Ҫ: ���͵����ؼ��ִ��������
����: CString sKey	�ؼ�������
*****************************************************/
CString CXJDisplayFault::TranslateSingleKey( CString sKey )
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
		if(sKey == "$DEVICE_NAME$") //�����豸
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
		if(sKey == "$FAULT_DISTANCE$") //���ϲ��
			return m_pReport->m_sFaultDistance;
		if(sKey == "$FAULT_TYPE$") //��������
			return m_pReport->GetFaultTypeString(m_pReport->m_nFaultType);
		if(sKey == "$FAULT_TIME$") //����ʱ��
			return m_pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		if(sKey == "$FAULT_TIME_MS$")//����ʱ�����ֵ
		{
			sReturn.Format("%03d", m_pReport->m_nmsFaultTime);
			return sReturn;
		}
		if(sKey == "$FAULT_STATION1$") //���ϳ�վ1
		{
			if(m_pReport->m_pStation1 != NULL)
				return m_pReport->m_pStation1->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_STATION2$") //���ϳ�վ2
		{
			if(m_pReport->m_pStation2 != NULL)
				return m_pReport->m_pStation2->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_DEVICE$") //һ���豸
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
CString CXJDisplayFault::GetValue_getcount( CString sKey )
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
		//�õ�ͷ
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
 �� �� ����GetValue_OneLayer()
 ���ܸ�Ҫ���õ�һ����ȹؼ��ֵ�ֵ
 �� �� ֵ: ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B840204
CString CXJDisplayFault::GetValue_OneLayer( CString sKey )
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
	if(strHead == "$DEVICE_LIST")
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
		if(strTail == "EVENT_TIME1_FULL$")
		{
			//�õ���һ��������Ϣ��ʱ��
			if(pSec != m_pReport->m_pTempSec)
			{
				//�齨��ʱ����
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
			//��������ֵ
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return "";
			}

			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//�����ź�
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_LIST")
		{
			//�澯
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//����¼��
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//¼����¼��
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
			CString str = pEvent->m_sEventDef;
			//�õ��ļ���
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
	
			int nOrgMs = 0;
			CTime tmStartTime;
			if(m_nFaultActionTime == 0)
			{
				nOrgMs = m_pReport->m_nmsFaultTime; //ԭʼ����ֵ
				tmStartTime = m_pReport->m_tmFaultTime;
			}
			else if(m_nFaultActionTime == 1)
			{
				if((pEvent->m_tmStart == pEvent->m_tmSign) &&(pEvent->m_nmsStart == pEvent->m_nmsSign))
				{
					nOrgMs = m_pReport->m_nmsFaultTime; //ԭʼ����ֵ
					tmStartTime = m_pReport->m_tmFaultTime;
				}
				else
				{
					nOrgMs = pEvent->m_nmsStart;
					tmStartTime = pEvent->m_tmStart;
				}
			}
			//int nOrgMs = m_pReport->m_nmsFaultTime; //ԭʼ����ֵ
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
CString CXJDisplayFault::GetValue_TwoLayer( CString sKey )
{
	CString strLog;
	strLog.Format("CXJDisplayFault::GetValue_TwoLayer, key is %s", sKey);
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
	CSecObj* pSec = NULL;
	pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
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
	if(strMid == "FAULT_ACTION_CHR_LIST")
	{
		//��������ֵ
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempChr[0];
	}
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
	if(strMid == "FAULT_PT_OSC_LIST")
	{
		//����¼��
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
		if(m_nFaultActionTime == 0)
		{
			nOrgMs = pEventFirst->m_nmsSign; //ԭʼ����ֵ
			tmStartTime = pEventFirst->m_tmSign;
		}
		else if(m_nFaultActionTime == 1)
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
CSize CXJDisplayFault::InitDisplay()
{
// 	if(m_pReport == NULL)
// 		return CSize(0,0);
	if(pReportDoc == NULL)
		return CSize(0,0);

	//������ϸ��Ϣ
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

	//�ж�Ӧ����ʾʲôģ��
	CString sFile = m_sSinglePtName;
	if(m_pReport != NULL && m_pReport->m_listSec.GetSize() > 1)
	{
		sFile = m_sMultiPtName;
	}

	CString sDir = m_sPath + sFile;

	//��ģ���ļ�
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
������:   SetTemplateFilePath	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sPath	
����: CString sSingle	
����: CString sMulti	
*****************************************************/
void CXJDisplayFault::SetTemplateFilePath( CString sPath, CString sSingle, CString sMulti )
{
	m_sPath = sPath;
	m_sSinglePtName = sSingle;
	m_sMultiPtName = sMulti;
}

/****************************************************
Date:2011/11/17  Author:LYH
������:   SetFaultReport	
����ֵ:   void	
���ܸ�Ҫ: 
����: CFaultReport * pReport	
*****************************************************/
void CXJDisplayFault::SetFaultReport( CFaultReport* pReport )
{
	m_pReport = pReport;
}

/****************************************************
Date:2011/11/22  Author:LYH
������:   HitTestOscEvent	
����ֵ:   int	
���ܸ�Ҫ: 
����: CPoint point	
����: CString & strPath	
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
	//�õ���Ӧ���¼�
	CFaultEvent* pEvent = GetEventByKey(strValueCode);
	if(pEvent != NULL)
	{
		//��¼����ʱ�Ų���
		if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
		{
			CString sPath = pEvent->m_sEventDef;

			//�ж��Ƿ��к�׺��
			if(sPath.Find(".cfg") == -1 && sPath.Find(".dat") == -1 && sPath.Find(".CFG") == -1 && sPath.Find(".DAT") == -1)
			{
				//����CFG��׺
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
 �� �� ����GetEventByKey()
 ���ܸ�Ҫ���ɹؼ��ֵõ���Ӧ�Ĺ����¼�, �Ƕ�Ӧ$OSC_FILE_NAME$�Ĳŷ���ָ��, ������ΪNULL
 �� �� ֵ: �����¼�ָ��
 ��    ����param1 �ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B8401E4
CFaultEvent* CXJDisplayFault::GetEventByKey( CString sKey )
{
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return NULL;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return NULL;
	//����ÿ���ؼ���, �����¼�, ��¼β���ַ�
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
	//�������
	if(pReturnEvent == NULL)
		return NULL;
	if(arrTail.GetSize() < 1)
	{
		//û��β���ؼ���
		return NULL;
	}
	return pReturnEvent;
}

/*************************************************************
 �� �� ����GetEventBySignleKey()
 ���ܸ�Ҫ���ɵ����ؼ����ҵ���Ӧ�Ĺ����¼�
 �� �� ֵ: �����¼�ָ��
 ��    ����param1	�ؼ���
		   Param2	β���ؼ���
**************************************************************/
//##ModelId=49B87B8401E6
CFaultEvent* CXJDisplayFault::GetEventBySignleKey( CString sKey, CString & sTail)
{
	//�ж�������Ч��
	if(sKey == "")
		return NULL;
	if(m_pReport == NULL)
		return NULL;
	sTail = "";
	CFaultEvent* pEvent = NULL;
	int nFind = -1;
	//�ж��Ƿ����[ ]�����
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//����[]�����
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//û�ҵ���������
			return NULL;
		}
		int nFind3 = sKey.Find('[', nFind2);
		if(nFind3 != -1)
		{
			//������
			int nFind4 = sKey.Find(']', nFind3);
			if(nFind4 == -1)
				return NULL;
			//�õ�����
			CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
			int nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			//�õ�ͷ
			CString strHead = sKey.Left(nFind);
			if(strHead != "$DEVICE_LIST")
				return NULL;
			//�豸�б�, ȡ���豸
			if(m_pReport->m_listSec.GetSize() < 1)
				return NULL;
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return NULL;
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return NULL;
			//������ʱ�¼��б�
			if(pSec != m_pReport->m_pTempSec)
			{
				//�齨��ʱ����
				m_pReport->BuildTempList(pSec);
			}
			
			//�õ�����
			strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
			nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);
			
			//�õ��¼�
			CFaultEvent* pEvent = NULL;
			if(strMid == "FAULT_ACTION_CHR_LIST")
			{
				//��������ֵ
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
			}
			if(strMid == "FAULT_ACTION_SIGN_LIST")
			{
				//�����ź�
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempSign.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempSign[nIndex];
			}
			if(strMid == "FAULT_PT_OSC_LIST")
			{
				//����¼��
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempPTOSC.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSC[nIndex];
			}
			//ȡ��β��
			CString str = sKey.Right(sKey.GetLength() - (nFind4 + 3));
			sTail = "$";
			sTail += str;
			return pEvent;
		}
		//ȡ��������
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		//ȡ��ͷ��
		CString strHead = sKey.Left(nFind);
		//ȡ���¼�
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//��������ֵ
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//�����ź�
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_ LIST")
		{
			//�澯
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//����¼��
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//¼����¼��
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(pEvent == NULL)
			return NULL;
		//ȡ��β���ַ�
		sTail = "$";
		sTail += sKey.Right(sKey.GetLength() - (nFind2 + 3));
	}
	return pEvent;
}

/****************************************************
Date:2011/11/22  Author:LYH
������:   GetOscFullPath	
����ֵ:   CString	
���ܸ�Ҫ: 
����: CSecObj * pSec	
����: CString strFileName	
*****************************************************/
CString CXJDisplayFault::GetOscFullPath( CSecObj* pSec, CString strFileName )
{
	return "";
}
