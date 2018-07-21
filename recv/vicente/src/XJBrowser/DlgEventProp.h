// ***************************************************************
//  DlgEventProp   version:  1.0    date: 6:10:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 事件属性对话框
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_DLGEVENTPROP_H__54AAEBCE_D5C6_4389_8EE8_D74DA9FB3936__INCLUDED_)
#define AFX_DLGEVENTPROP_H__54AAEBCE_D5C6_4389_8EE8_D74DA9FB3936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEventProp.h : header file
//
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEventProp dialog

/**
 * @defgroup 	CDlgEventProp	事件属性对话框
 * @{
 */
/**
 * @brief       事件属性对话框
*  @author      LYH
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA40271
class CDlgEventProp : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40281
	CDlgEventProp(CWnd* pParent = NULL);   // standard constructor
/*
 *  @brief   	CDlgEventProp	 无模式构造函数 
 *  @param 		[In]a param of Type  CFrameWnd*  调用窗口指针
 *  @return 	 
 */
	//##ModelId=49B87BA40283
	CDlgEventProp(CFrameWnd* pFrame);
	//##ModelId=49B87BA40285
	BOOL Create();

// Dialog Data
	//{{AFX_DATA(CDlgEventProp)
	enum { IDD = IDD_EVENT_PROP };
	/** @brief           事件详细描述*/
	//##ModelId=49B87BA40286
	CString	m_strDescribe;
	/** @brief           事件发生(动作,告警,录波,事件报告...)*/
	//##ModelId=49B87BA40287
	CString	m_strEventClass;
	/** @brief           事件来源[XX厂站][XX设备][XX装置]*/
	//##ModelId=49B87BA40288
	CString	m_strSource;
	/** @brief           发生时间*/
	//##ModelId=49B87BA40290
	CString	m_strTime;
	/** @brief           标签"第几条/共几条"*/
	//##ModelId=49B87BA40291
	CString	m_strRecord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEventProp)
	public:
//	virtual BOOL Create();
	protected:
	//##ModelId=49B87BA40292
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEventProp)
	//##ModelId=49B87BA40295
	afx_msg void OnBtnAck();
	//##ModelId=49B87BA40297
	afx_msg void OnBtnCopy();
	//##ModelId=49B87BA402A0
	afx_msg void OnBtnNext();
	//##ModelId=49B87BA402A2
	afx_msg void OnBtnPrev();
	//##ModelId=49B87BA402A4
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA402A6
	afx_msg void OnClose();
	//##ModelId=49B87BA402A8
	afx_msg void OnDestroy();
	//##ModelId=49B87BA402B0
	virtual void OnCancel();
	//##ModelId=49B87BA402B2
	virtual void OnOK();
	//}}AFX_MSG
	//##ModelId=49B87BA402B4
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA402B8
	afx_msg void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventPassiveAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
/*
 *  @brief   	UpdateControls	 更新事件属性页上控件的内容 
 *  @return 	void 
 */
	//##ModelId=49B87BA402BC
	void UpdateControls();

private:
	/** @brief           事件列表*/
	//##ModelId=49B87BA402C0
	CXJEventManager*	m_pEventManager;

	/** @brief           调用者窗口指针*/
	//##ModelId=49B87BA402C5
	CFrameWnd*	m_pFrame;

	/** @brief           是否保护事件列表打开的对话框,默认为FALSE*/
	//##ModelId=49B87BA402CE
	BOOL  m_bPTEvent;
public:
/*
 *  @brief   	SetEventmanager	 设置事件管理对象 
 *  @param 		[In]a param of Type  CXJEventManager*  事件管理对象指针
 *  @return 	void 
 */
	//##ModelId=49B87BA402CF
	void SetEventmanager(CXJEventManager* pEventManager);

/*
 *  @brief   	SetFlag	 设置是否保护属性页面打开的事件属性对话框 
 *  @param 		[In]a param of Type  BOOL  是否保护属性窗口要打开事件属性对话框
 *  @return 	void 
 */
	//##ModelId=49B87BA402D1
	void SetFlag(BOOL bPT);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEVENTPROP_H__54AAEBCE_D5C6_4389_8EE8_D74DA9FB3936__INCLUDED_)
