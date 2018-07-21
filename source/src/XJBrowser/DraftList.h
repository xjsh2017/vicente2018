#if !defined(AFX_DRAFTLIST_H__B35392B3_165C_4C9D_B122_D0933CEA8024__INCLUDED_)
#define AFX_DRAFTLIST_H__B35392B3_165C_4C9D_B122_D0933CEA8024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DraftList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDraftList view

class CDraftList : public CListView
{
protected:
	CDraftList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDraftList)

// Attributes
public:
private:
	int m_nSelected;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDraftList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDraftList)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTLIST_H__B35392B3_165C_4C9D_B122_D0933CEA8024__INCLUDED_)
