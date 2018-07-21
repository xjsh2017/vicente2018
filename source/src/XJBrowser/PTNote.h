#if !defined(AFX_PTNOTE_H__3C9D8706_6E3B_4E8D_9AA7_E093461EC6ED__INCLUDED_)
#define AFX_PTNOTE_H__3C9D8706_6E3B_4E8D_9AA7_E093461EC6ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTNote.h : header file
//
#include "ViewBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPTNote form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPTNote : public CViewBase
{
protected:
	CPTNote();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTNote)

// Form Data
public:
	//{{AFX_DATA(CPTNote)
	enum { IDD = IDD_PT_NOTE };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
private:
	CSecObj*	m_pObj;	//对应保护

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTNote)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	int InitListStyle();
	void ReFillAll();

	BOOL LoadPTNoteDate();
	void AddNoteToList(PT_NOTE* note, int row);
	void UpdateNoteInList(PT_NOTE* note, int row);

	void ClearNoteData();

	BOOL UpdateNoteInDB(PT_NOTE* note);
	BOOL AddNoteToDB(PT_NOTE* note);
	BOOL DeleteNoteInDB(PT_NOTE* note);

// Implementation
protected:
	virtual ~CPTNote();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTNote)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListNotes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnNoteAdd();
	//}}AFX_MSG
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTNOTE_H__3C9D8706_6E3B_4E8D_9AA7_E093461EC6ED__INCLUDED_)
