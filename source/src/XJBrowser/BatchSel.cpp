// BatchSel.cpp: implementation of the CBatchSel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchSel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////
/** @brief           ����������λ*/
int g_iBatchViewAsc;


CString CSel::GetCallString( int nCall )
{
	CString sReturn = "-";
	switch(nCall)
	{
	case 0:
		sReturn = StringFromID(IDS_CALLOPER_NOTCALL);
		break;
	case 1:
		sReturn = StringFromID(IDS_CALLOPER_CALL);
		break;
	case 2:
		sReturn = StringFromID(IDS_CALLOPER_NOTSUPPORT);
		break;
	case SUSPEND:
		sReturn = StringFromID(IDS_CALLOPER_SUSPEND);
		break;
	default:
		break;
	}
	return sReturn;
}

//CBatchSel
//##ModelId=49B87BB101F6
CBatchSel::CBatchSel()
{
	/** @brief           ��Ӧ�Ķ����豸*/
	m_pObj = NULL;
	m_pCpu = NULL;
	m_nCallZone = 0;
	/** @brief           �Ƿ��ٻ���ֵ*/
	m_nCallSetting = 0;
	/** @brief           �Ƿ��ٻ�������*/
	m_nCallDigital = 0;
	/** @brief           �Ƿ��ٻ���ѹ��*/
	m_nCallSoftBoard = 0;
	/** @brief           �Ƿ��ٻ�ģ����*/
	m_nCallAnalog = 0;
	m_nCallOsc = 0;
	m_nCallHistory = 0;
	/** @brief           ��ǰ��ֵ�����ٻ����,  0:����ٻ� 1���ɹ� 2��ʧ�� 3:��δ�ٻ�*/
	m_nZone = 0;
	/** @brief           ��ֵ�ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:��δ�ٻ�*/
	m_nSetting = 0;
	/** @brief           �������ٻ����,  0:����ٻ� 1���ɹ� 2��ʧ�� 3:��δ�ٻ�*/
	m_nDigital = 0;
	/** @brief           ��ѹ���ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:��δ�ٻ�*/
	m_nSoftBoard = 0;
	/** @brief           ģ�����ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:��δ�ٻ�*/
	m_nAnalog = 0;
	m_nOsc = 0;
	m_nHistory = 0;

	/** @brief           ��ֵ���Ž��˵��*/
	m_nZoneNote = 0;
	/** @brief           ��ֵ���˵��*/
	m_nSettingNote = 0;
	/** @brief           ���������˵��*/
	m_nDigitalNote = 0;
	/** @brief           ��ѹ����˵��*/
	m_nSoftBoardNote = 0;
	/** @brief           ģ�������˵��*/
	m_nAnalogNote = 0;
	m_nOscNote = 0;
	m_nHistoryNote = 0;

	m_bChecked = FALSE;

	pMap = NULL;
}


//##ModelId=49B87BB10213
CBatchSel::CBatchSel( CSecObj* pObj,CSecCPU* pCpu, BOOL bZone,  BOOL bSetting, BOOL bDigital, BOOL bSoftBoard, BOOL bAnalog )
{
	/** @brief           ��Ӧ�Ķ����豸*/
	m_pObj = pObj;
	m_pCpu = pCpu;
	m_nCallZone = bZone;
	if(pObj->m_nType == TYPE_WAVEREC)
		m_nCallZone = 2;//��֧��
	/** @brief           �Ƿ��ٻ���ֵ*/
	m_nCallSetting = bSetting;
	if(pCpu->m_nSettingDefs <= 0)
	{
		//û�е��
		m_nCallSetting = 2;//��֧��
	}
	/** @brief           �Ƿ��ٻ�������*/
	m_nCallDigital = bDigital;
	if(pCpu->m_nDigitalDefs <= 0)
	{
		//û�е��
		m_nCallDigital = 2;//��֧��
	}
	/** @brief           �Ƿ��ٻ���ѹ��*/
	m_nCallSoftBoard = bSoftBoard;
	if(pCpu->m_nSoftBoardDefs <= 0)
	{
		//û�е��
		m_nCallSoftBoard = 2;//��֧��
	}
	/** @brief           �Ƿ��ٻ�ģ����*/
	m_nCallAnalog = bAnalog;
	if(pCpu->m_nAnalogDefs <= 0)
	{
		//û�е��
		m_nCallAnalog = 2;//��֧��
	}
	
	//��ֵ���Ž��
	m_nZone = 9;
	if(m_nCallZone == 0)
	{
		//��ֵ���Ų��ٻ�
		m_nZone = 0;
	}
	else if(m_nCallZone == 2)
	{
		//װ�ò�֧��
		m_nZone = 3;
	}
	//��ֵ���
	m_nSetting = 9;
	if(m_nCallSetting == 0)
	{
		//��ֵ���ٻ�
		m_nSetting = 0;
	}
	else if(m_nCallSetting == 2)
	{
		//װ�ò�֧��
		m_nSetting = 3;
	}

	//���������
	m_nDigital = 9;
	if(m_nCallDigital == 0)
	{
		//���������ٻ�
		m_nDigital = 0;
	}
	else if(m_nCallDigital == 2)
	{
		//װ�ò�֧��
		m_nDigital = 3;
	}

	//��ѹ����
	m_nSoftBoard = 9;
	if(m_nCallSoftBoard == 0)
	{
		//��ѹ�岻�ٻ�
		m_nSoftBoard = 0;
	}
	else if(m_nCallSoftBoard == 2)
	{
		//װ�ò�֧��
		m_nSoftBoard = 3;
	}

	//ģ�������
	m_nAnalog = 9;
	if(m_nCallAnalog == 0)
	{
		//ģ�������ٻ�
		m_nAnalog = 0;
	}
	else if(m_nCallAnalog == 2)
	{
		//װ�ò�֧��
		m_nAnalog = 3;
	}

	/** @brief           ��ֵ���Ž��˵��*/
	m_nZoneNote = 0;
	/** @brief           ��ֵ���˵��*/
	m_nSettingNote = 0;
	/** @brief           ���������˵��*/
	m_nDigitalNote = 0;
	/** @brief           ��ѹ����˵��*/
	m_nSoftBoardNote = 0;
	/** @brief           ģ�������˵��*/
	m_nAnalogNote = 0;
}
//##ModelId=49B87BB10232
CBatchSel::~CBatchSel()
{
	
}

/*************************************************************
 �� �� ����SaveData()
 ���ܸ�Ҫ���������ݵ����ݿ� 
 �� �� ֵ: �������ݳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���Զ��������û��������ٻ�����.0-�Զ�����. 1-�����ٻ�
		   Param2
**************************************************************/
//##ModelId=49B87BB10251
BOOL CBatchSel::SaveData( int nIS_OC )
{
	//���������Ч��
	if(m_pObj == NULL)
		return FALSE;

	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//�����ֶ�

	//��վID
	Field field1;
	pApp->m_DBEngine.SetField(sql, field1, "Station_ID", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sStationID);

	//����ID
	Field field2;
	pApp->m_DBEngine.SetField(sql, field2, "PT_ID", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sID);

	//IsZone
	Field field9;
	CString str;
	str.Format("%d", m_nCallZone);
	pApp->m_DBEngine.SetField(sql, field9, "Reverse1", EX_STTP_DATA_TYPE_INT, str);
	//IsCallSetting
	Field field3;
	str.Format("%d", m_nCallSetting);
	pApp->m_DBEngine.SetField(sql, field3, "IsCallSetting", EX_STTP_DATA_TYPE_INT, str);

	//IsCallDi
	Field field4;
	str.Format("%d", m_nCallDigital);
	pApp->m_DBEngine.SetField(sql, field4, "IsCallDi", EX_STTP_DATA_TYPE_INT, str);

	//IsCallSoftboard
	Field field5;
	str.Format("%d", m_nCallSoftBoard);
	pApp->m_DBEngine.SetField(sql, field5, "IsCallSoftboard", EX_STTP_DATA_TYPE_INT, str);

	//IsCallAi
	Field field6;
	str.Format("%d", m_nCallAnalog);
	pApp->m_DBEngine.SetField(sql, field6, "IsCallAi", EX_STTP_DATA_TYPE_INT, str);

	//Is_Auto
	Field field7;
	str.Format("%d", nIS_OC);
	pApp->m_DBEngine.SetField(sql, field7, "Is_Auto", EX_STTP_DATA_TYPE_INT, str);

	//cpu_code
	Field field8;
	str.Format("%d", m_pCpu->code);
	pApp->m_DBEngine.SetField(sql, field8, "cpu_code", EX_STTP_DATA_TYPE_INT, str);

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_OC_CONFIG_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBatchSel::SaveData, ����ʧ��");
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

//##ModelId=49B87BB10271
BOOL CBatchSel::MultiSaveData( STTP_DBDATA_RECORD& sttpDB, int nIS_OC)
{
	try
	{
		//��վID
		STTP_DBDATA_UNIT field;
		CString str;
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_pObj->m_sStationID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//����ID
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_pObj->m_sID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallSetting
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallSetting);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallDi
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallDigital);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallSoftboard
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallSoftBoard);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//IsCallAi
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallAnalog);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//Is_Auto
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		CString strOC;
		strOC.Format("%d", nIS_OC);
		field.strValue = strOC;
		field.nDataLength = strOC.GetLength();
		sttpDB.field_list.push_back(field);

		//cpu_code
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_pCpu->code);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//IsCallZone
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallZone);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//�Ƿ��ٻ���ʷ�¼�
		field.nDataType = EX_STTP_DATA_TYPE_FLOAT;
		str.Format("%d", m_nCallHistory);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//�Ƿ��ٻ�¼��
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str.Format("%d", m_nCallOsc);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
	}
	catch (...)
	{
		WriteLog("CBatchSel::MultiSaveData, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ��: LoadFromDB()
 ���ܸ�Ҫ: �����ݿ�������Ϣ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ���ݿ�ָ��
		   Param2 
**************************************************************/
BOOL CBatchSel::LoadFromDB( CMemSet* pMemSet )
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //��վID
		CString sPTID = pMemSet->GetValue(1);//����ID

		str = pMemSet->GetValue(2); //�Ƿ��ٻ���ֵ
		m_nCallSetting = atoi(str);
		str = pMemSet->GetValue(3); //�Ƿ��ٻ�������
		m_nCallDigital = atoi(str);
		str = pMemSet->GetValue(4); //�Ƿ��ٻ���ѹ��
		m_nCallSoftBoard = atoi(str);
		str = pMemSet->GetValue(5); //�Ƿ��ٻ�ģ����
		m_nCallAnalog = atoi(str);
		str = pMemSet->GetValue(7); //CPU��
		int nCPU = atoi(str);
		str = pMemSet->GetValue(8); //�Ƿ��ٻ���ֵ����
		m_nCallZone = atoi(str);
		str = pMemSet->GetValue(9); //�Ƿ��ٻ���ʷ�¼�
		m_nCallHistory = atoi(str);
		str = pMemSet->GetValue(10); //�Ƿ��ٻ�¼��
		m_nCallOsc = atoi(str);
	
		//��ֵ���Ž��
		m_nZone = 9;
		if(m_nCallZone == 0)
		{
			//��ֵ���Ų��ٻ�
			m_nZone = 0;
		}
		else if(m_nCallZone == 2)
		{
			//װ�ò�֧��
			m_nZone = 3;
		}
		//��ֵ���
		m_nSetting = 9;
		if(m_nCallSetting == 0)
		{
			//��ֵ���ٻ�
			m_nSetting = 0;
		}
		else if(m_nCallSetting == 2)
		{
			//װ�ò�֧��
			m_nSetting = 3;
		}
		
		//���������
		m_nDigital = 9;
		if(m_nCallDigital == 0)
		{
			//���������ٻ�
			m_nDigital = 0;
		}
		else if(m_nCallDigital == 2)
		{
			//װ�ò�֧��
			m_nDigital = 3;
		}
		
		//��ѹ����
		m_nSoftBoard = 9;
		if(m_nCallSoftBoard == 0)
		{
			//��ѹ�岻�ٻ�
			m_nSoftBoard = 0;
		}
		else if(m_nCallSoftBoard == 2)
		{
			//װ�ò�֧��
			m_nSoftBoard = 3;
		}
		
		//ģ�������
		m_nAnalog = 9;
		if(m_nCallAnalog == 0)
		{
			//ģ�������ٻ�
			m_nAnalog = 0;
		}
		else if(m_nCallAnalog == 2)
		{
			//װ�ò�֧��
			m_nAnalog = 3;
		}
		
		//���ұ���
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		m_pObj = (CSecObj*)pData->FindDevice(sPTID, TYPE_SEC);
		if(m_pObj == NULL)
			return FALSE;
		m_pCpu = m_pObj->FindCpu(nCPU);
		if(m_pCpu == NULL)
			return FALSE;
		
	}
	catch (...)
	{
		WriteLog("CBatchSel::LoadFromDB, ��������ʧ��");
		return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////


BatchSel::BatchSel()
{
	/** @brief           �Ƿ��ٻ���ֵ����.0-���ٻ�,1-�ٻ�,2-��֧��*/
	m_nCallZone = 0;
	/** @brief           �Ƿ��ٻ���ֵ.0-���ٻ�,1-�ٻ�,2-��֧��*/
	m_nCallSetting = 0;
	/** @brief           �Ƿ��ٻ�������.0-���ٻ�,1-�ٻ�,2-��֧��*/
	m_nCallDigital = 0;
	/** @brief           �Ƿ��ٻ���ѹ��.0-���ٻ�,1-�ٻ�,2-��֧��*/
	m_nCallSoftBoard = 0;
	/** @brief           �Ƿ��ٻ�ģ����.0-���ٻ�,1-�ٻ�,2-��֧��*/
	m_nCallAnalog = 0;	
	m_nCallOsc = 0;
	m_nCallHistory = 0;

	m_bChecked = FALSE;

	pMap = NULL;
}


