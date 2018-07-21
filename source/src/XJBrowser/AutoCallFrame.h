#if !defined(AFX_AUTOCALLFRAME_H__278740DE_AB93_4538_B0D1_183F4DCBD9D0__INCLUDED_)
#define AFX_AUTOCALLFRAME_H__278740DE_AB93_4538_B0D1_183F4DCBD9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCallFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoCallFrame frame

//##ModelId=49B87BB500CB
class CAutoCallFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAutoCallFrame)
public:
	//##ModelId=49B87BB500DA
	CAutoCallFrame();           // protected constructor used by dynamic creation
	//##ModelId=49B87BB500DB
	~CAutoCallFrame();
// Attributes
public:
	/** @brief           分隔窗口*/
	//##ModelId=49B87BB500DD
	CSplitterWnd	m_wndSplitter;
	/** @brief           是否已经最大化显示*/
	//##ModelId=49B87BB500E1
	BOOL			m_bMaximized;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCallFrame)
	public:
	//##ModelId=49B87BB500E2
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	//##ModelId=49B87BB500EB
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//##ModelId=49B87BB500EE
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CAutoCallFrame)
	//##ModelId=49B87BB500F2
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=49B87BB500FB
	afx_msg void OnClose();
	//}}AFX_MSG
	//##ModelId=49B87BB500FD
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50101
	afx_msg void OnBatchCheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5010A
	afx_msg void OnBatchOption(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5010E
	afx_msg void OnBatchDetailUncheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50112
	afx_msg void OnBatchUnselectAll(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB5011C
	afx_msg void OnBatchSelectAllPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50120
	afx_msg void OnBatchSelectAllWR(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB50129
	afx_msg void OnBatchEnableControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAutoCallStationChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnShowList( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBatchCallEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCALLFRAME_H__278740DE_AB93_4538_B0D1_183F4DCBD9D0__INCLUDED_)
