#if !defined(AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_)
#define AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRecOSC.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B800167
class CWaveRecOSC : public CViewBase
{
protected:
	//##ModelId=49B87B800169
	CWaveRecOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWaveRecOSC)

// Form Data
public:
	//{{AFX_DATA(CWaveRecOSC)
	enum { IDD = IDD_WAVEREC_OSC };
	//##ModelId=49B87B80016B
	CListCtrlEx	m_List;
	//##ModelId=49B87B80017B
	CButton	m_btnRefur;
	//##ModelId=49B87B800180
	CButton	m_btnOpen;
	//##ModelId=49B87B800184
	CString	m_sDirPath;
	//##ModelId=49B87B800185
	BOOL	m_bOnlyCFG;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B800187
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	//##ModelId=49B87B80018C
	CSecObj*	m_pObj;	//对应录波器
	/** @brief           线程退出标志*/
	//##ModelId=49B87B800190
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程指针*/
	//##ModelId=49B87B800192
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B800197
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B80019B
	int		m_nMaxRow;	//事件最大显示行数
	//##ModelId=49B87B80019D
	CTypedPtrArray<CPtrArray, PT_OSC*> m_arrOSC; //存储录波文件描述列表
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B8001A5
	int		m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B8001A6
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8001AB
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8001B5
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8001B6
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8001C5
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8001C6
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8001C7
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B8001D4
	int InitControls();

/*
 *  @brief   	GetOSCFileDir	 得到录波文件存放目录 
 *  @return 	CString 录波文件存放目录
 */
	//##ModelId=49B87B8001D5
	CString	GetOSCFileDir();
	
/*
 *  @brief   	SearchOSCFiles	 探索目录查找录波文件 
 *  @param 		[In]a param of Type  CString&  目录名,后带'\'
 *  @return 	BOOL 搜索成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B8001D6
	BOOL SearchOSCFiles(CString& path);

/*
 *  @brief   	IsValidOSC	 判断文件是否录波文件 
 *  @param 		[In]a param of Type  CString  文件后缀名
 *  @return 	BOOL 是录波文件返回TRUE, 不是录波文件返回FALSE
 */
	//##ModelId=49B87B8001D8
	BOOL IsValidOSC(CString strName);

/*
 *  @brief   	ClearOSCMsg	 清除录波文件信息 
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8001E5
	BOOL ClearOSCMsg();

	/*
	*  @brief   	EndOwnerThread	 退出所有自己起的线程
	*  @return 	void 
	*/
	//##ModelId=49B87B8001E6
	void EndOwnerThread();

/*
 *  @brief   	OpenWaveFile	 打开波形文件 
 *  @param 		[In]a param of Type  PT_OSC*  波形文件结构
 *  @return 	void 
 */
	//##ModelId=49B87B9000AC
	void OpenOSCWaveFile(PT_OSC* pts);

public:
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B8001E7
	BOOL ReFillAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveRecOSC)
	public:
	//##ModelId=49B87B8001E8
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8001F4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8001F7
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B800207
	virtual ~CWaveRecOSC();
#ifdef _DEBUG
	//##ModelId=49B87B800213
	virtual void AssertValid() const;
	//##ModelId=49B87B800215
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWaveRecOSC)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckWroscShow();
	afx_msg void OnBtnWroscRf();
	afx_msg void OnColumnclickListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnWroscOpen();
	afx_msg void OnDblclkListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87B800255
	void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800258
	void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_)
