#if !defined(AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_)
#define AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCallResult.h : header file
//
#include <afxdb.h>
#include "DlgExportProcess.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BB303C8
class CCallResult : public CObject
{
public:
	//##ModelId=49B87BB303CA
	CCallResult();
	//##ModelId=49B87BB40000
	~CCallResult();
public:
	//0:未召唤 1：成功 2：失败 3: 装置不支持
	/** @brief           定值区号结果*/
	//##ModelId=49B87BB4001F
	int m_nZone;
	/** @brief           定值结果*/
	//##ModelId=49B87BB40020
	int m_nSetting;
	/** @brief           开关量结果*/
	//##ModelId=49B87BB4002E
	int m_nDigital;
	/** @brief           软压板结果*/
	//##ModelId=49B87BB4002F
	int m_nSoftBoard;
	/** @brief           模拟量结果*/
	//##ModelId=49B87BB40030
	int m_nAnalog;
	int	m_nOsc; //录波结果
	int m_nHistory; //历史事件结果

	/** @brief           定值区号结果说明*/
	//##ModelId=49B87BB40031
	CString m_sZoneNote;
	/** @brief           定值结果说明*/
	//##ModelId=49B87BB40032
	CString m_sSettingNote;
	/** @brief           开关量结果说明*/
	//##ModelId=49B87BB4003E
	CString m_sDigitalNote;
	/** @brief           软压板结果说明*/
	//##ModelId=49B87BB4003F
	CString m_sSoftBoardNote;
	/** @brief           模拟量结果说明*/
	//##ModelId=49B87BB40040
	CString m_sAnalogNote;
	CString	m_sOscNote; //录波结果说明
	CString m_sHistoryNote;//历史事件结果说明

	CString		m_sSituation; //情况说明编号
	CString		m_sDispose; //处理意见编号

	/** @brief           所属批次*/
	//##ModelId=49B87BB40041
	int m_nSaveNo;

	/** @brief           二次设备ID*/
	//##ModelId=49B87BB40042
	CString		m_sSecID;
	/** @brief           二次设备指针*/
	//##ModelId=49B87BB4004F
	CSecObj*	m_pSec;
	/** @brief           CPU号*/
	//##ModelId=49B87BB40053
	int			m_nCPU;
	/** @brief           完成时间*/
	//##ModelId=49B87BB4005D
	CTime		m_tmEnd;
	/** @brief           厂站ID*/
	//##ModelId=49B87BB4005E
	CString		m_sStationId;
	/** @brief           厂站指针*/
	//##ModelId=49B87BB40060
	CStationObj*	m_pStation;

public:
/*
 *  @brief   	LoadDataFromDB	 从数据库读入数据信息 
 *  @param 		[In]a param of Type  CMemSet*  数据库对象
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB4006D
	BOOL LoadDataFromDB(CMemSet* pMemset);

private:
/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB4008C
	BOOL BuildReleation();
};

/** @brief           召唤结果链表*/
//##ModelId=49B87BB400AB
typedef CTypedPtrList<CObList , CCallResult*>	CALLRESULT_LIST;

//##ModelId=49B87BB400BC
typedef struct Result_Stat
{
	int nSuccess;
	int nFail;
}Result_Stat;

//##ModelId=49B87BB400BE
typedef struct SaveNumber
{
	int		nSaveNo; //批次号
	CTime	tmTime;	 //时间
}SaveNumber;

//##ModelId=49B87BB40148
class CAutoCallResult : public CFormView
{
protected:
	//##ModelId=49B87BB40158
	CAutoCallResult();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAutoCallResult)

// Form Data
public:
	//{{AFX_DATA(CAutoCallResult)
	enum { IDD = IDD_AUTOCALL_RESULT };
	/** @brief           概述结果列表框*/
	//##ModelId=49B87BB40178
	CListCtrlEx	m_ListSummary;
	/** @brief           详细结果列表框*/
	//##ModelId=49B87BB4017D
	CListCtrlEx	m_List;
	//}}AFX_DATA

	//##ModelId=49B87BB40182
	CLSID clsid;
	//##ModelId=49B87BB40187
	LPCLASSFACTORY pClf;
	//##ModelId=49B87BB4018C
	LPUNKNOWN pUnk;
	//##ModelId=49B87BB40191
	IXJReport * pReportDoc;

	CDlgExportProcess* m_pExportProcess;

// Attributes
private:
	/** @brief           列表的列信息*/
	//##ModelId=49B87BB40196
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           结果链表*/
	//##ModelId=49B87BB4019B
	CALLRESULT_LIST	m_listResult;

	/** @brief           定值区号结果统计*/
	//##ModelId=49B87BB401A0
	Result_Stat	m_ZoneStat;
	/** @brief           定值结果统计*/
	//##ModelId=49B87BB401A6
	Result_Stat	m_SettingStat;
	/** @brief           开关量结果统计*/
	//##ModelId=49B87BB401AB
	Result_Stat m_DigitalStat;
	/** @brief           软压板结果统计*/
	//##ModelId=49B87BB401B0
	Result_Stat m_SoftBoardStat;
	/** @brief           模拟量结果统计*/
	//##ModelId=49B87BB401B6
	Result_Stat m_AnalogStat;
	Result_Stat	m_OscStat;//录波结果统计
	Result_Stat	m_nHistoryStat;//历史事件统计

	/** @brief           总召开始时间*/
	//##ModelId=49B87BB401BA
	CTime		m_tmStart;
	/** @brief           总召结束时间*/
	//##ModelId=49B87BB401C5
	CTime		m_tmEnd;

	/** @brief           记录之前被排序的列 */
	//##ModelId=49B87BB401C6
	int		m_nOldSortCol;

	/** @brief           记录当前选择的厂站*/
	//##ModelId=49B87BB401D5
	CStationObj* m_pStation;

	/** @brief           记录总召的批次号*/
	//##ModelId=49B87BB401E5
	CPtrArray	m_arrSaveNo;

	/** @brief           记录最新的批次号*/
	//##ModelId=49B87BB401E9
	int			m_nNewSaveNo;

	/** @brief           记录最老批次号*/
	//##ModelId=49B87BB401F4
	int			m_nOldSaveNo;

	/** @brief           记录最大批次号*/
	//##ModelId=49B87BB40203
	int			m_nMaxSaveNo;

	/** @brief           记录最后批次号*/
	//##ModelId=49B87BB40204
	int			m_nMinSaveNo;

	/** @brief           当前选择的批次号*/
	//##ModelId=49B87BB40213
	int			m_nSelSaveNo;

	/** @brief           是否显示无失败条目*/
	BOOL		m_bShowNoFailed;

	BOOL		m_bSelectRoot;
public:
/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87BB40214
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB40232
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB40242
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87BB40261
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 		void 
	*/
	//##ModelId=49B87BB40271
	void RegulateControlSize();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87BB40280
	int	InitListStyle();

/*
 *  @brief   	InitSummaryListStyle	 初始化概述列表框的风格 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87BB40290
	int InitSummaryListStyle();

/*
 *  @brief   	LoadDataFromDB	 从数据库读入数据 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB4029F
	BOOL LoadDataFromDB(int nSaveNo);

/*
 *  @brief   	Clear	 清除数据 
 *  @return 	void 
 */
	//##ModelId=49B87BB402BF
	void Clear();

/*
 *  @brief   	StatResult	 将指定结果加入到结果统计中 
 *  @param 		[In]a param of Type  CCallResult*  指定结果
 *  @return 	void 
 */
	//##ModelId=49B87BB402C0
	void StatResult(CCallResult* pResult);

/*
 *  @brief   	FillListData	 将所有召唤结果加入到列表框中显示 
 *  @return 	void 
 */
	//##ModelId=49B87BB402DE
	void FillListData();

/*
 *  @brief   	AddResultToList	 把指定结果加入到列表中显示 
 *  @param 		[In]a param of Type  CCallResult*  指定结果
 *	@param		[In]a param of Type int	要加入的位置
 *  @return 	void 
 */
	//##ModelId=49B87BB402DF
	void AddResultToList(CCallResult* pResult, int nIndex = 0);

/*
 *  @brief   	GetResultString	 取得结果字符串 
 *  @param 		[In]a param of Type  int  结果标志
 *  @return 	CString 结果字符串
 */
	//##ModelId=49B87BB402EF
	CString GetResultString(int nResult);

/*
 *  @brief   	GetCPU	 取得指定二次设备的指定CPU号的CPU对象 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @param 		[In]a param of Type  int  指定CPU号
 *  @return 	CSecCPU* 找到返回CPU对象的指针, 未找到返回NULL
 */
	//##ModelId=49B87BB402FE
	CSecCPU* GetCPU(CSecObj* pObj, int nCpu);

/*
 *  @brief   	FillSummaryListData	 填充概述列表框数据 
 *  @return 	void 
 */
	//##ModelId=49B87BB4031C
	void FillSummaryListData();

/*
 *  @brief   	GetOCTime	 从数据库查询自动总召的时间 
 *  @param 		[In]a param of Type  CString  对应tb_sys_config中的KEY
 *  @param 		[Out]a param of Type  CTime&  时间
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB4031D
	BOOL GetOCTime(CString sKey, CTime& tmTime);

/*
 *  @brief   	ReFillAll	 重新载入所有数据 
 *  @return 	void 
 */
	//##ModelId=49B87BB4032D
	void ReFillAll(int nSaveNo);

/*
 *  @brief   	ClearStat	 清除统计 
 *  @return 	void 
 */
	//##ModelId=49B87BB4033C
	void ClearStat();

/*
 *  @brief   	AddResultToExcel	 将指定的结果项写入到指定的Excel文件中 
 *  @param 		[In]a param of Type  CString  数据库对象
 *  @param 		[In]a param of Type  CCallResult*  结果项
 *  @return 	void 
 */
	//##ModelId=49B87BB4033D
	void AddResultToExcel(CDatabase* pDatabase, CCallResult* pResult);

/*
 *  @brief   	WriteResultToExcel	将结果导出到Excel 
 *  @param 		[In]a param of Type  CString  文件路径
 *  @return 	void 
 */
	//##ModelId=49B87BB4034C
	BOOL WriteResultToExcel(CString strFullPath);

	

/*
 *  @brief   	InitCOM	 初始化打印模板COM组件 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB4035B
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 删除COM组件 
 *  @return 	void 
 */
	//##ModelId=49B87BB4035C
	void ReleaseCOM();

/*
 *  @brief   	QuerySaveNo	 查询指定站点的总召批次 
 *  @param 		[In]a param of Type  CString  站ID
 *  @return 	void 
 */
	//##ModelId=49B87BB4035D
	void QuerySaveNo(CString sStationID);

/*
 *  @brief   	QueryNewSaveNo	 查询指定站点的最新的总召批次号 
 *  @param 		[In]a param of Type  CString  站ID
 *  @return 	void 
 */
	//##ModelId=49B87BB4035F
	void QueryNewSaveNo(CString sStationID);

/*
 *  @brief   	GetPrevSaveNo	 取得上一批次的批次号 
 *  @param 		[In]a param of Type  int  当前批次号
 *  @return 	int 上一批次批次号
 */
	//##ModelId=49B87BB4036C
	int	GetPrevSaveNo(int nCurSaveNo);

/*
 *  @brief   	GetNextSaveNo	 取得下一批次的批次号 
 *  @param 		[In]a param of Type  int  当前批次号
 *  @return 	int 下一批次批次号
 */
	//##ModelId=49B87BB4036E
	int GetNextSaveNo(int nCurSaveNo);

/*
 *  @brief   	ClearSaveNo	 清除批次记录 
 *  @return 	void 
 */
	//##ModelId=49B87BB4037A
	void ClearSaveNo();

/*
 *  @brief   	SortSaveNo	 对批次按时间从旧到新进行排序 
 *  @return 	void 
 */
	//##ModelId=49B87BB4037B
	void SortSaveNo();

/*
 *  @brief   	HasFaildItem	 判断一个自动总召的结果项是否有失败条目 
 *  @param 		[In]a param of Type  CCallResult*  指定自动总召结果项对象
 *  @return 	BOOL 有失败条目返回TRUE, 无失败条目返回FALSE
 */
	BOOL HasFaildItem(CCallResult* pResult);

public:
	/*
 *  @brief   	TranslateSingleKey	 解释单个关键字的意义 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 关键字表示的值
 */
	//##ModelId=49B87BB4037C
	CString TranslateSingleKey(CString sKey);

	BOOL WriteResultToExcel( CString strFullPath, CString sStationID );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCallResult)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB403C8
	virtual ~CAutoCallResult();
#ifdef _DEBUG
	//##ModelId=49B87BB50000
	virtual void AssertValid() const;
	//##ModelId=49B87BB5000F
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAutoCallResult)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListAutocallResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnResultExport();
	afx_msg void OnResultPrint();
	afx_msg void OnResultPrev();
	afx_msg void OnResultNext();
	afx_msg void OnResultShowfaild();
	afx_msg void OnWinClose();
	afx_msg void OnResultStationexport();
	//}}AFX_MSG
	//##ModelId=49B87BB5009E
	afx_msg void OnAutoCallResultFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB500AB
	afx_msg void OnAutoCallResultChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_)
