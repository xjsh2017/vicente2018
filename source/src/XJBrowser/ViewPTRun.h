// ***************************************************************
//  ViewPTRun   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 保护运行状态视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_)
#define AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPTRun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewPTRun form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B8102EE
class CViewPTRun : public CFormView
{
public:
	//##ModelId=49B87B8102F0
	CViewPTRun();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPTRun)

// Form Data
public:
	//{{AFX_DATA(CViewPTRun)
	enum { IDD = IDD_XJ_VIEW_PTRUN };
	CButton	m_cboxShowPTRunCheck;
	CButton	m_cboxShowPTRunCode;
	CButton	m_cboxShowPTRunDebug;
	CButton	m_cboxShowPTRunOFF;
	CButton	m_cboxShowPTRunON;
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bShowPTRunCheck;
	BOOL	m_bShowPTRunCode;
	BOOL	m_bShowPTRunDebug;
	BOOL	m_bShowPTRunOFF;
	BOOL	m_bShowPTRunON;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	//##ModelId=49B87B81030E
	CXJEventManager		m_EventManager;	//事件管理器
	//##ModelId=49B87B810312
	int					m_nMaxRow;	//最大行数
	//##ModelId=49B87B810313
	BOOL				m_bShowAcked; //是否显示已确认事件
private:
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B810314
	int					m_nOldSortCol;
	/** @brief           记录定时器*/
	//##ModelId=49B87B81031C
	int		m_nTimer;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B81031E
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B81032D
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B81033D
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B810342
	IXJReport * pReportDoc;

// Operations
public:
	CString m_sChooseStationID;

	void InitComBox();
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B81034B
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	/*
	*  @brief   	TranslateSingleKey	 解释单个关键字的意义 
	*  @param 		[In]a param of Type  CString  关键字
	*  @return 	CString 关键字表示的值
	*/
	//##ModelId=49B87B81035B
	CString TranslateSingleKey(CString sKey);
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B81035D
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B81036B
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B81037A
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B81038A
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B810399
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8103A9
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8103B9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8103C8
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 把指定装置的运行状态加入到列表中显示 
 *  @param 		[In]a param of Type  CSecObj *  要加入的装置
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8103D8
	BOOL AddEventToList(CSecObj * pSec, int i);

/*
 *  @brief   	IsNeedShow	 判断当前运行状态的保护信息是否需要显示 
 *  @param 		[In]a param of Type  int  运行状态
 *  @return 	BOOL 需要显示返回TRUE, 不需要显示返回FALSE
 */
	//##ModelId=49B87B820002
	BOOL IsNeedShow(int nStatus);

/*
 *  @brief   	ChangeStatusInList	 改变指定二次设备的运行状态在通信状态列表框中的显示 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	//##ModelId=49B87B82000F
	void ChangeStatusInList(CSecObj* pSec);

/*
 *  @brief   	RefreshDuravieTime	 刷新LIST中指定位置的持续时间显示 
 *  @param 		[In]a param of Type  int  指定在LIST中的位置
 *  @return 	void 
 */
	//##ModelId=49B87B820011
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 刷新所有保护的通信状态的持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87B820013
	void RefreshTime();

	/*
	*  @brief   	WriteResultToExcel	将结果导出到Excel 
	*  @param 		[In]a param of Type  CString  文件路径
	*  @return 	void 
	*/
	//##ModelId=49B87B82001F
	BOOL WriteResultToExcel(CString strFullPath);

	/*
	*  @brief   	AddResultToExcel	 将指定的结果项写入到指定的Excel文件中 
	*  @param 		[In]a param of Type  CString  数据库对象
	*  @param 		[In]a param of Type  CSecObj*  设备对象
	*  @return 	void 
	*/
	//##ModelId=49B87B820021
	void AddResultToExcel(CDatabase* pDatabase, CSecObj* pSec);

	/*
	*  @brief   	InitCOM	 初始化打印模板COM组件 
	*  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B82002E
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 删除COM组件 
	*  @return 	void 
	*/
	//##ModelId=49B87B82002F
	void ReleaseCOM();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPTRun)
	public:
	//##ModelId=49B87B820030
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B82003E
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B82005D
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B82007D
	virtual ~CViewPTRun();
#ifdef _DEBUG
	//##ModelId=49B87B82008C
	virtual void AssertValid() const;
	//##ModelId=49B87B82009C
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewPTRun)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventDevice();
	afx_msg void OnEventSec();
	afx_msg void OnClickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPtrunOn();
	afx_msg void OnPtrunOff();
	afx_msg void OnPtrunCheck();
	afx_msg void OnPtrunDebug();
	afx_msg void OnPtrunCode();
	afx_msg void OnPtrunHistory();
	afx_msg void OnPtrunPrint();
	afx_msg void OnPtrunExport();
	afx_msg void OnPtrunRefresh();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowptrunon();
	afx_msg void OnCheckShowptrunoff();
	afx_msg void OnCheckShowptruncheck();
	afx_msg void OnCheckShowptrundebug();
	afx_msg void OnCheckShowptruncode();
	//}}AFX_MSG
	//##ModelId=49B87B82016B
	void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_)
