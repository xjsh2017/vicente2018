#if !defined(AFX_DLGAPP_H__A05C556A_24FE_4E96_843D_B453B8F94F7A__INCLUDED_)
#define AFX_DLGAPP_H__A05C556A_24FE_4E96_843D_B453B8F94F7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgApp.h : header file
//
#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgApp dialog

class CDlgApp : public CDialog
{
// Construction
public:
	CDlgApp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgApp)
	enum { IDD = IDD_DLG_APP };
	CListCtrlEx	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgApp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B870280
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitListStyle();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B87029F
	int FillListData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgApp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAPP_H__A05C556A_24FE_4E96_843D_B453B8F94F7A__INCLUDED_)
