/********************************************************************
	created:	2008/10/08
	created:	8:10:2008   0:34
	file base:	XJNet_Connector
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "XJNet_Connector.h"

/*************************************************************
 函 数 名:   CXJNet_Connector
 功能概要:   构造函数
 返 回 值:   
**************************************************************/
CXJNet_Connector::CXJNet_Connector()
{
    //ctor
}

/*************************************************************
 函 数 名:   ~CXJNet_Connector
 功能概要:   析构函数
 返 回 值:   
**************************************************************/
CXJNet_Connector::~CXJNet_Connector()
{
    //dtor
}

/*************************************************************
 函 数 名:   Connect
 功能概要:   连接服务端
 返 回 值:   bool true-成功 false-失败
 参    数:   CXJNet_Stream & pNew_stream 用于建立连接的流对象
 参    数:   const char * ip             服务端IP
 参    数:   int port                    服务端端口号
 参    数:   int nLocalPort              本地端口号
**************************************************************/
bool CXJNet_Connector::Connect( CXJNet_Stream& pNew_stream,const char * ip/*=NULL*/,int port/*=-1*/,int nLocalPort/*=-1*/,int nTimeout/*=-1*/ )
{
	if (!SharedConnectStart(pNew_stream,nTimeout))
		return false;
	
	bool bResult = pNew_stream.GetNet().ConnectServer(ip,port,nLocalPort,nTimeout);
	
	return ShardConnectFinish(pNew_stream,nTimeout,bResult);
}

/*************************************************************
 函 数 名:   SharedConnectStart
 功能概要:   连接前置操作,打开Stream对象，并设为非阻塞方式
 返 回 值:   bool true-成功 false-失败 
 参    数:   CXJNet_Stream & pNew_stream 用于建立连接的流对象
 参    数:   int nTimeout 超时时间，单位s
**************************************************************/
bool CXJNet_Connector::SharedConnectStart( CXJNet_Stream& pNew_stream ,int nTimeout )
{
	if (!pNew_stream.Open())
		return false;

	if ((nTimeout > 0) && (!pNew_stream.GetNet().SetOptions(UNBLOCKING,0,0)))
	{
		pNew_stream.Close();
		return false;
	} 
	else
	{
		return true;
	}
}

/*************************************************************
 函 数 名:   ShardConnectFinish
 功能概要:   连接后置操作，设为非阻塞方式
 返 回 值:   bool true-成功 false-失败
 参    数:   CXJNet_Stream & pNew_stream 用于建立连接的流对象
 参    数:   int nTimeout 超时时间，单位s
**************************************************************/
bool CXJNet_Connector::ShardConnectFinish( CXJNet_Stream& pNew_stream ,int nTimeout, bool bResult )
{
	if ((bResult) && (nTimeout > 0))
	{
		bResult = pNew_stream.GetNet().SetOptions(BLOCKING,0,0);
	}

	if (!bResult)
	{
		pNew_stream.Close();
	} 

	return bResult;

}