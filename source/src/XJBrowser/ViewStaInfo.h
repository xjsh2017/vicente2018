#if !defined(AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_)
#define AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_

#include "..\..\COMMON\define.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStaInfo.h : header file
//
#include "Event.h"
#include "EventManager.h"
/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewStaInfo : public CFormView
{
protected:
	CViewStaInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewStaInfo)

// Form Data
public:
	//{{AFX_DATA(CViewStaInfo)
	enum { IDD = IDD_XJ_VIEW_STAINFO };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B810303
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ҳ���Ӧ�ĳ�վ����ָ��*/
	CStationObj*	m_pStation;
private:

	
	/** @brief           ֮ǰ���������*/
	//##ModelId=49B87B850159
	int					m_nOldSortCol;
	

	/** @brief           ��¼��ʱ��*/
	int					m_nTimer;

	/** @brief           ��ӡģ���ʶ*/
	CLSID				clsid;
	/** @brief           COM�๤��ָ��*/
	LPCLASSFACTORY		pClf;
	/** @brief           COMָ��*/
	LPUNKNOWN			pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	IXJReport*			pReportDoc;
public:
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL				m_bShowDebug;
	
	int					m_nMaxRow;
	/** @brief           �Ƿ���ʾ��ȷ���¼�*/
	//##ModelId=49B87B850158
	BOOL				m_bShowAcked;
	/** @brief           �¼��������*/
	//##ModelId=49B87B850153
	CXJEventManager		m_EventManager;
// Operations
public:
	BOOL NeedSave();
	BOOL SaveListConfig();
	BOOL LoadProNameFromDB();
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
 *  @brief   	RefreshDuravieTime	 ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ 
 *  @param 		[In]a param of Type  int  ָ����LIST�е�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8202EE
	void RefreshDuravieTime(int nIndex);
/*
 *  @brief   	ChangeCommStatusInList	 �ı�ָ����վ��ͨ��״̬��ͨ��״̬�б���е���ʾ 
 *  @param 		[In]a param of Type  CSecObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87B8202E0
	void ChangeStationStatusInList(CStationObj* pObj);
/*
 *  @brief   	RefreshTime	 ˢ�����б�����ͨ��״̬������״̬�ĳ���ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B8202F0
		void RefreshTime();
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
 *  @brief   	AddStationInfoToList	 ��һ����վ����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CSecObj *  Ҫ�����װ��
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8501B7
	BOOL AddStationInfoToList(CStationObj* pObj,CDeviceObj* pDev, int i);
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8501A5
	void RegulateControlSize();

		/*
	*  @brief   	WriteResultToExcel	�����������Excel 
	*  @param 		[In]a param of Type  CString  �ļ�·��
	*  @return 	void 
	*/
	BOOL WriteResultToExcel(CString strFullPath);
		/*
	*  @brief   	AddResultToExcel	 ��ָ���Ľ����д�뵽ָ����Excel�ļ��� 
	*  @param 		[In]a param of Type  CString  ���ݿ����
	*  @param 		[In]a param of Type  CSecObj*  �豸����
	*  @return 	void 
	*/
	//##ModelId=49B87B820021
	void AddResultToExcel(CDatabase* pDatabase, CStationObj* pObj,CDeviceObj* pDev);
// Operations


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewStaInfo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewStaInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CViewStaInfo)
	afx_msg void OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCommOn();
	afx_msg void OnCommOff();
	afx_msg void OnStationHistory();
	afx_msg void OnCommExport();
	afx_msg void OnCommPrint();
	afx_msg void OnStationTree();
	afx_msg void OnStationMap();	
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnStaInfoOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTAINFO_H__1A0B3CF0_BED7_4370_B7D0_8F6C9C4B8161__INCLUDED_)
