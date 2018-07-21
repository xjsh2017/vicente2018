#if !defined(AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_)
#define AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTSetting.h : header file
//
#include "ViewBase.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTSetting form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
//##ModelId=49B87B8B0232
class CPTSetting : public CViewBase
{
protected:
	//##ModelId=49B87B8B0234
	CPTSetting();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSetting)

// Form Data
public:
	//{{AFX_DATA(CPTSetting)
	enum { IDD = IDD_PT_SETTING };
	CListCtrlEx	m_ListZone;
	CButtonST	m_btnVerify2;
	CButtonST	m_btnVerify1;
	CPictureEx			m_gif;
	CButton	m_btnModifyZone;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnModifySetting;
	CButton	m_btnCallZone;
	CButton	m_btnCallSetting;
	CListCtrlEx	m_List;
	CTabCtrl	m_TabCtrl;
	CString	m_sCurrentZone;
	CString	m_sEditZone;
	CString m_sOldZoneValue;
	bool    m_bCallZoneSuccess;
	bool    m_bOperTow;
	int     m_nModifySettingOrModifyZone;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           列表的列信息*/
	//##ModelId=49B87B8B030F
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           对应保护*/
	//##ModelId=49B87B8B031D
	CSecObj*	m_pObj;
	/** @brief           退出线程标志*/
	//##ModelId=49B87B8B032C
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程*/
	//##ModelId=49B87B8B032E
	CWinThread*	m_pLoadThread;

private:
	int				m_nNewZoneIndex;
	CString        m_strUser;
	/** @brief           XP风格辅助*/
	//##ModelId=49B87B8B033D
	CThemeHelperST	m_ThemeHelper;
	/** @brief           定值描述列表*/
	//##ModelId=49B87B8B034C
	CTypedPtrArray<CPtrArray, PT_SETTING*> m_arrSetting; //定值描述列表
	/** @brief           定值组描述列表*/
	//##ModelId=49B87B8B035C				
	CTypedPtrArray<CPtrArray, PT_GROUP*> m_arrGroup;	//定值组列表
	/** @brief           下拉框选择的CPU号*/
	//##ModelId=49B87B8B0360
	CString		m_sCPU;
	/** @brief           下拉框选择的定值区号*/
	//##ModelId=49B87B8B036B
	CString		m_sZone;
	/** @brief           当前选择的组号*/
	//##ModelId=49B87B8B036C
	int			m_nGroup;

	/** @brief           最新值列*/
	//##ModelId=49B87B8B038A
	int m_nNewValueIndex;
	/** @brief           当前状态. 0-空闲. 1-正在召唤定值. 2-正在召唤定值区号. 3-正在修改定值. 4-正在切换定值组*/
	//##ModelId=49B87B8B038B
	int m_nCurrentStatus;
	/** @brief           要修改的值链表*/
	//##ModelId=49B87B8B039A
	MODIFY_LIST	m_arrModifyList;
	/** @brief           修改的值文本描述*/
	//##ModelId=49B87B8B03A9
	CString		m_strOutPut;
	/** @brief			 定值来源*/
	//##ModelId=49B87B8B03AA
	int			m_nSrc;
	/** @brief           操作超时定时器索引*/
	//##ModelId=49B87B8B03B9
	int			m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;
	/** @brief           是否能按组显示*/
	//##ModelId=49B87B8B03C8
	BOOL		m_bCanGroup;
	CString     m_sOperUser;
	CString     m_sMonUser;
	bool        m_bOperVerify;
	bool        m_bMonVerify;

	/** @brief			是否真在预校*/
	BOOL		m_bChecking;

	/** @brief			要切换到的定值区*/
	CString m_sModifyZone;

	/** @brief			记录新装置修改定值区号时修改的行*/
	int m_nModifyItem;

	//多帧处理
	/** @brief           保存多帧报文时已收到的帧序号*/
	//##ModelId=49B87B8B03CA
	CUIntArray		m_arrFrame;
	/** @brief           多帧报文总帧数*/
	//##ModelId=49B87B8B03D8
	int		m_nFrameCount;
	/** @brief           收取多帧报文时缓存值*/
	//##ModelId=49B87B8C0001
	CTypedPtrArray<CPtrArray, PT_SETTING*>		m_arrNewSetting;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B8C0010
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B8C0015
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B8C0020
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B8C0025
	IXJReport * pReportDoc;
	
	/** @brief			查询号,最大值1000，超过1000后从0开始计数*/
	int			m_nQueryNo;
	/** @brief			数据载入类型.0-全部，包括当前定值区、定值配置、历史值，适用于切换保护和切换CPU
									 1-只载入历史值，适用于切换定值区号
									 2-不载入数据，只重载数据，适用于TAB切换*/
	int	m_nLoadType;
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
private:

	/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8C002E
	int InitListCtrl();
	int InitZoneListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8C009C
	int	InitListStyle();
	int	InitZoneListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8C00FA
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8C0157
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8C01B5
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8C0222
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B8C0271
	int InitControls();

/*
 *  @brief   	ClearSettingConfig	 清除定值配置 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8C02CE
	BOOL ClearSettingConfig();

/*
 *  @brief   	ClearSettingGroup	 清除定值组配置 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8C031C
	BOOL ClearSettingGroup();

/*
 *  @brief   	LoadPTGroupConfig	 查找定值组配置 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8C036B
	BOOL LoadPTGroupConfig(int nQueryNo);

/*
 *  @brief   	LoadNewOrderTime	 查询最新的基准值时间 
 *  @return 	CString 基准值时间
 */
	//##ModelId=49B87B8D001F
	CString LoadNewOrderTime();

	/*
 *  @brief   	AddSettingValue	 加入某定值值到列表 
 *  @param 		[In]a param of Type  CString  要加入的定值的ID
 *  @param 		[In]a param of Type  CString  要加入的的值
 *  @param 		[In]a param of Type  int  指定要加入到第几列
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8D0021
	BOOL AddSettingValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadDataFromSTTP	 从STTP报文中读取定值数据 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  报文数据
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8D00BB
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 根据当前状态更新控件可用性
 *  @return 	void 
 */
	//##ModelId=49B87B8D00EA
	void UpdateControlsEnable();

/*
 *  @brief   	CheckModifyData	 组建要修改的定值列表 
 *  @param 		[Out]a param of Type  MODIFY_LIST&  要修改的定值列表
 *  @param 		[Out]a param of Type  CString&  输出的文字报告
 *  @return 	void 
 */
	//##ModelId=49B87B8D0119
	void CheckModifyData(MODIFY_LIST& modifyList, CString& strOut);

/*
 *  @brief   	GetTranslatedValue	 获得经过翻译VPickList后的Value 
 *  @param 		[In]a param of Type  PT_SETTING *  定值定义
 *  @param 		[In]a param of Type  const CString  值
 *  @return 	CString 经过翻译VPickList后的Value
 */
	//##ModelId=49B87B8D0148
	CString GetTranslatedValue(PT_SETTING *pst,const CString strValue);

/*
 *  @brief   	ClearModifyData	 清除修改值链表
 *  @return 	void 
 */
	//##ModelId=49B87B8D0167
	void ClearModifyData();

/*
 *  @brief   	GetSettingObject	 根据定值ID找到定值对象 
 *  @param 		[In]a param of Type  CString  定值ID
 *  @return 	PT_SETTING * 定值对象
 */
	//##ModelId=49B87B8D0186
	PT_SETTING * GetSettingObject(CString strID);

/*
 *  @brief   	GetIDFromValue	 根据值查找值的定义 
 *  @param 		[In]a param of Type  CString  值说明
 *  @param 		[In]a param of Type  CString  值
 *  @return 	CString 
 */
	//##ModelId=49B87B8D01A5
	CString GetIDFromValue(CString strPickList, CString strValue);

/*
 *  @brief   	RefreshControls	 刷新控件值 
 *  @return 	void 
 */
	//##ModelId=49B87B8D01D4
	void RefreshControls();

/*
 *  @brief   	ReFillCPU	 重新填充CPU下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87B8D01E4
	void ReFillCPU();
	
/*
 *  @brief   	SaveSelCPU	 保存当前选择的CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU下拉框指针
 *  @return 	void 
 */
	//##ModelId=49B87B8D01F4
	BOOL SaveSelCPU( CComboBox * pCmb );
/*
 *  @brief   	ReFillZone	 重新填充定值区号下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0203
	void ReFillZone();
	
/*
 *  @brief   	SaveSelZone	 保存当前选择的定值区号 
 *  @param 		[In]a param of Type  CComboBox *  定值区下拉框指针
 *  @return 	void 
 */
	//##ModelId=49B87B8D0213
	BOOL SaveSelZone( CComboBox * pCmb );
/*
 *  @brief   	ResetGroupTab	 重新设置分组TAB 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0222
	void ResetGroupTab();
	
/*
 *  @brief   	SaveSelGroup	 保存当前选择的组号 
 *  @param 		[In]a param of Type  CTabCtrl *  分组TAB指针
 *  @return 	void 
 */
	//##ModelId=49B87B8D0232
	BOOL SaveSelGroup( CTabCtrl * pTab );

/*
 *  @brief   	ClearMultiFrame	 清空多帧报文的缓存数据 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0242
	void ClearMultiFrame();

/*
 *  @brief   	TranslateMultiFrame	 解析多帧的定值报文 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *  @return 	void 
 */
	//##ModelId=49B87B8D0251
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	ClearDataCol	 删除新加的数据列 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0261
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B8D0262
	void CancelCall();

/*
 *  @brief   	SetSettingValue	 设置指定的定值 
 *  @param 		[In]a param of Type  CString  定值ID
 *  @param 		[In]a param of Type  CString  定值值
 *  @param 		[In]a param of Type  int  列索引
 *  @return 	void 
 */
	//##ModelId=49B87B8D0271
	void SetSettingValue( CString ID, CString VALUE, int iCol);

/*
 *  @brief   	RevertModifyValue	 回复修改定值时修改的值 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0280
	void RevertModifyValue(int nType = 1);

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B8D0281
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	FindSetting	 根据定值ID找到定值对象
 *  @param 		[In]a param of Type  CString  定值ID
 *  @return 	PT_SETTING* 定值对象
 */
	//##ModelId=49B87B8D0291
	PT_SETTING* FindSetting(CString sID);

/*
 *  @brief   	SetGroupName	 设置定值组的名称 
 *  @param 		[In]a param of Type  int  定值组ID
 *  @param 		[In]a param of Type  CString  定值组名称
 *  @return 	void 
 */
	//##ModelId=49B87B8D029F
	void SetGroupName(int nGroupID, CString sName);

	CString GetGroupName(int nGroupID);
	/*
 *  @brief   	CheckVPicklist	检查vpicklist是否符合规范 
 *  @return 	BOOL 
 */
	BOOL CheckVPicklist(CString strVPickList);

	void SetZoneCmbToCurrentZone();

	/*
	* @brief	FindNewValue 查找指定点的最新值	
	* @param	[In]a param of Type int nID	指定定值点	
	* @return   CString	指定点的最新值
	*/
	CString FindNewValue(int nID);

	int GetNextQueryNo();

	/*
 *  @brief   	LoadOrderValue	 查找定值基准值 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8C03B9
	BOOL LoadOrderValue(int nQueryNo);

	/*
 *  @brief   	LoadCurrentZone	 从数据库查询当前定值区 
 *  @return 	void 
 */
	void LoadCurrentZone();

	/*
 *  @brief   	LoadSettingGroup	 查找定值组 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8D0020
	BOOL LoadSettingGroup(int nQueryNo);

	/*
 *  @brief   	LoadPTHistorySettingData	 载入定值历史值 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B8D006D
	BOOL LoadPTHistorySettingData(int nQueryNo);

	/*
 *  @brief   	LoadSettingConfig	 从数据库读入保护定值定义 
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8C031D
	BOOL LoadPTSettingConfig(int nQueryNo);

	BOOL LoadDefaultCpu();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
	int	GetLoadType(){ return m_nLoadType; };

	BOOL ReLoadData(int nQueryNo, int nLoadType);

	int m_nOperationNum;
		/*
	* @brief	FindNewValue 设定精度值	
	* @param	[In]a param of Type int nID	指定定值	
	* @return   
	*/
	CString SetFloatValue(PT_SETTING* pst, CString strValue);

	CString GetDisplayValue(PT_SETTING* pst, CString strValue);

/*
 *  @brief   	GetTranslatedNumber	读取VPicklist最大选择值 
 *  @return 	int 成功返回nID, 失败返回-1
 */
	int GetTranslatedNumber(CString str);
/*
 *  @brief   	ReFillAll	重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8D02AF
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B8D02B0
	void EndOwnerThread();

	void StartThread();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B8D02BF
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8D02C1
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B8D02C2
	void ReleaseCOM();

/*
 *  @brief   	DoPrintSetting	 打印定值 
 *  @param 		[In]a param of Type  CString  模板文件全路径
 *  @return 	void 
 */
	//##ModelId=49B87B8D02CE
	void DoPrintSetting(CString strPath);

	BOOL GetZone(CString pt_id, int nCpu, int zone_id, PT_ZONE* pt_zone);
	void OnBtnPtsetModify1();
	void OnBtnPtsetZoneModify1();

	void AddValueCol( int nQueryNo );
	void FillHisData( int nQueryNo );

	void FillData(int nQueryNo, int nLoadType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSetting)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8D03C9
	virtual ~CPTSetting();
#ifdef _DEBUG
	//##ModelId=49B87B8E004E
	virtual void AssertValid() const;
	//##ModelId=49B87B8E00BB
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTSetting)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtsetCall();
	afx_msg void OnBtnPtsetCallzone();
	afx_msg void OnBtnPtsetModify();
	afx_msg void OnBtnPtsetZoneModify();
	afx_msg void OnSelchangeTabPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbPtsetCpu();
	afx_msg void OnSelchangeCmbPtsetSrc();
	afx_msg void OnSelchangeCmbPtsetSelzone();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtsetStyle();
	afx_msg void OnBtnPtsetPrint();
	afx_msg void OnSettingprintNew1();
	afx_msg void OnSettingprintNew2();
	afx_msg void OnSettingprintNew3();
	afx_msg void OnSettingprintOrder1();
	afx_msg void OnSettingprintOrder2();
	afx_msg void OnSettingprintOrder3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtsetCall();
	afx_msg void OnDoubleclickedBtnPtsetCallzone();
	afx_msg void OnBtnPtsetVerify1();
	afx_msg void OnBtnPtsetVerify2();
	afx_msg void OnDblclkListPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtzone(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtzone(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtsetExcel();
	//}}AFX_MSG
	//##ModelId=49B87B8E034B
	void OnSTTP20016(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8E038A
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8E03C8
	void OnSTTP20125(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F000F
	void OnSTTP20107(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F004E
	void OnSTTP20052(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F006D
	void OnSTTP20054(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F008C
	void OnSTTP20048(WPARAM wParam,LPARAM lParam);
	void OnSTTP20170(WPARAM wParam,LPARAM lParam);
	void OnSTTP20174(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00AB
	void OnSTTP20056(WPARAM wParam,LPARAM lParam);
	void OnSTTP20172(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00CB
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00DC
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F00FA
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F0109
	//void ShowTip();
	afx_msg void OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_)
