// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexPropertySheet_h__
#define __NSFlexPropertySheet_h__

#include "NSViewsAfxExt.h"

class CNSFlexConstraintList;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexPropertySheet turns a normal CPropertySheet into a "flexible" 
// property sheet with pages that automatically reposition their controls when 
// the property sheet window is resized. Note that pages of a CPropertySheet 
// must be derived from CNSFlexPropertyPage.

// See the MFC CPropertySheet documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexPropertySheet : public CPropertySheet
{

  DECLARE_DYNAMIC(CNSFlexPropertySheet)

  // --------------------------------------------------------------------------

  public:

    // Constructors.
    CNSFlexPropertySheet();
    CNSFlexPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, 
        UINT nSelectPage = 0, void* pSubject = NULL);
    CNSFlexPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, 
        UINT nSelectPage = 0, void* pSubjecgt = NULL);
    // nIDCaption - ID of the caption to be used for the property sheet.
    // pParentWnd - Points to the parent window of the property sheet. If NULL,
    //   the parent window will be the main window of the application.
    // nSelectPage - The index of the page that will initially be on top. 
    //   Default is the first page added to the sheet.
    // pszCaption - Points to a string containing the caption to be used for 
    //   the property sheet. Cannot be NULL.
    // pSubject - Pointer to subject that will be passed to OnApplyChanges().

    // Destructor.
    virtual ~CNSFlexPropertySheet();

    // Returns the subject pointer that was set in the constructor or using
    // SetSubject(). This function only applies to modal property sheets 
    // (i.e. a CNSFlexPropertySheet that is NOT contained in a 
    // CNSFlexPropertySheetView).
    void* GetSubject() const;

    // Called when either the Apply or OK button is pressed. Provides a nice 
    // place to handle the application of changes to a subject. Nonzero TRUE 
    // return indicates changes were successfully applied. Zero FALSE return 
    // indicates application of changes failed. This function only applies
    // to modal property sheets (i.e. a CNSFlexPropertySheet that is NOT
    // contained in a CNSFlexPropertySheetView).
    virtual BOOL OnApplyChanges(void* pSubject);
    // pSubject - Pointer to subject that was set in the constructor or using 
    //   SetSubject(). 

    // Set the subject pointer that gets passed to OnApplyChanges.
    // The typical time to call this function is after the CNSFlexDialog has
    // been constructed, but before calling DoModal(). This function only 
    // applies to modal property sheets (i.e. a CNSFlexPropertySheet that is 
    // NOT contained in a CNSFlexPropertySheetView).
    virtual void SetSubject(void* pSubject);
    // pSubject - Pointer to subject that will be passed to OnApplyChanges().

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexPropertySheet)
    public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexPropertySheet)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    afx_msg LRESULT OnPageSetActive(WPARAM wParam, LPARAM lParam);
    afx_msg void OnApply();
    afx_msg void OnCancel();
    afx_msg void OnOK();
    DECLARE_MESSAGE_MAP()

  private:

    CNSFlexConstraintList* m_pFlexConstraintList;
    int m_nMinTrackingWidth;
    int m_nMinTrackingHeight;
    void* m_pSubject;
    bool m_bLastCommandWasEscape;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
