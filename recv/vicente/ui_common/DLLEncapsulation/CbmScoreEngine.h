// CbmScoreEngine.h: interface for the CbmScoreEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBMSCOREENGINE_H__2B3E4B11_C43F_4CB3_8EBE_B05D41EB9949__INCLUDED_)
#define AFX_CBMSCOREENGINE_H__2B3E4B11_C43F_4CB3_8EBE_B05D41EB9949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../common/define.h"
#include "../../common/XJString.h"
#include "../../common/xjlib.h"

#ifdef OS_WINDOWS
#define LIBNAME_CBMSCORE "XJCBMScore.dll"
#else 
#define LIBNAME_CBMSCORE "libXJCBMScore.so"
#endif

//定义名以cbm开头
typedef int (*cbm_ParseCbmData)(char*, int, XJHANDLE&);
typedef int (*cbm_ParseCbmDataEX)(char*, int, int, int, XJHANDLE&);
typedef int (*cbm_GetTotalScore)(XJHANDLE);
typedef int (*cbm_GetRuleTypeScore)(int,XJHANDLE);
typedef void (*cbm_CloseCbmData)(XJHANDLE);
typedef void (*cbm_SetLogPath)(char*);

class CbmScoreEngine  
{
public:
	CbmScoreEngine();
	virtual ~CbmScoreEngine();
private:
	/** @brief			动态库句柄*/
	XJHANDLE	m_hModule;
	cbm_ParseCbmData	m_pParseCbmData;
	cbm_ParseCbmDataEX	m_pParseCbmDataEx;
	cbm_GetTotalScore	m_pGetTotalScore;
	cbm_GetRuleTypeScore m_pGetRuleTypeScore;
	cbm_CloseCbmData	m_pCloseCbmData;
	cbm_SetLogPath		m_pSetLogPath;
public:
	/*
	* @brief	xjcbm_LoadCbmLib 装载状态检修分值计算动态库	
	* @return   bool	装载成功返回TRUE,失败返回FALSE
	*/
	bool xjcbm_LoadCbmLib();

	int xjcbm_ParseCbmData(CXJString sPTID, int nWarning, XJHANDLE& hCbm);
	int xjcbm_ParseCbmDataEx(CXJString sPTID, int nDataID, int nMaxDetailID, int nWarning, XJHANDLE& hCbm);
	int xjcbm_GetTotalScore(XJHANDLE hCbm);
	int xjcbm_GetRuleTypeScore(int nRuleType, XJHANDLE hCbm);
	void xjcbm_CloseCbmData(XJHANDLE hCbm);
	void xjcbm_SetLogPath(CXJString sLogPath);
};

#endif // !defined(AFX_CBMSCOREENGINE_H__2B3E4B11_C43F_4CB3_8EBE_B05D41EB9949__INCLUDED_)
