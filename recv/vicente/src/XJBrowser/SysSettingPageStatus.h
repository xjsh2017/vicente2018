#if !defined(AFX_SYSSETTINGPAGESTATUS_H__6053CCC5_AC69_4C1C_A9AF_0AEE63618470__INCLUDED_)
#define AFX_SYSSETTINGPAGESTATUS_H__6053CCC5_AC69_4C1C_A9AF_0AEE63618470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettingPageStatus.h : header file
//
#include "CtrlButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageStatus dialog

//##ModelId=49B87B8802FC
class CSysSettingPageStatus : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysSettingPageStatus)

// Construction
public:
	//##ModelId=49B87B8802FE
	CSysSettingPageStatus();
	//##ModelId=49B87B8802FF
	~CSysSettingPageStatus();

// Dialog Data
	//{{AFX_DATA(CSysSettingPageStatus)
	enum { IDD = IDD_PAGE_SYS_STATUS };
	//##ModelId=49B87B880301
	CColorButton	m_btnRunON;
	//##ModelId=49B87B88030E
	CColorButton	m_btnRunOFF;
	//##ModelId=49B87B880313
	CColorButton	m_btnRunDebug;
	//##ModelId=49B87B88031D
	CColorButton	m_btnRunCode;
	//##ModelId=49B87B880322
	CColorButton	m_btnRunCheck;
	//##ModelId=49B87B88032D
	CColorButton	m_btnCommON;
	//##ModelId=49B87B880332
	CColorButton	m_btnCommOFF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSysSettingPageStatus)
	protected:
	//##ModelId=49B87B88033C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysSettingPageStatus)
	//##ModelId=49B87B88033F
	afx_msg void OnBtnCommOnColor();
	//##ModelId=49B87B880341
	afx_msg void OnBtnCommOffColor();
	//##ModelId=49B87B88034B
	afx_msg void OnBtnRunOnColor();
	//##ModelId=49B87B88034D
	afx_msg void OnBtnRunCheckColor();
	//##ModelId=49B87B88034F
	afx_msg void OnBtnRunOffColor();
	//##ModelId=49B87B880351
	afx_msg void OnBtnRunDebugColor();
	//##ModelId=49B87B880353
	afx_msg void OnBtnRunCodeColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGPAGESTATUS_H__6053CCC5_AC69_4C1C_A9AF_0AEE63618470__INCLUDED_)
