// DraftFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DraftFrame.h"
#include "DraftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDraftFrame

IMPLEMENT_DYNCREATE(CDraftFrame, CMiniFrameWnd)

CDraftFrame::CDraftFrame()
{
}

CDraftFrame::~CDraftFrame()
{
}


BEGIN_MESSAGE_MAP(CDraftFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CDraftFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftFrame message handlers

BOOL CDraftFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(150, 0), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
	
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CDraftView), CSize(0, 0), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	
	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));

	this->CenterWindow();
	
	return TRUE;
}

int CDraftFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DRAFT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	m_wndToolBar.LoadTrueColorToolBar( 32, IDB_TOOLBAR_ZOOM_N, IDB_TOOLBAR_ZOOM_N, IDB_TOOLBAR_ZOOM_G);
	

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	return 0;
}

BOOL CDraftFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	cs.cx = g_nDraftWndWidth;
	cs.cy = g_nDraftWndHeight;
	CString name = StringFromID(IDS_DRAFT_SUBSTATION);
	cs.lpszName = name.GetBuffer(1);
	cs.hwndParent = AfxGetApp()->m_pMainWnd->m_hWnd;

	return CMiniFrameWnd::PreCreateWindow(cs);
}
