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
 函 数 名:   CSTTPStream
 功能概要:   构造函数
 返 回 值:   
 参    数:   const char * pSzHost 客户名
 参    数:   bool pLogFlag 是否保存报文
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
 函 数 名:   ~CSTTPStream
 功能概要:	 析构函数
 返 回 值:   
**************************************************************/
CSTTPStream::~CSTTPStream()
{

	m_LogFile.Close();

}

/*************************************************************
 函 数 名:   SendSTTPMessage
 功能概要:   发送一条STTP报文
 返 回 值:   int >0-成功发送的实际长度 <=0-发送失败 
 参    数:   const STTPMSG * pMsg  待发送的STTP报文
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
 函 数 名:   RecvSTTPMessage
 功能概要:   接收一条STTP报文
 返 回 值:   int >0-成功接收的实际长度 <=0-接收失败
 参    数:   STTPMSG * pMsg 保存收到的STTP报文
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
		sprintf(cError,"读到的实际长度：%d,预期的长度：%d",nRet2,iMsgLength);
		WriteErrorLog(cError);
		return 0;
	}

	return (nRet+nRet2);
}

/*************************************************************
 函 数 名:   PrintSttpMsg
 功能概要:   输出报文到日志文件
 返 回 值:   void
 参    数:   const STTPMSG * pMsg 待输出的报文
 参    数:   const LOG_BUFFER_HEAD & pHead 输出的日志头
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
 函 数 名:   SetLogPath
 功能概要:   设置日志路径
 返 回 值:   bool True-成功 False-失败
 参    数:   const char * pszPath 日志路径
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
 函 数 名:   SetHostName
 功能概要:   
 返 回 值:   bool
 参    数:   const char * pHostName
**************************************************************/
bool CSTTPStream::SetHostName( const char * pHostName )
{
	return m_LogFile.SetHostName(pHostName);
}

/*************************************************************
 函 数 名:   WriteErrorLog
 功能概要:   写错误日志信息
 返 回 值:   void
 参    数:   const char * chLog 待输出的日志
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
 函 数 名:   WriteMessageLog
 功能概要:   写报文日志
 返 回 值:   void
 参    数:   const char * chLog 待输出的报文
 参    数:   int iSizeOfStrLog 报文长度
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
 函 数 名:   Open
 功能概要:   打开流对象
 返 回 值:   bool true-成功 false-失败
**************************************************************/
bool CSTTPStream::Open()
{
	m_LogFile.Open();
	return CXJNet_Stream::Open();	
}

/*************************************************************
 函 数 名:   Close
 功能概要:   关闭流对象
 返 回 值:   bool true-成功 false-失败
**************************************************************/
bool CSTTPStream::Close()
{
	m_LogFile.Close();
	return CXJNet_Stream::Close();
}