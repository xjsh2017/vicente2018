#if !defined(AFX_SYSSETTINGSHEET_H__0D58D863_FD5B_4B0C_BE9D_1C6B684DD0F7__INCLUDED_)
#define AFX_SYSSETTINGSHEET_H__0D58D863_FD5B_4B0C_BE9D_1C6B684DD0F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettingSheet.h : header file
//
#include "SysSettingPageGeneral.h"
#include "SysSettingPageInfoCase.h"
#include "SysSettingPageStatus.h"
#include "SysSettingPagePTProp.h"
#include "CustomStyleSetting.h"
#include "PageSysBrowser.h"

/////////////////////////////////////////////////////////////////////////////
// CSysSettingSheet

//##ModelId=49B87B8802D2
class CSysSettingSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSysSettingSheet)

// Construction
public:
	//##ModelId=49B87B8802DE
	CSysSettingSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//##ModelId=49B87B8802E2
	CSysSettingSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:
	/** @brief           系统设置页面*/
	//##ModelId=49B87B8802E7
	CSysSettingPageGeneral	m_PageGeneral;
	/** @brief           信息定制页面*/
	//##ModelId=49B87B8802EC
	CSysSettingPageInfoCase m_PageInfoCase;
	/** @brief           设备状态显示页面*/
	//##ModelId=49B87B8802F1
	CSysSettingPageStatus	m_PageStatus;
	/** @brief           保护属性显示页面*/
	//##ModelId=49B87B8802F6
	CSysSettingPagePTProp	m_PagePTProp;
	/** @brief           自定义风格设置页面*/
	//##ModelId=49B87B8802F6
	CCustomStyleSetting	   m_PageStyle;
	/** @brief           信息浏览页面*/
	CSysSettingPageBrowser	m_PageBrowser;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSettingSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B8802FA
	virtual ~CSysSettingSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSysSettingSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGSHEET_H__0D58D863_FD5B_4B0C_BE9D_1C6B684DD0F7__INCLUDED_)
