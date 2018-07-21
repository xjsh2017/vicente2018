/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   17:00
	filename: 	F:\vicente\Code\src\XJBrowser\CommFrame.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	CommFrame
	file ext:	cpp
	author:		LYH
	
	purpose:	����վͨѶ״̬���ڿ��
*********************************************************************/
// CommFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CommFrame.h"

#include "ViewComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommFrame

IMPLEMENT_DYNCREATE(CCommFrame, CMDIChildWnd)

//##ModelId=49B87BAA0011
CCommFrame::CCommFrame()
{
	m_bMaximized = FALSE;
	m_pComm = NULL;
	m_pCommMap = NULL;
	m_nOldHeight = 0;
}

//##ModelId=49B87BAA0035
CCommFrame::~CCommFrame()
{
	TRACE("CCommFrame::~CCommFrame");
}


BEGIN_MESSAGE_MAP(CCommFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCommFrame)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20146, OnSTTP20146)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(STATION_CHANGED, OnStationStatusChanged)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(STATION_INFO_CHANGED, OnStationInfoChanged)
	ON_MESSAGE(TEST_STATIONSTATUS, OnTestStationStatus)
	ON_MESSAGE(REFRESH_COMMMAP, OnRefreshCommMap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommFrame message handlers

//##ModelId=49B87BAA002E
BOOL CCommFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(0, g_nCommSplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewComm), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	
	//��¼�б���ͼ��ָ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	m_pCommMap = (CViewCommMap*)m_wndSplitter.GetPane(0, 0);
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));		
	pApp->m_pComm = (CViewComm*)(m_wndSplitter.GetPane(1, 0));
	m_pComm = (CViewComm*)m_wndSplitter.GetPane(1, 0);
	
	
	return TRUE;
}

//##ModelId=49B87BAA0025
void CCommFrame::ActivateFrame(int nCmdShow) 
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

//##ModelId=49B87BAA0032
BOOL CCommFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
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

//##ModelId=49B87BAA003E
void CCommFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//ȡ�ÿͻ�����С
	CRect rc;
	GetClientRect(&rc);
	
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnSTTP20144()
 ���ܸ�Ҫ��ת������վͨѶ״̬�仯(20144)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAA0041
void CCommFrame::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(WM_STTP_20144, wParam, lParam);
	}
	if(m_pCommMap != NULL)
	{
		m_pCommMap->SendMessage(WM_STTP_20144, wParam, lParam);
	}
}

//##ModelId=49B87BAA0044
void CCommFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(WM_ALL_CLOSE);
	}
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CCommFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
	
	if(m_pCommMap != NULL)
	{
		m_pCommMap->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
}

void CCommFrame::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(WM_STTP_20146, wParam, lParam);
	}
}

void CCommFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

BOOL CCommFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		g_nCommSplitterWnd = nCur;
	}
	return CMDIChildWnd::DestroyWindow();
}

/****************************************************
Date:2012/5/23  Author:LYH
������:   OnStationStatusChanged	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CCommFrame::OnStationStatusChanged( WPARAM wParam, LPARAM lParam )
{
	if(m_pCommMap != NULL)
	{
		m_pCommMap->PostMessage(STATION_CHANGED, wParam, lParam);
	}
}

/****************************************************
Date:2013/6/4  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CCommFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
	{
		m_pComm->SendMessage(STATIONINIT, wParam, lParam);
	}
	if(m_pCommMap != NULL)
	{
		m_pCommMap->SendMessage(STATIONINIT, wParam, lParam);
	}
}

void CCommFrame::OnStationInfoChanged( WPARAM wParam, LPARAM lParam )
{
	if(m_pCommMap != NULL)
	{
		m_pCommMap->PostMessage(STATION_INFO_CHANGED, wParam, lParam);
	}
}

/****************************************************
Date:2013/10/8  Author:LYH
������:   OnTestStationStatus	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CCommFrame::OnTestStationStatus( WPARAM wParam, LPARAM lParam )
{
	if(m_pComm != NULL)
		m_pComm->PostMessage(TEST_STATIONSTATUS, wParam, lParam);
}

void CCommFrame::OnRefreshCommMap(WPARAM wParam, LPARAM lParam)
{
	if(m_pCommMap != NULL)
	{
		m_pCommMap->PostMessage(REFRESH_COMMMAP, wParam, lParam);
	}
}


