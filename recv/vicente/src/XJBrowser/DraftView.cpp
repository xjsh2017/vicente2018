// DraftView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DraftView.h"
#include "DraftDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDraftView

IMPLEMENT_DYNCREATE(CDraftView, CScrollView)

CDraftView::CDraftView()
{
	m_fRate = 1.0f;
}

CDraftView::~CDraftView()
{
}


BEGIN_MESSAGE_MAP(CDraftView, CScrollView)
	//{{AFX_MSG_MAP(CDraftView)
	ON_COMMAND(ID_DRAFT_ZOOMOUT, OnDraftZoomout)
	ON_UPDATE_COMMAND_UI(ID_DRAFT_ZOOMOUT, OnUpdateDraftZoomout)
	ON_COMMAND(ID_DRAFT_ZOOMIN, OnDraftZoomin)
	ON_UPDATE_COMMAND_UI(ID_DRAFT_ZOOMIN, OnUpdateDraftZoomin)
	ON_COMMAND(ID_DRAFT_WINDOW, OnDraftWindow)
	ON_UPDATE_COMMAND_UI(ID_DRAFT_WINDOW, OnUpdateDraftWindow)
	ON_COMMAND(ID_DRAFT_REAL, OnDraftReal)
	ON_UPDATE_COMMAND_UI(ID_DRAFT_REAL, OnUpdateDraftReal)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftView drawing

void CDraftView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CDraftDocument* pDoc = (CDraftDocument*)GetDocument();
	
	pDoc->SetTitle( StringFromID(IDS_DRAFT_SUBSTATION));
	GetParentFrame()->SetTitle(pDoc->m_sStationName);

	OpenSelectedDraft();
	
}

void CDraftView::OnDraw(CDC* pDC)
{
// 	CDC memDC;
// 	memDC.CreateCompatibleDC(pDC);
// 	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);
// 	pDC->BitBlt(0, 0, m_nImageWidth, m_nImageHeight, &memDC, 0, 0, SRCCOPY);
// 	memDC.DeleteDC(); 

	CRect r;
	//GetClientRect(&r);
	pDC->GetClipBox(&r);
	
	CRect rect = r;
	pDC->LPtoDP(rect);
	
	CDC dc;
	CBitmap bitmap;
	CDC* pDrawDC = pDC;
	CBitmap* pOldBitmap = NULL;
	
	if(dc.CreateCompatibleDC(pDC))
	{
		if(bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
		{
			OnPrepareDC(&dc, NULL);
			pDrawDC = &dc;
			
			dc.OffsetViewportOrg(-rect.left, -rect.top);
			pOldBitmap = dc.SelectObject(&bitmap);
		}
	}
	
	pDrawDC ->FillSolidRect(rect, RGB(255, 255, 255));
	
	
	//Draw the bitmap
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap1 = memDC.SelectObject(&m_Bitmap);
	//pDrawDC->BitBlt(0, 0,  m_nImageWidth, m_nImageHeight, &memDC, 0, 0, SRCCOPY);	
	pDrawDC->StretchBlt(0, 0, m_nImageWidth*m_fRate, m_nImageHeight*m_fRate, &memDC, 0, 0, m_nImageWidth, m_nImageHeight, SRCCOPY);
	memDC.SelectObject(pOldBitmap1);
	memDC.DeleteDC(); 
	
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0,0);
	pDC->SetMapMode(MM_TEXT);
	dc.SetViewportOrg(0, 0);
	dc.SetWindowOrg(0,0);
	dc.SetMapMode(MM_TEXT);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBitmap);
	dc.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CDraftView diagnostics

#ifdef _DEBUG
void CDraftView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDraftView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDraftView message handlers

void CDraftView::OnDraftZoomout() 
{
	// TODO: Add your command handler code here
	m_fRate += 0.2f;

	CSize sizeTotal;
	sizeTotal.cx = m_nImageWidth * m_fRate;
	sizeTotal.cy = m_nImageHeight * m_fRate;
	SetScrollSizes(MM_TEXT, sizeTotal);

	Invalidate();
}

void CDraftView::OnUpdateDraftZoomout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CDraftView::OnDraftZoomin() 
{
	// TODO: Add your command handler code here
	m_fRate -= 0.1f;
	if( m_fRate < 0.1f )
		m_fRate = 0.1f;

	CSize sizeTotal;
	sizeTotal.cx = m_nImageWidth * m_fRate;
	sizeTotal.cy = m_nImageHeight * m_fRate;
	SetScrollSizes(MM_TEXT, sizeTotal);

	Invalidate();
}

void CDraftView::OnUpdateDraftZoomin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CDraftView::OnDraftWindow() 
{
	// TODO: Add your command handler code here
	DoFitWindow();
	Invalidate();
}

void CDraftView::OnUpdateDraftWindow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CDraftView::OnDraftReal() 
{
	// TODO: Add your command handler code here
	m_fRate = 1.0f;

	CSize sizeTotal = 100;
	sizeTotal.cx = m_nImageWidth;
	sizeTotal.cy = m_nImageHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);

	Invalidate();
}

void CDraftView::OnUpdateDraftReal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CDraftView::DoFitWindow()
{
	CRect rcClient;
	GetClientRect( &rcClient );
	float rateX = (float)rcClient.Width() / (float)m_nImageWidth;
	float rateY = (float)rcClient.Height() / (float)m_nImageHeight;

	m_fRate = rateX < rateY ? rateX : rateY;

	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDraftView::OpenSelectedDraft()
{
	CDraftDocument* pDoc = (CDraftDocument*)GetDocument();
	
	HANDLE hBitmap = LoadImage(NULL, pDoc->GetSelectedDraftPath(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(hBitmap != NULL)
	{
		m_Bitmap.Detach();
		m_Bitmap.Attach(hBitmap);
		BITMAP bm;    
		m_Bitmap.GetObject(sizeof(bm), &bm);
		m_nImageWidth = bm.bmWidth;    
		m_nImageHeight = bm.bmHeight;
		
		CSize sizeTotal;
		sizeTotal.cx = m_nImageWidth;
		sizeTotal.cy = m_nImageHeight;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_DRAFT_OPEN_FAIL));
		AfxMessageBox(pDoc->GetSelectedDraftPath());
	}
}

void CDraftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( lHint == 1010 )
	{
		OpenSelectedDraft();
		Invalidate();
	}
}

BOOL CDraftView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if( zDelta > 0 )
	{
		m_fRate -= 0.05f;
		if( m_fRate < 0.1f )
			m_fRate = 0.1f;
		
		CSize sizeTotal;
		sizeTotal.cx = m_nImageWidth * m_fRate;
		sizeTotal.cy = m_nImageHeight * m_fRate;
		SetScrollSizes(MM_TEXT, sizeTotal);
		
		Invalidate();
	}
	else if( zDelta < 0 )
	{
		m_fRate += 0.1f;
		
		CSize sizeTotal;
		sizeTotal.cx = m_nImageWidth * m_fRate;
		sizeTotal.cy = m_nImageHeight * m_fRate;
		SetScrollSizes(MM_TEXT, sizeTotal);
		
		Invalidate();
	}
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
