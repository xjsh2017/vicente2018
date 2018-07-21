// ***************************************************************
//  LogView   version:  1.0    date: 2:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 日志输出类定义
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_MSGVIEW_H__9B65BF42_063A_42FF_B0FB_C9B652A6AAE0__INCLUDED_)
#define AFX_MSGVIEW_H__9B65BF42_063A_42FF_B0FB_C9B652A6AAE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgView view

//显示特殊交互信息
//##ModelId=49B87B9901C5
class CMsgView : public CEditView
{
protected:
 	DECLARE_DYNCREATE(CMsgView)

// Construction
public:
	//##ModelId=49B87B9901C7
	CMsgView();
	//##ModelId=49B87B9901C8
	virtual ~CMsgView();

	virtual void OnInitialUpdate();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgView)
	public:
	protected:
	//##ModelId=49B87B9901CA
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
private:
	//##ModelId=49B87B9901D6
	int m_nMaxLineCount;	
	
	/** @brief         定时器  */
	int	m_nTimer;

public:
	//##ModelId=49B87B9901D7
	void SetMaxLineCount(int nValue);
	//##ModelId=49B87B9901D9
	void DoClear();
	//##ModelId=49B87B9901DA
	void DoCopy();
	//##ModelId=49B87B9901DB
	void AppendString(const CString& strOut);
#ifdef _DEBUG
	//##ModelId=49B87B9901DD
	virtual void AssertValid() const;
	//##ModelId=49B87B9901E4
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgView)
	afx_msg void OnChange();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnLogClearAll();
	afx_msg void OnLogSelectall();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGVIEW_H__9B65BF42_063A_42FF_B0FB_C9B652A6AAE0__INCLUDED_)
