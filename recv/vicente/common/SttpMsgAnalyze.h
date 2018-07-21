/********************************************************************
	created:	2008/09/05
	created:	5:9:2008   0:14
	file base:	SttpMsgAnalyze
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef SttpMsgAnalyze_h__
#define SttpMsgAnalyze_h__

#pragma   warning   (disable   :   4275)
#pragma  warning  (disable : 4786)

#include "define.h"
#include "XJSttpNetAPI.h"
#include "xjlib.h"
//#include "SttpMsgParser.h"


#ifdef OS_LINUX

/** @brief            STTP报文分析共享库名称定义*/
#define LIB_STTP_ANALYZER_NAME              "libXJSttpAnalyzer.so"

#endif

#ifdef OS_WINDOWS

/** @brief            STTP报文分析共享库名称定义*/
#define LIB_STTP_ANALYZER_NAME              "XJSttpAnalyzer.dll"

#endif

/************************************************************************/

typedef int (*P_FUNCDATATOSTTP)(STTP_FULL_DATA*, STTPMSG_QUEUE&);

typedef	int (*P_FUNCSTTPTODATA)(STTPMSG*, STTP_FULL_DATA*);

/************************************************************************/

//##ModelId=48C00DA2009C
class CSttpMsgAnalyze  
{
public:
    //##ModelId=48C00DA200D2
	CSttpMsgAnalyze();
    //##ModelId=48C00E7102FD
    CSttpMsgAnalyze(STTP_FULL_DATA* pData);
    //##ModelId=48C00DA200D3
	virtual ~CSttpMsgAnalyze();
    //##ModelId=48C00F55036B
    void Attach(STTP_FULL_DATA* pData);

	int Data_To_Sttp(STTPMSG_QUEUE& pMsgQueue);
	
	int Sttp_To_Data(STTPMSG* pSttpMsg);

	int MultiSttp_To_Data(STTPMSG_QUEUE& pMsgQueue);

private:
	void InitLibrary();

private:

	STTP_FULL_DATA*			m_pSttpData;

	/** @brief				STTP报文解析共享库handle*/
	XJHANDLE				m_hLibXJSTTPAnalyzer;
	
	/** @brief				Data_To_Sttp函数指针*/
	P_FUNCDATATOSTTP		m_pFunDataToSttp;
	
	/** @brief				Sttp_To_Data函数指针*/
	P_FUNCSTTPTODATA		m_pFunSttpToData;





};

#endif // SttpMsgAnalyze_h__











