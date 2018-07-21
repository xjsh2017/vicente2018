#if !defined(AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_)
#define AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAnalog.h : header file
//
#include "ViewBase.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B930157
class CPTAnalog : public CViewBase
{
protected:
	//##ModelId=49B87B930159
	CPTAnalog();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAnalog)

// Form Data
public:
	//{{AFX_DATA(CPTAnalog)
	enum { IDD = IDD_PT_ANALOG };
	CButton	m_btnExcel;
	CPictureEx			m_gif;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnCall;
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B930191
	CSecObj *	m_pObj; //对应的保护指针
	//##ModelId=49B87B930196
	CString		m_sCPU;	//所选CPU
	/** @brief           操作超时定时器*/
	//##ModelId=49B87B930197
	int			m_nTimer;
	/** @brief           线程退出标志*/
	//##ModelId=49B87B9301A5
	BOOL		m_bExitThread;
	/** @brief           记录载入线程指针*/
	//##ModelId=49B87B9301A7
	CWinThread*	m_pLoadThread;

private:
	//##ModelId=49B87B9301B6
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B9301BB
	CTypedPtrArray<CPtrArray, PT_ANALOG*> m_arrAnalog; //存储开关量描述列表
	/** @brief           最新值列*/
	//##ModelId=49B87B9301C8
	int m_nNewValueIndex;
	/** @brief			实时值列*/
	int m_nRealtimeValueIndex;
	UINT m_nRealtimeValueTimer; //实时值刷新定时器


	/** @brief           当前状态. 0-空闲状态 1-正在召唤模拟量*/
	//##ModelId=49B87B9301D4
	int	m_nCurrentStatus;

	/** @brief           是否第一帧*/
	BOOL m_bFirstFrame;

	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B9301D6
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B9301E5
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B9301EA
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B9301F7
	IXJReport * pReportDoc;

	/** @brief			查询号,最大值1000，超过1000后从0开始计数*/
	int			m_nQueryNo;

	CHisValueMgr	m_HisValueMgr;

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
	//##ModelId=49B87B930204
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B930208
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B930222
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B930232
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B930242
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B930251
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B930261
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B930271
	int InitControls();

/*
 *  @brief   	ClearAnalogConfig	 清除模拟量配置信息 
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B930290
	BOOL ClearAnalogConfig();

/*
 *  @brief   	AddAnalogValue	 加入某模拟量值到列表 
 *  @param 		[In]a param of Type  CString  要加入的模拟量的ID
 *  @param 		[In]a param of Type  CString  要加入的模拟量的值
 *  @param 		[In]a param of Type  int  指定要加入到第几列
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B93029F
	BOOL AddAnalogValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadPTHistoryAnalogData	 载入模拟量历史值 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9302AF
	BOOL LoadPTHistoryAnalogData();

/*
 *  @brief   	LoadDataFromSTTP	 从STTP报文中读取模拟量值 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9302B0
	BOOL LoadDataFromSTTP(STTP_FULL_DATA* sttpData);

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性
 *  @return 	void 
 */
	//##ModelId=49B87B9302C0
	void UpdateControlsEnable();

/*
 *  @brief   	RefreshControls	 刷新控件值 
 *  @return 	void 
 */
	//##ModelId=49B87B9302C1
	void RefreshControls();

/*
 *  @brief   	SaveSelCPU	 保存当前选择的CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU下拉框指针
 *  @return 	BOOL 值有改变返回TRUE,值未改变返回FALSE 
 */
	//##ModelId=49B87B9302CE
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 删除新加的数据列 
 *  @return 	void 
 */
	//##ModelId=49B87B9302D0
	void ClearDataCol();


/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B9302DE
	void CancelCall();

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B9302DF
	void EndOwnerThread();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B9302E0
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	IsVaild20125	 判断是否自己功能的20125报文 
 *  @param 		[In]a param of Type  int  操作报文ID
 *  @return 	BOOL 是自己功能的报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B9302EE
	BOOL IsVaild20125(int nMsgID);

	PT_ANALOG* FindAnalog(CString sID);

	void StartThread();

	int GetNextQueryNo();

	/*
	* @brief	AddValueCol	为列表加入一列用来显示值
	* @param	[In]a param of Type int nQueryNo		
	* @return   void	
	*/
	void AddValueCol(int nQueryNo);

	BOOL LoadDefaultCpu();

	void startRealtimeValueTimer();
	void stopRealtimeValueTimer();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	/*
 *  @brief   	LoadPTAnalogConfig	 从数据库读取模拟量配置信息 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B930280
	BOOL LoadPTAnalogConfig(int nQueryNo);
/*
 *  @brief   	ReFillAll	 重新读入所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9302F0
	BOOL ReFillAll();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B9302F1
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9302FD
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B9302FE
	void ReleaseCOM();
/*
 *  @brief   	ReleaseCOM	 按精度设计实际值
 *  @return 	void 
 */
	//##ModelId=49B87B9302FE
	CString SetFloatValue(PT_ANALOG* pst, CString strValue);
/*
 *  @brief   	DoPrintAnalog	 打印模拟量 
 *  @param 		[In]a param of Type  CString  打印模板全路径
 *  @return 	void 
 */
	//##ModelId=49B87B9302FF
	void DoPrintAnalog(CString strPath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAnalog)
	public:
	//##ModelId=49B87B930301
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B93031C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B93033C
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B930341
	virtual ~CPTAnalog();
#ifdef _DEBUG
	//##ModelId=49B87B93035B
	virtual void AssertValid() const;
	//##ModelId=49B87B93036B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAnalog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtanalogCall();
	afx_msg void OnSelchangeCmbPtanalogCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtanalogStyle();
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtanalogCall();
	afx_msg void OnBtnPtanalogExcel();
	//}}AFX_MSG
	//##ModelId=49B87B940006
	void OnSTTP20008(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B940010
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B94001F
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94002E
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B940031
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B940040
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_)
