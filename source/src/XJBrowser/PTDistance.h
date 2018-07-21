#if !defined(AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_)
#define AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTDistance.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTDistance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPTDistance : public CViewBase
{
protected:
	CPTDistance();           // protected constructor used by dynamic creation
	virtual ~CPTDistance();
	DECLARE_DYNCREATE(CPTDistance)

// Form Data
public:
	//{{AFX_DATA(CPTDistance)
	enum { IDD = IDD_PT_DISTANCE };
	CListCtrlEx	m_List;
	CButton	m_btnPrint;
	CButton	m_btnDetail;
	CButton	m_btnAckAll;
	CButton	m_btnAck;
	BOOL	m_bShowAcked;
	//}}AFX_DATA

// Attributes
public:
	CSecObj*	m_pObj;	//对应保护
	/** @brief           退出线程标志*/
	BOOL	m_bExitThread;
	/** @brief           读取线程指针*/
	CWinThread*	m_pLoadThread;
	
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
private:
	int		m_nMaxRow;	//事件最大显示行数
	CXJEventManager	m_EventManager; //事件管理
	/** @brief           记录之前被排序的列*/
	int		m_nOldSortCol;
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
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
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
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	int FillListData(int nQueryNo);
	
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
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 从数据库读取数据 
 *  @param 		[In]a param of Type  int  要查找的个数
 *  @param 		[In]a param of Type  int  是否要查询已确认的事件
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

	void StartThread();

	void EndOwnerThread();

	int GetNextQueryNo();

	/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAlarm *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	BOOL AddEventToList(CXJEventDistance * pEvent, int i);

	/*
 *  @brief   	RefreshAck	 确认成功后刷新界面显示 
 *  @param 		[In]a param of Type  CXJEventAlarm*  确认的事件
 *  @param 		[In]a param of Type  int  事件位置
 *  @return 	void 
 */
	void RefreshAck(CXJEventDistance* pEvent, int nIndex = -1);

	/*
 *  @brief   	FindLocalPoint	 查找某条事件在本地的地址 
 *  @param 		[In]a param of Type  CXJEventAlarm*  要查找的事件
 *  @return 	CXJEventAlarm* 本地地址
 */
	CXJEventDistance* FindLocalPoint(CXJEventDistance* pEvent);

	/*
 *  @brief   	CheckLineCount	 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 *  @return 	void 
 */
	void CheckLineCount();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTDistance)
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTDistance)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtdistanceAck();
	afx_msg void OnBtnPtdistanceAckall();
	afx_msg void OnBtnPtdistanceDetail();
	afx_msg void OnBtnPtdistancePrint();
	afx_msg void OnCheckPtdistanceHide();
	afx_msg void OnDblclkListPtdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListPtdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_)
