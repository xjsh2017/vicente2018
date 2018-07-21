#if !defined(AFX_DLGSETTINGDETAIL_H__9094EE78_24E6_4618_B49D_5A02FD41B862__INCLUDED_)
#define AFX_DLGSETTINGDETAIL_H__9094EE78_24E6_4618_B49D_5A02FD41B862__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSettingDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingDetail dialog

class CDlgSettingDetail : public CDialog
{
// Construction
public:
	PT_SETTING *m_pts;
	CDlgSettingDetail(CWnd* pParent = NULL);   // standard constructor

	CStringArray m_arrName;
	CStringArray m_arrValue;
// Dialog Data
	//{{AFX_DATA(CDlgSettingDetail)
	enum { IDD = IDD_DLG_SETTING_DETAIL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSettingDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetTranslatedValue(PT_SETTING *pst,const CString strValue);

	CString TranslateName(CString sName);
	BOOL TranslateValue(CString sValue);
	
	CString HexToBin(CString strValue);
	
	CListCtrl m_ListCtrl;
	void InitColumns();

	// Generated message map functions
	//{{AFX_MSG(CDlgSettingDetail)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGDETAIL_H__9094EE78_24E6_4618_B49D_5A02FD41B862__INCLUDED_)
