// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TestTrue.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static TCHAR *TextTips[]={"四个字的","工具栏才","比较好看","",
							"剪切","","粘贴","",
							"打印"};
static TCHAR *Text1[]={"对话框", "upload", "view", "paste"};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_1, OnUpdate1)
	ON_UPDATE_COMMAND_UI(ID_2, OnUpdate2)
	ON_UPDATE_COMMAND_UI(ID_3, OnUpdate3)
	ON_UPDATE_COMMAND_UI(ID_4, OnUpdate4)
	ON_COMMAND(ID_1, On1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.LoadTrueColorToolBar( 32, IDB_BITMAP1, IDB_BITMAP1, IDB_BITMAP2);

		if (!m_wndToolBar1.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar1.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar1.LoadTrueColorToolBar( 32, IDB_BITMAP3, IDB_BITMAP4, IDB_BITMAP3);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndToolBar1);
	//增加文字
//	UpdateToolBar(&m_wndToolBar, TextTips);
	UpdateToolBar(&m_wndToolBar1, Text1);

	return 0;
}

void CMainFrame::UpdateToolBar(CToolBar* tb, TCHAR* text[])
{
	CRect rect;
	CSize sizeButton,sizeImage;
	CSize m_sizeMax;
	m_sizeMax.cx = 0;
	m_sizeMax.cy = 0;
	//取得最大尺寸
	for (int i=0; i<tb->GetCount();i++)
	{
		tb->SetButtonText(i,text[i]);
		tb->GetItemRect(i,rect);
		m_sizeMax.cx = __max(rect.Size().cx, m_sizeMax.cx);
		m_sizeMax.cy = __max(rect.Size().cy, m_sizeMax.cy);
	}
	//设置按钮尺寸
	sizeButton.cx = m_sizeMax.cx;
	sizeButton.cy = m_sizeMax.cy;
	sizeImage.cx = 32;
	sizeImage.cy = 32;
	tb->SetSizes(sizeButton,sizeImage);
	ShowControlBar(tb,FALSE,FALSE);
	ShowControlBar(tb,TRUE,FALSE);
	tb->RedrawWindow();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdate1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdate2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdate3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdate4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CMainFrame::On1() 
{
	// TODO: Add your command handler code here
//	CDlgTest dlg;
//	dlg.DoModal();
}
