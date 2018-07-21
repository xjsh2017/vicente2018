// SoeFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SoeFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoeFrame

IMPLEMENT_DYNCREATE(CSoeFrame, CMDIChildWnd)

CSoeFrame::CSoeFrame()
{
	m_bMaximized = FALSE;
	m_nType = 0;
	m_pEvent = NULL;
}

CSoeFrame::~CSoeFrame()
{
}


BEGIN_MESSAGE_MAP(CSoeFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSoeFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWSOE_SELECTCHANGED, OnViewSoeSelectChanged)
	ON_MESSAGE(VIEW_SOE_LIST_ACK, OnViewSoeListAck)
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoeFrame message handlers

BOOL CSoeFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

void CSoeFrame::ActivateFrame(int nCmdShow) 
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

BOOL CSoeFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);

	// create a splitter with 2 row, 1 columns
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(0, g_nSOESplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	CViewSOE* pSoeView = (CViewSOE*)m_wndSplitter.GetPane(0,0);
	if(pSoeView != NULL)
		pSoeView->m_nType = m_nType;
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewSOEDetail), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	return TRUE;
}

void CSoeFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{	
	RegulateSize();
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
}

void CSoeFrame::OnViewSoeSelectChanged( WPARAM wParam, LPARAM lParam )
{
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(VIEWSOE_SELECTCHANGED, wParam, lParam);
	CXJEvent* pEvent = (CXJEvent*)lParam;
	if(pEvent == NULL || m_pEvent == NULL)
	{
		m_pEvent = pEvent;
		return;
	}
	if(pEvent->m_nEventType == m_pEvent->m_nEventType)
	{
		m_pEvent = pEvent;
	}
	else
	{
		m_pEvent = pEvent;
	}
}

void CSoeFrame::OnViewSoeListAck( WPARAM wParam, LPARAM lParam )
{
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(VIEW_SOE_LIST_ACK, wParam, lParam);
}

void CSoeFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	//转发给列表视图
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20022, wParam, lParam);
}

void CSoeFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	//转发给列表视图
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20018, wParam, lParam);
}

void CSoeFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//转发给列表视图
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView != NULL)
		pView->SendMessage(VIEW_REFRESH, wParam, lParam);
}

void CSoeFrame::OnSTTP20010( WPARAM wParam, LPARAM lParam )
{
	//转发给列表视图
	//转发
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20010, wParam, lParam);
}

void CSoeFrame::RegulateSize()
{
	//取得客户区大小
	CRect rc;
	GetClientRect(&rc);
}

void CSoeFrame::OnSize(UINT nType, int cx, int cy) 
{
	RegulateSize();
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	RegulateSize();
}

BOOL CSoeFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		g_nSOESplitterWnd = nCur;
	}
	return CMDIChildWnd::DestroyWindow();
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CSoeFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView != NULL)
		pView->SendMessage(STATIONINIT, wParam, lParam);
}
