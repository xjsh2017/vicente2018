// AutoResultFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AutoResultFrame.h"
#include "AutoCallResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoResultFrame

IMPLEMENT_DYNCREATE(CAutoResultFrame, CMDIChildWnd)

//##ModelId=49B87BB3038C
CAutoResultFrame::CAutoResultFrame()
{
}

//##ModelId=49B87BB3038D
CAutoResultFrame::~CAutoResultFrame()
{
	TRACE("CAutoResultFrame::~CAutoResultFrame");
}


BEGIN_MESSAGE_MAP(CAutoResultFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAutoResultFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(AUTOCALL_RESULT_OPEN, OnAutoCallResultFrameOpen)
	ON_MESSAGE(AUTOCALL_RESULT_CHANGE, OnAutoCallResultChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoResultFrame message handlers

//##ModelId=49B87BB3039F
BOOL CAutoResultFrame::PreCreateWindow(CREATESTRUCT& cs) 
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

/*************************************************************
 �� �� ����OnAutoCallResultFrameOpen()
 ���ܸ�Ҫ����Ӧ�Զ����ٴ��ڴ���Ϣ,�ı��ĵ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB303AA
void CAutoResultFrame::OnAutoCallResultFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���ô��ڱ���
//	SetWindowText("�Զ����ٽ��");
	//֪ͨˢ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);

	//ת������ͼ
	//ȡ�û��ͼָ��
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView == NULL)
		return;
	pView->SendMessage(AUTOCALL_RESULT_OPEN, 0, 0);
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView == NULL)
		return;
	pView->SendMessage(AUTOCALL_RESULT_OPEN);
	
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);

}

//##ModelId=49B87BB303A6
void CAutoResultFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//����Ϣ֪ͨ�����
	AfxGetApp() ->m_pMainWnd->SendMessage(AUTOCALL_RESULT_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

//##ModelId=49B87BB303A2
BOOL CAutoResultFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(200, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CAutoCallResult), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	//��¼��ϸ��ͼ��ָ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->m_pAutoCallResult = (CAutoCallResult*)(m_wndSplitter.GetPane(0, 1));
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	return TRUE;
}

/*************************************************************
 �� �� ����OnAutoCallResultChange()
 ���ܸ�Ҫ����Ӧ��վѡ�񴰿�ѡ��ı���Ϣ, ת������ʾ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB303AE
void CAutoResultFrame::OnAutoCallResultChange( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(AUTOCALL_RESULT_CHANGE, wParam, lParam);
}

void CAutoResultFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
