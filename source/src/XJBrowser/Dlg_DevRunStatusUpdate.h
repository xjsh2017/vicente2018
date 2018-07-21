#if !defined(AFX_DLG_DEVRUNSTATUSUPDATE_H__40F9DE29_6D24_4A0A_846C_B48744CD26CB__INCLUDED_)
#define AFX_DLG_DEVRUNSTATUSUPDATE_H__40F9DE29_6D24_4A0A_846C_B48744CD26CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_DevRunStatusUpdate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_DevRunStatusUpdate dialog

class CDlg_DevRunStatusUpdate : public CDialog
{
// Construction
public:
	CDlg_DevRunStatusUpdate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_DevRunStatusUpdate)
	enum { IDD = IDD_DIALOG_DevRunStatusUpdate };
	CComboBox	m_cbPTRunStatus;
	CButton	m_btOk;
	CButton	m_btExit;
	CString	m_cstrDevName;
	CString	m_cstrTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_DevRunStatusUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg_DevRunStatusUpdate)
	afx_msg void OnBUTTONDevRunStatusOK();
	afx_msg void OnBUTTONDevRunStatusExit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_cstrPTID,m_cstrPTName;
	int m_nStatus;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_DEVRUNSTATUSUPDATE_H__40F9DE29_6D24_4A0A_846C_B48744CD26CB__INCLUDED_)
