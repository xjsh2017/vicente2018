#if !defined(AFX_WAVERECGENERAL_H__1CB22FE7_7CE0_45B0_90D4_58FCD965E622__INCLUDED_)
#define AFX_WAVERECGENERAL_H__1CB22FE7_7CE0_45B0_90D4_58FCD965E622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRecGeneral.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B800280
class CWaveRecGeneral : public CViewBase
{
protected:
	//##ModelId=49B87B800291
	CWaveRecGeneral();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWaveRecGeneral)

// Form Data
public:
	//{{AFX_DATA(CWaveRecGeneral)
	enum { IDD = IDD_WAVEREC_GENERAL };
	CStatic	m_iconStatLight;
	CStatic	m_iconCommLight;
	CButton	m_btnCall;
	CButton	m_btnCommHis;
	CButton	m_btnRunHis;
	CPictureEx			m_gif;
	CButton	m_btnTime;
	CButton	m_btnStart;
	CButton	m_btnRel;
	CButton	m_btnMore;
	CString	m_strCommStatus;
	CString	m_strRunStatus;
	CString	m_strType;
	CString	m_strVer;
	bool    m_bGetStatus;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           ҳ���Ӧ�����豸ָ��*/
	//##ModelId=49B87B8002D1
	CSecObj*	m_pObj;
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B8002DE
	BOOL		m_bExitThread;
	/** @brief           ��¼���������߳�ָ��*/
	//##ModelId=49B87B8002E0
	CWinThread*	m_pLoadThread;
	HICON m_hIcon_close;
	
	HICON m_hIcon_open;

private:
	//##ModelId=49B87B8002EF
	CThemeHelperST	m_ThemeHelper;
	/** @brief           �Ƿ���ʾ��Ϣ*/
	//##ModelId=49B87B8002F3
	BOOL	m_bMoreInfo;
	/** @brief           ҳ�浱ǰ����״̬.0-����. 1-����ǿ�ƶ�ʱ. 2-����Զ������*/
	//##ModelId=49B87B8002FD
	int		m_nCurrentStatus;
	/** @brief           CPU������ǰѡ���CPU��*/
	//##ModelId=49B87B8002FE
	int		m_nCPU;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B80030D
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

// Operations
public:
private:
	/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B80030E
	int InitControls();

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B80031C
	void RefreshControls();

/*
 *  @brief   	HideSomeInfo	 ����/��ʾĳЩ��Ϣ 
 *  @param 		[In]a param of Type  BOOL  ָ���Ƿ�����
 *  @return 	void 
 */
	//##ModelId=49B87B80031D
	void HideSomeInfo(BOOL bHide);
	
/*
 *  @brief   	UpdateControlsEnable	 ����ҳ�浱ǰ����״̬�ı�ؼ����ö� 
 *  @return 	void 
 */
	//##ModelId=49B87B80032C
	void UpdateControlsEnable();

/*
 *  @brief   	FillCPU	 ���CPU������  
 *  @return 	void 
 */
	//##ModelId=49B87B80032D
	void FillCPU();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B80032E
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B80033C
	BOOL IsVaild20069(int nMsgID);

	/*
	*  @brief   	EndOwnerThread	 �˳������Լ�����߳�
	*  @return 	void 
	*/
	//##ModelId=49B87B80033E
	void EndOwnerThread();
	
	int LoadOSCStatus();
public:
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B80033F
	BOOL ReFillAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveRecGeneral)
	public:
	//##ModelId=49B87B800340
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B80034B
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B80034E
	virtual ~CWaveRecGeneral();
#ifdef _DEBUG
	//##ModelId=49B87B80035B
	virtual void AssertValid() const;
	//##ModelId=49B87B80035D
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWaveRecGeneral)
	afx_msg void OnBtnWrgenMore();
	afx_msg void OnBtnWrgenTime();
	afx_msg void OnBtnWrgenStart();
	afx_msg void OnSelchangeCmbWrgenCpu();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnWrgenCommhis();
	afx_msg void OnBtnWrgenRunhis();
	afx_msg void OnBtnWrgenCall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDoubleclickedBtnWrgenCall();
	afx_msg void OnDoubleclickedBtnWrgenStart();
	afx_msg void OnDoubleclickedBtnWrgenTime();
	//}}AFX_MSG
	//##ModelId=49B87B80038D
	void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800399
	void OnSTTP20060(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B80039C
	void OnSTTP20124(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8003AB
	void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8003B9
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8003BC
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	void OnSTTP20086(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8003C8
	void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8003CB
	void OnWindowRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERECGENERAL_H__1CB22FE7_7CE0_45B0_90D4_58FCD965E622__INCLUDED_)
