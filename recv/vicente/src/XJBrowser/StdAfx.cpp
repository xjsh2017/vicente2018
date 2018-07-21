// stdafx.cpp : source file that includes just the standard includes
//	XJBrowser.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "resource.h"

COXSplashWnd g_wndWait;
UINT g_dwESDataFormat;

#ifdef _DEBUG
BOOL g_bLoginVerify = 1;
BOOL g_bConnectCommServer = 1;
BOOL g_bVerify = 0;
#else
BOOL g_bLoginVerify = 1;
BOOL g_bConnectCommServer = 1;
BOOL g_bVerify = 1;
#endif

BOOL g_Exiting = FALSE;


DWORD	g_PTThreadExitTime = INFINITE;

BOOL g_bSuspend = FALSE;

int g_OperTableType = 0;

int			g_DetailWndHeight = 200;

int g_RemindMaxCount = 0; //�����ʾ��
int g_RemindTime = 5;

/** @brief			��վͨ��ͼ������. XXXX.svg*/
CString g_strStationSvg = "stationcomm.svg";
CString g_strOscName = "STWave.exe";
CString g_strStyleName = "";
CString g_StyleConfig = "StyleConfig.xml";
CString g_strToolBarBK = "";
CString g_strStartBmp = "";
BOOL    g_bExit = 0;
int     g_hook = 0;

CString	CANCEL_CALL = "";
CString CANCEL_NULL = "";
CString STTP_DISCONNECT = "";

int g_ConnectServerButton = 1;
int g_DeviceTreeButton = 1;
int g_LogButton = 1;
int g_HistoryQueryButton = 1;
int g_AlarmCountButton = 1;
int g_SystemSettingButton = 1;
int g_CallAllButton = 1;
int g_CommonFileButton = 1;
int g_ExitButton = 1;


int     g_DeviceStatButton = 1;
int     g_DIButton = 1;
int     g_ActionEventButton = 1;
int     g_AlarmEventButton = 1;
int     g_OscEventButton = 1;

int		g_GeoButton = 1;

int	    g_Temp = 0;
int     g_style = 0;
int     g_bSend20162 = 0;
int     g_role = 0;                 //��������ӽ�ɫ 0:��վ 1:��վ
int     g_FindDeviceWay = 0;        //���SVGͼ�����豸ʱ�����豸��ʽ 0:103��� 1:uri
int		g_ShowOutputWnd		= 1;	//�Ƿ���ʾ�������
int		g_ShowDeviceTreeWnd = 1;	//�Ƿ���ʾ�豸������
int		g_DeviceTreeType	= 2;	//�豸������
int		g_PutOutToLogFile	= 1;
int		g_OutPutToWnd		= 1;
int		g_LogLevel			= 2;
int     g_LoadSttpDll       = 0;   //��̬���ѡ��  0:SttpNet.Dll 1:SttpProxy
CString	g_LocalPath			= "";
CString	g_UIConfig			= "";
CString g_LogPath			= "";
CString	g_SysConfig			= "";
CString	g_SoundFile			= "";
CString g_DefaultStation	= "";
CString g_ImageFile			= "";
int		g_TabPosition		= 0;		//tab��λ��. 0-�·�, 1-�Ϸ�
int		g_ListItemHeight	= 18;		//�б��и�
int		g_AutoCallMaxSaveNo = 10;
int		g_DisconnectTime	= 10;	//ͨѶ�Ͽ��೤ʱ����������Ҫ���¶���(��λ:��)
COLORREF	g_ListSpaceColor = RGB(150, 237, 239);	//�б�����ɫ
COLORREF    g_ListSpaceColor2 = RGB(231,229,236);
COLORREF    g_ListSpaceColor3 = RGB(222,255,255);
COLORREF    g_ListSpaceColor4 = RGB(0,0,0);
COLORREF    g_ListHeadColor = RGB(0,0,0);
COLORREF    g_TreeBkColor = RGB(0,0,0);
COLORREF    g_TreeTextColor = RGB(0,0,0);
COLORREF    g_TitleLightColor =  RGB(0,0,0);
COLORREF    g_TitleDeepColor = RGB(0,0,0);
int			g_UseColorFaultReport = 1;
int			g_UseLandscapeMultiStation = 1;
int			g_FaultReportFilter = 1;
CString		g_WorkStatusFileName = "status.log";//��վ����״̬�ļ�,����׺
int			g_LoadDataModel = 0;
int			g_ThreadTime = 10; 
int			g_SecType = -1;//��ȡ�Ķ����豸������.Ϊ-1�Ƕ�ȡ��������ֵ>0���豸.���ڵ���0ʱֻ��ָ�����͵��豸
CString		g_sAppName="";//������
int			g_PostTime = 1000;	//�������ͼ��
int			g_UseLocalIp = 0;//�Ƿ�ʹ�ñ������õ�IP, 0��1�ǡ�Ϊ0ʱ�����ݿ��ж�ȡ����,Ĭ��Ϊ0 
CString		g_LocalIp = "";//�������õ�IP
int			g_LockTime = 5;//��¼����ʱ��,����
CTime		g_LastFailTime = CTime::GetCurrentTime();//���һ��ʧ�ܵ�ʱ��
int			g_LoginFailTimes = 0;//��¼ʧ�ܴ���
int			g_UILockTime = 1;//�����ò������ᱻ����.����
int			g_UseXJWave = 1;
int			g_MarkDownOscPT = 0;
int			g_AutoStationCommMap = 1;
CString		g_Language = "zh_CN";

//���ϱ���
int			g_FaultActionTime = 0;//���ϱ����м��㶯�����ʱ�䷽����0Ϊ���������ź�ʱ�����һ���������ź�ʱ�䣬1Ϊ�������ź�ʱ�������ʱ��
int			g_FaultFilterIII = 0;//���ϱ����Ƿ����III����Ϣ.0��1��
int			g_FaultShowWave = 0;  //¼���������Ƿ���ʾ��0-��1-��
//event
COLORREF g_colEventI		= RGB(255,0,0);
COLORREF g_colEventII		= RGB(0, 255, 255);
COLORREF g_colEventIII		= RGB(214, 223, 247);
COLORREF g_colNotAcked		= RGB(0, 0, 255);
COLORREF g_colAcked			= RGB(0, 0, 0);
int			g_ShowEventI = 1;		//I���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
int			g_AlarmEventI = 1;		//I���¼��Ƿ񱨾�. 0-������. 1-����
int			g_ShowEventII = 1;		//II���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
int			g_AlarmEventII = 1;		//II���¼��Ƿ񱨾�. 0-������. 1-����
int			g_ShowEventIII = 1;		//III���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
int			g_AlarmEventIII = 1;	//III���¼��Ƿ񱨾�. 0-������. 1-����
//ͨѶ״̬״̬��Ӧ��ɫ
COLORREF XJ_STATUS_COMM_ON	= RGB(0, 111, 0);		//��Ѷ����,��ɫ
COLORREF XJ_STATUS_COMM_OFF	= RGB(225, 0, 0);		//ͨѶ�Ͽ�,��ɫ
COLORREF XJ_STATUS_COMM_PART = RGB(220,220,0);

//����״̬��Ӧ��ɫ
COLORREF XJ_STATUS_RUN_ON		= RGB(0, 111, 0);		//����, ��ɫ
COLORREF XJ_STATUS_RUN_CHECK	= RGB(240, 240, 0);		//����/����, ��ɫ
COLORREF XJ_STATUS_RUN_OFF		= RGB(162, 230, 165);	//ͣ��, ����
COLORREF XJ_STATUS_RUN_DEBUG	= RGB(157, 0, 183);		//����, ��ɫ
COLORREF XJ_STATUS_RUN_CODE		= RGB(121, 121, 121);		//�����, ��ɫ

//����
int		g_BlinkDevice		= 1;
int		g_MustShowDevice	= 1;
int		g_BlinkOwner		= 1;
int		g_ShowEventDlg		= 0;		//�յ�װ�õĶ�����澯ʱ, �Ƿ񵯳��¼����Կ�
int		g_UseAlarmSound = 0;	//�Ƿ�ʹ����������. 0-��,1-��
int		g_SoundTime = 60;		//������������ʱ��. >=0����, ��λ:��
int     g_FlashTime = 1200;
int		g_BlinkStation		= 1;
int		g_AutoChangeMap = 0;

//����վͨ��״̬ҳ��
int		g_RefreshStationTime = 10;	//��վ״̬����ʱ��ˢ�����ڵ�λ(����)
int		g_ShowCommON	= 1;		//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
int		g_ShowCommOFF	= 1;		//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
int     g_QueryDay  = 10;

int			g_ShowDevStatusInStation = 1;

int			g_DoubleChannel = 0;			//��վ�Ƿ�ʹ��˫ͨ��ͨ��
COLORREF	g_SCOnly0OFF = RGB(255, 140, 0);	//ֻ��0��ͨ���Ͽ�ʱ����ɫ,���ɫ
COLORREF	g_SCOnly1OFF = RGB(245, 222, 131);//ֻ��1��ͨ���Ͽ�ʱ����ɫ,ǳ��ɫ
COLORREF	g_SCAllOFF = RGB(255, 0, 0);//����ͨ��ȫ�Ͽ�ʱ����ɫ,ɫ��ɫ

COLORREF		g_CommMapBK = RGB(0, 79, 79); //����վͨѶ״̬ͼ�α���ɫ
CString		g_CommMapFont = "";//����վͨѶ״��ͼ�г�վ������������
int			g_CommMapFontSize = 18;//����վͨѶ״��ͼ�г�վ���������С
COLORREF		g_CommMapFontColor = RGB(0,0, 0);//����վͨѶ״��ͼ�г�վ����������ɫ

//����ͨ��״̬ҳ��
int			g_RefreshPTCommTime = 10;	//ˢ�±���ͨ��״̬����ʱ��ļ��ʱ��,��λ(����)
int			g_ShowPTCommON	= 1;		//�б����Ƿ���ʾͨ��״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
int			g_ShowPTCommOFF	= 1;		//�б����Ƿ���ʾͨ��״̬�Ͽ���װ����Ϣ.0-����ʾ. 1-��ʾ

//��������״̬ҳ��
int			g_RefreshPTRunTime = 10;	//ˢ�±�������״̬����ʱ��ļ��ʱ��,��λ(����)
int			g_ShowPTRunON = 1;			//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
int			g_ShowPTRunCheck = 1;		//�б����Ƿ���ʾ����״̬���޵�װ����Ϣ.0-����ʾ. 1-��ʾ
int			g_ShowPTRunOFF = 1;			//�б����Ƿ���ʾ����״̬ͣ�˵�װ����Ϣ.0-����ʾ. 1-��ʾ
int			g_ShowPTRunDebug = 1;		//�б����Ƿ���ʾ����״̬���Ե�װ����Ϣ.0-����ʾ. 1-��ʾ
int			g_ShowPTRunCode	= 1;		//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ

//��վ��Ϣҳ��
int			g_RefreshTime = 1;			//ˢ�±���ͨѶ״̬������״̬����ʱ��ļ��ʱ��,��λ(����)

//��������ҳ
int			g_PTSettingValues = 5;	//��ֵ��ʷֵ��������. 0-10����
int			g_PTSettingGroup = 1;	//��ֵ�Ƿ�����ʾ. 0-��, 1-��
int			g_PTSettingCompare = 0; //��ֵ������Ƚ�, 0-��ǰһ��ֵ�Ƚ�, 1-���׼ֵ�Ƚ�
int			g_PTDIValues = 5;		//��������ʷֵ��������. 0-10����
int			g_PTDIStyle = 0;		//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��.
int			g_PTSoftValues = 5;		//��ѹ����ʷֵ��������. 0-10����
int			g_PTSoftStyle = 0;		//��ѹ��ֵ��ʾ���: 0: 1/0, 1: ON/OFF, 3: Ͷ/��.
int			g_PTAIValues = 5;		//ģ������ʷֵ��������. 0-10����
int			g_PTAIRealtimeValue = 0;	//�Ƿ���ʾģ����ʵʱֵ������ʱˢ��
int			g_PTAIRealtimeRefresh = 60;	//ģ����ʵʱֵˢ��ʱ��(��)
int			g_PTControl	= 1;		//���ƹ����Ƿ����. 0:������. 1:����
int			g_PTGeneralMore = 0;	//��������ҳ���"��ʾ����"��ť�Ƿ���ʾ.0-��, 1-��
int			g_PTActionNum = 100;	//�����¼���ʾ��
int			g_PTAlarmNum = 100;		//�澯�¼���ʾ��
int			g_PTShowHistory = 1;	//��ʷ�¼�ҳ���Ƿ���ʾ��0-��1-��
int			g_PTNewZoneTable = 0;	//�Ƿ�ʹ���µĶ�ֵ���ű�.Ϊ1ʱʹ��tb_pt_zone_def, Ϊ0ʱʹ��tb_secdev_zone_base
int			g_PTControlModel = 1;	//���ƹ���ģʽ��1-��ͨ��2-�㶫
int			g_PTModifyZone = 1;	
int			g_PTShowGeneral = 1;	//������Ϣҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowSetting = 1;	//��ֵҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowDI = 1;			//������ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowSoftBoard = 1;	//��ѹ��ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowAI = 1;			//ģ����ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowAction = 1;		//����ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowAlarm = 1;		//�澯ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowOsc = 1;		//����¼��ҳ���Ƿ���ʾ��0-��1-��
int			g_PTShowNote = 1;
int			g_PTShowOscList = 1;	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
int			g_PTSettingJudge = 0;
int			g_PTIndexType = 0;	
int			g_PTNotModifyCurrentZone = 0; //�Ƿ��ֹ�޸ĵ�ǰ��ֵ���Ķ�ֵ�� 0-����ֹ,1-��ֹ
int			g_PTOscListCompare = 1;
int			g_PTSetting160x = 0; //16���ƶ�ֵ��ʾʱ�Ƿ�ǿ����ǰ�����0x,0-��ǿ�ƣ�������վ�����˵�ֵ������ʾ��1-ǿ�Ƽ���0x


//¼��������ҳ
int			g_WRShowGeneral = 1;	//������Ϣҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowSetting = 1;	//��ֵҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowDI = 1;			//������ҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowAI = 1;			//ģ����ҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowOsc = 1;		//����¼��ҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowOscList = 1;	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
int			g_WRShowNote = 0;
//�¼���ϸ����
int		g_ShowActionDetail	= 1;	//�Ƿ���ʾ�����¼���ϸ����
int		g_ShowAlarmDetail	= 0;	//�Ƿ���ʾ�澯�¼���ϸ����
int		g_ShowSysAlarmDetail = 1;	//�Ƿ���ʾϵͳ�澯�¼���ϸ����

//��ҳ���Ƿ���ʾ����
BOOL		g_bShowActionWnd = TRUE;		//�Ƿ���ʾ�����¼�����
BOOL		g_bShowAlarmWnd = TRUE;			//�Ƿ���ʾ�澯�¼�����
BOOL		g_bShowFlautWnd = TRUE;			//�Ƿ���ʾ���ϱ��洰��
BOOL		g_bShowDIWnd = TRUE;			//�Ƿ���ʾ����������
BOOL		g_bShowOSCWnd = TRUE;			//�Ƿ���ʾ¼������
BOOL		g_bShowPTCommWnd = TRUE;		//�Ƿ���ʾ����ͨѶ״̬����
BOOL		g_bShowPTRunWnd = TRUE;			//�Ƿ���ʾ��������״̬����
BOOL		g_bShowSysAlarmWnd = TRUE;		//�Ƿ���ʾϵͳ�澯����
BOOL		g_bShowCrossWnd = FALSE;		//�Ƿ���ʾԤ������
BOOL		g_bShowSOEIWnd = TRUE;			//�Ƿ���ʾI���¼�����
BOOL		g_bShowSOEIIWnd = TRUE;			//�Ƿ���ʾII���¼�����
BOOL		g_bShowSOEIIIWnd = TRUE;		//�Ƿ���ʾIII���¼�����
BOOL		g_bShowSOEDebugWnd = TRUE;		//�Ƿ���ʾ�����¼�����
BOOL		g_bShowGeoWnd	= TRUE;			//�Ƿ���ʾ����ͼ����
BOOL		g_bShowDeviceMapWnd = TRUE;		//�Ƿ���ʾһ�ν���ͼ����
BOOL		g_bShowDistanceWnd = TRUE;		//�Ƿ���ʾС����ര��
BOOL		g_bShowStationInfoWnd = TRUE;	//�Ƿ���ʾ��վ��Ϣ����
BOOL		g_bShowRemindWnd = FALSE;
BOOL		g_bShowDraft = 0;//�Ƿ�����ʾʩ��ͼ���ܣ���������һ�ν���ͼ��������һ��"��ʾʩ��ͼ"��ť�������ʾʩ��ͼ


//�����ٻ����Զ���������
COLORREF	g_BatchCall		= RGB(0, 0, 0);	//�����ٻ�ѡ����, "�ٻ�"��������ɫ
COLORREF	g_BatchNotCall	= RGB(255, 0, 0);//�����ٻ�ѡ����, "���ٻ�"��������ɫ
COLORREF	g_BatchNotSupport = RGB(0, 0, 255);//�����ٻ�ѡ����, "��֧��"��������ɫ
COLORREF	g_BatchSuccess	= RGB(0, 0, 0);	//�����ٻ������, "�ɹ�"��������ɫ
COLORREF	g_BatchFail		= RGB(255, 0, 0);//�����ٻ������, "ʧ��"��������ɫ
COLORREF	g_BatchNULL		= RGB(80, 240, 120);//�����ٻ������, "����ٻ�"��������ɫ
COLORREF	g_BatchLater	= RGB(121, 121, 121);	//�����ٻ������, "��δ�ٻ�"��������ɫ
CString		g_AutoCall_IP	= "10.123.16.133";	//�Զ�����ģ�����ڵ�IP��ַ
int			g_AutoCall_Port = 8888;	//�Զ�����ģ��ļ����˿�

//���ָ����ڴ�С��¼
int		g_nCommSplitterWnd = 350;	//��վͨ�ϴ��ڷָ���С
int		g_nActionSplitterWnd = 350;	//�������ڷָ���С
int		g_nAlarmSplitterWnd = 350;	//�澯���ڷָ���С
int		g_nFaultSplitterWnd = 200;	//���Ϲ鵵���ڷָ���С
int		g_nSysAlarmSplitterWnd = 350;//ϵͳ�澯���ڷָ���С
int		g_nSOESplitterWnd = 350;		//SOE��Ϣ���ڷָ���С
int		g_nCrossSplitterWnd= 350;//Ԥ�����ڷָ���С
int		g_nDraftWndWidth = 1024 ;
int		g_nDraftWndHeight = 600 ;

void AddNoEraseControl(CWnd* pWnd, CRgn & bgRgn, int nID )
{
	CWnd* pControl = pWnd->GetDlgItem(nID);
	if(pControl == NULL)
		return;
	if(!pControl->IsWindowVisible())
		return;
    CRect controlRect;
    pWnd->GetDlgItem(nID)->GetWindowRect(&controlRect);
    CRgn controlRgn;
    controlRgn.CreateRectRgnIndirect(controlRect);
    if(bgRgn.CombineRgn(&bgRgn, &controlRgn, RGN_XOR)==ERROR)
		return;
}

void EraseBK( CDC* pDC, CRect bgRect, CRgn & bgRgn, COLORREF colBK )
{
	CBrush brush;
	brush.CreateSolidBrush(colBK);
	CPoint saveOrg = (pDC)->GetWindowOrg();
	(pDC)->SetWindowOrg(bgRect.TopLeft());
	(pDC)->FillRgn(&bgRgn, &brush);
	(pDC)->SetWindowOrg(saveOrg);
    brush.DeleteObject();
}

char g_arrStrTable[2][90];
//Ѱ�Ҵ��ڵĻص�����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	static char szBuffer[90];
	static char szTemp[80];
	static char szFormat[] = _T("%08lX: %s");
	
	GetWindowText(hwnd, szTemp, sizeof(szTemp));
	sprintf(szBuffer, szFormat, hwnd, szTemp);
	
	switch( lParam )
	{
	case EW_STWAVE:
		if( strstr( szBuffer, g_arrStrTable[0]) != NULL )
		{
			::SetForegroundWindow( hwnd );
			::ShowWindow( hwnd, SW_SHOWMAXIMIZED );
			
			//֪ͨSTWave�򿪼�������Comrtrade�ļ���
			::PostMessage(hwnd, WM_OPEN_WAVEFILE , 0, 0);
			return FALSE;
		}
		break;

	case EW_XJQUERY:
		if( strstr( szBuffer, g_arrStrTable[1]) != NULL )
		{
			::SetForegroundWindow( hwnd );
			::ShowWindow( hwnd, SW_SHOW );
			
			//֪ͨ
			::PostMessage(hwnd, WM_QUERY_EVENT , 0, 0);
			return FALSE;
		}
		break;
	case EW_XJALARMQUERY:
		if( strstr( szBuffer, g_arrStrTable[1]) != NULL )
		{
			::SetForegroundWindow( hwnd );
			::ShowWindow( hwnd, SW_SHOW );
			
			//֪ͨ
			::PostMessage(hwnd, WM_QUERY_EVENT , 0, 0);
			return FALSE;
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ��: GetSelectDir()
 ���ܸ�Ҫ: ȡ���û�ѡ����ļ���·��
 �� �� ֵ: �û�ѡ����ļ���·��
 ��    ��: param1 
		   Param2 
**************************************************************/
CString GetSelectDir()
{
	//ѡ��Ŀ���ļ���
	CString strNewDir = "";
	BROWSEINFO   bi;       
	char   dispname[MAX_PATH],   path[MAX_PATH];       
	ITEMIDLIST   *   pidl;       
	
	bi.hwndOwner   =   0;       
	bi.pidlRoot   =   0;       
    
	bi.pszDisplayName   =   dispname;       
	bi.lpszTitle   =   StringFromID(IDS_SELECT_DIR);       
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS;       
	bi.lpfn   =   0;       
	bi.lParam   =   0;       
	bi.iImage   =   0;       
	
	if   (pidl   =   SHBrowseForFolder(&bi))   //�����ļ���������ڣ���ѡȡĿ¼   
	{       
		if(   SHGetPathFromIDList(pidl,   path)   ==   TRUE   )   
			strNewDir   =   path   ;   
		else   
			strNewDir.Empty();
	}
	//��ʽ��·��
	if(!strNewDir.IsEmpty())
	{
		//���п��ܵ�/������"\\"
		strNewDir.Replace("/", "\\");
		if(strNewDir.Right(1)!="\\")
			strNewDir += "\\";
	}
	return strNewDir;
}


void GetFirstLetter(CString strName, CString& strFirstLetter)
{
    TBYTE ucHigh, ucLow;
    int  nCode;
    CString strRet;

    strFirstLetter.Empty();

    for (int i=0; i<strName.GetLength(); i++)
    {
        if ( (TBYTE)strName[i] < 0x80 )
            continue;

        ucHigh = (TBYTE)strName[i];
        ucLow  = (TBYTE)strName[i+1];
        if ( ucHigh < 0xa1 || ucLow < 0xa1)
            continue;
        else
            nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;

        strRet = FirstLetter(nCode);
        strFirstLetter += strRet;
        i++;
    }
}


CString FirstLetter(int nCode)
{
   if(nCode >= 1601 && nCode < 1637) return "A";
   if(nCode >= 1637 && nCode < 1833) return "B";
   if(nCode >= 1833 && nCode < 2078) return "C";
   if(nCode >= 2078 && nCode < 2274) return "D";
   if(nCode >= 2274 && nCode < 2302) return "E";
   if(nCode >= 2302 && nCode < 2433) return "F";
   if(nCode >= 2433 && nCode < 2594) return "G";
   if(nCode >= 2594 && nCode < 2787) return "H";
   if(nCode >= 2787 && nCode < 3106) return "J";
   if(nCode >= 3106 && nCode < 3212) return "K";
   if(nCode >= 3212 && nCode < 3472) return "L";
   if(nCode >= 3472 && nCode < 3635) return "M";
   if(nCode >= 3635 && nCode < 3722) return "N";
   if(nCode >= 3722 && nCode < 3730) return "O";
   if(nCode >= 3730 && nCode < 3858) return "P";
   if(nCode >= 3858 && nCode < 4027) return "Q";
   if(nCode >= 4027 && nCode < 4086) return "R";
   if(nCode >= 4086 && nCode < 4390) return "S";
   if(nCode >= 4390 && nCode < 4558) return "T";
   if(nCode >= 4558 && nCode < 4684) return "W";
   if(nCode >= 4684 && nCode < 4925) return "X";
   if(nCode >= 4925 && nCode < 5249) return "Y";
   if(nCode >= 5249 && nCode < 5590) return "Z";
   if(nCode >= 5601 && nCode <= 8794)
   {
    CString CodeData = "cjwgnspgcenegypbtwxzdxykygtpjnmjqmbsgzscyjsyyfpggbzgydywjkgaljswkbjqhyjwpdzlsgmr";
     CString str1 = "ybywwccgznkydgttngjeyekzydcjnmcylqlypyqbqrpzslwbdgkjfyxjwcltbncxjjjjcxdtqsqzycdxxhgckbphffss";
     CString str2 = "pybgmxjbbyglbhlssmzmpjhsojnghdzcdklgjhsgqzhxqgkezzwymcscjnyetxadzpmdssmzjjqjyzcjjfwqjbdzbjgd";
     CString str3 = "nzcbwhgxhqkmwfbpbqdtjjzkqhylcgxfptyjyyzpsjlfchmqshgmmxsxjpkdcmbbqbefsjwhwwgckpylqbgldlcctnma";
     CString str4 = "eddksjngkcsgxlhzaybdbtsdkdylhgymylcxpycjndqjwxqxfyyfjlejbzrwccqhqcsbzkymgplbmcrqcflnymyqmsqt";
     CString str5 = "rbcjthztqfrxchxmcjcjlxqgjmshzkbswxemdlckfsydsglycjjssjnqbjctyhbftdcyjdgwyghqfrxwckqkxebpdjpx";
     CString str6 = "jqsrmebwgjlbjslyysmdxlclqkxlhtjrjjmbjhxhwywcbhtrxxglhjhfbmgykldyxzpplggpmtcbbajjzyljtyanjgbj";
     CString str7 = "flqgdzyqcaxbkclecjsznslyzhlxlzcghbxzhznytdsbcjkdlzayffydlabbgqszkggldndnyskjshdlxxbcghxyggdj";
     CString str8 = "mmzngmmccgwzszxsjbznmlzdthcqydbdllscddnlkjyhjsycjlkohqasdhnhcsgaehdaashtcplcpqybsdmpjlpcjaql";
     CString str9 = "cdhjjasprchngjnlhlyyqyhwzpnccgwwmzffjqqqqxxaclbhkdjxdgmmydjxzllsygxgkjrywzwyclzmcsjzldbndcfc";
     CString str10 = "xyhlschycjqppqagmnyxpfrkssbjlyxyjjglnscmhcwwmnzjjlhmhchsyppttxrycsxbyhcsmxjsxnbwgpxxtaybgajc";
     CString str11 = "xlypdccwqocwkccsbnhcpdyznbcyytyckskybsqkkytqqxfcwchcwkelcqbsqyjqcclmthsywhmktlkjlychwheqjhtj";
     CString str12 = "hppqpqscfymmcmgbmhglgsllysdllljpchmjhwljcyhzjxhdxjlhxrswlwzjcbxmhzqxsdzpmgfcsglsdymjshxpjxom";
     CString str13 = "yqknmyblrthbcftpmgyxlchlhlzylxgsssscclsldclepbhshxyyfhbmgdfycnjqwlqhjjcywjztejjdhfblqxtqkwhd";
     CString str14 = "chqxagtlxljxmsljhdzkzjecxjcjnmbbjcsfywkbjzghysdcpqyrsljpclpwxsdwejbjcbcnaytmgmbapclyqbclzxcb";
     CString str15 = "nmsggfnzjjbzsfqyndxhpcqkzczwalsbccjxpozgwkybsgxfcfcdkhjbstlqfsgdslqwzkxtmhsbgzhjcrglyjbpmljs";
     CString str16 = "xlcjqqhzmjczydjwbmjklddpmjegxyhylxhlqyqhkycwcjmyhxnatjhyccxzpcqlbzwwwtwbqcmlbmynjcccxbbsnzzl";
     CString str17 = "jpljxyztzlgcldcklyrzzgqtgjhhgjljaxfgfjzslcfdqzlclgjdjcsnclljpjqdcclcjxmyzftsxgcgsbrzxjqqcczh";
     CString str18 = "gyjdjqqlzxjyldlbcyamcstylbdjbyregklzdzhldszchznwczcllwjqjjjkdgjcolbbzppglghtgzcygezmycnqcycy";
     CString str19 = "hbhgxkamtxyxnbskyzzgjzlqjdfcjxdygjqjjpmgwgjjjpkjsbgbmmcjssclpqpdxcdyykypcjddyygywchjrtgcnyql";
     CString str20 = "dkljczzgzccjgdyksgpzmdlcphnjafyzdjcnmwescsglbtzcgmsdllyxqsxsbljsbbsgghfjlwpmzjnlyywdqshzxtyy"; 
     CString str21 = "whmcyhywdbxbtlmswyyfsbjcbdxxlhjhfpsxzqhfzmqcztqcxzxrdkdjhnnyzqqfnqdmmgnydxmjgdhcdycbffallztd";
     CString str22 = "ltfkmxqzdngeqdbdczjdxbzgsqqddjcmbkxffxmkdmcsychzcmljdjynhprsjmkmpcklgdbqtfzswtfgglyplljzhgjj";
     CString str23 = "gypzltcsmcnbtjbhfkdhbyzgkpbbymtdlsxsbnpdkleycjnycdykzddhqgsdzsctarlltkzlgecllkjljjaqnbdggghf";
     CString str24 = "jtzqjsecshalqfmmgjnlyjbbtmlycxdcjpldlpcqdhsycbzsckbzmsljflhrbjsnbrgjhxpdgdjybzgdlgcsezgxlblg";
     CString str25 = "yxtwmabchecmwyjyzlljjshlgndjlslygkdzpzxjyyzlpcxszfgwyydlyhcljscmbjhblyjlycblydpdqysxktbytdkd";
     CString str26 = "xjypcnrjmfdjgklccjbctbjddbblblcdqrppxjcglzcshltoljnmdddlngkaqakgjgyhheznmshrphqqjchgmfprxcjg";
     CString str27 = "dychghlyrzqlcngjnzsqdkqjymszswlcfqjqxgbggxmdjwlmcrnfkkfsyyljbmqammmycctbshcptxxzzsmphfshmclm";
     CString str28 = "ldjfyqxsdyjdjjzzhqpdszglssjbckbxyqzjsgpsxjzqznqtbdkwxjkhhgflbcsmdldgdzdblzkycqnncsybzbfglzzx"; 
     CString str29 = "swmsccmqnjqsbdqsjtxxmbldxcclzshzcxrqjgjylxzfjphymzqqydfqjjlcznzjcdgzygcdxmzysctlkphtxhtlbjxj";
     CString str30 = "lxscdqccbbqjfqzfsltjbtkqbsxjjljchczdbzjdczjccprnlqcgpfczlclcxzdmxmphgsgzgszzqjxlwtjpfsyaslcj";
     CString str31 = "btckwcwmytcsjjljcqlwzmalbxyfbpnlschtgjwejjxxglljstgshjqlzfkcgnndszfdeqfhbsaqdgylbxmmygszldyd";
     CString str32 = "jmjjrgbjgkgdhgkblgkbdmbylxwcxyttybkmrjjzxqjbhlmhmjjzmqasldcyxyqdlqcafywyxqhz";
	 CodeData += str1;
	 CodeData += str2;
	 CodeData += str3;
	 CodeData += str4;
	 CodeData += str5;
	 CodeData += str6;
	 CodeData += str7;
	 CodeData += str8;
	 CodeData += str9;
	 CodeData += str10;
	 CodeData += str11;
	 CodeData += str12;
	 CodeData += str13;
	 CodeData += str14;
	 CodeData += str15;
	 CodeData += str16;
	 CodeData += str17;
	 CodeData += str18;
	 CodeData += str19;
	 CodeData += str20;
	 CodeData += str21;
	 CodeData += str22;
	 CodeData += str23;
	 CodeData += str24;
	 CodeData += str25;
	 CodeData += str26;
	 CodeData += str27;
	 CodeData += str28;
	 CodeData += str29;
	 CodeData += str30;
	 CodeData += str31;
	 CodeData += str32;

    CString gbcode;
    gbcode.Format("%d", nCode);
	int pos1 = atoi(gbcode.Mid(0,2));
	int pos2 = atoi(gbcode.Mid(gbcode.GetLength()-2,2));
    int pos = (pos1-56)*94 + pos2;
    CString strResult = CodeData.Mid(pos-1,1);
    strResult.MakeUpper();
    return strResult;
   } 
   return "";
}

typedef HRESULT (__stdcall * pfnHello)(REFCLSID,REFIID,void**);
BOOL InitSVGComNoReg( IClassFactory*& pFactory, ISvgViewer*& pSvgDoc )
{
	pfnHello fnHello= NULL;
	HINSTANCE hdllInst = LoadLibrary("XJSVGEX.dll");
	fnHello=(pfnHello)GetProcAddress(hdllInst,"DllGetClassObject");
	if (fnHello != 0)
	{
		pFactory = NULL;
		HRESULT hr=(fnHello)(IID_ISvgViewer,IID_IClassFactory,(void**)&pFactory);
		if (SUCCEEDED(hr) && (pFactory != NULL))
		{
			hr = pFactory->CreateInstance(NULL, IID_ISvgViewer, (void**)&pSvgDoc);
			if (SUCCEEDED(hr) && (pSvgDoc != NULL))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL InitReportComNoReg( IClassFactory*& pFactory, IXJReport*& pReportDoc )
{
	pfnHello fnHello= NULL;
	HINSTANCE hdllInst = LoadLibrary("XJReportMgrEx.dll");
	fnHello=(pfnHello)GetProcAddress(hdllInst,"DllGetClassObject");
	if (fnHello != 0)
	{
		pFactory = NULL;
		HRESULT hr=(fnHello)(IID_IXJReport,IID_IClassFactory,(void**)&pFactory);
		if (SUCCEEDED(hr) && (pFactory != NULL))
		{
			hr = pFactory->CreateInstance(NULL, IID_IXJReport, (void**)&pReportDoc);
			if (SUCCEEDED(hr) && (pReportDoc != NULL))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL DelPostfixFromPath(CString& str)
{
	if( str.IsEmpty() )
		return FALSE;
	str.Replace("/", "\\");
	int nFind = str.ReverseFind('\\');
	int nFind2 = str.ReverseFind('.');
	if( nFind2 <= nFind )
		return FALSE;
	if(nFind2 >= 0)
	{
		str = str.Left(nFind2); //ȥ����׺
	}
	return TRUE;
}




