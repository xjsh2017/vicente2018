// BatchFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchFrame.h"
#include "BatchDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMySplitter,CSplitterWnd)

BEGIN_MESSAGE_MAP(CMySplitter,CSplitterWnd)
ON_WM_NCHITTEST()
END_MESSAGE_MAP()

UINT CMySplitter::OnNcHitTest(CPoint point)
{
	//����HTNOWHERE...
	return HTNOWHERE;
}

/////////////////////////////////////////////////////////////////////////////
// CBatchFrame

IMPLEMENT_DYNCREATE(CBatchFrame, CMDIChildWnd)

//##ModelId=49B87BB10179
CBatchFrame::CBatchFrame()
{
	m_bMaximized = FALSE;
}

//##ModelId=49B87BB1017A
CBatchFrame::~CBatchFrame()
{
	TRACE("CBatchFrame::~CBatchFrame");
}


BEGIN_MESSAGE_MAP(CBatchFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBatchFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_CHECK, OnBatchCheck)
	ON_MESSAGE(BATCH_OPTION, OnBatchOption)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
	ON_MESSAGE(WM_STTP_20154, OnSTTP20154)
	ON_MESSAGE(BATCHCALL_END, OnBatchCallEnd)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchFrame message handlers

//##ModelId=49B87BB1018C
void CBatchFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if ( !m_bMaximized )
// 	{
// 		m_bMaximized =TRUE;
// 		nCmdShow =SW_SHOWMAXIMIZED;		
// 	}
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87BB1018F
BOOL CBatchFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CMDIChildWnd::PreCreateWindow(cs);
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
	
	cs.style |= WS_VISIBLE;
	return TRUE;
//	return CMDIChildWnd::PreCreateWindow(cs);
}

//##ModelId=49B87BB10192
BOOL CBatchFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(0, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CBatchDetailView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	CBatchDetailView* pView = (CBatchDetailView*)m_wndSplitter.GetPane(0, 1);
	
	CWnd* pWnd = m_wndSplitter.GetPane(0,0);
	pWnd->ShowWindow(FALSE);
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	return TRUE;
}

//##ModelId=49B87BB10198
void CBatchFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	//ȡ�ÿͻ�����С

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����OnBatchFrameOpen()
 ���ܸ�Ҫ�����ڴ���Ϣ, ת��������ͼ�Ա�ˢ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1019D
void CBatchFrame::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���ô��ڱ���
//	SetWindowText("�����ٻ����");
	//֪ͨˢ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);

	//LPARAM ��ʾ��ͼ����
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(1));
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(1));
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 �� �� ����OnBatchCheck()
 ���ܸ�Ҫ���豸�����豸���в�����Ϣ, ת������ϸ��ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101A5
void CBatchFrame::OnBatchCheck( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCH_CHECK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchOption()
 ���ܸ�Ҫ���ٻ�ѡ��ı���Ϣ, ת������ϸ��ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101A9
void CBatchFrame::OnBatchOption( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCH_OPTION, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchDetailUncheck()
 ���ܸ�Ҫ������ϸ��ͼȡ��ĳ��ѡ��, ת����ѡ����ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101AD
void CBatchFrame::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_DETAIL_UNCHECK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchUnselectAll()
 ���ܸ�Ҫ��ȡ������ѡ����Ϣ, ת�����豸������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101B6
void CBatchFrame::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_UNSELECT_ALL, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchSelectAllPT()
 ���ܸ�Ҫ����Ӧ����ȫѡ��Ϣ, ת�����豸����ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101BA
void CBatchFrame::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_SELECT_ALL_PT, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchSelectAllWR()
 ���ܸ�Ҫ����Ӧ¼����ȫѡ��Ϣ, ת�����豸����ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101BE
void CBatchFrame::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_SELECT_ALL_WR, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchEnableControls()
 ���ܸ�Ҫ����Ӧ�ؼ��ı��������Ϣ, ת�����豸����ͼ.BATCH_ENABLE_CONTROLS
 �� �� ֵ: void 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101C8
void CBatchFrame::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_ENABLE_CONTROLS, wParam, lParam);
}

//##ModelId=49B87BB1019B
void CBatchFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	//���͸���ҳ��
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_CLOSE, 0, 0);
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_CLOSE, 0, 0);

	//����Ϣ֪ͨ�����
	//lparam������ʾ�����ִ���, 0-�Զ�����. 1-�����ٻ�
	AfxGetApp() ->m_pMainWnd->SendMessage(BATCH_FRAME_CLOSE, 0, 1);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 �� �� ����OnSTTP20154()
 ���ܸ�Ҫ: �յ�20154,�����ٻ���Ӧ����,ת������ϸ��ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB101CC
void CBatchFrame::OnSTTP20154( WPARAM wParam, LPARAM lParam )
{
	//���͸���ҳ��
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20154, wParam, lParam);
}

void CBatchFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//Invalidate();
}

void CBatchFrame::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	AfxGetApp() ->m_pMainWnd->PostMessage(BATCHCALL_END);
}

/****************************************************
Date:2013/8/7  Author:LYH
������:   OnSTTP00901	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CBatchFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_00901, wParam, lParam);
}

/****************************************************
Date:2013/8/7  Author:LYH
������:   OnSTTP20069	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CBatchFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20069, wParam, lParam);
}
