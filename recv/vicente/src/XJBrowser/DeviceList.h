#if !defined(AFX_DEVICELIST_H__7E51FA2E_C083_49BE_9412_0A08543418B0__INCLUDED_)
#define AFX_DEVICELIST_H__7E51FA2E_C083_49BE_9412_0A08543418B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceList.h : header file
//
#include "BatchSel.h"

/////////////////////////////////////////////////////////////////////////////
// CDeviceList dialog

class CDeviceList : public CDialog
{
// Construction
public:
	CDeviceList(CWnd* pParent = NULL);   // standard constructor
	CDeviceList(CStationObj* pStationObj, BATCHSEL_MAP* pBatchMap, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDeviceList)
	enum { IDD = IDD_DLG_DEVICE_LIST };
	CReportCtrl	m_List;
	//}}AFX_DATA
private:
	CStationObj* m_pStationObj;
	BATCHSEL_MAP*  m_pBatchMap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	void FillDataToList();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceList)
	virtual BOOL OnInitDialog();
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICELIST_H__7E51FA2E_C083_49BE_9412_0A08543418B0__INCLUDED_)
