// ***************************************************************
//  BatchDetailSettingView   version:  1.0    date: 3:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 批量召唤设备选择及各设备的召唤选项
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
#define AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchDetailSettingView.h : header file
//
#include "StdAfx.h"
#include "ViewBase.h"
#include "BatchSel.h"
#include "ReportCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailSettingView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



/**
 * @defgroup 	CBatchDetailSettingView	批量召唤设备设置类
 * @{
 */
/**
 * @brief       描述批量召唤设备选择及各设备的召唤选项
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
class CBatchDetailSettingView : public CViewBase
{
protected:
	//##ModelId=49B87BB103DA
	CBatchDetailSettingView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBatchDetailSettingView)

// Form Data
public:
	//{{AFX_DATA(CBatchDetailSettingView)
	enum { IDD = IDD_BATCH_DETAIL_SETTING };
	CButton         	m_btnCallResult;
	CButton 	        m_btnStartCall;
	CButton         	m_btnCallStop;
	CButton         	m_btnAutoClose;
	CButton          	m_btnDeviceList;
	CButton	m_btnReset;
	CButton	m_btnSet;
	CButton	m_btnSelWR;
	CButton	m_btnSelPT;
	CButton	m_btnSave;
	
	CPictureEx	m_gif;
	CReportCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           自动召唤第一次启动时间*/
	//##ModelId=49B87BB20072
	CString		m_sStartTime;
	/** @brief           自动召唤周期*/
	//##ModelId=49B87BB2007D
	CString		m_sCyc;
	/** @brief           视图类型, 0表示是自动总召的设置视图. 1表示是批量召唤视图*/
	//##ModelId=49B87BB20119
	int		m_nViewType;

private:
	//##ModelId=49B87BB2008D
	CThemeHelperST	m_ThemeHelper;
	/** @brief           列表的列信息*/
	//##ModelId=49B87BB20092
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           选择项链表*/
	//##ModelId=49B87BB2009
	BATCHSEL_MAP		m_mapSel;
	/** @brief           存库链表*/
    BATCHSEL_MAP        m_mapSave;
	/** @brief           定值区号是否召唤*/
	BOOL	m_bZone;
	/** @brief           定值是否召唤*/
	//##ModelId=49B87BB200AB
	BOOL	m_bSetting;
	/** @brief           开关量是否召唤*/
	//##ModelId=49B87BB200AC
	BOOL	m_bDigital;
	/** @brief           软压板是否召唤*/
	//##ModelId=49B87BB200BB
	BOOL	m_bSoftBoard;
	/** @brief           模拟量是否召唤*/
	//##ModelId=49B87BB200CB
	BOOL	m_bAnalog;

	/** @brief           菜单项弹出前记录是否召唤定值区号*/
	BOOL	m_bMenuZone;
	/** @brief           菜单项弹出前记录是否召唤定值*/
	//##ModelId=49B87BB200CC
	BOOL	m_bMenuSetting;
	/** @brief           菜单项弹出前记录是否召唤开关量*/
	//##ModelId=49B87BB200DA
	BOOL	m_bMenuDigital;
	/** @brief           菜单项弹出前记录是否召唤软压板*/
	//##ModelId=49B87BB200DB
	BOOL	m_bMenuSoftBoard;
	/** @brief           菜单项弹出前记录是否召唤模拟量*/
	//##ModelId=49B87BB200EA
	BOOL	m_bMenuAnalog;

	/** @brief           当前状态, 0-正在进行设备选择. 1-正在进行召唤*/
	//##ModelId=49B87BB200EB
	int		m_nCuruentStatus;

	/** @brief           保存与自动总召模块的连接句柄*/
	//##ModelId=49B87BB2010A
	XJHANDLE	m_pAutoCall;

	/** @brief           记录之前被排序的列 */
	//##ModelId=49B87BB2011A
	int		m_nOldSortCol;

	/** @brief           当前可操作厂站*/
	CStationObj* m_pActiveStation;

	/** @brief			定时器ID*/
	int m_nTimer;
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

/*
 *  @brief   	SetCyc	 设置自动总召周期 
 *  @param 		[In]a param of Type  CString  第一次启动时间
 *  @param 		[In]a param of Type  CString  周期
 *  @return 	void 
 */
	//##ModelId=49B87BB20138
	void SetCyc(CString sStartTime, CString sCyc);

private:
	BOOL HasSelected();
	void CheckCallSetting(CBatchSel* pBatchSel);
	void UpdateBatchSetting(int nItem, int nSubItem);
	void UpdateCheckState(CObject* pObj, BOOL bCheck);
	void InsertSingleCPU(WPARAM wParam, LPARAM lParam);
	void InsertSecDevice(WPARAM wParam, LPARAM lParam);
	void InsertCPU(WPARAM wParam, LPARAM lParam);
	void InsertWaverc(WPARAM wParam, LPARAM lParam);
	void InsertStation(WPARAM wParam, LPARAM lParam);
	void InsertDeviceType(WPARAM wParam, LPARAM lParam);
	void InsertPriDevice(WPARAM wParam, LPARAM lParam);
	void InsertNet();

	inline void InsertItem(CString strName, CSel* pBatchSel, int iCPU = -1);
	void AddBatchSel(CBatchSel *pSel);
	void AddBatchSel(CSecObj* pSecObj);
	void AddBatchSel(CStationObj* pStationObj);

	BatchSel* FindNetBatchSel(CString strNetName);
	BatchSel* FindStationBatchSel(CString strStationID, NET_MAP* pNetMap);
    BatchSel* FindStationBatchSel(CString strNetName, CString strStationID);
	BatchSel* FindDeviceTypeBatchSel(int iDeviceType, STATION_MAP* pStationMap);
    BatchSel* FindDeviceTypeBatchSel(CString strNetName, CString strStationID, int iDeviceType);
	BatchSel* FindPriDeviceBatchSel(CString strPriDeviceName, DEVICETYPE_MAP* pDeviceTypeMap);
    BatchSel* FindPriDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName);
	BatchSel* FindSecDeviceBatchSel(CString strSecDeviceName, PRIDEVICE_MAP* pPriDeviceMap);
    BatchSel* FindSecDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName);
	CBatchSel* FindSecCPUBatchSel(int iCPU, BatchSel* pBatchSel);
    CBatchSel* FindSecCPUBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName, int iCPU);

	void InsertNetBatchSel(CBatchSel *pSel);  
	void InsertStationBatchSel(CBatchSel *pSel, NET_MAP* pNetMap);
	void InsertDeviceTypeBatchSel(CBatchSel *pSel, STATION_MAP* pStationMap);
	void InsertPriDeviceBatchSel(CBatchSel *pSel, DEVICETYPE_MAP* pDeviceTypeMap);
	void InsertSecDeviceBatchSel(CBatchSel *pSel, PRIDEVICE_MAP* pPriDeviceMap);
	void InsertSecCPUBatchSel(CBatchSel* pSel, BATCH_LIST* pBatchSel);
    void SetBatchSel(int nSubItem, CSel* pSel, int nValue);
	void SetBatchSels(CSel* pSel, int nSubItem, int nValue);

	void ClearBatchSel();
    void ClearBatchSelMap(CObject* pObj);
	void ClearBatchSelArray(BATCH_LIST* pList);


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
 *  @brief   	UpdateControlsEnable	 更改控件可用性 
 *  @param 		[In]a param of Type  BOOL	TRUE-控件可用, FALSE-控件不可用  
 *  @return 	void 
 */
	//##ModelId=49B87BB2035E
	void UpdateControlsEnable(BOOL bEnable);

/*
 *  @brief   	NotifySelectView	 通知设备树选择视图让控件禁用/可用 
 *  @param 		[In]a param of Type  BOOL  TRUE-控件可用. FALSE-控件不可用
 *  @return 	void 
 */
	//##ModelId=49B87BB2036B
	void NotifySelectView(BOOL bEnable);

/*
 *  @brief   	SaveSelData	 保存数据到数据库
 *  @param 		[In]a param of Type  int  是自动总召配置还是批量召唤配置.0-自动总召. 1-批量召唤
 *  @return 	BOOL 
 */
	//##ModelId=49B87BB2037A
	BOOL SaveSelData(int nIs_OC);

/*
 *  @brief   	DeleteSelData	 清除自动总召配置中的数据 
 *  @param 		[In]a param of Type  int  0-删除自动总召的数据. 1-删除批量召唤的数据
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB2038A
	BOOL DeleteSelData(int nIs_OC);

/*
 *  @brief   	NotifyOC	 通知自动总召模块进行操作 
 *  @param 		[In]a param of Type  int  0-召唤. 1-取消召唤
 *  @return 	BOOL 通知成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB20399
	BOOL NotifyOC(int nAction);

/*
 *  @brief   	NotifyOCCall	 通知自动总召模块进行召唤操作 
 *  @return 	BOOL 通知成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB203A9
	BOOL NotifyOCCall();

/*
 *  @brief   	NotifyOCCancelCall	 通知自动总召模块进行取消召唤操作 
 *  @return 	BOOL 通知成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB203B9
	BOOL NotifyOCCancel();

/*
 *  @brief   	MultiSaveData	 保存数据到数据库 
 *  @return 	BOOL 
 */
	//##ModelId=49B87BB203BA
	BOOL MultiSaveData(int nIs_OC, POSITION pos);


/*
 *  @brief   	ReceiveAutoCall	 等待自动总召的回应 
 *  @return 	BOOL 收到回应且可召唤返回TRUE, 未收到回应或现在不可召唤返回FALSE
 */
	//##ModelId=49B87BB203D9
	BOOL ReceiveAutoCall();

/*
 *  @brief   	AddToComplete	 把指定选择项加入到已召唤完成链表 
 *  @param 		[In]a param of Type  CBatchSel*  指定选择项
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB30003
	BOOL AddToComplete(CBatchSel* pSel);

/*
 *  @brief   	SaveAutoCallSet	 保存自动总召设置到系统配置表 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[In]a param of Type  CString  值
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB30010
	BOOL SaveAutoCallSet(CString sKey, CString sValue);

/*
 *  @brief   	GetAutoCallConfig	 在系统配置表中查询自动总召配置 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString  值
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB3001F
	BOOL GetAutoCallConfig(CString sKey, CString& sValue);


/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @return 	void 
 */
	BOOL LoadDataFromDB();


/*
 *  @brief   	SetControls	 设置显示还是隐藏
 *  @param 		[In]a param of Type  int  视图类型,0表示是自动总召的设置视图. 1表示是批量召唤视图
 *  @return 	void
 */
	//##ModelId=49B87BB203C9
	void SetControls(int nViewType);

	void StartBatchCall();

	void EndBatchCall();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchDetailSettingView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB3009C
	virtual ~CBatchDetailSettingView();
#ifdef _DEBUG
	//##ModelId=49B87BB300DA
	virtual void AssertValid() const;
	//##ModelId=49B87BB30119
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBatchDetailSettingView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnBatchSelNull();
	afx_msg void OnBtnBatchSelPt();
	afx_msg void OnBtnBatchSelWr();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnBatchSave();

	afx_msg void OnBtnBatchSet();
	afx_msg void OnClickListBatchDetailSetting(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnBatchSettingReset();
	afx_msg void OnBtnBatchSettingDeviceList();
	afx_msg void OnBtnAutoClose();
	afx_msg void OnBtnCallStop();
	afx_msg void OnBtnBatchStartAllCall();
	afx_msg void OnBtnAutoCallResult();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnBatchStartAllCall();
	afx_msg void OnBtnBatchProcess();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	//##ModelId=49B87BB302E4
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB3031C
	afx_msg void OnBatchCheck(WPARAM wParam, LPARAM lParam);

	//##ModelId=49B87BB3037B
	afx_msg void OnAutoCallSetChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAutoCallStationChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowList(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCheckChange( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBatchCallEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
