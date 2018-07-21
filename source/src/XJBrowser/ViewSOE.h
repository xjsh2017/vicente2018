#if !defined(AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_)
#define AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSOE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSOE form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewSOE : public CFormView
{
protected:
	CViewSOE();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSOE)

// Form Data
public:
	//{{AFX_DATA(CViewSOE)
	enum { IDD = IDD_XJ_VIEW_SOE };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ��ͼ���͡�0-I����Ϣ�� 1-II����Ϣ��2-III����Ϣ��3-������Ϣ*/
	int		m_nType;
	/** @brief           ��¼֮ǰ���������*/
	int		m_nOldSortCol;
private:
	CXJEventManager	m_EventManager;	//�¼�������
	int	m_nMaxRow;	//�������
	BOOL m_bShowAcked; //�Ƿ���ʾ��ȷ���¼�
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL m_bShowDebug;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL m_bShowReturn;

// Operations
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
 *  @brief   	AddEventToList	 ��ָ���¼����뵽�б�����ʾ 
 *  @param 		[In]a param of Type  CXJEvent *  ָ���¼�
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL �ɹ�����TRUE��ʧ�ܷ���FALSE
 */
	BOOL AddEventToList(CXJEvent * pEvent, int i = 0);

		/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8603C9
	void RegulateControlSize();

/*
 *  @brief   	LoadActionFromDB	 �����ݿ��ȡ�������� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadActionFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadAlarmFromDB	 �����ݿ��ȡ�澯���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8702DF
	BOOL LoadAlarmFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadDIFromDB	 �����ݿ��ȡ���������� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  BOOL  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDIFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  BOOL  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	void 
 */
	void LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEvent*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B87000F
	void RefreshAck(CXJEvent* pEvent, int nIndex = -1);

/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEvent*  ��ȷ�ϵ��¼�
 *	@param		[In]a param of Type	 BOOL �Ƿ񱻶�ȷ��
 *  @return 	void 
 */
	//##ModelId=49B87B87030D
	void TellOtherWndAck( CXJEvent* pEvent, BOOL bIsPassive = FALSE);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
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

	int RefreshData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSOE)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSOE();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSOE)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventFilterDebug();
	afx_msg void OnDblclkListSoe(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventFilterReturn();
	//}}AFX_MSG
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSOE_H__9AC5843F_75B6_4E83_926A_D0BECF20B6F7__INCLUDED_)
