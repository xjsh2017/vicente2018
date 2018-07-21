#if !defined(AFX_PTGENERAL_H__55AB2876_5DC8_4918_AE51_F774DCA021FD__INCLUDED_)
#define AFX_PTGENERAL_H__55AB2876_5DC8_4918_AE51_F774DCA021FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTGeneral.h : header file
//
#include "ViewBase.h"
#include "DeviceObj.h"

/////////////////////////////////////////////////////////////////////////////
// CPTGeneral form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B91006D
class CPTGeneral : public CViewBase
{
protected:
	//##ModelId=49B87B91006F
	CPTGeneral();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTGeneral)

// Form Data
public:
	//{{AFX_DATA(CPTGeneral)
	enum { IDD = IDD_PT_GENERAL };
	CStatic	m_iconStatLight;
	CStatic	m_iconCommLight;
	CButton	m_btnCommCall;
	CButton	m_btnRunHis;
	CButton	m_btnCommHis;
	CPictureEx			m_gif;
	CButton	m_btnCallTime;
	CButton	m_btnSetTime;
	CButton	m_btnReSet;
	CButton	m_btnCallGen;
	CButton	m_btnOne;
	CButton	m_btnMore;
	CButton	m_btnCPU;
	CString	m_str103Addr;
	CString	m_strAddr;
	CString	m_strCOM;
	CString	m_strCOMCheck;
	CString	m_strComm;
	CString	m_strCOMParam;
	CString	m_strCOMPro;
	CString	m_strOne;
	CString	m_strRun;
	CString	m_strTime;
	CString	m_strType;
	CString	m_strVer;
	CString	m_strInsideTime;
	CString	m_strIPA;
	CString	m_strIPB;
	CString	m_strPortA;
	CString	m_strPortB;
	CString m_strSuspend;
	CButton m_btnSuspend;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           c对应二次设备*/
	//##ModelId=49B87B91010B
	CSecObj*	m_pObj;
	/** @brief           退出线程标志*/
	//##ModelId=49B87B910119
	BOOL		m_bExitThread;
	/** @brief           记录载入数据线程的指针*/
	//##ModelId=49B87B91011B
	CWinThread*	m_pLoadThread;
	bool        m_bQuery;
    HICON m_hIcon_close;
	
	HICON m_hIcon_open;

private:
	//##ModelId=49B87B910129
	CThemeHelperST	m_ThemeHelper;
	/** @brief           目前是否正在显示更多信息*/
	//##ModelId=49B87B910138
	BOOL m_bMoreInfo;
	/** @brief           页面当前状态.0-空闲,1-正在召唤基本属性,2-正在召唤时间,3-正在强制对时,4-正在信息复归*/
	//##ModelId=49B87B910139
	int		m_nCurrentStatus;
	/** @brief           操作超时定时器索引*/
	//##ModelId=49B87B910148
	int		m_nTimer;
	/** @brief           操作计时定时器索引*/
	int			m_nRecordTimer;
	/** @brief           操作用时*/
	int			m_nOPTime;

	
private:
	CPtrList	m_listThread;
	/** @brief			查询号,最大值1000，超过1000后从0开始计数*/
	int			m_nQueryNo;
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
	void StartThread();
	
	int GetNextQueryNo();
	
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();

// Operations
public:
private:
	/*
 *  @brief   	InitControls	 初始化其它控件(除List外的)
 *  @return 	int 失败返回-1, 成功返回>=0
 */	
	//##ModelId=49B87B910149
	int InitControls();

/*
 *  @brief   	RefreshControls	 刷新控件值 
 *  @return 	void 
 */
	//##ModelId=49B87B910157
	void RefreshControls();

/*
 *  @brief   	HideSomeInfo	 隐藏某些信息 
 *  @param 		[In]a param of Type  BOOL  是否隐藏
 *  @return 	void 
 */
	//##ModelId=49B87B910167
	void HideSomeInfo(BOOL bHide);

/*
 *  @brief   	GetPTType	 从数据库中查找保护类型
 *  @param 		[In]a param of Type  int  设备类型ID
 *  @return 	 BOOL 查找成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B910177
	BOOL GetPTType(int nID);

/*
 *  @brief   	GetCOMInfo	 从数据库中查找通信信息 
 *  @param 		[In]a param of Type  int  通信口ID
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @return 	BOOL BOOL 查找成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B910196
	BOOL GetCOMInfo(int nCOM_ID, CString sStationID);

/*
 *  @brief   	GetProtocolInfo	 从数据库中查找规约信息 
 *  @param 		[In]a param of Type  CString  规约ID
 *  @return 	BOOL 查找成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B9101A5
	BOOL GetProtocolInfo(CString sID);

/*
 *  @brief   	UpdateControlsEnable	 根据页面当前状态改变控件可用性 
 *  @return 	void 
 */
	//##ModelId=49B87B9101B5
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B9101B6
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 判断收到的20069是否自己的
 *  @param 		[In]a param of Type  int  失败的操作报文ID
 *  @return 	BOOL 是自己的失败报文返回TRUE, 否则返回FALSE
 */
	//##ModelId=49B87B9101B7
	BOOL IsVaild20069(int nMsgID);

	int LoadPTStatus();

public:
/*
 *  @brief   	ReFillAll	 重新读取和填充所有内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9101C5
	BOOL ReFillAll(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 退出所有自己起的线程
 *  @return 	void 
 */
	//##ModelId=49B87B9101C6
	void EndOwnerThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTGeneral)
	public:
	//##ModelId=49B87B9101C7
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B9101D4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B9101F4
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B910203
	virtual ~CPTGeneral();
#ifdef _DEBUG
	//##ModelId=49B87B910213
	virtual void AssertValid() const;
	//##ModelId=49B87B910222
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTGeneral)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnPtgenMore();
	afx_msg void OnBtnPtgenGen();
	afx_msg void OnBtnPtgenReset();
	afx_msg void OnBtnPtgenSettime();
	afx_msg void OnBtnPtgenTime();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBtnPtgenCommhis();
	afx_msg void OnBtnPtgenRunhis();
	afx_msg void OnBtnPtgenCall();
	afx_msg HBRUSH OnCtlColor(CDC *pDC,CWnd *pWnd,UINT nCtlColor);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtgenCall();
	afx_msg void OnDoubleclickedBtnPtgenGen();
	afx_msg void OnDoubleclickedBtnPtgenTime();
	afx_msg void OnDoubleclickedBtnPtgenSettime();
	afx_msg void OnDoubleclickedBtnPtgenReset();
	afx_msg void OnBtnPtgenSuspend();
	//}}AFX_MSG
	//##ModelId=49B87B910292
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B91029F
	void OnSTTP20111(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102B1
	void OnSTTP20089(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102C0
	void OnSTTP20060(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102CE
	void OnSTTP20058(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102D1
	void OnSTTP20069(WPARAM wparam, LPARAM lParam);
	//##ModelId=49B87B9102E0
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102EE
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102F1
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9102FE
	void OnWindowRefresh(WPARAM wParam, LPARAM lParam);
	void OnSTTP20004(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTGENERAL_H__55AB2876_5DC8_4918_AE51_F774DCA021FD__INCLUDED_)
