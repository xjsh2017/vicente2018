/********************************************************************
	created:	2008/09/10
	created:	10:9:2008   23:15
	file base:	STTPStream
	file ext:	cpp
	author:		qingch
	
	purpose:	mplementation of the CSTTPStream class.
*********************************************************************/


#include "STTPStream.h"



/*************************************************************
 �� �� ��:   CSTTPStream
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
 ��    ��:   const char * pSzHost �ͻ���
 ��    ��:   bool pLogFlag �Ƿ񱣴汨��
**************************************************************/
CSTTPStream::CSTTPStream(const char* pSzHost,bool pLogFlag,const char* pSzLogPath/*="../Xj_Log/"*/)
:CXJNet_Stream(pSzLogPath,CLogFile::trace),m_bLog(pLogFlag),m_RecvLock("STTPStream"),m_SendLock("STTPStream"),
m_LogLock("STTPStream")
{
	m_LogFile.SetLogLevel(CLogFile::trace);
	/*
	* ----- commented by qingch   8/30/2009 ------
  	if (pSzLogPath == NULL){
  		m_LogFile.SetLogPath("../Xj_Log/");
  	}
  	else{
	*/
		m_LogFile.SetLogPath(pSzLogPath);
	/*
	* ----- commented by qingch   8/30/2009 ------
  	}
	*/

	if (pSzHost != NULL){
		m_LogFile.SetHostName(pSzHost);
	}else{
		m_LogFile.SetHostName("STTPStream");
	}
}

/*************************************************************
 �� �� ��:   ~CSTTPStream
 ���ܸ�Ҫ:	 ��������
 �� �� ֵ:   
**************************************************************/
CSTTPStream::~CSTTPStream()
{

	m_LogFile.Close();

}

/*************************************************************
 �� �� ��:   SendSTTPMessage
 ���ܸ�Ҫ:   ����һ��STTP����
 �� �� ֵ:   int >0-�ɹ����͵�ʵ�ʳ��� <=0-����ʧ�� 
 ��    ��:   const STTPMSG * pMsg  �����͵�STTP����
**************************************************************/
int CSTTPStream::SendSTTPMessage( const STTPMSG* pMsg )
{
	if( pMsg == NULL)
		return 0;
	
	// set log head
	LOG_BUFFER_HEAD log_head;
	bzero(&log_head,sizeof(LOG_BUFFER_HEAD));
	
	log_head.nWay = MSG_SEND;
	GetRemoteAddr(&log_head.addr);
	
	CSttpMsgParser tempParser(const_cast<STTPMSG*>(pMsg));	
	int iMsgLength = tempParser.GetMsgLength() 	;
	iMsgLength += sizeof(STTPMSGHDR); 
	
	if(iMsgLength <= 0){
		char strLog[255] = "";
		sprintf(strLog,"the len of sttp which send to %s is %d,msgid=%d,error",
			log_head.addr.IpAddr,iMsgLength,tempParser.GetMsgID());
		WriteErrorLog(strLog);
		return 0;
	}
	
	// begin send
	// need lock m_pNet because socket is unsafe in multi thread 

	CLockUp lockup(&m_SendLock);

	int nRet(0);
	try {
		nRet = Write((char*)pMsg,iMsgLength);
	}
	catch(...){
		WriteErrorLog("sttpstream Write exception");
		nRet = R_RUN_EXCEPTION;
	}
	
	if (nRet > 0){
		PrintSttpMsg(pMsg,log_head);
	}

	return nRet;
}


/*************************************************************
 �� �� ��:   RecvSTTPMessage
 ���ܸ�Ҫ:   ����һ��STTP����
 �� �� ֵ:   int >0-�ɹ����յ�ʵ�ʳ��� <=0-����ʧ��
 ��    ��:   STTPMSG * pMsg �����յ���STTP����
**************************************************************/
int CSTTPStream::RecvSTTPMessage(STTPMSG* pMsg)
{
	if( pMsg == NULL)
		return 0;
	
	// set log head
	LOG_BUFFER_HEAD log_head;
	bzero(&log_head,sizeof(LOG_BUFFER_HEAD));
	
	log_head.nWay = MSG_RECV;
	GetRemoteAddr(&log_head.addr);
	
	// begin read sttp head
	// need lock m_pNet because socket is unsafe in multi thread 
	CLockUp lockup(&m_RecvLock);
	int nRet(0);
	try{ 
		nRet = Read((char*)&pMsg->MsgHdr,sizeof(STTPMSGHDR));
	}
	catch(...){
		WriteErrorLog("sttpstream read exception");
		nRet = R_RUN_EXCEPTION;
	}
	
	if(nRet <= 0){
		return nRet;
	}

	if (nRet < sizeof(STTPMSGHDR))
	{
		return 0;
	}

	CSttpMsgParser tempParser(pMsg);	
	int iMsgLength = tempParser.GetMsgLength(); 
	// assert the length is valid
	if( (iMsgLength < 0) ||
		(iMsgLength > MAX_STTP_MSG_LEN) ){
		char strLog[255] = "";
		sprintf(strLog,"the len of sttp which recv from %s is %d,error",
			log_head.addr.IpAddr,iMsgLength);
		WriteErrorLog(strLog);
		return 0;
	}
    
	if(iMsgLength == 0){
		PrintSttpMsg(pMsg,log_head);
		return nRet;
	}
	
	// continue read by sttp length
    int nRet2(0); 
	try{
		nRet2= Read(pMsg->MsgBody,iMsgLength);
	}
	catch(...){
		WriteErrorLog("sttpstream read exception");
		nRet = R_RUN_EXCEPTION;
	}

	PrintSttpMsg(pMsg,log_head);

	if(nRet2 <= 0){
		return nRet;
	}
	
	if (nRet2 < iMsgLength)
	{
		char cError[255] = "";
		sprintf(cError,"������ʵ�ʳ��ȣ�%d,Ԥ�ڵĳ��ȣ�%d",nRet2,iMsgLength);
		WriteErrorLog(cError);
		return 0;
	}

	return (nRet+nRet2);
}

/*************************************************************
 �� �� ��:   PrintSttpMsg
 ���ܸ�Ҫ:   ������ĵ���־�ļ�
 �� �� ֵ:   void
 ��    ��:   const STTPMSG * pMsg ������ı���
 ��    ��:   const LOG_BUFFER_HEAD & pHead �������־ͷ
**************************************************************/
void CSTTPStream::PrintSttpMsg( const STTPMSG * pMsg,const LOG_BUFFER_HEAD& pHead )
{
	if((pMsg == NULL) || (!m_bLog))
		return ;
	
	// print log head
	char chHead[255] = "";
	int  nHeadLen  = 0;
	switch(pHead.nWay)
	{
	case MSG_RECV:
		sprintf(chHead,"recv from %s(%d) \r\n",
			pHead.addr.IpAddr,pHead.addr.nPort);
		
		nHeadLen = strlen(chHead);
		break;
	case MSG_SEND:
		sprintf(chHead,"send to %s(%d) \r\n",
			pHead.addr.IpAddr,pHead.addr.nPort);
		
		nHeadLen = strlen(chHead);
		break;
	default:
		break;
	}

	// get msg length
	char   chtemp[10]="";
	memcpy(chtemp,pMsg->MsgHdr.m_byteMsgLength,5);
	int nBodyLen = atoi(chtemp);
	if(nBodyLen > 0){
		nBodyLen = (nBodyLen > MAX_STTP_MSG_LEN)?MAX_STTP_MSG_LEN:nBodyLen;
	}
	else{
		nBodyLen = 0;
	}

	int iSizeOfStrLog = nHeadLen+sizeof(STTPMSGHDR)+nBodyLen+7;
	string strLog = "";
	strLog.resize(iSizeOfStrLog);
	char * pchar = (char *)&strLog[0];
    
	// printf log 
	try{
		//print log head
		memcpy(pchar,chHead,nHeadLen);
		pchar += nHeadLen;

		//print msg head
		memcpy(pchar,pMsg,sizeof(STTPMSGHDR));
		pchar += sizeof(STTPMSGHDR);
		
		// devision msg head and body with fellow grow
		strncpy(pchar,"---->",5); 
		pchar += 5;
		
		//print msg body
		memcpy(pchar,pMsg->MsgBody,nBodyLen);
		pchar += nBodyLen;
		
		// add enter
		memcpy(pchar,"\r\n",2);
		pchar += 2;
		
		try {
			WriteMessageLog(strLog, iSizeOfStrLog);
		}
		catch(...)
		{
			WriteErrorLog("WriteMessageLog() exception");
		}
	}
	catch(...)
	{
		WriteErrorLog("PrintfSttpMsg() exception");
		return;
	}	
}

/*************************************************************
 �� �� ��:   SetLogPath
 ���ܸ�Ҫ:   ������־·��
 �� �� ֵ:   bool True-�ɹ� False-ʧ��
 ��    ��:   const char * pszPath ��־·��
**************************************************************/
bool CSTTPStream::SetLogPath( const char * pszPath )
{
	CXJNet_Stream::SetLogPath(pszPath);
	m_LogFile.SetLogPath(pszPath);
	/*
	* ----- commented by qingch   8/30/2009 ------
  	return (m_LogFile.Open()?true:false);
	*/
	return true;
}

/*************************************************************
 �� �� ��:   SetHostName
 ���ܸ�Ҫ:   
 �� �� ֵ:   bool
 ��    ��:   const char * pHostName
**************************************************************/
bool CSTTPStream::SetHostName( const char * pHostName )
{
	return m_LogFile.SetHostName(pHostName);
}

/*************************************************************
 �� �� ��:   WriteErrorLog
 ���ܸ�Ҫ:   д������־��Ϣ
 �� �� ֵ:   void
 ��    ��:   const char * chLog ���������־
**************************************************************/
void CSTTPStream::WriteErrorLog( const char* chLog )
{
	/*
	* ----- commented by qingch   9/1/2009 ------
  	CLockUp lockup(&m_LogLock);
  	if (m_LogFile.Open()){
	*/
		m_LogFile.Add(chLog,CLogFile::error);
		/*
		* ----- commented by qingch   9/1/2009 ------
  		m_LogFile.Close();
  	}
		*/
}

/*************************************************************
 �� �� ��:   WriteMessageLog
 ���ܸ�Ҫ:   д������־
 �� �� ֵ:   void
 ��    ��:   const char * chLog ������ı���
 ��    ��:   int iSizeOfStrLog ���ĳ���
**************************************************************/
void CSTTPStream::WriteMessageLog( const string& strLog, int iSizeOfStrLog )
{
	/*
	* ----- commented by qingch   9/1/2009 ------
  	CLockUp lockup(&m_LogLock);
  	if (m_LogFile.Open()){
	*/
	m_LogFile.AddFixLen(strLog.c_str(),iSizeOfStrLog,CLogFile::trace);
		/*
		* ----- commented by qingch   9/1/2009 ------
  		m_LogFile.Close();
  		}
		*/
}

/*************************************************************
 �� �� ��:   Open
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
**************************************************************/
bool CSTTPStream::Open()
{
	m_LogFile.Open();
	return CXJNet_Stream::Open();	
}

/*************************************************************
 �� �� ��:   Close
 ���ܸ�Ҫ:   �ر�������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
**************************************************************/
bool CSTTPStream::Close()
{
	m_LogFile.Close();
	return CXJNet_Stream::Close();
}