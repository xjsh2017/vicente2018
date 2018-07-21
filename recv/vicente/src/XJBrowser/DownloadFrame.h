#if !defined(AFX_DOWNLOADFRAME_H__DC065A41_E313_419E_822B_6F20A9223B21__INCLUDED_)
#define AFX_DOWNLOADFRAME_H__DC065A41_E313_419E_822B_6F20A9223B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownloadFrame frame

//##ModelId=49B87BA3031F
class CDownloadFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDownloadFrame)
public:
	//##ModelId=49B87BA3032D
	CDownloadFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BA3032E
	virtual ~CDownloadFrame();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CDownloadFrame)
	//##ModelId=49B87BA30333
	afx_msg void OnClose();
	//}}AFX_MSG
	//##ModelId=49B87BA30335
	afx_msg void OnDownloadOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA3033D
	afx_msg void OnSTTP00204(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30341
	afx_msg void OnSTTP00212(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30345
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30349
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA3034E
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADFRAME_H__DC065A41_E313_419E_822B_6F20A9223B21__INCLUDED_)
