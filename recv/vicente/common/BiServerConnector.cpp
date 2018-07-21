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
 �� �� ��:   CBiServerConnector
 ���ܸ�Ҫ:	 ���캯��
 �� �� ֵ:   
 ��    ��:   const STRUCT_DESTINATION_SERVER & pDestServer 
			����Ŀ��������Ϣ�Ľṹ��
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
 �� �� ��:   ~CBiServerConnector
 ���ܸ�Ҫ:    ��������
 �� �� ֵ:   
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
 �� �� ��:   connect
 ���ܸ�Ҫ:   ����Ŀ�꣬������������ʧ�ܺ��б��÷�����ʱ�Զ����ӱ��÷�����
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   CSTTPStream& pStream
			 �������ӵ�SttpStream��������
			 int nTimeout ���ӳ�ʱ
			 pError �������Ӵ�����
		     Option ѡ�����ڿ�������˳��
**************************************************************/
bool CBiServerConnector::connect( CXJNet_Stream& pStream , int nTimeout/*=-1*/,char* pError,int Option/*=0*/ )
{

	bool pbResult = false;
	
	//���a������������������ڣ�������a������������ֱ�ӷ���ʧ��
	if (Option==0)
	{
		pbResult = (m_pServerA != NULL)?m_pServerA->connect(pStream,nTimeout,pError):false;
		if (pbResult)
		{
			m_iOnLineServer = SERVER_ONLINE_A;
		}

		//���a����������ʧ���ұ��÷��������ڣ������ӱ��÷�����
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

		//���B����������ʧ���ұ���A���������ڣ������ӱ��÷�����
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
 �� �� ��:	GetOnlineServer
 ���ܸ�Ҫ:	��ȡ���߱�־
 �� �� ֵ:	int ���߷��������
*************************************************************/
int CBiServerConnector::GetOnlineServer()
{
	return m_iOnLineServer;
}

/*************************************************************
 �� �� ��:	GetHasReservedServer
 ���ܸ�Ҫ:	��ȡ�Ƿ�Ϊ˫��������־
 �� �� ֵ:	�Ƿ�Ϊ˫������
*************************************************************/
bool CBiServerConnector::GetHasReservedServer()
{
	return m_bHasReservedServer;
}
