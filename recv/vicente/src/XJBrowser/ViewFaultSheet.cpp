// ViewFaultSheet.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewFaultSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewFaultSheet

IMPLEMENT_DYNCREATE(CViewFaultSheet, CTabView)

//##ModelId=49B87B8301D5
CViewFaultSheet::CViewFaultSheet()
{
}

//##ModelId=49B87B8301E6
CViewFaultSheet::~CViewFaultSheet()
{
}


BEGIN_MESSAGE_MAP(CViewFaultSheet, CTabView)
	//{{AFX_MSG_MAP(CViewFaultSheet)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFaultSheet drawing

//##ModelId=49B87B8301D9
void CViewFaultSheet::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

//##ModelId=49B87B8301D6
void CViewFaultSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
				dc.SetTextColor(RGB(0,0,0));
				//	dc.SelectObject(pFont);
			}
			else
			{
				poldbrush = dc.SelectObject(&selectBrush);
				//dc.SetBkColor(RGB(252, 252, 254));
				dc.SelectObject(pOldPen);
				dc.SetTextColor(RGB(0,0,0));
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

/////////////////////////////////////////////////////////////////////////////
// CViewFaultSheet diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8301E8
void CViewFaultSheet::AssertValid() const
{
	CTabView::AssertValid();
}

//##ModelId=49B87B8301EA
void CViewFaultSheet::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewFaultSheet message handlers

//##ModelId=49B87B8301DC
void CViewFaultSheet::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_FAULTREPORT));
	//GetParentFrame()->SetWindowText("¹ÊÕÏ¹éµµ");
	CTabView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B8301ED
void CViewFaultSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabView::OnLButtonDown(nFlags, point);
}

void CViewFaultSheet::OnSize(UINT nType, int cx, int cy) 
{	
	// TODO: Add your message handler code here
	CSize sz;
	sz.cx = 75;
	sz.cy = 22;
	
	CTabCtrl & tab = GetTabCtrl();
	tab.SetItemSize(sz);

	CTabView::OnSize(nType, cx, cy);
}
