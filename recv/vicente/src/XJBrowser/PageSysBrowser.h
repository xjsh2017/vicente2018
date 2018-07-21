#if !defined(AFX_PAGESYSBROWSER_H__E5AACC7F_84AF_4FAD_BCDF_6A1F6F57534C__INCLUDED_)
#define AFX_PAGESYSBROWSER_H__E5AACC7F_84AF_4FAD_BCDF_6A1F6F57534C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSysBrowser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSysBrowser dialog

class CSysSettingPageBrowser : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysSettingPageBrowser)

// Construction
public:
	CSysSettingPageBrowser();
	~CSysSettingPageBrowser();

// Dialog Data
	//{{AFX_DATA(CPageSysBrowser)
	enum { IDD = IDD_PAGE_SYS_BROWSER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSysBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSysBrowser)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnBtnWorkStatus();
	afx_msg void OnBtnWorkAPP();
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESYSBROWSER_H__E5AACC7F_84AF_4FAD_BCDF_6A1F6F57534C__INCLUDED_)
