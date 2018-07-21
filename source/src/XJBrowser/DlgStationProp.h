#if !defined(AFX_DLGSTATIONPROP_H__3CA3D1C1_A03D_4979_B148_DCDF807476BF__INCLUDED_)
#define AFX_DLGSTATIONPROP_H__3CA3D1C1_A03D_4979_B148_DCDF807476BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStationProp.h : header file
//
#include "DeviceObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgStationProp dialog

class CDlgStationProp : public CDialog
{
// Construction
public:
	CDlgStationProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStationProp)
	enum { IDD = IDD_DLG_STATION };
	CString	m_sGetWay;
	CString	m_sIP;
	CString	m_sName;
	CString	m_sNet;
	CString	m_sPort;
	CString	m_sProtocol;
	CString	m_sManufacturer;
	//}}AFX_DATA

	CStationObj* m_pStation;

	CString QueryProtocolName(int nProtocolID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStationProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStationProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATIONPROP_H__3CA3D1C1_A03D_4979_B148_DCDF807476BF__INCLUDED_)
