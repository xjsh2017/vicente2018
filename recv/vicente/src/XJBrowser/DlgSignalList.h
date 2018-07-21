#if !defined(AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_)
#define AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSignalList.h : header file
//
#include "DeviceObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSignalList dialog

//##ModelId=49B87BA30399
class CDlgSignalList : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA3039B
	CDlgSignalList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSignalList)
	enum { IDD = IDD_SIGNAL_LIST };
	//##ModelId=49B87BA3039E
	CTabCtrl	m_Tab;
	//##ModelId=49B87BA303A3
	CListCtrlEx	m_List;
	//}}AFX_DATA
	/** @brief           二次设备指针*/
	//##ModelId=49B87BA303AA
	CSecObj*	m_pObj;
private:
	/** @brief           之前被排序的列*/
	//##ModelId=49B87BA303AE
	int		m_nOldSortCol;

public:
	/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87BA303AF
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
/*
 *  @brief   	InitListStyle	 初始化列表风格 
 *  @return 	void 
 */
	//##ModelId=49B87BA303B9
	void InitListStyle();

/*
 *  @brief   	AddSignalToList	 把信号量加入到列表中显示 
 *  @param 		[In]a param of Type  PT_Signal*  要加入的信号量
 *	@param		[In]a param of Type	 int	要加入的位置
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA303BA
	BOOL AddSignalToList(PT_Signal* sign, int nIndex = 0);

/*
 *  @brief   	InitActionSignalData	 初始化动作信号量数据 
 *  @return 	void 
 */
	//##ModelId=49B87BA303BD
	void InitActionSignalData();

/*
 *  @brief   	InitAlarmSignalData	 初始化告警信号量数据 
 *  @return 	void 
 */
	//##ModelId=49B87BA303BE
	void InitAlarmSignalData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSignalList)
	protected:
	//##ModelId=49B87BA303BF
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSignalList)
	//##ModelId=49B87BA303C9
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA303CB
	afx_msg void OnSelchangeTabSignalList(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA303CF
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA303D8
	afx_msg void OnColumnclickListSignalList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_)
