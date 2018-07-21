#if !defined(AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_)
#define AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTHistory.h : header file
//
#include "FaultEvent.h"
/////////////////////////////////////////////////////////////////////////////
// CPTHistory form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B9001A5
class CPTHistory : public CViewBase
{
protected:
	//##ModelId=49B87B9001A7
	CPTHistory();           // protected constructor used by dynamic creation
	//##ModelId=49B87B9001C5
	~CPTHistory();
	DECLARE_DYNCREATE(CPTHistory)

// Form Data
public:
	//{{AFX_DATA(CPTHistory)
	enum { IDD = IDD_PT_HISTORY };
	CButton	m_btnCopy;
	CButton	m_btnSelNull;
	CButton	m_btnSelAll;
	CButton	m_btnSave;
	CListCtrlEx			m_List;
	CPictureEx			m_gif;
	CButton	m_btnCall;
	//}}AFX_DATA

// Attributes
public:

private:
	/** @brief           XP风格按钮辅助类*/
	//##ModelId=49B87B9001E7
	CThemeHelperST	m_ThemeHelper;
	/** @brief           列表框列信息*/
	//##ModelId=49B87B9001EC
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           当前选择的数据来源类型.0-保护装置. 1-子站数据库*/
	//##ModelId=49B87B9001F4
	int		m_nSelSrc;
	/** @brief           要召唤的事件类型, 0-全部, 1-动作, 2-自检信息, 3-硬压板*/
	//##ModelId=49B87B9001F5
	int		m_nEventType;
	/** @brief           当前状态. 0-空闲. 1-正在召唤录波列表. 2-正在下载录波文件*/
	//##ModelId=49B87B900203
	int		m_nCurrentStatus;
	/** @brief           对应二次设备*/
	//##ModelId=49B87B900205
	CSecObj*	m_pObj;
	/** @brief           保存定时器*/
	//##ModelId=49B87B900213
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;
	/** @brief           存储开关量描述列表*/
	//##ModelId=49B87B900223
	CTypedPtrArray<CPtrArray, PT_DIGITAL*> m_arrDigital; //存储开关量描述列表

	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B900233
	int		m_nOldSortCol;
	

public:
	/** @brief           记录动作序号*/
	int	m_nActionIndex;
	/** @brief           20026报文链表*/
	CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           退出解析线程标志*/
	BOOL m_bExitParse;
	/** @brief           报文收取完成标志，让线程在处理完现有的报文后自动退出*/
	BOOL m_bDataEnd;
	/** @brief           自增序号*/
	//##ModelId=49B87B900232
	int m_nNum;
	/** @brief           事件链表*/
	//##ModelId=49B87B900215
	HISTORYEVENT_LIST	m_EventList;
	/** @brief           解析20026报文线程指针*/
	CWinThread* m_pParse20116Thread;

// Operations
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B900234
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

public:
/*
 *  @brief   	AddTail20026SttpData	 将20116报文加入到链表尾 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  20116报文数据
 *  @return 	void 
 */
	void AddTail20116SttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	RemoveHead20026SttpData	 删除20116报文链表的第一项 
 *  @return 	void 
 */
	void RemoveHead20116SttpData();

/*
 *  @brief   	TranslateHistoryAction	 解析历史动作信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *	@param		[In]a param of Type	 int 位置
 *  @return 	void 
 */
	//##ModelId=49B87B9002DE
	void TranslateHistoryAction(STTP_FULL_DATA* pSttpData, int i);

/*
 *  @brief   	TranslateHistoryAlarm	 解析历史告警信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *	@param		[In]a param of Type	 int 位置
 *  @return 	void 
 */
	//##ModelId=49B87B9002E0
	void TranslateHistoryAlarm(STTP_FULL_DATA* pSttpData, int i);

/*
 *  @brief   	TranslateHistoryDI	 解析历史开关量信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
  *	@param		[In]a param of Type	 int 位置
 *  @return 	void 
 */
	//##ModelId=49B87B9002EE
	void TranslateHistoryDI(STTP_FULL_DATA* pSttpData, int i);
private:
/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B900246
	int InitControls();

/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B900251
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B900261
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B900271
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B900280
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B900290
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B90029F
	BOOL NeedSave();

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87B9002AF
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B9002BF
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B9002C0
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	AddEventToList	 把指定事件加入到列表框中显示 
 *  @param 		[In]a param of Type  CFaultEvent*  指定事件
 *  @param 		[In]a param of Type  int  指定列表行索引
 *  @return 	void 
 */
	//##ModelId=49B87B9002CE
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	ClearAllEvent	 删除所有事件记录 
 *  @return 	void 
 */
	//##ModelId=49B87B9002F0
	void ClearAllEvent();

/*
 *  @brief   	GetChrName	 查找指定特征值描述 
 *  @param 		[In]a param of Type  int  特征值ID
 *	@param		[Out]a param of Type CString 单位
 *  @return 	CString 特征值描述
 */
	//##ModelId=49B87B9002F1
	CString GetChrName(int nID, CString& strUnit);

/*
 *  @brief   	GetActionSignName	 查找指定信号量描述 
 *  @param 		[In]a param of Type  int  信号量ID
 *  @return 	CString 信号量描述
 */
	//##ModelId=49B87B9002FE
	CString GetActionSignName(int nID);

/*
 *  @brief   	GetAlarmSignName	 查找指定告警信号量的描述 
 *  @param 		[In]a param of Type  int  信号量ID
 *  @return 	CString 信号量描述
 */
	//##ModelId=49B87B900300
	CString GetAlarmSignName(int nID);

/*
 *  @brief   	ClearAllDI	 清除所有开关量定义 
 *  @return 	void 
 */
	//##ModelId=49B87B90030E
	void ClearAllDI();

/*
 *  @brief   	GetPTDI	 从数据库载入本保护的所有开关量定义 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B90030F
	BOOL GetPTDI(int nCPU);

/*
 *  @brief   	GetDIName	 得到指定开关量的描述 
 *  @param 		[In]a param of Type  CString  开关量ID
 *  @return 	CString 开关量描述
 */
	//##ModelId=49B87B900310
	CString GetDIName(CString sID, int nCPU);

/*
 *  @brief   	GetSelected	 取得选择项 
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  保存所选动作事件
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  保存所选告警事件
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  保存所选开关量事件
 *  @return 	void 
 */
	void GetSelected(HISTORYEVENT_LIST* listAction, HISTORYEVENT_LIST* listAlarm, HISTORYEVENT_LIST* listDI);

/*
 *  @brief   	SendAlarm	 发送告警报文 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  告警事件链表
 *  @return 	void 
 */
	void SendAlarm(HISTORYEVENT_LIST* listAlarm);

/*
 *  @brief   	SendDI	 发送开关量报文 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  开关量事件链表
 *  @return 	void 
 */
	void SendDI(HISTORYEVENT_LIST* listDI);

/*
 *  @brief   	SendAction	 发送动作报文 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  动作事件链表
 *  @return 	void 
 */
	void SendAction(HISTORYEVENT_LIST* listAction);

/*
 *  @brief   	StartParse20116Thread	 启动解析20026报文线程 
 *  @return 	void 
 */
	void StartParse20116Thread();

/*
 *  @brief   	EndParse20116Thread	 强制停止20026解析报文 
 *  @return 	void 
 */
	void EndParse20116Thread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTHistory)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	//##ModelId=49B87B90034B
	virtual void AssertValid() const;
	//##ModelId=49B87B90035B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTHistory)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPthistoryCall();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeCmbPthistorySrc();
	afx_msg void OnSelchangeCmbPthistoryType();
	afx_msg void OnColumnclickListPthistory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPthistorySave();
	afx_msg void OnBtnPthistorySelNull();
	afx_msg void OnBtnPthistorySelAll();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnPthistorySelCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPthistoryCall();
	afx_msg void OnBtnPthistoryFault();
	//}}AFX_MSG
	//##ModelId=49B87B9003C0
	afx_msg void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9003C9
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9003D9
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B910000
	afx_msg void OnSTTP20116(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B91000F
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B910013
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadWorkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_)
