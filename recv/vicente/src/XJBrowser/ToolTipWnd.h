#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ToolTipWnd.h : header file
// add by liuyuhua 20061009

class CToolTipWnd : public CWnd
{

private:
// Construction
	LPCTSTR lpWndCls;
public:
	CToolTipWnd();
	// Attributes
public:
	void ShowToolTip(CPoint point, CStringArray& texts);
	void HideToolTip();

	BOOL Create(CWnd*);

	void SetBkColor(COLORREF clrRef) { m_clrBkColor = clrRef; }
	void SetFrameColor(COLORREF clrRef) { m_clrFrameColor = clrRef; }
	void SetDefTextColor(COLORREF clrRef) { m_clrTextColor = clrRef; }
	void SetFontHeight(int iHeight) { m_iFontHeight = iHeight; }
	void SetFontName(CString strFontName) { m_strFontName = strFontName; }
private:
	CFont m_fontText;
	
	COLORREF m_clrTextColor;
	COLORREF m_clrBkColor;
	COLORREF m_clrFrameColor;

	int m_iFontHeight;
	CString m_strFontName;

	CStringArray	m_arrText;
	CSize m_szSize;
	CSize m_szCellSize;
	int m_nCellPadding;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipWnd)
	protected:
	//}}AFX_VIRTUAL
	

// Implementation
public:
	virtual ~CToolTipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
