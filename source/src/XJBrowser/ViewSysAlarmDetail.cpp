// ViewSysAlarmDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewSysAlarmDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarmDetail

IMPLEMENT_DYNCREATE(CViewSysAlarmDetail, CFormView)

//##ModelId=49B87B81009E
CViewSysAlarmDetail::CViewSysAlarmDetail()
	: CFormView(CViewSysAlarmDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewSysAlarmDetail)
	m_sTime = _T("");
	m_sDevice = _T("");
	m_sDetail = _T("");
	//}}AFX_DATA_INIT
	m_pEvent = NULL;
}

//##ModelId=49B87B8100BC
CViewSysAlarmDetail::~CViewSysAlarmDetail()
{
}

//##ModelId=49B87B8100B5
void CViewSysAlarmDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSysAlarmDetail)
	DDX_Control(pDX, IDC_LIST_SYSALARM_DETAIL, m_List);
	DDX_Text(pDX, IDC_EDIT_SYSALARM_1, m_sTime);
	DDX_Text(pDX, IDC_EDIT_SYSALARM_2, m_sDevice);
	DDX_Text(pDX, IDC_EDIT_SYSALARM_3, m_sDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSysAlarmDetail, CFormView)
	//{{AFX_MSG_MAP(CViewSysAlarmDetail)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWSYSALARM_SELECTCHANGED, OnListSelChanged)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarmDetail diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8100BE
void CViewSysAlarmDetail::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B8100C0
void CViewSysAlarmDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarmDetail message handlers

//##ModelId=49B87B8100C3
void CViewSysAlarmDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С,λ�� 
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100B1
void CViewSysAlarmDetail::RegulateControlSize()
{
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	
	//�ı���
	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SYSALARM_1);
	CRect rcStatic;
	long lWidth = 0;
	long lHeight = 0;
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SYSALARM_1);
	CRect rcEdit;
	
	//����ʱ��
	if(pStatic != NULL)
	{
		//��̬�ı�λ�÷����·�10���صĵط�, ��߶�ͷ	
		pStatic->GetClientRect(&rcStatic);
		lWidth = rcStatic.Width();
		lHeight = rcStatic.Height();
		rcStatic.left = rc.left + 10;
		rcStatic.right = rcStatic.left + lWidth;
		rcStatic.top = rc.top + 10;
		rcStatic.bottom = rcStatic.top + lHeight;
		pStatic->MoveWindow(rcStatic);
	}
	if(pEdit != NULL)
	{ //�༭������Ӧ�ľ�̬�ı�10����, �����쵽�ͻ����м�
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top-4;
		rcEdit.left = rcStatic.right + 4;
		rcEdit.right = rc.left + rc.Width()/2 - 10;
		rcEdit.bottom = rcEdit.top + 20;
		pEdit ->MoveWindow(rcEdit);
	}
	
	//�豸
	pStatic = NULL;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SYSALARM_2);
	if(pStatic != NULL)
	{	//��̬�ı����·�10����, ��߶��ͻ����м�
		pStatic ->GetClientRect(&rcStatic);
		lWidth = rcStatic.Width();
		lHeight = rcStatic.Height();
		rcStatic.left = rc.left + rc.Width()/2+4;
		rcStatic.top = rc.top + 10;
		rcStatic.right = rcStatic.left + lWidth;
		rcStatic.bottom = rcStatic.top + lHeight;
		pStatic ->MoveWindow(rcStatic);
	}
	pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SYSALARM_2);
	if(pEdit != NULL)
	{	//�༭�����ӵ��ͻ����ұ�
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top - 4;
		rcEdit.left = rcStatic.right + 4;
		rcEdit.bottom = rcEdit.top + 20;
		rcEdit.right = rc.right - 10;
		pEdit ->MoveWindow(rcEdit);
	}

	//�б�
	if(m_List.GetSafeHwnd() != NULL)
	{
		m_List.GetClientRect(&rcStatic);
		rcStatic.left = rc.left;
		rcStatic.right = rc.right;
		rcStatic.top = rcEdit.bottom + 10;
		rcStatic.bottom = rc.bottom;
		m_List.MoveWindow(rcStatic);
	}
}

//##ModelId=49B87B8100CB
void CViewSysAlarmDetail::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();	
}

//##ModelId=49B87B8100B3
void CViewSysAlarmDetail::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	//��ʼ���б�
	InitListStyle();
}

/*************************************************************
 �� �� ����RefureDetail()
 ���ܸ�Ҫ��ˢ����ʾ����ϸ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100B2
void CViewSysAlarmDetail::RefureDetail()
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
		//����б�
		FillListData();
	}
	else
	{
		m_List.DeleteAllItems();
	}
	
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����OnListSelChanged()
 ���ܸ�Ҫ����Ӧ�¼��б�ѡ��ı���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100D0
void CViewSysAlarmDetail::OnListSelChanged( WPARAM wParam, LPARAM lParam )
{
	CXJEventSysAlarm * pEvent = (CXJEventSysAlarm*)lParam;
	
	m_pEvent = pEvent;
	RefureDetail();
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ�¼��б�ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100D4
void CViewSysAlarmDetail::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
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
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100DA
void CViewSysAlarmDetail::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
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
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSysAlarmDetail::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	//char* arColumn[4]={"���","����", "�ı�ǰ��ֵ", "�ı���ֵ"};
	for (int nCol=0; nCol < 4; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 60; //�п�50���� ���
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 240; //�п�60���� ����
			colName = StringFromID(IDS_COMMON_NAME);
			break;
		case 2://
			lvCol.cx = 180; //�п�60���� ֵ
			colName = StringFromID(IDS_VALUE_BEFORE);
			break;
		case 3:
			lvCol.cx = 180;
			colName = StringFromID(IDS_VALUE_AFTER);
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	//Ĭ�����ص�һ��(���)
//	m_List.SetColumnHide(0, TRUE);
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}

/*************************************************************
 �� �� ��: AddDetailToList()
 ���ܸ�Ҫ: �����ϸ��Ϣ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ��ϸ��Ϣ����
		   Param2 ��ӵ�λ��
**************************************************************/
BOOL CViewSysAlarmDetail::AddDetailToList( SYSALARM* sa, int nIndex )
{
	//������Ϊ��, ����FALSE
	if(sa == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", nIndex+1);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:����
	m_List.SetItemText(nIndex, 1, sa->sName);
	//2.��ֵ
	m_List.SetItemText(nIndex, 2, sa->sOldValue);
	//3.��ֵ
	m_List.SetItemText(nIndex, 3, sa->sNewValue);
	
	//��������s
	m_List.SetItemData(nIndex, (LPARAM)(sa));
	return TRUE;
}

/*************************************************************
 �� �� ��: FillListData()
 ���ܸ�Ҫ: ����б�����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSysAlarmDetail::FillListData()
{
	//���������Ч��
	if(m_pEvent == NULL)
		return;

	//�ź���
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	for(int i = 0; i < m_pEvent->m_arrDetail.GetSize(); i++)
	{
		SYSALARM* sa = (SYSALARM*)m_pEvent->m_arrDetail.GetAt(i);
		AddDetailToList(sa, i);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

void CViewSysAlarmDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_SYSALARM));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
