// DisDevFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DisDevFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisDevFrame

IMPLEMENT_DYNCREATE(CDisDevFrame, CMDIChildWnd)

CDisDevFrame::CDisDevFrame()
{
	m_pTabView = NULL;
	m_pGeneralPage = NULL;
	m_pDistancePage = NULL;
	m_pAlarmPage = NULL;
	
	m_pCurSec = NULL;
}

CDisDevFrame::~CDisDevFrame()
{
}


BEGIN_MESSAGE_MAP(CDisDevFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDisDevFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISDEVFRAME_OPEN, OnOpenDisDevProp)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisDevFrame message handlers

void CDisDevFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CDisDevFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( !CMDIChildWnd::OnCreateClient(lpcs, pContext))
		return FALSE;
	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	if( pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CPTSheet)))
	{
		CPTSheet* pView = (CPTSheet*)pWnd; 
		m_pTabView = pView; 

		int nIndex = pView->Insert(0, StringFromID(IDS_SECPROP_GENERAL), RUNTIME_CLASS(CPTGeneral), pContext); 
		if(nIndex == -1)
		{ 
			return FALSE; 
		}
		//保存视图指针
		m_pGeneralPage = (CPTGeneral*)pView->GetView(nIndex);

		nIndex = pView->Insert(1, StringFromID(IDS_SECPROP_DISTANCE), RUNTIME_CLASS(CPTDistance), pContext); 
		if(nIndex == -1)
		{ 
			return FALSE; 
		}
		//保存视图指针
		m_pDistancePage = (CPTDistance*)pView->GetView(nIndex);

		nIndex = pView->Insert(2, StringFromID(IDS_SECPROP_ALARM), RUNTIME_CLASS(CPTAlarm), pContext); 
		if(nIndex == -1)
		{ 
			return FALSE; 
		}
		//保存视图指针
		m_pAlarmPage = (CPTAlarm*)pView->GetView(nIndex);

		SetActiveView(pView);
	}
	return TRUE;
}

BOOL CDisDevFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

void CDisDevFrame::OnOpenDisDevProp( WPARAM wParam, LPARAM lParam )
{
	CXJBrowserApp* pApp = (CXJBrowserApp *)AfxGetApp();
	
	//先转发给各属性性
	CSecObj* pObj = (CSecObj*)lParam;
	m_pCurSec = pObj;	
	
	if(m_pCurSec != NULL)
	{
		CString strStation = "";
		if(m_pCurSec->m_pStation != NULL)
			strStation = m_pCurSec->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pCurSec->m_sName);
		pApp->SetAppTile(str);
	}
	else
		pApp->SetAppTile("");
	
	pApp->GetMainWnd()->SendMessage(WM_MDITAB_REFRESH, 0, 0);
	//基本属性页
	if(m_pGeneralPage != NULL)
	{
		m_pGeneralPage ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	
	//测距信息
	if(m_pDistancePage != NULL)
	{
		m_pDistancePage ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	
	if(m_pAlarmPage != NULL)
	{
		m_pAlarmPage->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	
	//激活页面
	int nPage = (int)wParam;
	int nIndex = nPage;
	
	if(nIndex <= 2 && nIndex >= 0)
		m_pTabView->ActivateView(nIndex);
	else
		m_pTabView->ActivateView(1);
	
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

CDisDevFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	if(m_pGeneralPage)
		m_pGeneralPage->SendMessage(WM_PTFRAME_CLOSE, wParam, lParam);
	if(m_pDistancePage)
		m_pDistancePage->SendMessage(WM_PTFRAME_CLOSE, wParam, lParam);
	if(m_pAlarmPage)
		m_pAlarmPage->SendMessage(WM_PTFRAME_CLOSE, wParam, lParam);
}

void CDisDevFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->SendMessage(WM_DISDEVFRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

void CDisDevFrame::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	if(m_pDistancePage)
		m_pDistancePage->SendMessage(WM_STTP_20168, wParam, lParam);
}

void CDisDevFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	if(m_pAlarmPage)
		m_pAlarmPage->SendMessage(WM_STTP_20018, wParam, lParam);
}

void CDisDevFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	if(m_pDistancePage)
		m_pDistancePage->SendMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
	if(m_pAlarmPage)
		m_pAlarmPage->SendMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

void CDisDevFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pDistancePage)
		m_pDistancePage->SendMessage(WM_EVENTPROP_ACK, wParam, lParam);
	if(m_pAlarmPage)
		m_pAlarmPage->SendMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

void CDisDevFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pDistancePage)
		m_pDistancePage->SendMessage(WM_EVENTLIST_ACK, wParam, lParam);
	if(m_pAlarmPage)
		m_pAlarmPage->SendMessage(WM_EVENTLIST_ACK, wParam, lParam);
}
