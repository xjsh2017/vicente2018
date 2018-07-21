#if !defined(AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_)
#define AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStationInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewStationInfo : public CFormView
{
protected:
	CViewStationInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewStationInfo)

// Form Data
public:
	//{{AFX_DATA(CViewStationInfo)
	enum { IDD = IDD_XJ_VIEW_STATIONINFO };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ҳ���Ӧ�ĳ�վ����ָ��*/
	CStationObj*	m_pStation;
private:
	/** @brief           ��¼֮ǰ���������*/
	int		m_nOldSortCol;
	/** @brief           ��¼��ʱ��*/
	int		m_nTimer;

	/** @brief           ��ӡģ���ʶ*/
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	IXJReport * pReportDoc;

// Operations
public:
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
 *  @brief   	AddEventToList	 ��ָ��װ�õ�����״̬���뵽�б�����ʾ 
 *  @param 		[In]a param of Type  CSecObj *  Ҫ�����װ��
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8103D8
	BOOL AddEventToList(CSecObj * pSec, int i);

/*
 *  @brief   	ChangeCommStatusInList	 �ı�ָ�������豸��ͨ��״̬��ͨ��״̬�б���е���ʾ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeCommStatusInList(CSecObj* pSec);

/*
 *  @brief   	ChangeRunStatusInList	 �ı�ָ�������豸������״̬��ͨ��״̬�б���е���ʾ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	void ChangeRunStatusInList(CSecObj* pSec);

	/*
	*  @brief   	WriteResultToExcel	�����������Excel 
	*  @param 		[In]a param of Type  CString  �ļ�·��
	*  @return 	�����ɹ�����TRUE, ʧ�ܷ���FALSE
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
	//##ModelId=49B87B8202F1
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 ɾ��COM��� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8202F2
	void ReleaseCOM();

/*
 *  @brief   	RefreshDuravieTime	 ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ 
 *  @param 		[In]a param of Type  int  ָ����LIST�е�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);

/*
 *  @brief   	RefreshTime	 ˢ�����б�����ͨ��״̬������״̬�ĳ���ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
	void RefreshTime();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewStationInfo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewStationInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewStationInfo)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStationinfoSec();
	afx_msg void OnStationinfoCommH();
	afx_msg void OnStationinfoRunH();
	afx_msg void OnStationinfoExclel();
	afx_msg void OnStationinfoPrint();
	afx_msg void OnStationinfoRefresh();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnStationInfoOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATIONINFO_H__102F86D2_186F_476C_B2A9_0FC881F088F8__INCLUDED_)
