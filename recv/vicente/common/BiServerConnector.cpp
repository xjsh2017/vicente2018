/********************************************************************
	created:	2008/09/09
	created:	9:9:2008   14:07
	file base:	BiServerConnector
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "BiServerConnector.h"



/*************************************************************
 函 数 名:   CBiServerConnector
 功能概要:	 构造函数
 返 回 值:   
 参    数:   const STRUCT_DESTINATION_SERVER & pDestServer 
			包含目标服务端信息的结构体
**************************************************************/
CBiServerConnector::CBiServerConnector( const STRUCT_DESTINATION_SERVER& pDestServer , const char* pLogPath/* = "Xj_Log/"*/)
:m_bHasReservedServer(pDestServer.bHasReservedServer),m_iOnLineServer(SERVER_ONLINE_UNK)
{
	m_pServerA = NULL;
	m_pServerB = NULL;
	m_pServerA = new CBiNetServerConnector(pDestServer.serverA,pLogPath);
	if (pDestServer.bHasReservedServer)
	{
		m_pServerB = new CBiNetServerConnector(pDestServer.serverB,pLogPath);
	}
}

/*************************************************************
 函 数 名:   ~CBiServerConnector
 功能概要:    析构函数
 返 回 值:   
**************************************************************/
CBiServerConnector::~CBiServerConnector()
{
	if (m_pServerA != NULL)
	{
		delete m_pServerA;
	}
	if (m_pServerB != NULL)
	{
		delete m_pServerB;
	}		
}

//##ModelId=489F08560271
/*************************************************************
 函 数 名:   connect
 功能概要:   连接目标，主服务器连接失败后，有备用服务器时自动连接备用服务器
 返 回 值:   bool true-成功 false-失败
 参    数:   CSTTPStream& pStream
			 用于连接的SttpStream对象引用
			 int nTimeout 连接超时
			 pError 返回连接错误码
		     Option 选项用于控制连接顺序
**************************************************************/
bool CBiServerConnector::connect( CXJNet_Stream& pStream , int nTimeout/*=-1*/,char* pError,int Option/*=0*/ )
{

	bool pbResult = false;
	
	//如果a服务器连接器对象存在，则连接a服务器，否则直接返回失败
	if (Option==0)
	{
		pbResult = (m_pServerA != NULL)?m_pServerA->connect(pStream,nTimeout,pError):false;
		if (pbResult)
		{
			m_iOnLineServer = SERVER_ONLINE_A;
		}

		//如果a服务器连接失败且备用服务器存在，则连接备用服务器
		if ((!pbResult) && (m_bHasReservedServer))
		{
			pbResult = (m_pServerB != NULL)?m_pServerB->connect(pStream,nTimeout,pError):false;
			if (pbResult)
			{
				m_iOnLineServer = SERVER_ONLINE_B;
			} 
		}
	}else{
		if ((m_bHasReservedServer))
		{
			pbResult = (m_pServerB != NULL)?m_pServerB->connect(pStream,nTimeout,pError):false;
			if (pbResult)
			{
				m_iOnLineServer = SERVER_ONLINE_B;
			} 
		}

		//如果B服务器连接失败且备用A服务器存在，则连接备用服务器
		if ((!pbResult) && (m_bHasReservedServer))
		{
			pbResult = (m_pServerA != NULL)?m_pServerA->connect(pStream,nTimeout,pError):false;
			if (pbResult)
			{
				m_iOnLineServer = SERVER_ONLINE_A;
			} 
		}
	}

	if (!pbResult)
	{
		m_iOnLineServer = SERVER_ONLINE_UNK;
	}
	return pbResult;
}

/*************************************************************
 函 数 名:	GetOnlineServer
 功能概要:	获取在线标志
 返 回 值:	int 在线服务器编号
*************************************************************/
int CBiServerConnector::GetOnlineServer()
{
	return m_iOnLineServer;
}

/*************************************************************
 函 数 名:	GetHasReservedServer
 功能概要:	获取是否为双服务器标志
 返 回 值:	是否为双服务器
*************************************************************/
bool CBiServerConnector::GetHasReservedServer()
{
	return m_bHasReservedServer;
}
