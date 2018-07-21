// ***************************************************************
//  FaultFrame   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 事故报告子窗口框架
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_FAULTFRAME_H__C9AB449D_A919_4E39_B097_9A23EAFE52AF__INCLUDED_)
#define AFX_FAULTFRAME_H__C9AB449D_A919_4E39_B097_9A23EAFE52AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaultFrame.h : header file
//
#include "ViewFaultSheet.h"
#include "ViewFaultDetail.h"
#include "ViewFault.h"
#include "ViewFaultQuery.h"

/////////////////////////////////////////////////////////////////////////////
// CFaultFrame frame

//##ModelId=49B87B9A0177
class CFaultFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFaultFrame)
public:
	//##ModelId=49B87B9A0179
	CFaultFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	//##ModelId=49B87B9A017B
	CViewFaultSheet * m_pTabView;
private:
	//##ModelId=49B87B9A0187
	CSplitterWnd	  m_wndSplitter;
	//##ModelId=49B87B9A018B
	BOOL			  m_bMaximized;
	/** @brief           记录故障事件列表指针*/
	//##ModelId=49B87B9A018D
	CViewFault*		  m_pFault;
	CViewFaultQuery*	m_pFaultQuery;

	BOOL m_bCreateSplitterWnd;

// Operations
public:
/*
 *  @brief   	GetFaultDetailView	 得到故障报告详细视图指针 
 *  @return 	CViewFaultDetail* 故障报告详细视图指针
 */
	//##ModelId=49B87B9A0191
	CViewFaultDetail* GetFaultDetailView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaultFrame)
	public:
	//##ModelId=49B87B9A0192
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	//##ModelId=49B87B9A0198
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//##ModelId=49B87B9A019C
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B9A019F
	virtual ~CFaultFrame();

	// Generated message map functions
	//{{AFX_MSG(CFaultFrame)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	//##ModelId=49B87B9A01A6
	afx_msg void OnSelChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9A01AA
	afx_msg void OnPrintFaultReport(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9A01AE
	afx_msg void OnRefreshFaultDetail(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9A01B5
	afx_msg void OnSTTP20151(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportExcelFaultReport(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		friend class CViewFaultQuery;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTFRAME_H__C9AB449D_A919_4E39_B097_9A23EAFE52AF__INCLUDED_)
