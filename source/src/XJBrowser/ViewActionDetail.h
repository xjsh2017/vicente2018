// ***************************************************************
//  ViewActionDetail   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����¼���ϸ������ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWACTIONDETAIL_H__EDA259EC_0302_4A10_A340_2686806015E0__INCLUDED_)
#define AFX_VIEWACTIONDETAIL_H__EDA259EC_0302_4A10_A340_2686806015E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewActionDetail.h : header file
//
#include "Event.h"

/////////////////////////////////////////////////////////////////////////////
// CViewActionDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B870196
class CViewActionDetail : public CFormView
{
protected:
	//##ModelId=49B87B8701A5
	CViewActionDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewActionDetail)

// Form Data
public:
	//{{AFX_DATA(CViewActionDetail)
	enum { IDD = IDD_XJ_VIEW_ACTION_DETAIL };
	CListCtrlEx	m_ListChr;
	CListCtrlEx	m_ListSign;
	CString	m_sTime;
	CString	m_sDevice;
	CString	m_sDetail;
	//}}AFX_DATA

// Attributes
public:
private:
	//##ModelId=49B87B8701AA
	CXJEventAction * m_pEvent;

// Operations
public:
private:
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 		void 
	*/
	//##ModelId=49B87B8701B5
	void RegulateControlSize();

/*
 *  @brief   	RefureDetail	 ˢ����ʾ����ϸ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B8701B6
	void RefureDetail();

/*
 *  @brief   	InitSignListStyle	 ��ʼ���ź����б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitSignListStyle();

/*
 *  @brief   	InitSignListStyle	 ��ʼ���������б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitChrListStyle();

/*
 *  @brief   	AddSignToList	 ���ź�����ӵ��б� 
 *  @param 		[In]a param of Type  PT_Signal*  ָ���ź���
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddSignToList(PT_Signal* pSign, int nIndex = 0);

/*
 *  @brief   	AddChrToList	 ����������ӵ��б� 
 *  @param 		[In]a param of Type  PT_ActionCharacter*  ָ��������
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddChrToList(PT_ActionCharacter* pChr, int nIndex = 0);

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	void 
 */
	void FillListData();

	int GetSignRelativeTime(CTime tmSignTime, int nSignMS);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewActionDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8701BC
	virtual ~CViewActionDetail();
#ifdef _DEBUG
	//##ModelId=49B87B8701C6
	virtual void AssertValid() const;
	//##ModelId=49B87B8701C8
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewActionDetail)
	//##ModelId=49B87B8701CB
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=49B87B8701CE
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	//##ModelId=49B87B8701D8
	afx_msg void OnViewActionSelectChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8701DC
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8701E5
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWACTIONDETAIL_H__EDA259EC_0302_4A10_A340_2686806015E0__INCLUDED_)
