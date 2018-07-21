#if !defined(AFX_REMINDVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_)
#define AFX_REMINDVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemindView.h : header file
//
#include "RemindObj.h"

/////////////////////////////////////////////////////////////////////////////
// CRemindView view

class CRemindView : public CScrollView
{
protected:
	CRemindView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRemindView)

// Attributes
private:
	CRemindObj*		m_pAction;
	CRemindObj*		m_pAlarm;
	CRemindObj*		m_pOsc;
	CRemindObj*		m_pFault;

	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	CWinThread*	m_pThread;
public:
	/** @brief			线程退出标志*/
	BOOL m_bThreadExit;
	

public:
	void UpdateAllObj();
	void EndThread();
// Operations
private:
	void StartThread();

	void ResetObjSize();
	

	/*
 *  @brief   	DocToClient	 逻辑坐标转设备坐标 
 *  @param 		[In]a param of Type  CPoint &  要转换的坐标
 *  @return 	void 
 */
	//##ModelId=49B87B8502E6
	void DocToClient(CPoint & pt);
/*
 *  @brief   	DocToClient	 逻辑坐标转设备坐标 
 *  @param 		[In]a param of Type  CRect &  要转换的范围
 *  @return 	void 
 */
	//##ModelId=49B87B8502EE
	void DocToClient(CRect & rect);
/*
 *  @brief   	ClientToDoc	 设备坐标转逻辑坐标 
 *  @param 		[In]a param of Type  CPoint &  要转换的坐标
 *  @return 	void 
 */
	//##ModelId=49B87B8502F0
	void ClientToDoc(CPoint & pt);
/*
 *  @brief   	ClientToDoc	 设备坐标转逻辑坐标 
 *  @param 		[In/Out]a param of Type  CRect &  要转换的范围
 *  @return 	void 
 */
	//##ModelId=49B87B8502F2
	void ClientToDoc(CRect & rect);

	void UpdateActive(CPoint pt);

	CRemindObj* MouseOnObj(CPoint pt);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemindView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRemindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CRemindView)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnAddRemind(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMINDVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_)
