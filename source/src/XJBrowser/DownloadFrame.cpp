// DownloadFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownloadFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadFrame

IMPLEMENT_DYNCREATE(CDownloadFrame, CMDIChildWnd)

//##ModelId=49B87BA3032D
CDownloadFrame::CDownloadFrame()
{
}

//##ModelId=49B87BA3032E
CDownloadFrame::~CDownloadFrame()
{
}


BEGIN_MESSAGE_MAP(CDownloadFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDownloadFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNLOAD_OPEN, OnDownloadOpen)
	ON_MESSAGE(WM_STTP_00204, OnSTTP00204)
	ON_MESSAGE(WM_STTP_00212, OnSTTP00212)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadFrame message handlers

//##ModelId=49B87BA30330
BOOL CDownloadFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
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
//	return CMDIChildWnd::PreCreateWindow(cs);
}

//##ModelId=49B87BA30333
void CDownloadFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->SendMessage(DOWNLOAD_CLOSE);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 函 数 名：OnDownloadOpen()
 功能概要：响应窗口打开消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30335
void CDownloadFrame::OnDownloadOpen( WPARAM wParam, LPARAM lParam )
{
	//设置窗口标题
	
//	SetWindowText("通用文件下载");
	//通知刷新
	//CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(DOWNLOAD_OPEN, 0, 0);
	
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 函 数 名：OnSTTP00204()
 功能概要：响应00204报文, 转发给视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA3033D
void CDownloadFrame::OnSTTP00204( WPARAM wParam, LPARAM lParam )
{
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_00204, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP00212()
 功能概要：响应00212报文, 转发给视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30341
void CDownloadFrame::OnSTTP00212( WPARAM wParam, LPARAM lParam )
{
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_00212, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：收到20069报文, 转发给视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30345
void CDownloadFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20069, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：收到20125报文, 转发给视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30349
void CDownloadFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20125, wParam, lParam);
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA3034E
void CDownloadFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20157, wParam, lParam);
}

void CDownloadFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
