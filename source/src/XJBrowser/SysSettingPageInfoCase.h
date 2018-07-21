#if !defined(AFX_SYSSETTINGPAGEINFOCASE_H__68685E5C_B3E1_42F1_8444_9EA4D646382F__INCLUDED_)
#define AFX_SYSSETTINGPAGEINFOCASE_H__68685E5C_B3E1_42F1_8444_9EA4D646382F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettingPageInfoCase.h : header file
//

#include "CtrlButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageInfoCase dialog

//##ModelId=49B87B88038D
class CSysSettingPageInfoCase : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysSettingPageInfoCase)

// Construction
public:
	//##ModelId=49B87B88039A
	CSysSettingPageInfoCase();
	//##ModelId=49B87B88039B
	~CSysSettingPageInfoCase();

// Dialog Data
	//{{AFX_DATA(CSysSettingPageInfoCase)
	enum { IDD = IDD_PAGE_SYS_INFOCASE };
	CColorButton	m_btnViewIIIColor;
	CColorButton	m_btnViewIIColor;
	CColorButton	m_btnViewIColor;
	BOOL	m_bBlinkDevice;
	BOOL	m_bBlinkSec;
	BOOL	m_bPopWnd;
	BOOL	m_bTopSec;
	BOOL	m_bUseSound;
	BOOL	m_bShowViewI;
	BOOL	m_bAlarmViewI;
	BOOL	m_bShowViewII;
	BOOL	m_bAlarmViewII;
	BOOL	m_bShowViewIII;
	BOOL	m_bAlarmViewIII;
	int		m_nAlarmTime;
	int		m_nFlashTime;
	BOOL	m_bAutoChange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSysSettingPageInfoCase)
	protected:
	//##ModelId=49B87B8803C9
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysSettingPageInfoCase)
	//##ModelId=49B87B8803CC
	virtual BOOL OnInitDialog();
	//##ModelId=49B87B8803CE
	afx_msg void OnBtnViewiColor();
	//##ModelId=49B87B8803D0
	afx_msg void OnBtnViewiiColor();
	//##ModelId=49B87B8803D8
	afx_msg void OnBtnViewiiiColor();
	//##ModelId=49B87B8803DA
	afx_msg void OnCheckBlinksec();
	//##ModelId=49B87B8803DC
	afx_msg void OnCheckViewi();
	//##ModelId=49B87B8803DE
	afx_msg void OnCheckViewii();
	//##ModelId=49B87B8803E0
	afx_msg void OnCheckViewiii();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGPAGEINFOCASE_H__68685E5C_B3E1_42F1_8444_9EA4D646382F__INCLUDED_)
