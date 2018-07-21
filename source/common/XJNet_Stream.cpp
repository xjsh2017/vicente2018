/********************************************************************
	created:	2008/10/06
	created:	6:10:2008   10:55
	file base:	XJNet_Stream
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "XJNet_Stream.h"

/*************************************************************
 函 数 名:   CXJNet_Stream
 功能概要:   构造函数
 返 回 值:   
**************************************************************/
CXJNet_Stream::CXJNet_Stream()
:m_iReadTimeOut(5000),m_iWriteTimeOut(5000)
{
    //ctor
}

CXJNet_Stream::CXJNet_Stream( const char* pLogPath , int pLogLevel /*= CLogFile::error*/)
:m_iReadTimeOut(5000),m_iWriteTimeOut(5000)
{
	m_Net.SetLogLevel(pLogLevel);
	m_Net.SetLogPath((char*)pLogPath);
}
/*************************************************************
 函 数 名:   ~CXJNet_Stream
 功能概要:   析构函数
 返 回 值:   
**************************************************************/
CXJNet_Stream::~CXJNet_Stream()
{
	m_Net.close();
}

/*************************************************************
 函 数 名:   GetNet
 功能概要:   获取CNet资源
 返 回 值:   CNet& 数据模式使用的CNet对象的引用
**************************************************************/
CNet& CXJNet_Stream::GetNet()
{
	return m_Net;	
}

/*************************************************************
 函 数 名:   GetLocalAddr
 功能概要:   获取本地地址信息
 返 回 值:   int 0-成功 -1-失败
 参    数:   NETADDR * pAddr   地址对象指针
**************************************************************/
int CXJNet_Stream::GetLocalAddr( NETADDR* pAddr )
{
	if (pAddr == NULL)
	{
		return -1;
	} 
	else
	{
		m_Net.GetLocalAddr(pAddr);
		return 0;
	}
		
}

/*************************************************************
 函 数 名:   GetRemoteAddr
 功能概要:   获取对端地址信息
 返 回 值:   int 0-成功 -1-失败
 参    数:   NETADDR * pAddr 地址对象指针
**************************************************************/
int CXJNet_Stream::GetRemoteAddr( NETADDR* pAddr )
{
	if (pAddr == NULL)
	{
		return -1;
	} 
	else
	{
		m_Net.GetOpponentAddr(pAddr);
		return 0;
	}	
}

/*************************************************************
 函 数 名:   SetOptions
 功能概要:   设置CNet各选项
 返 回 值:   bool true-成功 false-失败
 参    数:   int nFlag 要设置的选项名称
 参    数:   int nTime 选项值,设置超时时为超时时间(ms)
 		                     其它选项时清0
 参    数:   int Reserve 保留
**************************************************************/
bool CXJNet_Stream::SetOptions( int nFlag,int nTime,int Reserve/*=0*/ )
{
	bool bResult = false;

	switch (nFlag)
	{
	case RECVTIME:
		if (bResult = m_Net.SetOptions(nFlag,nTime,Reserve))
			m_iReadTimeOut = nTime;
		break;
	case SENDTIME:
		if (bResult = m_Net.SetOptions(nFlag,nTime,Reserve))
			m_iWriteTimeOut = nTime;	
		break;
	default:
		break;
	}

	return bResult;
}

/*************************************************************
 函 数 名:   Write
 功能概要:   发送指定缓冲区中指定字节的数据
 返 回 值:   int 返回实际发出的字节数 (<=nLen)
 参    数:   void * pBuf  指定的发送缓冲区指针
 参    数:   int nLen     指定发送字节数
 参    数:   int nTimeout 超时时间
**************************************************************/
int CXJNet_Stream::Write( void * pBuf,int nLen,int nTimeout/*=-1*/ )
{
	if (!SharedWriteStart(nTimeout))
	{
		return SOCKET_ERROR;
	}
	int nResult = SendToPeer(pBuf,nLen);
	SharedWriteFinished(nTimeout);
	return nResult;
}

/*************************************************************
 函 数 名:   Read
 功能概要:   读取指定字节的数据到指定缓冲区中
 返 回 值:   int 返回实际读到的字节数 (<=nLen)
 参    数:   void * pBuf   存放接收数据的缓冲区
 参    数:   int nLen      指定读取字节数
 参    数:   int nTimeout  超时时间
**************************************************************/
int CXJNet_Stream::Read( void * pBuf,int nLen,int nTimeout/*=-1*/ )
{
	if (!SharedReadStart(nTimeout))
	{
		return SOCKET_ERROR;
	}
	int nResult = RecvFromPeer(pBuf,nLen);
	SharedReadFinished(nTimeout);
	return nResult;	
}

/*************************************************************
 函 数 名:   Open
 功能概要:   打开流对象
 返 回 值:   bool true-成功 false-失败
**************************************************************/
bool CXJNet_Stream::Open()
{
	return m_Net.open();
}
/*************************************************************
 函 数 名:   Close
 功能概要:   关闭流对象
 返 回 值:   bool true-成功 false-失败
**************************************************************/
bool CXJNet_Stream::Close()
{
	return m_Net.close();	
}

/*************************************************************
 函 数 名:   SetLogPath
 功能概要:   设置日志路径
 返 回 值:   void
 参    数:   const char * pszPath 日志路径
**************************************************************/
void CXJNet_Stream::SetLogPath( const char * pszPath )
{
	m_Net.SetLogPath((char*)pszPath);	
}

/*************************************************************
 函 数 名:   SharedReadStart
 功能概要:   读操作前置操作
 返 回 值:   bool true-成功 false-失败
 参    数:   int nTimeout 设定的超时时间
**************************************************************/
bool CXJNet_Stream::SharedReadStart( int nTimeout )
{
	if ((nTimeout >= 0)) 
	{
		return SetOptions(RECVTIME,nTimeout,0);
	} 
	else
	{
		return true;
	}
}

/*************************************************************
 函 数 名:   SharedReadFinished
 功能概要:   读操作后置操作
 返 回 值:   bool true-成功 false-失败
 参    数:   int nTimeout 设定的超时时间
**************************************************************/
bool CXJNet_Stream::SharedReadFinished( int nTimeout )
{
	if ((nTimeout >= 0)) 
	{
		return SetOptions(RECVTIME,m_iReadTimeOut,0);
	} 
	else
	{
		return true;
	}
}

/*************************************************************
 函 数 名:   SharedWriteStart
 功能概要:   写操作前置操作
 返 回 值:   bool true-成功 false-失败
 参    数:   int nTimeout 设定的超时时间
**************************************************************/
bool CXJNet_Stream::SharedWriteStart( int nTimeout )
{
	if ((nTimeout >= 0)) 
	{
		return SetOptions(SENDTIME,nTimeout,0);
	} 
	else
	{
		return true;
	}
}

/*************************************************************
 函 数 名:   SharedWriteFinished
 功能概要:   写操作后置操作
 返 回 值:   bool true-成功 false-失败
 参    数:   int nTimeout 设定的超时时间
**************************************************************/
bool CXJNet_Stream::SharedWriteFinished( int nTimeout )
{
	if ((nTimeout >= 0)) 
	{
		return SetOptions(SENDTIME,m_iWriteTimeOut,0);
	} 
	else
	{
		return true;
	}	
}

int CXJNet_Stream::SendToPeer( void * pBuf,int nLen )
{
    int iRet(0);
    int iSendLen(0);
    int iLeftLen(nLen);
    char* pChar = (char*)pBuf;
    while(iLeftLen > 0){
        iRet =m_Net.write(pChar,iLeftLen);
        if (iRet <= 0)
        {
            if (iSendLen > 0)
            {
                break;
            } 
            else
            {
                return iRet;
            }
        }
        iLeftLen -= iRet;
        iSendLen += iRet;
        pChar += iRet;
    }
    return iSendLen;  
}

int CXJNet_Stream::RecvFromPeer( void * pBuf,int nLen )
{
    int iRet(0);
    int iRecvLen(0);
    int iLeftLen(nLen);
    char* pChar = (char*)pBuf;
    while(iLeftLen > 0){
        iRet =m_Net.read(pChar,iLeftLen);
        if (iRet <= 0)
        {
            if (iRecvLen > 0)
            {
                break;
            } 
            else
            {
                return iRet;
            }
        }
        iLeftLen -= iRet;
        iRecvLen += iRet;
        pChar += iRet;
    }
    return iRecvLen;
}