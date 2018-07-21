// FaultReport.cpp : implementation file
//

#include "FaultReport.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CFaultReport

/****************************************************
Date:2011/10/17  Author:LYH
������:   CFaultReport	
����ֵ:   	
���ܸ�Ҫ: 
����: CDataEngine * pDataEngine	
*****************************************************/
CFaultReport::CFaultReport( CDataEngine* pDataEngine ) : CXJNotify(pDataEngine)
{
	/** @brief           ���ϰ�ID*/
	m_nID = -1;
	/** @brief           �鵵����*/
	m_sName = "";
	/** @brief           һ���豸ID*/
	m_sPriDeviceID = "";
	/** @brief           һ���豸ָ��*/
	m_pPriDevice = NULL;
	/** @brief           ���ϲ��*/
	m_sFaultDistance = "";
	/** @brief           ��������*/
	m_nFaultType = 0;
	/** @brief           ��վ1ID*/
	m_sStation1ID = "";
	/** @brief           ��վ2ID*/
	m_sStation2ID = "";
	/** @brief           �¼��ȼ�*/
	m_nEventLevel = 1;
	m_nmsFaultTime = 0;

	m_pDecisionReport = NULL;
	m_bLoadDecision = TRUE;

	/** @brief           ��վ1ָ��*/
	m_pStation1 = NULL;
	/** @brief           ��վ2ָ��*/
	m_pStation2 = NULL;
	/** @brief           һ���豸ָ��*/
	m_pPriDevice = NULL;

	m_bLoadDetail = FALSE;

	m_bIsSaved = 0;

	m_listChr.RemoveAll();
	m_listSign.RemoveAll();
	m_listAlarm.RemoveAll();
	m_listPTOSC.RemoveAll();
	m_listWROSC.RemoveAll();
	m_listOSCList.RemoveAll();

	m_listSec.RemoveAll();
	/** @brief           ��ʱ�����ź�������*/
	m_listTempSign.RemoveAll();
	/** @brief           ��ʱ����������*/
	m_listTempChr.RemoveAll();
	/** @brief           ��ʱ����¼������*/
	m_listTempPTOSC.RemoveAll();
	m_pTempSec = NULL;

	m_bShowReturn = TRUE;
	m_bFilterDistance = FALSE;

	CTime tmMin(1971, 1, 1, 1, 1, 1);
	m_tmFaultTime = tmMin;
}

//##ModelId=49B87B9A005E
CFaultReport::~CFaultReport()
{
	if(m_pDecisionReport != NULL)
	{
		delete m_pDecisionReport;
		m_pDecisionReport = NULL;
	}
	//�����ϸ��Ϣ
	ClearDetail();
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ�������ݿ�������ϰ���Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݼ�ָ��
		   Param2
**************************************************************/
//##ModelId=49B87B9A010E
BOOL CFaultReport::LoadFromDB( CMemSet* pMemset )
{
	// �ж�������Ч��
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0); //ID
	m_nID = atoi(str);
	m_sName = pMemset->GetValue(1); //���ϰ�����
	m_sPriDeviceID = pMemset->GetValue(2); //һ���豸ID
	str = pMemset->GetValue(3); //����ʱ��
	m_tmFaultTime = StringToTime14(str);
	m_sFaultDistance = pMemset->GetValue(4); //���ϲ��
	str = pMemset->GetValue(5); //��������
	m_nFaultType = atoi(str);
	m_sStation1ID = pMemset->GetValue(6); //��վ1ID
	m_sStation2ID = pMemset->GetValue(7); //��վ2ID
	str = pMemset->GetValue(8); 
	m_nEventLevel = atoi(str); //�¼��ȼ�
	str = pMemset->GetValue(13);
	m_nmsFaultTime = atoi(str); //����ʱ�����ֵ
	str = pMemset->GetValue(9);
	m_bIsSaved = atoi(str);  //�Ƿ�浵
    m_sFaultReason = pMemset->GetValue(11);
	str = pMemset->GetValue(12);
	m_tmCreateTime = StringToTime14(str);
	

	if(!BuildReleation())
	{
		WriteLog("CFaultReport::LoadFromDB ��������ʧ��");
		return FALSE;
	}
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
BOOL CFaultReport::BuildReleation()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	//�ж�������Ч��
	/*
	if(m_sPriDeviceID == "")
		return FALSE;
		*/
	//��֪��һ���豸����, ֻ�ܶ�������
	//��·
	m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_LINE);
	if(m_pPriDevice == NULL)
	{
		//��ѹ��
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_TRANS);
	}
	if(m_pPriDevice == NULL)
	{
		//ĸ��
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BUS);
	}
	if(m_pPriDevice == NULL)
	{
		//����
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BREAK);
	}
	if(m_pPriDevice == NULL)
	{
		//������
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_CAP);
	}
	if(m_pPriDevice == NULL)
	{
		//�翹��
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_REACTOR);
	}
	if(m_pPriDevice == NULL)
	{
		//�����
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_GEN);
	}
	if(m_pPriDevice == NULL)
	{
		//��բ
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_SWITCH);
	}
	if(m_pPriDevice == NULL)
	{
		//���������
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_PMS);
	}
	if(m_pPriDevice == NULL)
	{
		//���
		m_pPriDevice = m_pDataEngine->FindDevice(m_sPriDeviceID, TYPE_BAY);
	}

	//���Ҳ���
//	MYASSERT_BOOL(m_pPriDevice);
	m_pStation1 = (CStationObj*)m_pDataEngine->FindDevice(m_sStation1ID, TYPE_STATION);
	m_pStation2 = (CStationObj*)m_pDataEngine->FindDevice(m_sStation2ID, TYPE_STATION);
	return TRUE;
}

/*************************************************************
 �� �� ����LoadDetail()
 ���ܸ�Ҫ��������ϱ�����ϸ��Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�Ƿ�ǿ����������
		   Param2
**************************************************************/
//##ModelId=49B87B9A011A
BOOL CFaultReport::LoadDetail( BOOL bForce /*= FALSE*/ )
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	//�ѱ�����, �Ҳ���������
	if(m_bLoadDetail && !bForce)
		return TRUE;
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
	str.Format("MAINRECID = %d", m_nID);
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
		WriteLog("CFaultReport::LoadDetail, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CFaultReport::LoadDetail, ��ȡ��%d���¼���¼", nCount);
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
			
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//�����������й�����ź͹��ϲ��==0�Ĺ���
				int nEventdef = atoi(pEvent->m_sEventDef);
				if(m_bFilterNULL)
				{
					
					if(nEventdef == 94) //�������
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				if(m_bFilterDistance)
				{
					if(nEventdef == 4) //���ϲ��
					{
						int nEventConnect = atoi(pEvent->m_sEventContent);
						if(nEventConnect == 0)
						{
							delete pEvent;
							pMemset->MoveNext();
							continue;
						}
					}
				}
				//����III���¼�����
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listChr.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
			{
				if(!m_bShowReturn)
				{
					//ȥ��������Ϣ
					if(pEvent->IsReturnEvent())
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				//����III���¼�����
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listSign.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
			{
				if(!m_bShowReturn)
				{
					//ȥ��������Ϣ
					if(pEvent->IsReturnEvent())
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				//����III���¼�����
				if(m_bFilterIII)
				{
					if(pEvent->GetEventProperty() == 2)
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
				}
				m_listAlarm.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
			{
				if(pEvent->m_pSec == NULL)
				{
					delete pEvent;
					pMemset->MoveNext();
					continue;
				}
				if(pEvent->m_pSec->m_nType == TYPE_PROTECT)
					m_listPTOSC.Add(pEvent);
				else
					m_listWROSC.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_OSCLIST)
			{
				m_listOSCList.Add(pEvent);
			}
			//���뵽�����豸
			//AddSec(pEvent->m_pSec);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CFaultReport::LoadDetail,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	m_bLoadDetail = TRUE;

	CString strLog;
	strLog.Format("CFaultReport::LoadDetail,������%d��,�ź���%d��,����¼��%d��,¼����¼��%d��", m_listChr.GetSize(), m_listSign.GetSize(), m_listPTOSC.GetSize(), m_listWROSC.GetSize());
	WriteLog(strLog);

	//����
	SortActionSign();
	//���������Ϣ
	if(m_bLoadDecision)
		LoadDecision();

	return bReturn;
}

/*************************************************************
 �� �� ����ClearDetail()
 ���ܸ�Ҫ�������ϸ��Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0128
void CFaultReport::ClearDetail()
{
	//��ո�����
	//����������
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		delete pEvent;
	}
	m_listChr.RemoveAll();
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
	//�澯�¼�
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		delete pEvent;
	}
	m_listPTOSC.RemoveAll();
	//¼����¼��
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		delete pEvent;
	}
	m_listWROSC.RemoveAll();
	//¼���б�
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		delete pEvent;
	}
	m_listOSCList.RemoveAll();

	m_listTempChr.RemoveAll();
	m_listTempSign.RemoveAll();
	m_listTempPTOSC.RemoveAll();
	m_pTempSec = NULL;

	//�����豸�б�
	m_listSec.RemoveAll();
}

/*************************************************************
 �� �� ����GetFaultType()
 ���ܸ�Ҫ���õ��������͵���������
 �� �� ֵ: void
 ��    ����param1	����������������
		   Param2
**************************************************************/
//##ModelId=49B87B9A0129
CString CFaultReport::GetFaultTypeString(int nFaultType)
{
	CString strReturn = "";
	switch(nFaultType)
	{
	case 0:
		strReturn = "";
		break;
	case 1:
		strReturn = "A��ӵ�";
		break;
	case 2:
		strReturn = "B��ӵ�";
		break;
	case 3:
		strReturn = "C��ӵ�";
		break;
	case 4:
		strReturn = "AB�����·";
		break;
	case 5:
		strReturn = "BC�����·";
		break;
	case 6:
		strReturn = "CA�����·";
		break;
	case 7:
		strReturn = "AB�����·�ӵ�";
		break;
	case 8:
		strReturn = "BC�����·�ӵ�";
		break;
	case 9:
		strReturn = "CA�����·�ӵ�";
		break;
	case 10:
		strReturn = "ABC�����·";
		break;
	case 11:
		strReturn = "ABC�����·�ӵ�";
		break;
	default:
		strReturn = "δ֪";
		break;
	}
	return strReturn;
}


BOOL CFaultReport::SaveToDB()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	//�������µ�num����¼
	//�齨��ѯ����
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//ָ��ID,����CaseName, PriDeviceID, FaultTime, FaultLocation, FaultType, STATION1_ID, STATION2_ID
	
	CString str;	
	//�ֶ�
	//CaseName
	BLOB_FIELD Field1;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field1, "CaseName", EX_STTP_DATA_TYPE_STRING, m_sName);
 
	//PriDeviceID
	BLOB_FIELD Field2;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field2, "PriDeviceID", EX_STTP_DATA_TYPE_STRING, m_sPriDeviceID);

	//FaultTime
	BLOB_FIELD Field3;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field3, "FaultTime", EX_STTP_DATA_TYPE_TIME, m_tmFaultTime.Format("%Y%m%d%H%M%S"));

	//FaultLocation
	BLOB_FIELD Field4;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field4, "FaultLocation", EX_STTP_DATA_TYPE_STRING, m_sFaultDistance);

	//FaultType
	BLOB_FIELD Field5;
	str.Format("%d", m_nFaultType);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field5, "FaultType", EX_STTP_DATA_TYPE_INT, str);

	//STATION1_ID
    BLOB_FIELD Field6;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field6, "STATION1_ID", EX_STTP_DATA_TYPE_STRING, m_sStation1ID);

	//STATION2_ID
	BLOB_FIELD Field7;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field7, "STATION2_ID", EX_STTP_DATA_TYPE_STRING, m_sStation2ID);

	//REVERSE1
	BLOB_FIELD Field9;
	str.Format("%d", m_bIsSaved);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field9, "REVERSE1", EX_STTP_DATA_TYPE_INT, str);

    //REVERSE3
    BLOB_FIELD Field11;
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field11, "REVERSE3", EX_STTP_DATA_TYPE_STRING, m_sFaultReason);


	//FaultMsTime
    BLOB_FIELD Field13;
	CString strMs;
	strMs.Format("%d", m_nmsFaultTime);
	m_pDataEngine->GetDBEngine()->SetBlobField(sql, Field13, "FaultMsTime", EX_STTP_DATA_TYPE_INT, strMs);
	
	//����
	//id
	BLOB_CONDITION condition2;
	str.Format("ID = %d", m_nID);
	m_pDataEngine->GetDBEngine()->SetBlobCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJUpdateBlogData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
		if (nResult)
		{
			WriteLog("CFaultReport::SaveToDB ���½���ɹ�\n");
		}
	}
	catch (...)
	{
		WriteLog("CFaultReport::SaveToDB, ����ʧ��");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("CFaultReport::SaveToDB,����ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;

	return bReturn;
}


/*************************************************************
 CreateDate: 13:1:2010     Author:LYH
 �� �� ��: MultiSaveEvent()
 ���ܸ�Ҫ: ���������¼�
 �� �� ֵ: ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 ��    ��: param1 
		   Param2 
**************************************************************/
int CFaultReport::MultiSaveEvent()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	if(!DeleteAllEventFromDB())
	{
		//ɾ��ʧ��
		WriteLog("CFaultReport::MultiSaveEvent, ɾ��������ʧ��");
		return 1;
	}
	//ѭ������ѡ����
	STTP_FULL_DATA sttpData;
	//��սṹ
	zero_sttp_full_data(sttpData);
	int i = 0;
	//��������ֵ
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return FALSE;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//�澯�ź���
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//¼����¼��
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//¼���б�
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}

	STTP_DBDATA_RECORD sttpDB;
	sttpDB.condition_list.clear();
	sttpDB.field_list.clear();

	//MainRecID
	STTP_DBDATA_UNIT field;
	CString str;
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "MainRecID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//����ID
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "PTID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventType
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventType";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultStartTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultStartTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultReceiveTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultReceiveTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventDef
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventDef";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventContent
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "EventContent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//SignalTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "SignalTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//IsKeyEvent
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "IsKeyEvent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);

	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nReturn = 0;
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJInsertMutiData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sttpData, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::MultiSaveEvent, ����ʧ��");
		delete[] sError;
		return 2;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		nReturn = 0;
	}
	else
		nReturn = 2;

	zero_sttp_full_data(sttpData);
	
	delete[] sError;
	if(nReturn == 0) //�ɹ�
	{
		WriteLog("CFaultReport::MultiSaveEvent, ����ɹ�");
	}
	else
	{
		WriteLog("CFaultReport::MultiSaveEvent, ����ʧ��");
	}
	return nReturn;	
}

/*************************************************************
 �� �� ����DeleteAllEventFromDB()
 ���ܸ�Ҫ�������ݿ���ɾ�������¼�
 �� �� ֵ: ɾ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
BOOL CFaultReport::DeleteAllEventFromDB()
{
	MYASSERT_BOOL(m_pDataEngine);
	MYASSERT_BOOL(m_pDataEngine->GetDBEngine());

	BOOL bReturn = TRUE;
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	Condition con1;
	CString str;
	str.Format("MainRecID=%d", m_nID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, con1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::DeleteAllEventFromDB, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����һ��STTP�����ж�ȡ���ϱ�������,(20151)
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	STTP�������ݽṹ
		   param2
**************************************************************/
BOOL CFaultReport::LoadFromSTTP( STTP_FULL_DATA& sttpData )
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20151)
	{
		CString str;
		str.Format("CFaultReport::LoadFromSTTP, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	//����һ���豸ID
	m_sPriDeviceID = sttpData.sttp_body.ch_HandlerName;
	//��վ1
	m_sStation1ID = sttpData.sttp_body.ch_station_id;
	//��վ2
	m_sStation2ID = sttpData.sttp_body.ch_pt_id;
	//��Ϣ��������ʱ��ֵ
	m_nmsFaultTime = sttpData.sttp_body.nFlag;
	//��Ϣ�����
	CString str;
	str = sttpData.sttp_body.ch_check_code;
	m_nID = atoi(str);
	//��Ϣ��������
	m_sName = XJToMS(sttpData.sttp_body.strMessage);
	//����ʱ��
	CString sTime = sttpData.sttp_body.ch_time_15_BIT1;
	m_tmFaultTime = StringToTime14(sTime);
	//���ϲ��ֵ
	m_sFaultDistance = sttpData.sttp_body.ch_version;
	//��������
	str = XJToMS(sttpData.sttp_body.strRelativePath);
	m_nFaultType = GetFaultTypeInt(str);
	//����ʱ�ĺ���ֵ
	m_nmsFaultTime = sttpData.sttp_body.nFlag;
	m_tmCreateTime = CTime::GetCurrentTime();
//	CString strTime = FindCreateTime(m_sName);
//	m_tmFaultTime = StringToTime14(strTime);

	m_bLoadDetail = FALSE;

	//�����豸
	if(!BuildReleation())
	{
		WriteLog("CFaultReport::LoadFromSTTP, �������豸ʧ��");
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����GetFaultTypeInt()
 ���ܸ�Ҫ�����������ʹ��ַ���תΪ����
 �� �� ֵ: �������ͱ�ʶ
 ��    ����param1	���������ַ���
		   Param2
**************************************************************/
int CFaultReport::GetFaultTypeInt( CString sType )
{
	// A/B/C/AB/BC/AC/ABG/ACG/BCG/ABC/ABCG
	sType.MakeUpper();
	if ((sType == "AG") || (sType == "AN") || (sType == "A"))
	{
		//"A��ӵ�";
		return 1;
	}
	if ((sType == "BG") || (sType == "BN") || (sType == "B"))
	{
		//"B��ӵ�";
		return 2;
	}
	if ((sType == "CG") || (sType == "CN") || (sType == "C"))
	{
		//"C��ӵ�";
		return 3;
	}
	if ((sType == "AB") || (sType == "BA"))
	{
		//"AB�����·";
		return 4;
	}
	if ((sType == "BC") || (sType == "CB"))
	{
		//"BC�����·";
		return 5;
	}
	if ((sType == "CA") || (sType == "AC"))
	{
		//"CA�����·";
		return 6;
	}
	if ((sType == "ABG") || (sType == "ABN") || (sType == "BAG") || (sType == "BAN"))
	{
		//"AB�����·�ӵ�";
		return 7;
	}
	if ((sType == "BCG") || (sType == "BCN") || (sType == "CBG") || (sType == "CBN"))
	{
		//"BC�����·�ӵ�";
		return 8;
	}
	if ((sType == "CAG") || (sType == "CAN") || (sType == "ACG") || (sType == "ACN"))
	{
		//"CA�����·�ӵ�";
		return 9;
	}
	if (sType == "ABC")
	{
		//"ABC�����·";
		return 10;
	}
	if ((sType == "ABCG") || (sType == "ABCN"))
	{
		//"ABC����ӵ�";
		return 11;
	}	
	else
	{
		//"δ֪����";
		return 0;
	}
	return 0;
}

/*************************************************************
 �� �� ����SortActionSign()
 ���ܸ�Ҫ���ź���ʱ��ͺ���ֵ�������ź��б�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CFaultReport::SortActionSign()
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
	//����������
	for(int i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
	//�����ź���
	for(int i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}

	//����¼��
	for(int i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
}

/*************************************************************
 �� �� ����CompareSign()
 ���ܸ�Ҫ���Ƚ����������¼��Ķ����ź�ʱʱ���С
 �� �� ֵ: 1>2����>0, 1=2����0, 1<2����<0
 ��    ����param1	�����¼�1
		   Param2	�����¼�2
**************************************************************/
int CFaultReport::CompareSign( CFaultEvent* pEvent1, CFaultEvent* pEvent2 )
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

/*************************************************************
 �� �� ����GetFaultTypeLetter()
 ���ܸ�Ҫ�����������ʹ�����תΪ��ĸ��ʽ
 �� �� ֵ: ���������ַ���
 ��    ����param1	�������ʹ���
		   Param2
**************************************************************/
//##ModelId=49B87B9A014A
CString CFaultReport::GetFaultTypeLetter( int nType )
{
	CString sType = "";
	switch(nType)
	{
	case 1:
		sType = "A"; //"A��ӵ�";
		break;
	case 2:
		sType = "B"; //"B��ӵ�";
		break;
	case 3:
		sType = "C"; //"C��ӵ�";
		break;
	case 4:
		sType = "AB"; //"AB�����·";
		break;
	case 5:
		sType = "BC"; //"BC�����·";
		break;
	case 6:
		sType = "AC"; //"CA�����·";
		break;
	case 7:
		sType = "ABG"; //"AB�����·�ӵ�";
		break;
	case 8:
		sType = "BCG"; //"BC�����·�ӵ�";
		break;
	case 9:
		sType = "ACG"; ////"CA�����·�ӵ�";
		break;
	case 10:
		sType = "ABC"; //"ABC�����·";
		break;
	case 11:
		sType = "ABCG"; //"ABC����ӵ�";
		break;
	default:
		sType = "";
		break;
	}
	return sType;
}

/*************************************************************
 �� �� ����AddSec()
 ���ܸ�Ҫ�����豸���뵽�����豸����
 �� �� ֵ: �豸ָ��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A015B
void CFaultReport::AddSec( CDeviceObj* pObj )
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

/*************************************************************
 �� �� ����BuildTempList()
 ���ܸ�Ҫ��ָ�������豸������ʱ����
 �� �� ֵ: ָ�������豸
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0168
void CFaultReport::BuildTempList( CSecObj* pSec )
{
	if(pSec == NULL)
		return;

	m_listTempChr.RemoveAll();
	m_listTempSign.RemoveAll();
	m_listTempPTOSC.RemoveAll();

	m_pTempSec = pSec;
		
	//����������
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempChr.Add(pEvent);
		}
	}

	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempSign.Add(pEvent);
		}
	}
	
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempPTOSC.Add(pEvent);
		}
	}

	CString str;
	str.Format("CFaultReport::BuildTempList,����%s,������%d��,�ź���%d��,¼��%d��", pSec->m_sName, m_listTempChr.GetSize(), m_listTempSign.GetSize(), m_listTempPTOSC.GetSize());
	WriteLog(str);
}

/****************************************************
Date:2011/10/31  Author:LYH
������:   GetNotifyType	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CFaultReport::GetNotifyType()
{
	return XJ_NOTIFY_FAULT;
}

/****************************************************
Date:2011/10/31  Author:LYH
������:   GetDescribe	
����ֵ:   CString	�¼������ַ���
���ܸ�Ҫ: ����¼�����
*****************************************************/
CString CFaultReport::GetDescribe()
{
	return "��Һ�,���ǹ��ϱ���!";
}

/****************************************************
Date:2011/11/8  Author:LYH
������:   GetSimpleDescribe	
����ֵ:   CString	�¼���Ҫ�����ַ���
���ܸ�Ҫ: ����¼���Ҫ����
*****************************************************/
CString CFaultReport::GetSimpleDescribe()
{
	CString str,strTemp;
	str = "���ϱ���:";
	strTemp.Format("[%s] ",m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S"));
	str += strTemp;
	strTemp = "";
	if(m_pPriDevice != NULL)
		strTemp = m_pPriDevice->m_sName;
	str += strTemp;
	str += " ";
	strTemp.Format("[%s]���� ", GetFaultTypeString(m_nFaultType));
	str += strTemp;

	if(m_pStation1 != NULL)
		str += "[��վ1:" + m_pStation1->m_sName+"]";
	if(m_pStation2 != NULL)
		str += "[��վ2:" + m_pStation2->m_sName+"]";

	return str;
}

/****************************************************
Date:2011/11/17  Author:LYH
������:   LoadDecision	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CFaultReport::LoadDecision()
{
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//���� MainRecID���������ֶ�
	CString str;		
	//����

	//MainRecID
	Condition condition2;
	str.Format("MainRecID = %d", m_nID);
	m_pDataEngine->GetDBEngine()->SetCondition(sql, condition2, str);

	CMemSet* pMemset;
	pMemset = new CMemSet;

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	int nResult;
	try
	{
		nResult = m_pDataEngine->GetDBEngine()->XJSelectData(EX_STTP_INFO_ANALYFAULT_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CFaultReport::LoadDecision, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString sLog;
		sLog.Format("CFaultReport::LoadDecision, ��ȡ��%d���¼���¼", nCount);
		WriteLog(sLog, XJ_LOG_LV3);
		if(nCount == 1)
		{
			if(m_pDecisionReport == NULL)
				m_pDecisionReport = new CDecisionReport(m_pDataEngine);
			m_pDecisionReport->LoadFromDB(pMemset);
		}
		else
		{
			CString sLog;
			sLog.Format("CFaultReport::LoadDecision, ��¼���࣬%d,��Ч", nCount);
			WriteLog(sLog, XJ_LOG_LV3);
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(m_pDecisionReport != NULL)
		{
			//��ĳЩ���Ը��߸�������
			m_pDecisionReport->m_sFaultDistance = m_sFaultDistance;
			m_pDecisionReport->m_sFaultPhase = GetFaultTypeString(m_nFaultType);
			if(m_pPriDevice != NULL)
				m_pDecisionReport->m_sFaultDevice = m_pPriDevice->m_sName;
		}
	}
	delete[] sError;
	delete pMemset;
	return TRUE;
}



