#if !defined(AFX_SOEFRAME_H__9BFFAAE8_8502_46DB_A9D4_1A222F70DE40__INCLUDED_)
#define AFX_SOEFRAME_H__9BFFAAE8_8502_46DB_A9D4_1A222F70DE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoeFrame.h : header file
//
#include "ViewSOE.h"
#include "ViewSOEDetail.h"

/////////////////////////////////////////////////////////////////////////////
// CSoeFrame frame

class CSoeFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSoeFrame)
public:
	CSoeFrame();           // protected constructor used by dynamic creation
	virtual ~CSoeFrame();
// Attributes
public:
	/** @brief           分隔窗口*/
	//##ModelId=49B87BB5019C
	CSplitterWnd	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87BB501A5
	BOOL			m_bMaximized;
	/** @brief           窗口类型。0-I类信息。 1-II类信息。2-III类信息。3-调试信息*/
	int				m_nType;
	/** @brief           详细窗口对应事件*/
	CXJEvent*		m_pEvent;

// Operations
public:
private:
/*
 *  @brief   	RegulateSize	 更改窗口大小 
 *  @return 	void 
 */
	void RegulateSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoeFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoeFrame)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnViewSoeSelectChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewSoeListAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOEFRAME_H__9BFFAAE8_8502_46DB_A9D4_1A222F70DE40__INCLUDED_)
