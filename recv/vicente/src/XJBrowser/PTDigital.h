#if !defined(AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_)
#define AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTDigital.h : header file
//
#include "ViewBase.h"
#include "DeviceObj.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTDigital form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B920177
class CPTDigital : public CViewBase
{
protected:
	//##ModelId=49B87B920179
	CPTDigital();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTDigital)

// Form Data
public:
	//{{AFX_DATA(CPTDigital)
	enum { IDD = IDD_PT_DIGITAL };
	//##ModelId=49B87B920197
	CPictureEx			m_gif;
	//##ModelId=49B87B9201A6
	CButton	m_btnStyle;
	//##ModelId=49B87B9201AB
	CXPStyleButtonST	m_btnPrint;
	//##ModelId=49B87B9201B9
	CButton	m_btnCall;
	//##ModelId=49B87B9201C6
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B9201CB
	CSecObj *	m_pObj; //对应的保护指针
	//##ModelId=49B87B9201D4
	CString		m_sCPU;	//所选CPU	
	/** @brief           退出线程标志*/
	//##ModelId=49B87B9201D5
	BOOL		m_bExitThread;
	/** @brief           记录载入线程指针*/
	//##ModelId=49B87B9201E5
	CWinThread*	m_pLoadThread;
	
private:
	//##ModelId=49B87B9201EA
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B9201F5
	CTypedPtrArray<CPtrArray, PT_DIGITAL*> m_arrDigital; //存储开关量描述列表
	/** @brief           当前状态. 0-空闲状态 1-正在召唤开关量*/
	//##ModelId=49B87B920203
	int		m_nCurrentStatus;
	/** @brief           是否第一帧*/
	BOOL	m_bFirstFrame;
	/** @brief           操作超时定时器索引*/
	//##ModelId=49B87B920204
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B920214
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B920219
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B920225
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B920233
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
	//##ModelId=49B87B920238
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
private:
	/** @brief           最新值列*/
	//##ModelId=49B87B920242
	int m_nNewValueIndex;
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B920243
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B920261
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B920271
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B920290
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B92029F
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B9202AF
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B9202BF
	int InitControls();


/*
 *  @brief   	LoadDataFromSTTP	 从STTP报文读入开关量数据 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  报文数据
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B92031C
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);
/*
 *  @brief   	ClearDigitalConfig	 清除开关量配置信息 
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9202EE
	BOOL ClearDigitalConfig();

/*
 *  @brief   	AddDigitalValue	 加入某关开量值到列表 
 *  @param 		[In]a param of Type  CString  要加入的开关量的ID
 *  @param 		[In]a param of Type  CString  要加入的开关量的值
 *  @param 		[In]a param of Type  int  指定要加入到第几列
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9202EF
	BOOL AddDigitalValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87B92031E
	void UpdateControlsEnable();

/*
 *  @brief   	RefreshControls	 刷新控件值 
 *  @return 	void 
 */
	//##ModelId=49B87B92032C
	void RefreshControls();

/*
 *  @brief   	SaveSelCPU	 保存当前选择的CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU下拉框指针
 *  @return 	BOOL 值有改变返回TRUE,值未改变返回FALSE 
 */
	//##ModelId=49B87B92032D
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 删除新加的数据列 
 *  @return 	void 
 */
	//##ModelId=49B87B92033C
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B92033D
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B92033E
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	DisplayValue	 得到开关量的值显示字符串 
 *  @param 		[In]a param of Type  int  开关量值
 *  @return 	CString 显示值
 */
	//##ModelId=49B87B92034B
	CString DisplayValue(int nValue);

	PT_DIGITAL* FindDigital(CString sID);

	void StartThread();

	int GetNextQueryNo();

	BOOL LoadDefaultCpu();

public:
	/*
 *  @brief   	LoadPTHistoryDigitalData	 载入开关量历史值 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B92030D
	BOOL LoadPTHistoryDigitalData(int nQueryNo);

	/*
 *  @brief   	LoadPTDigitalConfig	 从数据库读取开关量配置信息 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9202DE
	BOOL LoadPTDigitalConfig(int nQueryNo);


	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B92034D
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B92035B
	void EndOwnerThread();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B92035C
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B92035E
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B92035F
	void ReleaseCOM();

/*
 *  @brief   	DoPrintDigital	 打印开关量 
 *  @param 		[In]a param of Type  CString  打印模板全路径
 *  @return 	void 
 */
	//##ModelId=49B87B920360
	void DoPrintDigital(CString strPath);

	void AddValueCol( int nQueryNo );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTDigital)
	public:
	//##ModelId=49B87B92036B
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B92037A
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B920399
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B9203AD
	virtual ~CPTDigital();
#ifdef _DEBUG
	//##ModelId=49B87B9203C8
	virtual void AssertValid() const;
	//##ModelId=49B87B930000
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTDigital)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtdiCall();
	afx_msg void OnSelchangeCmbPtdiCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtdiStyle();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtdiCall();
	afx_msg void OnBtnPtdiExcel();
	//}}AFX_MSG
	//##ModelId=49B87B930090
	void OnSTTP20012(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B93009C
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300AB
	void OnSTTP20125(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300BB
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300CB
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9300CE
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9300DC
	afx_msg void OnRefreshPTDI(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_)
