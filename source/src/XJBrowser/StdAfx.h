// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_)
#define AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_


#if(WINVER < 0x0500)
#undef WINVER
#define WINVER 0x0501
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
// #include <afxdisp.h>        // MFC Automation classes
// #include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxmt.h>
#import <msxml6.dll>
#include <afxole.h>
#pragma warning(disable:4700)

#include "PictureEx.h"	//Gif������
#include "..\..\ui_common\OXSplashWnd\OXSplashWnd.h"
#include "..\..\common\define.h"
#include "..\..\common\MessageLog.h"
#include "..\..\common\SttpDefine.h"
#include "..\..\common\MemSet.h"
#include "..\..\common\MemField.h"
#include "..\..\common\XJErrorID.h"
#include "ListCtrlEx.h"
#include "..\..\ui_common\button\XPStyleButtonST.h"
#include "..\..\ui_common\button\ThemeHelperST.h"
#include "HideHeaderCtrl.h"
#include "OXTreeCtrl.h"
#include "TreeListHeaderCtrl.h"
#include "TreeListTipCtrl.h"
#include "TreeListStaticCtrl.h"
#include "TreeListEditCtrl.h"
#include "TreeListComboCtrl.h"
#include "TreeListItem.h"
#include "TreeListCtrl.h"
#include "MyTreeListCtrl.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "..\..\ui_common\Interface\SvgViewerInterface.h"
#include "..\..\ui_common\Interface/XJReportInterface.h"

#include "..\..\ui_common\CompareComboBox\CompareComboBox.h"


extern CString g_strOscName;
extern CString g_strStyleName;
extern CString g_StyleConfig;
extern CString g_strToolBarBK;
extern CString g_strStartBmp;
extern BOOL g_bExit;
extern int g_hook;

/** @brief			�߳��˳�ʱ��,����ʱ��δ�˳���ǿɱ*/
extern DWORD	g_PTThreadExitTime;

/** @brief			��־������.0-�ɱ�7���ֶΡ� 1-�±�>7���ֶ�*/
extern int g_OperTableType;

/** @brief			��վͨ��ͼ������. XXXX.svg*/
extern CString g_strStationSvg;
//ȫ�ֱ�������
extern int	g_Temp;
extern int  g_style;
extern BOOL g_bLoginVerify;
extern BOOL g_bConnectCommServer;
extern BOOL g_bVerify;

extern COXSplashWnd g_wndWait;				//�ȴ�����
extern UINT g_dwESDataFormat;	//register for drag and drop

extern int g_ConnectServerButton;
extern int g_DeviceTreeButton;
extern int g_LogButton;
extern int g_HistoryQueryButton;
extern int g_AlarmCountButton;
extern int g_SystemSettingButton;
extern int g_CallAllButton;
extern int g_CommonFileButton;
extern int g_ExitButton;

//�����¼�
extern int g_RemindMaxCount; //�����ʾ��
extern int g_RemindTime; //ÿ���¼��ı���ʱ��(����)


/** @brief			��ͣ���ж�ʱ�����߳�*/
extern BOOL g_bSuspend;

extern BOOL	g_Exiting; //�����˳�����

//ϵͳ
extern  int         g_bSend20162;
extern  int         g_role;             //��������ӽ�ɫ 0:��վ 1:��վ
extern  int         g_FindDeviceWay;    //���SVGͼ�����豸ʱ�����豸��ʽ 0:103��� 1:uri
extern  int			g_ShowOutputWnd;	//�Ƿ���ʾ�������
extern	int			g_ShowDeviceTreeWnd;//�Ƿ���ʾ�豸������
extern	int			g_DeviceTreeType;	//�豸�����ͣ�0��ֻ����վ��1��վ-��ѹ�ȼ�-�豸��2��վ-��ѹ�ȼ�-���-�豸. 3:վ-��ѹ�ȼ�-�豸(��ɾ����һ���սڵ�). 4:վ-�����豸
extern	int			g_PutOutToLogFile;	//�Ƿ��������־�ļ� 0Ϊ�����, >=1Ϊ���
extern	int			g_OutPutToWnd;		//�Ƿ��������־����	0Ϊ�����, >0Ϊ���
extern	int			g_LogLevel;			//�����־�ĵȼ�, 1Ϊ��Ҫ����־�����; 2Ϊ��ͨ��־���; 3Ϊ������־�����(����);
extern  int			g_LoadSttpDll;      //���õĶ�̬��ѡ�� 0Ϊ����SttpNet.Dll; 1Ϊ����SttpProxy.Dll
extern	CString		g_LocalPath;		//��������Ŀ¼
extern	CString		g_UIConfig;			//UI�����ļ���·��
extern	CString		g_LogPath;			//log�ļ�λ��
extern	CString		g_SysConfig;		//���������ļ�λ��
extern  CString		g_SoundFile;		//�����ļ�λ��
extern	CString		g_ImageFile;		//����ͼ�����ļ�λ��
extern	CString		g_DefaultStation;	//��������ʱĬ�ϴ򿪵ĳ�վ
extern	int			g_TabPosition;		//tab��λ��. 0-�·�, 1-�Ϸ�
extern	int			g_ListItemHeight;	//�б��и�
extern	int			g_AutoCallMaxSaveNo; //�Զ�����������κ�
extern	int			g_DisconnectTime;	//ͨѶ�Ͽ��೤ʱ����������Ҫ���¶���
extern  COLORREF	g_ListSpaceColor;	//�б�����ɫ
extern  COLORREF    g_ListSpaceColor2;
extern  COLORREF    g_ListSpaceColor3;
extern  COLORREF    g_ListSpaceColor4;
extern  COLORREF    g_ListHeadColor;
extern  COLORREF    g_TreeBkColor;
extern  COLORREF    g_TreeTextColor;
extern  COLORREF    g_TitleLightColor;
extern  COLORREF    g_TitleDeepColor;
extern	int			g_UseColorFaultReport; //�Ƿ�ʹ�ô���ɫ�Ĺ��ϱ�����1�ǣ�0��
extern	int			g_UseLandscapeMultiStation;//˫�˹��ϱ����Ƿ�ʹ�ú����ʽ.1�ǣ�0��
extern	int			g_FaultReportFilter;//���ϱ����Ƿ��������ֵ94��4
extern	CString			g_WorkStatusFileName;//��վ����״̬�ļ�,����׺
extern  int			g_LoadDataModel;
extern	int			g_ThreadTime;
extern	int			g_SecType;//��ȡ�Ķ����豸������.Ϊ-1�Ƕ�ȡ��������ֵ>0���豸.���ڵ���0ʱֻ��ָ�����͵��豸
extern	CString			g_sAppName;//������
extern	int			g_PostTime;	//�������ͼ��
extern	int			g_UseLocalIp;//�Ƿ�ʹ�ñ������õ�IP, 0��1�ǡ�Ϊ0ʱ�����ݿ��ж�ȡ����,Ĭ��Ϊ0 
extern	CString		g_LocalIp;//�������õ�IP
extern	int			g_LockTime;//��¼����ʱ��,����
extern	CTime		g_LastFailTime;//���һ��ʧ�ܵ�ʱ��
extern	int			g_LoginFailTimes;//��¼ʧ�ܴ���
extern	int			g_UILockTime;//�����ò������ᱻ����.����
extern	int			g_UseXJWave;//�Ƿ�ʹ��XJWave
extern	int			g_MarkDownOscPT;//�Ƿ���һ�ν���ͼ�б��������¼���ļ��ı���
extern	int			g_AutoStationCommMap;//�Ƿ��Զ����Ƴ�վͨ��״̬ͼ��
extern CString		g_Language; //����.ch_ZN, en_US

//���ϱ���
extern	int			g_FaultActionTime;//���ϱ����м��㶯�����ʱ�䷽����0Ϊ���������ź�ʱ�����һ���������ź�ʱ�䣬1Ϊ�������ź�ʱ�������ʱ��
extern	int			g_FaultFilterIII;//���ϱ����Ƿ����III����Ϣ.0��1��
//ͨѶ״̬״̬��Ӧ��ɫ
extern COLORREF		XJ_STATUS_COMM_ON;		//��Ѷ����
extern COLORREF		XJ_STATUS_COMM_OFF;		//ͨѶ�Ͽ�
extern COLORREF		XJ_STATUS_COMM_PART;	//���ڳ�վͨѶ״̬����������վ���б����Ͽ������

//����״̬��Ӧ��ɫ
extern COLORREF		XJ_STATUS_RUN_ON;		//����
extern COLORREF		XJ_STATUS_RUN_CHECK;	//����/����
extern COLORREF		XJ_STATUS_RUN_OFF;		//ͣ��
extern COLORREF		XJ_STATUS_RUN_DEBUG;	//����(�����)
extern COLORREF		XJ_STATUS_RUN_CODE;		//δ����
extern COLORREF		XJ_STATUS_RUN_MARK;		//��ֵ�޸�װ�ù���
//�¼�
extern	int			g_ShowEventI;		//I���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
extern	int			g_AlarmEventI;		//I���¼��Ƿ񱨾�. 0-������. 1-����
extern  COLORREF	g_colEventI;		//I���¼���ɫ

extern	int			g_ShowEventII;		//II���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
extern	int			g_AlarmEventII;		//II���¼��Ƿ񱨾�. 0-������. 1-����
extern	COLORREF	g_colEventII;		//II���¼���ɫ

extern	int			g_ShowEventIII;		//III���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
extern	int			g_AlarmEventIII;	//III���¼��Ƿ񱨾�. 0-������. 1-����
extern	COLORREF	g_colEventIII;		//III���¼���ɫ

extern	COLORREF	g_colNotAcked;		//δȷ���¼�������ɫ
extern	COLORREF	g_colAcked;			//��ȷ���¼�������ɫ

extern	int			g_BlinkDevice;		//�յ�װ�õĶ�����澯ʱ,�Ƿ���˸��װ��ͼ��.0-��,1-��
extern	int			g_MustShowDevice;	//�յ�װ�õĶ�����澯ʱ,�Ƿ��������վ��ͼ�λ����Ƶ���ǰ��.0-��,1-��
extern	int			g_BlinkOwner;		//�����豸��˸ʱ,�Ƿ��������һ���豸һ����˸.0-��,1-��
extern  int			g_ShowEventDlg;		//�յ�װ�õĶ�����澯ʱ, �Ƿ񵯳��¼����Կ�. 0-��, 1-��
extern	int			g_UseAlarmSound;	//�Ƿ�ʹ����������. 0-��,1-��
extern	int			g_SoundTime;		//������������ʱ��. >=0����. ��λ.��
extern  int         g_FlashTime;
extern	int			g_BlinkStation;		//�յ�������澯ʱ���Ƿ���˸��վͼ��
extern	int			g_AutoChangeMap;	//Ҫ��˸�豸ʱ�������Ӧ��վͼû��ҳ������ʾ���Ƿ��Զ��л�����Ӧ��վͼ
//����վͨѶ״̬����
extern	int			g_RefreshStationTime;//ˢ�³�վ����ʱ��ļ��ʱ��,��λ(����)
extern	int			g_ShowCommON;		//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
extern	int			g_ShowCommOFF;		//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
extern  int         g_QueryDay;     //��վͨѶ״̬����¼��

extern  int			g_DoubleChannel;	//��վ�Ƿ�ʹ��˫ͨ��ͨ��.0-��ʹ�� .1-ʹ��
extern	COLORREF	g_SCOnly0OFF;		//ֻ��0��ͨ���Ͽ�ʱ����ɫ
extern	COLORREF	g_SCOnly1OFF;		//ֻ��1��ͨ���Ͽ�ʱ����ɫ
extern	COLORREF	g_SCAllOFF;			//����ͨ��ȫ�Ͽ�ʱ����ɫ

extern int			g_ShowDevStatusInStation; //�Ƿ��ڳ�վͨѶͼ����ʾվ��װ�õ�ͨѶ���
extern COLORREF		g_CommMapBK; //����վͨѶ״̬ͼ�α���ɫ
extern CString		g_CommMapFont;//����վͨѶ״��ͼ�г�վ������������
extern int			g_CommMapFontSize;//����վͨѶ״��ͼ�г�վ���������С
extern COLORREF		g_CommMapFontColor;//����վͨѶ״��ͼ�г�վ����������ɫ

extern	int			g_DetailWndHeight;
//����ͼ����
//�����¼�����
extern  int			g_ShowActionDetail;	//�Ƿ���ʾ�����¼���ϸ����
//�澯�¼�����
extern	int			g_ShowAlarmDetail;	//�Ƿ���ʾ�澯�¼���ϸ����
//���ϱ��洰��
extern  int			g_FaultShowWave;//�Ƿ���ʾ¼��ͼ��
//¼������
//��������λ����
//ͨѶ״̬����
extern int			g_RefreshPTCommTime;	//ˢ�±���ͨ��״̬����ʱ��ļ��ʱ��,��λ(����)
extern int			g_ShowPTCommON;			//�б����Ƿ���ʾͨ��״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
extern int			g_ShowPTCommOFF;		//�б����Ƿ���ʾͨ��״̬�Ͽ���װ����Ϣ.0-����ʾ. 1-��ʾ

//����״̬����
extern int			g_RefreshPTRunTime;		//ˢ�±�������״̬����ʱ��ļ��ʱ��,��λ(����)
extern int			g_ShowPTRunON;			//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
extern int			g_ShowPTRunCheck;		//�б����Ƿ���ʾ����״̬���޵�װ����Ϣ.0-����ʾ. 1-��ʾ
extern int			g_ShowPTRunOFF;			//�б����Ƿ���ʾ����״̬ͣ�˵�װ����Ϣ.0-����ʾ. 1-��ʾ
extern int			g_ShowPTRunDebug;		//�б����Ƿ���ʾ����״̬���Ե�װ����Ϣ.0-����ʾ. 1-��ʾ
extern int			g_ShowPTRunCode;		//�б����Ƿ���ʾ����״̬δ�����װ����Ϣ.0-����ʾ. 1-��ʾ

//��վ��Ϣ����
extern int			g_RefreshTime;			//ˢ�±���ͨѶ״̬������״̬����ʱ��ļ��ʱ��,��λ(����)

//ϵͳ�澯�洰��
extern	int			g_ShowSysAlarmDetail; //�Ƿ���ʾϵͳ�澯�¼���ϸ����
//�����ٻ�����
extern	COLORREF	g_BatchCall;	//�����ٻ�ѡ����, "�ٻ�"��������ɫ
extern	COLORREF	g_BatchNotCall;	//�����ٻ�ѡ����, "���ٻ�"��������ɫ
extern  COLORREF	g_BatchNotSupport;//�����ٻ�ѡ����, "��֧��"��������ɫ
extern	COLORREF	g_BatchSuccess;	//�����ٻ������, "�ɹ�"��������ɫ
extern	COLORREF	g_BatchFail;	//�����ٻ������, "ʧ��"��������ɫ
extern	COLORREF	g_BatchNULL;	//�����ٻ������, "����ٻ�"��������ɫ
extern	COLORREF	g_BatchLater;	//�����ٻ������, "��δ�ٻ�"��������ɫ
extern	CString		g_AutoCall_IP;	//�Զ�����ģ�����ڵ�IP��ַ
extern	int			g_AutoCall_Port;//�Զ�����ģ��ļ����˿�

//��������ҳ
extern	int			g_PTSettingValues;	//��ֵ��ʷֵ��������. 0-10����
extern	int			g_PTSettingGroup;	//��ֵ�Ƿ�����ʾ. 0-��, 1-��
extern	int			g_PTSettingCompare; //��ֵ������Ƚ�, 0-��ǰһ��ֵ�Ƚ�, 1-���׼ֵ�Ƚ�
extern	int			g_PTDIValues;		//��������ʷֵ��������. 0-10����
extern	int			g_PTDIStyle;		//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��, 4:����/����.
extern	int			g_PTSoftValues;		//��ѹ����ʷֵ��������. 0-10����
extern	int			g_PTSoftStyle;		//��ѹ��ֵ��ʾ���: 0: 1/0, 1: ON/OFF, 3: Ͷ/��.
extern	int			g_PTAIValues;		//ģ�����Ƿ���ʾ��ʷֵ 0:����ʾ. 1:��ʾ
extern	int			g_PTAIRealtimeValue; //�Ƿ���ʾģ����ʵʱֵ������ʱˢ��
extern	int			g_PTAIRealtimeRefresh; //ģ����ʵʱֵˢ��ʱ��(��)
extern	int			g_PTControl;		//���ƹ����Ƿ���� 0:������. 1:����
extern	int			g_PTGeneralMore;	//��������ҳ���"��ʾ����"��ť�Ƿ���ʾ.0-��, 1-��
extern	int			g_PTActionNum;		//�����¼���ʾ��
extern	int			g_PTAlarmNum;		//�澯�¼���ʾ��
extern	int			g_PTShowHistory;	//��ʷ�¼�ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTNewZoneTable;	//�Ƿ�ʹ���µĶ�ֵ���ű�.Ϊ1ʱʹ��tb_pt_zone_def, Ϊ0ʱʹ��tb_secdev_zone_base
extern	int			g_PTControlModel;	//���ƹ���ģʽ��1-��ͨ��2-�㶫
extern	int			g_PTModifyZone;		//�Ƿ�֧�ֶ�ֵ���л�
extern	int			g_PTShowGeneral;	//������Ϣҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowSetting;	//��ֵҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowDI;			//������ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowSoftBoard;	//��ѹ��ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowAI;			//ģ����ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowAction;		//����ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowAlarm;		//�澯ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowOsc;		//����¼��ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowOscList;	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_PTShowNote;		//��עҳ���Ƿ���ʾ.0-��1-��
extern	int			g_PTSettingJudge;	//��ֵ�޸�ʱ�Ƿ��ж������ޡ�0-���ж���1-�ж�
extern	int			g_PTIndexType;		//��ʾ���ֵ�����͡�0-��ţ�1-��ţ���1��ʼ�ࣩ
extern	int			g_PTNotModifyCurrentZone; //�Ƿ��ֹ�޸ĵ�ǰ��ֵ���Ķ�ֵ�� 0-����ֹ,1-��ֹ
extern	int			g_PTOscListCompare;	//�ٻ�������¼���б��Ƿ��뱾�ر����¼���ļ��Աȡ�0-���Ա�,1-�Ա�
extern	int			g_PTSetting160x; //16���ƶ�ֵ��ʾʱ�Ƿ�ǿ����ǰ�����0x,0-��ǿ�ƣ�������վ�����˵�ֵ������ʾ��1-ǿ�Ƽ���0x

//¼��������ҳ
extern	int			g_WRShowGeneral;	//������Ϣҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowSetting;	//��ֵҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowDI;			//������ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowAI;			//ģ����ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowOsc;		//����¼��ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowOscList;	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
extern	int			g_WRShowNote;

//��ҳ���Ƿ���ʾ����
extern  BOOL		g_bShowActionWnd;		//�Ƿ���ʾ�����¼�����
extern	BOOL		g_bShowAlarmWnd;		//�Ƿ���ʾ�澯�¼�����
extern	BOOL		g_bShowFlautWnd;		//�Ƿ���ʾ���ϱ��洰��
extern	BOOL		g_bShowDIWnd;			//�Ƿ���ʾ����������
extern	BOOL		g_bShowOSCWnd;			//�Ƿ���ʾ¼������
extern	BOOL		g_bShowPTCommWnd;		//�Ƿ���ʾ����ͨѶ״̬����
extern	BOOL		g_bShowPTRunWnd;		//�Ƿ���ʾ��������״̬����
extern	BOOL		g_bShowSysAlarmWnd;		//�Ƿ���ʾϵͳ�澯����
extern	BOOL		g_bShowCrossWnd;		//�Ƿ���ʾԤ������
extern  BOOL		g_bShowSOEIWnd;			//�Ƿ���ʾI���¼�����
extern	BOOL		g_bShowSOEIIWnd;		//�Ƿ���ʾII���¼�����
extern	BOOL		g_bShowSOEIIIWnd;		//�Ƿ���ʾIII���¼�����
extern	BOOL		g_bShowSOEDebugWnd;		//�Ƿ���ʾ�����¼�����
extern	BOOL		g_bShowGeoWnd;			//�Ƿ���ʾ����ͼ����
extern	BOOL		g_bShowDeviceMapWnd;	//�Ƿ���ʾһ�ν���ͼ����
extern	BOOL		g_bShowDistanceWnd;		//�Ƿ���ʾС����ര��
extern	BOOL		g_bShowStationInfoWnd;	//�Ƿ���ʾ��վ��Ϣ����
extern	BOOL		g_bShowRemindWnd;		//�Ƿ���ʾ���Ѵ���
extern	BOOL		g_bShowDraft;//�Ƿ�����ʾʩ��ͼ���ܣ���������һ�ν���ͼ��������һ��"��ʾʩ��ͼ"��ť�������ʾʩ��ͼ


//���ָ����ڴ�С��¼
extern  int		g_nCommSplitterWnd;	//��վͨ�ϴ��ڷָ���С
extern	int		g_nActionSplitterWnd;	//�������ڷָ���С
extern	int		g_nAlarmSplitterWnd;	//�澯���ڷָ���С
extern	int		g_nFaultSplitterWnd;	//���Ϲ鵵���ڷָ���С
extern	int		g_nSysAlarmSplitterWnd;//ϵͳ�澯���ڷָ���С
extern	int		g_nSOESplitterWnd;		//SOE��Ϣ���ڷָ���С
extern	int		g_nCrossSplitterWnd;	//Ԥ�����ڷָ���С

//ʩ��ͼ���ڴ�С
extern int g_nDraftWndWidth;
extern int g_nDraftWndHeight;
//////////////////////////////////////////////////////////////////////////
//ϵͳ����
#define MAX_PATH		260
#define PATH_STWAVE		"..\\STWave\\"	//STWave·��
#define EW_STWAVE		0				//����STWave
#define PATH_XJQUERY	"..\\XJQuery\\" //XJQuery·��
#define EW_XJQUERY		1				//����XJQUERY
#define PATH_XJALARMQUERY	"..\\XJAlarmQuery\\" //XJAlarmQuery·��
#define EW_XJALARMQUERY	2				//����XJALARMQUERY
#define PATH_XJWAVE		"..\\XJWave\\"	//XJWave·��
#define PATH_XJFAULTINVERSION	"..\\XJFaultInversion\\"

extern char g_arrStrTable[2][90];
extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
//��STWave������ϢWM_OPEN_WAVEFILE��֪ͨ��򿪼�������ָ�����ļ�
#define WM_OPEN_WAVEFILE		WM_USER + 1000
//��XJQuery������ϢWM_QUERY_EVENT��֪ͨ��ѯ������ָװ�õ��¼���Ϣ
#define WM_QUERY_EVENT			WM_USER + 999
//��XJAlarmQuery������ϢWM_QUERY_ALARM��֪ͨ��ѯ������ָװ�õ��¼���Ϣ
#define WM_QUERY_ALARM			WM_USER + 5001	
//////////////////////////////////////////////////////////////////////////
//ȫ�ֺ�������

/*
 *  @brief   	WriteLog	 д��־����־�ļ�����־������� 
 *  @param 		[In]a param of Type  CString  ��־�ַ���
 *  @param 		[In]a param of Type  int  ��־�ȼ�
 *  @return 	void 
 */
void WriteLog(CString sLog, int nLV = 3);

void WriteLogTemp(CString sLog, int nLV = 3);

/*
 *  @brief   	WriteLogEx	 д��־����־�ļ� 
 *  @param 		[In]a param of Type  CString  ��־�ַ���
 *  @param 		[In]a param of Type  int  ��־�ȼ�
 *  @return 	void 
 */
void WriteLogEx(CString sLog, int nLV = 3);

/*
 *  @brief   	StringToTime	 �����ݿ��е�19λʱ���ַ���ת��ΪCTime���� 
 *  @param 		[In]a param of Type  CString &  ʱ���ַ���
 *  @return 	CTime ʱ�����
 */
CTime	StringToTime(CString & sTime);

/*
 *  @brief   	StringToTimeSttp15	 ��STTP������15λ��ʱ��ת����CTime���� 
 *  @param 		[In]a param of Type  CString &  ʱ���ַ���
 *  @param 		[Out]a param of Type  int&  ����ֵ
 *  @return 	CTime ʱ�����
 */
CTime	StringToTimeSttp15(CString & sTime, int& nms);

/*
 *  @brief   	StringToTimeSttp12	 ��STTP������12λ��ʱ��ת����CTime���� 
 *  @param 		[In]a param of Type  CString &  ʱ���ַ���
 *  @return 	CTime ʱ�����
 */
CTime	StringToTimeSttp12(CString & sTime);

/*
 *  @brief   	StringToTimeSttp26	 ��STTP������26λ��ʱ��ת����CTime���� 
 *  @param 		[In]a param of Type  CString &  ʱ���ַ���
 *  @param 		[Out]a param of Type  int&  ����ֵ
 *  @return 	CTime ʱ�����
 */
CTime	StringToTimeSttp26(CString & sTime, int& nms);

/*
 *  @brief   	XJToMS	 string�����ַ���ת��ΪCString�ַ��� 
 *  @param 		[In/Out]a param of Type  string  string�ַ���
 *  @return 	CString CString�ַ���
 */
CString XJToMS(string xjStr);

/*
 *  @brief   	MSToXJ	 CString�����ַ���ת��Ϊstring�ַ��� 
 *  @param 		[In]a param of Type  CString  CString�ַ���
 *  @return 	string string�ַ���
 */
string MSToXJ(CString str);

/*
 *  @brief   	ComplexExchange	 ��ָ���ַ���ת���ɸ�����ʽ 
 *  @param 		[In]a param of Type  CString  Ҫת�����ַ���
 *  @return 	CString ����ַ���
 */
CString	ComplexExchange(CString sStr);

/*
 *  @brief   	CompareComplex	 �Ƚ�����������С
 *  @param 		[In]a param of Type  CString  ����1
 *  @param 		[In]a param of Type  CString  ����2
 *  @return 	int  1>2 ����1, 1=2����0, 1<2����-1
 */
int		CompareComplex(CString str1, CString str2);

/*
 *  @brief   	StringToTime14	 �����ݿ���14���ֽڵ�ʱ��ת����CTime���� 
 *  @param 		[In]a param of Type  CString  ʱ���ַ���
 *  @return 	CTime CTime����
 */
CTime	StringToTime14(CString sTime);

/*
 *  @brief   	IsValidKeyString	 �ж�һ�����йؼ��ֵ�����Ƿ���Ч��� 
 *  @param 		[In]a param of Type  CString  �ؼ������
 *  @return 	BOOL ��Ч����TRUE, ��Ч����FALSE
 */
BOOL	IsValidKeyString(CString sKey);

/*
 *  @brief   	FindOutAllKey	 �ҳ�һ����������еĹؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ������
 *  @param 		[Out]a param of Type  CStringArray&  ���ؼ���
 *  @return 	CString 
 */
CString FindOutAllKey( CString sKey, CStringArray& arrString );

/*
 *  @brief   	TranslateKeyInFaultDetail	 ���ϱ���ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInFaultDetail(CString sKey, CString & sValue);

/*
 *  @brief   	TranslateKeyInPTSetting	 ������ֵҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTSetting(CString sKey, CString & sValue);

/*
 *  @brief   	TranslateKeyInPTSetting	 ����ģ����ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTAnalog(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTDigital	 ����������ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTDigital(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTSoftBoard	 ������ѹ��ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTSoftBoard(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInWRSetting	 ¼������ֵҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInWRSetting(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInWRSetting	 ¼����������ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInWRDigital(CString sKey, CString &sValue);
/*
 *  @brief   	TranslateKeyInWRSetting	 ¼����ģ����ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInWRAnalog(CString sKey, CString &sValue);
/*
 *  @brief   	TranslateKeyInPTAction	 ��������ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTAction(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTAlarm	 �����澯ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInPTAlarm(CString sKey, CString &sValue);


/*
 *  @brief   	TranslateKeyInAutoCallResult	 �Զ�����ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int TranslateKeyInAutoCallResult(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInStationComm	 ����վͨ��״̬ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int	TranslateKeyInStationComm(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTComm	 ����ͨ��״̬ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int	TranslateKeyInPTComm(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTRun	 ��������״̬ҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int	TranslateKeyInPTRun(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInStationInfo	 ��վ��Ϣҳ����������ӡ�ؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString &  �ؼ��ֱ�ʾ��ֵ
 *  @return 	int 
 */
int	TranslateKeyInStationInfo(CString sKey, CString &sValue);

/*
 *  @brief   	UpdateBlinkRect	 ˢ����˸ͼ������ 
 *  @param 		[In]a param of Type  CRect  ָ����˸ͼ����Ҫˢ�µ�����
 *  @return 	int ˢ�³ɹ�����1,ʧ�ܷ���0
 */
int UpdateBlinkRect(CRect rcBlink);

/*
 *  @brief   	AckAllEvent	 ȷ��ָ����������¼� 
 *  @param 		[In]a param of Type  int  ָ�����ݱ��ʶ
 *	@param		[In]a param of Type	 int  ָ���¼�����
 *  @return 	BOOL ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 */
BOOL AckAllEvent(int nTable, int nEventType = -1);

/*
 *  @brief   	AckPTAllEvent	 ȷ��ָ�������������¼� 
 *  @param 		[In]a param of Type  int  ָ��Ҫȷ�ϵ��¼���
 *  @param 		[In]a param of Type  CString  ָ������
 *  @return 	BOOL 
 */
BOOL AckPTAllEvent(int nTable, CString sID);


/*
 *  @brief      GetOscFilePath       ȡ��¼���ļ�·��
 *  @param 		[In]a param of Type  CString  ¼���ļ�ȫ·��
 *  @return 	void 
 */
void GetOscFilePath(CString& sFullPath);


/*
 *  @brief   	NeedShow	 �ж�ָ�����͵��¼��Ƿ���Ҫ��ʾ 
 *  @param 		[In]a param of Type  int  �¼�����.0-I��, 1-II��, 2-III��
 *  @return 	BOOL ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 */
BOOL NeedShow(int nEventType);

/*
 *  @brief   	NeedAlarm	 �ж�ָ�����͵��¼��Ƿ���Ҫ���� 
 *  @param 		[In]a param of Type  int  �¼�����.0-I��, 1-II��, 2-III��
 *  @return 	BOOL ��Ҫ��������TRUE, ����Ҫ��������FALSE
 */
BOOL NeedAlarm(int nEventType);

/*
 *  @brief   	GetCommStatusString	 ����״̬����ȡ��ͨѶ״̬�����ַ��� 
 *  @param 		[In]a param of Type  int  ״̬����
 *  @return 	CString ͨѶ״̬�����ַ���
 */
	CString GetCommStatusString(int nStatus);
	/*
 *  @brief   	GetStationStatusString	 ����״̬����ȡ�ó�վͨѶ״̬�����ַ��� 
 *  @param 		[In]a param of Type  int  ״̬����
 *  @return 	CString ͨѶ״̬�����ַ���
 */
CString GetStationStatusString( int nStatus );
/*
 *  @brief   	GetRunStatusString	 ����״̬����ȡ������״̬�����ַ��� 
 *  @param 		[In]a param of Type  int  ״̬����
 *  @return 	CString ����״̬�����ַ��� 
 */
	CString GetRunStatusString(int nStatus);

/*
 *  @brief   	GetSelectDir	 ȡ���û�ѡ����ļ���·�� 
 *  @return 	CString �û�ѡ����ļ���·��
 */
	CString GetSelectDir();


/*
 *  @brief   	OpenWaveFile	 ��¼���ļ� 
 *  @param 		[In]a param of Type  CString  ¼���ļ�ȫ·��
 *  @return 	void 
 */
void OpenWaveFile(CString sFullPath, HWND hWnd);

/*
 *  @brief   	OpenXJQuery	 ���¼���ѯ��ͳ��ģ�� 
 *  @param 		[In]a param of Type  CString  ��������
 *  @param 		[In]a param of Type  HWND  �򿪴���
 *  @return 	void 
 */
void OpenXJQuery(CString sCmd, HWND hWnd);

/*
 *  @brief   	OpenXJAlarmQuery	 �򿪸澯ͳ��ģ�� 
 *  @param 		[In]a param of Type  CString  ��������
 *  @param 		[In]a param of Type  HWND  �򿪴���
 *  @return 	void 
 */
void OpenXJAlarmQuery(CString sCmd, HWND hWnd);

void OpenXJFaultInversion(CString sFullPath, HWND hWnd);


/*
 *  @brief   	GetSituationDescribe	ȡ���Զ�Ѳ���е����˵������	 
 *	@param 		[In]a param of Type  int nSituationNo	���˵�����
 *  @return 	CString	 �Զ�Ѳ���е����˵������
 */
CString GetSituationDescribe( int nSituationNo );

/*
 *  @brief   	GetDetailSituation	 ȡ���Զ�Ѳ���е���ϸ���˵��
 *	@param 		[In]a param of Type  CString sSituation ���˵�������	
 *  @return 	CString	 �Զ�Ѳ���е���ϸ���˵��
 */
CString GetDetailSituation( CString sSituation );

/*
 *  @brief   	GetDisposeOpinion	ȡ���Զ�Ѳ���еĴ����������
 *	@param 		[In]a param of Type  int nOpinion	����������
 *  @return 	CString	 �Զ�Ѳ���еĴ����������
 */
CString GetDisposeOpinion( int nOpinion );

CString GetDetailDispose( CString sDispose );

CString GetDurationFormatStr();

CString StringFromID(UINT id);


//�ļ�����
#define INI_File		"XJBrowser.ini"		//ini,���������ļ�
#define UI_File			"UIConfig.xml"		//xml,���������ļ�(List)
#define LOG_File		"XJBrowser_Log.txt"	//LOG�ļ�
#define SOUND_File		"Virus.wav"			//���������ļ�
#define STATIONCOMM_File "StationComm.svg"	//����վͨѶ״̬ͼ�ļ�
#define IMAGE_FILE		"Image/Background.bmp" //����ͼ����

//��ӡģ��
#define FAULTREPORT_SINGLE_PT	"ReportPrint/FaultReport_Single.xml"	//���ϱ����ӡģ��,������
#define FAULTREPORT_MULTI_PT	"ReportPrint/FaultReport_Multi.xml"	//���ϱ����ӡģ��,�ౣ��
#define FAULTREPORT_MULTI_COLOR	"ReportPrint/FaultReport_Multi_Color.xml"
#define FAULTREPORT_SINGLE_COLOR "ReportPrint/FaultReport_Single_Color.xml"
#define FAULTREPORT_SINGLESTATION_MULTI_PT	"ReportPrint/FaultReport_SingleStation_Multi.xml"	//���ϱ����ӡģ��,���˶ౣ��
#define FAULTREPORT_SINGLESTATION_MULTI_COLOR	"ReportPrint/FaultReport_SingleStation_Multi_Color.xml"//���˶ౣ��

#define FAULTREPORT_MULTI_PT_LANDSCAPE	"ReportPrint/FaultReport_Multi_landscape.xml" //���ϱ����ӡģ��,˫��
#define FAULTREPORT_MULTI_COLOR_LANDSCAPE	"ReportPrint/FaultReport_Multi_Color_landscape.xml" //���ϱ����ӡģ��,˫��

#define SETTING_NEW1_TP		"ReportPrint/Setting_new1.xml" //��ֵ��ӡģ��, ֻ��ӡ����һ��ֵ
#define SETTING_NEW2_TP		"ReportPrint/Setting_new2.xml" //��ֵ��ӡģ��, ��ӡ���¶���ֵ
#define SETTING_NEW3_TP		"ReportPrint/Setting_new3.xml" //��ֵ��ӡģ��, ��ӡ��������ֵ
#define SETTING_ORDER1_TP	"ReportPrint/Setting_order1.xml" //��ֵ��ӡģ��(�л�׼ֵ), ֻ��ӡ����һ��ֵ
#define SETTING_ORDER2_TP	"ReportPrint/Setting_order2.xml" //��ֵ��ӡģ��(�л�׼ֵ), ֻ��ӡ��������ֵ
#define SETTING_ORDER3_TP	"ReportPrint/Setting_order3.xml" //��ֵ��ӡģ��(�л�׼ֵ), ֻ��ӡ��������ֵ

#define ANALOG_NEW1_TP		"ReportPrint/Analog_new1.xml" //ģ������ӡģ��, ֻ��ӡ����һ��ֵ
#define ANALOG_NEW2_TP		"ReportPrint/Analog_new2.xml" //ģ������ӡģ��, ��ӡ���¶���ֵ
#define ANALOG_NEW3_TP		"ReportPrint/Analog_new3.xml" //ģ������ӡģ��, ��ӡ��������ֵ

#define DIGITAL_NEW1_TP		"ReportPrint/Digital_new1.xml" //��������ӡģ��, ֻ��ӡ����һ��ֵ
#define DIGITAL_NEW2_TP		"ReportPrint/Digital_new2.xml" //��������ӡģ��, ��ӡ���¶���ֵ
#define DIGITAL_NEW3_TP		"ReportPrint/Digital_new3.xml" //��������ӡģ��, ��ӡ��������ֵ

#define SOFTBOARD_NEW1_TP	"ReportPrint/SoftBoard_new1.xml" //��ѹ���ӡģ��, ֻ��ӡ����һ��ֵ
#define SOFTBOARD_NEW2_TP	"ReportPrint/SoftBoard_new2.xml" //��ѹ���ӡģ��, ��ӡ���¶���ֵ
#define SOFTBOARD_NEW3_TP	"ReportPrint/SoftBoard_new3.xml" //��ѹ���ӡģ��, ��ӡ��������ֵ

#define WRSETTING_NEW1_TP	"ReportPrint/WRSetting_new1.xml" //¼������ֵ��ӡģ��, ֻ��ӡ����һ��ֵ
#define WRSETTING_NEW2_TP	"ReportPrint/WRSetting_new2.xml" //¼������ֵ��ӡģ��, ��ӡ���¶���ֵ
#define WRSETTING_NEW3_TP	"ReportPrint/WRSetting_new3.xml" //¼������ֵ��ӡģ��, ��ӡ��������ֵ

#define PTACTION_LIST		"ReportPrint/PTActionList.xml"	 //���������¼��б�
#define PTALARM_LIST		"ReportPrint/PTAlarmList.xml"	 //�����澯�¼��б�

#define AUTORESULT_LIST		"ReportPrint/AutoCallResult.xml"	//�Զ����ٽ����ӡģ��

#define STATIONCOMM_LIST	"ReportPrint/StationStatus.xml"		//����վͨ��״̬��ӡģ��
#define STATIONCOMM_LIST_DOUBLE "ReportPrint/StationStatusDouble.xml"
#define PTCOMM_LIST			"ReportPrint/PTCommStatus.xml"		//����ͨ��״̬��ӡģ��
#define PTRUN_LIST			"ReportPrint/PTRunStatus.xml"		//����ͨ��״̬��ӡģ��
#define STATIONINFO_LIST	"ReportPrint/StationInfo.xml"		//��վ���б�����Ϣ��ӡģ��

#define ERROR_NULL			"No error"

//�Զ���ṹ
//��������ֵ tb_pt_action_character
typedef struct PT_ActionCharacter
{
	int		nID;			//����ֵID
	CString strName;		//����ֵ��
	CString strCodeName;	//����ֵ������
	CString strValue;		//����ֵ
	CString strUnits;		//��λ
	int     nEventType;     //�ź�������0 һ���¼� 1 �����¼� 2 �����¼�

	CString	strPT_ID;		//����ID
	int		nCPU;			//����CPU
	const PT_ActionCharacter& operator = (const PT_ActionCharacter& character)
	{
		nID         = character.nID;
		strName     = character.strName;
		strValue    = character.strValue;
		strCodeName = character.strCodeName;
		strUnits	= character.strUnits;
		strPT_ID	= character.strPT_ID;
		nCPU		= character.nCPU;
		nEventType	= character.nEventType;
		return *this;
	}

	PT_ActionCharacter()
	{
		nID         = -1;
		strName     = "";
		strValue    = "";
		strCodeName = "";
		strUnits	= "";
		strPT_ID	= "";
		nCPU		= -1;
		nEventType	= 2;
	}
}PT_ActionCharacter;

//�ź���
//##ModelId=49B87B890263
typedef struct PT_Signal
{
	int	    nID;				//�ź����
	CString strName;		//�ź�����
	CString strCodeName;	//�źŴ�����
	int     nEventType;         //�ź�������0 һ���¼� 1 �����¼� 2 �����¼�  3,ͨѶ�澯�¼�
	CString strTime;			//�źŷ���ʱ��
	int	    nValue;				//�ź�ֵ��1-ON,0-OFF
	int		nRelativeTime;		//���ʱ��
	
	CString strPT_ID;		//����ID
	CString strPTName;		//��������
	CString strPT_Model;	//�����ͺ�
	int		nCPU;			//����CPU��
	
	CString strStationId;   //�������ڳ�վID
	CString strStation;		//�������ڵĳ�վ����
	
    const PT_Signal& operator =(const PT_Signal& sign)
	{
		nID          = sign.nID;     
        strName      = sign.strName;
		strCodeName  = sign.strCodeName;
		nEventType   = sign.nEventType;
		strTime      = sign.strTime;
		nValue       = sign.nValue;
		strPT_ID     = sign.strPT_ID;
		strPTName    = sign.strPTName;
		strPT_Model  = sign.strPT_Model;
		nCPU		 = sign.nCPU;
		strStationId = sign.strStationId;
		strStation   = sign.strStation;
		nRelativeTime = sign.nRelativeTime;
		return *this;
	}
}PT_Signal;

//����CPU TB_SECDEV_CPU_BASE
//##ModelId=49B87B890272
typedef struct PT_CPU
{
	int   code;
	CString   des;
	CString   ver;
	int       from_zone;
	int		  to_zone;
	int       fun;
	
    const PT_CPU& operator =(const PT_CPU& sign)
	{
		code      = sign.code;     
        des       = sign.des;
	//	ver		  = sign.ver;
		from_zone    = sign.from_zone;
		to_zone      = sign.to_zone;
	//	fun			 = sign.fun;
		return *this;
	}
}PT_CPU;

//��ֵ��
typedef struct PT_ZONE
{
	
	int		state; //��ʶ�Ƿ�ǰ��ֵ����,0-��,1-��
	
	CString  PT_ID;//����ID
	int		cpu; //cpu��
	int		code; //��ֵ����
	CString zone_name;
	CString code_name;
	int     group;
	int     item;
	int     minValue;
	int     maxValue;
	int     stepValue;
	int     zoneValue;


	const PT_ZONE& operator =(const PT_ZONE& sign)
	{
		code = sign.code;
		state = sign.state;
		cpu = sign.cpu;
		PT_ID = sign.PT_ID;
		zone_name = sign.zone_name;
		code_name = sign.code_name;
		group = sign.group;
		item = sign.item;
		minValue = sign.minValue;
		maxValue = sign.maxValue;
		stepValue = sign.stepValue;
		zoneValue = sign.zoneValue;
		return *this;
	}

}PT_ZONE;

//��������������
typedef struct PT_DIGITAL
{
	CString	ID;			//������ID
	CString CodeName;	//����������
	CString	Name;		//����������
	int     CpuCode;
	CString	Value;		//ʵ��ֵ

	const PT_DIGITAL& operator =(const PT_DIGITAL& digital)
	{
		ID			= digital.ID;
		CodeName	= digital.CodeName;
		Name		= digital.Name;
		CpuCode     = digital.CpuCode;
		Value		= digital.Value;
		return *this;
	}
}PT_DIGITAL;

//������ѹ�嶨��
typedef struct PT_SOFTBOARD{
	CString ID;			//��ѹ��ID
	CString CodeName;	//��ѹ�����
	CString Name;		//��ѹ������
	CString Value;		//ʵ��ֵ

	const PT_SOFTBOARD& operator =(const PT_SOFTBOARD& softboard)
	{
		ID			= softboard.ID;
		CodeName	= softboard.CodeName;
		Name		= softboard.Name;
		Value		= softboard.Value;
		return *this;
	}
}PT_SOFTBOARD;

//����ģ��������
typedef struct PT_ANALOG{
	CString ID;			//ģ����ID
	CString CodeName;	//ģ��������
	CString Name;		//ģ��������
	CString Value;		//ʵ��ֵ
	CString Unit;		//ֵ��λ
	CString Precision;	//ֵ����

	const PT_ANALOG& operator =(const PT_ANALOG& analog)
	{
		ID			= analog.ID;
		CodeName	= analog.CodeName;
		Name		= analog.Name;
		Value		= analog.Value;
		Unit		= analog.Unit;
		Precision	= analog.Precision;
		return *this;
	}
}PT_ANALOG;

//¼���ļ�����
typedef struct PT_OSC{
	CString FileName;	//�ļ�����,����׺��
	CString	FileType;	//�ļ�����,����׺��
	float	FileSize;	//�ļ���С,��λbytes
	CTime	ModifyTime;	//����޸�ʱ��
	CString	DirPath;	//Ŀ¼·�� 
	int		Code;		//���

	const PT_OSC& operator =(const PT_OSC& osc)
	{
		FileName	= osc.FileName;
		FileType	= osc.FileType;
		FileSize	= osc.FileSize;
		ModifyTime	= osc.ModifyTime;
		DirPath		= osc.DirPath;
		return *this;
	}

}PT_OSC;

typedef struct THREADNODE_TAG{
	int		nNo;
	HANDLE	hThread;
}THREADNODE, *THREADNODE_PTR;

//��ֵ�޸Ķ���
//##ModelId=49B87B8902BF
typedef CTypedPtrArray<CPtrArray, STTP_DATA*> MODIFY_LIST; //�޸�ֵ����

//��ֵ����
//##ModelId=49B87B8902C1
typedef struct PT_SETTING{
	CString ID;			//��ֵID
	CString CodeName;	//��ֵ����
	CString Name;		//��ֵ����
	int		Group;		//���
	CString Value;		//ʵ��ֵ
	CString VPickList;	//ֵ˵��
	CString Unit;		//ֵ��λ
	CString	Precision;	//ֵ���� ��ʽ"2,2"
	int		DataType;   //ֵ���� 0-���� 1������ 2 ������3-�ַ���
	CString OrderValue; //��׼ֵ
	CString minValue;
	CString maxValue;
	CString stepValue;
	int     sValue;
	int		Item;
	int		nRefType;//Ϊ0ʱ��ʾΪ�������ã������޸�
	int		ntimeMStoS;//ֵ���⴦��ֵΪ1ʱ������ʱ����ʾΪ��ʱ��

	const PT_SETTING& operator =(const PT_SETTING& setting)
	{
		ID			= setting.ID;
		CodeName	= setting.CodeName;
		Name		= setting.Name;
		Group		= setting.Group;
		Value		= setting.Value;
		VPickList	= setting.VPickList;
		Unit		= setting.Unit;
		Precision	= setting.Precision;
		DataType	= setting.DataType;
		OrderValue	= setting.OrderValue;
		minValue    = setting.minValue;
		maxValue    = setting.maxValue;
		stepValue   = setting.stepValue;
		sValue      = setting.sValue;
		Item		= setting.Item;
		nRefType	= setting.nRefType;
		ntimeMStoS	 = setting.ntimeMStoS;
		return *this;
	}
}PT_SETTING;

//103������
//##ModelId=49B87B8902CF
typedef struct PT_GROUP{
	int		Group;	//���
	CString	Name;	//����
	const PT_GROUP& operator =(const PT_GROUP& gp)
	{
		Group = gp.Group;
		Name = gp.Name;
		return *this;
	}
}PT_GROUP;

//ϵͳ�澯��ϸ��Ϣ
typedef struct SYSALARM{
	CString		sName; //����
	CString		sOldValue;	//��ֵ
	CString		sNewValue;	//��ֵ
	const SYSALARM& operator =(const SYSALARM& sa)
	{
		sNewValue = sa.sName;
		sOldValue = sa.sOldValue;
		sNewValue = sa.sNewValue;
		return *this;
	}
}SYSALARM;

//������ע��Ϣ
typedef struct PT_NOTE{
	CString id;
	CString pt_id;
	CString note_name;
	CString note_content;
	CTime	create_time;
	CTime	update_time;
	const PT_NOTE& operator =(const PT_NOTE& note)
	{
		id = note.id;
		pt_id = note.pt_id;
		note_name = note.note_name;
		note_content = note.note_content;
		create_time = note.create_time;
		update_time = note.update_time;
	}
}PT_NOTE;

//Listֵ�ĵ�һ����ڶ��в���
const COLORREF XJ_LIST_ONE_TWO		= RGB(255, 0, 0);	//��ɫ
//�ڶ���������еĲ���
const COLORREF XJ_LIST_TWO_THREE	= RGB(255, 0, 255); //�ۺ�
//�������͵�ֵ�û�ɫ����
const COLORREF XJ_LIST_TYPE_CONTROL	= RGB(220, 220, 220);

//����ģʽ
const int MODE_MAIN = 0; //��վ
const int MODE_SUB = 1;//��վ
const int MODE_SEP = 2;	//��վ

//��������ʶ���豸����
/*const int TYPE_BUS		= 1;		//ĸ��
const int TYPE_LINE		= 2;		//��·
const int TYPE_BREAK	= 3;		//����
const int TYPE_GEN		= 4;		//�����
const int TYPE_REACTOR	= 5;		//��ѹ�翹��
const int TYPE_TRANS	= 6;		//��ѹ��
const int TYPE_SWITCH	= 7;		//��բ
const int TYPE_CAP      = 8;		//������
const int TYPE_PMS		= 9;		//���������
const int TYPE_BAY		= 10;		//���

const int TYPE_STATION	= 18;		//��վ
const int TYPE_NET		= 19;		//����

const int TYPE_SEC		= 20;		//�����豸
const int TYPE_PROTECT	= 21;		//���� 
const int TYPE_WAVEREC	= 22;		//¼����*/

//�����豸����Node��Image��Index����
const int IMAGEINDEX_BUS		=1;		//ĸ��
const int IMAGEINDEX_LINE		=2;		//��·
const int IMAGEINDEX_GEN		=3;		//�����
const int IMAGEINDEX_CAP		=14;	//�����
const int IMAGEINDEX_TRANS		=4;		//��ѹ��
const int IMAGEINDEX_REACTOR	=5;		//��ѹ�翹��
const int IMAGEINDEX_SWITCH		=6;		//��բ
const int IMAGEINDEX_BREAK		=7;		//����
const int IMAGEINDEX_STATION	=13;	//��վ8
const int IMAGEINDEX_STATION_BREAK = 8;
const int IMAGEINDEX_PROTECT1	=9;		//����װ��1
const int IMAGEINDEX_PROTECT2	=10;	//����װ��2
const int IMAGEINDEX_WAVEREC	=11;	//¼����
const int IMAGEINDEX_NETAREA    =12;    //����
const int IMAGEINDEX_PMS        =15;    //���������
const int IMAGEINDEX_VOLTAGE    =16;    //��ѹ�ȼ�
const int IMAGEINDEX_SELSEC		=17;	//��ǰѡ��Ķ����豸
const int IMAGEINDEX_DISTANCE	=18;	//���װ��

/*
//���ַ�����ʶ���豸����
#define STRING_BUS		"ĸ��"		//ĸ��
#define STRING_LINE		"��·"		//��·
#define STRING_BREAK	"����"		//����
#define STRING_GEN		"�����"	//�����
#define STRING_CAP		"������"	//������
#define STRING_REACTOR	"��ѹ�翹��"//��ѹ�翹��
#define STRING_TRANS	"��ѹ��"	//��ѹ��
#define STRING_SWITCH	"��բ"		//��բ
#define STRING_PMS      "���������" //���������
#define STRING_BAY		"���"		//���
#define STRING_NET		"����"		//����
#define STRING_STATION	"��վ"		//��վ
#define	STRING_SEC		"�����豸"	//�����豸
#define STRING_PROTECT	"����װ��"	//����װ��
#define STRING_WAVEREC	"¼����"	//¼����
*/ 

//�¼����Ͷ���
int const XJ_EVENT_ACTION	= 0;	//��������
int const XJ_EVENT_ALARM	= 1;	//�����澯
int const XJ_EVENT_FAULT	= 2;	//�¹ʱ���
int const XJ_EVENT_OSC		= 3;	//¼��
int const XJ_EVENT_DI		= 4;	//��������λ��Ϣ
int const XJ_EVENT_PTCOMM	= 5;	//����ͨѶ״̬
int const XJ_EVENT_PTRUN	= 6;	//��������״̬
int const XJ_EVENT_SYSALARM = 7;	//ϵͳ�澯
int const XJ_EVENT_COMM		= 8;	//����վͨѶ״̬
int const XJ_EVENT_DISTANCE	= 9;	//С�����

//�¼���Ϣ���Ͷ���
int const XJ_EVENTINFO_ALL		= 0;	//����
int const XJ_EVENTINFO_NORMAL	= 1;	//������Ϣ
int const XJ_EVENTINFO_DEBUG	= 2;	//������Ϣ

//���ϱ�����¼����Ͷ���
int const XJ_FAULT_EVENT_CHR		= 1; //�����¼�������
int const XJ_FAULT_EVENT_SING		= 2; //�����¼��ź���
int const XJ_FAULT_EVENT_ALARM		= 3; //�澯�¼�
int const XJ_FAULT_EVENT_OSCREPORT	= 4; //¼����
int const XJ_FAULT_EVENT_OSCLIST	= 5; //¼���б�
int const XJ_FAULT_EVENT_WAVEDISTANCE	= 6; //С�����
int const XJ_FAULT_EVENT_OSCPARSE		= 7; //¼������
int const XJ_FAULT_EVENT_DICHANGE	= 8; //��������λ
int const XJ_FAULT_EVENT_DISTANCE	= 9; //���ϲ��

//��ͼ�ඨ��
int const XJ_VIEW_MAP		= 1; //����ͼ��ͼ
int const XJ_VIEW_COMM		= 2; //����վͨѶ��ͼ
int const XJ_VIEW_ACTION	= 3; //�����¼���ͼ
int const XJ_VIEW_ALARM		= 4; //�澯�¼���ͼ
int const XJ_VIEW_FAULT		= 5; //�¹ʱ����¼���ͼ
int const XJ_VIEW_OSC		= 6; //¼���¼���ͼ
int const XJ_VIEW_DI		= 7; //�������¼���ͼ
int const XJ_VIEW_PTCOMM	= 8; //����ͨѶ״̬��ͼ
int const XJ_VIEW_PTRUN		= 9; //��������״̬��ͼ
int const XJ_VIEW_SYSALARM	= 10;//ϵͳ�澯��ͼ
int const XJ_VIEW_PTPRO		= 11;//����������ͼ
int const XJ_VIEW_WRPRO		= 12;//¼����������ͼ
int const XJ_VIEW_DEVPRO	= 13;//����һ���豸������ͼ
int const XJ_VIEW_STAINFO   = 14;//��վ��Ϣ��ͼ

//��־�ȼ�����
int const XJ_LOG_LV1		= 1;	//1����־, ��Ҫ, �����
int const XJ_LOG_LV2		= 2;	//2����־, һ��
int const XJ_LOG_LV3		= 3;	//3����־, ����Ҫ, ������

//��վ��ʼ������
int const STATIONINIT_CHANGE	= 1; //��վ���øı�
int const STATIONINIT_CREATE	= 2; //�½���վ
int const STATIONINIT_DELETE	= 3; //ɾ����վ

//������ܷ��͵���Ϣ
int const	WM_NEWDOCUMENTOPENED	= WM_USER + 1001; //���ĵ���Ϣ
int const	WM_DOCUMENTCLOSEED		= WM_USER + 1002; //�ر��ĵ���Ϣ
int const	WM_STATUS_INFO			= WM_USER + 1003; //״̬����Ϣ�ı�
int const	WM_PTFRAME_CLOSE		= WM_USER + 1004; //����������ͼ�ر�
int const	WM_PTFRAME_OPEN			= WM_USER + 1005; //����������ͼ��
int const	WM_WRFRAME_CLOSE		= WM_USER + 1006; //¼����������ͼ�ر�
int const	WM_WRFRAME_OPEN			= WM_USER + 1007; //¼����������ͼ��
int const	WM_EVENTPROP_CLOSE		= WM_USER + 1008; //�¼����ԶԻ���ر�
int const	WM_EVENTPROP_ACK		= WM_USER + 1009; //�¼����ԶԻ���ȷ���¼����¼�
int const	WM_EVENTLIST_ACK		= WM_USER + 1010; //�¼��б�ȷ���¼�
int const	WM_EVENTPT_ACK			= WM_USER + 1011; //�����¼��б�ȷ���¼�
int const	WM_EVENTPROP_ACK_PT		= WM_USER + 1012; //�¼����ԶԻ���ȷ�ϱ������Դ��ڴ򿪵��¼�
int const	WM_EVENTLIST_PASSIVE_ACK= WM_USER + 1013; //�¼�������ȷ��
int const	WM_EVENTPT_PASSIVE_ACK	= WM_USER + 1014; //��������ҳ���¼��б���ȷ��
int const	WM_WINDOW_REFRESH		= WM_USER + 1015; //֪ͨ����ˢ��
int const	WM_MDITAB_REFRESH		= WM_USER + 1016; //֪ͨTAB��ˢ��
int const	WM_ALL_CLOSE			= WM_USER + 1017; //������ر���Ϣ
int const	FAULTREPORT_SEL_CHANGED = WM_USER + 1018; //���ϱ����б�ѡ��ı���Ϣ
int const	FAULTREPORT_PRINT		= WM_USER + 1019; //���ϱ����ӡ֪ͨ
int const	BATCH_CHECK				= WM_USER + 1020; //�����ٻ�, ��ĳ�豸������ѡ���ȡ��ѡ�����
int const	BATCH_OPTION			= WM_USER + 1021; //�����ٻ�, �ٻ�ѡ�����
int const	BATCH_UNSELECT_ALL		= WM_USER + 1022; //�����ٻ�, ȡ������ѡ��
int const	BATCH_SELECT_ALL_PT		= WM_USER + 1023; //�����ٻ�, ѡ�����б���
int const	BATCH_SELECT_ALL_WR		= WM_USER + 1024; //�����ٻ�, ѡ������¼����
int const	BATCH_FRAME_CLOSE		= WM_USER + 1025; //�����ٻ�, �رմ���
int const	BATCH_FRAME_OPEN		= WM_USER + 1026; //�����ٻ�, �򿪴���
int const	BATCH_DETAIL_UNCHECK	= WM_USER + 1027; //�����ٻ�, ����ϸ��ͼ��ȡ����ĳ�豸ѡ��
int const	BATCH_ENABLE_CONTROLS	= WM_USER + 1028; //�����ٻ�, ��ϸ��ͼ֪ͨ�豸����ͼ�ÿؼ�����/����
int const	PTOSCLIST_CALLFILE_END	= WM_USER + 1029; //�ٻ�¼���ļ����
int const	FAULTREPORT_EDITED		= WM_USER + 1030; //���ϱ�����Ϣ���֪ͨ
int const	FAULTDETAIL_REFRESH		= WM_USER + 1031; //֪ͨ���ϱ�����ϸ��ͼˢ��
int const	AUTOCALL_RESULT_OPEN	= WM_USER + 1032; //�Զ����ٽ�����ڴ���Ϣ
int const	AUTOCALL_RESULT_CLOSE	= WM_USER + 1033; //�Զ����ٽ�����ڹر���Ϣ
int const	AUTOCALL_SET_CHANGE		= WM_USER + 1034; //�Զ��������øı���Ϣ
int const	REFRESH_ACTIONVIEW		= WM_USER + 1035; //ˢ�¶����¼���ʾ
int const	REFRESH_ALARMVIEW		= WM_USER + 1036; //ˢ�¸澯�¼���ʾ
int const	REFRESH_PTSETTING		= WM_USER + 1037; //ˢ�±�����ֵҳ��
int const	REFRESH_PTDI			= WM_USER + 1038; //ˢ�±���������ҳ��
int const	REFRESH_PTSOFTBOARD		= WM_USER + 1039; //ˢ�±�����ѹ��ҳ��
int const	PT_STATUS_CHANGED		= WM_USER + 1040; //����״̬�ı�.
int const	STATION_STATUS_CHANGED	= WM_USER + 1041; //��վ״̬�ı�.
int const	DEVICETREE_HIDE			= WM_USER + 1042; //��ʾ�������豸��, ��0Ϊ����ʱ����, 1Ϊ����ʱ��ʾ
int const	ALARM_SOUND				= WM_USER + 1043; //֪ͨ������������.LPARAMΪ0ʱ֪ͨ��ʼ����,Ϊ1ʱֹ֪ͨͣ����
int const	DOWNLOAD_OPEN			= WM_USER + 1044; //ͨ���ļ����ش��ڴ���Ϣ
int const	DOWNLOAD_CLOSE			= WM_USER + 1045; //ͨ���ļ����ش��ڹر���Ϣ
int const	COMMONFILE_DOWNLOAD_END = WM_USER + 1046; //ͨ���ļ��������֪ͨ
int const	AUTOCALL_RESULT_CHANGE	= WM_USER + 1047; //�Զ����ٽ����ʾ,��վѡ��ı�
int const	STATIONINFO_FRAME_OPEN	= WM_USER + 1048; //��վ��Ϣ���ڴ�
int const	STATIONINFO_FRAME_CLOSE	= WM_USER + 1049; //��վ��Ϣ���ڹر�
int const	AUTOCALL_STATION_CHANGE	= WM_USER + 1050; //�Զ����ٿɲ�����վ�ı�
int const	DOWNOSC_FRAME_OPEN		= WM_USER + 1051; //¼������ҳ���
int const	DOWNOSC_FRAME_CLOSE		= WM_USER + 1052; //¼������ҳ��ر�
int const	COMMVIEW_QUERY_END		= WM_USER + 1053; //�ֶ��ٻ���վͨѶ״̬���
int const	STATION_INFO_CHANGED	= WM_USER + 1054; //��վ���豸ͨ������仯
int const   ITEM_SELECT             = WM_USER + 1055; //CListTreeCtrl itemѡ��֪ͨ
int const   SHOW_BATCH              = WM_USER + 1056; //��ʾ��վ�豸�б� 
int const   CHECK_CHANGE            = WM_USER + 1057; //check״̬�ı�
int	const	BATCHCALL_END			= WM_USER + 1060;//�����ٻ����

int const	STATION_CHANGED	= WM_USER + 1058; //��վ��ͨ������仯
int const   SHOW_RESULT = WM_USER + 1059; 

int const	FAULTREPORT_EXPORT_EXCEL		= WM_USER + 1061; //���ϱ��浼����Excel֪ͨ

int const	TEST_STATIONSTATUS = WM_USER + 1062; //���Գ�վ״̬֪ͨ
int const	ADD_REMIND	= WM_USER + 1063; //����һ����������
int const	PUSHTOP_FVIEW = WM_USER + 1064; //��ָ��ҳ��ǰ��
int	const	THREAD_FILL_DATA = WM_USER + 1065; //���̷߳����ý��������������

int const	WM_DISDEVFRAME_CLOSE		= WM_USER + 1066; //���װ��������ͼ�ر�
int const	WM_DISDEVFRAME_OPEN			= WM_USER + 1067; //���װ��������ͼ��

//ҳ�涨��
int const FVIEW_ACTION = 1;
int const FVIEW_ALARM = 2;
int const FVIEW_OSC = 3;
int const FVIEW_FAULT = 4;
//�����¼���ͼ��Ϣ,��WM_USER+1100��ʼ
int const	VIEWACTION_SELECTCHANGED	= WM_USER +	1100; //������ͼ�б�ѡ��ı�

//�澯�¼���ͼ��Ϣ,��WM_USER+1120��ʼ
int const	VIEWALARM_SELECTCHANGED	= WM_USER + 1120; //�澯��ͼѡ��ı�

//ϵͳ�����¼���ͼ��Ϣ,��WM_USER+1130��ʼ
int const	VIEWSYSALARM_SELECTCHANGED = WM_USER + 1130; //ϵͳ�澯ѡ��ı�

//Ԥ����ͼ��Ϣ,��WM_USER+1105��ʼ
int const	VIEWCROSS_SELECTCHANGED	= WM_USER +	1105; //Ԥ����ͼ�б�ѡ��ı�

//������ͼǿ��ˢ��
int const	VIEW_REFRESH = WM_USER + 1140;
int const	COMM_STATUS_CHANGE = WM_USER + 1141; //ͨѶ״̬�ı�. LPARAM 0-����, 1-�Ͽ�

//����SOE��ͼ֪ͨ
int const	VIEW_SOE_LIST_ACK = WM_USER + 1150;	//��SOE�¼��б�ȷ���¼�
int const	VIEWSOE_SELECTCHANGED = WM_USER + 1153; //SOE�¼��б�ѡ��ı�

//�߳�֪ͨ
int const	THREAD_WORKEND	= WM_USER + 1154; //�̹߳������֪ͨ
int	const	STARTLOCK = WM_USER + 1155;//���Կ�ʼ��ʱ����֪ͨ

//��վ��ʼ��֪ͨ
int	const	STATIONINIT = WM_USER + 1156;//��վ��ʼ��

int const	EXPORT_PROCESS	= WM_USER + 1157; //����������Ϣ
int const	EXPORT_RESULT	= WM_USER + 1158; //���������Ϣ

int const	REFRESH_COMMMAP = WM_USER + 1159; //ˢ������վͨѶ״̬ͼ��

int const	SHOWTOOPTIP = WM_USER + 1160; 

//CJ�ؼ�
int const	IDC_BUTTON_HIDE		= WM_APP + 9000;
int const	IDC_BUTTON_MINI		= WM_APP + 9001;

//�û�����(��ӦTB_SYS_FUNC�е�Func_ID)
#define FUNC_XJBROWSER_BROWSER	 "��������Ӳ鿴"		//�����������ӵ���Ϣ
#define FUNC_XJBROWSER_OPERATE   "��������Ӳ���"		//��ع��ܵ�Ȩ��,�����ٻ���ֵ,������,ģ����,��ѹ�幦��
#define FUNC_XJBROWSER_CONTROL	 "���ƹ���"				//ִ�п��ƹ��ܵ�Ȩ��,������ֵ�޸�,��ֵ���л�,��ѹ��Ͷ��,�����źŸ���,��ʱ,�ٻ�ʱ��.¼������ʱ,Զ������
#define FUNC_XJBROWSER_SIGNAL    "�ź�������"			//�޸�װ���ź������õ�Ȩ��
#define FUNC_XJBROWSER_CUSTODY	 "�໤����"				//�໤���ƹ���ִ��, ,������ֵ�޸�,��ֵ���л�,��ѹ��Ͷ�˵�ִ�м໤
#define FUNC_XJBROWSER_SETTING	 "�������������"		//�����������������

//�����������
#define FUNC_QUY_CALLSETTING		"�ٻ���ֵ"
#define FUNC_QUY_CALLTIME			"�ٻ�װ��ʱ��"
#define FUNC_QUY_CALLDIGITAL		"�ٻ�������"
#define FUNC_QUY_CALLANALOG			"�ٻ�ģ����"
#define FUNC_QUY_CALLHEVENT			"�ٻ���ʷ�¼�"
#define FUNC_QUY_CALLZONE			"�ٻ���ǰ��ֵ����"
#define FUNC_QUY_CALLSOFT			"�ٻ���ѹ��"
#define FUNC_QUY_CALLGENERAL		"�ٻ�װ�û�����Ϣ"
#define FUNC_QUY_CALLOSCLIST		"�ٻ�¼���б�"
#define FUNC_QUY_CALLOSC			"�ٻ�¼���ļ�"
#define FUNC_QUY_CALLFILELIST		"�ٻ�ͨ���ļ��б�"
#define FUNC_QUY_DOWNLOADFILE		"�ٻ�ͨ���ļ�"
#define FUNC_QUY_STATIONSTATUS		"�ٻ���վͨѶ״̬"

#define FUNC_SIGNAL_SETTING			"�ź�������"
#define FUNC_SIGNAL_SYNC			"ͬ����վ�ź�������"

#define FUNC_OPT_SIGNRESET			"�źŸ���"
#define FUNC_OPT_RUNWAVEREC			"Զ������¼����"
#define FUNC_OPT_SETPTTIME			"������ʱ"
#define FUNC_OPT_SETWRTIME			"¼������ʱ"
#define FUNC_OPT_ACKWRALARM			"¼�����澯�¼�ȷ��"

#define FUNC_OPT_CHANGEZONE			"��ֵ���л�"
#define FUNC_OPT_SETSETTING			"�޸Ķ�ֵ"
#define FUNC_OPT_SETSOFTBOARD		"��ѹ��Ͷ��"

/*
#define FUNC_SYS_LOGOUT				"ע���û�"
#define FUNC_SYS_LOGIN				"�û���½"
#define FUNC_SYS_CONNECT			"����ͨ�ŷ�����"
#define FUNC_SYS_DISCONNECT			"�Ͽ�ͨ�ŷ�����"
#define FUNC_SYS_SETTING			"����ϵͳ����"
*/
#define FUNC_SYS_LOGOUT				""
#define FUNC_SYS_LOGIN				""
#define FUNC_SYS_CONNECT			""
#define FUNC_SYS_DISCONNECT			""
#define FUNC_SYS_SETTING			""

//�ٻ��������
extern CString	CANCEL_CALL;
extern CString CANCEL_NULL;
extern CString STTP_DISCONNECT;

//��������
/** @brief			��ֵ���л�ԤУ*/
int const XJ_OPER_CHANGEZONE_PRECORRECTION = 0;
/** @brief			��ֵ���л�ִ��*/
int const XJ_OPER_CHANGEZONE_EXECUTE = 1;
/** @brief			��ֵ�޸�ԤУ*/
int const XJ_OPER_MODIFYSETTING_PRECORRECTION = 2;
/** @brief			��ֵ�޸�ִ��*/
int const XJ_OPER_MODIFYSETTING_EXECUTE = 3;
/** @brief			��ѹ��Ͷ��ԤУ*/
int const XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION = 4;
/** @brief			��ѹ��Ͷ��ִ��*/
int const XJ_OPER_MODIFYSOFTBOARD_EXECUTE = 5;
/** @brief			�źŸ���*/
int const XJ_OPER_SIGNRESET = 6;
/** @brief			Զ����*/
int const XJ_OPER_RemoteStart = 7;
/** @brief			�ٻ���ֵ*/
int const XJ_OPER_CALL_SETTING = 8;
/** @brief			�ٻ�װ��ʱ��*/
int const XJ_OPER_CALL_DEVTIME = 9;
/** @brief			�ٻ�������*/
int const XJ_OPER_CALL_DIGTIAL = 10;
/** @brief			�ٻ�ģ����*/
int const XJ_OPER_CALL_ANALOG = 11;
/** @brief			�ٻ���ʷ�¼�*/
int const XJ_OPER_CALL_HISTORY = 12;
/** @brief			�ٻ���ǰ��ֵ����*/
int const XJ_OPER_CALL_ZONE = 13;
/** @brief			�ٻ���ѹ��*/
int const XJ_OPER_CALL_SOFTBOARD = 14;
/** @brief			�ٻ�װ�û�����Ϣ*/
int const XJ_OPER_CALL_DEVINFO = 15;
/** @brief			�ٻ�¼���б�*/
int const XJ_OPER_CALL_OSCLIST = 16;
/** @brief			�ٻ�¼���ļ�*/
int const XJ_OPER_CALL_OSCFILE = 17;
/** @brief			�ٻ�ͨ���ļ��б�*/
int const XJ_OPER_CALL_COMMFILELIST = 18;
/** @brief			�ٻ�ͨ���ļ�*/
int const XJ_OPER_CALL_COMMFILE = 19;
/** @brief			�ٻ���վͨѶ״̬*/
int const XJ_OPER_CALL_STATIONCOMM = 20;
/** @brief			ȡ���ٻ�*/
int const XJ_OPER_CALL_CANCEL = 21;
/** @brief			������ʱ*/
int const XJ_OPER_CALL_PROTECT_SETTIME = 22;
/** @brief			¼������ʱ*/
int const XJ_OPER_CALL_WAVEREC_SETTIME = 23;
/** @brief			����������û���¼*/
int const XJ_OPER_LOGIN = 24;
/** @brief			����ͨ�ŷ�����*/
int const XJ_OPER_CONNECT = 25;
/** @brief			�Ͽ�ͨ�ŷ�����*/
int const XJ_OPER_DISCONNECT = 26;
/** @brief			����ϵͳ����*/
int const XJ_OPER_CHANGECONF = 27;

/** @brief			�����ɹ�*/
int const OPER_SUCCESS = 0;
/** @brief			����ʧ��*/
int const OPER_FAILD = 1;


/** @brief           �򿪱�������ҳ��ʱ,�����ȡ����ʧ��,���Դ���*/
int const PT_LOAD_COUNT		= 3;
/** @brief           �򿪱�������ҳ��ʱ,�����ȡ����ʧ��,��������������*/
int const PT_LOAD_SLEEP		= 2;

//ϵͳ���ø�ҳ������
int const SYS_SETTING_GENERAL = 0;
int const SYS_SETTING_INFOCASE = 1;
int const SYS_SETTING_STATUS = 2;
int const SYS_SETTING_PTPROP = 3;

//��������ҳ�и�ҳ������
int const PT_PROP_GENERAL	= 0;
int const PT_PROP_SETTING	= 1;
int const PT_PROP_DIGITAL	= 2;
int const PT_PROP_SOFT		= 3;
int const PT_PROP_ANALOG	= 4;
int const PT_PROP_ACTION	= 5;
int const PT_PROP_ALARM		= 6;
int const PT_PROP_HISTORY	= 7;
int const PT_PROP_OSC		= 8;
int const PT_PROP_OSCLIST	= 9;
int const PT_PROP_NOTE		= 10;

//¼��������ҳ�и�ҳ������
int const WR_PROP_GENERAL	= 0;
int const WR_PROP_SETTING	= 1;
int const WR_PROP_DIGITAL	= 2;
int const WR_PROP_ANALOG	= 3;
int const WR_PROP_OSC		= 4;
int const WR_PROP_OSCLIST	= 5;
int const WR_PROP_NOTE		= 6;

//ͨѶ�������
int const	XJ_CLIENT_BROWSER		= 1;

//STTP������ض���
#define INQ 'I'		//��Ϣ����INQ��
#define RES 'R'		//response�͵�
#define EX  'E'   //��չ��

#define FIX 'F'  //��Ϣ����Ϊ�̶���
#define VAR 'V'  //�䳤

//////////////////////////////////////////////////////////////////////////
//��Ϣʹ��SendMessage����,�Ա�֤����Ϣ������֮ǰ���ݵĲ�����Ч
//SendMessage�ĵڶ�������(LPARAM lparam)�зű������ݽṹ(STTP_FULL_DATA)ָ��

//��ӦSTTP������Ϣ
const long WM_STTP_FIRST		=WM_USER +2000;	//��ʼ��Ϣ
//�ٻ���Ӧ���
const long WM_STTP_00901		=WM_USER +901;  //�����ɹ�����
const long WM_STTP_20008		=WM_USER +2008;	//����20008���ġ�������ģ��������ֵ����֪ͨ
const long WM_STTP_20012		=WM_USER +2012;	//����20012���ġ�������������֪ͨ
const long WM_STTP_20016		=WM_USER +2016; //����20016���ġ���������ֵ����֪ͨ
const long WM_STTP_20026		=WM_USER +2026; //����20026����----¼���б�����֪ͨ
const long WM_STTP_20043		=WM_USER +2043;	//����20043����----¼���ļ��������֪ͨ
const long WM_STTP_20048		=WM_USER +2048; //����20048���ġ���������ֵ���л�֪ͨ
const long WM_STTP_20170		=WM_USER +2170; //����20048���ġ���������ֵ���л�֪ͨ
const long WM_STTP_20052		=WM_USER +2052; //����20052���ġ���������ֵ�޸�֪ͨ
const long WM_STTP_20054		=WM_USER +2054; //����20054���ġ���������ֵ�޸�ִ��֪ͨ
const long WM_STTP_20056		=WM_USER +2056; //����20056���ġ���������ֵ�л�ִ��֪ͨ
const long WM_STTP_20172		=WM_USER +2172; //����20056���ġ���������ֵ�л�ִ��֪ͨ
const long WM_STTP_20058		=WM_USER +2058;	//����20058���ġ��������źŸ���֪ͨ
const long WM_STTP_20060		=WM_USER +2060;	//����20060���ġ���������ʱ֪ͨ 
const long WM_STTP_20069		=WM_USER +2069;	//����20069���ġ������Ʊ���ͨ�÷���֪ͨ
const long WM_STTP_20089		=WM_USER +2089; //����20089����----�ٻ�װ��ʱ��֪ͨ
const long WM_STTP_20107		=WM_USER +2107;	//����20107���ġ����ٻ�������ǰ��ֵ����֪ͨ
const long WM_STTP_20174		=WM_USER +2174;	//����20174���ġ����ٻ�������ǰ��ֵ����֪ͨ
const long WM_STTP_20109		=WM_USER +2109;	//����20109���ġ����ٻ�������ѹ��֪ͨ
const long WM_STTP_20111		=WM_USER +2111;	//����20111���ġ����ٻ�������Ϣ֪ͨ
const long WM_STTP_20114		=WM_USER +2114;	//����20114���ġ�����ѹ��Ͷ��ִ��֪ͨ
const long WM_STTP_20116		=WM_USER +2116; //����20116����----������ʷ�����¼�֪ͨ
const long WM_STTP_20118		=WM_USER +2118;	//����20118���ġ�����ѹ��Ͷ��֪ͨ
const long WM_STTP_20124		=WM_USER +2124;	//����20124���ġ���¼����Զ����������֪ͨ
const long WM_STTP_20125		=WM_USER +2125;	//����20125���ġ������ܲ�֧��֪ͨ
const long WM_STTP_20154		=WM_USER +2154; //����20154����----�����ٻ���Ӧ
const long WM_STTP_20157		=WM_USER +2157; //����20157����----��ʱ����֪ͨ
const long WM_STTP_20004        =WM_USER +2004; //����20004����----�ٻ�����ͨѶ״̬
const long WM_STTP_20086        =WM_USER +2086; //����20086����----�ٻ�¼����ͨѶ״̬
//�Զ�������ر���
const long WM_STTP_20002		=WM_USER + 2002; //����20002����,����ͨѶ״̬����֪ͨ
const long WM_STTP_20006		=WM_USER + 2006; //����20006����,����ģ��������ֵ����֪ͨ
const long WM_STTP_20010		=WM_USER + 2010; //����20010����,��������������ֵ����֪ͨ
const long WM_STTP_20014		=WM_USER + 2014; //����20014����,������ֵ����֪ͨ
const long WM_STTP_20018		=WM_USER + 2018; //����20018����,�����澯�¼�����֪ͨ
const long WM_STTP_20022		=WM_USER + 2022; //����20022����,���������¼�����֪ͨ
const long WM_STTP_20079		=WM_USER + 2079; //����20079����,װ������״̬����֪ͨ
const long WM_STTP_20084		=WM_USER + 2084; //����20084����,¼����ͨѶ״̬����֪ͨ
const long WM_STTP_20132		=WM_USER + 2132; //����20132����,¼���ļ��Զ�����֪ͨ
const long WM_STTP_20138		=WM_USER + 2138; //����20138����,��վ���ñ仯֪ͨ
const long WM_STTP_20139		=WM_USER + 2139; //����20139����,��ֵ���ű仯����֪ͨ
const long WM_STTP_20141		=WM_USER + 2141; //����20141����,��ѹ��仯����֪ͨ
const long WM_STTP_20151		=WM_USER + 2151; //����20151����,�Զ��鵵��Ϣ���仯֪ͨ
const long WM_STTP_20144		=WM_USER + 2144; //����20144����,��վͨѶ״̬�仯����֪ͨ
const long WM_STTP_20155		=WM_USER + 2155; //����20155����,�����ٻ���Ϣ�仯�澯֪ͨ
const long WM_STTP_20159		=WM_USER + 2159; //����20159����,Ԥ���ṹ֪ͨ
const long WM_STTP_20146		=WM_USER + 2146; //����20146����,��վͨѶ״̬����֪ͨ
const long WM_STTP_20176		=WM_USER + 2176; //����20176����,ϵͳ�澯
const long WM_STTP_20168		=WM_USER + 2168; //����20168����,С�����֪ͨ
const long WM_STTP_20177		=WM_USER + 2177; //����20177����,վ�㱾������Ҫ��
//const long WM_GETTIP            =WM_USER + 8888;
//
const long WM_STTP_LAST			=WM_USER +2999;	//������Ϣ
//ͨ���ļ�����
const long WM_STTP_00204		=WM_USER + 30204; //����00204����,ͨ���ļ���Ϣһ��֪ͨ
const long WM_STTP_00212		=WM_USER + 30212; //����00212����,�ļ����سɹ�֪ͨ

void AddNoEraseControl(CWnd* pWnd, CRgn & bgRgn, int nID);
void EraseBK(CDC* pDC, CRect bgRect, CRgn & bgRgn, COLORREF colBK);

void GetFirstLetter(CString strName, CString& strFirstLetter);
CString FirstLetter(int nCode);

#ifndef XJHANDLE
#define XJHANDLE HANDLE
#endif

#ifndef XJBROWSER
#define XJBROWSER 
#endif

BOOL InitSVGComNoReg(IClassFactory*& pFactory, ISvgViewer*& pSvgDoc);
BOOL InitReportComNoReg(IClassFactory*& pFactory, IXJReport*& pReportDoc);

CString QueryDIName(CString sPTID, int nDIID, int nCPU);

int GetOperationNum();

/*
 *  @brief   	DelPostfixFromPath	 ��·����ȥ���ļ���׺
 *	@param 		[In]a param of Type  CString & str	
 *  @return 	BOOL	 
 */
BOOL DelPostfixFromPath(CString& str);

CSize OpenReportFile(IXJReport*& pDoc, CString filename);

const COLORREF colBG = GetSysColor(COLOR_3DFACE);
const UINT WM_PAINTMYCAPTION = WM_USER+5;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <afxtempl.h>
#include <afxpriv.h>
#include <..\src\afximpl.h>
#include <Afxcview.h>

#define AFX_IDW_SIZEBAR_LEFT	AFX_IDW_DOCKBAR_LEFT	+ 4
#define AFX_IDW_SIZEBAR_RIGHT	AFX_IDW_DOCKBAR_RIGHT	+ 5
#define AFX_IDW_SIZEBAR_TOP		AFX_IDW_DOCKBAR_TOP		+ 6
#define AFX_IDW_SIZEBAR_BOTTOM	AFX_IDW_DOCKBAR_BOTTOM	+ 7

const DWORD dwSizeBarMap[4][2] =
{
	{ AFX_IDW_SIZEBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_SIZEBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_SIZEBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_SIZEBAR_RIGHT,    CBRS_RIGHT  },
};
#define safe_delete(p){if(p){delete p;p=NULL;}}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_)
