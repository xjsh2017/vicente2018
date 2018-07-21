// DlgEventProp.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgEventProp.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CDlgEventProp dialog


//##ModelId=49B87BA40281
CDlgEventProp::CDlgEventProp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEventProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEventProp)
	m_strDescribe = _T("");
	m_strEventClass = _T("");
	m_strSource = _T("");
	m_strTime = _T("");
	m_strRecord = _T("");
	//}}AFX_DATA_INIT
	m_pFrame = NULL;
	m_pEventManager = NULL;
	m_bPTEvent = FALSE;
}

/*************************************************************
 �� �� ����CDlgEventProp()
 ���ܸ�Ҫ����ģʽ���캯��,������ģʽ����µĳ�ʼ��
 �� �� ֵ: 
 ��    ����param1	�����ߴ���ָ��, ������Ϣ����
**************************************************************/
//##ModelId=49B87BA40283
CDlgEventProp::CDlgEventProp( CFrameWnd* pFrame )
{
	m_pFrame = pFrame;
	m_pEventManager = NULL;
	m_bPTEvent = FALSE;
}
//##ModelId=49B87BA40292
void CDlgEventProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEventProp)
	DDX_Text(pDX, IDC_EDT_DESCRIBE, m_strDescribe);
	DDX_Text(pDX, IDC_EDT_EVENTCLASS, m_strEventClass);
	DDX_Text(pDX, IDC_EDT_SOURCE, m_strSource);
	DDX_Text(pDX, IDC_EDT_TIME, m_strTime);
	DDX_Text(pDX, IDC_TXT_RECORD, m_strRecord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEventProp, CDialog)
	//{{AFX_MSG_MAP(CDlgEventProp)
	ON_BN_CLICKED(IDC_BTN_ACK, OnBtnAck)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPT_PASSIVE_ACK, OnEventPassiveAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEventProp message handlers

//##ModelId=49B87BA40295
void CDlgEventProp::OnBtnAck() 
{
	// TODO: Add your control notification handler code here
	CXJEvent *pEvent=m_pEventManager->m_pCurrentEvent;

	BOOL bSucess=pEvent->Ack();
	if(bSucess)
	{
		GetDlgItem(IDC_BTN_ACK)->EnableWindow(FALSE);
		SetWindowText( StringFromID(IDS_EVENT_PROPERTY));//������ȷ�ϱ�־
		if(m_bPTEvent == FALSE)
		{
			//������Ϣ�������
			if(m_pFrame != NULL)
				m_pFrame->PostMessage(WM_EVENTPROP_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
		else
		{
			//������Ϣ�������
			if(m_pFrame != NULL)
				m_pFrame->PostMessage(WM_EVENTPROP_ACK_PT, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
		UpdateControls();
	}
	else
	{
		GetDlgItem(IDC_BTN_ACK)->EnableWindow(TRUE);
		AfxMessageBox( StringFromID(IDS_EVENT_ACK_FAIL));//ȷ���¼�ʧ��
	}
}

//##ModelId=49B87BA40297
void CDlgEventProp::OnBtnCopy() 
{
	// TODO: Add your control notification handler code here
	if(OpenClipboard())
	{
		CString str=m_strDescribe; 
		
		//����str��������
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, str.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,str);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
}

//##ModelId=49B87BA402A0
void CDlgEventProp::OnBtnNext() 
{
	// TODO: Add your control notification handler code here
	//ѭ������������ĩβ��ص���ͷ
	if(!m_pEventManager->IsEOF())
	{
		m_pEventManager->MoveNext();
	}
	else
	{
		if(AfxMessageBox( StringFromID(IDS_EVENT_CIRCLE_END),MB_YESNO|MB_ICONQUESTION)==IDYES)
			m_pEventManager->MoveFirst();
	}
	UpdateControls();
}

//##ModelId=49B87BA402A2
void CDlgEventProp::OnBtnPrev() 
{
	// TODO: Add your control notification handler code here
	//ѭ��������������ͷ��ص�ĩβ
	if(!m_pEventManager->IsBOF())
	{
		m_pEventManager->MovePrev();
	}
	else
	{
		if(AfxMessageBox( StringFromID(IDS_EVENT_CIRCLE_FRONT),MB_YESNO|MB_ICONQUESTION)==IDYES)
			m_pEventManager->MoveLast();
	}
	UpdateControls();
}

//##ModelId=49B87BA402A4
BOOL CDlgEventProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDT_DESCRIBE);
	pEdit->FmtLines(TRUE);// Add soft line-break breaks.
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ����UpdateControls()
 ���ܸ�Ҫ�������¼�����ҳ�Ͽؼ������ݣ���Initdialog��Next��Prevʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA402BC
void CDlgEventProp::UpdateControls()
{
	if(m_pEventManager == NULL)
		return;
	CXJEvent* pEvent=m_pEventManager->m_pCurrentEvent;	//��ǰ�¼�
	//"Next""Prev""ȷ���¼�"��ť��Ч��
	//	GetDlgItem(IDC_BTN_PREV)->EnableWindow(!m_pEventManager->IsBOF());
	//	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(!m_pEventManager->IsEOF());
	GetDlgItem(IDC_BTN_ACK)->EnableWindow(pEvent->m_nIsAck==0);
	
	//��ʾ��ǰ��¼���ڵ�λ��
	m_strRecord.Format("%s:%d��%s:%d", StringFromID(IDS_EVENTCOUNT_CURRENT),m_pEventManager->GetCurrentPos()+1,
		StringFromID(IDS_EVENTCOUNT_TOTAL),m_pEventManager->GetCount());
	
	//����event������		
	//�¼��Ƿ�ȷ��(���Ĵ���Caption,������"ȷ���¼�����Ч��")
	if(pEvent->m_nIsAck==0)
		SetWindowText( StringFromID(IDS_EVENT_PROP_NOTACK));
	else
		SetWindowText( StringFromID(IDS_EVENT_PROPERTY));
	
	//�¼�����
	switch (pEvent->m_nEventType)
	{
	case XJ_EVENT_ACTION:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_ACTION);
		break;
	case XJ_EVENT_ALARM:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_ALARM);
		break;
	case XJ_EVENT_SYSALARM:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_SYSALARM);
		break;
	case XJ_EVENT_DI:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_DI);
		break;
	case XJ_EVENT_OSC:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_OSC);
		break;
	case XJ_EVENT_FAULT:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_FAULT);
		break;
	case XJ_EVENT_PTCOMM:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_PTCOMM);
		break;
	case XJ_EVENT_PTRUN:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_PTRUN);
		break;
	case XJ_EVENT_DISTANCE:
		m_strEventClass = StringFromID(IDS_EVENTTYPE_DISTANCE);
		break;
	default:
		break;
	}
	
	//�Ƿ������Ϣ
	if (pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
		m_strEventClass += "["+ StringFromID(IDS_RUNSTATUS_TESTING) +"]";
	else
	{
		;
	}
	
	//����ʱ��
	m_strTime = pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
	
	//�¼���Դ([XX��վ][XX�豸][XX����װ��])
	if(pEvent->m_nEventType == XJ_EVENT_DISTANCE)
	{
		CXJEventDistance* pDistance = (CXJEventDistance*)pEvent;
		m_strSource.Format("[%s]", pDistance->m_sLineName);
	}
	else
		m_strSource.Format("[%s][%s][%s]",pEvent->m_sStationName,pEvent->m_sDeviceName, pEvent->m_sSecName);
	
	//�¼�������
	m_strDescribe = pEvent->GetDescribe();
	
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����SetEventmanager()
 ���ܸ�Ҫ�������¼��������
 �� �� ֵ: void
 ��    ����param1	�¼��������ָ��
**************************************************************/
//##ModelId=49B87BA402CF
void CDlgEventProp::SetEventmanager( CXJEventManager* pEventManager )
{
	m_pEventManager = pEventManager;
	UpdateControls();
}

//##ModelId=49B87BA40285
BOOL CDlgEventProp::Create() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(CDlgEventProp::IDD);
}

//##ModelId=49B87BA402A6
void CDlgEventProp::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

//##ModelId=49B87BA402A8
void CDlgEventProp::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

//##ModelId=49B87BA402B0
void CDlgEventProp::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_pFrame != NULL)
		m_pFrame->SendMessage(WM_EVENTPROP_CLOSE, 0, 0);
//	CDialog::OnCancel();
}

//##ModelId=49B87BA402B2
void CDlgEventProp::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pFrame != NULL)
		m_pFrame->SendMessage(WM_EVENTPROP_CLOSE, 0, 0);
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ�¼���ȷ���¼���Ϣ,ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BA402B4
void CDlgEventProp::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	if(m_bPTEvent)
	{
		//�����Ǳ�������ҳ��򿪵��¼����ԶԻ���,�������¼�����������Ϣ
		return;
	}
	CXJEvent* pEvent = (CXJEvent*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent == m_pEventManager->m_pCurrentEvent)
	{
		//ˢ��
		UpdateControls();
	}
}

/*************************************************************
 �� �� ����SetFlag()
 ���ܸ�Ҫ�������Ƿ񱣻����Դ���Ҫ���¼����ԶԻ���
 �� �� ֵ: void
 ��    ����param1 �Ƿ񱣻����Դ���Ҫ���¼����ԶԻ���
**************************************************************/
//##ModelId=49B87BA402D1
void CDlgEventProp::SetFlag( BOOL bPT )
{
	m_bPTEvent = bPT;
}

/*************************************************************
 �� �� ����OnEventPTAck()
 ���ܸ�Ҫ����Ӧ��������ҳ��ȷ���¼���Ϣ, ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BA402B8
void CDlgEventProp::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	if(m_bPTEvent == FALSE)
	{
		//���ڲ��Ǳ�������ҳ��򿪵��¼����ԶԻ���, �����������Ϣ
		return;
	}

	CXJEvent* pEvent = (CXJEvent*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent == m_pEventManager->m_pCurrentEvent)
	{
		//ˢ��
		UpdateControls();
	}
}

void CDlgEventProp::OnEventPassiveAck( WPARAM wParam, LPARAM lParam )
{
	CXJEvent* pEvent = (CXJEvent*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent == m_pEventManager->m_pCurrentEvent)
	{
		//ˢ��
		UpdateControls();
	}
}
