// ***************************************************************
//  ViewAlarm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �澯�¼��б���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_)
#define AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAlarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "ViewBase.h"
#include "ListCtrlEx.h"
#include "EventManager.h"
#include "Event.h"

//##ModelId=49B87B86033C
class CViewAlarm : public CFormView
{
protected:
	//##ModelId=49B87B86033E
	CViewAlarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAlarm)

// Form Data
public:
	//{{AFX_DATA(CViewAlarm)
	enum { IDD = IDD_XJ_VIEW_ALARM };
	CStatic	m_staticChooseStation;
	CButton	m_cboxShowReturn;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowReturn;
	BOOL	m_bCBoxShowDebug;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B860351
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
private:
	//##ModelId=49B87B86035C
	CXJEventManager	m_EventManager;	//�¼�������
	//##ModelId=49B87B860360
	int	m_nMaxRow;	//�������
	//##ModelId=49B87B860361
	BOOL m_bShowAcked; //�Ƿ���ʾ��ȷ���¼�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B86036B
	int m_nOldSortCol;
	/** @brief           �Ƿ���ʾ�����¼�*/
	BOOL m_bShowDebug;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL	m_bShowReturn;
	/** @brief           ѡ���ĳ�վ��ID*/
	CString m_sChooseStationID;
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B86036C
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
		/*
 *  @brief   	InitComBox	 ����combox��Ϣ
 *  @param 		
 *  @param 		
 *  @return 	
 */
	void InitComBox();	
private:
/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B86037A
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B86038A
	int	InitListStyle();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B860399
	int FillListData();

/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8603A9
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8603B9
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87B8603C8
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8603C9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8603D8
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B870000
	BOOL AddEventToList(CXJEventAlarm * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B87000F
	void RefreshAck(CXJEventAlarm* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventAlarm*  ��ȷ�ϵ��¼�
 *	@param		[In]a param of Type  BOOL �Ƿ񱻶�ȷ��
 *  @return 	void 
 */
	//##ModelId=49B87B870012
	void TellOtherWndAck( CXJEventAlarm* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	FindLocalPoint	 ����ĳ���¼��ڱ��صĵ�ַ 
 *  @param 		[In]a param of Type  CXJEventAlarm*  Ҫ���ҵ��¼�
 *  @return 	CXJEventAlarm* ���ص�ַ
 */
	//##ModelId=49B87B87001F
	CXJEventAlarm* FindLocalPoint(CXJEventAlarm* pEvent);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B870021
	void CheckLineCount();

/*
 *  @brief   	BuildOrSQL	 �齨I,II,II����Ϣ�Ĺ��˲�ѯ��� 
 *  @return 	CString ��ѯ���
 */
	//##ModelId=49B87B870022
	CString BuildOrSQL();

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


	void AddSignToEvent(CXJEventAlarm* pEvent);


	void RefreshData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAlarm)
	public:
	//##ModelId=49B87B870023
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B87003E
	virtual void OnInitialUpdate();
	//##ModelId=49B87B87004E
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	//##ModelId=49B87B87006D
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B87007D
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//##ModelId=49B87B87009C
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8700AB
	virtual ~CViewAlarm();
#ifdef _DEBUG
	//##ModelId=49B87B8700BB
	virtual void AssertValid() const;
	//##ModelId=49B87B8700CB
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAlarm)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSign();
	afx_msg void OnDblclkListAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterReturn();
	afx_msg void OnSelchangeComboVachstation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnCheckShowreturn();
	//}}AFX_MSG
	//##ModelId=49B87B87016E
	void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B870178
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B87017B
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B870188
	void OnEventPropAckPT(WPARAM wParma, LPARAM lParam);
	//##ModelId=49B87B87018B
	void OnRefreshAlarmView(WPARAM wParam, LPARAM lParam);
	afx_msg OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWALARM_H__750201A3_3FAF_4DEA_8FDC_DD0F17962A8B__INCLUDED_)
