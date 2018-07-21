#ifndef NSFLEXPROPERTYPAGE_IMPLEMENTATION_FILE
#define NSFLEXPROPERTYPAGE_IMPLEMENTATION_FILE

// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NSFlexPropertyPage.h"
#include "NSFlexConstraintList.h"
#include "NSFlexPropertySheet.h"
#include "NSViewsMessages.h"

//#include <basis/convert_utf.h>

IMPLEMENT_DYNAMIC(CNSFlexPropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CNSFlexPropertyPage, CPropertyPage)
  //{{AFX_MSG_MAP(CNSFlexPropertyPage)
  ON_WM_CREATE()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CNSFlexPropertyPage::CNSFlexPropertyPage() 
: CPropertyPage()
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexPropertyPage::CNSFlexPropertyPage(UINT nIDTemplate, 
  UINT nIDCaption /*=0*/) : CPropertyPage(nIDTemplate,nIDCaption)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}
  
CNSFlexPropertyPage::CNSFlexPropertyPage(LPCTSTR lpszTemplateName, 
  UINT nIDCaption /*=0*/) : CPropertyPage(lpszTemplateName,nIDCaption)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexPropertyPage::~CNSFlexPropertyPage()
{
    delete m_pFlexConstraintList;
}

void CNSFlexPropertyPage::AddFlexConstraint(UINT nControlID, 
  const CNSFlexHorizontalConstraint& HorizontalConstraint,
  const CNSFlexVerticalConstraint& VerticalConstraint)
{
  m_pFlexConstraintList->AddConstraint(nControlID,
    HorizontalConstraint, VerticalConstraint);
}

CDocument* CNSFlexPropertyPage::GetDocument() const
{
  // This function was revised 2/24/97 to work properly for
  // SDI applications. (Ed Smetak)

  // ORIGINAL CODE:

    // CFrameWnd* pFrameWnd = (CFrameWnd *)GetParentFrame();
    // if (pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
  //     pFrameWnd->IsKindOf(RUNTIME_CLASS(COleIPFrameWnd)))
    //         return pFrameWnd->GetActiveDocument();
    // return NULL;

  // REVISED CODE: (Ver 1.03)

    CFrameWnd* pFrameWnd = GetParentFrame();

    if (pFrameWnd)
    return pFrameWnd->GetActiveDocument();

    return NULL;
}

CView* CNSFlexPropertyPage::GetView() const
{
  // This function was revised 2/24/97 to work properly for
  // SDI applications. (Ed Smetak)

  // ORIGINAL CODE:

    // CFrameWnd* pFrameWnd = (CFrameWnd *)GetParentFrame();
    // if (pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)) ||
  //      pFrameWnd->IsKindOf(RUNTIME_CLASS(COleIPFrameWnd)))
    //         return pFrameWnd->GetActiveView();
    // return NULL;

  // REVISED CODE: (Ver 1.03)

    CFrameWnd* pFrameWnd = GetParentFrame();

  if (pFrameWnd)
    return pFrameWnd->GetActiveView();

  return NULL;
}

int CNSFlexPropertyPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    m_pFlexConstraintList->
    SetClientBaseSize(lpCreateStruct->cx,lpCreateStruct->cy);

  if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  return 0;
}

BOOL CNSFlexPropertyPage::OnInitDialog() 
{
  CPropertyPage::OnInitDialog();

  if (GetDocument())
    OnUpdate(NULL,0,NULL);

#ifdef _DEBUG
  if (!GetParent()->IsKindOf(RUNTIME_CLASS(CNSFlexPropertySheet)))
    TRACE_PRINT("Warning!  "
         "CNSFlexPropertyPage used without CNSFlexPropertySheet.\n");
#endif

  ASSERT(m_pFlexConstraintList->AssertAllFlexControlsExist());


  return TRUE;
}

BOOL CNSFlexPropertyPage::OnSetActive() 
{
  if (!CPropertyPage::OnSetActive())
    return FALSE;

  GetParent()->SendMessage(WM_PAGESETACTIVE);
    return TRUE;
}

void CNSFlexPropertyPage::OnSize(UINT nType, int cx, int cy) 
{
  CPropertyPage::OnSize(nType, cx, cy);
  
    if (nType != SIZE_MINIMIZED)
    {
        m_pFlexConstraintList->PositionControls(cx,cy);

    // REVISION 4/10/97 TO WORK WITH ACTIVEX CONTROLS (Ed Smetak)
        // To get rid of screen garbage left on Windows NT before version 4.0
        // and under Win32s
        // DWORD dwVersion = GetVersion(); 
        // if (LOBYTE(LOWORD(dwVersion)) < 4)
    //     Invalidate();
        Invalidate();
    // END REVISION (Ver 1.04)
    }
}

void CNSFlexPropertyPage::OnUpdate(CView* pSender, LPARAM lHint, 
  CObject* pHint)  
{

}


#endif //NSFLEXPROPERTYPAGE_IMPLEMENTATION_FILE

