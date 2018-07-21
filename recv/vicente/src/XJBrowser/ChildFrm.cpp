// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "ChildFrm.h"

#include "ViewDI.h"
#include "ViewPTComm.h"
#include "ViewPTRun.h"
#include "ViewOSC.h"
#include "ViewStaInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(WM_STTP_20132, OnSTTP20132)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(STATION_CHANGED, OnStationChanged)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

//##ModelId=49B87BAB0138
CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_bMaximized = FALSE;
	m_nType = -1;
}

//##ModelId=49B87BAB0141
CChildFrame::~CChildFrame()
{
	TRACE("CChildFrame::~CChildFrame");
}

//##ModelId=49B87BAB013B
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//防止闪烁
	//得到屏幕分辨率
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //宽   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //高 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| /*FWS_ADDTOTITLE |*/ WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //把窗口样式设置为最大化，但先不显示
	
	cs.cx = nSx;   //把窗口大小设置为整个屏幕大小
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //创建完成只之后再显示窗口
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
//##ModelId=49B87BAB0143
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

//##ModelId=49B87BAB0148
void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

//##ModelId=49B87BAB013E
void CChildFrame::ActivateFrame(int nCmdShow) 
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

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息
 返 回 值: void
 参    数：param1	事件属性
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87BAB014B
void CChildFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针,发送消息
	CView * pView = GetActiveView();
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20002()
 功能概要：响应保护通讯状态上载通知,转发给视图
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BAB014F
void CChildFrame::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//判断是否通讯状态视图
	if(pView->IsKindOf(RUNTIME_CLASS(CViewPTComm)))
	{
		pView->SendMessage(WM_STTP_20002, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20010()
 功能概要：响应开关量采样值上载通知, 转发给视图
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BAB0153
void CChildFrame::OnSTTP20010( WPARAM wParam , LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//判断是否开关量事件视图
	if(pView->IsKindOf(RUNTIME_CLASS(CViewDI)))
	{
		pView->SendMessage(WM_STTP_20010, wParam, lParam);
	}
}
/*************************************************************
 函 数 名：OnSTTP20010()
 功能概要：响应开关量采样值上载通知, 转发给视图
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BAB0153
void CChildFrame::OnSTTP20144( WPARAM wParam , LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//判断是否开关量事件视图
	if(pView->IsKindOf(RUNTIME_CLASS(CViewStaInfo)))
	{
		pView->SendMessage(WM_STTP_20144, wParam, lParam);
	}
}
/*************************************************************
 函 数 名：OnSTTP20079()
 功能概要：响应保护运行上载通知, 转发给视图
 返 回 值: void
 参    数：param1	消息参数	
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BAB0158
void CChildFrame::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//判断是否运行状态事件视图
	if(pView->IsKindOf(RUNTIME_CLASS(CViewPTRun)))
	{
		pView->SendMessage(WM_STTP_20079, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20132()
 功能概要：响应录波文件上载通知, 转发给视图
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BAB015C
void CChildFrame::OnSTTP20132( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//判断是否录波事件视图
	if(pView->IsKindOf(RUNTIME_CLASS(CViewOSC)))
	{
		pView->SendMessage(WM_STTP_20132, wParam, lParam);
	}
}

//##ModelId=49B87BAB0160
void CChildFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_ALL_CLOSE, wParam, lParam);
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 转发页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CChildFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(VIEW_REFRESH, wParam, lParam);
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_capp.Install(this, WM_PAINTMYCAPTION);
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CChildFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINIT, wParam, lParam);
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   OnSTTP20168	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CChildFrame::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20168, wParam, lParam);
}

void CChildFrame::OnStationChanged( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATION_CHANGED, wParam, lParam);
}

void CChildFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_EVENTPT_ACK, wParam, lParam);
}

void CChildFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

