#if !defined(AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_)
#define AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaultReport.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultReport dialog

//##ModelId=49B87BA4002E
class CDlgFaultReport : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40030
	CDlgFaultReport(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA4003E
	~CDlgFaultReport();

// Dialog Data
	//{{AFX_DATA(CDlgFaultReport)
	enum { IDD = IDD_DLG_FAULT };
	CCompareCmb	m_cmbStation2;
	CCompareCmb	m_cmbStation1;
	CCompareCmb	m_cmbDevice;
	CEdit	m_FaultReasonEdit;
	CTabCtrl	m_Tab;
	CListCtrl	m_List;
	CString	m_sName;
	CString	m_sDistance;
	CString	m_sMs;
	//}}AFX_DATA
public:
	/** @brief           保存对应的故障报告指针*/
	//##ModelId=49B87BA40051
	CFaultReport* m_pFaultReport;
	/** @brief           记录是否需要保存事件*/
	//##ModelId=49B87BA40055
	BOOL		  m_bEventNeedSave;
	/** @brief           保存调用者指针*/
	//##ModelId=49B87BA40057
	CWnd*		  m_pWnd;
	CString       m_sTitle;
	
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
public:
	/** @brief           动作故障量事件数组*/
	//##ModelId=49B87BA40060
	CPtrArray		m_listChr;
	/** @brief           动作信号量事件数组*/
	//##ModelId=49B87BA40065
	CPtrArray		m_listSign;
	/** @brief           告警事件数组*/
	//##ModelId=49B87BA4006A
	CPtrArray		m_listAlarm;
	/** @brief           保护录波数组*/
	//##ModelId=49B87BA4006F
	CPtrArray		m_listPTOSC;
	/** @brief           录波器录波数组表*/
	//##ModelId=49B87BA40074
	CPtrArray		m_listWROSC;
	/** @brief			开关量变位数组*/
	CPtrArray		m_listDIChange;
	/** @brief			小波测距数组*/
	CPtrArray		m_listDistance;
	/** @brief			录波分析数组*/
	CPtrArray		m_listOSCParse;

	int             m_nCurTabSel;
	CString         m_sFaultReason;
private:
/*
 *  @brief   	InitCMBFaultType	 初始化故障类型下拉框 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4007E
	BOOL InitCMBFaultType();

/*
 *  @brief   	InitCMBStation	 初始化厂站下拉框 
 *  @param 		[In]a param of Type  CString  预选择的厂站ID
 *	@param		[In]a param of Type	 UINT	下拉框ID
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4007F
	BOOL InitCMBStation(CString sID, UINT nID);

/*
 *  @brief   	InitCMBDevice	 初始化一次设备下拉框 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4008E
	BOOL InitCMBDevice();

/*
 *  @brief   	InitCMBSaved     初始化是否存档下拉框
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4007E
	BOOL InitCMBIsSaved();

/*
 *  @brief   	FillSpecificPriDevice	 将指定厂站下的指定类型的一次设备填充到指定下拉框 
 *  @param 		[In]a param of Type  CComboBox*  下拉框指针
 *  @param 		[In]a param of Type  CStationObj*  厂站指针
 *  @param 		[In]a param of Type  int  一次设备类型
 *  @return 	void 
 */
	//##ModelId=49B87BA4009C
	void FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType);

/*
 *  @brief   	InitListStyle	 初始化列表风格 
 *  @return 	void 
 */
	//##ModelId=49B87BA400AB
	void InitListStyle();

/*
 *  @brief   	ClearDetail	 清除详细信息
 *  @return 	void 
 */
	//##ModelId=49B87BA400AC
	void ClearDetail();

/*
 *  @brief   	ReadDetail	 读入详细信息 
 *  @return 	void 
 */
	//##ModelId=49B87BA400AD
	void ReadDetail();

/*
 *  @brief   	FillListData	 填充值到列表框 
 *  @param 		[In]a param of Type  int  要填充入的类型.0-动作特征值.1-动作信息号.2-告警.3-录波简报
 *  @return 	void 
 */
	//##ModelId=49B87BA400AE
	void FillListData(int nType);

/*
 *  @brief   	AddEventToList	 加入事件到列表 
 *  @param 		[In]a param of Type  CFaultEvent*  事件指针
 *  @param 		[In]a param of Type  int  要插入的位置索引
 *  @return 	void 
 */
	//##ModelId=49B87BA400BC
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	NeedSaveReport	 判断本故障信息包数据是否需要保存 
 *  @return 	BOOL 需要保存返回TRUE, 不需要保存返回FALSE
 */
	//##ModelId=49B87BA400CB
	BOOL NeedSaveReport();

/*
 *  @brief   	RemoveFromList	 从列表框中移除事件 
 *  @param 		[In]a param of Type  CFaultEvent*  指定要移除的事件
 *  @return 	void 
 */
	//##ModelId=49B87BA400CC
	void RemoveFromList(CFaultEvent* pEvent);

/*
 *  @brief   	RemoveFromArray	 从事件链表中删除事件 
 *  @param 		[In]a param of Type  CFaultEvent*  指定要移除的事件
 *  @return 	void 
 */
	//##ModelId=49B87BA400CE
	void RemoveFromArray(CFaultEvent* pEvent);

/*
 *  @brief   	SaveEvent	 保存故障事件 
 *  @return 	int 保存成功0,删除数据失败返回1,保存数据失败返回2
 */
	//##ModelId=49B87BA400DA
	int SaveEvent();

/*
 *  @brief   	RenameOscFile	 录波文件重命名 
 *  @return 	CFaultEvent*     录波简报事件对象
 */
	void RenameOscFile( CFaultEvent* pEvent );

public:
	/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
/*
 *  @brief   	UpdateList	 刷新列表框显示 
 *  @return 	void 
 */
	//##ModelId=49B87BA400DB
	void UpdateList();

	void SelCurTab(int nSel);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaultReport)
	protected:
	//##ModelId=49B87BA400DC
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaultReport)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDlgFaultTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDlgFaultRemove();
	afx_msg void OnDlgFaultAdd();
	afx_msg void OnDlgFaultAddosc();
	afx_msg void OnColumnclickDlgFaultList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_)
