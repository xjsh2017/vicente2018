// ***************************************************************
//  ViewPTComm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 保护通讯状态视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_)
#define AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPTComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B8201E4
class CViewPTComm : public CFormView
{
public:
	//##ModelId=49B87B8201F5
	CViewPTComm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPTComm)

// Form Data
public:
	//{{AFX_DATA(CViewPTComm)
	enum { IDD = IDD_XJ_VIEW_PTCOMM };
	CButton	m_cBoxShowPTCommON;
	CButton	m_cBoxShowPTcommOFF;
	CCompareCmb	m_cmbChooseStation;
	CStatic	m_staticChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bShowPTCommOFF;
	BOOL	m_bShowPTCommON;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B820209
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	//##ModelId=49B87B820214
	CXJEventManager		m_EventManager; //事件管理器
	//##ModelId=49B87B820218
	int					m_nMaxRow;	//最大显示行数
	//##ModelId=49B87B820219
	BOOL				m_bShowAcked;//是否显示已确认事件
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B82021A
	int		m_nOldSortCol;
	/** @brief           记录定时器*/
	//##ModelId=49B87B820222
	int		m_nTimer;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B820224
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B820229
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B820233
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B820243
	IXJReport * pReportDoc;
// Operations
public:
	void InitComBox();
	CString m_sChooseStationID;
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	/*
	*  @brief   	TranslateSingleKey	 解释单个关键字的意义 
	*  @param 		[In]a param of Type  CString  关键字
	*  @return 	CString 关键字表示的值
	*/
	//##ModelId=49B87B820252
	CString TranslateSingleKey(CString sKey);
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B820254
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B820271
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B820280
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B820281
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B82029F
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8202A0
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202AF
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8202BF
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);


/*
 *  @brief   	AddEventToList	 将指定二次设备的通信状态信息加入到列表框中 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8202CE
	BOOL AddEventToList(CSecObj* pSec, int i);

/*
 *  @brief   	IsNeedShow	 判断当前通信状态的保护信息是否需要显示 
 *  @param 		[In]a param of Type  int  通信状态
 *  @return 	BOOL 需要显示返回TRUE, 不需要显示返回FALSE
 */
	//##ModelId=49B87B8202DE
	BOOL IsNeedShow(int nStatus);

/*
 *  @brief   	ChangeStatusInList	 改变指定二次设备的通信状态在通信状态列表框中的显示 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeStatusInList(CSecObj* pSec);

/*
 *  @brief   	RefreshDuravieTime	 刷新LIST中指定位置的持续时间显示 
 *  @param 		[In]a param of Type  int  指定在LIST中的位置
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 刷新所有保护的通信状态的持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
	void RefreshTime();

	/*
	*  @brief   	InitCOM	 初始化打印模板COM组件 
	*  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8202F1
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 删除COM组件 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F2
	void ReleaseCOM();

	/*
	*  @brief   	WriteResultToExcel	将结果导出到Excel 
	*  @param 		[In]a param of Type  CString  文件路径
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F3
	BOOL WriteResultToExcel(CString strFullPath);

	/*
	*  @brief   	AddResultToExcel	 将指定的结果项写入到指定的Excel文件中 
	*  @param 		[In]a param of Type  CString  数据库对象
	*  @param 		[In]a param of Type  CSecObj*  设备对象
	*  @return 	void 
	*/
	//##ModelId=49B87B8202FE
	void AddResultToExcel(CDatabase* pDatabase, CSecObj* pSec, int index);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPTComm)
	public:
	virtual void OnInitialUpdate();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B82033C
	virtual ~CViewPTComm();
#ifdef _DEBUG
	//##ModelId=49B87B82035B
	virtual void AssertValid() const;
	//##ModelId=49B87B82036B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewPTComm)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventDevice();
	afx_msg void OnEventSec();
	afx_msg void OnClickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPtcommOn();
	afx_msg void OnPtcommOff();
	afx_msg void OnPtcommHistory();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPtcommExport();
	afx_msg void OnPtcommPrint();
	afx_msg void OnPtcommRefresh();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowptcommon();
	afx_msg void OnCheckShowptcommoff();
	//}}AFX_MSG
	//##ModelId=49B87B830021
	void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_)
