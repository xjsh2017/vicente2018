// SttpEngine.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SttpEngine.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ��ָ���ͻ��˽�������*/
XJHANDLE (*STTPConnectDestination)(const STRUCT_DESTINATION_SERVER& pDestination , int pClientModle , int & pConnected, const char* pLogPath, int pRunModle);
/** @brief           ����ͨѶ������*/
XJHANDLE (*SttpConnectServer)(int pClientModle, int & pConnected, const char* pLogPath, int pRunModle);
/** @brief           �ر������˵�����*/
int (*SttpCloseConnection)(XJHANDLE pClientHandle);
/** @brief           ����ָ�����ӵ��Զ�����*/
int (*STTPActiveTest) (XJHANDLE pClientHandle);
/** @brief           ��ȡSTTP����*/
int (*STTPRead)(XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);
/** @brief           д��STTP����*/
int (*STTPWrite)(XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);
/** @brief           ��������*/
int (*STTPSetOption)(XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);
/** @brief           ע�ᴦ����*/
int (*STTPRegisterHandler)(XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );
/** @brief           ��־·������*/
int (*STTPSetLogPath)(XJHANDLE pClientHandle ,const char* pLogPath);

/////////////////////////////////////////////////////////////////////////////
// CSttpEngine
//##ModelId=49B87B8900EB
CSttpEngine::CSttpEngine()
{
	//LoadSttpLib();
	m_hModule = NULL;
}

//##ModelId=49B87B8900FA
CSttpEngine::~CSttpEngine()
{
	if(m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CSttpEngine commands
//##ModelId=49B87B89024B
BOOL CSttpEngine::LoadSttpLib()
{
	if (g_LoadSttpDll == 0)
	{
		m_hModule = LoadLibrary("XJSttpNet.dll");
	}
	else if (g_LoadSttpDll == 1)
	{
		m_hModule = LoadLibrary("XJSttpProxy.dll");
	}
	
	if(m_hModule != NULL)
	{
		//����ָ���ͻ���
		STTPConnectDestination = (XJHANDLE (*)(const STRUCT_DESTINATION_SERVER&, int, int &, const char* , int))GetProcAddress(m_hModule, "STTPConnectDestination");
		//����ͨѶ������
		SttpConnectServer = (XJHANDLE (*)(int, int &, const char* , int))GetProcAddress(m_hModule, "STTPConnectServer");
		//�ر������˵�����
		SttpCloseConnection = (int (*)(XJHANDLE))GetProcAddress(m_hModule, "STTPCloseConnection");
		//����ָ�����ӵ��Զ�����
		STTPActiveTest = (int (*) (XJHANDLE))GetProcAddress(m_hModule, "STTPActiveTest");
		/** @brief           ��ȡSTTP����*/
		STTPRead = (int (*)(XJHANDLE, STTP_FULL_DATA&))GetProcAddress(m_hModule, "STTPRead");
		//д��STTP����
		STTPWrite = (int (*)(XJHANDLE, const STTP_FULL_DATA&))GetProcAddress(m_hModule, "STTPWrite");
		//��������
		STTPSetOption = (int (*)(XJHANDLE, int, int, int))GetProcAddress(m_hModule, "STTPSetOption");
		//ע�ᴦ����
		STTPRegisterHandler = (int (*)(XJHANDLE, PFUNCALLBACK, int))GetProcAddress(m_hModule, "STTPRegisterHandler");
		//��־·������
		STTPSetLogPath = (int(*)(XJHANDLE, const char*))GetProcAddress(m_hModule, "STTPSetLogPath");
	}
	return TRUE;
}

/*************************************************************
 �� �� ����XJSttpConnectServer()
 ���ܸ�Ҫ��������ͨ�ŷ�����������,����ָ���Զ˵�ַ����̬�����д����ݿ��л��
 �� �� ֵ: ������Sttp�ͻ��˶���ľ��
 ��    ����param1 �ͻ��˵����ͱ�ţ����ڵ�¼��֤
		   Param2 �������״̬ 0-�ж� 1-����
		   Param3 ��־�ļ�Ŀ¼
		   param4 ����ģʽ0��ͬ��ģʽ��Ĭ�ϣ�1���첽ģʽ
**************************************************************/
//##ModelId=49B87B890119
XJHANDLE CSttpEngine::XJSttpConnectServer(int pClientModle, int & pConnected, char* sLogPath, int pRunModle)
{
	if(m_hModule == NULL)
	{
		pConnected = 0;
		return NULL;
	}
	if(SttpConnectServer == NULL)
	{
		pConnected = 0;
		return NULL;
	}
	return SttpConnectServer(pClientModle, pConnected, sLogPath, pRunModle);
}

/*************************************************************
 �� �� ����XJSttpCloseConnection()
 ���ܸ�Ҫ���ر�ָ������ϵ�����
 �� �� ֵ:  1:�ɹ� 0:��Ч�ľ��
 ��    ����param1 ָ��Ҫ�رյľ��
**************************************************************/
//##ModelId=49B87B89011E
int CSttpEngine::XJSttpCloseConnection(XJHANDLE pClientHandle)
{
	if(m_hModule == NULL)
		return 0;
	if(SttpCloseConnection == NULL)
		return 0;
	return SttpCloseConnection(pClientHandle);
}

/*************************************************************
 �� �� ����XJSTTPActiveTest()
 ���ܸ�Ҫ������ָ�������ϵ��Զ����ԡ�Ĭ������¿ͻ��˶�̬�ⲻ�Զ������˷��Ͳ��Ա��ģ���Ҫ�Զ����Ե��û�����øýӿڼ����Զ����Թ���
 �� �� ֵ:  1:�ɹ�  0: ��Ч�ľ��
 ��    ����param1 ָ�����ӵľ��
**************************************************************/
//##ModelId=49B87B890129
int CSttpEngine::XJSTTPActiveTest(XJHANDLE pClientHandle)
{
	if(m_hModule == NULL)
		return 0;
	if(STTPActiveTest == NULL)
		return 0;
	return STTPActiveTest(pClientHandle);
}

/*************************************************************
 �� �� ����XJSTTPRead()
 ���ܸ�Ҫ����ָ�������϶�ȡһ��Sttp��������
 �� �� ֵ: 1:�ɹ� ͬ��ģʽ�³�ʱ��������ʱ����0���첽ģʽ�����������̷���0�������ж�ʱ����-1
 ��    ����param1	ָ�����ӵľ��
		   Param2	��������
**************************************************************/
//##ModelId=49B87B890138
int CSttpEngine::XJSTTPRead(XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData)
{
	if(m_hModule == NULL)
		return 0;
	if(STTPRead == NULL)
		return 0;
	return STTPRead(pClientHandle, pSttpData);
}

/*************************************************************
 �� �� ����XJSTTPWrite()
 ���ܸ�Ҫ����ָ��������д��һ��Sttp��������,ͬ��ģʽ�·���֮�󷵻ؽ�����첽ģʽ�·��뷢�ͻ������̷���
 �� �� ֵ: 1:�ɹ� 0:��Ч���� -1:�����ж� -2:��������
 ��    ����param1	ָ�����ӵľ��
		   Param2	��������
**************************************************************/
//##ModelId=49B87B890148
int CSttpEngine::XJSTTPWrite(XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData)
{
	if(m_hModule == NULL)
		return 0;
	if(STTPWrite == NULL)
		return 0;
	return STTPWrite(pClientHandle, pSttpData);
}

/*************************************************************
 �� �� ����XJSTTPSetOption()
 ���ܸ�Ҫ��Ϊָ�����������ö�����
 �� �� ֵ:  1:�ɹ� 0:ʧ��
 ��    ����param1	ָ�����ӵľ��
		   Param2	��������  1�����ճ�ʱ  2�����ͳ�ʱ
		   Param3	��ʱʱ�䣨ms��
		   Param4	����
**************************************************************/
//##ModelId=49B87B890157
int CSttpEngine::XJSTTPSetOption(XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved)
{
	if(m_hModule == NULL)
		return 0;
	if(STTPSetOption == NULL)
		return 0;
	return STTPSetOption(pClientHandle, pOptionName, pTime, pReserved);
}

/*************************************************************
 �� �� ����XJSTTPRegisterHandler()
 ���ܸ�Ҫ����ָ������ע��ص�����ָ�룬�ɹ�����ָ������Ŀǰֻ��������״̬�ı䣬ʹ�������Լ�ʵ�ָûص���������ʹ�ñ��ӿ�ע�ᵽָ�������Ӿ��
 �� �� ֵ: 1:�ɹ� 0:ʧ��
 ��    ����param1	ָ�����ӵľ��
		   Param2	�ص�����ָ��
		   Param3	������  0��onConnectChange
**************************************************************/
//##ModelId=49B87B890167
int CSttpEngine::XJSTTPRegisterHandler(XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask)
{
	if(m_hModule == NULL)
		return 0;
	if(STTPRegisterHandler == NULL)
		return 0;
	return STTPRegisterHandler(pClientHandle, pFunction, pMask);
}

/*************************************************************
 �� �� ����BuildDataFor20015()
 ���ܸ�Ҫ��Ϊ20015(������ֵ����Ҫ��)�����齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����ID
		   Param3	CPU
		   Param4	��ֵ����
		   Param5	������Դ
		   Param6	��ֵ��
**************************************************************/
//##ModelId=49B87B890177
BOOL CSttpEngine::BuildDataFor20015PTSetting(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom /* = 0 */, int nDataNum /* = 0 */)
{
	try
	{
		//���FULL_DATA
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20015;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 21;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nZone = nZone;	//��ֵ����
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
		sttpData.sttp_body.nSource = nDataFrom;	//��ֵ��Դ
		sttpData.sttp_body.nChangeConfigType = nDataNum; //��ֵ��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20015PTSetting, ����", 3);
		return FALSE;
	}

	return TRUE;
}


/*************************************************************
 �� �� ����BuildDataFor20007PTAnalog()
 ���ܸ�Ҫ��Ϊ20007(����ģ��������ֵ����Ҫ��)�齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����װ��ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B89017E
BOOL CSttpEngine::BuildDataFor20007PTAnalog( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20007;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20007PTAnalog, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20011PTDigital()
 ���ܸ�Ҫ��Ϊ20011(��������������ֵ����Ҫ��)�齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����װ��ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B890189
BOOL CSttpEngine::BuildDataFor20011PTDigital( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20011;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20011PTDigital, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20110PTGeneral()
 ���ܸ�Ҫ��Ϊ20110(����������Ϣ����Ҫ��)�齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����װ��ID
		   param3	cpu
		   param4	��־λ,"1"�ٻ�MMI�汾��Ϣ����ʱCPU�������壻"0"�ٻ�CPU�汾��Ϣ
**************************************************************/
//##ModelId=49B87B890196
BOOL CSttpEngine::BuildDataFor20110PTGeneral( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nFlag )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20110;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 15;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
		sttpData.sttp_body.nFlag = nFlag;	//��־λ
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20110PTGeneral, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20106PTZone()
 ���ܸ�Ҫ��Ϊ20106(�ٻ���ֵ����Ҫ��)�齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����װ��ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B8901A5
BOOL CSttpEngine::BuildDataFor20106PTZone( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20106;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20106PTZone, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20173PTZone( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20173;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20106PTZone, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}


/*************************************************************
 �� �� ����BuildDataFor20108PTSoftBoard()
 ���ܸ�Ҫ��Ϊ20108(�ٻ���ѹ��Ҫ��)�齨����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   param2	����װ��ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B8901A9
BOOL CSttpEngine::BuildDataFor20108PTSoftBoard( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20108;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20108PTSoftBoard, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20042DownloadWaveFile()
 ���ܸ�Ҫ���齨20042(¼���ļ�����Ҫ��)�������� 
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	Ҫ���ص�¼���ļ���
		   param4	������Դ
		   param5	֧�ֶϵ�����ʱ���������ʼƫ���������´���ʱȫΪ��
		   param6	Ҫ�����ļ���װ���еı�ţ�������ԴΪ0ʱ��Ч��
**************************************************************/
//##ModelId=49B87B8901B6
BOOL CSttpEngine::BuildDataFor20042DownloadWaveFile( STTP_FULL_DATA& sttpData, CString PTID, CString FileName, int nDataFrom, int nOffset, int nNo )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20042;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 132;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		//�ļ���
		CString str = FileName;
//		sttpData.sttp_body.variant_member.file_data.strFileName = MSToXJ(str);
		sttpData.sttp_body.variant_member.file_data.strFileName = str;
		sttpData.sttp_body.variant_member.file_data.nOffset = nOffset; //ƫ����
		sttpData.sttp_body.variant_member.file_data.nFileNO = nNo; //���
		sttpData.sttp_body.nSource = nDataFrom; //������Դ
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20042DownloadWaveFile, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20025WaveFileList()
 ���ܸ�Ҫ���齨20025(¼���ļ��б�����Ҫ��)�������� 
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	������Դ 0-��װ�� 1-����վ
		   param4	��ʼ��ѯʱ��,��ʽYYmmddHHMMSS
		   param5	������ѯʱ��,��ʽYYmmddHHMMSS
**************************************************************/
//##ModelId=49B87B8901C5
BOOL CSttpEngine::BuildDataFor20025WaveFileList( STTP_FULL_DATA& sttpData, CString PTID, int nDataFrom, CTime tmStart, CTime tmEnd )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20025;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 40;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nSource = nDataFrom;	//����Դ
		CString strTime;
		strTime = tmStart.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, strTime, 12);//��ʼ��ѯʱ��
		strTime = tmEnd.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, strTime, 12);//������ѯʱ��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20025WaveFileList, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20051SettingModify()
 ���ܸ�Ҫ���齨20051(������ֵ�޸�Ҫ��) ��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
		   param4	��ֵ����
		   param5	����Ŀ�꣺0-����װ�ã�1-���ݿ�
		   param6	Ҫ�޸ĵĶ�ֵ����������
**************************************************************/
//##ModelId=49B87B8901CB
BOOL CSttpEngine::BuildDataFor20051SettingModify( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//�޸�ֵ����
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//����ͷ
		sttpData.sttp_head.uMsgID = 20051;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 21 + 16*nCount;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nZone = nZone; //��ֵ����
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nSource = nDataFrom; //����Ŀ��
		//Ҫ�޸ĵĶ�ֵ
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20051SettingModify, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20053SettingModifyExe()
 ���ܸ�Ҫ���齨20053(������ֵ�޸�ִ��Ҫ��) ��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
		   param4	��ֵ����
		   param5	����Ŀ�꣺0-����װ�ã�1-���ݿ�
		   param6	Ҫ�޸ĵĶ�ֵ����������
**************************************************************/
//##ModelId=49B87B8901DA
BOOL CSttpEngine::BuildDataFor20053SettingModifyExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//�޸�ֵ����
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//����ͷ
		sttpData.sttp_head.uMsgID = 20053;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 21 + 16*nCount;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nZone = nZone; //��ֵ����
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nSource = nDataFrom; //����Ŀ��
		//Ҫ�޸ĵĶ�ֵ
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20051SettingModifyExe, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20047SettingChange()
 ���ܸ�Ҫ���齨20047(������ֵ�л�ԤУҪ��)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	Ҫ�л����Ķ�ֵ��
**************************************************************/
//##ModelId=49B87B8901E5
BOOL CSttpEngine::BuildDataFor20047SettingChange( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20047;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
		sttpData.sttp_body.nZone = nZone;	//��ֵ��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20047SettingChange, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20169SettingChange( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20169;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 18 + 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��

		STTP_DATA sData;
		memset(&sData, '\0', sizeof(STTP_DATA));
		sData.id = id;
		CString str;
		str.Format("%d", nZone);
		sData.str_value = str;
		sttpData.sttp_body.variant_member.value_data.push_back(sData);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20169SettingChange, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20055SettingChangeExe()
 ���ܸ�Ҫ���齨20055(������ֵ�л�ִ��Ҫ��)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	Ҫ�л����Ķ�ֵ��
**************************************************************/
//##ModelId=49B87B8901EA
BOOL CSttpEngine::BuildDataFor20055SettingChangeExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20055;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
		sttpData.sttp_body.nZone = nZone;	//��ֵ��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20055SettingChangeExe, ����", 3);
		return FALSE;
	}
	
	return TRUE;	
}

BOOL CSttpEngine::BuildDataFor20171SettingChangeExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id)
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		//����ͷ
		sttpData.sttp_head.uMsgID = 20171;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 18 + 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU��
		
		STTP_DATA sData;
		memset(&sData, '\0', sizeof(STTP_DATA));
		sData.id = id;
		CString str;
		str.Format("%d", nZone);
		sData.str_value = str;
		sttpData.sttp_body.variant_member.value_data.push_back(sData);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20171SettingChange, ����", 3);
		return FALSE;
	}
	
	return TRUE;	
}

/*************************************************************
 �� �� ����BuildDataFor20117SoftBoardModify()
 ���ܸ�Ҫ���齨20117(��ѹ��Ͷ��ԤУҪ��)��������
 �� �� ֵ: BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
		   param4	Ҫ�޸ĵ���ѹ������������
**************************************************************/
//##ModelId=49B87B8901F6
BOOL CSttpEngine::BuildDataFor20117SoftBoardModify( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);

		//�޸�ֵ����
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//����ͷ
		sttpData.sttp_head.uMsgID = 20117;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 16 + 9*nCount;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		//Ҫ�޸ĵĶ�ֵ
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			CString str;
			str.Format("20117 id:%d ----  values:%s \n ", data->id, data->str_value.c_str());
			TRACE0(str);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20117SoftBoardModify, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20113SoftBoardModifyExe()
 ���ܸ�Ҫ���齨20113(��ѹ��Ͷ��ִ��Ҫ��)��������
 �� �� ֵ: BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
		   param4	Ҫ�޸ĵ���ѹ������������
**************************************************************/
//##ModelId=49B87B8901FB
BOOL CSttpEngine::BuildDataFor20113SoftBoardModifyExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//�޸�ֵ����
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//����ͷ
		sttpData.sttp_head.uMsgID = 20113;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 16 + 9*nCount;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		//Ҫ�޸ĵĶ�ֵ
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			CString str;
			str.Format("20113 id:%d ----  values:%s \n", data->id, data->str_value.c_str());
			TRACE0(str);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20113SoftBoardModify, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20123WaveRecRun()
 ���ܸ�Ҫ���齨20123(¼����Զ����������Ҫ��)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
**************************************************************/
//##ModelId=49B87B890205
BOOL CSttpEngine::BuildDataFor20123WaveRecRun( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);

		//����ͷ
		sttpData.sttp_head.uMsgID = 20123;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20123WaveRecRun, ����", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 �� �� ��:	BuildDataFor20059PTSetTime()
 ���ܸ�Ҫ���齨20059(������ʱҪ��)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
**************************************************************/
//##ModelId=49B87B890209
BOOL CSttpEngine::BuildDataFor20059PTSetTime( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20059;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20059PTSetTime, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20088PTTime()
 ���ܸ�Ҫ���齨20088(�ٻ�װ��ʱ��)����
 �� �� ֵ: �齨�ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
**************************************************************/
//##ModelId=49B87B890213
BOOL CSttpEngine::BuildDataFor20088PTTime( STTP_FULL_DATA& sttpData, CString PTID )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20088;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 12;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20088PTTime, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20057ResetPT()
 ���ܸ�Ҫ���齨20057(�����źŸ���)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	װ��ID
		   param3	CPU��
**************************************************************/
//##ModelId=49B87B890216
BOOL CSttpEngine::BuildDataFor20057ResetPT( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20057;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20057ResetPT, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20137Cancel()
 ���ܸ�Ҫ���齨20137(ȡ�����������)��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	����ID
		   Param3	װ��ID
		   Param4	CPU
		   Param5	��ֵ����
		   Param6	�¼�����
		   Param7	��ʼʱ��
		   Param8	����ʱ��
		   Param9	¼���ļ���
**************************************************************/
//##ModelId=49B87B89021A
BOOL CSttpEngine::BuildDataFor20137Cancel( STTP_FULL_DATA& sttpData, int MsgID, CString PTID, int nCPU, int nZone /*= 0*/, int nEventType /*= 0*/, CString strStartTime /*= ""*/, CString strEndTime /*= ""*/, CString strFile /*= ""*/ )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20137;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 146;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		sttpData.sttp_body.nMsgId = MsgID; //����ID
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//����ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nZone = nZone; //��ֵ����
		sttpData.sttp_body.nEventType = nEventType; //�¼�����
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, strStartTime, 12);//��ʼʱ��
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, strEndTime, 12);//����ʱ��
		strFile = "sttpData.sttp_body.nMsgId";
//		sttpData.sttp_body.strFilenameWithPath = MSToXJ(strFile);//¼ȡ�ļ���
		sttpData.sttp_body.strFilenameWithPath = strFile;//¼ȡ�ļ���
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20057ResetPT, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����XJSTTPConnectDestination()
 ���ܸ�Ҫ����ָ��Ŀ�꽨������
 �� �� ֵ: ������Sttp�ͻ��˶���ľ��
 ��    ����param1	ָ������Ŀ��Ľṹ��
		   Param2	�ͻ��˵����ͱ�ţ����ڵ�¼��֤
		   Param3	�������״̬ 0-�ж� 1-����
		   Param4	��־�ļ�Ŀ¼
		   Param5	����ģʽ
**************************************************************/
//##ModelId=49B87B89010A
XJHANDLE CSttpEngine::XJSTTPConnectDestination( const STRUCT_DESTINATION_SERVER& pDestination , int pClientModle , int & pConnected, char* sLogPath, int pRunModle )
{
	if(m_hModule == NULL)
	{
		pConnected = 0;
		return NULL;
	}
	if(STTPConnectDestination == NULL)
	{
		pConnected = 0;
		return NULL;
	}
	return STTPConnectDestination(pDestination, pClientModle, pConnected, sLogPath, pRunModle);
}

/*************************************************************
 �� �� ����BuildDataFor20153Batch()
 ���ܸ�Ҫ���齨�����ٻ���ϢҪ��(20153)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�������ݽṹ
		   Param2	��������
		   Param3	վID����ָ��
**************************************************************/
//##ModelId=49B87B890228
BOOL CSttpEngine::BuildDataFor20153Batch( STTP_FULL_DATA& sttpData , int nType, CStringArray* arrStation)
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20153;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 0;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
        sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		sttpData.sttp_body.nEventType = nType;
		if(arrStation != NULL)
		{
			
			for(int i = 0; i <arrStation->GetSize(); i++)
			{
				STTP_DATA data;
				bzero(&data, sizeof(STTP_DATA));
				data.str_value = arrStation->GetAt(i);
				sttpData.sttp_body.variant_member.value_data.push_back(data);
			}
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20153Batch, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor20105HistoryEvent()
 ���ܸ�Ҫ���齨������ʷ�����¼�Ҫ��(20105)���� 
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�������ݽṹ
		   Param2	����ID
		   Param3	�¼�����,1-������Ϣ 2���Լ���Ϣ 3��Ӳѹ����Ϣ 4��SOE  5-��� 6-�澯��Ϣ 0-ȫ�� 
		   Param4	��ʼʱ��
		   Param5	����ʱ��
		   Param6	������Դ
**************************************************************/
//##ModelId=49B87B89022C
BOOL CSttpEngine::BuildDataFor20105HistoryEvent( STTP_FULL_DATA& sttpData, CString PT_ID, int nType, CTime tmStart, CTime tmEnd, int nSrc )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20105;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 38;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, PT_ID, 12);//����ID
		sttpData.sttp_body.nEventType = nType;	//�¼�����
		CString str;
		str = tmStart.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, str, 12);//��ʼʱ��
		str = tmEnd.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, str, 12);//����ʱ��
		sttpData.sttp_body.nSource = nSrc; //������Դ
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20105HistoryEvent, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor00203CommonFileList()
 ���ܸ�Ҫ���齨�ٻ�ͨ���ļ��б�Ҫ��(00203)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�������ݽṹ
		   Param2	��վID
		   Param3	��������:1-���ļ�����,2-ָ��ʱ����ڵ��ļ�����
		   Param4	��ѯ��ʼʱ��
		   Param5	��ѯ����ʱ��
		   Param6	��ѯ����
**************************************************************/
//##ModelId=49B87B890237
BOOL CSttpEngine::BuildDataFor00203CommonFileList( STTP_FULL_DATA& sttpData, CString sStationID, int nConditionType, CTime tmStart, CTime tmEnd, CString sCondition )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 203;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = VAR;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = VAR; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, sStationID, 12);//��վID
		sttpData.sttp_body.nEventType = nConditionType;	//��������
		CString str;
		str = tmStart.Format("%Y-%m-%d %H:%M:%S");
		strncpy(sttpData.sttp_body.ch_time_20_BIT1, str, 20);//��ʼʱ��
		str = tmEnd.Format("%Y-%m-%d %H:%M:%S");
		strncpy(sttpData.sttp_body.ch_time_20_BIT2, str, 20);//����ʱ��
		sttpData.sttp_body.strFilenameWithPath = sCondition; //����
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor00203CommonFileList, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildDataFor00210CommonFileDown()
 ���ܸ�Ҫ���齨ͨ���ļ�����Ҫ��(00210)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�������ݽṹ
		   Param2	��վID
		   Param3	�ļ���
		   Param4	ƫ����
		   Param5	��վIP
		   Param6	��Ϣ��ʶ��
**************************************************************/
//##ModelId=49B87B89023E
BOOL CSttpEngine::BuildDataFor00210CommonFileDown( STTP_FULL_DATA& sttpData, CString sStationID, CString sFileName, int nOffest, CString sIP, int nRII /*= 0*/ )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 210;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 264;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		strncpy(sttpData.sttp_body.ch_pt_id, sStationID, 12);//��վID
		sttpData.sttp_body.variant_member.file_data.strFileName = sFileName;//�ļ�����
		sttpData.sttp_body.variant_member.file_data.nOffset = nOffest; //ƫ����
		sttpData.sttp_body.variant_member.file_data.strReport = sIP; //��վIP
		sttpData.sttp_head.uMsgRii = nRII; //������Ϣ��ʶ��
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor00210CommonFileDown, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����XJSTTPSetLogPath()
 ���ܸ�Ҫ��������־·��
 �� �� ֵ: 1:�ɹ�	0:ʧ��
 ��    ����param1	ָ�����
		   Param2	��־·��
**************************************************************/
//##ModelId=49B87B89016B
int CSttpEngine::XJSTTPSetLogPath( XJHANDLE pClientHandle ,const char* pLogPath )
{
	if(m_hModule == NULL)
		return 0;
	if(STTPSetLogPath == NULL)
		return 0;
	return STTPSetLogPath(pClientHandle, pLogPath);
}

/*************************************************************
 �� �� ����BuildDataFor20151FaultPacket()
 ���ܸ�Ҫ���齨�Զ��鵵��Ϣ���仯֪ͨ(20151)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�������ݽṹ
		   Param2	��Ϣ���仯����
		   Param3	��Ϣ��ָ��
**************************************************************/
//##ModelId=49B87B890247
BOOL CSttpEngine::BuildDataFor20151FaultPacket( STTP_FULL_DATA& sttpData, int nType, CFaultReport* pReport )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);

		if(pReport == NULL)
			return FALSE;
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20151;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 153;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		//1.��Ϣ�����
		sttpData.sttp_body.nEventType = nType;
		//2.��Ϣ�����
		CString str;
		str.Format("%d", pReport->m_nID);
		strncpy(sttpData.sttp_body.ch_check_code, str, 12);
		//3.��Ϣ������
		sttpData.sttp_body.strMessage = pReport->m_sName;
		//4.����һ���豸ID
		strncpy(sttpData.sttp_body.ch_HandlerName, pReport->m_sPriDeviceID, 12);
		//5.����ʱ��
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, pReport->m_tmFaultTime.Format("%Y%m%d%H%M%S"), 15);
		//6.���ϲ��
		strncpy(sttpData.sttp_body.ch_version, pReport->m_sFaultDistance, 30);
		//7.��������
		sttpData.sttp_body.strRelativePath = pReport->GetFaultTypeLetter(pReport->m_nFaultType);
		//8.��վ1ID
		strncpy(sttpData.sttp_body.ch_station_id, pReport->m_sStation1ID, 12);
		//9.��վ2ID
		strncpy(sttpData.sttp_body.ch_pt_id, pReport->m_sStation2ID, 12);
		//10.����ʱ�ĺ���ֵ
		sttpData.sttp_body.nFlag = pReport->m_nmsFaultTime;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20151FaultPacket, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20018FaultAlarm()
 ���ܸ�Ҫ: �齨�����澯�¼�����֪ͨ(20018)����,ֻ����CFaultEvent��ʾ�ĸ澯����ÿ���¼�ֻ��һ���ź���
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 �澯�¼�
**************************************************************/
BOOL CSttpEngine::BuildDataFor20018FaultAlarm( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		if(pEvent == NULL)
			return FALSE;
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20018;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 52;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);		
		//2.�澯�ź�
		STTP_DATA alarmData;
		alarmData.id = atoi(pEvent->m_sEventDef); //�ź�ID
		alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //ֵ
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(alarmData.ch_Time, str, 15);	
		sttpData.sttp_body.variant_member.value_data.push_back(alarmData);	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20018FaultAlarm, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20010FaultDI()
 ���ܸ�Ҫ: �齨������������λ�¼�����֪ͨ(20010)����,ֻ����CFaultEvent��ʾ��DI
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 ��������λ�¼�
**************************************************************/
BOOL CSttpEngine::BuildDataFor20010FaultDI( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		if(pEvent == NULL)
			return FALSE;
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20010;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 42;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		Sleep(1000);
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.CPU
		sttpData.sttp_body.nCpu = pEvent->m_nCPU;
		//3.ʱ��
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, str, 15);
		//3.DI�ź�
		STTP_DATA alarmData;
		alarmData.id = atoi(pEvent->m_sEventDef); //�ź�ID
		alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //ֵ
		sttpData.sttp_body.variant_member.value_data.push_back(alarmData);	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20010FaultDI, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20022FaultAction()
 ���ܸ�Ҫ: 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CSttpEngine::BuildDataFor20022FaultAction( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		if(listEvent == NULL)
			return FALSE;
		if(listEvent->GetCount() < 1)
			return FALSE;
		//��������¼���������ź����ĸ���
		int nSign = 0;
		int nChr = 0;
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20022;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 42;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		CFaultEvent* pEvent = listEvent->GetHead();
		if(pEvent == NULL)
			return FALSE;
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.����ʱ��
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, str, 15);
		//2.���ʱ��
		str.Format("%s%d", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"), pEvent->m_nmsReceive);
		strncpy(sttpData.sttp_body.ch_time_15_BIT2, str, 15);
		//3.ѭ����
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			if(pEvent == NULL)
				continue;
			STTP_DATA alarmData;
			alarmData.id = atoi(pEvent->m_sEventDef); //�ź�ID
			alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //ֵ
			if(pEvent->m_nType == 99)//�ź���
			{
				str.Format("%s%d", pEvent->m_tmSign.Format("%y%m%d%H%M%S"), pEvent->m_nmsSign);
				strncpy(alarmData.ch_Time, str, 15);
				sttpData.sttp_body.variant_member.value_data.push_back(alarmData);
				nSign++;
			}
			else if(pEvent->m_nType == 1)//������
			{
				sttpData.sttp_body.variant_member.value_data2.push_back(alarmData);
				nChr++;
			}
		}	
		//���㱨�ĳ���
		sttpData.sttp_head.uMsgLength = 42 + (2+ 12*nChr) + (2 + 24*nSign);	//���ĳ���
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20022FaultAction, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20116FaultAlarm()
 ���ܸ�Ҫ: �齨�����澯��ʷ�¼�����֪ͨ(20116)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 �澯�¼�
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultAlarm( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);

		if(listEvent->GetCount() < 1)
			return FALSE;

		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20116;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 52;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.�¼�����.�澯
		sttpData.sttp_body.nEventType = 2;
		//�澯ѭ��
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			STTP_DBDATA_RECORD datarecord;
			datarecord.condition_list.clear();
			datarecord.field_list.clear();
			//����װ��ID
			datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
			//����ʱ��
			CString str;
			str.Format("%s", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"));
			datarecord.strReserved2 = MSToXJ(str);

			STTP_DBDATA_UNIT dbdata;
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //�ź�ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //ֵ
			//CString str;
			str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
			dbdata.strReserved = MSToXJ(str);

			datarecord.field_list.push_back(dbdata);
			sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultAlarm, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20116FaultDI()
 ���ܸ�Ҫ: �齨������������ʷ�¼�����֪ͨ(20116)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 �������¼�
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultDI( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		if(listEvent->GetCount() < 1)
			return FALSE;
		
		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20116;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 52;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.�¼�����.DI
		sttpData.sttp_body.nEventType = 3;
		//int size = listEvent->GetSize();
		//sttpData.sttp_body.variant_member.dataflat_data.record_list.size()
		//�澯ѭ��
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			if(pEvent == NULL)
				continue;
			STTP_DBDATA_RECORD datarecord;
			datarecord.condition_list.clear();
			datarecord.field_list.clear();
			//����װ��ID
			datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
			//CPU
			datarecord.nReserved = pEvent->m_nCPU;
			//����ʱ��
			CString str;
			str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
			WriteLog("CSttpEngine::BuildDataFor20116FaultDI::����ʱ��" + str, 3);
			datarecord.strReserved2 = MSToXJ(str);

			STTP_DBDATA_UNIT dbdata;
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //�ź�ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //ֵ
			
			datarecord.field_list.push_back(dbdata);
			sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultDI, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20116FaultAction()
 ���ܸ�Ҫ: �齨����������ʷ�¼�����֪ͨ(20116)����,һ��ֻ����һ�������Ķ����¼�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 �����¼�
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultAction( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		if(listEvent->GetCount() < 1)
			return FALSE;
		
		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20116;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 52;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//������
		//1.����װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.�¼�����.����
		sttpData.sttp_body.nEventType = 1;

		STTP_DBDATA_RECORD datarecord;
		datarecord.condition_list.clear();
		datarecord.field_list.clear();
		//����װ��ID
		datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
		//��ʼʱ��
		CString str;
		str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		datarecord.strReserved2 = MSToXJ(str);
		//���ʱ��
		str.Format("%s%03d", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"), pEvent->m_nmsReceive);
		datarecord.strReserved3 = MSToXJ(str);
		//�澯ѭ��
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			
			STTP_DBDATA_UNIT dbdata;
			memset(&dbdata, 0, sizeof(STTP_DBDATA_UNIT));
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //�ź�ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //ֵ

			if(pEvent->m_nType == 99)//�ź���
			{
				str.Format("%s%03d", pEvent->m_tmSign.Format("%y%m%d%H%M%S"), pEvent->m_nmsSign);
				dbdata.strReserved = MSToXJ(str);
				datarecord.condition_list.push_back(dbdata);
			}
			else if(pEvent->m_nType == 1)//������
			{
				datarecord.field_list.push_back(dbdata);
			}	
		}
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultDI, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildDataFor20145StationStatus()
 ���ܸ�Ҫ: �齨��վͨѶ״̬����Ҫ�� (20145) ����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 ��վID
		   Param3 ͨ����
**************************************************************/
BOOL CSttpEngine::BuildDataFor20145StationStatus( STTP_FULL_DATA& sttpData, CString strStationID, int nChannel )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20145;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 14;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//������
		//1.��վID
		strncpy(sttpData.sttp_body.ch_station_id, strStationID, 12);
		//2.ͨ����
		sttpData.sttp_body.nFlag = nChannel;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20145StationStatus, ����", 3);
		return FALSE;
	}
	return TRUE;
}


/*************************************************************
 �� �� ��: BuildDataFor20079UpdatePtRunStatus()
 ���ܸ�Ҫ: �齨װ������״̬����(20079)����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �������ݽṹ
		   Param2 װ��ID
		   Param3 װ��״̬ID
**************************************************************/
BOOL CSttpEngine::BuildDataFor20079UpdatePtRunStatus(STTP_FULL_DATA& sttpData, CString& strPtID, int nPtStatus)
{
	try
	{
		CString strTime;

		// 1 time
		SYSTEMTIME time;
		GetLocalTime(&time);
		strTime.Format("%02d%02d%02d%02d%02d%02d", time.wYear-2000, time.wMonth, time.wDay, time.wHour,
			time.wMinute,time.wSecond);

        //��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20079;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 25;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
        sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, (LPCSTR)strPtID, strlen((LPCSTR)strPtID));
		sttpData.sttp_body.nStatus = nPtStatus;
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, (LPCSTR)strTime, strlen((LPCSTR)strTime));
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox("����װ������״̬(ID 20079)���±��ĺ������!");
	}
	return FALSE;
}

/*************************************************************
 CreateDate: 30:12:2009     Author:LYH
 �� �� ��: FreeSttpLib()
 ���ܸ�Ҫ: ж��STTP�ͻ��˶�̬��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CSttpEngine::FreeSttpLib()
{
	if(m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}


BOOL CSttpEngine::BuildDataFor20162RenameOscFile( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent )
{

	try
	{
		MYASSERT_BOOL(pEvent);
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20162;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;

//		Sleep(1000);
		//������
		//1.װ��ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.ʱ��
		CString strTime;
		strTime = pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		strTime.Format(strTime + ".%d", pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_HandlerName, strTime, 26);
		//3.¼���ļ���С
		sttpData.sttp_body.nFlag = atoi(pEvent->m_sEventContent);
		//4.¼���ļ�Դ����
		CString strOldPath = pEvent->m_sOldDir;
		strOldPath.Replace("\\", "/");
		sttpData.sttp_body.strFilenameWithPath = strOldPath;
		CString strPath = pEvent->m_sDir;
		strPath.Replace("\\", "/");
		sttpData.sttp_body.strAbsolutePath = strPath;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20162RenameOscFile, ����", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL  CSttpEngine::BuildDataFor20138StationConfigChange(STTP_FULL_DATA& sttpData, CString strStationName)
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20138;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		//sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strStationName, 12);
	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138StationConfigChange, ����", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL CSttpEngine::BuildDataFor20003CommStatus(STTP_FULL_DATA& sttpData, CString strPTID)
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20003;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 12;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strPTID, 12);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20003CommStatus, ����", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL CSttpEngine::BuildDataFor20085CommStatus(STTP_FULL_DATA& sttpData, CString strOscID)
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20085;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 12;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strOscID, 12);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20085CommStatus, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2012/6/15  Author:LYH
������:   BuildDataFor20138StationAdd	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: STTP_FULL_DATA & sttpData	
����: CString strStationName	
*****************************************************/
BOOL CSttpEngine::BuildDataFor20138StationAdd( STTP_FULL_DATA& sttpData, CString strStationName, CString strSepName )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20138;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		//sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
		strncpy(sttpData.sttp_body.ch_pt_id, strSepName, 12);
		STTP_DBDATA_RECORD record;
		record.nReserved = 11;//����һ����վ
		STTP_DBDATA_UNIT unit;
		unit.strValue = strStationName;
		record.field_list.push_back(unit);
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(record);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138StationAdd, ����", 3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2012/6/15  Author:LYH
������:   BuildDataFor20138SecConfigChange	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: STTP_FULL_DATA & sttpData	
����: CString strStationName	
����: CString strDevID	
����: BOOL bAdd	
*****************************************************/
BOOL CSttpEngine::BuildDataFor20138SecConfigChange( STTP_FULL_DATA& sttpData, CString strStationName, CString strDevID, BOOL bAdd /*= FALSE*/ )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20138;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		//sttpData.sttp_head.uMsgLength = 16;	//���ĳ���
		sttpData.sttp_head.uMsgLengthType = FIX; //�������
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
		strncpy(sttpData.sttp_body.ch_pt_id, strStationName, 12);
		STTP_DBDATA_RECORD record;
		if(bAdd)
			record.nReserved = 12;//����һ��װ��
		else
			record.nReserved = 2;
		STTP_DBDATA_UNIT unit;
		unit.strValue = strDevID;
		record.field_list.push_back(unit);
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(record);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138SecConfigChange, ����", 3);
		return FALSE;
	}
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20178StationInit( STTP_FULL_DATA& sttpData, CString sStationID, int nInitFlag, int nStatus )
{
	try
	{
		//��սṹ
		zero_sttp_full_data(sttpData);
		
		//����ͷ
		sttpData.sttp_head.uMsgID = 20178;	//����ID
		sttpData.sttp_head.uMsgType = INQ;	//�������
		sttpData.sttp_head.uMsgLength = 279;	//���ĳ���
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
		strncpy(sttpData.sttp_body.ch_station_id, sStationID, 12);
		sttpData.sttp_body.nFlag = nInitFlag;
		sttpData.sttp_body.nStatus = nStatus;
		CTime tm = CTime::GetCurrentTime();
		CString sTime;
		sTime = tm.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, sTime, 12);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20177StationInit, ����", 3);
		return FALSE;
	}
	return TRUE;
}
