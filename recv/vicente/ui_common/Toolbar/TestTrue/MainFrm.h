// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D93EBE53_D029_4A68_AAB9_C09F350AA532__INCLUDED_)
#define AFX_MAINFRM_H__D93EBE53_D029_4A68_AAB9_C09F350AA532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TrueColorToolBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateToolBar(CToolBar* tb, TCHAR* text[]);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CTrueColorToolBar    m_wndToolBar;
	CTrueColorToolBar	m_wndToolBar1;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdate1(CCmdUI* pCmdUI);
	afx_msg void OnUpdate2(CCmdUI* pCmdUI);
	afx_msg void OnUpdate3(CCmdUI* pCmdUI);
	afx_msg void OnUpdate4(CCmdUI* pCmdUI);
	afx_msg void On1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D93EBE53_D029_4A68_AAB9_C09F350AA532__INCLUDED_)
