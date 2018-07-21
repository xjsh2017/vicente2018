// Event.cpp : implementation file
//

#include "Event.h"

#pragma warning(disable : 4996)//����4786����
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
 �� �� ����BuildReleation()
 ���ܸ�Ҫ�������¼����豸�Ĺ�ϵ,�����ҵ������豸ָ��,���һ���豸ID,����,��վID,��վ��,������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   �����豸����
**************************************************************/
//##ModelId=49B87B9C0109
BOOL CXJEvent::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	
	//�Ȳ��Ҷ����豸ָ��
	m_pSec = (CSecObj*)m_pDataEngine->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL)
		return FALSE;
	//�����豸����
	m_sSecName = m_pSec->m_sName;

	if(m_pSec->m_pOwner != NULL && m_pSec->m_nType == TYPE_PROTECT)
	{
		//һ���豸ID,����
		m_sDeviceId = m_pSec->m_pOwner->m_sID;
		m_sDeviceName = m_pSec->m_pOwner->m_sName;
	}
	//��վID
	m_sStationId = m_pSec->m_sStationID;
	if(m_pSec->m_pStation != NULL)
	{
		m_sStationName = m_pSec->m_pStation->m_sName;
		CNetObj * pNet = (CNetObj*)m_pDataEngine->FindDevice(m_pSec->m_pStation->m_sNetID, TYPE_NET);
		if(pNet != NULL)
		{
			//��������
			m_sNetName = pNet->m_sName;
		}
	}

	return TRUE;
}

/*************************************************************
 �� �� ����Clear()
 ���ܸ�Ҫ������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
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
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ�������ݿ��в�ѯ���¼���ID
 �� �� ֵ: ��ѯ�ɹ�����TRUE,ʧ�ܷ���FALSE(�н���ҽ���һ�������Ϊ��ѯ�ɹ�)
**************************************************************/
//##ModelId=49B87B9C01B5
BOOL CXJEvent::QueryNewEventID()
{
	return TRUE;
}

/*************************************************************
 �� �� ��: IsReturnEvent()
 ���ܸ�Ҫ: �ж��Ƿ�"����"�¼�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CXJEvent::IsReturnEvent()
{
	//û���ź���
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
������:   GetNotifyType	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CXJEvent::GetNotifyType()
{
	return XJ_NOTIFY_EVENT;
}

/****************************************************
Date:2011/11/15  Author:LYH
������:   GetAlarmType	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CXJEvent::GetAlarmType()
{
	//�����źź�, ȡ��߼��ź�����Ϊ�¼��Ĺ�������
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
 �� �� ����Ack()
 ���ܸ�Ҫ���澯�¼�ȷ��(��TB_PT_ALARM�޸�Is_Ack�ֶ�),TRUE-�ɹ�
 �� �� ֵ: TRUE-�ɹ�, FALSE-ʧ��
**************************************************************/
//##ModelId=49B87B9D0290
BOOL CXJEventAlarm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;

	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�

		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventAlarm::Ack, ����ʧ��");
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
			str.Format("CXJEventAlarm::Ack,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}

	//д��־���������
	CString str;
	if(bReturn)
		str.Format("�澯�¼�[%d]�ѱ�ȷ��",m_lID);
	else
		str.Format("�澯�¼�[%d]ȷ��ʱ����",m_lID);
	WriteLog(str);

	return bReturn;	
}

/*
 *  @brief   	CSTEventAlarm::GetDescribe	 �õ��澯�¼����� 
 *  @return 	CString �澯�¼�����
 */
//##ModelId=49B87B9D031C
CString CXJEventAlarm::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";

	str += "��վID: " + m_sStationName + "\r\n";  //��վ��
	str += "һ���豸��: " + m_sDeviceName + "\r\n"; //һ���豸����
	str += "�����豸��: " + m_sSecName + "\r\n"; //�����豸����

	if (m_pSec != NULL)
	{
		//��ʾ��������
		str += "��������: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0)?"δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";

	str += "*******************************************\r\n";
	strTemp.Format("�����Ǹ澯�¼���Ϣ:\r\n %s", m_sMsg);
	str += strTemp;
	
	return str;
}

/*
 *  @brief   	CSTEventAlarm::Clone	 ���Ƹ澯�¼�����
 *  @return 	CSTEvent* �������ɵĶ���
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
	
	//�Ƿ������Ϣ
	pEvent->m_IsDebug		= m_IsDebug;
	//�Ƿ񷵻���Ϣ
	pEvent->m_nIsReturn = m_nIsReturn;

	//�����豸ָ��
	pEvent->m_pSec = m_pSec;

    //�ź�������
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
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_alarm
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9E0157
BOOL CXJEventAlarm::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0);//�¼�ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //��Ϣ���� 
		m_sSecID = pMemSet->GetValue(2); //����ID
		str = pMemSet->GetValue(3); //�澯ID
		m_lAlarmID = atol(str);
		str = pMemSet ->GetValue(4); //Դ��ֵ
		m_nValue = atoi(str);
		str = pMemSet->GetValue(5); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(6); //����ʱ��
		//ʱ��ת��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(7); //����ʱ�����ֵ
		m_nms = atoi(str);
		str = pMemSet->GetValue(8); //���ʱ�����ֵ
		m_nms2 = atoi(str);
		str = pMemSet->GetValue(9); //��Ϣ����ʱ��
		m_tmTime2 = StringToTime(str);
		str = pMemSet->GetValue(10); //�Ƿ������Ϣ
		m_IsDebug = atoi(str);
		str = pMemSet->GetValue(12); //�¼�����
		m_nProperty = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventAlarm::LoadFromDB, ��������ʧ��", XJ_LOG_LV3);
	}

	//�������豸�Ĺ�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	//�Ƿ񷵻���Ϣ
	m_nIsReturn = IsReturnEvent();

	//����FData
// 	if(!TranslateFData())
// 	{
// 		WriteLogEx("CXJEventAlarm::LoadFromDB, ����FDataʧ��", XJ_LOG_LV3);
// 		return FALSE;
// 	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP�����ж�ȡ�澯�¼�����,(20018)
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	STTP�������ݽṹ
		   param2	ָ���Ǳ��������еĵڼ���������
**************************************************************/
//##ModelId=49B87B9E01E4
BOOL CXJEventAlarm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20018)
	{
		CString str;
		str.Format("CXJEventAlarm::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	
	//�ж϶�Ӧ����
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventAlarm::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}

	//�Ƿ������Ϣ
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	CString str;
	str.Format("%d", sttpData.sttp_head.uMsgLengthType);
	WriteLog("CXJEventAlarm::LoadFromSttp sttpData.sttp_head.uMsgLengthType = " + str, 3);
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//�����澯�ź���
	if(!TranslateSttpData(sttpData, nIndex))
	{
		//����ʧ��
		WriteLog("CXJEventAlarm::LoadFromSttp, ������������ʧ��", XJ_LOG_LV3);
		return FALSE;
	}

	//�Ƿ񷵻���Ϣ
	m_nIsReturn = IsReturnEvent();

	return TRUE;
}

/*************************************************************
 �� �� ����TranslateFData()
 ���ܸ�Ҫ������FData�ֶ�, ������ź����б�
 �� �� ֵ: ����ֵ˵��
**************************************************************/
//##ModelId=49B87B9D0186
BOOL CXJEventAlarm::TranslateFData()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	m_bLoadDetail = TRUE;
	if (m_Signals.GetSize() > 0)//�Ѿ����������
		return TRUE;
	if(m_pSec == NULL)
	{
		return FALSE;
	}

	if(m_pSec != NULL)
	{
		if(m_pSec->m_arrAlarmSignal.GetSize() == 0) //���û�г�ʼ����
		{
			m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
		}
	}

	//���֮ǰ��
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
 �� �� ����TranslateSttpData()
 ���ܸ�Ҫ�������澯�¼������ź�������
 �� �� ֵ: �����ɹ�����TURE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ��������еĵڼ���������
**************************************************************/
//##ModelId=49B87B9D0203
BOOL CXJEventAlarm::TranslateSttpData(STTP_FULL_DATA& sttpData, int nIndex)
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());
	if (m_Signals.GetSize() > 0)//�Ѿ����������
		return TRUE;
	if(m_pSec == NULL)
	{
		return FALSE;
	}

	//���֮ǰ��
	Clear();

	//�ź�������
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//���������ֵ
		return FALSE;
	}

	if(m_pSec != NULL)
	{
		if(m_pSec->m_arrAlarmSignal.GetSize() == 0) //���û�г�ʼ����
		{
			m_pSec->RefreshConfig(m_pDataEngine->GetDBEngine());
		}
	}

	PT_Signal *pSignal = new PT_Signal;
	pSignal->nID = sttpData.sttp_body.variant_member.value_data[nIndex].id;
	//�����ź�����Ϣ
	PT_Signal *pSignalConfig = GetSignalConfig(pSignal->nID);
	if(pSignalConfig == NULL)
	{
		//δ�ҵ�,delete
		delete pSignal;
		pSignal = NULL;
		return FALSE;
	}

	//��������
	*pSignal  = *pSignalConfig;

	//ֵ
	CString strValue = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	pSignal->nValue = atoi(strValue);
	//�����¼�ֵ
	m_lAlarmID = pSignal->nID;
	m_nValue = pSignal->nValue;
	//ʱ��,ת����ʽ
	CString strTime = sttpData.sttp_body.variant_member.value_data[nIndex].ch_Time;
	int nms;
	CTime tm = StringToTimeSttp15(strTime, nms);
	CString strTemp;
	strTemp.Format("%s.%03d",tm.Format("%Y-%m-%d %H:%M:%S"), nms);
	//�����¼�ʱ��
	m_tmTime = tm;
	m_nms = nms;
	//�����ź���ʱ��
	pSignal->strTime = strTemp;

	//����msg
	m_sMsg.Format("%s %s", pSignal->strName, (pSignal->nValue == 1)?"����":"����");

	//�����¼�����
	m_nProperty = pSignal->nEventType;
	CString strOut;
	strOut.Format("���¼�Ϊ%s���¼�", GetAlarmType());
	WriteLog(strOut, 3);

	m_Signals.Add(pSignal);

	//û���ź����ĸ澯�¼���Ϊ�Ǵ����
	if(m_Signals.GetSize() <= 0)
		return FALSE;

	return TRUE;
}

/*************************************************************
 �� �� ����GetSignalConfig()
 ���ܸ�Ҫ���õ�ָ�����ź���
 �� �� ֵ: �ҵ�ID��ָ���ź����򷵻��ź���, δ�ҵ�����NULL
 ��    ����param1   �ź���ID
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
 �� �� ����GetAlarmType()
 ���ܸ�Ҫ��ȡ�ø澯�¼����
 �� �� ֵ: �澯�¼�����
**************************************************************/
//##ModelId=49B87B9E005D
CString CXJEventAlarm::GetAlarmType()
{
	//�����źź�, ȡ��߼��ź�����Ϊ�¼��Ĺ�������
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
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
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
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,ms,alaram_id,value����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventAlarm::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventAlarm::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
������:   GetSimpleDescribe	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CXJEventAlarm::GetSimpleDescribe()
{
	//common
	CString str,strTemp;
	str = "�澯:";
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
 �� �� ����Ack()
 ���ܸ�Ҫ�������¼�ȷ��(��TB_PT_ACTION�޸�Is_Ack�ֶ�)
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9F00BB
BOOL CXJEventAction::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	try
	{
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventAction::Ack, ����ʧ��");
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
			str.Format("CXJEventAction::Ack,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
	}

	//д��־���������
	CString str;
	if(bReturn)
		str.Format("���������¼�[%d]�ѱ�ȷ��",m_lID);
	else
		str.Format("���������¼�[%d]ȷ��ʱ����",m_lID);
	WriteLog(str);

	return bReturn;
}
	
/*************************************************************
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;
	//�Ƿ񷵻���Ϣ
	pEvent->m_nIsReturn = m_nIsReturn;

	//special
	m_nEventType			= XJ_EVENT_ACTION;
	pEvent->m_nms2			= m_nms2		;
	pEvent->m_tmTime2		= m_tmTime2		;
	pEvent->m_FData			= m_FData		;

	//����װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	//�������������ź���
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
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ������¼���ϸ����
 �� �� ֵ: ��ʽ���������ַ���
**************************************************************/
//##ModelId=49B87B9F0128
CString CXJEventAction::GetDescribe()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("�����¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";

	str += "��վ��:   " + m_sStationName  + "\r\n";
	str += "һ���豸��:   " + m_sDeviceName  + "\r\n";
	str += "�����豸��:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "�豸����: " + m_pSec->m_sModel + "\r\n";
	else
		str += "�豸����: δ֪\r\n";
	//��������
	strTemp.Format("��������: %s ", GetFaultType(m_sFaultType));
	str += strTemp + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("װ������ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	strTemp.Format("��Ϣ����ʱ��: %s.%03d",m_tmTime2.Format("%Y-%m-%d %H:%M:%S"),m_nms2);
	str += strTemp + "\r\n";
 
	//ToDo: ����Msg����ʽex: 2002-01-07 11:59:59.785  REL521 V2.0 AB  
	str += "*******************************************\r\n";
	if(!TranslateFData())	//��FData�л������ֵ�б�Ϳ������б�
		return str;

	//����ɸѡ���� add by yinzhehong 20051230
    strTemp.Empty();
    for(i=0; i< m_Signals.GetSize(); i++)
	{
		pSignal = (PT_Signal *)m_Signals[i];
		nEventType = pSignal->nEventType;
		strValue.Format("%25s  %20s(%12s)\t%3s\r\n",pSignal->strTime,pSignal->strName,pSignal->strCodeName,pSignal->nValue ==1 ? "����":"����");
		strTemp += strValue;
		nCount++;		   
	}
	strValue.Format("�������ź����б���Ϣ:(��%d��)\r\n", nCount);
	str += strValue;
	str += strTemp;

	str += "*******************************************\r\n";
	strTemp.Format("�����ǲ���ֵ��Ϣ:(��%d��)\r\n", m_ActChrs.GetSize());
	str += strTemp;
	for(i=0;i<m_ActChrs.GetSize();i++)
	{
		//2008-04-29 ���뵥λ��ʾ
		strTemp.Format("%s\t:%s %s\r\n",m_ActChrs[i]->strName,m_ActChrs[i]->strValue,m_ActChrs[i]->strUnits);
		str += strTemp;
	}
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����TranslateFData()
 ���ܸ�Ҫ������FData���ݣ������� m_ActChrs m_Signals
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
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
	
	if ((m_ActChrs.GetSize() >0) || (m_Signals.GetSize() >0))//�Ѿ����������
		return TRUE;
	if (!RefreshConfig())
		return FALSE;

	int i = 0;
	//ɾ��ԭ����	
	Clear();

	//�����µ�
	int nPos, nCount, nErrAC, nErrS;
	CString strData = m_FData;
	//CString strData = "1 ,94 ,2827,1 ,18 ,1,2010-09-14 15:28:42.502";
	CString strTemp;
	BOOL bSucAC = TRUE, bSucS = TRUE;
	try
	{
		//����������
		nPos = strData.Find(',', 0);
		nCount = _ttoi(strData.Left(nPos));

		//��������ֵ
		strData.Delete(0, nPos+1);
		for( i = 0; i < nCount; i++ )
		{
			PT_ActionCharacter *pAC = new PT_ActionCharacter;

			//ID
			nPos = strData.Find(',',0);
			pAC->nID = _ttoi(strData.Left(nPos));
			strData.Delete(0, nPos+1);

			//ȡ������������
			PT_ActionCharacter *pACConfig = GetActChrsConfig(pAC->nID);
			if (pACConfig == NULL)
			{
				//���Ҳ�������������
				//��ͨ��������������ȥ��
				pACConfig = GetActChrsFromUniversal(pAC ->nID);
				if(pACConfig == NULL)
				{
					bSucAC = FALSE;
					nErrAC = pAC->nID;
					nPos = strData.Find(',',0);
					strData.Delete(0, nPos+1);
					//���
					delete pAC;
					pAC = NULL;
					continue;
				}
			}

			//��������������, ֱ�Ӹ���
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

		//�ź�������
		nPos = strData.Find(',',0);
		nCount = _ttoi(strData.Left(nPos));

		//�����ź���
		strData.Delete(0, nPos+1);
		for( i = 0; i < nCount; i++ )
		{
			PT_Signal *pSignal = new PT_Signal;
			//ID
			nPos = strData.Find(',',0);
			pSignal->nID = _ttoi(strData.Left(nPos));
			strData.Delete(0, nPos+1);

			//ȡ���ź�������
			PT_Signal *pSignalConfig = GetSignalConfig(pSignal->nID);
			if (pSignalConfig == NULL)
			{
				bSucS = FALSE;
				nErrS = pSignal->nID;
				nPos = strData.Find(',',0);
				strData.Delete(0, nPos+1);
				nPos = strData.Find(',',0);
				strData.Delete(0, nPos+1);
				////ɾ����new�����Ŀռ�
				delete pSignal;
				pSignal = NULL;
				////
				continue;
			}

			//�����ź�������
			*pSignal = *pSignalConfig;//ֱ�ӿ���ȫ������
			
			//Value                     �޸�ֵ
			nPos = strData.Find(',',0);
			pSignal->nValue = _ttoi(strData.Left(nPos));
			strData.Delete(0,nPos+1);

			//DateTime
			nPos = strData.Find(',',0); //ʱ��
			if(nPos>0)
			{
				pSignal->strTime = strData.Left(nPos);
				strData.Delete(0, nPos+1);
			}
			else//���һ��
				pSignal->strTime = strData;
			
			m_Signals.Add(pSignal);
		}
	}
	catch(...)
	{
	//	return FALSE;
	}

	//�ڴ���ɸѡ
	if ((m_ActChrs.GetSize() > 0)||(m_Signals.GetSize() > 0)) //�������̳�����Ȼ������ų���
		return TRUE;
	else
	{
		//û�����������ź������¼���Ϊ�Ǵ����¼�
		return FALSE;
	}
}

//##ModelId=49B87B9F0261
BOOL CXJEventAction::TranslateSttpData(STTP_FULL_DATA& sttpData)
{
	if ((m_ActChrs.GetSize() >0) || (m_Signals.GetSize() >0))//�Ѿ����������
		return TRUE;
	if (!RefreshConfig())
		return FALSE;
	
	int i = 0;
	//ɾ��ԭ����	
	Clear();

	int nPro = 2;
	//����������
	int nCount = sttpData.sttp_body.variant_member.value_data2.size();
	//ѭ����ȡ��������
	for(i = 0; i < nCount; i++)
	{
		//�½�����������
		PT_ActionCharacter* pAC = new PT_ActionCharacter;
		//ID
		pAC->nID = sttpData.sttp_body.variant_member.value_data2[i].id;
		//ȡ������������
		PT_ActionCharacter *pACConfig = GetActChrsConfig(pAC->nID);
		if (pACConfig == NULL)
		{
			//���Ҳ�������������
			//��ͨ��������������ȥ��
			pACConfig = GetActChrsFromUniversal(pAC ->nID);
			if(pACConfig == NULL)
			{
				//���
				delete pAC;
				pAC = NULL;
				continue;
			}
		}		
		//��������������, ֱ�Ӹ���
		*pAC = *pACConfig;
		
		//Value
		string str = sttpData.sttp_body.variant_member.value_data2[i].str_value;
		CString sValue = XJToMS(str);
		pAC->strValue = ComplexExchange(sValue);
		//�����¼���������
		if(pAC->nID == 2)
		{
			m_sFaultType = pAC->strValue;
		}
		//�����¼��ȼ�
		if(pAC->nEventType < nPro)
			nPro = pAC->nEventType;
		//���뵽����
		m_ActChrs.Add(pAC);
	}

	
	//��Ϣ��
	nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nCount > 0)
		nPro = 2;
	//ѭ����ȡ���ź���
	for(i = 0; i < nCount; i++)
	{
		//�½��ź�������
		PT_Signal* pSN = new PT_Signal;
		//ID
		pSN->nID = sttpData.sttp_body.variant_member.value_data[i].id;
		//ȡ���ź�������
		PT_Signal * pSignalConfig = GetSignalConfig(pSN->nID);
		if(pSignalConfig == NULL)
		{
			//���
			delete pSN;
			pSN = NULL;
			continue;
		}

		//�����ź���
		*pSN = *pSignalConfig;

		//����ֵ
		//stringҪת��ΪCString
		string str = sttpData.sttp_body.variant_member.value_data[i].str_value;
		CString sValue = XJToMS(str);
		pSN->nValue = atoi(sValue);
		//����ʱ��
		pSN->strTime = "";
		pSN->strTime = sttpData.sttp_body.variant_member.value_data[i].ch_Time;
		CTime tm;
		int nms = 0;
		tm = StringToTimeSttp15(pSN->strTime, nms);
		pSN->strTime.Format("%s.%03d", tm.Format("%Y-%m-%d %H:%M:%S"), nms);

		//�����¼��ȼ�
		if(pSN->nEventType < nPro)
			nPro = pSN->nEventType;

		if(i == 0)
		{
			//�����¼���ʾ
			m_sMsg.Format("%s %s", pSN->strName, (pSN->nValue == 1)?"����":"����");
		}

		//���뵽����
		m_Signals.Add(pSN);
	}

	//���û���ź��������Ե�һ������֮Ϊ����
	if(m_Signals.GetSize() <= 0)
	{
		for(i = 0; i < m_ActChrs.GetSize(); i++)
		{
			PT_ActionCharacter* pAC = (PT_ActionCharacter*)m_ActChrs.GetAt(i);
			if(pAC == NULL)
				continue;
			if(pAC->nID == 94) //�������
				continue;
			//�����¼���ʾ
			if(pAC->strUnits == "")
				m_sMsg.Format("%s %s", pAC->strName, pAC->strValue);
			else
				m_sMsg.Format("%s %s(%s)", pAC->strName, pAC->strValue, pAC->strUnits);
			break;
		}
	}

	//�����¼�����
	m_nProperty = nPro;
	CString strOut;
	strOut.Format("���¼�Ϊ%s���¼�", GetAlarmType());
	WriteLog(strOut, 3);

	if ((m_ActChrs.GetSize() > 0)||(m_Signals.GetSize() > 0)) //�������̳�����Ȼ������ų���
		return TRUE;
	else
	{
		//û�����������ź������¼���Ϊ�Ǵ����¼�
		return FALSE;
	}
}

/*************************************************************
 �� �� ����GetSignalConfig()
 ���ܸ�Ҫ������ָ��ID�õ�ָ�����ź���
 �� �� ֵ: ���ҵ�ָ�����ź����򷵻��ź�������, δ���ҵ�����NULL
 ��    ����param1   �ź���ID
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
 �� �� ����GetActChrsConfig()
 ���ܸ�Ҫ������ָ��ID�õ�ָ����������
 �� �� ֵ: ���ҵ�ָ�����������򷵻�����������, δ���ҵ�����NULL
 ��    ����param1   ������ID
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
 �� �� ����GetActChrsFromUniversal()
 ���ܸ�Ҫ����ͨ���������в�������������
 �� �� ֵ: ���ҵ��򷵻�������ָ��, δ�ҵ�����NULL
 ��    ����param1   ����˵��
           Param2   ����˵��
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
 �� �� ����RefreshConfig()
 ���ܸ�Ҫ��ˢ�¶�Ӧ�����豸���ź����б�
 �� �� ֵ: ˢ�³ɹ�����TRUE, ʧ�ܷ���FALSE
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
 �� �� ����GetAlarmType()
 ���ܸ�Ҫ��ȡ�ù�������,I,II,III��
 �� �� ֵ: ��������
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
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_action
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9F030D
BOOL CXJEventAction::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //msg
		m_sSecID = pMemSet->GetValue(2); //pt_id
		str = pMemSet->GetValue(3); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(4); //װ������ʱ��
		//m_tmTime1 �˴���Ҫʱ��ת��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(5); //����ʱ�����ֵ
		m_nms = atoi(str);
		str = pMemSet->GetValue(6); //����ʱ�����ֵ
		m_nms2 = atoi(str);
		str = pMemSet->GetValue(8); //��վϵͳ�յ��¼���ʱ��,������Ϊ����ʱ��
		//m_tmTime2 �˴���Ҫʱ��ת��
		m_tmTime2 = StringToTime(str);
		m_sFaultType = pMemSet->GetValue(10); //��������
		str = pMemSet->GetValue(11); //�Ƿ����
		m_IsDebug = atoi(str);
		//---------------------------------------------------------------
		m_FData = pMemSet->GetValue(12); //��������
		str = pMemSet->GetValue(14); //�¼�����
		m_nProperty = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventAction::LoadFromDB, ��������ʧ��");
		return FALSE;
	}
	

	//����������ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	//����FData
	if(!TranslateFData())
	{
		WriteLog("CXJEventAction::LoadFromDB, ����FDataʧ��");
		return FALSE;
	}

	//�Ƿ񷵻���Ϣ
	m_nIsReturn = IsReturnEvent();

	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����STTP�����ж�ȡ��Ϣ��䵽���¼�������,(20022)
 �� �� ֵ: ��ȡ��Ϣ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ��������еĵڼ���������
**************************************************************/
//##ModelId=49B87B9F036B
BOOL CXJEventAction::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20022)
	{
		CString str;
		str.Format("CXJEventAction::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}

	//�ж϶�Ӧ����
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventAction::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//װ������ʱ��
	CString sTime;
	sTime = sttpData.sttp_body.ch_time_15_BIT1;
	//��ʱ���ַ���תΪCTime����
	m_tmTime = StringToTimeSttp15(sTime, m_nms);

	//�������ʱ��
	sTime = sttpData.sttp_body.ch_time_15_BIT2;
	m_tmTime2 = StringToTimeSttp15(sTime, m_nms2);

	//�Ƿ������Ϣ
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//������������
	if(!TranslateSttpData(sttpData))
	{
		//����ʧ��
		WriteLog("CXJEventAction::LoadFromSttp, ������������ʧ��");
		return FALSE;
	}

	//�Ƿ񷵻���Ϣ
	m_nIsReturn = IsReturnEvent();
	
	WriteLog("CXJEventAction::LoadFromSttp, ����");
	return TRUE;
}

/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼������ݿ��е�ID(��Ϊ���¼�����STTP���Ľ�������,��֪�������ݿ��е�ID)
 �� �� ֵ: ��ѯ�ɹ�����TRUE,ʧ�ܷ���FALSE(�н���ҽ���һ�������Ϊ��ѯ�ɹ�)
**************************************************************/
//##ModelId=49B87BA0002E
BOOL CXJEventAction::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;

	if(m_pSec == NULL)
		return FALSE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,ms,recvtime,ms2����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventAction::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventAction::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
������:   GetSimpleDescribe	
����ֵ:   CString	�¼���Ҫ�����ַ���
���ܸ�Ҫ: ����¼���Ҫ����
*****************************************************/
CString CXJEventAction::GetSimpleDescribe()
{
	//common
	CString str,strTemp;
	str = "����:";
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
//CXJEventFault �¹ʱ���

//////////////////////////////////////////////////////////////////////////
//CXJEventDI	������
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
 �� �� ����Ack()
 ���ܸ�Ҫ����������λ�¼�ȷ��(��tb_pt_change_di�޸�Is_Ack�ֶ�)
 �� �� ֵ: TRUE-�ɹ�, FALSE-ʧ��
**************************************************************/
//##ModelId=49B87BA002B2
BOOL CXJEventDI::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	
	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventDI::Ack, ����ʧ��");
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
			str.Format("CXJEventDI::Ack,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;		
	}
	catch(...)
	{
	}
	//д��־���������
	CString str;
	if(bReturn)
		str.Format("�������¼�[%d]�ѱ�ȷ��",m_lID);
	else
		str.Format("�������¼�[%d]ȷ��ʱ����",m_lID);
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_DI;
	pEvent->m_nCPU			= m_nCPU		;
	pEvent->m_nDIID			= m_nDIID		;
	pEvent->m_nValue		= m_nValue		;
	pEvent->m_sDIName		= m_sDIName		;

	//����װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_change_di
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA0037A
BOOL CXJEventDI::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sMsg = pMemSet->GetValue(1); //����
		m_sDIName = m_sMsg; //��MSG�ֶ��б��濪�������������
		m_sSecID = pMemSet->GetValue(2); //����ID
		str = pMemSet->GetValue(3); //CPU��
		m_nCPU = atoi(str);
		str = pMemSet->GetValue(4); //������ID
		m_nDIID = atoi(str);
		str = pMemSet->GetValue(5); //�Ƿ�ȷ��
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(6); //����ֵ
		m_nValue = atoi(str);
		str = pMemSet->GetValue(7); //����ʱ��
		//time exchange
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(8); //����ʱ�����ֵ
		m_nms = atoi(str);
		str = pMemSet->GetValue(9); //����?
		m_IsDebug = atoi(str);
	}
	catch (...)
	{
		WriteLog("CXJEventDI::LoadFromDB, ��������ʧ��");
		return FALSE;
	}
	
	//�������豸�Ĺ�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventDI::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog);
		return FALSE;
	}
	//ȡ�ÿ���������
//	GetDIName();
	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ��������λ��Ϣ,��20010��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA003B9
BOOL CXJEventDI::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20010)
	{
		CString str;
		str.Format("CXJEventDI::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//�ж϶�Ӧ����
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventDI::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//�Ƿ������Ϣ
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//�Ƿ�����
		return FALSE;
	}

	//cpu
	m_nCPU = sttpData.sttp_body.nCpu;

	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_15_BIT1;
	m_tmTime = StringToTimeSttp15(strTime, m_nms);

	//������ID
	m_nDIID = sttpData.sttp_body.variant_member.value_data[nIndex].id;

	//����ֵ
	CString strValue = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	m_nValue = atoi(strValue);

	//ȡ�ÿ���������
	GetDIName();

	return TRUE;
}

/*************************************************************
 �� �� ����GetDIName()
 ���ܸ�Ҫ������,�����豸ID, CPU��, ������ID,�����ݿ��в�ѯ�õ�����������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA1000F
BOOL CXJEventDI::GetDIName()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	//�齨��ѯ����

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	CString str;

	//Ҫ��ѯ���ֶ�
	//������Name
	Field Name;
	bzero(&Name, sizeof(Field));
	str = "Name";
	strncpy(Name.FieldName, str, str.GetLength());
	Name.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Name);

	//����
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
		str.Format("CXJEventDI::GetDIName,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼���ID
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA1004E
BOOL CXJEventDI::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,ms,di_id,value,cpu����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventDI::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventDI::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ��¼���ϸ����
 �� �� ֵ: �¼������ַ���
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA0033C
CString CXJEventDI::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("��������λ�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վID: " + m_sStationName + "\r\n";  //��վ��
	str += "һ���豸��: " + m_sDeviceName + "\r\n"; //һ���豸����
	str += "�����豸��: " + m_sSecName + "\r\n"; //�����豸����
	
	if (m_pSec != NULL)
	{
		//��ʾ��������
		str += "��������: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0)?"δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("�¼�: %s %s", m_sDIName, (m_nValue == 1)?"����":"����");
	str += strTemp + "\r\n";
	
	return str;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventOSC	¼��
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
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_OSC;
	pEvent->m_sFaultDistance = m_sFaultDistance;
	pEvent->m_sFileName		= m_sFileName;
	pEvent->m_nHaveFault	= m_nHaveFault;
	pEvent->m_sFilePath		= m_sFilePath;
	pEvent->m_sTriggerType	= m_sTriggerType;
	pEvent->m_nRecloseTime  = m_nRecloseTime;

	//װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
�� �� ����LoadFromDB()
���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_osc_report
�� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA10242
BOOL CXJEventOSC::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //�����豸ID
		m_sFileName = pMemSet->GetValue(2); //¼�������ļ���
		//ȥ����׺��
		int nFind = m_sFileName.ReverseFind('.');
		if(nFind > -1)
		{
			m_sFileName = m_sFileName.Left(nFind);
		}
		str = pMemSet ->GetValue(3); //¼��������ʱ��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //����ʱ�����ֵ
		//���������λ��ȥ������λ
		if(str.GetLength() > 3)
		{
			str.Delete(str.GetLength() - 3, 3);
		}
		m_nms = atoi(str);
		m_sFilePath = pMemSet->GetValue(5); //���·��
		str = pMemSet->GetValue(6); //�Ƿ��й���
		m_nHaveFault = atoi(str);
		m_sFaultType = pMemSet->GetValue(7); //��������
		m_sTriggerType = pMemSet->GetValue(8); //��բ���
		str = pMemSet->GetValue(9); //�غ�բʱ��
		m_nRecloseTime = atoi(str);
		str = pMemSet->GetValue(10); //���ϲ��
		m_sFaultDistance = ComplexExchange(str);
		str = pMemSet->GetValue(14); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(16); //�Ƿ������Ϣ
		m_IsDebug = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventOSC::LoadFromDB, ��������ʧ��");
		return FALSE;
	}
	
	//�������豸�͹�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_osc_report
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA10242
BOOL CXJEventOSC::LoadFromDB_(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //�����豸ID
		m_sFileName = pMemSet->GetValue(2); //¼�������ļ���
		//ȥ����׺��
		int nFind = m_sFileName.ReverseFind('.');
		if(nFind > -1)
		{
			m_sFileName = m_sFileName.Left(nFind);
		}
		str = pMemSet ->GetValue(3); //¼��������ʱ��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //����ʱ�����ֵ
		//���������λ��ȥ������λ
		if(str.GetLength() > 3)
		{
			str.Delete(str.GetLength() - 3, 3);
		}
		m_nms = atoi(str);
		m_sFilePath = pMemSet->GetValue(5); //���·��
		str = pMemSet->GetValue(6);
		m_nFileSize = atoi(str);
		str = pMemSet->GetValue(7); //�Ƿ��й���
		m_nHaveFault = atoi(str);
		m_sFaultType = pMemSet->GetValue(8); //��������
		m_sTriggerType = pMemSet->GetValue(9); //��բ���
		str = pMemSet->GetValue(10); //�غ�բʱ��
		m_nRecloseTime = atoi(str);
		str = pMemSet->GetValue(11); //���ϲ��
		m_sFaultDistance = ComplexExchange(str);
		str = pMemSet->GetValue(14); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
		str = pMemSet->GetValue(16); //�Ƿ������Ϣ
		m_IsDebug = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventOSC::LoadFromDB, ��������ʧ��");
		return FALSE;
	}

	//�������豸�͹�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}

	return TRUE;
}
/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ¼���¼���Ϣ,
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA10280
BOOL CXJEventOSC::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20132)
	{
		CString str;
		str.Format("CXJEventOSC::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//�ж϶�Ӧװ��ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventOSC::LoadFromSttp, �Ҳ�����Ӧװ��, װ��IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//�Ƿ������Ϣ
	m_IsDebug = sttpData.sttp_head.uMsgLengthType;
	if(m_IsDebug != XJ_EVENTINFO_DEBUG)
	{
		if(m_pSec->m_nRunStatu == 0)
			m_IsDebug = XJ_EVENTINFO_DEBUG;
	}

	//��������
	//ʱ��
	CString sTime = sttpData.sttp_body.variant_member.file_data.ch_time;
	CString strTemp = "";
	strTemp.Format("¼��ʱ��Ϊ:%s", sTime);
	WriteLog(strTemp);
	m_tmTime = StringToTimeSttp26(sTime, m_nms);
	//�ļ���
	m_sFileName = XJToMS(sttpData.sttp_body.variant_member.file_data.strFileName);
	//
	m_sFileName.Replace("/", "\\");
	m_sFilePath = m_sFileName;

	//����·�����ļ���
	int nFind = m_sFileName.ReverseFind('\\');
	if(nFind > -1)
	{
		m_sFilePath = m_sFileName.Left(nFind); //·��
		m_sFileName.Delete(0, nFind+1); //����
	}

	//ȥ����׺��
	nFind = m_sFileName.ReverseFind('.');
	if(nFind > -1)
	{
		m_sFileName = m_sFileName.Left(nFind);
	}
	//�Ƿ��й���
	m_nHaveFault = sttpData.sttp_body.nFlag;
	//��������
	m_sFaultType = sttpData.sttp_body.ch_version;
	m_sFaultType.TrimLeft();
	m_sFaultType.TrimRight();
	//��բ���
	m_sTriggerType = sttpData.sttp_body.ch_check_code;
	m_sTriggerType.TrimLeft();
	m_sTriggerType.TrimRight();
	//�غ�բʱ��
	m_nRecloseTime = sttpData.sttp_body.nSource;
	//���ϲ��
	CString str = sttpData.sttp_body.ch_HandlerName;
	str.TrimLeft();
	str.TrimRight();
	m_sFaultDistance = ComplexExchange(str);
	
	return TRUE;
}

/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼������ݿ��е�ID
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA102DE
BOOL CXJEventOSC::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,ms����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventOSC::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventOSC::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Ack()
 ���ܸ�Ҫ��ȷ���¼�,�޸����ݿ���е�ȷ���ֶ�
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA101C5
BOOL CXJEventOSC::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	
	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventOSC::Ack, ����ʧ��");
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
			str.Format("CXJEventOSC::Ack,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;		
	}
	catch(...)
	{
	}
	//д��־���������
	CString str;
	if(bReturn)
		str.Format("¼�������¼�[%d]�ѱ�ȷ��",m_lID);
	else
		str.Format("¼�������¼�[%d]ȷ��ʱ����",m_lID);
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ¼���¼���Ϣ,20026
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA102AF
BOOL CXJEventOSC::LoadFromSttpForPT( STTP_FULL_DATA& sttpData, int nIndex )
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20026)
	{
		CString str;
		str.Format("CXJEventOSC::LoadFromSttpForPT, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//�ж϶�Ӧװ��ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventOSC::LoadFromSttpForPT, �Ҳ�����Ӧװ��, װ��IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}
	
	//��������
	//ʱ��
	CString sTime = sttpData.sttp_body.variant_member.file_list[nIndex].ch_time;
	m_tmTime = StringToTimeSttp26(sTime, m_nms);
	//�ļ���
	m_sFileName = XJToMS(sttpData.sttp_body.variant_member.file_list[nIndex]. strFielName);
	m_sFileName.Replace("\\", "/");
	m_sFileName.Replace("\\", "/");
	while(m_sFileName.Left(1) == "/")
	{
		m_sFileName.Delete(1);
	}
	//�ļ���С
	m_nFileSize = sttpData.sttp_body.variant_member.file_list[nIndex].nFileSize;
	//�ļ����
	m_nFileNum = sttpData.sttp_body.variant_member.file_list[nIndex]. nFileNO;
	//����Դ
	m_nDataSrc = sttpData.sttp_body.nSource;
	//�ļ�·��
	m_sFilePath = XJToMS(sttpData.sttp_body.strRelativePath);
	m_sFilePath.Replace("\\", "/");
	if(m_sFilePath.Right(1) != "/")
		m_sFilePath += "/";

	//�ж��Ƿ�������
	m_nDownloaded = IsDownloaded();

	return TRUE;
}

/*************************************************************
 �� �� ����IsDownloaded()
 ���ܸ�Ҫ���жϵ�ǰ¼���б��Ƿ�������
 �� �� ֵ: 0-δ����. 1-������. 2-��������
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1030D
int CXJEventOSC::IsDownloaded()
{
	CString strFilePath = GetFilePath();
	//����ļ��Ƿ����
	CFileStatus   status;	   
	if(CFile::GetStatus(strFilePath ,status) != TRUE)
	{
		//δ�ҵ��ļ�, �����Ƿ���.tmp��ʱ�ļ�
		CString str = GetFilePathNoSuff();
		str += ".tmp";
		if(CFile::GetStatus(str, status) == TRUE)
		{
			//�ҵ����ļ�
			m_nOffset = (int)(status.m_size);
			return 2;
		}
	}
	else
		return 1;
	return 0;
}

/*************************************************************
 �� �� ����GetFilePath()
 ���ܸ�Ҫ���õ�����¼���ļ�ȫ·��
 �� �� ֵ: ����¼���ļ�ȫ·��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1032C
CString CXJEventOSC::GetFilePath()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//�õ���ַ
	CString strDownDir = m_pDataEngine->GetDownDir();
	//Ŀ¼���'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//��֯·��
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\¼����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//�����ļ���
	strFullPath += m_sFileName;
	//����.cfg��׺
	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ��¼���ϸ����
 �� �� ֵ: �¼������ַ���
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA10222
CString CXJEventOSC::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("¼���¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վID: " + m_sStationName + "\r\n";  //��վ��
	str += "һ���豸��: " + m_sDeviceName + "\r\n"; //һ���豸����
	str += "�����豸��: " + m_sSecName + "\r\n"; //�����豸����
	
	if (m_pSec != NULL)
	{
		//��ʾ��������
		str += "��������: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0)?"δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("¼���ļ�����: %s", m_sFileName);
	str += strTemp + "\r\n";

	strTemp.Format("¼���ļ���С: %d", m_nFileSize);
	str += strTemp + "\r\n";
	
	strTemp.Format("��������: %s", GetFaultType(m_sFaultType));
	str += strTemp + "\r\n";

	strTemp.Format("��բ���: %s", GetTriggerType(m_sTriggerType));
	str += strTemp + "\r\n";

	strTemp.Format("���ϲ��: %s", m_sFaultDistance);
	str += strTemp + "\r\n";
	
	return str;
}

/*************************************************************
 �� �� ����GetDownloadString()
 ���ܸ�Ҫ���õ��Ƿ����ص���������
 �� �� ֵ: �Ƿ����ص���������
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA10399
CString CXJEventOSC::GetDownloadString()
{
	if(m_nDownloaded == 0)
		return "δ����";
	else if(m_nDownloaded == 1)
		return "������";
	else if(m_nDownloaded == 2)
		return "��������";
	else if(m_nDownloaded == -1)
		return "����ʧ��";
	else if(m_nDownloaded == -2)
		return "ȡ������";
	return "";
}

/*************************************************************
 �� �� ����GetFilePathNoSuff()
 ���ܸ�Ҫ���õ�������׺����·��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1034B
CString CXJEventOSC::GetFilePathNoSuff()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//�õ���ַ
	CString strDownDir = m_pDataEngine->GetDownDir();
	//Ŀ¼���'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//��֯·��
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\¼����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//�����ļ���
	CString strName = m_sFileName;
	strName.Replace("/", "\\");
	int nFind = strName.ReverseFind('\\');
	if(nFind > -1)
	{
		//����·�����ļ���, ��ֻȡ�ļ�������
		strName = strName.Right(strName.GetLength() - nFind - 1);
	}
	strFullPath += strName;
	//����.cfg��׺
//	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 �� �� ����GetFileRelatePath()
 ���ܸ�Ҫ���õ�������׺�������·��
 �� �� ֵ: ���·��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA1037A
CString CXJEventOSC::GetFileRelatePath()
{
	if(m_pSec == NULL)
		return "";
	//��֯·��
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = m_pSec->m_sStationID + "\\����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = m_pSec->m_sStationID + "\\¼����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	//�����ļ���
	CString strName = m_sFileName;
	strName.Replace("/", "\\");
	int nFind = strName.ReverseFind('\\');
	if(nFind > -1)
	{
		//����·�����ļ���, ��ֻȡ�ļ�������
		strName = strName.Right(strName.GetLength() - nFind - 1);
	}
	strFullPath += strName;
	//����.cfg��׺
	//	strFullPath += ".cfg";
	return strFullPath;
}

/*************************************************************
 �� �� ��: GetFileDir()
 ���ܸ�Ҫ: ȡ���ļ�Ŀ¼
 �� �� ֵ: �ļ�Ŀ¼
 ��    ��: param1 
		   Param2 
**************************************************************/
CString CXJEventOSC::GetFileDir()
{
	MYASSERT_STRING(m_pDataEngine);
	if(m_pSec == NULL)
		return "";
	//�õ���ַ
	CString strDownDir = m_pDataEngine->GetDownDir();
	//Ŀ¼���'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//��֯·��
	CString strFullPath;
	if(m_pSec->m_nType == TYPE_PROTECT)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";
	else if(m_pSec->m_nType == TYPE_WAVEREC)
		strFullPath = strDownDir + m_pSec->m_sStationID + "\\¼����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\";

	return strFullPath;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventPTComm ����ͨѶ״̬
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
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_PTCOMM;
	pEvent->m_nStatus		= m_nStatus		;

	//װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_commu_status
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA200CB
BOOL CXJEventPTComm::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //�����豸ID
		str = pMemSet->GetValue(2); //״̬
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //ʱ��
		//m_tmTime  exchange time
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventPTComm::LoadFromDB, ��������ʧ��");
		return FALSE;
	}

	//�������豸�Ĺ�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ����ͨѶ״̬��Ϣ,(20002)
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA200EA
BOOL CXJEventPTComm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20002)
	{
		CString str;
		str.Format("CXJEventPTComm::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}

	//�ж�����
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//�Ƿ�����
		return FALSE;
	}
	
	//�ж϶�Ӧװ��ID
	m_sSecID = XJToMS(sttpData.sttp_body.variant_member.value_data[nIndex].str_value);
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventPTComm::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//״̬
	m_nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].id;

	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼���ID
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA20109
BOOL CXJEventPTComm::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,status����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventPTComm::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventPTComm::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Ack()
 ���ܸ�Ҫ��ȷ���¼�, ���ݿ����IS_ACK�ֶθ�Ϊ1
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA2007D
BOOL CXJEventPTComm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	
	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventPTComm::Ack, ����ʧ��");
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
			str.Format("CXJEventPTComm::Ack,����ʧ��,ԭ��Ϊ%s", sError);
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
		str.Format("����ͨѶ״̬��λ�¼�[%d]�ѱ�ȷ��",m_lID);
	}
	else
	{
		str.Format("����ͨѶ״̬��λ�¼�[%d]ȷ��ʱ����",m_lID);
	}
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ��¼���ϸ����
 �� �� ֵ: �¼������ַ���
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA200AB
CString CXJEventPTComm::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("ͨѶ״̬��λ�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վID: " + m_sStationName + "\r\n";  //��վ��
	str += "һ���豸��: " + m_sDeviceName + "\r\n"; //һ���豸����
	str += "�����豸��: " + m_sSecName + "\r\n"; //�����豸����
	
	if (m_pSec != NULL)
	{
		//��ʾ��������
		str += "��������: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0)?"δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	strTemp.Format("ͨѶ״̬: %s", (m_nStatus == 0)?"����":"�Ͽ�");
	str += strTemp + "\r\n";
	
	return str;
}
//////////////////////////////////////////////////////////////////////////
//CXJEventPTRun	��������״̬
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
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;

	//special
	m_nEventType			= XJ_EVENT_PTRUN;
	pEvent->m_nStatus		= m_nStatus		;

	//װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_run_status
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA201F4
BOOL CXJEventPTRun::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sSecID = pMemSet->GetValue(1); //�����豸ID
		str = pMemSet->GetValue(2); //״̬
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //ʱ��
		//m_tmTime  exchange time
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(4); //�Ƿ���ȷ��
		m_nIsAck = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventPTRun::LoadFromDB, ��������ʧ��");
		return FALSE;
	}

	//�������豸�Ĺ�����ϵ
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ��������״̬��Ϣ,(20079)
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA20203
BOOL CXJEventPTRun::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CXJEventPTRun::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//�ж϶�Ӧװ��ID
	m_sSecID = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventPTRun::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}

	//״̬
	m_nStatus = sttpData.sttp_body.nStatus;

	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼������ݿ��е�ID
 �� �� ֵ: ��ѯ�ɹ�����TRUE,ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA20222
BOOL CXJEventPTRun::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,curtime,status����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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
		WriteLog("CXJEventPTRun::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventPTRun::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Ack()
 ���ܸ�Ҫ��ȷ���¼�,�ı����ݱ��е�ȷ���ֶ�
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA201B6
BOOL CXJEventPTRun::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	
	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventPTRun::Ack, ����ʧ��");
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
			str.Format("CXJEventPTRun::Ack,����ʧ��,ԭ��Ϊ%s", sError);
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
		str.Format("��������״̬��λ�¼�[%d]�ѱ�ȷ��",m_lID);
	}
	else
	{
		str.Format("��������״̬��λ�¼�[%d]ȷ��ʱ����",m_lID);
	}
	WriteLog(str);
	return bReturn;	
}

/*************************************************************
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ��¼���ϸ����
 �� �� ֵ: �¼������ַ���
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA201D4
CString CXJEventPTRun::GetDescribe()
{
	//common
	CString str,strTemp;
	strTemp.Format("����״̬��λ�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վID: " + m_sStationName + "\r\n";  //��վ��
	str += "һ���豸��: " + m_sDeviceName + "\r\n"; //һ���豸����
	str += "�����豸��: " + m_sSecName + "\r\n"; //�����豸����
	
	if (m_pSec != NULL)
	{
		//��ʾ��������
		str += "��������: " + m_pSec->m_sModel + "\r\n"; //
	}
	else
	{
	}
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0)?"δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d", m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	
	CString str1 = "";
	str1 = GetRunStatusString(m_nStatus);

	strTemp.Format("����״̬: %s", str1);
	str += strTemp + "\r\n";
	
	return str;
}

/****************************************************
Date:2011/10/17  Author:LYH
������:   GetRunStatusString	
����ֵ:   CString	
���ܸ�Ҫ: 
����: int nStatus	
*****************************************************/
CString CXJEventPTRun::GetRunStatusString( int nStatus )
{
	CString sReturn = "";
	switch(nStatus)
	{
	case 0:
		sReturn = "����";
		break;
	case 1:
		sReturn = "ͣ��";
		break;
	case 2:
		sReturn = "Ͷ��";
		break;
	case 3:
		sReturn = "δ����";
		break;
	case 4:
		sReturn = "����(�����)";
		break;
	default:
		sReturn = "δ֪";
	}
	return sReturn;
}

//////////////////////////////////////////////////////////////////////////
//CXJEventSysAlarm ϵͳ�쳣�澯
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
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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

	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;

	//special
	pEvent->m_nEventType	= XJ_EVENT_SYSALARM;
	pEvent->m_nType			= m_nType		;
	pEvent->m_sDetailMsg	= m_sDetailMsg;
	pEvent->m_sCPU			= m_sCPU;
	pEvent->m_sZone			= m_sZone;
	pEvent->m_sDetail		= m_sDetail;

	//װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_sys_alarm
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA2033C
BOOL CXJEventSysAlarm::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sStationId = pMemSet->GetValue(1); //վID
		m_sSecID = pMemSet->GetValue(2); //PT_ID
		str = pMemSet->GetValue(3); //����
		m_nType = atoi(str);
		m_sMsg = pMemSet->GetValue(4); //�澯����
		str = pMemSet->GetValue(6);	//ʱ��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(9); //��ȷ��
		m_nIsAck = atoi(str);
		m_sDetailMsg = pMemSet->GetValue(10); //��ϸ����
		m_sCPU = pMemSet->GetValue(11); //CPU��
		m_sZone = pMemSet->GetValue(12); //��ֵ����
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventSysAlarm::LoadFromDB, ��������ʧ��");
		return FALSE;
	}
	//�����豸
	if(!BuildReleation())
	{
		CString strLog;
		strLog.Format("CXJEventAction::LoadFromDB, �������豸%s�Ĺ�����ϵʧ��", m_sSecID);
		WriteLog(strLog, XJ_LOG_LV3);
	}
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡϵͳ�澯��Ϣ,
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA2034B
BOOL CXJEventSysAlarm::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//����Ƿ�20155����
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
		//������
		m_sMsg = GetMsg();
		//����
		int nCount = sttpData.sttp_body.variant_member.value_data.size();
		CString str = "";
		str.Format("%d", nCount);
		//ѭ����֯��ϸ�������
		for(int i = 0; i < nCount; i++)
		{
			CString strTemp = "";
			//id
			strTemp.Format("%d", sttpData.sttp_body.variant_member.value_data[i].id);
			//���뵽���
			str += ",";
			str += strTemp;
			//old value
			strTemp = XJToMS(sttpData.sttp_body.variant_member.value_data[i].str_value);
			//���뵽���
			str += ",";
			str += strTemp;
			//new value
			strTemp = XJToMS(sttpData.sttp_body.variant_member.value_data[i].str_reserved);
			//���뵽���
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
		WriteLog("CXJEventSysAlarm::LoadFromSttp ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼������ݿ��е�ID
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA2036B
BOOL CXJEventSysAlarm::QueryNewEventID()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	if(m_pSec == NULL)
		return FALSE;
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	////����PT_ID,Cpu_code,type,Zone_code����id
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	//����
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

	//����ʱ��
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
		WriteLog("CXJEventSysAlarm::QueryNewEventID, ��ѯʧ��");
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
			//���ҵ�ID
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
		str.Format("CXJEventSysAlarm::QueryNewEventID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Ack()
 ���ܸ�Ҫ��ȷ���¼�,�ı����ݱ��е�ȷ���ֶ�
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA20321
BOOL CXJEventSysAlarm::Ack()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	if(m_nIsAck == 1)	//�Ѿ�ȷ�Ϲ�
		return FALSE;
	
	try
	{
		//ȥ���ݿ��޸�
		if(m_lID < 0)
		{
			//���¼�, ����֪���¼�ID,����ID��
			if(!QueryNewEventID())
			{
				//���Ҳ���ID��ȷ��
				return FALSE;
			}
		}
		//�����ݿ��޸�Is_Ack�ֶ�
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��ID,����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		m_pDataEngine->GetDBEngine()->SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");
		
		//����
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
			WriteLog("CXJEventSysAlarm::Ack, ����ʧ��");
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
			str.Format("CXJEventSysAlarm::Ack,����ʧ��,ԭ��Ϊ%s", sError);
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
		str.Format("ϵͳ�澯�¼�[%d]�ѱ�ȷ��",m_lID);
	}
	else
	{
		str.Format("ϵͳ�澯�¼�[%d]ȷ��ʱ����",m_lID);
	}
	WriteLog(str);
	return bReturn;
}

/*************************************************************
 �� �� ����GetDescribe()
 ���ܸ�Ҫ������¼���ϸ����
 �� �� ֵ: ��ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2035B
CString CXJEventSysAlarm::GetDescribe()
{
//	1����ֵ���ţ�2����ѹ�壻3����ǰһ�ζԱȻ���վֱ�����͵Ķ�ֵ�仯; 4-���׼ֵ�Աȶ�ֵ�仯; 5-��վ���ñ仯
	if(m_nType == 1)
	{
		//��ֵ����
		return GetDescribeZone();
	}
	else if(m_nType == 2)
	{
		//��ѹ��仯
		return GetDescribeSoftBoard();
	}
	else if(m_nType == 3)
	{
		//��ֵ�仯
		return GetDescribeSetting();
	}
	else if(m_nType == 4)
	{
		//���׼ֵ�Աȶ�ֵ�仯
		return GetDescribeOrder();
	}
	else if(m_nType == 5)
	{
		//��վ���ñ仯
		return GetDescribeSubStation();
	}
	return "";
}

/*************************************************************
 �� �� ����GetDescribeSubStation()
 ���ܸ�Ҫ��ȡ����վ���ñ仯��ϸ����
 �� �� ֵ: ��վ���ñ仯��ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2037A
CString CXJEventSysAlarm::GetDescribeSubStation()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("ϵͳ�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վ��:   " + m_sStationName  + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "��������վ���ñ仯��Ϣ:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����TranslateDetailMsg()
 ���ܸ�Ҫ�������¼���ϸ����������m_sDetail��
 �� �� ֵ: 
 ��    ����param1	�¼�����
		   Param2
**************************************************************/
//##ModelId=49B87BA2038D
void CXJEventSysAlarm::TranslateDetailMsg( int nType )
{
	if(nType == 1)
	{
		//��ֵ���ű仯��Ϣ
		TranslateZone();
		return;
	}
	else if(nType == 2)
	{
		//��ѹ��仯��Ϣ
		TranslateSoftBoard();
	}
	else if(m_nType == 3)
	{
		//��ֵ�仯
		TranslateSetting();
	}
	else if(m_nType == 4)
	{
		//���׼ֵ�Աȶ�ֵ�仯
		TranslateOrder();
	}
	else if(m_nType == 5)
	{
		//��վ���ñ仯
		TranslateSubStation();
	}
}

/*************************************************************
 �� �� ����TranslateSubStation()
 ���ܸ�Ҫ��������վ���ñ仯����ϸ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2039A
void CXJEventSysAlarm::TranslateSubStation()
{
	if(m_sDetail != "")
	{
		//��ʾ�ѽ�����, ���ٽ���
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//û����ϸ��Ϣ
		return;
	}

	int nCount = 0;
	int nPos = -1;
	try
	{
		//���ҵ�һ������, �õ���Ŀ��
		nPos = m_sDetailMsg.Find(',', 0);
		if(nPos == -1)
			return;
		//��Ŀ��
		nCount = atoi(m_sDetailMsg.Left(nPos));
		if(nCount < 1)
			return;
		//ѭ����ȡ������Ŀ
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";
		//����
		str.Format("(��%d��)\r\n", nCount);
		CString strData = m_sDetailMsg;
		for(int i = 0; i < nCount; i++)
		{
			strData.Delete(0, nPos1 + 1);
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���Ŀ
			CString strTemp = strData.Left(nPos1);
			str += strTemp;
			str += "\r\n";
		}
		m_sDetail = str;
	}
	catch (...)
	{
		WriteLog("������վ���ñ仯����ϸ������������", 3);
	}
}

/*************************************************************
 �� �� ����TranslateSetting()
 ���ܸ�Ҫ��������ֵ�仯����ϸ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203A9
void CXJEventSysAlarm::TranslateSetting()
{
	if(m_sDetail != "")
	{
		//��ʾ�ѽ�����, ���ٽ���
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//û����ϸ��Ϣ
		return;
	}
	if(m_pSec == NULL)
		return;

	//��ն�ֵ����
	ClearSettingConfig();
	//���붨ֵ����
	GetSettingConfig(m_pSec->m_sID, atoi(m_sCPU), atoi(m_sZone));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//���ҵ�һ������, �õ���Ŀ��
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//��Ŀ��
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//ѭ����ȡ������Ŀ
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//����
		str.Format("(��%d��)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//�õ��˶�ֵID������
			CString strName = GetSettingDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;

			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵ
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;

			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//�õ���ֵ
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
		WriteLog("������ֵ�仯����ϸ������������", 3);
	}
}

/*************************************************************
 �� �� ����GetSettingDes()
 ���ܸ�Ҫ���õ�ָ����ֵID�Ķ�ֵ����
 �� �� ֵ: ��ֵ����
 ��    ����param1	��ֵID
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
 �� �� ����TranslateZone()
 ���ܸ�Ҫ��������ֵ���ű仯��ϸ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203CB
void CXJEventSysAlarm::TranslateZone()
{
	if(m_sDetail != "")
	{
		//��ʾ�ѽ�����, ���ٽ���
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//û����ϸ��Ϣ
		return;
	}
	if(m_pSec == NULL)
		return;
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//���ҵ�һ������, �õ���Ŀ��
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//��Ŀ��
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//ѭ����ȡ������Ŀ
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//����
		str.Format("(��%d��)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			
// 			CString strName = GetSettingDes(m_pSec->m_sID, atoi(strTemp));
// 			str += strName;
// 			str += "\t";
			SYSALARM* sa = new SYSALARM;
			sa->sName = strTemp;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵ
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//�õ���ֵ
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
		WriteLog("������ֵ���仯����ϸ������������", 3);
	}
}

/*************************************************************
 �� �� ����GetDescribeSetting()
 ���ܸ�Ҫ��ȡ�ö�ֵ�仯����ϸ����
 �� �� ֵ: ��ֵ�仯��ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2037B
CString CXJEventSysAlarm::GetDescribeSetting()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("ϵͳ�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վ��:   " + m_sStationName  + "\r\n";
	str += "һ���豸��:   " + m_sDeviceName  + "\r\n";
	str += "�����豸��:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "�豸����: " + m_pSec->m_sModel + "\r\n";
	else
		str += "�豸����: δ֪\r\n";
	str += "CPU��:	" + m_sCPU + "\r\n";
	str += "��ֵ����:	" + m_sZone + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "�����Ƕ�ֵ�仯��Ϣ:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����TranslateSoftBoard()
 ���ܸ�Ҫ��������ѹ��仯����ϸ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203CC
void CXJEventSysAlarm::TranslateSoftBoard()
{
	if(m_sDetail != "")
	{
		//��ʾ�ѽ�����, ���ٽ���
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//û����ϸ��Ϣ
		return;
	}
	if(m_pSec == NULL)
		return;

	//�������
	ClearSoftBoardConfig();
	//��ȡ����
	GetSoftBoardConfig(m_pSec->m_sID, atoi(m_sCPU));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//���ҵ�һ������, �õ���Ŀ��
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//��Ŀ��
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//ѭ����ȡ������Ŀ
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";	
		//����
		str.Format("(��%d��)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//�õ��˶�ֵID������
			CString strName = GetSoftBoardDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵ
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//�õ���ֵ
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
		//�������
		ClearSoftBoardConfig();
	}
	catch (...)
	{
		WriteLog("������ѹ��仯����ϸ������������", 3);
	}
}

/*************************************************************
 �� �� ����GetSoftBoardDes()
 ���ܸ�Ҫ���õ�ָ����ѹ��ID�Ķ�ֵ����
 �� �� ֵ: ��ѹ������
 ��    ����Param1	��ѹ��ID
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
 �� �� ����GetDescribeSoftBoard()
 ���ܸ�Ҫ��ȡ����ѹ��仯����ϸ����
 �� �� ֵ: ��ѹ��仯����ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038A
CString CXJEventSysAlarm::GetDescribeSoftBoard()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("ϵͳ�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վ��:   " + m_sStationName  + "\r\n";
	str += "һ���豸��:   " + m_sDeviceName  + "\r\n";
	str += "�����豸��:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "�豸����: " + m_pSec->m_sModel + "\r\n";
	else
		str += "�豸����: δ֪\r\n";
	str += "CPU��:	" + m_sCPU + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "��������ѹ��仯��Ϣ:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����GetDescribeZone()
 ���ܸ�Ҫ��ȡ�ö�ֵ���仯����ϸ����
 �� �� ֵ: ��ֵ���仯����ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038B
CString CXJEventSysAlarm::GetDescribeZone()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("ϵͳ�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վ��:   " + m_sStationName  + "\r\n";
	str += "һ���豸��:   " + m_sDeviceName  + "\r\n";
	str += "�����豸��:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "�豸����: " + m_pSec->m_sModel + "\r\n";
	else
		str += "�豸����: δ֪\r\n";
	str += "CPU��:	" + m_sCPU + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "�����Ƕ�ֵ���仯��Ϣ:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����GetDescribeOrder()
 ���ܸ�Ҫ��ȡ�����׼ֵ�Աȶ�ֵ�仯����ϸ����
 �� �� ֵ: ��ϸ����
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA2038C
CString CXJEventSysAlarm::GetDescribeOrder()
{
	//common
	CString str,strTemp,strValue;
	int  i = 0, nCount = 0, nEventType = 0;
	PT_Signal * pSignal = NULL;
	strTemp.Format("ϵͳ�澯�¼�ID: %d ", m_lID);
	str += strTemp + "\r\n";
	
	str += "��վ��:   " + m_sStationName  + "\r\n";
	str += "һ���豸��:   " + m_sDeviceName  + "\r\n";
	str += "�����豸��:   " + m_sSecName  + "\r\n";    
	if (m_pSec != NULL)
		str += "�豸����: " + m_pSec->m_sModel + "\r\n";
	else
		str += "�豸����: δ֪\r\n";
	str += "CPU��:	" + m_sCPU + "\r\n";
	str += "��ֵ����:	" + m_sZone + "\r\n";
	
	strTemp.Format("�Ƿ���ȷ��: %s", (m_nIsAck==0) ? "δȷ��" : "��ȷ��" );
	str += strTemp + "\r\n";
	//special
	strTemp.Format("����ʱ��: %s.%03d",m_tmTime.Format("%Y-%m-%d %H:%M:%S"),m_nms);
	str += strTemp + "\r\n";
	str += "*******************************************\r\n";
	str += "���������׼ֵ�Աȶ�ֵ�仯��Ϣ:";
	TranslateDetailMsg(m_nType);
	str += m_sDetail;
	str += "*******************************************\r\n";
	return str;
}

/*************************************************************
 �� �� ����TranslateOrder()
 ���ܸ�Ҫ���������׼ֵ�Աȶ�ֵ����ϸ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA203B9
void CXJEventSysAlarm::TranslateOrder()
{
	if(m_sDetail != "")
	{
		//��ʾ�ѽ�����, ���ٽ���
		return;
	}
	m_sDetail = "";
	if(m_sDetailMsg == "")
	{
		//û����ϸ��Ϣ
		return;
	}
	if(m_pSec == NULL)
		return;

	//��ն�ֵ����
	ClearSettingConfig();
	//���붨ֵ����
	GetSettingConfig(m_pSec->m_sID, atoi(m_sCPU), atoi(m_sZone));
	
	int nCount = 0;
	int nPos = -1;
	try
	{
		//���ҵ�һ������, �õ���Ŀ��
		CString strData = m_sDetailMsg;
		nPos = strData.Find(',', 0);
		if(nPos == -1)
			return;
		//��Ŀ��
		nCount = atoi(strData.Left(nPos));
		if(nCount < 1)
			return;
		strData.Delete(0, nPos + 1);
		//ѭ����ȡ������Ŀ
		int nPos1 = nPos;
		int nPos2 = nPos;
		CString str = "";
		//����
		str.Format("(��%d��)\r\n", nCount);
		for(int i = 0; i < nCount; i++)
		{
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵID
			CString strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);

			SYSALARM* sa = new SYSALARM;
			//�õ��˶�ֵID������
			CString strName = GetSettingDes(strTemp);
			str += strName;
			str += "\t";
			sa->sName = strName;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
				return;
			//�õ���ֵ
			strTemp = strData.Left(nPos1);
			nPos2 = nPos1;
			strData.Delete(0, nPos2 + 1);
			str += strTemp;
			str += "  ->  ";
			sa->sOldValue = strTemp;
			
			//��ֵ
			nPos1 = strData.Find(',', 0);
			if(nPos1 == -1)
			{
				strTemp = strData;
			}
			else
			{
				//�õ���ֵ
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
		WriteLog("�������׼ֵ��ȶ�ֵ�仯����ϸ������������", 3);
	}
}

/*************************************************************
 �� �� ����GetMsg()
 ���ܸ�Ҫ���õ���������Ϣ
 �� �� ֵ: ��������Ϣ
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30001
CString CXJEventSysAlarm::GetMsg()
{
	CString str = "";
	if(m_nType == 1)
	{
		str = "��ֵ���ű仯";
	}
	else if(m_nType == 2)
	{
		str = "��ѹ��仯";
	}
	else if(m_nType == 3)
	{
		str = "��ǰһ�ζԱȻ���վֱ�����͵Ķ�ֵ�仯";
	}
	else if(m_nType == 4)
	{
		str = "���׼ֵ�Աȶ�ֵ�仯";
	}
	else if(m_nType == 5)
	{
		str = "��վ���ñ仯";
	}
	return str;
}

/*************************************************************
 �� �� ����GetSettingConfig()
 ���ܸ�Ҫ����ȡָ������ָ��CPU�µ����ж�ֵ����
 �� �� ֵ: 
 ��    ����param1	ָ������
		   Param2	ָ��CPU��
		   param3	ָ����ֵ����
**************************************************************/
//##ModelId=49B87BA203BC
void CXJEventSysAlarm::GetSettingConfig( CString PT_ID, int nCPU, int nZone)
{
	MYASSERT(m_pDataEngine);
	MYASSERT(m_pDataEngine->GetDBEngine());

	//������ƽ̨����DLL�����ӿ���
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//SETTING_ID
	Field field1;
	m_pDataEngine->GetDBEngine()->SetField(sql, field1, "setting_id", EX_STTP_DATA_TYPE_INT);

	Field field2;
	m_pDataEngine->GetDBEngine()->SetField(sql, field2, "name", EX_STTP_DATA_TYPE_STRING);

	CString str;
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", PT_ID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition1, str);
	
	//CPU
	Condition condition3;
	str.Format("cpu_code = %d", nCPU);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition3, str);

	//zone 2009-5-5 ȥ����ֵ���ŵ�ָ��
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
		WriteLog("CXJEventSysAlarm::GetSettingConfig, ��ѯʧ��");
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
			//������ֵ����
			PT_SETTING * pts = new PT_SETTING;
			//Ϊ�˼�,ֻ����ID��NAME
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
		str.Format("CXJEventSysAlarm::GetSettingConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
}

/*************************************************************
 �� �� ����ClearSettingConfig()
 ���ܸ�Ҫ�������ֵ����
 �� �� ֵ: 
 ��    ����param1
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
 �� �� ����GetSoftBoardConfig()
 ���ܸ�Ҫ���õ�ָ������ָ��CPU�µ���ѹ�嶨��
 �� �� ֵ: 
 ��    ����param1	����ID
		   Param2	CPU��
**************************************************************/
//##ModelId=49B87BA203DA
void CXJEventSysAlarm::GetSoftBoardConfig( CString PT_ID, int nCPU )
{
	MYASSERT(m_pDataEngine);
	MYASSERT(m_pDataEngine->GetDBEngine());
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code����SOFT_ID,NAME
		
	CString str;
		
	//�ֶ�
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

	//����
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
		WriteLog("CXJEventSysAlarm::GetSoftBoardConfig, ��ѯʧ��");
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
			//��������
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
		str.Format("CXJEventSysAlarm::GetSoftBoardConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 �� �� ����ClearSoftBoardConfig()
 ���ܸ�Ҫ�������ѹ�嶨��
 �� �� ֵ: 
 ��    ����param1
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
//CXJEventComm	����վͨѶ״̬�¼�
//##ModelId=49B87BA30030
CXJEventComm::CXJEventComm(CDataEngine* pDataEngine) : CXJEvent(pDataEngine)
{
	m_nEventType = XJ_EVENT_COMM;
	m_nStatus = 0;
}

/*************************************************************
 �� �� ����Clone()
 ���ܸ�Ҫ�������¼�����
 �� �� ֵ: �������ɵ��¼�����
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
	
	//�Ƿ������Ϣ
	pEvent->m_IsDebug = m_IsDebug;
	
	//special
	m_nEventType			= XJ_EVENT_COMM;
	pEvent->m_nStatus		= m_nStatus		;
	
	//װ��ָ��
	pEvent->m_pSec = m_pSec;
	
	return pEvent;	
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from tb_pt_station_status
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA30044
BOOL CXJEventComm::LoadFromDB( CMemSet * pMemSet )
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //�¼�ID
		m_lID = atol(str);
		m_sStationName = pMemSet->GetValue(1); //վ��
		str = pMemSet->GetValue(2); //ͨѶ״̬
		m_nStatus = atoi(str);
		str = pMemSet->GetValue(3); //ʱ��
		m_tmTime = StringToTime(str);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJEventComm::LoadFromDB, ��������ʧ��");
		return FALSE;
	}	
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP���Ķ�ȡ����վͨѶ��λ��Ϣ,
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 STTP�������ݽṹ
		   param2 ָ���Ǳ������ݵĵڼ���������
**************************************************************/
//##ModelId=49B87BA30047
BOOL CXJEventComm::LoadFromSttp( STTP_FULL_DATA& sttpData, int nIndex )
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20144)
	{
		CString str;
		str.Format("CXJEventComm::LoadFromSttp, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	
	//�ж϶�Ӧװ��ID
	m_sStationId = sttpData.sttp_body.ch_pt_id;
	if(!BuildReleation())
	{
		//����������ϵʧ��
		CString str;
		str.Format("CXJEventComm::LoadFromSttp, �Ҳ�����Ӧ����, ����IDΪ%s", m_sSecID);
		WriteLog(str);
		return FALSE;
	}
	
	//״̬
	m_nStatus = sttpData.sttp_body.nStatus;
	
	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmTime = StringToTimeSttp12(strTime);
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildReleation()
 ���ܸ�Ҫ�������¼����豸�Ĺ�ϵ,���ظ���ʵ��, ����ֻ��Ҫ�ҵ���վ����Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   �����豸����
**************************************************************/
//##ModelId=49B87BA3004B
BOOL CXJEventComm::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);

	//��վID
	m_sStationId = m_pSec->m_sStationID;
	CStationObj * pStation = (CStationObj*)m_pDataEngine->FindDevice(m_sStationId, TYPE_STATION);
	if(pStation != NULL)
	{
		//��վ����
		m_sStationName = pStation->m_sName;
		CNetObj * pNet = (CNetObj*)m_pDataEngine->FindDevice(pStation->m_sNetID, TYPE_NET);
		if(pNet != NULL)
		{
			//��������
			m_sNetName = pNet->m_sName;
		}
	}
	else
		return FALSE;
	
	return TRUE;
}

/****************************************************
Date:2011/10/18  Author:LYH
������:   ~CXJEventComm	
����ֵ:   	
���ܸ�Ҫ: 
*****************************************************/
CXJEventComm::~CXJEventComm()
{

}
