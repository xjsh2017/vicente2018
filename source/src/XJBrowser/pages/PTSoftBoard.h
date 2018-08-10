#if !defined(AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_)
#define AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTSoftBoard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ViewBase.h"
#include "HisValueMgr.h"

//##ModelId=49B87B89037A
class CPTSoftBoard : public CViewBase
{
protected:
	//##ModelId=49B87B89037C
	CPTSoftBoard();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSoftBoard)

// Form Data
public:
	//{{AFX_DATA(CPTSoftBoard)
	enum { IDD = IDD_PT_SOFTBOARD };
	CButtonST	m_btnVerify2;
	CButtonST	m_btnVerify1;
	CPictureEx			m_gif;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnViewProg;
	CButton	m_btnViewHis;
	CButton	m_btnModify;
	CButton	m_btnCall;
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B8903C9
	CSecObj *	m_pObj; //��Ӧ�ı���ָ��
	//##ModelId=49B87B8903CD
	CString		m_sCPU;	//��ѡCPU	
	//##ModelId=49B87B8903CF
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           �߳��˳���־*/
	//##ModelId=49B87B8903D3
	BOOL		m_bExitThread;
	/** @brief           ��¼���������߳�ָ��*/
	//##ModelId=49B87B8903D9
	CWinThread*	m_pLoadThread;

	CString     m_sOperUser;
	CString     m_sMonUser;
	bool        m_bOperVerify;
	bool        m_bMonVerify;

	CPoint  m_pointViewProg;
	bool	m_bAlreadyShowOnce;

	/** @brief			�Ƿ�����ԤУ*/
	BOOL		m_bChecking;
private:
	CString         m_strUser;
	//##ModelId=49B87B8903DE
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B8A0001
	CTypedPtrArray<CPtrArray, PT_SOFTBOARD*> m_arrSoftBoard; //�洢�����������б�
	/** @brief           ����ֵ��*/
	//##ModelId=49B87B8A0005
	int m_nNewValueIndex;
	/** @brief           ��ǰ״̬. 0-����״̬, 1-�����ٻ���ѹ��, 2-������ѹ��Ͷ��*/
	//##ModelId=49B87B8A000F
	int m_nCurrentStatus;
	/** @brief           �Ƿ��һ֡*/
	BOOL m_bFirstFrame;
	/** @brief           �޸�ֵ����*/
	//##ModelId=49B87B8A0011
	MODIFY_LIST		m_arrModifyList;
	/** @brief           �޸�ֵ�ı�����*/
	//##ModelId=49B87B8A001F
	CString m_strOutPut;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B8A0020
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;	
	/** @brief           ��ѹ��Ͷ�˶�ʱ*/
	int			m_nPTSetTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B8A002F
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B8A003F
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B8A0044
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B8A004F
	IXJReport * pReportDoc;

	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;
	CHisValueMgr	m_HisValueMgr;
	/** @brief           ��ǰ״̬. 0-����. 1-����ִ����ѹ��Ͷ��*/
	int m_nCurrentDetailStatus;

private:
	CPtrList	m_listThread;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();
	int GetThreadCount();
// Operations
public:
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8A005D
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8A007D
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8A00AB
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8A00CB
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8A00EA
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8A0109
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B8A0128
	int InitControls();

/*
 *  @brief   	ClearDigitalConfig	 �����ѹ��������Ϣ 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0167
	BOOL ClearSoftBoardConfig();

/*
 *  @brief   	AddDigitalValue	 ����ĳ��ѹ��ֵ���б� 
 *  @param 		[In]a param of Type  CString  Ҫ�������ѹ���ID
 *  @param 		[In]a param of Type  CString  Ҫ�������ѹ���ֵ
 *  @param 		[In]a param of Type  int  ָ��Ҫ���뵽�ڼ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0177
	BOOL AddSoftBoardValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadDataFromSTTP	 ��STTP�����ж�ȡ��ѹ������ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  ��������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A01B5
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 ���¸��ؼ��Ƿ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01C5
	void UpdateControlsEnable();

/*
 *  @brief   	CheckModifyData	 �齨�޸�ֵ���� 
 *  @param 		[Out]a param of Type  MODIFY_LIST&  �޸�ֵ����
 *  @param 		[Out]a param of Type  CString&  ����ı�
 *  @return 	void 
 */
	//##ModelId=49B87B8A01D4
	void CheckModifyData( MODIFY_LIST& modifyList, CString& strOut);

/*
 *  @brief   	ClearModifyList	 ����޸����� 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01E5
	void ClearModifyList();

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B8A01F4
	void RefreshControls();
	
/*
 *  @brief   	SaveSelCPU	 ���浱ǰѡ���CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU������ָ��
 *  @return 	void 
 */	
	//##ModelId=49B87B8A01F5
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 ɾ���¼ӵ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0203
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B8A0204
	void CancelCall();

/*
 *  @brief   	SetSoftBoardValue	 ����ָ������ѹ��ֵ 
 *  @param 		[In]a param of Type  CString  ��ѹ��ID
 *  @param 		[In]a param of Type  CString  ��ѹ��ֵ
 *  @param 		[In]a param of Type  int  ������
 *  @return 	void 
 */
	//##ModelId=49B87B8A0213
	void SetSoftBoardValue( CString ID, CString VALUE, int iCol);

/*
 *  @brief   	RevertModifyValue	 �ظ��޸���ѹ��ֵʱ�޸ĵ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0222
	void RevertModifyValue();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B8A0223
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	DisplayValue	 �õ���ѹ���ֵ��ʾ�ַ��� 
 *  @param 		[In]a param of Type  int  ��ѹ��ֵ
 *  @return 	CString ��ʾֵ
 */
	//##ModelId=49B87B8A0225
	CString DisplayValue(int nValue);

	/*
	* @brief	FindNewValue	����ָ���������ֵ	
	* @param	[In]a param of Type int nID	ָ����ֵ��	
	* @return   CString	ָ���������ֵ
	*/
	CString FindNewValue(int nID);

	PT_SOFTBOARD* FindSoftBoard(CString sID);

	void StartThread();

	int GetNextQueryNo();

	void AddValueCol( int nQueryNo );

	BOOL LoadDefaultCpu();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	/*
 *  @brief   	LoadPTHistoryDigitalData	 ������ѹ����ʷֵ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0196
	BOOL LoadPTHistorySoftBoardData(int nQueryNo);

	/*
 *  @brief   	LoadPTDigitalConfig	 �����ݿ��ȡ��ѹ��������Ϣ 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0148
	BOOL LoadPTSoftBoardConfig(int nQueryNo);

	
	int m_nOperationNum;
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0232
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B8A0233
	void EndOwnerThread();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B8A0234
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8A0242
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B8A0243
	void ReleaseCOM();

/*
 *  @brief   	DoPrintSoftBoard	 ��ӡ��ѹ�� 
 *  @param 		[In]a param of Type  CString  ��ӡģ���ļ�
 *  @return 	void 
 */
	//##ModelId=49B87B8A0244
	void DoPrintSoftBoard(CString strPath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSoftBoard)
	public:
	//##ModelId=49B87B8A0246
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8A0271
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8A029F
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8A02B1
	virtual ~CPTSoftBoard();
#ifdef _DEBUG
	//##ModelId=49B87B8A02DE
	virtual void AssertValid() const;
	//##ModelId=49B87B8A02FD
	virtual void Dump(CDumpContext& dc) const;
#endif

	// ִ����ѹ��Ͷ�˵��޸�
	void ExcutePTSet_Soft();

	// Generated message map functions
	//{{AFX_MSG(CPTSoftBoard)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtsoftCall();
	afx_msg void OnBtnPtsoftModify();
	afx_msg void OnBtnPtsoftModify2();
	afx_msg void OnBtnViewProg();
	afx_msg void OnBtnViewHis();
	afx_msg void OnSelchangeCmbPtsoftCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtsoft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtsoftStyle();
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtsoftCall();
	afx_msg void OnBtnPtsoftVerify1();
	afx_msg void OnBtnPtsoftVerify2();
	afx_msg void OnDblclkListPtsoft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtsoftExcel();
	//}}AFX_MSG
	//##ModelId=49B87B8A03DE
	void OnSTTP20109(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B000F
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B001F
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B003E
	void OnSTTP20118(WPARAM wParam,LPARAM lparam);
	//##ModelId=49B87B8B004E
	void OnSTTP20114(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8B005D
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B0060
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B006F
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8B007F
	afx_msg void OnRefreshPTSoft(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSOFTBOARD_H__93D4BB2F_68ED_4268_8C28_EC69363C24AB__INCLUDED_)
