#if !defined(AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_)
#define AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSOE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSOE form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewSOE : public CFormView
{
protected:
	CViewSOE();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSOE)

// Form Data
public:
	//{{AFX_DATA(CViewSOE)
	enum { IDD = IDD_XJ_VIEW_SOE };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           视图类型。0-I类信息。 1-II类信息。2-III类信息。3-调试信息*/
	int		m_nType;
	/** @brief           记录之前被排序的列*/
	int		m_nOldSortCol;
private:
	CXJEventManager	m_EventManager;	//事件管理器
	int	m_nMaxRow;	//最大行数
	BOOL m_bShowAcked; //是否显示已确认事件
	/** @brief           是否显示调试信息*/
	BOOL m_bShowDebug;
	/** @brief           是否显示返回信息*/
	BOOL m_bShowReturn;

// Operations
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B86036C
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B86037A
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

	/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8603A9
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8603B9
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87B8603C8
	BOOL NeedSave();

/*
 *  @brief   	AddEventToList	 将指定事件加入到列表中显示 
 *  @param 		[In]a param of Type  CXJEvent *  指定事件
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 成功返回TRUE，失败返回FALSE
 */
	BOOL AddEventToList(CXJEvent * pEvent, int i = 0);

		/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8603C9
	void RegulateControlSize();

/*
 *  @brief   	LoadActionFromDB	 从数据库读取动作数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadActionFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadAlarmFromDB	 从数据库读取告警数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadAlarmFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadDIFromDB	 从数据库读取开关量数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  BOOL  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDIFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  BOOL  是否要查询已确认的事件
 *  @return 	void 
 */
	void LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEvent*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B87000F
	void RefreshAck(CXJEvent* pEvent, int nIndex = -1);

/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEvent*  被确认的事件
 *	@param		[In]a param of Type	 BOOL 是否被动确认
 *  @return 	void 
 */
	//##ModelId=49B87B87030D
	void TellOtherWndAck( CXJEvent* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

/*
 *  @brief   	DisplayValue	 得到开关量的值显示字符串 
 *  @param 		[In]a param of Type  int  开关量值
 *  @return 	CString 显示值
 */
	//##ModelId=49B87B8501D6
	CString DisplayValue(int nValue);

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

	int RefreshData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSOE)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSOE();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSOE)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventFilterDebug();
	afx_msg void OnDblclkListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventFilterReturn();
	//}}AFX_MSG
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_)
