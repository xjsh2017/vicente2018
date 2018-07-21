#include "StdAfx.h"
#include "PaintCap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////
// Class used to get the caption rectangle of a window in window coords.
// This is the area of the title bar inside the window frame, including
// the icon and min/max/close buttons.
// 
class CCaptionRect : public CRect {
public:
	CCaptionRect(const CWnd& wnd); // use reference to deny NULL ptr
};

IMPLEMENT_DYNAMIC(CCaptionPainter, CSubclassWnd);

CCaptionPainter::CCaptionPainter()
{
	Invalidate();
}

CCaptionPainter::~CCaptionPainter()
{
}

//////////////////
// Install caption handler. nPaintMsg is message I will send too frame
// when its caption needs painting.
//
BOOL CCaptionPainter::Install(CFrameWnd* pFrameWnd,UINT nPaintMsg,BOOL bMod)
{
	ASSERT_KINDOF(CFrameWnd, pFrameWnd);
	m_nPaintMsg = nPaintMsg;
	m_bModified=bMod;
	return HookWindow(pFrameWnd);
}

//////////////////
// Message handler handles caption-related messages
//
LRESULT CCaptionPainter::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{

	if (g_hook)
	{
		return CSubclassWnd::WindowProc(msg, wp, lp);
	}

//	CString strq;
	switch (msg) {
	case WM_NCPAINT:
		OnNcPaint(HRGN(wp));
		return 0;
	case WM_NCACTIVATE:
//		strq.Format("%d", msg);
//		WriteLog("msg ---->" + strq);
		return OnNcActivate(wp);
	case WM_SETTEXT:
		OnSetText((LPCTSTR)lp);
		return 0;
	case WM_SYSCOLORCHANGE:
	case WM_SETTINGCHANGE:
		//used for test
		//WriteLog("CCaptionPainter::WM_SETTINGCHANGE");
		Invalidate();
		CWnd::FromHandle(m_hWnd)->SendMessage(m_nPaintMsg, 0, 0L);
//		return 0;
	}
	// I don't handle it: pass along
	/*
	if (msg != 273 && msg != 32)
	{
		CString str;
		str.Format("CCaptionPainter::WindowProc msg:%d", msg);
		WriteLog(str);
	}
	*/
	if (msg == 174)
	{
		return 0;
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}

/////////////////
// Handle WM_NCPAINT for main window
//
void CCaptionPainter::OnNcPaint(HRGN hRgn)
{
	//used for test
	//WriteLog("CCaptionPainter::OnNcPaint");
	ASSERT_VALID(CWnd::FromHandle(m_hWnd));
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	CCaptionRect rc(wnd);		// caption rectangle in window coords
	CRect rcWin;				// window rect
	wnd.GetWindowRect(&rcWin);	// .. get window rect
	rc += rcWin.TopLeft();		// convert caption rect to screen coords

	// Don't bother painting if the caption doesn't lie within the region.
	//
	if ((WORD)hRgn > 1 && !::RectInRegion(hRgn, &rc)) {
		Default();					// just do default thing
		return;						// and quit
	}

	// Exclude caption from update region
	//
	HRGN hRgnCaption = ::CreateRectRgnIndirect(&rc);
	HRGN hRgnNew     = ::CreateRectRgnIndirect(&rc);
	if ((WORD)hRgn > 1) {
		// wParam is a valid region: subtract caption from it
		::CombineRgn(hRgnNew, hRgn, hRgnCaption, RGN_DIFF);
	} else {
		// wParam is not a valid region: create one that's the whole
		// window minus the caption bar
		HRGN hRgnAll = ::CreateRectRgnIndirect(&rcWin);
		CombineRgn(hRgnNew, hRgnAll, hRgnCaption, RGN_DIFF);
		DeleteObject(hRgnAll);
	}

	// Call Windows to do WM_NCPAINT with altered update region
	//
	MSG& msg = AfxGetThreadState()->m_lastSentMsg;
	WPARAM savewp = msg.wParam;	// save original wParam
	msg.wParam = (WPARAM)hRgnNew;	// set new region for DefWindowProc
	Default();
	DeleteObject(hRgnCaption);		// clean up
	DeleteObject(hRgnNew);			// ...
	msg.wParam = savewp;				// restore original wParam

	PaintCaption();					// Now paint my special caption
}

//////////////////
// Handle WM_NCACTIVATE for main window
//
BOOL CCaptionPainter::OnNcActivate(BOOL bActive)
{
	//used for test
	//WriteLog("CCaptionPainter::OnNcActivate");
	ASSERT_VALID(CWnd::FromHandle(m_hWnd));
	CFrameWnd& frame = *((CFrameWnd*)CWnd::FromHandle(m_hWnd));
	ASSERT_KINDOF(CFrameWnd, &frame);

	// Mimic MFC kludge to stay active if WF_STAYACTIVE bit is on
	if (frame.m_nFlags & WF_STAYACTIVE)
		bActive = TRUE;
	if (!frame.IsWindowEnabled())			// but not if disabled
		bActive = FALSE;
	if (bActive==m_bActive)
		return TRUE;					// nothing to do

	/*
	if (!frame.IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		return FALSE;
	}
	*/

	// In case this is a MDI app, manually activate/paint active MDI child
	// window, because Windows won't do it if parent frame is invisible.
	// Must do this BEFORE calling Default, or it will not work.
	//
	CFrameWnd* pActiveFrame = frame.GetActiveFrame();
	if (pActiveFrame!=&frame) {
		pActiveFrame->SendMessage(WM_NCACTIVATE,bActive);
		pActiveFrame->SendMessage(WM_NCPAINT);
	}

	// Turn WS_VISIBLE off before calling DefWindowProc,
	// so DefWindowProc won't paint and thereby cause flicker.
	//
	DWORD dwStyle = frame.GetStyle();
	if (dwStyle & WS_VISIBLE)
		::SetWindowLong(frame, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));

	MSG& msg = AfxGetThreadState()->m_lastSentMsg;
	msg.wParam = bActive;
	Default();
	if (dwStyle & WS_VISIBLE)
		::SetWindowLong(frame, GWL_STYLE, dwStyle);

	// At this point, nothing has happened (since WS_VISIBLE was off).
	// Now it's time to paint.
	//
	m_bActive = bActive;					// update state
	frame.SendMessage(WM_NCPAINT);	// paint non-client area (frame too)
	return TRUE;							// done OK
}

//////////////////
// Handle WM_SETTEXT for main window
//
void CCaptionPainter::OnSetText(LPCTSTR lpText)
{
	//used for test
	//WriteLog("CCaptionPainter::OnSetText");
	ASSERT_VALID(CWnd::FromHandle(m_hWnd));
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	// Turn WS_VISIBLE style off before calling Windows to
	// set the text, then turn it back on again after.
	//
	DWORD dwStyle = wnd.GetStyle();
	if (dwStyle & WS_VISIBLE)
		SetWindowLong(wnd.m_hWnd, GWL_STYLE, dwStyle & ~ WS_VISIBLE);
	Default();
	if (dwStyle & WS_VISIBLE)
		SetWindowLong(wnd.m_hWnd, GWL_STYLE, dwStyle);

	wnd.SendMessage(WM_NCPAINT);		// paint non-client (frame)
	Invalidate();							// force new bitmap
	PaintCaption();
}

//////////////////
// Paint custom caption. Flag tells whether active or not. Just blast the
// bitmap to the title bar, but not if minimized (iconic).
//
void CCaptionPainter::PaintCaption()
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);
	//uesd for test
	//WriteLog("start PaintCaption");
	if (wnd.IsIconic())
		return;

	// Get caption DC and rectangle
	CWindowDC dcWin(&wnd);					// window DC
	CDC dc;										// memory DC
	dc.CreateCompatibleDC(&dcWin);		// ...create it
	CCaptionRect rc(wnd);					// get caption rectangle
	if (rc.Size() != m_szCaption) {		// if size changed:
		m_bmCaption[0].DeleteObject();	// invalidate bitmaps
		m_bmCaption[1].DeleteObject();	// ...
		m_szCaption = rc.Size();			// update new size
	}

	// Get active/inactive bitmap & determine if needs to be regenerated
	CBitmap& bm = m_bmCaption[m_bActive!=0];	// get bitmap
	BOOL bPaintIt = FALSE;							// paint anew?
	if (!(HBITMAP)bm) {											// no bitmap:
		bm.CreateCompatibleBitmap(&dcWin, rc.Width(), rc.Height()); // create one
		bPaintIt = TRUE;								// and paint it
	}

	CBitmap* pOldBitmap = dc.SelectObject(&bm);	// select bitmap into memory DC

	// If bitmap needs painting, call client to do it.

	if (bPaintIt) {
		PAINTCAP pc;
		pc.m_pDC = &dc;
		pc.m_szCaption = rc.Size();
		pc.m_bActive=m_bActive;
		if(m_nPaintMsg)
		{
			wnd.SendMessage(m_nPaintMsg, m_bActive, (LPARAM)&pc);
		}
		else
		{
			DrawNormalCaption(pc);
		}
	}

	// blast bits to screen
	//used for test
	//CString str;
	//str.Format("left: %d   top   %d     width     %d     height     %d", rc.left, rc.top, rc.Width(), rc.Height());
	//WriteLog(str);
	dcWin.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dc,0,0,SRCCOPY);
	dc.SelectObject(pOldBitmap); // restore DC
}

void CCaptionPainter::DrawNormalCaption(const PAINTCAP& pc)
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);
  UINT uFlags = DC_TEXT|DC_ICON;
  if(pc.m_bActive)uFlags|=DC_ACTIVE;
  CRect rc(CPoint(0,0),pc.m_szCaption);
  ::DrawCaption(wnd,*pc.m_pDC,&rc,uFlags);
  DrawButtons(pc);
}
////////////////
// Draw caption icon. Returns width of icon.
//
int CCaptionPainter::DrawIcon(const PAINTCAP& pc)
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	// Within the basic button rectangle, Windows 95 uses a 1 or 2 pixel border
	// Icon has 2 pixel border on left, 1 pixel on top/bottom, 0 right
	//
	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	CRect rc(0, 0, cxIcon, GetSystemMetrics(SM_CYSIZE));
	rc.DeflateRect(0,1);
	rc.left += 2;
	DrawIconEx(pc.m_pDC->m_hDC, rc.left, rc.top,
		(HICON)GetClassLong(wnd.m_hWnd, GCL_HICONSM),
		rc.Width(), rc.Height(), 0, NULL, DI_NORMAL);
	return cxIcon;
}

////////////////
// Draw min, max/restore, close buttons.
// Returns total width of buttons drawn.
//
int CCaptionPainter::DrawButtons(const PAINTCAP& pc)
{
	
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);
	DWORD dwStyle = wnd.GetStyle();
	if (!(dwStyle & WS_CAPTION))
		return 0;

	ASSERT(pc.m_pDC);
	CDC& dc = *pc.m_pDC;

	int cxIcon = 20 /*GetSystemMetrics(SM_CXSIZE)*/;
	int cyIcon = 21 /*GetSystemMetrics(SM_CYSIZE)*/;

	// Draw caption buttons. These are all drawn inside a rectangle
	// of dimensions SM_CXSIZE by SM_CYSIZE
	CRect rc(0, 0, cxIcon, cyIcon);
	rc += CPoint(pc.m_szCaption.cx-cxIcon, 0);	// move right

	// Close box has a 2 pixel border on all sides but left, which is zero
	rc.DeflateRect(2,2);
	//rc.left -= 2;
	dc.DrawFrameControl(&rc, DFC_CAPTION, DFCS_CAPTIONCLOSE);

	// Max/restore button is like close box; just shift rectangle left
	// Also does help button, if any.
	BOOL bMaxBox = dwStyle & WS_MAXIMIZEBOX;
	if (bMaxBox || (wnd.GetExStyle() & WS_EX_CONTEXTHELP)) {
		rc -= CPoint(cxIcon - 2, 0);
		dc.DrawFrameControl(&rc, DFC_CAPTION,
			bMaxBox ? (wnd.IsZoomed() ? DFCS_CAPTIONRESTORE : DFCS_CAPTIONMAX) :
				DFCS_CAPTIONHELP);
	}

	// Minimize button has 2 pixel border on all sides but right.
	if (dwStyle & WS_MINIMIZEBOX) {
		rc -= CPoint(cxIcon - 2, 0);
		dc.DrawFrameControl(&rc, DFC_CAPTION, DFCS_CAPTIONMIN);
	}
	return pc.m_szCaption.cx - rc.left - 2;

	//return 0;
}

//////////////////
// CCaptionRect Constructor computes caption rectangle in window coords.
//
CCaptionRect::CCaptionRect(const CWnd& wnd)
{
	// Get size of frame around window
	DWORD dwStyle = wnd.GetStyle();
	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
		CSize(GetSystemMetrics(SM_CXSIZEFRAME),
			   GetSystemMetrics(SM_CYSIZEFRAME)) :
		CSize(GetSystemMetrics(SM_CXFIXEDFRAME),
				GetSystemMetrics(SM_CYFIXEDFRAME));

	int cxIcon = GetSystemMetrics(SM_CXSIZE); // width of caption icon/button

	// Compute rectangle
	wnd.GetWindowRect(this);		// window rect in screen coords
	*this -= CPoint(left, top);	// shift origin to (0,0)
	left  += szFrame.cx;				// frame
	right -= szFrame.cx;				// frame
	top   += szFrame.cy;				// top = end of frame
	bottom = top + GetSystemMetrics(SM_CYCAPTION)  // height of caption
		- GetSystemMetrics(SM_CYBORDER);				  // minus gray shadow border
}

//////////////////
// Helper function to compute the luminosity for an RGB color.
// Measures how bright the color is. I use this so I can draw the caption
// text using the user's chosen color, unless it's too dark. See MSDN for
// definition of luminosity and how to compute it.
//
int CCaptionPainter::GetLuminosity(COLORREF color)
{
#define HLSMAX 240	// This is what Display Properties uses
#define RGBMAX 255	// max r/g/b value is 255
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	int rgbMax = max( max(r,g), b);
	int rgbMin = min( min(r,g), b);
	return (((rgbMax+rgbMin) * HLSMAX) + RGBMAX ) / (2*RGBMAX);
}

#define COLOR_WHITE RGB(255,255,255)
#define COLOR_BLACK RGB(0,0,0)
#define NCOLORSHADES 64		// this many shades in gradient

//////////////////
// Helper to paint rectangle with a color.
//
void CCaptionPainter::PaintRect(CDC& dc, int x, int y, int w, int h, COLORREF color)
{
	CBrush brush(color);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.PatBlt(x, y, w, h, PATCOPY);
	dc.SelectObject(pOldBrush);
}

//////////////////
// Paint custom caption.
// This is the function that actually does the shading. It creates a
// bitmap that's used to paint the caption. It looks horrible, but it's
// just a lot of bit-twiddling GDI stuff.
//
void CCaptionPainter::PaintMyCaption(WPARAM bActive, LPARAM lParam, CString m_strTitle)
{
	if (lParam == 0) {
		// lParam = 0 means system setting change: invalidate fonts.
		m_fontCaption.DeleteObject();
		return;
	}

	const PAINTCAP& pc = *((PAINTCAP*)lParam);
	ASSERT(pc.m_pDC);
	CDC& dc = *pc.m_pDC;

	int cxCap = pc.m_szCaption.cx;
	int cyCap = pc.m_szCaption.cy;

	COLORREF clrFrom;	// the beginning color
	COLORREF clrTo;		// the ending color

	// These are the default colors in Windows(R) 98
	// Modify them to suit your needs.

	if (bActive) {
		// Active caption
	//	clrFrom	= GetSysColor(COLOR_ACTIVECAPTION);
		if(g_style == 3)
		{
			clrFrom = g_TitleLightColor;
			clrTo	= g_TitleDeepColor;
		}
		else if (g_style == 2)
		{
			clrFrom = g_ListSpaceColor3;
			clrTo	= g_ListSpaceColor3;
		}
		else
		{
			clrFrom = RGB(212, 233, 250);
			clrTo	= RGB(16, 132, 208);
		}
	}
	else
	{
		// Inactive caption
		clrFrom	= GetSysColor(COLOR_INACTIVECAPTION);
		clrTo	= RGB(184, 180, 184);
	}

	// Get the intensity values for the ending color
	int r1 = GetRValue(clrTo); // red
	int g1 = GetGValue(clrTo); // green
	int b1 = GetBValue(clrTo); // blue
	
	// Get the intensity values for the begining color
	int r2 = GetRValue(clrFrom); // red
	int g2 = GetGValue(clrFrom); // green
	int b2 = GetBValue(clrFrom); // blue

	int x = 5*cxCap/6;					// start 5/6 of the way right
	int w = x;							// width of area to shade
	int xDelta= max(w/NCOLORSHADES,1);	// width of one shade band

	// Paint far right 1/6 of caption the background color
	PaintRect(dc, x, 0, cxCap-x, cyCap, clrTo);

	int r, g, b;
	while (x > xDelta) {
		x -= xDelta;
		if (r1 > r2)
			r = r1 - (r1-r2)*(w-x)/w;
		else
			r = r1 + (r2-r1)*(w-x)/w;

		if (g1 > g2)
			g = g1 - (g1-g2)*(w-x)/w;
		else
			g = g1 + (g2-g1)*(w-x)/w;

		if (b1 > b2)
			b = b1 - (b1-b2)*(w-x)/w;
		else
			b = b1 + (b2-b1)*(w-x)/w;

		// Paint bands right to left
		PaintRect(dc, x, 0, xDelta, cyCap, RGB(r, g, b));
	}

	// Paint what's left of the caption with the beginning color
	PaintRect(dc, 0, 0, x, cyCap, clrFrom);

	// Use caption painter to draw icon and buttons
	int cxIcon  = DrawIcon(pc);
	int cxButns = DrawButtons(pc);

	// Now draw text. First Create fonts if needed
	if (!m_fontCaption.m_hObject)
		CreateFonts();

	CString dt=GetDocTitle();
	CString s;

	// app title
	if(dt.IsEmpty()) s = " " + m_strTitle;
	else s = " " + m_strTitle + "  -  [" + dt + "]";

	CRect rc(CPoint(0,0), pc.m_szCaption); // text rectangle
	rc.left  += cxIcon+2;		// start after icon
	rc.right -= cxButns;		// don't draw past buttons
	dc.SetBkMode(TRANSPARENT);	// draw on top of our shading

	// This is a trial and error value that sets the point
	// where the caption background is too light/dark in
	// order to display the text with white/black.
	#define	LUMINOSITY_MARK 120

	if (GetLuminosity(clrFrom) > LUMINOSITY_MARK)
		dc.SetTextColor(COLOR_BLACK);
	else
		dc.SetTextColor(COLOR_WHITE);

	CFont* pOldFont = dc.SelectObject(&m_fontCaption);
	dc.DrawText(s, &rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	// Restore DC
	dc.SelectObject(pOldFont);
}

//////////////////
// Helper function to build the fonts I need.
//
void CCaptionPainter::CreateFonts()
{
	// Get current system caption font, just to get its size
	//
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0));
	m_fontCaption.CreateFontIndirect(&ncm.lfCaptionFont);
}

// Private MFC function only sets the title if it's different
//
extern void AFXAPI AfxSetWindowText(HWND, LPCTSTR);

void CCaptionPainter::UpdateFrameTitle(HWND m_hWnd, CString m_strTitle)
{
  CString s = (m_strTitle + " " + GetDocTitle());
	AfxSetWindowText(m_hWnd,(LPCTSTR)s);
}

//////////////////
// Get doc title: I use full path name or "untitled"
//
CString  CCaptionPainter::GetDocTitle()
{
	static CString s;
  s.Empty();
	CFrameWnd *mFrame = (CFrameWnd *)AfxGetMainWnd();
  if(mFrame){
  	CFrameWnd *pFrame = mFrame->GetActiveFrame();
    if(pFrame!=mFrame && pFrame){
      pFrame->GetWindowText(s);
      if(!s.IsEmpty()){
        if(m_bModified){
        	CDocument *pDoc = pFrame->GetActiveDocument();
          if(pDoc){
            if(pDoc->IsModified()&&s.GetAt(s.GetLength()-1)!='*')s+="*";
          }
        }
      }
    }
  }
  return s;
}