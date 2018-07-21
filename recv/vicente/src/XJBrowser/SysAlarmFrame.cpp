// SysAlarmFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysAlarmFrame.h"
#include "ViewSysAlarm.h"
#include "ViewSysAlarmDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysAlarmFrame

IMPLEMENT_DYNCREATE(CSysAlarmFrame, CMDIChildWnd)

//##ModelId=49B87B890021
CSysAlarmFrame::CSysAlarmFrame()
{
	m_bMaximized = FALSE;
}

//##ModelId=49B87B890022
CSysAlarmFrame::~CSysAlarmFrame()
{
}


BEGIN_MESSAGE_MAP(CSysAlarmFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSysAlarmFrame)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(VIEWSYSALARM_SELECTCHANGED, OnListSelChanged)
	ON_MESSAGE(WM_STTP_20155, OnSTTP20155)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20138, OnSTTP20138)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_STTP_20176, OnSTTP20176)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysAlarmFrame message handlers

//##ModelId=49B87B89002E
void CSysAlarmFrame::ActivateFrame(int nCmdShow) 
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

//##ModelId=49B87B890031
BOOL CSysAlarmFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(0, g_nSysAlarmSplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewSysAlarmDetail), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	return TRUE;
}

//##ModelId=49B87B890038
void CSysAlarmFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//ȡ�ÿͻ�����С
	CRect rc;
	GetClientRect(&rc);
	
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		if(g_ShowSysAlarmDetail <= 0)
		{	//����ʾ��ϸ
			int nHeight = rc.Height();
			m_wndSplitter.SetRowInfo(0, nHeight, 0);
		}
		else
		{
			m_wndSplitter.SetRowInfo(0, g_nSysAlarmSplitterWnd, 0);
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

//##ModelId=49B87B890035
BOOL CSysAlarmFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::PreCreateWindow(cs);

	//��ֹ��˸
	//�õ���Ļ�ֱ���
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //��   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //�� 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		/*| FWS_ADDTOTITLE*/ | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //�Ѵ�����ʽ����Ϊ��󻯣����Ȳ���ʾ
	
	cs.cx = nSx;   //�Ѵ��ڴ�С����Ϊ������Ļ��С
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //�������ֻ֮������ʾ����
	
	return TRUE;
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ���������ԶԻ���ȷ���¼���Ϣ,ת��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B890040
void CSysAlarmFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�¼���ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
	//�¼��б���ͼ
	pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ���¼���ȷ���¼���Ϣ, ת��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B890044
void CSysAlarmFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�¼���ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnListSelChanged()
 ���ܸ�Ҫ����Ӧ�¼��б�ѡ��ı���Ϣ, ת������ϸ��ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B890048
void CSysAlarmFrame::OnListSelChanged( WPARAM wParam, LPARAM lParam )
{
	//�¼���ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(VIEWSYSALARM_SELECTCHANGED, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20155()
 ���ܸ�Ҫ����Ӧ20155, �����ٻ���Ϣ�仯�澯֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B89004F
void CSysAlarmFrame::OnSTTP20155( WPARAM wParam, LPARAM lParam )
{
	//�¼��б���ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20155, wParam, lParam);
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CSysAlarmFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//�¼��б���ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(VIEW_REFRESH, wParam, lParam);
}

BOOL CSysAlarmFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		if(nCur > 0)
			g_nSysAlarmSplitterWnd = nCur;
	}
	return CMDIChildWnd::DestroyWindow();
}

/****************************************************
Date:2012/5/7  Author:LYH
������:   OnSTTP20138	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CSysAlarmFrame::OnSTTP20138( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20138, wParam, lParam);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CSysAlarmFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(STATIONINIT, wParam, lParam);
}

void CSysAlarmFrame::OnSTTP20176( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20176, wParam, lParam);
}
