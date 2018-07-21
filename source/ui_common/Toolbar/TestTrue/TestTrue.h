// TestTrue.h : main header file for the TESTTRUE application
//

#if !defined(AFX_TESTTRUE_H__2B60635A_4B02_48B6_A44B_3F4C4A9A8819__INCLUDED_)
#define AFX_TESTTRUE_H__2B60635A_4B02_48B6_A44B_3F4C4A9A8819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestTrueApp:
// See TestTrue.cpp for the implementation of this class
//

class CTestTrueApp : public CWinApp
{
public:
	CTestTrueApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestTrueApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestTrueApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTTRUE_H__2B60635A_4B02_48B6_A44B_3F4C4A9A8819__INCLUDED_)
