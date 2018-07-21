// ***************************************************************
//  ViewAction   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 动作事件列表视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_)
#define AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAction.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAction form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "ListCtrlEx.h"
#include "EventManager.h"

//##ModelId=49B87B870242
class CViewAction : public CFormView
{
protected:
	CViewAction();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAction)

// Form Data
public:
	//{{AFX_DATA(CViewAction)
	enum { IDD = IDD_XJ_VIEW_ACTION };
	CButton	m_cboxShowReturn;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CStatic	m_StateChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowReturn;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           列表的列信息*/
	//##ModelId=49B87B870267
	CArray<SColumn, SColumn&> m_arrColum;
	BOOL m_bQuitThread;
private:
	/** @brief           事件最大显示行数*/
	//##ModelId=49B87B87026B
	int		m_nMaxRow;
	/** @brief           事件管理*/
	//##ModelId=49B87B87026D
	CXJEventManager	m_EventManager;
	/** @brief           是否显示已确认事件*/
	//##ModelId=49B87B870271
	BOOL	m_bShowAcked;
	/** @brief           记录之前被排序的列 */
	//##ModelId=49B87B870272
	int		m_nOldSortCol;
	/** @brief           是否显示调试信息*/
	BOOL	m_bShowDebug;
	/** @brief           是否显示返回信息*/
	BOOL	m_bShowReturn;
	/** @brief           选定的厂站名ID*/
	CString m_sChooseStationID;
	
	CWinThread*	m_pLoadDataThread;

	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
public:
	/*
 *  @brief   	InitComBox	 设置combox信息
 *  @param 		
 *  @param 		
 *  @return 	
 */
	void InitComBox();		
	
	//##ModelId=49B87B870273
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadDataFromDB();

	void RefreshData();

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B87029F
	int FillListData();

	void NotifyThreadEnd();
private:
/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B870280
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B870290
	int	InitListStyle();


/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8702AF
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8702BF
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87B8702CE
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8702DE
	void RegulateControlSize();

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAction *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8702EF
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
 *  @brief   	RefreshAck	 简要的函数功能说明文字 
 *  @param 		[In]a param of Type  CXJEventAction*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B8702FE
	void RefreshAck(CXJEventAction* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEventAction*  被确认的事件
 *	@param		[In]a param of Type	 BOOL 是否被动确认
 *  @return 	void 
 */
	//##ModelId=49B87B87030D
	void TellOtherWndAck( CXJEventAction* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	FindLocalPoint	 查找某条事件在本地的地址 
 *  @param 		[In]a param of Type  CXJEventAction*  要查找的事件
 *  @return 	CXJEventAction* 本地地址
 */
	//##ModelId=49B87B870310
	CXJEventAction* FindLocalPoint(CXJEventAction* pEvent);

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

/*
 *  @brief   	BuildOrSQL	 组建I,II,II类信息的过滤查询语句 
 *  @return 	CString 查询语句
 */
	//##ModelId=49B87B87031C
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

	void StartThead();
	void EndThread();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAction)
	public:
	//##ModelId=49B87B87031D
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B87032C
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B87033C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B87035B
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B87037A
	virtual ~CViewAction();
#ifdef _DEBUG
	//##ModelId=49B87B87038A
	virtual void AssertValid() const;
	//##ModelId=49B87B870399
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAction)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListAction(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnColumnclickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSign();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterReturn();
	afx_msg void OnExportExcel();
	afx_msg void OnSelchangeCOMBOChooseStation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnCheckShowreturn();
	//}}AFX_MSG
	//##ModelId=49B87B88005E
	void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B880061
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B88006F
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B880072
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B88007E
	afx_msg void OnRefreshActionView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_)
