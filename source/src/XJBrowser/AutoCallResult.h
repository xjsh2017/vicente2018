#if !defined(AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_)
#define AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCallResult.h : header file
//
#include <afxdb.h>
#include "DlgExportProcess.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BB303C8
class CCallResult : public CObject
{
public:
	//##ModelId=49B87BB303CA
	CCallResult();
	//##ModelId=49B87BB40000
	~CCallResult();
public:
	//0:δ�ٻ� 1���ɹ� 2��ʧ�� 3: װ�ò�֧��
	/** @brief           ��ֵ���Ž��*/
	//##ModelId=49B87BB4001F
	int m_nZone;
	/** @brief           ��ֵ���*/
	//##ModelId=49B87BB40020
	int m_nSetting;
	/** @brief           ���������*/
	//##ModelId=49B87BB4002E
	int m_nDigital;
	/** @brief           ��ѹ����*/
	//##ModelId=49B87BB4002F
	int m_nSoftBoard;
	/** @brief           ģ�������*/
	//##ModelId=49B87BB40030
	int m_nAnalog;
	int	m_nOsc; //¼�����
	int m_nHistory; //��ʷ�¼����

	/** @brief           ��ֵ���Ž��˵��*/
	//##ModelId=49B87BB40031
	CString m_sZoneNote;
	/** @brief           ��ֵ���˵��*/
	//##ModelId=49B87BB40032
	CString m_sSettingNote;
	/** @brief           ���������˵��*/
	//##ModelId=49B87BB4003E
	CString m_sDigitalNote;
	/** @brief           ��ѹ����˵��*/
	//##ModelId=49B87BB4003F
	CString m_sSoftBoardNote;
	/** @brief           ģ�������˵��*/
	//##ModelId=49B87BB40040
	CString m_sAnalogNote;
	CString	m_sOscNote; //¼�����˵��
	CString m_sHistoryNote;//��ʷ�¼����˵��

	CString		m_sSituation; //���˵�����
	CString		m_sDispose; //����������

	/** @brief           ��������*/
	//##ModelId=49B87BB40041
	int m_nSaveNo;

	/** @brief           �����豸ID*/
	//##ModelId=49B87BB40042
	CString		m_sSecID;
	/** @brief           �����豸ָ��*/
	//##ModelId=49B87BB4004F
	CSecObj*	m_pSec;
	/** @brief           CPU��*/
	//##ModelId=49B87BB40053
	int			m_nCPU;
	/** @brief           ���ʱ��*/
	//##ModelId=49B87BB4005D
	CTime		m_tmEnd;
	/** @brief           ��վID*/
	//##ModelId=49B87BB4005E
	CString		m_sStationId;
	/** @brief           ��վָ��*/
	//##ModelId=49B87BB40060
	CStationObj*	m_pStation;

public:
/*
 *  @brief   	LoadDataFromDB	 �����ݿ����������Ϣ 
 *  @param 		[In]a param of Type  CMemSet*  ���ݿ����
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB4006D
	BOOL LoadDataFromDB(CMemSet* pMemset);

private:
/*
 *  @brief   	BuildReleation	 �������豸�Ĺ��� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB4008C
	BOOL BuildReleation();
};

/** @brief           �ٻ��������*/
//##ModelId=49B87BB400AB
typedef CTypedPtrList<CObList , CCallResult*>	CALLRESULT_LIST;

//##ModelId=49B87BB400BC
typedef struct Result_Stat
{
	int nSuccess;
	int nFail;
}Result_Stat;

//##ModelId=49B87BB400BE
typedef struct SaveNumber
{
	int		nSaveNo; //���κ�
	CTime	tmTime;	 //ʱ��
}SaveNumber;

//##ModelId=49B87BB40148
class CAutoCallResult : public CFormView
{
protected:
	//##ModelId=49B87BB40158
	CAutoCallResult();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAutoCallResult)

// Form Data
public:
	//{{AFX_DATA(CAutoCallResult)
	enum { IDD = IDD_AUTOCALL_RESULT };
	/** @brief           ��������б��*/
	//##ModelId=49B87BB40178
	CListCtrlEx	m_ListSummary;
	/** @brief           ��ϸ����б��*/
	//##ModelId=49B87BB4017D
	CListCtrlEx	m_List;
	//}}AFX_DATA

	//##ModelId=49B87BB40182
	CLSID clsid;
	//##ModelId=49B87BB40187
	LPCLASSFACTORY pClf;
	//##ModelId=49B87BB4018C
	LPUNKNOWN pUnk;
	//##ModelId=49B87BB40191
	IXJReport * pReportDoc;

	CDlgExportProcess* m_pExportProcess;

// Attributes
private:
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87BB40196
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           �������*/
	//##ModelId=49B87BB4019B
	CALLRESULT_LIST	m_listResult;

	/** @brief           ��ֵ���Ž��ͳ��*/
	//##ModelId=49B87BB401A0
	Result_Stat	m_ZoneStat;
	/** @brief           ��ֵ���ͳ��*/
	//##ModelId=49B87BB401A6
	Result_Stat	m_SettingStat;
	/** @brief           ���������ͳ��*/
	//##ModelId=49B87BB401AB
	Result_Stat m_DigitalStat;
	/** @brief           ��ѹ����ͳ��*/
	//##ModelId=49B87BB401B0
	Result_Stat m_SoftBoardStat;
	/** @brief           ģ�������ͳ��*/
	//##ModelId=49B87BB401B6
	Result_Stat m_AnalogStat;
	Result_Stat	m_OscStat;//¼�����ͳ��
	Result_Stat	m_nHistoryStat;//��ʷ�¼�ͳ��

	/** @brief           ���ٿ�ʼʱ��*/
	//##ModelId=49B87BB401BA
	CTime		m_tmStart;
	/** @brief           ���ٽ���ʱ��*/
	//##ModelId=49B87BB401C5
	CTime		m_tmEnd;

	/** @brief           ��¼֮ǰ��������� */
	//##ModelId=49B87BB401C6
	int		m_nOldSortCol;

	/** @brief           ��¼��ǰѡ��ĳ�վ*/
	//##ModelId=49B87BB401D5
	CStationObj* m_pStation;

	/** @brief           ��¼���ٵ����κ�*/
	//##ModelId=49B87BB401E5
	CPtrArray	m_arrSaveNo;

	/** @brief           ��¼���µ����κ�*/
	//##ModelId=49B87BB401E9
	int			m_nNewSaveNo;

	/** @brief           ��¼�������κ�*/
	//##ModelId=49B87BB401F4
	int			m_nOldSaveNo;

	/** @brief           ��¼������κ�*/
	//##ModelId=49B87BB40203
	int			m_nMaxSaveNo;

	/** @brief           ��¼������κ�*/
	//##ModelId=49B87BB40204
	int			m_nMinSaveNo;

	/** @brief           ��ǰѡ������κ�*/
	//##ModelId=49B87BB40213
	int			m_nSelSaveNo;

	/** @brief           �Ƿ���ʾ��ʧ����Ŀ*/
	BOOL		m_bShowNoFailed;

	BOOL		m_bSelectRoot;
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BB40214
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB40232
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB40242
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87BB40261
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 		void 
	*/
	//##ModelId=49B87BB40271
	void RegulateControlSize();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BB40280
	int	InitListStyle();

/*
 *  @brief   	InitSummaryListStyle	 ��ʼ�������б��ķ�� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BB40290
	int InitSummaryListStyle();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ�������� 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB4029F
	BOOL LoadDataFromDB(int nSaveNo);

/*
 *  @brief   	Clear	 ������� 
 *  @return 	void 
 */
	//##ModelId=49B87BB402BF
	void Clear();

/*
 *  @brief   	StatResult	 ��ָ��������뵽���ͳ���� 
 *  @param 		[In]a param of Type  CCallResult*  ָ�����
 *  @return 	void 
 */
	//##ModelId=49B87BB402C0
	void StatResult(CCallResult* pResult);

/*
 *  @brief   	FillListData	 �������ٻ�������뵽�б������ʾ 
 *  @return 	void 
 */
	//##ModelId=49B87BB402DE
	void FillListData();

/*
 *  @brief   	AddResultToList	 ��ָ��������뵽�б�����ʾ 
 *  @param 		[In]a param of Type  CCallResult*  ָ�����
 *	@param		[In]a param of Type int	Ҫ�����λ��
 *  @return 	void 
 */
	//##ModelId=49B87BB402DF
	void AddResultToList(CCallResult* pResult, int nIndex = 0);

/*
 *  @brief   	GetResultString	 ȡ�ý���ַ��� 
 *  @param 		[In]a param of Type  int  �����־
 *  @return 	CString ����ַ���
 */
	//##ModelId=49B87BB402EF
	CString GetResultString(int nResult);

/*
 *  @brief   	GetCPU	 ȡ��ָ�������豸��ָ��CPU�ŵ�CPU���� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @param 		[In]a param of Type  int  ָ��CPU��
 *  @return 	CSecCPU* �ҵ�����CPU�����ָ��, δ�ҵ�����NULL
 */
	//##ModelId=49B87BB402FE
	CSecCPU* GetCPU(CSecObj* pObj, int nCpu);

/*
 *  @brief   	FillSummaryListData	 �������б������ 
 *  @return 	void 
 */
	//##ModelId=49B87BB4031C
	void FillSummaryListData();

/*
 *  @brief   	GetOCTime	 �����ݿ��ѯ�Զ����ٵ�ʱ�� 
 *  @param 		[In]a param of Type  CString  ��Ӧtb_sys_config�е�KEY
 *  @param 		[Out]a param of Type  CTime&  ʱ��
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB4031D
	BOOL GetOCTime(CString sKey, CTime& tmTime);

/*
 *  @brief   	ReFillAll	 ���������������� 
 *  @return 	void 
 */
	//##ModelId=49B87BB4032D
	void ReFillAll(int nSaveNo);

/*
 *  @brief   	ClearStat	 ���ͳ�� 
 *  @return 	void 
 */
	//##ModelId=49B87BB4033C
	void ClearStat();

/*
 *  @brief   	AddResultToExcel	 ��ָ���Ľ����д�뵽ָ����Excel�ļ��� 
 *  @param 		[In]a param of Type  CString  ���ݿ����
 *  @param 		[In]a param of Type  CCallResult*  �����
 *  @return 	void 
 */
	//##ModelId=49B87BB4033D
	void AddResultToExcel(CDatabase* pDatabase, CCallResult* pResult);

/*
 *  @brief   	WriteResultToExcel	�����������Excel 
 *  @param 		[In]a param of Type  CString  �ļ�·��
 *  @return 	void 
 */
	//##ModelId=49B87BB4034C
	BOOL WriteResultToExcel(CString strFullPath);

	

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB4035B
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87BB4035C
	void ReleaseCOM();

/*
 *  @brief   	QuerySaveNo	 ��ѯָ��վ����������� 
 *  @param 		[In]a param of Type  CString  վID
 *  @return 	void 
 */
	//##ModelId=49B87BB4035D
	void QuerySaveNo(CString sStationID);

/*
 *  @brief   	QueryNewSaveNo	 ��ѯָ��վ������µ��������κ� 
 *  @param 		[In]a param of Type  CString  վID
 *  @return 	void 
 */
	//##ModelId=49B87BB4035F
	void QueryNewSaveNo(CString sStationID);

/*
 *  @brief   	GetPrevSaveNo	 ȡ����һ���ε����κ� 
 *  @param 		[In]a param of Type  int  ��ǰ���κ�
 *  @return 	int ��һ�������κ�
 */
	//##ModelId=49B87BB4036C
	int	GetPrevSaveNo(int nCurSaveNo);

/*
 *  @brief   	GetNextSaveNo	 ȡ����һ���ε����κ� 
 *  @param 		[In]a param of Type  int  ��ǰ���κ�
 *  @return 	int ��һ�������κ�
 */
	//##ModelId=49B87BB4036E
	int GetNextSaveNo(int nCurSaveNo);

/*
 *  @brief   	ClearSaveNo	 ������μ�¼ 
 *  @return 	void 
 */
	//##ModelId=49B87BB4037A
	void ClearSaveNo();

/*
 *  @brief   	SortSaveNo	 �����ΰ�ʱ��Ӿɵ��½������� 
 *  @return 	void 
 */
	//##ModelId=49B87BB4037B
	void SortSaveNo();

/*
 *  @brief   	HasFaildItem	 �ж�һ���Զ����ٵĽ�����Ƿ���ʧ����Ŀ 
 *  @param 		[In]a param of Type  CCallResult*  ָ���Զ����ٽ�������
 *  @return 	BOOL ��ʧ����Ŀ����TRUE, ��ʧ����Ŀ����FALSE
 */
	BOOL HasFaildItem(CCallResult* pResult);

public:
	/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87BB4037C
	CString TranslateSingleKey(CString sKey);

	BOOL WriteResultToExcel( CString strFullPath, CString sStationID );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCallResult)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB403C8
	virtual ~CAutoCallResult();
#ifdef _DEBUG
	//##ModelId=49B87BB50000
	virtual void AssertValid() const;
	//##ModelId=49B87BB5000F
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAutoCallResult)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListAutocallResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnResultExport();
	afx_msg void OnResultPrint();
	afx_msg void OnResultPrev();
	afx_msg void OnResultNext();
	afx_msg void OnResultShowfaild();
	afx_msg void OnWinClose();
	afx_msg void OnResultStationexport();
	//}}AFX_MSG
	//##ModelId=49B87BB5009E
	afx_msg void OnAutoCallResultFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB500AB
	afx_msg void OnAutoCallResultChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCALLRESULT_H__5DF70470_15E3_4B3D_8397_33A2CA0B81CA__INCLUDED_)
