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
 �� �� ��: OnDownOscOpen()
 ���ܸ�Ҫ: ��Ӧ�򿪴��ڲ���
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 ָ��¼��
**************************************************************/
void CDownOscFrame::OnDownOscOpen( WPARAM wParam, LPARAM lParam )
{
	//���ô��ڱ���
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->SetAppTile("¼���ļ�����");
	//SetWindowText("¼���ļ�����");
	//֪ͨˢ��
	//CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//ת������ͼ
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(DOWNOSC_FRAME_OPEN, wParam, lParam);
	
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 �� �� ��: OnSTTP20069()
 ���ܸ�Ҫ: ת��20069���ĸ���ͼ
 �� �� ֵ: 
 ��    ��: param1 
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
 �� �� ��: OnSTTP20157()
 ���ܸ�Ҫ: ת��20157���ĸ���ͼ
 �� �� ֵ: 
 ��    ��: param1 
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
 �� �� ��: OnSTTP20125()
 ���ܸ�Ҫ: ת��20125���ĸ���ͼ
 �� �� ֵ: 
 ��    ��: param1 
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
 �� �� ��: OnSTTP20043()
 ���ܸ�Ҫ: ת��20043���ĸ���ͼ
 �� �� ֵ: 
 ��    ��: param1 
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
	//����Ϣ֪ͨ�����
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
