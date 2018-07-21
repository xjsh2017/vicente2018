#if !defined(AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_)
#define AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_

#include "..\..\COMMON\define.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStaInfo.h : header file
//
#include "Event.h"
#include "EventManager.h"
/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewStaInfo : public CFormView
{
protected:
	CViewStaInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewStaInfo)

// Form Data
public:
	//{{AFX_DATA(CViewStaInfo)
	enum { IDD = IDD_XJ_VIEW_STAINFO };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           页面对应的厂站对象指针*/
	CStationObj*	m_pStation;
private:

	
	/** @brief           之前被排序的列*/
	//##ModelId=49B87B850159
	int					m_nOldSortCol;
	

	/** @brief           记录定时器*/
	int					m_nTimer;

	/** @brief           打印模板标识*/
	CLSID				clsid;
	/** @brief           COM类工厂指针*/
	LPCLASSFACTORY		pClf;
	/** @brief           COM指针*/
	LPUNKNOWN			pUnk;
	/** @brief           打印模板接口指针*/
	IXJReport*			pReportDoc;
public:
	/** @brief           是否显示调试信息*/
	BOOL				m_bShowDebug;
	
	int					m_nMaxRow;
	/** @brief           是否显示已确认事件*/
	//##ModelId=49B87B850158
	BOOL				m_bShowAcked;
	/** @brief           事件管理对象*/
	//##ModelId=49B87B850153
	CXJEventManager		m_EventManager;
// Operations
public:
	BOOL NeedSave();
	BOOL SaveListConfig();
	BOOL LoadProNameFromDB();
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
 *  @brief   	RefreshDuravieTime	 刷新LIST中指定位置的持续时间显示 
 *  @param 		[In]a param of Type  int  指定在LIST中的位置
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);
/*
 *  @brief   	ChangeCommStatusInList	 改变指定厂站的通信状态在通信状态列表框中的显示 
 *  @param 		[In]a param of Type  CSecObj*  指定厂站
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeStationStatusInList(CStationObj* pObj);
/*
 *  @brief   	RefreshTime	 刷新所有保护的通信状态和运行状态的持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
		void RefreshTime();
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
 *  @brief   	AddStationInfoToList	 把一条厂站的信息加入到列表 
 *  @param 		[In]a param of Type  CSecObj *  要加入的装置
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8501B7
	BOOL AddStationInfoToList(CStationObj* pObj,CDeviceObj* pDev, int i);
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8501A5
	void RegulateControlSize();

		/*
	*  @brief   	WriteResultToExcel	将结果导出到Excel 
	*  @param 		[In]a param of Type  CString  文件路径
	*  @return 	void 
	*/
	BOOL WriteResultToExcel(CString strFullPath);
		/*
	*  @brief   	AddResultToExcel	 将指定的结果项写入到指定的Excel文件中 
	*  @param 		[In]a param of Type  CString  数据库对象
	*  @param 		[In]a param of Type  CSecObj*  设备对象
	*  @return 	void 
	*/
	//##ModelId=49B87B820021
	void AddResultToExcel(CDatabase* pDatabase, CStationObj* pObj,CDeviceObj* pDev);
// Operations


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewStaInfo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewStaInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CViewStaInfo)
	afx_msg void OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCommOn();
	afx_msg void OnCommOff();
	afx_msg void OnStationHistory();
	afx_msg void OnCommExport();
	afx_msg void OnCommPrint();
	afx_msg void OnStationTree();
	afx_msg void OnStationMap();	
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnStaInfoOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_)
