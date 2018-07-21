#if !defined(AFX_DRAFTFRAME_H__4AFFE3E1_745C_4605_A181_485E5D7C2F48__INCLUDED_)
#define AFX_DRAFTFRAME_H__4AFFE3E1_745C_4605_A181_485E5D7C2F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DraftFrame.h : header file
//
#include "TrueColorToolBar.h"

/////////////////////////////////////////////////////////////////////////////
// CDraftFrame frame

class CDraftFrame : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CDraftFrame)
public:
	CDraftFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CTrueColorToolBar m_wndToolBar;
	CSplitterWnd	m_wndSplitter;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDraftFrame();

	// Generated message map functions
	//{{AFX_MSG(CDraftFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTFRAME_H__4AFFE3E1_745C_4605_A181_485E5D7C2F48__INCLUDED_)
