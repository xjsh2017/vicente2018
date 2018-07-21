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
	/** @brief           c��Ӧ�����豸*/
	//##ModelId=49B87B91010B
	CSecObj*	m_pObj;
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B910119
	BOOL		m_bExitThread;
	/** @brief           ��¼���������̵߳�ָ��*/
	//##ModelId=49B87B91011B
	CWinThread*	m_pLoadThread;
	bool        m_bQuery;
    HICON m_hIcon_close;
	
	HICON m_hIcon_open;

private:
	//##ModelId=49B87B910129
	CThemeHelperST	m_ThemeHelper;
	/** @brief           Ŀǰ�Ƿ�������ʾ������Ϣ*/
	//##ModelId=49B87B910138
	BOOL m_bMoreInfo;
	/** @brief           ҳ�浱ǰ״̬.0-����,1-�����ٻ���������,2-�����ٻ�ʱ��,3-����ǿ�ƶ�ʱ,4-������Ϣ����*/
	//##ModelId=49B87B910139
	int		m_nCurrentStatus;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B910148
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	
private:
	CPtrList	m_listThread;
	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;
	/** @brief           ��*/
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
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B910149
	int InitControls();

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B910157
	void RefreshControls();

/*
 *  @brief   	HideSomeInfo	 ����ĳЩ��Ϣ 
 *  @param 		[In]a param of Type  BOOL  �Ƿ�����
 *  @return 	void 
 */
	//##ModelId=49B87B910167
	void HideSomeInfo(BOOL bHide);

/*
 *  @brief   	GetPTType	 �����ݿ��в��ұ�������
 *  @param 		[In]a param of Type  int  �豸����ID
 *  @return 	 BOOL ���ҳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B910177
	BOOL GetPTType(int nID);

/*
 *  @brief   	GetCOMInfo	 �����ݿ��в���ͨ����Ϣ 
 *  @param 		[In]a param of Type  int  ͨ�ſ�ID
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	BOOL BOOL ���ҳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B910196
	BOOL GetCOMInfo(int nCOM_ID, CString sStationID);

/*
 *  @brief   	GetProtocolInfo	 �����ݿ��в��ҹ�Լ��Ϣ 
 *  @param 		[In]a param of Type  CString  ��ԼID
 *  @return 	BOOL ���ҳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9101A5
	BOOL GetProtocolInfo(CString sID);

/*
 *  @brief   	UpdateControlsEnable	 ����ҳ�浱ǰ״̬�ı�ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B9101B5
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B9101B6
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B9101B7
	BOOL IsVaild20069(int nMsgID);

	int LoadPTStatus();

public:
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9101C5
	BOOL ReFillAll(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
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
