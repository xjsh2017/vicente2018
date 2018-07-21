#if !defined(AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_)
#define AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTDistance.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTDistance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPTDistance : public CViewBase
{
protected:
	CPTDistance();           // protected constructor used by dynamic creation
	virtual ~CPTDistance();
	DECLARE_DYNCREATE(CPTDistance)

// Form Data
public:
	//{{AFX_DATA(CPTDistance)
	enum { IDD = IDD_PT_DISTANCE };
	CListCtrlEx	m_List;
	CButton	m_btnPrint;
	CButton	m_btnDetail;
	CButton	m_btnAckAll;
	CButton	m_btnAck;
	BOOL	m_bShowAcked;
	//}}AFX_DATA

// Attributes
public:
	CSecObj*	m_pObj;	//��Ӧ����
	/** @brief           �˳��̱߳�־*/
	BOOL	m_bExitThread;
	/** @brief           ��ȡ�߳�ָ��*/
	CWinThread*	m_pLoadThread;
	
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
private:
	int		m_nMaxRow;	//�¼������ʾ����
	CXJEventManager	m_EventManager; //�¼�����
	/** @brief           ��¼֮ǰ���������*/
	int		m_nOldSortCol;
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
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int InitListCtrl();

		/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

	void StartThread();

	void EndOwnerThread();

	int GetNextQueryNo();

	/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	BOOL AddEventToList(CXJEventDistance * pEvent, int i);

	/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	void RefreshAck(CXJEventDistance* pEvent, int nIndex = -1);

	/*
 *  @brief   	FindLocalPoint	 ����ĳ���¼��ڱ��صĵ�ַ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  Ҫ���ҵ��¼�
 *  @return 	CXJEventAlarm* ���ص�ַ
 */
	CXJEventDistance* FindLocalPoint(CXJEventDistance* pEvent);

	/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	void CheckLineCount();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTDistance)
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTDistance)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtdistanceAck();
	afx_msg void OnBtnPtdistanceAckall();
	afx_msg void OnBtnPtdistanceDetail();
	afx_msg void OnBtnPtdistancePrint();
	afx_msg void OnCheckPtdistanceHide();
	afx_msg void OnDblclkListPtdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListPtdistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTDISTANCE_H__C9318CA8_2D06_4A8A_81D8_E6B8FFF84AEA__INCLUDED_)
