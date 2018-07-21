/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   17:37
	filename: 	F:\vicente\Code\src\XJBrowser\DeviceObj.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	DeviceObj
	file ext:	cpp
	author:		LYH
	
	purpose:	���豸����ϸ
*********************************************************************/
// DeviceObj.cpp : implementation file
//

#define SECCONFIG_CLEARTIME		3600	//���ù���ʱ��(��)

#include "stdafx.h"
#include "xjbrowser.h"
#include "DeviceObj.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceObj
UINT StationLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CStationObj* pStation = (CStationObj*)pParam;
	pStation->BuildData();
	return 0;
}
IMPLEMENT_DYNCREATE(CDeviceObj, CObject)

//##ModelId=49B87BA50149
CDeviceObj::CDeviceObj()
{
	m_sID		= "";	
	m_sName		= "";
	m_nType		= -1;	
	m_sType		= "";
	m_nVoltage	= 0;
	m_n103Group = -1;
	m_n103Item	= -1;
	m_sStationID = "";
	m_nRunStatu = 0;
	m_nCommStatus = 0;
	m_nCode		= -1;

	m_sIPA = "";
	m_sIPB = "";
	m_nPortA = 0;
	m_nPortB = 0;

	m_htItem = NULL;
	m_Tag = NULL;
	m_Tag0 = NULL;
	m_Tag1 =NULL;
	m_Tag2 = NULL;
	m_bHasSecDevice = FALSE;
}

//##ModelId=49B87BA50271
CDeviceObj::~CDeviceObj()
{
}

/*************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ,������ʵ��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA50204
BOOL CDeviceObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��

	//����ʵ��
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�, ������ʵ��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA50242
BOOL CDeviceObj::Save()
{
	//����ʵ��
	return TRUE;
}
//CDeviceObj end
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//CNetObj ����
IMPLEMENT_DYNCREATE(CNetObj, CDeviceObj)
//##ModelId=49B87BA502DE
CNetObj::CNetObj()
{
	m_nType		= TYPE_NET;	
	m_sType		= StringFromID(IDS_MODEL_AREA);

	m_sNote		= "";

	m_nStationAbnormity = 0;
	m_nPTCommAbnormity = 0;
	m_nPTRunAbnormity = 0;
}

//##ModelId=49B87BA5030D
CNetObj::~CNetObj()
{

}

/*************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA5035C
BOOL CNetObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sName = pMemSet->GetValue(1); //����
	m_sNote = pMemSet->GetValue(2); //��ע
	if(g_role == MODE_SEP)
		m_sLocalID = pMemSet->GetValue(4);//��վID
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA50399
BOOL CNetObj::Save()
{
	return TRUE;
}

/*************************************************************
 �� �� ����GetCommAbnormityNum()
 ���ܸ�Ҫ���õ������¸����쳣������,������վͨ��״̬�쳣, ����ͨ��״̬�쳣, ��������״̬�쳣
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA503C8
void CNetObj::GetCommAbnormityNum()
{
	//�������г�վ,�ҵ��������ĳ�վ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	pData->GetStationList(listDevice, m_sID);
	//�����������
	m_nStationAbnormity = 0;
	m_nPTCommAbnormity = 0;
	m_nPTRunAbnormity = 0;

	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if(pObj->m_sNetID != m_sID)
			continue;
		if(!pObj->IsCommStatusON())
		{
			//�Ͽ�
			m_nStationAbnormity++;
		}
		//ͨ��״̬�쳣����
		int nPTComm = pObj->GetCommAbnormityNum();
		m_nPTCommAbnormity += nPTComm;
		//����״̬�쳣����
		int nPTRun = pObj->GetRunAbnormityNum();
		m_nPTRunAbnormity += nPTRun;
	}
}

//CNetObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CStationObj ��վ
IMPLEMENT_DYNCREATE(CStationObj, CDeviceObj)

//##ModelId=49B87BA600AC
CStationObj::CStationObj()
{
	m_nType = TYPE_STATION;
	m_sType = StringFromID(IDS_MODEL_SUBSTATION);

	m_sNetID = "";
	m_nProtocolId = -1;
	m_sProtocol = "";
	m_nCommMode	= -1;

	m_sSvgPath = "";
	m_sManufacturer = "";

	m_sNewStatusReason = "";
	m_sDurativeTime = "";

	m_nStationType = 0;

	/** @brief           ͨ��0״̬*/
	m_nCommStatus = -1;
	/** @brief           ͨ��1״̬*/
	m_nCommStatus1 = -1;
	/** @brief           ͨ��1���һ��״̬��λ��ԭ��*/
	m_sNewStatusReason1 = "";
	/** @brief           ͨ��1����ʱ��*/
	m_sDurativeTime1 = "";
	/** @brief           ͨ��1����ʱ����ַ���ɵ�����*/
	m_lDurative1 = 0;

	m_sIPReserve = "";
	m_sIP = "";
	m_nIPFlag = 0;
	m_sStationModel = "";//��վ�ͺ�
	m_sStationVer = "";
	m_arrCommAbnormity.RemoveAll();
	m_arrRunAbnormity.RemoveAll();

	m_arrVoltage.RemoveAll();

	m_nShowInGeo = 0;
	m_ptCenter.x = -1;
	m_ptCenter.y = -1;
	m_bSelected = FALSE;
	m_nChannel = 0;
	m_nOrderStrategy = -1;
	m_nMaintaindepartment = -1;
	m_sMaintaindepartment = "";
	m_nManagedepartment = -1;
	m_sManagedepartment = "";

	m_pLock = NULL;

	m_IsDistanceVirtual = FALSE;
}

//##ModelId=49B87BA600DA
CStationObj::~CStationObj()
{
	if(m_pLock != NULL)
	{
		delete m_pLock;
		m_pLock = NULL;
	}
	Clear();
	
}

/*************************************************************
 �� �� ����AddDevice()
 ���ܸ�Ҫ�������豸����������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   �豸����
           Param2   �豸ָ��
**************************************************************/
//##ModelId=49B87BA602AF
BOOL CStationObj::AddDevice(int nType, CDeviceObj* pObj)
{
	if(pObj == NULL)
		return FALSE;
	CSecObj* pSec = NULL;
	//ͳ�Ƶ�ѹ�ȼ�
	if(pObj->m_nType == TYPE_TRANS)
	{
		CTransObj* pTrans = (CTransObj*)pObj;
		StatVoltage(pTrans->m_nVoltageH);
	}
	else
		StatVoltage(pObj->m_nVoltage);

	switch(nType)
	{
	case TYPE_BAY: //���
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_BayMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_BayMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_BREAK: //����
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_BreakMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_BreakMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_BUS: //ĸ��
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_BusMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_BusMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_CAP: //������
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_GenMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_CapMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_GEN: //�����
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_GenMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_GenMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_LINE: //��·
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_LineMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_LineMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_PMS: //���������
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_PmsMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_PmsMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_REACTOR: //�翹��
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_ReactorMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_ReactorMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_SWITCH: //��բ
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_SwitchMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_SwitchMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_TRANS: //��ѹ��
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_TransMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;
			m_TransMap.SetAt(pObj->m_sID, pObj);
		}
		break;

	case TYPE_SEC: //���������豸
	case TYPE_PROTECT: //���� 
	case TYPE_WAVEREC: //¼����
		{
			CString sKey = pObj->m_sID;
			CDeviceObj* pFind = NULL;
			m_SecMap.Lookup(sKey, (CObject*&)pFind );
			if(pFind)
				return FALSE;

			pSec = (CSecObj*)pObj;
			pSec->m_pOwner = FindDeviceNoLock(pSec->m_sOwnerID, pSec->m_nOwnerType);
			if(pSec->m_pOwner != NULL)
				pSec->m_pOwner->m_bHasSecDevice = TRUE;
			m_SecMap.SetAt(pObj->m_sID, pObj);
			m_UriMap.SetAt(pObj->m_sUri, (CSecObj*)pObj);
		}
		break;

	default:
		break;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA60251
BOOL CStationObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //��վID
	m_sName = pMemSet->GetValue(int(2)); //����
	m_nStationType = atoi(pMemSet->GetValue(int(4)));//type
	m_sIP = pMemSet->GetValue(6);//IP
	m_sPort = pMemSet->GetValue(7);//port
	CString str;
	str = pMemSet->GetValue(int(8)); //ͨ�Ź�Լ
	m_nProtocolId = atoi(str);
	str = pMemSet->GetValue(int(9)); //ͨ�ŷ�ʽ
	m_nCommMode = atoi(str);
	m_sTel = pMemSet->GetValue(10); //telphone
	m_sNetID = pMemSet->GetValue(int(12)); //��������ID
	str = pMemSet->GetValue((int)16);
	GetGeoPosition(str);
	m_sIPReserve = pMemSet->GetValue(17);
	str = pMemSet->GetValue((int)19); //IPFalg
	m_nIPFlag = atoi(str);
	m_sSvgPath = pMemSet->GetValue(20); //SVG�ļ�·��
	m_sManufacturer = pMemSet->GetValue(21);
	str = pMemSet->GetRealDataValue("orderstrategy");
	m_nOrderStrategy = atoi(str); //��������
	str = pMemSet->GetRealDataValue("managedepartment");
	m_nManagedepartment = atoi(str); //�������ܵ�λ
	str = pMemSet->GetRealDataValue("maintaindepartment");
	m_nMaintaindepartment = atoi(str); //��ά��λ
	str = pMemSet->GetRealDataValue("servicedepartment");
	m_nServiceDepartment = atoi(str); //��ά��λ
	//ͨ��0
	str = pMemSet->GetRealDataValue("ch0_commu_status");
	m_nCommStatus = atoi(str);
	str = pMemSet->GetRealDataValue("ch0_commustatus_time");
	m_tmNewStatus = StringToTime( str);
	str = pMemSet->GetRealDataValue("ch0_commu_reason");
	m_sNewStatusReason = GetReason(atoi(str));

	//ͨ��1
	str = pMemSet->GetRealDataValue("ch1_commu_status");
	m_nCommStatus1 = atoi(str);
	str = pMemSet->GetRealDataValue("ch1_commustatus_time");
	m_tmNewStatus1 = StringToTime( str);
	str = pMemSet->GetRealDataValue("ch1_commu_reason");
	m_sNewStatusReason1 = GetReason(atoi(str));

	RefreshDurativeTime();


	return TRUE;
}

/*************************************************************
 �� �� ����Clear()
 ���ܸ�Ҫ�������վ�������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA602DE
BOOL CStationObj::Clear()
{
	m_arrCommAbnormity.RemoveAll();
	m_arrRunAbnormity.RemoveAll();
	m_arrVoltage.RemoveAll();

	POSITION pos;
	CDeviceObj* pObj = NULL;
	CString sID;

	//�����豸
	for(pos = m_SecMap.GetStartPosition(); pos != NULL; )
	{
		m_SecMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_SecMap.RemoveAll();
	m_UriMap.RemoveAll();

	//����
	for(pos = m_BreakMap.GetStartPosition(); pos != NULL; )
	{
		m_BreakMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BreakMap.RemoveAll();
	
	//ĸ��
	for(pos = m_BusMap.GetStartPosition(); pos != NULL; )
	{
		m_BusMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BusMap.RemoveAll();
	
	//������
	for(pos = m_CapMap.GetStartPosition(); pos != NULL; )
	{
		m_CapMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_CapMap.RemoveAll();
	
	//�����
	for(pos = m_GenMap.GetStartPosition(); pos != NULL; )
	{
		m_GenMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_GenMap.RemoveAll();
	
	//��· 
	for(pos = m_LineMap.GetStartPosition(); pos != NULL; )
	{
		m_LineMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_LineMap.RemoveAll();

	//���������
	for(pos = m_PmsMap.GetStartPosition(); pos != NULL; )
	{
		m_PmsMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_PmsMap.RemoveAll();

	//�翹��
	for(pos = m_ReactorMap.GetStartPosition(); pos != NULL; )
	{
		m_ReactorMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_ReactorMap.RemoveAll();
	
	//��բ
	for(pos = m_SwitchMap.GetStartPosition(); pos != NULL; )
	{
		m_SwitchMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_SwitchMap.RemoveAll();
		
	//��ѹ��
	for(pos = m_TransMap.GetStartPosition(); pos != NULL; )
	{
		m_TransMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_TransMap.RemoveAll();

	//���
	for(pos = m_BayMap.GetStartPosition(); pos != NULL; )
	{
		m_BayMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BayMap.RemoveAll();

	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA60280
BOOL CStationObj::Save()
{
	BOOL bReturn = TRUE;
	//����ͼ����Ϣ
	try
	{
		//�����ݿ��޸�reverse1�ֶ�
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//ָ��station_id,����reverse1
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		str.Format("%d:%d,%d", m_nShowInGeo, m_ptCenter.x, m_ptCenter.y);
		pApp->m_DBEngine.SetField(sql, Field1, "reverse1", EX_STTP_DATA_TYPE_STRING, str);
		
		//����
		//id
		Condition condition2;
		str.Format("station_id = '%s'", m_sID);
		pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_STATION_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CStationObj::Save, ����ʧ��");
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
			str.Format("CStationObj::Save,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
		WriteLog("CStationObj::Save, ����ʧ��");
	}
	return TRUE;
}

//##ModelId=49B87BA6030D
CDeviceObj* CStationObj::FindDevice(CString sID, int nType)
{
	if(m_pLock != NULL)
	{
		CLockUp lockup(m_pLock);
	}
	
	return FindDeviceNoLock(sID, nType);
}

/*************************************************************
 �� �� ����FindDeviceBy103()
 ���ܸ�Ҫ������103�����Ŀ�Ų����豸
 �� �� ֵ: �豸ָ��
 ��    ����param1	103�����Ŀ��.��ʽ:���#��Ŀ��
		   Param2	�豸����
**************************************************************/
//##ModelId=49B87BA6035B
CDeviceObj * CStationObj::FindDeviceBy103( CString str103, int nType )
{
	//�жϴ��������Ƿ�103��ź���Ŀ��
	int nFind = str103.Find('#', 0);
	if(nFind == -1)
	{
		return FindDevice(str103, nType);
	}
	
	//�õ�103��ź���Ŀ��
	CString strGroup = str103.Left(nFind);
	CString strItem = str103.Right(str103.GetLength() - nFind - 1);

	CDeviceObj * pReturn = NULL;
	POSITION pos;
	CDeviceObj * pObj;
	CString strID;
	switch(nType)
	{
	case TYPE_BAY: //���
		for(pos = m_BayMap.GetStartPosition(); pos != NULL;)
		{
			m_BayMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_BREAK: //����
		for(pos = m_BreakMap.GetStartPosition(); pos != NULL;)
		{
			m_BreakMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_BUS: //ĸ��
		for(pos = m_BusMap.GetStartPosition(); pos != NULL;)
		{
			m_BusMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_CAP: //������
		for(pos = m_CapMap.GetStartPosition(); pos != NULL;)
		{
			m_CapMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_GEN: //�����
		for(pos = m_GenMap.GetStartPosition(); pos != NULL;)
		{
			m_BayMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_LINE: //��· 
		for(pos = m_LineMap.GetStartPosition(); pos != NULL;)
		{
			m_LineMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_PMS: //���������
		for(pos = m_PmsMap.GetStartPosition(); pos != NULL;)
		{
			m_PmsMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_REACTOR: //�翹��
		for(pos = m_ReactorMap.GetStartPosition(); pos != NULL;)
		{
			m_ReactorMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_SWITCH: //��բ
		for(pos = m_SwitchMap.GetStartPosition(); pos != NULL;)
		{
			m_SwitchMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_TRANS: //��ѹ��
		for(pos = m_TransMap.GetStartPosition(); pos != NULL;)
		{
			m_TransMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_SEC: //���������豸	
	case TYPE_PROTECT: //���� 		
	case TYPE_WAVEREC: //¼����
		for(pos = m_SecMap.GetStartPosition(); pos != NULL;)
		{
			m_SecMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
	default:
		break;
	}

	return pReturn;
}


/*
 *  @brief   	FindDeviceBy103	 ����uri�����豸 
 *  @param 		[In]a param of Type  CString uri
 *  @return 	CDeviceObj * �豸ָ��
 */
 CDeviceObj * CStationObj::FindDeviceByUri(CString strUri)
{		
	 CDeviceObj * pObj = NULL;
	 m_UriMap.Lookup(strUri, (CObject*&)pObj);
	 return pObj;
}



/*************************************************************
 �� �� ����LoadStatusFromDB()
 ���ܸ�Ҫ����Ӳ�����ݿ������վ����վͨѶ״̬,ֻ��ѯ����һ��
 �� �� ֵ: void ״̬
 ��    ����param1	ͨ��
**************************************************************/
//##ModelId=49B87BA6038A
void CStationObj::LoadStatusFromDB(int nChannel)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)

	//�����ֶ�
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "Reserve2", EX_STTP_DATA_TYPE_STRING);
	//��������
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);

	Condition Condition2;
	str.Format("Reserve1 = %d", nChannel);
	pApp->m_DBEngine.SetCondition(sql, Condition2, str);

	Condition Condition1;
//	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by reserve2 DESC, ID DESC", EX_STTP_WHERE_ABNORMALITY);
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet pMemset;
//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CStationObj::LoadStatusFromDB, ��ѯʧ��");
		delete[] sError;
//		delete pMemset;
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount == 1)
		{
			//ֻӦ����һ����¼
			if(nChannel == 0) //0��ͨ��
			{
				CString str = pMemset.GetValue((UINT)0); //status
				m_nCommStatus = atoi(str);
				str = pMemset.GetValue((UINT)1); //time
				m_tmNewStatus = StringToTime(str);
				str = pMemset.GetValue((UINT)2); //reason
				int nReason = atoi(str);
				m_sNewStatusReason = GetReason(nReason);
			}
			else if(nChannel == 1) //1��ͨ��
			{	
				CString str = pMemset.GetValue((UINT)0); //status
				m_nCommStatus1 = atoi(str);
				str = pMemset.GetValue((UINT)1); //time
				m_tmNewStatus1 = StringToTime(str);
				str = pMemset.GetValue((UINT)2); //reason
				int nReason = atoi(str);
				m_sNewStatusReason1 = GetReason(nReason);	
			}
			//ˢ�³���ʱ��
			RefreshDurativeTime();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::LoadStatusFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
//	delete pMemset;
	sError = NULL;
//	pMemset = NULL;
}

/*************************************************************
 �� �� ����RefreshDurativeTime()
 ���ܸ�Ҫ��ˢ�µ�ǰ״̬����ʱ��, ϵͳ��ǰʱ����״̬��λʱ��Ĳ�ֵ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA603A9
void CStationObj::RefreshDurativeTime()
{
	//ȡ�õ�ǰʱ��
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewStatus;
	
	if(tmSpan < 0)
	{
		tmSpan = 0;
		CString str;
		str.Format("��վ[%s] 0��ͨ��ʱ��ˢ�³��ָ�ֵ,ʱ��:%s", m_sID, m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}
	
	//0��ͨ��
	m_sDurativeTime = tmSpan.Format(GetDurationFormatStr());
	CString str = tmSpan.Format("%D%H%M");
	m_lDurative = atol(str);
	
	tmSpan = t - m_tmNewStatus1;
	
	if(tmSpan < 0)
	{
		tmSpan = 0;
		CString str;
		str.Format("��վ[%s] 1��ͨ��ʱ��ˢ�³��ָ�ֵ, ʱ��:%s", m_sID, m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}
	
	//1��ͨ��
	if(m_nCommStatus1 != 0 && m_nCommStatus1 != 1)
	{
		//��ͨ����Ч
		m_sDurativeTime1 = "";
		m_lDurative1 = 0;
	}
	else
	{
		m_sDurativeTime1 = tmSpan.Format(GetDurationFormatStr());
		str = tmSpan.Format("%D%H%M");
		m_lDurative1 = atol(str);
	}
}

/*************************************************************
 �� �� ����LoadStatusFromSTTP()
 ���ܸ�Ҫ���ӱ����ж�ȡ����վͨѶ״̬
 �� �� ֵ: void
 ��    ����param1	��������
**************************************************************/
//##ModelId=49B87BA603B9
void CStationObj::LoadStatusFromSTTP( STTP_FULL_DATA* pSttpData )
{
	MYASSERT(pSttpData);
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20144)
	{
		return;
	}
	
	//�õ���Ӧװ��
	CString strID = pSttpData->sttp_body.ch_station_id;
	//�ж��Ƿ񱾳�վ�ı���
	if(strID != m_sID)
	{
		return;
	}

	//ͨ��
	int nChannel = pSttpData->sttp_body.nFlag;
	
	//ԭ��
	int nReason = pSttpData->sttp_body.nEventType;
	CString  strLog;
	strLog.Format("�յ����Ķ���IDΪ%d", nReason);
	WriteLog(strLog);
	//ʱ��
	CString strStationTime = pSttpData->sttp_body.ch_time_12_BIT1;
	CString strReasonTime = pSttpData->sttp_body.ch_time_12_BIT2;
	CTime tmReason;
	if (strReasonTime.IsEmpty())
	{
		tmReason = CTime::GetCurrentTime();
	}
	else
	{
		tmReason = StringToTimeSttp12(strReasonTime);
	}
	CTime tmStation = StringToTimeSttp12(strStationTime);
 
	SetStatus(pSttpData->sttp_body.nStatus, tmStation, nReason, nChannel, tmReason, m_nIPFlag);
	
	//ˢ�³���ʱ��
	RefreshDurativeTime();
}

/*************************************************************
 �� �� ����SecCommChange()
 ���ܸ�Ҫ������ͨѶ״̬�仯, �˴���ά������ͨѶ״̬�쳣����
 �� �� ֵ: void
 ��    ����param1	״̬�仯�ı���
		   Param2	�仯���״̬
**************************************************************/
//##ModelId=49B87BA603D8
void CStationObj::SecCommChange( CSecObj* pObj, int nStatus )
{
	//�ж�������Ч��
	if(pObj == NULL)
		return;
	if(nStatus<0 || nStatus>1)
		return;
	//�ж��������Ƿ��Ѵ��ڴ�װ��
	BOOL bExit = FALSE;
	POSITION pos, posOld;
	pos = m_arrCommAbnormity.GetHeadPosition();
	while(pos != NULL)
	{
		//��¼��λ��
		posOld = pos;
		CSecObj* pSec = (CSecObj*)m_arrCommAbnormity.GetNext(pos);
		if(pSec->m_sID == pObj->m_sID)
		{
			//�Ѵ���
			bExit = TRUE;
			//�ж����ڵ�״̬�Ƿ�������,�ǵĻ�������ɾ���˱���
			if(nStatus == 0)
			{
				//ͨѶ״̬����
				m_arrCommAbnormity.RemoveAt(posOld);
			}
			break;
		}
	}
	//�����в����ڴ�װ��
	if(!bExit)
	{
		//���״̬�ǶϿ�,����뵽����
		if(nStatus == 1)
		{
			m_arrCommAbnormity.AddTail(pObj);
		}
	}
}

/*************************************************************
 �� �� ����SecRunChange()
 ���ܸ�Ҫ����������״̬�仯,�˴�ά������״̬�쳣����
 �� �� ֵ: void
 ��    ����param1	״̬�仯�ı���
		   Param2	�仯���״̬
**************************************************************/
//##ModelId=49B87BA7001F
void CStationObj::SecRunChange( CSecObj* pObj, int nStatus )
{
	//�ж�������Ч��
	if(pObj == NULL)
		return;
	if(nStatus<0 || nStatus>4)
		return;
	//�ж��������Ƿ��Ѵ��ڴ�װ��
	BOOL bExit = FALSE;
	POSITION pos, posOld;
	pos = m_arrRunAbnormity.GetHeadPosition();
	while(pos != NULL)
	{
		//��¼��λ��
		posOld = pos;
		CSecObj* pSec = (CSecObj*)m_arrRunAbnormity.GetNext(pos);
		if(pSec->m_sID == pObj->m_sID)
		{
			//�Ѵ���
			bExit = TRUE;
			//�ж����ڵ�״̬�Ƿ�������,�ǵĻ�������ɾ���˱���
			if(nStatus == 2)
			{
				//ͨѶ״̬����
				m_arrRunAbnormity.RemoveAt(posOld);
			}
		}
	}
	//�����в����ڴ�װ��
	if(!bExit)
	{
		//���״̬�Ƿ�����,����뵽����
		if(nStatus != 2)
		{
			m_arrRunAbnormity.AddTail(pObj);
		}
	}
}

/*************************************************************
 �� �� ����GetCommAbnormityNum()
 ���ܸ�Ҫ: �õ�����վ��ͨѶ״̬�쳣�ı�������
 �� �� ֵ: ͨѶ״̬�����ı�����
**************************************************************/
//##ModelId=49B87BA7003E
int CStationObj::GetCommAbnormityNum()
{
	int nCount = 0;
	nCount = m_arrCommAbnormity.GetCount();
	return nCount;
}

/*************************************************************
 �� �� ����GetRunAbnormityNum()
 ���ܸ�Ҫ���õ�����վ������״̬�쳣�ı�������
 �� �� ֵ: ����״̬�쳣�ı�����
**************************************************************/
//##ModelId=49B87BA7004E
int CStationObj::GetRunAbnormityNum()
{
	int nCount = 0;
	nCount = m_arrRunAbnormity.GetCount();
	return nCount;
}

/*************************************************************
 �� �� ����SaveSvgPath()
 ���ܸ�Ҫ������SVG�ļ�·�������ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	SVG·��(���·��)
		   Param2
**************************************************************/
//##ModelId=49B87BA7005D
BOOL CStationObj::SaveSvgPath( CString sPath )
{
	BOOL bReturn;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//��·����\ת����/
	sPath.Replace("\\", "/");
	
	//ָ��station_id,����SVG_Path
	
	CString str;	
	//�ֶ�
	//SVG_Path
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SVG_Path", EX_STTP_DATA_TYPE_STRING, sPath);
	
	//����
	//station_id
	Condition condition2;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_STATION_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CStationObj::SaveSvgPath, ����ʧ��");
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
		str.Format("CStationObj::SaveSvgPath,����ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ����GetReason()
 ���ܸ�Ҫ���õ���վ״̬�ı�ԭ������
 �� �� ֵ: ״̬�ı�ԭ������
 ��    ����param1	״̬�ı�ԭ��
		   Param2
**************************************************************/
//##ModelId=49B87BA7007D
CString CStationObj::GetReason( int nType )
{

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	CString strReason = _T("");
	if(pData->m_StationStatusConfigMap.Lookup(nType, strReason))
	{
		return strReason;
	}
	else
	{
		if(0 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_NORMAL);
		else if(1 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_STANDBY);
		else if(2 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_MACHINE);
		else if(3 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_GATEWAY);
		else if(4 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_NETWORK);
		else if(5 == nType)
			return StringFromID(IDS_DISCONNECT_REASON_LINK);
		else
			return StringFromID(IDS_DISCONNECT_REASON_UNKNOWN);
	}
}

/*************************************************************
 �� �� ��: StatVoltage()
 ���ܸ�Ҫ: ͳ��վ�ڵ�ѹ�ȼ�,������m_arrVoltage��
 �� �� ֵ: 
 ��    ��: param1 ��ѹ�ȼ�
		   Param2 
**************************************************************/
void CStationObj::StatVoltage( int nVoltage )
{
	//�ж�������Ч��
	if(nVoltage <= 0)
		return;
	BOOL bFind = FALSE;
	//�����Ƿ��Ѽ�¼�˵�ѹ
	for(int i = 0; i < m_arrVoltage.GetSize(); i++)
	{
		int nV = m_arrVoltage.GetAt(i);
		if(nV == nVoltage)
		{
			bFind = TRUE;
			break;
		}
	}

	//�����µ�ѹ�ȼ�
	if(!bFind)
	{
		m_arrVoltage.Add(nVoltage);
	}
}

/*************************************************************
 �� �� ��: IsShowInGeo()
 ���ܸ�Ҫ: �ж��Ƿ��ڵ���ͼ����ʾ
 �� �� ֵ: Ҫ��ʾ����TRUE,����ʾ����FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CStationObj::IsShowInGeo()
{
	if(m_nShowInGeo != 1)
	{
		//����ʾ
		return FALSE;
	}
	else
	{
		//��ʾ����λ�ò��ԣ�Ҳ��Ϊ����ʾ
		if(m_ptCenter.x < 0 || m_ptCenter.y < 0)
			return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: DrawObj()
 ���ܸ�Ҫ: ����ͼ��
 �� �� ֵ: 
 ��    ��: param1 �����豸ָ��
		   Param2 
**************************************************************/
void CStationObj::DrawObj( CDC* pDC )
{
	CPen pen;
	COLORREF colPen = RGB(0, 255, 0);
	if(m_nCommStatus == 0)
		colPen = XJ_STATUS_COMM_OFF;
	else if(m_nCommStatus == 1)
		colPen = XJ_STATUS_COMM_ON;
	pen.CreatePen(PS_SOLID,5,colPen);
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC ->SelectStockObject(NULL_BRUSH);

	CRect r(m_ptCenter.x - 5, m_ptCenter.y -5, m_ptCenter.x + 5, m_ptCenter.y + 5);

	//����ͼ��
	pDC->Ellipse(&r);
 	r.DeflateRect(-8,-8,-8,-8);	
 	pDC->Ellipse(&r);

	pDC->SelectObject(pOldPen);
	
	pen.DeleteObject();

	//����ѡ�п�
	if(m_bSelected)
	{
		CPen pen1;
		CPen * oldPen1;
		CBrush brush1;
		CBrush * oldBrush1;
		//CreateBrush
		if(!brush1.CreateSolidBrush(RGB(255, 0, 0)))
			return;
		//CreatePen
		if(!pen1.CreatePen(PS_SOLID , 1, RGB(255, 0, 0))) 
			return;

		oldPen1 = pDC->SelectObject(&pen1);
		oldBrush1 = pDC->SelectObject(&brush1);

		CRect rcSelected;
		int nOffset = 8;
		//����
		rcSelected.top = r.top;
		rcSelected.left = r.left;
		rcSelected.bottom = r.top + nOffset;
		rcSelected.right = r.left + nOffset;
		pDC->Rectangle(rcSelected);

		//����
		rcSelected.top = r.top;
		rcSelected.left = r.right - nOffset;
		rcSelected.bottom = r.top + nOffset;
		rcSelected.right = r.right;
		pDC->Rectangle(rcSelected);

		//����
		rcSelected.top = r.bottom - nOffset;
		rcSelected.left = r.left;
		rcSelected.bottom = r.bottom;
		rcSelected.right = r.left + nOffset;
		pDC->Rectangle(rcSelected);

		//����
		rcSelected.top = r.bottom - nOffset;
		rcSelected.left = r.right;
		rcSelected.bottom = r.bottom;
		rcSelected.right = r.right - nOffset;
		pDC->Rectangle(rcSelected);

		pDC ->SelectObject(oldPen1);
		pDC ->SelectObject(oldBrush1);
	}
}

/*************************************************************
 �� �� ��: GetGeoPosition()
 ���ܸ�Ҫ: ���ݱ�ﳧվ�ڵ���ͼ�е�λ�ü�¼�ַ����ĵ�λ��.��ʽ��    �Ƿ���ʾ��Xλ�ã�Yλ�ã�����1:100,200��,�Ƿ���ʾ��0/1��ʾ��0-����ʾ�� 1-��ʾ.Xλ�ú�Yλ�ö���������ʾ��
 �� �� ֵ: 
 ��    ��: param1 ��ﳧվ�ڵ���ͼ�е�λ�ü�¼�ַ���
		   Param2 
**************************************************************/
void CStationObj::GetGeoPosition( CString strPosition )
{
	if(strPosition == "")
		return;
	//����1:100,200
	int nFind = -1;
	//����:
	nFind = strPosition.Find(':', 0);
	if(nFind == -1)
		return;
	CString str = strPosition.Left(nFind);
	m_nShowInGeo = atoi(str);
	strPosition.Delete(0, nFind+1);
	//����,
	nFind = strPosition.Find(',', 0);
	if(nFind == -1)
		return;
	str = strPosition.Left(nFind);
	m_ptCenter.x = atoi(str);
	strPosition.Delete(0, nFind+1);
	m_ptCenter.y = atoi(strPosition);
}

/*************************************************************
 �� �� ��: HitTest()
 ���ܸ�Ҫ: �жϳ�վͼ���Ƿ񱻵��
 �� �� ֵ: ���������TRUE,δ���������FALSE
 ��    ��: param1 �����λ��
		   Param2 
**************************************************************/
BOOL CStationObj::HitTest( CPoint ptHit )
{
	//ȷ��ͼ�η�Χ
	CRect r = GetBound();
	return r.PtInRect(ptHit);
}

void CStationObj::SetSelected( BOOL bSelected )
{
	m_bSelected = bSelected;
}

/*************************************************************
 �� �� ��: GetBound()
 ���ܸ�Ҫ: ȡ��ͼ�η�Χ
 �� �� ֵ: ͼ�η�Χ
 ��    ��: param1 
		   Param2 
**************************************************************/
CRect CStationObj::GetBound()
{
	CRect r(m_ptCenter.x - 5, m_ptCenter.y -5, m_ptCenter.x + 5, m_ptCenter.y + 5);
	r.DeflateRect(-12,-12,-12,-12);

	return r;
}

/*************************************************************
 �� �� ��: MoveObj()
 ���ܸ�Ҫ: �ƶ�ͼ�ε�ָ��λ��
 �� �� ֵ: 
 ��    ��: param1 ָ��λ��
		   Param2 
**************************************************************/
void CStationObj::MoveObj( CPoint pt )
{
	m_ptCenter = pt;
}

/*************************************************************
 �� �� ��: ShowInGeo()
 ���ܸ�Ҫ: �ڵ���ͼ����ʾ
 �� �� ֵ: 
 ��    ��: param1 ����λ��
		   Param2 
**************************************************************/
void CStationObj::ShowInGeo( CPoint pt )
{
	m_nShowInGeo = 1;
	m_ptCenter = pt;
}

/*************************************************************
 �� �� ��: HideInGeo()
 ���ܸ�Ҫ: ����Ϊ���ڵ���ͼ����ʾ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CStationObj::HideInGeo()
{
	m_nShowInGeo = 0;
}

/*************************************************************
 �� �� ��: SetStatus()
 ���ܸ�Ҫ: ���ó�վ״̬
 �� �� ֵ: 
 ��    ��: param1 ״̬
		   Param2 ʱ��
		   Param3 ԭ��
		   Param4 ͨ��
**************************************************************/
void CStationObj::SetStatus( int nStatus, CTime tm, int nReason, int nChannel, CTime tmReasonTime, int nIPFlag)
{
	m_nChannel = nChannel;
	if(nChannel == 0)
	{
		//0��ͨ��
		m_nCommStatus = nStatus;
		m_tmNewStatus = tm;
		m_sNewStatusReason = GetReason(nReason);
		m_tmReasonTime = tmReasonTime;
// 		if (nIPFlag != 2)
// 		{
// 			m_nCommStatus1 = -1;
// 		}
	}
	else
	{
		//1��ͨ��
		m_nCommStatus1 = nStatus;
		m_tmNewStatus1 = tm;
		m_sNewStatusReason1 = GetReason(nReason);
		m_tmReasonTime1 = tmReasonTime;
// 		if (nIPFlag != 2)
// 		{
// 			m_nCommStatus = -1;
// 		}
	}
	//ˢ�³���ʱ��
	RefreshDurativeTime();
}

/************************************
Date:2011/7/19  Author:LYH
������:    BuildBusMap
����ֵ:   BOOL �ɹ�����true��ʧ�ܷ���false
���ܸ�Ҫ: ��ȡ����վ��ĸ�����ò�����ĸ��MAP
************************************/
BOOL CStationObj::BuildBusMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	m_BusMap.RemoveAll();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_BUS_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BUS_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBusMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildBusMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CBusObj * pObj = new CBusObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_BusMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBusMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::LoadData()
{
	
	AfxBeginThread(StationLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
	return TRUE;
}

void CStationObj::InitLock()
{
	m_pLock = new CXJLock(m_sID);
}

BOOL CStationObj::BuildData()
{
	CLockUp lockup(m_pLock);
	Clear();
	BuildBusMap();
	BuildBayMap();
	BuildBreakMap();
	BuildCapMap();
	BuildGenMap();
	BuildLineMap();
	BuildPmsMap();
	BuildSwitchMap();
	BuildTransMap();
	BuildSecMap();
	BuildCpuData();
	BuildZoneData();
	return TRUE;
}

BOOL CStationObj::BuildLineMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_LineMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "line_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code", EX_STTP_DATA_TYPE_INT);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "run_status", EX_STTP_DATA_TYPE_INT);
	
	Field Field41;
	pApp->m_DBEngine.SetField(sql, Field41, "voltage", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "103group", EX_STTP_DATA_TYPE_INT);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "103item", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "station_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "bay_id", EX_STTP_DATA_TYPE_STRING);

	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_LINE_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_LINE_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildLineMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildLineMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CLineObj * pObj = new CLineObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_LineMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildLineMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildBreakMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_BreakMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_BREAKER_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BREAKER_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBreakMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildBreakMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CBreakObj * pObj = new CBreakObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_BreakMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBreakMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildBayMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_BayMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_BAY_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_BAY_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBayMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildBayMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CBayObj * pObj = new CBayObj;
			if(pObj->Load(pMemset))
			{
				//��ȡ���ݳɹ�,����map
				m_BayMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBayMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildCapMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_CapMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_CAPACITOR_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CAPACITOR_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildCapMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildCapMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CCapObj * pObj = new CCapObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_CapMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildCapMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildReactorMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_ReactorMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_REACTOR_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_REACTOR_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildReactorMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildReactorMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CReactorObj * pObj = new CReactorObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_ReactorMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildReactorMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildSwitchMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_SwitchMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_SWITCH_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SWITCH_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildSwitchMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildSwitchMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CSwitchObj * pObj = new CSwitchObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_SwitchMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildSwitchMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildPmsMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_PmsMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_PMS_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PMS_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildPmsMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildPmsMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CPmsObj * pObj = new CPmsObj;
			if(pObj->Load(pMemset))
			{
				//��ȡ���ݳɹ�,����map
				m_PmsMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildPmsMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildTransMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_TransMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_TRANS_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TRANS_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildTransMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildTransMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CTransObj * pObj = new CTransObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltageH);
				//��ȡ���ݳɹ�,����map
				m_TransMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildTransMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildGenMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);

	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_GenMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_GEN_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_GEN_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildGenMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildGenMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CGenObj * pObj = new CGenObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//��ȡ���ݳɹ�,����map
				m_GenMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildGenMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildSecMap()
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);
	
	BOOL bReturn = TRUE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	m_SecMap.RemoveAll();
	m_UriMap.RemoveAll();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

// 	Field Field1;
// 	pApp->m_DBEngine.SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field2;
// 	pApp->m_DBEngine.SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field3;
// 	pApp->m_DBEngine.SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field4;
// 	pApp->m_DBEngine.SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field41;
// 	pApp->m_DBEngine.SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field5;
// 	pApp->m_DBEngine.SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field6;
// 	pApp->m_DBEngine.SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field7;
// 	pApp->m_DBEngine.SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field8;
// 	pApp->m_DBEngine.SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);
// 	
// 	Field Field9;
// 	pApp->m_DBEngine.SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field10;
// 	pApp->m_DBEngine.SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field11;
// 	pApp->m_DBEngine.SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field12;
// 	pApp->m_DBEngine.SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field13;
// 	pApp->m_DBEngine.SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field14;
// 	pApp->m_DBEngine.SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field15;
// 	pApp->m_DBEngine.SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field16;
// 	pApp->m_DBEngine.SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field17;
// 	pApp->m_DBEngine.SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field18;
// 	pApp->m_DBEngine.SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field19;
// 	pApp->m_DBEngine.SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field20;
// 	pApp->m_DBEngine.SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field21;
// 	pApp->m_DBEngine.SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field22;
// 	pApp->m_DBEngine.SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);
// 	
// 	Field Field23;
// 	pApp->m_DBEngine.SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);
// 
// 	Field Field24;
// 	pApp->m_DBEngine.SetField(sql, Field24, "ip_a", EX_STTP_DATA_TYPE_STRING);
// 
// 	Field Field25;
// 	pApp->m_DBEngine.SetField(sql, Field25, "port_a", EX_STTP_DATA_TYPE_INT);
// 
// 	Field Field26;
// 	pApp->m_DBEngine.SetField(sql, Field26, "ip_b", EX_STTP_DATA_TYPE_STRING);
// 	
// 	Field Field27;
// 	pApp->m_DBEngine.SetField(sql, Field27, "port_b", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field28;
// 	pApp->m_DBEngine.SetField(sql, Field28, "orderstrategy", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field29;
// 	pApp->m_DBEngine.SetField(sql, Field29, "suspendstatus", EX_STTP_DATA_TYPE_INT);
// 	
// 	Field Field30;
// 	pApp->m_DBEngine.SetField(sql, Field30, "suspendreason", EX_STTP_DATA_TYPE_STRING);

// 	REALDATA_CONDITION realCon;
// 	realCon.IsUse = TRUE;
	// 	realCon.station_id = m_sID;
	Condition con1;
	CString str;
	str.Format("station_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		//nResult = pApp->m_DBEngine.XJSelectRealData(EX_STTP_INFO_SECDEV_CFG, sql, realCon, sError, pMemset);
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SECDEV_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildSecMap, ��ѯʧ��,��վID:%s", m_sID);
		logMgr.WriteLog(strLog, XJ_LOG_LV1);
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
			str.Format("CStationObj::BuildSecMap, ��ȡ��%d������, ��վID:%s", nCount, m_sID);
			logMgr.WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CSecObj * pObj = new CSecObj;
			if(pObj->Load(pMemset))
			{
				//����һ���豸
				pObj->m_pOwner = FindDeviceNoLock(pObj->m_sOwnerID, pObj->m_nOwnerType);
				//���ó�վ
				pObj->m_pStation = this;
				//��ȡ���ݳɹ�,����map
				m_SecMap.SetAt(pObj->m_sID, pObj);
				m_UriMap.SetAt(pObj->m_sUri, pObj);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildSecMap,��ѯʧ��,��վID:%s,ԭ��Ϊ%s", m_sID, sError);
		logMgr.WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/************************************
Date:2011/7/21  Author:LYH
������:    FindDeviceNoLock
����ֵ:   CDeviceObj*
���ܸ�Ҫ: ���ұ���վ�µ��豸,������ 
����: CString sID �豸ID
����: int nType �豸����
************************************/
CDeviceObj* CStationObj::FindDeviceNoLock( CString sID, int nType )
{
	//ȥ�ո�
	sID.TrimLeft();
	sID.TrimRight();
	CDeviceObj * pReturn = NULL;
	
	switch(nType)
	{
	case TYPE_BAY: //���
		m_BayMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_BREAK: //����
		m_BreakMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_BUS: //ĸ��
		m_BusMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_CAP: //������
		m_CapMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_GEN: //�����
		m_GenMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_LINE: //��· 
		m_LineMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_PMS: //���������
		m_PmsMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_REACTOR: //�翹��
		m_ReactorMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_SWITCH: //��բ
		m_SwitchMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_TRANS: //��ѹ��
		m_TransMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_SEC: //���������豸
	case TYPE_PROTECT: //���� 
	case TYPE_WAVEREC: //¼����
		m_SecMap.Lookup(sID, (CObject*&)pReturn);
		break;
	default:
		break;
	}
	return pReturn;
}

/************************************
Date:2011/7/21  Author:LYH
������:    GetPriDeviceList
����ֵ:   void
���ܸ�Ҫ: ȡ��һ���豸����
����: DEVICE_LIST & listDevice ���ڱ���һ���豸������
����: int nDeviceType ָ��Ҫ���ҵ�һ���豸���ͣ�-1��ʾ��������
************************************/
void CStationObj::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType )
{
	POSITION pos = NULL;
	CString sID;
	CDeviceObj* pObj = NULL;
	//���
	if(nDeviceType == TYPE_BAY || nDeviceType == -1)
	{
		for(pos = m_BayMap.GetStartPosition(); pos != NULL; )
		{
			m_BayMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//����
	if(nDeviceType == TYPE_BREAK || nDeviceType == -1)
	{
		for(pos = m_BreakMap.GetStartPosition(); pos != NULL; )
		{
			m_BreakMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//ĸ��
	if(nDeviceType == TYPE_BUS || nDeviceType == -1)
	{
		for(pos = m_BusMap.GetStartPosition(); pos != NULL; )
		{
			m_BusMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//������
	if(nDeviceType == TYPE_CAP || nDeviceType == -1)
	{
		for(pos = m_CapMap.GetStartPosition(); pos != NULL; )
		{
			m_CapMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//�����
	if(nDeviceType == TYPE_GEN || nDeviceType == -1)
	{
		for(pos = m_GenMap.GetStartPosition(); pos != NULL; )
		{
			m_GenMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//��·
	if(nDeviceType == TYPE_LINE || nDeviceType == -1)
	{
		for(pos = m_LineMap.GetStartPosition(); pos != NULL; )
		{
			m_LineMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//���������
	if(nDeviceType == TYPE_PMS || nDeviceType == -1)
	{
		for(pos = m_PmsMap.GetStartPosition(); pos != NULL; )
		{
			m_PmsMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//�翹��
	if(nDeviceType == TYPE_REACTOR || nDeviceType == -1)
	{
		for(pos = m_ReactorMap.GetStartPosition(); pos != NULL; )
		{
			m_ReactorMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//��բ
	if(nDeviceType == TYPE_SWITCH || nDeviceType == -1)
	{
		for(pos = m_SwitchMap.GetStartPosition(); pos != NULL; )
		{
			m_SwitchMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//��ѹ��
	if(nDeviceType == TYPE_TRANS || nDeviceType == -1)
	{
		for(pos =  m_TransMap.GetStartPosition(); pos != NULL; )
		{
			m_TransMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
}

/************************************
Date:2011/7/21  Author:LYH
������:    GetSecList
����ֵ:   void
���ܸ�Ҫ: ȡ�ö����豸����
����: DEVICE_LIST & listDevice ���ڱ�������豸������
����: int nDeviceType ָ��Ҫ���ҵĶ����豸���ͣ�-1��type_sec��ʾ��������
************************************/
void CStationObj::GetSecList( DEVICE_LIST& listDevice,CString sPriID, int nSecType, int nDetailKind )
{
	POSITION pos = NULL;
	CString sID;
	CDeviceObj* pObj = NULL;
	for(pos = m_SecMap.GetStartPosition(); pos != NULL; )
	{
		m_SecMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj == NULL)
			continue;
		CSecObj* pSec = (CSecObj*)pObj;
		if(sPriID.IsEmpty() || sPriID == pSec->m_sOwnerID)
		{
			if(nSecType == pSec->m_nType || nSecType == -1 || nSecType == TYPE_SEC)
			{
				if( nDetailKind == pSec->m_nSecType || nDetailKind == -1 )
					listDevice.AddTail(pObj);
			}
		}
	}
}

void CStationObj::SetVisibility( BOOL bVisi )
{
	if(bVisi)
		m_nShowInGeo = 1;
	else
		HideInGeo();
}

BOOL CStationObj::GetVisibility()
{
	return IsShowInGeo();
}

/****************************************************
Date:2012/11/26  Author:LYH
������:   GetPositionInGeo	
����ֵ:   CPoint	
���ܸ�Ҫ: 
*****************************************************/
CPoint CStationObj::GetPositionInGeo()
{
	return m_ptCenter;
}

/****************************************************
Date:2013/6/4  Author:LYH
������:   BuildCpuData	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CStationObj::BuildCpuData()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	Field field1;
	pApp->m_DBEngine.SetField(sql, field1, "pt_id", EX_STTP_DATA_TYPE_STRING);

	Field field2;
	pApp->m_DBEngine.SetField(sql, field2, "cpu_code", EX_STTP_DATA_TYPE_INT);

	Field field3;
	pApp->m_DBEngine.SetField(sql, field3, "cpu_des", EX_STTP_DATA_TYPE_STRING);

	Field field31;
	pApp->m_DBEngine.SetField(sql, field31, "cpu_ver", EX_STTP_DATA_TYPE_STRING);

	Field field4;
	pApp->m_DBEngine.SetField(sql, field4, "from_zone", EX_STTP_DATA_TYPE_INT);

	Field field5;
	pApp->m_DBEngine.SetField(sql, field5, "to_zone", EX_STTP_DATA_TYPE_INT);

	Condition con1;
	CString str;
	str.Format("pt_id IN (SELECT pt_id FROM tb_secdev_base WHERE station_id = '%s')", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_CPU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CStationObj::BuildCpuData, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}

	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CStationObj::BuildCpuData, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			CSecCPU * cpu = new CSecCPU;
			if(cpu->Load(pMemset))
			{
				//��ȡ�ɹ�
				CSecObj* pSec = (CSecObj*)FindDeviceNoLock(cpu->PT_ID, TYPE_SEC);
				if(pSec == NULL)
				{
					//���Ҳ��������豸
					delete cpu;
					cpu = NULL;
				}
				else
				{
					pSec->m_arrCPU.Add(cpu);
				}
			}
			else
			{
				delete cpu;
				cpu = NULL;
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildCpuData,��ѯʧ��,ԭ��Ϊ%s", sError);
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
Date:2013/6/4  Author:LYH
������:   BuildZoneData	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CStationObj::BuildZoneData()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;

	Condition con1;
	CString str;
	str.Format("pt_id IN (SELECT pt_id FROM tb_secdev_base WHERE station_id = '%s')", m_sID);
	pApp->m_DBEngine.SetCondition(sql, con1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CStationObj::BuildZoneData, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CStationObj::BuildZoneData, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			/*
			CString  PT_ID;//����ID
			int		cpu; //cpu��
			int		code; //��ֵ����
			CString zone_name;
			CString code_name;
			int     group;
			int     item;
			int     minValue;
			int     maxValue;
			int     stepValue;
			int     zoneValue;
			*/


			//����
			PT_ZONE * ptz = new PT_ZONE;
			CString str;
			ptz->PT_ID = pMemset->GetValue((UINT)0); //PT_ID
			str = pMemset->GetValue(1); //CPU_Code
			ptz->cpu = atoi(str);
			str = pMemset->GetValue(2);//Zone_CODE
			ptz->code = atoi(str);
			ptz->zone_name = pMemset->GetValue(3);
			ptz->code_name = pMemset->GetValue(4);
			str = pMemset->GetValue(5);
			ptz->group = atoi(str);
			str = pMemset->GetValue(6);
			ptz->item = atoi(str);
			str = pMemset->GetValue(7);
			ptz->minValue = atoi(str);
			str = pMemset->GetValue(8);
			ptz->maxValue = atoi(str);
			str = pMemset->GetValue(9);
			ptz->stepValue = atoi(str);
			str = pMemset->GetValue(12);
			ptz->zoneValue = atoi(str);

			/*
			str = pMemset->GetValue(3);//Zone_State
			ptz->state = atoi(str);
			*/

			//���Ҷ����豸
			CSecObj* pSec = (CSecObj*)FindDevice(ptz->PT_ID, TYPE_SEC);
			if(pSec == NULL)
			{
				delete ptz;
				ptz = NULL;
			}
			else
			{
				if(!pSec->AddZone(ptz))
				{
					delete ptz;
					ptz = NULL;
				}
				if (ptz != NULL && ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					pSec->m_bNewDevice = true;
				}
			}
						
			pMemset->MoveNext();
		}
	}
	else
	{
		
		CString str;
		str.Format("CStationObj::BuildZoneData,��ѯʧ��,ԭ��Ϊ%s", sError);
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
Date:2013/7/10  Author:LYH
������:   IsCommStatusON	
����ֵ:   BOOL	ͨѶ״̬��������TRUE,�Ͽ�����FALSE
���ܸ�Ҫ: �ж�ͨѶ״̬�Ƿ�����. ipflagΪ0ʱ����ͨ��1�ж�,Ϊ1ʱ��һ��������Ϊ����,Ϊ2ʱ����ͨ����������Ϊ����
*****************************************************/
BOOL CStationObj::IsCommStatusON()
{
	if( g_role == MODE_SUB )
		return TRUE;

	BOOL bReturn = FALSE;
	switch(m_nIPFlag)
	{
	case 0:
		bReturn = (m_nCommStatus==1);
		break;
	case 1:
		if(m_nCommStatus == 1)
			bReturn = TRUE;
		if(m_nCommStatus1 == 1)
			bReturn = TRUE;
		//bReturn = ((m_nCommStatus==1)||(m_nCommStatus1 == 1));
		break;
	case 2:
	case 3:
		bReturn = ((m_nCommStatus==1)&&(m_nCommStatus1 == 1));
		break;
	default:break;
	}
	return bReturn;
}

/****************************************************
Date:2013/7/18  Author:LYH
������:   GetStatusDesc	
����ֵ:   CString	
���ܸ�Ҫ: ȡ��ͨ��״̬����
����: int nChannel	
*****************************************************/
CString CStationObj::GetStatusDesc( int nChannel )
{
	CString sReturn = "";
	if(nChannel == 0)
	{
		//��ͨ��
		switch(m_nCommStatus)
		{
		case 0:
			sReturn = StringFromID(IDS_COMMUSTATUS_OFF);
			break;
		case 1:
			sReturn = StringFromID(IDS_COMMUSTATUS_ON);
			break;
		default:
			sReturn = StringFromID(IDS_COMMON_UNKNOWN);
			break;
		}
	}
	else
	{
		if(m_nIPFlag == 0)
			sReturn = StringFromID(IDS_COMMU_ALONE);
		else
		{
			//��ͨ��
			switch(m_nCommStatus1)
			{
			case 0:
				sReturn = StringFromID(IDS_COMMUSTATUS_OFF);
				break;
			case 1:
				sReturn = StringFromID(IDS_COMMUSTATUS_ON);
				break;
			default:
				sReturn = StringFromID(IDS_COMMON_UNUSED);
				break;
			}
		}
	}
	return sReturn;
}

CString CStationObj::QueryProtocolName(int nProtocolID)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str = "";
	
	//�ֶ�
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	//����
	Condition condition4;
	CString strFilter;
	strFilter.Format("ID=%d", nProtocolID);
	pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_PROTOCOL_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CStationObj::QueryProtocolName, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CStationObj::QueryProtocolName, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		if(nCount == 1)
			str = pMemset->GetValue((UINT)0);
		pMemset->MoveNext();
	}
	else
	{
		CString str;
		str.Format("CStationObj::QueryProtocolName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return str;
}

CString CStationObj::GetProtocolName()
{
	if(m_sProtocol.IsEmpty())
		m_sProtocol = QueryProtocolName( m_nProtocolId );
	return m_sProtocol;
}

CTime CStationObj::GetStatusTime()
{
	//ȡ�õ�ǰʱ��
	switch(m_nIPFlag)
	{
	case 0:
		return m_tmNewStatus;
		break;
	case 1:
		if(m_nCommStatus == 1)
			return m_tmNewStatus;
		else if(m_nCommStatus1 == 1)
			return m_tmNewStatus1;
		else
			return m_tmNewStatus>m_tmNewStatus1?m_tmNewStatus:m_tmNewStatus1;
		break;
	case 2:
	case 3:
		if((m_nCommStatus==1)&&(m_nCommStatus1 == 1) )
		{
			return m_tmNewStatus>m_tmNewStatus1?m_tmNewStatus:m_tmNewStatus1;
		}
		else if((m_nCommStatus==0)&&(m_nCommStatus1 == 0))
		{
			return m_tmNewStatus<m_tmNewStatus1?m_tmNewStatus:m_tmNewStatus1;
		}
		else if(m_nCommStatus==0)
		{
			return m_tmNewStatus;
		}
		else if(m_nCommStatus1==0)
		{
			return m_tmNewStatus1;
		}
		break;
	default:break;
	}
	return m_tmNewStatus;
}

CTimeSpan CStationObj::GetStatusDurative()
{
	//ȡ�õ�ǰʱ��
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewStatus;
	CTimeSpan tmSpan1 = t - m_tmNewStatus1;
	//tmSpan.Format("%D��%HСʱ%M����")
	switch(m_nIPFlag)
	{
	case 0:
		return tmSpan;
		break;
	case 1:
		if(m_nCommStatus == 1)
			return tmSpan;
		else if(m_nCommStatus1 == 1)
			return tmSpan1;
		else
			return tmSpan<tmSpan1?tmSpan:tmSpan1;
		break;
	case 2:
	case 3:
		if((m_nCommStatus==1)&&(m_nCommStatus1 == 1) )
		{
			return tmSpan<tmSpan1?tmSpan:tmSpan1;
		}
		else if((m_nCommStatus==0)&&(m_nCommStatus1 == 0))
		{
			return tmSpan>tmSpan1?tmSpan:tmSpan1;
		}
		else if(m_nCommStatus==0)
		{
			return tmSpan;
		}
		else if(m_nCommStatus1==0)
		{
			return tmSpan1;
		}
		break;
	default:break;
	}
	return tmSpan;
}

CString CStationObj::GetStatusReason()
{
	switch(m_nIPFlag)
	{
	case 0:
		return m_sNewStatusReason;
		break;
	case 1:
		if(m_nCommStatus == 1)
			return m_sNewStatusReason;
		else if(m_nCommStatus1 == 1)
			return m_sNewStatusReason1;
		else
			return m_tmNewStatus>m_tmNewStatus1?m_sNewStatusReason:m_sNewStatusReason1;
		break;
	case 2:
	case 3:
		if((m_nCommStatus==1)&&(m_nCommStatus1 == 1) )
		{
			return m_tmNewStatus>m_tmNewStatus1?m_sNewStatusReason:m_sNewStatusReason1;
		}
		else if((m_nCommStatus==0)&&(m_nCommStatus1 == 0))
		{
			return m_tmNewStatus<m_tmNewStatus1?m_sNewStatusReason:m_sNewStatusReason1;
		}
		else if(m_nCommStatus==0)
		{
			return m_sNewStatusReason;
		}
		else if(m_nCommStatus1==0)
		{
			return m_sNewStatusReason1;
		}
		break;
	default:break;
	}
	return m_sNewStatusReason;
}

//CStationObj End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBusObj ĸ��
IMPLEMENT_DYNCREATE(CBusObj, CDeviceObj)
//##ModelId=49B87BA700BC
CBusObj::CBusObj()
{
	m_nType = TYPE_BUS;
	m_sType = StringFromID(IDS_MODEL_BUS);

	m_nBusType = -1;
}

//##ModelId=49B87BA700CB
CBusObj::~CBusObj()
{
	
}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA700EA
BOOL CBusObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)1); //ID
	m_sName = pMemSet->GetValue(2); //����
	CString str;
	str = pMemSet->GetValue(3); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(5); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(6); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(7); //���߷�ʽ
	m_nBusType = atoi(str);
	str = pMemSet->GetValue(8); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(9); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(10); //��վID
	m_sBayID = pMemSet->GetValue(14);
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA700FA
BOOL CBusObj::Save()
{
	return TRUE;
}
//CBusObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CLineObj	��·
IMPLEMENT_DYNCREATE(CLineObj, CDeviceObj)
//##ModelId=49B87BA7012A
CLineObj::CLineObj()
{
	m_nType = TYPE_LINE;
	m_sType = StringFromID(IDS_MODEL_LINE);
}

//##ModelId=49B87BA70138
CLineObj::~CLineObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70157
BOOL CLineObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(0)); //ID
	m_sName = pMemSet->GetValue(int(1)); //����
	CString str;
	str = pMemSet->GetValue(int(2)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(3)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(4)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(5)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(6)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(7)); //��վID
	m_sBayID = pMemSet->GetValue(int(8)); //���ID

	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70167
BOOL CLineObj::Save()
{
	return TRUE;
}

//CLineObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBreakObj ����,��·��
IMPLEMENT_DYNCREATE(CBreakObj, CDeviceObj)
//##ModelId=49B87BA70198
CBreakObj::CBreakObj()
{
	m_nType = TYPE_BREAK;
	m_sType = StringFromID(IDS_MODEL_BREAKER);

	m_nBreakType = -1;
}

//##ModelId=49B87BA701A5
CBreakObj::~CBreakObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA701C5
BOOL CBreakObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(6)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(7)); //��������
	m_nBreakType = atoi(str);
	str = pMemSet->GetValue(int(8)); //���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //��վID
	m_sBayID = pMemSet->GetValue(int(12)); //���ID
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA701D4
BOOL CBreakObj::Save()
{
	return TRUE;
}
//CBreakObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CGenObj	�����
IMPLEMENT_DYNCREATE(CGenObj, CDeviceObj)
//##ModelId=49B87BA70204
CGenObj::CGenObj()
{
	m_nType = TYPE_GEN;
	m_sType = StringFromID(IDS_MODEL_GEN);

	m_nVolume = -1;
}

//##ModelId=49B87BA70213
CGenObj::~CGenObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70223
BOOL CGenObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(7)); //����
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //��վID
	m_sBayID = pMemSet->GetValue(int(12)); //���ID
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70232
BOOL CGenObj::Save()
{
	return TRUE;
}
//CGenObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CReactorObj	��ѹ�翹��
IMPLEMENT_DYNCREATE(CReactorObj, CDeviceObj)
//##ModelId=49B87BA70263
CReactorObj::CReactorObj()
{
	m_nType = TYPE_REACTOR;
	m_sType = StringFromID(IDS_MODEL_REACTOR);

	m_nVolume = -1;
}

//##ModelId=49B87BA70271
CReactorObj::~CReactorObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70281
BOOL CReactorObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(7); //����
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //��վID
	m_sBayID = pMemSet->GetValue(int(12)); //���ID
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA70290
BOOL CReactorObj::Save()
{
	return TRUE;
}
//CReactorObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CTransObj		��ѹ��
IMPLEMENT_DYNCREATE(CTransObj, CDeviceObj)
//##ModelId=49B87BA702CE
CTransObj::CTransObj()
{
	m_nType = TYPE_TRANS;
	m_sType = StringFromID(IDS_MODEL_TRANS);

	m_nTransType = -1;
	m_nVolume = -1;
	m_nVoltageH = -1;
	m_nVoltageM = -1;
	m_nVoltageL = -1;
}

//##ModelId=49B87BA702CF
CTransObj::~CTransObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA702F1
BOOL CTransObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //����
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(6)); //��ѹ���ѹ
	m_nVoltageH = atoi(str);
	str = pMemSet->GetValue(int(7)); //��ѹ���ѹ
	m_nVoltageL = atoi(str);
	str = pMemSet->GetValue(int(8)); //��ѹ���ѹ
	m_nVoltageM = atoi(str);
	str = pMemSet->GetValue(int(9)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(10)); //��ѹ������
	m_nTransType = atoi(str);
	str = pMemSet->GetValue(int(11)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(12)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(13)); //��վID
	m_sBayID = pMemSet->GetValue(int(15)); //���ID

	m_nVoltage = m_nVoltageH;

	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA702FE
BOOL CTransObj::Save()
{
	return TRUE;
}

//CTransObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSwitchObj	��բ
IMPLEMENT_DYNCREATE(CSwitchObj, CDeviceObj)
//##ModelId=49B87BA7032E
CSwitchObj::CSwitchObj()
{
	m_nType = TYPE_SWITCH;
	m_sType = StringFromID(IDS_MODEL_SWITCH);

	m_nSwitchType = -1;
}

//##ModelId=49B87BA7033C
CSwitchObj::~CSwitchObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA7034C
BOOL CSwitchObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(6)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(7)); //��բ����
	m_nSwitchType = atoi(str);
	str = pMemSet->GetValue(int(8)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //��վID
	m_sBayID = pMemSet->GetValue(int(12)); //���ID
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA7035B
BOOL CSwitchObj::Save()
{
	return TRUE;
}

//CSwitchObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CCapObj	������
IMPLEMENT_DYNCREATE(CCapObj, CDeviceObj)
//##ModelId=49B87BA7037C
CCapObj::CCapObj()
{
	m_nType = TYPE_CAP;
	m_sType = StringFromID(IDS_MODEL_CAP);

	m_nVolume = -1;
}

//##ModelId=49B87BA7038A
CCapObj::~CCapObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA7039A
BOOL CCapObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //����
	CString str;
	str = pMemSet->GetValue(int(3)); //���
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //����״̬
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //��ѹ�ȼ�
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(7)); //����
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103��Ŀ��
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //��վID
	m_sBayID = pMemSet->GetValue(int(12)); //���ID
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA703A9
BOOL CCapObj::Save()
{
	return TRUE;
}
//CCapObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CPmsObj		���������
IMPLEMENT_DYNCREATE(CPmsObj, CDeviceObj)
//##ModelId=49B87BA703CA
CPmsObj::CPmsObj()
{
	m_nType = TYPE_PMS;
	m_sType = StringFromID(IDS_MODEL_PMS);
}

//##ModelId=49B87BA703D8
CPmsObj::~CPmsObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA703D9
BOOL CPmsObj::Load(CMemSet * pMemSet)
{
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA80000
BOOL CPmsObj::Save()
{
	return TRUE;
}
//CPmsObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBayObj	���
IMPLEMENT_DYNCREATE(CBayObj, CDeviceObj)
//##ModelId=49B87BA80021
CBayObj::CBayObj()
{
	m_nType = TYPE_BAY;	
	m_sType = StringFromID(IDS_MODEL_BAY);
	m_sSvgPath = "";
}

//##ModelId=49B87BA8002E
CBayObj::~CBayObj()
{

}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݿ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA8002F
BOOL CBayObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)1);	//ID
	m_sName = pMemSet->GetValue(2); //����
	CString str;
	str = pMemSet->GetValue(3); //���
	m_nCode = atoi(str); //���
	str = pMemSet->GetValue(5); //��ѹ�ȼ�
	m_nVoltage = atoi(str);//��ѹ�ȼ�
	m_sStationID = pMemSet->GetValue(6); //������վID
	m_sSvgPath = pMemSet->GetValue(7); //SVG path
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA80040
BOOL CBayObj::Save()
{
	return TRUE;
}

/*************************************************************
 �� �� ��: SaveSvgPath()
 ���ܸ�Ҫ: ����SVG·�������ݿ�
 �� �� ֵ: SVG�ļ�·��
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CBayObj::SaveSvgPath( CString strPath )
{
	BOOL bReturn;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��·����\ת����/
	strPath.Replace("\\", "/");
	
	//ָ��station_id,����SVG_Path
	
	CString str;	
	//�ֶ�
	//SVG_Path
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SVG_Path", EX_STTP_DATA_TYPE_STRING, strPath);
	
	//����
	//bay_id
	Condition condition2;
	str.Format("bay_id = '%s'", m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_BAY_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBayObj::SaveSvgPath, ����ʧ��");
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
		str.Format("CBayObj::SaveSvgPath,����ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}
//CBayObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecObj	�����豸
IMPLEMENT_DYNCREATE(CSecObj, CDeviceObj)
//##ModelId=49B87BA800CD
CSecObj::CSecObj()
{
	m_nType = TYPE_SEC;
	m_sType = StringFromID(IDS_MODEL_SECONDARY);

	m_nSecType = -1;
	m_sKind = "";
	m_sModel = "";
	m_sSysVer = "";
	m_nCpuNum = -1;
//	m_tDevTime;	//�豸ʱ��(������ʾ: ��������ʱ��)
	m_sCrc = "";
	m_sAddr = "";
	m_nComID = -1;
	m_sOwnerID = "";
	m_nOwnerType = -1;
	m_nPTTypeID = -1;
	m_nProtocolType = -1;
	m_sPMSID = "";	
	m_n103Addr = -1;
	m_nSuspendstatus = -1;  
	m_nSuspendreason = -1;
	m_sSuspendreason = "";

	m_pOwner = NULL;
	m_pStation = NULL;

	m_bNoActChr = FALSE;
	m_bNoActSign = FALSE;
	m_bNoAlaSign = FALSE;

	CTime tmTemp(1971, 1, 1, 1, 1, 1);
	m_tmNewCommStatus = tmTemp;
	m_tmNewRunStatus = tmTemp;
	m_tmLastActive = CTime::GetCurrentTime();

	m_sCommDurativeTime = "";
	m_sRunDurativeTime = "";

	m_lCommDurative = 0;
	m_lRunDurative = 0;

	m_arrActChr.RemoveAll();
	m_arrActionSignal.RemoveAll();
	m_arrAlarmSignal.RemoveAll();
	m_arrCPU.RemoveAll();
	m_bNewDevice = false;

	m_nNoteCount = 0;

	InitializeCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87BA800DA
CSecObj::~CSecObj()
{
	ClearConfig();
	DeleteCriticalSection(&m_CriticalSection);
	//���CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		delete m_arrCPU[i];
	}
	m_arrCPU.RemoveAll();
}

/**************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݼ������豸��Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA80187
BOOL CSecObj::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetRealDataValue("pt_id"); //ID
	m_sUri = pMemSet->GetRealDataValue("uri"); //ͳһ��Դ���
	m_sName = pMemSet->GetRealDataValue("name"); //����
	CString str;
	str = pMemSet->GetRealDataValue("type"); //�豸����
	m_nSecType = atoi(str);
	m_sKind = pMemSet->GetRealDataValue("kind"); //��������
	m_sModel = pMemSet->GetRealDataValue("model"); //�豸�ͺ�
	m_sSysVer = pMemSet->GetRealDataValue("sysversion"); //�豸�汾��
	str = pMemSet->GetRealDataValue("cpunum"); //CPU����
	m_nCpuNum = atoi(str); 
	m_sDevTime = pMemSet->GetRealDataValue("dev_time");//�豸ʱ��
	m_sCrc = pMemSet->GetRealDataValue("crc"); //�豸У����
	m_sAddr = pMemSet->GetRealDataValue("addr"); //�豸��ַ
	str = pMemSet->GetRealDataValue("com_id"); //���ں�
	m_nComID = atoi(str);
	str = pMemSet->GetRealDataValue("run_status"); //����״̬
	m_nRunStatu = atoi(str);
	m_sStationID = pMemSet->GetRealDataValue("station_id"); //��վID
	m_sOwnerID = pMemSet->GetRealDataValue("primdev_id"); //һ���豸ID
	str = pMemSet->GetRealDataValue("primdev_type"); //һ���豸����
	m_nOwnerType = atoi(str);
	str = pMemSet->GetRealDataValue("pt_type_id"); //�����豸����ID
	m_nPTTypeID = atoi(str);
	str = pMemSet->GetRealDataValue("protocoltype"); //��Լ����
	m_nProtocolType = atoi(str);
	str = pMemSet->GetRealDataValue("103addr");	//103��ַ
	m_n103Addr = atoi(str);
	str = pMemSet->GetRealDataValue("103group"); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetRealDataValue("103item"); //103��Ŀ��
	m_n103Item = atoi(str);
	str = pMemSet->GetRealDataValue("commu_status"); //ͨѶ״̬
	m_nCommStatus = atoi(str);
	str = pMemSet->GetRealDataValue("commstatustime"); //��ǰͨ��״̬�ı��ʱ��
	m_tmNewCommStatus = StringToTime(str);
	str = pMemSet->GetRealDataValue("runstatustime"); //��ǰ����״̬�ı��ʱ��
	m_tmNewRunStatus = StringToTime(str);
	m_sIPA = pMemSet->GetRealDataValue("ip_a");
	m_nPortA = atoi(pMemSet->GetRealDataValue("port_a"));
	m_sIPB = pMemSet->GetRealDataValue("ip_b");
	m_nPortB = atoi(pMemSet->GetRealDataValue("port_b"));
	str = pMemSet->GetRealDataValue("suspendstatus");
	m_nSuspendstatus = atoi(str); //����״̬
	str = pMemSet->GetRealDataValue("suspendreason");
	m_nSuspendreason = atoi(str); //����ԭ��ID
	str = pMemSet->GetRealDataValue("reverse2");
	m_nCanControl = atoi(str);//�Ƿ��Զ������
	str = pMemSet->GetRealDataValue("commu_type");
	m_nCanCallHistory = atoi(str);

	//ȷ���豸����
	switch(m_nSecType)
	{
	case 1: //ĸ���
	case 2: //��ѹ������
	case 3: //��·����
	case 4: //�����鱣��
	case 5: //��·������
	case 6: //�翹������
	case 8: //����������
	case 11: //ĸ������
		m_nType = TYPE_PROTECT;
		break;
	case 9:
		m_nType = TYPE_WAVEREC;
		break;
	default:
		m_nType = TYPE_SEC;
		break;
	}

	//��ȡ������Ϣ
	m_sSuspendreason = GetMarkInfo(m_nSuspendreason);
	//��ȡCPU��Ϣ
//	GetCPUInfo();
	//����ͨѶ״̬
	SetCommStatus(m_nCommStatus);
//	RefreshCommDurativeTime();
	//��������״̬
	SetRunStatus(m_nRunStatu);
//	RefreshRunDurativeTime();
	//��һ���豸��������DataEngine����
	//������Ϣ��ͨѶ״̬����������߳̽��ж�ȡ����
	
	return TRUE;
}

/*************************************************************
 �� �� ����Save()
 ���ܸ�Ҫ�������豸��Ϣ�����ݿ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA80196
BOOL CSecObj::Save()
{
	return TRUE;
}

//##ModelId=49B87BA801D6
BOOL CSecObj::ClearConfig(int nAim /* = 0 */)
{
	EnterCriticalSection(&m_CriticalSection);
	int i = 0;
	try
	{
		if(nAim == 0 || nAim == 1)
		{
			//���������
			for(i=0;i<m_arrActChr.GetSize();i++)
			{		
				delete m_arrActChr[i];
			}
			m_arrActChr.RemoveAll();
		}
		
		if(nAim == 0 || nAim == 2)
		{
			//��������ź���
			for(i=0;i<m_arrActionSignal.GetSize();i++)
			{
				delete m_arrActionSignal[i];
			}
			m_arrActionSignal.RemoveAll();
		}
		
		if(nAim == 0 || nAim == 3)
		{
			for(i=0;i<m_arrAlarmSignal.GetSize();i++)
			{
				delete m_arrAlarmSignal[i];
			}
			m_arrAlarmSignal.RemoveAll();
		}
	}
	catch (CException* e)
	{
		e->Delete();
		LeaveCriticalSection(&m_CriticalSection);
		return FALSE;
	}
	LeaveCriticalSection(&m_CriticalSection);
	return TRUE;
}

/*************************************************************
 �� �� ����RefreshConfig()
 ���ܸ�Ҫ��ˢ�±�������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSES
**************************************************************/
//##ModelId=49B87BA80198
BOOL CSecObj::RefreshConfig(BOOL bRefresh)
{
	EnterCriticalSection(&m_CriticalSection);  
	m_tmLastActive = CTime::GetCurrentTime();
	if(bRefresh == FALSE)
	{
		//��ǿ��ˢ��, ����Ƿ������ô���
		//���û�����������ݿ��������ݵı�־ΪFALSE(��ʾ������Ϊ���ݿ����������ݵ�)�Ļ�,
		//˵�����ݿ��������ݵ���û������, ǿ��ˢ��һ��
		if(m_arrActChr.GetSize() <= 0 && !m_bNoActChr)
		{
			GetActionChr();
		}
		if(m_arrActionSignal.GetSize() <= 0 && !m_bNoActSign)
		{	
			GetActionSignal();
		}
		if(m_arrAlarmSignal.GetSize() <= 0 && !m_bNoAlaSign)
		{
			GetAlarmSignal();
		}
	}
	else
	{
		//ǿ��ˢ������
		GetActionChr();
		GetActionSignal();
		GetAlarmSignal();
	}
	LeaveCriticalSection(&m_CriticalSection);

	return TRUE;
}

/*************************************************************
 �� �� ����GetActionChr()
 ���ܸ�Ҫ�������ݿ��ȡ������������ֵ,tb_pt_character_def
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA801C9
BOOL CSecObj::GetActionChr()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//char_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "char_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);
	//Unit
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "Unit";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field5);
	//Reserve1(�¼�����)
	Field Field6;
	bzero(&Field6, sizeof(Field));
	str = "Reserve1";
	strncpy(Field6.FieldName, str, str.GetLength());
	Field6.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field6);
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_CHARCTOR_DEF_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CSecObj::GetActionChr, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetActionChr, ��ȡ��%d������", nCount);
			WriteLogEx(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoActChr = TRUE;
			CString str;
			str.Format("IDΪ%s�ı���û��ָ��������������Ϣ", m_sID);
			WriteLogEx(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			PT_ActionCharacter * sign = new PT_ActionCharacter;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //action_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			sign->strUnits = pMemset->GetValue(4); //Unit
			str = pMemset->GetValue(5);//�¼�����
			sign->nEventType = atoi(str);

			//��λȥ�ո�
			sign->strUnits.TrimLeft();
			sign->strUnits.TrimRight();

			m_arrActChr.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetActionChr,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����GetActionSignal()
 ���ܸ�Ҫ�������ݿ��ȡ���������ź���,tb_pt_action_def
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA801D4
BOOL CSecObj::GetActionSignal()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//action_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "action_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);

	//property
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "property";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field5);
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CSecObj::GetActionSignal, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetActionSignal, ��ȡ��%d������", nCount);
			WriteLogEx(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoActSign = TRUE;
			CString str;
			str.Format("IDΪ%s�ı���û�ж����ź�����Ϣ", m_sID);
			WriteLogEx(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			PT_Signal * sign = new PT_Signal;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //action_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			str = pMemset->GetValue(4); //property
			sign->nEventType = atoi(str);

			//�����б�
			sign->strPT_ID     = m_sID;
			sign->strPTName    = m_sName;	        //��������	
			sign->strPT_Model  = m_sModel;			//�����ͺ�
			sign->strStationId = m_sStationID;		//�������ڳ�վID

			m_arrActionSignal.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetActionSignal,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����GetAlarmSignal()
 ���ܸ�Ҫ�������ݿ��ȡ�����澯�ź���,tb_pt_alarm_def
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA801D5
BOOL CSecObj::GetAlarmSignal()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//alarm_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "alarm_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);

	//property
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "property";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field5);
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CSecObj::GetAlarmSignal, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetAlarmSignal, ��ȡ��%d������", nCount);
			WriteLogEx(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoAlaSign = TRUE;
			CString str;
			str.Format("IDΪ%s�ı���û�и澯�ź�����Ϣ", m_sID);
			WriteLogEx(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			PT_Signal * sign = new PT_Signal;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //alarm_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			str = pMemset->GetValue(4); //property
			sign->nEventType = atoi(str);

			//�����б�
			sign->strPT_ID     = m_sID;
			sign->strPTName    = m_sName;	        //��������	
			sign->strPT_Model  = m_sModel;			//�����ͺ�
			sign->strStationId = m_sStationID;		//�������ڳ�վID
		//	sign->strStation   = m_sSTATIONName;	//�������ڵĳ�վ����

			m_arrAlarmSignal.Add(sign);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetAlarmSignal,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����GetCPUInfo()
 ���ܸ�Ҫ�������ݿ��ȡCPU��Ϣ,tb_secdev_cpu_base
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
//##ModelId=49B87BA801E5
BOOL CSecObj::GetCPUInfo()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_CPU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecObj::GetCPUInfo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			CString str;
			str.Format("IDΪ%s�ı���û��CPU��Ϣ", m_sID);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			CSecCPU * cpu = new CSecCPU;
			if(cpu->Load(pMemset))
			{
				//��ȡ�ɹ�
				m_arrCPU.Add(cpu);
			}
			else
			{
				delete cpu;
				cpu = NULL;
			}

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetCPUInfo,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����SetCommStatus()
 ���ܸ�Ҫ���ı�װ��ͨѶ״̬
 �� �� ֵ: void
 ��    ����param1	ָ��ͨѶ״̬.0-����, 1-�Ͽ�
		   Param2
**************************************************************/
//##ModelId=49B87BA801A6
void CSecObj::SetCommStatus( int nStatus )
{
	//�ж�������Ч��
	if(nStatus<0 || nStatus>1)
		return;
	m_nCommStatus = nStatus;
	//���߳�վ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	CStationObj* pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(pStation != NULL)
		pStation->SecCommChange(this, m_nCommStatus);
}
/*************************************************************
 �� �� ����GetMarkInfo()
 ���ܸ�Ҫ���ı�װ��ͨѶ״̬
 �� �� ֵ: void
 ��    ����param1	ָ��ͨѶ״̬.0-����, 1-�Ͽ�
		   Param2
**************************************************************/
//##ModelId=49B87BA801A6
CString CSecObj::GetMarkInfo(int nStatus)
{

	CString sMarkInfo = "";
	//�ж�������Ч��
	if(nStatus < 0)
		return sMarkInfo;
	//���߳�վ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return sMarkInfo;
// 	DEVICE_LIST listDevice;
// 	listDevice.RemoveAll();
// 	pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
	//ѭ������
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if (nStatus == pObj->m_nMarkID)
		{
			sMarkInfo = pObj->m_sMarkInfo;
		}
	}
	return sMarkInfo;
}
/*************************************************************
 �� �� ����SetRunStatus()
 ���ܸ�Ҫ���ı�״̬����״̬
 �� �� ֵ: void
 ��    ����param1	ָ������״̬ 0:���� 1������/���� 2��ͣ�� 3������  4: �����
**************************************************************/
//##ModelId=49B87BA801A8
void CSecObj::SetRunStatus( int nStatus )
{
	//�ж�������Ч��
	if(nStatus<0 || nStatus>4)
		return;
	m_nRunStatu = nStatus;
	//���߳�վ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	CStationObj* pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(pStation != NULL)
		pStation->SecRunChange(this, m_nRunStatu);
}

/*************************************************************
 �� �� ����AddZone()
 ���ܸ�Ҫ�����붨ֵ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��ֵ���Žṹ
		   Param2
**************************************************************/
//##ModelId=49B87BA801B6
BOOL CSecObj::AddZone( PT_ZONE* ptz )
{
	//���������Ч��
	if(ptz == NULL)
		return FALSE;
	//���Ҷ�Ӧ��CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pCpu = (CSecCPU*)m_arrCPU.GetAt(i);
		if(pCpu->code == ptz->cpu)
		{
			//����
			pCpu->m_arrZONE.Add(ptz);
			pCpu->m_bUseSpeZone = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

/*************************************************************
 �� �� ����RefreshCommDurativeTime()
 ���ܸ�Ҫ��ˢ��ͨ��״̬����ʱ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA801B8
void CSecObj::RefreshCommDurativeTime()
{
	//ȡ�õ�ǰʱ��
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewCommStatus;

	if (tmSpan < 0)
	{
		tmSpan = 0;
		CString str;
		str.Format("[%s]״̬����ʱ����ָ���,״̬�仯ʱ��Ϊ%s", m_sID, m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}
	
	m_sCommDurativeTime = tmSpan.Format(GetDurationFormatStr());
	CString str = tmSpan.Format("%D%H%M");
	m_lCommDurative = atol(str);
}

/*************************************************************
 �� �� ����RefreshRunDurativeTime()
 ���ܸ�Ҫ��ˢ������״̬����ʱ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA801B9
void CSecObj::RefreshRunDurativeTime()
{
	//ȡ�õ�ǰʱ��
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewRunStatus;
	
	if (tmSpan < 0)
	{
		tmSpan = 0;
		WriteLog(_T("�����豸����״̬����ʱ����ָ���"));
	}

	m_sRunDurativeTime = tmSpan.Format(GetDurationFormatStr());
	CString str = tmSpan.Format("%D%H%M");
	m_lRunDurative = atol(str);
}

/*************************************************************
 �� �� ����LoadCommStatusFromSTTP()
 ���ܸ�Ҫ����STTP�����ж�ȡͨ��״̬��Ϣ
 �� �� ֵ: 
 ��    ����param1	STTP��������
		   Param2	����
**************************************************************/
//##ModelId=49B87BA801BA
void CSecObj::LoadCommStatusFromSTTP( STTP_FULL_DATA& sttpData, int nIndex )
{
	//���жϱ���ID
	int nMsgID = sttpData.sttp_head.uMsgID;
	if(nMsgID != 20002 && nMsgID != 20084)
	{
		CString str;
		str.Format("CSecObj::LoadCommStatusFromSTTP, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//�ж�����
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//�Ƿ�����
		return;
	}
	
	//״̬
	int nStatus = -1;
	if(nMsgID == 20002)
		nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].id;
	else if(nMsgID == 20084)
		nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].int_reserved;
	
	//�ı�״̬
	SetCommStatus(nStatus);

	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmNewCommStatus = StringToTimeSttp12(strTime);

	//ˢ��״̬����ʱ��
	RefreshCommDurativeTime();
}

/*************************************************************
 �� �� ����LoadRunStatusFromSTTP()
 ���ܸ�Ҫ����STTP�����ж�ȡ����״̬��Ϣ
 �� �� ֵ: 
 ��    ����param1	STTP��������
		   Param2	����
**************************************************************/
//##ModelId=49B87BA801C6
void CSecObj::LoadRunStatusFromSTTP( STTP_FULL_DATA& sttpData, int nIndex )
{
	//���жϱ���ID
	if(sttpData.sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CSecObj::LoadRunStatusFromSTTP, �յ�������, ����Ϊ%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//״̬
	int nStatus = sttpData.sttp_body.nStatus;

	//�ı�״̬
	SetRunStatus(nStatus);
	
	//ʱ��
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmNewRunStatus = StringToTimeSttp12(strTime);

	//ˢ������״̬����ʱ��
	RefreshRunDurativeTime();
}

/*************************************************************
 �� �� ��: RefreshStatus()
 ���ܸ�Ҫ: �ı䱾�豸��״̬Ϊָ���豸��״̬
 �� �� ֵ: 
 ��    ��: param1 ָ���豸
		   Param2 
**************************************************************/
void CSecObj::RefreshStatus( CSecObj* pSec )
{
	if(pSec == NULL)
		return;
	//ͨѶ״̬
	SetCommStatus(pSec->m_nCommStatus);
	m_tmNewCommStatus = pSec->m_tmNewCommStatus;
	RefreshCommDurativeTime();
	//����״̬
	SetRunStatus(pSec->m_nRunStatu);
	m_tmNewRunStatus = pSec->m_tmNewRunStatus;
	RefreshRunDurativeTime();
}

/*************************************************************
 �� �� ��: LoadSimple()
 ���ܸ�Ҫ: �����豸��Ϣ,ֻ���򵥵�����,������������
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ���ݿ�����
		   Param2 
**************************************************************/
BOOL CSecObj::LoadSimple( CMemSet* pMemSet )
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sName = pMemSet->GetValue(2); //����
	CString str;
	str = pMemSet->GetValue(3); //�豸����
	m_nSecType = atoi(str);
	m_sKind = pMemSet->GetValue(4); //��������
	m_sModel = pMemSet->GetValue(8); //�豸�ͺ�
	m_sSysVer = pMemSet->GetValue(9); //�豸�汾��
	str = pMemSet->GetValue(10); //CPU����
	m_nCpuNum = atoi(str); 
	m_sDevTime = pMemSet->GetValue(11);//�豸ʱ��
	m_sCrc = pMemSet->GetValue(12); //�豸У����
	m_sAddr = pMemSet->GetValue(13); //�豸��ַ
	str = pMemSet->GetValue(14); //���ں�
	m_nComID = atoi(str);
	str = pMemSet->GetValue(19); //����״̬
	m_nRunStatu = atoi(str);
	m_sStationID = pMemSet->GetValue(20); //��վID
	m_sOwnerID = pMemSet->GetValue(21); //һ���豸ID
	str = pMemSet->GetValue(22); //һ���豸����
	m_nOwnerType = atoi(str);
	str = pMemSet->GetValue(23); //�����豸����ID
	m_nPTTypeID = atoi(str);
	str = pMemSet->GetValue(24); //��Լ����
	m_nProtocolType = atoi(str);
	str = pMemSet->GetValue(26);	//103��ַ
	m_n103Addr = atoi(str);
	str = pMemSet->GetValue(27); //103���
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(28); //103��Ŀ��
	m_n103Item = atoi(str);
	str = pMemSet->GetValue(36); //ͨѶ״̬
	m_nCommStatus = atoi(str);
	str = pMemSet->GetValue(37); //��ǰͨ��״̬�ı��ʱ��
	m_tmNewCommStatus = StringToTime(str);
	str = pMemSet->GetValue(38); //��ǰ����״̬�ı��ʱ��
	m_tmNewRunStatus = StringToTime(str);
	
	//ȷ���豸����
	switch(m_nSecType)
	{
	case 1: //ĸ���
	case 2: //��ѹ������
	case 3: //��·����
	case 4: //�����鱣��
	case 5: //��·������
	case 6: //�翹������
	case 8: //����������
	case 11: //ĸ������
		m_nType = TYPE_PROTECT;
		break;
	case 9:
		m_nType = TYPE_WAVEREC;
		break;
	default:
		break;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ��: FindCpu()
 ���ܸ�Ҫ: ָ��CPU�Ų���CPU��Ϣ
 �� �� ֵ: CPU��Ϣ
 ��    ��: param1 ָ��CPU��
		   Param2 
**************************************************************/
CSecCPU* CSecObj::FindCpu( int nCpu )
{
	CSecCPU* cpu = NULL;
	//���Ҷ�Ӧ��CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pCpu = (CSecCPU*)m_arrCPU.GetAt(i);
		if(pCpu->code == nCpu)
		{
			cpu = pCpu;
			break;
		}
	}
	return cpu;
}

PT_ActionCharacter* CSecObj::FindActionCharacter( int nID )
{
	EnterCriticalSection(&m_CriticalSection); 
	PT_ActionCharacter *pActChr = NULL;

	for (int i = 0; i < m_arrActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) m_arrActChr[i];
		if ( pActChr && nID == pActChr->nID)
		{
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);

	return pActChr;
}

PT_Signal* CSecObj::FindActionSignal( int nID )
{
	EnterCriticalSection(&m_CriticalSection); 
	PT_Signal* pSignal = NULL;
	for (int i = 0; i < m_arrActionSignal.GetSize(); i++)
	{
		pSignal = (PT_Signal*) m_arrActionSignal[i];
		if( pSignal && nID == pSignal->nID )
			break;
	}
	LeaveCriticalSection(&m_CriticalSection);

	return pSignal;
}

PT_Signal* CSecObj::FindAlarmSignal( int nID )
{
	EnterCriticalSection(&m_CriticalSection);
	PT_Signal* pSignal = NULL;
	for (int i = 0; i < m_arrAlarmSignal.GetSize(); i++)
	{
		pSignal = (PT_Signal*) m_arrAlarmSignal[i];
		if( pSignal && nID == pSignal->nID )
			break;
	}
	LeaveCriticalSection(&m_CriticalSection);

	return pSignal;
}

int CSecObj::GetActionCharacterCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_arrActChr.GetSize();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

int CSecObj::GetActionSignalCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_arrActionSignal.GetSize();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

int CSecObj::GetAlarmSignalCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_arrAlarmSignal.GetSize();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

PT_ActionCharacter* CSecObj::QueryActionCharacter( int arrayIndex )
{
	PT_ActionCharacter* pAC = NULL;
	EnterCriticalSection(&m_CriticalSection);
	if( arrayIndex >= 0 && arrayIndex < m_arrActChr.GetSize() )
		pAC = m_arrActChr.GetAt(arrayIndex);
	LeaveCriticalSection(&m_CriticalSection);
	return pAC;
}

PT_Signal* CSecObj::QueryActionSignal( int arrayIndex )
{
	PT_Signal* pAC = NULL;
	EnterCriticalSection(&m_CriticalSection);
	if( arrayIndex >= 0 && arrayIndex < m_arrActionSignal.GetSize() )
		pAC = m_arrActionSignal.GetAt(arrayIndex);
	LeaveCriticalSection(&m_CriticalSection);
	return pAC;
}

PT_Signal* CSecObj::QueryAlarmSignal( int arrayIndex )
{
	PT_Signal* pAC = NULL;
	EnterCriticalSection(&m_CriticalSection);
	if( arrayIndex >= 0 && arrayIndex < m_arrAlarmSignal.GetSize() )
		pAC = m_arrAlarmSignal.GetAt(arrayIndex);
	LeaveCriticalSection(&m_CriticalSection);
	return pAC;
}

BOOL CSecObj::CanClear()
{
	BOOL bReturn = FALSE;
	EnterCriticalSection(&m_CriticalSection);
	if( m_arrActChr.GetSize() > 0 || m_arrActionSignal.GetSize() > 0 || m_arrAlarmSignal.GetSize() > 0)
	{
		CTime tm = CTime::GetCurrentTime(); 
		CTimeSpan ts = tm - m_tmLastActive; 
		if(ts.GetTotalSeconds() >= SECCONFIG_CLEARTIME) 
		{
			bReturn =  TRUE;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
	return bReturn;
}

//CSecObj	end
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSecCPU, CDeviceObj)
//////////////////////////////////////////////////////////////////////////
//CSecCPU	CPU��
//##ModelId=49B87BA80215
CSecCPU::CSecCPU()
{
	PT_ID = "";
	code = -1;
	des = "";
	ver = "";
	from_zone = -1;
	to_zone = -1;
	fun = -1;
	m_bUseSpeZone = FALSE;
	m_nType = TYPE_CPU;
	m_arrZONE.RemoveAll();
	/** @brief           ��CPU�¶�ֵ��������*/
	m_nSettingDefs = 0;
	/** @brief           ��CPU�¿�������������*/
	m_nDigitalDefs = 0;
	/** @brief           ��CPU����ѹ���������*/
	m_nSoftBoardDefs = 0;
	/** @brief           ��CPU��ģ������������*/
	m_nAnalogDefs = 0;
}

//##ModelId=49B87BA80222
CSecCPU::~CSecCPU()
{
	//���ZONE����
	ClearZone();
}

/*************************************************************
 �� �� ����ClearZone()
 ���ܸ�Ҫ����ն�ֵ��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA80242
BOOL CSecCPU::ClearZone()
{
	for(int i = 0; i < m_arrZONE.GetSize(); i++)
	{
		delete m_arrZONE[i];
	}
	m_arrZONE.RemoveAll();
	
	return TRUE;
}

/*************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݿ��ж�������, tb_secdev_cpu_base
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   ���ݼ�ָ��
**************************************************************/
//##ModelId=49B87BA80238
BOOL CSecCPU::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		PT_ID = pMemSet->GetValue((UINT)0);
		CString str;
		str = pMemSet->GetValue(1); //cpu_code
		code = atoi(str);
		des = pMemSet->GetValue(2); //cpu_des
		ver = pMemSet->GetValue(3); //cpu_ver
		str = pMemSet->GetValue(4); //from_zone
		from_zone = atoi(str);
		str = pMemSet->GetValue(5); //to_zone
		to_zone = atoi(str);
		str = pMemSet->GetValue(6); //fun
		fun = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}
// 	if(!GetZone())
// 	{
// 		//��ȡ��ֵ����ʧ�ܻ�û��ָ���Ķ�ֵ����
// 		m_bUseSpeZone = FALSE;
// 	}
// 	else
// 	{
// 		m_bUseSpeZone = TRUE;
// 	}
	return TRUE;
}

/*************************************************************
 �� �� ����GetZone()
 ���ܸ�Ҫ�������ݿ���Ҷ�Ӧ�Ķ�ֵ����
 �� �� ֵ: ���ҳɹ�����TRUE, ʧ�ܻ������ݷ���FALSE
**************************************************************/
//##ModelId=49B87BA8023A
BOOL CSecCPU::GetZone()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "Zone_CODE";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//char_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "Zone_State";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", PT_ID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_Code = %d", code);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecCPU::GetZone, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			bReturn = FALSE;
		}
		for(int i = 0; i < nCount; i++)
		{
			//����
			PT_ZONE * sign = new PT_ZONE;
			CString str;
			str = pMemset->GetValue((UINT)0); //zone_code
			sign->code = atoi(str);
			str = pMemset->GetValue(1); //zone_state
			sign->state = atoi(str);

			m_arrZONE.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
				
		CString str;
		str.Format("CSecCPU::GetZone,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

//SecCPU end
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMark, CDeviceObj)
//////////////////////////////////////////////////////////////////////////
//CSecCPU	CPU��
//##ModelId=49B87BA80215
CMark::CMark()
{
	m_sMarkInfo = "";
	m_nMarkID = -1;
}

//##ModelId=49B87BA80222
CMark::~CMark()
{
	//��չ�����Ϣ����
	//ClearMarkInfo();
}
// /*************************************************************
//  �� �� ����ClearZone()
//  ���ܸ�Ҫ����ն�ֵ��������
//  �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
// **************************************************************/
// //##ModelId=49B87BA80242
// BOOL CMark::ClearMarkInfo()
// {
// 	for(int i = 0; i < m_arrZONE.GetSize(); i++)
// 	{
// 		delete m_arrZONE[i];
// 	}
// 	m_arrZONE.RemoveAll();
// 	
// 	return TRUE;
// }

/*************************************************************
 �� �� ����Load()
 ���ܸ�Ҫ�������ݿ��ж�������, tb_suspend_reason_def
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   ���ݼ�ָ��
**************************************************************/
//##ModelId=49B87BA80238
BOOL CMark::Load(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0);
		m_nMarkID = atoi(str);
		m_sMarkInfo = pMemSet->GetValue(1); 
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}
//CMark	end
//////////////////////////////////////////////////////////////////////////