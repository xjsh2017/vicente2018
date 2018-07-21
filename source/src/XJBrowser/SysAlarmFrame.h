#if !defined(AFX_SYSALARMFRAME_H__94A7BF2D_2867_446B_9A5C_A49A8B3D4044__INCLUDED_)
#define AFX_SYSALARMFRAME_H__94A7BF2D_2867_446B_9A5C_A49A8B3D4044__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysAlarmFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysAlarmFrame frame

//##ModelId=49B87B89001F
class CSysAlarmFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSysAlarmFrame)
public:
	//##ModelId=49B87B890021
	CSysAlarmFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87B890022
	virtual ~CSysAlarmFrame();
// Attributes
public:
	/** @brief           分隔窗口*/
	//##ModelId=49B87B890025
	CSplitterWnd	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87B890029
	BOOL			m_bMaximized;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysAlarmFrame)
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
	//{{AFX_MSG(CSysAlarmFrame)
	//##ModelId=49B87B890038
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	//##ModelId=49B87B890040
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B890044
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B890048
	afx_msg void OnListSelChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B89004F
	afx_msg void OnSTTP20155(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20138(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20176(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSALARMFRAME_H__94A7BF2D_2867_446B_9A5C_A49A8B3D4044__INCLUDED_)
