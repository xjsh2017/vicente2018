#if !defined(AFX_VIEWCROSSDETAIL_H__70671A57_0022_44C9_90F7_4E771B8A45A2__INCLUDED_)
#define AFX_VIEWCROSSDETAIL_H__70671A57_0022_44C9_90F7_4E771B8A45A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewCrossDetail.h : header file
//
#include "CrossDef.h"
#include "CrossPoint.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewCrossDetail : public CFormView
{
protected:
	CViewCrossDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCrossDetail)

// Form Data
public:
	//{{AFX_DATA(CViewCrossDetail)
	enum { IDD = IDD_XJ_VIEW_CROSS_DETAIL };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ����Ԥ����*/
	CCrossDef* m_pCrossDef;
	/** @brief           ��¼֮ǰ���������*/
	int m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B86036C
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B86037A
	int InitListCtrl();
/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8603A9
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8603B9
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87B8603C8
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	RegulateControlSize	 �����ؼ�λ��
 *  @return 	void 
 */
	void RegulateControlSize();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

/*
 *  @brief   	AddEventToList	 ��ָ��Ԥ�����ݵ���䵽�б� 
 *  @param 		[In]a param of Type  CCrossPoint *  ָ��Ԥ�����ݵ�
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddEventToList(CCrossPoint * pEvent, int i = 0);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCrossDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewCrossDetail();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewCrossDetail)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickListCrossdetail(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnViewCrossSelChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCROSSDETAIL_H__70671A57_0022_44C9_90F7_4E771B8A45A2__INCLUDED_)
