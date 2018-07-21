#ifndef NSFLEXCONSTRAINTS_IMPLEMENTATION_FILE
#define NSFLEXCONSTRAINTS_IMPLEMENTATION_FILE

// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NSFlexConstraints.h"
#include "NSFlexConstraintList.h"

//#include <basis/convert_utf.h>

const CNSFlexHorizontalConstraint NSFlexHorizontallyFixed (0.0f, 0.0f);
const CNSFlexHorizontalConstraint NSFlexShiftRight        (1.0f, 1.0f);
const CNSFlexHorizontalConstraint NSFlexExpandRight       (0.0f, 1.0f);

CNSFlexHorizontalConstraint::CNSFlexHorizontalConstraint(
    float fLeft, float fRight) :
        m_fLeft(fLeft), m_fRight(fRight)
{
    ASSERT(m_fLeft >= 0.0f && m_fLeft <= 1.0f);
    ASSERT(m_fRight >= 0.0f && m_fRight <= 1.0f);
    ASSERT(m_fRight >= m_fLeft);
        // or else the control shrinks as the dialog grows.
}

CNSFlexHorizontalConstraint::CNSFlexHorizontalConstraint(
    const CNSFlexHorizontalConstraint& ref)
{
    m_fLeft = ref.m_fLeft;
    m_fRight = ref.m_fRight;
}

const CNSFlexHorizontalConstraint& CNSFlexHorizontalConstraint::operator=(
    CNSFlexHorizontalConstraint& ref)
{
    m_fLeft = ref.m_fLeft;
    m_fRight = ref.m_fRight;
    return(*this);
}

CNSFlexHorizontalConstraint* CNSFlexHorizontalConstraint::CopyMe() const
{
    return new CNSFlexHorizontalConstraint(*this);
}

CNSFlexHorizontalConstraint::~CNSFlexHorizontalConstraint()
{
}

void CNSFlexHorizontalConstraint::Init(
    const CWnd* pContainerWnd, int nWidth)
{
}

void CNSFlexHorizontalConstraint::SetLeft(float fLeft)
{
    m_fLeft = fLeft;
    ASSERT(m_fLeft >= 0.0f && m_fLeft <= 1.0f);
}

void CNSFlexHorizontalConstraint::SetRight(float fRight)
{
    m_fRight = fRight;
    ASSERT(m_fRight >= 0.0f && m_fRight <= 1.0f);
}

    

const CNSFlexVerticalConstraint NSFlexVerticallyFixed (0.0f, 0.0f);
const CNSFlexVerticalConstraint NSFlexShiftDown       (1.0f, 1.0f);
const CNSFlexVerticalConstraint NSFlexExpandDown      (0.0f, 1.0f);

CNSFlexVerticalConstraint::CNSFlexVerticalConstraint(
    float fTop, float fBottom) :
        m_fTop(fTop), m_fBottom(fBottom)
{
    ASSERT(m_fTop >= 0.0f && m_fTop <= 1.0f);
    ASSERT(m_fBottom >= 0.0f && m_fBottom <= 1.0f);
    ASSERT(m_fBottom >= m_fTop);
        // or else the control shrinks as the dialog grows.
}

CNSFlexVerticalConstraint::CNSFlexVerticalConstraint(
    const CNSFlexVerticalConstraint& ref)
{
    m_fTop = ref.m_fTop;
    m_fBottom = ref.m_fBottom;
}

const CNSFlexVerticalConstraint& CNSFlexVerticalConstraint::operator=(
    CNSFlexVerticalConstraint& ref)
{
    m_fTop = ref.m_fTop;
    m_fBottom = ref.m_fBottom;
    return(*this);
}

CNSFlexVerticalConstraint* CNSFlexVerticalConstraint::CopyMe() const
{
    return new CNSFlexVerticalConstraint(*this);
}

CNSFlexVerticalConstraint::~CNSFlexVerticalConstraint()
{
}

void CNSFlexVerticalConstraint::Init(
    const CWnd* pContainerWnd, int nHeight)
{
}
    
void CNSFlexVerticalConstraint::SetTop(float fTop)
{
    m_fTop = fTop;
    ASSERT(m_fTop >= 0.0f && m_fTop <= 1.0f);
}

void CNSFlexVerticalConstraint::SetBottom(float fBottom)
{
    m_fBottom = fBottom;
    ASSERT(m_fBottom >= 0.0f && m_fBottom <= 1.0f);
}


CNSFlexConstraint::CNSFlexConstraint(UINT nControlID,
    const CNSFlexHorizontalConstraint& HorizontalConstraint,
    const CNSFlexVerticalConstraint& VerticalConstraint) :
        m_nControlID(nControlID)
{
    // Using the CopyMe functions allows the Constraints to be
    // passed in by const ref as the base type and then get a pointer
    // we can use to a copy of the exact type.
    m_pHorizontalConstraint = HorizontalConstraint.CopyMe();
    m_pVerticalConstraint = VerticalConstraint.CopyMe();

    ASSERT(m_pHorizontalConstraint);
    ASSERT(m_pVerticalConstraint);
}

CNSFlexConstraint::~CNSFlexConstraint()
{
    delete m_pHorizontalConstraint;
    delete m_pVerticalConstraint;
}

void CNSFlexConstraint::SetBasePosition(int nBaseLeft, int nBaseTop, 
    int nBaseRight, int nBaseBottom)
{
    m_nBaseLeft = nBaseLeft;
    m_nBaseTop = nBaseTop;
    m_nBaseRight = nBaseRight;
    m_nBaseBottom = nBaseBottom;
}

BOOL CNSFlexConstraint::Init(const CWnd* pContainerWnd,
    int nClientWidth, int nClientHeight)
{
    ASSERT_VALID(pContainerWnd);

    CWnd* pFlexControlWnd = pContainerWnd->GetDlgItem(GetControlID());
    if(!pFlexControlWnd)
        return FALSE;

    CRect rectFlexControlWnd;
    pFlexControlWnd->GetWindowRect(rectFlexControlWnd);

    // Special ComboBox handling is needed on Windows NT before version 4.0
    // and under Win32s
    DWORD dwVersion = GetVersion(); 
    if (LOBYTE(LOWORD(dwVersion)) < 4)
    {
        char szClassName[10];
        USES_CONVERSION;
        GetClassName(pFlexControlWnd->GetSafeHwnd(), A2T(szClassName), 10);
        if (lstrcmp(A2T(szClassName), A2T("ComboBox")) == 0)  // It is a combo box.
        {
            CRect rectDrop;
            ((CComboBox *)pFlexControlWnd)->
                GetDroppedControlRect(&rectDrop);
            rectFlexControlWnd.BottomRight().y =
                rectFlexControlWnd.TopLeft().y + rectDrop.Height();
        }
    }

    pContainerWnd->ScreenToClient(rectFlexControlWnd);

    SetBasePosition(rectFlexControlWnd.left,
        rectFlexControlWnd.top,rectFlexControlWnd.right, 
        rectFlexControlWnd.bottom);

    m_pHorizontalConstraint->Init(pContainerWnd, nClientWidth);
    m_pVerticalConstraint->Init(pContainerWnd, nClientHeight);
    
    return TRUE;
}

CNSFlexConstraintList::CNSFlexConstraintList(CWnd* pContainerWnd) :
    m_pContainerWnd(pContainerWnd), m_bConstraintsInitialized(FALSE)
{

}

CNSFlexConstraintList::~CNSFlexConstraintList()
{
    for (POSITION pos = m_FlexConstraints.GetHeadPosition(); pos; )
    {
        CNSFlexConstraint* pFlexConstraint = 
            (CNSFlexConstraint*)m_FlexConstraints.GetNext(pos);
        delete pFlexConstraint;
    }
}

void CNSFlexConstraintList::AddConstraint(UINT nControlID,
    const CNSFlexHorizontalConstraint& HorizontalConstraint,
    const CNSFlexVerticalConstraint& VerticalConstraint)
{
    CNSFlexConstraint* pFlexConstraint = new CNSFlexConstraint(nControlID,
        HorizontalConstraint, VerticalConstraint);
    ASSERT(pFlexConstraint);
    m_FlexConstraints.AddTail(pFlexConstraint);
}

#ifdef _DEBUG

BOOL CNSFlexConstraintList::AssertAllFlexControlsExist()
{
    for (POSITION pos = m_FlexConstraints.GetHeadPosition(); pos; )
    {
        CNSFlexConstraint* pFlexConstraint = 
            (CNSFlexConstraint*)m_FlexConstraints.GetNext(pos);
        
        UINT nControlID = pFlexConstraint->GetControlID();

        CWnd* pFlexControlWnd = 
            m_pContainerWnd->GetDlgItem(nControlID);

        if (!pFlexControlWnd)
        {
            ASSERT(FALSE);
            return FALSE;
        }
    }

    return TRUE;
}

#endif

void CNSFlexConstraintList::PositionControls(int nClientWidth,
    int nClientHeight)
{
    if (m_FlexConstraints.GetCount() == 0)
        return;

    if (!m_bConstraintsInitialized)
        InitConstraints();

    if (!m_bConstraintsInitialized)
        return;

    int nDeltaClientWidth = max(nClientWidth - m_nClientBaseWidth,0);
    int nDeltaClientHeight = max(nClientHeight - m_nClientBaseHeight,0);

  // REVISION 4/10/97 TO WORK WITH ACTIVEX CONTROLS (Ed Smetak)
  // BeginDeferWindowPos and DeferWindowPos do not work with
  // ActiveX controls. MoveWindow works OK, so do that instead.
    //  HDWP  hDeferWndPosStruct =
    //    BeginDeferWindowPos(m_FlexConstraints.GetCount());  
  // END REVISION (Ver 1.04)

    for (POSITION pos = m_FlexConstraints.GetHeadPosition(); pos; )
    {
        CNSFlexConstraint* pFlexConstraint = 
            (CNSFlexConstraint*)m_FlexConstraints.GetNext(pos);
        
        CWnd* pFlexControlWnd = 
            m_pContainerWnd->GetDlgItem(pFlexConstraint->GetControlID());
        ASSERT(pFlexControlWnd);

        int nNewLeft = pFlexConstraint->GetBaseLeft() + 
            (int)(pFlexConstraint->GetLeftLocator()*nDeltaClientWidth);
        int nNewTop = pFlexConstraint->GetBaseTop() + 
            (int)(pFlexConstraint->GetTopLocator()*nDeltaClientHeight);
        int nNewRight = pFlexConstraint->GetBaseRight() + 
            (int)(pFlexConstraint->GetRightLocator()*nDeltaClientWidth);
        int nNewBottom = pFlexConstraint->GetBaseBottom() + 
            (int)(pFlexConstraint->GetBottomLocator()*nDeltaClientHeight);

  // REVISION 4/10/97 TO WORK WITH ACTIVEX CONTROLS (Ed Smetak)
  //  hDeferWndPosStruct = DeferWindowPos(hDeferWndPosStruct,
  //    pFlexControlWnd->GetSafeHwnd(), NULL,
  //    nNewLeft,nNewTop,nNewRight-nNewLeft,nNewBottom-nNewTop,
  //    SWP_NOACTIVATE|SWP_NOZORDER);

    pFlexControlWnd->MoveWindow(nNewLeft,nNewTop,nNewRight-nNewLeft,
      nNewBottom-nNewTop,FALSE);
    pFlexControlWnd->Invalidate();
    for (CWnd* pWnd = pFlexControlWnd->GetTopWindow(); pWnd;
      pWnd = pWnd->GetNextWindow())
        if (pWnd)
          pWnd->Invalidate();
  // END REVISION (Ver 1.04)

    }

  // REVISION 4/10/97 TO WORK WITH ACTIVEX CONTROLS (Ed Smetak)
  //  EndDeferWindowPos(hDeferWndPosStruct);
  // END REVISION (Ver 1.04)
}

void CNSFlexConstraintList::SetClientBaseSize(int nClientBaseWidth, 
    int nClientBaseHeight)
{
    m_nClientBaseWidth = nClientBaseWidth;
    m_nClientBaseHeight = nClientBaseHeight;
}

void CNSFlexConstraintList::InitConstraints()
{
    for (POSITION pos = m_FlexConstraints.GetHeadPosition(); pos; )
    {
        CNSFlexConstraint* pFlexConstraint = 
            (CNSFlexConstraint*)m_FlexConstraints.GetNext(pos);
        
        if (!pFlexConstraint->Init(m_pContainerWnd,
            m_nClientBaseWidth, m_nClientBaseHeight))
                return;
    }

    m_bConstraintsInitialized = TRUE;
}

#endif //NSFLEXCONSTRAINTS_IMPLEMENTATION_FILE

