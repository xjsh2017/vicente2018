#if !defined(AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_)
#define AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAlarm.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B94009C
class CPTAlarm : public CViewBase
{
protected:
	//##ModelId=49B87B9400AC
	CPTAlarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAlarm)

// Form Data
public:
	//{{AFX_DATA(CPTAlarm)
	enum { IDD = IDD_PT_ALARM };
	CButton	m_btnExcel;
	CButton	m_btnSign;
	CButton	m_btnPrint;
	CButton	m_btnDetail;
	CButton	m_btnAckAll;
	CButton	m_btnAck;
	CListCtrlEx	m_List;
	BOOL	m_bQueryAcked;
	BOOL	m_bShowDebug;
	BOOL	m_bShowReturn;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B9400FC
	CSecObj*	m_pObj;	//��Ӧ����
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B940109
	BOOL	m_bExitThread;
	/** @brief           ��ȡ�߳�ָ��*/
	//##ModelId=49B87B94010B
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B94011A
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B94011E
	int		m_nMaxRow;	//�¼������ʾ����
	//##ModelId=49B87B940129
	CXJEventManager	m_EventManager; //�¼�����
	//##ModelId=49B87B94012D
	BOOL	m_bShowAcked;	//�Ƿ���ʾ��ȷ���¼�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B940138
	int		m_nOldSortCol;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B940149
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B94014E
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B940158
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B940168
	IXJReport * pReportDoc;

	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;

private:
	CPtrList	m_listThread;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();
	int GetThreadCount();
// Operations
public:
	//##ModelId=49B87B94016D
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B940177
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B940186
	int InitListCtrl();

		/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B940196
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B9401A5
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B9401B5
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B9401C5
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B9401C6
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B9401D4
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9401E4
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B9401F4
	BOOL AddEventToList(CXJEventAlarm * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B940203
	void RefreshAck(CXJEventAlarm* pEvent, int nIndex = -1);

/*
 *  @brief   	FindLocalPoint	 ����ĳ���¼��ڱ��صĵ�ַ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  Ҫ���ҵ��¼�
 *  @return 	CXJEventAlarm* ���ص�ַ
 */
	//##ModelId=49B87B940213
	CXJEventAlarm* FindLocalPoint(CXJEventAlarm* pEvent);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

	void StartThread();

	int GetNextQueryNo();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 ��������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B940215
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B940216
	void EndOwnerThread();

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B940222
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B940223
	void ReleaseCOM();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B940224
	CString TranslateSingleKey(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAlarm)
	public:
	//##ModelId=49B87B940226
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B940232
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B940251
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B940261
	virtual ~CPTAlarm();
#ifdef _DEBUG
	//##ModelId=49B87B940271
	virtual void AssertValid() const;
	//##ModelId=49B87B940280
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAlarm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmAck();
	afx_msg void OnColumnclickListPtalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmDetail();
	afx_msg void OnBtnPtalarmSign();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtalarmAckall();
	afx_msg void OnBtnPtalarmPrint();
	afx_msg void OnCheckPtalarmHide();
	afx_msg void OnCheckPtalarmDebug();
	afx_msg void OnCheckPtalarmReturn();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtalarmExport();
	//}}AFX_MSG
	//##ModelId=49B87B9402F2
	void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9402FD
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94030D
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94031C
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94031F
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94032D
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTALARM_H__2E1DE837_E52A_43A0_A42A_8759BE156FF4__INCLUDED_)
