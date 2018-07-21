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

/** @brief           预警链表*/
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
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
private:
	/** @brief           预警链表*/
	CROSSDEF_LIST	m_listCross;
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
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

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
 *  @brief   	AddEventToList	 将指定预警填充到列表 
 *  @param 		[In]a param of Type  CCrossDef *  指定预警
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 填充成功返回TRUE, 失败返回FALSE
 */
	BOOL AddEventToList(CCrossDef * pEvent, int i = 0);

/*
 *  @brief   	RegulateControlSize	 调整控件位置
 *  @return 	void 
 */
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDataFromDB();

/*
 *  @brief   	Clear	 清除旧数据 
 *  @return 	void 
 */
	void Clear();

/*
 *  @brief   	FindCross	 查找指定ID的预警点 
 *  @param 		[In]a param of Type  int  指定ID
 *  @return 	CCrossDef* 预警点
 */
	CCrossDef* FindCross(int nID);

/*
 *  @brief   	UpdateCrossList	 更新预警在列表中的显示 
 *  @param 		[In]a param of Type  CCrossDef*  指定预警
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
