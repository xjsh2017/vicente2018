// FaultFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "FaultFrame.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif



/////////////////////////////////////////////////////////////////////////////
// CFaultFrame

IMPLEMENT_DYNCREATE(CFaultFrame, CMDIChildWnd)

//##ModelId=49B87B9A0179
CFaultFrame::CFaultFrame()
{
	m_pTabView = NULL;
	m_bMaximized = NULL;
	m_pFault = NULL;
	m_bCreateSplitterWnd = FALSE;
	m_pFaultQuery = NULL;
}

//##ModelId=49B87B9A019F
CFaultFrame::~CFaultFrame()
{
	TRACE("CFaultFrame::~CFaultFrame");
}


BEGIN_MESSAGE_MAP(CFaultFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFaultFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(FAULTREPORT_SEL_CHANGED, OnSelChanged)
	ON_MESSAGE(FAULTREPORT_PRINT, OnPrintFaultReport)
	ON_MESSAGE(FAULTDETAIL_REFRESH, OnRefreshFaultDetail)
	ON_MESSAGE(WM_STTP_20151, OnSTTP20151)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(FAULTREPORT_EXPORT_EXCEL, OnExportExcelFaultReport)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFaultFrame message handlers

//##ModelId=49B87B9A0198
BOOL CFaultFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);

	// create a splitter with 2 row, 1 columns
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	CRect rect;
	
	GetClientRect(&rect);

	// add the first splitter pane - the default view in row 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(rect.Width(), rect.Height()/2), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in row 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewFaultDetail), CSize(rect.Width(), rect.Height()/2), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	m_bCreateSplitterWnd = TRUE;
	WriteLog("CFaultFrame::OnCreateClient, 分隔窗口创建完成");
	//记录详细视图的指针
 	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->m_pFaultDetail = (CViewFaultDetail*)(m_wndSplitter.GetPane(1, 0));

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE); 
	if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CViewFaultSheet))) 
	{ 
		CViewFaultSheet* pView = (CViewFaultSheet*)pWnd; 
		m_pTabView = pView; 
		
		// Here you can add your dialogs and views and whatever you like.... 
		if (pView->Insert(0, StringFromID(IDS_COMMON_LATEST), RUNTIME_CLASS(CViewFault), pContext) == -1) 
		{ 
			return FALSE; 
		} 
		if(pView->Insert(1, StringFromID(IDS_COMMON_QUERY), RUNTIME_CLASS(CViewFaultQuery), pContext) == -1)
		{
			return FALSE;
		}
		m_pFault = (CViewFault*)pView->GetView(0);
		m_pFaultQuery = (CViewFaultQuery*)pView->GetView(1);
	}
	WriteLog("CFaultFrame::OnCreateClient, 故障报告窗口创建完成");
	return TRUE;
}

//##ModelId=49B87B9A0192
void CFaultFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( !m_bMaximized )
	{
	//	m_bMaximized =TRUE;
		nCmdShow =SW_SHOWMAXIMIZED;		
	}
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87B9A019C
BOOL CFaultFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::PreCreateWindow(cs);
	//防止闪烁
	//得到屏幕分辨率
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //宽   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //高 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		/*| FWS_ADDTOTITLE*/ | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //把窗口样式设置为最大化，但先不显示
	
	cs.cx = nSx;   //把窗口大小设置为整个屏幕大小
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //创建完成只之后再显示窗口
	
	return TRUE;
}

//##ModelId=49B87B9A01A1
void CFaultFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//取得客户区大小
	CRect rc;
	GetClientRect(&rc);
	
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nHeight = int(rc.Height() * (1 - 0.618));
		m_wndSplitter.SetRowInfo(0, nHeight, 0);
	}

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here	
}

/*************************************************************
 函 数 名：OnSelChanged()
 功能概要：转发选择更改消息给详细信息窗口
 返 回 值: void
 参    数：param1	消息参数
		   Param2	被选择的故障报告
**************************************************************/
//##ModelId=49B87B9A01A6
void CFaultFrame::OnSelChanged( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(FAULTREPORT_SEL_CHANGED, wParam, lParam);
}

/*************************************************************
 函 数 名：GetFaultDetailView()
 功能概要：取得故障报告详细视图指针
 返 回 值: 故障报告详细视图指针
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A0191
CViewFaultDetail* CFaultFrame::GetFaultDetailView()
{
	CViewFaultDetail* pView = (CViewFaultDetail*)m_wndSplitter.GetPane(1, 0);
	return pView;
}

/*************************************************************
 函 数 名：OnPrintFaultReport()
 功能概要：转发打印故障报告消息给详细信息窗口
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01AA
void CFaultFrame::OnPrintFaultReport( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(FAULTREPORT_PRINT, wParam, lParam);
}

/*************************************************************
 函 数 名：OnRefreshFaultDetail()
 功能概要：通知故障报告详细视图刷新消息, 转发给详细视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01AE
void CFaultFrame::OnRefreshFaultDetail( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->SendMessage(FAULTDETAIL_REFRESH, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20151()
 功能概要：收到20151报文,自动归档信息包变化通知, 转发
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9A01B5
void CFaultFrame::OnSTTP20151( WPARAM wParam, LPARAM lParam )
{
	if(m_pFault != NULL)
		m_pFault->SendMessage(WM_STTP_20151, wParam, lParam);
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CFaultFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	if(m_pFault != NULL)
		m_pFault->SendMessage(VIEW_REFRESH, wParam, lParam);
}

void CFaultFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	int   cyCur,   cyMin;
	if (m_bCreateSplitterWnd)
	{
		m_wndSplitter.GetRowInfo(0, cyCur, cyMin);
		m_wndSplitter.SetRowInfo(0, cy*3/5, cyMin);
		m_wndSplitter.RecalcLayout();
	}
}

/****************************************************
Date:2013/4/1  Author:LYH
函数名:   OnExportExcelFaultReport	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CFaultFrame::OnExportExcelFaultReport( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(FAULTREPORT_EXPORT_EXCEL, wParam, lParam);
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CFaultFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(m_pFault != NULL)
		m_pFault->SendMessage(STATIONINIT, wParam, lParam);
	if(m_pFaultQuery != NULL)
		m_pFaultQuery->SendMessage(STATIONINIT, wParam, lParam);
}
