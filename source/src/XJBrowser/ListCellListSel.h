#if !defined(AFX_LISTCELLLISTSEL_H__D3FBFC18_B3DA_424A_91F2_F496B4D2F343__INCLUDED_)
#define AFX_LISTCELLLISTSEL_H__D3FBFC18_B3DA_424A_91F2_F496B4D2F343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCellListSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCellListSel window

class CListCellListSel : public CListBox
{
// Construction
public:
	CListCellListSel(int nItem, int nSubItem, CString strInitText);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellListSel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetListItemText();
	virtual ~CListCellListSel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCellListSel)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
protected:
	int			m_nItem;
    int			m_nSubItem;
    CString		m_strInitText;
    BOOL		m_bEscape;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCELLLISTSEL_H__D3FBFC18_B3DA_424A_91F2_F496B4D2F343__INCLUDED_)
