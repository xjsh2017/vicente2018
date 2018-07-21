#if !defined(AFX_DLGCHANNELSTATUS_H__6912F799_1C55_4D35_8D4C_E856CE82D5E6__INCLUDED_)
#define AFX_DLGCHANNELSTATUS_H__6912F799_1C55_4D35_8D4C_E856CE82D5E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChannelStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelStatus dialog

class CDlgChannelStatus : public CDialog
{
// Construction
public:
	CDlgChannelStatus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChannelStatus)
	enum { IDD = IDD_DLG_CHANNEL };
	CListCtrlEx	m_List;
	CString	m_sStation;
	CString	m_sStatus;
	CString	m_sTime;
	CString	m_sTime2;
	CString	m_sIP;
	CString	m_sChannel;
	//}}AFX_DATA
	CStationObj* m_pStation;
	int	m_nChannel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChannelStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	/*
 *  @brief   	InitListStyle	 初始化列表风格 
 *  @return 	void 
 */
	void InitListStyle();

	void AddDataToList(int nIndex, CString sTime, CString sStatus, CString sReason);

	void LoadData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChannelStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANNELSTATUS_H__6912F799_1C55_4D35_8D4C_E856CE82D5E6__INCLUDED_)
