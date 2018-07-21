#if !defined(AFX_DLGSTATIONSVG_H__7720DB4D_08EA_4C10_BE00_2F6EBE8EE07E__INCLUDED_)
#define AFX_DLGSTATIONSVG_H__7720DB4D_08EA_4C10_BE00_2F6EBE8EE07E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStationSvg.h : header file
//
#include "DeviceObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgStationSvg dialog

//##ModelId=49B87BA3036F
class CDlgStationSvg : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA3037A
	CDlgStationSvg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStationSvg)
	enum { IDD = IDD_DLG_SELECTSTATION_SVGPATH };
	//##ModelId=49B87BA3037C
	CString	m_sPath;
	//}}AFX_DATA

	//##ModelId=49B87BA3037E
	/** @brief           对应厂站指针*/
	CStationObj*	m_pStation;
	/** @brief           对应间隔指针*/
	CBayObj*		m_pBay;
	/** @brief           打开类型, 1-设置厂站图形. 2-设置间隔图形*/
	int	m_nType;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStationSvg)
	protected:
	//##ModelId=49B87BA30382
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStationSvg)
	//##ModelId=49B87BA30385
	afx_msg void OnBtnStationsvgAdd();
	//##ModelId=49B87BA3038A
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA3038C
	afx_msg void OnBtnStationsvgDelete();
	//##ModelId=49B87BA3038E
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATIONSVG_H__7720DB4D_08EA_4C10_BE00_2F6EBE8EE07E__INCLUDED_)
