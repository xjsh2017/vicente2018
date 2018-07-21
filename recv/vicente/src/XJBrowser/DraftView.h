#if !defined(AFX_DRAFTVIEW_H__A69AE0EC_043F_485A_B72E_07BB44D11434__INCLUDED_)
#define AFX_DRAFTVIEW_H__A69AE0EC_043F_485A_B72E_07BB44D11434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DraftView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDraftView view

class CDraftView : public CScrollView
{
public:
	CDraftView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDraftView)

// Attributes
private:
	CBitmap m_Bitmap;
	int m_nImageWidth;
	int m_nImageHeight;
	float m_fRate;

	void DoFitWindow();

	void OpenSelectedDraft();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDraftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDraftView)
	afx_msg void OnDraftZoomout();
	afx_msg void OnUpdateDraftZoomout(CCmdUI* pCmdUI);
	afx_msg void OnDraftZoomin();
	afx_msg void OnUpdateDraftZoomin(CCmdUI* pCmdUI);
	afx_msg void OnDraftWindow();
	afx_msg void OnUpdateDraftWindow(CCmdUI* pCmdUI);
	afx_msg void OnDraftReal();
	afx_msg void OnUpdateDraftReal(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTVIEW_H__A69AE0EC_043F_485A_B72E_07BB44D11434__INCLUDED_)
