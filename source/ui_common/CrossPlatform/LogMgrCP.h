// LogMgrCP.h: interface for the LogMgrCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_)
#define AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../common/LogFile.h"
#include "../../common/XJLock.h"
#include "../../common/MessageLog.h"
#include "../../common/XJString.h"

const int log_level_error     =      1;          // 错误日志
const int log_level_warning   =      2;          // 警告日志
const int log_level_trace     =      3;          // 跟踪日志
void WriteLog_CP(CXJString sLog, int nLogLevel = 3);

/**
 * @brief    MaintenanceData  跨平台版本日志管理               
 * @author   lyh
 * @version   1.0
 * @date      2012/7/3
 */
class LogMgrCP  
{
private:
	LogMgrCP();
public:
	virtual ~LogMgrCP();

///////////////////实现单件模式/////////////////////////////
private:
	/** @brief			本类静态对象*/
	static LogMgrCP* m_instance;
	/** @brief			用于getInstance函数的锁*/
	static CXJLock* m_InstanceLock;
public:
	/*
	* @brief	getInstance	取得LogMgrCP对象指针
	* @return   LogMgrCP*	LogMgrCP对象指针
	*/
	static LogMgrCP* getInstance();
///////////////////实现单件模式 end//////////////////////////

private:
	/** @brief			日志文件*/
	CMessageLog m_File;
public:
	/*
	* @brief	SetLogPath	设置日志文件路径
	* @param	[In]a param of Type CXJString sPath	路径	
	* @param	[In]a param of Type CXJString sFolder 日志文件夹名称		
	* @return   void	
	*/
	void SetLogPath(CXJString sPath = "./", CXJString sFolder = "Temp_Log");

	/*
	* @brief	WriteLog	写日志操作,供外部调用
	* @param	[In]a param of Type CXJString sLog	日志字符串	
	* @param	[In]a param of Type int nLogLevel	日志等级	
	* @return   void	
	*/
	void WriteLog(CXJString sLog, int nLogLevel);

	/*
	* @brief	SetLogLevel	设置日志文件的记录等级
	* @param	[In]a param of Type int nLogLevel 日志等级		
	* @return   void	
	*/
	void SetLogLevel(int nLogLevel);
};

#endif // !defined(AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_)
