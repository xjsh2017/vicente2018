// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "MainFrm.h"

#include "ViewComm.h"	//����վͨѶ״̬��ͼ
#include "ViewAction.h"		//�����¼���ͼ
#include "ViewAlarm.h"		//�澯�¼���ͼ
#include "ViewFault.h"	//�¹ʱ�����ͼ
#include "ViewOSC.h"		//¼���¼���ͼ
#include "ViewDI.h"			//�������¼���ͼ
#include "ViewPTComm.h"	//����ͨѶ״̬��ͼ
#include "ViewPTRun.h"	//��������״̬��ͼ
#include "ViewSysAlarm.h"		//ϵͳ�澯��ͼ
#include "XJBrowserView.h"
#include "ViewFaultSheet.h" //�¹ʱ���SHEET��ͼ
#include "ViewStaInfo.h"
#include "PTSheet.h" //��������SHEET��ͼ

#include "BatchSettingView.h"//�Զ�����������ͼ
#include "BatchSelectView.h"//�����ٻ�ѡ����ͼ
#include "AutoCallResult.h" //�Զ����ٽ����ͼ
#include "ACResultSel.h"

#include "DownLoadView.h"  //ͨ���ļ�������ͼ

#include "ViewCross.h" //Ԥ����ͼ

#include "ViewSOE.h"	//SOE�¼���ͼ

#include "Mmsystem.h" //playsound

#include "DlgAutoCallStop.h" //ֹͣ�Զ�����

#include "ViewStationInfo.h"
#include "GlobalFunc.h"
//#include <winuser.h>
#include "windows.h"

#include "ViewDistance.h"

#include "RemindObj.h"

#include "MsgView.h"	//��Ϣ�������
#include "PTSetProgView.h"
#include "DlgCheckPro.h"
#include "DlgValidateID.h"

#include "XJPTSetStore.h"
#include "XJUserStore.h"
#include "qptsetstatetable.h"


#define PTSET_REFRESHTIME	3000

UINT CreateGeoThread(LPVOID pParam)
{
	TRACE("CreateGeoThread start!\n");
	CMainFrame* pFrame = (CMainFrame*)pParam;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CCreateContext context;

	TRACE("CreateGeoThread end!\n");
	return 0;
}

UINT RefreshPTSetState(LPVOID pParam)
{
	if(!pParam)
	{
		WriteLog("������ֵ�޸�״̬��ˢ���߳�ʧ��!", XJ_LOG_LV3);
		return -1;
	}
	WriteLog("������ֵ�޸�״̬��ˢ���̳߳ɹ�!", XJ_LOG_LV3);

	CMainFrame * pFrame = (CMainFrame*)pParam;
	while(1)
	{
		if(pFrame->m_bThreadExit)
			break;
		int ntemptime = 0;
		while( ntemptime < PTSET_REFRESHTIME )
		{
			Sleep(10);
			ntemptime+=10;
			if(pFrame->m_bThreadExit)
				break;
		}
		if(pFrame->m_bThreadExit)
			break;

		CXJPTSetStore *pPTSetStore = CXJPTSetStore::GetInstance();
		pPTSetStore->ReLoadState();

		CXJUserStore *pUserStore = CXJUserStore::GetInstance();
		pUserStore->ReLoad();
		//pStore->Save();

		//pView->UpdateAllObj();
		//pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	}

	WriteLog("�˳���ֵ�޸�״̬��ˢ���̳߳ɹ�!", XJ_LOG_LV3);

	return 0;
}

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CCJMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CCJMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_COMMAND(ID_TOOL_BATCH, OnTestToolBatch)
	ON_COMMAND(ID_TOOL_AUTOCALL, OnToolAutocall)
	ON_COMMAND(ID_TOOL_AUTOCALL_RESULT, OnToolAutocallResult)
	ON_COMMAND(ID_VIEW_DEVICETREE, OnViewDevicetree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEVICETREE, OnUpdateViewDevicetree)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOL_DOWNLOAD_FILE, OnToolDownloadFile)
	ON_COMMAND(ID_TOOL_AUTOCALL_STOP, OnToolAutocallStop)
	ON_COMMAND(ID_TOOL_FAULTREPORT, OnToolFaultreport)
	ON_COMMAND(ID_TOOL_MAINSVG, OnToolMainsvg)
	ON_COMMAND(ID_TOOL_PTCOMM, OnToolPtcomm)
	ON_COMMAND(ID_TOOL_STATIONCOMM, OnToolStationcomm)
	ON_UPDATE_COMMAND_UI(ID_TOOL_FAULTREPORT, OnUpdateToolFaultreport)
	ON_UPDATE_COMMAND_UI(ID_TOOL_MAINSVG, OnUpdateToolMainsvg)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PTCOMM, OnUpdateToolPtcomm)
	ON_UPDATE_COMMAND_UI(ID_TOOL_STATIONCOMM, OnUpdateToolStationcomm)
	ON_COMMAND(ID_TOOL_QUERYEVENT, OnToolQueryevent)
	ON_COMMAND(ID_TOOL_QUERYALARM, OnToolQueryalarm)
	ON_COMMAND(ID_TOOL_PTSTAT, OnToolPTStat)
	ON_COMMAND(ID_TOOL_DI, OnToolDI)
	ON_COMMAND(ID_TOOL_ACTION_EVENT, OnToolActionEvent)
	ON_COMMAND(ID_TOOL_ALARM_EVENT, OnToolAlarmEvent)
	ON_COMMAND(ID_TOOL_OSC, OnToolOsc)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PTSTAT, OnUpdateToolPTStat)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DI, OnUpdateToolDI)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ACTION_EVENT, OnUpdateToolActionEvent)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ALARM_EVENT, OnUpdateToolAlarmEvent)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OSC, OnUpdateToolOsc)
	ON_UPDATE_COMMAND_UI(ID_TOOL_QUERYALARM, OnUpdateToolQueryalarm)
	ON_UPDATE_COMMAND_UI(ID_TOOL_AUTOCALL, OnUpdateToolAutocall)
	ON_UPDATE_COMMAND_UI(ID_TOOL_AUTOCALL_RESULT, OnUpdateToolAutocallResult)
	ON_UPDATE_COMMAND_UI(ID_TOOL_AUTOCALL_STOP, OnUpdateToolAutocallStop)
	ON_UPDATE_COMMAND_UI(ID_TOOL_QUERYEVENT, OnUpdateToolQueryevent)
	ON_UPDATE_COMMAND_UI(ID_TOOL_BATCH, OnUpdateToolBatch)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DOWNLOAD_FILE, OnUpdateToolDownloadFile)
	ON_COMMAND(ID_TOOL_SYSALARM, OnToolSysalarm)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SYSALARM, OnUpdateToolSysalarm)
	ON_COMMAND(ID_TOOL_GEO, OnToolGeo)
	ON_UPDATE_COMMAND_UI(ID_TOOL_GEO, OnUpdateToolGeo)
	ON_COMMAND(ID_VIEW_PROP, OnViewProp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROP, OnUpdateViewProp)
	ON_UPDATE_COMMAND_UI(ID_TOOL_STATIONINFO, OnUpdateToolStationinfo)
	ON_COMMAND(ID_TOOL_STATIONINFO, OnToolStationinfo)
	ON_UPDATE_COMMAND_UI(ID_FILE_DOWNLOAD, OnUpdateToolDownloadFile)
	ON_UPDATE_COMMAND_UI(ID_CALL_ALL, OnUpdateToolBatch)
	ON_COMMAND(ID_TOOL_DISTANCE, OnToolDistance)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DISTANCE, OnUpdateToolDistance)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEWDOCUMENTOPENED, OnDocumentOpened)
	ON_MESSAGE(WM_DOCUMENTCLOSEED, OnDocumentClosed)
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20058, OnSTTP20058)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20089, OnSTTP20089)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
	ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_STTP_20109, OnSTTP20109)
	ON_MESSAGE(WM_STTP_20111, OnSTTP20111)
	ON_MESSAGE(WM_STTP_20114, OnSTTP20114)
	ON_MESSAGE(WM_STTP_20118, OnSTTP20118)
	ON_MESSAGE(WM_STTP_20124, OnSTTP20124)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20006, OnSTTP20006)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(WM_STTP_20014, OnSTTP20014)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(WM_STTP_20132, OnSTTP20132)
	ON_MESSAGE(WM_STTP_20138, OnSTTP20138)
	ON_MESSAGE(WM_STTP_20139, OnSTTP20139)
	ON_MESSAGE(WM_STTP_20141, OnSTTP20141)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20151, OnSTTP20151)
	ON_MESSAGE(WM_STTP_20154, OnSTTP20154)
	ON_MESSAGE(WM_STTP_20155, OnSTTP20155)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20116, OnSTTP20116)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_00204, OnSTTP00204)
	ON_MESSAGE(WM_STTP_00212, OnSTTP00212)
	ON_MESSAGE(WM_STTP_20159, OnSTTP20159)
	ON_MESSAGE(WM_STTP_20084, OnSTTP20084)
	ON_MESSAGE(WM_STTP_20146, OnSTTP20146)
    ON_MESSAGE(WM_STTP_20004, OnSTTP20004)
	ON_MESSAGE(WM_STTP_20086, OnSTTP20086)
	ON_MESSAGE(WM_STTP_20176, OnSTTP20176)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
	ON_MESSAGE(WM_STTP_20177, OnSTTP20177)
	ON_MESSAGE(WM_STATUS_INFO, OnStatusInfo)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
	ON_MESSAGE(WM_EVENTPROP_CLOSE, OnEventPropClose)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTLIST_PASSIVE_ACK, OnEventListPassiveAck)
	ON_MESSAGE(WM_EVENTPT_PASSIVE_ACK, OnEventPTPassiveAck)
	ON_MESSAGE(WM_MDITAB_REFRESH, OnMdiTabRefresh)
	ON_MESSAGE(BATCH_FRAME_CLOSE, OnBatchFrameClose)
	ON_MESSAGE(AUTOCALL_RESULT_CLOSE, OnAutoCallResultClose)
	ON_MESSAGE(REFRESH_ACTIONVIEW, OnRefreshActionView)
	ON_MESSAGE(REFRESH_ALARMVIEW, OnRefreshAlarmView)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
	ON_MESSAGE(REFRESH_PTDI, OnRefreshPTDI)
	ON_MESSAGE(REFRESH_PTSOFTBOARD, OnRefreshPTSoft)
	ON_MESSAGE(DEVICETREE_HIDE, DoHideDeviceTree)
	ON_MESSAGE(ALARM_SOUND, OnAlarmSound)
	ON_MESSAGE(DOWNLOAD_CLOSE, OnDownloadClose)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(COMM_STATUS_CHANGE, OnCommStatusChange)
	ON_MESSAGE(STATIONINFO_FRAME_CLOSE, OnViewStationInfoClose)
	ON_MESSAGE(DOWNOSC_FRAME_CLOSE, OnDownloadOscClose)
	ON_MESSAGE(WM_PAINTMYCAPTION,OnPaintMyCaption)
	ON_MESSAGE(BATCHCALL_END,OnBatchCallEnd)
	ON_MESSAGE(STARTLOCK, OnStartLock)
	ON_MESSAGE(STATION_CHANGED, OnStationStatusChanged)
	ON_MESSAGE(TEST_STATIONSTATUS, OnTestStationStatus)
	ON_MESSAGE(ADD_REMIND, OnAddRemind)
	ON_MESSAGE(PUSHTOP_FVIEW, OnPushTopView)
	ON_MESSAGE(WM_DISDEVFRAME_CLOSE, OnDisDevFrameClose)
//	ON_COMMAND(ID_UPDATEBK, OnUpdatebk)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_STTPSTATE,
	ID_INDICATOR_LOGINTIP,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

//##ModelId=49B87B970198
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	pCommFrame		= NULL;
	pMapFrame		= NULL;
	pActionFrame	= NULL;
	pAlarmFrame		= NULL;
	pDIFrame		= NULL;
	pFaultFrame		= NULL;
	pOSCFrame		= NULL;
	pPTCommFrame	= NULL;
	pPTRunFrame		= NULL;
	pSysAlarmFrame	= NULL;
	pGeoFrame		= NULL;
	pStaInfoFrame   = NULL;

	pPTFrame = NULL;
	pWaveRecFrame = NULL;
	pDisDevFrame = NULL;

	pBatchFrame = NULL;
	pAutoCallFrame = NULL;
	pAutoResultFrame = NULL;

	pDownloadFrame = NULL;
	pCrossFrame = NULL;

	pSOEIFrame = NULL;
	pSOEIIFrame = NULL;
	pSOEIIIFrame = NULL;
	pSOEDebugFrame = NULL;

	pStationInfoFrame = NULL;
	pDistanceFrame = NULL;

	m_pDoc = NULL;

	m_pDlgEventProp = NULL;

	m_nAlarmTimer = -1;
	m_nCommStatusTimer = -1;
	m_nCommDisconnect = 0;

	m_pRemindView = NULL;

	pDownOsc = NULL;
	pTempDoc = NULL;
	::InitializeCriticalSection(&m_criSection);
	m_oper = 0;
	m_pThread = NULL;
	m_bThreadExit = FALSE;

	m_nIdleTimer = -1;
}

//##ModelId=49B87B9702FD
CMainFrame::~CMainFrame()
{
	EndThread();

	if(m_pDlgEventProp != NULL)
		delete m_pDlgEventProp;
	TRACE("CMainFrame::~CMainFrame \n");
	::DeleteCriticalSection(&m_criSection);
}

//##ModelId=49B87B97038A
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//	SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;  

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP 
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CStringArray arrTips;
	arrTips.Add( StringFromID(IDS_TOOLBAR_CONNECT));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_DEVICETREE));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_LOG));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_GEO));
	arrTips.Add( StringFromID(IDS_TOOLBAR_STATIONGRPHA) );
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_STATIONCOMMU));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_STATIONINFO));
	arrTips.Add( StringFromID(IDS_TOOLBAR_DEVICECOMMU));
	arrTips.Add( StringFromID(IDS_TOOLBAR_DEVICERUN));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_FAULTREPORT));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_ACTION));
	arrTips.Add( StringFromID(IDS_TOOLBAR_ALARM));
	arrTips.Add( StringFromID(IDS_TOOLBAR_OSC));
	arrTips.Add( StringFromID(IDS_TOOLBAR_DIGITAL));
	arrTips.Add( StringFromID(IDS_TOOLBAR_LOCATION));
	arrTips.Add( StringFromID(IDS_TOOLBAR_SYSALARM));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_BATCHCALL));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_COMMONFILE));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_HISQUERY));
	arrTips.Add( StringFromID(IDS_TOOLBAR_ALARMSTAT));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_SYSCONF));
	arrTips.Add( StringFromID(IDS_TOOLBAR_ABOUT));
	arrTips.Add( "" );
	arrTips.Add( StringFromID(IDS_TOOLBAR_EXIT));
	
	//���ӹ���������
	UpdateToolBar(&m_wndToolBar, arrTips);

	/*
	int i,j=m_wndToolBar.GetToolBarCtrl().GetButtonCount();
	LPRECT btnpos=new RECT[j+1];
	for(i=0;i<j;i++)
		m_wndToolBar.GetItemRect(i,&btnpos[i]);
	  
	m_wndToolBar.SetFullColorImage(IDB_TOOL1_NOR, RGB(255, 0, 255),btnpos,j);
	*/
	
	m_wndToolBar.LoadTrueColorToolBar( 32, IDB_TOOL1_NOR, IDB_TOOL1_NOR, IDB_TOOL1_G);
	if (g_role == MODE_SUB)
	{
		g_CommonFileButton = 0;
		g_bShowStationInfoWnd = FALSE;
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_STATIONCOMM, TBSTATE_HIDDEN);
	}

	//ֻ���ư�ť�Ƿ���ʾ������һֱ����
	if (!g_ConnectServerButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_STTP_CONNECT, TBSTATE_HIDDEN);
	}
	if (!g_DeviceTreeButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_VIEW_DEVICETREE, TBSTATE_HIDDEN);
	}
	if (!g_LogButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_VIEW_OUTPUT, TBSTATE_HIDDEN);
	}
	if (!g_HistoryQueryButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_QUERYEVENT, TBSTATE_HIDDEN);
	}
	if (!g_AlarmCountButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_QUERYALARM, TBSTATE_HIDDEN);
	}
	if (!g_SystemSettingButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_SYS_SETTING, TBSTATE_HIDDEN);
	}
	if (!g_CallAllButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_CALL_ALL, TBSTATE_HIDDEN);
	}
	if (!g_CommonFileButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_FILE_DOWNLOAD, TBSTATE_HIDDEN);
	}
	if (!g_ExitButton)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_SYS_EXIT, TBSTATE_HIDDEN);
	}

	//���¸��ݹ����Ƿ��������ƹ������ϰ�ť�Ƿ���ʾ
	if(!g_bShowGeoWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_GEO, TBSTATE_HIDDEN);
	}
	if (!g_bShowPTCommWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_PTCOMM, TBSTATE_HIDDEN);
	}
	if (!g_bShowPTRunWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_PTSTAT, TBSTATE_HIDDEN);
	}
	if (!g_bShowFlautWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_FAULTREPORT, TBSTATE_HIDDEN);
	}
	if (!g_bShowDIWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_DI, TBSTATE_HIDDEN);
	}
	if (!g_bShowActionWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_ACTION_EVENT, TBSTATE_HIDDEN);
	}
	if (!g_bShowAlarmWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_ALARM_EVENT, TBSTATE_HIDDEN);
	}
	if (!g_bShowOSCWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_OSC, TBSTATE_HIDDEN);
	}
	if(!g_bShowSysAlarmWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_SYSALARM, TBSTATE_HIDDEN);
	}
	if (!g_bShowStationInfoWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_STATIONINFO,TBSTATE_HIDDEN);
	}
	if(!g_bShowDistanceWnd)
	{
		m_wndToolBar.GetToolBarCtrl().SetState(ID_TOOL_DISTANCE,TBSTATE_HIDDEN);
	}
	

	if (g_style == 1)
	{
		m_wndDialogBar.Create(this, IDD_DIALOG_LOGO, CBRS_TOP, IDD_DIALOG_LOGO);
		m_wndReBar.Create(this, RBS_FIXEDORDER & ~RBS_BANDBORDERS);             //����ReBar����
		//����������Ŀ�깤����	
		m_wndReBar.AddBar(&m_wndDialogBar, NULL, NULL, RBBS_FIXEDSIZE | RBBS_NOGRIPPER);
		m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_NOGRIPPER);
	
		m_wndReBar.RedrawWindow();                                                    //�ػ洰��
		REBARBANDINFO info;                                                        // REBARBANDINFO�ṹ����
		info.cbSize = sizeof(info);                                                       //���ô�С
		info.fMask = RBBIM_BACKGROUND;                                     //��� 
       // info.fStyle |= RBBS_FIXEDBMP;
		m_wndToolBar.ModifyStyle(0,TBSTYLE_TRANSPARENT);                  //���ù���������͸��
		info.hbmBack = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TOOL));//����λͼ��Դ
		m_wndReBar.GetReBarCtrl().SetBandInfo(1,&info); //����ReBar����
		m_wndReBar.GetReBarCtrl().SetBandInfo(0,&info);
		//m_wndReBar.CalcFixedLayout(TRUE, TRUE);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//��ʾ������
    //  RepositionBars(0,0xffff,0,NULL,NULL,FALSE); 
		m_capp.Install(this, WM_PAINTMYCAPTION);
	}
	else if (g_style == 2)
	{
		m_wndDialogBar.Create(this, IDD_DIALOG_LOGO2, CBRS_TOP, IDD_DIALOG_LOGO2);
		m_wndReBar.Create(this, RBS_FIXEDORDER);             //����ReBar����
		//����������Ŀ�깤����
		m_wndReBar.AddBar(&m_wndDialogBar, NULL, NULL, RBBS_FIXEDSIZE | RBBS_NOGRIPPER);
		m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_NOGRIPPER);

		m_wndReBar.RedrawWindow();                                                    //�ػ洰��
		REBARBANDINFO info;                                                        // REBARBANDINFO�ṹ����
		info.cbSize = sizeof(info);                                                       //���ô�С
		info.fMask = RBBIM_BACKGROUND;                                     //��� 
		// info.fStyle |= RBBS_FIXEDBMP;
		m_wndToolBar.ModifyStyle(0,TBSTYLE_TRANSPARENT);                  //���ù���������͸��
		info.hbmBack = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TOOL2));//����λͼ��Դ
		m_wndReBar.GetReBarCtrl().SetBandInfo(1,&info); //����ReBar����
		m_wndReBar.GetReBarCtrl().SetBandInfo(0,&info);
		//	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_PANE_FIRST, reposDefault);//��ʾ������
		//  RepositionBars(0,0xffff,0,NULL,NULL,FALSE); 
		m_capp.Install(this, WM_PAINTMYCAPTION);
	}
	else if (g_style == 3)
	{
		//m_wndDialogBar.Create(this, IDD_DIALOG_LOGO3, CBRS_TOP, IDD_DIALOG_LOGO3);
		m_wndReBar.Create(this, RBS_FIXEDORDER & ~RBS_BANDBORDERS);             //����ReBar����
		//����������Ŀ�깤����
		m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_FIXEDSIZE | RBBS_NOGRIPPER);
		
		m_wndReBar.RedrawWindow();                                                    //�ػ洰��
		REBARBANDINFO info;                                                        // REBARBANDINFO�ṹ����
		info.cbSize = sizeof(info);                                                       //���ô�С
		info.fMask = RBBIM_BACKGROUND;                                     //��� 
		// info.fStyle |= RBBS_FIXEDBMP;
		m_wndToolBar.ModifyStyle(0,TBSTYLE_TRANSPARENT);                  //���ù���������͸��
		
		createBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), g_strToolBarBK, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION /*| LR_DEFAULTSIZE*/); 
		info.hbmBack = createBitmap;
		
		//info.hbmBack = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TOOL));//����λͼ��Դ
		m_wndReBar.GetReBarCtrl().SetBandInfo(0,&info); //����ReBar����
		//m_wndReBar.CalcFixedLayout(TRUE, TRUE);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);//��ʾ������
		//  RepositionBars(0,0xffff,0,NULL,NULL,FALSE); 
		m_capp.Install(this, WM_PAINTMYCAPTION);
	}
	else
	{
		//m_capp.Install(this, WM_PAINTMYCAPTION);
	}



	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

//	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_TOOLTIPS );
	//����״̬����С
	UINT nID[3], nStyle[3];
	int iWidth[3];   
	m_wndStatusBar.GetPaneInfo(0, nID[0], nStyle[0], iWidth[0]);
	m_wndStatusBar.GetPaneInfo(1, nID[1], nStyle[1], iWidth[1]);
	m_wndStatusBar.GetPaneInfo(2, nID[2], nStyle[2], iWidth[2]);

	m_wndStatusBar.SetPaneInfo(0, nID[0], SBPS_STRETCH, 0);
	m_wndStatusBar.SetPaneInfo(1, nID[1], nStyle[1], 360); //ͨ��״̬
	m_wndStatusBar.SetPaneInfo(2, nID[2], nStyle[2], 360); //��¼��Ϣ

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	if (!g_style)
	{
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		//m_wndToolBar.EnableDocking(CBRS_TOP);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBar);
	}
	
	//��ʼ��ͣ������
	InitDockWindows();
	//����ͣ�����ڵĹ���
	HookDockBar();
	//��ʼ��tabBar, ע���ⲽҪ������������ܴ֮��
	UINT tabPos = MT_BOTTOM;

//	m_wndMDITabs.Create(this, tabPos|MT_HIDEWLT2VIEWS);
	//m_wndMDITabs.Create(this, MT_BOTTOM|MT_IMAGES);

	SetTitle(g_sAppName);

	// ����Ѱ�ұ�� 
	::SetProp(m_hWnd, "XJBrowser", (HANDLE)1); 
	
	m_nTimer = SetTimer(10, 1*1000, NULL);
	// ������ֵ�޸�״̬��ˢ���߳�
	m_nMsgTimer = SetTimer(201, 3*1000, NULL);
	StartThread();
	
	//	DrawMenuBar();
	/*
	CBitmap   map; 
	BOOL bResult = map.LoadBitmap(IDB_BMP_LOGO); //IDB_MAP��Ҫ��ӵı���ͼƬ�� 
	if(!m_wndReBar.AddBar(&m_wndToolBar,NULL,NULL,RBBS_FIXEDSIZE|RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP )) 
	{ 
		TRACE0( "Failed   create   rebar! "); 
		return   -1; 
	} 
	//CPictureEx* pic = new CPictureEx;
	//pic->Load(IDB_BMP_LOGO);
	//m_wndReBar.AddBar(pic);
	map.Detach();
	*/
	//delete []btnpos;
	
	return 0;
}

//##ModelId=49B87B9702DE
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	//cs.style &=   ~FWS_ADDTOTITLE;
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
//##ModelId=49B87B97033C
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

//##ModelId=49B87B97034B
void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//##ModelId=49B87B9702AF
int CMainFrame::InitDockWindows()
{
	SetInitialSize(125,140,320,125);
	//��ߵ�Bar
	if (!m_wndWorkSpaceBar.Create(this, 100,
		StringFromID(IDS_DOCK_DEVICETABLE), CSize(320,400), CBRS_LEFT| CBRS_SIZE_DYNAMIC))
	{
		TRACE0("failed to create workspacebar\n");    
		return -1;      // fail to create
	}
	//�±ߵ�Bar
	if(!m_wndOutputBar.Create(this, 101, StringFromID(IDS_DOCK_OUTPUT),
		CSize(200,200), CBRS_BOTTOM | CBRS_SIZE_DYNAMIC))
	{
		TRACE("failed to create outputbar\n");
		return -1;
	}
	// ������Bar
	if(!m_wndGlobalMsgBar.Create(this, 102, StringFromID(IDS_WINDOW_MSG),
		CSize(700,260), CBRS_BOTTOM | CBRS_SIZE_DYNAMIC))
	{
		TRACE("failed to create msgbar\n");
		return -1;
	}
	//m_wndGlobalMsgBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndWorkSpaceBar.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);
	m_wndOutputBar.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);
	m_wndGlobalMsgBar.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);
	EnableDockingSizeBar(CBRS_ALIGN_ANY);
	DockSizeBar(&m_wndWorkSpaceBar);
	DockSizeBar(&m_wndOutputBar);
	DockSizeBar(&m_wndGlobalMsgBar);

	//����DeviceView(��ߵĳ�վ)
	m_wndWorkSpaceBar.AddView(StringFromID(IDS_DOCK_DEVICETREE), RUNTIME_CLASS(CDeviceView));
	m_pDeviceView = (CDeviceView*)(m_wndWorkSpaceBar.GetActiveView());
	m_wndWorkSpaceBar.SetActiveView(RUNTIME_CLASS(CDeviceView));
	
	//������־��
	m_wndOutputBar.AddView(StringFromID(IDS_DOCK_OUTPUT_OUT), RUNTIME_CLASS(CLogView));
	m_pLogView	 = (CLogView*)(m_wndOutputBar.GetView(RUNTIME_CLASS(CLogView)));
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp ->m_LogMgr.SetLogView(m_pLogView);	
	if(g_bShowRemindWnd)
	{
		m_wndOutputBar.AddView(StringFromID(IDS_DOCK_OUTPUT_REMIND), RUNTIME_CLASS(CRemindView));
		m_pRemindView = (CRemindView*)(m_wndOutputBar.GetView(RUNTIME_CLASS(CRemindView)));
	}

	// ������Ϣ��
	//m_wndGlobalMsgBar.AddView(StringFromID(IDS_DOCK_MSG), RUNTIME_CLASS(CMsgView));
	m_wndGlobalMsgBar.ModifyTabStyle(WS_VISIBLE, 0);
	m_wndGlobalMsgBar.AddView(StringFromID(IDS_DOCK_MSG), RUNTIME_CLASS(CPTSetProgView));
	//m_pLogView	 = (CLogView*)(m_wndGlobalMsgBar.GetView(RUNTIME_CLASS(CLogView)));
	
	//����������ʾ
	if(g_ShowOutputWnd == 0)
	{
		//����
		m_wndOutputBar.GetDockingFrame()->ShowControlBar(&m_wndOutputBar,FALSE,FALSE);
	}

	if(g_ShowDeviceTreeWnd == 0)
	{
		//����
		m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,FALSE,FALSE);
	}
	
	//if(0 == g_ShowMsgWnd)
	{
		//����
		m_wndGlobalMsgBar.GetDockingFrame()->ShowControlBar(&m_wndGlobalMsgBar,FALSE,FALSE);
		//FloatControlBar(&m_wndGlobalMsgBar, CPoint(100,100), CBRS_ALIGN_LEFT);
		//m_wndGlobalMsgBar.ShowWindow(SW_HIDE);
	}

	return 0;
}

/*************************************************************
�� �� ����HookDockBar()
���ܸ�Ҫ������ͣ��������Ϣ�ص�
�� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9702BF
void CMainFrame::HookDockBar()
{
	DWORD dwDockBarMap[4] = { AFX_IDW_DOCKBAR_TOP,  AFX_IDW_DOCKBAR_BOTTOM, 
		AFX_IDW_DOCKBAR_LEFT, AFX_IDW_DOCKBAR_RIGHT};
	CWnd *pWnd;
	for(int i = 0; i < 4; i++)
	{
		pWnd = GetControlBar(dwDockBarMap[i]);
		m_contextBar[i].HookWindow(pWnd);
		//	m_contextBar[i].SetContextMenu(IDR_MENU_TOOLCONTEXT);
	}
}

/*************************************************************
�� �� ����OnUpdateFrameTitle()
���ܸ�Ҫ������TabCtrl����
�� �� ֵ: void
**************************************************************/
//##ModelId=49B87B97031C
void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
//	m_wndMDITabs.Update(); // sync the mditabctrl with all views

	m_capp.UpdateFrameTitle(m_hWnd,m_strTitle);
}

/*************************************************************
 �� �� ����OnDocumentOpened()
 ���ܸ�Ҫ�����ĵ���Ϣ����
 �� �� ֵ: void
 ��    ����param1   WPARAM
           Param2   LPARAM	�ĵ�����
**************************************************************/
//##ModelId=49B87B98008C
void CMainFrame::OnDocumentOpened(WPARAM, LPARAM lParam)
{
	LONGLONG start = StartCount();
	LONGLONG startTime = 0;
	//�õ��ĵ�����
	pTempDoc = (CXJBrowserDoc*)lParam;
	MYASSERT(pTempDoc);
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//CMainFrame* pFrame = (CMainFrame*)pParam;
	CMainFrame* pFrame = (CMainFrame*)this;
	CCreateContext context;

	//CWinThread* ViewThread = AfxBeginThread(CreateGeoThread, this, THREAD_PRIORITY_NORMAL);

	if (g_role != MODE_SUB)
	{	
		startTime = StartCount();
		if(pFrame->pMapFrame == NULL && g_bShowDeviceMapWnd)
		{
			g_wndWait.AddDetailString( StringFromID(IDS_INIT_STATIONCOMMU_VIEW));
			//	::EnterCriticalSection(&pFrame->m_criSection);
			pFrame->pMapFrame = new CChildFrame();
			pFrame->pMapFrame ->m_nType = XJ_VIEW_MAP;
			context.m_pNewViewClass=RUNTIME_CLASS(CXJBrowserView);
			context.m_pCurrentDoc=pFrame->pTempDoc;
			context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtMap]; //pDoc->GetDocTemplate();
			pFrame->pMapFrame->LoadFrame(IDR_XJBROWMAP,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
			pFrame->pMapFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
			pApp->m_pMapView = (CXJBrowserView*)pFrame->pMapFrame->GetActiveView();
			//	::LeaveCriticalSection(&pFrame->m_criSection);
		}
		EndCount("������վͨ����ͼʱ��", startTime);

		
		//����ͼ����
		if(pFrame->pGeoFrame == NULL && g_bShowGeoWnd)
		{
			g_wndWait.AddDetailString( StringFromID(IDS_INIT_GEO_VIEW));
		//	::EnterCriticalSection(&pFrame->m_criSection);
			pFrame->pGeoFrame = new CChildFrame();
			context.m_pNewViewClass=RUNTIME_CLASS(CGeoView);
			context.m_pCurrentDoc=pFrame->pTempDoc;
			context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtGeo]; //pDoc->GetDocTemplate();
			pFrame->pGeoFrame->LoadFrame(IDR_XJBROWMAP,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
			pFrame->pGeoFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
		//	::LeaveCriticalSection(&pFrame->m_criSection);
		}
	}

	if(pPTFrame == NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_PTPROP_VIEW));
		int start = GetTickCount();
		WriteLog("��ʼ������������ҳ��ͼ", XJ_LOG_LV3);
		pPTFrame = new CPTFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CPTSheet);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtPTSheet]; //pDoc->GetDocTemplate();
		pPTFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pPTFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
		WriteLog("������������ҳ��ͼ���", XJ_LOG_LV3);
		int interval = GetTickCount() - start;
		CString str;
		str.Format("������������ҳ��ͼ����ʱ��Ϊ%d ms", interval);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	//�¹ʱ��洰��
	if(pFrame->pFaultFrame == NULL && g_bShowFlautWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_FAULTREPORT_VIEW));
		//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pFaultFrame = new CFaultFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CViewFaultSheet);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtFault]; //pDoc->GetDocTemplate();
		pFrame->pFaultFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pFaultFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
		//	::LeaveCriticalSection(&pFrame->m_criSection);
	}


	
	startTime = StartCount();
	//�����¼�����
	if(pFrame->pActionFrame == NULL && g_bShowActionWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_ACTION_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pActionFrame = new CActionFrame();
		//	pActionFrame->m_nType = XJ_VIEW_ACTION;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewAction);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtAction]; //pDoc->GetDocTemplate();
		pFrame->pActionFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pActionFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	EndCount("����������ͼʱ��", startTime);


	
	startTime = StartCount();
		//�澯�¼�����
	if(pFrame->pAlarmFrame == NULL && g_bShowAlarmWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_ALARM_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pAlarmFrame = new CAlarmFrame();
		//	pAlarmFrame->m_nType = XJ_VIEW_ALARM;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewAlarm);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtAlarm]; //pDoc->GetDocTemplate();
		pFrame->pAlarmFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pAlarmFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	EndCount("�����澯��ͼʱ��", startTime);
	
	
	//¼��
	if(pFrame->pOSCFrame == NULL && g_bShowOSCWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_RECODER_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pOSCFrame = new CChildFrame();
		pFrame->pOSCFrame->m_nType = XJ_VIEW_OSC;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewOSC);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtOSC]; //pDoc->GetDocTemplate();
		pFrame->pOSCFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pOSCFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}

	//��վ��Ϣ
	if(pFrame->pStaInfoFrame == NULL && g_bShowStationInfoWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_STATIONINFO_VIEW));
		//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pStaInfoFrame = new CChildFrame();
		pFrame->pStaInfoFrame->m_nType = XJ_VIEW_STAINFO;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewStaInfo);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtStaInfo]; //pDoc->GetDocTemplate();
		pFrame->pStaInfoFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pStaInfoFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
		//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	//������
	if(pFrame->pDIFrame == NULL && g_bShowDIWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_DIGITAL_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pDIFrame = new CChildFrame();
		pFrame->pDIFrame->m_nType = XJ_VIEW_DI;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewDI);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtDI]; //pDoc->GetDocTemplate();
		pFrame->pDIFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pDIFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	
	//ͨѶ״̬
	if(pFrame->pPTCommFrame == NULL && g_bShowPTCommWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_DEVCOMMU_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pPTCommFrame = new CChildFrame();
		pFrame->pPTCommFrame->m_nType = XJ_VIEW_PTCOMM;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewPTComm);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtPTCommStatus]; //pDoc->GetDocTemplate();
		pFrame->pPTCommFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pPTCommFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	if(pFrame->pPTCommFrame != NULL)
	{
		//ȡ�û��ͼָ��
		CView * pView = pFrame->pPTCommFrame->GetActiveView();
		if(pView != NULL)
		{
			pApp->m_pPTComm = (CViewPTComm*)pView;
		} 
	}
	
	
	//����״̬
	if(pFrame->pPTRunFrame == NULL && g_bShowPTRunWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_DEVRUN_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pPTRunFrame = new CChildFrame();
		pFrame->pPTRunFrame->m_nType = XJ_VIEW_PTRUN;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewPTRun);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtPTRunStatus]; //pDoc->GetDocTemplate();
		pFrame->pPTRunFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pPTRunFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	if(pFrame->pPTRunFrame != NULL)
	{
		//ȡ�û��ͼָ��
		CView * pView = pFrame->pPTRunFrame->GetActiveView();
		if(pView != NULL)
		{
			pApp->m_pPTRun = (CViewPTRun*)pView;
		} 
	}

	//ϵͳ�澯
	if(pFrame->pSysAlarmFrame == NULL && g_bShowSysAlarmWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_SYSALARM_VIEW));
		//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pSysAlarmFrame = new CSysAlarmFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CViewSysAlarm);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtSysAlarm]; //pDoc->GetDocTemplate();
		pFrame->pSysAlarmFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pSysAlarmFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
		//	::LeaveCriticalSection(&pFrame->m_criSection);
	}

	//Ԥ��
	if(pFrame->pCrossFrame == NULL && g_bShowCrossWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_CROSS_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pCrossFrame = new CCrossFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CViewCross);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtCross];
		pFrame->pCrossFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pCrossFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	//I���¼�
	if(pFrame->pSOEIFrame == NULL && g_bShowSOEIWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_EVENTI_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pSOEIFrame = new CSoeFrame();
		pFrame->pSOEIFrame->m_nType = 0;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewSOE);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtSOEI];
		pFrame->pSOEIFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pSOEIFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	//II���¼�
	if(pFrame->pSOEIIFrame == NULL && g_bShowSOEIIWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_EVENTII_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pSOEIIFrame = new CSoeFrame();
		pFrame->pSOEIIFrame->m_nType = 1;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewSOE);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtSOEII];
		pFrame->pSOEIIFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pSOEIIFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	//III���¼�
	if(pFrame->pSOEIIIFrame == NULL && g_bShowSOEIIIWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_EVENTIII_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pSOEIIIFrame = new CSoeFrame();
		pFrame->pSOEIIIFrame->m_nType = 2;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewSOE);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtSOEIII];
		pFrame->pSOEIIIFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pSOEIIIFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}
	
	//�����¼�
	if(pFrame->pSOEDebugFrame == NULL && g_bShowSOEDebugWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_TESTING_VIEW));
	//	::EnterCriticalSection(&pFrame->m_criSection);
		pFrame->pSOEDebugFrame = new CSoeFrame();
		pFrame->pSOEDebugFrame->m_nType = 3;
		context.m_pNewViewClass=RUNTIME_CLASS(CViewSOE);
		context.m_pCurrentDoc=pFrame->pTempDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtSOEDebug];
		pFrame->pSOEDebugFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pFrame->pSOEDebugFrame->InitialUpdateFrame(pFrame->pTempDoc,TRUE);
	//	::LeaveCriticalSection(&pFrame->m_criSection);
	}

	if(pDistanceFrame == NULL && g_bShowDistanceWnd)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_INIT_DISTANCE_VIEW));
		pDistanceFrame = new CChildFrame();
		context.m_pNewViewClass = RUNTIME_CLASS(CViewDistance);
		context.m_pCurrentDoc = pTempDoc;
		context.m_pNewDocTemplate = pApp->m_pDocTempl[CXJBrowserApp::wtDistance];
		pDistanceFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame, &context);
		pDistanceFrame->InitialUpdateFrame(pTempDoc,TRUE);
	}

	//WaitForSingleObject(ViewThread->m_hThread, INFINITE);

	TRACE("CMainFrame::OnDocumentOpened \n");
	
	EndCount("OnDocumentOpened����ʱ��Ϊ", start);
}

/*************************************************************
 �� �� ����OnDocumentClosed()
 ���ܸ�Ҫ���ر��ĵ���Ϣ����
 �� �� ֵ: void
 ��    ����param1   WPARAM
           Param2   LPARAM	�ĵ�����
**************************************************************/
//##ModelId=49B87B9800AB
void CMainFrame::OnDocumentClosed(WPARAM, LPARAM lParam)
{
	TRACE("CMainFrame::OnDocumentClosed start\n");
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�رո�����
	if(pMapFrame != NULL)
	{
		pMapFrame->DestroyWindow();
		pMapFrame = NULL;
	}

	if(pGeoFrame != NULL)
	{
		pGeoFrame->DestroyWindow();
		pGeoFrame = NULL;
	}

	if(pActionFrame	!= NULL)
	{
		pActionFrame ->DestroyWindow();
		pActionFrame = NULL;
	}

	if(pAlarmFrame != NULL)
	{
		pAlarmFrame ->DestroyWindow();
		pAlarmFrame = NULL;
	}

	if(pDIFrame != NULL)
	{
		pDIFrame ->DestroyWindow();
		pDIFrame = NULL;
	}

	if(pFaultFrame != NULL)
	{
		pFaultFrame ->DestroyWindow();
		pFaultFrame = NULL;
	}

	if(pOSCFrame != NULL)
	{
		pOSCFrame ->DestroyWindow();
		pOSCFrame = NULL;
	}
	if (pStaInfoFrame != NULL)
	{
		pStaInfoFrame ->DestroyWindow();
		pStaInfoFrame = NULL;
	}
	if(pPTCommFrame	!= NULL)
	{
		pPTCommFrame ->DestroyWindow();
		pPTCommFrame = NULL;
	}

	if(pPTRunFrame != NULL)
	{
		pPTRunFrame ->DestroyWindow();
		pPTRunFrame = NULL;
	}

	if(pSysAlarmFrame != NULL)
	{
		pSysAlarmFrame ->DestroyWindow();
		pSysAlarmFrame = NULL;
	}

	if(pPTFrame != NULL)
	{
		pPTFrame->DestroyWindow();
		pPTFrame = NULL;
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->DestroyWindow();
		pWaveRecFrame = NULL;
	}
	if(pBatchFrame != NULL)
	{
		pBatchFrame->DestroyWindow();
		pBatchFrame = NULL;
	}

	if(pAutoCallFrame != NULL)
	{
		pAutoCallFrame->DestroyWindow();
		pAutoCallFrame = NULL;
	}

	if(pAutoResultFrame != NULL)
	{
		pAutoResultFrame->DestroyWindow();
		pAutoResultFrame = NULL;
	}

	if(pDownloadFrame != NULL)
	{
		pDownloadFrame->DestroyWindow();
		pDownloadFrame = NULL;
	}

	if(pCrossFrame != NULL)
	{
		pCrossFrame->DestroyWindow();
		pCrossFrame = NULL;
	}

	if(pSOEIFrame != NULL)
	{
		pSOEIFrame->DestroyWindow();
		pSOEIFrame = NULL;
	}

	if(pSOEIIFrame != NULL)
	{
		pSOEIIFrame->DestroyWindow();
		pSOEIIFrame = NULL;
	}

	if(pSOEIIIFrame != NULL)
	{
		pSOEIIIFrame->DestroyWindow();
		pSOEIIIFrame = NULL;
	}

	if(pSOEDebugFrame != NULL)
	{
		pSOEDebugFrame->DestroyWindow();
		pSOEDebugFrame = NULL;
	}

	if(pStationInfoFrame != NULL)
	{
		pStationInfoFrame->DestroyWindow();
		pStationInfoFrame = NULL;
	}

	if(pDownOsc != NULL)
	{
		pDownOsc->DestroyWindow();
		pDownOsc = NULL;
	}

	if(pDisDevFrame != NULL)
	{
		pDisDevFrame->DestroyWindow();
		pDisDevFrame = NULL;
	}

	TRACE("CMainFrame::OnDocumentClosed end\n");
}

//##ModelId=49B87B9703A9
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWnd::OnSysCommand(nID, lParam);
	if(nID == SC_MINIMIZE || nID == SC_MAXIMIZE)
	{
		CWnd * pWnd = (CWnd *)GetActiveFrame();
		if(pWnd != NULL)
		{
		//	pWnd->SendMessage(WM_SYSCOMMAND, WPARAM(nID), lParam);
		}
	}

	/*
	if (nID == SC_RESTORE && !IsZoomed())
	{
		m_wndDialogBar.ShowWindow(FALSE);
		WriteLog("nID == SC_RESTORE && !IsZoomed()");
	}
	else if (nID == SC_RESTORE && IsZoomed())
	{
		m_wndDialogBar.ShowWindow(TRUE);
		WriteLog("nID == SC_RESTORE && IsZoomed()");
	}
	else 
	{
		m_wndDialogBar.ShowWindow(TRUE);
		WriteLog("else");
	}
	Invalidate(TRUE);
	*/
	
}

//##ModelId=49B87B9703C8
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	rc.bottom += 30;
	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	return TRUE;
//	return CCJMDIFrameWnd::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����OnSTTP20008()
 ���ܸ�Ҫ��ת��ģ��������֪ͨ����(20008)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9800CB
void CMainFrame::OnSTTP20008( WPARAM wParam, LPARAM lParam )
{
	//ģ��������֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20008, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20008, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20012()
 ���ܸ�Ҫ��ת��20012(��������������֪ͨ)
 �� �� ֵ: void 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9800EA
void CMainFrame::OnSTTP20012( WPARAM wParam, LPARAM lParam )
{
	//����������֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20012, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20012, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20016()
 ���ܸ�Ҫ��ת��������ֵ����֪ͨ(20016)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980109
void CMainFrame::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	//��ֵ����֪ͨ,ת�����������Դ��ں�¼�������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20016, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20016, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20048()
 ���ܸ�Ҫ��ת��������ֵ���л�(ԤУ)֪ͨ(20048)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980119
void CMainFrame::OnSTTP20048( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20048, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20048, wParam, lParam);
	}
}

void CMainFrame::OnSTTP20170( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20170, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20170, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20052()
 ���ܸ�Ҫ��ת��������ֵ�޸�(ԤУ)֪ͨ(20052)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980138
VOID CMainFrame::OnSTTP20052( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20052, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20052, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20054()
 ���ܸ�Ҫ��ת��������ֵ�޸�(ִ��)֪ͨ(20054)
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980157
void CMainFrame::OnSTTP20054( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�ִ��֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20054, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20054, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20056()
 ���ܸ�Ҫ��ת��������ֵ���л�ִ��֪ͨ(20056)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980177
void CMainFrame::OnSTTP20056( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20056, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20056, wParam, lParam);
	}
}

void CMainFrame::OnSTTP20172( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20172, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20172, wParam, lParam);
	}
}


/*************************************************************
 �� �� ����OnSTTP20058()
 ���ܸ�Ҫ��ת�������źŸ���֪ͨ(20058)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980196
void CMainFrame::OnSTTP20058( WPARAM wParam, LPARAM lParam )
{
	//�����źŸ���֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20058, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20060()
 ���ܸ�Ҫ��ת��������ʱ֪ͨ(20060)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9801A5
void CMainFrame::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	//������ʱ֪ͨ,ת�����������Դ���,¼�������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20060, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame ->SendMessage(WM_STTP_20060, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ��ת�����Ʊ���ͨ�÷���֪ͨ(20069)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9801C5
void CMainFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//���Ʊ���ͨ�÷���֪ͨ,ת�����������Դ���,¼�������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(pDownloadFrame != NULL)
	{
		pDownloadFrame ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(pDownOsc != NULL)
	{
		pDownOsc->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(pCommFrame != NULL)
	{
		pCommFrame->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	
	if(pAutoCallFrame != NULL)
	{
		pAutoCallFrame->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20107()
 ���ܸ�Ҫ��ת�������ٻ���ǰ��ֵ��֪ͨ����(20107)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9801F4
void CMainFrame::OnSTTP20107( WPARAM wParam, LPARAM lParam )
{
	//�����ٻ���ǰ��ֵ��֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20107, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20107, wParam, lParam);
	}
}

void CMainFrame::OnSTTP20174( WPARAM wParam, LPARAM lParam )
{
	//�����ٻ���ǰ��ֵ��֪ͨ,ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20174, wParam, lParam);
	}
	if(pWaveRecFrame)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20174, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20109()
 ���ܸ�Ҫ��ת�������ٻ���ѹ��֪ͨ(20109)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980213
void CMainFrame::OnSTTP20109( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�������ѹ��֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20109, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20111()
 ���ܸ�Ҫ��ת���ٻ���������֪ͨ(20111)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980222
void CMainFrame::OnSTTP20111( WPARAM wParam, LPARAM lParam )
{
	//�ٻ���������֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20111, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20114()
 ���ܸ�Ҫ��ת����ѹ��Ͷ��ִ��֪ͨ(20114)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980242
void CMainFrame::OnSTTP20114( WPARAM wParam, LPARAM lParam )
{
	//��ѹ��Ͷ��ִ��֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20114, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20118()
 ���ܸ�Ҫ��ת����ѹ��Ͷ��(ԤУ)֪ͨ(20118)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980264
void CMainFrame::OnSTTP20118( WPARAM wParam, LPARAM lParam )
{
	//��ѹ��Ͷ��֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20118, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20124()
 ���ܸ�Ҫ��¼����Զ����������֪ͨ(20124)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980280
void CMainFrame::OnSTTP20124( WPARAM wParam, LPARAM lParam )
{
	//¼����Զ����������֪ͨ, ת����¼��������
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame ->SendMessage(WM_STTP_20124, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ��ת��װ�ò�֧�ֱ���(20125)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980290
void CMainFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//װ�ò�֧�ֹ���, ת�����������Դ���,¼�������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(pDownloadFrame != NULL)
	{
		pDownloadFrame ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(pDownOsc != NULL)
	{
		pDownOsc->SendMessage(WM_STTP_20125, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20002()
 ���ܸ�Ҫ��������, Ȼ��ѽ���������ҳ��
 �� �� ֵ: void
 ��    ����param1
		   Param2 ��������
**************************************************************/
//##ModelId=49B87B9802AF
void CMainFrame::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pSec = (CSecObj*)lParam;
	if(!pSec)
		return;
	//֪ͨ����ͨѶ״̬ҳ��
	if(pPTCommFrame != NULL)
	{
		pPTCommFrame ->SendMessage(WM_STTP_20002, (WPARAM)pSec, 0);
	}
	//֪ͨ��վ��Ϣ�б�ҳ��
	if(pStationInfoFrame != NULL)
	{
		pStationInfoFrame->SendMessage(WM_STTP_20002, (WPARAM)pSec, 0);
	}
	//�ı�ͼ����ɫ��ʾ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserView * pView = pApp->GetSVGView();
	if(pView != NULL)
	{
		if(pView->m_sStationID == pSec->m_sStationID)
		{
			pView->SetDeviceCol(pSec);
		}
	}
	
	//ȡ�ó�վID
	CDataEngine* pData = pApp->GetDataEngine();
	CStationObj* pStation = (CStationObj*)pData->FindDevice(pSec->m_sStationID, TYPE_STATION);
	//֪ͨ�豸������
	if(m_pDeviceView != NULL && pStation)
	{
		m_pDeviceView->SendMessage(STATION_INFO_CHANGED, 0, (LPARAM)pStation);
	}
	//֪ͨ��վͨѶ״̬ҳ��
	if(pCommFrame != NULL && pStation)
	{
		pCommFrame->SendMessage(STATION_INFO_CHANGED, 0, (LPARAM)pStation);
	}
}

/*************************************************************
 �� �� ����OnSTTP20006()
 ���ܸ�Ҫ��ת������ģ��������ֵ����֪ͨ(20006)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9802BF
void CMainFrame::OnSTTP20006( WPARAM wParam, LPARAM lParam )
{
	//����ģ��������ֵ����֪ͨ,�ݲ�ת��
}

/*************************************************************
 �� �� ����OnSTTP20010()
 ���ܸ�Ҫ��ת����������������ֵ����֪ͨ(20010)
 �� �� ֵ:  void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9802DE
void CMainFrame::OnSTTP20010( WPARAM wParam, LPARAM lParam )
{
	CXJEventDI* pEvent = (CXJEventDI*)lParam;
	if(!pEvent)
		return;
	//ת��
	if(pDIFrame != NULL)
	{
		pDIFrame->SendMessage(WM_STTP_20010, wParam, lParam);
	}
	if(pSOEDebugFrame != NULL && pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		pSOEDebugFrame->SendMessage(WM_STTP_20010, wParam, (LPARAM)pEvent);
	}
}

/*************************************************************
 �� �� ����OnSTTP20014()
 ���ܸ�Ҫ��ת��������ֵ����֪ͨ(20014)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9802EE
void CMainFrame::OnSTTP20014( WPARAM wParam, LPARAM lParam )
{
	//������ֵ����֪ͨ,�ݲ�ת��
}

/*************************************************************
 �� �� ����OnSTTP20018()
 ���ܸ�Ҫ��ת�������澯�¼�����֪ͨ(20018)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98030D
void CMainFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	
	CXJEventAlarm * pEvent = (CXJEventAlarm *)lParam;
	//�����澯�¼�����֪ͨ, ת�����澯�¼�����
	if(pAlarmFrame != NULL)
	{
		pAlarmFrame ->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pSOEIFrame != NULL && pEvent->m_nProperty == 0)
	{
		//I��
		pSOEIFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pSOEIIFrame != NULL && pEvent->m_nProperty == 1)
	{
		//II��
		pSOEIIFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pSOEIIIFrame != NULL && pEvent->m_nProperty == 2)
	{
		//III��
		pSOEIIIFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pSOEDebugFrame != NULL && pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		//������Ϣ
		pSOEDebugFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pMapFrame != NULL)
	{
		pMapFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	if(pDisDevFrame != NULL)
	{
		pDisDevFrame->SendMessage(WM_STTP_20018, wParam, (LPARAM)pEvent);
	}
	
	if(m_pRemindView != NULL)
	{
		CString sContent = "";
		sContent.Format("{%s.%03d [%s][%s][%s] %s}", 
			pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"),pEvent->m_nms, 
			pEvent->m_sStationName, pEvent->m_sDeviceName, pEvent->m_sSecName, 
			pEvent->m_sMsg);
		CRemindContent * pRC = new CRemindContent(StringFromID(IDS_TOOLBAR_ALARM), sContent);
		m_pRemindView->SendMessage(ADD_REMIND, 0, (LPARAM)pRC);
	}
}

/*************************************************************
 �� �� ����OnSTTP20022()
 ���ܸ�Ҫ��ת�����������¼�����֪ͨ(20022)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98031C
void CMainFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	CXJEventAction * pEvent = (CXJEventAction *)lParam;

	//���������¼�����֪ͨ, ת���������¼�����
	if(pActionFrame != NULL)
	{
		pActionFrame ->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pSOEIFrame != NULL && pEvent->m_nProperty == 0)
	{
		//I��
		pSOEIFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pSOEIIFrame != NULL && pEvent->m_nProperty == 1)
	{
		//II��
		pSOEIIFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pSOEIIIFrame != NULL && pEvent->m_nProperty == 2)
	{
		//III��
		pSOEIIIFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pSOEDebugFrame != NULL && pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		//������Ϣ
		pSOEDebugFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(pMapFrame != NULL)
	{
		pMapFrame->SendMessage(WM_STTP_20022, wParam, (LPARAM)pEvent);
	}
	if(m_pRemindView != NULL)
	{
		CString sContent = "";
		sContent.Format("{%s.%03d [%s][%s][%s] %s}", 
			pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"),pEvent->m_nms, 
			pEvent->m_sStationName, pEvent->m_sDeviceName, pEvent->m_sSecName, 
			pEvent->m_sMsg);
		CRemindContent * pRC = new CRemindContent(StringFromID(IDS_TOOLBAR_ACTION), sContent);
		m_pRemindView->SendMessage(ADD_REMIND, 0, (LPARAM)pRC);
	}
}

/*************************************************************
 �� �� ����OnSTTP20079()
 ���ܸ�Ҫ��ת����������״̬����֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98033C
void CMainFrame::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pSec = (CSecObj*)lParam;
	if(!pSec)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;

	if(pPTRunFrame != NULL)
	{
		pPTRunFrame ->SendMessage(WM_STTP_20079, (WPARAM)pSec, 0);
	}
	if(pStationInfoFrame != NULL)
	{
		pStationInfoFrame->SendMessage(WM_STTP_20079, (WPARAM)pSec, 0);
	}
	
	//�ı�ͼ����ɫ��ʾ
	CXJBrowserView * pView = pApp->GetSVGView();
	if(pView != NULL)
	{
		if(pView->m_sStationID == pSec->m_sStationID)
		{
			pView->SetDeviceCol(pSec);
		}
	}
	//֪ͨ�豸������
	if(m_pDeviceView != NULL)
	{
		//ȡ�ó�վID
		CStationObj* pStation = (CStationObj*)pData->FindDevice(pSec->m_sStationID, TYPE_STATION);
		if(pStation != NULL)
			m_pDeviceView->SendMessage(STATION_INFO_CHANGED, 0, (LPARAM)pStation);
	}
}

/*************************************************************
 �� �� ����OnSTTP20132()
 ���ܸ�Ҫ��ת��¼���ļ��Զ�����֪ͨ(20132)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98034B
void CMainFrame::OnSTTP20132( WPARAM wParam, LPARAM lParam )
{
	CXJEventOSC* pEvent = (CXJEventOSC*)lParam;
	if(!pEvent)
		return;
	//¼���ļ��Զ�����֪ͨ, ת����¼���¼�����
	if(pOSCFrame != NULL)
	{
		pOSCFrame ->SendMessage(WM_STTP_20132, wParam, lParam);
	}
	if(m_pRemindView)
	{
		CString sContent = "";
		sContent.Format("{%s.%03d [%s][%s][%s] %s}", 
			pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"),pEvent->m_nms, 
			pEvent->m_sStationName, pEvent->m_sDeviceName, pEvent->m_sSecName, 
			pEvent->m_sFileName);
		CRemindContent * pRC = new CRemindContent(StringFromID(IDS_TOOLBAR_OSC), sContent);
		m_pRemindView->SendMessage(ADD_REMIND, 0, (LPARAM)pRC);
	}
}

/*************************************************************
 �� �� ����OnSTTP20138()
 ���ܸ�Ҫ��ת����վ���ñ仯֪ͨ(20138)
 �� �� ֵ: void 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98036B
void CMainFrame::OnSTTP20138( WPARAM wParam, LPARAM lParam )
{
	//��վ���ñ仯֪ͨ,�ݲ�ת��
	if(pSysAlarmFrame != NULL)
		pSysAlarmFrame->SendMessage(WM_STTP_20138, wParam, lParam);
}

/*************************************************************
 �� �� ����OnSTTP20139()
 ���ܸ�Ҫ��ת����ֵ���仯����֪ͨ(20139)
 �� �� ֵ:  void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98037A
void CMainFrame::OnSTTP20139( WPARAM wParam, LPARAM lParam )
{
	//��ֵ���仯����֪ͨ, �ݲ�ת��
}

/*************************************************************
 �� �� ����OnSTTP20141()
 ���ܸ�Ҫ��ת����ѹ��仯����֪ͨ(20141)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B98038B
void CMainFrame::OnSTTP20141( WPARAM wParam, LPARAM lParam )
{
	//��ѹ��仯����֪ͨ, �ݲ�ת��
}

/*************************************************************
 �� �� ����OnSTTP20144()
 ���ܸ�Ҫ��ת����վͨѶ״̬�仯����֪ͨ(20144)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9803A9
void CMainFrame::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
	CStationObj* pObj = (CStationObj *)lParam;
	if(!pObj)
		return;
	//��վͨѶ״̬�仯����֪ͨ, ת��������վͨѶ״̬����
	if(pCommFrame != NULL)
	{
		pCommFrame ->SendMessage(WM_STTP_20144, wParam, lParam);
	}
	if (pStaInfoFrame != NULL)
	{
		pStaInfoFrame ->SendMessage(WM_STTP_20144,wParam,lParam);
	}
	
	
	//֪ͨ�豸������
	
	if(m_pDeviceView != NULL && pObj != NULL)
	{
		m_pDeviceView->SendMessage(STATION_CHANGED, 0, (LPARAM)pObj);
	}
	if(pMapFrame != NULL)
	{
		pMapFrame->SendMessage(STATION_CHANGED, 0, (LPARAM)pObj);
	}
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CGeoView* pGeoView = pApp->GetGeoView();
	if(pGeoView != NULL && pObj != NULL)
		pGeoView->RefreshObj(pObj);
}

/*************************************************************
 �� �� ����OnSTTP20089()
 ���ܸ�Ҫ��ת�������ٻ�ʱ��֪ͨ(20089)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9801E4
void CMainFrame::OnSTTP20089( WPARAM wParam, LPARAM lParam )
{
	//�����ٻ�ʱ��֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20089, wParam, lParam);
	}
}


/*************************************************************
 �� �� ����OnSTTP20004()
 ���ܸ�Ҫ��ת���ٻ���������֪ͨ(20004)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980222
void CMainFrame::OnSTTP20004( WPARAM wParam, LPARAM lParam )
{
	//�ٻ���������֪ͨ, ת�����������Դ���
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20004, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20086()
 ���ܸ�Ҫ��ת���ٻ���������֪ͨ(20086)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980222
void CMainFrame::OnSTTP20086( WPARAM wParam, LPARAM lParam )
{
	//�ٻ���������֪ͨ, ת�����������Դ���
	if (pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20086, wParam, lParam);
	}
}

//##ModelId=49B87B980000
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(AfxMessageBox( StringFromID(IDS_TIP_EXITCONFIRM), MB_YESNO) != IDYES)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	// �û��˳����ñ�־λ
	//pApp->SetUserLoginFlag(pApp->m_User.m_strUSER_ID, pApp->m_User.m_strGROUP_ID, CString(""));
	CXJUserStore::GetInstance()->SetUserFlags(pApp->m_User.m_strUSER_ID.GetBuffer(0)
		, pApp->m_User.m_strGROUP_ID.GetBuffer(0), 0);
	CXJUserStore::GetInstance()->Save("c:/tb_sys_user.txt");

	WriteLog("CMainFrame::OnClose start", XJ_LOG_LV3);

	g_Exiting = TRUE;

	//�����ʱָ��
	pApp->ClearAllTempView();

	//�˳��߳�
	WriteLog("CMainFrame::OnClose, ��ʼ�˳������߳�", XJ_LOG_LV3);
	pApp->EndOwnerThread();
	WriteLog("CMainFrame::OnClose, �����������߳̽���", XJ_LOG_LV3);
	
	if( m_pRemindView != NULL )
	{
		m_pRemindView->EndThread();
		WriteLog("CMainFrame::OnClose, �¼�����ҳ���߳̽���", XJ_LOG_LV3);
	}
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_ALL_CLOSE);
		WriteLog("CMainFrame::OnClose, ��������ҳ�����߳̽���", XJ_LOG_LV3);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_ALL_CLOSE);
		WriteLog("CMainFrame::OnClose, ¼��������ҳ�����߳̽���", XJ_LOG_LV3);
	}
	if(pCommFrame)
	{
		pCommFrame->SendMessage(WM_ALL_CLOSE);
		WriteLog("CMainFrame::OnClose, ͨ��״̬ҳ���߳̽���", XJ_LOG_LV3);
	}
	if(pActionFrame)
	{
		pActionFrame->SendMessage(WM_ALL_CLOSE);
		WriteLog("CMainFrame::OnClose, �����¼�ҳ���߳̽���", XJ_LOG_LV3);
	}
	WriteLog("CMainFrame::OnClose, �˳������߳̽���", XJ_LOG_LV3);

	//�Ͽ���ͨ�ŷ���������
	pApp->DisConnectSTTPServer();
	WriteLog("CMainFrame::OnClose, �Ͽ���ͨ�ŷ���������", XJ_LOG_LV3);

	WriteLog("CMainFrame::OnClose, ��ʼж��STTP�ͻ���DLL", XJ_LOG_LV3);
	pApp->m_SttpEngine.FreeSttpLib();
	WriteLog("CMainFrame::OnClose, ��ʼж��DB�ͻ���DLL", XJ_LOG_LV3);
	pApp->m_DBEngine.FreeDBLib();

	WriteLog("CMainFrame::OnClose ��ʼ�رմ���", XJ_LOG_LV3);
	CMDIFrameWnd::OnClose();
	WriteLog("CMainFrame::OnClose �رմ��ڽ���", XJ_LOG_LV3);

	//��������
	pApp->SaveConfig();

	WriteLog("CMainFrame::OnClose end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnStatusInfo()
 ���ܸ�Ҫ���ı�״̬����Ϣ��ʾ
 �� �� ֵ: void
 ��    ����param1	Ҫ�ı��״̬��ID
		   Param2	��Ϣ
**************************************************************/
//##ModelId=49B87B99005E
void CMainFrame::OnStatusInfo( WPARAM wParam, LPARAM lParam )
{
	int nIndex = m_wndStatusBar.CommandToIndex(UINT(wParam));
	LPCTSTR pstr=(LPCTSTR )lParam;
	m_wndStatusBar.SetPaneText(nIndex, pstr, TRUE);
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ����Ӧ��������ҳ��ر���Ϣ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��Ϣ����
**************************************************************/
//##ModelId=49B87B99006E
void CMainFrame::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	//�������Կ��ָ����Ϊ��
	if(pPTFrame != NULL)
	{
//		pPTFrame->DestroyWindow();
		pPTFrame = NULL;
	}
}

/*************************************************************
 �� �� ����OpenSecPropPage()
 ���ܸ�Ҫ���򿪶����豸����ҳ
 �� �� ֵ: void
 ��    ����param1	�����豸ָ��
**************************************************************/
//##ModelId=49B87B970280
void CMainFrame::OpenSecPropPage( CSecObj* pObj, int nPage)
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;

	if(pObj == NULL)
		return;

	if(pObj->m_nType == TYPE_WAVEREC)
	{
		
		//¼����
		if(pWaveRecFrame == NULL)
		{
			pWaveRecFrame = new CWaveRecFrame();
			context.m_pNewViewClass=RUNTIME_CLASS(CPTSheet);
			context.m_pCurrentDoc=pDoc;
			context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtWaveRecSheet]; //pDoc->GetDocTemplate();
			pWaveRecFrame->LoadFrame(IDR_XJBROWTYPE,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
			pWaveRecFrame->InitialUpdateFrame(pDoc,TRUE);
		}
		//֪ͨ��ܶ�Ӧ���豸��ʲô
		if(pWaveRecFrame != NULL)
		{
			pWaveRecFrame->SendMessage(WM_WRFRAME_OPEN, (WPARAM)nPage, LPARAM(pObj));
		}
	}
	else if(pObj->m_nSecType == 21)//���װ��
	{
		if(pDisDevFrame == NULL)
		{
			pDisDevFrame = new CDisDevFrame();
			context.m_pNewViewClass = RUNTIME_CLASS(CPTSheet);
			context.m_pCurrentDoc = pDoc;
			context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtDisDevSheet]; //pDoc->GetDocTemplate();
			pDisDevFrame->LoadFrame(IDR_XJBROWTYPE,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
			pDisDevFrame->InitialUpdateFrame(pDoc,TRUE);
		}
		//֪ͨ��ܶ�Ӧ���豸��ʲô
		if(pDisDevFrame != NULL)
		{
			pDisDevFrame->SendMessage(WM_DISDEVFRAME_OPEN, (WPARAM)nPage, LPARAM(pObj));
		}
	}
	else
	{
		//����
		if(pPTFrame == NULL)
		{
			int start = GetTickCount();
			WriteLog("��ʼ������������ҳ��ͼ", XJ_LOG_LV3);
			pPTFrame = new CPTFrame();
			context.m_pNewViewClass=RUNTIME_CLASS(CPTSheet);
			context.m_pCurrentDoc=pDoc;
			context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtPTSheet]; //pDoc->GetDocTemplate();
			pPTFrame->LoadFrame(IDR_XJBROWTYPE,
				WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
			pPTFrame->InitialUpdateFrame(pDoc,TRUE);
			WriteLog("������������ҳ��ͼ���", XJ_LOG_LV3);
			int interval = GetTickCount() - start;
			CString str;
			str.Format("������������ҳ��ͼ����ʱ��Ϊ%d ms", interval);
			WriteLog(str, XJ_LOG_LV3);
		}

		//֪ͨ��ܶ�Ӧ���豸��ʲô
		if(pPTFrame != NULL)
		{
			int start = GetTickCount();
			WriteLog("��ʼ��ȡ����������Ϣ", XJ_LOG_LV3);
			pPTFrame->SendMessage(WM_PTFRAME_OPEN, (WPARAM)nPage, LPARAM(pObj));
			WriteLog("��ȡ����������Ϣ���", XJ_LOG_LV3);
			int interval = GetTickCount() - start;
			CString str;
			str.Format("��ȡ����������Ϣ����ʱ��Ϊ%d ms", interval);
			WriteLog(str, XJ_LOG_LV3);
			
		}
	}
}

/*************************************************************
 �� �� ����OnWRFrameClose()
 ���ܸ�Ҫ����Ӧ¼��������ҳ��ر�֪ͨ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��Ϣ����
**************************************************************/
//##ModelId=49B87B99008C
void CMainFrame::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	//¼��������ҳ����ΪNULL
	pWaveRecFrame = NULL;
}

//##ModelId=49B87B980002
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����ShowEventProp()
 ���ܸ�Ҫ����ʾ�¼����Դ���
 �� �� ֵ: void
 ��    ����param1	�¼�������ָ��
		   param2	�Ƿ񱣻����Դ���Ҫ���¼����ԶԻ���
**************************************************************/
//##ModelId=49B87B970290
void CMainFrame::ShowEventProp( CXJEventManager* pEventManager, BOOL bPT)
{
	//��֤������Ч��
	if(pEventManager == NULL)
		return;
	if(m_pDlgEventProp == NULL)
	{
		//δ������,����֮
		m_pDlgEventProp = new CDlgEventProp(this);
		m_pDlgEventProp->Create();
	}
	//���öԻ�������
	m_pDlgEventProp->SetFlag(bPT);
	//�����¼����� 
	m_pDlgEventProp->SetEventmanager(pEventManager);
	//��ʾ�Ի���
	m_pDlgEventProp->ShowWindow(SW_SHOW);
}

/*************************************************************
 �� �� ����OnEventPropClose()
 ���ܸ�Ҫ���������Դ��ڹر�
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��Ϣ����
**************************************************************/
//##ModelId=49B87B99009C
void CMainFrame::OnEventPropClose( WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgEventProp != NULL)
	{
		m_pDlgEventProp->DestroyWindow();
		delete m_pDlgEventProp;
		m_pDlgEventProp = NULL;
	}
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ, ת�����¼����ͱ�������ҳ��
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9900AB
void CMainFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//��֤������Ч��
	CXJEvent * pEvent = (CXJEvent*)lParam;
	if(pEvent == NULL)
		return;
	int nType = (int)wParam;
	switch(nType)
	{
	case XJ_EVENT_ACTION: //���������¼�
		if(pActionFrame != NULL)
			pActionFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_ALARM: //�澯�¼�
		if(pAlarmFrame != NULL)
			pAlarmFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_FAULT: //�¹ʱ���
		if(pFaultFrame != NULL)
			pFaultFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_OSC: //¼������
		if(pOSCFrame != NULL)
			pOSCFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_DI: //��������λ
		if(pDIFrame != NULL)
			pDIFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_SYSALARM: //ϵͳ�澯
		if(pSysAlarmFrame != NULL)
			pSysAlarmFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
		break;
	case XJ_EVENT_DISTANCE:
		if(pDistanceFrame != NULL)
			pDistanceFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
	default:
		break;
	}
	//��������ҳ��ֻ���ն����͸澯����Ϣ
	if(pPTFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ACTION: //����
		case XJ_EVENT_ALARM: //�澯
			pPTFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
			break;
		default:
			break;
		}
	}
	if(pDisDevFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ALARM:
			pDisDevFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
			break;
		case XJ_EVENT_DISTANCE:
			pDisDevFrame->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
			break;
		default:
			break;
		}
	}
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ�¼���ȷ���¼���Ϣ, ת�����¼����ԶԻ���ͱ�������ҳ��
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9900BC
void CMainFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�¼����ԶԻ���
	if(m_pDlgEventProp != NULL)
		m_pDlgEventProp->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
	//��������ҳ��ֻ���ն����͸澯����Ϣ
	int nType = (int)wParam;
	if(pPTFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ACTION: //����
			pPTFrame->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
			break;
		case XJ_EVENT_ALARM: //�澯
			pPTFrame->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
			break;
		default:
			break;
		}
	}
	if(pDisDevFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ALARM:
			pDisDevFrame->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
			break;
		case XJ_EVENT_DISTANCE:
			pDisDevFrame->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
			break;
		default:
			break;
		}
	}
}

/*************************************************************
 �� �� ����OnEventPTAck()
 ���ܸ�Ҫ����Ӧ����������ȷ���¼���Ϣ, ת�����¼����ԶԻ�����¼���
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9900CC
void CMainFrame::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//�¼����ԶԻ���
	if(m_pDlgEventProp != NULL)
		m_pDlgEventProp->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
	//�¼���
	int nType = (int)wParam;
	switch(nType)
	{
	case XJ_EVENT_ACTION: //���������¼�
		if(pActionFrame != NULL)
			pActionFrame->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
		break;
	case XJ_EVENT_ALARM: //�澯�¼�
		if(pAlarmFrame != NULL)
			pAlarmFrame->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
		break;
	case XJ_EVENT_DISTANCE:
		if(pDistanceFrame != NULL)
			pDistanceFrame->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
		break;
	default:
		break;
	}
}

/*************************************************************
 �� �� ����OnEventPropAckPT()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ����ɱ�������ҳ���ʱȷ���¼�����Ϣ,ת������������ҳ���¼���
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9900EA
void CMainFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//�¼���
	int nType = (int)wParam;
	switch(nType)
	{
	case XJ_EVENT_ACTION: //���������¼�
		if(pActionFrame != NULL)
			pActionFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
		break;
	case XJ_EVENT_ALARM: //�澯�¼�
		if(pAlarmFrame != NULL)
			pAlarmFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
		break;
	case XJ_EVENT_DISTANCE:
		if(pDistanceFrame != NULL)
			pDistanceFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
		break;
	default:
		break;
	}
	//��������ҳ��ֻ���ն����͸澯����Ϣ
	if(pPTFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ACTION: //����
		case XJ_EVENT_ALARM: //�澯
			pPTFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
			break;
		default:
			break;
		}
	}
	if(pDisDevFrame != NULL)
	{
		switch(nType)
		{
		case XJ_EVENT_ALARM:
			pDisDevFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
			break;
		case XJ_EVENT_DISTANCE:
			pDisDevFrame->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
			break;
		default:
			break;
		}
	}
}

/*************************************************************
 �� �� ����OnEventListPassiveAck()
 ���ܸ�Ҫ����Ӧ�¼�����������ȷ����Ϣ, ���͸��¼�����
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9900FA
void CMainFrame::OnEventListPassiveAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgEventProp != NULL)
	{
		m_pDlgEventProp->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnEventPTPassiveAck()
 ���ܸ�Ҫ����Ӧ��������ҳ�淢������ȷ����Ϣ, ת�����¼����ԶԻ���һ����Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B990109
void CMainFrame::OnEventPTPassiveAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pDlgEventProp != NULL)
	{
		m_pDlgEventProp->PostMessage(WM_EVENTPT_ACK, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnMdiTabRefresh()
 ���ܸ�Ҫ����Ӧ��TAB���µ���Ϣ,����TAB��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990119
void CMainFrame::OnMdiTabRefresh( WPARAM wParam, LPARAM lParam )
{
//	m_wndMDITabs.Update(); // sync the mditabctrl with all views
	CFrameWnd* pFrame = GetActiveFrame();
	if(pFrame == NULL)
		return;
}

//##ModelId=49B87B98001F
void CMainFrame::OnTestToolBatch() 
{
	
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	if(pBatchFrame == NULL)
	{
		pBatchFrame = new CBatchFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CBatchSelectView);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtBatchCall]; //pDoc->GetDocTemplate();
		pBatchFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pBatchFrame->InitialUpdateFrame(pDoc,TRUE);

		//������Ϣ
		//lparam������ʾ�����ִ���, 0-�Զ�����. 1-�����ٻ�
		pBatchFrame->SendMessage(BATCH_FRAME_OPEN, 0, 1);
	}
	if(pBatchFrame != NULL)
	{
		//�����
		pBatchFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

//##ModelId=49B87B990128
void CMainFrame::OnBatchFrameClose( WPARAM wParam, LPARAM lParam )
{
	//lparam������ʾ�����ִ���, 0-�Զ�����. 1-�����ٻ�
	//�����ٻ����ڹر�
	int nFlag = (int)lParam;
	if(nFlag == 1)
	{
		pBatchFrame = NULL;
	}
	if(nFlag == 0)
	{
		pAutoCallFrame = NULL;
	}
}

//##ModelId=49B87B980021
void CMainFrame::OnToolAutocall() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();	
	if (g_bVerify)
	{
		//��֤Ȩ��
		if (g_bVerify)
		{
			if(!pApp->TryEnter(FUNC_XJBROWSER_SETTING))
				return;
		}
	}

	if(m_wndWorkSpaceBar.IsWindowVisible())
	{
		//֮ǰ����ʾ��,��������
		m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,FALSE,FALSE);
	}

	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	if(pAutoCallFrame == NULL)
	{
		CDataEngine* pData = pApp->GetDataEngine();
		pData->QueryHasAnalogConfig();
		pData->QueryHasDigitalConfig();
		pData->QueryHasSettingConfig();
		pData->QueryHasSoftBoardConfig();
		pAutoCallFrame = new CAutoCallFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CBatchSettingView);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtAutoCall]; //pDoc->GetDocTemplate();
		pAutoCallFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pAutoCallFrame->InitialUpdateFrame(pDoc,TRUE);
		
		//������Ϣ
		//lparam������ʾ�����ִ���, 0-�Զ�����. 1-�����ٻ�
		pAutoCallFrame->PostMessage(BATCH_FRAME_OPEN, 0, 0);

	}
	else
	{
		//�����
		pAutoCallFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
	//��¼����
	//pApp->AddManOperator(FUNC_XJBROWSER_SETTING, FUNC_SYS_SETTING);
}

//##ModelId=49B87B9803B9
void CMainFrame::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20026, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20026, wParam, lParam);
	}
}

//##ModelId=49B87B9803C9
void CMainFrame::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20043, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->SendMessage(WM_STTP_20043, wParam, lParam);
	}
	if(pDownOsc != NULL)
	{
		pDownOsc->SendMessage(WM_STTP_20043, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20154()
 ���ܸ�Ҫ���յ������ٻ���Ӧ, ת���������ٻ�����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990001
void CMainFrame::OnSTTP20154( WPARAM wParam, LPARAM lParam )
{
	if(pBatchFrame != NULL)
	{
		pBatchFrame->SendMessage(WM_STTP_20154, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20155()
 ���ܸ�Ҫ����Ӧ20155, �����ٻ���Ϣ�仯�澯֪ͨ, ת����ϵͳ�澯��ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990011
void CMainFrame::OnSTTP20155( WPARAM wParam, LPARAM lParam )
{
	if(pSysAlarmFrame != NULL)
	{
		pSysAlarmFrame->SendMessage(WM_STTP_20155, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20151()
 ���ܸ�Ҫ���յ�20151,�Զ��鵵��Ϣ���仯֪ͨ, ת�������ϱ��洰��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9803D9
void CMainFrame::OnSTTP20151( WPARAM wParam, LPARAM lParam )
{
	if(pFaultFrame != NULL)
	{
		pFaultFrame->SendMessage(WM_STTP_20151, wParam, lParam);
	}
}

//##ModelId=49B87B980023
void CMainFrame::OnToolAutocallResult() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	if(pAutoResultFrame == NULL)
	{
		pAutoResultFrame = new CAutoResultFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CACResultSel);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtAutoCallResult]; //pDoc->GetDocTemplate();
		pAutoResultFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pAutoResultFrame->InitialUpdateFrame(pDoc,TRUE);
		//������Ϣ
		pAutoResultFrame->SendMessage(AUTOCALL_RESULT_OPEN, 0, 0);
	}
	else
	{
		//�����
		pAutoResultFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

/*************************************************************
 �� �� ����OnAutoCallResultClose()
 ���ܸ�Ҫ���Զ����ٽ���鿴���ڹر�, ��ָ����ΪNULL
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990139
void CMainFrame::OnAutoCallResultClose( WPARAM wParam, LPARAM lParam )
{
	pAutoResultFrame = NULL;
}

//##ModelId=49B87B98002F
void CMainFrame::OnViewDevicetree() 
{
	// TODO: Add your command handler code here
	if(m_wndWorkSpaceBar.IsWindowVisible())
	{
		//֮ǰ����ʾ��,��������
		m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,FALSE,FALSE);
	}
	else
	{
		//֮ǰ�����ص�, ������ʾ
		m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,TRUE,FALSE);
	}
}

//##ModelId=49B87B980031
void CMainFrame::OnUpdateViewDevicetree(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_wndWorkSpaceBar.IsWindowVisible());
}

//##ModelId=49B87B98003E
void CMainFrame::OnViewOutput() 
{
	// TODO: Add your command handler code here
	if(m_wndOutputBar.IsWindowVisible())
	{
		//֮ǰ����ʾ��, ��������
		m_wndOutputBar.GetDockingFrame()->ShowControlBar(&m_wndOutputBar,FALSE,FALSE);
	}
	else
	{
		//֮ǰ�����ص�, ������ʾ
		m_wndOutputBar.GetDockingFrame()->ShowControlBar(&m_wndOutputBar,TRUE,FALSE);
	}
}

//##ModelId=49B87B980040
void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_wndOutputBar.IsWindowVisible());
}

/*************************************************************
 �� �� ����UpdateToolBar()
 ���ܸ�Ҫ�����¹���������
 �� �� ֵ: void
 ��    ����param1	������ָ��
		   Param2	����
**************************************************************/
//##ModelId=49B87B97029F
void CMainFrame::UpdateToolBar( CToolBar* tb, CStringArray& arrText )
{
	CRect rect;
	CSize sizeButton,sizeImage;
	CSize m_sizeMax;
	m_sizeMax.cx = 0;
	m_sizeMax.cy = 0;
	//ȡ�����ߴ�
	for (int i=0; i<tb->GetCount();i++)
	{
		tb->SetButtonText(i,arrText.GetAt(i));
		tb->GetItemRect(i,rect);
		m_sizeMax.cx = __max(rect.Size().cx, m_sizeMax.cx);
		m_sizeMax.cy = __max(rect.Size().cy, m_sizeMax.cy);
	}
	//���ð�ť�ߴ�
	sizeButton.cx = m_sizeMax.cx;
	sizeButton.cy = m_sizeMax.cy;
	sizeImage.cx = 32;
	sizeImage.cy = 32;
	tb->SetSizes(sizeButton,sizeImage);
	ShowControlBar(tb,FALSE,FALSE);
	ShowControlBar(tb,TRUE,FALSE);
	tb->RedrawWindow();
}

/*************************************************************
 �� �� ����OnSTTP20116()
 ���ܸ�Ҫ����Ӧ20116(������ʷ�¼�֪ͨ), ת������������ҳ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B980251
void CMainFrame::OnSTTP20116( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_20116, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnRefreshActionView()
 ���ܸ�Ҫ����Ӧǿ��ˢ�¶����¼��б���Ϣ, ת���������¼���ͼ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990149
void CMainFrame::OnRefreshActionView( WPARAM wParam, LPARAM lParam )
{
	if(pActionFrame != NULL)
	{
		pActionFrame->PostMessage(REFRESH_ACTIONVIEW, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnRefreshAlarmView()
 ���ܸ�Ҫ����Ӧǿ��ˢ�¸澯�¼��б���Ϣ, ת�����澯�¼���ͼ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990158
void CMainFrame::OnRefreshAlarmView( WPARAM wParam, LPARAM lParam )
{
	if(pAlarmFrame != NULL)
	{
		pAlarmFrame->PostMessage(REFRESH_ALARMVIEW, wParam, lParam);
	}
}

//##ModelId=49B87B990168
void CMainFrame::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->PostMessage(REFRESH_PTSETTING, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame->PostMessage(REFRESH_PTSETTING, wParam, lParam);
	}
}

//##ModelId=49B87B990178
void CMainFrame::OnRefreshPTDI( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->PostMessage(REFRESH_PTDI, wParam, lParam);
	}
}

//##ModelId=49B87B990186
void CMainFrame::OnRefreshPTSoft( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->PostMessage(REFRESH_PTSOFTBOARD, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����DoHideDeviceTree()
 ���ܸ�Ҫ����ʾ�������豸����
 �� �� ֵ: 
 ��    ����param1
		   Param2	Ϊ0ʱ����, Ϊ1ʱ��ʾ
**************************************************************/
//##ModelId=49B87B990196
void CMainFrame::DoHideDeviceTree( WPARAM wParam, LPARAM lParam )
{
	int nFlag = (int)lParam;
	if(0 == nFlag)
	{
		//Ҫ����
		if(m_wndWorkSpaceBar.IsWindowVisible())
		{
			//����
			m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,FALSE,FALSE);
		}
		else
			return;
	}
	else if(1 == nFlag)
	{
		//Ҫ��ʾ
		if(m_wndWorkSpaceBar.IsWindowVisible())
		{
			//�Ѿ�����ʾ
			return;
		}
		else
			m_wndWorkSpaceBar.GetDockingFrame()->ShowControlBar(&m_wndWorkSpaceBar,TRUE,FALSE);
	}
}

/*************************************************************
 �� �� ����OnAlarmSound()
 ���ܸ�Ҫ����Ӧʹ������������Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9901A5
void CMainFrame::OnAlarmSound( WPARAM wParam, LPARAM lParam )
{
	int nFlag = (int)lParam;
	if(nFlag == 0)
	{
		//��ʼ����
		PlaySound(g_SoundFile, NULL, SND_ASYNC|SND_LOOP);
		
		//ֹͣ��ǰ��ʱ�ӣ����¼�ʱ
		KillTimer(m_nAlarmTimer);
		
		//ȷ������������ʱ��
		UINT uElapse = g_SoundTime*1000;
		if (uElapse != 0)
		m_nAlarmTimer = SetTimer(1, uElapse, NULL);
	}
	else
	{
		//ֹͣ����
		//ֹͣ��ǰ��ʱ�ӣ����¼�ʱ
		KillTimer(m_nAlarmTimer);
		//ֹͣ��������
		PlaySound(NULL, NULL, NULL);
	}
	
}

//##ModelId=49B87B98004E
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nAlarmTimer)
	{
		//ֹͣ��������
		PlaySound(NULL, NULL, NULL);
		
		//ֹͣʱ��
		KillTimer(m_nAlarmTimer);
	}
	if(nIDEvent == m_nCommStatusTimer)
	{
		//ͨѶ״̬��ʱ
		m_nCommDisconnect++;
	}
	if(nIDEvent == m_nIdleTimer)
	{
		LASTINPUTINFO lpi;
		lpi.cbSize = sizeof(lpi);
		GetLastInputInfo(&lpi);
		DWORD nsec = (GetTickCount() - lpi.dwTime)/1000;
		if(g_UILockTime > 0 && nsec > g_UILockTime*60)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->LockUI();
		}
	}
	
	CXJPTSetStore *pStore = CXJPTSetStore::GetInstance();
	QPTSetStateTable *pState = pStore->GetState();
	if (nIDEvent == m_nMsgTimer){
		
		int nPTSetState = pState->GetStateID();
		
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		if (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)
			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_RUNNER)){
			
			int nCurPTSetModState = pState->GetStateID();
			CString sRunnerUserID = pState->GetRunnerUserID().data();
			// ������Ա���߳����û�
			if (pApp->m_User.m_strUSER_ID == sRunnerUserID)
				if (XJ_OPER_PTSET_STATE_3 == nCurPTSetModState){
					KillTimer(m_nMsgTimer);
					DoPtsetVerify0();
					m_oper = 0;
				}else if (XJ_OPER_PTSET_STATE_5 == nCurPTSetModState && 0 == m_oper){
					KillTimer(m_nMsgTimer);
					AfxMessageBox("���ж�ֵ�޸���ִ�гɹ������ڶ�ֵҳ�����ٻ�һ����ȷ���Ƿ���ȷ", MB_OK|MB_ICONINFORMATION);
					m_oper = 1;
					m_nMsgTimer = SetTimer(201, 3*1000, NULL);
				}
		}
	}

	if (nIDEvent == m_nTimer){

		int nPTSetState = pState->GetStateID();

		CString text;
		m_wndGlobalMsgBar.GetWindowText(text);
		QByteArray winText = text.GetBuffer(0);
		QByteArray dynText;
		if (winText.contains(" / "))
			dynText = " -- ";
		else if (winText.contains(" -- "))
			dynText = " \\ ";
		else if (winText.contains(" \\ "))
			dynText = " | ";
		else if (winText.contains(" | "))
			dynText = " / ";
		
		if (dynText.isEmpty())
			dynText = " -- ";
		QByteArray baHangoutReasonType = pState->GetHangoutReasonName();
		if (baHangoutReasonType.isEmpty())
			baHangoutReasonType << "��������";
		baHangoutReasonType << "���Ӵ���";

		if (nPTSetState != XJ_OPER_UNHANGOUT)
			baHangoutReasonType << dynText;
		m_wndGlobalMsgBar.SetWindowText(baHangoutReasonType.constData());
		m_wndGlobalMsgBar.GetParent()->SetWindowText(baHangoutReasonType.constData());
		m_wndGlobalMsgBar.GetParent()->GetParent()->SetWindowText(baHangoutReasonType.constData());
		OnUpdateFrameTitle(TRUE);
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::StartThread()
{
	if (m_pThread == NULL)
	{
		m_bThreadExit = FALSE;
		//������������豸�����߳�
		m_pThread = AfxBeginThread(RefreshPTSetState, this, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pThread != NULL)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

void CMainFrame::EndThread()
{
	if(m_pThread != NULL)
	{
		WriteLog("��ʼ�˳���ֵ�޸�״̬��ˢ���߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		m_bThreadExit = TRUE;	//�����˳���־
		DWORD dw = WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pThread->m_hThread,NULL);
			WriteLog("��ֵ�޸�״̬��ˢ���߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pThread;
		m_pThread = NULL;
		WriteLog("�˳���ֵ�޸�״̬��ˢ���߳����", XJ_LOG_LV3);
	}
}

void CMainFrame::DoPtsetVerify0() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString strOutPut;
	CString str;
	CXJPTSetStore *pPTSetStore = CXJPTSetStore::GetInstance();
	QPTSetStateTable *pPTSetState = pPTSetStore->GetState();

	int nPTSetState = pPTSetState->GetStateID();

	//������Աȷ��
	CDlgCheckPro dlg(this, 0);
	dlg.m_strModify = strOutPut;
	CString sRunUser;
	if(dlg.DoModal() == IDOK)
	{
		if(g_PTControlModel == 1)
		{
			//��ͨģʽ,Ҫ��������Ա��֤
			CDlgValidateID dlgID(0, 1);
			dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
			if(dlgID.DoModal() == IDOK)
			{	
				sRunUser = dlgID.m_strUser;
				pPTSetState->Next_PTSet_State_4(sRunUser.GetBuffer(0));
			}
			else
			{
				// Ȩ�޲���
				str.Format("�û�%s������Ա�����֤ʧ�ܣ���ֵ�޸�������֤ʧ��", pPTSetState->GetRunnerUserID().constData());
				WriteLog(str, XJ_LOG_LV2);
				pPTSetState->SetFlags(1);
				pPTSetState->RevertTo_PTSet_State_1(XJ_OPER_PTSET_STATE_4, pPTSetState->GetRunnerUserID().constData()
						, QByteArray(str.GetBuffer(0)));
			}
		}
	}
	else
	{
		//��ͬ���޸�
		//�ظ��޸�ǰ��ֵ
		str.Format("�û�%s������Ա�����֤ʧ�ܣ���ͬ���޸Ķ�ֵ�޸�", pPTSetState->GetRunnerUserID().constData());
			WriteLog(str, XJ_LOG_LV2);
		pPTSetState->RevertTo_PTSet_State_1(XJ_OPER_PTSET_STATE_4, pPTSetState->GetRunnerUserID().constData()
						, QByteArray(str.GetBuffer(0)));
	}
	
	KillTimer(m_nMsgTimer);
	m_nMsgTimer = SetTimer(201, 3*1000, NULL);
}

/*************************************************************
 �� �� ����OnSTTP00901()
 ���ܸ�Ҫ���յ�00901����, ת������ֵ����ҳ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B99002F
void CMainFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame->SendMessage(WM_STTP_00901, wParam, lParam);
	}
	if(pAutoCallFrame != NULL)
	{
		pAutoCallFrame->SendMessage(WM_STTP_00901, wParam, lParam);
	}
}

//##ModelId=49B87B980051
void CMainFrame::OnToolDownloadFile() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	if(pDownloadFrame == NULL)
	{
//		g_wndWait.Show(SPLASH_NOTIMER);
		pDownloadFrame = new CDownloadFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CDownLoadView);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtDownload]; //pDoc->GetDocTemplate();
		pDownloadFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pDownloadFrame->InitialUpdateFrame(pDoc,TRUE);
		//������Ϣ
		pDownloadFrame->SendMessage(DOWNLOAD_OPEN, 0, 0);
		
// 		if(g_wndWait.GetSafeHwnd() != NULL)
// 			g_wndWait.Hide();
	}
	else
	{
		//�����
		pDownloadFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

/*************************************************************
 �� �� ����OnDownloadClose()
 ���ܸ�Ҫ����Ӧͨ���ļ����ش��ڹر���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9901A9
void CMainFrame::OnDownloadClose( WPARAM wParam, LPARAM lParam )
{
	pDownloadFrame = NULL;
}

/*************************************************************
 �� �� ����OnSTTP00204()
 ���ܸ�Ҫ����Ӧ00204����, ת����ͨ���ļ����ش���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B99003F
void CMainFrame::OnSTTP00204( WPARAM wParam, LPARAM lParam )
{
	if(pDownloadFrame != NULL)
	{
		pDownloadFrame->SendMessage(WM_STTP_00204, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP00212()
 ���ܸ�Ҫ����Ӧ00212����, ת����ͨ���ļ����ش���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B99004F
void CMainFrame::OnSTTP00212( WPARAM wParam, LPARAM lParam )
{
	if(pDownloadFrame != NULL)
	{
		pDownloadFrame->SendMessage(WM_STTP_00212, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ20157����, ת��������Ҫ�ٻ��Ĵ���
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B990021
void CMainFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(pPTFrame != NULL)
	{
		pPTFrame ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(pWaveRecFrame != NULL)
	{
		pWaveRecFrame ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(pDownloadFrame != NULL)
	{
		pDownloadFrame ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(pDownOsc != NULL)
	{
		pDownOsc->SendMessage(WM_STTP_20157, wParam, lParam);
	}
}

//##ModelId=49B87B980053
void CMainFrame::OnToolAutocallStop() 
{
	// TODO: Add your command handler code here
	CDlgAutoCallStop dlg;
	dlg.DoModal();
}

//##ModelId=49B87B980055
void CMainFrame::OnToolFaultreport() 
{
	// TODO: Add your command handler code here
	if(pFaultFrame != NULL)
		pFaultFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B98005E
void CMainFrame::OnToolMainsvg() 
{
	// TODO: Add your command handler code here
	if(g_bShowDeviceMapWnd && pMapFrame != NULL)
		pMapFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	else if(!g_bShowDeviceMapWnd && pStationInfoFrame != NULL)
		pStationInfoFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B980060
void CMainFrame::OnToolPtcomm() 
{
	// TODO: Add your command handler code here
	if(pPTCommFrame != NULL)
		pPTCommFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B980062
void CMainFrame::OnToolStationcomm() 
{
	// TODO: Add your command handler code here
	if(pCommFrame != NULL)
		pCommFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B980064
void CMainFrame::OnUpdateToolFaultreport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if(pFaultFrame == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(GetActiveFrame() != pFaultFrame);
	*/
	pCmdUI->Enable(g_bShowFlautWnd);
	if (GetActiveFrame() == pFaultFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

//##ModelId=49B87B98006F
void CMainFrame::OnUpdateToolMainsvg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if(pMapFrame == NULL)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(GetActiveFrame() != pMapFrame);
	}
	*/
	pCmdUI->Enable(TRUE);
	if(g_bShowDeviceMapWnd)
	{
		if (GetActiveFrame() == pMapFrame)
		{
			pCmdUI->SetCheck(TRUE);
		}
		else
		{
			pCmdUI->SetCheck(FALSE);
		}
	}
	else
	{
		if (GetActiveFrame() == pStationInfoFrame)
		{
			pCmdUI->SetCheck(TRUE);
		}
		else
		{
			pCmdUI->SetCheck(FALSE);
		}
	}
	
	
//	pCmdUI->Enable(TRUE);
}

//##ModelId=49B87B98007D
void CMainFrame::OnUpdateToolPtcomm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if(pPTCommFrame == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(GetActiveFrame() != pPTCommFrame);
	*/
	pCmdUI->Enable(g_bShowPTCommWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pPTCommFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

//##ModelId=49B87B980080
void CMainFrame::OnUpdateToolStationcomm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(GetActiveFrame() != pCommFrame);
	pCmdUI->Enable(TRUE);
	//pCmdUI->SetCheck(GetActiveFrame() == pCommFrame);
	if (GetActiveFrame() == pCommFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ������ͼǿ��ˢ����Ϣ, ת����������ͼ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CMainFrame::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	if( lParam == 0 ){
		//�ǳ�վ���ҳ��,�Ͽ�ʱ��ﵽҪ��ʱˢ��
		if(pActionFrame	!= NULL)
		{
			//����
			pActionFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pAlarmFrame != NULL)
		{
			//�澯
			pAlarmFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pDIFrame	!= NULL)
		{
			//������
			pDIFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pFaultFrame	!= NULL)
		{
			//���ϱ���
			pFaultFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pOSCFrame != NULL)
		{
			//¼��
			pOSCFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pPTCommFrame	!= NULL)
		{
			//����ͨѶ״̬
			pPTCommFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pPTRunFrame	!= NULL)
		{
			//��������״̬
			pPTRunFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pSysAlarmFrame != NULL)
		{
			//ϵͳ�澯
			pSysAlarmFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pSOEDebugFrame != NULL)
		{
			//������Ϣ
			pSOEDebugFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pSOEIFrame != NULL)
		{
			//I����Ϣ
			pSOEIFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pSOEIIFrame != NULL)
		{
			//II����Ϣ
			pSOEIIFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pSOEIIIFrame != NULL)
		{
			//III����Ϣ
			pSOEIIIFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
		if(pDistanceFrame != NULL)
		{
			pDistanceFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
		}
	}
	//��վ���ҳ��,�κ�ʱ��ˢ��
	if(pCommFrame != NULL)
	{
		//����վͨѶ״̬
		pCommFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
	if(m_pDeviceView != NULL ){
		m_pDeviceView->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
	if(pStaInfoFrame != NULL ){
		pStaInfoFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
	/*
	if(pMapFrame != NULL)
	{
		//����ͼ
		pMapFrame->SendMessage(VIEW_REFRESH, wParam, lParam);
	}
	*/
	
}

/*************************************************************
 �� �� ��: OnCommStatusChange()
 ���ܸ�Ҫ: ��ӦͨѶ״̬�ı���Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CMainFrame::OnCommStatusChange( WPARAM wParam, LPARAM lParam )
{
	int nStatus = (int)lParam;
	if(0 == nStatus)
	{
		//ͨѶ����, ֹͣ��ʱ
		m_nCommDisconnect = 0;
		KillTimer(m_nCommStatusTimer);
	}
	if(1 == nStatus)
	{
		//ͨѶ�Ͽ�, ��ʼ��ʱ
		m_nCommDisconnect = 0;
		m_nCommStatusTimer = SetTimer(2, 1000, 0);
	}
}

/*************************************************************
 �� �� ��: GetCommDisconnectTime()
 ���ܸ�Ҫ: ȡ��ϵͳͨѶ�Ͽ�ʱ��
 �� �� ֵ: ͨѶ�Ͽ�ʱ��
 ��    ��: param1 
		   Param2 
**************************************************************/
int CMainFrame::GetCommDisconnectTime()
{
	return m_nCommDisconnect;
}

/*************************************************************
 �� �� ��: OnSTTP20159()
 ���ܸ�Ҫ: �յ�Ԥ���ṹ֪ͨ����, ת����Ԥ����ͼ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CMainFrame::OnSTTP20159( WPARAM wParam, LPARAM lParam )
{
	if(pCrossFrame != NULL)
	{
		pCrossFrame->SendMessage(WM_STTP_20159, wParam, lParam);
	}
}

void CMainFrame::OnToolQueryevent() 
{
	// TODO: Add your command handler code here
	OpenXJQuery("", this->GetSafeHwnd());
}

/*************************************************************
 �� �� ��: ShowStationInfo()
 ���ܸ�Ҫ: ��ָ����վ�ĳ�վ��Ϣ��ͼ
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 
**************************************************************/
void CMainFrame::ShowStationInfo( CStationObj* pStation )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	
	if(pStation == NULL)
		return;
	
//	g_wndWait.Show(SPLASH_NOTIMER);
	//����
	if(pStationInfoFrame == NULL)
	{
		WriteLog("��ʼ������վ��Ϣ��ͼ", XJ_LOG_LV3);
		pStationInfoFrame = new CStationInfoFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CViewStationInfo);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtStationInfo]; //pDoc->GetDocTemplate();
		pStationInfoFrame->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pStationInfoFrame->InitialUpdateFrame(pDoc,TRUE);
		WriteLog("������վ��Ϣ��ͼ���", XJ_LOG_LV3);
		//��¼��ϸ��ͼ��ָ��
		if(pStationInfoFrame != NULL)
			pApp->m_pStationInfo = (CViewStationInfo*)pStationInfoFrame->GetActiveView();
	}
	
	//֪ͨ��ܶ�Ӧ�ĳ�վ��ʲô
	if(pStationInfoFrame != NULL)
	{
		pStationInfoFrame->SendMessage(STATIONINFO_FRAME_OPEN, 0, LPARAM(pStation));
	}
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();
}

void CMainFrame::OnViewStationInfoClose( WPARAM wParam, LPARAM lParam )
{
	pStationInfoFrame = NULL;	
}

void CMainFrame::OnToolQueryalarm() 
{
	// TODO: Add your command handler code here
	OpenXJAlarmQuery("", this->GetSafeHwnd());
}

/*************************************************************
 �� �� ��: OnSTTP20084()
 ���ܸ�Ҫ: ��Ӧ20084���ģ�¼����ͨѶ״̬����֪ͨ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CMainFrame::OnSTTP20084( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pSec = (CSecObj*)lParam;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	//֪ͨ����ͨѶ״̬ҳ��
	if(pPTCommFrame != NULL)
	{
		//lParam ��дԭ����
		//wParam ��д�豸ָ��
		pPTCommFrame ->SendMessage(WM_STTP_20002, (WPARAM)pSec, lParam);
	}
	if(pStationInfoFrame != NULL)
	{
		pStationInfoFrame->SendMessage(WM_STTP_20002, (WPARAM)pSec, lParam);
	}
	//�ı�ͼ����ɫ��ʾ
	CXJBrowserView * pView = pApp->GetSVGView();
	if(pView != NULL)
	{
		if(pView->m_sStationID == pSec->m_sStationID)
		{
			pView->SetDeviceCol(pSec);
		}
	}
	//ȡ�ó�վID
	CStationObj* pStation = (CStationObj*)pData->FindDevice(pSec->m_sStationID, TYPE_STATION);
	//֪ͨ�豸������
	if(m_pDeviceView != NULL && pStation)
	{
		m_pDeviceView->SendMessage(STATION_INFO_CHANGED, 0, (LPARAM)pStation);
	}
	//֪ͨ��վͨѶ״̬ҳ��
	if(pCommFrame != NULL && pStation)
	{
		pCommFrame->SendMessage(STATION_INFO_CHANGED, 0, (LPARAM)pStation);
	}
}

/*************************************************************
 �� �� ��: ShowDownLoadOsc()
 ���ܸ�Ҫ: ��¼������ҳ�棬������ָ��¼��
 �� �� ֵ: 
 ��    ��: param1 ָ��Ҫ���ص�¼��
		   Param2 
**************************************************************/
void CMainFrame::ShowDownLoadOsc( CXJEventOSC* pOsc )
{
	if(pOsc == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc* pDoc = pApp->GetCurDocument();
	CCreateContext context;
	
	//g_wndWait.Show(SPLASH_NOTIMER);
	//
	if(pDownOsc == NULL)
	{
		WriteLog("��ʼ����¼��������ͼ", XJ_LOG_LV3);
		pDownOsc = new CDownOscFrame();
		context.m_pNewViewClass=RUNTIME_CLASS(CDownOscView);
		context.m_pCurrentDoc=pDoc;
		context.m_pNewDocTemplate=pApp->m_pDocTempl[CXJBrowserApp::wtDownOsc]; //pDoc->GetDocTemplate();
		pDownOsc->LoadFrame(IDR_XJBROWTYPE,
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, this, &context);
		pDownOsc->InitialUpdateFrame(pDoc,TRUE);
		WriteLog("����¼��������ͼ���", XJ_LOG_LV3);
		//��¼��ϸ��ͼ��ָ��
	}
	
	//֪ͨ��ܶ�Ӧ�ĳ�վ��ʲô
	if(pDownOsc != NULL)
	{
		pDownOsc->SendMessage(DOWNOSC_FRAME_OPEN, 0, LPARAM(pOsc));
	}
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();
}

void CMainFrame::OnDownloadOscClose( WPARAM wParam, LPARAM lParam )
{
	pDownOsc = NULL;	
}

void CMainFrame::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	//ת������վͨѶҳ��
	if(pCommFrame != NULL)
		pCommFrame->SendMessage(WM_STTP_20146, wParam, lParam);
	if(m_pDeviceView != NULL)
	{
		m_pDeviceView->SendMessage(WM_STTP_20146, wParam, lParam);
	}
}

void CMainFrame::OnUpdateToolQueryalarm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_role == MODE_SUB)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateToolAutocall(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_role == MODE_SUB)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateToolAutocallResult(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_role == MODE_SUB)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateToolAutocallStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_role == MODE_SUB)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateToolQueryevent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if (g_role)
	{
		pCmdUI->Enable(FALSE);
	}
	*/
}

void CMainFrame::OnUpdateToolBatch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if (g_role)
	{
		pCmdUI->Enable(FALSE);
	}
	*/
	if (GetActiveFrame() == pAutoCallFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateToolDownloadFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_role == MODE_SUB)
	{
		pCmdUI->Enable(FALSE);
	}
	if (GetActiveFrame() == pDownloadFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


LRESULT CMainFrame::OnPaintMyCaption(WPARAM bActive, LPARAM lParam)
{
	m_capp.PaintMyCaption(bActive,lParam,m_strTitle);
	return 0;
}


void CMainFrame::OnUpdatebk() 
{
	// TODO: Add your command handler code here
//	m_wndToolBar.UpdateBk(IDB_TOOL1_NOR ,RGB(255, 0, 255) , IDB_BMP_LOGO);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->XJSetMenuInfo();
	CCJMDIFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnToolPTStat()
{
	if(pPTRunFrame != NULL)
		pPTRunFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

void CMainFrame::OnToolDI()
{
	if(pDIFrame != NULL)
	{
		pDIFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnToolActionEvent()
{
	if(pActionFrame != NULL)
	{
		pActionFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnToolAlarmEvent()
{
	if(pAlarmFrame != NULL)
	{
		pAlarmFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnToolOsc()
{
	if(pOSCFrame != NULL)
	{
		pOSCFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnUpdateToolPTStat(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pPTRunFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateToolDI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_bShowDIWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pDIFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateToolActionEvent(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_bShowActionWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pActionFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateToolAlarmEvent(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_bShowAlarmWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pAlarmFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateToolOsc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_bShowOSCWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pOSCFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnToolSysalarm() 
{
	// TODO: Add your command handler code here
	if(pSysAlarmFrame != NULL)
	{
		pSysAlarmFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnUpdateToolSysalarm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pSysAlarmFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnToolGeo() 
{
	// TODO: Add your command handler code here
	if(pGeoFrame != NULL)
	{
		pGeoFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnUpdateToolGeo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_bShowGeoWnd);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pGeoFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnViewProp() 
{
	// TODO: Add your command handler code here
	if(pPTFrame != NULL)
	{
		//pPTFrame->ShowWindow(SW_SHOW);
		pPTFrame->ActivateFrame(SW_SHOW);
	}
}

void CMainFrame::OnUpdateViewProp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(pPTFrame == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	if(pAutoCallFrame != NULL)
	{
		pAutoCallFrame->PostMessage(BATCHCALL_END);
	}
}

/****************************************************
Date:2011/12/5  Author:LYH
������:   OnStartLock	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParma	
*****************************************************/
void CMainFrame::OnStartLock( WPARAM wParam, LPARAM lParma )
{
	m_nIdleTimer = SetTimer(3, 1000, 0);
}

/****************************************************
Date:2012/5/23  Author:LYH
������:   OnStationStatusChanged	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CMainFrame::OnStationStatusChanged( WPARAM wParam, LPARAM lParam )
{
	//��վͨѶ״̬�仯����֪ͨ, ת��������վͨѶ״̬����
	if(pCommFrame != NULL)
	{
		pCommFrame ->SendMessage(STATION_CHANGED, wParam, lParam);
	}
	//֪ͨ�豸������
	
	if(m_pDeviceView != NULL && lParam != NULL)
	{
	//	m_pDeviceView->PostMessage(STATION_INFO_CHANGED, wParam, lParam);
		m_pDeviceView->SendMessage(STATION_CHANGED, wParam, lParam);
	}
	if(pMapFrame != NULL)
	{
		pMapFrame->SendMessage(STATION_CHANGED, wParam, lParam);
	}
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CGeoView* pGeoView = pApp->GetGeoView();
	if(pGeoView != NULL && lParam != NULL)
			pGeoView->RefreshObj((CStationObj*)lParam);
}

void CMainFrame::OnUpdateToolStationinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	//pCmdUI->SetCheck(GetActiveFrame() == pPTCommFrame);
	if (GetActiveFrame() == pStaInfoFrame)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnToolStationinfo() 
{
	// TODO: Add your command handler code here
	if(pStaInfoFrame != NULL)
	{
		pStaInfoFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

int CMainFrame::StationInit( CString sStationID, int nInitType )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return 0;

	g_wndWait.m_bDrawDetail = TRUE;
	g_wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	g_wndWait.Show(SPLASH_NOTIMER);

	CString sDetail;
	sDetail.Format("%s[%s]....", StringFromID(IDS_REINIT_STATION),sStationID);
	WriteLog(sDetail, XJ_LOG_LV3);
	g_wndWait.AddDetailString(sDetail);

	//��ͣ�̺߳ͼ�ʱ��
	g_bSuspend = TRUE;

	CStationObj* pStation = pData->StationInit(sStationID, nInitType);
	

	if(pPTFrame != NULL && pPTFrame->m_pCurSec != NULL && pStation != NULL)
	{
		if(pPTFrame->m_pCurSec->m_sStationID == pStation->m_sID)
		{
			g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_PTPROP));
			pPTFrame->DestroyWindow();
			pPTFrame = NULL;
		}
	}
	if(pWaveRecFrame != NULL && pWaveRecFrame->m_pCurSec != NULL && pStation != NULL)
	{
		if(pWaveRecFrame->m_pCurSec->m_sStationID == pStation->m_sID)
		{
			g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_WRPROP));
			pWaveRecFrame->DestroyWindow();
			pWaveRecFrame = NULL;
		}
	}
	if(pDownloadFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_COMMONFILE));
		pDownloadFrame->DestroyWindow();
		pDownloadFrame = NULL;
	}
	if(pBatchFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_BATCHCALL));
		pBatchFrame->DestroyWindow();
		pBatchFrame = NULL;
	}
	if(pAutoCallFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_AUTOCALLCONF));
		pAutoCallFrame->DestroyWindow();
		pAutoCallFrame = NULL;
	}
	if(pAutoResultFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_REINIT_CLOSE_AUTOCALLRESULT));
		pAutoResultFrame->DestroyWindow();
		pAutoResultFrame = NULL;
	}
	if(pDownOsc != NULL)
	{
		pDownOsc->DestroyWindow();
		pDownOsc = NULL;
	}
	if(pCrossFrame != NULL)
	{
		pCrossFrame->DestroyWindow();
		pCrossFrame = NULL;
	}

	if(pCommFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_STATIONCOMMU));
		//����վͨѶ״̬
		pCommFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	
	if(pMapFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_STATIONMAP));
		pMapFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	
	
	if(pStationInfoFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_STATIONDEV));
		pStationInfoFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(m_pDeviceView != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_DEVTREE));
		m_pDeviceView->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pActionFrame	!= NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_ACTION));
		//����
		pActionFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pAlarmFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_ALARM));
		//�澯
		pAlarmFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pDIFrame	!= NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_DIGITAL));
		//������
		pDIFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pFaultFrame	!= NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_FAULTREPORT));
		//���ϱ���
		pFaultFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pOSCFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_OSC));
		//¼��
		pOSCFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pPTCommFrame	!= NULL)
	{
		g_wndWait.AddDetailString(StringFromID(IDS_RELOAD_DEVCOMMU));
		//����ͨѶ״̬
		pPTCommFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pPTRunFrame	!= NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_DEVRUNSTATUS));
		//��������״̬
		pPTRunFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pSysAlarmFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_SYSALARM));
		//ϵͳ�澯
		pSysAlarmFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pSOEDebugFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_TESTING));
		//������Ϣ
		pSOEDebugFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pSOEIFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_EVENTI));
		//I����Ϣ
		pSOEIFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pSOEIIFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_EVENTI));
		//II����Ϣ
		pSOEIIFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	if(pSOEIIIFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_EVENTI));
		//III����Ϣ
		pSOEIIIFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	//��վ��Ϣ
	if(pStaInfoFrame != NULL)
	{
		g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_STATIONINFO));
		pStaInfoFrame->SendMessage(STATIONINIT, (WPARAM)pStation, (LPARAM)nInitType);
	}
	sDetail.Format("��վ[%s]��ʼ�����,��������������", pStation==NULL?"δ֪��վ":pStation->m_sName);
	WriteLog(sDetail, XJ_LOG_LV1);
	if(nInitType == STATIONINIT_DELETE && pStation != NULL)
	{
		delete pStation;
		pStation = NULL;
	}

	g_bSuspend = FALSE;

	g_wndWait.AddDetailString( StringFromID(IDS_RELOAD_COMPLETED));
	g_wndWait.Hide();
	return 1;
}

/****************************************************
Date:2013/6/7  Author:LYH
������:   OnSTTP20176	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CMainFrame::OnSTTP20176( WPARAM wParam, LPARAM lParam )
{
	if(pSysAlarmFrame != NULL)
		pSysAlarmFrame->SendMessage(WM_STTP_20176, wParam, lParam);
}

void CMainFrame::OnToolDistance() 
{
	// TODO: Add your command handler code here
	if(pDistanceFrame != NULL)
	{
		pDistanceFrame->ActivateFrame(SW_SHOWMAXIMIZED);
	}
}

void CMainFrame::OnUpdateToolDistance(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	if(GetActiveFrame() == pDistanceFrame)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   OnSTTP20168	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CMainFrame::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{

	if(pDistanceFrame != NULL)
		pDistanceFrame->SendMessage(WM_STTP_20168, wParam, lParam);
	if(pDisDevFrame != NULL)
		pDisDevFrame->SendMessage(WM_STTP_20168, wParam, lParam);
}

void CMainFrame::OnSTTP20177( WPARAM wParam, LPARAM lParam )
{
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20177)
		return;
	
	//�õ���Ӧװ��
	CString strID = pSttpData->sttp_body.ch_station_id;
	int nflag = pSttpData->sttp_body.nFlag;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	if(pApp->MgrStation(strID))
		StationInit(strID, nflag);

	//�ظ���Ϣ
	ReportStationInitResult(strID, nflag, 0);
}

BOOL CMainFrame::ReportStationInitResult( CString sStationID, int nInitType, int nResult )
{
	CXJBrowserApp * pApp = (CXJBrowserApp *)AfxGetApp();
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		return FALSE;
	}
	
	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	//��֯20178����,����վ����
	if(!pApp->m_SttpEngine.BuildDataFor20178StationInit(sttpData, sStationID, nInitType, nResult))
	{
		//��֯����ʧ��
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		return FALSE;
	}
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		//ʧ��
		if(nReturn == -1)
		{
			CString str;
			str.Format("%s����20178����ʧ��, ԭ��Ϊ�����ж�", sStationID);
			WriteLog(str, XJ_LOG_LV3);
			return FALSE;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("%s����20178����ʧ��,ԭ��Ϊ��������", sStationID);
			WriteLog(str, XJ_LOG_LV3);
			return FALSE;
		}
	}
	else
	{
		CString str;
		str.Format("%s����20178����ʧ��,ԭ��Ϊ��������", sStationID);
		WriteLog(str, XJ_LOG_LV3);
	}
	return TRUE;
}

/****************************************************
Date:2013/10/8  Author:LYH
������:   OnTestStationStatus	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CMainFrame::OnTestStationStatus( WPARAM wParam, LPARAM lParam )
{
	if(pCommFrame)
	{
		pCommFrame->PostMessage(TEST_STATIONSTATUS, wParam, lParam);
	}
}

/****************************************************
Date:2013/10/10  Author:LYH
������:   OnAddRemind	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CMainFrame::OnAddRemind( WPARAM wParam, LPARAM lParam )
{
	if(m_pRemindView)
		m_pRemindView->SendMessage(ADD_REMIND, wParam, lParam);
}

void CMainFrame::OnPushTopView( WPARAM wParam, LPARAM lParam )
{
	switch (lParam)
	{
	case FVIEW_ACTION:
		if(pActionFrame != NULL)
		{
			pActionFrame->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		break;
	case FVIEW_ALARM:
		if(pAlarmFrame != NULL)
		{
			pAlarmFrame->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		break;
	case FVIEW_OSC:
		if(pOSCFrame != NULL)
		{
			pOSCFrame->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		break;
	case FVIEW_FAULT:
		if(pFaultFrame != NULL)
		{
			pFaultFrame->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		break;
	}
}

void CMainFrame::OnDisDevFrameClose( WPARAM wParam, LPARAM lParam )
{
	pDisDevFrame = NULL;
}
