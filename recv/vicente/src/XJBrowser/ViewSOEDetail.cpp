// ViewSOEDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewSOEDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewSOEDetail

IMPLEMENT_DYNCREATE(CViewSOEDetail, CFormView)

CViewSOEDetail::CViewSOEDetail()
	: CFormView(CViewSOEDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewSOEDetail)
	m_sTime = _T("");
	m_sDevice = _T("");
	m_sDetail = _T("");
	//}}AFX_DATA_INIT
	m_pEvent = NULL;
}

CViewSOEDetail::~CViewSOEDetail()
{
}

void CViewSOEDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSOEDetail)
	DDX_Control(pDX, IDC_LIST_SOEDETAIL_SIGN, m_ListSign);
	DDX_Control(pDX, IDC_LIST_SOEDETAIL_CHR, m_ListChr);
	DDX_Text(pDX, IDC_EDIT_SOE_1, m_sTime);
	DDX_Text(pDX, IDC_EDIT_SOE_2, m_sDevice);
	DDX_Text(pDX, IDC_EDIT_SOE_3, m_sDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSOEDetail, CFormView)
	//{{AFX_MSG_MAP(CViewSOEDetail)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWSOE_SELECTCHANGED, OnViewSoeSelectChanged)
	ON_MESSAGE(VIEW_SOE_LIST_ACK, OnViewSoeListAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSOEDetail diagnostics

#ifdef _DEBUG
void CViewSOEDetail::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSOEDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

/*************************************************************
 函 数 名: RegulateControlSize()
 功能概要: 调整控件大小,位置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOEDetail::RegulateControlSize()
{
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	
	//文本框
	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SOE_1);
	CRect rcStatic;
	long lWidth = 0;
	long lHeight = 0;
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SOE_1);
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
	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SOE_2);
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
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SOE_2);
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
	
	//信号列表
	if(m_ListSign.GetSafeHwnd() != NULL)
	{
		CRect rcList;
		rcList.left = rc.left;
		rcList.top = rcStatic.bottom+10;
		rcList.right = rc.left+rc.Width()/2-3;
		rcList.bottom = rc.bottom;
		m_ListSign.MoveWindow(rcList);
	}
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SOEDETAIL_P);
	if(pWnd != NULL)
	{	
		CRect rcList;
		rcList.left = rc.left+rc.Width()/2;
		rcList.top = rcStatic.bottom+12;
		rcList.right = rcList.left+1;
		rcList.bottom = rc.bottom - 2;
		pWnd ->MoveWindow(rcList);
	}
	//特征量
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

/*************************************************************
 函 数 名: RefureDetail()
 功能概要: 刷新显示的详细内容
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOEDetail::RefureDetail()
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
		//
		FillListData();
	}
	else
	{
		m_ListChr.DeleteAllItems();
		m_ListSign.DeleteAllItems();
	}
	
	UpdateData(FALSE);
}

void CViewSOEDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewSOEDetail::OnViewSoeSelectChanged( WPARAM wParam, LPARAM lParam )
{
	CXJEvent * pEvent = (CXJEvent*)lParam;
	
	m_pEvent = pEvent;
	
	RefureDetail();
}

void CViewSOEDetail::OnViewSoeListAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	CXJEvent* pEvent = (CXJEvent*)lParam;
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
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewSOEDetail::InitSignListStyle()
{	
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListSign.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;

	//char* arColumn[4]={"序号","信号量名称", "值","发生时间"};
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
			lvCol.cx = 140; //列宽60象素 信号量名称
			colName = StringFromID(IDS_COMMON_NAME);
			break;
		case 2://
			lvCol.cx = 60; //列宽60象素 值
			colName = StringFromID(IDS_COMMON_VALUE);
			break;
		case 3://
			lvCol.cx = 160; //列宽160象素 时间
			colName = StringFromID(IDS_COMMON_TIME);
			break;
		default:
			lvCol.cx=100;//列宽100象素
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_ListSign.InsertColumn(nCol,&lvCol);
	}
	//默认隐藏第一列(序号)
	m_ListSign.SetColumnHide(0, TRUE);
	//设置风格
	m_ListSign.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名: InitChrListStyle()
 功能概要: 初始化特征量列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewSOEDetail::InitChrListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListChr.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	//char* arColumn[3]={"序号","特征量名称", "值"};
	for (int nCol=0; nCol < 3; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName="";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 60; //列宽50象素 序号
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 200; //列宽60象素 特征量名称
			colName = StringFromID(IDS_COMMON_NAME);
			break;
		case 2://
			lvCol.cx = 140; //列宽60象素 值
			colName = StringFromID(IDS_COMMON_VALUE);
			break;
		default:
			lvCol.cx=100;//列宽100象素
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_ListChr.InsertColumn(nCol,&lvCol);
	}
	//默认隐藏第一列(序号)
	m_ListChr.SetColumnHide(0, TRUE);
	//设置风格
	m_ListChr.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名: AddSignToList()
 功能概要: 将信号量添加到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定信号量
		   Param2 指定在列表中的位置
**************************************************************/
BOOL CViewSOEDetail::AddSignToList( PT_Signal* pSign, int nIndex /*= 0*/ )
{
	//信号量为空, 返回FALSE
	if(pSign == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", pSign->nID);
	if(m_ListSign.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:名称
	m_ListSign.SetItemText(nIndex, 1, pSign->strName);
	//2.值
	m_ListSign.SetItemText(nIndex, 2, pSign->nValue ==1 ? StringFromID(IDS_CASE_ACTION):StringFromID(IDS_CASE_RETURN));
	//3:时间
	m_ListSign.SetItemText(nIndex, 3, pSign->strTime);
	//设置数据s
	m_ListSign.SetItemData(nIndex, (LPARAM)(pSign));
	return TRUE;
}

/*************************************************************
 函 数 名: AddChrToList()
 功能概要: 将特征量添加到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定特征量
		   Param2 指定在列表中的位置
**************************************************************/
BOOL CViewSOEDetail::AddChrToList( PT_ActionCharacter* pChr, int nIndex /*= 0*/ )
{
	//特征量为空, 返回FALSE
	if(pChr == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", pChr->nID);
	if(m_ListChr.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:名称
	m_ListChr.SetItemText(nIndex, 1, pChr->strName);
	//2.值
	CString str;
	str = pChr->strValue;
	if(pChr->strUnits != "")
		str.Format("%s(%s)", pChr->strValue, pChr->strUnits);
	m_ListChr.SetItemText(nIndex, 2, str);

	//设置数据s
	m_ListChr.SetItemData(nIndex, (LPARAM)(pChr));
	return TRUE;
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOEDetail::FillListData()
{
	if(m_pEvent == NULL)
		return;

	//信号量
	//填充数据时禁止刷新
	m_ListSign.SetRedraw(FALSE);
	//删除列表数据
	m_ListSign.DeleteAllItems();
	
	for(int i = 0; i < m_pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pEvent->m_Signals.GetAt(i);
		AddSignToList(pSign, i);
	}
	//恢复刷新
	m_ListSign.SetRedraw(TRUE);

	//特征量
	//填充数据时禁止刷新
	m_ListChr.SetRedraw(FALSE);
	//删除列表数据
	m_ListChr.DeleteAllItems();
	
	for(i = 0; i < m_pEvent->m_ActChrs.GetSize(); i++)
	{
		PT_ActionCharacter* pSign = (PT_ActionCharacter*)m_pEvent->m_ActChrs.GetAt(i);
		AddChrToList(pSign, i);
	}
	//恢复刷新
	m_ListChr.SetRedraw(TRUE);
}


void CViewSOEDetail::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//初始化列表
	InitChrListStyle();
	InitSignListStyle();
}
