// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexDialog_h__
#define __NSFlexDialog_h__

#include "NSViewsAfxExt.h"
#include "NSFlexConstraints.h"

class CNSFlexConstraintList;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexDialog turns a normal CDialog into a "flexible" dialog that
// automatically repositions its controls when the dialog window is resized. 
// The class also supports an Apply button and provides a convenient virtual 
// function called OnApplyChanges() to handle the application of changes to a 
// subject whenever the Apply or OK button is pressed.

// See the MFC CDialog documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexDialog : public CDialog
{

  DECLARE_DYNAMIC(CNSFlexDialog)

  // --------------------------------------------------------------------------

  public: 

    // Constructors.
    CNSFlexDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL,
        void* pSubject = NULL);
    CNSFlexDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL,
        void* pSubject = NULL);
    //  nIDTemplate - Contains the ID number of a dialog-box template resource.
    //  lpszTemplateName - Contains a null-terminated string that is the name
    //    of a dialog-box template resource.
    //  pParentWnd - Points to the parent or owner window object (of type CWnd)
    //    to which the dialog object belongs. If it is NULL, the dialog 
    //    object’s parent window is set to the main application window.
    //  pSubject - Pointer to subject that will be passed to OnApplyChanges().

    // Destructor.
    virtual ~CNSFlexDialog();
    
    // Add horizontal and vertical positioning constraints to a specified  
    // control that will be applied when the window is resized.
    void AddFlexConstraint(UINT nControlID,
        const CNSFlexHorizontalConstraint& HorizontalConstraint,
        const CNSFlexVerticalConstraint& VerticalConstraint);
    //  nControlID - ID of specified dialog control.
    //  HorizontalConstraint - Horizontal positioning constraint.
    //  Vertical Constraint - Vertical positioning constraint.

    // Returns the subject pointer that was set in the constructor or using
    // SetSubject();
    void* GetSubject() const;

    // Called when either the Apply or OK button is pressed after dialog
    // data has been validated. Provides a nice place to handle the application
    // of changes to a subject. Nonzero TRUE return indicates changes were
    // successfully applied. Zero FALSE return indicates application of
    // changes failed.
    virtual BOOL OnApplyChanges(void* pSubject);
    //  pSubject - Pointer to subject that was set in the constructor or using 
    //    SetSubject(). 

    // Enable or disable the Apply button.
    void SetModified(BOOL bChanged = TRUE);
    //  bChanged - TRUE to indicate that the dialog settings have been modified
    //    since the last time they were applied; FALSE to indicate that the 
    //    dialog settings have been applied, or should be ignored.

    // Set the dialog subject pointer that gets passed to OnApplyChanges.
    // The typical time to call this function is after the CNSFlexDialog has
    // been constructed, but before calling DoModal().
    virtual void SetSubject(void* pSubject);
    //  pSubject - Pointer to subject that will be passed to OnApplyChanges().

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexDialog)
    //}}AFX_VIRTUAL

    virtual void OnOK();

  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexDialog)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg void OnApply();
    DECLARE_MESSAGE_MAP()

  private:

    CNSFlexConstraintList* m_pFlexConstraintList;
    int m_nMinTrackingWidth;
    int m_nMinTrackingHeight;
    BOOL m_bModified;
    void* m_pSubject;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
