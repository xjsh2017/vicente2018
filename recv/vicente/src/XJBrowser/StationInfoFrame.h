#if !defined(AFX_STATIONINFOFRAME_H__D4DDA83A_8EF6_4E32_B5FF_0CC5021560AF__INCLUDED_)
#define AFX_STATIONINFOFRAME_H__D4DDA83A_8EF6_4E32_B5FF_0CC5021560AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StationInfoFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStationInfoFrame frame

class CStationInfoFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CStationInfoFrame)
public:
	CStationInfoFrame();           // protected constructor used by dynamic creation
	virtual ~CStationInfoFrame();
// Attributes
public:
	BOOL m_bMaximized;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStationInfoFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CStationInfoFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnStationInfoOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInfoClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATIONINFOFRAME_H__D4DDA83A_8EF6_4E32_B5FF_0CC5021560AF__INCLUDED_)
