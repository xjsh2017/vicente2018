#if !defined(AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_)
#define AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAlarm.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B94009C
class CPTAlarm : public CViewBase
{
protected:
	//##ModelId=49B87B9400AC
	CPTAlarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAlarm)

// Form Data
public:
	//{{AFX_DATA(CPTAlarm)
	enum { IDD = IDD_PT_ALARM };
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
	//##ModelId=49B87B9400FC
	CSecObj*	m_pObj;	//对应保护
	/** @brief           退出线程标志*/
	//##ModelId=49B87B940109
	BOOL	m_bExitThread;
	/** @brief           读取线程指针*/
	//##ModelId=49B87B94010B
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B94011A
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B94011E
	int		m_nMaxRow;	//事件最大显示行数
	//##ModelId=49B87B940129
	CXJEventManager	m_EventManager; //事件管理
	//##ModelId=49B87B94012D
	BOOL	m_bShowAcked;	//是否显示已确认事件
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B940138
	int		m_nOldSortCol;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B940149
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B94014E
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B940158
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B940168
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
	int GetThreadCount();
// Operations
public:
	//##ModelId=49B87B94016D
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B940177
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B940186
	int InitListCtrl();

		/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B940196
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B9401A5
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B9401B5
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B9401C5
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B9401C6
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B9401D4
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9401E4
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAlarm *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B9401F4
	BOOL AddEventToList(CXJEventAlarm * pEvent, int i);

/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventAlarm*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	//##ModelId=49B87B940203
	void RefreshAck(CXJEventAlarm* pEvent, int nIndex = -1);

/*
 *  @brief   	FindLocalPoint	 查找某条事件在本地的地址 
 *  @param 		[In]a param of Type  CXJEventAlarm*  要查找的事件
 *  @return 	CXJEventAlarm* 本地地址
 */
	//##ModelId=49B87B940213
	CXJEventAlarm* FindLocalPoint(CXJEventAlarm* pEvent);

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
 *  @brief   	ReFillAll	 重新填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B940215
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B940216
	void EndOwnerThread();

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B940222
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B940223
	void ReleaseCOM();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B940224
	CString TranslateSingleKey(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAlarm)
	public:
	//##ModelId=49B87B940226
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B940232
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B940251
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B940261
	virtual ~CPTAlarm();
#ifdef _DEBUG
	//##ModelId=49B87B940271
	virtual void AssertValid() const;
	//##ModelId=49B87B940280
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAlarm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmAck();
	afx_msg void OnColumnclickListPtalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmDetail();
	afx_msg void OnBtnPtalarmSign();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtalarmAckall();
	afx_msg void OnBtnPtalarmPrint();
	afx_msg void OnCheckPtalarmHide();
	afx_msg void OnCheckPtalarmDebug();
	afx_msg void OnCheckPtalarmReturn();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmExport();
	//}}AFX_MSG
	//##ModelId=49B87B9402F2
	void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9402FD
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94030D
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94031C
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94031F
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94032D
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_)
