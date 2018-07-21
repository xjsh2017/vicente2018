#if !defined(AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_)
#define AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDistance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDistance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewDistance : public CFormView
{
protected:
	CViewDistance();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDistance)

// Form Data
public:
	//{{AFX_DATA(CViewDistance)
	enum { IDD = IDD_XJ_VIEW_DISTANCE };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief			�¼�������*/
	CXJEventManager		m_EventManager;
	/** @brief           �����ʾ����*/
	int					m_nMaxRow;
	/** @brief           �Ƿ���ʾ��ȷ���¼�*/
	BOOL				m_bShowAcked;
	int					m_nOldSortCol;

/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B85015A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
// Operations
private:
	/*
	* @brief	InitListCtrl ��ʼ���б��	
	* @return   int	ʧ�ܷ���<=0, �ɹ�����>0
	*/
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���<=0, �ɹ�����>0
	*/
	int	InitListStyle();

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
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	void RegulateControlSize();

	/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

	/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventDistance *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	BOOL AddEventToList(CXJEventDistance * pEvent, int i);

	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���<=0, �ɹ�����>0
	*/
	int FillListData();

	/*
	* @brief	RefreshData	ˢ�����ݣ�����������ݡ������ݿ���롢��䵽�б��
	* @return   int	ʧ�ܷ���<=0, �ɹ�����>0
	*/
	int RefreshData();

	/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventDistance*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	void RefreshAck(CXJEventDistance* pEvent, int nIndex = -1);

	/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventDistance*  ��ȷ�ϵ��¼�
 *  @return 	void 
 */
	void TellOtherWndAck( CXJEventDistance* pEvent );

	/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	void CheckLineCount();
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

	CXJEventDistance* FindLocalPoint(CXJEventDistance* pEvent);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDistance)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewDistance();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDistance)
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventProp();
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventHide();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDistance(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListDistance(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	void OnEventPropAckPT(WPARAM wParma, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDISTANCE_H__4953F47A_2F1C_42C8_8A53_98929FD6AE01__INCLUDED_)
