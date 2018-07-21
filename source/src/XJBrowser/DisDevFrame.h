#if !defined(AFX_DISDEVFRAME_H__C583A17D_A975_4693_8AF6_D7310F553B39__INCLUDED_)
#define AFX_DISDEVFRAME_H__C583A17D_A975_4693_8AF6_D7310F553B39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisDevFrame.h : header file
//
#include "PTSheet.h"
#include "PTGeneral.h"
#include "PTDistance.h"
#include "PTAlarm.h"

/////////////////////////////////////////////////////////////////////////////
// CDisDevFrame frame

class CDisDevFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDisDevFrame)
public:
	CDisDevFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	/** @brief           SHEET“≥√Ê÷∏’Î*/
	CPTSheet *		m_pTabView;
	CPTGeneral*	m_pGeneralPage;
	CPTDistance*	m_pDistancePage;
	CPTAlarm*	m_pAlarmPage;

	CSecObj*			m_pCurSec;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisDevFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDisDevFrame();

	// Generated message map functions
	//{{AFX_MSG(CDisDevFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	void OnOpenDisDevProp(WPARAM wParam, LPARAM lParam);
	afx_msg OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISDEVFRAME_H__C583A17D_A975_4693_8AF6_D7310F553B39__INCLUDED_)
