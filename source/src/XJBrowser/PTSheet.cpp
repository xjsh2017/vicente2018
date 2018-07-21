// PTSheet.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSheet.h"
#include "PTGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTSheet

IMPLEMENT_DYNCREATE(CPTSheet, CTabView)

//##ModelId=49B87B8B009D
CPTSheet::CPTSheet()
{
	m_bPoor = FALSE;
	m_nType = 0;
}

//##ModelId=49B87B8B00B1
CPTSheet::~CPTSheet()
{

}


BEGIN_MESSAGE_MAP(CPTSheet, CTabView)
	//{{AFX_MSG_MAP(CPTSheet)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSheet drawing

//##ModelId=49B87B8B00A0
void CPTSheet::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
//	CRect rc;
//	GetClientRect(&rc);
//	pDC ->FillSolidRect(rc, RGB(236, 233, 216));
}

/////////////////////////////////////////////////////////////////////////////
// CPTSheet diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8B00B3
void CPTSheet::AssertValid() const
{
	CTabView::AssertValid();
}

//##ModelId=49B87B8B00B5
void CPTSheet::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSheet message handlers
//##ModelId=49B87B8B00A3
void CPTSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LPDRAWITEMSTRUCT lp = lpDrawItemStruct;
	if(lp->CtlType == ODT_TAB)
	{
		CBrush brush, selectBrush, *poldbrush;
		CPen* pOldPen,whitepen,backclrPen,blackPen;
		CDC dc;

		
		brush.CreateSolidBrush(RGB(208,216,232));
	//	brush.CreateSolidBrush(RGB(236,233,216));
	//	selectBrush.CreateSolidBrush(RGB(192, 221, 214));
		selectBrush.CreateSolidBrush(RGB(255, 0, 0));
		backclrPen.CreatePen(PS_SOLID,1,RGB(208,216,232));
		whitepen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		blackPen.CreatePen(PS_SOLID,1,RGB(0,0,0));

		CFont font;
		VERIFY(font.CreateFont(
			12,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,					// nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					StringFromID(IDS_DEFAULT_FONT)));                 // lpszFacename
		
		dc.Attach(lp->hDC);

		dc.SetBkMode(TRANSPARENT);

		CTabCtrl& pTab = GetTabCtrl();
		if(pTab.GetSafeHwnd() == NULL)
			return;
		CRect rect,rectMain,rectItem;
		pTab.GetClientRect(rect);
		
		//±³¾°»­Ë¢
		poldbrush = dc.SelectObject(&brush);
		//±³¾°»­±Ê
		pOldPen = dc.SelectObject(&backclrPen);
		//Ä¬ÈÏ×ÖÌå
		CFont* pFont = (CFont*)dc.SelectObject(&font);
	
		int nSel = pTab.GetCurSel();
		
		//»­¸÷item
		for(int i=0;i<pTab.GetItemCount();i++)
		{
			dc.SelectObject(pFont);

			pTab.GetItemRect(i,rectItem);
			if(i != nSel)
			{
				//dc.SetBkColor(RGB(245, 245, 241));
				dc.SelectObject(pOldPen);
				dc.SetTextColor(RGB(60,181,210));
			//	dc.SelectObject(pFont);
			}
			else
			{
				poldbrush = dc.SelectObject(&selectBrush);
				//dc.SetBkColor(RGB(252, 252, 254));
				dc.SelectObject(pOldPen);
			//	dc.SetTextColor(RGB(231,219,5));
				dc.SetTextColor(RGB(255,0,128));
				dc.SelectObject(&font);
			}
			TCITEM tc;
			char temp[256];
			for(int j = 0; j < 256; j++)
			{
				temp[j] = '\0';
			}
			tc.mask = TCIF_TEXT;
			tc.pszText = temp;
			tc.cchTextMax = 256;
			pTab.GetItem(i, &tc);
			dc.DrawText(tc.pszText,rectItem,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

			dc.SelectObject(pFont);
		}

		dc.Detach();
		dc.DeleteDC();

		whitepen.DeleteObject();
		blackPen.DeleteObject();
		backclrPen.DeleteObject();
		brush.DeleteObject();
		selectBrush.DeleteObject();
		font.DeleteObject();
	}
	
}

//##ModelId=49B87B8B00B8
int CPTSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	CTabCtrl & tab = GetTabCtrl();
	tab.ModifyStyle(0, /*TCS_VERTICAL |*/ TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH);

	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //¿í
	if(nSx < 1000)
	{
		m_bPoor = TRUE;
	}
	
	return 0;
}

//##ModelId=49B87B8B00BD
void CPTSheet::OnSize(UINT nType, int cx, int cy) 
{
//	CTabView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);

	CSize sz;
	sz.cx = 75;
	sz.cy = 22;
	
	CTabCtrl & tab = GetTabCtrl();
	int nCount = tab.GetItemCount();
	tab.SetItemSize(sz);
	
	CTabView::OnSize(nType, cx, cy);
}

//##ModelId=49B87B8B00AC
void CPTSheet::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CTabView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B8B00C2
BOOL CPTSheet::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return TRUE;
	return CTabView::OnEraseBkgnd(pDC);
}
