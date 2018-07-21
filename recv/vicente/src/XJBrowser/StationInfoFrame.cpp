// StationInfoFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "StationInfoFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStationInfoFrame

IMPLEMENT_DYNCREATE(CStationInfoFrame, CMDIChildWnd)

CStationInfoFrame::CStationInfoFrame()
{
	m_bMaximized = FALSE;
}

CStationInfoFrame::~CStationInfoFrame()
{
}


BEGIN_MESSAGE_MAP(CStationInfoFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStationInfoFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStationInfoOpen)
	ON_MESSAGE(STATIONINFO_FRAME_CLOSE, OnStationInfoClose)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStationInfoFrame message handlers

void CStationInfoFrame::ActivateFrame(int nCmdShow) 
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

BOOL CStationInfoFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

/*************************************************************
 函 数 名: OnStationInfoOpen()
 功能概要: 转发厂站信息窗口打开消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CStationInfoFrame::OnStationInfoOpen( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CStationObj* pObj = (CStationObj*)lParam;
	
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINFO_FRAME_OPEN, wParam, lParam);
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 函 数 名: OnStationInfoClose()
 功能概要: 转发厂站信息窗口关闭消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CStationInfoFrame::OnStationInfoClose( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINFO_FRAME_CLOSE, wParam, lParam);
}

void CStationInfoFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->PostMessage(STATIONINFO_FRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

void CStationInfoFrame::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20002, wParam, lParam);
}

void CStationInfoFrame::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20079, wParam, lParam);
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CStationInfoFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINIT, wParam, lParam);
}
