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

	// Generated message map functions
protected:
	//{{AFX_MSG(CHideHeaderCtrl)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUseHideCursor;		//����������أ���ô���ָ�벻��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__HENGAI_CLASS_HIDEHEADERCTRL_H__)
