#if !defined(AFX_SYSSETTINGPAGEGENERAL_H__170C35EE_54B1_4DAC_A89F_CD83BBF033F3__INCLUDED_)
#define AFX_SYSSETTINGPAGEGENERAL_H__170C35EE_54B1_4DAC_A89F_CD83BBF033F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettingPageGeneral.h : header file
//
#include "CtrlButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageGeneral dialog

//##ModelId=49B87B890000
class CSysSettingPageGeneral : public CPropertyPage
{
// Construction
public:
	//##ModelId=49B87B890002
	CSysSettingPageGeneral();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysSettingPageGeneral)
	enum { IDD = IDD_PAGE_SYS_GENERAL };
	CComboBox	m_cmbFontSize;
	CComboBox	m_cmbFont;
	CColorButton	m_btnFontColor;
	CColorButton	m_btnBkColor;
	CComboBox	m_comboStyle;
	BOOL	m_bActionDetail;
	BOOL	m_bDeviceTree;
	BOOL	m_bOutPut;
	BOOL	m_bSysAlarmDetail;
	CString	m_sStyleName;
	int     m_iSel;
	//}}AFX_DATA
	CString	m_sFont;
	int		m_nFontSize;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSettingPageGeneral)
	protected:
	//##ModelId=49B87B890012
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSettingPageGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboStyleChose();
	afx_msg void OnBtnCommmapBk();
	afx_msg void OnBtnCommmapFontcolor();
	afx_msg void OnSelchangeCmbCommmapFont();
	afx_msg void OnSelchangeCmbCommmapFontsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGPAGEGENERAL_H__170C35EE_54B1_4DAC_A89F_CD83BBF033F3__INCLUDED_)
