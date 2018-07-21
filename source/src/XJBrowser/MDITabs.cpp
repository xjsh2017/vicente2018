/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : CMDITabs implementation
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: ?2001 by Christian Rodemeyer
\****************************************************************************/

#include "stdafx.h"
#include "MDITabs.h"
#include <AFXPRIV.H>
#include <algorithm>
#include <vector>

#include "CJMDIFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD CMDITabs::s_dwCustomLook = 1;

COLORREF Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);
	
	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);
	
	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;
	
	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);
	
	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);
	
	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);
	
	fFactor = max(fFactor, 1.0f);
	
	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;
	
	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);
	
	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);
	
	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}
/////////////////////////////////////////////////////////////////////////////
// CMDITabs

//##ModelId=49B87B970050
CMDITabs::CMDITabs()
{
  m_mdiClient = NULL;
  m_minViews = 0;
  m_bImages = false;
  m_bTop    = false;
  m_crBack = (COLORREF)-1; // use default color
}

BEGIN_MESSAGE_MAP(CMDITabs, CTabCtrl)
  //{{AFX_MSG_MAP(CMDITabs)
  ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
  ON_WM_PAINT()
  ON_WM_NCPAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
  ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDITabs message handlers

//##ModelId=49B87B97009C
afx_msg LRESULT CMDITabs::OnSizeParent(WPARAM, LPARAM lParam)
{
  if (GetItemCount() < m_minViews) 
  {
    ShowWindow(SW_HIDE);
  }
  else 
  {  
    AFX_SIZEPARENTPARAMS* pParams = reinterpret_cast<AFX_SIZEPARENTPARAMS*>(lParam);

    const int height = 28 + (m_bImages ? 1 : 0);
    const int offset = 4;

    m_height = height + offset;
    m_width  = pParams->rect.right - pParams->rect.left;

    if (m_bTop)
    {
      pParams->rect.top += height;
      MoveWindow(pParams->rect.left, pParams->rect.top - height, m_width, m_height, true);
    }
    else
    {
      pParams->rect.bottom -= height;
      MoveWindow(pParams->rect.left, pParams->rect.bottom - offset, m_width, m_height, true);
    }
    ShowWindow(SW_NORMAL);
  }
  return 0;
}

//##ModelId=49B87B97007E
void CMDITabs::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
  TCITEM item;
  item.mask = TCIF_PARAM;
  GetItem(GetCurSel(), &item);
  /*
  CRect rect;
  GetItemRect(GetCurSel(), rect);
  CPaintDC dc(this); // device context for painting
  dc.FillSolidRect(&rect, g_ListSpaceColor);
  */
  ::BringWindowToTop(HWND(item.lParam));
  *pResult = 0;
}

//##ModelId=49B87B970054
void CMDITabs::Update()
{
  SetRedraw(false);

  HWND active = ::GetTopWindow(m_mdiClient); // get active view window (actually the frame of the view)

  typedef std::vector<HWND> TWndVec;
  typedef TWndVec::iterator TWndIter;

  TWndVec vChild; // put all child windows in a list (actually a vector)
  for (HWND child = active; child; child = ::GetNextWindow(child, GW_HWNDNEXT))
  {
    vChild.push_back(child);
  }

  TCITEM item;
  char text[256];
  item.pszText = text;

  for (int i = GetItemCount(); i--;)  // for each tab
  {
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    TWndIter it = std::find(vChild.begin(), vChild.end(), HWND(item.lParam));
    if (it == vChild.end()) // associatete view does no longer exist, so delete the tab
    {
      DeleteItem(i);
      if (m_bImages) RemoveImage(i);
    }
    else // update the tab's text, image and selection state
    {
      item.mask = TCIF_TEXT;
      ::GetWindowText(*it, text, 256);
      if (m_bImages) m_images.Replace(i, (HICON)::GetClassLong(*it, GCL_HICONSM));
      SetItem(i, &item);
      if (*it == active) SetCurSel(i); // associated view is active => make it the current selection
      vChild.erase(it);                // remove view from list
    }
  }

  // all remaining views in vChild have to be added as new tabs
  i = GetItemCount();
  for (TWndIter it = vChild.begin(), end = vChild.end(); it != end; ++it)
  {
	  item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	  ::GetWindowText(*it, text, 256);
	  if (m_bImages) m_images.Add((HICON)::GetClassLong(*it, GCL_HICONSM));
	  item.iImage = i;
	  item.lParam = LPARAM(*it);
	  InsertItem(i, &item);
	  if (*it == active) SetCurSel(i);
	  ++i;
  }

  // this removes the control when there are no tabs and shows it when there is at least one tab
  bool bShow = GetItemCount() >= m_minViews;
  if ((!bShow && IsWindowVisible()) || (bShow && !IsWindowVisible())) 
  {
   // static_cast<CMDIFrameWnd*>(FromHandlePermanent(::GetParent(m_mdiClient)))->RecalcLayout();
	  static_cast<CCJMDIFrameWnd*>(FromHandlePermanent(::GetParent(m_mdiClient)))->RecalcLayout();
  }

  RedrawWindow(NULL, NULL, RDW_FRAME|RDW_INVALIDATE|RDW_ERASE);
  SetRedraw(true);
}

//##ModelId=49B87B970082
void CMDITabs::OnPaint()
{
  CPaintDC dc(this);

  if (GetItemCount() == 0) return; // do nothing

  // cache some system colors
  DWORD shadow  = ::GetSysColor(COLOR_3DSHADOW);
  DWORD dark    = ::GetSysColor(COLOR_3DDKSHADOW);
  DWORD hilight = ::GetSysColor(COLOR_3DHILIGHT);
  DWORD light   = ::GetSysColor(COLOR_3DLIGHT);

  // Special preparations for spin-buttons (in case there are more tabs than fit into the window)
  // extend borders and prevent system from overdrawing our new pixels
  if (m_bTop)
  {
    ::SetPixel(dc, m_width - 5, m_height - 8, hilight);
    ::SetPixel(dc, m_width - 5, m_height - 7, light);
    ::SetPixel(dc, m_width - 6, m_height - 8, hilight);
    ::SetPixel(dc, m_width - 6, m_height - 7, light);
    ::ExcludeClipRect(dc, 0, m_height - 6, m_width, m_height - 2);
    ::ExcludeClipRect(dc, m_width - 6, m_height - 8, m_width - 2, m_height - 6);
  }
  else
  {
    ::SetPixel(dc, m_width - 5, 2, shadow);
    ::SetPixel(dc, m_width - 5, 3, dark);
    ::SetPixel(dc, m_width - 6, 2, shadow);
    ::SetPixel(dc, m_width - 6, 3, dark);
    ::ExcludeClipRect(dc, 0, 0, m_width, 2); 
    ::ExcludeClipRect(dc, m_width - 6, 2, m_width - 2, 4);
  }
  
  // windows should draw the control as usual
  _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
  pThreadState->m_lastSentMsg.wParam = WPARAM(HDC(dc));
  //Default();

  //CPaintDC dc(this); // device context for painting
  
		// prepare dc
  dc.SelectObject(GetFont());
		
		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;
		
		// draw the rest of the border
		CRect rClient, rPage;
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
		dis.rcItem.top = rPage.top - 2;
		
		DrawMainBorder(&dis);
		
		// paint the tabs first and then the borders
		int nTab = GetItemCount();
		int nSel = GetCurSel();
		
		if (!nTab) // no pages added
			return;
		
		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;
				
				VERIFY(GetItemRect(nTab, &dis.rcItem));
				
				dis.rcItem.bottom -= 2;
				DrawItem(&dis);
				DrawItemBorder(&dis);
			}
		}
		
		// now selected tab
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;
		
		VERIFY(GetItemRect(nSel, &dis.rcItem));
		
		dis.rcItem.bottom += 2;
		dis.rcItem.top -= 2;
		DrawItem(&dis);
		DrawItemBorder(&dis);
		


  // extend the horizontal border to the left margin
  if (m_bTop)
  {
    ::SetPixel(dc, 0, m_height - 8, hilight);
    ::SetPixel(dc, 0, m_height - 7, light);
  }
  else
  {
    ::SetPixel(dc, 0, 2, shadow);
  }

  // special drawing if the leftmost tab is selected
  CRect rect;
  GetItemRect(GetCurSel(), rect);
  if (rect.left == 2) // is at the leftmost position a tab selected?
  {
    // if yes, remove the leftmost white line and extend the bottom border of the tab
    int j = m_bImages ? 1 : 0;

    HPEN pen = ::CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
    HGDIOBJ old = ::SelectObject(dc, pen);
      ::MoveToEx(dc, 0, 2, NULL);
      ::LineTo(dc, 0, 22 + j);
      ::MoveToEx(dc, 1, 2, NULL); 
      ::LineTo(dc, 1, 22 + j);    
    ::SelectObject(dc, old);
    ::DeleteObject(pen);

    if (m_bTop)
    {
      ::SetPixel(dc, 0, 0, hilight); ::SetPixel(dc, 1, 0, hilight);
      ::SetPixel(dc, 0, 1, light);   ::SetPixel(dc, 1, 1, light);
    }
    else
    {
      ::SetPixel(dc, 0, 22 + j, shadow); ::SetPixel(dc, 1, 22 + j, shadow);
      ::SetPixel(dc, 0, 23 + j, dark);   ::SetPixel(dc, 1, 23 + j, dark);
    }
  }
}

//##ModelId=49B87B97008C
void CMDITabs::OnNcPaint()
{
  HDC hdc = ::GetWindowDC(m_hWnd);

  CRect rect;
  rect.left = 0;
  rect.top = m_bTop ? 0 : 0;
  rect.right = m_width;
  rect.bottom = m_height;

  HPEN pen = ::CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_3DFACE));
  HGDIOBJ old = ::SelectObject(hdc, pen);
  if (m_bTop)
  {
    DrawEdge(hdc, rect, EDGE_SUNKEN, BF_LEFT|BF_RIGHT|BF_TOP);
    ::MoveToEx(hdc, 2, m_height - 1, NULL);
    ::LineTo(hdc, m_width - 2, m_height - 1);
    ::MoveToEx(hdc, 2, m_height - 2, NULL);
    ::LineTo(hdc, m_width - 2, m_height - 2);
  }
  else
  {
    DrawEdge(hdc, rect, EDGE_SUNKEN, BF_LEFT|BF_RIGHT|BF_BOTTOM);
    ::MoveToEx(hdc, 2, 0, NULL);
    ::LineTo(hdc, m_width - 2, 0);
    ::MoveToEx(hdc, 2, 1, NULL);
    ::LineTo(hdc, m_width - 2, 1);
  }
  ::SelectObject(hdc, old);
  ::DeleteObject(pen);
  ::ReleaseDC(m_hWnd, hdc);

}

//##ModelId=49B87B970051
void CMDITabs::Create(CFrameWnd* pMainFrame, DWORD dwStyle)
{
  m_bTop = (dwStyle & MT_TOP);
  m_minViews = (dwStyle & MT_HIDEWLT2VIEWS) ? 2 : 1;

  CTabCtrl::Create(WS_CHILD|WS_VISIBLE|(m_bTop?0:TCS_BOTTOM)|TCS_SINGLELINE|TCS_FOCUSONBUTTONDOWN|TCS_FORCEICONLEFT|WS_CLIPSIBLINGS|TCS_HOTTRACK|TCS_TABS /*|TCS_OWNERDRAWFIXED*/, CRect(0, 0, 0, 0), pMainFrame, 42);
  ModifyStyleEx(0, WS_EX_CLIENTEDGE);
  SendMessage(WM_SETFONT, WPARAM(GetStockObject(DEFAULT_GUI_FONT)), 0);

  for (HWND wnd = ::GetTopWindow(*pMainFrame); wnd; wnd = ::GetNextWindow(wnd, GW_HWNDNEXT))
  {
    char wndClass[32];
    ::GetClassName(wnd, wndClass, 32);
    if (strncmp(wndClass, "MDIClient", 32) == 0) break;
  }
  m_mdiClient = wnd;

  ASSERT(m_mdiClient); // Ooops, no MDIClient window?

  // manipulate Z-order so, that our tabctrl is above the mdi client, but below any status bar
  ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
  ::SetWindowPos(m_mdiClient, m_hWnd, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
  m_bImages = (dwStyle & MT_IMAGES) != 0;
  if (m_bImages)
  {
    if (m_images.GetSafeHandle()) 
    {
      m_images.SetImageCount(0);
    }
    else    
    {
      m_images.Create(16, 16, ILC_COLORDDB|ILC_MASK, 1, 1);
    }
    SetImageList(&m_images);
  }

  SetItemSize(CSize(50, 22)); // Fixed Width Experiment
}

//##ModelId=49B87B97008E
void CMDITabs::OnContextMenu(CWnd* pWnd, CPoint point) 
{
  return;
 //将浮动菜单关闭   yinzhehong 20070413
  TCHITTESTINFO hit;
  hit.pt = point;
  ScreenToClient(&hit.pt);
  int i = HitTest(&hit);
  if (i >= 0) 
  {
    TCITEM item;
    item.mask = TCIF_PARAM;
    GetItem(i, &item);

    HWND hWnd = HWND(item.lParam);
    SetCurSel(i);
    ::BringWindowToTop(hWnd);

    HMENU menu = HMENU(::SendMessage(::GetTopWindow(hWnd), WM_GETTABSYSMENU, 0, 0));
    if (menu == 0) menu = ::GetSystemMenu(hWnd, FALSE);
    UINT cmd = ::TrackPopupMenu(menu, TPM_RETURNCMD|TPM_RIGHTBUTTON|TPM_VCENTERALIGN, point.x, point.y, 0, m_hWnd, NULL);
    ::SendMessage(hWnd, WM_SYSCOMMAND, cmd, 0);
  }
}

//##ModelId=49B87B970092
void CMDITabs::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  int i = GetCurSel();
  if (i >= 0) 
  {
    TCITEM item;
    item.mask = TCIF_PARAM;
    GetItem(i, &item);
    HWND hWnd = HWND(item.lParam);
    ::ShowWindow(hWnd, SW_MAXIMIZE);
  }
}

void CMDITabs::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	//return;
	// TODO: Add your message handler code here and/or call default
	LPDRAWITEMSTRUCT lp = lpDrawItemStruct;
	if(lp->CtlType == ODT_TAB)
	{
		CDC dc;
		
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
			"Microsoft YaHei UI"));                 // lpszFacename
		
		dc.Attach(lp->hDC);
		
		if(GetSafeHwnd() == NULL)
			return;
		CRect rect,rectMain,rectItem;
		GetClientRect(rect);
		
		//默认字体
		CFont* pFont = (CFont*)dc.SelectObject(&font);
		
		int nSel = GetCurSel();
		
		//画各item
		for(int i=0;i<GetItemCount();i++)
		{
			dc.SelectObject(pFont);
			
			GetItemRect(i,rectItem);
			if(i != nSel)
			{
			//	dc.SetBkColor(RGB(245, 245, 241));
				dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
				dc.SetTextColor(RGB(0,0,0));
				//dc.SetBkColor(RGB(255, 0, 0));
				//dc.SetTextColor(RGB(255,0,0));
			}
			else
			{
			//	dc.SetBkColor(RGB(252, 252, 254));
				//dc.SetTextColor(RGB(0,0,0));
				//dc.SetBkColor(RGB(255, 0, 0));
				dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
				dc.SetTextColor(RGB(255,0,0));
				//dc.SelectObject(&font);
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
			GetItem(i, &tc);
			dc.DrawText(tc.pszText,rectItem,DT_EXPANDTABS|DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			
			dc.SelectObject(pFont);
		}
		
		dc.Detach();
		dc.DeleteDC();
		
		font.DeleteObject();
	}
}


void CMDITabs::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	
	COLORREF crTab = GetTabColor();
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);
	
	pDC->Draw3dRect(rBorder, crHighlight, crShadow);
}



void CMDITabs::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT (s_dwCustomLook & ETC_FLAT);
	
	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);
	
	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	
	COLORREF crTab = GetTabColor(bSelected);
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);
	
	if (bSelected || bBackTabs)
	{
		rItem.bottom += bSelected ? -1 : 1;
		
		// edges
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), crHighlight);
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), crHighlight);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
	else // draw simple dividers
	{
		pDC->FillSolidRect(CRect(rItem.left - 1, rItem.top, rItem.left, rItem.bottom), crShadow);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
}


COLORREF CMDITabs::GetTabColor(BOOL bSelected)
{
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	BOOL bHiliteSel = (s_dwCustomLook & ETC_SELECTION);
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);
	BOOL bFlat = (s_dwCustomLook & ETC_FLAT);
	
	if (bSelected && bHiliteSel)
	{
		if (bColor)
			return Lighter((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 1.4f);
		else
			return Lighter(::GetSysColor(COLOR_3DFACE), 1.4f);
	}
	else if (!bSelected)
	{
		if (bBackTabs || !bFlat)
		{
			if (bColor)
				return Darker((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.9f);
			else
				return Darker(::GetSysColor(COLOR_3DFACE), 0.9f);
		}
		else
			return (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;
	}
	
	// else
	return ::GetSysColor(COLOR_3DFACE);
}