// ***************************************************************
//  AlarmFrame   version:  1.0    date: 23:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 告警事件窗口框架
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_ALARMFRAME_H__AE133A25_4CD4_4AB4_AE86_E0BCE9D36E5E__INCLUDED_)
#define AFX_ALARMFRAME_H__AE133A25_4CD4_4AB4_AE86_E0BCE9D36E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmFrame frame

/**
 * @defgroup 	CAlarmFrame	告警事件窗口框架
 * @{
 */
/**
 * @brief       告警事件窗口框架
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BB50138
class CAlarmFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAlarmFrame)
public:
	//##ModelId=49B87BB5013A
	CAlarmFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BB5013B
	virtual ~CAlarmFrame();
// Attributes
public:
protected:
	/** @brief           分隔窗口*/
	//##ModelId=49B87BB5013E
	CSplitterWnd	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87BB50148
	BOOL			m_bMaximized;
	/** @brief           之前的窗口高度*/
	int				m_nOldHeight;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmFrame)
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
	//{{AFX_MSG(CAlarmFrame)
	//##ModelId=49B87BB50159
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
/*
 *  @brief   	OnViewAlarmSelectChanged	 响应列表选择更改消息 
 *  @param 		[In]a param of Type  WPARAM  消息参数, 此参数不用
 *  @param 		[In]a param of Type  LPARAM  消息参数, 记录被选择的事件
 *  @return 	void 
 */
	//##ModelId=49B87BB5015C
	afx_msg void OnViewAlarmSelectChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50167
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5016B
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5016F
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50178
	afx_msg void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5017C
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50186
	afx_msg void OnRefreshAlarmView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMFRAME_H__AE133A25_4CD4_4AB4_AE86_E0BCE9D36E5E__INCLUDED_)
