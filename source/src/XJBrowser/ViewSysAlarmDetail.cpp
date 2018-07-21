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
 函 数 名：RegulateControlSize()
 功能概要：调整控件大小,位置 
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100B1
void CViewSysAlarmDetail::RegulateControlSize()
{
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	
	//文本框
	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SYSALARM_1);
	CRect rcStatic;
	long lWidth = 0;
	long lHeight = 0;
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SYSALARM_1);
	CRect rcEdit;
	
	//发生时间
	if(pStatic != NULL)
	{
		//静态文本位置放在下方10像素的地方, 左边顶头	
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
	{ //编辑框离相应的静态文本10像素, 并延伸到客户区中间
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top-4;
		rcEdit.left = rcStatic.right + 4;
		rcEdit.right = rc.left + rc.Width()/2 - 10;
		rcEdit.bottom = rcEdit.top + 20;
		pEdit ->MoveWindow(rcEdit);
	}
	
	//设备
	pStatic = NULL;
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SYSALARM_2);
	if(pStatic != NULL)
	{	//静态文本在下方10像素, 左边顶客户区中间
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
	{	//编辑框伸延到客户区右边
		pEdit ->GetClientRect(&rcEdit);
		lWidth = rcEdit.Width();
		lHeight = rcEdit.Height();
		rcEdit.top = rcStatic.top - 4;
		rcEdit.left = rcStatic.right + 4;
		rcEdit.bottom = rcEdit.top + 20;
		rcEdit.right = rc.right - 10;
		pEdit ->MoveWindow(rcEdit);
	}

	//列表
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
	//设置可滚动视图的必要属性
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	//初始化列表
	InitListStyle();
}

/*************************************************************
 函 数 名：RefureDetail()
 功能概要：刷新显示的详细内容
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100B2
void CViewSysAlarmDetail::RefureDetail()
{
	//先清空
	m_sTime = "";
	m_sDevice = "";
	m_sDetail = "";
	if(m_pEvent != NULL)
	{
		//发生时间
		m_sTime = m_pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		//事件来源([XX厂站][XX设备][XX保护装置])
		m_sDevice.Format("[%s][%s][%s]",m_pEvent->m_sStationName,m_pEvent->m_sDeviceName, m_pEvent->m_sSecName);
		//详细内容
		m_sDetail = m_pEvent->GetDescribe();
		//填充列表
		FillListData();
	}
	else
	{
		m_List.DeleteAllItems();
	}
	
	UpdateData(FALSE);
}

/*************************************************************
 函 数 名：OnListSelChanged()
 功能概要：响应事件列表选择改变消息
 返 回 值: void
 参    数：param1
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
 函 数 名：OnEventListAck()
 功能概要：响应事件列表确认事件消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100D4
void CViewSysAlarmDetail::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//判断是否此处显示的事件
	if(pEvent == m_pEvent)
	{
		//刷新显示
		RefureDetail();
	}
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8100DA
void CViewSysAlarmDetail::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//判断是否此处显示的事件
	if(pEvent == m_pEvent)
	{
		//刷新显示
		RefureDetail();
	}
}

/*************************************************************
 函 数 名: InitListStyle()
 功能概要: 初始化列表
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSysAlarmDetail::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	//char* arColumn[4]={"序号","名称", "改变前的值", "改变后的值"};
	for (int nCol=0; nCol < 4; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 60; //列宽50象素 序号
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 240; //列宽60象素 名称
			colName = StringFromID(IDS_COMMON_NAME);
			break;
		case 2://
			lvCol.cx = 180; //列宽60象素 值
			colName = StringFromID(IDS_VALUE_BEFORE);
			break;
		case 3:
			lvCol.cx = 180;
			colName = StringFromID(IDS_VALUE_AFTER);
			break;
		default:
			lvCol.cx=100;//列宽100象素
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	//默认隐藏第一列(序号)
//	m_List.SetColumnHide(0, TRUE);
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}

/*************************************************************
 函 数 名: AddDetailToList()
 功能概要: 添加详细信息到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 详细信息数据
		   Param2 添加的位置
**************************************************************/
BOOL CViewSysAlarmDetail::AddDetailToList( SYSALARM* sa, int nIndex )
{
	//特征量为空, 返回FALSE
	if(sa == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", nIndex+1);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:名称
	m_List.SetItemText(nIndex, 1, sa->sName);
	//2.旧值
	m_List.SetItemText(nIndex, 2, sa->sOldValue);
	//3.新值
	m_List.SetItemText(nIndex, 3, sa->sNewValue);
	
	//设置数据s
	m_List.SetItemData(nIndex, (LPARAM)(sa));
	return TRUE;
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSysAlarmDetail::FillListData()
{
	//检查数据有效性
	if(m_pEvent == NULL)
		return;

	//信号量
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	for(int i = 0; i < m_pEvent->m_arrDetail.GetSize(); i++)
	{
		SYSALARM* sa = (SYSALARM*)m_pEvent->m_arrDetail.GetAt(i);
		AddDetailToList(sa, i);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

void CViewSysAlarmDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_SYSALARM));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
