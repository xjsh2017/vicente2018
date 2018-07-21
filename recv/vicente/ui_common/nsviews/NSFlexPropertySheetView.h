// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexPropertySheetView_h__
#define __NSFlexPropertySheetView_h__

#include "NSViewsAfxExt.h"

class CNSFlexPropertySheet;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexPropertySheetView, while adding the control repositioning behavior of
// the other classes in this library, does a lot more. It adds a property sheet
// that can live in a view - something that Microsoft did not include in MFC. 
// The basic idea is that a CNSFlexPropertySheet is enclosed in a CScrollView. 
// The CNSFlexPropertyPage property pages that live in the property sheet 
// include two functions, GetDocument() and OnUpdate() that make each page look 
// like a CView from a programming point of view. With this approach, 
// programming an individual page in the property sheet is almost like 
// programming a CFormView.

// Make sure to use a CNSFlexMDIChildWnd as the frame window for a 
// CNSFlexPropertySheetView! See the MFC CFormView and CScrollView 
// documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexPropertySheetView : public CScrollView
{

  DECLARE_DYNAMIC(CNSFlexPropertySheetView)

  // --------------------------------------------------------------------------

  public:

    // Constructor.
    CNSFlexPropertySheetView(UINT nSelectPage = 0);
    // nSelectPage - The index of the page that will initially be on top. 
    //   Default is the first page added to the sheet.

    // Destructor.
    virtual ~CNSFlexPropertySheetView();

    // Get the CNSFlexPropertySheet contained in this CNSFlexPropertySheetView.
    CNSFlexPropertySheet* GetFlexPropertySheet();

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexPropertySheetView)
    public:
    virtual void OnInitialUpdate();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexPropertySheetView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  private:

      CNSFlexPropertySheet* m_pFlexPropertySheet;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
