// btndemo.h : main header file for the BTNDEMO application
//

#if !defined(AFX_BTNDEMO_H__BF4D457D_7809_4EAC_8D17_02CC1FE95B1E__INCLUDED_)
#define AFX_BTNDEMO_H__BF4D457D_7809_4EAC_8D17_02CC1FE95B1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBtndemoApp:
// See btndemo.cpp for the implementation of this class
//

class CBtndemoApp : public CWinApp
{
public:
	CBtndemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtndemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBtndemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTNDEMO_H__BF4D457D_7809_4EAC_8D17_02CC1FE95B1E__INCLUDED_)
