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

int g_RemindMaxCount = 0; //最大显示数
int g_RemindTime = 5;

/** @brief			厂站通断图形名称. XXXX.svg*/
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
int     g_role = 0;                 //运行与监视角色 0:主站 1:子站
int     g_FindDeviceWay = 0;        //点击SVG图二次设备时查找设备方式 0:103组号 1:uri
int		g_ShowOutputWnd		= 1;	//是否显示输出窗口
int		g_ShowDeviceTreeWnd = 1;	//是否显示设备树窗口
int		g_DeviceTreeType	= 2;	//设备树类型
int		g_PutOutToLogFile	= 1;
int		g_OutPutToWnd		= 1;
int		g_LogLevel			= 2;
int     g_LoadSttpDll       = 0;   //动态库的选择  0:SttpNet.Dll 1:SttpProxy
CString	g_LocalPath			= "";
CString	g_UIConfig			= "";
CString g_LogPath			= "";
CString	g_SysConfig			= "";
CString	g_SoundFile			= "";
CString g_DefaultStation	= "";
CString g_ImageFile			= "";
int		g_TabPosition		= 0;		//tab栏位置. 0-下方, 1-上方
int		g_ListItemHeight	= 18;		//列表行高
int		g_AutoCallMaxSaveNo = 10;
int		g_DisconnectTime	= 10;	//通讯断开多长时间再重连上要重新读库(单位:秒)
COLORREF	g_ListSpaceColor = RGB(150, 237, 239);	//列表间隔颜色
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
CString		g_WorkStatusFileName = "status.log";//子站工作状态文件,带后缀
int			g_LoadDataModel = 0;
int			g_ThreadTime = 10; 
int			g_SecType = -1;//读取的二次设备的类型.为-1是读取所有类型值>0的设备.大于等于0时只读指定类型的设备
CString		g_sAppName="";//程序名
int			g_PostTime = 1000;	//报文推送间隔
int			g_UseLocalIp = 0;//是否使用本地配置的IP, 0否1是。为0时在数据库中读取配置,默认为0 
CString		g_LocalIp = "";//本地配置的IP
int			g_LockTime = 5;//登录锁定时间,分钟
CTime		g_LastFailTime = CTime::GetCurrentTime();//最后一次失败的时间
int			g_LoginFailTimes = 0;//登录失败次数
int			g_UILockTime = 1;//界面多久不操作会被锁定.分钟
int			g_UseXJWave = 1;
int			g_MarkDownOscPT = 0;
int			g_AutoStationCommMap = 1;
CString		g_Language = "zh_CN";

//故障报告
int			g_FaultActionTime = 0;//故障报告中计算动作相对时间方法。0为本条动作信号时间减第一条动作的信号时间，1为本条的信号时间减启动时间
int			g_FaultFilterIII = 0;//故障报告是否过滤III类信息.0否，1是
int			g_FaultShowWave = 0;  //录波器波形是否显示。0-否，1-是
//event
COLORREF g_colEventI		= RGB(255,0,0);
COLORREF g_colEventII		= RGB(0, 255, 255);
COLORREF g_colEventIII		= RGB(214, 223, 247);
COLORREF g_colNotAcked		= RGB(0, 0, 255);
COLORREF g_colAcked			= RGB(0, 0, 0);
int			g_ShowEventI = 1;		//I类事件是否显示. 0-不显示. 1-显示
int			g_AlarmEventI = 1;		//I类事件是否报警. 0-不报警. 1-报警
int			g_ShowEventII = 1;		//II类事件是否显示. 0-不显示. 1-显示
int			g_AlarmEventII = 1;		//II类事件是否报警. 0-不报警. 1-报警
int			g_ShowEventIII = 1;		//III类事件是否显示. 0-不显示. 1-显示
int			g_AlarmEventIII = 1;	//III类事件是否报警. 0-不报警. 1-报警
//通讯状态状态对应颜色
COLORREF XJ_STATUS_COMM_ON	= RGB(0, 111, 0);		//断讯正常,绿色
COLORREF XJ_STATUS_COMM_OFF	= RGB(225, 0, 0);		//通讯断开,红色
COLORREF XJ_STATUS_COMM_PART = RGB(220,220,0);

//运行状态对应颜色
COLORREF XJ_STATUS_RUN_ON		= RGB(0, 111, 0);		//运行, 绿色
COLORREF XJ_STATUS_RUN_CHECK	= RGB(240, 240, 0);		//检修/挂牌, 黄色
COLORREF XJ_STATUS_RUN_OFF		= RGB(162, 230, 165);	//停用, 淡绿
COLORREF XJ_STATUS_RUN_DEBUG	= RGB(157, 0, 183);		//调试, 紫色
COLORREF XJ_STATUS_RUN_CODE		= RGB(121, 121, 121);		//对码表, 灰色

//报警
int		g_BlinkDevice		= 1;
int		g_MustShowDevice	= 1;
int		g_BlinkOwner		= 1;
int		g_ShowEventDlg		= 0;		//收到装置的动作或告警时, 是否弹出事件属性框
int		g_UseAlarmSound = 0;	//是否使用声音报警. 0-否,1-是
int		g_SoundTime = 60;		//报警声音持续时间. >=0整数, 单位:秒
int     g_FlashTime = 1200;
int		g_BlinkStation		= 1;
int		g_AutoChangeMap = 0;

//主子站通信状态页面
int		g_RefreshStationTime = 10;	//厂站状态持续时间刷新周期单位(分钟)
int		g_ShowCommON	= 1;		//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
int		g_ShowCommOFF	= 1;		//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
int     g_QueryDay  = 10;

int			g_ShowDevStatusInStation = 1;

int			g_DoubleChannel = 0;			//厂站是否使用双通道通信
COLORREF	g_SCOnly0OFF = RGB(255, 140, 0);	//只有0号通道断开时的颜色,深橙色
COLORREF	g_SCOnly1OFF = RGB(245, 222, 131);//只有1号通道断开时的颜色,浅黄色
COLORREF	g_SCAllOFF = RGB(255, 0, 0);//两个通道全断开时的颜色,色红色

COLORREF		g_CommMapBK = RGB(0, 79, 79); //主子站通讯状态图形背景色
CString		g_CommMapFont = "";//主子站通讯状体图中厂站名的字体类型
int			g_CommMapFontSize = 18;//主子站通讯状体图中厂站名的字体大小
COLORREF		g_CommMapFontColor = RGB(0,0, 0);//主子站通讯状体图中厂站名的字体颜色

//保护通信状态页面
int			g_RefreshPTCommTime = 10;	//刷新保护通信状态持续时间的间隔时间,单位(分钟)
int			g_ShowPTCommON	= 1;		//列表中是否显示通信状态正常的装置信息.0-不显示. 1-显示
int			g_ShowPTCommOFF	= 1;		//列表中是否显示通信状态断开的装置信息.0-不显示. 1-显示

//保护运行状态页面
int			g_RefreshPTRunTime = 10;	//刷新保护运行状态持续时间的间隔时间,单位(分钟)
int			g_ShowPTRunON = 1;			//列表中是否显示运行状态正常的装置信息.0-不显示. 1-显示
int			g_ShowPTRunCheck = 1;		//列表中是否显示运行状态检修的装置信息.0-不显示. 1-显示
int			g_ShowPTRunOFF = 1;			//列表中是否显示运行状态停运的装置信息.0-不显示. 1-显示
int			g_ShowPTRunDebug = 1;		//列表中是否显示运行状态调试的装置信息.0-不显示. 1-显示
int			g_ShowPTRunCode	= 1;		//列表中是否显示运行状态对码表的装置信息.0-不显示. 1-显示

//厂站信息页面
int			g_RefreshTime = 1;			//刷新保护通讯状态和运行状态持续时间的间隔时间,单位(分钟)

//保护属性页
int			g_PTSettingValues = 5;	//定值历史值载入项数. 0-10整数
int			g_PTSettingGroup = 1;	//定值是否按组显示. 0-否, 1-是
int			g_PTSettingCompare = 0; //定值与哪项比较, 0-与前一次值比较, 1-与基准值比较
int			g_PTDIValues = 5;		//开关量历史值载入项数. 0-10整数
int			g_PTDIStyle = 0;		//开关量值显示风格.0: 1/0, 1: ON/OFF, 2: 投/退, 3: 合/开.
int			g_PTSoftValues = 5;		//软压板历史值载入项数. 0-10整数
int			g_PTSoftStyle = 0;		//软压板值显示风格: 0: 1/0, 1: ON/OFF, 3: 投/退.
int			g_PTAIValues = 5;		//模拟量历史值载入项数. 0-10整数
int			g_PTAIRealtimeValue = 0;	//是否显示模拟量实时值，并定时刷新
int			g_PTAIRealtimeRefresh = 60;	//模拟量实时值刷新时间(秒)
int			g_PTControl	= 1;		//控制功能是否可用. 0:不可用. 1:可用
int			g_PTGeneralMore = 0;	//基本属性页面的"显示更多"按钮是否显示.0-否, 1-是
int			g_PTActionNum = 100;	//动作事件显示数
int			g_PTAlarmNum = 100;		//告警事件显示数
int			g_PTShowHistory = 1;	//历史事件页面是否显示。0-否，1-是
int			g_PTNewZoneTable = 0;	//是否使用新的定值区号表.为1时使用tb_pt_zone_def, 为0时使用tb_secdev_zone_base
int			g_PTControlModel = 1;	//控制功能模式。1-普通。2-广东
int			g_PTModifyZone = 1;	
int			g_PTShowGeneral = 1;	//基本信息页面是否显示。0-否，1-是
int			g_PTShowSetting = 1;	//定值页面是否显示。0-否，1-是
int			g_PTShowDI = 1;			//开关量页面是否显示。0-否，1-是
int			g_PTShowSoftBoard = 1;	//软压板页面是否显示。0-否，1-是
int			g_PTShowAI = 1;			//模拟量页面是否显示。0-否，1-是
int			g_PTShowAction = 1;		//动作页面是否显示。0-否，1-是
int			g_PTShowAlarm = 1;		//告警页面是否显示。0-否，1-是
int			g_PTShowOsc = 1;		//本地录波页面是否显示。0-否，1-是
int			g_PTShowNote = 1;
int			g_PTShowOscList = 1;	//远方录波页面是否显示。0-否，1-是
int			g_PTSettingJudge = 0;
int			g_PTIndexType = 0;	
int			g_PTNotModifyCurrentZone = 0; //是否禁止修改当前定值区的定值。 0-不禁止,1-禁止
int			g_PTOscListCompare = 1;
int			g_PTSetting160x = 0; //16进制定值显示时是否强制在前面加上0x,0-不强制，根据子站送上了的值进行显示。1-强制加上0x


//录波器属性页
int			g_WRShowGeneral = 1;	//基本信息页面是否显示。0-否，1-是
int			g_WRShowSetting = 1;	//定值页面是否显示。0-否，1-是
int			g_WRShowDI = 1;			//开关量页面是否显示。0-否，1-是
int			g_WRShowAI = 1;			//模拟量页面是否显示。0-否，1-是
int			g_WRShowOsc = 1;		//本地录波页面是否显示。0-否，1-是
int			g_WRShowOscList = 1;	//远方录波页面是否显示。0-否，1-是
int			g_WRShowNote = 0;
//事件详细窗口
int		g_ShowActionDetail	= 1;	//是否显示动作事件详细内容
int		g_ShowAlarmDetail	= 0;	//是否显示告警事件详细内容
int		g_ShowSysAlarmDetail = 1;	//是否显示系统告警事件详细内容

//各页面是否显示设置
BOOL		g_bShowActionWnd = TRUE;		//是否显示动作事件窗口
BOOL		g_bShowAlarmWnd = TRUE;			//是否显示告警事件窗口
BOOL		g_bShowFlautWnd = TRUE;			//是否显示故障报告窗口
BOOL		g_bShowDIWnd = TRUE;			//是否显示开关量窗口
BOOL		g_bShowOSCWnd = TRUE;			//是否显示录波窗口
BOOL		g_bShowPTCommWnd = TRUE;		//是否显示保护通讯状态窗口
BOOL		g_bShowPTRunWnd = TRUE;			//是否显示保护运行状态窗口
BOOL		g_bShowSysAlarmWnd = TRUE;		//是否显示系统告警窗口
BOOL		g_bShowCrossWnd = FALSE;		//是否显示预警窗口
BOOL		g_bShowSOEIWnd = TRUE;			//是否显示I类事件窗口
BOOL		g_bShowSOEIIWnd = TRUE;			//是否显示II类事件窗口
BOOL		g_bShowSOEIIIWnd = TRUE;		//是否显示III类事件窗口
BOOL		g_bShowSOEDebugWnd = TRUE;		//是否显示调试事件窗口
BOOL		g_bShowGeoWnd	= TRUE;			//是否显示地理图窗口
BOOL		g_bShowDeviceMapWnd = TRUE;		//是否显示一次接线图窗口
BOOL		g_bShowDistanceWnd = TRUE;		//是否显示小波测距窗口
BOOL		g_bShowStationInfoWnd = TRUE;	//是否显示厂站信息窗口
BOOL		g_bShowRemindWnd = FALSE;
BOOL		g_bShowDraft = 0;//是否开启显示施工图功能，开启后在一次接线图窗口中有一个"显示施工图"按钮，点击显示施工图


//批量召唤，自动总召设置
COLORREF	g_BatchCall		= RGB(0, 0, 0);	//批量召唤选择中, "召唤"的文字颜色
COLORREF	g_BatchNotCall	= RGB(255, 0, 0);//批量召唤选择中, "不召唤"的文字颜色
COLORREF	g_BatchNotSupport = RGB(0, 0, 255);//批量召唤选择中, "不支持"的文字颜色
COLORREF	g_BatchSuccess	= RGB(0, 0, 0);	//批量召唤结果中, "成功"的文字颜色
COLORREF	g_BatchFail		= RGB(255, 0, 0);//批量召唤结果中, "失败"的文字颜色
COLORREF	g_BatchNULL		= RGB(80, 240, 120);//批量召唤结果中, "此项不召唤"的文字颜色
COLORREF	g_BatchLater	= RGB(121, 121, 121);	//批量召唤结果中, "尚未召唤"的文字颜色
CString		g_AutoCall_IP	= "10.123.16.133";	//自动总召模块所在的IP地址
int			g_AutoCall_Port = 8888;	//自动总召模块的监听端口

//各分隔窗口大小记录
int		g_nCommSplitterWnd = 350;	//厂站通断窗口分隔大小
int		g_nActionSplitterWnd = 350;	//动作窗口分隔大小
int		g_nAlarmSplitterWnd = 350;	//告警窗口分隔大小
int		g_nFaultSplitterWnd = 200;	//故障归档窗口分隔大小
int		g_nSysAlarmSplitterWnd = 350;//系统告警窗口分隔大小
int		g_nSOESplitterWnd = 350;		//SOE信息窗口分隔大小
int		g_nCrossSplitterWnd= 350;//预警窗口分隔大小
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
//寻找窗口的回调函数
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
			
			//通知STWave打开剪贴板获得Comrtrade文件名
			::PostMessage(hwnd, WM_OPEN_WAVEFILE , 0, 0);
			return FALSE;
		}
		break;

	case EW_XJQUERY:
		if( strstr( szBuffer, g_arrStrTable[1]) != NULL )
		{
			::SetForegroundWindow( hwnd );
			::ShowWindow( hwnd, SW_SHOW );
			
			//通知
			::PostMessage(hwnd, WM_QUERY_EVENT , 0, 0);
			return FALSE;
		}
		break;
	case EW_XJALARMQUERY:
		if( strstr( szBuffer, g_arrStrTable[1]) != NULL )
		{
			::SetForegroundWindow( hwnd );
			::ShowWindow( hwnd, SW_SHOW );
			
			//通知
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
 函 数 名: GetSelectDir()
 功能概要: 取得用户选择的文件夹路径
 返 回 值: 用户选择的文件夹路径
 参    数: param1 
		   Param2 
**************************************************************/
CString GetSelectDir()
{
	//选择目标文件夹
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
	
	if   (pidl   =   SHBrowseForFolder(&bi))   //弹出文件夹浏览窗口，并选取目录   
	{       
		if(   SHGetPathFromIDList(pidl,   path)   ==   TRUE   )   
			strNewDir   =   path   ;   
		else   
			strNewDir.Empty();
	}
	//格式化路径
	if(!strNewDir.IsEmpty())
	{
		//把有可能的/都换成"\\"
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
		str = str.Left(nFind2); //去除后缀
	}
	return TRUE;
}




