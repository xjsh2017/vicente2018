#ifndef _DLG_VALIDATE_USER_H
#define _DLG_VALIDATE_USER_H

#include "resource.h"

#include "qbytearraymatrix.h"
// DlgValidateUser.h : header file
//

//##ModelId=49B87BA30352
class CDlgValidateUser : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA3035C
	CDlgValidateUser(CWnd* pParent = NULL);   // standard constructor
	CDlgValidateUser(int nUserGroupType, int nValidateType, CWnd* pParent = NULL);
	CDlgValidateUser(int nUserGroupType, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgValidateUser)
	enum { IDD = IDD_VALIDATE_ID };
	//##ModelId=49B87BA3035E
	CString	m_strPassword;
	//##ModelId=49B87BA3035F
	CString	m_strUser;
	//}}AFX_DATA

	int				m_nUserGroupType;
	/** @brief           如果用户名是正确的,保存用户组*/
	//##ModelId=49B87BA30360
	CString m_strUserGroup;
	/** @brief           功能ID*/
	//##ModelId=49B87BA30361
	CString		m_strFuncID;
	CString		m_strAuthUserID;

	int			m_nFuncID;

	int			m_nValidateType;
	QByteArrayMatrix	m_excludeUserList;
	//用于比较的用户ID
	CString m_strComUserID;

	void FillUserName();

	void FillUserName2();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgValidateUser)
	protected:
	//##ModelId=49B87BA30362
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgValidateUser)
	//##ModelId=49B87BA3036B
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA3036D
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_DLG_VALIDATE_USER_H)
