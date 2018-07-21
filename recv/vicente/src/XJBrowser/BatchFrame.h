#if !defined(AFX_BATCHFRAME_H__D7BBCFDB_4093_41DC_A244_833798006A09__INCLUDED_)
#define AFX_BATCHFRAME_H__D7BBCFDB_4093_41DC_A244_833798006A09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchFrame.h : header file
//

class CMySplitter:public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitter)
public:
	DECLARE_MESSAGE_MAP()
protected:
	
	//重载进行非客户区鼠标测试函数
	afx_msg UINT OnNcHitTest(CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
// CBatchFrame frame

//##ModelId=49B87BB10177
class CBatchFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBatchFrame)
public:
	//##ModelId=49B87BB10179
	CBatchFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BB1017A
	virtual ~CBatchFrame();
// Attributes
public:
	/** @brief           分隔窗口*/
	//##ModelId=49B87BB10187
	CMySplitter	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87BB1018B
	BOOL			m_bMaximized;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchFrame)
	public:
	//##ModelId=49B87BB1018C
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	//##ModelId=49B87BB1018F
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//##ModelId=49B87BB10192
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBatchFrame)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	//##ModelId=49B87BB1019D
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101A5
	afx_msg void OnBatchCheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101A9
	afx_msg void OnBatchOption(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101AD
	afx_msg void OnBatchDetailUncheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101B6
	afx_msg void OnBatchUnselectAll(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101BA
	afx_msg void OnBatchSelectAllPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101BE
	afx_msg void OnBatchSelectAllWR(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101C8
	afx_msg void OnBatchEnableControls(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB101CC
	afx_msg void OnSTTP20154(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBatchCallEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHFRAME_H__D7BBCFDB_4093_41DC_A244_833798006A09__INCLUDED_)
