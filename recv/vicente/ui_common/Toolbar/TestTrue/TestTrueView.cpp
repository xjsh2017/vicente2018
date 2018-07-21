// TestTrueView.cpp : implementation of the CTestTrueView class
//

#include "stdafx.h"
#include "TestTrue.h"

#include "TestTrueDoc.h"
#include "TestTrueView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView

IMPLEMENT_DYNCREATE(CTestTrueView, CView)

BEGIN_MESSAGE_MAP(CTestTrueView, CView)
	//{{AFX_MSG_MAP(CTestTrueView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView construction/destruction

CTestTrueView::CTestTrueView()
{
	// TODO: add construction code here

}

CTestTrueView::~CTestTrueView()
{
}

BOOL CTestTrueView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView drawing

void CTestTrueView::OnDraw(CDC* pDC)
{
	CTestTrueDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView printing

BOOL CTestTrueView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestTrueView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestTrueView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView diagnostics

#ifdef _DEBUG
void CTestTrueView::AssertValid() const
{
	CView::AssertValid();
}

void CTestTrueView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestTrueDoc* CTestTrueView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestTrueDoc)));
	return (CTestTrueDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestTrueView message handlers
