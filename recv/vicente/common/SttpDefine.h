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

/*****************************��������***********************************/

            

/**
 * @brief       STTP�ͻ�������ģʽ����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        ͬ��ģʽ*/
	const int STTPNET_RUNMODLE_SYNC   = 0;
	/** @brief        �첽ģʽ*/
	const int STTPNET_RUNMODLE_ASYNC  = 1;

/**
 * @brief       STTP�ͻ������Ͷ���
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
	
	/** @brief        �������վ*/
	const int STTPNET_MODLE_SUBSTATION_SERVER	= 1;
	/** @brief        ��վ*/
	const int STTPNET_MODLE_MAINSTATION			= 2;	
	/** @brief        ����*/
	const int STTPNET_MODLE_OTHERSYSTEM			= 3;
	/** @brief        ¼��������վ*/
	const int STTPNET_MODLE_SUBSTATION_RECORDER	= 4;	
	/** @brief        �ͻ�����վ*/
	const int STTPNET_MODLE_SUBSTATION_CLIENT	= 5;
	/** @brief        ����˷�վ*/
	const int STTPNET_MODLE_BRASTATION_SERVER	= 6;
	/** @brief        ��վ֮��վ*/
	const int STTPNET_MODLE_SUBSTATION_BRANCH	= 7;
	/** @brief        ֧��¼�������ķ������վ*/
	const int STTPNET_MODLE_SUBSTATION_SRV_OSC	= 8;	
	/** @brief        61850��վ*/
	const int STTPNET_MODLE_SUBSTATION_61850	= 9;
	/** @brief        ���������*/
	const int STTPNET_MODLE_XJBROWSER			= 10;
	/** @brief        �Զ��鵵*/
	const int STTPNET_MODLE_XJPACKAGE			= 11;
	/** @brief        ��ֵУ��ǰ̨*/
	const int STTPNET_MODLE_XJCHECKMANAGER		= 12;
	/** @brief        ��ֵУ�˺�̨*/
	const int STTPNET_MODLE_XJAUTOCHECK			= 13;
	/** @brief        ���̽�ģ����*/
	const int STTPNET_MODLE_XJMODELINGTOOL		= 14;
	/** @brief        �Զ�����*/
	const int STTPNET_MODLE_XJAUTOCALLTOTAL		= 15;
	/** @brief        WEBͬ��*/
	const int STTPNET_MODLE_XJWEBRECEIVER		= 16;
	/** @brief        ��ѯ��ͳ��*/
	const int STTPNET_MODLE_XJQUERY				= 17;	
	/** @brief        �糧Ԥ��*/
	const int STTPNET_MODLE_XJANTIDEFECT		= 18;
	/** @brief        ����Ա����*/
	const int STTPNET_MODLE_XJDISPATCHER		= 19;
	/** @brief        ����ת������ģ�� */
	const int STTPNET_MODEL_XJDATAROUTER        = 20;
	/** @brief        ¼����������ģ�� */
	const int STTPNET_MODEL_WAVE_ANALYST        = 21;
	/** @brief        ���ݿ�ӿ�*/
	const int STTPNET_MODLE_XJDBFACADE			= 100;
	/** @brief        �ɼ���*/
	const int STTPNET_MODLE_STFRONT				= 101;
	/** @brief        ͨ�ŷ�����*/
	const int STTPNET_MODLE_XJCOMMUSERVER		= 102;
	/** @brief        �������ݿͻ���*/
	const int STTPNET_MODLE_SECTIONDATACLIENT	= 103;
	/** @brief        ˫�˲��ģ��*/
	const int STTPNET_MODEL_XJFAULTLOCATION     = 104; //add by lipeng
	/** @brief        ¼�����Ϸ���ģ��*/
	const int STTPNET_MODEL_XJLIPROCESS			= 105; 
	/** @brief        ��վ������ģ��*/
	const int STTPNET_MODEL_SUBSTATIONTEST		= 106; 	
	/** @brief        ¼��ͳһƽ̨*/
	const int STTPNET_MODLE_UNIQUERECORDER		= 107;	
	/** @brief        ��������ϵͳ */
	const int STTPNET_MODLE_AUXILIARYDECISION	= 108;	
	/** @brief        �ȿ�ϵͳ */
	const int STTPNET_MODLE_STABLECONTROL		= 109;	
	/** @brief        ����Ԥ��ϵͳ */
	const int STTPNET_MODLE_SMARTPREWARNING		= 110;
	/** @brief        ��Ϣ���߿ͻ��� */
	const int STTPNET_MODLE_MSGBUSSERVER        = 111;
	/** @brief        ͨѶ״̬����ģ�� */
	const int STTPNET_MODLE_STATUSMONITER       = 112;	
	/** @brief        ���ϵͳ2010��վ*/
	const int STTPNET_MODLE_SUBSTATION_WFL2010  = 113;  
	/** @brief        ���ϵͳ2012��վ*/
	const int STTPNET_MODLE_SUBSTATION_WFL2012 =  114;
	/** @brief        webͬ�����Ͷ�*/
	const int STTPNET_MODLE_WEB_PROVIDER_SENDER = 115;

/**
 * @brief       STTP�ͻ�������״̬����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        �ж�*/
	const int STTPNET_UNCONNECTED   = 0;
	/** @brief        ����*/
	const int STTPNET_CONNECTED  = 1;

/**
 * @brief       STTP�ͻ���ע��ص����������붨��
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief        ���ӱ仯*/
	const int STTPNET_CALLBACK_ONCONNECTCHANGE   = 0;


/**
 * @brief       �������Ͷ���
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           ������*/
	const UINT EX_STTP_DATA_TYPE_FLOAT      = 0;
	/** @brief           ����*/
	const UINT EX_STTP_DATA_TYPE_INT        = 1;
	/** @brief           ��������*/
	const UINT EX_STTP_DATA_TYPE_CONTRL     = 2;
	/** @brief           �ַ���*/
	const UINT EX_STTP_DATA_TYPE_STRING     = 3;
	/** @brief           ʱ������*/
	const UINT EX_STTP_DATA_TYPE_TIME       = 4;

/**
 * @brief       �������Ͷ���
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           ���ýṹ����*/
	const UINT EX_STTP_CONFIG_FIELD_DES     = 1;
	/** @brief           ��̬����*/
	const UINT EX_STTP_CONFIG_STATIC_DATA   = 2;
	/** @brief           ��̬����*/
	const UINT EX_STTP_CONFIG_DYNAMIC_DATA  = 3;

/**
 * @brief       SQL���ؼ��ֶ���
 * @author      ����
 * @version     ver1.0
 * @date        18/09/2008
 *
 */
	/** @brief           TOPʱ����������*/
	const UINT EX_STTP_DATA_TYPE_TOP		= 99;
	
	/** @brief           ��������ѯʱ����������*/
	const UINT EX_STTP_WHERE_ABNORMALITY    = 98;


/**
 * @brief       �������Ͷ���
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           ȫ������*/
	const UINT EX_STTP_FUN_TYPE_FULL        = 1;
	/** @brief           ��������*/
	const UINT EX_STTP_FUN_TYPE_PART		= 2;
	/** @brief           �������*/
	const UINT EX_STTP_FUN_TYPE_ADD		    = 3;
	/** @brief           ��������*/
	const UINT EX_STTP_FUN_TYPE_UPDATE		= 4;
	/** @brief           ɾ������*/
	const UINT EX_STTP_FUN_TYPE_DELETE      = 5;

/**
 * @brief       ��Ϣ���Ͷ���
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

	/** @brief           ��Ч����*/
	const UINT EX_STTP_INFO_INVALID=0;
	/** @brief           ��������*/
	const UINT EX_STTP_INFO_LOCAL_CFG=1;
	/** @brief           վ����*/
	const UINT EX_STTP_INFO_STATION_CFG=2;                
	/** @brief           �Զ�������*/
	const UINT EX_STTP_INFO_AUTOMATION_CFG=3;              
	/** @brief           ĸ������*/
	const UINT EX_STTP_INFO_BUS_CFG=4;                     
	/** @brief           ����������Ϣ*/
	const UINT EX_STTP_INFO_BREAKER_CFG=5;                
	/** @brief           ��բ������Ϣ*/
	const UINT EX_STTP_INFO_SWITCH_CFG=6;                
	/** @brief           ��·������Ϣ*/
	const UINT EX_STTP_INFO_LINE_CFG=7;                   
	/** @brief           ��ѹ������*/
	const UINT EX_STTP_INFO_TRANS_CFG=8;                   
	/** @brief           ���������*/
	const UINT EX_STTP_INFO_GEN_CFG=9;                   
	/** @brief           �翹������*/
	const UINT EX_STTP_INFO_REACTOR_CFG=10;              
	/** @brief           ����������*/
	const UINT EX_STTP_INFO_CAPACITOR_CFG=11;            
	/** @brief           �����豸���� ������¼����*/
	const UINT EX_STTP_INFO_SECDEV_CFG=12;               
	/** @brief           װ��CPU����*/        
	const UINT EX_STTP_INFO_PT_CPU_CFG=13;               
	/** @brief           վ��ͨѶ��Լ����*/
	const UINT EX_STTP_INFO_COMMU_PROTOCOL_CFG=14;       
	/** @brief           ��Ϣ��������*/
	const UINT EX_STTP_INFO_BOOK_INFO_CFG=15;            
	/** @brief           ���ܳ�ʱ����*/
	const UINT EX_STTP_INFO_FUN_TIMEOUT_CFG=16;          
	/** @brief           ��վ�Զ�������Ϣ����*/
	const UINT EX_STTP_INFO_COMMU_DEV_MAINSTATION_CFG=17;
	/** @brief           վ��103�������*/
	const UINT EX_STTP_INFO_COMMU_103GROUP_CFG=18;       
	/** @brief           ������������ϢTb_pt_character_def ,��Ա���*/
	const UINT EX_STTP_INFO_PT_CHARCTOR_DEF_CFG=19;   
	/** @brief           ˫��˫������*/
	const UINT EX_STTP_INFO_COMMU_DOUBLESERVER_CFG=20;
	/** @brief           �澯������Ϣ*/
	const UINT EX_STTP_INFO_PT_ALARM_CFG=21;          
	/** @brief           ����������Ϣ*/
	const UINT EX_STTP_INFO_PT_ACTION_CFG=22;         
	/** @brief           ��ֵ����*/
	const UINT EX_STTP_INFO_PT_SETTING_CFG=23;        
	/** @brief           ������������Ϣ*/
	const UINT EX_STTP_INFO_PT_DI_CFG=24;             
	/** @brief           ģ����������Ϣ*/
	const UINT EX_STTP_INFO_PT_AI_CFG=25;             
	/** @brief           ��ѹ��������Ϣ*/
	const UINT EX_STTP_INFO_PT_SOFTBOARD_CFG=26;      
	/** @brief           ϵͳ���õ�������*/
	const UINT EX_STTP_INFO_CHARCTOR_CFG=27;          
	/** @brief           װ�ö�ֵ��������*/ 
	const UINT EX_STTP_INFO_PT_ZONE_CFG=28;           
	/** @brief           װ��ͨѶ״̬����*/ 
	const UINT EX_STTP_INFO_PT_COMMU_CFG=29;          
	/** @brief           װ������״̬����*/
	const UINT EX_STTP_INFO_PT_RUN_CFG=30;            
	/** @brief           ����¼��ģ����ͨ������*/
	const UINT EX_STTP_INFO_PT_OSC_AI_CFG=31;         
	/** @brief           ����¼������������*/
	const UINT EX_STTP_INFO_PT_OSC_DI_CFG=32;         
	/** @brief           ����ͨ�÷����������*/
	const UINT EX_STTP_INFO_PT_103GROUP_CFG=33;       
	/** @brief           GPS��������(�ݲ�ʹ��)*/
	const UINT EX_STTP_INFO_GPS_CFG=34;               
	/** @brief           ABB���������¼�����*/
	const UINT EX_STTP_INFO_ABB_EVENT_CFG=35;         
	/** @brief           �ɼ�����ѵ��������*/
	const UINT EX_STTP_INFO_FRONT_QUERY_CFG=36;       
	/** @brief           �˿�����*/
	const UINT EX_STTP_INFO_PORT_CFG=37;              
	/** @brief           �ɼ�����Լ���ñ�*/
	const UINT EX_STTP_INFO_FRONT_PROTOCOL_CFG=38;    
	/** @brief           ¼���ļ��б�����*/
	const UINT EX_STTP_INFO_OSC_LIST=39;              
	/** @brief           �ɼ��������豸���ñ�*/      
	const UINT EX_STTP_INFO_FRONT_SECDEV_CFG=40;      
	/** @brief           �澯����*/ 
	const UINT EX_STTP_INFO_PT_ALARM_DATA=41;                 
	/** @brief           ��������*/
	const UINT EX_STTP_INFO_PT_ACTION_DATA=42;                
	/** @brief           ��ѹ������*/
	const UINT EX_STTP_INFO_PT_SOFTBOARD_DATA=43;              
	/** @brief           ��ֵ����*/
	const UINT EX_STTP_INFO_PT_SETTING_DATA=44;               
	/** @brief           ģ����*/
	const UINT EX_STTP_INFO_PT_AI_DATA=45;                    
	/** @brief           ������*/
	const UINT EX_STTP_INFO_PT_DI_DATA=46;                    
	/** @brief           ¼����*/
	const UINT EX_STTP_INFO_OSC_REPORT_DATA=47;
	/** @brief           ϵͳά����Ϣ*/
	const UINT EX_STTP_INFO_SYS_MAINTENANCE_DATA=48;
	/** @brief           ����������Ϣ*/
	const UINT EX_STTP_INFO_NET_CFG=49;
	/** @brief           һ���豸�����������������Ϣ*/
	const UINT EX_STTP_INFO_PMS_CFG=50;
	/** @brief           һ���豸�����������Ϣ*/
	const UINT EX_STTP_INFO_BAY_CFG=51;
	/** @brief           �û�����Ϣ*/
	const UINT EX_STTP_INFO_SYS_GROUP_CFG=52;
	/** @brief           �û���Ϣ*/
	const UINT EX_STTP_INFO_SYS_USER_CFG=53;
	/** @brief           ϵͳ������Ϣ*/
	const UINT EX_STTP_INFO_SYS_FUNC_CFG=54;
	/** @brief           �鹦��Ȩ�޹�����Ϣ*/
	const UINT EX_STTP_INFO_SYS_GROUP_FUNC_CFG=55;
	/** @brief           ������һ���豸������Ϣ*/
	const UINT EX_STTP_INFO_BREAKER_DEVICE_CFG=56;
	/** @brief           ��ֵ����������Ϣ*/
	const UINT EX_STTP_INFO_ZONE_BASE_CFG=57;
	/** @brief           ����¼������������λ��������Ϣ*/
	const UINT EX_STTP_INFO_SOE_CFG=58;
	/** @brief           �˹�������ʷ��ѯ*/
	const UINT EX_STTP_INFO_OPERATION_CFG=59;
	/** @brief           �ļ�������Ϣ*/
	const UINT EX_STTP_INFO_DOWNLOAD_FILE_CFG=60;
	/** @brief           װ���ͺ�������Ϣ*/
	const UINT EX_STTP_INFO_PT_TYPE_CFG=61;
	/** @brief           ϵͳ�澯��Ϣ*/
	const UINT EX_STTP_INFO_SYS_ALARM_CFG=62;
	/** @brief           �˵���Ϣ*/
	const UINT EX_STTP_INFO_MENU_INFO_CFG=63;
	/** @brief           �������仯��Ϣ*/
	const UINT EX_STTP_INFO_CHANGE_DI_CFG=64;
	/** @brief           ������Ϣ*/
	const UINT EX_STTP_INFO_TABLE_INFO_CFG=65;
	/** @brief           ������Ϣ*/
	const UINT EX_STTP_INFO_FIELD_INFO_CFG=66;
	/** @brief           6185������ƿ���Ϣ��*/
	const UINT EX_STTP_INFO_61850_REPORT_CFG=67;
	/** @brief           61850������ת��Ϣ*/
	const UINT EX_STTP_INFO_61850_TEMP_CFG=68;
	/** @brief           ��ֵУ�˻�׼��ֵ��Ϣ*/
	const UINT EX_STTP_INFO_ST_ORDERSETTING_CFG=69;
	/** @brief           ��ֵУ�˽����־��Ϣ*/
	const UINT EX_STTP_INFO_ST_CHECKSETTING_CFG=70;
	/** @brief           ��ֵУ����ֵ��Ϣ*/
	const UINT EX_STTP_INFO_ST_ERRORSETTING_CFG=71;
	/** @brief           ��ֵ��׼�޸���־��Ϣ*/
	const UINT EX_STTP_INFO_ST_OPERATION_CFG=72;
	/** @brief           ��ֵУ�˱���������Ϣ*/
	const UINT EX_STTP_INFO_ST_CHECKPROTECT_CFG=73;
	/** @brief           װ��ģ����Ϣ*/
	const UINT EX_STTP_INFO_PT_MODEL_CFG=74;
	/** @brief           ¼���ļ���¼��Ϣ(�ݲ�ʹ��)*/
	const UINT EX_STTP_INFO_WAVE_FILE_CFG=75;
	/** @brief           ��վ����վͨѶ״̬��¼��Ϣ*/
	const UINT EX_STTP_INFO_STATION_STATUS_CFG=76;
	/** @brief           װ��ģ���Ԫ��ģ��Ĺ�ϵ��Ϣ*/
	const UINT EX_STTP_INFO_DDEFUDEF_CFG=77;
	/** @brief           ����ͼ��ģ�嶨����Ϣ*/
	const UINT EX_STTP_INFO_DLL_CFG=78;
	/** @brief           ����װ�ù�����Ϣ*/
	const UINT EX_STTP_INFO_TBDUT_CFG=79;
	/** @brief           װ��ģ�嶨����Ϣ*/
	const UINT EX_STTP_INFO_TBDUTDEF_CFG=80;
	/** @brief           Ԫ��ģ��IEEE���ܺ���Ϣ*/
	const UINT EX_STTP_INFO_TBFUNCTION_CFG=81;
	/** @brief           ����װ�ó�����Ϣ*/
	const UINT EX_STTP_INFO_MANUFACTORY_CFG=82;
	/** @brief           ��ʱ�����Բ�����Ϣ*/
	const UINT EX_STTP_INFO_TBREVERSE_CFG=83;
	/** @brief           Ԫ���Ͷ�ֵ������ϵ��Ϣ*/
	const UINT EX_STTP_INFO_SETTINGRELATION_CFG=84;
	/** @brief           Ԫ��������Ϣ*/
	const UINT EX_STTP_INFO_TBUNIT_CFG=85;
	/** @brief           Ԫ��ģ����Ϣ*/
	const UINT EX_STTP_INFO_TBUNITDEF_CFG=86;
	/** @brief           Ԫ������ͼ�ι�����Ϣ*/
	const UINT EX_STTP_INFO_UNITDEFDLL_CFG=87;
	/** @brief           ����ͼ�ΰ���������ֵ��Ϣ*/
	const UINT EX_STTP_INFO_UNITPARADEF_CFG=88;
	/** @brief           Ԫ������ͼ�ι���������ֵ��ֵ��Ϣ*/
	const UINT EX_STTP_INFO_UNITPARAVALUE_CFG=89;
	/** @brief           ͨ��������Ϣ*/
	const UINT EX_STTP_INFO_ACHANNEL_CFG=90;
	/** @brief           ��Ϣ�鵵������(�ݲ�ʹ��)*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_CFG=91;
	/** @brief           ���Ϲ鵵�������¼��Ĺ�ϵ��Ϣ(�ݲ�ʹ��)*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_STEVENT_CFG=92;
	/** @brief           ��Ϣ�鵵������Ϣ*/
	const UINT EX_STTP_INFO_FAULTSHORTENCONFIG_CFG=93;
	/** @brief           ��Ϣ�鵵����*/
	const UINT EX_STTP_INFO_FAULTSHORTEN_DATA=94;
	/** @brief           ��Ϣ�鵵�¼���Ϣ*/
	const UINT EX_STTP_INFO_FAULTSHORTENEVENT_CFG=95;
	/** @brief          С�������Ϣ*/
	const UINT EX_STTP_INFO_DISTANCE_CFG=96;
	/** @brief           С�����ϵͳ��·�����Ϣ*/
	const UINT EX_STTP_INFO_DISTANCE_LINE_CFG=97;
	/** @brief           ��վSVG�ļ���Ϣ*/
	const UINT EX_STTP_INFO_STATION_SVG_CFG=98;
	/** @brief           �Զ��ٻ������Ϣ*/
	const UINT EX_STTP_INFO_OC_RESULT_CFG=99;
	/** @brief           �Զ�����װ��ѡ��������Ϣ*/
	const UINT EX_STTP_INFO_OC_CONFIG_CFG=100;
	/** @brief           ϵͳ���ñ�*/
	const UINT EX_STTP_INFO_TBSYSCONFIG=101;
	/** @brief           ��ѹ��У�˻�׼ֵ��*/
	const UINT EX_STTP_INFO_TBORDERSOFT_CFG=102;
	/** @brief           ��ѹ��У�˽����־��*/
	const UINT EX_STTP_INFO_TBORDERSOFTSETTING=103;
	/** @brief           ��ѹ���׼�޸���־��*/
	const UINT EX_STTP_INFO_TBSOFTOPERATION=104;
	/** @brief           �������ݾ����Ӧ�㶨���*/
	const UINT EX_STTP_INFO_SECTION_CFG=105;
	/** @brief           �������ݱ�*/
	const UINT EX_STTP_INFO_SECTION_DATA=106;
	/** @brief           �������ݶ����㹫ʽ��*/
	const UINT EX_STTP_INFO_SECTION_COMB=107;
	/** @brief           Խ�޸澯���Ͷ����*/
	const UINT EX_STTP_INFO_CROSS_CFG=108;
	/** @brief           Ԥ�����ݱ�*/
	const UINT EX_STTP_INFO_CROSS_DATA=109;
	/** @brief           ���ܴ���������·������*/
	const UINT EX_STTP_INFO_DEVICEPARAM_CFG=110; //add by lipeng
	/** @brief           ���ܴ���¼������Ϣ��*/
	const UINT EX_STTP_INFO_DFRCFG_CFG=111; //add by lipeng
	/** @brief           ���ܴ������¼����������·��Ϣ��*/
	const UINT EX_STTP_INFO_PDEVICE_CFG=112; //add by lipeng
	/** @brief           ���ܴ������¼����������·��Ϣ��*/
	const UINT EX_STTP_INFO_COMM_TOTAL_DATA=113;
	/** @brief           �豸ͨѶ״̬ͨ��ԭ���*/
	const UINT EX_STTP_INFO_PT_COMMU_DETAIL=114;
	/** @brief           ����վͨѶ״̬ԭ��������ñ�*/
	const UINT EX_STTP_INFO_STATION_STATUS_CONFIG=115;
	/** @brief           ����վͨѶ״̬�仯ԭ���*/
	const UINT EX_STTP_INFO_STATION_STATUS_CHANGE=116;
	/** @brief           һ���豸emsֵ���ñ�*/
	const UINT EX_STTP_INFO_PT_EMS_CONFIG=117;		
	/** @brief           �����豸��ֵ�������ñ�*/
	const UINT EX_STTP_INFO_PT_ZONE_CONFIG=118;
	/** @brief           �����������ݱ�*/
	const UINT EX_STTP_INFO_ANALYFAULT_DATA=119;
	/** @brief           �������߹ؼ��ֹ��˱�*/
	const UINT EX_STTP_INFO_ANALYFILTER_CONFIG=120;
	/** @brief           ��������з��������ñ�*/
	const UINT EX_STTP_INFO_COMMU_SERVER_CONFIG=121;


	/*****************������¼�����������ݱ���*****************/
	/** @brief           ¼���Ǳ�*/
	const UINT EX_RECORD_OSC=1000;
	/** @brief           ͨ����¼���ǹ�����ϵ��*/
	const UINT EX_RECORD_OSC_CHANNEL=1001;
	/** @brief           ¼���б�*/
	const UINT EX_RECORD_OSC_LIST=1002;
	/** @brief           ͨѶ״̬���ݱ�*/
	const UINT EX_RECORD_COMMU_STATUS=1003;
	/** @brief           �澯���ݱ�*/
	const UINT EX_RECORD_ALARM_DATA=1004;
	/** @brief           ϵͳ�澯��*/
	const UINT EX_RECORD_SYS_ALARM=1005;
	/** @brief           ϵͳ������־��*/
	const UINT EX_RECORD_OPERATION=1006;
	/**@brief   һ���豸���·������ϵ��*/
	const UINT EX_RECORD_CIRCUIT=1007;
	/** @brief           �����볧վ��ϵ��ͼ*/
	const UINT VIEW_RECORD_STATION_NET=1008;
	/** @brief           һ���豸������ͼ*/
	const UINT VIEW_RECORD_FDEV_UNION=1009;
	/** @brief           һ���豸����������վ������ͼ*/
	const UINT VIEW_RECORD_FDEV_BASE=1010;
	/** @brief           ¼������ͼ*/
	const UINT VIEW_RECORD_OSC_BASE=1011;
	/** @brief           ¼���ǳ�վ������ͼ*/
	const UINT VIEW_RECORD_OSC_TYPE=1012;
	/** @brief           �豸�ͺź͹�Լ������ͼ*/
	const UINT VIEW_RECORD_DEVICE_PROTOCOL=1013;
	/** @brief           һ���豸���·������ϵ��ͼ*/
	const UINT VIEW_RECORD_CIRCUIT_FDEV=1014;
	/** @brief           ¼����ͨ����·������ͼ*/
	const UINT VIEW_RECORD_OSC_CHANNEL_CIRCUIT=1015;
	/** @brief           ¼��������ֵ���ݱ�*/
	const UINT EX_RECORD_SETTING_DATA=1016;
	/** @brief           ¼�������������ݱ�*/
	const UINT EX_RECORD_ACTION=1017;


	/*****************������61850��վ��Ŀ�����ݱ���*****************/
	/** @brief           61850���ݼ������*/
	const UINT EX_STTP_INFO_TB_61850_DS_TYPE=122;
	/** @brief           61850�߼��ڵ�����*/
	const UINT EX_STTP_INFO_TB_61850_LN_TYPE=123;
	/** @brief           61850������ƿ�����*/
	const UINT EX_STTP_INFO_TB_61850_RCB_TYPE=124;

	/*****************������״̬������Ŀ����ر���*****************/
	/** @brief           ״̬�������ֹ����������ñ�*/
	const UINT EX_STTP_INFO_TB_CBM_RULETYPE_CONFIG=125;
	/** @brief           ״̬�������ֹ������ñ�*/
	const UINT EX_STTP_INFO_TB_CBM_RULE_CONFIG=126;
	/** @brief           ״̬�����ñ�*/
	const UINT EX_STTP_INFO_TB_CBM_STATUSPOINT_CONFIG=127;
	/** @brief           �����豸ȱ�����ñ�*/
	const UINT EX_STTP_INFO_TB_CBM_DEFECT_CONFIG=128;
	/** @brief           �����豸ȱ�����ݱ�*/
	const UINT EX_STTP_INFO_TB_CBM_DEFECT=129;
	/** @brief           ״̬���޽�����ݱ�*/
	const UINT EX_STTP_INFO_TB_CBM_RESULT_DATA=130;
	/** @brief           ״̬���޽����ϸ��*/
	const UINT EX_STTP_INFO_TB_CBM_RESULT_DETAIL=131;
	/** @brief           װ��״̬����Ԥ���¼���*/
	const UINT EX_STTP_INFO_TB_CBM_EARLYWARNING=132;
	/** @brief           װ�ü��޼�¼��*/
	const UINT EX_STTP_INFO_TB_CBM_MAINTENANCE_RECORD=133;
	/** @brief           װ�ü��޲������ݱ�*/
	const UINT EX_STTP_INFO_TB_CBM_THRESHOLD_CONFIG=134;

	/*��ֵУ��������*/
	/** @brief           ��ֵ��������*/
	const UINT EX_STTP_INFO_TB_ST_ORDER_SETTING_INDEX=135;

	/** @brief           �ֻ���Ϣ���Ʊ�*/
	const UINT EX_STTP_INFO_TB_SMS_MOBILE=136;
	/** @brief           ϵͳ�±����źŶ����*/
	const UINT EX_STTP_INFO_TB_SCADA_SIGNAL_DEF=137;
	/** @brief           ����ԭ�����ñ�*/
	const UINT EX_STTP_INFO_TB_SUSPEND_REASON_DEF=138;
	/** @brief           ���ܵ�λ���ñ�*/
	const UINT EX_STTP_INFO_TB_RESPONSDEPART_DEF=139;
	/** @brief           ��ά��λ���ñ�*/
	const UINT EX_STTP_INFO_TB_MAINTAINDEPART_DEF=140;
	/** @brief           ���޵�λ�������ñ�*/
	const UINT EX_STTP_INFO_TB_SERVICEDEPART_TYPE_DEF=141;
	/** @brief           ���޵�λ���ñ�*/
	const UINT EX_STTP_INFO_TB_SERVICEDEPART_DEF=142;
	/** @brief           ������У�˻�׼ֵ��*/
	const UINT EX_STTP_INFO_ST_ORDERDIGITAL=143;
	/** @brief           ������У�˽����־��*/
	const UINT EX_STTP_INFO_ST_DIGITALSETTING=144;
	/** @brief           ��������׼�޸���־��*/
	const UINT EX_STTP_INFO_ST_DIGITALOPERATION=145;
	/** @brief           ģ����У�˻�׼ֵ��*/
	const UINT EX_STTP_INFO_ST_ORDERANALOG=146;
	/** @brief           ģ����У�˽����־��*/
	const UINT EX_STTP_INFO_ST_ANALOGSETTING=147;
	/** @brief           ģ������׼�޸���־��*/
	const UINT EX_STTP_INFO_ST_ANALOGOPERATION=148;
	/** @brief           ģ����У����ֵ��Ϣ*/
	const UINT EX_STTP_INFO_ST_ERRORANALOG_CFG=149;
	/** @brief           ������״̬��*/
	const UINT EX_STTP_INFO_TB_MAINPROTECTSTATUS=150;
	/** @brief           װ��ʱ��ƫ��澯��*/
	const UINT EX_STTP_INFO_TB_DEVCLOCK_OFFSET=151;
	/** @brief           �ٻ����ӳ���*/
	const UINT EX_STTP_INFO_TB_OC_DUTY_CONFIG=152;
	/** @brief           ϵͳ�����*/
	const UINT EX_STTP_INFO_TB_SYSTEM_CODE_CONFIG=153;
	/** @brief           ��׼��ֵģ���*/
	const UINT EX_STTP_INFO_ST_STANDARD_SETTING = 154;
	/** @brief           ��ֵУ�˽����Ҫ��Ϣ��*/
	const UINT EX_STTP_INFO_ST_CHECKSETTING_SUM = 155;
	/** @brief           ��ֵ���б��*/
	const UINT EX_STTP_INFO_ST_SETTING_ORDER_LIST = 156;
	/** @brief           �û��볧վ������ϵ��*/
	const UINT EX_STTP_INFO_TB_USER_RELATED = 157;
	/** @brief           �ձ���ͳ�Ƹ�Ҫ��Ϣ��*/
	const UINT EX_STTP_INFO_TB_STAT_DAILY_BRIEF = 158;
	/** @brief           �ձ�����վͳ����Ϣ��*/
	const UINT EX_STTP_INFO_TB_STAT_DAILY_DETAIL = 159;
	/** @brief           �±���ͳ�Ƹ�Ҫ��Ϣ��*/
	const UINT EX_STTP_INFO_TB_STAT_MONTHLY_BRIEF = 160;
	/** @brief           �±�����վͳ����Ϣ��*/
	const UINT EX_STTP_INFO_TB_STAT_MONTHLY_DETAIL = 161;
	/** @brief           ͨ���ļ��б�����*/
	const UINT EX_STTP_INFO_GENERAL_LIST = 162;


	
/**
 * @brief       ���ķ��ͷ�����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
	/** @brief            ����*/
	const UINT  MSG_SEND =      1;
	
	/** @brief            ����*/
	const UINT  MSG_RECV =      2;
/************************************************************************/


	/**
 * @brief       ͨѶ����������
 * @author      hxq
 * @version     ver1.0
 * @date        14/12/2015
 *
 */

	/** @brief            ����ͨѶ������*/
	const UINT COMMON_COMMUSERVER = 1;
	/** @brief            61850ͨѶ������*/
	const UINT C61850_COMMUSERVER = 2;
	/** @brief            ¼������ͨѶ������*/
	const UINT WAVE_COMMUSERVER = 3;

/**
* @defgroup    
* @{
*/
	
/**
 * @brief       ��ӡ���Ľṹͷ
 * @author      ������
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
	/** @brief   �Զ������ַ*/
	NETADDR      addr;
	
	/** @brief   ���ݴ��䷽��*/
	int          nWay;
		
	/** @brief   ��������*/
	char         ch_port_name[32];
}LOG_BUFFER_HEAD; 


/** @} */ //OVER


#endif // SttpDefine_h__