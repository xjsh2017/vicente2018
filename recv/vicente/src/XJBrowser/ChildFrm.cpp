// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "ChildFrm.h"

#include "ViewDI.h"
#include "ViewPTComm.h"
#include "ViewPTRun.h"
#include "ViewOSC.h"
#include "ViewStaInfo.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(WM_STTP_20132, OnSTTP20132)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(STATION_CHANGED, OnStationChanged)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

//##ModelId=49B87BAB0138
CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_bMaximized = FALSE;
	m_nType = -1;
}

//##ModelId=49B87BAB0141
CChildFrame::~CChildFrame()
{
	TRACE("CChildFrame::~CChildFrame");
}

//##ModelId=49B87BAB013B
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//��ֹ��˸
	//�õ���Ļ�ֱ���
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //��   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //�� 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| /*FWS_ADDTOTITLE |*/ WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //�Ѵ�����ʽ����Ϊ��󻯣����Ȳ���ʾ
	
	cs.cx = nSx;   //�Ѵ��ڴ�С����Ϊ������Ļ��С
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //�������ֻ֮������ʾ����
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
//##ModelId=49B87BAB0143
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

//##ModelId=49B87BAB0148
void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

//##ModelId=49B87BAB013E
void CChildFrame::ActivateFrame(int nCmdShow) 
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

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87BAB014B
void CChildFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��,������Ϣ
	CView * pView = GetActiveView();
	if(pView != NULL)
		pView->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20002()
 ���ܸ�Ҫ����Ӧ����ͨѶ״̬����֪ͨ,ת������ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAB014F
void CChildFrame::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//�ж��Ƿ�ͨѶ״̬��ͼ
	if(pView->IsKindOf(RUNTIME_CLASS(CViewPTComm)))
	{
		pView->SendMessage(WM_STTP_20002, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20010()
 ���ܸ�Ҫ����Ӧ����������ֵ����֪ͨ, ת������ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAB0153
void CChildFrame::OnSTTP20010( WPARAM wParam , LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//�ж��Ƿ񿪹����¼���ͼ
	if(pView->IsKindOf(RUNTIME_CLASS(CViewDI)))
	{
		pView->SendMessage(WM_STTP_20010, wParam, lParam);
	}
}
/*************************************************************
 �� �� ����OnSTTP20010()
 ���ܸ�Ҫ����Ӧ����������ֵ����֪ͨ, ת������ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAB0153
void CChildFrame::OnSTTP20144( WPARAM wParam , LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//�ж��Ƿ񿪹����¼���ͼ
	if(pView->IsKindOf(RUNTIME_CLASS(CViewStaInfo)))
	{
		pView->SendMessage(WM_STTP_20144, wParam, lParam);
	}
}
/*************************************************************
 �� �� ����OnSTTP20079()
 ���ܸ�Ҫ����Ӧ������������֪ͨ, ת������ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����	
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAB0158
void CChildFrame::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//�ж��Ƿ�����״̬�¼���ͼ
	if(pView->IsKindOf(RUNTIME_CLASS(CViewPTRun)))
	{
		pView->SendMessage(WM_STTP_20079, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20132()
 ���ܸ�Ҫ����Ӧ¼���ļ�����֪ͨ, ת������ͼ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87BAB015C
void CChildFrame::OnSTTP20132( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	//�ж��Ƿ�¼���¼���ͼ
	if(pView->IsKindOf(RUNTIME_CLASS(CViewOSC)))
	{
		pView->SendMessage(WM_STTP_20132, wParam, lParam);
	}
}

//##ModelId=49B87BAB0160
void CChildFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_ALL_CLOSE, wParam, lParam);
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ת��ҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CChildFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(VIEW_REFRESH, wParam, lParam);
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_capp.Install(this, WM_PAINTMYCAPTION);
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CChildFrame::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	//ȡ�û��ͼָ��
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATIONINIT, wParam, lParam);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   OnSTTP20168	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CChildFrame::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_STTP_20168, wParam, lParam);
}

void CChildFrame::OnStationChanged( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(STATION_CHANGED, wParam, lParam);
}

void CChildFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_EVENTPT_ACK, wParam, lParam);
}

void CChildFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	CView * pView = GetActiveView();
	if(pView == NULL)
		return;
	pView->SendMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

