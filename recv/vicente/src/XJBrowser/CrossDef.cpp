// CrossDef.cpp : implementation file
//
#include "stdafx.h"
#include "xjbrowser.h"
#include "CrossDef.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CCrossDef

IMPLEMENT_DYNCREATE(CCrossDef, CObject)

CCrossDef::CCrossDef()
{
	/** @brief           ID*/
	m_nID = -1;
	/** @brief           ������վID*/
	m_sStationID = "";
	/** @brief           ������վָ��*/
	m_pStation = NULL;
	/** @brief           ��������. 1.��ֵ 2.ģ���� 3.������ 4.��ѹ��*/
	m_nDataType = -1;
	/** @brief           Ԥ��������*/
	m_sName = "";
	/** @brief           �õ�Խ�޸澯���ͱ��,��Ӧtb_pt_cross_def�е�type_id*/
	m_nAlarmType = -1;
	/** @brief           ��ǰ״̬. 1��Ԥ����0������*/
	m_nCurStatus = -1;

	CTime tm(1971, 1, 1, 1, 1, 1);
	/** @brief           ״̬�ı�ʱ��*/
	m_tmTime = tm;
	
	/** @brief           Ԥ�����ݵ�����*/
	m_listCross.RemoveAll();
	/** @brief           �Ƿ���������ϸ��Ϣ*/
	m_bLoadDetail = FALSE;
}

CCrossDef::~CCrossDef()
{
	ClearDetail();
}

/*************************************************************
 �� �� ��: LoadFromDB()
 ���ܸ�Ҫ: �����ݿ��������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ���ݿ�����
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadFromDB( CMemSet* pMemSet )
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
		m_nID = atoi(str);
		m_sStationID = pMemSet->GetValue(1); //Ԥ����������վID
		str = pMemSet->GetValue(2); //��������
		m_nDataType = atoi(str);
		m_sName = pMemSet->GetValue(3); //Ԥ��������
		str = pMemSet->GetValue(5); //�õ�Խ�޸澯����
		m_nAlarmType = atoi(str);
		str = pMemSet->GetValue(11); //��Ԥ���㵱ǰ״ֵ̬
		m_nCurStatus = atoi(str);
		str = pMemSet->GetValue(12); //ʱ��
		m_tmTime = StringToTime(str);
	}
	catch (...)
	{
		WriteLog("CCrossDef::LoadFromDB, ��������ʧ��");
		return FALSE;
	}	
	
	//����������ϵ
	if(!BuildReleation())
	{
		WriteLog("CCrossDef::LoadFromDB, ����������ϵʧ��");
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ��: GetStatusDes()
 ���ܸ�Ҫ: ȡ��ָ��״̬������
 �� �� ֵ: ״̬����
 ��    ��: param1 ָ��״̬
		   Param2 
**************************************************************/
CString CCrossDef::GetStatusDes( int nStatus )
{
	if(nStatus == 0)
		return StringFromID(IDS_CROSSSTATUS_NORMAL);
	else if(nStatus == 1)
		return StringFromID(IDS_CROSSSTATUS_ALERT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 �� �� ��: GetDataTypeDes()
 ���ܸ�Ҫ: ȡ��������������
 �� �� ֵ: ����
 ��    ��: param1 ָ������
		   Param2 
**************************************************************/
CString CCrossDef::GetDataTypeDes( int nType )
{
	if(nType == 1)
		return StringFromID(IDS_SECPROP_SETTING);
	else if(nType == 2)
		return StringFromID(IDS_SECPROP_ANALOG);
	else if(nType == 3)
		return StringFromID(IDS_SECPROP_DIGITAL);
	else if(nType == 4)
		return StringFromID(IDS_SECPROP_SOFT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 �� �� ��: LoadDetail()
 ���ܸ�Ҫ: �����ݿ��ȡ��ϸ��Ϣ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadDetail()
{
	//�Ȳ���������
	int nSaveNo = -1;
	LoadLastSaveNo(nSaveNo);

	//���������
	ClearDetail();

	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����
	CString str;

	Condition condition1;
	str.Format("CD_BATH = %d", nSaveNo);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	Condition condition2;
	str.Format("SC_ID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CROSS_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CCrossDef::LoadDetail, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CCrossDef::LoadDetail, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CCrossPoint* pPoint = new CCrossPoint;
			if(!pPoint->LoadFromDB(pMemset))
			{
				//��ȡʧ��,ɾ��
				delete pPoint;
			}
			else
			{
				m_listCross.AddTail(pPoint);
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CCrossDef::LoadDetail,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	m_bLoadDetail = TRUE;
	
	return bReturn;
}

/*************************************************************
 �� �� ��: LoadLastSaveNo()
 ���ܸ�Ҫ: �����ݿ��ȡ��������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ��������
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadLastSaveNo( int& nSaveNo )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//�ֶ�
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "MAX(CD_BATH)", EX_STTP_DATA_TYPE_INT);

	//����
	CString str;
	Condition condition2;
	str.Format("SC_ID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CROSS_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CCrossDef::LoadLastSaveNo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CCrossDef::LoadLastSaveNo, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		CString str = pMemset->GetValue((UINT)0);
		nSaveNo = atoi(str);
	}
	else
	{
		CString str;
		str.Format("CCrossDef::LoadLastSaveNo,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: ClearDetail()
 ���ܸ�Ҫ: ���������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CCrossDef::ClearDetail()
{
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossPoint* pPoint = (CCrossPoint*)m_listCross.GetNext(pos);
		if(pPoint != NULL)
			delete pPoint;
	}
	m_listCross.RemoveAll();
}

/*************************************************************
 �� �� ��: LoadFromSTTP()
 ���ܸ�Ҫ: ��STTP�����ж�ȡ�¼���Ϣ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 STTP�������ݽṹ
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadFromSTTP( STTP_FULL_DATA& sttpData )
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20159)
	{
		CString str;
		str.Format("CCrossDef::LoadFromSTTP, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	//������վID
	m_sStationID = sttpData.sttp_body.ch_pt_id;
	//Ԥ����ID
	m_nID = sttpData.sttp_body.nFlag;
	//����������ϵ
	if(!BuildReleation())
	{
		WriteLog("CCrossDef::LoadFromSTTP, ����������ϵʧ��");
		return FALSE;
	}
	//Ԥ����״̬
	m_nCurStatus = sttpData.sttp_body.nStatus;
	
	//Ԥ������ʱ��
	CString sTime = sttpData.sttp_body.ch_time_15_BIT1;
	int nMS = 0;
	m_tmTime = StringToTimeSttp15(sTime, nMS);
	
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildReleation()
 ���ܸ�Ҫ: ����������ϵ
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CCrossDef::BuildReleation()
{
	//����������ϵ
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;
	
	//��վָ��
	m_pStation = NULL;
	m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(m_pStation == NULL)
	{	
		return FALSE;
	}
	return TRUE;
}