#if !defined(AFX_DLGVALIDATEID_H__33425A9D_7CFA_4BB8_B1FF_6CC092F33521__INCLUDED_)
#define AFX_DLGVALIDATEID_H__33425A9D_7CFA_4BB8_B1FF_6CC092F33521__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgValidateID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgValidateID dialog

//##ModelId=49B87BA30352
class CDlgValidateID : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA3035C
	CDlgValidateID(CWnd* pParent = NULL);   // standard constructor
	CDlgValidateID(int nPersonType, int nValidateType = -1, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgValidateID)
	enum { IDD = IDD_VALIDATE_ID };
	//##ModelId=49B87BA3035E
	CString	m_strPassword;
	//##ModelId=49B87BA3035F
	CString	m_strUser;
	//}}AFX_DATA
	/** @brief           如果用户名是正确的,保存用户组*/
	//##ModelId=49B87BA30360
	CString m_strUserGroup;
	/** @brief           功能ID*/
	//##ModelId=49B87BA30361
	CString m_strFuncID;
	int      m_nPersonType;
	int      m_nValidateType;
	//用于比较的用户ID
	CString m_strComUserID;

	void FillUserName();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgValidateID)
	protected:
	//##ModelId=49B87BA30362
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgValidateID)
	//##ModelId=49B87BA3036B
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA3036D
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVALIDATEID_H__33425A9D_7CFA_4BB8_B1FF_6CC092F33521__INCLUDED_)
