#if !defined(AFX_DLGPTNOTE_H__F018673C_BF33_4C33_835C_42E88078CC98__INCLUDED_)
#define AFX_DLGPTNOTE_H__F018673C_BF33_4C33_835C_42E88078CC98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPtnote.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPtnote dialog

class CDlgPtnote : public CDialog
{
// Construction
public:
	CDlgPtnote(CWnd* pParent = NULL);   // standard constructor

	PT_NOTE* m_pNote;

// Dialog Data
	//{{AFX_DATA(CDlgPtnote)
	enum { IDD = IDD_DLG_PTNOTE };
	CString	m_sContent;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPtnote)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPtnote)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPTNOTE_H__F018673C_BF33_4C33_835C_42E88078CC98__INCLUDED_)
