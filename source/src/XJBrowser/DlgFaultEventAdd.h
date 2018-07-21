#if !defined(AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_)
#define AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaultEventAdd.h : header file
//
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultEventAdd dialog

//##ModelId=49B87BA40128
class CDlgFaultEventAdd : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA4012A
	CDlgFaultEventAdd(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA40138
	~CDlgFaultEventAdd();

// Dialog Data
	//{{AFX_DATA(CDlgFaultEventAdd)
	enum { IDD = IDD_DLG_FAULTEVENT };
	CCompareCmb	m_cmbPriDev;
	CCompareCmb	m_cmbSec;
	CCompareCmb	m_cmbStation;
	CTabCtrl	m_Tab;
	CListCtrl	m_List;
	//}}AFX_DATA

private:
	/** @brief           动作故障量事件数组*/
	//##ModelId=49B87BA40144
	CPtrArray		m_listChr;
	/** @brief           动作信号量事件数组*/
	//##ModelId=49B87BA4014B
	CPtrArray		m_listSign;
	/** @brief           告警事件数组*/
	//##ModelId=49B87BA40150
	CPtrArray		m_listAlarm;
	/** @brief           录波数组*/
	//##ModelId=49B87BA40158
	CPtrArray		m_listOSC;
	/** @brief			开关量变位事件*/
	CPtrArray		m_listDIChange;

	/** @brief           故障信息包ID*/
	//##ModelId=49B87BA40167
	int				m_nMainRecID;

	CString m_sPridevID;
	CString m_sStationID;
	int m_nPridevType;
public:
	
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
private:
/*
 *  @brief   	FillStation	 填充厂站下拉框 
 *  @return 	void 
 */
	//##ModelId=49B87BA40168
	void FillStation();

/*
 *  @brief   	FillSec	 填充指定厂站下的二次设备到下拉框 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站,为NULL时表示所有厂站
 *  @return 	void 
 */
	//##ModelId=49B87BA40169
	void FillSec(CStationObj* pStation);

/*
 *  @brief   	FillSpecificSec	 填充指定厂站下的二次设备到指定下拉框 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CComboBox*  指定下拉框
 *  @return 	void 
 */
	//##ModelId=49B87BA40178
	void FillSpecificSec(CStationObj* pStation, CComboBox* pCmb);

/*
 *  @brief   	SetTimeCtrl	 设置时间控件 
 *  @return 	void 
 */
	//##ModelId=49B87BA40186
	void SetTimeCtrl();

/*
 *  @brief   	InitListStyle	 初始化列表风格 
 *  @return 	void 
 */
	//##ModelId=49B87BA40187
	void InitListStyle();

/*
 *  @brief   	ClearEvent	 删除所有事件 
 *  @return 	void 
 */
	//##ModelId=49B87BA40196
	void ClearEvent();

/*
 *  @brief   	QueryAction	 查询动作事件 
 *  @param 		[In]a param of Type  CString  二次设备ID, ""代表所有二次设备
 *  @param 		[In]a param of Type  CTime  起始时间
 *  @param 		[In]a param of Type  CTime  结束时间
 *  @return 	void 
 */
	//##ModelId=49B87BA40197
	void QueryAction(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakAction	 分解动作事件为故障事件 
 *  @param 		[In]a param of Type  CXJEventAction* 指定动作事件
 *  @return 	void 
 */
	//##ModelId=49B87BA401A6
	void BreakAction(CXJEventAction* pEvent);

/*
 *  @brief   	FillListData	 填充值到列表框 
 *  @param 		[In]a param of Type  int  要填充入的类型.0-动作特征值.1-动作信息号.2-告警.3-录波简报
 *  @return 	void 
 */
	//##ModelId=49B87BA401A8
	void FillListData(int nType);

/*
 *  @brief   	AddEventToList	 加入事件到列表 
 *  @param 		[In]a param of Type  CFaultEvent*  事件指针
 *  @param 		[In]a param of Type  int  要插入的位置索引
 *  @return 	void 
 */
	//##ModelId=49B87BA401B5
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	QueryAction	 查询告警事件 
 *  @param 		[In]a param of Type  CString  二次设备ID, ""代表所有二次设备
 *  @param 		[In]a param of Type  CTime  起始时间
 *  @param 		[In]a param of Type  CTime  结束时间
 *  @return 	void 
 */
	//##ModelId=49B87BA401B8
	void QueryAlarm(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakAlarm	 分解告警事件为故障事件-告警类型 
 *  @param 		[In]a param of Type  CXJEventAlarm*  指定告警事件
 *  @return 	void 
 */
	//##ModelId=49B87BA401C8
	void BreakAlarm(CXJEventAlarm* pEvent);

/*
 *  @brief   	QueryAction	 查询录波事件 
 *  @param 		[In]a param of Type  CString  二次设备ID, ""代表所有二次设备
 *  @param 		[In]a param of Type  CTime  起始时间
 *  @param 		[In]a param of Type  CTime  结束时间
 *  @return 	void 
 */
	//##ModelId=49B87BA401CA
	void QueryOSC(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakOSC	 分解录波事件为故障录波 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波事件
 *  @return 	void 
 */
	//##ModelId=49B87BA401D7
	void BreakOSC(CXJEventOSC* pEvent);

	void QueryDIChange(CString sSec, CTime tmStart, CTime tmEnd);
	void BreakDIChange(CXJEventDI* pEvent);

/*
 *  @brief   	SaveEvent	 保存被选择的事件到故障包 
 *  @return 	void 
 */
	//##ModelId=49B87BA401D9
	void SaveEvent();
public:
	/*
 *  @brief   	GetDevNameList	 取得根据厂站和设备类型决定的一次设备 
 *  @param 		[Out]a param of Type  DEVICE_LIST*  保存一次设备的链表
 *  @return 	void 
 */
	void GetDevNameList( DEVICE_LIST& listDevice );

/*
 *  @brief   	FillDevType	填充设备类型下拉框列表
 *  @return 	void 
 */
	void FillDevType();
	/*
 *  @brief   	FillDevName	填充一次设备下拉框列表
 *  @return 	void 
 */
	void FillDevName(CStationObj* pStation);
	void FillSecType(CStationObj* pStation);
	/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaultEventAdd)
	protected:
	//##ModelId=49B87BA401E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaultEventAdd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgFaulteventQuery();
	afx_msg void OnSelchangeDlgFaulteventTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeDlgFaulteventCmbStation();
	afx_msg void OnColumnclickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDlgFaulteventCmbDevice();
	afx_msg void OnSelchangeDlgFaulteventCmbSectype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_)
