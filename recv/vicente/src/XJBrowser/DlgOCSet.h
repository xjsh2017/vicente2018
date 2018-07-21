#if !defined(AFX_DLGOCSET_H__91066729_C547_46DD_9551_DE993511A915__INCLUDED_)
#define AFX_DLGOCSET_H__91066729_C547_46DD_9551_DE993511A915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOCSet.h : header file
//
#include "BatchDetailView.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgOCSet dialog

//##ModelId=49B87BA303DC
class CDlgOCSet : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40001
	CDlgOCSet(CWnd* pParent = NULL);   // standard constructor
	void ReadData();
// Dialog Data
	//{{AFX_DATA(CDlgOCSet)
	enum { IDD = IDD_DLG_OCSET };
	//##ModelId=49B87BA40003
	int		m_nCyc;
	CTime   m_tmLastCallTime;
	//}}AFX_DATA
	
	//##ModelId=49B87BA40005
	CViewBase* pView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOCSet)
	protected:
	//##ModelId=49B87BA40009
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOCSet)
	//##ModelId=49B87BA4000F
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA40011
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOCSET_H__91066729_C547_46DD_9551_DE993511A915__INCLUDED_)
