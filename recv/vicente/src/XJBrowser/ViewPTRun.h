// ***************************************************************
//  ViewPTRun   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ��������״̬��ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_)
#define AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPTRun.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewPTRun form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B8102EE
class CViewPTRun : public CFormView
{
public:
	//##ModelId=49B87B8102F0
	CViewPTRun();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewPTRun)

// Form Data
public:
	//{{AFX_DATA(CViewPTRun)
	enum { IDD = IDD_XJ_VIEW_PTRUN };
	CButton	m_cboxShowPTRunCheck;
	CButton	m_cboxShowPTRunCode;
	CButton	m_cboxShowPTRunDebug;
	CButton	m_cboxShowPTRunOFF;
	CButton	m_cboxShowPTRunON;
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bShowPTRunCheck;
	BOOL	m_bShowPTRunCode;
	BOOL	m_bShowPTRunDebug;
	BOOL	m_bShowPTRunOFF;
	BOOL	m_bShowPTRunON;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B81030E
	CXJEventManager		m_EventManager;	//�¼�������
	//##ModelId=49B87B810312
	int					m_nMaxRow;	//�������
	//##ModelId=49B87B810313
	BOOL				m_bShowAcked; //�Ƿ���ʾ��ȷ���¼�
private:
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B810314
	int					m_nOldSortCol;
	/** @brief           ��¼��ʱ��*/
	//##ModelId=49B87B81031C
	int		m_nTimer;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B81031E
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B81032D
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B81033D
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B810342
	IXJReport * pReportDoc;

// Operations
public:
	CString m_sChooseStationID;

	void InitComBox();
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B81034B
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	/*
	*  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
	*  @param 		[In]a param of Type  CString  �ؼ���
	*  @return 	CString �ؼ��ֱ�ʾ��ֵ
	*/
	//##ModelId=49B87B81035B
	CString TranslateSingleKey(CString sKey);
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B81035D
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B81036B
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B81037A
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B81038A
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B810399
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8103A9
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8103B9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8103C8
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 ��ָ��װ�õ�����״̬���뵽�б�����ʾ 
 *  @param 		[In]a param of Type  CSecObj *  Ҫ�����װ��
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8103D8
	BOOL AddEventToList(CSecObj * pSec, int i);

/*
 *  @brief   	IsNeedShow	 �жϵ�ǰ����״̬�ı�����Ϣ�Ƿ���Ҫ��ʾ 
 *  @param 		[In]a param of Type  int  ����״̬
 *  @return 	BOOL ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 */
	//##ModelId=49B87B820002
	BOOL IsNeedShow(int nStatus);

/*
 *  @brief   	ChangeStatusInList	 �ı�ָ�������豸������״̬��ͨ��״̬�б���е���ʾ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	//##ModelId=49B87B82000F
	void ChangeStatusInList(CSecObj* pSec);

/*
 *  @brief   	RefreshDuravieTime	 ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ 
 *  @param 		[In]a param of Type  int  ָ����LIST�е�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B820011
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 ˢ�����б�����ͨ��״̬�ĳ���ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B820013
	void RefreshTime();

	/*
	*  @brief   	WriteResultToExcel	�����������Excel 
	*  @param 		[In]a param of Type  CString  �ļ�·��
	*  @return 	void 
	*/
	//##ModelId=49B87B82001F
	BOOL WriteResultToExcel(CString strFullPath);

	/*
	*  @brief   	AddResultToExcel	 ��ָ���Ľ����д�뵽ָ����Excel�ļ��� 
	*  @param 		[In]a param of Type  CString  ���ݿ����
	*  @param 		[In]a param of Type  CSecObj*  �豸����
	*  @return 	void 
	*/
	//##ModelId=49B87B820021
	void AddResultToExcel(CDatabase* pDatabase, CSecObj* pSec);

	/*
	*  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
	*  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B82002E
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 ɾ��COM��� 
	*  @return 	void 
	*/
	//##ModelId=49B87B82002F
	void ReleaseCOM();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPTRun)
	public:
	//##ModelId=49B87B820030
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B82003E
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B82005D
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B82007D
	virtual ~CViewPTRun();
#ifdef _DEBUG
	//##ModelId=49B87B82008C
	virtual void AssertValid() const;
	//##ModelId=49B87B82009C
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewPTRun)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventDevice();
	afx_msg void OnEventSec();
	afx_msg void OnClickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPtrunOn();
	afx_msg void OnPtrunOff();
	afx_msg void OnPtrunCheck();
	afx_msg void OnPtrunDebug();
	afx_msg void OnPtrunCode();
	afx_msg void OnPtrunHistory();
	afx_msg void OnPtrunPrint();
	afx_msg void OnPtrunExport();
	afx_msg void OnPtrunRefresh();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowptrunon();
	afx_msg void OnCheckShowptrunoff();
	afx_msg void OnCheckShowptruncheck();
	afx_msg void OnCheckShowptrundebug();
	afx_msg void OnCheckShowptruncode();
	//}}AFX_MSG
	//##ModelId=49B87B82016B
	void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPTRUN_H__594C8020_DDBC_45F1_92BB_5E5F00BC4F27__INCLUDED_)
