#if !defined(AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_)
#define AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownLoadView.h : header file
//
#include "CommonFile.h"

/** @brief           通用文件链表*/
//##ModelId=49B87BA300BD
typedef CTypedPtrList<CObList , CCommonFile*>	COMMONFILE_LIST;
/////////////////////////////////////////////////////////////////////////////
// CDownLoadView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BA30109
class CDownLoadView : public CViewBase
{
protected:
	//##ModelId=49B87BA3010B
	CDownLoadView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownLoadView)

// Form Data
public:
	//{{AFX_DATA(CDownLoadView)
	enum { IDD = IDD_DOWNLOAD_FILE };
	CCompareCmb	m_cmbStation;
	CButton	m_btnFileClose;
	CPictureEx	m_gif;
	CButton	m_btnDown;
	CButton	m_btnCall;
	CListCtrlEx	m_List;
	CString	m_strCondition;
	//}}AFX_DATA

// Attributes
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87BA3013A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
	//##ModelId=49B87BA30140
	CThemeHelperST	m_ThemeHelper;
	/** @brief           列表的列信息*/
	//##ModelId=49B87BA3014C
	CArray<SColumn, SColumn&> m_arrColum;

	/** @brief           通用文件链表*/
	//##ModelId=49B87BA30158
	COMMONFILE_LIST m_FileList;

	/** @brief           记录当前召唤的厂站ID, 为空时表示所有厂站*/
	//##ModelId=49B87BA3015C
	CString m_sStationID;
	/** @brief           条件类型 1-按文件名称,2-指定时间段内的文件名称*/
	//##ModelId=49B87BA30167
	int		m_nConditionType;
	/** @brief           当前状态. 0-空闲. 1-正在召唤录波列表. 2-正在下载录波文件*/
	//##ModelId=49B87BA30168
	int		m_nCurrentStatus;

	/** @brief           记录定时器*/
	//##ModelId=49B87BA30177
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	/** @brief           待下载文件列表*/
	//##ModelId=49B87BA30179
	COMMONFILE_LIST	m_FileDownload;

	/** @brief           结果字符串*/
	//##ModelId=49B87BA30186
	CString	m_CallFileResult;

	/** @brief           是否使用断点续传*/
	//##ModelId=49B87BA30187
	BOOL	m_bUseBreakPoint;

	/** @brief           自增序号*/
	//##ModelId=49B87BA30196
	int m_nNum;
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87BA30197
	int		m_nOldSortCol;

// Operations
public:

private:
/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA301A5
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA301B5
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87BA301C5
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87BA301D4
	int	InitListStyle();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87BA301D5
	int InitControls();

/*
 *  @brief   	FillStation	 填充厂站下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87BA301E4
	void FillStation();

/*
 *  @brief   	ClearFileList	 清除所有通用文件记录 
 *  @return 	void 
 */
	//##ModelId=49B87BA301E5
	void ClearFileList();

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87BA301F4
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87BA301F5
	void CancelCall();

/*
 *  @brief   	FillListData	 填充列表框数据 
 *  @return 	void 
 */
	//##ModelId=49B87BA30203
	void FillListData();

/*
 *  @brief   	AddFileToList	 增加文件对象到列表框中显示 
 *  @param 		[In]a param of Type  CCommonFile*  文件对象
 *  @param 		[In]a param of Type  int  列表框索引
 *  @return 	void 
 */
	//##ModelId=49B87BA30204
	void AddFileToList(CCommonFile* pFile, int nIndex = 0);

/*
 *  @brief   	ChangeValue	 改变文件列表是否已下载的显示
 *  @param 		[In]a param of Type  CCommonFile*  指定列表
 *  @return 	void 
 */
	//##ModelId=49B87BA30213
	void ChangeValue(CCommonFile* pFile);

/*
 *  @brief   	DownloadCommonFile	 下载通用文件 
 *  @param 		[In]a param of Type  CCommonFile*  指定列表
 *  @return 	void 
 */
	//##ModelId=49B87BA30215
	void DownloadCommonFile(CCommonFile* pFile);

/*
 *  @brief   	CheckDownload	 核对要下载的文件 
 *  @return 	void 
 */
	//##ModelId=49B87BA30222
	void CheckDownload();

/*
 *  @brief   	ChangeDownloadedValue	 更新正在下载的文件的已下载内容大小 
 *  @param 		[In]a param of Type  CCommonFile*  正在下载的文件
 *  @param 		[In]a param of Type  CString  已下载文件大小
 *  @return 	void 
 */
	void ChangeDownloadedValue(CCommonFile* pFile, CString sValue);

	void OpenFileDir(CCommonFile* pFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BA30251
	virtual ~CDownLoadView();
#ifdef _DEBUG
	//##ModelId=49B87BA30261
	virtual void AssertValid() const;
	//##ModelId=49B87BA30271
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDownLoadView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCmbDownCondition();
	afx_msg void OnBtnDownloadCall();
	afx_msg void OnBtnDownloadDown();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnColumnclickListDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnFileClose();
	afx_msg void OnDoubleclickedBtnDownloadCall();
	afx_msg void OnBtnDownloadOpen();
	afx_msg void OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87BA302CF
	afx_msg void OnDownloadOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302DE
	afx_msg void OnSTTP00204(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302EF
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302FD
	afx_msg void DownloadFileEnd(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30301
	afx_msg void OnSTTP00212(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA3030E
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30312
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_)
