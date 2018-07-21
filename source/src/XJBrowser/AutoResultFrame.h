#if !defined(AFX_AUTORESULTFRAME_H__814C4E3B_5096_43D6_B892_87C06A4A4BF7__INCLUDED_)
#define AFX_AUTORESULTFRAME_H__814C4E3B_5096_43D6_B892_87C06A4A4BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoResultFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoResultFrame frame

//##ModelId=49B87BB3038A
class CAutoResultFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAutoResultFrame)
public:
	//##ModelId=49B87BB3038C
	CAutoResultFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BB3038D
	virtual ~CAutoResultFrame();
// Attributes
public:
	/** @brief           ·Ö¸ô´°¿Ú*/
	//##ModelId=49B87BB3039B
	CSplitterWnd	m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoResultFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CAutoResultFrame)
	//##ModelId=49B87BB303A6
	afx_msg void OnClose();
	//}}AFX_MSG
	//##ModelId=49B87BB303AA
	afx_msg void OnAutoCallResultFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB303AE
	afx_msg void OnAutoCallResultChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTORESULTFRAME_H__814C4E3B_5096_43D6_B892_87C06A4A4BF7__INCLUDED_)
