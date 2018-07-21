#if !defined(AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_)
#define AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

int const	LISTCTRLEX_MARK = 40022; //识别码
int const	ITEM_TITLE		= 40020;	//题头行
int const	ITEM_SEPARATOR	= 40021; //分隔
int const	ITEM_LISTCTRLEX	= 40000;
int const	ITEM_1 = ITEM_LISTCTRLEX + 1;
int const	ITEM_2 = ITEM_LISTCTRLEX + 2;
int const	ITEM_3 = ITEM_LISTCTRLEX + 3;
int const	ITEM_4 = ITEM_LISTCTRLEX + 4;
int const	ITEM_5 = ITEM_LISTCTRLEX + 5;
int const	ITEM_6 = ITEM_LISTCTRLEX + 6;
int const	ITEM_7 = ITEM_LISTCTRLEX + 7;
int const	ITEM_8 = ITEM_LISTCTRLEX + 8;
int const	ITEM_9 = ITEM_LISTCTRLEX + 9;
int const	ITEM_10 = ITEM_LISTCTRLEX + 10;
int const	ITEM_11 = ITEM_LISTCTRLEX + 11;
int const	ITEM_12 = ITEM_LISTCTRLEX + 12;
int const	ITEM_13 = ITEM_LISTCTRLEX + 13;
int const	ITEM_14 = ITEM_LISTCTRLEX + 14;
int const	ITEM_15 = ITEM_LISTCTRLEX + 15;
int const	ITEM_16 = ITEM_LISTCTRLEX + 16;
int const	ITEM_17 = ITEM_LISTCTRLEX + 17;
int const	ITEM_18 = ITEM_LISTCTRLEX + 18;
int const	ITEM_19 = ITEM_LISTCTRLEX + 19;

#include <afxtempl.h>
#include "HideHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window
class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:
	CHideHeaderCtrl	m_hideHeader;
	CMenu * pMenu;

// Operations
public:
    int HitTestEx(CPoint& Point, int& nSubItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearEdited();
	void SetListType(int nListType);
	CArray<CPoint,CPoint&> m_arrEdited;
	CListBox* ListSubItem(int nItem, int nSubItem, CString strPickList);
	CEdit* EditSubItem(int nItem, int nSubItem);

	int SetColumnHide(int nCol, BOOL bHide);
	BOOL GetColumnIsHide(int nCol);
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );
	int InsertColumn(int nCol, const LVCOLUMN *pColumn);
	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, BOOL bHide, int nFormat, int nWidth, int nSubItem );
	int InsertColumn(int nCol, const LVCOLUMN *pColumn, BOOL bHide);
	BOOL DeleteColumn( int nCol );
	
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
//	BOOL m_bIsSetting;
	int  m_nListType;//框的类型
	CString m_strOldText;
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCommandItem(UINT nID);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__E29BCEF5_3FCB_4A2E_8EFF_4C6DCCD6485C__INCLUDED_)
