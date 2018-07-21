#ifndef NSFLEXFORMVIEW_IMPLEMENTATION_FILE
#define NSFLEXFORMVIEW_IMPLEMENTATION_FILE

// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NSFlexFormView.h"
#include "NSFlexConstraintList.h"
#include "NSFlexMDIChildWnd.h"
#include "NSViewsMessages.h"

//#include <basis/convert_utf.h>

IMPLEMENT_DYNAMIC(CNSFlexFormView, CFormView)

BEGIN_MESSAGE_MAP(CNSFlexFormView, CFormView)
  //{{AFX_MSG_MAP(CNSFlexFormView)
  ON_WM_CREATE()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CNSFlexFormView::CNSFlexFormView(UINT nIDTemplate) :
    CFormView(nIDTemplate)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);

  // REVISION 4/10/97 TO WORK WITH SDI APPLICATIONS (Ed Smetak)
  m_bNeedInitialUpdate = TRUE;
  // END REVISION (Ver 1.04)
}

CNSFlexFormView::CNSFlexFormView(LPCTSTR lpszTemplateName) :
  CFormView(lpszTemplateName)
{
  m_pFlexConstraintList = new CNSFlexConstraintList(this);

  // REVISION 4/10/97 TO WORK WITH SDI APPLICATIONS (Ed Smetak)
  m_bNeedInitialUpdate = TRUE;
  // END REVISION (Ver 1.04)
}

CNSFlexFormView::~CNSFlexFormView()
{
    delete m_pFlexConstraintList;
}

void CNSFlexFormView::AddFlexConstraint(UINT nControlID, 
  const CNSFlexHorizontalConstraint& HorizontalConstraint,
  const CNSFlexVerticalConstraint& VerticalConstraint)
{
  m_pFlexConstraintList->AddConstraint(nControlID,
    HorizontalConstraint, VerticalConstraint);
}

int CNSFlexFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;
  
    m_pFlexConstraintList->
    SetClientBaseSize(lpCreateStruct->cx,lpCreateStruct->cy);
  
  return 0;
}

void CNSFlexFormView::OnInitialUpdate() 
{
  // REVISION 4/10/97 TO WORK WITH SDI APPLICATIONS (Ed Smetak)
  if (m_bNeedInitialUpdate)
  {
    m_bNeedInitialUpdate = FALSE;
  // END REVISION (Ver 1.04)


    ResizeParentToFit(FALSE);

  CWnd *pParentFrame = GetParentFrame();
  ASSERT(pParentFrame);

  if (pParentFrame->IsKindOf(RUNTIME_CLASS(CNSFlexMDIChildWnd)))
    pParentFrame->SendMessage(WM_SETMINTRACKSIZE);
#ifdef _DEBUG
  else
	  TRACE_PRINT("Warning!  "
	  "CNSFlexFormView used without CNSFlexMDIChildWnd frame.\n");
#endif

  CFormView::OnInitialUpdate();

  ASSERT(m_pFlexConstraintList->AssertAllFlexControlsExist());

  // REVISION 4/10/97 TO WORK WITH SDI APPLICATIONS (Ed Smetak)
  }
  else
    CFormView::OnInitialUpdate();
  // END REVISION (Ver 1.04)
}

void CNSFlexFormView::OnSize(UINT nType, int cx, int cy) 
{
  CFormView::OnSize(nType, cx, cy);
  
    if (nType != SIZE_MINIMIZED)
        m_pFlexConstraintList->PositionControls(cx,cy);

/* Here is an alternate version of this function that was presented in the context of a 
   bug fix but does not resemble the code above.  
   May be useful to try if things aren't working as expected here.  MTM 12-8-2000

    if (nType != SIZE_MINIMIZED)
  {
    CPoint* pt = NULL;
        CPoint scrollPos;
    if (m_nMapMode > 0)
    {
      scrollPos = GetScrollPosition();
      pt = &scrollPos;
    }
        m_pFlexConstraintList->PositionControls(cx, cy, pt);
    Invalidate();
  }
*/
}

#endif //NSFLEXFORMVIEW_IMPLEMENTATION_FILE

