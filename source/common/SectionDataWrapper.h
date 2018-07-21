/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   16:57
	filename: 	\VICENTE\CODE\COMMON\SectionDataWrapper.h
	file path:	\VICENTE\CODE\COMMON
	file base:	SectionDataWrapper
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#pragma   warning   (disable   :   4786) 
#pragma   warning   (disable   :   4275) 

#ifndef SectionDataWrapper_h__
#define SectionDataWrapper_h__

#include "define.h"
#include "xjlib.h"


#ifdef OS_LINUX

/** @brief            断面数据共享库名称定义*/
#define LIB_SHARESECTIONDATA_NAME              "libXJShareSectionData.so"

#endif

#ifdef OS_WINDOWS

/** @brief            断面数据共享库名称定义*/
#define LIB_SHARESECTIONDATA_NAME              "XJShareSectionData.dll"

#endif

/** @brief            CreateSectionData函数定义*/
typedef bool (*PFUNCREATESECTIONDATA)(string,vector<string>&,string,int,string&);

/** @brief            OpenSectionData函数定义*/
typedef bool (*PFUNOPENSECTIONDATA)(string,string,int,string&);

/** @brief            CloseSectionData函数定义*/
typedef bool (*PFUNCLOSESECTION)(string&);

/** @brief            SetFullSectionData函数定义*/
typedef bool (*PFUNSETFULLSD)(string,const string&,string&);

/** @brief            GetFullSectionData函数定义*/
typedef bool (*PFUNGETFULLSD)(string,string&,string&);

/** @brief            SetSectionData函数定义*/
typedef bool (*PFUNSETSD)(string,int,int,const string&,string&);

/** @brief            GetSectionData函数定义*/
typedef bool (*PFUNGETSD)(string,int,int,string&,string&);

/** @brief            SetPointData函数定义*/
typedef bool (*PFUNSETPOINTDATA)(string,int,int,const string&,string&);

/** @brief            GetPointData函数定义*/
typedef bool (*PFUNGETPOINTDATA)(string,int,int,string&,string&);

class CSectionDataWrapper
{
public:
	CSectionDataWrapper();
	~CSectionDataWrapper();
	bool InitLibrary(char* pErrorMsg);
	/**
	* @brief         创建断面数据共享内存
	* @param[in]     pSHMName:共享内存名
	* @param[in]     pStationIDList:厂站ID列表	
	* @param[in]     pLogPath:日志路径
	* @param[in]     pLogLevel:日志级别 
	* @param[out]    pErrorMsg:错误信息 
	* @return        启动服务的结果 bool true-成功 false-失败
	*
    */
	bool CreateSectionData(string pSHMName, vector<string>&	pStationIDList, string pLogPath, int pLogLevel, string& pErrorMsg);

	/**
	* @brief         打开断面数据共享内存 
	* @param[in]     pSHMName:共享内存名
	* @param[in]     pLogPath:日志路径
	* @param[in]     pLogLevel:日志级别 
	* @param[out]    pErrorMsg:错误信息 
	* @return        启动服务的结果 true-成功 false-失败
	*
    */
	bool OpenSectionData(string pSHMName ,string pLogPath, int pLogLevel, string& pErrorMsg);

	/**
	* @brief         关闭断面数据共享内存
	* @param[out]    pErrorMsg:错误信息 
	* @return        停止服务的结果 true-成功 false-失败
	*
    */
	bool CloseSectionData(string& pErrorMsg);

	/**
	* @brief         设置某站全部断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[in]     pSectionData:断面数据块
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool SetFullSectionData(string pStationId, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         获取某站全部断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[out]    pSectionData:断面数据块
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool GetFullSectionData(string pStationId, string& pSectionData, string& pErrorMsg);

	/**
	* @brief         设置某站部分断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[in]     pOffSet:断面数据块起始地址
	* @param[in]     pSize:断面数据块大小
	* @param[in]     pSectionData:断面数据块
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool SetSectionData(string pStationId, int pOffSet, int pSize, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         获取某站部分断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[in]     pOffSet:断面数据块起始地址
	* @param[in]     pSize:断面数据块大小
	* @param[out]    pSectionData:断面数据块
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool GetSectionData(string pStationId, int pOffSet, int pSize, string& pSectionData, string& pErrorMsg);

	/**
	* @brief         设置某点断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[in]     pRow:行号 
	* @param[in]     pColumn:列号
	* @param[in]     pSectionData:断面数据
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool SetPointData(string pStationId, int pRow, int pColumn, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         获取某点断面数据
	* @param[in]     pStationId:厂站编号 
	* @param[in]     pRow:行号 
	* @param[in]     pColumn:列号
	* @param[out]     pSectionData:断面数据
	* @param[out]    pErrorMsg:错误信息 
	* @return        操作结果 true-成功 false-失败
	*
    */
	bool GetPointData(string pStationId, int pRow, int pColumn, string& pSectionData, string& pErrorMsg);
protected:
private:
	/** @brief				断面数据共享库handle*/
	XJHANDLE					m_hLibXJShareSD;
	
	/** @brief				CreateSectionData函数指针*/
	PFUNCREATESECTIONDATA	m_pFunCreateSectionData;
	
	/** @brief				OpenSectionData函数指针*/
	PFUNOPENSECTIONDATA		m_pFunOpenSectionData;
	
	/** @brief				CloseSectionData函数指针*/
	PFUNCLOSESECTION		m_pFunCloseSectionData;
	
	/** @brief				GetFullSectionData函数指针*/
	PFUNSETFULLSD		    m_pFunSetFullSectionData;
	
	/** @brief				GetFullSectionData函数指针*/
	PFUNGETFULLSD			m_pFunGetFullSectionData;
	
	/** @brief				SetSectionData函数指针*/
	PFUNSETSD				m_pFunSetSectionData;
	
	/** @brief				GetSectionData函数指针*/
	PFUNGETSD				m_pFunGetSectionData;
	
	/** @brief				SetPointData函数指针*/
	PFUNSETPOINTDATA		m_pFunSetPointData;
	
	/** @brief				GetPointData函数指针*/
	PFUNGETPOINTDATA		m_pFunGetPointData;
};

#endif // SectionDataWrapper_h__