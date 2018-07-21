// ViewActionDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewActionDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewActionDetail

IMPLEMENT_DYNCREATE(CViewActionDetail, CFormView)

//##ModelId=49B87B8701A5
CViewActionDetail::CViewActionDetail()
	: CFormView(CViewActionDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewActionDetail)
	m_sTime = _T("");
	m_sDevice = _T("");
	m_sDetail = _T("");
	//}}AFX_DATA_INIT
	m_pEvent = NULL;
}

//##ModelId=49B87B8701BC
CViewActionDetail::~CViewActionDetail()
{
	WriteLog("CViewActionDetail::~CViewActionDetail");
}

//##ModelId=49B87B8701B9
void CViewActionDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewActionDetail)
	DDX_Control(pDX, IDC_LIST_ACTIONDETAIL_SIGN, m_ListSign);
	DDX_Control(pDX, IDC_LIST_ACTIONDETAIL_CHR, m_ListChr);
	DDX_Text(pDX, IDC_EDIT_ACTION_1, m_sTime);
	DDX_Text(pDX, IDC_EDIT_ACTION_2, m_sDevice);
	DDX_Text(pDX, IDC_EDIT_ACTION_3, m_sDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewActionDetail, CFormView)
	//{{AFX_MSG_MAP(CViewActionDetail)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWACTION_SELECTCHANGED, OnViewActionSelectChanged)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewActionDetail diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8701C6
void CViewActionDetail::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B8701C8
void CViewActionDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewActionDetail message handlers

//##ModelId=49B87B8701B7
void CViewActionDetail::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//��ʼ���б�
	InitChrListStyle();
	InitSignListStyle();
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С,λ��.�ڴ���λ���б䶯ʱ����
 �� �� ֵ: ����ֵ˵��
 ��    ����
**************************************************************/
//##ModelId=49B87B8701B5
void CViewActionDetail::RegulateControlSize()
{
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);

	//�ı���
	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ACTION_1);
	CRect rcStatic;
	long lWidth = 0;
	long lHeight = 0;
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ACTION_1);
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
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_ACTION_2);
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
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ACTION_2);
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

	//�ź��б�
	if(m_ListSign.GetSafeHwnd() != NULL)
	{
		CRect rcList;
		rcList.left = rc.left;
		rcList.top = rcStatic.bottom+10;
		rcList.right = rc.left+rc.Width()/2-3;
		rcList.bottom = rc.bottom;
		m_ListSign.MoveWindow(rcList);
	}
	CWnd* pWnd = GetDlgItem(IDC_STATIC_ACTIONDETAIL_P);
	if(pWnd != NULL)
	{	
		CRect rcList;
		rcList.left = rc.left+rc.Width()/2;
		rcList.top = rcStatic.bottom+12;
		rcList.right = rcList.left+1;
		rcList.bottom = rc.bottom - 2;
		pWnd ->MoveWindow(rcList);
	}
	//������
	if(m_ListChr.GetSafeHwnd() != NULL)
	{
		CRect rcList;
		rcList.left = rc.left+rc.Width()/2 +4;
		rcList.top = rcStatic.bottom+10;
		rcList.right = rc.right;
		rcList.bottom = rc.bottom;
		m_ListChr.MoveWindow(rcList);
	}
}


void CViewActionDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
//	RegulateControlSize();
}


void CViewActionDetail::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}


void CViewActionDetail::OnViewActionSelectChanged(WPARAM wParam, LPARAM lParam)
{
	CXJEventAction * pEvent = (CXJEventAction*)lParam;
	
	m_pEvent = pEvent;
	
	RefureDetail();
}

/*************************************************************
 �� �� ����RefureDetail()
 ���ܸ�Ҫ��ˢ����ʾ����ϸ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8701B6
void CViewActionDetail::RefureDetail()
{
	//�����
	m_sTime = "";
	m_sDevice = "";
	m_sDetail = "";
	if(m_pEvent != NULL)
	{	//����ʱ��
		m_sTime = m_pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		//�¼���Դ([XX��վ][XX�豸][XX����װ��])
		m_sDevice.Format("[%s][%s][%s]",m_pEvent->m_sStationName,m_pEvent->m_sDeviceName, m_pEvent->m_sSecName);
		//��ϸ����
		m_sDetail = m_pEvent->GetDescribe();
		//
		FillListData();
	}
	else
	{
		//ɾ���б�����
		m_ListSign.DeleteAllItems();
		m_ListChr.DeleteAllItems();
	}
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ, �˴�ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	��ȷ���¼�ָ��
**************************************************************/
void CViewActionDetail::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
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
 ���ܸ�Ҫ���¼���ȷ���¼���Ϣ, ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B8701E5
void CViewActionDetail::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
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
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewActionDetail::InitSignListStyle()
{	
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListSign.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;

	//char* arColumn[5]={"���","�ź�������", "ֵ","����ʱ��","���ʱ��"};
	for (int nCol=0; nCol < 5; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName="";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 60; //�п�50���� ���
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 140; //�п�60���� �ź�������
			colName = StringFromID(IDS_SIGNAL_NAME);
			break;
		case 2://
			lvCol.cx = 60; //�п�60���� ֵ
			colName = StringFromID(IDS_COMMON_VALUE);
			break;
		case 3://
			lvCol.cx = 160; //�п�160���� ����ʱ��
			colName = StringFromID(IDS_EVENT_STARTTIME);
			break;
		case 4:
			lvCol.cx = 100; //���ʱ��
			colName = StringFromID(IDS_EVENT_RELATIVETIME);
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_ListSign.InsertColumn(nCol,&lvCol);
	}
	//Ĭ�����ص�һ��(���)
	m_ListSign.SetColumnHide(0, TRUE);
	//���÷��
	m_ListSign.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 �� �� ��: InitChrListStyle()
 ���ܸ�Ҫ: ��ʼ���������б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewActionDetail::InitChrListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListChr.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	//char* arColumn[3]={"���","����������", "ֵ"};
	for (int nCol=0; nCol < 3; nCol++)
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
			lvCol.cx = 200; //�п�60���� �ź�������
			colName = StringFromID(IDS_COMMON_NAME);
			break;
		case 2://
			lvCol.cx = 140; //�п�60���� ֵ
			colName = StringFromID(IDS_COMMON_VALUE);
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_ListChr.InsertColumn(nCol,&lvCol);
	}
	//Ĭ�����ص�һ��(���)
	m_ListChr.SetColumnHide(0, TRUE);
	//���÷��
	m_ListChr.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 �� �� ��: AddSignToList()
 ���ܸ�Ҫ: ���ź�����ӵ��б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ���ź���
		   Param2 ָ�����б��е�λ��
**************************************************************/
BOOL CViewActionDetail::AddSignToList( PT_Signal* pSign, int nIndex /*= 0*/ )
{
	//�ź���Ϊ��, ����FALSE
	if(pSign == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", pSign->nID);
	if(m_ListSign.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:����
	m_ListSign.SetItemText(nIndex, 1, pSign->strName);
	//2.ֵ
	m_ListSign.SetItemText(nIndex, 2, pSign->nValue ==1 ? StringFromID(IDS_CASE_ACTION):StringFromID(IDS_CASE_RETURN));
	//3:ʱ��
	m_ListSign.SetItemText(nIndex, 3, pSign->strTime);
	//4:���ʱ��
	//int nSignMS = 0;
	//CTime tmSignMS = StringToTimeSttp26(pSign->strTime, nSignMS);
	//int nMS = GetSignRelativeTime(tmSignMS, nSignMS);
	int nMS = pSign->nRelativeTime;
	if(nMS >= 0)
		sID.Format("%d ms", nMS);
	else
	{
		sID.Format(StringFromID(IDS_COMMON_ERROR));
		CString str;
		str.Format("�ź����ʱ��Ϊ����,�¼�ID:%d,�ź�ID:%d,���ʱ��:%d", m_pEvent==NULL?-1:m_pEvent->m_lID, pSign->nID, nMS);
		WriteLog(str, XJ_LOG_LV3);
	}
	m_ListSign.SetItemText(nIndex, 4, sID);
	//��������s
	m_ListSign.SetItemData(nIndex, (LPARAM)(pSign));
	return TRUE;
}

/*************************************************************
 �� �� ��: AddChrToList()
 ���ܸ�Ҫ: ����������ӵ��б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ��������
		   Param2 ָ�����б��е�λ��
**************************************************************/
BOOL CViewActionDetail::AddChrToList( PT_ActionCharacter* pChr, int nIndex /*= 0*/ )
{
	//������Ϊ��, ����FALSE
	if(pChr == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", pChr->nID);
	if(m_ListChr.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:����
	m_ListChr.SetItemText(nIndex, 1, pChr->strName);
	//2.ֵ
	CString str;
	str = pChr->strValue;
	if(pChr->strUnits != "")
		str.Format("%s(%s)", pChr->strValue, pChr->strUnits);
	m_ListChr.SetItemText(nIndex, 2, str);

	//��������s
	m_ListChr.SetItemData(nIndex, (LPARAM)(pChr));
	return TRUE;
}

/*************************************************************
 �� �� ��: FillListData()
 ���ܸ�Ҫ: ����б�����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewActionDetail::FillListData()
{
	if(m_pEvent == NULL)
		return;

	//�ź���
	//�������ʱ��ֹˢ��
	m_ListSign.SetRedraw(FALSE);
	//ɾ���б�����
	m_ListSign.DeleteAllItems();
	
	for(int i = 0; i < m_pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pEvent->m_Signals.GetAt(i);
		AddSignToList(pSign, i);
	}
	//�ָ�ˢ��
	m_ListSign.SetRedraw(TRUE);

	//������
	//�������ʱ��ֹˢ��
	m_ListChr.SetRedraw(FALSE);
	//ɾ���б�����
	m_ListChr.DeleteAllItems();
	
	for(i = 0; i < m_pEvent->m_ActChrs.GetSize(); i++)
	{
		PT_ActionCharacter* pSign = (PT_ActionCharacter*)m_pEvent->m_ActChrs.GetAt(i);
		AddChrToList(pSign, i);
	}
	//�ָ�ˢ��
	m_ListChr.SetRedraw(TRUE);
}

/****************************************************
Date:2012/3/15  Author:LYH
������:   GetSignRelativeTime	
����ֵ:   int	
���ܸ�Ҫ: 
����: CTime tmSignTime	
����: int nSignMS	
*****************************************************/
int CViewActionDetail::GetSignRelativeTime( CTime tmSignTime, int nSignMS )
{
	if(m_pEvent == NULL)
		return 0;
	CTimeSpan tmSpan = tmSignTime - m_pEvent->m_tmTime;
	LONG nSEC = tmSpan.GetTotalSeconds(); //�������
	//תΪ����ֵ
	LONG nMs = nSEC* 1000;
	nMs += (nSignMS - m_pEvent->m_nms);

	return nMs;
}

void CViewActionDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->SetAppTile(StringFromID(IDS_SECPROP_ACTION));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
