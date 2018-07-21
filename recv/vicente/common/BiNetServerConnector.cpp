/********************************************************************
	created:	2008/09/09
	created:	9:9:2008   14:05
	file base:	CBiNetServerConnector
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "BiNetServerConnector.h"

/*************************************************************
 函 数 名:   CBiNetServerConnector
 功能概要:	 构造函数
 返 回 值:   
 参    数:   const STRUCT_SERVER_INFO & pServerInfo 
			 包含服务端地址信息的结构体
**************************************************************/
CBiNetServerConnector::CBiNetServerConnector(const STRUCT_SERVER_INFO& pServerInfo , const char* pLogPath /*= "Xj_Log/"*/ )
:m_bHasReservedNet(pServerInfo.bHasReservedAddr)
{
	bzero(&m_NetAddrA,sizeof(NETADDR));
	bzero(&m_NetAddrB,sizeof(NETADDR));

	m_NetAddrA.nPort = pServerInfo.netAddrA.nPort;
	strncpy(m_NetAddrA.IpAddr,pServerInfo.netAddrA.IpAddr,40);

	if (pServerInfo.bHasReservedAddr) //如果有备用地址，则初始化备用地址
	{
		m_NetAddrB.nPort = pServerInfo.netAddrB.nPort;
		strncpy(m_NetAddrB.IpAddr,pServerInfo.netAddrB.IpAddr,40);
	}

	// 设置默认日志级别和路径
// 	m_LogFile.SetLogLevel(CLogFile::trace);
// 	m_LogFile.SetLogPath(pLogPath);
// 	m_LogFile.Close();

}

/*************************************************************
 函 数 名:   ~CBiNetServerConnector
 功能概要:	 析构函数
 返 回 值:   
**************************************************************/
CBiNetServerConnector::~CBiNetServerConnector()
{
	//m_LogFile.Close();	
}

//##ModelId=489F0C110290
/*************************************************************
 函 数 名:   connect
 功能概要:	 连接服务端，主地址连接失败后，有备用地址时自动连接备用地址
 返 回 值:   bool true-成功 false-失败
 参    数:   CXJNet_Stream& pStream 用于连接的CXJNet_Stream资源
**************************************************************/
bool CBiNetServerConnector::connect( CXJNet_Stream& pStream , int nTimeout/*=-1*/, char* pError )
{
	bool tResult = false;

	/*----------------code by rwp 20111114----------------*/ 
	//m_LogFile.Open("STTPConnector.txt");
	char chError[255]="";	
	/*----------------code by rwp 20111114----------------*/
	//先连接主网络地址
	if (!m_Connector.Connect(pStream,m_NetAddrA.IpAddr, m_NetAddrA.nPort,-1,nTimeout)) {
// 		m_LogFile.FormatAdd(CLogFile::error, "connect destination(%s:%d) failed",
// 			m_NetAddrA.IpAddr, m_NetAddrA.nPort);
		char chError[255]="";
		sprintf(chError,"connect destination(%s:%d) failed",
				 			m_NetAddrA.IpAddr, m_NetAddrA.nPort);
		if (pError)	{
			strncpy(pError,chError,254);
		}

		//主地址连接失败时，如果有备用地址，则继续连接
		if (m_bHasReservedNet)
		{
			if (!m_Connector.Connect(pStream,m_NetAddrB.IpAddr, m_NetAddrB.nPort,-1,nTimeout)) {		
// 				m_LogFile.FormatAdd(CLogFile::error, "connect destination(%s:%d) failed",
// 					m_NetAddrB.IpAddr, m_NetAddrB.nPort);
				if (pError)	{
					bzero(chError,sizeof(chError));
					sprintf(chError,"connect destination(%s:%d) failed",
				 					m_NetAddrB.IpAddr, m_NetAddrB.nPort);
					strncpy(pError,chError,254);
				}

				tResult = false;
			}
			else{			
// 				m_LogFile.FormatAdd(CLogFile::trace, "connect destination(%s:%d) success",
// 					m_NetAddrB.IpAddr, m_NetAddrB.nPort);	
				if (pError)	{
					bzero(chError,sizeof(chError));
					sprintf(chError,"connect destination(%s:%d) success",
  					m_NetAddrB.IpAddr, m_NetAddrB.nPort);
					strncpy(pError,chError,254);
				}
				tResult = true;
			}
		}
		else{
			tResult = false;
		}
	}
	else{
// 		m_LogFile.FormatAdd(CLogFile::trace, "connect destination(%s:%d) success",
// 				m_NetAddrA.IpAddr, m_NetAddrA.nPort);
		if (pError)	{
			bzero(chError,sizeof(chError));
			sprintf(chError,"connect destination(%s:%d) success",
				m_NetAddrA.IpAddr, m_NetAddrA.nPort);
			strncpy(pError,chError,254);
		}
		tResult = true;
	}

	//m_LogFile.Close();
	return tResult;
}

