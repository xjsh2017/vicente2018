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
 �� �� ��:   CBiNetServerConnector
 ���ܸ�Ҫ:	 ���캯��
 �� �� ֵ:   
 ��    ��:   const STRUCT_SERVER_INFO & pServerInfo 
			 ��������˵�ַ��Ϣ�Ľṹ��
**************************************************************/
CBiNetServerConnector::CBiNetServerConnector(const STRUCT_SERVER_INFO& pServerInfo , const char* pLogPath /*= "Xj_Log/"*/ )
:m_bHasReservedNet(pServerInfo.bHasReservedAddr)
{
	bzero(&m_NetAddrA,sizeof(NETADDR));
	bzero(&m_NetAddrB,sizeof(NETADDR));

	m_NetAddrA.nPort = pServerInfo.netAddrA.nPort;
	strncpy(m_NetAddrA.IpAddr,pServerInfo.netAddrA.IpAddr,40);

	if (pServerInfo.bHasReservedAddr) //����б��õ�ַ�����ʼ�����õ�ַ
	{
		m_NetAddrB.nPort = pServerInfo.netAddrB.nPort;
		strncpy(m_NetAddrB.IpAddr,pServerInfo.netAddrB.IpAddr,40);
	}

	// ����Ĭ����־�����·��
// 	m_LogFile.SetLogLevel(CLogFile::trace);
// 	m_LogFile.SetLogPath(pLogPath);
// 	m_LogFile.Close();

}

/*************************************************************
 �� �� ��:   ~CBiNetServerConnector
 ���ܸ�Ҫ:	 ��������
 �� �� ֵ:   
**************************************************************/
CBiNetServerConnector::~CBiNetServerConnector()
{
	//m_LogFile.Close();	
}

//##ModelId=489F0C110290
/*************************************************************
 �� �� ��:   connect
 ���ܸ�Ҫ:	 ���ӷ���ˣ�����ַ����ʧ�ܺ��б��õ�ַʱ�Զ����ӱ��õ�ַ
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   CXJNet_Stream& pStream �������ӵ�CXJNet_Stream��Դ
**************************************************************/
bool CBiNetServerConnector::connect( CXJNet_Stream& pStream , int nTimeout/*=-1*/, char* pError )
{
	bool tResult = false;

	/*----------------code by rwp 20111114----------------*/ 
	//m_LogFile.Open("STTPConnector.txt");
	char chError[255]="";	
	/*----------------code by rwp 20111114----------------*/
	//�������������ַ
	if (!m_Connector.Connect(pStream,m_NetAddrA.IpAddr, m_NetAddrA.nPort,-1,nTimeout)) {
// 		m_LogFile.FormatAdd(CLogFile::error, "connect destination(%s:%d) failed",
// 			m_NetAddrA.IpAddr, m_NetAddrA.nPort);
		char chError[255]="";
		sprintf(chError,"connect destination(%s:%d) failed",
				 			m_NetAddrA.IpAddr, m_NetAddrA.nPort);
		if (pError)	{
			strncpy(pError,chError,254);
		}

		//����ַ����ʧ��ʱ������б��õ�ַ�����������
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

