// PushMsgProcess.cpp: implementation of the CPushMsgProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xjbrowser.h"
#include "PushMsgProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPushMsgProcess::CPushMsgProcess()
{
	m_bExit = FALSE;
	m_listFullData.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
}

CPushMsgProcess::~CPushMsgProcess()
{
	ClearSttpData();
	DeleteCriticalSection(&m_CriticalSection);
}

int CPushMsgProcess::AddSttpData( STTP_FULL_DATA* pSttpData )
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	m_listFullData.AddTail(pSttpData);
	nCount = m_listFullData.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

int CPushMsgProcess::RemoveFirstSttpData()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	POSITION pos = m_listFullData.GetHeadPosition();
	STTP_FULL_DATA* pSttp = NULL;
	if(pos != NULL)
	{
		STTP_FULL_DATA* pSttp = (STTP_FULL_DATA*)m_listFullData.GetHead();
		if(pSttp)
		{
			zero_sttp_full_data(*pSttp);
			delete pSttp;
			pSttp = NULL;
		}
		m_listFullData.RemoveHead();
	}
	nCount = m_listFullData.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

STTP_FULL_DATA* CPushMsgProcess::GetFirstSttpData()
{
	STTP_FULL_DATA* pSttp = NULL;
	EnterCriticalSection(&m_CriticalSection);
	POSITION pos = m_listFullData.GetHeadPosition();
	if(pos != NULL)
	{
		pSttp = (STTP_FULL_DATA*)m_listFullData.GetHead();
	}
	LeaveCriticalSection(&m_CriticalSection);
	return pSttp;
}

void CPushMsgProcess::ClearSttpData()
{
	EnterCriticalSection(&m_CriticalSection);
	POSITION pos = m_listFullData.GetHeadPosition();
	while(pos != NULL)
	{
		STTP_FULL_DATA* pSttp = (STTP_FULL_DATA*)m_listFullData.GetNext(pos);
		if(pSttp)
		{
			zero_sttp_full_data(*pSttp);
			delete pSttp;
			pSttp = NULL;
		}
	}
	m_listFullData.RemoveAll();
	LeaveCriticalSection(&m_CriticalSection);
}

void CPushMsgProcess::TranslateSttpData()
{
	STTP_FULL_DATA* pSttp = GetFirstSttpData();
	if(!pSttp)
		return;
	int nmsgid = pSttp->sttp_head.uMsgID;
	CString strLog;
	strLog.Format("��Ϣ��������,ȡ�����ױ���:%d", pSttp->sttp_head.uMsgID);
	WriteLog(strLog, XJ_LOG_LV3);
	switch(pSttp->sttp_head.uMsgID)
	{
	case 20002:
		Translate20002(pSttp);
		break;
	case 20010:
		Translate20010(pSttp);
		break;
	case 20018:
		Translate20018(pSttp);
		break;
	case 20022:
		Translate20022(pSttp);
		break;
	case 20079:
		Translate20079(pSttp);
		break;
	case 20084:
		Translate20084(pSttp);
		break;
	case 20132:
		Translate20132(pSttp);
		break;
	case 20138:
		Translate20138(pSttp);
		break;
	case 20144:
		Translate20144(pSttp);
		break;
	case 20151:
		Translate20151(pSttp);
		break;
	case 20155:
		Translate20155(pSttp);
		break;
	case 20168:
		Translate20168(pSttp);
		break;
	case 20176:
		Translate20176(pSttp);
		break;
	default:
		strLog.Format("��Ϣ��������,����:%d û��Ԥ����Ľ�����ʽ��ֱ�ӷ��������", pSttp->sttp_head.uMsgID);
		int nMessage = (pSttp->sttp_head.uMsgID - 20000) + 2000 + WM_USER;
		AfxGetApp()->GetMainWnd()->SendMessage(nMessage, 0, (LPARAM)pSttp);
		WriteLog(strLog, XJ_LOG_LV3);
		break;
	}
	int nCount = RemoveFirstSttpData();
	strLog.Format("��Ϣ��������,���ױ���%d�������,������ʣ�౨��:%d", nmsgid, nCount);
	WriteLog(strLog, XJ_LOG_LV3);
}

void CPushMsgProcess::Translate20002( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(!pData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20002)
		return;
	
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	
	for(int i = 0; i < nCount; i++)
	{
		if(!ThreadContinueRun())
			break;

		//ȡ��PT_ID
		CString sPTID = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		//ȡ�ö���
		CSecObj* pSec = (CSecObj*)pData->FindDevice(sPTID, TYPE_SEC);
		if(pSec == NULL)
			continue;
		
		//��������
		pSec->LoadCommStatusFromSTTP(*pSttpData, i);
		if(!ThreadContinueRun())
			break;
		//����
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20002, 0, (LPARAM)pSec);
		Sleep(2);
	}
}

void CPushMsgProcess::Translate20010( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;

	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20010)
		return;
	//�½��¼�
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		if(!ThreadContinueRun())
			break;
		CXJEventDI tempevent;
		if(!tempevent.LoadFromSttp(*pSttpData, i)) //ָ������
		{
			//��ȡ����ʧ��
			continue;
		}
		if(!ThreadContinueRun())
			break;
		//����
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20010, 0, (LPARAM)(&tempevent));
		Sleep(2);
	}
}

void CPushMsgProcess::Translate20018( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20018)
		return;
	//�½��¼�
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		if(!ThreadContinueRun())
			break;
		CXJEventAlarm tempEvent;
		if(!tempEvent.LoadFromSttp(*pSttpData, i)) //ָ������
		{
			//��ȡ����ʧ��
			continue;
		}
		if(!ThreadContinueRun())
			break;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20018, 0, (LPARAM)(&tempEvent));
		Sleep(2);
	}
}

void CPushMsgProcess::Translate20022( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20022)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventAction tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) //�����¼�����ָ������
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20022, 0, (LPARAM)(&tempEvent));
	}
}

void CPushMsgProcess::Translate20079( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(!pData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20079)
		return;
	//�����豸
	CString sSecID = pSttpData->sttp_body.ch_pt_id;
	CSecObj* pSec = (CSecObj*)pData->FindDevice(sSecID, TYPE_SEC);
	if(pSec == NULL)
		return;
	if(!ThreadContinueRun())
		return;
	//��������
	pSec->LoadRunStatusFromSTTP(*pSttpData, 0);
	if(!ThreadContinueRun())
		return;
	AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20079, 0, (LPARAM)pSec);
}

void CPushMsgProcess::Translate20084( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(!pData)
		return;

	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20084)
		return;
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	
	for(int i = 0; i < nCount; i++)
	{
		//ȡ��PT_ID
		CString sPTID = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		//ȡ�ö���
		CSecObj* pSec = (CSecObj*)pData->FindDevice(sPTID, TYPE_SEC);
		if(pSec == NULL)
			continue;
		if(!ThreadContinueRun())
			return;
		//��������
		pSec->LoadCommStatusFromSTTP(*pSttpData, i);
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20084, 0, (LPARAM)pSec);
		Sleep(2);
	}
}

void CPushMsgProcess::Translate20132( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;

	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20132)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventOSC  tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) //¼���¼�����ָ������
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20132, 0, (LPARAM)(&tempEvent));
	}
}

void CPushMsgProcess::Translate20138( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20138)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventSysAlarm  tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) //����ָ������
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20138, 0, (LPARAM)(&tempEvent));
	}
}

void CPushMsgProcess::Translate20144( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(!pData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20144)
		return;
	//�õ���Ӧװ��
	CString strID = pSttpData->sttp_body.ch_station_id;
	//���ҳ�վ
	CStationObj * pObj = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
	if(!pObj)
		return;
	//ˢ�³�վ״̬
	pObj->LoadStatusFromSTTP(pSttpData);

	if(!ThreadContinueRun())
		return;
	AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20144, 0, (LPARAM)(pObj));
}

void CPushMsgProcess::Translate20151( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20151)
		return;
	if(!ThreadContinueRun())
		return;
	//�������¼�
	CFaultReport tempReport;
	if(tempReport.LoadFromSTTP(*pSttpData))
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20151, 0, (LPARAM)(&tempReport));
	}
}

void CPushMsgProcess::Translate20155( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20155)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventSysAlarm  tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) //����ָ������
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20155, 0, (LPARAM)(&tempEvent));
	}
}

void CPushMsgProcess::Translate20168( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20168)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventDistance  tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) 
	{
		if(!ThreadContinueRun())
		 return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20168, 0, (LPARAM)(&tempEvent));
	}
}

void CPushMsgProcess::Translate20176( STTP_FULL_DATA* pSttpData )
{
	if(!pSttpData)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20176)
		return;
	if(!ThreadContinueRun())
		return;
	//�½��¼�
	CXJEventSysAlarm  tempEvent;
	if(tempEvent.LoadFromSttp(*pSttpData, 0)) //����ָ������
	{
		if(!ThreadContinueRun())
			return;
		AfxGetApp()->GetMainWnd()->SendMessage(WM_STTP_20176, 0, (LPARAM)(&tempEvent));
	}
}

BOOL CPushMsgProcess::IsAutoPushMsg(int uMsgID)
{
	switch(uMsgID)
	{
	case 20002:
	case 20010:
	case 20018:
	case 20022:
	case 20079:
	case 20084:
	case 20132:
	case 20138:
	case 20144:
	case 20151:
	case 20155:
	case 20168:
	case 20176:
	case 20177:
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

BOOL CPushMsgProcess::IsAutoNotifyMsg( int uMsgID )
{
	switch(uMsgID)
	{
	case 20001:
	case 20009:
	case 20017:
	case 20021:
	case 20081:
	case 20083:
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

