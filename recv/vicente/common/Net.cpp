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
//                  Class  CCommon   实现                                    
//////////////////////////////////////////////////////////////////////////

CCommon::CCommon(int nType):m_nType(nType)
{
	//清空接收发送缓冲区
	memset(&m_SendBuff,0,sizeof(XJBUFFER));

	memset(&m_RecvBuff,0,sizeof(XJBUFFER));

	//初始化互坼体
	char ch[20]="";
	sprintf(ch,"CCommon(type=%d)",m_nType);
	m_Lock.init(ch);

	// 设置默认日志级别和路径
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
	//关闭文件
	m_LogFile.Close();
	//释放互坼体
    m_Lock.del();
}

/*************************************************************
 函 数 名：GetSendBuffer()
 功能概要：取得发送缓冲区指针
 返 回 值：void * 
 参    数：无
***************************************************************/
void *CCommon::GetSendBuffer()
{
	return (void*)&m_SendBuff;
}

/*************************************************************
 函 数 名：GetRecvBuffer()
 功能概要：取得接收缓冲区指针
 返 回 值：void * 
 参    数：无
***************************************************************/
void *CCommon::GetRecvBuffer()
{
	return (void*)&m_RecvBuff;
}

/*************************************************************
 函 数 名：SetLogLevel()
 功能概要：设置日志输出级别
 返 回 值：void 
 参    数：
           int nLevel :日志级别
***************************************************************/
void CCommon::SetLogLevel(int nLevel)
{
	m_LogFile.SetLogLevel(nLevel);
}

/*************************************************************
 函 数 名：GetLogLevel()
 功能概要：获得日志输出标志
 返 回 值：int
 参    数：无  
***************************************************************/
int CCommon::GetLogLevel()
{
	return m_LogFile.GetLogLevel();
}

/*************************************************************
 函 数 名：SetLogPath()
 功能概要：设置日志输出路径
 返 回 值：void 
 参    数：
           pPath :日志路径
***************************************************************/
void CCommon::SetLogPath(char * pPath)
{
	m_LogFile.SetLogPath(pPath);

	/* ------ Added by qingch   2/24/2009 ----- */
	//m_LogFile.Open("CNet.txt");//屏蔽写CNet日志              
	/* ---------------------------------------- */
}

/*************************************************************
 函 数 名：InitLogFile()
 功能概要：打开日志文件
 返 回 值：void 
 参    数：pFileName :文件名(不带路径)
***************************************************************/
BOOL CCommon::InitLogFile(char * pFileName)
{
	m_LogFile.Close();

	return ( m_LogFile.Open(pFileName) ); 
}

//////////////////////////////////////////////////////////////////////////
//                         Class   CNet 实现
//////////////////////////////////////////////////////////////////////////

/*************************************************************
 函 数 名：CNet()
 功能概要：构造函数(需初始化基类构造函数)
 返 回 值：无
 参    数：domain          域名         
		   type            socket类型 TCP/FTP
           protocol        协议类型 一般取0
***************************************************************/
//##ModelId=45190CD801CC
CNet::CNet(int domain, int type, int protocol):CCommon(ID_NET)
,m_nRecvTimes(60000),m_nSendTimes(60000)
{
	//初始化句柄
	m_hSocket	= INVALID_SOCKET;

	//初始化域
	m_nDomain   = domain;

	//初始化socket类型
	m_Type     = type;

	//协议类型
	m_nProtocol = protocol;
	
	//状态为关闭
	m_eState       = CLOSED;

	//角色未知
	m_nLocalRole = LOCAL_ACT_AS_UNKNOW;

	//对端地址
	memset(&m_OpponentAddr,0,sizeof(NETADDR));
}

/*************************************************************
 函 数 名：CNet()
 功能概要：构造函数(需初始化基类构造函数)
 返 回 值：无
 参    数：add             指向对端NET地址的常指针          
		   domain          域名         
		   type            socket类型 TCP/FTP
           protocol        协议类型 一般取0
***************************************************************/
//##ModelId=45190CD801D6
CNet::CNet(const NETADDR* addr,int domain, 
		   int type, int protocol ):CCommon(ID_NET)
{
	//初始化句柄
	m_hSocket	= INVALID_SOCKET;

	//初始化域
	m_nDomain   = domain;

	//初始化socket类型
	m_Type     = type;

	//协议类型
	m_nProtocol = protocol;
	
	//状态为关闭
	m_eState       = CLOSED;

	//角色未知
	m_nLocalRole = LOCAL_ACT_AS_UNKNOW;

	//对端地址
	memcpy(&m_OpponentAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 函 数 名：~CNet()
 功能概要：析构函数,释放对象空间
 返 回 值：无
 参    数：无
***************************************************************/
//##ModelId=45190CD801DB
CNet::~CNet()
{
	close();
}

/*************************************************************
 函 数 名：open()
 功能概要：创建socket并初始化状态
 返 回 值: BOOL  TRUE:创建成功 FALSE:创建失败
 参    数：无
***************************************************************/
//##ModelId=45190CD801DD
bool CNet::open()
{
	if((m_eState == INITIALIZED) ||
       (m_eState == BINDED)      ||
	   (m_eState == LISTENING)   || 
	   (m_eState == CONNECTED) )

		return true;
	
	//创建套接字
	m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  

	if(m_hSocket == INVALID_SOCKET)
	{
		m_LogFile.FormatAdd(CLogFile::error,"::socket() error,reason:%s(%d)",\
			                strerror(errno),errno);
		
		return false;
	}
	
	//创建成功置初始状态为INITIALIZED,否则置为CLOSED
	m_eState = ( m_hSocket != INVALID_SOCKET ) ? INITIALIZED : CLOSED;  

	if ( m_eState == INITIALIZED )
		return true;
	else
		return false;
}

/*************************************************************
 函 数 名：open()
 功能概要：创建socket并初始化状态
 返 回 值: BOOL  TRUE:创建成功 FALSE:创建失败
 参    数：
		   flag	通讯标志位(采集器专用)
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
		//UDP组播
		//创建套接字，同时设置一些属性
		m_hSocket = WSASocket(m_nDomain, m_Type, m_nProtocol, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);
	}
	else
	{
		//创建套接字
		m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  
	}
#endif

#ifdef OS_LINUX
	//创建套接字
	m_hSocket = ::socket(m_nDomain, m_Type, m_nProtocol);  
#endif

	if(m_hSocket == INVALID_SOCKET)
	{
		m_LogFile.FormatAdd(CLogFile::error,"::socket() error,reason:%s(%d)",\
			                strerror(errno),errno);
		
		return false;
	}
	
	//创建成功置初始状态为INITIALIZED,否则置为CLOSED
	m_eState = ( m_hSocket != INVALID_SOCKET ) ? INITIALIZED : CLOSED;  

	if ( m_eState == INITIALIZED )
		return true;
	else
		return false;
}

/*************************************************************
 函 数 名：close()
 功能概要：关闭socket
 返 回 值: BOOL  
           TRUE:成功 FALSE:失败
 参    数：无
***************************************************************/
//##ModelId=45190CD801DF
bool CNet::close()
{
	if (m_eState != CLOSED)
	{
		try
		{
// 			if(m_nLocalRole == LOCAL_ACT_AS_SERVER) //服务器被动关闭，消除TIME_WAIT状态
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
// 					// linux下 用ioctl清空输入输出缓冲区
// 					// 0-清空输入 1-清空输出 2-清空全部
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
			//复位标志
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
 函 数 名：GetLocalRole()
 功能概要：获得当前socket的角色
 返 回 值: int  LOCAL_ACT_AS_UNKNOW:未知 
                LOCAL_ACT_AS_SERVER:服务器
                LOCAL_ACT_AS_CLIENT:客户端
 参    数：无
***************************************************************/
//##ModelId=45190CD8021D
int CNet::GetLocalRole()
{
	return m_nLocalRole;
}

/*************************************************************
 函 数 名：SetLocalRole()
 功能概要：获设置当前socket的角色
 返 回 值: void  
 参    数：     LOCAL_ACT_AS_UNKNOW: -1 
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
 函 数 名：SetHandler()
 功能概要：重新设置当前对象的socket句柄
 返 回 值: void  
 参    数：handler  socket句柄
***************************************************************/
//##ModelId=45190CD80221
void CNet::SetHandler(SOCKET handler)
{
	close();

	m_hSocket=handler;
}

/*************************************************************
 函 数 名：GetHandler()
 功能概要：获得当前对象的socket句柄
 返 回 值: handler  socket句柄  
 参    数：void 
***************************************************************/
//##ModelId=45190CD80220
SOCKET CNet::GetHandler()
{
	return m_hSocket;
}

/*************************************************************
 函 数 名：SetState()
 功能概要：设置当前socket的状态
 返 回 值: void 
 参    数：enum sockstate_type 
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
 函 数 名：SetOpponentAddr()
 功能概要：设置通讯对端地址
 返 回 值: void 
 参    数：NETADDR *addr  网络地址结构(IP+port)
***************************************************************/
//##ModelId=45190CD80216
void CNet::SetOpponentAddr(const NETADDR *addr)
{
	memcpy(&m_OpponentAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 函 数 名：SetLocalAddr()
 功能概要：设置通讯本地地址
 返 回 值: void 
 参    数：NETADDR *addr  网络地址结构(IP+port)
***************************************************************/
void CNet::SetLocalAddr(const NETADDR *addr)
{
	memcpy(&m_LocalAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 函 数 名：SetMultiCastAddr()
 功能概要：设置通讯多播地址
 返 回 值: void 
 参    数：NETADDR *addr  网络地址结构(IP+port)
***************************************************************/
void CNet::SetMultiCastAddr(const NETADDR *addr)
{
	memcpy(&m_MultiCastAddr,addr,sizeof(NETADDR));
}

/*************************************************************
 函 数 名：GetOpponentAddr()
 功能概要：获得通讯对端地址
 返 回 值: void 
 参    数：NETADDR *addr  保存网络地址结构(IP+port)
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
 函 数 名：GetLocalAddr()
 功能概要：获得通讯本地地址
 返 回 值: void 
 参    数：NETADDR *addr  保存网络地址结构(IP+port)
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
 函 数 名：GetMultiCastAddr()
 功能概要：获得通讯多播地址
 返 回 值: void 
 参    数：NETADDR *addr  保存网络地址结构(IP+port)
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
 函 数 名：Bind()
 功能概要：绑定端口
 返 回 值: BOOL  TRUE:成功 FALSE:失败
 参    数：port  指定要绑定的端口号
***************************************************************/
//##ModelId=45190CD801F6
bool CNet::Bind(u_short port)
{
	//错误码
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

	//邦定成功
	m_eState = BINDED;

	return true;
}

/*************************************************************
 函 数 名：Bind()
 功能概要：绑定指定网卡的端口
 返 回 值: BOOL  TRUE:成功 FALSE:失败
 参    数：host  网卡或主机指针
	       port  要绑定的端口
***************************************************************/
//##ModelId=45190CD801F8
bool CNet::Bind(const char* host,u_short port)
{
	int eCode = 0;      //错误码

	if (m_eState != INITIALIZED)
	{		
		m_LogFile.Add("bind in a invalid socket,failed",CLogFile::error);

		return false;		
	}

	//绑定地址结构设置
	struct sockaddr_in servaddr;

	memset ( &servaddr, 0, sizeof ( servaddr ) );

	servaddr.sin_family = m_nDomain;

	servaddr.sin_port = htons ( port );

	if(strlen(host)==0)    
	{
		//输入为空时,绑定所有IP
		servaddr.sin_addr.s_addr =inet_addr("0.0.0.0")/* htonl ( INADDR_ANY )*/;
	}
	else
	{
    	if ( isdigit ( host[ 0 ] ) )
		{
			//IP地址直接转换
			servaddr.sin_addr.s_addr = inet_addr(host);
		}    
		else
		{
			//主机名需要先转换为IP
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

	//开始绑定
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

	//绑定标志置位
	m_eState = BINDED;

	return true;
}

/*************************************************************
 函 数 名：Listen()
 功能概要：设置socket为监听状态
 返 回 值: BOOL  
           TRUE:成功 FALSE:失败
 参    数：
           int queue: 指定该socket同时能接收的最大连接数
***************************************************************/
//##ModelId=45190CD801FF
bool CNet::Listen(int queue)
{
	//保证是已经邦定的套接字上进行监听操作
	if ( m_eState != BINDED )
	{
		m_LogFile.Add("listen in a socket which isn't in binding state",CLogFile::error);
		return false;
	}

	if (::listen(m_hSocket, queue) == SOCKET_ERROR)
	{
		//监听失败
        m_LogFile.FormatAdd(CLogFile::error,"::listen() failed,reason:%s(%d)!",\
			               strerror(errno),errno);
		return false;
	}

	//监听标志置位
	m_eState = LISTENING;

	return true;
}

/*************************************************************
 函 数 名：Accept()
 功能概要：开始socket监听
 返 回 值: BOOL  
           TRUE:成功 FALSE:失败
 参    数：
           sock  保存监听到的Client CNet对象
***************************************************************/
//##ModelId=45190CD80201
bool CNet::Accept(CNet & sock)
{
	int eCode = 0;

	if ( m_eState != LISTENING )
	{
		//如果该Socket不处于监听状态就直接返回

       	m_LogFile.Add("accept in a socket which isn't in listening state",\
			         CLogFile::error);
		return false;
	}

	//用于接受连接方的地址信息
	struct sockaddr_in clientaddr;         

	SOCKET	handler;

	//windows 中 sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof(clientaddr); 

	handler = ::accept (m_hSocket, (struct sockaddr*)&clientaddr, &clientlen);

	if( handler == INVALID_SOCKET )
	{
		m_LogFile.FormatAdd(CLogFile::error,"accept error,reason:%s(%d)", \
			                strerror(errno),errno);
		return false;
	}

	//取得监听到的客户端IP/port
	sprintf(m_OpponentAddr.IpAddr, "%s", inet_ntoa(clientaddr.sin_addr));

	m_OpponentAddr.nPort=ntohs(clientaddr.sin_port);

    //设置sock属性
	sock.SetOpponentAddr(&m_OpponentAddr);

	sock.SetHandler(handler);

	sock.SetLocalRole(LOCAL_ACT_AS_SERVER);

	sock.SetState(CONNECTED);

	return true;			
}

/*************************************************************
 函 数 名：AddToMultiCast()
 功能概要：设置组播
 返 回 值: BOOL  
           TRUE:成功 FALSE:失败
 参    数：无
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

	/* 禁止loopback */  
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
		
	/* 加入multicast 组*/  	if (WSAJoinLeaf(m_hSocket, 
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
 函 数 名：read()
 功能概要：从socket读取指定字节的数据
           (不关心数据内容,收到数据放入自己的接收缓冲区中)
 返 回 值: 
           int  返回实际读到的字节数 (<=nLen)
 参    数：
           nLen 指定读取字节数
***************************************************************/
//##ModelId=45190CD801E2
int CNet::read(int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return read_udp(nLen);
	}

	//保证在连接的状态下进行接收
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

	//临时变量
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
			if(errno == EAGAIN/*11*/)  //超时
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

    //接收缓冲区
	XJBUFFER * pBuf=(XJBUFFER*)GetRecvBuffer();
    
	//互斥体保护
	if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//清空
	memset(pBuf,0,sizeof(XJBUFFER));

	//设置读到的字节数
	if(nRet >= 0)
	{
		pBuf->len=nRet;
		memcpy(pBuf->buf,rcvBuf,nRet);
	}
	
	//退出互斥体
    m_Lock.unlock();

	return nRet;
}

/*************************************************************
 函 数 名：read()
 功能概要：从socket读取指定字节的数据到指定缓冲区中
           
 返 回 值: 
           int  返回实际读到的字节数 (<=nLen)
 参    数：
           nLen 指定读取字节数
		   pBuf 存放接收数据的缓冲区
***************************************************************/
int CNet::read(void * pBuf,int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return read_udp(pBuf, nLen);
	}

	//保证在连接的状态下进行接收
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
			if(errno == EAGAIN/*11*/)  //超时
				return TIMEOUT;
			
			m_LogFile.FormatAdd(CLogFile::error,"::recv failed,reason:%s(%d)", \
				strerror(errno),errno);
#endif
			
			return SOCKET_ERROR;
		}

		 // 连接关闭
		if(readdone == 0)
			break;

		readleft -= readdone;
		pchar    += readdone;
		nRet     += readdone;
	}

	return nRet;
}

/*************************************************************
 函 数 名：read_udp()
 功能概要：从socket读取指定字节的数据
           (不关心数据内容,收到数据放入自己的接收缓冲区中)
 返 回 值: 
           int  返回实际读到的字节数 (<=nLen)
 参    数：
           nLen 指定读取字节数
***************************************************************/
int CNet::read_udp(int nLen)
{
	//保证在连接的状态下进行接收
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if (nLen <= 0)
	{
		return 0;
	}

	//临时变量
	char      rcvBuf[MAXMSGLEN+1]="";
	char      *pchar = rcvBuf;
	int		readdone=0;
	struct	sockaddr_in clientaddr;

#ifdef OS_LINUX
	SOCK_LEN addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, pchar, nLen, 0, (struct sockaddr *)&clientaddr, (SOCK_LEN *)&addrlen))
			== SOCKET_ERROR)
	{
		if(errno == EAGAIN)  //超时
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

    //接收缓冲区
	XJBUFFER * pBuf=(XJBUFFER*)GetRecvBuffer();
    
	//互斥体保护
	if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//清空
	memset(pBuf,0,sizeof(XJBUFFER));
	pBuf->len=readdone;
	memcpy(pBuf->buf,rcvBuf,readdone);
	
	//退出互斥体
    m_Lock.unlock();

	memset(&m_OpponentAddr, 0, sizeof(NETADDR));
	strcpy(m_OpponentAddr.IpAddr, inet_ntoa(clientaddr.sin_addr));
	m_OpponentAddr.nPort=ntohs(clientaddr.sin_port);

	return readdone;
}

/*************************************************************
 函 数 名：read_udp_from_addr()
 功能概要：从socket读取指定字节的数据到指定缓冲区中，并返回对方
			地址
           
 返 回 值: 
           int  返回实际读到的字节数 (<=nLen)
 参    数：
           nLen 指定读取字节数
		   pBuf 存放接收数据的缓冲区
		   sock_addr	对方地址缓冲区
***************************************************************/
int CNet::read_udp(void *pbuff, int nLen)
{
	//保证在连接的状态下进行接收
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("read in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if (nLen <= 0)
	{
		return 0;
	}

	//临时变量
	int readdone=0;
	struct	sockaddr_in clientaddr;

#ifdef OS_LINUX
	SOCK_LEN addrlen = sizeof(clientaddr);

	if((readdone = recvfrom(m_hSocket, pbuff, nLen, 0, (struct sockaddr *)&clientaddr, (SOCK_LEN *)&addrlen))
			== SOCKET_ERROR)
	{
		if(errno == EAGAIN/*11*/)  //超时
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
 函 数 名：write()
 功能概要：从socket发送指定字节的数据
           (发送自己的发送缓冲区中的数据)
 返 回 值: 
           int  返回实际发出的字节数 (<=nLen)
 参    数：
           nLen 指定发送字节数
***************************************************************/
//##ModelId=45190CD801E0
int CNet::write(int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return write_udp(nLen);
	}

	//保证在连接的状态下进行接收
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if(nLen < 0)
		return SOCKET_ERROR;

	//互斥体保护
    if( !m_Lock.lock() )
		return SOCKET_ERROR;

	//发送缓冲区
	XJBUFFER * pBuf=(XJBUFFER*)GetSendBuffer();

	//实际发送数取BUFFER中的size 与nLen最小值
	int size= ( nLen > pBuf->len ) ? (pBuf->len) : nLen;

	//临时变量
	char sndBuf[MAXMSGLEN+1]="";
	memcpy(sndBuf,pBuf->buf,size);

	//互斥体释放
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
            if(errno == EAGAIN/*11*/)  //超时
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
 函 数 名：write()
 功能概要：从指定的缓冲区中发送指定字节的数据
 返 回 值: 
           int  返回实际发出的字节数 (<=nLen)
 参    数：
           nLen 指定发送字节数
           pBuf 指定的发送缓冲区指针
***************************************************************/
//##ModelId=45190CD801E0
int CNet::write(void * pBuf,int nLen)
{
	if (m_Type == SOCK_DGRAM)
	{
		return write_udp(pBuf, nLen);
	}

	//保证在连接的状态下进行接收
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	//判断参数有效性
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
            if(errno == EAGAIN/*11*/)  //超时
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
 函 数 名：write_udp()
 功能概要：从socket发送指定字节的数据
           (发送自己的发送缓冲区中的数据)
 返 回 值: 
           int  返回实际发出的字节数 (<=nLen)
 参    数：
           nLen 指定发送字节数
***************************************************************/
int CNet::write_udp(int nLen)
{
	//保证在连接的状态下进行接收
	if (m_eState != CONNECTED)   
	{
        m_LogFile.Add("send in a none connected socket",CLogFile::error);
		return SOCKET_ERROR;
	}

	if(nLen < 0)
		return 0;

	//互斥体保护
    if( !m_Lock.lock() )
		return 0;

	//发送缓冲区
	XJBUFFER * pBuf=(XJBUFFER*)GetSendBuffer();

	//实际发送数取BUFFER中的size 与nLen最小值
	int size= ( nLen > pBuf->len ) ? (pBuf->len) : nLen;

	//临时变量
	char sndBuf[MAXMSGLEN+1]="";
	memcpy(sndBuf,pBuf->buf,size);

	//互斥体释放
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
 函 数 名：write()
 功能概要：从指定的缓冲区中发送指定字节的数据到指定的地址
 返 回 值: 
           int  返回实际发出的字节数 (<=nLen)
 参    数：
           nLen 指定发送字节数
           pBuf 指定的发送缓冲区指针
		   sock_addr	对方地址缓冲区
***************************************************************/
int CNet::write_udp(void *pBuf, int nLen)
{
	//保证在连接的状态下进行接收
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
 函 数 名：GetLocalComputerIp()
 功能概要：获得本机IP
 返 回 值: bool 成功或失败
 参    数：
           char * ip  存放本机IP
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
	strcpy(ifr.ifr_name,"eth0");                   // eth0 第1块网卡IP ; lo 本地回环地址127.0.0.1

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
 函 数 名：SetSocketOption()
 功能概要：设置socket各选项(直接对socket各选项操作,需要清楚
                   socket的各个选项名称)
           用select设置超时,即使对方连接已关闭本地还仍然阻塞等待
		           直到超时
 返 回 值: bool   TRUE:成功  FALSE:失败
 参    数：int nOptionName   要设置的选项名称
           lpOptionValue     选项值
           nOptionLen        选项值的长度
           nLevel            选项级别
***************************************************************/
//##ModelId=45190CD8020A
bool CNet::SetSocketOption(int nOptionName, 
						   const char *lpOptionValue, 
						   int nOptionLen,
						   int nLevel)
{
	//返回值
	bool bReturn  = false;    

	//日志记录
	char cLog[255]="";

	fd_set	set;
	SOCKET	FDData = m_hSocket;
	struct timeval timeout;
	int ms=0;
	
	//根据nOptionName的不同选项进行设置
	switch(nOptionName) 
	{
	case SO_SNDBUF:   //设置发送缓冲区
		if(setsockopt(m_hSocket, nLevel, nOptionName, lpOptionValue, nOptionLen) == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket SO_SNDBUF failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
		bReturn = true;			
		break;

	case SO_RCVBUF:   //设置接收缓冲区
		if(setsockopt(m_hSocket, nLevel, nOptionName, lpOptionValue, nOptionLen) == SOCKET_ERROR)
		{
			sprintf(cLog,"set socket SO_RCVBUF failed,reason:%s(%d)", \
				   strerror(errno),errno);
			break;
		}
		bReturn = true;			
		break;

	case SO_SNDTIMEO:   //设置发送超时
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
	case SO_RCVTIMEO:   //设置接收超时
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
	default:   //别的设置不予处理		
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
 函 数 名：SetOptions()
 功能概要：设置socket各选项(无需清楚socket的各选项名称)
 返 回 值: bool   
           TRUE:成功  FALSE:失败
 参    数：int nFlag         要设置的选项名称
           int nTime         选项值,设置超时时为超时时间(ms)
		                     其它选项时清0
           int nReserve      保留
***************************************************************/
//##ModelId=45190CD801E6
bool CNet::SetOptions(int nFlag,int nTime,int nReserve)
{
	//日志
	char cLog[1024]="";

	//返回值
	bool bReturn = false;

	//若为无效socket则返回
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

	SOCK_LEN nActualTime=nTime;      //单位为毫秒
	unsigned long ul = 0;            //临时变量
	int status = 0;                  //临时变量
	fd_set r;                        //用于存放socket
   	int ret=0;                       //返回值

	//用超时设置,linux下超时设置
	struct timeval timeout ;       
	timeout.tv_sec = nTime/1000;     //连接超时时间
	timeout.tv_usec =(nTime%1000)*1000; //微秒

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
		  经验证该函数在设置的时候会阻塞指定时间，而实际接受超时没有生效
		  现改成在接收的时候调用select实现
		*/
#else
   #ifdef __SOPOLL_TIMEOUT
        /* 接受时再设置 */
   #else
		ret = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(void*)&timeout,sizeof(timeval)); 
        //可以用select设置超时,但是select设置超时时即使对方连接已关闭本地还仍然阻塞等待直到超时
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
		  经验证该函数在设置的时候会阻塞指定时间，而实际发送超时没有生效
		  现改成在发送的时候调用select实现
		*/
#else

   #ifdef __SOPOLL_TIMEOUT
        /* 发送时再设置 */
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
		//linux下
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
		//linux下
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
 函 数 名：ConnectServer()
 功能概要：连接服务器
 返 回 值: bool   TRUE:成功  FALSE:失败
 参    数：char * host       服务器端IP或主机名 (默认为NULL)
                              当IP=NULL时,取m_OpponentAddr成员
           int   port        服务器端port (默认为 -1)
		                      当port<0时,取m_OpponentAddr成员
           int  nLocalPort   连接前指定本地端口 (默认为-1)
		                       不指定时随机取一个端口
           int  nTimeout     连接超时参数, 缺省情况为系统超时时间(单位为s),如果配置该参数必须使用
							 SetOptions 将socket设置为UNBLOCKING.
***************************************************************/
//##ModelId=45190CD801EF
bool CNet::ConnectServer(const char * host,int port,int nLocalPort,int nTimeout)
{

	if (nTimeout > 0)
	{
		return ConnectServerB(host, port, nLocalPort,nTimeout);
	}

	//如果还没有初始化则先open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			return false;
		}
	}

	//如果已经在连接状态则先关闭
	if(m_eState == CONNECTED)
	{
		if((host == NULL)&&(port ==-1))
		{
			//没有传参数则认为是同一个连接,直接返回
			return true;
		}
		else
		{
			//不同的连接目标需要先关闭,重新初始化
			close();
			
			if(!open())
			{
				return false;
			}
		}
	}
    
	//先判断host/port是否有效
	if( ( host != NULL ) && ( port > 0 ) )
	{
		unsigned int nlen = (strlen(host) > 40) ? 40 : strlen(host);
		memset(&m_OpponentAddr,0,sizeof(NETADDR));
		//先修改成员变量中对端地址
		memcpy(m_OpponentAddr.IpAddr,host,nlen);

		m_OpponentAddr.nPort = (u_short)port;
	}

	//判断连接前是否需要指定端口
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

	//构造地址域结构
	struct sockaddr_in servaddr;
	memset (&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = m_nDomain;
	servaddr.sin_port = htons (m_OpponentAddr.nPort);
	
	//判断host为IP or hostname
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

	//windows 中 sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof (servaddr); 

	//开始连接		
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
 函 数 名：ConnectServerB()
 功能概要：连接服务器
 返 回 值: bool   TRUE:成功  FALSE:失败
 参    数：char * host       服务器端IP或主机名 (默认为NULL)
                              当IP=NULL时,取m_OpponentAddr成员
           int   port        服务器端port (默认为 -1)
		                      当port<0时,取m_OpponentAddr成员
           int  nLocalPort   连接前指定本地端口 (默认为-1)
		                       不指定时随机取一个端口
           int  nTimeout     连接超时参数
***************************************************************/
//##ModelId=45190CD801EF
bool CNet::ConnectServerB(const char * host,int port,int nLocalPort,int nTimeout)
{
	//如果还没有初始化则先open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			return false;
		}
	}

	//如果已经在连接状态则先关闭
	if(m_eState == CONNECTED)
	{
		if((host == NULL)&&(port ==-1))
		{
			//没有传参数则认为是同一个连接,直接返回
			return true;
		}
		else
		{
			//不同的连接目标需要先关闭,重新初始化
			close();
			
			if(!open())
			{
				return false;
			}
		}
	}
    
	//先判断host/port是否有效
	if( ( host != NULL ) && ( port > 0 ) )
	{
		unsigned int nlen = (strlen(host) > 40) ? 40 : strlen(host);

		//先修改成员变量中对端地址
		memcpy(m_OpponentAddr.IpAddr,host,nlen);

		m_OpponentAddr.nPort = (u_short)port;
	}

	//判断连接前是否需要指定端口
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

	//构造地址域结构
	struct sockaddr_in servaddr;

	memset (&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = m_nDomain;
	servaddr.sin_port = htons(m_OpponentAddr.nPort);
	
	//判断host为IP or hostname
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

	//windows 中 sizeof reutn size_t type; and size_t be defined 'unsigned int'
	SOCK_LEN clientlen = sizeof (servaddr); 
    int nRet = 0;

	//开始连接		
	if(::connect(m_hSocket, (struct sockaddr*)&servaddr, clientlen) == SOCKET_ERROR)
	{
		//对于非阻塞套接字，connect()始终返回SOCKET_ERROR，需要进一步判断连接是否建立。
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

		tv.tv_sec = nTimeout;	//设置超时为0，仅仅为了判断是否有数据可以读或异常。
		tv.tv_usec = 0; 

	    nRet = select(m_hSocket+1, NULL, &write_set, NULL, &tv);	//状态检测
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
 函 数 名：ListenServer()
 功能概要：完成一个socket的创建、绑定、监听操作
 返 回 值: bool  
           TRUE:成功  FALSE:失败
 参    数：const char * host  本地监听的IP/主机名,
                              host==NULL时,监听本机所有IP
           u_short port       监听端口 
		   nqueue             最大能接收的连接数
***************************************************************/
//##ModelId=45190CD80203
bool CNet::ListenServer(u_short port ,const char * host,int nqueue)
{
	//如果还没有初始化则先open
	if(m_eState == CLOSED)
	{
	    if(!open())
		{
			m_LogFile.Add("open () failed when ListenServer()",CLogFile::error);
			return false;
		}
	}

	//如果已经在连接状态则先关闭
	if(m_eState == CONNECTED)
	{
		close();
		if(!open())
		{
			m_LogFile.Add("open () failed when ListenServer()",CLogFile::error);
			return false;
		}
	}

	//如果已经open,则bind
	if(m_eState == INITIALIZED)
	{
		//如果host为null,则绑定本机上所有port
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
		else   //指定IP进行绑定
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

    //如果已经在绑定状态,则监听
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

	//全部成功则返回true
	return true;
}

/*************************************************************
 函 数 名：CopyTo()
 功能概要：拷贝源对象指定缓冲区数据到目标Net对象的发送缓冲区
 返 回 值: 
           int   实际拷贝字节数   失败时返回-1
 参    数：const CNet & net  目标对象
           int   nFlag       拷贝标志 默认取2
		                     1-拷贝源对象的发送buf 
							 2-拷贝源对象的接收buf
***************************************************************/
//##ModelId=45190CD801EC
int CNet::CopyTo(const CNet & net,int nFlag)
{
	//返回值
	int size=0;

	//如果是本身不拷贝
	if(this == &net)
	{
		m_LogFile.Add("the first parameter is owner-self in copyto(),error",CLogFile::error);
		return -1;
	}

	if(nFlag == 0)
		return -1;
//互斥体保护
    if( !m_Lock.lock())
		return -1;

	CCommon * pCommon=(CCommon*)&net;

	XJBUFFER * pDisSendBuf=(XJBUFFER*)pCommon->GetSendBuffer();
	XJBUFFER * pDisRecvBuf=(XJBUFFER*)pCommon->GetRecvBuffer();

	if(nFlag == 0)
		return -1;
	else if(nFlag == 1)
	{
		//发送缓冲区
		memcpy(pDisSendBuf,(XJBUFFER*)GetSendBuffer(),sizeof(XJBUFFER));
	    
		size+=((XJBUFFER*)GetSendBuffer())->len;
	}
	else if(nFlag == 2)
	{
		//接收缓冲区
		memcpy(pDisSendBuf,(XJBUFFER*)GetRecvBuffer(),sizeof(XJBUFFER));
	    
		size+=((XJBUFFER*)GetRecvBuffer())->len;
	}
	else
	{
		//互斥体释放
        m_Lock.unlock();

		m_LogFile.FormatAdd(CLogFile::error,"the second parameter in copyto() is \
			                %d ,invalid",nFlag);
		return -1;
	}

    //互斥体释放
    m_Lock.unlock();

	m_LogFile.FormatAdd(CLogFile::trace,"copyto() %d size success,Flag is %d",\
		               size,nFlag);
	return size;
}

/*************************************************************
 函 数 名：GetSocketErrorMsg()
 功能概要：获得socket出错信息
 返 回 值: void   
 参    数：char * cError     错误信息
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

