/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   16:53
	filename: 	F:\vicente\Code\src\XJBrowser\ActionFrame.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	ActionFrame
	file ext:	cpp
	author:		LYH
	
	purpose:	�����¼����ڿ��ʵ��
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
	//ȡ�÷ָ����ڴ�С
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

//##ModelId=49B87BB501B6
void CActionFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//ȡ�ÿͻ�����С
	CRect rc;
	GetClientRect(&rc);
	
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		if(g_ShowActionDetail <= 0)
		{	
			//����ʾ��ϸ����
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
	//�յ������¼���ͼѡ��ı����Ϣ, ת���������¼���ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(VIEWACTION_SELECTCHANGED, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ, ת�����Ӵ���
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BB501CB
void CActionFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
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
 �� �� ����OnSTTP20022()
 ���ܸ�Ҫ����Ӧ���������¼�����֪ͨ, ת�����¼��б���ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BB501D5
void CActionFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	//�¼��б���ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20022, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ���¼���ȷ���¼���Ϣ, ת������ϸ������ͼ
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BB501D9
void CActionFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�¼���ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventPTAck()
 ���ܸ�Ҫ����Ӧ��������ҳ�����¼�ȷ�ϵ���Ϣ, ת�����¼��б���ͼ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BB501E4
void CActionFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//�¼��б���ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventPropAckPT()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ����ɱ��������Դ�ʱ�����¼�ȷ�ϵ���Ϣ, ת�����¼��б���ͼ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BB501E8
void CActionFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//�¼��б���ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

/*************************************************************
 �� �� ����RefreshActionView()
 ���ܸ�Ҫ����Ӧǿ��ˢ�¶����¼��б���Ϣ, ת���������¼���ͼ����
 �� �� ֵ: void
 ��    ����param1
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
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ת��ҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
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
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
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
������:   OnAllClose	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CActionFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
	{
		pView->SendMessage(WM_ALL_CLOSE, wParam, lParam);
	}
}
