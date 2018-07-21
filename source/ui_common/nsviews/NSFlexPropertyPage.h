// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexPropertyPage_h__
#define __NSFlexPropertyPage_h__

#include "NSViewsAfxExt.h"
#include "NSFlexConstraints.h"

class CNSFlexConstraintList;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexPropertyPage turns a normal CPropertyPage into a "flexible" property
// page that automatically repositions its controls when the property page
// window is resized.

// Make sure to use a CNSFlexPropertySheet with a CNSFlexPropertyPage! See the
// MFC CPropertyPage documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexPropertyPage : public CPropertyPage
{

  DECLARE_DYNAMIC(CNSFlexPropertyPage)

  // --------------------------------------------------------------------------

  public:

    // Constructors.
    CNSFlexPropertyPage();
    CNSFlexPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
    CNSFlexPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
    // nIDTemplate - Contains the ID number of a dialog-box template resource.
    // lpszTemplateName - Contains a null-terminated string that is the name
    //   of a dialog-box template resource.
    // nIDCaption - ID of the name to be placed in the tab for this page. If 0, 
    //  the name will be taken from the dialog template for this page.

    // Destructor.
    virtual ~CNSFlexPropertyPage();

    // Add horizontal and vertical positioning constraints to a specified  
    // control that will be applied when the window is resized.
    void AddFlexConstraint(UINT nControlID,
        const CNSFlexHorizontalConstraint& HorizontalConstraint,
        const CNSFlexVerticalConstraint& VerticalConstraint);
    //  nControlID - ID of specified dialog control.
    //  HorizontalConstraint - Horizontal positioning constraint.
    //  Vertical Constraint - Vertical positioning constraint.
      
    // Get document object for any CNSFlexPropertyPage that is contained in a
    // CNSFlexPropertySheetView. This function makes a CNSFlexPropertyPage look
    // and feel a lot like a CView. Returns NULL if the CNSFlexPropertyPage is
    // not contained in a CNSFlexPropertySheetView.
    CDocument* GetDocument() const;

    // Get view object for any CNSFlexPropertyPage that is contained in a
    // CNSFlexPropertySheetView. Returns NULL if the CNSFlexPropertyPage is not
    // contained in a CNSFlexPropertySheetView.
    CView* GetView() const;

    // Called after the CNSFlexPropertySheetView’s document has been modified. 
    // This function makes a CNSFlexPropertyPage look and feel a lot like a 
    // CView. This function is only applicable for a CNSFlexPropertyPage that 
    // is contained in a CNSFlexPropertySheetView.
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);    

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexPropertyPage)
    public:
    virtual BOOL OnSetActive();
    //}}AFX_VIRTUAL
    
  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexPropertyPage)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

  private:

    CNSFlexConstraintList* m_pFlexConstraintList;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
