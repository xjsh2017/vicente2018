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
 �� �� ��:   CXJNet_Connector
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
**************************************************************/
CXJNet_Connector::CXJNet_Connector()
{
    //ctor
}

/*************************************************************
 �� �� ��:   ~CXJNet_Connector
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   
**************************************************************/
CXJNet_Connector::~CXJNet_Connector()
{
    //dtor
}

/*************************************************************
 �� �� ��:   Connect
 ���ܸ�Ҫ:   ���ӷ����
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   CXJNet_Stream & pNew_stream ���ڽ������ӵ�������
 ��    ��:   const char * ip             �����IP
 ��    ��:   int port                    ����˶˿ں�
 ��    ��:   int nLocalPort              ���ض˿ں�
**************************************************************/
bool CXJNet_Connector::Connect( CXJNet_Stream& pNew_stream,const char * ip/*=NULL*/,int port/*=-1*/,int nLocalPort/*=-1*/,int nTimeout/*=-1*/ )
{
	if (!SharedConnectStart(pNew_stream,nTimeout))
		return false;
	
	bool bResult = pNew_stream.GetNet().ConnectServer(ip,port,nLocalPort,nTimeout);
	
	return ShardConnectFinish(pNew_stream,nTimeout,bResult);
}

/*************************************************************
 �� �� ��:   SharedConnectStart
 ���ܸ�Ҫ:   ����ǰ�ò���,��Stream���󣬲���Ϊ��������ʽ
 �� �� ֵ:   bool true-�ɹ� false-ʧ�� 
 ��    ��:   CXJNet_Stream & pNew_stream ���ڽ������ӵ�������
 ��    ��:   int nTimeout ��ʱʱ�䣬��λs
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
 �� �� ��:   ShardConnectFinish
 ���ܸ�Ҫ:   ���Ӻ��ò�������Ϊ��������ʽ
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   CXJNet_Stream & pNew_stream ���ڽ������ӵ�������
 ��    ��:   int nTimeout ��ʱʱ�䣬��λs
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