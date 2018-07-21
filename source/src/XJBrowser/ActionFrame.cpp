/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   16:53
	filename: 	F:\vicente\Code\src\XJBrowser\ActionFrame.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	ActionFrame
	file ext:	cpp
	author:		LYH
	
	purpose:	动作事件窗口框架实现
*********************************************************************/
// ActionFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ActionFrame.h"
#include "ViewActionDetail.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CActionFrame

IMPLEMENT_DYNCREATE(CActionFrame, CMDIChildWnd)

//##ModelId=49B87BB50198
CActionFrame::CActionFrame()
{
	m_bMaximized = FALSE;
	m_nOldHeight = 0;
	//取得分隔窗口大小
}

//##ModelId=49B87BB50199
CActionFrame::~CActionFrame()
{
	TRACE("CActionFrame::~CActionFrame");
}


BEGIN_MESSAGE_MAP(CActionFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CActionFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWACTION_SELECTCHANGED, OnViewActionSelectChanged)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(REFRESH_ACTIONVIEW, RefreshActionView)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionFrame message handlers

//##ModelId=49B87BB501A9
BOOL CActionFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(0, g_nActionSplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewActionDetail), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));


	return TRUE;
}

//##ModelId=49B87BB501A6
void CActionFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( !m_bMaximized )
	{
		m_bMaximized =TRUE;
		nCmdShow =SW_SHOWMAXIMIZED;		
	}
	
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87BB501AD
BOOL CActionFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

//##ModelId=49B87BB501B6
void CActionFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//取得客户区大小
	CRect rc;
	GetClientRect(&rc);
	
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		if(g_ShowActionDetail <= 0)
		{	
			//不显示详细窗口
			int nHeight = rc.Height();
			m_wndSplitter.SetRowInfo(0, nHeight, 0);
		}
		else
		{
			m_wndSplitter.SetRowInfo(0, g_nActionSplitterWnd, 0);
// 			int nCur = 0;
// 			int nMin = 0;
// 			m_wndSplitter.GetRowInfo(1, nCur, nMin);
// 			if(nCur <= 0)
// 			{
// 				//	m_wndSplitter.SetRowInfo(1, g_DetailWndHeight, 0);
// 				m_wndSplitter.GetRowInfo(0, nCur, nMin);
// 				m_wndSplitter.SetRowInfo(0, (nCur-g_DetailWndHeight)>0?(nCur-g_DetailWndHeight):350, nMin);
// 			}
		}
	}

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
}

//##ModelId=49B87BB501B9
void CActionFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

//##ModelId=49B87BB501BE
void CActionFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnSysCommand(nID, lParam);
}

//##ModelId=49B87BB501C7
void CActionFrame::OnViewActionSelectChanged(WPARAM wParam, LPARAM lParam)
{
	//收到动作事件视图选择改变的消息, 转发给动作事件详细内容视图
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(VIEWACTION_SELECTCHANGED, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息, 转发给子窗口
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB501CB
void CActionFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//事件详细内容视图
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
	//事件列表视图
	pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20022()
 功能概要：响应保护动作事件上载通知, 转发给事件列表视图
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BB501D5
void CActionFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20022, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventListAck()
 功能概要：事件栏确认事件消息, 转发给详细内容视图
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB501D9
void CActionFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//事件详细内容视图
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPTAck()
 功能概要：响应保护属性页发生事件确认的消息, 转发给事件列表视图
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB501E4
void CActionFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPropAckPT()
 功能概要：响应事件属性对话框由保护属性性打开时发生事件确认的消息, 转发给事件列表视图
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB501E8
void CActionFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

/*************************************************************
 函 数 名：RefreshActionView()
 功能概要：响应强制刷新动作事件列表消息, 转发给动作事件视图窗口
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB501EC
void CActionFrame::RefreshActionView( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
	{
		pView->PostMessage(REFRESH_ACTIONVIEW, wParam, lParam);
	}
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 转发页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CActionFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
	{
		pView->PostMessage(VIEW_REFRESH, wParam, lParam);
	}
}

void CActionFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		if(nCur > 0)
			g_nActionSplitterWnd = nCur;
	}
	CMDIChildWnd::OnClose();
}

BOOL CActionFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		g_nActionSplitterWnd = nCur;
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
void CActionFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
	{
		pView->SendMessage(STATIONINIT, wParam, lParam);
	}
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   OnAllClose	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CActionFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
	{
		pView->SendMessage(WM_ALL_CLOSE, wParam, lParam);
	}
}
