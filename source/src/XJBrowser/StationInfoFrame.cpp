// StationInfoFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "StationInfoFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStationInfoFrame

IMPLEMENT_DYNCREATE(CStationInfoFrame, CMDIChildWnd)

CStationInfoFrame::CStationInfoFrame()
{
	m_bMaximized = FALSE;
}

CStationInfoFrame::~CStationInfoFrame()
{
}


BEGIN_MESSAGE_MAP(CStationInfoFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStationInfoFrame)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStationInfoOpen)
	ON_MESSAGE(STATIONINFO_FRAME_CLOSE, OnStationInfoClose)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStationInfoFrame message handlers

void CStationInfoFrame::ActivateFrame(int nCmdShow) 
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

BOOL CStationInfoFrame::PreCreateWindow(CREATESTRUCT& cs) 
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
 �� �� ��: OnStationInfoOpen()
 ���ܸ�Ҫ: ת����վ��Ϣ���ڴ���Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CStationInfoFrame::OnStationInfoOpen( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CStationObj* pObj = (CStationObj*)lParam;
	
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINFO_FRAME_OPEN, wParam, lParam);
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
}

/*************************************************************
 �� �� ��: OnStationInfoClose()
 ���ܸ�Ҫ: ת����վ��Ϣ���ڹر���Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CStationInfoFrame::OnStationInfoClose( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINFO_FRAME_CLOSE, wParam, lParam);
}

void CStationInfoFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//����Ϣ֪ͨ�����
	AfxGetApp() ->m_pMainWnd->PostMessage(STATIONINFO_FRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

void CStationInfoFrame::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20002, wParam, lParam);
}

void CStationInfoFrame::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20079, wParam, lParam);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CStationInfoFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINIT, wParam, lParam);
}
