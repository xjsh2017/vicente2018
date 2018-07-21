// ***************************************************************
//  ActionFrame   version:  1.0    date: 23:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 动作事件窗口框架
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_ACTIONFRAME_H__74F3EC00_9FEE_4F23_B027_4EBBCD82D99E__INCLUDED_)
#define AFX_ACTIONFRAME_H__74F3EC00_9FEE_4F23_B027_4EBBCD82D99E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActionFrame frame

//##ModelId=49B87BB50196
class CActionFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CActionFrame)
public:
	//##ModelId=49B87BB50198
	CActionFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BB50199
	virtual ~CActionFrame();
// Attributes
public:
protected:
	/** @brief           分隔窗口*/
	//##ModelId=49B87BB5019C
	CSplitterWnd	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87BB501A5
	BOOL			m_bMaximized;
	/** @brief           之前的窗口高度*/
	int				m_nOldHeight;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CActionFrame)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	//##ModelId=49B87BB501C7
	afx_msg void OnViewActionSelectChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501CB
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501D5
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501D9
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501E4
	afx_msg void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501E8
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB501EC
	afx_msg void RefreshActionView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONFRAME_H__74F3EC00_9FEE_4F23_B027_4EBBCD82D99E__INCLUDED_)
