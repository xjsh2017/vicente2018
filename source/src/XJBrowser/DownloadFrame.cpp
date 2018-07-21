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

//##ModelId=49B87BA30333
void CDownloadFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//����Ϣ֪ͨ�����
	AfxGetApp() ->m_pMainWnd->SendMessage(DOWNLOAD_CLOSE);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 �� �� ����OnDownloadOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30335
void CDownloadFrame::OnDownloadOpen( WPARAM wParam, LPARAM lParam )
{
	//���ô��ڱ���
	
//	SetWindowText("ͨ���ļ�����");
	//֪ͨˢ��
	//CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(DOWNLOAD_OPEN, 0, 0);
	
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 �� �� ����OnSTTP00204()
 ���ܸ�Ҫ����Ӧ00204����, ת������ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA3033D
void CDownloadFrame::OnSTTP00204( WPARAM wParam, LPARAM lParam )
{
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_00204, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP00212()
 ���ܸ�Ҫ����Ӧ00212����, ת������ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30341
void CDownloadFrame::OnSTTP00212( WPARAM wParam, LPARAM lParam )
{
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_00212, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ���յ�20069����, ת������ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30345
void CDownloadFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20069, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ���յ�20125����, ת������ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30349
void CDownloadFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20125, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA3034E
void CDownloadFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
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
