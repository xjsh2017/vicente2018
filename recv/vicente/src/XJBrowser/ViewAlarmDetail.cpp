// ViewAlarmDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewAlarmDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewAlarmDetail

IMPLEMENT_DYNCREATE(CViewAlarmDetail, CFormView)

//##ModelId=49B87B8602A1
CViewAlarmDetail::CViewAlarmDetail()
	: CFormView(CViewAlarmDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewAlarmDetail)
	m_sTime = _T("");
	m_sDevice = _T("");
	m_sDetail = _T("");
	//}}AFX_DATA_INIT
	m_pEvent = NULL;
}

//##ModelId=49B87B8602C4
CViewAlarmDetail::~CViewAlarmDetail()
{
	WriteLog("CViewAlarmDetail::~CViewAlarmDetail");
}

//##ModelId=49B87B8602C1
void CViewAlarmDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAlarmDetail)
	DDX_Text(pDX, IDC_EDIT_ALARM_1, m_sTime);
	DDX_Text(pDX, IDC_EDIT_ALARM_2, m_sDevice);
	DDX_Text(pDX, IDC_EDIT_ALARM_3, m_sDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewAlarmDetail, CFormView)
	//{{AFX_MSG_MAP(CViewAlarmDetail)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWALARM_SELECTCHANGED, OnViewAlarmSelectChanged)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAlarmDetail diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8602C6
void CViewAlarmDetail::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B8602C8
void CViewAlarmDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewAlarmDetail message handlers

//##ModelId=49B87B8602BF
void CViewAlarmDetail::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С,λ��.�ڴ���λ���б䶯ʱ����
 �� �� ֵ: ����ֵ˵��
 ��    ����
**************************************************************/
//##ModelId=49B87B8602B7
void CViewAlarmDetail::RegulateControlSize()
{
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);

	//�ı���
	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ALARM_1);
	CRect rcStatic;
	long lWidth = 0;
	long lHeight = 0;
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ALARM_1);
	CRect rcEdit;

	//����ʱ��
	if(pStatic != NULL)
	{
		//��̬�ı�λ�÷����·�10���صĵط�, ��߶�ͷ	
		pStatic->GetClientRect(&rcStatic);
		lWidth = rcStatic.Width();
		lHeight = rcStatic.Height();
		rcStatic.top = rc.top + 10;
		rcStatic.bottom = rcStatic.top + lHeight;
		pStatic->MoveWindow(rcStatic);
	}
	if(pEdit != NULL)
	{ //�༭������Ӧ�ľ�̬�ı�10����, �����쵽�ͻ����м�
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top;
		rcEdit.left = rcStatic.right + 10;
		rcEdit.right = rc.left + rc.Width()/2 - 20;
		rcEdit.bottom = rcEdit.top + 20;
		pEdit ->MoveWindow(rcEdit);
	}

	//�豸
	pStatic = NULL;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ALARM_2);
	if(pStatic != NULL)
	{	//��̬�ı����·�10����, ��߶��ͻ����м�
		pStatic ->GetClientRect(&rcStatic);
		lWidth = rcStatic.Width();
		lHeight = rcStatic.Height();
		rcStatic.left = rc.left + rc.Width()/2;
		rcStatic.top = rc.top + 10;
		rcStatic.right = rcStatic.left + lWidth;
		rcStatic.bottom = rcStatic.top + lHeight;
		pStatic ->MoveWindow(rcStatic);
	}
	pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ALARM_2);
	if(pEdit != NULL)
	{	//�༭�����ӵ��ͻ����ұ�
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top;
		rcEdit.left = rcStatic.right + 10;
		rcEdit.bottom = rcEdit.top + 20;
		rcEdit.right = rc.right - 20;
		pEdit ->MoveWindow(rcEdit);
	}

	//��ϸ��Ϣ
	pStatic = NULL;
	pStatic = (CStatic *)GetDlgItem(IDC_STATIC_ALARM_3);
	if(pStatic != NULL)
	{
		pStatic ->GetClientRect(&rcStatic);
		lWidth = rcStatic.Width();
		lHeight = rcStatic.Height();
		rcStatic.top = rcEdit.bottom + 10;
		rcStatic.bottom = rcStatic.top + lHeight;
		pStatic ->MoveWindow(rcStatic);
	}
	pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ALARM_3);
	if(pEdit != NULL)
	{
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top;
		rcEdit.left = rcStatic.right + 10;
		rcEdit.right = rc.right - 20;
		rcEdit.bottom = rc.bottom - 10;
		pEdit ->MoveWindow(rcEdit);
	}
}

//##ModelId=49B87B8602CB
void CViewAlarmDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

//##ModelId=49B87B8602D0
void CViewAlarmDetail::OnViewAlarmSelectChanged(WPARAM wParam, LPARAM lParam)
{
	CXJEventAlarm * pEvent = (CXJEventAlarm*)lParam;
	
	m_pEvent = pEvent;
	RefureDetail();
}

/*************************************************************
 �� �� ����RefureDetail()
 ���ܸ�Ҫ��ˢ����ʾ����ϸ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8602B8
void CViewAlarmDetail::RefureDetail()
{
	//�����
	m_sTime = "";
	m_sDevice = "";
	m_sDetail = "";
	if(m_pEvent != NULL)
	{
		//����ʱ��
		m_sTime = m_pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		//�¼���Դ([XX��վ][XX�豸][XX����װ��])
		m_sDevice.Format("[%s][%s][%s]",m_pEvent->m_sStationName,m_pEvent->m_sDeviceName, m_pEvent->m_sSecName);
		//��ϸ����
		m_sDetail = m_pEvent->GetDescribe();
	}
	
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	��ȷ���¼�ָ��
**************************************************************/
//##ModelId=49B87B8602D4
void CViewAlarmDetail::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//�ж��Ƿ�˴���ʾ���¼�
	if(pEvent == m_pEvent)
	{
		//ˢ����ʾ
		RefureDetail();
	}
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ�¼����¼�ȷ����Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B8602D7
void CViewAlarmDetail::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//�ж��Ƿ�˴���ʾ���¼�
	if(pEvent == m_pEvent)
	{
		//ˢ����ʾ
		RefureDetail();
	}
}

void CViewAlarmDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->SetAppTile(StringFromID(IDS_SECPROP_ALARM));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
