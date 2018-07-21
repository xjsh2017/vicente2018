/********************************************************************
	created:	2008/10/28
	created:	28:10:2008   14:32
	file base:	SttpMsgMaker
	file ext:	cppBody
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "SttpMsgMaker.h"

CSttpMsgMaker::CSttpMsgMaker()
{
    //ctor
}

CSttpMsgMaker::~CSttpMsgMaker()
{
    //dtor
}

void CSttpMsgMaker::Make00215Msg( STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , int pResult , int pSize )
{
    //清空输出报文
    bzero(&pDestinationMsg,sizeof(STTPMSG));
    
    STTP_FULL_DATA sttp_source_data;
    zero_sttp_full_data(sttp_source_data);
    CSttpMsgAnalyze pAnalyze(&sttp_source_data);
    pAnalyze.Sttp_To_Data(const_cast<STTPMSG*>(&pSourceMsg));
    
    //获取RII
    UINT uRii = sttp_source_data.sttp_head.uMsgRii;
    
    CSttpMsgParser parser(&pDestinationMsg);
    //设置 sttp head
    parser.SetMsgRII(uRii);
    parser.SetMsgID(215);
    parser.SetMsgType('E');
    parser.SetMsgLength(227);
    parser.SetMsgLengthType('F');
    parser.SetMsgEndFlag(false);
    
    char* pBody = parser.GetMsgBody();
    //设置PtId
    memcpy(pBody,sttp_source_data.sttp_body.ch_pt_id,12);
    pBody += 12;
    
    //设置文件名
    strncpy(pBody,sttp_source_data.sttp_body.variant_member.file_data.strFileName.c_str(),200);
    pBody+=200;
    
    //result
    sprintf(pBody,"%-1d",pResult);
    pBody+=1;	
    
    //size
    sprintf(pBody,"%d",pSize);
	pBody+=10;

    //RII
    sprintf(pBody,"%d",sttp_source_data.sttp_body.variant_member.file_data.nFileNO);
    pBody+=4;
}
void CSttpMsgMaker::Make00901Msg(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg ,  int pResult , const char * pHandleName /*= NULL */, const char * pVarData /*= NULL*/ )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(const_cast<STTPMSG*>(&pSourceMsg));
	//获取RII
	UINT uRii = parser.GetMsgRII();
	//获取MsgId
	UINT uMsgId = parser.GetMsgID();

	//计算报文长度
	UINT uMsgLength = (pVarData==NULL)?78:( 78+_min(strlen(pVarData), MAX_STTP_MSG_LEN - 78 ));

	//获取系统时间
	char  szDate[15] = "";
	SYSTIME sysTime;
	
	GetSysTime(sysTime);
	sprintf(szDate,"%04d%02d%02d%02d%02d%02d",
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	//设置 sttp head
	parser.Attach(&pDestinationMsg);
	parser.SetMsgRII(uRii);
	parser.SetMsgID(901);
	parser.SetMsgType('E');
	parser.SetMsgLength(uMsgLength);
	parser.SetMsgLengthType('V');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置 set id
	memcpy(pBody,"000",3);
	pBody += 3;
	
	// source sttp msgId
	sprintf(pBody,"%-5d",uMsgId);
	pBody += 5;
	
	// result status
	sprintf(pBody,"%04d",pResult);
	pBody += 4;

	// time
	memcpy(pBody,szDate,14);
	pBody += 14;

	// sys code
	memcpy(pBody,"00",2);
	pBody += 2;
	
	// device code
	memcpy(pBody,"00",2);
	pBody += 2;
	
	// the name of sttpmsghandle ...(32 bytes)
	if (pHandleName != NULL){
		memcpy(pBody,pHandleName,_min( strlen(pHandleName), 32 ));		
	}
	pBody += 32;

	// set varible part
	if(pVarData != NULL){
		memcpy(pBody,pVarData,_min( strlen(pVarData), MAX_STTP_MSG_LEN - 78 ));	
	}
}

void CSttpMsgMaker::Make00901Msg( STTPMSG& pDestinationMsg , UINT uRII , UINT uMsgId ,int pResult , const char * pHandleName /*= NULL */, const char * pVarData /*= NULL*/ )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	//计算报文长度
	UINT uMsgLength = (pVarData==NULL)?78:( 78+_min(strlen(pVarData), MAX_STTP_MSG_LEN - 78 ));
	
	//获取系统时间
	char  szDate[15] = "";
	SYSTIME sysTime;
	
	GetSysTime(sysTime);
	sprintf(szDate,"%04d%02d%02d%02d%02d%02d",
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	CSttpMsgParser parser(&pDestinationMsg);	
	//设置 sttp head
	parser.Attach(&pDestinationMsg);
	parser.SetMsgRII(uRII);
	parser.SetMsgID(901);
	parser.SetMsgType('E');
	parser.SetMsgLength(uMsgLength);
	parser.SetMsgLengthType('V');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置 set id
	memcpy(pBody,"000",3);
	pBody += 3;
	
	// source sttp msgId
	sprintf(pBody,"%-5d",uMsgId);
	pBody += 5;
	
	// result status
	sprintf(pBody,"%04d",pResult);
	pBody += 4;
	
	// time
	memcpy(pBody,szDate,14);
	pBody += 14;
	
	// sys code
	memcpy(pBody,"00",2);
	pBody += 2;
	
	// device code
	memcpy(pBody,"00",2);
	pBody += 2;
	
	// the name of sttpmsghandle ...(32 bytes)
	if (pHandleName != NULL){
		memcpy(pBody,pHandleName,_min( strlen(pHandleName), 32 ));		
	}
	pBody += 32;
	
	// set varible part
	if(pVarData != NULL){
		memcpy(pBody,pVarData,_min( strlen(pVarData), MAX_STTP_MSG_LEN - 78 ));	
	}	
}

void CSttpMsgMaker::Make20069Msg( STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , int pResult )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	STTP_FULL_DATA sttp_source_data;
	zero_sttp_full_data(sttp_source_data);
	CSttpMsgAnalyze pAnalyze(&sttp_source_data);
	pAnalyze.Sttp_To_Data(const_cast<STTPMSG*>(&pSourceMsg));

	//获取RII
	UINT uRii = sttp_source_data.sttp_head.uMsgRii;
	//获取MsgId
	UINT uMsgId = sttp_source_data.sttp_head.uMsgID;

	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(uRii);
	parser.SetMsgID(20069);
	parser.SetMsgType('R');
	parser.SetMsgLength(21);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);

	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,sttp_source_data.sttp_body.ch_pt_id,12);
	pBody += 12;

	//设置cpu号
	sprintf(pBody,"%-2d",sttp_source_data.sttp_body.nCpu);
	pBody+=2;
	
	//result
	sprintf(pBody,"%-2d",pResult);
	pBody+=2;	

	//MsgId
	sprintf(pBody,"%-5d",uMsgId);
	pBody+=5;
}

void CSttpMsgMaker::Make20069Msg( STTPMSG& pDestinationMsg , UINT pRII , const char* pPtId , int pCpu, int pResult , UINT pMsgId )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(pRII);
	parser.SetMsgID(20069);
	parser.SetMsgType('R');
	parser.SetMsgLength(21);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,pPtId,12);
	pBody += 12;
	
	//设置cpu号
	sprintf(pBody,"%-2d",pCpu);
	pBody+=2;
	
	//result
	sprintf(pBody,"%-2d",pResult);
	pBody+=2;	
	
	//MsgId
	sprintf(pBody,"%-5d",pMsgId);
	pBody+=5;	
}

void CSttpMsgMaker::Make20125Msg( STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , const char* pReason )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	STTP_FULL_DATA sttp_source_data;
	zero_sttp_full_data(sttp_source_data);
	CSttpMsgAnalyze pAnalyze(&sttp_source_data);
	pAnalyze.Sttp_To_Data(const_cast<STTPMSG*>(&pSourceMsg));
	
	//获取RII
	UINT uRii = sttp_source_data.sttp_head.uMsgRii;
	//获取MsgId
	UINT uMsgId = sttp_source_data.sttp_head.uMsgID;

	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(uRii);
	parser.SetMsgID(20125);
	parser.SetMsgType('I');
	parser.SetMsgLength(274);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,sttp_source_data.sttp_body.ch_pt_id,12);
	pBody += 12;
	//原因
	strncpy(pBody,pReason,255);
	pBody += 255;
	//设置cpu号
	sprintf(pBody,"%-2d",sttp_source_data.sttp_body.nCpu);
	pBody+=2;	
	//MsgId
	sprintf(pBody,"%-5d",uMsgId);
	pBody+=5;
}

void CSttpMsgMaker::Make20125Msg( STTPMSG& pDestinationMsg , UINT uRII , UINT uMsgId , const char* PtId , const char* pReason , int iCpu )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
		
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(uRII);
	parser.SetMsgID(20125);
	parser.SetMsgType('I');
	parser.SetMsgLength(274);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	strncpy(pBody,PtId,12);
	pBody += 12;
	//原因
	strncpy(pBody,pReason,255);
	pBody += 255;
	//设置cpu号
	sprintf(pBody,"%-2d",iCpu);
	pBody+=2;	
	//MsgId
	sprintf(pBody,"%-5d",uMsgId);
	pBody+=5;	
}

void CSttpMsgMaker::MakeTestMsg( STTPMSG& pDestinationMsg, unsigned int pRII )
{
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(pRII);
	parser.SetMsgID(20115);
	parser.SetMsgType('I');
	parser.SetMsgLength(4);
	parser.SetMsgLengthType('V');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,"TEST",4);
	pBody += 4;	
}

void CSttpMsgMaker::MakeTestResponse(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg)
{
	CSttpMsgParser parser(const_cast<STTPMSG*>(&pSourceMsg));
	//获取RII
	UINT uRii = parser.GetMsgRII();

	parser.Attach(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(uRii);
	parser.SetMsgID(20115);
	parser.SetMsgType('I');
	parser.SetMsgLength(12);
	parser.SetMsgLengthType('V');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,"Test Success",12);
	pBody += 12;		
}

void CSttpMsgMaker::Make20146Msg( STTPMSG& pDestinationMsg, UINT uRII , const char* pStationId ,int pStatus, int pChannel, int pReason )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	CSttpMsgParser parser(&pDestinationMsg);

	//设置 sttp head
	parser.SetMsgRII(uRII);
	parser.SetMsgID(20146);
	parser.SetMsgType('I');
	parser.SetMsgLength(17);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	strncpy(pBody,pStationId,12);
	pBody += 12;

	//Status
	sprintf(pBody,"%1d",pStatus);
	pBody+=1;	

	//channel
	sprintf(pBody,"%-2d",pChannel);
	pBody+=2;

    //reason
    sprintf(pBody,"%-2d",pReason);
	pBody+=2;

    SYSTIME sysTime;
	GetSysTime(sysTime);
    CXJTime tTime(sysTime);

    strncpy(pBody,tTime.GetTimeString(CXJTime::STTP12Time).c_str(),12);
    pBody+=12;

    strncpy(pBody,tTime.GetTimeString(CXJTime::STTP12Time).c_str(),12);
    pBody+=12;
}

void CSttpMsgMaker::Make20144Msg( STTPMSG& pDestinationMsg , const char* pStationId ,int pStatus, int pReason,int pChannel )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(0);
	parser.SetMsgID(20144);
	parser.SetMsgType('I');
	parser.SetMsgLength(41);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	strncpy(pBody,pStationId,12);
	pBody += 12;
	
	//Status
	sprintf(pBody,"%1d",pStatus);
	pBody+=1;

	//reason
	sprintf(pBody,"%-2d",pReason);
	pBody+=2;

	SYSTIME sysTime;
	GetSysTime(sysTime);
    CXJTime tTime(sysTime);

	//time
    strncpy(pBody,tTime.GetTimeString(CXJTime::STTP12Time).c_str(),12);
	pBody+=12;

	//channel
	sprintf(pBody,"%-2d",pChannel);
	pBody+=2;
    
    //time
    strncpy(pBody,tTime.GetTimeString(CXJTime::STTP12Time).c_str(),12);
    pBody+=12;
}

void CSttpMsgMaker::Make20156Msg( STTPMSG& pDestinationMsg , UINT uRII , UINT uOptionType , const char* ptId ,UINT uInfoType , UINT uDescType , UINT uGroup , UINT uResult , UINT uCpu , const char* pReason /*= NULL*/ )
{
    //清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);	
	//设置 sttp head
	parser.Attach(&pDestinationMsg);
	parser.SetMsgRII(uRII);
	parser.SetMsgID(20156);
	parser.SetMsgType('I');
	parser.SetMsgLength(281);
	parser.SetMsgLengthType('V');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置 配置类型
	sprintf(pBody,"%-2d",uOptionType);
	pBody += 2;
	
	// 装置ID
	strncpy(pBody,ptId,12);
	pBody += 12;
	
	// 信息类型
	sprintf(pBody,"%-2d",uInfoType);
	pBody += 2;
	
	// 描述类型
	sprintf(pBody,"%-2d",uDescType);
	pBody += 2;
	
	// group
	sprintf(pBody,"%-4d",uGroup);
	pBody += 4;

	// 初始化结果
	sprintf(pBody,"%-2d",uResult);
	pBody += 2;

	// cpu
	sprintf(pBody,"%-2d",uCpu);
	pBody += 2;

	// 原因
	if(pReason != NULL){
		strncpy(pBody,pReason,255);	
		pBody +=255;
	}	
}

void CSttpMsgMaker::Make20138Msg( STTPMSG& pDestinationMsg , const char* pStation )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(0);
	parser.SetMsgID(20138);
	parser.SetMsgType('I');
	parser.SetMsgLength(14);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置stationid
	memcpy(pBody,pStation,12);
	pBody += 12;

	sprintf(pBody,"%-2d",0);
	pBody+=2;	
}

void CSttpMsgMaker::Make20128Msg( STTPMSG& pDestinationMsg , const char* pStation,int pSource )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(0);
	parser.SetMsgID(20128);
	parser.SetMsgType('I');
	parser.SetMsgLength(13);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置stationid
	memcpy(pBody,pStation,12);
	pBody += 12;

	sprintf(pBody,"%d",pSource);//20130902
	pBody += 1;

}

void CSttpMsgMaker::Make20157Msg( STTPMSG& pDestinationMsg , UINT pRII , const char* pPtId , int pCpu , int pZone , UINT pMsgId , int pSize )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(pRII);
	parser.SetMsgID(20157);
	parser.SetMsgType('R');
	parser.SetMsgLength(37);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置PtId
	memcpy(pBody,pPtId,12);
	pBody += 12;
	
	//设置cpu号
	sprintf(pBody,"%-2d",pCpu);
	pBody+=2;
	
	//zone
	sprintf(pBody,"%-2d",pZone);
	pBody+=2;	

	pBody += 2;
	
	//MsgId
	sprintf(pBody,"%-5d",pMsgId);
	pBody+=5;	

	pBody += 2;

	//传输内容长度
	double tdSize =  pSize / 1000.0;
	sprintf(pBody,"%.3f",tdSize);
	pBody += 12;
}

void CSttpMsgMaker::Make20126Msg( STTPMSG& pDestinationMsg , const char* pStation )
{
	//清空输出报文
	bzero(&pDestinationMsg,sizeof(STTPMSG));
	
	CSttpMsgParser parser(&pDestinationMsg);
	//设置 sttp head
	parser.SetMsgRII(0);
	parser.SetMsgID(20126);
	parser.SetMsgType('I');
	parser.SetMsgLength(27);
	parser.SetMsgLengthType('F');
	parser.SetMsgEndFlag(false);
	
	char* pBody = parser.GetMsgBody();
	//设置stationid
	memcpy(pBody,pStation,12);
	pBody += 12;
	
    SYSTIME sysTime;
	GetSysTime(sysTime);
    CXJTime tTime(sysTime);
	
    strncpy(pBody,tTime.GetTimeString(CXJTime::STTP15Time).c_str(),15);
    pBody+=15;

}
