#if !defined(AFX_VIEWCROSS_H__45D39500_B6C3_40D0_BD88_53CD81962952__INCLUDED_)
#define AFX_VIEWCROSS_H__45D39500_B6C3_40D0_BD88_53CD81962952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewCross.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewCross form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "CrossDef.h"

/** @brief           Ԥ������*/
typedef CTypedPtrList<CObList , CCrossDef*>	CROSSDEF_LIST;

class CViewCross : public CFormView
{
protected:
	CViewCross();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCross)

// Form Data
public:
	//{{AFX_DATA(CViewCross)
	enum { IDD = IDD_XJ_VIEW_CROSS };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B860351
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
private:
	/** @brief           Ԥ������*/
	CROSSDEF_LIST	m_listCross;
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
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

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
 *  @brief   	AddEventToList	 ��ָ��Ԥ����䵽�б� 
 *  @param 		[In]a param of Type  CCrossDef *  ָ��Ԥ��
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddEventToList(CCrossDef * pEvent, int i = 0);

/*
 *  @brief   	RegulateControlSize	 �����ؼ�λ��
 *  @return 	void 
 */
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDataFromDB();

/*
 *  @brief   	Clear	 ��������� 
 *  @return 	void 
 */
	void Clear();

/*
 *  @brief   	FindCross	 ����ָ��ID��Ԥ���� 
 *  @param 		[In]a param of Type  int  ָ��ID
 *  @return 	CCrossDef* Ԥ����
 */
	CCrossDef* FindCross(int nID);

/*
 *  @brief   	UpdateCrossList	 ����Ԥ�����б��е���ʾ 
 *  @param 		[In]a param of Type  CCrossDef*  ָ��Ԥ��
 *  @return 	void 
 */
	void UpdateCrossList(CCrossDef* pDef);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCross)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewCross();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewCross)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListCross(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListCross(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListCross(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnSTTP20159(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCROSS_H__45D39500_B6C3_40D0_BD88_53CD81962952__INCLUDED_)
