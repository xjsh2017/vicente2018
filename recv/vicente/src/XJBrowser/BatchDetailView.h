// ***************************************************************
//  BatchDetailView   version:  1.0    date: 3:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 批量召唤设备选择及各设备的召唤选项
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
 * @defgroup 	CBatchDetailView	批量总召过程与结果查看页面
 * @{
 */
/**
 * @brief       批量总召过程与结果查看页面
*  @author      LYH
 * @version     ver1.0
 * @date        03/11/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
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
	/** @brief           列表的列信息*/
	//##ModelId=49B87BB20092
	CArray<SColumn, SColumn&> m_arrColum;
	
	/** @brief           已完成召唤的选择项链表*/
	//##ModelId=49B87BB200FB
	BATCHSEL_MAP	m_mapComplete;

	/** @brief           记录之前被排序的列 */
	//##ModelId=49B87BB2011A
	int		m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87BB20128
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB2013B
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB20177
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87BB201A5
	BOOL NeedSave();

/*
 *  @brief   	InitListCtrl	 初始化列表框 
 *  @return 	void 
 */
	//##ModelId=49B87BB201D4
	void InitListCtrl();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87BB20203
	int	InitListStyle();

/*
 *  @brief   	AddSelToList	 把选择项加入到列表框中显示 
 *  @param 		[In]a param of Type  CBatchSel*  指定选择项对象
 *  @param 		[In]a param of Type  int  指定在列表中的位置索引
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB20280
	BOOL AddSelToList(CBatchSel* pSel, int nIndex);

/*
 *  @brief   	GetResult	 得到召唤结果文字描述 
 *  @param 		[In]a param of Type  int  召唤结果
 *  @return 	CString 召唤结果文字描述
 */
	//##ModelId=49B87BB202AF
	CString GetResult(int nResult);

/*
 *  @brief   	ChangeValueCalling	 在召唤时改变选择项的显示 
 *  @param 		[In]a param of Type  CBatchSel*  指定选择项
 *  @param 		[In]a param of Type  int  指定在列表框中的位置,可为-1,表示未知位置,函数自己会查找
 *  @return 	void 
 */
	//##ModelId=49B87BB2035B
	int ChangeValueCalling(CBatchSel* pSel, int nIndex = -1);


	CBatchSel* FindMapBatchSel( CString PT_ID, int nCpu );
/*
 *  @brief   	AddToComplete	 把指定选择项加入到已召唤完成链表 
 *  @param 		[In]a param of Type  CBatchSel*  指定选择项
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
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
