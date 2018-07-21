#if !defined(AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_)
#define AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTHistory.h : header file
//
#include "FaultEvent.h"
/////////////////////////////////////////////////////////////////////////////
// CPTHistory form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B9001A5
class CPTHistory : public CViewBase
{
protected:
	//##ModelId=49B87B9001A7
	CPTHistory();           // protected constructor used by dynamic creation
	//##ModelId=49B87B9001C5
	~CPTHistory();
	DECLARE_DYNCREATE(CPTHistory)

// Form Data
public:
	//{{AFX_DATA(CPTHistory)
	enum { IDD = IDD_PT_HISTORY };
	CButton	m_btnCopy;
	CButton	m_btnSelNull;
	CButton	m_btnSelAll;
	CButton	m_btnSave;
	CListCtrlEx			m_List;
	CPictureEx			m_gif;
	CButton	m_btnCall;
	//}}AFX_DATA

// Attributes
public:

private:
	/** @brief           XP���ť������*/
	//##ModelId=49B87B9001E7
	CThemeHelperST	m_ThemeHelper;
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87B9001EC
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ��ǰѡ���������Դ����.0-����װ��. 1-��վ���ݿ�*/
	//##ModelId=49B87B9001F4
	int		m_nSelSrc;
	/** @brief           Ҫ�ٻ����¼�����, 0-ȫ��, 1-����, 2-�Լ���Ϣ, 3-Ӳѹ��*/
	//##ModelId=49B87B9001F5
	int		m_nEventType;
	/** @brief           ��ǰ״̬. 0-����. 1-�����ٻ�¼���б�. 2-��������¼���ļ�*/
	//##ModelId=49B87B900203
	int		m_nCurrentStatus;
	/** @brief           ��Ӧ�����豸*/
	//##ModelId=49B87B900205
	CSecObj*	m_pObj;
	/** @brief           ���涨ʱ��*/
	//##ModelId=49B87B900213
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;
	/** @brief           �洢�����������б�*/
	//##ModelId=49B87B900223
	CTypedPtrArray<CPtrArray, PT_DIGITAL*> m_arrDigital; //�洢�����������б�

	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B900233
	int		m_nOldSortCol;
	

public:
	/** @brief           ��¼�������*/
	int	m_nActionIndex;
	/** @brief           20026��������*/
	CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           �˳������̱߳�־*/
	BOOL m_bExitParse;
	/** @brief           ������ȡ��ɱ�־�����߳��ڴ��������еı��ĺ��Զ��˳�*/
	BOOL m_bDataEnd;
	/** @brief           �������*/
	//##ModelId=49B87B900232
	int m_nNum;
	/** @brief           �¼�����*/
	//##ModelId=49B87B900215
	HISTORYEVENT_LIST	m_EventList;
	/** @brief           ����20026�����߳�ָ��*/
	CWinThread* m_pParse20116Thread;

// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B900234
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

public:
/*
 *  @brief   	AddTail20026SttpData	 ��20116���ļ��뵽����β 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  20116��������
 *  @return 	void 
 */
	void AddTail20116SttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	RemoveHead20026SttpData	 ɾ��20116��������ĵ�һ�� 
 *  @return 	void 
 */
	void RemoveHead20116SttpData();

/*
 *  @brief   	TranslateHistoryAction	 ������ʷ������Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *	@param		[In]a param of Type	 int λ��
 *  @return 	void 
 */
	//##ModelId=49B87B9002DE
	void TranslateHistoryAction(STTP_FULL_DATA* pSttpData, int i);

/*
 *  @brief   	TranslateHistoryAlarm	 ������ʷ�澯��Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *	@param		[In]a param of Type	 int λ��
 *  @return 	void 
 */
	//##ModelId=49B87B9002E0
	void TranslateHistoryAlarm(STTP_FULL_DATA* pSttpData, int i);

/*
 *  @brief   	TranslateHistoryDI	 ������ʷ��������Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
  *	@param		[In]a param of Type	 int λ��
 *  @return 	void 
 */
	//##ModelId=49B87B9002EE
	void TranslateHistoryDI(STTP_FULL_DATA* pSttpData, int i);
private:
/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B900246
	int InitControls();

/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B900251
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B900261
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B900271
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B900280
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B900290
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B90029F
	BOOL NeedSave();

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B9002AF
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B9002BF
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B9002C0
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	AddEventToList	 ��ָ���¼����뵽�б������ʾ 
 *  @param 		[In]a param of Type  CFaultEvent*  ָ���¼�
 *  @param 		[In]a param of Type  int  ָ���б�������
 *  @return 	void 
 */
	//##ModelId=49B87B9002CE
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	ClearAllEvent	 ɾ�������¼���¼ 
 *  @return 	void 
 */
	//##ModelId=49B87B9002F0
	void ClearAllEvent();

/*
 *  @brief   	GetChrName	 ����ָ������ֵ���� 
 *  @param 		[In]a param of Type  int  ����ֵID
 *	@param		[Out]a param of Type CString ��λ
 *  @return 	CString ����ֵ����
 */
	//##ModelId=49B87B9002F1
	CString GetChrName(int nID, CString& strUnit);

/*
 *  @brief   	GetActionSignName	 ����ָ���ź������� 
 *  @param 		[In]a param of Type  int  �ź���ID
 *  @return 	CString �ź�������
 */
	//##ModelId=49B87B9002FE
	CString GetActionSignName(int nID);

/*
 *  @brief   	GetAlarmSignName	 ����ָ���澯�ź��������� 
 *  @param 		[In]a param of Type  int  �ź���ID
 *  @return 	CString �ź�������
 */
	//##ModelId=49B87B900300
	CString GetAlarmSignName(int nID);

/*
 *  @brief   	ClearAllDI	 ������п��������� 
 *  @return 	void 
 */
	//##ModelId=49B87B90030E
	void ClearAllDI();

/*
 *  @brief   	GetPTDI	 �����ݿ����뱾���������п��������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B90030F
	BOOL GetPTDI(int nCPU);

/*
 *  @brief   	GetDIName	 �õ�ָ�������������� 
 *  @param 		[In]a param of Type  CString  ������ID
 *  @return 	CString ����������
 */
	//##ModelId=49B87B900310
	CString GetDIName(CString sID, int nCPU);

/*
 *  @brief   	GetSelected	 ȡ��ѡ���� 
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  ������ѡ�����¼�
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  ������ѡ�澯�¼�
 *  @param 		[Out]a param of Type  HISTORYEVENT_LIST*  ������ѡ�������¼�
 *  @return 	void 
 */
	void GetSelected(HISTORYEVENT_LIST* listAction, HISTORYEVENT_LIST* listAlarm, HISTORYEVENT_LIST* listDI);

/*
 *  @brief   	SendAlarm	 ���͸澯���� 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �澯�¼�����
 *  @return 	void 
 */
	void SendAlarm(HISTORYEVENT_LIST* listAlarm);

/*
 *  @brief   	SendDI	 ���Ϳ��������� 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �������¼�����
 *  @return 	void 
 */
	void SendDI(HISTORYEVENT_LIST* listDI);

/*
 *  @brief   	SendAction	 ���Ͷ������� 
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �����¼�����
 *  @return 	void 
 */
	void SendAction(HISTORYEVENT_LIST* listAction);

/*
 *  @brief   	StartParse20116Thread	 ��������20026�����߳� 
 *  @return 	void 
 */
	void StartParse20116Thread();

/*
 *  @brief   	EndParse20116Thread	 ǿ��ֹͣ20026�������� 
 *  @return 	void 
 */
	void EndParse20116Thread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTHistory)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	//##ModelId=49B87B90034B
	virtual void AssertValid() const;
	//##ModelId=49B87B90035B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTHistory)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPthistoryCall();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeCmbPthistorySrc();
	afx_msg void OnSelchangeCmbPthistoryType();
	afx_msg void OnColumnclickListPthistory(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPthistorySave();
	afx_msg void OnBtnPthistorySelNull();
	afx_msg void OnBtnPthistorySelAll();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnPthistorySelCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPthistoryCall();
	afx_msg void OnBtnPthistoryFault();
	//}}AFX_MSG
	//##ModelId=49B87B9003C0
	afx_msg void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9003C9
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9003D9
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B910000
	afx_msg void OnSTTP20116(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B91000F
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B910013
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadWorkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTHISTORY_H__A6E9C41E_2446_4963_8A9B_F3DF3219EBCC__INCLUDED_)
