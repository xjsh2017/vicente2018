#if !defined(AFX_DLGWORK_H__6E9FC6B8_5E9E_4F36_A6E8_88A1C10E18A3__INCLUDED_)
#define AFX_DLGWORK_H__6E9FC6B8_5E9E_4F36_A6E8_88A1C10E18A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWork dialog

class CDlgWork : public CDialog
{
// Construction
public:
	CDlgWork(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWork)
	enum { IDD = IDD_DLG_WORK };
	CString	m_strWork;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CString GetFilePath();
	void ReadWorkFile(CString strFilePath);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWork)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWORK_H__6E9FC6B8_5E9E_4F36_A6E8_88A1C10E18A3__INCLUDED_)
