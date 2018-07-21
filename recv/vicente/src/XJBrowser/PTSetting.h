#if !defined(AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_)
#define AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTSetting.h : header file
//
#include "ViewBase.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTSetting form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
//##ModelId=49B87B8B0232
class CPTSetting : public CViewBase
{
protected:
	//##ModelId=49B87B8B0234
	CPTSetting();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSetting)

// Form Data
public:
	//{{AFX_DATA(CPTSetting)
	enum { IDD = IDD_PT_SETTING };
	CListCtrlEx	m_ListZone;
	CButtonST	m_btnVerify2;
	CButtonST	m_btnVerify1;
	CPictureEx			m_gif;
	CButton	m_btnModifyZone;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnModifySetting;
	CButton	m_btnCallZone;
	CButton	m_btnCallSetting;
	CListCtrlEx	m_List;
	CTabCtrl	m_TabCtrl;
	CString	m_sCurrentZone;
	CString	m_sEditZone;
	CString m_sOldZoneValue;
	bool    m_bCallZoneSuccess;
	bool    m_bOperTow;
	int     m_nModifySettingOrModifyZone;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87B8B030F
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           ��Ӧ����*/
	//##ModelId=49B87B8B031D
	CSecObj*	m_pObj;
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B8B032C
	BOOL		m_bExitThread;
	/** @brief           ��¼���������߳�*/
	//##ModelId=49B87B8B032E
	CWinThread*	m_pLoadThread;

private:
	int				m_nNewZoneIndex;
	CString        m_strUser;
	/** @brief           XP�����*/
	//##ModelId=49B87B8B033D
	CThemeHelperST	m_ThemeHelper;
	/** @brief           ��ֵ�����б�*/
	//##ModelId=49B87B8B034C
	CTypedPtrArray<CPtrArray, PT_SETTING*> m_arrSetting; //��ֵ�����б�
	/** @brief           ��ֵ�������б�*/
	//##ModelId=49B87B8B035C				
	CTypedPtrArray<CPtrArray, PT_GROUP*> m_arrGroup;	//��ֵ���б�
	/** @brief           ������ѡ���CPU��*/
	//##ModelId=49B87B8B0360
	CString		m_sCPU;
	/** @brief           ������ѡ��Ķ�ֵ����*/
	//##ModelId=49B87B8B036B
	CString		m_sZone;
	/** @brief           ��ǰѡ������*/
	//##ModelId=49B87B8B036C
	int			m_nGroup;

	/** @brief           ����ֵ��*/
	//##ModelId=49B87B8B038A
	int m_nNewValueIndex;
	/** @brief           ��ǰ״̬. 0-����. 1-�����ٻ���ֵ. 2-�����ٻ���ֵ����. 3-�����޸Ķ�ֵ. 4-�����л���ֵ��*/
	//##ModelId=49B87B8B038B
	int m_nCurrentStatus;
	/** @brief           Ҫ�޸ĵ�ֵ����*/
	//##ModelId=49B87B8B039A
	MODIFY_LIST	m_arrModifyList;
	/** @brief           �޸ĵ�ֵ�ı�����*/
	//##ModelId=49B87B8B03A9
	CString		m_strOutPut;
	/** @brief			 ��ֵ��Դ*/
	//##ModelId=49B87B8B03AA
	int			m_nSrc;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B8B03B9
	int			m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;
	/** @brief           �Ƿ��ܰ�����ʾ*/
	//##ModelId=49B87B8B03C8
	BOOL		m_bCanGroup;
	CString     m_sOperUser;
	CString     m_sMonUser;
	bool        m_bOperVerify;
	bool        m_bMonVerify;

	/** @brief			�Ƿ�����ԤУ*/
	BOOL		m_bChecking;

	/** @brief			Ҫ�л����Ķ�ֵ��*/
	CString m_sModifyZone;

	/** @brief			��¼��װ���޸Ķ�ֵ����ʱ�޸ĵ���*/
	int m_nModifyItem;

	//��֡����
	/** @brief           �����֡����ʱ���յ���֡���*/
	//##ModelId=49B87B8B03CA
	CUIntArray		m_arrFrame;
	/** @brief           ��֡������֡��*/
	//##ModelId=49B87B8B03D8
	int		m_nFrameCount;
	/** @brief           ��ȡ��֡����ʱ����ֵ*/
	//##ModelId=49B87B8C0001
	CTypedPtrArray<CPtrArray, PT_SETTING*>		m_arrNewSetting;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B8C0010
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B8C0015
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B8C0020
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B8C0025
	IXJReport * pReportDoc;
	
	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;
	/** @brief			������������.0-ȫ����������ǰ��ֵ������ֵ���á���ʷֵ���������л��������л�CPU
									 1-ֻ������ʷֵ���������л���ֵ����
									 2-���������ݣ�ֻ�������ݣ�������TAB�л�*/
	int	m_nLoadType;
	CHisValueMgr	m_HisValueMgr;

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
	//##ModelId=49B87B8C002E
	int InitListCtrl();
	int InitZoneListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8C009C
	int	InitListStyle();
	int	InitZoneListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8C00FA
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8C0157
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8C01B5
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8C0222
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B8C0271
	int InitControls();

/*
 *  @brief   	ClearSettingConfig	 �����ֵ���� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8C02CE
	BOOL ClearSettingConfig();

/*
 *  @brief   	ClearSettingGroup	 �����ֵ������ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8C031C
	BOOL ClearSettingGroup();

/*
 *  @brief   	LoadPTGroupConfig	 ���Ҷ�ֵ������ 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8C036B
	BOOL LoadPTGroupConfig(int nQueryNo);

/*
 *  @brief   	LoadNewOrderTime	 ��ѯ���µĻ�׼ֵʱ�� 
 *  @return 	CString ��׼ֵʱ��
 */
	//##ModelId=49B87B8D001F
	CString LoadNewOrderTime();

	/*
 *  @brief   	AddSettingValue	 ����ĳ��ֵֵ���б� 
 *  @param 		[In]a param of Type  CString  Ҫ����Ķ�ֵ��ID
 *  @param 		[In]a param of Type  CString  Ҫ����ĵ�ֵ
 *  @param 		[In]a param of Type  int  ָ��Ҫ���뵽�ڼ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8D0021
	BOOL AddSettingValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadDataFromSTTP	 ��STTP�����ж�ȡ��ֵ���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  ��������
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8D00BB
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ�������
 *  @return 	void 
 */
	//##ModelId=49B87B8D00EA
	void UpdateControlsEnable();

/*
 *  @brief   	CheckModifyData	 �齨Ҫ�޸ĵĶ�ֵ�б� 
 *  @param 		[Out]a param of Type  MODIFY_LIST&  Ҫ�޸ĵĶ�ֵ�б�
 *  @param 		[Out]a param of Type  CString&  ��������ֱ���
 *  @return 	void 
 */
	//##ModelId=49B87B8D0119
	void CheckModifyData(MODIFY_LIST& modifyList, CString& strOut);

/*
 *  @brief   	GetTranslatedValue	 ��þ�������VPickList���Value 
 *  @param 		[In]a param of Type  PT_SETTING *  ��ֵ����
 *  @param 		[In]a param of Type  const CString  ֵ
 *  @return 	CString ��������VPickList���Value
 */
	//##ModelId=49B87B8D0148
	CString GetTranslatedValue(PT_SETTING *pst,const CString strValue);

/*
 *  @brief   	ClearModifyData	 ����޸�ֵ����
 *  @return 	void 
 */
	//##ModelId=49B87B8D0167
	void ClearModifyData();

/*
 *  @brief   	GetSettingObject	 ���ݶ�ֵID�ҵ���ֵ���� 
 *  @param 		[In]a param of Type  CString  ��ֵID
 *  @return 	PT_SETTING * ��ֵ����
 */
	//##ModelId=49B87B8D0186
	PT_SETTING * GetSettingObject(CString strID);

/*
 *  @brief   	GetIDFromValue	 ����ֵ����ֵ�Ķ��� 
 *  @param 		[In]a param of Type  CString  ֵ˵��
 *  @param 		[In]a param of Type  CString  ֵ
 *  @return 	CString 
 */
	//##ModelId=49B87B8D01A5
	CString GetIDFromValue(CString strPickList, CString strValue);

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B8D01D4
	void RefreshControls();

/*
 *  @brief   	ReFillCPU	 �������CPU������ 
 *  @return 	void 
 */
	//##ModelId=49B87B8D01E4
	void ReFillCPU();
	
/*
 *  @brief   	SaveSelCPU	 ���浱ǰѡ���CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU������ָ��
 *  @return 	void 
 */
	//##ModelId=49B87B8D01F4
	BOOL SaveSelCPU( CComboBox * pCmb );
/*
 *  @brief   	ReFillZone	 ������䶨ֵ���������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0203
	void ReFillZone();
	
/*
 *  @brief   	SaveSelZone	 ���浱ǰѡ��Ķ�ֵ���� 
 *  @param 		[In]a param of Type  CComboBox *  ��ֵ��������ָ��
 *  @return 	void 
 */
	//##ModelId=49B87B8D0213
	BOOL SaveSelZone( CComboBox * pCmb );
/*
 *  @brief   	ResetGroupTab	 �������÷���TAB 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0222
	void ResetGroupTab();
	
/*
 *  @brief   	SaveSelGroup	 ���浱ǰѡ������ 
 *  @param 		[In]a param of Type  CTabCtrl *  ����TABָ��
 *  @return 	void 
 */
	//##ModelId=49B87B8D0232
	BOOL SaveSelGroup( CTabCtrl * pTab );

/*
 *  @brief   	ClearMultiFrame	 ��ն�֡���ĵĻ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0242
	void ClearMultiFrame();

/*
 *  @brief   	TranslateMultiFrame	 ������֡�Ķ�ֵ���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *  @return 	void 
 */
	//##ModelId=49B87B8D0251
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	ClearDataCol	 ɾ���¼ӵ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0261
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B8D0262
	void CancelCall();

/*
 *  @brief   	SetSettingValue	 ����ָ���Ķ�ֵ 
 *  @param 		[In]a param of Type  CString  ��ֵID
 *  @param 		[In]a param of Type  CString  ��ֵֵ
 *  @param 		[In]a param of Type  int  ������
 *  @return 	void 
 */
	//##ModelId=49B87B8D0271
	void SetSettingValue( CString ID, CString VALUE, int iCol);

/*
 *  @brief   	RevertModifyValue	 �ظ��޸Ķ�ֵʱ�޸ĵ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B8D0280
	void RevertModifyValue(int nType = 1);

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B8D0281
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	FindSetting	 ���ݶ�ֵID�ҵ���ֵ����
 *  @param 		[In]a param of Type  CString  ��ֵID
 *  @return 	PT_SETTING* ��ֵ����
 */
	//##ModelId=49B87B8D0291
	PT_SETTING* FindSetting(CString sID);

/*
 *  @brief   	SetGroupName	 ���ö�ֵ������� 
 *  @param 		[In]a param of Type  int  ��ֵ��ID
 *  @param 		[In]a param of Type  CString  ��ֵ������
 *  @return 	void 
 */
	//##ModelId=49B87B8D029F
	void SetGroupName(int nGroupID, CString sName);

	CString GetGroupName(int nGroupID);
	/*
 *  @brief   	CheckVPicklist	���vpicklist�Ƿ���Ϲ淶 
 *  @return 	BOOL 
 */
	BOOL CheckVPicklist(CString strVPickList);

	void SetZoneCmbToCurrentZone();

	/*
	* @brief	FindNewValue ����ָ���������ֵ	
	* @param	[In]a param of Type int nID	ָ����ֵ��	
	* @return   CString	ָ���������ֵ
	*/
	CString FindNewValue(int nID);

	int GetNextQueryNo();

	/*
 *  @brief   	LoadOrderValue	 ���Ҷ�ֵ��׼ֵ 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8C03B9
	BOOL LoadOrderValue(int nQueryNo);

	/*
 *  @brief   	LoadCurrentZone	 �����ݿ��ѯ��ǰ��ֵ�� 
 *  @return 	void 
 */
	void LoadCurrentZone();

	/*
 *  @brief   	LoadSettingGroup	 ���Ҷ�ֵ�� 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8D0020
	BOOL LoadSettingGroup(int nQueryNo);

	/*
 *  @brief   	LoadPTHistorySettingData	 ���붨ֵ��ʷֵ 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B8D006D
	BOOL LoadPTHistorySettingData(int nQueryNo);

	/*
 *  @brief   	LoadSettingConfig	 �����ݿ���뱣����ֵ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8C031D
	BOOL LoadPTSettingConfig(int nQueryNo);

	BOOL LoadDefaultCpu();

public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
	int	GetLoadType(){ return m_nLoadType; };

	BOOL ReLoadData(int nQueryNo, int nLoadType);

	int m_nOperationNum;
		/*
	* @brief	FindNewValue �趨����ֵ	
	* @param	[In]a param of Type int nID	ָ����ֵ	
	* @return   
	*/
	CString SetFloatValue(PT_SETTING* pst, CString strValue);

	CString GetDisplayValue(PT_SETTING* pst, CString strValue);

/*
 *  @brief   	GetTranslatedNumber	��ȡVPicklist���ѡ��ֵ 
 *  @return 	int �ɹ�����nID, ʧ�ܷ���-1
 */
	int GetTranslatedNumber(CString str);
/*
 *  @brief   	ReFillAll	���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8D02AF
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B8D02B0
	void EndOwnerThread();

	void StartThread();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B8D02BF
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8D02C1
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B8D02C2
	void ReleaseCOM();

/*
 *  @brief   	DoPrintSetting	 ��ӡ��ֵ 
 *  @param 		[In]a param of Type  CString  ģ���ļ�ȫ·��
 *  @return 	void 
 */
	//##ModelId=49B87B8D02CE
	void DoPrintSetting(CString strPath);

	BOOL GetZone(CString pt_id, int nCpu, int zone_id, PT_ZONE* pt_zone);
	void OnBtnPtsetModify1();
	void OnBtnPtsetZoneModify1();

	void AddValueCol( int nQueryNo );
	void FillHisData( int nQueryNo );

	void FillData(int nQueryNo, int nLoadType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSetting)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8D03C9
	virtual ~CPTSetting();
#ifdef _DEBUG
	//##ModelId=49B87B8E004E
	virtual void AssertValid() const;
	//##ModelId=49B87B8E00BB
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTSetting)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtsetCall();
	afx_msg void OnBtnPtsetCallzone();
	afx_msg void OnBtnPtsetModify();
	afx_msg void OnBtnPtsetZoneModify();
	afx_msg void OnSelchangeTabPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbPtsetCpu();
	afx_msg void OnSelchangeCmbPtsetSrc();
	afx_msg void OnSelchangeCmbPtsetSelzone();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtsetStyle();
	afx_msg void OnBtnPtsetPrint();
	afx_msg void OnSettingprintNew1();
	afx_msg void OnSettingprintNew2();
	afx_msg void OnSettingprintNew3();
	afx_msg void OnSettingprintOrder1();
	afx_msg void OnSettingprintOrder2();
	afx_msg void OnSettingprintOrder3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtsetCall();
	afx_msg void OnDoubleclickedBtnPtsetCallzone();
	afx_msg void OnBtnPtsetVerify1();
	afx_msg void OnBtnPtsetVerify2();
	afx_msg void OnDblclkListPtset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListPtzone(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtzone(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtsetExcel();
	//}}AFX_MSG
	//##ModelId=49B87B8E034B
	void OnSTTP20016(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8E038A
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8E03C8
	void OnSTTP20125(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F000F
	void OnSTTP20107(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F004E
	void OnSTTP20052(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F006D
	void OnSTTP20054(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F008C
	void OnSTTP20048(WPARAM wParam,LPARAM lParam);
	void OnSTTP20170(WPARAM wParam,LPARAM lParam);
	void OnSTTP20174(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00AB
	void OnSTTP20056(WPARAM wParam,LPARAM lParam);
	void OnSTTP20172(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00CB
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B8F00DC
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F00FA
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F0109
	//void ShowTip();
	afx_msg void OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSETTING_H__9E18E27C_6527_4E34_B53A_771313FB5EA4__INCLUDED_)
