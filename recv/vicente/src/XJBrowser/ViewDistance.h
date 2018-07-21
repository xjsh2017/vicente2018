#if !defined(AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_)
#define AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDistance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDistance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewDistance : public CFormView
{
protected:
	CViewDistance();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDistance)

// Form Data
public:
	//{{AFX_DATA(CViewDistance)
	enum { IDD = IDD_XJ_VIEW_DISTANCE };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief			事件管理器*/
	CXJEventManager		m_EventManager;
	/** @brief           最大显示行数*/
	int					m_nMaxRow;
	/** @brief           是否显示已确认事件*/
	BOOL				m_bShowAcked;
	int					m_nOldSortCol;

/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B85015A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
// Operations
private:
	/*
	* @brief	InitListCtrl 初始化列表框	
	* @return   int	失败返回<=0, 成功返回>0
	*/
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回<=0, 成功返回>0
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
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

	/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventDistance *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	BOOL AddEventToList(CXJEventDistance * pEvent, int i);

	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回<=0, 成功返回>0
	*/
	int FillListData();

	/*
	* @brief	RefreshData	刷新数据，负责清空数据、从数据库读入、填充到列表框
	* @return   int	失败返回<=0, 成功返回>0
	*/
	int RefreshData();

	/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventDistance*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	void RefreshAck(CXJEventDistance* pEvent, int nIndex = -1);

	/*
 *  @brief   	TellOtherWndAck	 告诉其它窗口这里确认了事件 
 *  @param 		[In]a param of Type  CXJEventDistance*  被确认的事件
 *  @return 	void 
 */
	void TellOtherWndAck( CXJEventDistance* pEvent );

	/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
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

	CXJEventDistance* FindLocalPoint(CXJEventDistance* pEvent);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDistance)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewDistance();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDistance)
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventProp();
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventHide();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListDistance(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	void OnEventPropAckPT(WPARAM wParma, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_)
