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
 �� �� ��:   CXJNet_Stream
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
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
 �� �� ��:   ~CXJNet_Stream
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   
**************************************************************/
CXJNet_Stream::~CXJNet_Stream()
{
	m_Net.close();
}

/*************************************************************
 �� �� ��:   GetNet
 ���ܸ�Ҫ:   ��ȡCNet��Դ
 �� �� ֵ:   CNet& ����ģʽʹ�õ�CNet���������
**************************************************************/
CNet& CXJNet_Stream::GetNet()
{
	return m_Net;	
}

/*************************************************************
 �� �� ��:   GetLocalAddr
 ���ܸ�Ҫ:   ��ȡ���ص�ַ��Ϣ
 �� �� ֵ:   int 0-�ɹ� -1-ʧ��
 ��    ��:   NETADDR * pAddr   ��ַ����ָ��
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
 �� �� ��:   GetRemoteAddr
 ���ܸ�Ҫ:   ��ȡ�Զ˵�ַ��Ϣ
 �� �� ֵ:   int 0-�ɹ� -1-ʧ��
 ��    ��:   NETADDR * pAddr ��ַ����ָ��
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
 �� �� ��:   SetOptions
 ���ܸ�Ҫ:   ����CNet��ѡ��
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int nFlag Ҫ���õ�ѡ������
 ��    ��:   int nTime ѡ��ֵ,���ó�ʱʱΪ��ʱʱ��(ms)
 		                     ����ѡ��ʱ��0
 ��    ��:   int Reserve ����
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
 �� �� ��:   Write
 ���ܸ�Ҫ:   ����ָ����������ָ���ֽڵ�����
 �� �� ֵ:   int ����ʵ�ʷ������ֽ��� (<=nLen)
 ��    ��:   void * pBuf  ָ���ķ��ͻ�����ָ��
 ��    ��:   int nLen     ָ�������ֽ���
 ��    ��:   int nTimeout ��ʱʱ��
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
 �� �� ��:   Read
 ���ܸ�Ҫ:   ��ȡָ���ֽڵ����ݵ�ָ����������
 �� �� ֵ:   int ����ʵ�ʶ������ֽ��� (<=nLen)
 ��    ��:   void * pBuf   ��Ž������ݵĻ�����
 ��    ��:   int nLen      ָ����ȡ�ֽ���
 ��    ��:   int nTimeout  ��ʱʱ��
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
 �� �� ��:   Open
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
**************************************************************/
bool CXJNet_Stream::Open()
{
	return m_Net.open();
}
/*************************************************************
 �� �� ��:   Close
 ���ܸ�Ҫ:   �ر�������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
**************************************************************/
bool CXJNet_Stream::Close()
{
	return m_Net.close();	
}

/*************************************************************
 �� �� ��:   SetLogPath
 ���ܸ�Ҫ:   ������־·��
 �� �� ֵ:   void
 ��    ��:   const char * pszPath ��־·��
**************************************************************/
void CXJNet_Stream::SetLogPath( const char * pszPath )
{
	m_Net.SetLogPath((char*)pszPath);	
}

/*************************************************************
 �� �� ��:   SharedReadStart
 ���ܸ�Ҫ:   ������ǰ�ò���
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int nTimeout �趨�ĳ�ʱʱ��
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
 �� �� ��:   SharedReadFinished
 ���ܸ�Ҫ:   ���������ò���
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int nTimeout �趨�ĳ�ʱʱ��
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
 �� �� ��:   SharedWriteStart
 ���ܸ�Ҫ:   д����ǰ�ò���
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int nTimeout �趨�ĳ�ʱʱ��
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
 �� �� ��:   SharedWriteFinished
 ���ܸ�Ҫ:   д�������ò���
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int nTimeout �趨�ĳ�ʱʱ��
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