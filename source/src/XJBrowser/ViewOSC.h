// ***************************************************************
//  ViewOSC   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 录波事件视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_)
#define AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewOSC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B83006D
class CViewOSC : public CFormView
{
public:
	//##ModelId=49B87B83006F
	CViewOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewOSC)

// Form Data
public:
	//{{AFX_DATA(CViewOSC)
	enum { IDD = IDD_XJ_VIEW_OSC };
	CButton	m_cboxShowFault;
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowFault;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B830083
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	//##ModelId=49B87B830088
	CXJEventManager		m_EventManager;	//事件管理器
	//##ModelId=49B87B83008C
	int					m_nMaxRow;	//最大行数
	//##ModelId=49B87B83008D
	BOOL				m_bShowAcked; //是否显示已确认事件
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B83008E
	int		m_nOldSortCol;
	/** @brief           是否显示调试信息*/
	BOOL	m_bShowDebug;
	/** @brief           是否无故障信息*/
	BOOL    m_bShowFault;
	/** @brief           厂站ID**/
	CString m_sChooseStationID;
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B83008F
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B83009E
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8300AB
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8300AC
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8300BB
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8300CB
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8300CC
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8300DA
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8300DB
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAlarm *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8300EC
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventOSC*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B8300FA
	void RefreshAck(CXJEventOSC* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEventOSC*  被确认的事件
 *  @return 	void 
 */
	//##ModelId=49B87B8300FD
	void TellOtherWndAck( CXJEventOSC* pEvent );

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B830109
	void CheckLineCount();

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
// Operations
/*
 *  @brief   	InitComBox	 初始化combox 
 *  @param 		
 *  @param 		
 *  @return 
 */
	
	void InitComBox();

	int RefreshData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewOSC)
	public:
	//##ModelId=49B87B83010A
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B830119
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B830128
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B830138
	virtual ~CViewOSC();
#ifdef _DEBUG
	//##ModelId=49B87B830148
	virtual void AssertValid() const;
	//##ModelId=49B87B830157
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewOSC)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterFault();
	afx_msg void OnEventOpenosc();
	afx_msg void OnEventDownosc();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnSelchangeCOMBOChooseStation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowFault();
	//}}AFX_MSG
	//##ModelId=49B87B8301BC
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8301C6
	afx_msg void OnSTTP20132(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_)
