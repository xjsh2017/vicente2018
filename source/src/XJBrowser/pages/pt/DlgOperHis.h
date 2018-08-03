#ifndef _DLG_OPER_HIS_H
#define _DLG_OPER_HIS_H

#include "resource.h"
// DlgOperHis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOperHis dialog

//##ModelId=49B87BA402DF
class CSecObj;
class CDlgOperHis : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA402E1
	CDlgOperHis(CWnd* pParent = NULL, int nType = 1);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOperHis)
	enum { IDD = IDD_OPER_HIS };
	//##ModelId=49B87BA402E3
	CString	m_strModify;
	CListCtrlEx	m_List;
	//}}AFX_DATA
	
	CString m_strDESC;
	CString m_sCPU;
	CString m_sZone;
	int	m_nType;

	CSecObj* m_pObj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOperHis)
	protected:
	//##ModelId=49B87BA402E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8C009C
	int	InitListStyle();
	void UpdateLabels();

	void FillData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOperHis)
	virtual BOOL OnInitDialog();
    afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHECK_PRO_H__48F5010D_E549_4A81_82D4_5621C482EE91__INCLUDED_)
