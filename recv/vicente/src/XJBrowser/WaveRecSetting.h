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
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87B7F006E
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           ��Ӧ����*/
	//##ModelId=49B87B7F0073
	CSecObj*	m_pObj;
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B7F007D
	BOOL		m_bExitThread;
	/** @brief           ��¼���������߳�ָ��*/
	//##ModelId=49B87B7F007F
	CWinThread*	m_pLoadThread;
private:
	/** @brief           XP�����*/
	//##ModelId=49B87B7F0084
	CThemeHelperST	m_ThemeHelper;
	/** @brief           ��ֵ�����б�*/
	//##ModelId=49B87B7F008D
	CTypedPtrArray<CPtrArray, PT_SETTING*> m_arrSetting;
	/** @brief           ��ֵ�������б�*/
	//##ModelId=49B87B7F0092
	CTypedPtrArray<CPtrArray, PT_GROUP*> m_arrGroup;
	/** @brief           ������ѡ���CPU��*/
	//##ModelId=49B87B7F0096
	CString		m_sCPU;
	/** @brief           ��ǰѡ������*/
	//##ModelId=49B87B7F009C
	int			m_nGroup;
	/** @brief           ����������*/
	//##ModelId=49B87B7F009D
	int			m_nNewValueIndex;
	/** @brief           ��ǰ״̬. 0-����. 1-�����ٻ���ֵ.*/
	//##ModelId=49B87B7F00AB
	int m_nCurrentStatus;
	/** @brief			 ��ֵ��Դ*/
	//##ModelId=49B87B7F00AC
	int			m_nSrc;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B7F00AD
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;
	/** @brief           ��¼�Ƿ��ܷ�����ʾ*/
	//##ModelId=49B87B7F00BB
	BOOL	m_bCanGroup;

	//��֡����
	/** @brief           �����֡����ʱ���յ���֡���*/
	//##ModelId=49B87B7F00BD
	CUIntArray		m_arrFrame;
	/** @brief           ��֡������֡��*/
	//##ModelId=49B87B7F00CB
	int		m_nFrameCount;
	/** @brief           ��ȡ��֡����ʱ����ֵ*/
	//##ModelId=49B87B7F00CD
	CTypedPtrArray<CPtrArray, PT_SETTING*>		m_arrNewSetting;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B7F00DB
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B7F00E0
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B7F00EE
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B7F00FB
	IXJReport * pReportDoc;

// Operations
public:
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B7F0109
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B7F0128
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B7F0148
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B7F0167
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B7F0196
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B7F01B5
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B7F01D4
	int InitControls();

/*
 *  @brief   	ClearSettingConfig	 �����ֵ���� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F01E4
	BOOL ClearSettingConfig();

/*
 *  @brief   	LoadSettingConfig	 �����ݿ���뱣����ֵ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F0203
	BOOL LoadPTSettingConfig();

/*
 *  @brief   	LoadPTGroupConfig	 ���Ҷ�ֵ������ 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F0213
	BOOL LoadPTGroupConfig();

/*
 *  @brief   	LoadOrderValue	 ���Ҷ�ֵ��׼ֵ 
 *  @param 		[In/Out]a param of Type  PT_SETTING*  ��ֵ�ṹָ��
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F0232
	BOOL LoadOrderValue(PT_SETTING* pts);

	/*
 *  @brief   	AddSettingValue	 ����ĳ��ֵֵ���б� 
 *  @param 		[In]a param of Type  CString  Ҫ����Ķ�ֵ��ID
 *  @param 		[In]a param of Type  CString  Ҫ����ĵ�ֵ
 *  @param 		[In]a param of Type  int  ָ��Ҫ���뵽�ڼ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F0251
	BOOL AddSettingValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadPTHistorySettingData	 ���붨ֵ��ʷֵ 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B7F0261
	BOOL LoadPTHistorySettingData();

/*
 *  @brief   	LoadDataFromSTTP	 ��STTP�����ж�ȡ��ֵ���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  ��������
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F0280
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0282
	void UpdateControlsEnable();

/*
 *  @brief   	ReSetGroupTab	 �������÷���TAB�� 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0290
	void ReSetGroupTab();

/*
 *  @brief   	SaveSelGroup	 ����TAB����ǰѡ�����Ϣ 
 *  @param 		[In]a param of Type  CTabCtrl*  ָ��TAB��
 *  @return 	void 
 */
	//##ModelId=49B87B7F029F
	void SaveSelGroup(CTabCtrl* pTab);

/*
 *  @brief   	ReFillCPU	 �������CPU������  
 *  @return 	void 
 */
	//##ModelId=49B87B7F02AF
	void ReFillCPU();

/*
 *  @brief   	SaveSelCPU	 ���浱ǰѡ���CPU�� 
 *  @param 		[In]a param of Type  CComboBox*  CPU������
 *  @return 	void 
 */
	//##ModelId=49B87B7F02BF
	void SaveSelCPU(CComboBox* pCmb);

/*
 *  @brief   	ClearMultiFrame	 ��ն�֡���ĵĻ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B7F02CE
	void ClearMultiFrame();

/*
 *  @brief   	TranslateMultiFrame	 ������֡�Ķ�ֵ���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *  @return 	void 
 */
	//##ModelId=49B87B7F02CF
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B7F02DE
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B7F02DF
	BOOL IsVaild20069(int nMsgID);

	/*
	*  @brief   	EndOwnerThread	 �˳������Լ�����߳�
	*  @return 	void 
	*/
	//##ModelId=49B87B7F02EE
	void EndOwnerThread();

/*
 *  @brief   	ClearSettingGroup	 �����ֵ������ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F02EF
	BOOL ClearSettingGroup();

/*
 *  @brief   	LoadSettingGroup	 ���Ҷ�ֵ�� 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F02F0
	BOOL LoadSettingGroup();

/*
 *  @brief   	SetGroupName	 ���ö�ֵ������� 
 *  @param 		[In]a param of Type  int  ��ֵ��ID
 *  @param 		[In]a param of Type  CString  ��ֵ������
 *  @return 	void 
 */
	//##ModelId=49B87B7F02F1
	void SetGroupName(int nGroupID, CString sName);

public:
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F02FF
	BOOL ReFillAll();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B7F030D
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7F030F
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B7F0310
	void ReleaseCOM();

/*
 *  @brief   	DoPrintWRSetting	 ��ӡ¼������ֵ 
 *  @param 		[In]a param of Type  CString  ��ӡģ���ļ�
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
