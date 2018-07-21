#if !defined(AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_)
#define AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStationInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewStationInfo : public CFormView
{
protected:
	CViewStationInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewStationInfo)

// Form Data
public:
	//{{AFX_DATA(CViewStationInfo)
	enum { IDD = IDD_XJ_VIEW_STATIONINFO };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           页面对应的厂站对象指针*/
	CStationObj*	m_pStation;
private:
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

// Operations
public:
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
 *  @brief   	AddEventToList	 把指定装置的运行状态加入到列表中显示 
 *  @param 		[In]a param of Type  CSecObj *  要加入的装置
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8103D8
	BOOL AddEventToList(CSecObj * pSec, int i);

/*
 *  @brief   	ChangeCommStatusInList	 改变指定二次设备的通信状态在通信状态列表框中的显示 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeCommStatusInList(CSecObj* pSec);

/*
 *  @brief   	ChangeRunStatusInList	 改变指定二次设备的运行状态在通信状态列表框中的显示 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	void ChangeRunStatusInList(CSecObj* pSec);

	/*
	*  @brief   	WriteResultToExcel	将结果导出到Excel 
	*  @param 		[In]a param of Type  CString  文件路径
	*  @return 	导出成功返回TRUE, 失败返回FALSE
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
	//##ModelId=49B87B8202F1
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 删除COM组件 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F2
	void ReleaseCOM();

/*
 *  @brief   	RefreshDuravieTime	 刷新LIST中指定位置的持续时间显示 
 *  @param 		[In]a param of Type  int  指定在LIST中的位置
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 刷新所有保护的通信状态和运行状态的持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
	void RefreshTime();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewStationInfo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewStationInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewStationInfo)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStationinfoSec();
	afx_msg void OnStationinfoCommH();
	afx_msg void OnStationinfoRunH();
	afx_msg void OnStationinfoExclel();
	afx_msg void OnStationinfoPrint();
	afx_msg void OnStationinfoRefresh();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnStationInfoOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_)
