#if !defined(AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_)
#define AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTOSCList.h : header file
//
//##ModelId=49B87B8F011A
typedef CTypedPtrList<CObList , CXJEventOSC*>		EventOSC_LIST;
/////////////////////////////////////////////////////////////////////////////
// CPTOSCList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B8F0177
class CPTOSCList : public CViewBase
{
protected:
	//##ModelId=49B87B8F0179
	CPTOSCList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTOSCList)

// Form Data
public:
	//{{AFX_DATA(CPTOSCList)
	enum { IDD = IDD_PT_OSCLIST };
	//##ModelId=49B87B8F0187
	CButton	m_btnOpen;
	//##ModelId=49B87B8F0197
	CListCtrlEx			m_List;
	//##ModelId=49B87B8F019C
	CPictureEx			m_gif;
	//##ModelId=49B87B8F01A1
	CButton	m_btnDown;
	//##ModelId=49B87B8F01A6
	CButton	m_btnCall;
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
	//##ModelId=49B87B8F01AA
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	//##ModelId=49B87B8F01B0
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B8F01B5
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           当前选择的数据来源类型.0-保护装置. 1-子站数据库*/
	//##ModelId=49B87B8F01B9
	int		m_nSelSrc;
	/** @brief           当前状态. 0-空闲. 1-正在召唤录波列表. 2-正在下载录波文件*/
	//##ModelId=49B87B8F01BA
	int		m_nCurrentStatus;
	/** @brief           对应二次设备*/
	//##ModelId=49B87B8F01BC
	CSecObj*	m_pObj;
	/** @brief           保存定时器*/
	//##ModelId=49B87B8F01C5
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	/** @brief           保存多帧报文时已收到的帧序号*/
	//##ModelId=49B87B8F01CC
	CUIntArray			m_arrFrame;
	/** @brief           多帧报文总帧数*/
	//##ModelId=49B87B8F01D4
	int		m_nFrameCount;
	/** @brief           待下载的录波列表*/
	//##ModelId=49B87B8F01D6
	EventOSC_LIST		m_EventDownload;
	/** @brief           召唤录波文件结果. 格式为: 召唤"文件名", "结果"*/
	//##ModelId=49B87B8F01E4
	CString				m_CallFileResult;

	/** @brief           是否使用断点续传*/
	//##ModelId=49B87B8F01E5
	BOOL	m_bUseBreakPoint;

	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;

	/** @brief           下载成功数*/
	int		m_nSuccess;
	/** @brief           下载失败数*/
	int		m_nFaild;
	/** @brief           部分下载数*/
	int		m_nPart;
	/** @brief           解析20026报文线程指针*/
	CWinThread* m_pParse20026Thread;
	CWinThread*	m_pQueryThread;
public:
	/** @brief           20026报文链表*/
	CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           退出解析线程标志*/
	BOOL m_bExitParse;
	/** @brief           报文收取完成标志，让线程在处理完现有的报文后自动退出*/
	BOOL m_bDataEnd;
	/** @brief           自增序号*/
	//##ModelId=49B87B8F01F4
	int m_nNum;
	/** @brief           事件管理器*/
	//##ModelId=49B87B8F01C7
	CXJEventManager		m_EventManager;

	BOOL m_bExitQuery; //退出查询线程标志

// Operations
public:
/*
 *  @brief   	AddTail20026SttpData	 将20026报文加入到链表尾 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  20026报文数据
 *  @return 	void 
 */
	void AddTail20026SttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	RemoveHead20026SttpData	 删除20026报文链表的第一项 
 *  @return 	void 
 */
	void RemoveHead20026SttpData();

/*
 *  @brief   	Parse20026	 解析20026报文数据
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据对象
 *  @return 	void 
 */
	void Parse20026(STTP_FULL_DATA* pSttpData);

	BOOL LoadDataFromDB();

	void NotifyThreadEnd();

private:
/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B8F0203
	int InitControls();

/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8F0213
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8F0222
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8F0232
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8F0233
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8F0242
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8F0251
	BOOL NeedSave();

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87B8F0261
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B8F0271
	void CancelCall();

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventOSC *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8F0272
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	TranslateMultiFrame	 解析多帧的定值报文 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *  @return 	void 
 */
	//##ModelId=49B87B8F0280
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	DownloadOSCFile	 下载指定录波列表的录波文件 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波列表
 *  @return 	void 
 */
	//##ModelId=49B87B8F0290
	void DownloadOSCFile(CXJEventOSC* pEvent);

/*
 *  @brief   	ChangeValue	 改变录波列表是否已下载的显示
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波列表
 *  @return 	void 
 */
	//##ModelId=49B87B8F0292
	void ChangeValue(CXJEventOSC* pEvent);

/*
 *  @brief   	CheckDownload	 检查要下载的文件 
 *  @return 	void 
 */
	//##ModelId=49B87B8F029F
	void CheckDownload();

/*
 *  @brief   	ChangeDownloadedValue	 修改正在下载的文件的已下载内容大小 
 *  @param 		[In]a param of Type  CXJEventOSC*  正在下载的文件
 *  @param 		[In]a param of Type  CString  已下载内容大小
 *  @return 	void 
 */
	void ChangeDownloadedValue(CXJEventOSC* pEvent, CString sValue);

/*
 *  @brief   	GetOSCFileDir	 得到录波文件存放目录 
 *  @return 	CString 录波文件存放目录
 */
	//##ModelId=49B87B90008F
	CString	GetOSCFileDir(BOOL bNewPath);

/*
 *  @brief   	StartParse20026Thread	 启动解析20026报文线程 
 *  @return 	void 
 */
	void StartParse20026Thread();

/*
 *  @brief   	EndParse20026Thread	 强制停止20026解析报文 
 *  @return 	void 
 */
	void EndParse20026Thread();

	void StartQueryThread();
	void EndQueryThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTOSCList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8F02BF
	virtual ~CPTOSCList();
#ifdef _DEBUG
	//##ModelId=49B87B8F02DE
	virtual void AssertValid() const;
	//##ModelId=49B87B8F02EE
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTOSCList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCmbPtosclistSrc();
	afx_msg void OnBtnPtosclistCall();
	afx_msg void OnBtnPtosclistDown();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnPtosclistOpen();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickListPtosclist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtosclist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtosclistCopy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtosclistCall();
	afx_msg void OnDoubleclickedBtnPtosclistDown();
	//}}AFX_MSG
	//##ModelId=49B87B8F035C
	afx_msg void OnSTTP20026(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F036B
	afx_msg void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F036F
	afx_msg void OnCallFileEnd(WPARAM wparam, LPARAM lParam);
	//##ModelId=49B87B8F037D
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F038B
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F0399
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F039D
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F03AA
	afx_msg void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadWorkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_)
