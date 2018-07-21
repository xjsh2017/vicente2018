// DownOscFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownOscFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownOscFrame

IMPLEMENT_DYNCREATE(CDownOscFrame, CMDIChildWnd)

CDownOscFrame::CDownOscFrame()
{
}

CDownOscFrame::~CDownOscFrame()
{
}


BEGIN_MESSAGE_MAP(CDownOscFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDownOscFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNOSC_FRAME_OPEN, OnDownOscOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownOscFrame message handlers

BOOL CDownOscFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

/*************************************************************
 函 数 名: OnDownOscOpen()
 功能概要: 响应打开窗口操作
 返 回 值: 
 参    数: param1 
		   Param2 指定录波
**************************************************************/
void CDownOscFrame::OnDownOscOpen( WPARAM wParam, LPARAM lParam )
{
	//设置窗口标题
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->SetAppTile("录波文件下载");
	//SetWindowText("录波文件下载");
	//通知刷新
	//CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//转发给视图
	//取得活动视图指针
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(DOWNOSC_FRAME_OPEN, wParam, lParam);
	
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 函 数 名: OnSTTP20069()
 功能概要: 转发20069报文给视图
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20069, wParam, lParam);
}

/*************************************************************
 函 数 名: OnSTTP20157()
 功能概要: 转发20157报文给视图
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20157, wParam, lParam);
}

/*************************************************************
 函 数 名: OnSTTP20125()
 功能概要: 转发20125报文给视图
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20125, wParam, lParam);
}

/*************************************************************
 函 数 名: OnSTTP20043()
 功能概要: 转发20043报文给视图
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscFrame::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20043, wParam, lParam);
}

void CDownOscFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->SendMessage(DOWNOSC_FRAME_CLOSE);
	CMDIChildWnd::OnClose();
}

void CDownOscFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
