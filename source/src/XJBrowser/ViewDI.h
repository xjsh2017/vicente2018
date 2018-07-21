// ***************************************************************
//  ViewDI   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 开关量变位事件列表视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_)
#define AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDI.h : header file
//
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDI form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B850128
class CViewDI : public CFormView
{
public:
	//##ModelId=49B87B850139
	CViewDI();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDI)

// Form Data
public:
	//{{AFX_DATA(CViewDI)
	enum { IDD = IDD_XJ_VIEW_DI };
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CButton	m_cmbShowReady;
	CButton	m_cmbShowDebug;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B85014E
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息

// Operations
public:
	/** @brief           事件管理对象*/
	//##ModelId=49B87B850153
	CXJEventManager		m_EventManager;
	/** @brief           最大显示行数*/
	//##ModelId=49B87B850157
	int					m_nMaxRow;
	/** @brief           是否显示已确认事件*/
	//##ModelId=49B87B850158
	BOOL				m_bShowAcked;
	/** @brief           之前被排序的列*/
	//##ModelId=49B87B850159
	int		m_nOldSortCol;
	/** @brief           是否显示调试信息*/
	BOOL	m_bShowDebug;

public:
	/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
	void InitComBox();
	/** @brief           厂站ID*/
	CString m_sChooseStationID;

/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B85015A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B850169
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B850177
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B850178
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B850186
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B850196
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B850197
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8501A5
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8501A6
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAlarm *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8501B7
	BOOL AddEventToList(CXJEventDI * pEvent, int i);

/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventDI*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B8501C6
	void RefreshAck(CXJEventDI* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEventDI*  被确认的事件
 *  @return 	void 
 */
	//##ModelId=49B87B8501C9
	void TellOtherWndAck( CXJEventDI* pEvent );

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B8501D5
	void CheckLineCount();

/*
 *  @brief   	DisplayValue	 得到开关量的值显示字符串 
 *  @param 		[In]a param of Type  int  开关量值
 *  @return 	CString 显示值
 */
	//##ModelId=49B87B8501D6
	CString DisplayValue(int nValue);

/*
 *  @brief   	LoadConfig	 读取配置 
 *  @return 	void 
 */
	void LoadConfig();

/*
 *  @brief   	SaveConfig	 保存配置 
 *  @return 	void 
 */
	void SaveConfig();
	
	void AddSignToEvent(CXJEventDI* pEvent);

	int RefreshData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDI)
	public:
	//##ModelId=49B87B8501D8
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8501E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8501F4
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B850213
	virtual ~CViewDI();
#ifdef _DEBUG
	//##ModelId=49B87B850222
	virtual void AssertValid() const;
	//##ModelId=49B87B850224
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDI)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnClickListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventSign();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	//}}AFX_MSG
	//##ModelId=49B87B850296
	void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8502A0
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_)
