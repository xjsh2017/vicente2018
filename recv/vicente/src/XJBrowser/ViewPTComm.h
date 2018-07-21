// ***************************************************************
//  ViewPTComm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ����ͨѶ״̬��ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_)
#define AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPTComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B8201E4
class CViewPTComm : public CFormView
{
public:
	//##ModelId=49B87B8201F5
	CViewPTComm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPTComm)

// Form Data
public:
	//{{AFX_DATA(CViewPTComm)
	enum { IDD = IDD_XJ_VIEW_PTCOMM };
	CButton	m_cBoxShowPTCommON;
	CButton	m_cBoxShowPTcommOFF;
	CCompareCmb	m_cmbChooseStation;
	CStatic	m_staticChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bShowPTCommOFF;
	BOOL	m_bShowPTCommON;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B820209
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B820214
	CXJEventManager		m_EventManager; //�¼�������
	//##ModelId=49B87B820218
	int					m_nMaxRow;	//�����ʾ����
	//##ModelId=49B87B820219
	BOOL				m_bShowAcked;//�Ƿ���ʾ��ȷ���¼�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B82021A
	int		m_nOldSortCol;
	/** @brief           ��¼��ʱ��*/
	//##ModelId=49B87B820222
	int		m_nTimer;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B820224
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B820229
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B820233
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B820243
	IXJReport * pReportDoc;
// Operations
public:
	void InitComBox();
	CString m_sChooseStationID;
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	/*
	*  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
	*  @param 		[In]a param of Type  CString  �ؼ���
	*  @return 	CString �ؼ��ֱ�ʾ��ֵ
	*/
	//##ModelId=49B87B820252
	CString TranslateSingleKey(CString sKey);
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B820254
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B820271
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B820280
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B820281
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B82029F
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8202A0
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202AF
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8202BF
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);


/*
 *  @brief   	AddEventToList	 ��ָ�������豸��ͨ��״̬��Ϣ���뵽�б���� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8202CE
	BOOL AddEventToList(CSecObj* pSec, int i);

/*
 *  @brief   	IsNeedShow	 �жϵ�ǰͨ��״̬�ı�����Ϣ�Ƿ���Ҫ��ʾ 
 *  @param 		[In]a param of Type  int  ͨ��״̬
 *  @return 	BOOL ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 */
	//##ModelId=49B87B8202DE
	BOOL IsNeedShow(int nStatus);

/*
 *  @brief   	ChangeStatusInList	 �ı�ָ�������豸��ͨ��״̬��ͨ��״̬�б���е���ʾ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeStatusInList(CSecObj* pSec);

/*
 *  @brief   	RefreshDuravieTime	 ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ 
 *  @param 		[In]a param of Type  int  ָ����LIST�е�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 ˢ�����б�����ͨ��״̬�ĳ���ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
	void RefreshTime();

	/*
	*  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
	*  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8202F1
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 ɾ��COM��� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F2
	void ReleaseCOM();

	/*
	*  @brief   	WriteResultToExcel	�����������Excel 
	*  @param 		[In]a param of Type  CString  �ļ�·��
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F3
	BOOL WriteResultToExcel(CString strFullPath);

	/*
	*  @brief   	AddResultToExcel	 ��ָ���Ľ����д�뵽ָ����Excel�ļ��� 
	*  @param 		[In]a param of Type  CString  ���ݿ����
	*  @param 		[In]a param of Type  CSecObj*  �豸����
	*  @return 	void 
	*/
	//##ModelId=49B87B8202FE
	void AddResultToExcel(CDatabase* pDatabase, CSecObj* pSec, int index);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPTComm)
	public:
	virtual void OnInitialUpdate();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B82033C
	virtual ~CViewPTComm();
#ifdef _DEBUG
	//##ModelId=49B87B82035B
	virtual void AssertValid() const;
	//##ModelId=49B87B82036B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewPTComm)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventDevice();
	afx_msg void OnEventSec();
	afx_msg void OnClickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtcomm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPtcommOn();
	afx_msg void OnPtcommOff();
	afx_msg void OnPtcommHistory();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPtcommExport();
	afx_msg void OnPtcommPrint();
	afx_msg void OnPtcommRefresh();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowptcommon();
	afx_msg void OnCheckShowptcommoff();
	//}}AFX_MSG
	//##ModelId=49B87B830021
	void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPTCOMM_H__1E6444A8_3341_4FA1_B970_91C3F0F0C2FD__INCLUDED_)
