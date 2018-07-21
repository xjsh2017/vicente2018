#ifndef NSFLEXDIALOG_IMPLEMENTATION_FILE
#define NSFLEXDIALOG_IMPLEMENTATION_FILE

// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#include "stdafx.h"
#include "NSFlexDialog.h"
#include "NSFlexConstraintList.h"

IMPLEMENT_DYNAMIC(CNSFlexDialog, CDialog)

BEGIN_MESSAGE_MAP(CNSFlexDialog, CDialog)
    //{{AFX_MSG_MAP(CNSFlexDialog)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_APPLY_NOW, OnApply)
END_MESSAGE_MAP()

CNSFlexDialog::CNSFlexDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/, 
    void* pSubject /*=NULL*/) : CDialog(nIDTemplate,pParentWnd),
    m_nMinTrackingWidth(0), m_nMinTrackingHeight(0), m_bModified(FALSE),
    m_pSubject(pSubject)
{
    m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexDialog::CNSFlexDialog(LPCTSTR lpszTemplateName, 
    CWnd* pParentWnd /*=NULL*/, void* pSubject /*=NULL*/) : 
    CDialog(lpszTemplateName,pParentWnd),
    m_nMinTrackingWidth(0), m_nMinTrackingHeight(0), m_bModified(FALSE),
    m_pSubject(pSubject)
{
    m_pFlexConstraintList = new CNSFlexConstraintList(this);
}

CNSFlexDialog::~CNSFlexDialog()
{
    delete m_pFlexConstraintList;
}

void CNSFlexDialog::AddFlexConstraint(UINT nControlID, 
    const CNSFlexHorizontalConstraint& HorizontalConstraint,
    const CNSFlexVerticalConstraint& VerticalConstraint)
{
    m_pFlexConstraintList->AddConstraint(nControlID,
        HorizontalConstraint, VerticalConstraint);
}

void* CNSFlexDialog::GetSubject() const
{
    return m_pSubject;
}

void CNSFlexDialog::OnApply()
{
    if (!UpdateData(TRUE))
        return;

    if (!OnApplyChanges(m_pSubject))
        return;

    SetModified(FALSE);
}

BOOL CNSFlexDialog::OnApplyChanges(void*pSubject)
{
    return TRUE;
}

void CNSFlexDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    lpMMI->ptMinTrackSize.x = m_nMinTrackingWidth;
    lpMMI->ptMinTrackSize.y = m_nMinTrackingHeight;
}

BOOL CNSFlexDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    CRect rect;

    GetWindowRect(rect);
    m_nMinTrackingWidth = rect.Width();
    m_nMinTrackingHeight = rect.Height();

    GetClientRect(rect);
    m_pFlexConstraintList->
        SetClientBaseSize(rect.Width(),rect.Height());

    SetModified(FALSE);

    ASSERT(m_pFlexConstraintList->AssertAllFlexControlsExist());

    return TRUE; 
}

void CNSFlexDialog::OnOK()
{
    if (!UpdateData(TRUE))
        return;

    if (!OnApplyChanges(m_pSubject))
        return;

    EndDialog(IDOK);
}

void CNSFlexDialog::OnSize(UINT nType, int cx, int cy) 
{
    CDialog::OnSize(nType, cx, cy);

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

void CNSFlexDialog::SetModified(BOOL bChanged)
{
    m_bModified = bChanged;

    CWnd* pWnd = GetDlgItem(ID_APPLY_NOW);
        
    if (pWnd)
        pWnd->EnableWindow(m_bModified);
}

void CNSFlexDialog::SetSubject(void* pSubject)
{
    m_pSubject = pSubject;
}

#endif //NSFLEXDIALOG_IMPLEMENTATION_FILE

