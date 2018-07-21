// FaultReport.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "FaultReport.h"
#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CFaultReport

IMPLEMENT_DYNCREATE(CFaultReport, CObject)

//##ModelId=49B87B9A005D
CFaultReport::CFaultReport()
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

	/** @brief           ��վ1ָ��*/
	m_pStation1 = NULL;
	/** @brief           ��վ2ָ��*/
	m_pStation2 = NULL;
	/** @brief           һ���豸ָ��*/
	m_pPriDevice = NULL;

	m_bLoadDetail = FALSE;

	m_bIsSaved = 0;

	m_nTypicalcase = 0;
	m_nOverhaul = 0;

	m_nRportType = 0;
	
	m_sFis_path = "";
	m_listChr.RemoveAll();
	m_listSign.RemoveAll();
	m_listAlarm.RemoveAll();
	m_listPTOSC.RemoveAll();
	m_listWROSC.RemoveAll();
	m_listOSCList.RemoveAll();
	m_listWaveDistance.RemoveAll();
	m_listOSCParse.RemoveAll();
	m_listDIChange.RemoveAll();

	m_listSec.RemoveAll();
	/** @brief           ��ʱ�����ź�������*/
	m_listTempSign.RemoveAll();
	/** @brief           ��ʱ����������*/
	m_listTempChr.RemoveAll();
	/** @brief           ��ʱ����¼������*/
	m_listTempPTOSC.RemoveAll();
	m_listTempDIChange.RemoveAll();
	m_pTempSec = NULL;

	m_bShowReturn = TRUE;

	CTime tmMin(1971, 1, 1, 1, 1, 1);
	m_tmFaultTime = tmMin;
}

//##ModelId=49B87B9A005E
CFaultReport::~CFaultReport()
{
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
	str = pMemset->GetValue(4); //���ϲ��
	str.TrimLeft();
	str.TrimRight();
	if( str.IsEmpty() )
	{
		m_sFaultDistance = str;
	}
	else
	{
		float f = atof(str);
		m_sFaultDistance.Format("%.2f", f);
	}
	
	str = pMemset->GetValue(5); //��������
	m_nFaultType = atoi(str);
	m_sStation1ID = pMemset->GetValue(6); //��վ1ID
	m_sStation2ID = pMemset->GetValue(7); //��վ2ID
	str = pMemset->GetValue(8); 
	m_nEventLevel = atoi(str); //�¼��ȼ�
	str = pMemset->GetValue(13);
	m_nmsFaultTime = atoi(str); //����ʱ�����ֵ
	str = pMemset->GetValue(10);
	m_bIsSaved = atoi(str);  //�Ƿ�浵
    m_sFaultReason = pMemset->GetValue(11);
	str = pMemset->GetValue(12);
	m_tmCreateTime = StringToTime14(str);
	m_sFis_path = pMemset->GetRealDataValue("fis_path");
	str = pMemset->GetRealDataValue("typicalcase");
	m_nTypicalcase = atoi(str);
	str = pMemset->GetRealDataValue("overhaul_flag");
	m_nOverhaul = atoi(str);
	

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
	//�ж�������Ч��
	/*
	if(m_sPriDeviceID == "")
		return FALSE;
		*/
	//��֪��һ���豸����, ֻ�ܶ�������
	//��·
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_LINE);
	if(m_pPriDevice == NULL)
	{
		//��ѹ��
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_TRANS);
	}
	if(m_pPriDevice == NULL)
	{
		//ĸ��
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_BUS);
	}
	if(m_pPriDevice == NULL)
	{
		//����
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_BREAK);
	}
	if(m_pPriDevice == NULL)
	{
		//������
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_CAP);
	}
	if(m_pPriDevice == NULL)
	{
		//�翹��
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_REACTOR);
	}
	if(m_pPriDevice == NULL)
	{
		//�����
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_GEN);
	}
	if(m_pPriDevice == NULL)
	{
		//��բ
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_SWITCH);
	}
	if(m_pPriDevice == NULL)
	{
		//���������
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_PMS);
	}
	if(m_pPriDevice == NULL)
	{
		//���
		m_pPriDevice = pData->FindDevice(m_sPriDeviceID, TYPE_BAY);
	}

	if(!m_pPriDevice)
	{
		return FALSE;
	}
	//���Ҳ���
//	MYASSERT_BOOL(m_pPriDevice);
	m_pStation1 = (CStationObj*)pData->FindDevice(m_sStation1ID, TYPE_STATION);
	m_pStation2 = (CStationObj*)pData->FindDevice(m_sStation2ID, TYPE_STATION);
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
	//�ѱ�����, �Ҳ���������
	if(m_bLoadDetail && !bForce)
		return TRUE;
	//���֮ǰ����Ϣ
	ClearDetail();

	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
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
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("order by PTID, FaultStartTime");
	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError, pMemset);
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
			CFaultEvent* pEvent = new CFaultEvent();
			if(!pEvent->LoadFromDB(pMemset))
			{
				delete pEvent;
				pMemset->MoveNext();
				continue;
			}
			
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//�����������й�����ź͹��ϲ��==0�Ĺ���
				if(g_FaultReportFilter == 1)
				{
					int nEventdef = atoi(pEvent->m_sEventDef);
					if(nEventdef == 94) //�������
					{
						delete pEvent;
						pMemset->MoveNext();
						continue;
					}
					if(nEventdef == 4) //���ϲ��
					{
						//2015/8/25 ���ϲ�಻����Ϊ��������ֵ��ʾ
						//delete pEvent;
						//pMemset->MoveNext();
						//continue;
						//2016-03-14 ���ǿ��Ź��ϲ�����ʾ
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
				if(g_FaultFilterIII == 1)
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
				if(g_FaultFilterIII == 1)
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
				if(g_FaultFilterIII == 1)
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
			else if(pEvent->m_nType == XJ_FAULT_EVENT_WAVEDISTANCE)
			{
				m_listWaveDistance.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_OSCPARSE)
			{
				m_listOSCParse.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
			{
				m_listDIChange.Add(pEvent);
			}
			else if(pEvent->m_nType == XJ_FAULT_EVENT_DISTANCE)
			{
				m_listDistance.Add(pEvent);
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

	//����
	SortActionSign();

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
	m_listTempChr.RemoveAll();
	m_listTempSign.RemoveAll();
	m_listTempPTOSC.RemoveAll();
	m_listTempDIChange.RemoveAll();
	m_pTempSec = NULL;

	m_pTempSecStation1 = NULL;
	m_pTempSecStation2 = NULL;
	
	m_listTempSignStation1.RemoveAll();
	m_listTempChrStation1.RemoveAll();
	m_listTempPTOSCStation1.RemoveAll();
	m_listTempDIChangeStation1.RemoveAll();
	
	m_listTempSignStation2.RemoveAll();
	m_listTempChrStation2.RemoveAll();
	m_listTempPTOSCStation2.RemoveAll();
	m_listTempDIChangeStation2.RemoveAll();

	m_bLoadDetail = FALSE;
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
	//С�����
	for(i = 0; i < m_listWaveDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWaveDistance.GetAt(i);
		delete pEvent;
	}
	m_listWaveDistance.RemoveAll();
	//¼������
	for(i = 0; i < m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCParse.GetAt(i);
		delete pEvent;
	}
	m_listOSCParse.RemoveAll();
	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		delete pEvent;
	}
	m_listDIChange.RemoveAll();
	//���ϲ��
	for(i=0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		delete pEvent;
	}
	m_listDistance.RemoveAll();

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
CString CFaultReport::GetFaultType()
{
	CString strReturn = "";
	switch(m_nFaultType)
	{
	case 0:
		strReturn = "";
		break;
	case 1:
		strReturn = StringFromID(IDS_FAULTTYPE_A);
		break;
	case 2:
		strReturn = StringFromID(IDS_FAULTTYPE_B);
		break;
	case 3:
		strReturn = StringFromID(IDS_FAULTTYPE_C);
		break;
	case 4:
		strReturn = StringFromID(IDS_FAULTTYPE_AB);
		break;
	case 5:
		strReturn = StringFromID(IDS_FAULTTYPE_BC);
		break;
	case 6:
		strReturn = StringFromID(IDS_FAULTTYPE_CA);
		break;
	case 7:
		strReturn = StringFromID(IDS_FAULTTYPE_ABN);
		break;
	case 8:
		strReturn = StringFromID(IDS_FAULTTYPE_BCN);
		break;
	case 9:
		strReturn = StringFromID(IDS_FAULTTYPE_CAN);
		break;
	case 10:
		strReturn = StringFromID(IDS_FAULTTYPE_ABC);
		break;
	case 11:
		strReturn = StringFromID(IDS_FAULTTYPE_ABCN);
		break;
	default:
		strReturn = StringFromID(IDS_FAULTTYPE_UNKNOWN);
		break;
	}
	return strReturn;
}


BOOL CFaultReport::SaveToDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	CString str;	
	//�ֶ�
	//CaseName
	BLOB_FIELD Field1;
	pApp->m_DBEngine.SetBlobField(sql, Field1, "CaseName", EX_STTP_DATA_TYPE_STRING, m_sName);
 
	//PriDeviceID
	BLOB_FIELD Field2;
	pApp->m_DBEngine.SetBlobField(sql, Field2, "PriDeviceID", EX_STTP_DATA_TYPE_STRING, m_sPriDeviceID);

	//FaultTime
	BLOB_FIELD Field3;
	pApp->m_DBEngine.SetBlobField(sql, Field3, "FaultTime", EX_STTP_DATA_TYPE_TIME, m_tmFaultTime.Format("%Y%m%d%H%M%S"));

	//FaultLocation
	BLOB_FIELD Field4;
	pApp->m_DBEngine.SetBlobField(sql, Field4, "FaultLocation", EX_STTP_DATA_TYPE_STRING, m_sFaultDistance);

	//FaultType
	BLOB_FIELD Field5;
	str.Format("%d", m_nFaultType);
	pApp->m_DBEngine.SetBlobField(sql, Field5, "FaultType", EX_STTP_DATA_TYPE_INT, str);

	//STATION1_ID
    BLOB_FIELD Field6;
	pApp->m_DBEngine.SetBlobField(sql, Field6, "STATION1_ID", EX_STTP_DATA_TYPE_STRING, m_sStation1ID);

	//STATION2_ID
	BLOB_FIELD Field7;
	pApp->m_DBEngine.SetBlobField(sql, Field7, "STATION2_ID", EX_STTP_DATA_TYPE_STRING, m_sStation2ID);

	//REVERSE1
	BLOB_FIELD Field9;
	str.Format("%d", m_bIsSaved);
	pApp->m_DBEngine.SetBlobField(sql, Field9, "REVERSE2", EX_STTP_DATA_TYPE_FLOAT, str);

    //REVERSE3
    BLOB_FIELD Field11;
	pApp->m_DBEngine.SetBlobField(sql, Field11, "REVERSE3", EX_STTP_DATA_TYPE_STRING, m_sFaultReason);


	//FaultMsTime
    BLOB_FIELD Field13;
	CString strMs;
	strMs.Format("%d", m_nmsFaultTime);
	pApp->m_DBEngine.SetBlobField(sql, Field13, "FaultMsTime", EX_STTP_DATA_TYPE_INT, strMs);
	
	//TYPICALCASE
	BLOB_FIELD Field14;
	str.Format("%d", m_nTypicalcase);
	pApp->m_DBEngine.SetBlobField(sql, Field14, "TYPICALCASE", EX_STTP_DATA_TYPE_INT, str);

	//fis_path
	BLOB_FIELD Field15;
	pApp->m_DBEngine.SetBlobField(sql, Field15, "FIS_PATH", EX_STTP_DATA_TYPE_STRING, m_sFis_path);

	//overhaul_flag
	BLOB_FIELD Field16;
	str.Format("%d", m_nOverhaul);
	pApp->m_DBEngine.SetBlobField(sql, Field16, "OVERHAUL_FLAG", EX_STTP_DATA_TYPE_INT, str);

	//����
	//id
	BLOB_CONDITION condition2;
	str.Format("ID = %d", m_nID);
	pApp->m_DBEngine.SetBlobCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateBlogData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
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

	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
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

	//С�����
	for(i = 0; i < m_listWaveDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWaveDistance.GetAt(i);
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

	//¼������
	for(i = 0; i < m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCParse.GetAt(i);
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

	//���ϲ��
	for(i = 0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
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

	//wavefileno
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "wavefileno";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//wavedescribe
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "wavedescribe";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//reverse2
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "reverse2";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//reverse3
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "reverse3";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nReturn = 0;
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertMutiData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sttpData, sError);
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
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	Condition con1;
	CString str;
	str.Format("MainRecID=%d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
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
	m_nRportType = sttpData.sttp_body.nEventType;
	//����һ���豸ID
	m_sPriDeviceID = sttpData.sttp_body.ch_HandlerName;
	//��վ1
	m_sStation1ID = sttpData.sttp_body.ch_station_id;
	//��վ2
	m_sStation2ID = sttpData.sttp_body.ch_pt_id;
	//��Ϣ��������ʱ��ֵ
	CString strNms = sttpData.sttp_body.nFlag;
	m_nmsFaultTime = atoi(strNms);
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
		}
	}

	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		for(int k = 1; k < m_listDIChange.GetSize()-i; k++)
		{
			CFaultEvent* pEvent1 = (CFaultEvent*)m_listDIChange.GetAt(k-1);
			CFaultEvent* pEvent2 = (CFaultEvent*)m_listDIChange.GetAt(k);
			if(CompareSign(pEvent1, pEvent2) > 0)
			{
				//ǰ��Ⱥ����, ��ǰ��ĺ���
				m_listDIChange.SetAt(k, pEvent1);
				m_listDIChange.SetAt(k-1, pEvent2);
			}
		}
	}

	//�������б���
	m_listSec.RemoveAll();
	//����������
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}

	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		if(pEvent != NULL)
			AddSec(pEvent->m_pSec);
	}

	m_listSecStation1.RemoveAll();
	m_listSecStation2.RemoveAll();
	//�������ֱ𱣴浽����������
	for( i = 0; i < m_listSec.GetSize(); i++)
	{
		CSecObj* pSec = (CSecObj*)m_listSec.GetAt(i);
		if(pSec == NULL)
			continue;
		if( m_pStation1 != NULL && pSec->m_sStationID == m_pStation1->m_sID )
		{
			m_listSecStation1.Add(pSec);
		}
		if( m_pStation2 != NULL && pSec->m_sStationID == m_pStation2->m_sID )
		{
			m_listSecStation2.Add(pSec);
		}
	}
	MatchSec();
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
	m_pTempSec = pSec;
		
	//����������
	int i = 0;
	m_listTempChr.RemoveAll();
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
	
	//����¼��
	m_listTempPTOSC.RemoveAll();
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempPTOSC.Add(pEvent);
		}
	}
	//��������λ
	m_listTempDIChange.RemoveAll();
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSec)
				m_listTempDIChange.Add(pEvent);
		}
	}
}

/****************************************************
Date:2012/4/5  Author:LYH
������:   DeleteFaultShortenData	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CFaultReport::DeleteFaultShortenData()
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("casename = '%s' and id = %d", m_sName, m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::DeleteFaultShortenData, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		DeleteFaultShortenEvents();
		DeleteFaultAnalyData();
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;	
}

/****************************************************
Date:2012/4/5  Author:LYH
������:   DeleteFaultShortenEvents	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CFaultReport::DeleteFaultShortenEvents()
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::DeleteFaultShortenEvents, ʧ��");
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

/****************************************************
Date:2012/4/5  Author:LYH
������:   DeleteFaultAnalyData	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CFaultReport::DeleteFaultAnalyData()
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_ANALYFAULT_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CFaultReport::DeleteFaultAnalyData, ʧ��");
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

/****************************************************
Date:2013/8/8  Author:LYH
������:   GetFisPath	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CFaultReport::GetFisPath()
{
	if(!m_sFis_path.IsEmpty())
		return m_sFis_path;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "fis_path", EX_STTP_DATA_TYPE_STRING);

	CString str = "";
	//MainRecID
	Condition condition1;
	str.Format("id = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	CMemSet pMemset;
	char sError[1024];
	memset(sError, 0, 1024);

	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		return "";
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		m_sFis_path = pMemset.GetValue((UINT)0);
	}
	return m_sFis_path;
}

CFaultReport* CFaultReport::Clone()
{
	CFaultReport* pNew = new CFaultReport();
	if(!pNew)
		return NULL;
	pNew->m_nID = m_nID ;
	pNew->m_sName = m_sName;
	pNew->m_sPriDeviceID = m_sPriDeviceID ;
	pNew->m_pPriDevice = m_pPriDevice;
	pNew->m_sFaultDistance = m_sFaultDistance;
	pNew->m_nFaultType = m_nFaultType;
	pNew->m_sStation1ID = m_sStation1ID;
	pNew->m_sStation2ID = m_sStation2ID;
	pNew->m_pStation1 = m_pStation1;
	pNew->m_pStation2 = m_pStation2;
	pNew->m_nEventLevel = m_nEventLevel;
	pNew->m_tmFaultTime = m_tmFaultTime;
	pNew->m_nmsFaultTime = m_nmsFaultTime;
	pNew->m_tmCreateTime = m_tmCreateTime;
	pNew->m_nRportType = m_nRportType;
	
	return pNew;
}

void CFaultReport::UpdateFrom( CFaultReport* pReport )
{
	if(!pReport)
		return;
	ClearDetail();
	m_nID				= pReport->m_nID ;
	m_sName				= pReport->m_sName;
	m_sPriDeviceID	 	= pReport->m_sPriDeviceID ;
	m_pPriDevice 		= pReport->m_pPriDevice;
	m_sFaultDistance 	= pReport->m_sFaultDistance;
	m_nFaultType 		= pReport->m_nFaultType;
	m_sStation1ID 		= pReport->m_sStation1ID;
	m_sStation2ID 		= pReport->m_sStation2ID;
	m_pStation1			= pReport->m_pStation1;
	m_pStation2 		= pReport->m_pStation2;
	m_nEventLevel 		= pReport->m_nEventLevel;
	m_tmFaultTime 		= pReport->m_tmFaultTime;
	m_nmsFaultTime 		= pReport->m_nmsFaultTime;
	m_tmCreateTime 		= pReport->m_tmCreateTime;
	m_nRportType 		= pReport->m_nRportType;
}

void CFaultReport::BuildTempListLandscape(CSecObj* pSecStation1, CSecObj* pSecStation2)
{
	m_pTempSecStation1 = pSecStation1;
	m_pTempSecStation2 = pSecStation2;
	
	//����������
	int i = 0;
	m_listTempChrStation1.RemoveAll();
	m_listTempChrStation2.RemoveAll();
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecStation1)
				m_listTempChrStation1.Add(pEvent);
			if(pEvent->m_pSec == pSecStation2)
				m_listTempChrStation2.Add(pEvent);
		}
	}
	//�����ź���
	m_listTempSignStation1.RemoveAll();
	m_listTempSignStation2.RemoveAll();
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecStation1)
				m_listTempSignStation1.Add(pEvent);
			if(pEvent->m_pSec == pSecStation2)
				m_listTempSignStation2.Add(pEvent);
		}
	}
	
	//����¼��
	m_listTempPTOSCStation1.RemoveAll();
	m_listTempPTOSCStation2.RemoveAll();
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecStation1)
				m_listTempPTOSCStation1.Add(pEvent);
			if(pEvent->m_pSec == pSecStation2)
				m_listTempPTOSCStation2.Add(pEvent);
		}
	}

	//��������λ
	m_listTempDIChangeStation1.RemoveAll();
	m_listTempDIChangeStation2.RemoveAll();
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecStation1)
				m_listTempDIChangeStation1.Add(pEvent);
			if(pEvent->m_pSec == pSecStation2)
				m_listTempDIChangeStation2.Add(pEvent);
		}
	}
}

void CFaultReport::MatchSec()
{
	for( int left = 0; left < m_listSecStation1.GetSize(); left++ )
	{
		int nSel = left;
		int nMax = 0;
		CSecObj* pSecLeft = (CSecObj*)m_listSecStation1.GetAt(left);
		for(int right = left; right < m_listSecStation2.GetSize(); right++)
		{
			CSecObj* pObj = (CSecObj*)m_listSecStation2.GetAt(right);
			int score = 0;
			if( !pObj->m_sModel.IsEmpty() && !pSecLeft->m_sModel.IsEmpty() && !pObj->m_sModel.CompareNoCase( pSecLeft->m_sModel))
			{
				//�ͺ���ȫһ��
				score = 10; 
			}
			else if( !pObj->m_sModel.IsEmpty() && !pSecLeft->m_sModel.IsEmpty())
			{
				//���ͺ�����ȡ��ĸ��Ϣ��һ����ĸ��Ϣһ�¿���Ϊ��ͬ����
				int start = -1;
				int end = -1;
				for( int i = 0; i < pSecLeft->m_sModel.GetLength(); i++)
				{
					char chr = pSecLeft->m_sModel.GetAt(i);
					if( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) )
					{
						//��ĸ
						if( start < 0 )
							start = i;
						end = i;
					}
					else
					{
						if( end >= 0 )
							break;
					}
				}
				CString leftFactory = pSecLeft->m_sModel.Mid(start, end - start + 1);

				start = end = -1;
				for( int k = 0; k < pObj->m_sModel.GetLength(); k++)
				{
					char chr = pObj->m_sModel.GetAt(k);
					if( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) )
					{
						//��ĸ
						if( start < 0 )
							start = k;
						end = k;
					}
					else
					{
						if( end >= 0 )
							break;
					}
				}
				CString rightFactory = pObj->m_sModel.Mid(start, end - start + 1);
				leftFactory.TrimLeft();
				leftFactory.TrimRight();
				rightFactory.TrimLeft();
				rightFactory.TrimRight();
				if( leftFactory.CompareNoCase(rightFactory) == 0 )
				{
					score = 8;
				}
			}
			else
			{
				//���ͺ��޷��жϣ����Դ����ֽ����ж�
				CString leftName = pSecLeft->m_sName;
				leftName.TrimLeft();
				leftName.TrimRight();
				CString rightName = pObj->m_sName;
				rightName.TrimLeft();
				rightName.TrimRight();
				int index = 0;
				for(int i = leftName.GetLength()-1; i>=0; i--)
				{
					index = i;
					if(leftName.GetAt(i) > 0x80 || leftName.GetAt(i) < 0)
						break;
				}
				if( !leftName.IsEmpty() && index < leftName.GetLength())
					leftName.Delete(0, index+1);
				index = 0;
				for( i = rightName.GetLength()-1; i>=0; i--)
				{
					index = i;
					if(rightName.GetAt(i) > 0x80 || rightName.GetAt(i) < 0)
						break;
				}
				if( !rightName.IsEmpty() && index < rightName.GetLength())
					rightName.Delete(0, index+1);
				if( leftName.IsEmpty() || rightName.IsEmpty() )
					break;

				leftName.TrimLeft();
				leftName.TrimRight();
				rightName.TrimLeft();
				rightName.TrimRight();

				if( leftName.CompareNoCase( rightName) == 0 )
				{
					//�ͺ���ȫһ��
					score = 10; 
				}
				else
				{
					//���ͺ�����ȡ��ĸ��Ϣ��һ����ĸ��Ϣһ�¿���Ϊ��ͬ����
					int start = -1;
					int end = -1;
					for( int i = 0; i < leftName.GetLength(); i++)
					{
						char chr = leftName.GetAt(i);
						if( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) )
						{
							//��ĸ
							if( start < 0 )
								start = i;
							end = i;
						}
						else
						{
							if( end >= 0 )
								break;
						}
					}
					CString leftFactory = leftName.Mid(start, end - start + 1);

					start = end = -1;
					for( int k = 0; k < rightName.GetLength(); k++)
					{
						char chr = rightName.GetAt(k);
						if( (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122) )
						{
							//��ĸ
							if( start < 0 )
								start = k;
							end = k;
						}
						else
						{
							if( end >= 0 )
								break;
						}
					}
					CString rightFactory = rightName.Mid(start, end - start + 1);

					leftFactory.TrimLeft();
					leftFactory.TrimRight();
					rightFactory.TrimLeft();
					rightFactory.TrimRight();
					if( leftFactory.CompareNoCase(rightFactory) == 0 )
					{
						score = 8;
					}
				}
			}

			//
			if( score > nMax )
			{
				nMax = score;
				nSel = right;
			}
		}
		//����˳��
		if( nSel != left )
		{
			CSecObj* pTemp = (CSecObj*)m_listSecStation2.GetAt(left);
			m_listSecStation2.SetAt(left, m_listSecStation2.GetAt(nSel));
			m_listSecStation2.SetAt(nSel, pTemp);
		}
	}
}

CString CFaultReport::GetPTFaultDistance(CString ptid)
{
	for( int i = 0; i < m_listDistance.GetSize(); i++ ){
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		if(pEvent->m_sSecID == ptid)
		{
			float f = atof(pEvent->m_sEventContent);
			CString str;
			str.Format("%.2f", f);
			return str;
		}
	}
	return "";
}



