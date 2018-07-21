// ***************************************************************
//  ViewFault   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 最新事故报告页面
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_)
#define AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFault.h : header file
//
#include "FaultReport.h"

/** @brief           事故报告链表*/
typedef CTypedPtrList<CObList , CFaultReport*>	FAULTREPORT_LIST;
/////////////////////////////////////////////////////////////////////////////
// CViewFault form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



//##ModelId=49B87B84033C
class CViewFault : public CFormView
{
protected:
	//##ModelId=49B87B84033E
	CViewFault();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFault)

// Form Data
public:
	//{{AFX_DATA(CViewFault)
	enum { IDD = IDD_XJ_VIEW_FAULT };
	//##ModelId=49B87B84034C
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B84035C
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           事故报告链表*/
	//##ModelId=49B87B840361
	FAULTREPORT_LIST		m_listFaultReport;
	/** @brief           最大显示数*/
	//##ModelId=49B87B84036B
	int		m_nMaxRow;
	/** @brief           之前被排序的列*/
	//##ModelId=49B87B84036C
	int		m_nOldSortCol;

	/** @brief			是否只显示非检修故障包*/
	BOOL	m_bOnlyShowNoOverhual;
	/** @brief			是否只显示存档故障包*/
	BOOL	m_bOnlyShowSaved;
	/** @brief			是否只显示典型故障*/
	BOOL	m_bOnlyShowTypical;

public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B84037A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

// Operations
public:
/*
 *  @brief   	IssueToWeb	 将指定的故障包发布到WEB 
 *  @param 		[In]a param of Type  CFaultReport*  指定要发布的故障包
 *  @return 	int	发布结果 0-成功. 1-通讯服务器未连接. 2-发送报文失败
 */
	//##ModelId=49B87B850022
	int IssueToWeb(CFaultReport* pReport);
private:

void DeleteFaultReport(CFaultReport* pReport);
BOOL DeleteFaultShortenData(CFaultReport* pReport);
void DeleteFaultShortenData(CList<int, int>&  itemList);
BOOL DeleteFaultShortenEvents(CFaultReport* pReport);
BOOL DeleteFaultAnalyData(CFaultReport* pReport);

	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B84038A
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B84038B
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B840399
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8403A9
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8403B9
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8403C8
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8403C9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 从数据库中载入数据
 *  @param 		[In]a param of Type  int  载入个数
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8403D8
	BOOL LoadDataFromDB(int nNum);

/*
 *  @brief   	Clear	 清除之前的故障报告信息 
 *  @return 	void 
 */
	//##ModelId=49B87B8403DA
	void Clear();

/*
 *  @brief   	AddReportToList	 把故障报告加入到列表框中显示 
 *  @param 		[In]a param of Type  CFaultReport*  要加入显示的故障报告对象
 *  @param 		[In]a param of Type  int  要加入的位置索引
 *  @return 	void 
 */
	//##ModelId=49B87B850000
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
 *  @brief   	FindFaultReport	 查找指定的故障报告是否有在内存中 
 *  @param 		[In]a param of Type  int  指定ID
 *  @return 	CFaultReport* 查找成功返回指针, 失败返回NULL
 */
	//##ModelId=49B87B850010
	CFaultReport* FindFaultReport(int nID);

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B850012
	void CheckLineCount();

/*
 *  @brief   	RemoveFromMem	 将指定故障报告从内存和链表中删除 
 *  @param 		[In]a param of Type  CFaultReport*  指定故障报告
 *  @return 	void 
 */
	//##ModelId=49B87B850013
	void RemoveFromMem(CFaultReport* pReport);

/*
 *  @brief   	RemoveFromList	 将指定故障报告从列表框中删除 
 *  @param 		[In]a param of Type  CFaultReport*  指定故障报告
 *  @return 	void 
 */
	//##ModelId=49B87B850020
	void RemoveFromList(CFaultReport* pReport);

/*
 *  @brief   	QueryLatsetID	 查询最新的故障包ID 
 *  @return 	int 最新的故障包ID
 */
	int QueryLatsetID();

/*
 *  @brief   	CreateFaultReport	 新建故障包 
 *  @param 		[In]a param of Type  int  包ID
 *  @return 	CFaultReport* 新创建的故障包
 */
	CFaultReport* CreateFaultReport(int nID);

/*
 *  @brief   	AddReportToDB	 将故障包加入到数据库,事件不处理
 *  @param 		[In]a param of Type  CFaultReport*  指定故障包
 *  @return 	BOOL 加入成功返回TRUE,失败返回FALSE
 */
	BOOL AddReportToDB(CFaultReport* pReport);

	int RefreshData();

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

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFault)
	public:
	//##ModelId=49B87B850024
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B85002F
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B85004E
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B85005D
	virtual ~CViewFault();
#ifdef _DEBUG
	//##ModelId=49B87B85006D
	virtual void AssertValid() const;
	//##ModelId=49B87B85007D
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewFault)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaultPrint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFaultEdit();
	afx_msg void OnFaultWeb();
	afx_msg void OnFaultRefresh();
	afx_msg void OnFaultCreate();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFaultDelete();
	afx_msg void OnFaultExport();
	afx_msg void OnManualFaultShorten();
	afx_msg void OnFaultOnlyshowoverhaul();
	afx_msg void OnFaultOnlyshowsaved();
	afx_msg void OnFaultOnlyshowtypical();
	//}}AFX_MSG
	//##ModelId=49B87B8500DC
	afx_msg void OnFaultReportEdited(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8500E0
	afx_msg void OnSTTP20151(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

		friend class CManualFaultShorten;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_)
