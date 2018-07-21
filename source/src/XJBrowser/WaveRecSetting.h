// ***************************************************************
//  WaveRecSetting   version:  1.0    date: 24:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_WAVERECSETTING_H__20844BEA_4587_4CBF_881D_F428C3A84E9F__INCLUDED_)
#define AFX_WAVERECSETTING_H__20844BEA_4587_4CBF_881D_F428C3A84E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRecSetting.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B7F000F
class CWaveRecSetting : public CViewBase
{
protected:
	//##ModelId=49B87B7F0020
	CWaveRecSetting();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWaveRecSetting)

// Form Data
public:
	//{{AFX_DATA(CWaveRecSetting)
	enum { IDD = IDD_WAVEREC_SETTING };
	//##ModelId=49B87B7F004F
	CPictureEx			m_gif;
	//##ModelId=49B87B7F0054
	CTabCtrl			m_TabCtrl;
	//##ModelId=49B87B7F0059
	CButton	m_btnStyle;
	//##ModelId=49B87B7F005E
	CXPStyleButtonST	m_btnPrint;
	//##ModelId=49B87B7F0063
	CButton	m_btnCall;
	//##ModelId=49B87B7F0068
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           列表的列信息*/
	//##ModelId=49B87B7F006E
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           对应保护*/
	//##ModelId=49B87B7F0073
	CSecObj*	m_pObj;
	/** @brief           退出线程标志*/
	//##ModelId=49B87B7F007D
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程指针*/
	//##ModelId=49B87B7F007F
	CWinThread*	m_pLoadThread;
private:
	/** @brief           XP风格辅助*/
	//##ModelId=49B87B7F0084
	CThemeHelperST	m_ThemeHelper;
	/** @brief           定值描述列表*/
	//##ModelId=49B87B7F008D
	CTypedPtrArray<CPtrArray, PT_SETTING*> m_arrSetting;
	/** @brief           定值组描述列表*/
	//##ModelId=49B87B7F0092
	CTypedPtrArray<CPtrArray, PT_GROUP*> m_arrGroup;
	/** @brief           下拉框选择的CPU号*/
	//##ModelId=49B87B7F0096
	CString		m_sCPU;
	/** @brief           当前选择的组号*/
	//##ModelId=49B87B7F009C
	int			m_nGroup;
	/** @brief           最新数据列*/
	//##ModelId=49B87B7F009D
	int			m_nNewValueIndex;
	/** @brief           当前状态. 0-空闲. 1-正在召唤定值.*/
	//##ModelId=49B87B7F00AB
	int m_nCurrentStatus;
	/** @brief			 定值来源*/
	//##ModelId=49B87B7F00AC
	int			m_nSrc;
	/** @brief           操作超时定时器索引*/
	//##ModelId=49B87B7F00AD
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;
	/** @brief           记录是否能分组显示*/
	//##ModelId=49B87B7F00BB
	BOOL	m_bCanGroup;

	//多帧处理
	/** @brief           保存多帧报文时已收到的帧序号*/
	//##ModelId=49B87B7F00BD
	CUIntArray		m_arrFrame;
	/** @brief           多帧报文总帧数*/
	//##ModelId=49B87B7F00CB
	int		m_nFrameCount;
	/** @brief           收取多帧报文时缓存值*/
	//##ModelId=49B87B7F00CD
	CTypedPtrArray<CPtrArray, PT_SETTING*>		m_arrNewSetting;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B7F00DB
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B7F00E0
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B7F00EE
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B7F00FB
	IXJReport * pReportDoc;

// Operations
public:
private:
	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B7F0109
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B7F0128
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B7F0148
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B7F0167
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B7F0196
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B7F01B5
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B7F01D4
	int InitControls();

/*
 *  @brief   	ClearSettingConfig	 清除定值配置 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F01E4
	BOOL ClearSettingConfig();

/*
 *  @brief   	LoadSettingConfig	 从数据库读入保护定值定义 
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F0203
	BOOL LoadPTSettingConfig();

/*
 *  @brief   	LoadPTGroupConfig	 查找定值组配置 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F0213
	BOOL LoadPTGroupConfig();

/*
 *  @brief   	LoadOrderValue	 查找定值基准值 
 *  @param 		[In/Out]a param of Type  PT_SETTING*  定值结构指针
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F0232
	BOOL LoadOrderValue(PT_SETTING* pts);

	/*
 *  @brief   	AddSettingValue	 加入某定值值到列表 
 *  @param 		[In]a param of Type  CString  要加入的定值的ID
 *  @param 		[In]a param of Type  CString  要加入的的值
 *  @param 		[In]a param of Type  int  指定要加入到第几列
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F0251
	BOOL AddSettingValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadPTHistorySettingData	 载入定值历史值 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B7F0261
	BOOL LoadPTHistorySettingData();

/*
 *  @brief   	LoadDataFromSTTP	 从STTP报文中读取定值数据 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  报文数据
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F0280
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0282
	void UpdateControlsEnable();

/*
 *  @brief   	ReSetGroupTab	 重新设置分组TAB栏 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0290
	void ReSetGroupTab();

/*
 *  @brief   	SaveSelGroup	 保存TAB栏当前选择的信息 
 *  @param 		[In]a param of Type  CTabCtrl*  指定TAB栏
 *  @return 	void 
 */
	//##ModelId=49B87B7F029F
	void SaveSelGroup(CTabCtrl* pTab);

/*
 *  @brief   	ReFillCPU	 重新填充CPU下拉框  
 *  @return 	void 
 */
	//##ModelId=49B87B7F02AF
	void ReFillCPU();

/*
 *  @brief   	SaveSelCPU	 保存当前选择的CPU号 
 *  @param 		[In]a param of Type  CComboBox*  CPU下拉框
 *  @return 	void 
 */
	//##ModelId=49B87B7F02BF
	void SaveSelCPU(CComboBox* pCmb);

/*
 *  @brief   	ClearMultiFrame	 清空多帧报文的缓存数据 
 *  @return 	void 
 */
	//##ModelId=49B87B7F02CE
	void ClearMultiFrame();

/*
 *  @brief   	TranslateMultiFrame	 解析多帧的定值报文 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *  @return 	void 
 */
	//##ModelId=49B87B7F02CF
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B7F02DE
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B7F02DF
	BOOL IsVaild20069(int nMsgID);

	/*
	*  @brief   	EndOwnerThread	 退出所有自己起的线程
	*  @return 	void 
	*/
	//##ModelId=49B87B7F02EE
	void EndOwnerThread();

/*
 *  @brief   	ClearSettingGroup	 清除定值组配置 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F02EF
	BOOL ClearSettingGroup();

/*
 *  @brief   	LoadSettingGroup	 查找定值组 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F02F0
	BOOL LoadSettingGroup();

/*
 *  @brief   	SetGroupName	 设置定值组的名称 
 *  @param 		[In]a param of Type  int  定值组ID
 *  @param 		[In]a param of Type  CString  定值组名称
 *  @return 	void 
 */
	//##ModelId=49B87B7F02F1
	void SetGroupName(int nGroupID, CString sName);

public:
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F02FF
	BOOL ReFillAll();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B7F030D
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7F030F
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0310
	void ReleaseCOM();

/*
 *  @brief   	DoPrintWRSetting	 打印录波器定值 
 *  @param 		[In]a param of Type  CString  打印模板文件
 *  @return 	void 
 */
	//##ModelId=49B87B7F0311
	void DoPrintWRSetting(CString strPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveRecSetting)
	public:
	//##ModelId=49B87B7F031C
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B7F033C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B7F037A
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B7F038A
	virtual ~CWaveRecSetting();
#ifdef _DEBUG
	//##ModelId=49B87B7F03A9
	virtual void AssertValid() const;
	//##ModelId=49B87B7F03D8
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWaveRecSetting)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnWrsetCall();
	afx_msg void OnSelchangeTabWrset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbWrsetCpu();
	afx_msg void OnSelchangeCmbWrsetSrc();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleclickedBtnWrsetCall();
	afx_msg void OnDblclkListWrset(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87B8000CC
	void OnSTTP20016(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8000DB
	void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8000FA
	void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800109
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800119
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B80011C
	void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800128
	afx_msg void OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERECSETTING_H__20844BEA_4587_4CBF_881D_F428C3A84E9F__INCLUDED_)
