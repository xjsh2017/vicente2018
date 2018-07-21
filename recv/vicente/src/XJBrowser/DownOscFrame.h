#if !defined(AFX_DOWNOSCFRAME_H__294BAEFE_98BD_45DA_9E3D_B6562A196564__INCLUDED_)
#define AFX_DOWNOSCFRAME_H__294BAEFE_98BD_45DA_9E3D_B6562A196564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownOscFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownOscFrame frame

class CDownOscFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDownOscFrame)
public:
	CDownOscFrame();           // protected constructor used by dynamic creation
	virtual ~CDownOscFrame();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownOscFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownOscFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnDownOscOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNOSCFRAME_H__294BAEFE_98BD_45DA_9E3D_B6562A196564__INCLUDED_)
