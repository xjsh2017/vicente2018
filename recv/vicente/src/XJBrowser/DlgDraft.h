//{{AFX_INCLUDES()
#include "ddvue.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGDRAFT_H__4B899F61_7B0A_431F_967B_8356F2B70A9E__INCLUDED_)
#define AFX_DLGDRAFT_H__4B899F61_7B0A_431F_967B_8356F2B70A9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDraft.h : header file
//
#include "TrueColorToolBar.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgDraft dialog

class CDlgDraft : public CDialog
{
	class DraftFile
	{
	public:
		DraftFile(){};
		~DraftFile(){};
	public:
		CString m_sPath;//全路径
		CString m_sTitle;//文件名，不带后缀
	};

// Construction
public:
	CDlgDraft(CWnd* pParent = NULL);   // standard constructor

private:
	CString m_sStationID;
	int	m_nSelect;
	CString m_sStationName;
	vector<DraftFile>	m_arrFiles;

	CTrueColorToolBar m_wndToolBar;

private:
	void OpenSelectedDraft();
	void FillList();

public:
	BOOL Init( CString sStationID, CString sStationName );
	BOOL SearchDraftFiles( CString path );

// Dialog Data
	//{{AFX_DATA(CDlgDraft)
	enum { IDD = IDD_DLG_DRAFT };
	CListCtrl	m_List;
	CDDVue	m_ddvue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDraft)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDraft)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseDownDdvuectrl1(long lX, long lY, short nButton);
	afx_msg void OnItemchangedListDraftfiles(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDraftZoomin();
	afx_msg void OnDraftZoomout();
	afx_msg void OnDraftWindow();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDRAFT_H__4B899F61_7B0A_431F_967B_8356F2B70A9E__INCLUDED_)
