#if !defined(AFX_PTSETMODPROGVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_)
#define AFX_PTSETMODPROGVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "qbytearray.h"

// PTSetModProgView.h : header file
//

class CPTSetModStateItem;
/////////////////////////////////////////////////////////////////////////////
// CPTSetModProgView view

typedef CTypedPtrArray<CPtrArray, CPTSetModStateItem*>			CPTSetModStateItemList;
class CPTSetModProgView : public CScrollView
{
protected:
	CPTSetModProgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSetModProgView)

// Attributes
private:
	CPTSetModStateItem*		m_pHeadItem;
	CPTSetModStateItemList	m_items;

	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	CWinThread*	m_pThread;

	int m_nTimer;

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

	CPTSetModStateItem* MouseOnObj(CPoint pt);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSetModProgView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPTSetModProgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTSetModProgView)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnAddRemind(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSETMODPROGVIEW_H__02361B75_885E_4CE2_A1E0_10EFDE1F6CC6__INCLUDED_)
