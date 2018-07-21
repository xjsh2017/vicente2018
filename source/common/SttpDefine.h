/********************************************************************
	created:	2008/09/04
	created:	3:9:2008   11:55
	file base:	SttpDefine
	file ext:	h
	author:		qingch
	description:
		
*********************************************************************/
#ifndef SttpDefine_h__
#define SttpDefine_h__

#include "define.h"
#include "XJString.h"
#include "XJSttpNetAPI.h"

/*****************************常量定义***********************************/

            

/**
 * @brief       STTP客户端运行模式定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        同步模式*/
	const int STTPNET_RUNMODLE_SYNC   = 0;
	/** @brief        异步模式*/
	const int STTPNET_RUNMODLE_ASYNC  = 1;

/**
 * @brief       STTP客户端类型定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
	
	/** @brief        服务端子站*/
	const int STTPNET_MODLE_SUBSTATION_SERVER	= 1;
	/** @brief        主站*/
	const int STTPNET_MODLE_MAINSTATION			= 2;	
	/** @brief        其他*/
	const int STTPNET_MODLE_OTHERSYSTEM			= 3;
	/** @brief        录播联网子站*/
	const int STTPNET_MODLE_SUBSTATION_RECORDER	= 4;	
	/** @brief        客户端子站*/
	const int STTPNET_MODLE_SUBSTATION_CLIENT	= 5;
	/** @brief        服务端分站*/
	const int STTPNET_MODLE_BRASTATION_SERVER	= 6;
	/** @brief        分站之子站*/
	const int STTPNET_MODLE_SUBSTATION_BRANCH	= 7;
	/** @brief        支持录播联网的服务端子站*/
	const int STTPNET_MODLE_SUBSTATION_SRV_OSC	= 8;	
	/** @brief        61850子站*/
	const int STTPNET_MODLE_SUBSTATION_61850	= 9;
	/** @brief        运行与监视*/
	const int STTPNET_MODLE_XJBROWSER			= 10;
	/** @brief        自动归档*/
	const int STTPNET_MODLE_XJPACKAGE			= 11;
	/** @brief        定值校核前台*/
	const int STTPNET_MODLE_XJCHECKMANAGER		= 12;
	/** @brief        定值校核后台*/
	const int STTPNET_MODLE_XJAUTOCHECK			= 13;
	/** @brief        工程建模工具*/
	const int STTPNET_MODLE_XJMODELINGTOOL		= 14;
	/** @brief        自动总召*/
	const int STTPNET_MODLE_XJAUTOCALLTOTAL		= 15;
	/** @brief        WEB同步*/
	const int STTPNET_MODLE_XJWEBRECEIVER		= 16;
	/** @brief        查询与统计*/
	const int STTPNET_MODLE_XJQUERY				= 17;	
	/** @brief        电厂预警*/
	const int STTPNET_MODLE_XJANTIDEFECT		= 18;
	/** @brief        调度员界面*/
	const int STTPNET_MODLE_XJDISPATCHER		= 19;
	/** @brief        数据转发服务模块 */
	const int STTPNET_MODEL_XJDATAROUTER        = 20;
	/** @brief        录波分析服务模块 */
	const int STTPNET_MODEL_WAVE_ANALYST        = 21;
	/** @brief        数据库接口*/
	const int STTPNET_MODLE_XJDBFACADE			= 100;
	/** @brief        采集器*/
	const int STTPNET_MODLE_STFRONT				= 101;
	/** @brief        通信服务器*/
	const int STTPNET_MODLE_XJCOMMUSERVER		= 102;
	/** @brief        断面数据客户端*/
	const int STTPNET_MODLE_SECTIONDATACLIENT	= 103;
	/** @brief        双端测距模块*/
	const int STTPNET_MODEL_XJFAULTLOCATION     = 104; //add by lipeng
	/** @brief        录波故障分析模块*/
	const int STTPNET_MODEL_XJLIPROCESS			= 105; 
	/** @brief        子站测试仪模块*/
	const int STTPNET_MODEL_SUBSTATIONTEST		= 106; 	
	/** @brief        录播统一平台*/
	const int STTPNET_MODLE_UNIQUERECORDER		= 107;	
	/** @brief        辅助决策系统 */
	const int STTPNET_MODLE_AUXILIARYDECISION	= 108;	
	/** @brief        稳控系统 */
	const int STTPNET_MODLE_STABLECONTROL		= 109;	
	/** @brief        智能预警系统 */
	const int STTPNET_MODLE_SMARTPREWARNING		= 110;
	/** @brief        消息总线客户端 */
	const int STTPNET_MODLE_MSGBUSSERVER        = 111;
	/** @brief        通讯状态监视模块 */
	const int STTPNET_MODLE_STATUSMONITER       = 112;	
	/** @brief        测距系统2010子站*/
	const int STTPNET_MODLE_SUBSTATION_WFL2010  = 113;  
	/** @brief        测距系统2012子站*/
	const int STTPNET_MODLE_SUBSTATION_WFL2012 =  114;
	/** @brief        web同步发送端*/
	const int STTPNET_MODLE_WEB_PROVIDER_SENDER = 115;

/**
 * @brief       STTP客户端连接状态定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        中断*/
	const int STTPNET_UNCONNECTED   = 0;
	/** @brief        连接*/
	const int STTPNET_CONNECTED  = 1;

/**
 * @brief       STTP客户端注册回调函数功能码定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        连接变化*/
	const int STTPNET_CALLBACK_ONCONNECTCHANGE   = 0;


/**
 * @brief       数据类型定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           浮点型*/
	const UINT EX_STTP_DATA_TYPE_FLOAT      = 0;
	/** @brief           整型*/
	const UINT EX_STTP_DATA_TYPE_INT        = 1;
	/** @brief           控制类型*/
	const UINT EX_STTP_DATA_TYPE_CONTRL     = 2;
	/** @brief           字符型*/
	const UINT EX_STTP_DATA_TYPE_STRING     = 3;
	/** @brief           时间类型*/
	const UINT EX_STTP_DATA_TYPE_TIME       = 4;

/**
 * @brief       配置类型定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           配置结构描述*/
	const UINT EX_STTP_CONFIG_FIELD_DES     = 1;
	/** @brief           静态数据*/
	const UINT EX_STTP_CONFIG_STATIC_DATA   = 2;
	/** @brief           动态数据*/
	const UINT EX_STTP_CONFIG_DYNAMIC_DATA  = 3;

/**
 * @brief       SQL语句关键字定义
 * @author      李鹏
 * @version     ver1.0
 * @date        18/09/2008
 *
 */
	/** @brief           TOP时的数据类型*/
	const UINT EX_STTP_DATA_TYPE_TOP		= 99;
	
	/** @brief           非正常查询时的数据类型*/
	const UINT EX_STTP_WHERE_ABNORMALITY    = 98;


/**
 * @brief       功能类型定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           全部数据*/
	const UINT EX_STTP_FUN_TYPE_FULL        = 1;
	/** @brief           部分数据*/
	const UINT EX_STTP_FUN_TYPE_PART		= 2;
	/** @brief           添加数据*/
	const UINT EX_STTP_FUN_TYPE_ADD		    = 3;
	/** @brief           更新数据*/
	const UINT EX_STTP_FUN_TYPE_UPDATE		= 4;
	/** @brief           删除数据*/
	const UINT EX_STTP_FUN_TYPE_DELETE      = 5;

/**
 * @brief       信息类型定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           无效配置*/
	const UINT EX_STTP_INFO_INVALID=0;
	/** @brief           本地配置*/
	const UINT EX_STTP_INFO_LOCAL_CFG=1;
	/** @brief           站配置*/
	const UINT EX_STTP_INFO_STATION_CFG=2;                
	/** @brief           自动化配置*/
	const UINT EX_STTP_INFO_AUTOMATION_CFG=3;              
	/** @brief           母线配置*/
	const UINT EX_STTP_INFO_BUS_CFG=4;                     
	/** @brief           开关配置信息*/
	const UINT EX_STTP_INFO_BREAKER_CFG=5;                
	/** @brief           刀闸配置信息*/
	const UINT EX_STTP_INFO_SWITCH_CFG=6;                
	/** @brief           线路配置信息*/
	const UINT EX_STTP_INFO_LINE_CFG=7;                   
	/** @brief           变压器配置*/
	const UINT EX_STTP_INFO_TRANS_CFG=8;                   
	/** @brief           发电机配置*/
	const UINT EX_STTP_INFO_GEN_CFG=9;                   
	/** @brief           电抗器配置*/
	const UINT EX_STTP_INFO_REACTOR_CFG=10;              
	/** @brief           电容器配置*/
	const UINT EX_STTP_INFO_CAPACITOR_CFG=11;            
	/** @brief           二次设备配置 保护、录波器*/
	const UINT EX_STTP_INFO_SECDEV_CFG=12;               
	/** @brief           装置CPU配置*/        
	const UINT EX_STTP_INFO_PT_CPU_CFG=13;               
	/** @brief           站间通讯规约配置*/
	const UINT EX_STTP_INFO_COMMU_PROTOCOL_CFG=14;       
	/** @brief           信息定制配置*/
	const UINT EX_STTP_INFO_BOOK_INFO_CFG=15;            
	/** @brief           功能超时配置*/
	const UINT EX_STTP_INFO_FUN_TIMEOUT_CFG=16;          
	/** @brief           主站自动上送信息定制*/
	const UINT EX_STTP_INFO_COMMU_DEV_MAINSTATION_CFG=17;
	/** @brief           站间103组号配置*/
	const UINT EX_STTP_INFO_COMMU_103GROUP_CFG=18;       
	/** @brief           特征量配置信息Tb_pt_character_def ,针对保护*/
	const UINT EX_STTP_INFO_PT_CHARCTOR_DEF_CFG=19;   
	/** @brief           双机双网配置*/
	const UINT EX_STTP_INFO_COMMU_DOUBLESERVER_CFG=20;
	/** @brief           告警配置信息*/
	const UINT EX_STTP_INFO_PT_ALARM_CFG=21;          
	/** @brief           动作配置信息*/
	const UINT EX_STTP_INFO_PT_ACTION_CFG=22;         
	/** @brief           定值配置*/
	const UINT EX_STTP_INFO_PT_SETTING_CFG=23;        
	/** @brief           开关量配置信息*/
	const UINT EX_STTP_INFO_PT_DI_CFG=24;             
	/** @brief           模拟量配置信息*/
	const UINT EX_STTP_INFO_PT_AI_CFG=25;             
	/** @brief           软压板配置信息*/
	const UINT EX_STTP_INFO_PT_SOFTBOARD_CFG=26;      
	/** @brief           系统共用的特征表*/
	const UINT EX_STTP_INFO_CHARCTOR_CFG=27;          
	/** @brief           装置定值区号配置*/ 
	const UINT EX_STTP_INFO_PT_ZONE_CFG=28;           
	/** @brief           装置通讯状态配置*/ 
	const UINT EX_STTP_INFO_PT_COMMU_CFG=29;          
	/** @brief           装置运行状态配置*/
	const UINT EX_STTP_INFO_PT_RUN_CFG=30;            
	/** @brief           保护录波模拟量通道配置*/
	const UINT EX_STTP_INFO_PT_OSC_AI_CFG=31;         
	/** @brief           保护录波开关量配置*/
	const UINT EX_STTP_INFO_PT_OSC_DI_CFG=32;         
	/** @brief           保护通用分类服务配置*/
	const UINT EX_STTP_INFO_PT_103GROUP_CFG=33;       
	/** @brief           GPS基本配置(暂不使用)*/
	const UINT EX_STTP_INFO_GPS_CFG=34;               
	/** @brief           ABB保护厂家事件配置*/
	const UINT EX_STTP_INFO_ABB_EVENT_CFG=35;         
	/** @brief           采集器轮训周期配置*/
	const UINT EX_STTP_INFO_FRONT_QUERY_CFG=36;       
	/** @brief           端口配置*/
	const UINT EX_STTP_INFO_PORT_CFG=37;              
	/** @brief           采集器规约配置表*/
	const UINT EX_STTP_INFO_FRONT_PROTOCOL_CFG=38;    
	/** @brief           录波文件列表数据*/
	const UINT EX_STTP_INFO_OSC_LIST=39;              
	/** @brief           采集器二次设备配置表*/      
	const UINT EX_STTP_INFO_FRONT_SECDEV_CFG=40;      
	/** @brief           告警数据*/ 
	const UINT EX_STTP_INFO_PT_ALARM_DATA=41;                 
	/** @brief           动作数据*/
	const UINT EX_STTP_INFO_PT_ACTION_DATA=42;                
	/** @brief           软压板数据*/
	const UINT EX_STTP_INFO_PT_SOFTBOARD_DATA=43;              
	/** @brief           定值数据*/
	const UINT EX_STTP_INFO_PT_SETTING_DATA=44;               
	/** @brief           模拟量*/
	const UINT EX_STTP_INFO_PT_AI_DATA=45;                    
	/** @brief           开关量*/
	const UINT EX_STTP_INFO_PT_DI_DATA=46;                    
	/** @brief           录波简报*/
	const UINT EX_STTP_INFO_OSC_REPORT_DATA=47;
	/** @brief           系统维护信息*/
	const UINT EX_STTP_INFO_SYS_MAINTENANCE_DATA=48;
	/** @brief           地区配置信息*/
	const UINT EX_STTP_INFO_NET_CFG=49;
	/** @brief           一次设备—保护管理机基本信息*/
	const UINT EX_STTP_INFO_PMS_CFG=50;
	/** @brief           一次设备—间隔基本信息*/
	const UINT EX_STTP_INFO_BAY_CFG=51;
	/** @brief           用户组信息*/
	const UINT EX_STTP_INFO_SYS_GROUP_CFG=52;
	/** @brief           用户信息*/
	const UINT EX_STTP_INFO_SYS_USER_CFG=53;
	/** @brief           系统功能信息*/
	const UINT EX_STTP_INFO_SYS_FUNC_CFG=54;
	/** @brief           组功能权限关联信息*/
	const UINT EX_STTP_INFO_SYS_GROUP_FUNC_CFG=55;
	/** @brief           开关与一次设备关联信息*/
	const UINT EX_STTP_INFO_BREAKER_DEVICE_CFG=56;
	/** @brief           定值区号配置信息*/
	const UINT EX_STTP_INFO_ZONE_BASE_CFG=57;
	/** @brief           故障录波器开关量变位信配置信息*/
	const UINT EX_STTP_INFO_SOE_CFG=58;
	/** @brief           人工操作历史查询*/
	const UINT EX_STTP_INFO_OPERATION_CFG=59;
	/** @brief           文件下载信息*/
	const UINT EX_STTP_INFO_DOWNLOAD_FILE_CFG=60;
	/** @brief           装置型号配置信息*/
	const UINT EX_STTP_INFO_PT_TYPE_CFG=61;
	/** @brief           系统告警信息*/
	const UINT EX_STTP_INFO_SYS_ALARM_CFG=62;
	/** @brief           菜单信息*/
	const UINT EX_STTP_INFO_MENU_INFO_CFG=63;
	/** @brief           开关量变化信息*/
	const UINT EX_STTP_INFO_CHANGE_DI_CFG=64;
	/** @brief           表名信息*/
	const UINT EX_STTP_INFO_TABLE_INFO_CFG=65;
	/** @brief           域名信息*/
	const UINT EX_STTP_INFO_FIELD_INFO_CFG=66;
	/** @brief           6185报告控制块信息表*/
	const UINT EX_STTP_INFO_61850_REPORT_CFG=67;
	/** @brief           61850配置中转信息*/
	const UINT EX_STTP_INFO_61850_TEMP_CFG=68;
	/** @brief           定值校核基准定值信息*/
	const UINT EX_STTP_INFO_ST_ORDERSETTING_CFG=69;
	/** @brief           定值校核结果日志信息*/
	const UINT EX_STTP_INFO_ST_CHECKSETTING_CFG=70;
	/** @brief           定值校核误差定值信息*/
	const UINT EX_STTP_INFO_ST_ERRORSETTING_CFG=71;
	/** @brief           定值基准修改日志信息*/
	const UINT EX_STTP_INFO_ST_OPERATION_CFG=72;
	/** @brief           定值校核保护配置信息*/
	const UINT EX_STTP_INFO_ST_CHECKPROTECT_CFG=73;
	/** @brief           装置模型信息*/
	const UINT EX_STTP_INFO_PT_MODEL_CFG=74;
	/** @brief           录波文件记录信息(暂不使用)*/
	const UINT EX_STTP_INFO_WAVE_FILE_CFG=75;
	/** @brief           主站与子站通讯状态记录信息*/
	const UINT EX_STTP_INFO_STATION_STATUS_CFG=76;
	/** @brief           装置模板和元件模板的关系信息*/
	const UINT EX_STTP_INFO_DDEFUDEF_CFG=77;
	/** @brief           特性图形模板定义信息*/
	const UINT EX_STTP_INFO_DLL_CFG=78;
	/** @brief           保护装置关联信息*/
	const UINT EX_STTP_INFO_TBDUT_CFG=79;
	/** @brief           装置模板定义信息*/
	const UINT EX_STTP_INFO_TBDUTDEF_CFG=80;
	/** @brief           元计模板IEEE功能号信息*/
	const UINT EX_STTP_INFO_TBFUNCTION_CFG=81;
	/** @brief           保护装置厂家信息*/
	const UINT EX_STTP_INFO_MANUFACTORY_CFG=82;
	/** @brief           反时限特性参数信息*/
	const UINT EX_STTP_INFO_TBREVERSE_CFG=83;
	/** @brief           元件和定值关联关系信息*/
	const UINT EX_STTP_INFO_SETTINGRELATION_CFG=84;
	/** @brief           元件关联信息*/
	const UINT EX_STTP_INFO_TBUNIT_CFG=85;
	/** @brief           元件模板信息*/
	const UINT EX_STTP_INFO_TBUNITDEF_CFG=86;
	/** @brief           元件特征图形关联信息*/
	const UINT EX_STTP_INFO_UNITDEFDLL_CFG=87;
	/** @brief           特征图形包含的特征值信息*/
	const UINT EX_STTP_INFO_UNITPARADEF_CFG=88;
	/** @brief           元件特征图形关联的特征值数值信息*/
	const UINT EX_STTP_INFO_UNITPARAVALUE_CFG=89;
	/** @brief           通道配置信息*/
	const UINT EX_STTP_INFO_ACHANNEL_CFG=90;
	/** @brief           信息归档的主表(暂不使用)*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_CFG=91;
	/** @brief           故障归档和其他事件的关系信息(暂不使用)*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_STEVENT_CFG=92;
	/** @brief           信息归档配置信息*/
	const UINT EX_STTP_INFO_FAULTSHORTENCONFIG_CFG=93;
	/** @brief           信息归档数据*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_DATA=94;
	/** @brief           信息归档事件信息*/
	const UINT EX_STTP_INFO_FAULTSHORTENEVENT_CFG=95;
	/** @brief          小波测距信息*/
	const UINT EX_STTP_INFO_DISTANCE_CFG=96;
	/** @brief           小波测距系统线路编号信息*/
	const UINT EX_STTP_INFO_DISTANCE_LINE_CFG=97;
	/** @brief           厂站SVG文件信息*/
	const UINT EX_STTP_INFO_STATION_SVG_CFG=98;
	/** @brief           自动召唤结果信息*/
	const UINT EX_STTP_INFO_OC_RESULT_CFG=99;
	/** @brief           自动总召装置选择配置信息*/
	const UINT EX_STTP_INFO_OC_CONFIG_CFG=100;
	/** @brief           系统配置表*/
	const UINT EX_STTP_INFO_TBSYSCONFIG=101;
	/** @brief           软压板校核基准值表*/
	const UINT EX_STTP_INFO_TBORDERSOFT_CFG=102;
	/** @brief           软压板校核结果日志表*/
	const UINT EX_STTP_INFO_TBORDERSOFTSETTING=103;
	/** @brief           软压板基准修改日志表*/
	const UINT EX_STTP_INFO_TBSOFTOPERATION=104;
	/** @brief           断面数据矩阵对应点定义表*/
	const UINT EX_STTP_INFO_SECTION_CFG=105;
	/** @brief           断面数据表*/
	const UINT EX_STTP_INFO_SECTION_DATA=106;
	/** @brief           断面数据多点计算公式表*/
	const UINT EX_STTP_INFO_SECTION_COMB=107;
	/** @brief           越限告警类型定义表*/
	const UINT EX_STTP_INFO_CROSS_CFG=108;
	/** @brief           预警数据表*/
	const UINT EX_STTP_INFO_CROSS_DATA=109;
	/** @brief           智能处理理论线路参数表*/
	const UINT EX_STTP_INFO_DEVICEPARAM_CFG=110; //add by lipeng
	/** @brief           智能处理录波器信息表*/
	const UINT EX_STTP_INFO_DFRCFG_CFG=111; //add by lipeng
	/** @brief           智能处理故障录波器所接线路信息表*/
	const UINT EX_STTP_INFO_PDEVICE_CFG=112; //add by lipeng
	/** @brief           智能处理故障录波器所接线路信息表*/
	const UINT EX_STTP_INFO_COMM_TOTAL_DATA=113;
	/** @brief           设备通讯状态通断原因表*/
	const UINT EX_STTP_INFO_PT_COMMU_DETAIL=114;
	/** @brief           主子站通讯状态原因基本配置表*/
	const UINT EX_STTP_INFO_STATION_STATUS_CONFIG=115;
	/** @brief           主子站通讯状态变化原因表*/
	const UINT EX_STTP_INFO_STATION_STATUS_CHANGE=116;
	/** @brief           一次设备ems值配置表*/
	const UINT EX_STTP_INFO_PT_EMS_CONFIG=117;		
	/** @brief           二次设备定值区号配置表*/
	const UINT EX_STTP_INFO_PT_ZONE_CONFIG=118;
	/** @brief           辅助决策数据表*/
	const UINT EX_STTP_INFO_ANALYFAULT_DATA=119;
	/** @brief           辅助决策关键字过滤表*/
	const UINT EX_STTP_INFO_ANALYFILTER_CONFIG=120;
	/** @brief           多服务热切服务器配置表*/
	const UINT EX_STTP_INFO_COMMU_SERVER_CONFIG=121;


	/*****************以下是录波联网的数据表编号*****************/
	/** @brief           录波仪表*/
	const UINT EX_RECORD_OSC=1000;
	/** @brief           通道与录波仪关联关系表*/
	const UINT EX_RECORD_OSC_CHANNEL=1001;
	/** @brief           录波列表*/
	const UINT EX_RECORD_OSC_LIST=1002;
	/** @brief           通讯状态数据表*/
	const UINT EX_RECORD_COMMU_STATUS=1003;
	/** @brief           告警数据表*/
	const UINT EX_RECORD_ALARM_DATA=1004;
	/** @brief           系统告警表*/
	const UINT EX_RECORD_SYS_ALARM=1005;
	/** @brief           系统操作日志表*/
	const UINT EX_RECORD_OPERATION=1006;
	/**@brief   一次设备与回路关联关系表*/
	const UINT EX_RECORD_CIRCUIT=1007;
	/** @brief           地区与厂站关系视图*/
	const UINT VIEW_RECORD_STATION_NET=1008;
	/** @brief           一次设备集合视图*/
	const UINT VIEW_RECORD_FDEV_UNION=1009;
	/** @brief           一次设备及地区及厂站关联视图*/
	const UINT VIEW_RECORD_FDEV_BASE=1010;
	/** @brief           录波仪视图*/
	const UINT VIEW_RECORD_OSC_BASE=1011;
	/** @brief           录波仪厂站地区视图*/
	const UINT VIEW_RECORD_OSC_TYPE=1012;
	/** @brief           设备型号和规约关联视图*/
	const UINT VIEW_RECORD_DEVICE_PROTOCOL=1013;
	/** @brief           一次设备与回路关联关系视图*/
	const UINT VIEW_RECORD_CIRCUIT_FDEV=1014;
	/** @brief           录波仪通道回路关联视图*/
	const UINT VIEW_RECORD_OSC_CHANNEL_CIRCUIT=1015;
	/** @brief           录波联网定值数据表*/
	const UINT EX_RECORD_SETTING_DATA=1016;
	/** @brief           录波联网动作数据表*/
	const UINT EX_RECORD_ACTION=1017;


	/*****************以下是61850主站项目的数据表编号*****************/
	/** @brief           61850数据集分类表*/
	const UINT EX_STTP_INFO_TB_61850_DS_TYPE=122;
	/** @brief           61850逻辑节点分类表*/
	const UINT EX_STTP_INFO_TB_61850_LN_TYPE=123;
	/** @brief           61850报告控制块分类表*/
	const UINT EX_STTP_INFO_TB_61850_RCB_TYPE=124;

	/*****************以下是状态检修项目的相关表编号*****************/
	/** @brief           状态检修评分规则类型配置表*/
	const UINT EX_STTP_INFO_TB_CBM_RULETYPE_CONFIG=125;
	/** @brief           状态检修评分规则配置表*/
	const UINT EX_STTP_INFO_TB_CBM_RULE_CONFIG=126;
	/** @brief           状态点配置表*/
	const UINT EX_STTP_INFO_TB_CBM_STATUSPOINT_CONFIG=127;
	/** @brief           二次设备缺陷配置表*/
	const UINT EX_STTP_INFO_TB_CBM_DEFECT_CONFIG=128;
	/** @brief           二次设备缺陷数据表*/
	const UINT EX_STTP_INFO_TB_CBM_DEFECT=129;
	/** @brief           状态检修结果数据表*/
	const UINT EX_STTP_INFO_TB_CBM_RESULT_DATA=130;
	/** @brief           状态检修结果详细表*/
	const UINT EX_STTP_INFO_TB_CBM_RESULT_DETAIL=131;
	/** @brief           装置状态检修预警事件表*/
	const UINT EX_STTP_INFO_TB_CBM_EARLYWARNING=132;
	/** @brief           装置检修记录表*/
	const UINT EX_STTP_INFO_TB_CBM_MAINTENANCE_RECORD=133;
	/** @brief           装置检修策略数据表*/
	const UINT EX_STTP_INFO_TB_CBM_THRESHOLD_CONFIG=134;

	/*定值校核新增表*/
	/** @brief           定值单索引表*/
	const UINT EX_STTP_INFO_TB_ST_ORDER_SETTING_INDEX=135;

	/** @brief           手机消息定制表*/
	const UINT EX_STTP_INFO_TB_SMS_MOBILE=136;
	/** @brief           系统下保护信号定义表*/
	const UINT EX_STTP_INFO_TB_SCADA_SIGNAL_DEF=137;
	/** @brief           挂牌原因配置表*/
	const UINT EX_STTP_INFO_TB_SUSPEND_REASON_DEF=138;
	/** @brief           调管单位配置表*/
	const UINT EX_STTP_INFO_TB_RESPONSDEPART_DEF=139;
	/** @brief           运维单位配置表*/
	const UINT EX_STTP_INFO_TB_MAINTAINDEPART_DEF=140;
	/** @brief           检修单位类型配置表*/
	const UINT EX_STTP_INFO_TB_SERVICEDEPART_TYPE_DEF=141;
	/** @brief           检修单位配置表*/
	const UINT EX_STTP_INFO_TB_SERVICEDEPART_DEF=142;
	/** @brief           开关量校核基准值表*/
	const UINT EX_STTP_INFO_ST_ORDERDIGITAL=143;
	/** @brief           开关量校核结果日志表*/
	const UINT EX_STTP_INFO_ST_DIGITALSETTING=144;
	/** @brief           开关量基准修改日志表*/
	const UINT EX_STTP_INFO_ST_DIGITALOPERATION=145;
	/** @brief           模拟量校核基准值表*/
	const UINT EX_STTP_INFO_ST_ORDERANALOG=146;
	/** @brief           模拟量校核结果日志表*/
	const UINT EX_STTP_INFO_ST_ANALOGSETTING=147;
	/** @brief           模拟量基准修改日志表*/
	const UINT EX_STTP_INFO_ST_ANALOGOPERATION=148;
	/** @brief           模拟量校核误差定值信息*/
	const UINT EX_STTP_INFO_ST_ERRORANALOG_CFG=149;
	/** @brief           主保护状态表*/
	const UINT EX_STTP_INFO_TB_MAINPROTECTSTATUS=150;
	/** @brief           装置时钟偏差告警表*/
	const UINT EX_STTP_INFO_TB_DEVCLOCK_OFFSET=151;
	/** @brief           召唤结果映射表*/
	const UINT EX_STTP_INFO_TB_OC_DUTY_CONFIG=152;
	/** @brief           系统编码表*/
	const UINT EX_STTP_INFO_TB_SYSTEM_CODE_CONFIG=153;
	/** @brief           基准定值模板表*/
	const UINT EX_STTP_INFO_ST_STANDARD_SETTING = 154;
	/** @brief           定值校核结果概要信息表*/
	const UINT EX_STTP_INFO_ST_CHECKSETTING_SUM = 155;
	/** @brief           定值单列表表*/
	const UINT EX_STTP_INFO_ST_SETTING_ORDER_LIST = 156;
	/** @brief           用户与厂站关联关系表*/
	const UINT EX_STTP_INFO_TB_USER_RELATED = 157;
	/** @brief           日报表统计概要信息表*/
	const UINT EX_STTP_INFO_TB_STAT_DAILY_BRIEF = 158;
	/** @brief           日报表子站统计信息表*/
	const UINT EX_STTP_INFO_TB_STAT_DAILY_DETAIL = 159;
	/** @brief           月报表统计概要信息表*/
	const UINT EX_STTP_INFO_TB_STAT_MONTHLY_BRIEF = 160;
	/** @brief           月报表子站统计信息表*/
	const UINT EX_STTP_INFO_TB_STAT_MONTHLY_DETAIL = 161;
	/** @brief           通用文件列表数据*/
	const UINT EX_STTP_INFO_GENERAL_LIST = 162;


	
/**
 * @brief       报文发送方向定义
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
	/** @brief            发送*/
	const UINT  MSG_SEND =      1;
	
	/** @brief            接收*/
	const UINT  MSG_RECV =      2;
/************************************************************************/


	/**
 * @brief       通讯服务器类型
 * @author      hxq
 * @version     ver1.0
 * @date        14/12/2015
 *
 */

	/** @brief            常规通讯服务器*/
	const UINT COMMON_COMMUSERVER = 1;
	/** @brief            61850通讯服务器*/
	const UINT C61850_COMMUSERVER = 2;
	/** @brief            录波联网通讯服务器*/
	const UINT WAVE_COMMUSERVER = 3;

/**
* @defgroup    
* @{
*/
	
/**
 * @brief       打印报文结构头
 * @author      姜健琳
 * @version     ver1.0
 * @date        12/07/2006
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */
typedef struct _LOG_BUFFER_HEAD
{
	/** @brief   对端网络地址*/
	NETADDR      addr;
	
	/** @brief   数据传输方向*/
	int          nWay;
		
	/** @brief   串口名称*/
	char         ch_port_name[32];
}LOG_BUFFER_HEAD; 


/** @} */ //OVER


#endif // SttpDefine_h__