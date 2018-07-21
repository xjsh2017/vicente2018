/********************************************************************
	created:	2008/10/06
	created:	6:10:2008   9:44
	file base:	XJNet_Acceptor
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "XJNet_Acceptor.h"

/*************************************************************
 函 数 名:   CXJNet_Acceptor
 功能概要:   构造函数
 返 回 值:   
**************************************************************/
CXJNet_Acceptor::CXJNet_Acceptor()
{
    //ctor
}

CXJNet_Acceptor::CXJNet_Acceptor( const char* pLogPath , int pLogLevel /*= CLogFile::error*/ )
{
	m_Net.SetLogLevel(pLogLevel);
	m_Net.SetLogPath((char*)pLogPath);
}
/*************************************************************
 函 数 名:   ~CXJNet_Acceptor
 功能概要:   析构函数
 返 回 值:   
**************************************************************/
CXJNet_Acceptor::~CXJNet_Acceptor()
{
   m_Net.close();
}

/*************************************************************
 函 数 名:   Open
 功能概要:   初始化被动模式，在一个指定的地址上被动侦听
 返 回 值:   bool true-成功 false-失败
 参    数:   u_short port 端口号
 参    数:   const char * host 本地ip
**************************************************************/
bool CXJNet_Acceptor::Open( u_short port,const char * host/*=NULL */, int nQueue /*= SOMAXCONN*/ )
{
	return m_Net.ListenServer(port,host,nQueue);	 
}

/*************************************************************
 函 数 名:   Accept
 功能概要:	 接受一个新客户端的连接，并初始化指定的CXJNet_Stream对象
 返 回 值:   bool true-成功 false-失败
 参    数:   CXJNet_Stream & pNew_stream
**************************************************************/
bool CXJNet_Acceptor::Accept( CXJNet_Stream& pNew_stream )
{
	bool bResult(false);
	if (m_Net.Accept(pNew_stream.GetNet()))
	{
		pNew_stream.Open();
		bResult = true;
	}
	return bResult; 	
}

/*************************************************************
 函 数 名:   Close
 功能概要:   关闭用于连接的网络资源
 返 回 值:   bool true-成功 false-失败
**************************************************************/
bool CXJNet_Acceptor::Close()
{
	return m_Net.close();	
}

void CXJNet_Acceptor::SetLogPath( const char * pszPath )
{
	m_Net.SetLogPath((char*)pszPath);
}