#if !defined(AFX_DLGEXPORTPROCESS_H__4F4F2062_ACD5_4B39_A0DC_1A116BFFD6C7__INCLUDED_)
#define AFX_DLGEXPORTPROCESS_H__4F4F2062_ACD5_4B39_A0DC_1A116BFFD6C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportProcess.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportProcess dialog

class CDlgExportProcess : public CDialog
{
// Construction
public:
	CDlgExportProcess(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExportProcess)
	enum { IDD = IDD_DLG_EXPORTPROCESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportProcess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportProcess)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	void OnProcessUpdate(WPARAM wParam, LPARAM lParam);
	void OnExportEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTPROCESS_H__4F4F2062_ACD5_4B39_A0DC_1A116BFFD6C7__INCLUDED_)
