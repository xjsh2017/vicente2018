// AutoCallFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AutoCallFrame.h"
#include "BatchDetailSettingView.h"
#include "BatchDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCallFrame

IMPLEMENT_DYNCREATE(CAutoCallFrame, CMDIChildWnd)

//##ModelId=49B87BB500DA
CAutoCallFrame::CAutoCallFrame()
{
	m_bMaximized = FALSE;
}

//##ModelId=49B87BB500DB
CAutoCallFrame::~CAutoCallFrame()
{
	TRACE("CAutoCallFrame::~CAutoCallFrame");
}


BEGIN_MESSAGE_MAP(CAutoCallFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAutoCallFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_CHECK, OnBatchCheck)
    ON_MESSAGE(SHOW_BATCH, OnShowList)
	ON_MESSAGE(BATCH_OPTION, OnBatchOption)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
	ON_MESSAGE(AUTOCALL_STATION_CHANGE, OnAutoCallStationChange)
	ON_MESSAGE(BATCHCALL_END, OnBatchCallEnd)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCallFrame message handlers

//##ModelId=49B87BB500E2
void CAutoCallFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow =SW_SHOWMAXIMIZED;	
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87BB500F2
void CAutoCallFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

//##ModelId=49B87BB500EB
BOOL CAutoCallFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CMDIChildWnd::PreCreateWindow(cs);
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
}

//##ModelId=49B87BB500EE
BOOL CAutoCallFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(400, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CBatchDetailSettingView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	CBatchDetailSettingView* pView = (CBatchDetailSettingView*)m_wndSplitter.GetPane(0, 1);
	if(pView!= NULL)
	{
		pView->m_nViewType = 0;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	return TRUE;
}

/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：窗口打开消息, 转发给各视图以便刷新
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB500FD
void CAutoCallFrame::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//设置窗口标题
	//SetWindowText("批量召唤");
	//通知刷新
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);

	//LPARAM 表示视图类型
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(0));
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(0));
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
//##ModelId=49B87BB50101
void CAutoCallFrame::OnBatchCheck( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB5010A
void CAutoCallFrame::OnBatchOption( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB5010E
void CAutoCallFrame::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB50112
void CAutoCallFrame::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_UNSELECT_ALL, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchSelectAllPT()
 功能概要：响应保护全选消息, 转发给设备树视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5011C
void CAutoCallFrame::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_SELECT_ALL_PT, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchSelectAllWR()
 功能概要：响应录波器全选消息, 转发给设备树视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50120
void CAutoCallFrame::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_SELECT_ALL_WR, wParam, lParam);
}

/*************************************************************
 函 数 名：OnBatchEnableControls()
 功能概要：响应控件改变可用性消息, 转发给设备树视图.BATCH_ENABLE_CONTROLS
 返 回 值: void 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50129
void CAutoCallFrame::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_ENABLE_CONTROLS, wParam, lParam);
}

//##ModelId=49B87BB500FB
void CAutoCallFrame::OnClose() 
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
	AfxGetApp() ->m_pMainWnd->SendMessage(BATCH_FRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

void CAutoCallFrame::OnAutoCallStationChange( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(AUTOCALL_STATION_CHANGE, wParam, lParam);
}


void CAutoCallFrame::OnShowList( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(SHOW_BATCH, wParam, lParam);
}

void CAutoCallFrame::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCHCALL_END, wParam, lParam);
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   OnSTTP00901	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CAutoCallFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
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
void CAutoCallFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20069, wParam, lParam);
}
