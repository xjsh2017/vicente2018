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
 �� �� ��:   CXJNet_Acceptor
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
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
 �� �� ��:   ~CXJNet_Acceptor
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   
**************************************************************/
CXJNet_Acceptor::~CXJNet_Acceptor()
{
   m_Net.close();
}

/*************************************************************
 �� �� ��:   Open
 ���ܸ�Ҫ:   ��ʼ������ģʽ����һ��ָ���ĵ�ַ�ϱ�������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   u_short port �˿ں�
 ��    ��:   const char * host ����ip
**************************************************************/
bool CXJNet_Acceptor::Open( u_short port,const char * host/*=NULL */, int nQueue /*= SOMAXCONN*/ )
{
	return m_Net.ListenServer(port,host,nQueue);	 
}

/*************************************************************
 �� �� ��:   Accept
 ���ܸ�Ҫ:	 ����һ���¿ͻ��˵����ӣ�����ʼ��ָ����CXJNet_Stream����
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   CXJNet_Stream & pNew_stream
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
 �� �� ��:   Close
 ���ܸ�Ҫ:   �ر��������ӵ�������Դ
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
**************************************************************/
bool CXJNet_Acceptor::Close()
{
	return m_Net.close();	
}

void CXJNet_Acceptor::SetLogPath( const char * pszPath )
{
	m_Net.SetLogPath((char*)pszPath);
}