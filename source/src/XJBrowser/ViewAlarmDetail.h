// ***************************************************************
//  ViewAlarmDetail   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 告警事件详细内容视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWALARMDETAIL_H__FDF0C0AC_B10E_497D_BA06_6882696A91D1__INCLUDED_)
#define AFX_VIEWALARMDETAIL_H__FDF0C0AC_B10E_497D_BA06_6882696A91D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAlarmDetail.h : header file
//
#include "Event.h"

/////////////////////////////////////////////////////////////////////////////
// CViewAlarmDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B86029F
class CViewAlarmDetail : public CFormView
{
protected:
	//##ModelId=49B87B8602A1
	CViewAlarmDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAlarmDetail)

// Form Data
public:
	//{{AFX_DATA(CViewAlarmDetail)
	enum { IDD = IDD_XJ_VIEW_ALARM_DETAIL };
	//##ModelId=49B87B8602AF
	CString	m_sTime;
	//##ModelId=49B87B8602B0
	CString	m_sDevice;
	//##ModelId=49B87B8602B1
	CString	m_sDetail;
	//}}AFX_DATA

// Attributes
public:
private:
	//##ModelId=49B87B8602B3
	CXJEventAlarm *		m_pEvent;	//页面所对应事件

// Operations
public:
private:
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8602B7
	void RegulateControlSize();

/*
 *  @brief   	RefureDetail	 刷新显示的详细内容 
 *  @return 	void 
 */
	//##ModelId=49B87B8602B8
	void RefureDetail();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAlarmDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8602C4
	virtual ~CViewAlarmDetail();
#ifdef _DEBUG
	//##ModelId=49B87B8602C6
	virtual void AssertValid() const;
	//##ModelId=49B87B8602C8
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAlarmDetail)
	//##ModelId=49B87B8602CB
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	//##ModelId=49B87B8602D0
	afx_msg void OnViewAlarmSelectChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8602D4
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8602D7
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWALARMDETAIL_H__FDF0C0AC_B10E_497D_BA06_6882696A91D1__INCLUDED_)
