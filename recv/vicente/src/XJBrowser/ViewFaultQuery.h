// ***************************************************************
//  ViewFaultQuery   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 事故报告查询页面,包含查询条件与显示查询结果的列表
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_)
#define AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFaultQuery.h : header file
//
#include "ViewBase.h"
#include "FaultReport.h"

/** @brief           事故报告链表*/
//##ModelId=49B87B8301F1
typedef CTypedPtrList<CObList , CFaultReport*>	FAULTREPORT_LIST;
/////////////////////////////////////////////////////////////////////////////
// CViewFaultQuery form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B830242
class CViewFaultQuery : public CViewBase
{
protected:
	//##ModelId=49B87B830244
	CViewFaultQuery();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFaultQuery)

// Form Data
public:
	//{{AFX_DATA(CViewFaultQuery)
	enum { IDD = IDD_XJ_VIEW_FAULT_QUERY };
	CCompareCmb	m_cmbPriDev;
	CCompareCmb	m_cmbSecDev;
	CCompareCmb	m_cmbStation;
	CButton	m_btnContinue;
	CButton	m_btnQuery;
	CListCtrlEx	m_List;
	CString	m_sQueryCondition;
	CString	m_sQueryResult;
	//}}AFX_DATA

// Attributes
public:
	MAP_VOLTAGE m_VoltageMap;

private:
	//##ModelId=49B87B830272
	CThemeHelperST m_ThemeHelper;
	//##ModelId=49B87B830277
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息

	/** @brief           事故报告链表*/
	//##ModelId=49B87B83027C
	FAULTREPORT_LIST		m_listFaultReport;
    /** @brief           选择的地区*/
	CString m_sSelNet;
	/** @brief           选择的厂站ID*/
	//##ModelId=49B87B830280
	CString m_sSelStationID;
	/** @brief           电压等级*/
	int m_nSelVoltage;
	/** @brief           设备类型*/
	int m_nSelDeviceType;
	/** @brief           选择的一次设备ID*/
	//##ModelId=49B87B830281
	CString m_sSelPriID;
	/** @brief           选择的二次设备*/
	CString m_sSelSecDevice;
	/** @brief           选择的故障相别*/
	//##ModelId=49B87B830282
	int		m_nSelFaultType;
	/** @brief           选择的故障类型*/
	int		m_nChooseFaultType;
	/** @brief           选择的起始时间*/
	//##ModelId=49B87B830290
	CTime	m_tmSelStart;
	/** @brief           选择的结束时间*/
	//##ModelId=49B87B830291
	CTime	m_tmSelEnd;
	/** @brief           是否存档*/
	int m_nIsSaved;
	/** @brief           是否是典型故障*/
	int m_nIsTypicalcase;
	int m_nOverhaul;

	/** @brief           之前被排序的列*/
	//##ModelId=49B87B830292
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
	//##ModelId=49B87B830293
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

/*
 *  @brief   	IssueToWeb	 将指定的故障包发布到WEB 
 *  @param 		[In]a param of Type  CFaultReport*  指定要发布的故障包
 *  @return 	int	发布结果 0-成功. 1-通讯服务器未连接. 2-发送报文失败
 */
	//##ModelId=49B87B850022
	int IssueToWeb(CFaultReport* pReport);
private:
		/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8302A2
	int InitListCtrl();

/*
 *  @brief   	InitControls	 初始化其它控件
 *  @return 	int  失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B8302AF
	int InitControls();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8302BF
	void RegulateControlSize();

/*
 *  @brief   	ShowResult	 显示查询结果 
 *  @param 		[In]a param of Type  BOOL  是否显示查询结果
 *  @return 	void 
 */
	//##ModelId=49B87B8302CE
	void ShowResult(BOOL bShow);

	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8302DE
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8302EE
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8302EF
	BOOL NeedSave();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8302FD
	int	InitListStyle();

/*
 *  @brief   	FillNet     填充地区下拉框 
 *  @return 	void 
 */
	void FillNet();

/*
 *  @brief   	FillStation	 填充厂站下拉框 
 *  @param 		[In]a param of Type  CNetObj*  一次设备所属厂站,为空表示不限制
 *  @return 	void 
 */
	//##ModelId=49B87B83030D
	void FillStation(CNetObj* pNet = NULL);

/*
 *  @brief   	FillVoltage 填充电压等级
 *  @param      [In]a param of Type  CStationObj*  厂站指针
 *  @return 	void 
 */
	void FillVoltage(CStationObj* pStationObj);

/*
 *  @brief   	FillDeviceType 填充设备类型 
 *  @return 	void 
 */	
	void FillDeviceType();

/*
 *  @brief   	FillPriDevice	 填充一次设备下拉框 
 *  @param 		[In]a param of Type  CStationObj*  一次设备所属厂站,为空表示不限制
 *  @param 		[In]a param of Type  int  电压等级
 *  @param      [In]a param of Type  int  设备类型
 *  @return 	void 
 */
	//##ModelId=49B87B83030E
	void FillPriDevice(CStationObj* pStation, int nVoltage, int nDeviceType);
	
/*
 *  @brief   	FillSecDevice	 填充二次设备下拉框 
 *  @param 	    [In]a param of Type  CStationObj*  一次设备所属厂站,为空表示不限制
 *  @param      [In]a param of Type  CDeviceObj*   一次设备指针
 *  @param      [In]a param of Type  int           电压等级
 *  @param      [In]a param of Type  int           设备类型
 *  @return 	void 
 */	
	void FillSecDevice(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType);

/*
 *  @brief   	FillSpecificPriDevice	 将指定厂站下的指定类型的一次设备填充到指定下拉框 
 *  @param 		[In]a param of Type  CComboBox*  下拉框指针
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  int  一次设备类型
 *  @param      [In]a param of Type  int  电压等级
 *  @return 	void 
 */
	//##ModelId=49B87B83031C
	void FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType, int nVoltage);

/*
 *  @brief   	FillSecDeviceMap	 将指定厂站下的二次设备填充到指定下拉框
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  CDeviceObj*  一次设备指针
 *  @param 		[In]a param of Type  int          电压等级
 *  @param      [In]a param of Type  int          设备类型
 *  @return 	void 
 */
	void FillSecDeviceMap(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType);	

/*
 *  @brief   	FillFaultType	 填充故障相别下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87B83032E
	void FillFaultType();
	
	/*
 *  @brief   	FillFaultType	 填充故障类型下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87B83032E
	void FillFaultType2();
/*
 *  @brief   	FillStartTime	 填充起始时间 
 *  @return 	void 
 */
	//##ModelId=49B87B83033C
	void FillStartTime();

/*
 *  @brief   	GetAllCurSel	 取得所有控件的当前选择值 
 *  @return 	void 
 */
	//##ModelId=49B87B83033D
	void GetAllCurSel();


/*
 *  @brief   	QuerySpecificPriDevice	 查询一次设备故障归档包
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE 
 */
	BOOL QuerySpecificPriDevice();

/*
 *  @brief   	QueryFaultReport	 查询故障报告 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B83035B
	BOOL QueryFaultReport();

/*
 *  @brief   	Clear	 清空原来的数据 
 *  @return 	void 
 */
	//##ModelId=49B87B83036B
	void Clear();

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	void 
 */
	//##ModelId=49B87B83037A
	void FillListData();

/*
 *  @brief   	AddReportToList	 把故障报告加入到列表框中显示 
 *  @param 		[In]a param of Type  CFaultReport*  要加入显示的故障报告对象
 *  @param 		[In]a param of Type  int  要加入的位置索引
 *  @return 	void 
 */
	//##ModelId=49B87B83037B
	void AddReportToList(CFaultReport* pReport, int nIndex);

/*
 *  @brief   	ChangedValueInList	 改变指定故障报告在列表中的显示 
 *  @param 		[In]a param of Type  CFaultReport*  指定故障报告
 *  @param 		[In]a param of Type  int  指定索引
 *  @return 	void 
 */
	//##ModelId=49B87B850003
	void ChangedValueInList(CFaultReport* pReport, int nIndex = -1);

/*
 *  @brief   	InitVoltageMap	 初始化电压等级Map
 *  @return 	void 
 */	
	void InitVoltageMap();
	void InitVoltageMap(CString strNetID, MAP_VOLTAGE* VoltageMap);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFaultQuery)
	public:
	//##ModelId=49B87B83038B
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B830399
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8303B9
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8303D8
	virtual ~CViewFaultQuery();
#ifdef _DEBUG
	//##ModelId=49B87B84000F
	virtual void AssertValid() const;
	//##ModelId=49B87B84001F
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewFaultQuery)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnBtnFaultQuery();
	afx_msg void OnBtnFaultContinue();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbFaultStation();
	afx_msg void OnItemchangedListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaultPrint();
	afx_msg void OnFaultEdit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFaultWeb();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbFaultNet();
	afx_msg void OnSelchangeCmbFaultOne();
	afx_msg void OnSelchangeCmbFaultDeviceType();
	afx_msg void OnSelchangeCmbFaultVoltage();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFaultExport();
	afx_msg void OnManualFaultShorten();
	afx_msg void OnFaultDelete();
	afx_msg void OnSelchangeCmbFaultType();
	afx_msg void OnSelchangeCmbFaultType2();
	//}}AFX_MSG
	afx_msg void OnFaultReportEdited(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
/*
 *  @brief   	GetVoltageCmbValue	 取得电压等级下拉框所选值
 *  @return 	int                  电压等级值 
 */
	inline int GetVoltageCmbValue();

/*
 *  @brief   	FillAllPriDevice	 在下拉框中添加所有一次设备
 *  @param 		[In]a param of Type  CComboBox*  指定下拉框
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  int  设备类型
 *  @param 		[In]a param of Type  int  电压等级
 *  @return 	void                  
 */
	inline void FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

/*
 *  @brief   	FillPriDeviceByType	 根据一次设备类型在下拉框中添加一次设备
 *  @param 		[In]a param of Type  CComboBox*  指定下拉框
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  int  设备类型
 *  @param 		[In]a param of Type  int  电压等级
 *  @return 	void                  
 */
	inline void FillPriDeviceByType(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

/*
 *  @brief   	IfHasSecDevice	 查询一次设备下是否有二次设备
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  CDeviceObj*   一次设备指针
 *  @return 	void                  
 */
    inline BOOL IfHasSecDevice(CStationObj* pStation, CDeviceObj* pDeviceObj);

void DeleteFaultShortenData(CList<int, int>&  itemList);
void DeleteFaultReport(CFaultReport* pReport);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_)
