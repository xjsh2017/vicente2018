// CrossFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CrossFrame.h"
#include "ViewCrossDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrossFrame

IMPLEMENT_DYNCREATE(CCrossFrame, CMDIChildWnd)

CCrossFrame::CCrossFrame()
{
}

CCrossFrame::~CCrossFrame()
{
}


BEGIN_MESSAGE_MAP(CCrossFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCrossFrame)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWCROSS_SELECTCHANGED, OnViewCrossSelChanged)
	ON_MESSAGE(WM_STTP_20159, OnSTTP20159)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrossFrame message handlers

BOOL CCrossFrame::PreCreateWindow(CREATESTRUCT& cs) 
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
//	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL CCrossFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(0, g_nCrossSplitterWnd), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CViewCrossDetail), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	return TRUE;
}

void CCrossFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{

	//ȡ�ÿͻ�����С
	CRect rc;
	GetClientRect(&rc);

	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ��: OnViewCrossSelChanged()
 ���ܸ�Ҫ: ��Ԥ����ͼѡ��ı����Ϣ, ת����Ԥ����ϸ������ͼ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CCrossFrame::OnViewCrossSelChanged( WPARAM wParam, LPARAM lParam )
{
	//�յ�Ԥ����ͼѡ��ı����Ϣ, ת����Ԥ����ϸ������ͼ
	CView * pView = (CView*)m_wndSplitter.GetPane(1,0);
	if(pView == NULL)
		return;
	pView->PostMessage(VIEWCROSS_SELECTCHANGED, wParam, lParam);
}

/*************************************************************
 �� �� ��: OnSTTP20159()
 ���ܸ�Ҫ: Ԥ���ṹ֪ͨ����, ת������ͼ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CCrossFrame::OnSTTP20159( WPARAM wParam, LPARAM lParam )
{
	CView * pView = (CView*)m_wndSplitter.GetPane(0,0);
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20159, wParam, lParam);
}

BOOL CCrossFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 0)) != NULL)
	{
		int nCur = 0;
		int nMin = 0;
		m_wndSplitter.GetRowInfo(0, nCur, nMin);
		g_nCrossSplitterWnd = nCur;
	}
	return CMDIChildWnd::DestroyWindow();
}

void CCrossFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
