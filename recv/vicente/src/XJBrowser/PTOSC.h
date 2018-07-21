#if !defined(AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_)
#define AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTOSC.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPTOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B900000
class CPTOSC : public CViewBase
{
protected:
	//##ModelId=49B87B900002
	CPTOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTOSC)

// Form Data
public:
	//{{AFX_DATA(CPTOSC)
	enum { IDD = IDD_PT_OSC };
	//##ModelId=49B87B900010
	CButton	m_btnRefur;
	//##ModelId=49B87B900015
	CButton	m_btnOpen;
	//##ModelId=49B87B90001A
	CListCtrlEx			m_List;
	//##ModelId=49B87B900022
	CString	m_sDirPath;
	//##ModelId=49B87B900023
	BOOL	m_bOnlyCFG;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B900025
	CSecObj*	m_pObj;	//对应保护
	//##ModelId=49B87B90002A
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           退出线程标志*/
	//##ModelId=49B87B900031
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程的指针*/
	//##ModelId=49B87B900033
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B900041
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B90004E
	int		m_nMaxRow;	//事件最大显示行数
	//##ModelId=49B87B900050
	CTypedPtrArray<CPtrArray, PT_OSC*> m_arrOSC; //存储录波文件描述列表
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B90005D
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
	//##ModelId=49B87B90005E
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B90006F
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B900070
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B90007D
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B90007E
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B90008C
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B90008D
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B90008E
	int InitControls();

/*
 *  @brief   	GetOSCFileDir	 得到录波文件存放目录 
 *  @return 	CString 录波文件存放目录
 */
	//##ModelId=49B87B90008F
	CString	GetOSCFileDir(BOOL bNewPath);
	
/*
 *  @brief   	SearchOSCFiles	 探索目录查找录波文件 
 *  @param 		[In]a param of Type  CString&  目录名,后带'\'
 *  @return 	BOOL 搜索成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B90009C
	BOOL SearchOSCFiles(CString& path, int nQueryNo);

/*
 *  @brief   	IsValidOSC	 判断文件是否录波文件 
 *  @param 		[In]a param of Type  CString  文件后缀名
 *  @return 	BOOL 是录波文件返回TRUE, 不是录波文件返回FALSE
 */
	//##ModelId=49B87B90009E
	BOOL IsValidOSC(CString strName);

/*
 *  @brief   	ClearOSCMsg	 清除录波文件信息 
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9000AB
	BOOL ClearOSCMsg();

/*
 *  @brief   	OpenWaveFile	 打开波形文件 
 *  @param 		[In]a param of Type  PT_OSC*  波形文件结构
 *  @return 	void 
 */
	//##ModelId=49B87B9000AC
	void OpenOSCWaveFile(PT_OSC* pts);

	void StartThread();

	int GetNextQueryNo();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9000AE
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B9000AF
	void EndOwnerThread();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
//	void RegulateControlSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTOSC)
	public:
	//##ModelId=49B87B9000B0
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B9000BB
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B9000CB
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B9000D0
	virtual ~CPTOSC();
#ifdef _DEBUG
	//##ModelId=49B87B9000DA
	virtual void AssertValid() const;
	//##ModelId=49B87B9000DC
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTOSC)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckPtoscShow();
	afx_msg void OnBtnPtoscRf();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtoscOpen();
	afx_msg void OnDblclkListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtoscCopy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87B90013A
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B90013D
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_)
