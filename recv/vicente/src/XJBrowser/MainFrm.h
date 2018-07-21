// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F2AE871F_F291_4DE2_9E76_42D1B7FDE6E4__INCLUDED_)
#define AFX_MAINFRM_H__F2AE871F_F291_4DE2_9E76_42D1B7FDE6E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StdAfx.h"
#include "CJ60Lib.h"
#include "ContextDockBar.h"
#include "MDITabs.h"
#include "XJBrowserDoc.h"

#include "LogView.h"	//��־�������
#include "DeviceView.h"	//�豸������
#include "ChildFrm.h"
#include "ActionFrame.h"//�����¼��Ӵ��ڿ��
#include "AlarmFrame.h" //�澯�¼��Ӵ��ڿ��
#include "FaultFrame.h" //�¹ʱ����Ӵ��ڿ��
#include "SysAlarmFrame.h"//ϵͳ�澯�Ӵ��ڿ��
#include "PTFrame.h"	//���������Ӵ��ڿ��
#include "WaveRecFrame.h"//¼�������Դ��ڿ��
#include "CommFrame.h"	//����վͨѶ״̬���ڿ��
#include "BatchFrame.h" //�����ٻ����ڿ��
#include "AutoCallFrame.h"//�Զ��������ô��ڿ��
#include "AutoResultFrame.h"//�Զ����ٽ�����ڿ��
#include "CrossFrame.h" //Ԥ�����ڿ��
#include "SoeFrame.h"	//SOE�¼����ڿ��
#include "DownOscFrame.h" //¼�����ش��ڿ��

#include "DlgEventProp.h" //�¼����Դ���
#include "EventManager.h"

#include "TrueColorToolBar.h"


#include "DownloadFrame.h" //ͨ���ļ����ؿ��
#include "StationInfoFrame.h"
#include "PaintCap.h"

#include "RemindView.h"

#include "DisDevFrame.h"//���װ������ҳ��

//##ModelId=49B87B970196
class CMainFrame : public CCJMDIFrameWnd
//class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	//##ModelId=49B87B970198
	CMainFrame();

// Attributes
public:
	CRITICAL_SECTION m_criSection;
	//##ModelId=49B87B9701B6
	CContextDockBar	m_contextBar[4];	//����ͣ������
	//##ModelId=49B87B9701C6
	CMDITabs		m_wndMDITabs;		//tabBar
	//##ModelId=49B87B9701CB
	CLogView*		m_pLogView;		//�������
	//##ModelId=49B87B9701D5
	CDeviceView*	m_pDeviceView;	//�豸������
	//##ModelId=49B87B9701DA
	CCJTabCtrlBar		m_wndWorkSpaceBar;	//���Bar, ���豸��
	//##ModelId=49B87B9701E5
	CCJTabCtrlBar		m_wndOutputBar;		//�����Bar, ���������
	CRemindView*	m_pRemindView;

	/** @brief           �����ĵ�ָ��*/
	//##ModelId=49B87B9701EA
	CXJBrowserDoc * m_pDoc;

	/** @brief           ͨѶ״̬��ʱ��*/
	int m_nCommStatusTimer;
	/** @brief           ͨѶ�Ͽ�ʱ��*/
	int m_nCommDisconnect;

	//�ӿ��
	//##ModelId=49B87B9701F8
	CCommFrame * pCommFrame;
	//##ModelId=49B87B970204
	CChildFrame * pMapFrame;
	//##ModelId=49B87B970209
	CActionFrame * pActionFrame;
	//##ModelId=49B87B970214
	CAlarmFrame * pAlarmFrame;
	//##ModelId=49B87B970219
	CChildFrame * pDIFrame;
	//##ModelId=49B87B970226
	CFaultFrame * pFaultFrame;
	//##ModelId=49B87B970233
	CChildFrame * pOSCFrame;
	//##ModelId=49B87B970238
	CChildFrame * pPTCommFrame;
	//##ModelId=49B87B970243
	CChildFrame * pPTRunFrame;
	//##ModelId=49B87B970248
	CSysAlarmFrame * pSysAlarmFrame;
	CChildFrame*  pGeoFrame;
	CChildFrame*  pDistanceFrame;

	//##ModelId=49B87B97024D
	CPTFrame *	  pPTFrame;	//�������Դ���ָ��
	//##ModelId=49B87B970252
	CWaveRecFrame *	  pWaveRecFrame; //¼�������Դ���ָ��
	CDisDevFrame *	pDisDevFrame;

	//##ModelId=49B87B970257
	CBatchFrame * pBatchFrame; //�����ٻ�����ָ��
	//##ModelId=49B87B970261
	CAutoCallFrame* pAutoCallFrame;//�Զ����ٴ���ָ��
	//##ModelId=49B87B970266
	CAutoResultFrame* pAutoResultFrame; //�Զ����ٽ������ָ��

	//##ModelId=49B87B97026B
	CDownloadFrame* pDownloadFrame; //ͨ���ļ����ش���

	CCrossFrame* pCrossFrame; //Ԥ������

	CSoeFrame*	pSOEIFrame; //I���¼�����
	CSoeFrame*	pSOEIIFrame; //II���¼�����
	CSoeFrame*	pSOEIIIFrame; //III���¼�����
	CSoeFrame*  pSOEDebugFrame;//�����¼�����

	CStationInfoFrame*	pStationInfoFrame; //��վ��Ϣ����

	CDownOscFrame*	pDownOsc; //¼�����ش���
	CXJBrowserDoc* pTempDoc;
	CReBar   m_wndReBar;
	CDialogBar m_wndDialogBar;
	int      m_oper;
	int	m_nIdleTimer;
	
private:
	/** @brief           �¼����Դ���ָ��*/
	//##ModelId=49B87B970272
	CDlgEventProp*	m_pDlgEventProp;
	/** @brief           ��¼������ʱ��*/
	//##ModelId=49B87B970276
	int				m_nAlarmTimer;
	int       m_CaptionHeight;     //�������ĸ߶�
	int       m_TitleDrawHeight;   //������ʵ�ʵĻ��Ƹ߶�
	CRect m_TitleRc;
	HBITMAP createBitmap;
// Operations
public:
/*
 *  @brief   	OpenSecPropPage	 �򿪶����豸����ҳ
 *  @param 		[In]a param of Type  CSecObj*  �����豸ָ��
 *	@param		[In]a param of Type int	Ҫ�����ĸ�ҳ��
 *  @return 	void 
 */
	//##ModelId=49B87B970280
	void OpenSecPropPage(CSecObj* pObj, int nPage);

/*
 *  @brief   	ShowEventProp	 ��ʾ�¼����Դ��� 
 *  @param 		[In]a param of Type  CXJEventManager*  �¼�������ָ��
 *	@param		[In]a param of Type BOOL �Ƿ񱣻����Դ���Ҫ���¼����ԶԻ���
 *  @return 	void 
 */
	//##ModelId=49B87B970290
	void ShowEventProp(CXJEventManager* pEventManager, BOOL bPT = FALSE);

/*
 *  @brief   	UpdateToolBar	 ���¹��������� 
 *  @param 		[In]a param of Type  CToolBar*  ������ָ��
 *  @param 		[In]a param of Type  TCHAR*  ����
 *  @return 	void 
 */
	//##ModelId=49B87B97029F
	void UpdateToolBar(CToolBar* tb, CStringArray& arrText);

/*
 *  @brief   	GetCommDisconnectTime	 ȡ��ϵͳͨѶ�Ͽ�ʱ�� 
 *  @return 	int ͨѶ�Ͽ�ʱ��
 */
	int GetCommDisconnectTime();

/*
 *  @brief   	ShowStationInfo	 ��ָ����վ�ĳ�վ��Ϣ��ͼ 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void ShowStationInfo(CStationObj* pStation);

/*
 *  @brief   	ShowDownLoadOsc	 ��¼������ҳ�棬������ָ��¼�� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ��Ҫ���ص�¼��
 *  @return 	void 
 */
	void ShowDownLoadOsc(CXJEventOSC* pOsc);

private:
/*
*  @brief  ��ʼ��ͣ������ 
*  @return ����-1˵��ʧ��, ����Ϊ�ɹ�
	*/
	//##ModelId=49B87B9702AF
	int InitDockWindows();
	
	/*
	*  @brief  ����ͣ�����ڵĹ���, �Ա��ڴ��ڴ�С�ı�ʱ��ͬ���ı� 
	*  @return �޷���ֵ
	*/
	//##ModelId=49B87B9702BF
	void HookDockBar();

	int StationInit(CString sStationID, int nInitType);

	BOOL ReportStationInitResult(CString sStationID, int nInitType, int nResult);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL
	
// Implementation
public:
	//##ModelId=49B87B9702FD
	virtual ~CMainFrame();
	//##ModelId=49B87B97031C
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
#ifdef _DEBUG
	//##ModelId=49B87B97033C
	virtual void AssertValid() const;
	//##ModelId=49B87B97034B
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CChildFrame* pStaInfoFrame;
	//##ModelId=49B87B97037A
	CStatusBar  m_wndStatusBar;
	//##ModelId=49B87B97037F
	CTrueColorToolBar    m_wndToolBar;
//	CToolBar24    m_wndToolBar;
	CCaptionPainter m_capp;	
	LRESULT OnPaintMyCaption(WPARAM wp,LPARAM lp);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTestToolBatch();
	afx_msg void OnToolAutocall();
	afx_msg void OnToolAutocallResult();
	afx_msg void OnViewDevicetree();
	afx_msg void OnUpdateViewDevicetree(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToolDownloadFile();
	afx_msg void OnToolAutocallStop();
	afx_msg void OnToolFaultreport();
	afx_msg void OnToolMainsvg();
	afx_msg void OnToolPtcomm();
	afx_msg void OnToolStationcomm();
	afx_msg void OnUpdateToolFaultreport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolMainsvg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolPtcomm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolStationcomm(CCmdUI* pCmdUI);
	afx_msg void OnToolQueryevent();
	afx_msg void OnToolQueryalarm();
	afx_msg void OnToolPTStat();
	afx_msg void OnToolDI();
	afx_msg void OnToolActionEvent();
	afx_msg void OnToolAlarmEvent();
	afx_msg void OnToolOsc();
	afx_msg void OnUpdateToolPTStat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolDI(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolActionEvent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAlarmEvent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolOsc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolQueryalarm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAutocall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAutocallResult(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAutocallStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolQueryevent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolBatch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolDownloadFile(CCmdUI* pCmdUI);
	afx_msg void OnToolSysalarm();
	afx_msg void OnUpdateToolSysalarm(CCmdUI* pCmdUI);
	afx_msg void OnToolGeo();
	afx_msg void OnUpdateToolGeo(CCmdUI* pCmdUI);
	afx_msg void OnViewProp();
	afx_msg void OnUpdateViewProp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolStationinfo(CCmdUI* pCmdUI);
	afx_msg void OnToolStationinfo();
	afx_msg void OnToolDistance();
	afx_msg void OnUpdateToolDistance(CCmdUI* pCmdUI);
	//}}AFX_MSG
	//##ModelId=49B87B98008C
	afx_msg void OnDocumentOpened(WPARAM, LPARAM lParam);
	//##ModelId=49B87B9800AB
	afx_msg void OnDocumentClosed(WPARAM, LPARAM lParam);
	//##ModelId=49B87B9800CB
	afx_msg void OnSTTP20008(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9800EA
	afx_msg void OnSTTP20012(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980109
	afx_msg void OnSTTP20016(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980119
	afx_msg void OnSTTP20048(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20170(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980138
	afx_msg void OnSTTP20052(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980157
	afx_msg void OnSTTP20054(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980177
	afx_msg void OnSTTP20056(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20172(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980196
	afx_msg void OnSTTP20058(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9801A5
	afx_msg void OnSTTP20060(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9801C5
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9801E4
	afx_msg void OnSTTP20089(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9801F4
	afx_msg void OnSTTP20107(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20174(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980213
	afx_msg void OnSTTP20109(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980222
	afx_msg void OnSTTP20111(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980242
	afx_msg void OnSTTP20114(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980251
	afx_msg void OnSTTP20116(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980264
	afx_msg void OnSTTP20118(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980280
	afx_msg void OnSTTP20124(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B980290
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9802AF
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9802BF
	afx_msg void OnSTTP20006(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9802DE
	afx_msg void OnSTTP20010(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9802EE
	afx_msg void OnSTTP20014(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98030D
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98031C
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98033C
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98034B
	afx_msg void OnSTTP20132(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98036B
	afx_msg void OnSTTP20138(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98037A
	afx_msg void OnSTTP20139(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B98038B
	afx_msg void OnSTTP20141(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9803A9
	afx_msg void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9803B9
	afx_msg void OnSTTP20026(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9803C9
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lparam);
	//##ModelId=49B87B9803D9
	afx_msg void OnSTTP20151(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990001
	afx_msg void OnSTTP20154(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990011
	afx_msg void OnSTTP20155(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990021
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99002F
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20159(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99003F
	afx_msg void OnSTTP00204(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99004F
	afx_msg void OnSTTP00212(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20084(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20004(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20086(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20176(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20168(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20177(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99005E
	afx_msg void OnStatusInfo(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99006E
	afx_msg void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99008C
	afx_msg void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B99009C
	afx_msg void OnEventPropClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9900AB
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9900BC
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9900CC
	afx_msg void OnEventPTAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9900EA
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9900FA
	afx_msg void OnEventListPassiveAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990109
	afx_msg void OnEventPTPassiveAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990119
	afx_msg void OnMdiTabRefresh(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990128
	afx_msg void OnBatchFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990139
	afx_msg void OnAutoCallResultClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990149
	afx_msg void OnRefreshActionView(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990158
	afx_msg void OnRefreshAlarmView(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990168
	afx_msg void OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990178
	afx_msg void OnRefreshPTDI(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990186
	afx_msg void OnRefreshPTSoft(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B990196
	afx_msg void DoHideDeviceTree(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9901A5
	afx_msg void OnAlarmSound(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9901A9
	afx_msg void OnDownloadClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCommStatusChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewStationInfoClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDownloadOscClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdatebk();
	afx_msg void OnBatchCallEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStartLock(WPARAM wParam, LPARAM lParma);
	afx_msg void OnStationStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTestStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAddRemind(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPushTopView(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisDevFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		friend class CXJBrowserApp;
	friend class CBatchDetailSettingView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F2AE871F_F291_4DE2_9E76_42D1B7FDE6E4__INCLUDED_)
