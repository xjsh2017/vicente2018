#if !defined(AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_)
#define AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownLoadView.h : header file
//
#include "CommonFile.h"

/** @brief           ͨ���ļ�����*/
//##ModelId=49B87BA300BD
typedef CTypedPtrList<CObList , CCommonFile*>	COMMONFILE_LIST;
/////////////////////////////////////////////////////////////////////////////
// CDownLoadView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BA30109
class CDownLoadView : public CViewBase
{
protected:
	//##ModelId=49B87BA3010B
	CDownLoadView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownLoadView)

// Form Data
public:
	//{{AFX_DATA(CDownLoadView)
	enum { IDD = IDD_DOWNLOAD_FILE };
	CCompareCmb	m_cmbStation;
	CButton	m_btnFileClose;
	CPictureEx	m_gif;
	CButton	m_btnDown;
	CButton	m_btnCall;
	CListCtrlEx	m_List;
	CString	m_strCondition;
	//}}AFX_DATA

// Attributes
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BA3013A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
	//##ModelId=49B87BA30140
	CThemeHelperST	m_ThemeHelper;
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87BA3014C
	CArray<SColumn, SColumn&> m_arrColum;

	/** @brief           ͨ���ļ�����*/
	//##ModelId=49B87BA30158
	COMMONFILE_LIST m_FileList;

	/** @brief           ��¼��ǰ�ٻ��ĳ�վID, Ϊ��ʱ��ʾ���г�վ*/
	//##ModelId=49B87BA3015C
	CString m_sStationID;
	/** @brief           �������� 1-���ļ�����,2-ָ��ʱ����ڵ��ļ�����*/
	//##ModelId=49B87BA30167
	int		m_nConditionType;
	/** @brief           ��ǰ״̬. 0-����. 1-�����ٻ�¼���б�. 2-��������¼���ļ�*/
	//##ModelId=49B87BA30168
	int		m_nCurrentStatus;

	/** @brief           ��¼��ʱ��*/
	//##ModelId=49B87BA30177
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	/** @brief           �������ļ��б�*/
	//##ModelId=49B87BA30179
	COMMONFILE_LIST	m_FileDownload;

	/** @brief           ����ַ���*/
	//##ModelId=49B87BA30186
	CString	m_CallFileResult;

	/** @brief           �Ƿ�ʹ�öϵ�����*/
	//##ModelId=49B87BA30187
	BOOL	m_bUseBreakPoint;

	/** @brief           �������*/
	//##ModelId=49B87BA30196
	int m_nNum;
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87BA30197
	int		m_nOldSortCol;

// Operations
public:

private:
/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA301A5
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA301B5
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87BA301C5
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BA301D4
	int	InitListStyle();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87BA301D5
	int InitControls();

/*
 *  @brief   	FillStation	 ��䳧վ������ 
 *  @return 	void 
 */
	//##ModelId=49B87BA301E4
	void FillStation();

/*
 *  @brief   	ClearFileList	 �������ͨ���ļ���¼ 
 *  @return 	void 
 */
	//##ModelId=49B87BA301E5
	void ClearFileList();

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87BA301F4
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87BA301F5
	void CancelCall();

/*
 *  @brief   	FillListData	 ����б������ 
 *  @return 	void 
 */
	//##ModelId=49B87BA30203
	void FillListData();

/*
 *  @brief   	AddFileToList	 �����ļ������б������ʾ 
 *  @param 		[In]a param of Type  CCommonFile*  �ļ�����
 *  @param 		[In]a param of Type  int  �б������
 *  @return 	void 
 */
	//##ModelId=49B87BA30204
	void AddFileToList(CCommonFile* pFile, int nIndex = 0);

/*
 *  @brief   	ChangeValue	 �ı��ļ��б��Ƿ������ص���ʾ
 *  @param 		[In]a param of Type  CCommonFile*  ָ���б�
 *  @return 	void 
 */
	//##ModelId=49B87BA30213
	void ChangeValue(CCommonFile* pFile);

/*
 *  @brief   	DownloadCommonFile	 ����ͨ���ļ� 
 *  @param 		[In]a param of Type  CCommonFile*  ָ���б�
 *  @return 	void 
 */
	//##ModelId=49B87BA30215
	void DownloadCommonFile(CCommonFile* pFile);

/*
 *  @brief   	CheckDownload	 �˶�Ҫ���ص��ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87BA30222
	void CheckDownload();

/*
 *  @brief   	ChangeDownloadedValue	 �����������ص��ļ������������ݴ�С 
 *  @param 		[In]a param of Type  CCommonFile*  �������ص��ļ�
 *  @param 		[In]a param of Type  CString  �������ļ���С
 *  @return 	void 
 */
	void ChangeDownloadedValue(CCommonFile* pFile, CString sValue);

	void OpenFileDir(CCommonFile* pFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BA30251
	virtual ~CDownLoadView();
#ifdef _DEBUG
	//##ModelId=49B87BA30261
	virtual void AssertValid() const;
	//##ModelId=49B87BA30271
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDownLoadView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCmbDownCondition();
	afx_msg void OnBtnDownloadCall();
	afx_msg void OnBtnDownloadDown();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnColumnclickListDown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnFileClose();
	afx_msg void OnDoubleclickedBtnDownloadCall();
	afx_msg void OnBtnDownloadOpen();
	afx_msg void OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87BA302CF
	afx_msg void OnDownloadOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302DE
	afx_msg void OnSTTP00204(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302EF
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA302FD
	afx_msg void DownloadFileEnd(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30301
	afx_msg void OnSTTP00212(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA3030E
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BA30312
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADVIEW_H__DF1CCB39_D219_4493_ACE4_23B9EC12E13B__INCLUDED_)
