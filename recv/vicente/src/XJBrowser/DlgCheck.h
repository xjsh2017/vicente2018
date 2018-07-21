#if !defined(AFX_DLGCHECK_H__48F5010D_E549_4A81_82D4_5621C482EE91__INCLUDED_)
#define AFX_DLGCHECK_H__48F5010D_E549_4A81_82D4_5621C482EE91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCheck dialog

//##ModelId=49B87BA402DF
class CDlgCheck : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA402E1
	CDlgCheck(CWnd* pParent = NULL, int nType = 1);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCheck)
	enum { IDD = IDD_CHECK };
	//##ModelId=49B87BA402E3
	CString	m_strModify;
	//}}AFX_DATA
	int	m_nType;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCheck)
	protected:
	//##ModelId=49B87BA402E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCheck)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHECK_H__48F5010D_E549_4A81_82D4_5621C482EE91__INCLUDED_)
