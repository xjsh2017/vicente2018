// FaultEvent.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "FaultEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFaultEvent

IMPLEMENT_DYNCREATE(CFaultEvent, CObject)

//##ModelId=49B87B9A01C7
CFaultEvent::CFaultEvent()
{
	m_strUnit = "";
	/** @brief           ���ϱ����¼�����*/
	m_nType = 0;
	/** @brief           �������е�ID*/
	m_nID = 0;
	m_nMainRecID = 0;
	/** @brief           �����豸ID*/
	m_sSecID = "";
	/** @brief           �¼�����*/
	m_sEventDef = "";
	/** @brief           �¼�ֵ*/
	m_sEventContent = "";
	/** @brief           �¼���������*/
	m_sEventDefName = "";
	m_nmsStart = 0;
	m_nmsReceive = 0;
	m_nmsSign = 0;
	m_nCPU = -1;
	m_sDir = _T("");

	CTime tm(1972, 1, 1, 1, 1,1);
	m_tmStart = tm;
	m_tmSign = tm;
	m_tmReceiveTime  = tm;

	m_pSec = NULL;

	m_bSelected = FALSE;
	m_nNum = -1;
	
	m_sReceiveTime = "";
	m_sSignaltime = "";
	m_nWaveFileNo = 0;
	m_sWaveDescribe = "";
	m_sReverse2 = "";
	m_sReverse3 = "";
}

//##ModelId=49B87B9A01C8
CFaultEvent::~CFaultEvent()
{
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ�������ݿ��ж�ȡ�¼���Ϣ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݼ�ָ��
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FA
BOOL CFaultEvent::LoadFromDB( CMemSet* pMemset )
{
	//�ж�������Ч��
	if(pMemset == NULL)
		return FALSE;
	CString str;
	str = pMemset->GetValue((UINT)0);
	m_nID = atoi(str); //ID
	str = pMemset->GetValue(1);
	m_nMainRecID = atoi(str); //MainRecID
	m_sSecID = pMemset->GetValue(2); //PTID
	str = pMemset->GetValue(3); 
	m_nType = atoi(str);//�¼�����
	str = pMemset->GetValue(4);//����ʱ��

	if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
	{
		//¼���򱨻�¼���б�
		m_tmStart = StringToTimeSttp26(str, m_nmsStart);
	}
	else
	{
		m_tmStart = StringToTimeSttp15(str, m_nmsStart);
	}

	str = pMemset->GetValue(5);//����ʱ��
	
	if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
	{
		//¼���򱨻�¼���б�
		m_tmReceiveTime = StringToTimeSttp26(str, m_nmsReceive);
	}
	else if(m_nType == XJ_FAULT_EVENT_OSCPARSE)
	{
		m_sReceiveTime = str;
	}
	else
	{
		m_tmReceiveTime = StringToTimeSttp15(str, m_nmsReceive);
	}
	m_sEventDef = pMemset->GetValue(6);//�¼�����
	str = pMemset->GetValue(7); //�¼�ֵ
	m_sEventContent = ComplexExchange(str); //�������Ǹ��������
	str = pMemset->GetValue(8); //�ź�ʱ��
	if(m_nType == XJ_FAULT_EVENT_SING||m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		m_tmSign = StringToTimeSttp15(str, m_nmsSign);
	}
	else
		m_sSignaltime = str;

	m_nWaveFileNo = atoi(pMemset->GetValue(9));//wavefileno
	m_sWaveDescribe = pMemset->GetValue(10);
	m_sReverse2 = pMemset->GetValue(13);
	m_sReverse3 = pMemset->GetValue(14);

	//�����豸
	if(!BuildReleation() && (m_nType != XJ_FAULT_EVENT_WAVEDISTANCE && m_nType != XJ_FAULT_EVENT_DISTANCE))
	{
		WriteLog("CFaultEvent::LoadFromDB, �������豸ʧ��");
		return FALSE;
	}
	if(m_pSec != NULL && m_nType == XJ_FAULT_EVENT_OSCREPORT)
	{
		m_sEventDef.Replace("/", "\\");
		DelPostfixFromPath(m_sEventDef);
		if(m_sEventDef.Find("\\", 0) == -1)
		{
			if(m_pSec->m_nType == TYPE_PROTECT)
				m_sEventDef = m_pSec->m_sStationID + "\\����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\"+m_sEventDef;
			else if(m_pSec->m_nType == TYPE_WAVEREC)
				m_sEventDef = m_pSec->m_sStationID + "\\¼����\\" + m_pSec->m_sName + "(" + m_pSec->m_sID + ")" + "\\"+m_sEventDef;
		}
	}
	//���Ҷ�������
	if(!FindEventDefName())
	{
		CString str;
		str.Format("CFaultEvent::LoadFromDB, �¼��������Ϣ, �¼�����Ϊ%d", m_nType);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildReleation()
 ���ܸ�Ҫ���������豸�Ĺ���
 �� �� ֵ: �ܹ������豸����TRUE, ���ܹ������豸����FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FC
BOOL CFaultEvent::BuildReleation()
{
	//�ж�������Ч��
	if(m_sSecID == "")
		return FALSE;
	//�����豸
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	m_pSec = (CSecObj*)pData->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL) //���Ҳ����豸,ʧ��
		return FALSE;
	return TRUE;
}

/*************************************************************
 �� �� ����FindEventDefName()
 ���ܸ�Ҫ�������¼����������
 �� �� ֵ: ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FD
BOOL CFaultEvent::FindEventDefName()
{
	//�ж�������Ч��
	if(m_nType == XJ_FAULT_EVENT_WAVEDISTANCE || m_nType == XJ_FAULT_EVENT_OSCPARSE || m_nType == XJ_FAULT_EVENT_DISTANCE)
		return TRUE;

	if(m_pSec == NULL)
		return FALSE;
	if(m_sEventDef == "")
		return FALSE;
	
	if(m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		m_sEventDefName = QueryDIName(m_sSecID, atoi(m_sEventDef), m_nWaveFileNo);
		return TRUE;
	}
	//ˢ�±�������
	m_pSec->RefreshConfig();
	int nEventDef = atoi(m_sEventDef);
	if(m_nType == XJ_FAULT_EVENT_CHR)
	{
		//����������, ���ڱ�������
		PT_ActionCharacter* ptChr = GetActChrsConfig(nEventDef);
		if(ptChr == NULL)
		{
			//�Ҳ���, ��ͨ������
			ptChr = GetActChrsFromUniversal(nEventDef);
		}
		if(ptChr == NULL)
			return FALSE;
		m_sEventDefName = ptChr->strName;
		m_strUnit = ptChr->strUnits;
	}
	if(m_nType == XJ_FAULT_EVENT_SING)
	{
		PT_Signal * pts = GetSignalConfig(nEventDef);
		if(pts == NULL)
			return FALSE;
		m_sEventDefName = pts->strName;
	}
	if(m_nType == XJ_FAULT_EVENT_ALARM)
	{
		PT_Signal * pts = GetAlarmConfig(nEventDef);
		if(pts == NULL)
			return FALSE;
		m_sEventDefName = pts->strName;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����GetActChrsFromUniversal()
 ���ܸ�Ҫ����ͨ���������в�������������
 �� �� ֵ: ���ҵ��򷵻�������ָ��, δ�ҵ�����NULL
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
//##ModelId=49B87B9A0207
PT_ActionCharacter * CFaultEvent::GetActChrsFromUniversal(int nID)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp ->GetDataEngine();
	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < pData->m_arrUniversalActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) pData->m_arrUniversalActChr[i];
		if (nID == pActChr->nID)
		{
			return pActChr;
		}
	}
	
	return NULL;
}

/*************************************************************
 �� �� ����MultiSaveData()
 ���ܸ�Ҫ����֯������������
 �� �� ֵ: ��֯�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݼ�¼
		   Param2
**************************************************************/
//##ModelId=49B87B9A01FE
BOOL CFaultEvent::MultiSaveData( STTP_DBDATA_RECORD& sttpDB )
{
	try
	{
		//MainRecID
		STTP_DBDATA_UNIT field;
		CString str;
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nMainRecID);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//����ID
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sSecID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventType
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nType);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//FaultStartTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			str.Format("%s.%03d", m_tmStart.Format("%Y-%m-%d %H:%M:%S"), m_nmsStart);
		}
		else
		{
			str.Format("%s%03d", m_tmStart.Format("%y%m%d%H%M%S"), m_nmsStart);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//FaultReceiveTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			str.Format("%s.%03d", m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), m_nmsReceive);
		}
		else if(m_nType == XJ_FAULT_EVENT_OSCPARSE)
		{
			str = m_sReceiveTime;
		}
		else
		{
			str.Format("%s%03d", m_tmReceiveTime.Format("%y%m%d%H%M%S"), m_nmsReceive);
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventDef
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sEventDef;
		if(m_nType == XJ_FAULT_EVENT_OSCREPORT || m_nType == XJ_FAULT_EVENT_OSCLIST)
		{
			//�޸�·��б��
			str.Replace("\\", "/");
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//EventContent
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sEventContent;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//SignalTime
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		if(m_nType == XJ_FAULT_EVENT_SING||m_nType == XJ_FAULT_EVENT_OSCPARSE||m_nType == XJ_FAULT_EVENT_DICHANGE)
		{
			str.Format("%s%03d", m_tmSign.Format("%y%m%d%H%M%S"), m_nmsSign);
		}
		else
		{
			str = m_sSignaltime;
		}
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//wavefileno
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nWaveFileNo);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//wavedescribe
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sWaveDescribe;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//reverse2
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sReverse2;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//reverse3
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_sReverse3;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
	}
	catch (...)
	{
		WriteLog("CFaultEvent::MultiSaveData, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����Clone()
 ���ܸ�Ҫ�����ƹ����¼�
 �� �� ֵ: �������ɵĹ����¼�
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0204
CFaultEvent* CFaultEvent::Clone()
{
	CFaultEvent* pEvent = new CFaultEvent;
	pEvent->m_nID			= m_nID;
	pEvent->m_nMainRecID	= m_nMainRecID;
	pEvent->m_nmsReceive	= m_nmsReceive;
	pEvent->m_nmsSign		= m_nmsSign;
	pEvent->m_nmsStart		= m_nmsStart;
	pEvent->m_nType			= m_nType;
	pEvent->m_pSec			= m_pSec;
	pEvent->m_sEventContent	= m_sEventContent;
	pEvent->m_sEventDef		= m_sEventDef;
	pEvent->m_sEventDefName	= m_sEventDefName;
	pEvent->m_sSecID		= m_sSecID;
	pEvent->m_tmReceiveTime	= m_tmReceiveTime;
	pEvent->m_tmSign		= m_tmSign;
	pEvent->m_tmStart		= m_tmStart;
	pEvent->m_strUnit		= m_strUnit;
	pEvent->m_nCPU			= m_nCPU;
	pEvent->m_nNum			= m_nNum;
	pEvent->m_sDir          = m_sDir;
	pEvent->m_sOldDir       = m_sOldDir;
	pEvent->m_sReceiveTime	= m_sReceiveTime;
	pEvent->m_sSignaltime	= m_sSignaltime;
	pEvent->m_nWaveFileNo	= m_nWaveFileNo;
	pEvent->m_sWaveDescribe	= m_sWaveDescribe;
	pEvent->m_sReverse2		= m_sReverse2;
	pEvent->m_sReverse3		= m_sReverse3;
	return pEvent;
}

/*************************************************************
 �� �� ��: GetFaultType()
 ���ܸ�Ҫ: ���������ͽ��ͳ�����
 �� �� ֵ: ���ͺ������
 ��    ��: param1 ��������
		   Param2 
**************************************************************/
CString CFaultEvent::GetFaultType( CString sType )
{
	CString strTemp;
	sType.MakeUpper();
	if ((sType == "AG") || (sType == "AN") || (sType == "A"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_A);
	}
	else if ((sType == "BG") || (sType == "BN") || (sType == "B"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_B);
	}
	else if ((sType == "CG") || (sType == "CN") || (sType == "C"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_C);
	}
	else if ((sType == "AB") || (sType == "BA"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_AB);
	}
	else if ((sType == "BC") || (sType == "CB"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_BC);
	}
	else if ((sType == "CA") || (sType == "AC"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_CA);
	}
	else if ((sType == "ABG") || (sType == "ABN") || (sType == "BAG") || (sType == "BAN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABN);
	}
	else if ((sType == "BCG") || (sType == "BCN") || (sType == "CBG") || (sType == "CBN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_BCN);
	}
	else if ((sType == "CAG") || (sType == "CAN") || (sType == "ACG") || (sType == "ACN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_CAN);
	}
	else if (sType == "ABC")
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABC);
	}
	else if ((sType == "ABCG") || (sType == "ABCN"))
	{
		strTemp = StringFromID(IDS_FAULTTYPE_ABCN);
	}	
	else
	{
		strTemp = sType;
	}
	
	return strTemp;
}

/*************************************************************
 �� �� ��: IsReturnEvent()
 ���ܸ�Ҫ: �Ƿ񷵻���Ϣ
 �� �� ֵ: �Ƿ�����Ϣ����TRUE,���Ƿ�����Ϣ����FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CFaultEvent::IsReturnEvent()
{
	int nValue = atoi(m_sEventContent);
	if(nValue == 0)
		return TRUE;
	return FALSE;
}

/****************************************************
Date:2011/10/14  Author:LYH
������:   GetEventProperty	
����ֵ:   int	�¼��ȼ�,Ĭ�Ϸ���0��0-I�࣬1-II�࣬2-III��
���ܸ�Ҫ: ȡ���¼��ȼ�
*****************************************************/
int CFaultEvent::GetEventProperty()
{
	//�ж�������Ч��
	if(m_pSec == NULL)
		return 0;
	if(m_sEventDef == "")
		return 0;
	int nProperty = 0;
	//ˢ�±�������
	m_pSec->RefreshConfig();
	int nEventDef = atoi(m_sEventDef);
	if(m_nType == XJ_FAULT_EVENT_CHR)
	{
		//����������, ���ڱ�������
		PT_ActionCharacter* ptChr = GetActChrsConfig(nEventDef);
		if(ptChr == NULL)
		{
			//�Ҳ���, ��ͨ������
			ptChr = GetActChrsFromUniversal(nEventDef);
		}
		if(ptChr == NULL)
			return 0;
		nProperty = ptChr->nEventType;
	}
	if(m_nType == XJ_FAULT_EVENT_SING)
	{
		PT_Signal * pts = GetSignalConfig(nEventDef);
		if(pts == NULL)
			return 0;
		nProperty = pts->nEventType;
	}
	if(m_nType == XJ_FAULT_EVENT_ALARM)
	{
		PT_Signal * pts = GetAlarmConfig(nEventDef);
		if(pts == NULL)
			return 0;
		nProperty = pts->nEventType;
	}
	return nProperty;
}