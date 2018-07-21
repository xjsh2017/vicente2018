// ***************************************************************
//  ViewAlarm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 告警事件列表视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_)
#define AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAlarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "ViewBase.h"
#include "ListCtrlEx.h"
#include "EventManager.h"
#include "Event.h"

//##ModelId=49B87B86033C
class CViewAlarm : public CFormView
{
protected:
	//##ModelId=49B87B86033E
	CViewAlarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAlarm)

// Form Data
public:
	//{{AFX_DATA(CViewAlarm)
	enum { IDD = IDD_XJ_VIEW_ALARM };
	CStatic	m_staticChooseStation;
	CButton	m_cboxShowReturn;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowReturn;
	BOOL	m_bCBoxShowDebug;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B860351
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
private:
	//##ModelId=49B87B86035C
	CXJEventManager	m_EventManager;	//事件管理器
	//##ModelId=49B87B860360
	int	m_nMaxRow;	//最大行数
	//##ModelId=49B87B860361
	BOOL m_bShowAcked; //是否显示已确认事件
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B86036B
	int m_nOldSortCol;
	/** @brief           是否显示调试事件*/
	BOOL m_bShowDebug;
	/** @brief           是否显示返回信息*/
	BOOL	m_bShowReturn;
	/** @brief           选定的厂站名ID*/
	CString m_sChooseStationID;
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
 *  @brief   	InitComBox	 设置combox信息
 *  @param 		
 *  @param 		
 *  @return 	
 */
	void InitComBox();	
private:
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
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8603C9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8603D8
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAlarm *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B870000
	BOOL AddEventToList(CXJEventAlarm * pEvent, int i);

/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventAlarm*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B87000F
	void RefreshAck(CXJEventAlarm* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEventAlarm*  被确认的事件
 *	@param		[In]a param of Type  BOOL 是否被动确认
 *  @return 	void 
 */
	//##ModelId=49B87B870012
	void TellOtherWndAck( CXJEventAlarm* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	FindLocalPoint	 查找某条事件在本地的地址 
 *  @param 		[In]a param of Type  CXJEventAlarm*  要查找的事件
 *  @return 	CXJEventAlarm* 本地地址
 */
	//##ModelId=49B87B87001F
	CXJEventAlarm* FindLocalPoint(CXJEventAlarm* pEvent);

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B870021
	void CheckLineCount();

/*
 *  @brief   	BuildOrSQL	 组建I,II,II类信息的过滤查询语句 
 *  @return 	CString 查询语句
 */
	//##ModelId=49B87B870022
	CString BuildOrSQL();

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


	void AddSignToEvent(CXJEventAlarm* pEvent);


	void RefreshData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAlarm)
	public:
	//##ModelId=49B87B870023
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B87003E
	virtual void OnInitialUpdate();
	//##ModelId=49B87B87004E
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	//##ModelId=49B87B87006D
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B87007D
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//##ModelId=49B87B87009C
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8700AB
	virtual ~CViewAlarm();
#ifdef _DEBUG
	//##ModelId=49B87B8700BB
	virtual void AssertValid() const;
	//##ModelId=49B87B8700CB
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAlarm)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSign();
	afx_msg void OnDblclkListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterReturn();
	afx_msg void OnSelchangeComboVachstation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnCheckShowreturn();
	//}}AFX_MSG
	//##ModelId=49B87B87016E
	void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B870178
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B87017B
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B870188
	void OnEventPropAckPT(WPARAM wParma, LPARAM lParam);
	//##ModelId=49B87B87018B
	void OnRefreshAlarmView(WPARAM wParam, LPARAM lParam);
	afx_msg OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_)
