#if !defined(AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_)
#define AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAction.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAction form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B94038A
class CPTAction : public CViewBase
{
protected:
	//##ModelId=49B87B940399
	CPTAction();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAction)

// Form Data
public:
	//{{AFX_DATA(CPTAction)
	enum { IDD = IDD_PT_ACTION };
	CButton	m_btnExcel;
	CButton	m_btnSign;
	CButton	m_btnPrint;
	CButton	m_btnDetail;
	CButton	m_btnAckAll;
	CButton	m_btnAck;
	CListCtrlEx	m_List;
	BOOL	m_bQueryAcked;
	BOOL	m_bShowDebug;
	BOOL	m_bShowReturn;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B9403CC
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	//##ModelId=49B87B9403D9
	CSecObj*	m_pObj;
	/** @brief           退出线程标志*/
	//##ModelId=49B87B9403DD
	BOOL	m_bExitThread;
	/** @brief           读取数据线程指针*/
	//##ModelId=49B87B950001
	CWinThread*		m_pLoadThread;
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B950005
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
	//##ModelId=49B87B950012
	CThemeHelperST	m_ThemeHelper;

	//##ModelId=49B87B95001F
	int		m_nMaxRow;	//事件最大显示行数
	//##ModelId=49B87B950021
	CXJEventManager	m_EventManager; //事件管理
	//##ModelId=49B87B950031
	BOOL	m_bShowAcked;	//是否显示已确认事件
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B95003E
	int		m_nOldSortCol;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B950040
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B95004F
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B95005E
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B950063
	IXJReport * pReportDoc;

	/** @brief			查询号,最大值1000，超过1000后从0开始计数*/
	int			m_nQueryNo;
private:
	CPtrList	m_listThread;
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();
	int	GetThreadCount();

// Operations
public:
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B95006D
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B95007D
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B95008C
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B95009C
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B9500AB
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B9500BB
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B9500BC
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9500CB
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAction *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B9500DB
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
 *  @brief   	RefreshAck	 简要的函数功能说明文字 
 *  @param 		[In]a param of Type  CXJEventAction*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B9500EB
	void RefreshAck(CXJEventAction* pEvent, int nIndex = -1);

/*
 *  @brief   	FindLocalPoint	 查找某条事件在本地的地址 
 *  @param 		[In]a param of Type  CXJEventAction*  要查找的事件
 *  @return 	CXJEventAction* 本地地址
 */
	//##ModelId=49B87B9500FB
	CXJEventAction* FindLocalPoint(CXJEventAction* pEvent);

/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

	void StartThread();

	int GetNextQueryNo();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 重新填入所有内容  
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9500FD
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B950109
	void EndOwnerThread();

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B95010A
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B95010B
	void ReleaseCOM();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B95010C
	CString TranslateSingleKey(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAction)
	public:
	//##ModelId=49B87B95010E
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B950128
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B950138
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B95014A
	virtual ~CPTAction();
#ifdef _DEBUG
	//##ModelId=49B87B950157
	virtual void AssertValid() const;
	//##ModelId=49B87B950167
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAction)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionAck();
	afx_msg void OnColumnclickListPtaction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionDetail();
	afx_msg void OnBtnPtactionSign();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtactionAckall();
	afx_msg void OnBtnPtactionPrint();
	afx_msg void OnCheckPtactionHide();
	afx_msg void OnCheckPtactionDebug();
	afx_msg void OnCheckPtactionReturn();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionExcel();
	//}}AFX_MSG
	//##ModelId=49B87B9501E6
	void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9501F4
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950203
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950206
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950215
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950223
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_)
