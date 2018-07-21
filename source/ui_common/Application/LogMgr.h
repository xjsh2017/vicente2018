// ***************************************************************
//  LogMgr   version:  1.0    date: 3:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 日志管理类定义
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_)
#define AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogMgr.h : header file
//
#include "vccommon_define.h"

/////////////////////////////////////////////////////////////////////////////
// CLogMgr view

/**
 * @defgroup 	CLogMgr	日志管理类
 * @{
 */
/**
 * @brief       1.写日志文件 2.管理日志文件(到一定大小清空日志文件) 3.控制日志是否写文件,是否输出到日志输出窗口 4.组织日志语句
*  @author      LYH
 * @version     ver1.0
 * @date        03/09/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87B990203
class CLogMgr
{
private:
	//##ModelId=49B87B990205
	CLogMgr(CString sPath, CString sFolder = "Temp_Log", int nLogLevel = XJ_LOG_LV3, BOOL bOutPutToFile = TRUE);

public:
	static CLogMgr* getInstance();
	static CLogMgr* CreateInstance(CString sPath, CString sFolder = "Temp_Log", int nLogLevel = XJ_LOG_LV3, BOOL bOutPutToFile = TRUE);

public:
	static CLogMgr* m_instance;
// Attributes
private:
	/** @brief           日志文件*/
	//##ModelId=49B87B99020D
	CMessageLog		m_File;

	/** @brief			日志等级*/
	int			m_nLogLevel;

	/** @brief			是否输出到文件*/
	BOOL		m_bOutPutToFile;

// Operations
public:
/*
 *  @brief   	SetLogPath	 设置日志文件路径 
 *  @param 		[In]a param of Type  CString&  要设置的路径
 *  @return 	void 
 */
	//##ModelId=49B87B990214
	void SetLogPath(CString& path,CString sFolder = "Temp_Log");

/*
 *  @brief   	WriteLog	 写日志操作,供外部调用 
 *  @param 		[In]a param of Type  CString&  日志字符串
 *  @param 		[In]a param of Type  const int&  日志等级
 *  @return 	void 
 */
	//##ModelId=49B87B990218
	void WriteLog(CString& sLog, const int& nLV = 2);

	/*
	* @brief	GetLogLevel			
	* @return   int	
	*/	
	int GetLogLevel() const { return m_nLogLevel; }

	/*
	* @brief	SetLogLevel	
	* @param	[In]a param of Type int		
	* @return   	
	*/	
	void SetLogLevel(int LogLevel) { m_nLogLevel = LogLevel; m_File.SetLogLevel(m_nLogLevel);}

	/*
	* @brief	GetOutPutToFile			
	* @return   BOOL	
	*/	
	BOOL GetOutPutToFile() const { return m_bOutPutToFile; }

	/*
	* @brief	SetOutPutToFile	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetOutPutToFile(BOOL OutPutToFile) { m_bOutPutToFile = OutPutToFile; }

private:
/*
 *  @brief   	MarkLogString	 组织日志输出的格式
 *  @param 		[In]a param of Type  CString*  要输出的字符串
 *  @return 	CString		格式化后的输出字符串
 */
	//##ModelId=49B87B990226
	CString	MarkLogString(CString& sLog);

/*
 *  @brief   	OutputToFile	 输出到日志文件 
 *  @param 		[In]a param of Type  CString&  输出的字符串
 *  @return 	void 
 */
	//##ModelId=49B87B990228
	void OutputToFile(CString& sLog, int nLv = 2);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogMgr)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B990233
	virtual ~CLogMgr();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_)
