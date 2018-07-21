// CbmScoreEngine.cpp: implementation of the CbmScoreEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "CbmScoreEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CbmScoreEngine::CbmScoreEngine()
{
	m_hModule = NULL;
	m_pParseCbmData = NULL;
	m_pParseCbmDataEx = NULL;
	m_pGetTotalScore = NULL;
	m_pGetRuleTypeScore = NULL;
	m_pCloseCbmData = NULL;
	m_pSetLogPath = NULL;
}

CbmScoreEngine::~CbmScoreEngine()
{
	if(m_hModule != NULL)
		xj_free_library(m_hModule);
	m_pParseCbmData = NULL;
	m_pParseCbmDataEx = NULL;
	m_pGetTotalScore = NULL;
	m_pGetRuleTypeScore = NULL;
	m_pCloseCbmData = NULL;
	m_pSetLogPath = NULL;
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_LoadCbmLib	
����ֵ:   bool	װ�سɹ�����TRUE,ʧ�ܷ���FALSE
���ܸ�Ҫ: װ��״̬���޷�ֵ���㶯̬��
*****************************************************/
bool CbmScoreEngine::xjcbm_LoadCbmLib()
{
	m_hModule = xj_load_library(LIBNAME_CBMSCORE);
	if(m_hModule != NULL)
	{
		m_pParseCbmData = (int(*)(char*, int, XJHANDLE&))xj_get_addr(m_hModule, "ParseCbmData");
		m_pParseCbmDataEx = (int(*)(char*, int, int, int, XJHANDLE&))xj_get_addr(m_hModule, "ParseCbmDataEx");
		m_pGetTotalScore = (int(*)(XJHANDLE))xj_get_addr(m_hModule, "GetTotalScore");
		m_pGetRuleTypeScore = (int(*)(int, XJHANDLE))xj_get_addr(m_hModule, "GetRuleTypeScore");
		m_pCloseCbmData = (void(*)(XJHANDLE))xj_get_addr(m_hModule, "CloseCbmData");
		m_pSetLogPath = (void(*)(char*))xj_get_addr(m_hModule, "SetLogPath");
	}
	else
		return false;
	return true;
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_ParseCbmData	
����ֵ:   int	
���ܸ�Ҫ: 
����: CXJString sPTID	
����: int nWarning	
����: XJHANDLE & hCbm	
*****************************************************/
int CbmScoreEngine::xjcbm_ParseCbmData( CXJString sPTID, int nWarning, XJHANDLE& hCbm )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pParseCbmData == NULL)
		return 0;
	return m_pParseCbmData(sPTID.GetBuffer(0), nWarning, hCbm);
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_ParseCbmDataEx	
����ֵ:   int	
���ܸ�Ҫ: 
����: CXJString sPTID	
����: int nDataID	
����: int nMaxDetailID	
����: int nWarning	
����: XJHANDLE & hCbm	
*****************************************************/
int CbmScoreEngine::xjcbm_ParseCbmDataEx( CXJString sPTID, int nDataID, int nMaxDetailID, int nWarning, XJHANDLE& hCbm )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pParseCbmDataEx == NULL)
		return 0;
	return m_pParseCbmDataEx(sPTID.GetBuffer(0), nDataID, nMaxDetailID, nWarning, hCbm);
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_GetTotalScore	
����ֵ:   int	
���ܸ�Ҫ: 
����: XJHANDLE hCbm	
*****************************************************/
int CbmScoreEngine::xjcbm_GetTotalScore( XJHANDLE hCbm )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTotalScore == NULL)
		return 0;
	return m_pGetTotalScore(hCbm);
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_GetRuleTypeScore	
����ֵ:   int	
���ܸ�Ҫ: 
����: int nRuleType	
����: XJHANDLE hCbm	
*****************************************************/
int CbmScoreEngine::xjcbm_GetRuleTypeScore( int nRuleType, XJHANDLE hCbm )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetRuleTypeScore == NULL)
		return 0;
	return m_pGetRuleTypeScore(nRuleType, hCbm);
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_CloseCbmData	
����ֵ:   void	
���ܸ�Ҫ: 
����: XJHANDLE hCbm	
*****************************************************/
void CbmScoreEngine::xjcbm_CloseCbmData( XJHANDLE hCbm )
{
	if(m_hModule == NULL)
		return ;
	if(m_pCloseCbmData == NULL)
		return ;
	m_pCloseCbmData(hCbm);
}

/****************************************************
Date:2012/7/19  Author:LYH
������:   xjcbm_SetLogPath	
����ֵ:   void	
���ܸ�Ҫ: 
����: CXJString sLogPath	
*****************************************************/
void CbmScoreEngine::xjcbm_SetLogPath( CXJString sLogPath )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetLogPath == NULL)
		return;
	m_pSetLogPath(sLogPath.GetBuffer(0));
}
