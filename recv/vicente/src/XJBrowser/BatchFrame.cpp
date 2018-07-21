// BatchFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchFrame.h"
#include "BatchDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMySplitter,CSplitterWnd)

BEGIN_MESSAGE_MAP(CMySplitter,CSplitterWnd)
ON_WM_NCHITTEST()
END_MESSAGE_MAP()

UINT CMySplitter::OnNcHitTest(CPoint point)
{
	//返回HTNOWHERE...
	return HTNOWHERE;
}

/////////////////////////////////////////////////////////////////////////////
// CBatchFrame

IMPLEMENT_DYNCREATE(CBatchFrame, CMDIChildWnd)

//##ModelId=49B87BB10179
CBatchFrame::CBatchFrame()
{
	m_bMaximized = FALSE;
}

//##ModelId=49B87BB1017A
CBatchFrame::~CBatchFrame()
{
	TRACE("CBatchFrame::~CBatchFrame");
}


BEGIN_MESSAGE_MAP(CBatchFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBatchFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_CHECK, OnBatchCheck)
	ON_MESSAGE(BATCH_OPTION, OnBatchOption)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
	ON_MESSAGE(WM_STTP_20154, OnSTTP20154)
	ON_MESSAGE(BATCHCALL_END, OnBatchCallEnd)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchFrame message handlers

//##ModelId=49B87BB1018C
void CBatchFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if ( !m_bMaximized )
// 	{
// 		m_bMaximized =TRUE;
// 		nCmdShow =SW_SHOWMAXIMIZED;		
// 	}
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87BB1018F
BOOL CBatchFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIChildWnd::PreCreateWindow(cs);
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
	
	cs.style |= WS_VISIBLE;
	return TRUE;
//	return CMDIChildWnd::PreCreateWindow(cs);
}

//##ModelId=49B87BB10192
BOOL CBatchFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(0, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CBatchDetailView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	CBatchDetailView* pView = (CBatchDetailView*)m_wndSplitter.GetPane(0, 1);
	
	CWnd* pWnd = m_wndSplitter.GetPane(0,0);
	pWnd->ShowWindow(FALSE);
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	return TRUE;
}

//##ModelId=49B87BB10198
void CBatchFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//取得客户区大小

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：窗口打开消息, 转发给各视图以便刷新
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1019D
void CBatchFrame::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//设置窗口标题
//	SetWindowText("批量召唤结果");
	//通知刷新
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);

	//LPARAM 表示视图类型
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(1));
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(1));
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 函 数 名：OnBatchCheck()
 功能概要：设备树对设备进行操作消息, 转发给详细视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101A5
void CBatchFrame::OnBatchCheck( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCH_CHECK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchOption()
 功能概要：召唤选项改变消息, 转发给详细视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101A9
void CBatchFrame::OnBatchOption( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCH_OPTION, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchDetailUncheck()
 功能概要：从详细视图取消某项选择, 转发给选择视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101AD
void CBatchFrame::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_DETAIL_UNCHECK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchUnselectAll()
 功能概要：取消所有选择消息, 转发给设备树窗口
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101B6
void CBatchFrame::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_UNSELECT_ALL, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchSelectAllPT()
 功能概要：响应保护全选消息, 转发给设备树视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101BA
void CBatchFrame::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_SELECT_ALL_PT, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchSelectAllWR()
 功能概要：响应录波器全选消息, 转发给设备树视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101BE
void CBatchFrame::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_SELECT_ALL_WR, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchEnableControls()
 功能概要：响应控件改变可用性消息, 转发给设备树视图.BATCH_ENABLE_CONTROLS
 返 回 值: void 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101C8
void CBatchFrame::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_ENABLE_CONTROLS, wParam, lParam);
}

//##ModelId=49B87BB1019B
void CBatchFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	//发送给各页面
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_CLOSE, 0, 0);
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_CLOSE, 0, 0);

	//发消息通知主框架
	//lparam参数表示是哪种窗口, 0-自动总召. 1-批量召唤
	AfxGetApp() ->m_pMainWnd->SendMessage(BATCH_FRAME_CLOSE, 0, 1);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 函 数 名：OnSTTP20154()
 功能概要: 收到20154,批量召唤回应报文,转发给详细视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101CC
void CBatchFrame::OnSTTP20154( WPARAM wParam, LPARAM lParam )
{
	//发送给各页面
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20154, wParam, lParam);
}

void CBatchFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//Invalidate();
}

void CBatchFrame::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	AfxGetApp() ->m_pMainWnd->PostMessage(BATCHCALL_END);
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   OnSTTP00901	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CBatchFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_00901, wParam, lParam);
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   OnSTTP20069	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CBatchFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20069, wParam, lParam);
}
