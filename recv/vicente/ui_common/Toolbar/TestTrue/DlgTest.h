#if !defined(AFX_DLGTEST_H__07B60E46_A96A_49D6_BFB8_8AE59301C24E__INCLUDED_)
#define AFX_DLGTEST_H__07B60E46_A96A_49D6_BFB8_8AE59301C24E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTest dialog

class CDlgTest : public CDialog
{
// Construction
public:
	CDlgTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTest)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEST_H__07B60E46_A96A_49D6_BFB8_8AE59301C24E__INCLUDED_)
