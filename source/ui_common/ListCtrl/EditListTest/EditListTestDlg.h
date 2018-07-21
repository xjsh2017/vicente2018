// EditListTestDlg.h : header file
//

#if !defined(AFX_EDITLISTTESTDLG_H__4B4799A3_BDF2_412D_9579_E09006A9BDD9__INCLUDED_)
#define AFX_EDITLISTTESTDLG_H__4B4799A3_BDF2_412D_9579_E09006A9BDD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEditListTestDlg dialog

class CEditListTestDlg : public CDialog
{
// Construction
public:
	CEditListTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEditListTestDlg)
	enum { IDD = IDD_EDITLISTTEST_DIALOG };
	CListCtrlEx	m_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEditListTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCheckAll();
	afx_msg void OnCheckNull();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLISTTESTDLG_H__4B4799A3_BDF2_412D_9579_E09006A9BDD9__INCLUDED_)
