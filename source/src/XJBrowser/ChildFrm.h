// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__DBCE4B14_D794_4756_8B2A_9445C4BA9D3B__INCLUDED_)
#define AFX_CHILDFRM_H__DBCE4B14_D794_4756_8B2A_9445C4BA9D3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PaintCap.h"

//##ModelId=49B87BAB0128
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	//##ModelId=49B87BAB0138
	CChildFrame();

// Attributes
public:
	//##ModelId=49B87BAB0139
	BOOL m_bMaximized;
	//##ModelId=49B87BAB013A
	int  m_nType;
//	CCaptionPainter m_capp;	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87BAB0141
	virtual ~CChildFrame();
#ifdef _DEBUG
	//##ModelId=49B87BAB0143
	virtual void AssertValid() const;
	//##ModelId=49B87BAB0148
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	//##ModelId=49B87BAB014B
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BAB014F
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BAB0153
	afx_msg void OnSTTP20010(WPARAM wParam , LPARAM lParam);
	afx_msg void OnSTTP20144(WPARAM wParam , LPARAM lParam);
	//##ModelId=49B87BAB0158
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BAB015C
	afx_msg void OnSTTP20132(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BAB0160
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__DBCE4B14_D794_4756_8B2A_9445C4BA9D3B__INCLUDED_)
