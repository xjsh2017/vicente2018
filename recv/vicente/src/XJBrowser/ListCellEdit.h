#if !defined(AFX_LISTCELLEDIT_H__3C59ACB3_1141_4A7F_8F72_37A4D9782868__INCLUDED_)
#define AFX_LISTCELLEDIT_H__3C59ACB3_1141_4A7F_8F72_37A4D9782868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCellEdit.h : header file
//
/////////////////////////////////////////
//该类改造于Lee Nowotny的gxEditCell类
//详细见例子http://www.codetools.com/listctrl/listeditor.asp
/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CListCellEdit window

class CListCellEdit : public CEdit
{
// Construction
public:
	CListCellEdit(int nItem, int nSubItem, CString strInitTex);
	CListCellEdit(int nItem, int nSubItem, CString strInitTex, double dMin, double dMax, int m_nType);
	CListCellEdit(int nItem, int nSubItem, CString strInitTex, double dMin, double dMax, int m_nType, int nDataType, float fStepValue, int nTextLength, int nJudge = 0, bool intasfloat = false);
	void SetListItemText();

// Attributes
public:
	CToolTipCtrl	m_tool;
	BOOL			m_bModifyed;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCellEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCellEdit();
 
protected:
	int			m_nItem;
    int			m_nSubItem;
    CString		m_strInitText;
    BOOL		m_bEscape;
	double         m_dMinValue;
	double         m_dMaxValue;
	int            m_nType;
	int			m_nDataType;
	int			m_nJudge;//是否判定上下限
	float		m_fStepValue;//步长
	int			m_nTextLength;
	bool		m_bIntAsFloat; //将Int类的值当作浮点处理

BOOL IsVaildStep(CString sSource);

BOOL IsVaildInput(CString sSource, int nDataType);

	// Generated message map functions
	//{{AFX_MSG(CListCellEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCELLEDIT_H__3C59ACB3_1141_4A7F_8F72_37A4D9782868__INCLUDED_)
