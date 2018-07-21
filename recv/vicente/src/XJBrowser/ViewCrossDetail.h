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
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           所属预警点*/
	CCrossDef* m_pCrossDef;
	/** @brief           记录之前被排序的列*/
	int m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B86036C
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B86037A
	int InitListCtrl();
/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8603A9
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8603B9
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87B8603C8
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	RegulateControlSize	 调整控件位置
 *  @return 	void 
 */
	void RegulateControlSize();

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

/*
 *  @brief   	AddEventToList	 将指定预警数据点填充到列表 
 *  @param 		[In]a param of Type  CCrossPoint *  指定预警数据点
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 填充成功返回TRUE, 失败返回FALSE
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
