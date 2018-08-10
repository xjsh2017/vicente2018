#if !defined(AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_)
#define AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTSoftBoard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ViewBase.h"
#include "HisValueMgr.h"

//##ModelId=49B87B89037A
class CPTSoftBoard : public CViewBase
{
protected:
	//##ModelId=49B87B89037C
	CPTSoftBoard();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSoftBoard)

// Form Data
public:
	//{{AFX_DATA(CPTSoftBoard)
	enum { IDD = IDD_PT_SOFTBOARD };
	CButtonST	m_btnVerify2;
	CButtonST	m_btnVerify1;
	CPictureEx			m_gif;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnViewProg;
	CButton	m_btnViewHis;
	CButton	m_btnModify;
	CButton	m_btnCall;
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B8903C9
	CSecObj *	m_pObj; //对应的保护指针
	//##ModelId=49B87B8903CD
	CString		m_sCPU;	//所选CPU	
	//##ModelId=49B87B8903CF
	CArray<SColumn, SColumn&> m_arrColum;	//列表的列信息
	/** @brief           线程退出标志*/
	//##ModelId=49B87B8903D3
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程指针*/
	//##ModelId=49B87B8903D9
	CWinThread*	m_pLoadThread;

	CString     m_sOperUser;
	CString     m_sMonUser;
	bool        m_bOperVerify;
	bool        m_bMonVerify;

	CPoint  m_pointViewProg;
	bool	m_bAlreadyShowOnce;

	/** @brief			是否正在预校*/
	BOOL		m_bChecking;
private:
	CString         m_strUser;
	//##ModelId=49B87B8903DE
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B8A0001
	CTypedPtrArray<CPtrArray, PT_SOFTBOARD*> m_arrSoftBoard; //存储开关量描述列表
	/** @brief           最新值列*/
	//##ModelId=49B87B8A0005
	int m_nNewValueIndex;
	/** @brief           当前状态. 0-空闲状态, 1-正在召唤软压板, 2-正在软压板投退*/
	//##ModelId=49B87B8A000F
	int m_nCurrentStatus;
	/** @brief           是否第一帧*/
	BOOL m_bFirstFrame;
	/** @brief           修改值链表*/
	//##ModelId=49B87B8A0011
	MODIFY_LIST		m_arrModifyList;
	/** @brief           修改值文本描述*/
	//##ModelId=49B87B8A001F
	CString m_strOutPut;
	/** @brief           操作超时定时器索引*/
	//##ModelId=49B87B8A0020
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;	
	/** @brief           软压板投退定时*/
	int			m_nPTSetTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	/** @brief           打印模板标识*/
	//##ModelId=49B87B8A002F
	CLSID clsid;
	/** @brief           COM类工厂指针*/
	//##ModelId=49B87B8A003F
	LPCLASSFACTORY pClf;
	/** @brief           COM指针*/
	//##ModelId=49B87B8A0044
	LPUNKNOWN pUnk;
	/** @brief           打印模板接口指针*/
	//##ModelId=49B87B8A004F
	IXJReport * pReportDoc;

	/** @brief			查询号,最大值1000，超过1000后从0开始计数*/
	int			m_nQueryNo;
	CHisValueMgr	m_HisValueMgr;
	/** @brief           当前状态. 0-空闲. 1-正在执行软压板投退*/
	int m_nCurrentDetailStatus;

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
	//##ModelId=49B87B8A005D
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8A007D
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 填充列表数据 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8A00AB
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	载入列信息配置	  
	*  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8A00CB
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 保存列信息配置 
	*  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
	*/
	//##ModelId=49B87B8A00EA
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 判断是否需要保存列表的列信息
	*  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
	*/
	//##ModelId=49B87B8A0109
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B8A0128
	int InitControls();

/*
 *  @brief   	ClearDigitalConfig	 清除软压板配置信息 
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0167
	BOOL ClearSoftBoardConfig();

/*
 *  @brief   	AddDigitalValue	 加入某软压板值到列表 
 *  @param 		[In]a param of Type  CString  要加入的软压板的ID
 *  @param 		[In]a param of Type  CString  要加入的软压板的值
 *  @param 		[In]a param of Type  int  指定要加入到第几列
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0177
	BOOL AddSoftBoardValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadDataFromSTTP	 从STTP报文中读取软压板数据 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  报文数据
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A01B5
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 更新各控件是否可用 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01C5
	void UpdateControlsEnable();

/*
 *  @brief   	CheckModifyData	 组建修改值链表 
 *  @param 		[Out]a param of Type  MODIFY_LIST&  修改值链表
 *  @param 		[Out]a param of Type  CString&  输出文本
 *  @return 	void 
 */
	//##ModelId=49B87B8A01D4
	void CheckModifyData( MODIFY_LIST& modifyList, CString& strOut);

/*
 *  @brief   	ClearModifyList	 清空修改链表 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01E5
	void ClearModifyList();

/*
 *  @brief   	RefreshControls	 刷新控件值 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01F4
	void RefreshControls();
	
/*
 *  @brief   	SaveSelCPU	 保存当前选择的CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU下拉框指针
 *  @return 	void 
 */	
	//##ModelId=49B87B8A01F5
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 删除新加的数据列 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0203
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B8A0204
	void CancelCall();

/*
 *  @brief   	SetSoftBoardValue	 设置指定的软压板值 
 *  @param 		[In]a param of Type  CString  软压板ID
 *  @param 		[In]a param of Type  CString  软压板值
 *  @param 		[In]a param of Type  int  列索引
 *  @return 	void 
 */
	//##ModelId=49B87B8A0213
	void SetSoftBoardValue( CString ID, CString VALUE, int iCol);

/*
 *  @brief   	RevertModifyValue	 回复修改软压板值时修改的值 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0222
	void RevertModifyValue();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B8A0223
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	DisplayValue	 得到软压板的值显示字符串 
 *  @param 		[In]a param of Type  int  软压板值
 *  @return 	CString 显示值
 */
	//##ModelId=49B87B8A0225
	CString DisplayValue(int nValue);

	/*
	* @brief	FindNewValue	查找指定点的最新值	
	* @param	[In]a param of Type int nID	指定定值点	
	* @return   CString	指定点的最新值
	*/
	CString FindNewValue(int nID);

	PT_SOFTBOARD* FindSoftBoard(CString sID);

	void StartThread();

	int GetNextQueryNo();

	void AddValueCol( int nQueryNo );

	BOOL LoadDefaultCpu();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	/*
 *  @brief   	LoadPTHistoryDigitalData	 载入软压板历史值 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0196
	BOOL LoadPTHistorySoftBoardData(int nQueryNo);

	/*
 *  @brief   	LoadPTDigitalConfig	 从数据库读取软压板配置信息 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0148
	BOOL LoadPTSoftBoardConfig(int nQueryNo);

	
	int m_nOperationNum;
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0232
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B8A0233
	void EndOwnerThread();

/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87B8A0234
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8A0242
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0243
	void ReleaseCOM();

/*
 *  @brief   	DoPrintSoftBoard	 打印软压板 
 *  @param 		[In]a param of Type  CString  打印模板文件
 *  @return 	void 
 */
	//##ModelId=49B87B8A0244
	void DoPrintSoftBoard(CString strPath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSoftBoard)
	public:
	//##ModelId=49B87B8A0246
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8A0271
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8A029F
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8A02B1
	virtual ~CPTSoftBoard();
#ifdef _DEBUG
	//##ModelId=49B87B8A02DE
	virtual void AssertValid() const;
	//##ModelId=49B87B8A02FD
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 执行软压板投退的修改
	void ExcutePTSet_Soft();

	// Generated message map functions
	//{{AFX_MSG(CPTSoftBoard)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtsoftCall();
	afx_msg void OnBtnPtsoftModify();
	afx_msg void OnBtnPtsoftModify2();
	afx_msg void OnBtnViewProg();
	afx_msg void OnBtnViewHis();
	afx_msg void OnSelchangeCmbPtsoftCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtsoft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtsoftStyle();
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtsoftCall();
	afx_msg void OnBtnPtsoftVerify1();
	afx_msg void OnBtnPtsoftVerify2();
	afx_msg void OnDblclkListPtsoft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtsoftExcel();
	//}}AFX_MSG
	//##ModelId=49B87B8A03DE
	void OnSTTP20109(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B000F
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B001F
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B003E
	void OnSTTP20118(WPARAM wParam,LPARAM lparam);
	//##ModelId=49B87B8B004E
	void OnSTTP20114(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B005D
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B0060
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B006F
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B007F
	afx_msg void OnRefreshPTSoft(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_)
