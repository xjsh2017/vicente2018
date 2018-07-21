/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   16:56
	filename: 	F:\vicente\Code\src\XJBrowser\AlarmFrame.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	AlarmFrame
	file ext:	cpp
	author:		LYH
	
	purpose:	告警事件窗口框架实现
*********************************************************************/
// AlarmFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AlarmFrame.h"

#include "ViewAlarmDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmFrame

IMPLEMENT_DYNCREATE(CAlarmFrame, CMDIChildWnd)

//##ModelId=49B87BB5013A
CAlarmFrame::CAlarmFrame()
{
	m_bMaximized = FALSE;
	m_nOldHeight = 0;
}

//##ModelId=49B87BB5013B
CAlarmFrame::~CAlarmFrame()
{
	TRACE("CAlarmFrame::~CAlarmFrame");
}


BEGIN_MESSAGE_MAP(CAlarmFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAlarmFrame)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWALARM_SELECTCHANGED, OnViewAlarmSelectChanged)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(REFRESH_ALARMVIEW, OnRefreshAlarmView)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmFrame message handlers

//##ModelId=49B87BB50149
void CAlarmFrame::ActivateFrame(int nCmdShow) 
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

//##ModelId=49B87BB5014C
BOOL CAlarmFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	// create a splitter with 2 row, 1 columns
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	if(g_nAlarmSplitterWnd == 0 && g_ShowAlarmDetail == 1)
		g_nAlarmSplitterWnd = 350;
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(0, g_nAlarmSplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewAlarmDetail), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));

	return TRUE;
}

//##ModelId=49B87BB50150
BOOL CAlarmFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

//##ModelId=49B87BB50159
void CAlarmFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{	
	// TODO: Add your message handler code here
	//取得客户区大小
	CRect rc;
	GetClientRect(&rc);
	
	//设置两个窗口的大小, 隐藏详细信息窗口
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		if(g_ShowAlarmDetail <= 0)
		{	//不显示详细窗口
			int nHeight = rc.Height();
			m_wndSplitter.SetRowInfo(0, nHeight, 0);
		}
		else
		{
			m_wndSplitter.SetRowInfo(0, g_nAlarmSplitterWnd, 0);
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
}

//##ModelId=49B87BB5015C
void CAlarmFrame::OnViewAlarmSelectChanged(WPARAM wParam, LPARAM lParam)
{
	//收到动作事件视图选择改变的消息, 转发给动作事件详细内容视图
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(VIEWALARM_SELECTCHANGED, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息,转发给各视图
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB50167
void CAlarmFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
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
 函 数 名：OnSTTP20018()
 功能概要：响应告警上载通知,转发给事件列表视图
 返 回 值: 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BB5016B
void CAlarmFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20018, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventListAck()
 功能概要：事件栏确认事件消息, 转发给详细内容视图
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB5016F
void CAlarmFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//事件详细内容视图
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPTAck()
 功能概要：响应保护属性页面发生事件确认的消息,转发给事件列表视图
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB50178
void CAlarmFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPropAckPT()
 功能概要：响应由保护属性页找开的事件属性对话框发生事件确认的消息, 转发给事件列表视图
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BB5017C
void CAlarmFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

/*************************************************************
 函 数 名：OnRefreshAlarmView()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50186
void CAlarmFrame::OnRefreshAlarmView( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(REFRESH_ALARMVIEW, wParam, lParam);
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 转发页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CAlarmFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(VIEW_REFRESH, wParam, lParam);
}

BOOL CAlarmFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		if(nCur > 0)
			g_nAlarmSplitterWnd = nCur;
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
void CAlarmFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	//事件列表视图
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(STATIONINIT, wParam, lParam);
}
