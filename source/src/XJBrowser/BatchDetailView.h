// ***************************************************************
//  BatchDetailView   version:  1.0    date: 3:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����ٻ��豸ѡ�񼰸��豸���ٻ�ѡ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_BATCHDETAILVIEW_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
#define AFX_BATCHDETAILVIEW_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchDetailView.h : header file
//
#include "ViewBase.h"
#include "BatchSel.h"
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


/**
 * @defgroup 	CBatchDetailView	�������ٹ��������鿴ҳ��
 * @{
 */
/**
 * @brief       �������ٹ��������鿴ҳ��
*  @author      LYH
 * @version     ver1.0
 * @date        03/11/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BB103D8
class CBatchDetailView : public CViewBase
{
protected:
	//##ModelId=49B87BB103DA
	CBatchDetailView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBatchDetailView)

// Form Data
public:
	//{{AFX_DATA(CBatchDetailView)
	enum { IDD = IDD_BATCH_DETAIL };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	BATCHSEL_MAP        m_mapSave;
private:
	//##ModelId=49B87BB2008D
	CThemeHelperST	m_ThemeHelper;
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87BB20092
	CArray<SColumn, SColumn&> m_arrColum;
	
	/** @brief           ������ٻ���ѡ��������*/
	//##ModelId=49B87BB200FB
	BATCHSEL_MAP	m_mapComplete;

	/** @brief           ��¼֮ǰ��������� */
	//##ModelId=49B87BB2011A
	int		m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BB20128
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB2013B
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB20177
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87BB201A5
	BOOL NeedSave();

/*
 *  @brief   	InitListCtrl	 ��ʼ���б�� 
 *  @return 	void 
 */
	//##ModelId=49B87BB201D4
	void InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BB20203
	int	InitListStyle();

/*
 *  @brief   	AddSelToList	 ��ѡ������뵽�б������ʾ 
 *  @param 		[In]a param of Type  CBatchSel*  ָ��ѡ�������
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB20280
	BOOL AddSelToList(CBatchSel* pSel, int nIndex);

/*
 *  @brief   	GetResult	 �õ��ٻ������������ 
 *  @param 		[In]a param of Type  int  �ٻ����
 *  @return 	CString �ٻ������������
 */
	//##ModelId=49B87BB202AF
	CString GetResult(int nResult);

/*
 *  @brief   	ChangeValueCalling	 ���ٻ�ʱ�ı�ѡ�������ʾ 
 *  @param 		[In]a param of Type  CBatchSel*  ָ��ѡ����
 *  @param 		[In]a param of Type  int  ָ�����б���е�λ��,��Ϊ-1,��ʾδ֪λ��,�����Լ������
 *  @return 	void 
 */
	//##ModelId=49B87BB2035B
	int ChangeValueCalling(CBatchSel* pSel, int nIndex = -1);


	CBatchSel* FindMapBatchSel( CString PT_ID, int nCpu );
/*
 *  @brief   	AddToComplete	 ��ָ��ѡ������뵽���ٻ�������� 
 *  @param 		[In]a param of Type  CBatchSel*  ָ��ѡ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB30003
	BOOL AddToComplete(CBatchSel* pSel);

	void FillMap();
	void ChangeSetting(CBatchSel* pSel);

	BOOL WriteResultToExcel( CString strFullPath );
	BOOL AddSelToExcel(CDatabase* pDatabase, CBatchSel* pSel);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchDetailView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB3009C
	virtual ~CBatchDetailView();
#ifdef _DEBUG
	//##ModelId=49B87BB300DA
	virtual void AssertValid() const;
	//##ModelId=49B87BB30119
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBatchDetailView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnColumnclickListBatchDetail(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnSTTP20154(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnShowResult( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBatchClose();

	afx_msg void OnBatchExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHDETAILVIEW_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
