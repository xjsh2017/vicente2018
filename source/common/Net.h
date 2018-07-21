//
// Net.h
//
// This is a TCP/IP socket  class
//
// On Windows the program that uses this utility
// should be linked with Ws2_32.lib
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef INCLUDED_SOCKETS_H
#define INCLUDED_SOCKETS_H

#include "define.h"
#include "Lock.h"
#include "LogFile.h"

/*************************************************************
 �� �� ����socketsInit()
 ���ܸ�Ҫ����ʼ��socket
           windows����Ҫ�ڳ�����ʹ��Ws2_32.dll
 �� �� ֵ���ɹ�true,ʧ��false 
 ��    ������
***************************************************************/
inline bool socketsInit()
{
#ifdef OS_WINDOWS

	WSADATA wsadata;

	//��ʼ����SOCKET2.0�ĵ���
	if (WSAStartup(MAKEWORD(2,0), &wsadata) == 0)
		return true;
	else
		return false;
#endif
	// Linux/Unix do not require any initialization
	return true;
}

/*************************************************************
 �� �� ����socketsEnd()
 ���ܸ�Ҫ��������Ws2_32.dll�ĵ���
 �� �� ֵ���� 
 ��    ������
 �޸���ʷ��
***************************************************************/
inline void socketsEnd()
{
#ifdef OS_WINDOWS

	//�����׽��ֵĵ���
	WSACleanup();
#endif
}


//ͨѶ�� (������)
class CCommon
{
private:

	//ͨѶ���� 0-���� 1-���� 2-���� 3-����
	int       m_nType;                           

protected:

	//���ͻ�����
    XJBUFFER    m_SendBuff;                     

	//���ջ�����
	XJBUFFER    m_RecvBuff;                           
	
	//������
	CLock     m_Lock;

	//��־��¼
	CLogFile  m_LogFile;

public:

	//���캯��
	CCommon(int nType=ID_NET/*Ĭ��Ϊ����*/);

    //��������
	~CCommon();

	//��ý��շ��ͻ�����
	void *      GetSendBuffer();

	void *      GetRecvBuffer();

	//������־�������
	void        SetLogLevel(int nLevel);  

	//�����־�������
	int         GetLogLevel();

	//������־·��
	void        SetLogPath(char *pPath);
	
	//��ʼ����־�ļ�
	BOOL        InitLogFile(char * pFileName);

//����Ϊ���麯��,����������

    //��ͨѶ��
    virtual bool open()=0;                                        

	//���ͱ��ط��ͻ�����ָ����Ŀ������(���������ݽṹ)
    virtual int  write(int nLen=MAXMSGLEN)=0;           
	
	//����ָ����������ָ�����ȵ�����
	virtual int  write(void * pBuf,int nLen) =0;
    
	//����ָ����Ŀ�����ݵ����ؽ��ջ�����(���������ݽṹ)
	virtual int  read(int nLen=MAXMSGLEN)=0;   
	
	//����ָ����Ŀ�����ݵ�ָ���Ļ�����
	virtual int  read(void * pBuf,int nLen) =0;
	
	//�ر�ͨѶ��
    virtual bool close()=0;                                       
    
	//����ͨѶ��ѡ��
    virtual bool SetOptions(int nFlag,int nTime,int Reserve=0)=0;               

	//��������������
    //virtual int  CopyTo(CCommon *target);                       
};

enum sockstate_type {INITIALIZED, BINDED, LISTENING, CONNECTED, CLOSED};

//����ͨѶ�� ���м̳�CCommon
//##ModelId=45190CD8015D
class CNet: public CCommon
{
private:
	//socket handler
    //##ModelId=45190CD801B7
	SOCKET			m_hSocket; 
	
	//�� AF_INET/PF_INET
    //##ModelId=45190CD801B8
	int				m_nDomain;  
	
	//socket������ TCP-SOCK_STREAM FTP-SOCK_DGRAM
    //##ModelId=45190CD801B9
	int				m_Type;      
	
    //Э������
    //##ModelId=45190CD801C1
	int				m_nProtocol;
	
    //���ؽ�ɫ��0--������, 1--�ͻ���, -1--��δ��ʼ��
    //##ModelId=45190CD801C2
	int				m_nLocalRole;	            

/* ------ Added by qingch   3/12/2009 ----- */	
    //���ܳ�ʱ ��λ��MS
    //##ModelId=45190CD801C1
	int				m_nRecvTimes;
	
	
    //���ͳ�ʱ ��λ��MS
    //##ModelId=45190CD801C1
	int				m_nSendTimes;
/* --------------------------------------- */
	
	//socket״̬
    //##ModelId=45190CD801C4
	sockstate_type	m_eState;                   

	//�Զ˵�ַ
    //##ModelId=45190CD801CB
	NETADDR         m_OpponentAddr;       
	
	//���ص�ַ
	NETADDR         m_LocalAddr;   

	//�鲥��ַ
	NETADDR         m_MultiCastAddr;   

protected:
	int			  write_udp(int nLen=MAXMSGLEN);
	int           write_udp(void *pBuf, int nLen);
	int			  read_udp(int nLen=MAXMSGLEN);
	int			  read_udp(void *pBuf, int nLen);

	//���socket���
    //##ModelId=45190CD80220
	SOCKET        GetHandler();

	//����socket���
    //##ModelId=45190CD80221
	void          SetHandler(SOCKET handler);

	//�ͻ��˹���(���ӷ�����)
    //##ModelId=45190CD801EF
	bool          ConnectServerB(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);
public:
//���캯��
    //##ModelId=45190CD801CC
	CNet(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

    //##ModelId=45190CD801D6
	CNet(const NETADDR* addr,int domain = AF_INET, \
		  int type = SOCK_STREAM, int protocol = 0);
	
//��������
    //##ModelId=45190CD801DB
	virtual ~CNet();

//����CCommon�麯��

	//����Socket
    //##ModelId=45190CD801DD
	bool          open();    
	bool          open(int flag); 

	 //�ر�socket
    //##ModelId=45190CD801DF
	bool          close();                                         
    
	//��������
    //##ModelId=45190CD801E0
	int           write(int nLen=MAXMSGLEN);     
	int           write(void * pBuf,int nLen);

	//��������
    //##ModelId=45190CD801E2
	int           read(int nLen=MAXMSGLEN);    
	int           read(void * pBuf,int nLen);
	
    //����ѡ��
    //##ModelId=45190CD801E6
	bool          SetOptions(int nFlag,int nTime,int Reserve=0);                   
    
//�����Ա����

	//��������������
    //##ModelId=45190CD801EC
	int           CopyTo(const CNet & net,int nFlag=2);                         

	//�ͻ��˹���(���ӷ�����), 
    //##ModelId=45190CD801EF
	bool          ConnectServer(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);

	//�������˹���(�󶨲���)
    //##ModelId=45190CD801F6
	bool          Bind (u_short port);                              

    //##ModelId=45190CD801F8
	bool          Bind (const char* host,u_short port);

	//��������,SERVER
    //##ModelId=45190CD801FF
	bool          Listen (int queue = SOMAXCONN);                   
	
	//��������,SERVER
    //##ModelId=45190CD80201
	bool          Accept(CNet &sock);  
	
	BOOL		  AddToMultiCast();
	
	//�������󶨡���������,SERVER
    //##ModelId=45190CD80203
	bool          ListenServer(u_short port,const char * host=NULL,\
		                       int nqueue=SOMAXCONN);

	//����socketѡ��
    //##ModelId=45190CD8020A
	bool SetSocketOption(int nOptionName,  \
		                 const char* lpOptionValue, \
		                 int nOptionLen, \
						 int nLevel);                      
	
	//ȡ��socket������Ϣ
    //##ModelId=45190CD80212
	void          GetSocketErrorMsg(char * cError);     
	
	//ȡ�ñ���IP��ַ
    //##ModelId=45190CD80214
	bool          GetLocalComputerIp(char *ipAddr);                 

	//���öԶ˵�ַ
    //##ModelId=45190CD80216
	void          SetOpponentAddr(const NETADDR * addr);    
	void		  SetLocalAddr(const NETADDR *addr);
	void		  SetMultiCastAddr(const NETADDR *addr);
	
	//��öԶ˵�ַ
    //##ModelId=45190CD8021B
	void          GetOpponentAddr(NETADDR * addr);    
	void          GetLocalAddr(NETADDR * addr); 
	void          GetMultiCastAddr(NETADDR * addr); 

	//��ý�ɫ
    //##ModelId=45190CD8021D
	int           GetLocalRole();  
	
	//���ý�ɫ
    //##ModelId=45190CD8021E
	void          SetLocalRole(int nRole);
 
	//���õ�ǰsocket״̬
    //##ModelId=45190CD80225
    void          SetState(sockstate_type state);

	//ȡ�õ�ǰsocket״̬
    //##ModelId=45190CD80227
	sockstate_type GetState() const { return m_eState; }
	
	int GetSocketType()const { return m_Type;}
};

#endif 
