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

/** @brief           与指定客户端进行连接*/
XJHANDLE (*STTPConnectDestination)(const STRUCT_DESTINATION_SERVER& pDestination , int pClientModle , int & pConnected, const char* pLogPath, int pRunModle);
/** @brief           连接通讯服务器*/
XJHANDLE (*SttpConnectServer)(int pClientModle, int & pConnected, const char* pLogPath, int pRunModle);
/** @brief           关闭与服务端的连接*/
int (*SttpCloseConnection)(XJHANDLE pClientHandle);
/** @brief           激活指定连接的自动测试*/
int (*STTPActiveTest) (XJHANDLE pClientHandle);
/** @brief           读取STTP数据*/
int (*STTPRead)(XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);
/** @brief           写入STTP数据*/
int (*STTPWrite)(XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);
/** @brief           参数设置*/
int (*STTPSetOption)(XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);
/** @brief           注册处理函数*/
int (*STTPRegisterHandler)(XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );
/** @brief           日志路径设置*/
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
		//连接指定客户端
		STTPConnectDestination = (XJHANDLE (*)(const STRUCT_DESTINATION_SERVER&, int, int &, const char* , int))GetProcAddress(m_hModule, "STTPConnectDestination");
		//连接通讯服务器
		SttpConnectServer = (XJHANDLE (*)(int, int &, const char* , int))GetProcAddress(m_hModule, "STTPConnectServer");
		//关闭与服务端的连接
		SttpCloseConnection = (int (*)(XJHANDLE))GetProcAddress(m_hModule, "STTPCloseConnection");
		//激活指定连接的自动测试
		STTPActiveTest = (int (*) (XJHANDLE))GetProcAddress(m_hModule, "STTPActiveTest");
		/** @brief           读取STTP数据*/
		STTPRead = (int (*)(XJHANDLE, STTP_FULL_DATA&))GetProcAddress(m_hModule, "STTPRead");
		//写入STTP数据
		STTPWrite = (int (*)(XJHANDLE, const STTP_FULL_DATA&))GetProcAddress(m_hModule, "STTPWrite");
		//参数设置
		STTPSetOption = (int (*)(XJHANDLE, int, int, int))GetProcAddress(m_hModule, "STTPSetOption");
		//注册处理函数
		STTPRegisterHandler = (int (*)(XJHANDLE, PFUNCALLBACK, int))GetProcAddress(m_hModule, "STTPRegisterHandler");
		//日志路径设置
		STTPSetLogPath = (int(*)(XJHANDLE, const char*))GetProcAddress(m_hModule, "STTPSetLogPath");
	}
	return TRUE;
}

/*************************************************************
 函 数 名：XJSttpConnectServer()
 功能概要：建立与通信服务器的连接,不需指定对端地址，动态库自行从数据库中获得
 返 回 值: 创建的Sttp客户端对象的句柄
 参    数：param1 客户端的类型编号，用于登录验证
		   Param2 输出连接状态 0-中断 1-连接
		   Param3 日志文件目录
		   param4 运行模式0：同步模式（默认）1：异步模式
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
 函 数 名：XJSttpCloseConnection()
 功能概要：关闭指定句柄上的连接
 返 回 值:  1:成功 0:无效的句柄
 参    数：param1 指定要关闭的句柄
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
 函 数 名：XJSTTPActiveTest()
 功能概要：激活指定连接上的自动测试。默认情况下客户端动态库不自动向服务端发送测试报文，需要自动测试的用户需调用该接口激活自动测试功能
 返 回 值:  1:成功  0: 无效的句柄
 参    数：param1 指定连接的句柄
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
 函 数 名：XJSTTPRead()
 功能概要：从指定连接上读取一条Sttp报文数据
 返 回 值: 1:成功 同步模式下超时后无数据时返回0、异步模式下无数据立刻返回0，网络中断时返回-1
 参    数：param1	指定连接的句柄
		   Param2	报文数据
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
 函 数 名：XJSTTPWrite()
 功能概要：从指定连接上写入一条Sttp报文数据,同步模式下发送之后返回结果、异步模式下放入发送缓存立刻返回
 返 回 值: 1:成功 0:无效数据 -1:连接中断 -2:缓存已满
 参    数：param1	指定连接的句柄
		   Param2	报文数据
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
 函 数 名：XJSTTPSetOption()
 功能概要：为指定的连接设置定参数
 返 回 值:  1:成功 0:失败
 参    数：param1	指定连接的句柄
		   Param2	参数类型  1：接收超时  2：发送超时
		   Param3	超时时间（ms）
		   Param4	备用
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
 函 数 名：XJSTTPRegisterHandler()
 功能概要：向指定连接注册回调函数指针，由功能码指定功能目前只考虑连接状态改变，使用者需自己实现该回调函数，并使用本接口注册到指定的连接句柄
 返 回 值: 1:成功 0:失败
 参    数：param1	指定连接的句柄
		   Param2	回调函数指针
		   Param3	功能码  0：onConnectChange
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
 函 数 名：BuildDataFor20015()
 功能概要：为20015(保护定值下载要求)报文组建数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护ID
		   Param3	CPU
		   Param4	定值区号
		   Param5	数据来源
		   Param6	定值数
**************************************************************/
//##ModelId=49B87B890177
BOOL CSttpEngine::BuildDataFor20015PTSetting(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom /* = 0 */, int nDataNum /* = 0 */)
{
	try
	{
		//清空FULL_DATA
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20015;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 21;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nZone = nZone;	//定值区号
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
		sttpData.sttp_body.nSource = nDataFrom;	//定值来源
		sttpData.sttp_body.nChangeConfigType = nDataNum; //定值数
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20015PTSetting, 错误", 3);
		return FALSE;
	}

	return TRUE;
}


/*************************************************************
 函 数 名：BuildDataFor20007PTAnalog()
 功能概要：为20007(保护模拟量采样值下载要求)组建报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护装置ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B89017E
BOOL CSttpEngine::BuildDataFor20007PTAnalog( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20007;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20007PTAnalog, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20011PTDigital()
 功能概要：为20011(保护开关量采样值下载要求)组建报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护装置ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B890189
BOOL CSttpEngine::BuildDataFor20011PTDigital( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20011;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20011PTDigital, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20110PTGeneral()
 功能概要：为20110(保护基本信息下载要求)组建报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护装置ID
		   param3	cpu
		   param4	标志位,"1"召唤MMI版本信息，此时CPU号无意义；"0"召唤CPU版本信息
**************************************************************/
//##ModelId=49B87B890196
BOOL CSttpEngine::BuildDataFor20110PTGeneral( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nFlag )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20110;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 15;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
		sttpData.sttp_body.nFlag = nFlag;	//标志位
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20110PTGeneral, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20106PTZone()
 功能概要：为20106(召唤定值区号要求)组建报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护装置ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B8901A5
BOOL CSttpEngine::BuildDataFor20106PTZone( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20106;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20106PTZone, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20173PTZone( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20173;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20106PTZone, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}


/*************************************************************
 函 数 名：BuildDataFor20108PTSoftBoard()
 功能概要：为20108(召唤软压板要求)组建报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   param2	保护装置ID
		   param3	cpu
**************************************************************/
//##ModelId=49B87B8901A9
BOOL CSttpEngine::BuildDataFor20108PTSoftBoard( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20108;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20108PTSoftBoard, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20042DownloadWaveFile()
 功能概要：组建20042(录波文件下载要求)报文数据 
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	要下载的录波文件名
		   param4	数据来源
		   param5	支持断点续传时，传输的起始偏移量，重新传输时全为零
		   param6	要下载文件在装置中的编号（当数据源为0时有效）
**************************************************************/
//##ModelId=49B87B8901B6
BOOL CSttpEngine::BuildDataFor20042DownloadWaveFile( STTP_FULL_DATA& sttpData, CString PTID, CString FileName, int nDataFrom, int nOffset, int nNo )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20042;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 132;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		//文件名
		CString str = FileName;
//		sttpData.sttp_body.variant_member.file_data.strFileName = MSToXJ(str);
		sttpData.sttp_body.variant_member.file_data.strFileName = str;
		sttpData.sttp_body.variant_member.file_data.nOffset = nOffset; //偏移量
		sttpData.sttp_body.variant_member.file_data.nFileNO = nNo; //编号
		sttpData.sttp_body.nSource = nDataFrom; //数据来源
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20042DownloadWaveFile, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20025WaveFileList()
 功能概要：组建20025(录波文件列表下载要求)报文数据 
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	数据来源 0-从装置 1-从子站
		   param4	起始查询时间,格式YYmmddHHMMSS
		   param5	结束查询时间,格式YYmmddHHMMSS
**************************************************************/
//##ModelId=49B87B8901C5
BOOL CSttpEngine::BuildDataFor20025WaveFileList( STTP_FULL_DATA& sttpData, CString PTID, int nDataFrom, CTime tmStart, CTime tmEnd )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20025;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 40;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nSource = nDataFrom;	//数据源
		CString strTime;
		strTime = tmStart.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, strTime, 12);//起始查询时间
		strTime = tmEnd.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, strTime, 12);//结束查询时间
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20025WaveFileList, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20051SettingModify()
 功能概要：组建20051(保护定值修改要求) 报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
		   param4	定值区号
		   param5	数据目标：0-保护装置，1-数据库
		   param6	要修改的定值数据项链表
**************************************************************/
//##ModelId=49B87B8901CB
BOOL CSttpEngine::BuildDataFor20051SettingModify( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//修改值个数
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//报文头
		sttpData.sttp_head.uMsgID = 20051;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 21 + 16*nCount;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nZone = nZone; //定值区号
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nSource = nDataFrom; //数据目标
		//要修改的定值
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20051SettingModify, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20053SettingModifyExe()
 功能概要：组建20053(保护定值修改执行要求) 报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
		   param4	定值区号
		   param5	数据目标：0-保护装置，1-数据库
		   param6	要修改的定值数据项链表
**************************************************************/
//##ModelId=49B87B8901DA
BOOL CSttpEngine::BuildDataFor20053SettingModifyExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//修改值个数
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//报文头
		sttpData.sttp_head.uMsgID = 20053;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 21 + 16*nCount;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nZone = nZone; //定值区号
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nSource = nDataFrom; //数据目标
		//要修改的定值
		for(int i = 0; i < modifyList->GetSize(); i++)
		{
			STTP_DATA* data = (STTP_DATA*)modifyList->GetAt(i);
			sttpData.sttp_body.variant_member.value_data.push_back(*data);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20051SettingModifyExe, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20047SettingChange()
 功能概要：组建20047(保护定值切换预校要求)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	要切换到的定值区
**************************************************************/
//##ModelId=49B87B8901E5
BOOL CSttpEngine::BuildDataFor20047SettingChange( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20047;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
		sttpData.sttp_body.nZone = nZone;	//定值区
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20047SettingChange, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20169SettingChange( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20169;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 18 + 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号

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
		WriteLog("CSttpEngine::BuildDataFor20169SettingChange, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20055SettingChangeExe()
 功能概要：组建20055(保护定值切换执行要求)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	要切换到的定值区
**************************************************************/
//##ModelId=49B87B8901EA
BOOL CSttpEngine::BuildDataFor20055SettingChangeExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20055;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
		sttpData.sttp_body.nZone = nZone;	//定值区
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20055SettingChangeExe, 错误", 3);
		return FALSE;
	}
	
	return TRUE;	
}

BOOL CSttpEngine::BuildDataFor20171SettingChangeExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id)
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		//报文头
		sttpData.sttp_head.uMsgID = 20171;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 18 + 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu	 = nCPU;	//CPU号
		
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
		WriteLog("CSttpEngine::BuildDataFor20171SettingChange, 错误", 3);
		return FALSE;
	}
	
	return TRUE;	
}

/*************************************************************
 函 数 名：BuildDataFor20117SoftBoardModify()
 功能概要：组建20117(软压板投退预校要求)报文数据
 返 回 值: BOOL 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
		   param4	要修改的软压板数据项链表
**************************************************************/
//##ModelId=49B87B8901F6
BOOL CSttpEngine::BuildDataFor20117SoftBoardModify( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);

		//修改值个数
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//报文头
		sttpData.sttp_head.uMsgID = 20117;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 16 + 9*nCount;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
	    sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		//要修改的定值
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
		WriteLog("CSttpEngine::BuildDataFor20117SoftBoardModify, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20113SoftBoardModifyExe()
 功能概要：组建20113(软压板投退执行要求)报文数据
 返 回 值: BOOL 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
		   param4	要修改的软压板数据项链表
**************************************************************/
//##ModelId=49B87B8901FB
BOOL CSttpEngine::BuildDataFor20113SoftBoardModifyExe( STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//修改值个数
		int nCount = modifyList->GetSize();
		if(nCount <= 0)
			return FALSE;
		//报文头
		sttpData.sttp_head.uMsgID = 20113;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 16 + 9*nCount;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		//要修改的定值
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
		WriteLog("CSttpEngine::BuildDataFor20113SoftBoardModify, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20123WaveRecRun()
 功能概要：组建20123(录波器远方触发下载要求)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
**************************************************************/
//##ModelId=49B87B890205
BOOL CSttpEngine::BuildDataFor20123WaveRecRun( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);

		//报文头
		sttpData.sttp_head.uMsgID = 20123;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20123WaveRecRun, 错误", 3);
		return FALSE;
	}

	return TRUE;
}

/*************************************************************
 函 数 名:	BuildDataFor20059PTSetTime()
 功能概要：组建20059(保护对时要求)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
**************************************************************/
//##ModelId=49B87B890209
BOOL CSttpEngine::BuildDataFor20059PTSetTime( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20059;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20059PTSetTime, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20088PTTime()
 功能概要：组建20088(召唤装置时间)报文
 返 回 值: 组建成功返回TRUE,失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
**************************************************************/
//##ModelId=49B87B890213
BOOL CSttpEngine::BuildDataFor20088PTTime( STTP_FULL_DATA& sttpData, CString PTID )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20088;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 12;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20088PTTime, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20057ResetPT()
 功能概要：组建20057(保护信号复归)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	装置ID
		   param3	CPU号
**************************************************************/
//##ModelId=49B87B890216
BOOL CSttpEngine::BuildDataFor20057ResetPT( STTP_FULL_DATA& sttpData, CString PTID, int nCPU )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20057;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20057ResetPT, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20137Cancel()
 功能概要：组建20137(取消操作命令报文)报文数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	报文ID
		   Param3	装置ID
		   Param4	CPU
		   Param5	定值区号
		   Param6	事件类型
		   Param7	起始时间
		   Param8	结束时间
		   Param9	录波文件名
**************************************************************/
//##ModelId=49B87B89021A
BOOL CSttpEngine::BuildDataFor20137Cancel( STTP_FULL_DATA& sttpData, int MsgID, CString PTID, int nCPU, int nZone /*= 0*/, int nEventType /*= 0*/, CString strStartTime /*= ""*/, CString strEndTime /*= ""*/, CString strFile /*= ""*/ )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20137;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 146;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		sttpData.sttp_body.nMsgId = MsgID; //报文ID
		strncpy(sttpData.sttp_body.ch_pt_id, PTID, 12);//保护ID
		sttpData.sttp_body.nCpu = nCPU;	//CPU
		sttpData.sttp_body.nZone = nZone; //定值区号
		sttpData.sttp_body.nEventType = nEventType; //事件类型
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, strStartTime, 12);//起始时间
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, strEndTime, 12);//结束时间
		strFile = "sttpData.sttp_body.nMsgId";
//		sttpData.sttp_body.strFilenameWithPath = MSToXJ(strFile);//录取文件名
		sttpData.sttp_body.strFilenameWithPath = strFile;//录取文件名
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20057ResetPT, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：XJSTTPConnectDestination()
 功能概要：与指定目标建立连接
 返 回 值: 创建的Sttp客户端对象的句柄
 参    数：param1	指定连接目标的结构体
		   Param2	客户端的类型编号，用于登录验证
		   Param3	输出连接状态 0-中断 1-连接
		   Param4	日志文件目录
		   Param5	运行模式
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
 函 数 名：BuildDataFor20153Batch()
 功能概要：组建批量召唤信息要求(20153)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据结构
		   Param2	命令类型
		   Param3	站ID数组指针
**************************************************************/
//##ModelId=49B87B890228
BOOL CSttpEngine::BuildDataFor20153Batch( STTP_FULL_DATA& sttpData , int nType, CStringArray* arrStation)
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20153;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 0;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
        sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
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
		WriteLog("CSttpEngine::BuildDataFor20153Batch, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor20105HistoryEvent()
 功能概要：组建下载历史动作事件要求(20105)报文 
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据结构
		   Param2	保护ID
		   Param3	事件类型,1-动作信息 2－自检信息 3－硬压板信息 4－SOE  5-测距 6-告警信息 0-全部 
		   Param4	起始时间
		   Param5	结束时间
		   Param6	数据来源
**************************************************************/
//##ModelId=49B87B89022C
BOOL CSttpEngine::BuildDataFor20105HistoryEvent( STTP_FULL_DATA& sttpData, CString PT_ID, int nType, CTime tmStart, CTime tmEnd, int nSrc )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20105;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 38;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, PT_ID, 12);//保护ID
		sttpData.sttp_body.nEventType = nType;	//事件类型
		CString str;
		str = tmStart.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, str, 12);//起始时间
		str = tmEnd.Format("%y%m%d%H%M%S");
		strncpy(sttpData.sttp_body.ch_time_12_BIT2, str, 12);//结束时间
		sttpData.sttp_body.nSource = nSrc; //数据来源
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20105HistoryEvent, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor00203CommonFileList()
 功能概要：组建召唤通用文件列表要求(00203)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据结构
		   Param2	厂站ID
		   Param3	条件类型:1-按文件名称,2-指定时间段内的文件名称
		   Param4	查询起始时间
		   Param5	查询结束时间
		   Param6	查询条件
**************************************************************/
//##ModelId=49B87B890237
BOOL CSttpEngine::BuildDataFor00203CommonFileList( STTP_FULL_DATA& sttpData, CString sStationID, int nConditionType, CTime tmStart, CTime tmEnd, CString sCondition )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 203;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = VAR;	//报文长度
		sttpData.sttp_head.uMsgLengthType = VAR; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, sStationID, 12);//厂站ID
		sttpData.sttp_body.nEventType = nConditionType;	//条件类型
		CString str;
		str = tmStart.Format("%Y-%m-%d %H:%M:%S");
		strncpy(sttpData.sttp_body.ch_time_20_BIT1, str, 20);//起始时间
		str = tmEnd.Format("%Y-%m-%d %H:%M:%S");
		strncpy(sttpData.sttp_body.ch_time_20_BIT2, str, 20);//结束时间
		sttpData.sttp_body.strFilenameWithPath = sCondition; //条件
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor00203CommonFileList, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildDataFor00210CommonFileDown()
 功能概要：组建通用文件下载要求(00210)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据结构
		   Param2	厂站ID
		   Param3	文件名
		   Param4	偏移量
		   Param5	子站IP
		   Param6	信息标识符
**************************************************************/
//##ModelId=49B87B89023E
BOOL CSttpEngine::BuildDataFor00210CommonFileDown( STTP_FULL_DATA& sttpData, CString sStationID, CString sFileName, int nOffest, CString sIP, int nRII /*= 0*/ )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 210;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 264;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		strncpy(sttpData.sttp_body.ch_pt_id, sStationID, 12);//厂站ID
		sttpData.sttp_body.variant_member.file_data.strFileName = sFileName;//文件名称
		sttpData.sttp_body.variant_member.file_data.nOffset = nOffest; //偏移量
		sttpData.sttp_body.variant_member.file_data.strReport = sIP; //子站IP
		sttpData.sttp_head.uMsgRii = nRII; //返回信息标识符
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor00210CommonFileDown, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：XJSTTPSetLogPath()
 功能概要：设置日志路径
 返 回 值: 1:成功	0:失败
 参    数：param1	指定句柄
		   Param2	日志路径
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
 函 数 名：BuildDataFor20151FaultPacket()
 功能概要：组建自动归档信息包变化通知(20151)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据结构
		   Param2	信息包变化类型
		   Param3	信息包指针
**************************************************************/
//##ModelId=49B87B890247
BOOL CSttpEngine::BuildDataFor20151FaultPacket( STTP_FULL_DATA& sttpData, int nType, CFaultReport* pReport )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);

		if(pReport == NULL)
			return FALSE;
		
		//报文头
		sttpData.sttp_head.uMsgID = 20151;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 153;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		//1.信息包类别
		sttpData.sttp_body.nEventType = nType;
		//2.信息包编号
		CString str;
		str.Format("%d", pReport->m_nID);
		strncpy(sttpData.sttp_body.ch_check_code, str, 12);
		//3.信息包名称
		sttpData.sttp_body.strMessage = pReport->m_sName;
		//4.所属一次设备ID
		strncpy(sttpData.sttp_body.ch_HandlerName, pReport->m_sPriDeviceID, 12);
		//5.故障时间
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, pReport->m_tmFaultTime.Format("%Y%m%d%H%M%S"), 15);
		//6.故障测距
		strncpy(sttpData.sttp_body.ch_version, pReport->m_sFaultDistance, 30);
		//7.故障类型
		sttpData.sttp_body.strRelativePath = pReport->GetFaultTypeLetter(pReport->m_nFaultType);
		//8.厂站1ID
		strncpy(sttpData.sttp_body.ch_station_id, pReport->m_sStation1ID, 12);
		//9.厂站2ID
		strncpy(sttpData.sttp_body.ch_pt_id, pReport->m_sStation2ID, 12);
		//10.故障时的毫秒值
		sttpData.sttp_body.nFlag = pReport->m_nmsFaultTime;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20151FaultPacket, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20018FaultAlarm()
 功能概要: 组建保护告警事件上载通知(20018)报文,只处理CFaultEvent表示的告警，故每条事件只有一个信号量
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 告警事件
**************************************************************/
BOOL CSttpEngine::BuildDataFor20018FaultAlarm( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		if(pEvent == NULL)
			return FALSE;
		
		//报文头
		sttpData.sttp_head.uMsgID = 20018;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 52;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);		
		//2.告警信号
		STTP_DATA alarmData;
		alarmData.id = atoi(pEvent->m_sEventDef); //信号ID
		alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //值
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(alarmData.ch_Time, str, 15);	
		sttpData.sttp_body.variant_member.value_data.push_back(alarmData);	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20018FaultAlarm, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20010FaultDI()
 功能概要: 组建保护开关量变位事件上载通知(20010)报文,只处理CFaultEvent表示的DI
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 开关量变位事件
**************************************************************/
BOOL CSttpEngine::BuildDataFor20010FaultDI( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		if(pEvent == NULL)
			return FALSE;
		
		//报文头
		sttpData.sttp_head.uMsgID = 20010;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 42;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		Sleep(1000);
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.CPU
		sttpData.sttp_body.nCpu = pEvent->m_nCPU;
		//3.时间
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, str, 15);
		//3.DI信号
		STTP_DATA alarmData;
		alarmData.id = atoi(pEvent->m_sEventDef); //信号ID
		alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //值
		sttpData.sttp_body.variant_member.value_data.push_back(alarmData);	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20010FaultDI, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20022FaultAction()
 功能概要: 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CSttpEngine::BuildDataFor20022FaultAction( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		if(listEvent == NULL)
			return FALSE;
		if(listEvent->GetCount() < 1)
			return FALSE;
		//计数，记录特征量和信号量的个数
		int nSign = 0;
		int nChr = 0;
		
		//报文头
		sttpData.sttp_head.uMsgID = 20022;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 42;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		CFaultEvent* pEvent = listEvent->GetHead();
		if(pEvent == NULL)
			return FALSE;
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.启动时间
		CString str;
		str.Format("%s%d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_time_15_BIT1, str, 15);
		//2.完成时间
		str.Format("%s%d", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"), pEvent->m_nmsReceive);
		strncpy(sttpData.sttp_body.ch_time_15_BIT2, str, 15);
		//3.循环体
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			if(pEvent == NULL)
				continue;
			STTP_DATA alarmData;
			alarmData.id = atoi(pEvent->m_sEventDef); //信号ID
			alarmData.str_value = MSToXJ(pEvent->m_sEventContent); //值
			if(pEvent->m_nType == 99)//信号量
			{
				str.Format("%s%d", pEvent->m_tmSign.Format("%y%m%d%H%M%S"), pEvent->m_nmsSign);
				strncpy(alarmData.ch_Time, str, 15);
				sttpData.sttp_body.variant_member.value_data.push_back(alarmData);
				nSign++;
			}
			else if(pEvent->m_nType == 1)//特征量
			{
				sttpData.sttp_body.variant_member.value_data2.push_back(alarmData);
				nChr++;
			}
		}	
		//计算报文长度
		sttpData.sttp_head.uMsgLength = 42 + (2+ 12*nChr) + (2 + 24*nSign);	//报文长度
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20022FaultAction, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20116FaultAlarm()
 功能概要: 组建保护告警历史事件上载通知(20116)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 告警事件
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultAlarm( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);

		if(listEvent->GetCount() < 1)
			return FALSE;

		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20116;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 52;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.事件类型.告警
		sttpData.sttp_body.nEventType = 2;
		//告警循环
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			STTP_DBDATA_RECORD datarecord;
			datarecord.condition_list.clear();
			datarecord.field_list.clear();
			//保护装置ID
			datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
			//接收时间
			CString str;
			str.Format("%s", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"));
			datarecord.strReserved2 = MSToXJ(str);

			STTP_DBDATA_UNIT dbdata;
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //信号ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //值
			//CString str;
			str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
			dbdata.strReserved = MSToXJ(str);

			datarecord.field_list.push_back(dbdata);
			sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultAlarm, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20116FaultDI()
 功能概要: 组建保护开关量历史事件上载通知(20116)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 开关量事件
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultDI( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		if(listEvent->GetCount() < 1)
			return FALSE;
		
		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20116;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 52;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.事件类型.DI
		sttpData.sttp_body.nEventType = 3;
		//int size = listEvent->GetSize();
		//sttpData.sttp_body.variant_member.dataflat_data.record_list.size()
		//告警循环
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			if(pEvent == NULL)
				continue;
			STTP_DBDATA_RECORD datarecord;
			datarecord.condition_list.clear();
			datarecord.field_list.clear();
			//保护装置ID
			datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
			//CPU
			datarecord.nReserved = pEvent->m_nCPU;
			//采样时间
			CString str;
			str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
			WriteLog("CSttpEngine::BuildDataFor20116FaultDI::采样时间" + str, 3);
			datarecord.strReserved2 = MSToXJ(str);

			STTP_DBDATA_UNIT dbdata;
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //信号ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //值
			
			datarecord.field_list.push_back(dbdata);
			sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
		}
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultDI, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20116FaultAction()
 功能概要: 组建保护动作历史事件上载通知(20116)报文,一次只处理一条完整的动作事件
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 动作事件
**************************************************************/
BOOL CSttpEngine::BuildDataFor20116FaultAction( STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		if(listEvent->GetCount() < 1)
			return FALSE;
		
		CFaultEvent* pEvent = (CFaultEvent*)listEvent->GetHead();
		
		MYASSERT_BOOL(pEvent);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20116;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 52;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		//Sleep(1000);
		//报文体
		//1.保护装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.事件类型.动作
		sttpData.sttp_body.nEventType = 1;

		STTP_DBDATA_RECORD datarecord;
		datarecord.condition_list.clear();
		datarecord.field_list.clear();
		//保护装置ID
		datarecord.strReserved1 = MSToXJ(pEvent->m_sSecID);
		//开始时间
		CString str;
		str.Format("%s%03d", pEvent->m_tmStart.Format("%y%m%d%H%M%S"), pEvent->m_nmsStart);
		datarecord.strReserved2 = MSToXJ(str);
		//完成时间
		str.Format("%s%03d", pEvent->m_tmReceiveTime.Format("%y%m%d%H%M%S"), pEvent->m_nmsReceive);
		datarecord.strReserved3 = MSToXJ(str);
		//告警循环
		POSITION pos = listEvent->GetHeadPosition();
		while(pos != NULL)
		{
			pEvent = (CFaultEvent*)listEvent->GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			
			STTP_DBDATA_UNIT dbdata;
			memset(&dbdata, 0, sizeof(STTP_DBDATA_UNIT));
			dbdata.nDataType = atoi(pEvent->m_sEventDef); //信号ID
			dbdata.strValue = MSToXJ(pEvent->m_sEventContent); //值

			if(pEvent->m_nType == 99)//信号量
			{
				str.Format("%s%03d", pEvent->m_tmSign.Format("%y%m%d%H%M%S"), pEvent->m_nmsSign);
				dbdata.strReserved = MSToXJ(str);
				datarecord.condition_list.push_back(dbdata);
			}
			else if(pEvent->m_nType == 1)//特征量
			{
				datarecord.field_list.push_back(dbdata);
			}	
		}
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(datarecord);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20116FaultDI, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名: BuildDataFor20145StationStatus()
 功能概要: 组建厂站通讯状态下载要求 (20145) 报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 厂站ID
		   Param3 通道号
**************************************************************/
BOOL CSttpEngine::BuildDataFor20145StationStatus( STTP_FULL_DATA& sttpData, CString strStationID, int nChannel )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20145;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 14;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		//报文体
		//1.厂站ID
		strncpy(sttpData.sttp_body.ch_station_id, strStationID, 12);
		//2.通道号
		sttpData.sttp_body.nFlag = nChannel;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20145StationStatus, 错误", 3);
		return FALSE;
	}
	return TRUE;
}


/*************************************************************
 函 数 名: BuildDataFor20079UpdatePtRunStatus()
 功能概要: 组建装置运行状态上载(20079)报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 报文数据结构
		   Param2 装置ID
		   Param3 装置状态ID
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

        //清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20079;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 25;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
        sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, (LPCSTR)strPtID, strlen((LPCSTR)strPtID));
		sttpData.sttp_body.nStatus = nPtStatus;
		strncpy(sttpData.sttp_body.ch_time_12_BIT1, (LPCSTR)strTime, strlen((LPCSTR)strTime));
		return TRUE;
	}
	catch (...)
	{
		AfxMessageBox("制作装置运行状态(ID 20079)更新报文函数溢出!");
	}
	return FALSE;
}

/*************************************************************
 CreateDate: 30:12:2009     Author:LYH
 函 数 名: FreeSttpLib()
 功能概要: 卸载STTP客户端动态库
 返 回 值: 
 参    数: param1 
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
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20162;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;

//		Sleep(1000);
		//报文体
		//1.装置ID
		strncpy(sttpData.sttp_body.ch_pt_id, pEvent->m_sSecID, 12);
		//2.时间
		CString strTime;
		strTime = pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		strTime.Format(strTime + ".%d", pEvent->m_nmsStart);
		strncpy(sttpData.sttp_body.ch_HandlerName, strTime, 26);
		//3.录波文件大小
		sttpData.sttp_body.nFlag = atoi(pEvent->m_sEventContent);
		//4.录波文件源名称
		CString strOldPath = pEvent->m_sOldDir;
		strOldPath.Replace("\\", "/");
		sttpData.sttp_body.strFilenameWithPath = strOldPath;
		CString strPath = pEvent->m_sDir;
		strPath.Replace("\\", "/");
		sttpData.sttp_body.strAbsolutePath = strPath;
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20162RenameOscFile, 错误", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL  CSttpEngine::BuildDataFor20138StationConfigChange(STTP_FULL_DATA& sttpData, CString strStationName)
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20138;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		//sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strStationName, 12);
	
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138StationConfigChange, 错误", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL CSttpEngine::BuildDataFor20003CommStatus(STTP_FULL_DATA& sttpData, CString strPTID)
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20003;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 12;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strPTID, 12);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20003CommStatus, 错误", 3);
		return FALSE;
	}
	return TRUE;
}


BOOL CSttpEngine::BuildDataFor20085CommStatus(STTP_FULL_DATA& sttpData, CString strOscID)
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20085;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 12;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
//		Sleep(1000);
		strncpy(sttpData.sttp_body.ch_pt_id, strOscID, 12);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20085CommStatus, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2012/6/15  Author:LYH
函数名:   BuildDataFor20138StationAdd	
返回值:   BOOL	
功能概要: 
参数: STTP_FULL_DATA & sttpData	
参数: CString strStationName	
*****************************************************/
BOOL CSttpEngine::BuildDataFor20138StationAdd( STTP_FULL_DATA& sttpData, CString strStationName, CString strSepName )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20138;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		//sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
		strncpy(sttpData.sttp_body.ch_pt_id, strSepName, 12);
		STTP_DBDATA_RECORD record;
		record.nReserved = 11;//新增一个子站
		STTP_DBDATA_UNIT unit;
		unit.strValue = strStationName;
		record.field_list.push_back(unit);
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(record);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138StationAdd, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2012/6/15  Author:LYH
函数名:   BuildDataFor20138SecConfigChange	
返回值:   BOOL	
功能概要: 
参数: STTP_FULL_DATA & sttpData	
参数: CString strStationName	
参数: CString strDevID	
参数: BOOL bAdd	
*****************************************************/
BOOL CSttpEngine::BuildDataFor20138SecConfigChange( STTP_FULL_DATA& sttpData, CString strStationName, CString strDevID, BOOL bAdd /*= FALSE*/ )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20138;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		//sttpData.sttp_head.uMsgLength = 16;	//报文长度
		sttpData.sttp_head.uMsgLengthType = FIX; //长度类别
		sttpData.sttp_head.uMsgRii = GetTickCount()/1000%256;
		
		strncpy(sttpData.sttp_body.ch_pt_id, strStationName, 12);
		STTP_DBDATA_RECORD record;
		if(bAdd)
			record.nReserved = 12;//新增一个装置
		else
			record.nReserved = 2;
		STTP_DBDATA_UNIT unit;
		unit.strValue = strDevID;
		record.field_list.push_back(unit);
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(record);
	}
	catch (...)
	{
		WriteLog("CSttpEngine::BuildDataFor20138SecConfigChange, 错误", 3);
		return FALSE;
	}
	return TRUE;
}

BOOL CSttpEngine::BuildDataFor20178StationInit( STTP_FULL_DATA& sttpData, CString sStationID, int nInitFlag, int nStatus )
{
	try
	{
		//清空结构
		zero_sttp_full_data(sttpData);
		
		//报文头
		sttpData.sttp_head.uMsgID = 20178;	//报文ID
		sttpData.sttp_head.uMsgType = INQ;	//报文类别
		sttpData.sttp_head.uMsgLength = 279;	//报文长度
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
		WriteLog("CSttpEngine::BuildDataFor20177StationInit, 错误", 3);
		return FALSE;
	}
	return TRUE;
}
