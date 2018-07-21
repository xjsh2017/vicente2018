// ***************************************************************
//  ViewAction   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����¼��б���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_)
#define AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAction.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewAction form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "ListCtrlEx.h"
#include "EventManager.h"

//##ModelId=49B87B870242
class CViewAction : public CFormView
{
protected:
	CViewAction();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAction)

// Form Data
public:
	//{{AFX_DATA(CViewAction)
	enum { IDD = IDD_XJ_VIEW_ACTION };
	CButton	m_cboxShowReturn;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CStatic	m_StateChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowReturn;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87B870267
	CArray<SColumn, SColumn&> m_arrColum;
	BOOL m_bQuitThread;
private:
	/** @brief           �¼������ʾ����*/
	//##ModelId=49B87B87026B
	int		m_nMaxRow;
	/** @brief           �¼�����*/
	//##ModelId=49B87B87026D
	CXJEventManager	m_EventManager;
	/** @brief           �Ƿ���ʾ��ȷ���¼�*/
	//##ModelId=49B87B870271
	BOOL	m_bShowAcked;
	/** @brief           ��¼֮ǰ��������� */
	//##ModelId=49B87B870272
	int		m_nOldSortCol;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL	m_bShowDebug;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL	m_bShowReturn;
	/** @brief           ѡ���ĳ�վ��ID*/
	CString m_sChooseStationID;
	
	CWinThread*	m_pLoadDataThread;

	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
public:
	/*
 *  @brief   	InitComBox	 ����combox��Ϣ
 *  @param 		
 *  @param 		
 *  @return 	
 */
	void InitComBox();		
	
	//##ModelId=49B87B870273
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadDataFromDB();

	void RefreshData();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B87029F
	int FillListData();

	void NotifyThreadEnd();
private:
/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B870280
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitListStyle();


/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8702AF
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8702BF
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87B8702CE
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8702DE
	void RegulateControlSize();

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAction *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8702EF
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ��Ҫ�ĺ�������˵������ 
 *  @param 		[In]a param of Type  CXJEventAction*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8702FE
	void RefreshAck(CXJEventAction* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventAction*  ��ȷ�ϵ��¼�
 *	@param		[In]a param of Type	 BOOL �Ƿ񱻶�ȷ��
 *  @return 	void 
 */
	//##ModelId=49B87B87030D
	void TellOtherWndAck( CXJEventAction* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	FindLocalPoint	 ����ĳ���¼��ڱ��صĵ�ַ 
 *  @param 		[In]a param of Type  CXJEventAction*  Ҫ���ҵ��¼�
 *  @return 	CXJEventAction* ���ص�ַ
 */
	//##ModelId=49B87B870310
	CXJEventAction* FindLocalPoint(CXJEventAction* pEvent);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

/*
 *  @brief   	BuildOrSQL	 �齨I,II,II����Ϣ�Ĺ��˲�ѯ��� 
 *  @return 	CString ��ѯ���
 */
	//##ModelId=49B87B87031C
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

	void StartThead();
	void EndThread();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAction)
	public:
	//##ModelId=49B87B87031D
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B87032C
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B87033C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B87035B
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B87037A
	virtual ~CViewAction();
#ifdef _DEBUG
	//##ModelId=49B87B87038A
	virtual void AssertValid() const;
	//##ModelId=49B87B870399
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAction)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListAction(NMHDR* pNMHDR, LRESULT* pResult);
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
	afx_msg void OnColumnclickListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSign();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterReturn();
	afx_msg void OnExportExcel();
	afx_msg void OnSelchangeCOMBOChooseStation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnCheckShowreturn();
	//}}AFX_MSG
	//##ModelId=49B87B88005E
	void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B880061
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B88006F
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B880072
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B88007E
	afx_msg void OnRefreshActionView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_VIEWACTION_H__48B1249A_F5F1_4BAC_8BD4_545B5A724BA0__INCLUDED_)
