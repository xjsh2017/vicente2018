// ***************************************************************
//  ViewDI   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ��������λ�¼��б���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_)
#define AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDI.h : header file
//
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDI form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B850128
class CViewDI : public CFormView
{
public:
	//##ModelId=49B87B850139
	CViewDI();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDI)

// Form Data
public:
	//{{AFX_DATA(CViewDI)
	enum { IDD = IDD_XJ_VIEW_DI };
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CButton	m_cmbShowReady;
	CButton	m_cmbShowDebug;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B85014E
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ

// Operations
public:
	/** @brief           �¼��������*/
	//##ModelId=49B87B850153
	CXJEventManager		m_EventManager;
	/** @brief           �����ʾ����*/
	//##ModelId=49B87B850157
	int					m_nMaxRow;
	/** @brief           �Ƿ���ʾ��ȷ���¼�*/
	//##ModelId=49B87B850158
	BOOL				m_bShowAcked;
	/** @brief           ֮ǰ���������*/
	//##ModelId=49B87B850159
	int		m_nOldSortCol;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL	m_bShowDebug;

public:
	/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
	void InitComBox();
	/** @brief           ��վID*/
	CString m_sChooseStationID;

/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B85015A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B850169
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B850177
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B850178
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B850186
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B850196
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B850197
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8501A5
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8501A6
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8501B7
	BOOL AddEventToList(CXJEventDI * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventDI*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8501C6
	void RefreshAck(CXJEventDI* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventDI*  ��ȷ�ϵ��¼�
 *  @return 	void 
 */
	//##ModelId=49B87B8501C9
	void TellOtherWndAck( CXJEventDI* pEvent );

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8501D5
	void CheckLineCount();

/*
 *  @brief   	DisplayValue	 �õ���������ֵ��ʾ�ַ��� 
 *  @param 		[In]a param of Type  int  ������ֵ
 *  @return 	CString ��ʾֵ
 */
	//##ModelId=49B87B8501D6
	CString DisplayValue(int nValue);

/*
 *  @brief   	LoadConfig	 ��ȡ���� 
 *  @return 	void 
 */
	void LoadConfig();

/*
 *  @brief   	SaveConfig	 �������� 
 *  @return 	void 
 */
	void SaveConfig();
	
	void AddSignToEvent(CXJEventDI* pEvent);

	int RefreshData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDI)
	public:
	//##ModelId=49B87B8501D8
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8501E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8501F4
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B850213
	virtual ~CViewDI();
#ifdef _DEBUG
	//##ModelId=49B87B850222
	virtual void AssertValid() const;
	//##ModelId=49B87B850224
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDI)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnClickListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventSign();
	afx_msg void OnSelchangeComboChoosestatioin();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	//}}AFX_MSG
	//##ModelId=49B87B850296
	void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8502A0
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDI_H__DBF7D3BC_9B31_4120_8C2D_AD54DFC1F603__INCLUDED_)
