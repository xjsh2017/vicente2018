// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexMDIChildWnd_h__
#define __NSFlexMDIChildWnd_h__

#include "NSViewsAfxExt.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexMDIChildWnd provides a frame window for use with a CNSFlexFormView or
// a CNSFlexPropertySheetView.

// See the MFC CMDIChildWnd documentation for additional information.

class AFX_EXT_NSVIEWS CNSFlexMDIChildWnd : public CMDIChildWnd
{

  DECLARE_DYNCREATE(CNSFlexMDIChildWnd)

  // --------------------------------------------------------------------------

  public:

    // Constructor.
    CNSFlexMDIChildWnd();

    // Destructor.
    virtual ~CNSFlexMDIChildWnd();

  // --------------------------------------------------------------------------

  public:

    // ClassWizard
    //{{AFX_VIRTUAL(CNSFlexMDIChildWnd)
    //}}AFX_VIRTUAL

  protected:

    // ClassWizard
    //{{AFX_MSG(CNSFlexMDIChildWnd)
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    //}}AFX_MSG
    afx_msg LRESULT OnSetMinTrackSize(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

  private:

    int m_nMinTrackingWidth;
    int m_nMinTrackingHeight;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
