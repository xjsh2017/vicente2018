// CrossPoint.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CrossPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrossPoint

IMPLEMENT_DYNCREATE(CCrossPoint, CObject)

CCrossPoint::CCrossPoint()
{
	/** @brief           ID(���)*/
	m_nID = -1;
	/** @brief           ����Ԥ�����ʽ��ID*/
	m_nCrossID = -1;
	/** @brief           ��������. 1.��ֵ 2.ģ���� 3.������ 4.��ѹ��*/
	m_nDataType = -1;
	/** @brief           ������վID*/
	m_sStationID = "";
	/** @brief           ������վָ��*/
	m_pStation = NULL;
	/** @brief           ���������豸ID*/
	m_sDevID = "";
	/** @brief           ���������豸ָ��*/
	m_pSec = NULL;
	/** @brief           ����CPU*/
	m_nCPU = -1;
	/** @brief           ���ݵ㶨���ID*/
	m_nDataPointID = -1;
	/** @brief           ���ݵ��ֵ*/
	m_sDataPointValue = "";
	/** @brief           ������ֵ*/
	m_sSectionValue = "";

	CTime tm(1971, 1, 1, 1, 1, 1);
	/** @brief           ״̬�ı�ʱ��*/
	m_tmTime = tm;
	/** @brief           1��Ԥ����0������*/
	m_nCrossType = -1;
	/** @brief           ���κ�*/
	m_nSaveNo = -1;

	m_sDataPointName = "";
}


CCrossPoint::~CCrossPoint()
{
}

/*************************************************************
 �� �� ��: LoadFromDB()
 ���ܸ�Ҫ: �����ݿ��������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ���ݿ�����
		   Param2 
**************************************************************/
BOOL CCrossPoint::LoadFromDB( CMemSet* pMemSet )
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
		str = pMemSet->GetValue(1); //Ԥ����id
		m_nCrossID = atoi(str);
		str = pMemSet->GetValue(2); //��������
		m_nDataType = atoi(str);
		m_sStationID = pMemSet->GetValue(3); //Ԥ����������վID
		m_sDevID = pMemSet->GetValue(4); //���ݵ����������豸ID��
		str = pMemSet->GetValue(5); //���ݵ������豸CPU��
		m_nCPU = atoi(str);
		str = pMemSet->GetValue(7); //���ݵ㶨��ID��
		m_nDataPointID = atoi(str);
		m_sDataPointValue = pMemSet->GetValue(8); //���ݵ��ֵ
		m_sSectionValue = pMemSet->GetValue(9); //Ԥ��ֵ
		str = pMemSet->GetValue(10); //ʱ��
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(11); //Ԥ����������
		m_nCrossType = atoi(str);
	}
	catch (...)
	{
		WriteLog("CCrossPoint::LoadFromDB, ��������ʧ��");
		return FALSE;
	}	
	
	//����������ϵ
	if(!BuildReleation(TYPE_SEC))
	{
		WriteLog("CCrossPoint::LoadFromDB, �������豸�Ĺ�����ϵʧ��");
		return FALSE;
	}
	QueryDataPointName();
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildReleation()
 ���ܸ�Ҫ: �������豸�Ĺ���
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �豸����
		   Param2 
**************************************************************/
BOOL CCrossPoint::BuildReleation( int nType )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//��վָ��
	m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(m_pStation == NULL)
		return FALSE;
	//�Ȳ��Ҷ����豸ָ��
	m_pSec = (CSecObj*)pData->FindDevice(m_sDevID, nType);
	if(m_pSec == NULL)
		return FALSE;
	
	return TRUE;
}

/*************************************************************
 �� �� ��: GetStatusDes()
 ���ܸ�Ҫ: ȡ��Ԥ����ָ��״̬������
 �� �� ֵ: ״̬����
 ��    ��: param1 ָ��״̬
		   Param2 
**************************************************************/
CString CCrossPoint::GetStatusDes( int nType )
{
	if(nType == 0)
		return StringFromID(IDS_CROSSSTATUS_RETURN);
	else if(nType == 1)
		return StringFromID(IDS_CROSSSTATUS_ALERT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 �� �� ��: QueryDataPointName()
 ���ܸ�Ҫ: ��ѯ���ݵ�����
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CCrossPoint::QueryDataPointName()
{
	if(m_nDataType == 1)
	{
		//��ֵ
		QuerySettingDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 2)
	{
		//ģ����
		QueryAIDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 3)
	{
		//������
		QueryDIDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 4)
	{
		//��ѹ��
		QuerySoftBoardDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: QueryAIDataPointName()
 ���ܸ�Ҫ: ��ѯָ��ģ�����������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �����豸ID
		   Param2 ģ����ID
		   Param3 CPU
		   Param4 ģ��������
**************************************************************/
BOOL CCrossPoint::QueryAIDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
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
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("AI_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QueryAIDataPointName, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}

	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QueryAIDataPointName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 �� �� ��: QueryDIDataPointName()
 ���ܸ�Ҫ: ��ѯָ��ģ���ص������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �����豸ID
		   Param2 ������ID
		   Param3 CPU
		   Param4 ����������
**************************************************************/
BOOL CCrossPoint::QueryDIDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
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
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("DI_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QueryDIDataPointName, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QueryDIDataPointName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ��: QuerySettingDataPointName()
 ���ܸ�Ҫ: ��ѯָ��ģ���ص������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �����豸ID
		   Param2 ��ֵID
		   Param3 CPU
		   Param4 ��ֵ����
**************************************************************/
BOOL CCrossPoint::QuerySettingDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
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
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("SETTING_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QuerySettingDataPointName, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QuerySettingDataPointName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ��: QuerySoftBoardDataPointName()
 ���ܸ�Ҫ: ��ѯָ����ѹ��������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �����豸ID
		   Param2 ��ѹ��ID
		   Param3 CPU
		   Param4 ��ѹ������
**************************************************************/
BOOL CCrossPoint::QuerySoftBoardDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
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
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("SOFT_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QuerySoftBoardDataPointName, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QuerySoftBoardDataPointName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}