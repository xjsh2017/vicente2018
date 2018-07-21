// Net.cpp: implementation of the CNet class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "Net.h"

#ifdef OS_LINUX
  #ifndef OS_SUNUNIX
	#include <net/if.h>
  #endif
#endif


//////////////////////////////////////////////////////////////////////////
//                  Class  CCommon   ʵ��                                    
//////////////////////////////////////////////////////////////////////////

CCommon::CCommon(int nType):m_nType(nType)
{
	//��ս��շ��ͻ�����
	memset(&m_SendBuff,0,sizeof(XJBUFFER));

	memset(&m_RecvBuff,0,sizeof(XJBUFFER));

	//��ʼ��������
	char ch[20]="";
	sprintf(ch,"CCommon(type=%d)",m_nType);
	m_Lock.init(ch);

	// ����Ĭ����־�����·��
	m_LogFile.SetLogLevel(CLogFile::error);

	/*
	* ----- commented by qingch   2/24/2009 ------
	*
	* 	m_LogFile.SetLogPath("Xj_Log/");
	*	m_LogFile.Open("CNet.txt"); 
	*/
}

CCommon::~CCommon()
{
	//�ر��ļ�
	m_LogFile.Close();
	//�ͷŻ�����
    m_Lock.del();
}

/*************************************************************
 �� �� ����GetSendBuffer()
 ���ܸ�Ҫ��ȡ�÷��ͻ�����ָ��
 �� �� ֵ��void * 
 ��    ������
***************************************************************/
void *CCommon::GetSendBuffer()
{
	return (void*)&m_SendBuff;
}

/*************************************************************
 �� �� ����GetRecvBuffer()
 ���ܸ�Ҫ��ȡ�ý��ջ�����ָ��
 �� �� ֵ��void * 
 ��    ������
***************************************************************/
void *CCommon::GetRecvBuffer()
{
	return (void*)&m_RecvBuff;
}

/*************************************************************
 �� �� ����SetLogLevel()
 ���ܸ�Ҫ��������־�������
 �� �� ֵ��void 
 ��    ����
           int nLevel :��־����
***************************************************************/
void CCommon::SetLogLevel(int nLevel)
{
	m_LogFile.SetLogLevel(nLevel);
}

/*************************************************************
 �� �� ����GetLogLevel()
 ���ܸ�Ҫ�������־�����־
 �� �� ֵ��int
 ��    ������  
***************************************************************/
int CCommon::GetLogLevel()
{
	return m_LogFile.GetLogLevel();
}

/*************************************************************
 �� �� ����SetLogPath()
 ���ܸ�Ҫ��������־���·��
 �� �� ֵ��void 
 ��    ����
           pPath :��־·��
***************************************************************/
void CCommon::SetLogPath(char * pPath)
{
	m_LogFile.SetLogPath(pPath);

	/* ------ Added by qingch   2/24/2009 ----- */
	//m_LogFile.Open("CNet.txt");//����дCNet��־              
	/* ---------------------------------------- */
}

/*************************************************************
 �� �� ����InitLogFile()
 ���ܸ�Ҫ������־�ļ�
 �� �� ֵ��void 
 ��    ����pFileName :�ļ���(����·��)
***************************************************************/
BOOL CCommon::InitLogFile(char * pFileName)
{
	m_LogFile.Close();

	return ( m_LogFile.Open(pFileName) ); 
}

//////////////////////////////////////////////////////////////////////////
//                         Class   CNet ʵ��
//////////////////////////////////////////////////////////////////////////

/*************************************************************
 �� �� ����CNet()
 ���ܸ�Ҫ�����캯��(���ʼ�����๹�캯��)
 �� �� ֵ����
 ��    ����domain          ����         
		   type            socket���� TCP/FTP
           protocol        Э������ һ��ȡ0
***************************************************************/
//##ModelId=45190CD801CC
CNet::CNet(int domain, int type, int protocol):CCommon(ID_NET)
,m_nRecvTimes(60000),m_nSendTimes(60000)
{
	//��ʼ�����
	m_hSocket	= INVALID_SOCKET;

	//��ʼ����
	m_nDomain   = domain;

	//��ʼ��socket����
	m_Type     = type;

	//Э������
	m_nProtocol = protocol;
	
	//״̬Ϊ�ر�
	m_eState       = CLOSED;

	//��ɫδ֪
	m_nLocalRole = LOCAL_ACT_AS_UNKNOW;

	//�Զ˵�ַ
	memset(&m_OpponentAddr,0,sizeof(NETADDR));
}

/*************************************************************
 �� �� ����CNet()
 ���ܸ�Ҫ�����캯��(���ʼ�����๹�캯��)
 �� �� ֵ����
 ��    ����add             ָ��Զ�NET��ַ�ĳ�ָ��          
		   domain          ����         
		   type            socket���� TCP/FTP
           protocol        Э������ һ��ȡ0
***************************************************************/
//##ModelId=45190CD801D6
CNet::CNet(const NETADDR* addr,int domain, 
		   int type, int protocol ):CCommon(ID_NET)
{
	//��ʼ�����
	m_hSocket	= INVALID_SOCKET;

	//��ʼ����
	m_nDomain   = domain;

	//��ʼ��socket����
	m_Type     = type;

	//Э������
	m_nProtocol = protocol;
	
	//״̬Ϊ�ر�
	m_eState       = CLOSED;

	//��ɫδ֪
	m_nLocalRole = LOCAL_ACT_AS_UNKNOW;

	//�Զ˵�ַ
	memcpy(&m_OpponentAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 �� �� ����~CNet()
 ���ܸ�Ҫ����������,�ͷŶ���ռ�
 �� �� ֵ����
 ��    ������
***************************************************************/
//##ModelId=45190CD801DB
CNet::~CNet()
{
	close();
}

/*************************************************************
 �� �� ����open()
 ���ܸ�Ҫ������socket����ʼ��״̬
 �� �� ֵ: BOOL  TRUE:�����ɹ� FALSE:����ʧ��
 ��    ������
***************************************************************/
//##ModelId=45190CD801DD
bool CNet::open()
{
	if((m_eState == INITIALIZED) ||
       (m_eState == BINDED)      ||
	   (m_eState == LISTENING)   || 
	   (m_eState == CONNECTED) )

		return true;
	
	//�����׽���
	m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  

	if(m_hSocket == INVALID_SOCKET)
	{
		m_LogFile.FormatAdd(CLogFile::error,"::socket() error,reason:%s(%d)",\
			                strerror(errno),errno);
		
		return false;
	}
	
	//�����ɹ��ó�ʼ״̬ΪINITIALIZED,������ΪCLOSED
	m_eState = ( m_hSocket != INVALID_SOCKET ) ? INITIALIZED : CLOSED;  

	if ( m_eState == INITIALIZED )
		return true;
	else
		return false;
}

/*************************************************************
 �� �� ����open()
 ���ܸ�Ҫ������socket����ʼ��״̬
 �� �� ֵ: BOOL  TRUE:�����ɹ� FALSE:����ʧ��
 ��    ����
		   flag	ͨѶ��־λ(�ɼ���ר��)
***************************************************************/
//##ModelId=45190CD801DD
bool CNet::open(int flag)
{
	if((m_eState == INITIALIZED) ||
       (m_eState == BINDED)      ||
	   (m_eState == LISTENING)   || 
	   (m_eState == CONNECTED) )

		return true;

#ifdef OS_WINDOWS
	if (flag==3 || flag==5)
	{
		//UDP�鲥
		//�����׽��֣�ͬʱ����һЩ����
		m_hSocket = WSASocket(m_nDomain, m_Type, m_nProtocol, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);
	}
	else
	{
		//�����׽���
		m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  
	}
#endif

#ifdef OS_LINUX
	//�����׽���
	m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  
#endif

	if(m_hSocket == INVALID_SOCKET)
	{
		m_LogFile.FormatAdd(CLogFile::error,"::socket() error,reason:%s(%d)",\
			                strerror(errno),errno);
		
		return false;
	}
	
	//�����ɹ��ó�ʼ״̬ΪINITIALIZED,������ΪCLOSED
	m_eState = ( m_hSocket != INVALID_SOCKET ) ? INITIALIZED : CLOSED;  

	if ( m_eState == INITIALIZED )
		return true;
	else
		return false;
}

/*************************************************************
 �� �� ����close()
 ���ܸ�Ҫ���ر�socket
 �� �� ֵ: BOOL  
           TRUE:�ɹ� FALSE:ʧ��
 ��    ������
***************************************************************/
//##ModelId=45190CD801DF
bool CNet::close()
{
	if (m_eState != CLOSED)
	{
		try
		{
// 			if(m_nLocalRole == LOCAL_ACT_AS_SERVER) //�����������رգ�����TIME_WAIT״̬
// 			{
// 				if(SetOptions(RECVTIME,2000))
// 				{
// 					u_long nRead;
// #ifdef OS_WINDOWS 	
// 					ioctlsocket(m_hSocket, FIONREAD, &nRead);
// 					if(nRead == 0){
// 						char Buf[2] = {'\0'};
// 						recv(m_hSocket, Buf, nRead, MSG_PEEK);
// 					}
// 					else if(nRead > 0){
// 						char *pBuf = new char[nRead+2];
// 						recv(m_hSocket, pBuf, nRead, MSG_PEEK);
// 						delete[] pBuf;
// 					}
// #endif
// 
// #ifdef OS_LINUX
// 					// linux�� ��ioctl����������������
// 					// 0-������� 1-������ 2-���ȫ��
// 					int n=2;               
// 					ioctl(m_hSocket,TCFLSH,&n);
// #endif				
// 				}
// 			}
			::shutdown(m_hSocket, 2/*SD_BOTH*/);
#ifdef OS_WINDOWS

			::closesocket (m_hSocket);
#endif 

#ifdef OS_LINUX
			::close(m_hSocket);
#endif
			//��λ��־
			m_hSocket = INVALID_SOCKET;
			m_nLocalRole = LOCAL_ACT_AS_UNKNOW;
			m_eState = CLOSED;
			return true;
		}
		catch(...)
		{
			m_LogFile.FormatAdd(CLogFile::error,"::close() error,reason:%s(%d)",\
			                   strerror(errno),errno);
			return false;
		}
	}
	return true;
}

/*************************************************************
 �� �� ����GetLocalRole()
 ���ܸ�Ҫ����õ�ǰsocket�Ľ�ɫ
 �� �� ֵ: int  LOCAL_ACT_AS_UNKNOW:δ֪ 
                LOCAL_ACT_AS_SERVER:������
                LOCAL_ACT_AS_CLIENT:�ͻ���
 ��    ������
***************************************************************/
//##ModelId=45190CD8021D
int CNet::GetLocalRole()
{
	return m_nLocalRole;
}

/*************************************************************
 �� �� ����SetLocalRole()
 ���ܸ�Ҫ�������õ�ǰsocket�Ľ�ɫ
 �� �� ֵ: void  
 ��    ����     LOCAL_ACT_AS_UNKNOW: -1 
                LOCAL_ACT_AS_SERVER:  0
                LOCAL_ACT_AS_CLIENT:  1
***************************************************************/
//##ModelId=45190CD8021E
void CNet::SetLocalRole(int nRole)
{
	if( (nRole > 1 ) || ( nRole <= -1 ) )
		m_nLocalRole=LOCAL_ACT_AS_UNKNOW;
	else if( nRole == 1 )
		m_nLocalRole=LOCAL_ACT_AS_CLIENT;
	else
		m_nLocalRole=LOCAL_ACT_AS_SERVER;
	return;
}

/*************************************************************
 �� �� ����SetHandler()
 ���ܸ�Ҫ���������õ�ǰ�����socket���
 �� �� ֵ: void  
 ��    ����handler  socket���
***************************************************************/
//##ModelId=45190CD80221
void CNet::SetHandler(SOCKET handler)
{
	close();

	m_hSocket=handler;
}

/*************************************************************
 �� �� ����GetHandler()
 ���ܸ�Ҫ����õ�ǰ�����socket���
 �� �� ֵ: handler  socket���  
 ��    ����void 
***************************************************************/
//##ModelId=45190CD80220
SOCKET CNet::GetHandler()
{
	return m_hSocket;
}

/*************************************************************
 �� �� ����SetState()
 ���ܸ�Ҫ�����õ�ǰsocket��״̬
 �� �� ֵ: void 
 ��    ����enum sockstate_type 
           {INITIALIZED, BINDED, LISTENING, CONNECTED, CLOSED}
***************************************************************/
//##ModelId=45190CD80225
void CNet::SetState(sockstate_type state)
{
	if( (state<0)||(state>4) )
	{
		m_LogFile.Add("CNet.SetState()'parameter is invalid",CLogFile::error);

		return;
	}

	m_eState=state;

	return;
}

/*************************************************************
 �� �� ����SetOpponentAddr()
 ���ܸ�Ҫ������ͨѶ�Զ˵�ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  �����ַ�ṹ(IP+port)
***************************************************************/
//##ModelId=45190CD80216
void CNet::SetOpponentAddr(const NETADDR *addr)
{
	memcpy(&m_OpponentAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 �� �� ����SetLocalAddr()
 ���ܸ�Ҫ������ͨѶ���ص�ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  �����ַ�ṹ(IP+port)
***************************************************************/
void CNet::SetLocalAddr(const NETADDR *addr)
{
	memcpy(&m_LocalAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 �� �� ����SetMultiCastAddr()
 ���ܸ�Ҫ������ͨѶ�ಥ��ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  �����ַ�ṹ(IP+port)
***************************************************************/
void CNet::SetMultiCastAddr(const NETADDR *addr)
{
	memcpy(&m_MultiCastAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 �� �� ����GetOpponentAddr()
 ���ܸ�Ҫ�����ͨѶ�Զ˵�ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  ���������ַ�ṹ(IP+port)
***************************************************************/
//##ModelId=45190CD8021B
void CNet::GetOpponentAddr(NETADDR * addr)
{
	if(addr !=NULL)
		memcpy(addr,&m_OpponentAddr,sizeof(NETADDR));
	else
	{
		m_LogFile.Add("CNet.GetOpponentAddr()'s parameter is invalid",CLogFile::error);

		return;
	}
}

/*************************************************************
 �� �� ����GetLocalAddr()
 ���ܸ�Ҫ�����ͨѶ���ص�ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  ���������ַ�ṹ(IP+port)
***************************************************************/
void CNet::GetLocalAddr(NETADDR * addr)
{
	if(addr !=NULL)
		memcpy(addr,&m_LocalAddr,sizeof(NETADDR));
	else
	{
		m_LogFile.Add("CNet.m_LocalAddr()'s parameter is invalid",CLogFile::error);

		return;
	}
}

/*************************************************************
 �� �� ����GetMultiCastAddr()
 ���ܸ�Ҫ�����ͨѶ�ಥ��ַ
 �� �� ֵ: void 
 ��    ����NETADDR *addr  ���������ַ�ṹ(IP+port)
***************************************************************/
void CNet::GetMultiCastAddr(NETADDR * addr)
{
	if(addr !=NULL)
		memcpy(addr,&m_MultiCastAddr,sizeof(NETADDR));
	else
	{
		m_LogFile.Add("CNet.GetMultiCastAddr()'s parameter is invalid",CLogFile::error);

		return;
	}
}

/*************************************************************
 �� �� ����Bind()
 ���ܸ�Ҫ���󶨶˿�
 �� �� ֵ: BOOL  TRUE:�ɹ� FALSE:ʧ��
 ��    ����port  ָ��Ҫ�󶨵Ķ˿ں�
***************************************************************/
//##ModelId=45190CD801F6
bool CNet::Bind(u_short port)
{
	//������
	int eCode = 0;

	if (m_eState != INITIALIZED)
	{		
		m_LogFile.Add("bind in a invalid socket,failed",CLogFile::error);

		return false;		
	}

	struct sockaddr_in servaddr;
	memset (&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = m_nDomain;
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0")/* htonl (INADDR_ANY )*/;
	servaddr.sin_port = htons(port);
    
	if (::bind (m_hSocket, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR )
	{	
#ifdef OS_WINDOWS 
		char cherror[255]="";
		GetSocketErrorMsg(cherror);
		m_LogFile.FormatAdd(CLogFile::error,"::bind failed in Prot=%d,reason=%s",\
			               port,cherror);
#endif

#ifdef OS_LINUX
		m_LogFile.FormatAdd(CLogFile::error,"::bind failed in prot=%d,reason=%s(%d)",\
			          port,strerror(errno),errno);
#endif
		return false;
	}

	//��ɹ�
	m_eState = BINDED;

	return true;
}

/*************************************************************
 �� �� ����Bind()
 ���ܸ�Ҫ����ָ�������Ķ˿�
 �� �� ֵ: BOOL  TRUE:�ɹ� FALSE:ʧ��
 ��    ����host  ����������ָ��
	       port  Ҫ�󶨵Ķ˿�
***************************************************************/
//##ModelId=45190CD801F8
bool CNet::Bind(const char* host,u_short port)
{
	int eCode = 0;      //������

	if (m_eState != INITIALIZED)
	{		
		m_LogFile.Add("bind in a invalid socket,failed",CLogFile::error);

		return false;		
	}

	//�󶨵�ַ�ṹ����
	struct sockaddr_in servaddr;

	memset ( &servaddr, 0, sizeof ( servaddr ) );

	servaddr.sin_family = m_nDomain;

	servaddr.sin_port = htons ( port );

	if(strlen(host)==0)    
	{
		//����Ϊ��ʱ,������IP
		servaddr.sin_addr.s_addr =inet_addr("0.0.0.0")/* htonl ( INADDR_ANY )*/;
	}
	else
	{
    	if ( isdigit ( host[ 0 ] ) )
		{
			//IP��ֱַ��ת��
			servaddr.sin_addr.s_addr = inet_addr(host);
		}    
		else
		{
			//��������Ҫ��ת��ΪIP
			struct hostent* hostStruct;

			struct in_addr* hostNode;

			hostStruct = gethostbyname (host);

			if ( hostStruct )
			{
				hostNode = (struct in_addr* )hostStruct->h_addr_list[0] ;

				servaddr.sin_addr.s_addr = hostNode -> s_addr;
			}	
			else
			{
				m_LogFile.FormatAdd(CLogFile::error,"gethostbyname() failed when \
					      bind in %s:%d,reason:%s",host,port,strerror(errno));

				return false;
			}
		}
	}

	//��ʼ��
	if ( ::bind ( m_hSocket, ( struct sockaddr* ) &servaddr,
                  sizeof ( servaddr ) ) == SOCKET_ERROR )
	{	
#ifdef OS_WINDOWS  
		char cherror[255]="";
		GetSocketErrorMsg(cherror);
		m_LogFile.FormatAdd(CLogFile::error,"::bind failed in %s:%d,reason=%s",\
			                host,port,cherror);
#endif

#ifdef OS_LINUX
		m_LogFile.FormatAdd(CLogFile::error,"::bind failed in %s:%d,reason=%s(%d)",\
			                host,port,strerror(errno),errno);
#endif
	
		return false;
	}

	//�󶨱�־��λ
	m_eState = BINDED;

	return true;
}

/*************************************************************
 �� �� ����Listen()
 ���ܸ�Ҫ������socketΪ����״̬
 �� �� ֵ: BOOL  
           TRUE:�ɹ� FALSE:ʧ��
 ��    ����
           int queue: ָ����socketͬʱ�ܽ��յ����������
***************************************************************/
//##ModelId=45190CD801FF
bool CNet::Listen(int queue)
{
	//��֤���Ѿ�����׽����Ͻ��м�������
	if ( m_eState != BINDED )
	{
		m_LogFile.Add("listen in a socket which isn't in binding state",CLogFile::error);
		return false;
	}

	if (::listen(m_hSocket, queue) == SOCKET_ERROR)
	{
		//����ʧ��
        m_LogFile.FormatAdd(CLogFile::error,"::listen() failed,reason:%s(%d)!",\
			               strerror(errno),errno);
		return false;
	}

	//������־��λ
	m_eState = LISTENING;

	return true;
}

/*************************************************************
 �� �� ����Accept()
 ���ܸ�Ҫ����ʼsocket����
 �� �� ֵ: BOOL  
           TRUE:�ɹ� FALSE:ʧ��
 ��    ����
           sock  �����������Client CNet����
***************************************************************/
//##ModelId=45190CD80201
bool CNet::Accept(CNet & sock)
{
	int eCode = 0;

	if ( m_eState != LISTENING )
	{
		//�����Socket�����ڼ���״̬��ֱ�ӷ���

       	m_LogFile.Add("accept in a socket which isn't in listening state",\
			         CLogFile::error);
		return false;
	}

	//���ڽ������ӷ��ĵ�ַ��Ϣ
	struct sockaddr_in clientaddr;         

	SOCKET	handler;

	//windows �� sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof(clientaddr); 

	handler = ::accept (m_hSocket, (struct sockaddr*)&clientaddr, &clientlen);

	if( handler == INVALID_SOCKET )
	{
		m_LogFile.FormatAdd(CLogFile::error,"accept error,reason:%s(%d)", \
			                strerror(errno),errno);
		return false;
	}

	//ȡ�ü������Ŀͻ���IP/port
	sprintf(m_OpponentAddr.IpAddr, "%s", inet_ntoa(clientaddr.sin_addr));

	m_OpponentAddr.nPort=ntohs(clientaddr.sin_port);

    //����sock����
	sock.SetOpponentAddr(&m_OpponentAddr);

	sock.SetHandler(handler);

	sock.SetLocalRole(LOCAL_ACT_AS_SERVER);

	sock.SetState(CONNECTED);

	return true;			
}

/*************************************************************
 �� �� ����AddToMultiCast()
 ���ܸ�Ҫ�������鲥
 �� �� ֵ: BOOL  
           TRUE:�ɹ� FALSE:ʧ��
 ��    ������
***************************************************************/
BOOL CNet::AddToMultiCast()
{
#ifdef OS_LINUX

	if (Bind(m_MultiCastAddr.IpAddr, m_MultiCastAddr.nPort)==false)
	{
		return FALSE;
	}

	struct ip_mreq mreq;
	inet_aton(m_MultiCastAddr.IpAddr, &mreq.imr_multiaddr);
	inet_aton(m_LocalAddr.IpAddr, &(mreq.imr_interface));

	if (setsockopt(m_hSocket, SOL_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0 )
	{
	   return FALSE;
	}
#endif

#ifdef OS_WINDOWS

	if (Bind(m_LocalAddr.IpAddr, m_LocalAddr.nPort)==false)
	{
		return FALSE;
	}

	/* ��ֹloopback */  
	/*
	BOOL bFlag = FALSE;  
	if (WSAIoctl(m_hSocket,   
		SIO_MULTIPOINT_LOOPBACK,   
		&bFlag,  
		sizeof(bFlag),   
		NULL,   
		0,  
		NULL,   
		NULL,  
		NULL)==SOCKET_ERROR)
	{
		return FALSE;
	}
	*/

	SOCKADDR_IN GroupAddr;

	memset(&GroupAddr, 0, sizeof(GroupAddr));
	GroupAddr.sin_family=AF_INET;
	GroupAddr.sin_addr.s_addr=inet_addr(m_MultiCastAddr.IpAddr);
	GroupAddr.sin_port=htons(m_MultiCastAddr.nPort);
		
	/* ����multicast ��*/  	if (WSAJoinLeaf(m_hSocket, 
					(PSOCKADDR)&GroupAddr, 
					sizeof(GroupAddr), 
					NULL, 
					NULL, 
					NULL, 
					NULL,   
					JL_BOTH) ==INVALID_SOCKET)
	{
		return FALSE;	
	} 

#endif
	
	m_eState = CONNECTED;
	return TRUE;		
}

/*************************************************************
 �� �� ����read()
 ���ܸ�Ҫ����socket��ȡָ���ֽڵ�����
           (��������������,�յ����ݷ����Լ��Ľ��ջ�������)
 �� �� ֵ: 
           int  ����ʵ�ʶ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ����ȡ�ֽ���
***************************************************************/
//##ModelId=45190CD801E2
int CNet::read(int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return read_udp(nLen);
	}

	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	/* ------ Added by qingch   3/12/2009 ----- */
#ifdef __SOSELECT_TIMEOUT
	fd_set	set;
	//FD_ZERO(&set);          // comment by jjl 2011-06-08

	SOCKET	FDData = m_hSocket;
	//FD_SET(FDData, &set);  // comment by jjl 2011-06-08

	struct timeval timeout;
	timeout.tv_sec = m_nRecvTimes/1000;
	timeout.tv_usec = (m_nRecvTimes%1000)*1000;
	
#endif

	/*----------add by jjl 2011/0608------------  */
#ifdef __SOPOLL_TIMEOUT
    struct pollfd tempfd[1];
	tempfd[0].fd    = m_hSocket;
	tempfd[0].events = POLLIN;
	int nPollRet    = 0;
#endif

	//��ʱ����
	char      rcvBuf[MAXMSGLEN+1]="";
	char      *pchar = rcvBuf;
	int       nRet = 0,readdone =0;
    int       readleft = nLen;

	while(readleft >0){

#ifdef __SOSELECT_TIMEOUT
		/* add by jjl 2011-06-08 */
		FD_ZERO(&set);
        FD_SET(FDData, &set);
		/* ***********************/
        int selectRet = select(FDData+1, &set, NULL, NULL, &timeout);
        if (selectRet <= 0)
        {
            if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == selectRet)
            {
                m_LogFile.Add("select error when read data!",CLogFile::error);
                return SOCKET_ERROR;;
            }
            else if (0 == selectRet) {
                return TIMEOUT;
            }
        }
#endif		

#ifdef __SOPOLL_TIMEOUT
		nPollRet =  poll(tempfd,1,m_nRecvTimes);
		if(nPollRet <= 0)  // timeout or error
		{
			if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == nPollRet)
            {
                m_LogFile.FormatAdd(CLogFile::error,
					                "poll() error when read data from %s:%d,reason:%s(%d)",
									m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
                return SOCKET_ERROR;;
            }
            else if (0 == nPollRet) 
			{
                return TIMEOUT;
            }
		}
#endif

		if((readdone = recv(m_hSocket, pchar,readleft, 0)) == SOCKET_ERROR)
		{
			if (nRet > 0)
			{
				break;
			}
			
#ifdef OS_WINDOWS
			if(WSAGetLastError() == WSAETIMEDOUT)
				return TIMEOUT;
			
			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s", \
				cherror);
#endif
			
#ifdef OS_LINUX
			if(errno == EAGAIN/*11*/)  //��ʱ
				return TIMEOUT;
			
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s(%d)", \
				strerror(errno),errno);
#endif
			return SOCKET_ERROR;
		}
	
		if(readdone == 0)
			break;

		readleft   -= readdone;
		nRet       += readdone;
        pchar      += readdone;
	}

    //���ջ�����
	XJBUFFER * pBuf=(XJBUFFER*)GetRecvBuffer();
    
	//�����屣��
	if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//���
	memset(pBuf,0,sizeof(XJBUFFER));

	//���ö������ֽ���
	if(nRet >= 0)
	{
		pBuf->len=nRet;
		memcpy(pBuf->buf,rcvBuf,nRet);
	}
	
	//�˳�������
    m_Lock.unlock();

	return nRet;
}

/*************************************************************
 �� �� ����read()
 ���ܸ�Ҫ����socket��ȡָ���ֽڵ����ݵ�ָ����������
           
 �� �� ֵ: 
           int  ����ʵ�ʶ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ����ȡ�ֽ���
		   pBuf ��Ž������ݵĻ�����
***************************************************************/
int CNet::read(void * pBuf,int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return read_udp(pBuf, nLen);
	}

	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if(pBuf == NULL){
		m_LogFile.Add("the first parameter is NULL of ::read(),error");
		return SOCKET_ERROR;
	}

	/* ------ Added by qingch   3/12/2009 ----- */
#ifdef __SOSELECT_TIMEOUT
	fd_set	set;
	//FD_ZERO(&set);
	
	SOCKET	FDData = m_hSocket;
	//FD_SET(FDData, &set);
	
	struct timeval timeout;
	timeout.tv_sec = m_nRecvTimes/1000;
	timeout.tv_usec = (m_nRecvTimes%1000)*1000;

#endif
/* --------------------------------------- */

/*----------add by jjl 2011/0608------------  */
#ifdef __SOPOLL_TIMEOUT
    struct pollfd tempfd[1];
	tempfd[0].fd    = m_hSocket;
	tempfd[0].events = POLLIN;
	int nPollRet    = 0;
#endif

	char      *pchar = (char*)pBuf;
	int       nRet = 0,readdone =0;
    int       readleft = nLen;

	while(readleft >0){

#ifdef __SOSELECT_TIMEOUT

		FD_ZERO(&set);
		FD_SET(FDData, &set);
        int selectRet = select(FDData+1, &set, NULL, NULL, &timeout);
        if (selectRet <= 0)
        {
            if (nRet > 0)
            {
                break;
			}
            if(SOCKET_ERROR == selectRet)
            {
                m_LogFile.Add("select error when read data!",CLogFile::error);
                return SOCKET_ERROR;;
            }
            else if (0 == selectRet) {
                return TIMEOUT;
            }
        }
#endif

#ifdef __SOPOLL_TIMEOUT
		nPollRet =  poll(tempfd,1,m_nRecvTimes);
		if(nPollRet <= 0)  // timeout or error
		{
			if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == nPollRet)
            {
                m_LogFile.FormatAdd(CLogFile::error,
					                "poll() error when read data from %s:%d,reason:%s(%d)",
									m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
                return SOCKET_ERROR;;
            }
            else if (0 == nPollRet) 
			{
                return TIMEOUT;
            }
		}
#endif
		if((readdone = recv(m_hSocket, pchar,readleft, 0)) == SOCKET_ERROR)
		{
			if (nRet > 0)
			{
				break;
			}

#ifdef OS_WINDOWS
			if(WSAGetLastError() == WSAETIMEDOUT)
				return TIMEOUT;
			
			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s", \
				cherror);
#endif
			
#ifdef OS_LINUX
			if(errno == EAGAIN/*11*/)  //��ʱ
				return TIMEOUT;
			
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s(%d)", \
				strerror(errno),errno);
#endif
			
			return SOCKET_ERROR;
		}

		 // ���ӹر�
		if(readdone == 0)
			break;

		readleft -= readdone;
		pchar    += readdone;
		nRet     += readdone;
	}

	return nRet;
}

/*************************************************************
 �� �� ����read_udp()
 ���ܸ�Ҫ����socket��ȡָ���ֽڵ�����
           (��������������,�յ����ݷ����Լ��Ľ��ջ�������)
 �� �� ֵ: 
           int  ����ʵ�ʶ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ����ȡ�ֽ���
***************************************************************/
int CNet::read_udp(int nLen)
{
	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if (nLen <= 0)
	{
		return 0;
	}

	//��ʱ����
	char      rcvBuf[MAXMSGLEN+1]="";
	char      *pchar = rcvBuf;
	int		readdone=0;
	struct	sockaddr_in clientaddr;

#ifdef OS_LINUX
	SOCK_LEN addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, pchar, nLen, 0, (struct sockaddr *)&clientaddr, (SOCK_LEN *)&addrlen))
			== SOCKET_ERROR)
	{
		if(errno == EAGAIN)  //��ʱ
		{
			return 0;
		}
		else
		{
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s(%d)", \
				strerror(errno),errno);

			return SOCKET_ERROR;
		}
	}
#endif

#ifdef OS_WINDOWS
	int addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, pchar, nLen, 0, (struct sockaddr *)&clientaddr, (int *)&addrlen))
			== SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAETIMEDOUT)
		{
			return 0;
		}
		else
		{		
			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s", \
				cherror);

			return SOCKET_ERROR;
		}
	}
#endif
	
	if(readdone == 0)
		return 0;

    //���ջ�����
	XJBUFFER * pBuf=(XJBUFFER*)GetRecvBuffer();
    
	//�����屣��
	if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//���
	memset(pBuf,0,sizeof(XJBUFFER));
	pBuf->len=readdone;
	memcpy(pBuf->buf,rcvBuf,readdone);
	
	//�˳�������
    m_Lock.unlock();

	memset(&m_OpponentAddr, 0, sizeof(NETADDR));
	strcpy(m_OpponentAddr.IpAddr, inet_ntoa(clientaddr.sin_addr));
	m_OpponentAddr.nPort=ntohs(clientaddr.sin_port);

	return readdone;
}

/*************************************************************
 �� �� ����read_udp_from_addr()
 ���ܸ�Ҫ����socket��ȡָ���ֽڵ����ݵ�ָ���������У������ضԷ�
			��ַ
           
 �� �� ֵ: 
           int  ����ʵ�ʶ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ����ȡ�ֽ���
		   pBuf ��Ž������ݵĻ�����
		   sock_addr	�Է���ַ������
***************************************************************/
int CNet::read_udp(void *pbuff, int nLen)
{
	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if (nLen <= 0)
	{
		return 0;
	}

	//��ʱ����
	int readdone=0;
	struct	sockaddr_in clientaddr;

#ifdef OS_LINUX
	SOCK_LEN addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, pbuff, nLen, 0, (struct sockaddr *)&clientaddr, (SOCK_LEN *)&addrlen))
			== SOCKET_ERROR)
	{
		if(errno == EAGAIN/*11*/)  //��ʱ
		{
			return 0;
		}
	
		else
		{
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s(%d)", \
				strerror(errno),errno);

			return SOCKET_ERROR;
		}
	}
#endif

#ifdef OS_WINDOWS
	int addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, (char *)pbuff, nLen, 0, (struct sockaddr *)&clientaddr, (int *)&addrlen))
			== SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAETIMEDOUT)
		{
			return 0;
		}
		else
		{
			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s", \
				cherror);

			return SOCKET_ERROR;
		}
	}
#endif

	if(readdone == 0)
		return 0;

	memset(&m_OpponentAddr, 0, sizeof(NETADDR));
	strcpy(m_OpponentAddr.IpAddr, inet_ntoa(clientaddr.sin_addr));
	m_OpponentAddr.nPort=ntohs(clientaddr.sin_port);

	return readdone;
}

/*************************************************************
 �� �� ����write()
 ���ܸ�Ҫ����socket����ָ���ֽڵ�����
           (�����Լ��ķ��ͻ������е�����)
 �� �� ֵ: 
           int  ����ʵ�ʷ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ�������ֽ���
***************************************************************/
//##ModelId=45190CD801E0
int CNet::write(int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return write_udp(nLen);
	}

	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if(nLen < 0)
		return SOCKET_ERROR;

	//�����屣��
    if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//���ͻ�����
	XJBUFFER * pBuf=(XJBUFFER*)GetSendBuffer();

	//ʵ�ʷ�����ȡBUFFER�е�size ��nLen��Сֵ
	int size= ( nLen > pBuf->len ) ? (pBuf->len) : nLen;

	//��ʱ����
	char sndBuf[MAXMSGLEN+1]="";
	memcpy(sndBuf,pBuf->buf,size);

	//�������ͷ�
    m_Lock.unlock();

	/* ------ Added by qingch   3/12/2009 ----- */
#ifdef __SOSELECT_TIMEOUT
	fd_set	set;
	//FD_ZERO(&set);
	
	SOCKET	FDData = m_hSocket;
	//FD_SET(FDData, &set);
	
	struct timeval timeout;
	timeout.tv_sec = m_nSendTimes/1000;
	timeout.tv_usec = (m_nSendTimes%1000)*1000;
	
#endif
	/* --------------------------------------- */

/*----------add by jjl 2011/0608------------  */
#ifdef __SOPOLL_TIMEOUT
    struct pollfd tempfd[1];
	tempfd[0].fd    = m_hSocket;
	tempfd[0].events = POLLOUT;
	int nPollRet    = 0;
#endif

	int nRet = 0 ,writedone =0,writeleft =size;
	char * pchar = sndBuf;

	while(writeleft >0){

#ifdef __SOSELECT_TIMEOUT
	
		FD_ZERO(&set);
		FD_SET(FDData, &set);
        int selectRet = select(FDData+1,NULL, &set ,NULL, &timeout);
        if (selectRet <= 0)
        {
            if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == selectRet)
            {
                m_LogFile.Add("select error when write data!",CLogFile::error);
                return SOCKET_ERROR;;
            }
            else if (0 == selectRet) {
                return TIMEOUT;
            }
        }
#endif		

#ifdef __SOPOLL_TIMEOUT
		nPollRet =  poll(tempfd,1,m_nSendTimes);
		if(nPollRet <= 0)  // timeout or error
		{
			if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == nPollRet)
            {
                m_LogFile.FormatAdd(CLogFile::error,
					                "poll() error when write data to %s:%d,reason:%s(%d)",
									m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
                return SOCKET_ERROR;;
            }
            else if (0 == nPollRet) 
			{
                return TIMEOUT;
            }
		}
#endif

		if ((writedone = send(m_hSocket, pchar, writeleft, 0/*SOCKET_SEND_FLAG*/)) == SOCKET_ERROR)
		{
            if (nRet > 0)
            {
                break;
            }
#ifdef OS_WINDOWS
            if(WSAGetLastError() == WSAETIMEDOUT)
				return TIMEOUT;

			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s",cherror);
#endif
			
#ifdef OS_LINUX
            if(errno == EAGAIN/*11*/)  //��ʱ
				return TIMEOUT;
			m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s(%d)",\
				strerror(errno),errno);
#endif
			return SOCKET_ERROR;
		}
        
		writeleft  -= writedone;
		pchar      += writedone;
		nRet       += writedone;
	}

	return nRet;
}

/*************************************************************
 �� �� ����write()
 ���ܸ�Ҫ����ָ���Ļ������з���ָ���ֽڵ�����
 �� �� ֵ: 
           int  ����ʵ�ʷ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ�������ֽ���
           pBuf ָ���ķ��ͻ�����ָ��
***************************************************************/
//##ModelId=45190CD801E0
int CNet::write(void * pBuf,int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return write_udp(pBuf, nLen);
	}

	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	//�жϲ�����Ч��
	if( (nLen < 0) || (pBuf == NULL) ){
		m_LogFile.Add("the parameters is invalid in write()",CLogFile::error);
		return SOCKET_ERROR;
	}

	/* ------ Added by qingch   3/12/2009 ----- */
#ifdef __SOSELECT_TIMEOUT
	fd_set	set;
	//FD_ZERO(&set);
	
	SOCKET	FDData = m_hSocket;
	//FD_SET(FDData, &set);
	
	struct timeval timeout;
	timeout.tv_sec = m_nSendTimes/1000;
	timeout.tv_usec = (m_nSendTimes%1000)*1000;
	
#endif
	/* --------------------------------------- */

/*----------add by jjl 2011/0608------------  */
#ifdef __SOPOLL_TIMEOUT
    struct pollfd tempfd[1];
	tempfd[0].fd    = m_hSocket;
	tempfd[0].events = POLLOUT;
	int nPollRet    = 0;
#endif

	int nRet = 0 ,writedone =0,writeleft =nLen;
	char * pchar = (char*)pBuf;

	while(writeleft >0){

#ifdef __SOSELECT_TIMEOUT
		FD_ZERO(&set);
		FD_SET(FDData, &set);
        int selectRet = select(FDData+1,NULL, &set ,NULL, &timeout);
        if (selectRet <= 0)
        {
            if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == selectRet)
            {
                m_LogFile.Add("select error when write data!",CLogFile::error);
                return SOCKET_ERROR;;
            }
            else if (0 == selectRet) {
                return TIMEOUT;
            }
        }
#endif		

#ifdef __SOPOLL_TIMEOUT
		nPollRet =  poll(tempfd,1,m_nSendTimes);
		if(nPollRet <= 0)  // timeout or error
		{
			if (nRet > 0)
            {
                break;
            }
            if(SOCKET_ERROR == nPollRet)
            {
                m_LogFile.FormatAdd(CLogFile::error,
					                "poll() error when write data to %s:%d,reason:%s(%d)",
									m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
                return SOCKET_ERROR;;
            }
            else if (0 == nPollRet) 
			{
                return TIMEOUT;
            }
		}
#endif

		if ((writedone = send(m_hSocket, pchar, writeleft,0/*SOCKET_SEND_FLAG*/)) == SOCKET_ERROR)
		{
            if (nRet > 0)
            {
                break;
            }
#ifdef OS_WINDOWS
            if(WSAGetLastError() == WSAETIMEDOUT)
				return TIMEOUT;
			char cherror[255]="";
			GetSocketErrorMsg(cherror);
			m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s",cherror);
#endif
			
#ifdef OS_LINUX
            if(errno == EAGAIN/*11*/)  //��ʱ
				return TIMEOUT;
			m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s(%d)",\
				strerror(errno),errno);
#endif
			return SOCKET_ERROR;
		}

		writeleft   -= writedone;
		pchar       += writedone;
		nRet        += writedone;
	}

	return nRet;
}

/*************************************************************
 �� �� ����write_udp()
 ���ܸ�Ҫ����socket����ָ���ֽڵ�����
           (�����Լ��ķ��ͻ������е�����)
 �� �� ֵ: 
           int  ����ʵ�ʷ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ�������ֽ���
***************************************************************/
int CNet::write_udp(int nLen)
{
	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if(nLen < 0)
		return 0;

	//�����屣��
    if( !m_Lock.lock() )
		return 0;

	//���ͻ�����
	XJBUFFER * pBuf=(XJBUFFER*)GetSendBuffer();

	//ʵ�ʷ�����ȡBUFFER�е�size ��nLen��Сֵ
	int size= ( nLen > pBuf->len ) ? (pBuf->len) : nLen;

	//��ʱ����
	char sndBuf[MAXMSGLEN+1]="";
	memcpy(sndBuf,pBuf->buf,size);

	//�������ͷ�
    m_Lock.unlock();

	int writedone =0,writeleft =size;
	char * pchar = sndBuf;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(m_OpponentAddr.IpAddr);
	servaddr.sin_port = htons(m_OpponentAddr.nPort);

	if ((writedone = sendto(m_hSocket, pchar, writeleft, SOCKET_SEND_FLAG, (struct sockaddr *) &servaddr, sizeof(servaddr))) == SOCKET_ERROR)
	{	
#ifdef OS_WINDOWS
		char cherror[255]="";
		GetSocketErrorMsg(cherror);
		m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s",cherror);
#endif

#ifdef OS_LINUX
		m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s(%d)",\
			strerror(errno),errno);
#endif
		return SOCKET_ERROR;
	}
        
	return writedone;
}

/*************************************************************
 �� �� ����write()
 ���ܸ�Ҫ����ָ���Ļ������з���ָ���ֽڵ����ݵ�ָ���ĵ�ַ
 �� �� ֵ: 
           int  ����ʵ�ʷ������ֽ��� (<=nLen)
 ��    ����
           nLen ָ�������ֽ���
           pBuf ָ���ķ��ͻ�����ָ��
		   sock_addr	�Է���ַ������
***************************************************************/
int CNet::write_udp(void *pBuf, int nLen)
{
	//��֤�����ӵ�״̬�½��н���
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if (pBuf==NULL || nLen<=0)
		return 0;

	int writedone=0,writeleft=nLen;
	char * pchar = (char *)pBuf;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(m_OpponentAddr.IpAddr);
	servaddr.sin_port = htons(m_OpponentAddr.nPort);

	if ((writedone = sendto(m_hSocket, pchar, writeleft, SOCKET_SEND_FLAG, (struct sockaddr *) &servaddr, sizeof(servaddr))) == SOCKET_ERROR)
	{	
#ifdef OS_WINDOWS
		char cherror[255]="";
		GetSocketErrorMsg(cherror);
		m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s",cherror);
#endif

#ifdef OS_LINUX
		m_LogFile.FormatAdd(CLogFile::error,"::send failed,reason:%s(%d)",\
			               strerror(errno),errno);
#endif
		return SOCKET_ERROR;
	}

	return writedone;
}

/*************************************************************
 �� �� ����GetLocalComputerIp()
 ���ܸ�Ҫ����ñ���IP
 �� �� ֵ: bool �ɹ���ʧ��
 ��    ����
           char * ip  ��ű���IP
***************************************************************/
//##ModelId=45190CD80214
bool CNet::GetLocalComputerIp(char * ipAddr)
{
	char         szLocalHostName[256]="";
	struct       hostent *he;
	struct       sockaddr_in their_addr;

#ifdef OS_LINUX
  
  #ifdef  OS_SUNUNIX
    strcpy(ipAddr,"127.0.0.1");	
  #else
	struct       ifreq ifr;
	strcpy(ifr.ifr_name,"eth0");                   // eth0 ��1������IP ; lo ���ػػ���ַ127.0.0.1

	if( (m_eState == CLOSED)           ||
        (m_hSocket == INVALID_SOCKET)  )
	{
		 strcpy(ipAddr,"127.0.0.1");
		 return false;
	}

	if(ioctl(m_hSocket,SIOCGIFADDR,&ifr)<0)
	{
	   m_LogFile.FormatAdd(CLogFile::error,"::get local computer ip failed,reason:%s(%d)",\
			              strerror(errno),errno);
       strcpy(ipAddr,"127.0.0.1");
	   return false;
	}
	else
	{
		strcpy(ipAddr,inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));
	}
  #endif

#endif

#ifdef OS_WINDOWS
	gethostname(szLocalHostName,256);
	he = gethostbyname(szLocalHostName);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);

	strcpy(ipAddr, inet_ntoa(their_addr.sin_addr));
#endif

	return true;
}

/*************************************************************
 �� �� ����SetSocketOption()
 ���ܸ�Ҫ������socket��ѡ��(ֱ�Ӷ�socket��ѡ�����,��Ҫ���
                   socket�ĸ���ѡ������)
           ��select���ó�ʱ,��ʹ�Է������ѹرձ��ػ���Ȼ�����ȴ�
		           ֱ����ʱ
 �� �� ֵ: bool   TRUE:�ɹ�  FALSE:ʧ��
 ��    ����int nOptionName   Ҫ���õ�ѡ������
           lpOptionValue     ѡ��ֵ
           nOptionLen        ѡ��ֵ�ĳ���
           nLevel            ѡ���
***************************************************************/
//##ModelId=45190CD8020A
bool CNet::SetSocketOption(int nOptionName, 
						   const char *lpOptionValue, 
						   int nOptionLen,
						   int nLevel)
{
	//����ֵ
	bool bReturn  = false;    

	//��־��¼
	char cLog[255]="";

	fd_set	set;
	SOCKET	FDData = m_hSocket;
	struct timeval timeout;
	int ms=0;
	
	//����nOptionName�Ĳ�ͬѡ���������
	switch(nOptionName) 
	{
	case SO_SNDBUF:   //���÷��ͻ�����
		if(setsockopt(m_hSocket, nLevel, nOptionName, lpOptionValue, nOptionLen) == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket SO_SNDBUF failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
		bReturn = true;			
		break;

	case SO_RCVBUF:   //���ý��ջ�����
		if(setsockopt(m_hSocket, nLevel, nOptionName, lpOptionValue, nOptionLen) == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket SO_RCVBUF failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
		bReturn = true;			
		break;

	case SO_SNDTIMEO:   //���÷��ͳ�ʱ
		FD_ZERO(&set);
		FD_SET(FDData, &set);
		ms = *(int *)lpOptionValue;
		timeout.tv_sec = ms/1000;
		timeout.tv_usec = (ms%1000)*1000;
		if( select(FDData+1, NULL, &set, NULL, &timeout) == SOCKET_ERROR )
		{
			sprintf(cLog,"set socket SO_SNDTIMEO failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}

		bReturn = true;
		break;
	case SO_RCVTIMEO:   //���ý��ճ�ʱ
		FD_ZERO(&set);
		FD_SET(FDData, &set);
		ms = *(int *)lpOptionValue;
		timeout.tv_sec = ms/1000;
		timeout.tv_usec = (ms%1000)*1000;
		if(select(FDData+1, &set, NULL, NULL, &timeout) == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket SO_RCVTIMEO failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
	
		bReturn = true;
		break;
	default:   //������ò��账��		
        sprintf(cLog,"the parameter of OptionName=%d  in SetSocketOption() is invalid",nOptionName);
		break;
	}
	
	if(!bReturn)
	{
		m_LogFile.Add(cLog,CLogFile::error);
	}
	return bReturn;
}

/*************************************************************
 �� �� ����SetOptions()
 ���ܸ�Ҫ������socket��ѡ��(�������socket�ĸ�ѡ������)
 �� �� ֵ: bool   
           TRUE:�ɹ�  FALSE:ʧ��
 ��    ����int nFlag         Ҫ���õ�ѡ������
           int nTime         ѡ��ֵ,���ó�ʱʱΪ��ʱʱ��(ms)
		                     ����ѡ��ʱ��0
           int nReserve      ����
***************************************************************/
//##ModelId=45190CD801E6
bool CNet::SetOptions(int nFlag,int nTime,int nReserve)
{
	//��־
	char cLog[1024]="";

	//����ֵ
	bool bReturn = false;

	//��Ϊ��Чsocket�򷵻�
	if(m_hSocket == INVALID_SOCKET)
	{
		m_LogFile.Add("setOpetions in a invalid socket,failed",CLogFile::error);
	    return false;
	}

	if(nTime<0)
	{
		m_LogFile.FormatAdd(CLogFile::error,"the second parameter of setOpetions \
	                        =%d error, should set a value >=0",nTime);
		return false;
	}

	SOCK_LEN nActualTime=nTime;      //��λΪ����
	unsigned long ul = 0;            //��ʱ����
	int status = 0;                  //��ʱ����
	fd_set r;                        //���ڴ��socket
   	int ret=0;                       //����ֵ

	//�ó�ʱ����,linux�³�ʱ����
	struct timeval timeout ;       
	timeout.tv_sec = nTime/1000;     //���ӳ�ʱʱ��
	timeout.tv_usec =(nTime%1000)*1000; //΢��

	switch(nFlag)
	{
	case RECVTIME:
		/* ------ Added by qingch   3/12/2009 ----- */
		m_nRecvTimes = nTime;
		/* --------------------------------------- */
#ifdef OS_WINDOWS
		ret = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&nActualTime,sizeof(nActualTime));
#endif

#ifdef OS_LINUX

#ifdef __SOSELECT_TIMEOUT 
		/*
		* ----- commented by qingch   3/12/2009 ------
		  ret = (SetSocketOption(SO_RCVTIMEO,(char*)&nTime,0,0))?0:SOCKET_ERROR;
		  ����֤�ú��������õ�ʱ�������ָ��ʱ�䣬��ʵ�ʽ��ܳ�ʱû����Ч
		  �ָĳ��ڽ��յ�ʱ�����selectʵ��
		*/
#else
   #ifdef __SOPOLL_TIMEOUT
        /* ����ʱ������ */
   #else
		ret = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(void*)&timeout,sizeof(timeval)); 
        //������select���ó�ʱ,����select���ó�ʱʱ��ʹ�Է������ѹرձ��ػ���Ȼ�����ȴ�ֱ����ʱ
   #endif

#endif

#endif

		if(ret == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket recv timeout failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
	
		bReturn = true ;
		break;

	case SENDTIME:
		/* ------ Added by qingch   3/12/2009 ----- */
		m_nSendTimes = nTime;
		/* --------------------------------------- */
#ifdef OS_WINDOWS
		ret = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&nActualTime,sizeof(nActualTime));
#endif

#ifdef OS_LINUX

#ifdef __SOSELECT_TIMEOUT
		/*
		* ----- commented by qingch   3/12/2009 ------
		  ret = (SetSocketOption(SO_SNDTIMEO,(char*)&nTime,0,0))?0:SOCKET_ERROR;
		  ����֤�ú��������õ�ʱ�������ָ��ʱ�䣬��ʵ�ʷ��ͳ�ʱû����Ч
		  �ָĳ��ڷ��͵�ʱ�����selectʵ��
		*/
#else

   #ifdef __SOPOLL_TIMEOUT
        /* ����ʱ������ */
   #else
		ret = setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeval));
		//if(!SetSocketOption(SO_SNDTIMEO,(char*)&nActualTime,sizeof(nActualTime),SOL_SOCKET))
		//   ret = -1;
   #endif
#endif

#endif



		if(ret == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket send timeout failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
	
		bReturn = true ;
		break;

	case BLOCKING:
#ifdef OS_WINDOWS
		ul=0;
		if(ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul)==0)
		{
			bReturn=true;
		}
		else
		{
			bReturn=false;
		}
#endif

#ifdef OS_LINUX
		//linux��
		status = fcntl(m_hSocket,F_GETFL);
		if( fcntl(m_hSocket,F_SETFL,status & (0xf7ff)) != -1) //O_NONBLOCK=0x0800;
			bReturn = true;
#endif
		if(!bReturn)
		{
			sprintf(cLog,"set socket blocking failed,reason:%s(%d)", \
				   strerror(errno),errno);
		}
	    break;

	case UNBLOCKING:
#ifdef OS_WINDOWS
		ul=1;
		if (ioctlsocket(m_hSocket, FIONBIO, (unsigned long*)&ul)==0)
		{
			bReturn=true;
		}
		else
		{
			bReturn=false;
		}
#endif

#ifdef OS_LINUX
		//linux��
		status = fcntl(m_hSocket,F_GETFL);
		if( fcntl(m_hSocket,F_SETFL,status|O_NONBLOCK) != -1)
			bReturn = true;
#endif
		if(!bReturn)
			sprintf(cLog,"set socket unblocking failed,reason:%s(%d)", \
				   strerror(errno),errno);
		break;

	case CONNTIME:
		FD_ZERO(&r);
		FD_SET(m_hSocket, &r);
		ret = select(0, 0, &r, 0, &timeout);
		if ( ret <= 0 )
			sprintf(cLog,"set socket conntect timeout failed,reason:%s(%d)", \
				   strerror(errno),errno);
		else
			bReturn = true;
		break;
	case BROADCAST:
		{
			int broadcast(1);
			ret = setsockopt(m_hSocket,SOL_SOCKET,SO_BROADCAST,(char*)&broadcast,sizeof(broadcast));
			if(ret == SOCKET_ERROR)
			{
				sprintf(cLog,"set socket broadcast failed,reason:%s(%d)", \
					strerror(errno),errno);
				break;
			}
			
			bReturn = true ;
		}
		break;
	case REUSEADDR:
		{
			int reuseaddr(1);
			ret = setsockopt(m_hSocket,SOL_SOCKET,SO_REUSEADDR,(char*)&reuseaddr,sizeof(reuseaddr));
			if(ret == SOCKET_ERROR)
			{
				sprintf(cLog,"set socket reuseaddr failed,reason:%s(%d)", \
					strerror(errno),errno);
				break;
			}
			
			bReturn = true;
		}
	default:
        sprintf(cLog,"the first parameter of SetOptions() is %d,invalid",nFlag);
		break;
	}

	if(!bReturn)
	{
		m_LogFile.Add(cLog,CLogFile::error);
	}

	return bReturn;
}

/*************************************************************
 �� �� ����ConnectServer()
 ���ܸ�Ҫ�����ӷ�����
 �� �� ֵ: bool   TRUE:�ɹ�  FALSE:ʧ��
 ��    ����char * host       ��������IP�������� (Ĭ��ΪNULL)
                              ��IP=NULLʱ,ȡm_OpponentAddr��Ա
           int   port        ��������port (Ĭ��Ϊ -1)
		                      ��port<0ʱ,ȡm_OpponentAddr��Ա
           int  nLocalPort   ����ǰָ�����ض˿� (Ĭ��Ϊ-1)
		                       ��ָ��ʱ���ȡһ���˿�
           int  nTimeout     ���ӳ�ʱ����, ȱʡ���Ϊϵͳ��ʱʱ��(��λΪs),������øò�������ʹ��
							 SetOptions ��socket����ΪUNBLOCKING.
***************************************************************/
//##ModelId=45190CD801EF
bool CNet::ConnectServer(const char * host,int port,int nLocalPort,int nTimeout)
{

	if (nTimeout > 0)
	{
		return ConnectServerB(host, port, nLocalPort,nTimeout);
	}

	//�����û�г�ʼ������open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			return false;
		}
	}

	//����Ѿ�������״̬���ȹر�
	if(m_eState == CONNECTED)
	{
		if((host == NULL)&&(port ==-1))
		{
			//û�д���������Ϊ��ͬһ������,ֱ�ӷ���
			return true;
		}
		else
		{
			//��ͬ������Ŀ����Ҫ�ȹر�,���³�ʼ��
			close();
			
			if(!open())
			{
				return false;
			}
		}
	}
    
	//���ж�host/port�Ƿ���Ч
	if( ( host != NULL ) && ( port > 0 ) )
	{
		unsigned int nlen = (strlen(host) > 40) ? 40 : strlen(host);
		memset(&m_OpponentAddr,0,sizeof(NETADDR));
		//���޸ĳ�Ա�����жԶ˵�ַ
		memcpy(m_OpponentAddr.IpAddr,host,nlen);

		m_OpponentAddr.nPort = (u_short)port;
	}

	//�ж�����ǰ�Ƿ���Ҫָ���˿�
	if(nLocalPort>0)
	{
		if(!Bind( (u_short)nLocalPort ) )
		{
			m_LogFile.FormatAdd(CLogFile::error,"bind in prot %d failed when \
				               connect %s:%d",nLocalPort,\
							   m_OpponentAddr.IpAddr,m_OpponentAddr.nPort);
			close();
			return false;
		}
	}

	//�����ַ��ṹ
	struct sockaddr_in servaddr;
	memset (&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = m_nDomain;
	servaddr.sin_port = htons (m_OpponentAddr.nPort);
	
	//�ж�hostΪIP or hostname
	if ( ( isdigit (m_OpponentAddr.IpAddr[0] ) ) &&  \
		  (strcmp(m_OpponentAddr.IpAddr,"0.0.0.0")!=0) )
	{
		servaddr.sin_addr.s_addr = inet_addr ( m_OpponentAddr.IpAddr );
	}    
	else
	{
		struct hostent* hostStruct;
		struct in_addr* hostNode;
		
		hostStruct = gethostbyname ( m_OpponentAddr.IpAddr );
		
		if ( hostStruct )
		{
			hostNode = (struct in_addr* )hostStruct->h_addr_list[0] ;
			
			servaddr.sin_addr.s_addr = hostNode->s_addr;
		}	
		else
		{
			m_LogFile.FormatAdd(CLogFile::error,"when ConnectServer(%s,%d) \
				               gethostbyname() failed,reason:%s(%d)",\
				    m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
			close();

			return false;
		}
	}

	//windows �� sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof (servaddr); 

	//��ʼ����		
	if(::connect(m_hSocket, (struct sockaddr*)&servaddr, clientlen) == SOCKET_ERROR)
	{
		m_LogFile.FormatAdd(CLogFile::error,"::connect(%s,%d) failed,reason:%s(%d)",\
			m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
		close();

		return false;
	}
    
	m_LogFile.FormatAdd(CLogFile::trace,"connect to %s:%d successfuly",\
                        m_OpponentAddr.IpAddr,m_OpponentAddr.nPort);

	m_nLocalRole = LOCAL_ACT_AS_CLIENT;

	m_eState = CONNECTED;

	return true;
}

/*************************************************************
 �� �� ����ConnectServerB()
 ���ܸ�Ҫ�����ӷ�����
 �� �� ֵ: bool   TRUE:�ɹ�  FALSE:ʧ��
 ��    ����char * host       ��������IP�������� (Ĭ��ΪNULL)
                              ��IP=NULLʱ,ȡm_OpponentAddr��Ա
           int   port        ��������port (Ĭ��Ϊ -1)
		                      ��port<0ʱ,ȡm_OpponentAddr��Ա
           int  nLocalPort   ����ǰָ�����ض˿� (Ĭ��Ϊ-1)
		                       ��ָ��ʱ���ȡһ���˿�
           int  nTimeout     ���ӳ�ʱ����
***************************************************************/
//##ModelId=45190CD801EF
bool CNet::ConnectServerB(const char * host,int port,int nLocalPort,int nTimeout)
{
	//�����û�г�ʼ������open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			return false;
		}
	}

	//����Ѿ�������״̬���ȹر�
	if(m_eState == CONNECTED)
	{
		if((host == NULL)&&(port ==-1))
		{
			//û�д���������Ϊ��ͬһ������,ֱ�ӷ���
			return true;
		}
		else
		{
			//��ͬ������Ŀ����Ҫ�ȹر�,���³�ʼ��
			close();
			
			if(!open())
			{
				return false;
			}
		}
	}
    
	//���ж�host/port�Ƿ���Ч
	if( ( host != NULL ) && ( port > 0 ) )
	{
		unsigned int nlen = (strlen(host) > 40) ? 40 : strlen(host);

		//���޸ĳ�Ա�����жԶ˵�ַ
		memcpy(m_OpponentAddr.IpAddr,host,nlen);

		m_OpponentAddr.nPort = (u_short)port;
	}

	//�ж�����ǰ�Ƿ���Ҫָ���˿�
	if(nLocalPort>0)
	{
		if(!Bind( (u_short)nLocalPort ) )
		{
			m_LogFile.FormatAdd(CLogFile::error,"bind in prot %d failed when \
				               connect %s:%d",nLocalPort,\
							   m_OpponentAddr.IpAddr,m_OpponentAddr.nPort);
			close();
			return false;
		}
	}

	//�����ַ��ṹ
	struct sockaddr_in servaddr;

	memset (&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = m_nDomain;
	servaddr.sin_port = htons(m_OpponentAddr.nPort);
	
	//�ж�hostΪIP or hostname
	if ( ( isdigit (m_OpponentAddr.IpAddr[0] ) ) &&  \
		  (strcmp(m_OpponentAddr.IpAddr,"0.0.0.0")!=0) )
	{
		servaddr.sin_addr.s_addr = inet_addr(m_OpponentAddr.IpAddr);
	}    
	else
	{
		struct hostent* hostStruct;
		struct in_addr* hostNode;
		
		hostStruct = gethostbyname ( m_OpponentAddr.IpAddr );
		
		if ( hostStruct )
		{
			hostNode = (struct in_addr* )hostStruct->h_addr_list[0] ;
			
			servaddr.sin_addr.s_addr = hostNode->s_addr;
		}	
		else
		{
			m_LogFile.FormatAdd(CLogFile::error,"when ConnectServer(%s,%d) \
				               gethostbyname() failed,reason:%s(%d)",\
				    m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
			close();

			return false;
		}
	}

	//windows �� sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof (servaddr); 
    int nRet = 0;

	//��ʼ����		
	if(::connect(m_hSocket, (struct sockaddr*)&servaddr, clientlen) == SOCKET_ERROR)
	{
		//���ڷ������׽��֣�connect()ʼ�շ���SOCKET_ERROR����Ҫ��һ���ж������Ƿ�����
#ifdef __SOPOLL_TIMEOUT
    struct pollfd tempfd[1];
	tempfd[0].fd    = m_hSocket;
	tempfd[0].events = POLLOUT;

	nRet =  poll(tempfd,1,nTimeout*1000);
#else
		fd_set	except_set;
		fd_set	write_set;
		timeval tv;

		FD_ZERO(&except_set);
		FD_ZERO(&write_set);
		FD_SET(m_hSocket, &except_set);
		FD_SET(m_hSocket, &write_set);

		tv.tv_sec = nTimeout;	//���ó�ʱΪ0������Ϊ���ж��Ƿ������ݿ��Զ����쳣��
		tv.tv_usec = 0; 

	    nRet = select(m_hSocket+1, NULL, &write_set, NULL, &tv);	//״̬���
#endif
		if (nRet <= 0)
		{
			if(nRet == 0)
			{
				m_LogFile.FormatAdd(CLogFile::error,"::connect(%s,%d) failed,reason:timeout in %ds",\
				                    m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,nTimeout);
			}
			else
				m_LogFile.FormatAdd(CLogFile::error,"::connect(%s,%d) failed,reason:%s(%d)",\
				                    m_OpponentAddr.IpAddr,m_OpponentAddr.nPort,strerror(errno),errno);
			close();

			return false;
		}
	}
    
	m_LogFile.FormatAdd(CLogFile::trace,"connect to %s:%d successfuly",\
                        m_OpponentAddr.IpAddr,m_OpponentAddr.nPort);

	m_nLocalRole = LOCAL_ACT_AS_CLIENT;

	m_eState = CONNECTED;

	return true;
}

/*************************************************************
 �� �� ����ListenServer()
 ���ܸ�Ҫ�����һ��socket�Ĵ������󶨡���������
 �� �� ֵ: bool  
           TRUE:�ɹ�  FALSE:ʧ��
 ��    ����const char * host  ���ؼ�����IP/������,
                              host==NULLʱ,������������IP
           u_short port       �����˿� 
		   nqueue             ����ܽ��յ�������
***************************************************************/
//##ModelId=45190CD80203
bool CNet::ListenServer(u_short port ,const char * host,int nqueue)
{
	//�����û�г�ʼ������open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			m_LogFile.Add("open () failed when ListenServer()",CLogFile::error);
			return false;
		}
	}

	//����Ѿ�������״̬���ȹر�
	if(m_eState == CONNECTED)
	{
		close();
		if(!open())
		{
			m_LogFile.Add("open () failed when ListenServer()",CLogFile::error);
			return false;
		}
	}

	//����Ѿ�open,��bind
	if(m_eState == INITIALIZED)
	{
		//���hostΪnull,��󶨱���������port
		if(host == NULL)
		{
			if( !Bind(port) )
			{
				m_LogFile.FormatAdd(CLogFile::error,"bind in port %d failed when  \
					               ListenServer()",port);
				close();
				return false;
			}
		}
		else   //ָ��IP���а�
		{
			if( !Bind(host,port) )
			{
				m_LogFile.FormatAdd(CLogFile::error,"bind in %s:%d failed when  \
					               ListenServer()",host,port);
				close();
				return false;
			}
		}
	}

    //����Ѿ��ڰ�״̬,�����
	if( m_eState == BINDED )
	{
		if( !Listen(nqueue) )
		{
			m_LogFile.FormatAdd(CLogFile::error,"listen(%d) failed when \
				               ListenServer()",nqueue);	              
			close();
			return false;
		}
	}

	//ȫ���ɹ��򷵻�true
	return true;
}

/*************************************************************
 �� �� ����CopyTo()
 ���ܸ�Ҫ������Դ����ָ�����������ݵ�Ŀ��Net����ķ��ͻ�����
 �� �� ֵ: 
           int   ʵ�ʿ����ֽ���   ʧ��ʱ����-1
 ��    ����const CNet & net  Ŀ�����
           int   nFlag       ������־ Ĭ��ȡ2
		                     1-����Դ����ķ���buf 
							 2-����Դ����Ľ���buf
***************************************************************/
//##ModelId=45190CD801EC
int CNet::CopyTo(const CNet & net,int nFlag)
{
	//����ֵ
	int size=0;

	//����Ǳ�������
	if(this == &net)
	{
		m_LogFile.Add("the first parameter is owner-self in copyto(),error",CLogFile::error);
		return -1;
	}

	if(nFlag == 0)
		return -1;
//�����屣��
    if( !m_Lock.lock())
		return -1;

	CCommon * pCommon=(CCommon*)&net;

	XJBUFFER * pDisSendBuf=(XJBUFFER*)pCommon->GetSendBuffer();
	XJBUFFER * pDisRecvBuf=(XJBUFFER*)pCommon->GetRecvBuffer();

	if(nFlag == 0)
		return -1;
	else if(nFlag == 1)
	{
		//���ͻ�����
		memcpy(pDisSendBuf,(XJBUFFER*)GetSendBuffer(),sizeof(XJBUFFER));
	    
		size+=((XJBUFFER*)GetSendBuffer())->len;
	}
	else if(nFlag == 2)
	{
		//���ջ�����
		memcpy(pDisSendBuf,(XJBUFFER*)GetRecvBuffer(),sizeof(XJBUFFER));
	    
		size+=((XJBUFFER*)GetRecvBuffer())->len;
	}
	else
	{
		//�������ͷ�
        m_Lock.unlock();

		m_LogFile.FormatAdd(CLogFile::error,"the second parameter in copyto() is \
			                %d ,invalid",nFlag);
		return -1;
	}

    //�������ͷ�
    m_Lock.unlock();

	m_LogFile.FormatAdd(CLogFile::trace,"copyto() %d size success,Flag is %d",\
		               size,nFlag);
	return size;
}

/*************************************************************
 �� �� ����GetSocketErrorMsg()
 ���ܸ�Ҫ�����socket������Ϣ
 �� �� ֵ: void   
 ��    ����char * cError     ������Ϣ
***************************************************************/
//##ModelId=45190CD80212
void CNet::GetSocketErrorMsg(char * cError)
{
	if(cError == NULL)
		return;
    
#ifdef OS_WINDOWS
	int nErr = WSAGetLastError();
	
	switch(nErr)
	{
	case WSAEFAULT:
		sprintf(cError,"Bad address.IP=%s",m_OpponentAddr.IpAddr);
		break;
	case WSAENETDOWN:
		sprintf(cError,"Network down. IP=%s",m_OpponentAddr.IpAddr);
		break;
	case WSAENETUNREACH:
		sprintf(cError,"Network is unreachable. IP:%s",m_OpponentAddr.IpAddr);
		break;
	case WSAENETRESET:
		sprintf(cError,"Network dropped connection on reset. IP:%s",m_OpponentAddr.IpAddr);
		break;
	case WSAECONNRESET:
		sprintf(cError,"Connection reset by peer. IP:%s",m_OpponentAddr.IpAddr);
		break;
	case WSAETIMEDOUT:
		sprintf(cError,"Connection timed out. IP:%s",m_OpponentAddr.IpAddr);
		break;
	case WSAECONNREFUSED:
		sprintf(cError,"Connection refused. IP:%s",m_OpponentAddr.IpAddr);
		break;
	case WSAEHOSTDOWN:
		sprintf(cError,"Host is down. IP:%s", m_OpponentAddr.IpAddr);
		break;
	case WSAEHOSTUNREACH:
		sprintf(cError,"No route to host. IP:%s",m_OpponentAddr.IpAddr);
		break;
	default:
		sprintf(cError,"Socket Access Error Occured, ErrNum:%d,IP:%s", nErr, m_OpponentAddr.IpAddr);
		break;
	}
	m_LogFile.Add(cError,CLogFile::error);
#endif

	return ;
}

