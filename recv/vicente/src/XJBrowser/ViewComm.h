// ***************************************************************
//  ViewComm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 主子站通讯状态详细列表视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_)
#define AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewComm.h : header file
//

class Record : public CObject
{
public:
	CString m_sName;
	int     m_nCommStatus;
	CString sTime;
    CString m_sDurativeTime;
    CString m_sReason;
	CString m_sReasonTime;
	int     m_nCommStatus1;
	CString sTime1;
    CString m_sDurativeTime1;
	CString m_sReason1;
	CString m_sReasonTime1;
	CString m_sManufacturer;
	int     m_nIPFlag;
    Record(CStationObj* pObj);
};

typedef CTypedPtrList< CObList, Record* > RecordList; 
class CommStatusList : public CObject
{
public:
	HTREEITEM hItem;
	CStationObj* pStationObj;
	RecordList recordList;
	CommStatusList();
};

typedef CMapStringToPtr CStationPosMap;

//召唤厂站通讯状态
typedef struct StationStauts_Call
{
	/** @brief           厂站*/
	CStationObj* pStation;
	/** @brief           通道*/
	int nChannel;
	/** @brief           0:成功. -1:发送报文失败. -2:收到20069. -3:超时*/
	int nRelust;
}StationStauts_Call;

/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B860033
int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

/////////////////////////////////////////////////////////////////////////////
// CViewComm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EventManager.h"

//##ModelId=49B87B8503C8
class CViewComm : public CFormView
{
protected:
	//##ModelId=49B87B8503CA
	CViewComm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewComm)

// Form Data
public:
	//{{AFX_DATA(CViewComm)
	enum { IDD = IDD_XJ_VIEW_COMM };
	//##ModelId=49B87B860001
	COXTreeCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
    /** @brief           场站位置Map*/
    CStationPosMap       m_mapStationPos;
	/** @brief           列表的列信息*/
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           退出线程标志*/
	BOOL	m_bExitThread;

	CMenu * pMenu;
private:
	/** @brief           事件最大显示行数*/
	int		m_nMaxRow;
	/** @brief           事件管理*/
	CXJEventManager	m_EventManager;
	/** @brief           记录之前被排序的列*/
	int		m_nOldSortCol;
	/** @brief           记录定时器*/
	int		m_nTimer;

	/** @brief           打印模板标识*/
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	IXJReport * pReportDoc;

	/** @brief           查询计时器*/
	int m_nQueryTimer;
	/** @brief           召唤列表*/
	CTypedPtrArray<CPtrArray, StationStauts_Call*> m_arrCall;
	/** @brief           召唤结果*/
	CString m_sResult;

	CWinThread*	m_pLoadDataThread;
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
// Operations


private:
   /*
    *  @brief   	GetStatus 取得某条记录的通断状态
    *  @return 	BOOL 
	*/
	int GetStatus(Record* pRecord, int* nCut);
    int GetStatus(Record* pRecord);
    int GetStatus(CStationObj* pRecord);
    int GetStatus(int nCommStatus, int nCommStatus1, int nIPFlag);
	
	/*
	*  @brief   	InitListCtrl	 初始化列表 
	*  @return 	int		失败返回-1, 成功返回>=0
	*/
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	int	InitListStyle();
	
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	BOOL NeedSave();
	
   /*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	void RegulateControlSize();
	
   /*
	*  @brief   	AddStationToList	 把一个厂站的状态信息加入到列表 
	*  @param 		[In]a param of Type  CStationObj *  要加入的厂站
	*  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
	*/
	BOOL AddStationToList(CStationObj* pStation);

	
   /*
	*  @brief   	InsertStationInfo       插入厂站节点信息 
	*  @param 		[In]a param of Type  HTREEITEM 设置信息节点
	*  @return     void 
	*/
    BOOL InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, POSITION pos);
    void InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, int nLastRecordStatus, POSITION pos);
	
   /*
	*  @brief   	SetStationInfo       设置厂站节点信息 
	*  @param 		[In]a param of Type  HTREEITEM 设置信息节点
	*  @return     void 
	*/
	void SetStationInfo(CStationObj* pStation, HTREEITEM hItem, BOOL bChildItem);
	
   /*
	*  @brief   	SetStationInfo       设置厂站节点信息 
	*  @param 		[In]a param of Type  HTREEITEM 设置信息节点
	*  @return     void 
	*/
    void SetStationInfo(Record* record, HTREEITEM hItem, BOOL bChildItem);
	
	/*
	*  @brief   	SetStationInfo       设置厂站节点信息 
	*  @param 		[In]a param of Type  HTREEITEM 设置信息节点
	*  @return     void 
	*/	
	BOOL SetStationInfo(CommStatusList* pList);
	
	/*
	*  @brief   	IsNeedShow	 判断当前通信状态的厂站信息是否需要显示 
	*  @param 		[In]a param of Type  int  通信状态
	*  @return 	BOOL 需要显示返回TRUE, 不需要显示返回FALSE
	*/
	BOOL IsNeedShow(int nStatus);
	
	/*
	*  @brief   	WriteResultToExcel	将结果导出到Excel 
	*  @param 		[In]a param of Type  CString  文件路径
	*  @return 	void 
	*/
	BOOL WriteResultToExcel(CString strFullPath);
	
	/*
	*  @brief   	AddResultToExcel	 将指定的结果项写入到指定的Excel文件中 
	*  @param 		[In]a param of Type  CString  数据库对象
	*  @param 		[In]a param of Type  CStationObj*  设备对象
	*  @return 	void 
	*/
	void AddResultToExcel(CDatabase* pDatabase, CStationObj* pStation);
	
	/*
	*  @brief   	InitCOM	 初始化打印模板COM组件 
	*  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
	*/
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 删除COM组件 
	*  @return 	void 
	*/
	void ReleaseCOM();
	
	/*
	*  @brief   	QueryStationStatus	 查询厂站通讯状态 
	*  @param 		[In]a param of Type  CStationObj*  指定厂站
	*  @param 		[In]a param of Type  int  指定通道
	*  @return 	int 失败返回<0的整数，成功返回0 
	*/
	int QueryStationStatus(CStationObj* pObj, int nChannel = 0);
	
	void TestStationStatus(CStationObj* pStation);
	
	/*
	*  @brief   	ClearCallList	 清空召唤列表 
	*  @return 	void 
	*/
	void ClearCallList();
	
	/*
	*  @brief   	AddToCallList	 将指定厂站指定通道加入到待召唤列表 
	*  @param 		[In]a param of Type  CStationObj*  指定厂站
	*  @param 		[In]a param of Type  int  指定通道
	*  @return 	void 
	*/
	void AddToCallList(CStationObj* pObj, int nChannel);
	
	/*
	*  @brief   	StartCall	 对召唤项进行召唤 
	*  @param 		[In]a param of Type  StationStauts_Call*  指定召唤项
	*  @return 	void 
	*/
	void StartCall(StationStauts_Call* pCall);
	
	/*
	*  @brief   	ClearStationPosMap	 清空StationPosMap
	*  @param 		
	*  @return 	void 
	*/
	void ClearStationPosMap();
	
    
	/*
	*  @brief   	ReadDetailReason	 读取断开详细原因
	*  @param 		
	*  @return 	void 
	*/
	void ReadStatusData(CStationObj* pStationObj, CommStatusList** pList);

	/*
	*  @brief   	ClearList	 清楚记录
	*  @param 		
	*  @return 	void 
	*/
	void ClearRecord(CommStatusList* pList);
    void ClearRecord(CStationObj* pStationObj, BOOL bClearAll);
	void ClearRecord(CommStatusList* pList, BOOL bCon);
	/*
	*  @brief   	AddRecordToList	 添加记录
	*  @param 		
	*  @return 	void 
	*/	
	BOOL AddRecordToList(CStationObj* pStationObj, CommStatusList** pList);
	BOOL AddRecordToList(CommStatusList* pList);
	
	/*
	*  @brief   	SetListData	 设置列表数据
	*  @param 		
	*  @return 	void 
	*/
	void SetListData();

	void StartThread();
	void EndThread();
	int FillListData_Multi();
	
public:
	void NotifyThreadEnd();

	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	int FillListData();
   /*
    *  @brief   	RefreshDuravieTime	 刷新LIST中指定位置的持续时间显示 
    *  @param 		[In]a param of Type  HTREEITEM  指定在LIST中的位置
    *  @return 	void 
	*/
	void RefreshDuravieTime(HTREEITEM hItem, CStationObj* pStationObj);
	
   /*
	*  @brief   	RefreshTime	 刷新所有厂站的持续时间 
	*  @return 	void 
	*/
	void RefreshTime();
	
	/*
	*  @brief   	TranslateSingleKey	 解释单个关键字的意义 
	*  @param 		[In]a param of Type  CString  关键字
	*  @return 	CString 关键字表示的值
	*/
	CString TranslateSingleKey(CString sKey);
	void LoadStatusFromDB(CStationObj* pObj, CommStatusList** pList);
	
	void RefreshStatusDetail();
	
	void LoadDetailDataFromDB();

	BOOL LoadAllDataFromDB();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewComm)
public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL IsSelected(const CObject* pDocItem) const;
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewComm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewComm)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnStationHistory();
	afx_msg void OnStationMap();
	afx_msg void OnStationTree();
	afx_msg void OnColumnclickListComm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnClickListComm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCommOn();
	afx_msg void OnCommOff();
	afx_msg void OnCommExport();
	afx_msg void OnCommPrint();
	afx_msg void OnCommandItem(UINT nID);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParma, LPARAM lParam);
	afx_msg void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndCallStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTestStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_)
