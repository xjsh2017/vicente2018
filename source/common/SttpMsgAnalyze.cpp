//////////////////////////////////////////////////////////////////////

#include "SttpMsgAnalyze.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSttpMsgAnalyze::CSttpMsgAnalyze()
:m_pSttpData(NULL),
 m_hLibXJSTTPAnalyzer(NULL),
 m_pFunDataToSttp(NULL),
 m_pFunSttpToData(NULL)
{
	InitLibrary();
}

CSttpMsgAnalyze::~CSttpMsgAnalyze()
{
	xj_free_library(m_hLibXJSTTPAnalyzer);
}

//##ModelId=48C00E7102FD
CSttpMsgAnalyze::CSttpMsgAnalyze(STTP_FULL_DATA* pData)
:m_pSttpData(pData),
 m_hLibXJSTTPAnalyzer(NULL),
 m_pFunDataToSttp(NULL),
 m_pFunSttpToData(NULL)
{
	InitLibrary();
}

void CSttpMsgAnalyze::Attach( STTP_FULL_DATA* pData )
{
	m_pSttpData = pData;
}

int CSttpMsgAnalyze::Data_To_Sttp( STTPMSG_QUEUE& pMsgQueue )
{
	if (NULL != m_pFunDataToSttp)
	{
		return m_pFunDataToSttp(m_pSttpData,pMsgQueue);
	} 
	else
	{
		return -1;
	}
}

int CSttpMsgAnalyze::Sttp_To_Data( STTPMSG* pSttpMsg )
{
	if (NULL != m_pFunSttpToData)
	{
		return m_pFunSttpToData(pSttpMsg,m_pSttpData);
	} 
	else
	{
		return -1;
	}
}

int CSttpMsgAnalyze::MultiSttp_To_Data( STTPMSG_QUEUE& pMsgQueue )
{
	return 0;
}

void CSttpMsgAnalyze::InitLibrary()
{
	m_hLibXJSTTPAnalyzer = xj_load_library(LIB_STTP_ANALYZER_NAME);
	if (m_hLibXJSTTPAnalyzer != NULL)
	{
		m_pFunDataToSttp = (P_FUNCDATATOSTTP) xj_get_addr(m_hLibXJSTTPAnalyzer,
			"Data_To_Sttp");

		
		m_pFunSttpToData = (P_FUNCSTTPTODATA) xj_get_addr(m_hLibXJSTTPAnalyzer,
			"Sttp_To_Data");
	
	} 
}

