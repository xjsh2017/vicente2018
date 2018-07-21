#ifndef NSFLEXPROPERTYSHEET_IMPLEMENTATION_FILE
#define NSFLEXPROPERTYSHEET_IMPLEMENTATION_FILE

// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NSFlexPropertySheet.h"
#include "NSFlexConstraints.h"
#include "NSFlexConstraintList.h"
#include "NSFlexMDIChildWnd.h"
#include "NSViewsMessages.h"

//#include <basis/convert_utf.h>

IMPLEMENT_DYNAMIC(CNSFlexPropertySheet, CPropertySheet)

BEGIN_MESSAGE_MAP(CNSFlexPropertySheet, CPropertySheet)
  //{{AFX_MSG_MAP(CNSFlexPropertySheet)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_WM_GETMINMAXINFO()
  ON_WM_ERASEBKGND()
  //}}AFX_MSG_MAP
    ON_MESSAGE(WM_PAGESETACTIVE, OnPageSetActive)
  ON_COMMAND(ID_APPLY_NOW, OnApply)
  ON_COMMAND(IDOK, OnOK)
  ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

CNSFlexPropertySheet::CNSFlexPropertySheet() 
: CPropertySheet(), 
  m_nMinTrackingWidth(0), m_nMinTrackingHeight(0), m_pSubject(NULL), m_bLastCommandWasEscape(false)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexPropertySheet::CNSFlexPropertySheet(UINT nIDCaption, CWnd* pParentWnd,
  UINT nSelectPage, void* pSubject) : 
  CPropertySheet(nIDCaption,pParentWnd,nSelectPage), 
  m_nMinTrackingWidth(0), m_nMinTrackingHeight(0), m_pSubject(pSubject), m_bLastCommandWasEscape(false)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexPropertySheet::CNSFlexPropertySheet(LPCTSTR pszCaption, 
  CWnd* pParentWnd, UINT nSelectPage, void* pSubject) :
  CPropertySheet(pszCaption,pParentWnd,nSelectPage),
  m_nMinTrackingWidth(0), m_nMinTrackingHeight(0), m_pSubject(pSubject), m_bLastCommandWasEscape(false)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexPropertySheet::~CNSFlexPropertySheet()
{
    delete m_pFlexConstraintList;
}

void* CNSFlexPropertySheet::GetSubject() const
{
  return m_pSubject;
}

void CNSFlexPropertySheet::OnApply() 
{
  if (!GetActivePage()->UpdateData(TRUE))
    return;

  if (!OnApplyChanges(m_pSubject))
    return;

  for (int nPage = 0; nPage < GetPageCount(); nPage++)
  {
    CPropertyPage* pPropertyPage = GetPage(nPage);
    ASSERT(pPropertyPage);
    pPropertyPage->SetModified(FALSE);
  }
}

BOOL CNSFlexPropertySheet::OnApplyChanges(void* pSubject)
{
  return TRUE;
}

void CNSFlexPropertySheet::OnCancel() 
{
  if (!m_bLastCommandWasEscape || GetActivePage()->OnQueryCancel())
    EndDialog(IDCANCEL);
}

int CNSFlexPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    return -1;
  
    LONG lSTYLE = GetWindowLong(m_hWnd,GWL_STYLE);
    LONG lEXSTYLE = GetWindowLong(m_hWnd,GWL_EXSTYLE);

    if ( ((lSTYLE & DS_MODALFRAME) == DS_MODALFRAME) ||
         ((lSTYLE & WS_THICKFRAME) != WS_THICKFRAME) )
    {
        lSTYLE &= ~(LONG)DS_MODALFRAME;
        lSTYLE |= WS_THICKFRAME;
    SetWindowLong(m_hWnd,GWL_STYLE,lSTYLE);
    }

    if ((lEXSTYLE & WS_EX_DLGMODALFRAME) == WS_EX_DLGMODALFRAME)
    {
        // AB - I removed this to get rid of the icon in the upper left
        //  hand corner of the dialog.
        //lEXSTYLE &= ~(DWORD)WS_EX_DLGMODALFRAME;
      //SetWindowLong(m_hWnd,GWL_EXSTYLE,lEXSTYLE);
    }
    else
    {
        lEXSTYLE |= (DWORD)WS_EX_CLIENTEDGE;
      SetWindowLong(m_hWnd,GWL_EXSTYLE,lEXSTYLE);
    }
  
  return 0;
}

BOOL CNSFlexPropertySheet::OnEraseBkgnd(CDC* pDC) 
{
  ASSERT_VALID(pDC);
  CBrush backBrush(GetSysColor(COLOR_BTNFACE));
  CBrush* pOldBrush = pDC->SelectObject(&backBrush);
  CRect rect;
  pDC->GetClipBox(&rect);
  pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);
  pDC->SelectObject(pOldBrush);
  return TRUE;
}

void CNSFlexPropertySheet::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
  CPropertySheet::OnGetMinMaxInfo(lpMMI);

    lpMMI->ptMinTrackSize.x = m_nMinTrackingWidth;
    lpMMI->ptMinTrackSize.y = m_nMinTrackingHeight;
}

BOOL CNSFlexPropertySheet::OnInitDialog() 
{
  CPropertySheet::OnInitDialog();

  if (m_bModeless)
  {
#ifdef _DEBUG
    if (!GetParentFrame()->IsKindOf(RUNTIME_CLASS(CNSFlexMDIChildWnd)))
      TRACE_PRINT("Warning!  Modeless CNSFlexPropertySheet "
             "used without a CNSFlexMDIChildWnd frame.\n");
#endif
    SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);       
  }

    CRect rect;

  GetWindowRect(rect);
    m_nMinTrackingWidth = rect.Width();
    m_nMinTrackingHeight = rect.Height();

  GetClientRect(rect);
    m_pFlexConstraintList->
    SetClientBaseSize(rect.Width(),rect.Height());

  m_pFlexConstraintList->AddConstraint(
    IDOK, NSFlexShiftRight, NSFlexShiftDown);
  m_pFlexConstraintList->AddConstraint(
    IDCANCEL, NSFlexShiftRight, NSFlexShiftDown);  
  if ((m_psh.dwFlags & PSH_NOAPPLYNOW) != PSH_NOAPPLYNOW)
    m_pFlexConstraintList->AddConstraint(
      ID_APPLY_NOW, NSFlexShiftRight, NSFlexShiftDown);  
  if ((m_psh.dwFlags & PSH_HASHELP) == PSH_HASHELP)
    m_pFlexConstraintList->AddConstraint(
      IDHELP, NSFlexShiftRight, NSFlexShiftDown);  
  m_pFlexConstraintList->AddConstraint(
    AFX_IDC_TAB_CONTROL, NSFlexExpandRight, NSFlexExpandDown);  

  return TRUE;
}

void CNSFlexPropertySheet::OnOK() 
{
  if (!GetActivePage()->UpdateData(TRUE))
    return;

  if (!OnApplyChanges(m_pSubject))
    return;

  EndDialog(IDOK);
}

LRESULT CNSFlexPropertySheet::OnPageSetActive(WPARAM wParam, LPARAM lParam)
{
    if (m_nMinTrackingWidth == 0)
    return 1;
  
  CRect rectTabControl;
  GetTabControl()->GetWindowRect(&rectTabControl);
  ScreenToClient(&rectTabControl);

  CRect rectTab;
  int nCurSel = GetTabControl()->GetCurSel();
  GetTabControl()->GetItemRect(nCurSel,&rectTab);

  CPropertyPage* pPropertyPage = GetPage(nCurSel);
  rectTabControl.TopLeft().y += rectTab.BottomRight().y;

  pPropertyPage->SetWindowPos(NULL,rectTabControl.TopLeft().x+4,
    rectTabControl.TopLeft().y+2,rectTabControl.Width()-8,
    rectTabControl.Height()-5,SWP_NOZORDER|SWP_NOACTIVATE);

  return 1;
}

void CNSFlexPropertySheet::OnSize(UINT nType, int cx, int cy) 
{
  CPropertySheet::OnSize(nType, cx, cy);
  
    if (nType != SIZE_MINIMIZED)
  {
        m_pFlexConstraintList->PositionControls(cx,cy);
    OnPageSetActive(0,0);

    // REVISION 4/10/97 TO WORK WITH ACTIVEX CONTROLS (Ed Smetak)
        // To get rid of screen garbage left on Windows NT before version 4.0
        // and under Win32s.
        // (MAK note: Is it needed under newer versions as well?)
        // DWORD dwVersion = GetVersion(); 
        // if (LOBYTE(LOWORD(dwVersion)) < 4)
        // {

        if (m_nMinTrackingWidth != 0)
        {
            CTabCtrl* pTabCtrl = GetTabControl();
            ASSERT(pTabCtrl);
            pTabCtrl->Invalidate();
        }

    Invalidate();

        // }
    // END REVISION (Ver 1.04)

  }
}

BOOL CNSFlexPropertySheet::PreTranslateMessage(MSG* pMsg) 
{
  if ((WM_KEYFIRST <= pMsg->message) && (pMsg->message <= WM_KEYLAST) && (pMsg->wParam == VK_ESCAPE))
    m_bLastCommandWasEscape = true;
  else
    m_bLastCommandWasEscape = false;

    CView* pParent = (CView*)GetParent();
    if (pParent && pParent->IsKindOf(RUNTIME_CLASS(CView)))
        if(pParent->PreTranslateMessage(pMsg))
            return TRUE;

    return CPropertySheet::PreTranslateMessage(pMsg);
}

void CNSFlexPropertySheet::SetSubject(void* pSubject)
{
  m_pSubject = pSubject;
}

#endif //NSFLEXPROPERTYSHEET_IMPLEMENTATION_FILE

