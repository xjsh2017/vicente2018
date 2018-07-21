#if !defined(AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_)
#define AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualFaultShorten.h : header file
//
typedef CMapStringToPtr SECOBJ_MAP;
/////////////////////////////////////////////////////////////////////////////
// CManualFaultShorten dialog
#include "ViewFault.h"

class CManualFaultShorten : public CDialog
{
// Construction
public:
	CManualFaultShorten(CWnd* pParent = NULL);   // standard constructor
    ~CManualFaultShorten();
// Dialog Data
	//{{AFX_DATA(CManualFaultShorten)
	enum { IDD = IDD_DLG_MANUAL_FAULTSHORTEN };
	CListCtrlEx	m_List;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	// Attributes
public:
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87B870267
	CArray<SColumn, SColumn&> m_arrColum;
	SECOBJ_MAP		m_SECMap;
	/** @brief           �����������¼�����*/
	CPtrArray		m_listChr;
	/** @brief           �����ź����¼�����*/
	CPtrArray		m_listSign;
	CViewFault*      pView;
private:
	CImageList m_imageList;
	CBitmap    m_bitmap;
	int        m_nMainRecID;
	CXJEventManager	m_EventManager;

public:

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B87029F
	int FillListData();

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAction *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8702EF
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	int	InitListStyle();


/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadListConfig();

/*
 *  @brief   	FindNetPos	 ���ҵ��������е�λ�� 
 *  @param 		[In]a param of Type  CString  ����ID
 *  @return 	HTREEITEM ���ڵ�ṹ
 */
	//##ModelId=49B87BA403BB
	HTREEITEM FindNetPos(CString sNetID);

	/*
 *  @brief   	InsertDevice	 �����豸���豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CDeviceObj* ָ���豸
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
*  @brief   	InitDeviceTree	 ��ʼ���豸�� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87BA403B4
	int InitDeviceTree();

/*
 *  @brief   	InsertNet	 ��������ڵ� 
 *  @param 		[In]a param of Type  CNetObj*  Ҫ����ĵ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403BD
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 ���볧վ�ڵ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403C8
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	InsertStation_Voltage	 ���볧վ�ڵ㣬��վ�½ӵ�ѹ�ȼ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL InsertStation_Voltage(CStationObj* pObj);

/*
 *  @brief   	InsertVoltage	 �����ѹ�ȼ��ڵ� 
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CStationObj* ָ����վ
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL InsertVoltage(int nVoltage, HTREEITEM htParent, CStationObj* pObj);

/*
 *  @brief   	SortVoltage	 ��վ�ڵ�ѹ�ȼ����� 
 *  @param 		[In]a param of Type  CStationObj*  ָ��վ
 *  @return 	void 
 */
	void SortVoltage(CStationObj* pObj);

/*
 *  @brief   	BuildBreak	 ��ָ����վָ����ѹ�ȼ������п��ؼ��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildBreak(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBus	 ��ָ����վָ����ѹ�ȼ�������ĸ�߼��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildBus(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildCap	 ��ָ����վָ����ѹ�ȼ������е��������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildCap(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildGen	 ��ָ����վָ����ѹ�ȼ������з�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildGen(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildLine	 ��ָ����վָ����ѹ�ȼ���������·���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildLine(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildReactor	 ��ָ����վָ����ѹ�ȼ������е翹�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildReactor(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildSwitch	 ��ָ����վָ����ѹ�ȼ������е�բ���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildSwitch(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildTrans	 ��ָ����վָ����ѹ�ȼ������б�ѹ�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildTrans(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildProtect	 ��ָ����վ�����б������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87BB100A1
	void BuildProtect(CStationObj* pStation);

/*
 *  @brief   	InsertProtect	 ���뱣�����豸�� 
 *  @param 		[In]a param of Type  CSecObj*  ָ������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1009C
	BOOL InsertProtect(CSecObj* pObj);

/*
 *  @brief   	BuildSecToStation	 ��ָ����վ�����ж����豸���뵽�豸���еĶ�Ӧ��վ�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void BuildSecToStation(CStationObj* pStation);

	BOOL InsertSecToStation(CSecObj* pObj);

/*
 *  @brief   	BuildWaveRec	 ��ָ����վ������¼�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1008E
	void BuildWaveRec(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	InsertFastness	 ���Ӳ�ڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @param 		[In]a param of Type  CString  ָ�����ID
 *  @return 	void 
 */
	void InsertFastness(HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBay	 ��ָ����վָ����ѹ�ȼ������м�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @return 	void 
 */
	void BuildBay(CStationObj* pStation, HTREEITEM htParent, int nVoltage);

/*
 *  @brief   	InsertBay	 ���������豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *	@param		[In]a param of Type	 CBayObj*	ָ�����
 *  @return 	void 
 */
	void InsertBay(HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay);

/*
 *  @brief   	ClearEmptyNodeType1	 ����豸������Ϊ1���豸���Ŀսڵ� 
 *  @return 	void 
 */
	void ClearEmptyNodeType1();

/*
 *  @brief   	ClearEmptyNodeType2	 ����豸������Ϊ2���豸���Ŀսڵ� 
 *  @return 	void 
 */
	void ClearEmptyNodeType2();

/*
 *  @brief   	FillTree	 ����豸�� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403CA
	BOOL FillTree();
	
	void BuildGPS( CStationObj* pStation, HTREEITEM htParent );
	
	BOOL QueryActionEvent();
	
	/*************************************************************
	�� �� ��: ClearCheck()
	���ܸ�Ҫ: �������ѡ��
	�� �� ֵ: 
	��    ��: param1 
	Param2 
	**************************************************************/
	void ClearCheck(HTREEITEM ht);
	
	
	/*************************************************************
	�� �� ����SetCheck_XJ()
	���ܸ�Ҫ������ָ���ڵ��ѡ��״̬
	�� �� ֵ: void
	��    ����param1	ָ���ڵ�
	Param2	TRUE-ѡ��. FALSE-ȡ��ѡ��
	**************************************************************/
	//##ModelId=49B87BB100CD
	void SetCheck_XJ( HTREEITEM hit, BOOL bCheck );
	
	/*************************************************************
	�� �� ����CycTreeDown()
	���ܸ�Ҫ�����±���ָ���ڵ�������ӽڵ�, �������ӽڵ�����ͬ��ѡ�����
	�� �� ֵ: void
	��    ����param1	ָ���ڵ�
	Param2	ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
	**************************************************************/
	//##ModelId=49B87BB100BB
	void CycTreeDown( HTREEITEM hRoot, BOOL bMark );
	void AddOrRemoveSecObj( HTREEITEM hRoot, BOOL bMark );
	
	/*************************************************************
	�� �� ����CycTreeUp()
	���ܸ�Ҫ�����ϱ����ڵ�ĸ��ڵ�, �����ǰ�ڵ��Ǹ��ڵ�����һ����ѡ����ӽڵ�, Ϊ���ڵ�Ҳ����ѡ���־
	�� �� ֵ: void
	��    ����param1	ָ���ڵ�
	Param2
	**************************************************************/
	void CycTreeUp( HTREEITEM hCurrent, BOOL bCheck );


/*
 *  @brief   	BreakAction	 �ֽ⶯���¼�Ϊ�����¼� 
 *  @param 		[In]a param of Type  CXJEventAction* ָ�������¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA401A6
	void BreakAction(CXJEventAction* pEvent);

	void OnFaultCreate();
	
/*
 *  @brief   	CreateFaultReport	 �½����ϰ� 
 *  @param 		[In]a param of Type  int  ��ID
 *  @return 	CFaultReport* �´����Ĺ��ϰ�
 */
	CFaultReport* CreateFaultReport(int nID);

/*
 *  @brief   	SaveEvent	 ���汻ѡ����¼������ϰ� 
 *  @return 	void 
 */
	//##ModelId=49B87BA401D9
	void SaveEvent(CFaultReport* pReport);

/*
 *  @brief   	QueryLatsetID	 ��ѯ���µĹ��ϰ�ID 
 *  @return 	int ���µĹ��ϰ�ID
 */
	int QueryLatsetID();


/*
 *  @brief   	AddReportToDB	 �����ϰ����뵽���ݿ�,�¼�������
 *  @param 		[In]a param of Type  CFaultReport*  ָ�����ϰ�
 *  @return 	BOOL ����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	BOOL AddReportToDB(CFaultReport* pReport);

/*
 *  @brief   	MultiSaveEvent	 ���������¼� 
 *  @return 	int ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 */
	int MultiSaveEvent();

/*
 *  @brief   	ClearEvent	 ɾ�������¼� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40196
	void ClearEvent();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualFaultShorten)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualFaultShorten)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtosclistDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_)
