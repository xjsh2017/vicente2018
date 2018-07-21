#if !defined(AFX_CUSTOMSTYLESETTING_H__02CBD9EF_D4A6_4445_A37A_9DEAA15A061A__INCLUDED_)
#define AFX_CUSTOMSTYLESETTING_H__02CBD9EF_D4A6_4445_A37A_9DEAA15A061A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
#include "CtrlButton.h"

class StyleSetting
{
public:
	CString m_sName;
	CString m_sToolBarBK;
	CString m_sStartBmp;
	CString m_sListSpaceColor;
	CString m_sListHeadColor;
	CString m_sDeviceTreeBk;
	CString m_sDeviceTreeTextColor;
	CString m_sTitleLightColor;
	CString m_sTitleDeepColor;
};

class  XMLParser
{
public:
	int CheckFileExist(CString fileName);
	int SaveNewStyleSetting(CString fileName);
	int SaveNewStyleSetting(CString fileName, CString& sNodeName);
	int RefreshItemSetting(CString fileName, CString sNodeName, CString sItemName, CString sValue);
	int DeleteStyleSetting(CString fileName, CString sNodeName);
	int ParserXmlFile(CString fileName);
	XMLParser();
	~XMLParser();
	int m_nSettingCount;
	CPtrList  m_list;
};


// _MSC_VER > 1000
// CustomStyleSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomStyleSetting dialog

class CCustomStyleSetting : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomStyleSetting)

// Construction
public:
	CCustomStyleSetting();
	~CCustomStyleSetting();

// Dialog Data
	//{{AFX_DATA(CCustomStyleSetting)
	enum { IDD = IDD_PAGE_SYS_CUSTOM };
	CColorButton	m_btnTreeTextColor;
	CColorButton	m_btnTreeBKColor;
	CColorButton	m_btnTitleLightColor;
	CColorButton	m_btnTitleDeepColor;
	CColorButton	m_btnListSpaceColor;
	CColorButton	m_btnListHeadColor;
	CComboBox	    m_ComboStyle;
	CString	        m_sStartPic;
	CString	        m_sToolBarBK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCustomStyleSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomStyleSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnListHeadColor();
	afx_msg void OnBtnListSpaceColor();
	afx_msg void OnBtnTitleDeepColor();
	afx_msg void OnBtnTitleLightColor();
	afx_msg void OnBtnTreeBkColor();
	afx_msg void OnBtnTreeTextColor();
	afx_msg void OnBtnToolbarBk();
	afx_msg void OnBtnStartPic();
	afx_msg void OnButtonAddStyle();
	afx_msg void OnButtonDeleteStyle();
	afx_msg void OnSelchangeComboStyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	XMLParser   m_xmlParser;
	BOOL m_bOper;
	friend class CXJBrowserApp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMSTYLESETTING_H__02CBD9EF_D4A6_4445_A37A_9DEAA15A061A__INCLUDED_)
