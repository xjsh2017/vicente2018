// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexFormView_h__
#define __NSFlexFormView_h__

#include "NSViewsAfxExt.h"
#include "NSFlexConstraints.h"

class CNSFlexConstraintList;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexFormView turns a normal CFormView into a "flexible" form view that
// automatically repositions its controls when the form view window is resized.

// Make sure to use a CNSFlexMDIChildWnd as the frame window for a 
// CNSFlexFormView! See MFC CFormView documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexFormView : public CFormView
{

  DECLARE_DYNAMIC(CNSFlexFormView)

  // --------------------------------------------------------------------------

  public:

    // Constructors.
    CNSFlexFormView(UINT nIDTemplate);
    CNSFlexFormView(LPCTSTR lpszTemplateName);
    // nIDTemplate - Contains the ID number of a dialog-box template resource.
    // lpszTemplateName - Contains a null-terminated string that is the name
    //   of a dialog-box template resource.

    // Destructor.
    virtual ~CNSFlexFormView();

    // Add horizontal and vertical positioning constraints to a specified  
    // control that will be applied when the window is resized.
    void AddFlexConstraint(UINT nControlID,
        const CNSFlexHorizontalConstraint& HorizontalConstraint,
        const CNSFlexVerticalConstraint& VerticalConstraint);
    //  nControlID - ID of specified dialog control.
    //  HorizontalConstraint - Horizontal positioning constraint.
    //  Vertical Constraint - Vertical positioning constraint.

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexFormView)
    public:
    virtual void OnInitialUpdate();
    //}}AFX_VIRTUAL

  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexFormView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  private:

    CNSFlexConstraintList* m_pFlexConstraintList;

        // REVISION 4/10/97 TO WORK WITH SDI APPLICATIONS (Ed Smetak)
        BOOL m_bNeedInitialUpdate;
        // END REVISION (Ver 1.04)

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
