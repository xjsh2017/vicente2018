/********************************************************************
	created:	2008/10/28
	created:	28:10:2008   14:33
	file base:	SttpMsgMaker
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef CSTTPMSGMAKER_H
#define CSTTPMSGMAKER_H

#pragma   warning   (disable   :   4275)

#include "define.h"
#include "SttpMsgParser.h"
#include "SttpMsgAnalyze.h"
#include "XJTime.h"

class CSttpMsgMaker
{
public:
    CSttpMsgMaker();
    virtual ~CSttpMsgMaker();
    void Make00215Msg(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , int pResult , int pSize);
	void Make00901Msg(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg ,  int pResult , const char * pHandleName = NULL , const char * pVarData = NULL);
	void Make00901Msg(STTPMSG& pDestinationMsg , UINT uRII , UINT uMsgId , int pResult , const char * pHandleName = NULL , const char * pVarData = NULL);

	void Make20069Msg(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , int pResult);
	void Make20069Msg(STTPMSG& pDestinationMsg , UINT pRII , const char* pPtId , int pCpu , int pResult , UINT pMsgId);

	void Make20125Msg(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg , const char* pReason);
	void Make20125Msg(STTPMSG& pDestinationMsg , UINT uRII , UINT uMsgId , const char* PtId , const char* pReason , int iCpu );

	void Make20144Msg(STTPMSG& pDestinationMsg , const char* pStationId ,int pStatus, int pReason,int pChannel);
    void Make20146Msg(STTPMSG& pDestinationMsg, UINT uRII , const char* pStationId ,int pStatus, int pChannel, int pReason);
	void Make20156Msg(STTPMSG& pDestinationMsg , UINT uRII , UINT uOptionType , const char* ptId ,UINT uInfoType , UINT uDescType , UINT uGroup , UINT uResult , UINT uCpu , const char* pReason = NULL);
	void Make20138Msg(STTPMSG& pDestinationMsg , const char* pStation);
	void Make20128Msg(STTPMSG& pDestinationMsg , const char* pStation,int pSource);
	void Make20126Msg(STTPMSG& pDestinationMsg , const char* pStation);	
	void Make20157Msg(STTPMSG& pDestinationMsg , UINT pRII , const char* pPtId , int pCpu , int pZone  , UINT pMsgId , int pSize);

	void MakeTestMsg(STTPMSG& pDestinationMsg, unsigned int pRII);
	void MakeTestResponse(STTPMSG& pDestinationMsg , const STTPMSG& pSourceMsg);
protected:
private:
};

#endif // CSTTPMSGMAKER_H
