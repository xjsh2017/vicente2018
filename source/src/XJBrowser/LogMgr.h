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
#include "LogView.h"


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
class CLogMgr : public CObject
{
public:
	//##ModelId=49B87B990205
	CLogMgr();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogMgr)

// Attributes
public:
private:
	//##ModelId=49B87B990206
	CString		m_sFullPath;	//日志文件全路径
	//##ModelId=49B87B990208
	CLogView*	m_pLogView;		//日志窗口指针
	/** @brief           日志文件*/
	//##ModelId=49B87B99020D
	CMessageLog		m_File;
	/** @brief           是否已打开日志文件*/
	//##ModelId=49B87B990213
	BOOL		m_bLogOpened;

// Operations
public:
/*
 *  @brief   	SetLogPath	 设置日志文件路径 
 *  @param 		[In]a param of Type  CString&  要设置的路径
 *  @return 	void 
 */
	//##ModelId=49B87B990214
	void SetLogPath(CString& path);

/*
 *  @brief   	SetLogView	 设置日志窗口指针 
 *  @param 		[In]a param of Type  CLogView*  日志输出窗口指针
 *  @return 	void 
 */
	//##ModelId=49B87B990216
	void SetLogView(CLogView* pView);

/*
 *  @brief   	WriteLog	 写日志操作,供外部调用 
 *  @param 		[In]a param of Type  CString&  日志字符串
 *  @param 		[In]a param of Type  const int&  日志等级
 *  @return 	void 
 */
	//##ModelId=49B87B990218
	void WriteLog(CString& sLog, const int& nLV = 2);

/*
 *  @brief   	WriteLog	 写日志到日志文件
 *  @param 		[In]a param of Type  CString  日志内容
 *  @param 		[In]a param of Type  int  日志等级
 *  @return 	void 
 */
	//##ModelId=49B87B990223
	void WriteLogEx(CString sLog, int nLV = 2);

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
