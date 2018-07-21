#if !defined(AFX_CROSSFRAME_H__890B009B_BE01_4DED_8F6D_DE04CF639ED8__INCLUDED_)
#define AFX_CROSSFRAME_H__890B009B_BE01_4DED_8F6D_DE04CF639ED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrossFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrossFrame frame

class CCrossFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCrossFrame)
public:
	CCrossFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	/** @brief           ·Ö¸ô´°¿Ú*/
	//##ModelId=49B87BB3039B
	CSplitterWnd	m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrossFrame)
	public:
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCrossFrame();

	// Generated message map functions
	//{{AFX_MSG(CCrossFrame)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	afx_msg void OnViewCrossSelChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20159(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSFRAME_H__890B009B_BE01_4DED_8F6D_DE04CF639ED8__INCLUDED_)
