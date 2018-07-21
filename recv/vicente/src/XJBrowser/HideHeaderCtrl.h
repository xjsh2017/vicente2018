#ifndef	__HENGAI_CLASS_HIDEHEADERCTRL_H__
#define __HENGAI_CLASS_HIDEHEADERCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HideHeaderCtrl.h : header file
//
#ifndef	__AFXTEMPL_H__
#pragma message ("������� stdafx.h �ļ���ʹ���� #include<afxtempl.h> �򲻻���������Ϣ��ʾ")
	#include <afxtempl.h>
#endif

class CMemDC : public CDC 
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;
	}
	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
			this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) 
			delete m_bitmap;
	}
	CMemDC* operator->() 
	{
		return this;
	}
};



struct SColumn{			//��¼��ÿ�е���Ϣ��
	BOOL bHide;
	int nItemWidth;		//������ǰ�Ŀ�ȡ���Ϊ���ص�ʱ����Ҫ�������Ϊ0
	CString sName;
	SColumn(){
		bHide = FALSE;
		nItemWidth = -1;
		sName = "";
	}
};
/////////////////////////////////////////////////////////////////////////////
// CHideHeaderCtrl window

class CHideHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CHideHeaderCtrl();

// Attributes
public:

// Operations
public:
	CArray<SColumn, SColumn&> m_aColum;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHideHeaderCtrl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHideHeaderCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// Generated message map functions
protected:
	//{{AFX_MSG(CHideHeaderCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUseHideCursor;		//����������أ���ô���ָ�벻��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HENGAI_CLASS_HIDEHEADERCTRL_H__)
