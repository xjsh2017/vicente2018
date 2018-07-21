// AutoCallFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AutoCallFrame.h"
#include "BatchDetailSettingView.h"
#include "BatchDetailView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCallFrame

IMPLEMENT_DYNCREATE(CAutoCallFrame, CMDIChildWnd)

//##ModelId=49B87BB500DA
CAutoCallFrame::CAutoCallFrame()
{
	m_bMaximized = FALSE;
}

//##ModelId=49B87BB500DB
CAutoCallFrame::~CAutoCallFrame()
{
	TRACE("CAutoCallFrame::~CAutoCallFrame");
}


BEGIN_MESSAGE_MAP(CAutoCallFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAutoCallFrame)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_CHECK, OnBatchCheck)
    ON_MESSAGE(SHOW_BATCH, OnShowList)
	ON_MESSAGE(BATCH_OPTION, OnBatchOption)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
	ON_MESSAGE(AUTOCALL_STATION_CHANGE, OnAutoCallStationChange)
	ON_MESSAGE(BATCHCALL_END, OnBatchCallEnd)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCallFrame message handlers

//##ModelId=49B87BB500E2
void CAutoCallFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow =SW_SHOWMAXIMIZED;	
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87BB500F2
void CAutoCallFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

//##ModelId=49B87BB500EB
BOOL CAutoCallFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CMDIChildWnd::PreCreateWindow(cs);
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
}

//##ModelId=49B87BB500EE
BOOL CAutoCallFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		pContext->m_pNewViewClass, CSize(400, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CBatchDetailSettingView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	CBatchDetailSettingView* pView = (CBatchDetailSettingView*)m_wndSplitter.GetPane(0, 1);
	if(pView!= NULL)
	{
		pView->m_nViewType = 0;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));
	return TRUE;
}

/*************************************************************
 �� �� ����OnBatchFrameOpen()
 ���ܸ�Ҫ�����ڴ���Ϣ, ת��������ͼ�Ա�ˢ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB500FD
void CAutoCallFrame::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���ô��ڱ���
	//SetWindowText("�����ٻ�");
	//֪ͨˢ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);

	//LPARAM ��ʾ��ͼ����
	CView* pView  = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(0));
	pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(BATCH_FRAME_OPEN, wParam, LPARAM(0));
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
//##ModelId=49B87BB50101
void CAutoCallFrame::OnBatchCheck( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB5010A
void CAutoCallFrame::OnBatchOption( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB5010E
void CAutoCallFrame::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87BB50112
void CAutoCallFrame::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_UNSELECT_ALL, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchSelectAllPT()
 ���ܸ�Ҫ����Ӧ����ȫѡ��Ϣ, ת�����豸����ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5011C
void CAutoCallFrame::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_SELECT_ALL_PT, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchSelectAllWR()
 ���ܸ�Ҫ����Ӧ¼����ȫѡ��Ϣ, ת�����豸����ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50120
void CAutoCallFrame::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_SELECT_ALL_WR, wParam, lParam);
}

/*************************************************************
 �� �� ����OnBatchEnableControls()
 ���ܸ�Ҫ����Ӧ�ؼ��ı��������Ϣ, ת�����豸����ͼ.BATCH_ENABLE_CONTROLS
 �� �� ֵ: void 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50129
void CAutoCallFrame::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 0);
	if(pView != NULL)
		pView->PostMessage(BATCH_ENABLE_CONTROLS, wParam, lParam);
}

//##ModelId=49B87BB500FB
void CAutoCallFrame::OnClose() 
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
	AfxGetApp() ->m_pMainWnd->SendMessage(BATCH_FRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

void CAutoCallFrame::OnAutoCallStationChange( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(AUTOCALL_STATION_CHANGE, wParam, lParam);
}


void CAutoCallFrame::OnShowList( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(SHOW_BATCH, wParam, lParam);
}

void CAutoCallFrame::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->PostMessage(BATCHCALL_END, wParam, lParam);
}

/****************************************************
Date:2013/8/7  Author:LYH
������:   OnSTTP00901	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CAutoCallFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
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
void CAutoCallFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	CView* pView = (CView*)m_wndSplitter.GetPane(0, 1);
	if(pView != NULL)
		pView->SendMessage(WM_STTP_20069, wParam, lParam);
}
