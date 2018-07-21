#if !defined(AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_)
#define AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACResultSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CACResultSel form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BB50203
class CACResultSel : public CFormView
{
protected:
	//##ModelId=49B87BB50205
	CACResultSel();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CACResultSel)

// Form Data
public:
	//{{AFX_DATA(CACResultSel)
	enum { IDD = IDD_ACRESULTSEL };
	//##ModelId=49B87BB50207
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           ������ڵ�*/
	//##ModelId=49B87BB5020B
	HTREEITEM	m_hRoot;

// Operations
public:
/*
 *  @brief   	InsertNet	 ��������ڵ� 
 *  @param 		[In]a param of Type  CNetObj*  Ҫ����ĵ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB50213
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 ���볧վ�ڵ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB50215
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	FindNetPos	 ���ҵ��������е�λ�� 
 *  @param 		[In]a param of Type  CString  ����ID
 *  @return 	HTREEITEM ���ڵ�ṹ
 */
	//##ModelId=49B87BB50217
	HTREEITEM FindNetPos(CString sNetID);

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87BB50219
	void RegulateControlSize();

/*
 *  @brief   	BuildNet	 ���豸���д������� 
 *  @return 	void 
 */
	//##ModelId=49B87BB5021A
	void BuildNet();

/*
 *  @brief   	BuildStation	 ���豸���д�����վ 
 *  @return 	void 
 */
	//##ModelId=49B87BB5021B
	void BuildStation();

/*
 *  @brief   	InitTree	 ��ʼ���豸�� 
 *  @return 	int ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 */
	//##ModelId=49B87BB5021C
	int InitTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACResultSel)
	public:
	//##ModelId=49B87BB50222
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BB50224
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB50227
	virtual ~CACResultSel();
#ifdef _DEBUG
	//##ModelId=49B87BB50229
	virtual void AssertValid() const;
	//##ModelId=49B87BB5022B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CACResultSel)
	//##ModelId=49B87BB50233
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87BB50238
	afx_msg void OnSelchangedTreeAcresultsel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87BB50242
	afx_msg void OnAutoCallResultFrameOpen(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_)
