// btndemoDlg.h : header file
//

#if !defined(AFX_BTNDEMODLG_H__5BE43591_191F_47F2_8779_0E758CE06BF3__INCLUDED_)
#define AFX_BTNDEMODLG_H__5BE43591_191F_47F2_8779_0E758CE06BF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XPStyleButtonST.h"
#include "ThemeHelperST.h"

/////////////////////////////////////////////////////////////////////////////
// CBtndemoDlg dialog

class CBtndemoDlg : public CDialog
{
// Construction
public:
	CBtndemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBtndemoDlg)
	enum { IDD = IDD_BTNDEMO_DIALOG };
	CXPStyleButtonST m_btnStandard;
	CXPStyleButtonST m_btnOption;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBtndemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CThemeHelperST m_ThemeHelper;


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBtndemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnMenu1();
	afx_msg void OnMenu2();
	afx_msg void OnMenu3();
	afx_msg void OnMenu4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTNDEMODLG_H__5BE43591_191F_47F2_8779_0E758CE06BF3__INCLUDED_)
