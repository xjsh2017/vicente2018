//
// LogFile.h
//
// This is a Log  operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "define.h"
#include "Lock.h"
#include "XJFile.h"
#include "xjlib.h"

using namespace std;

/** @brief             日志文件默认容量(单位:字节)*/
#define  MAX_LOG_FILE_SIZE        0x800000

/** @brief              每一次写入日志的最大字节数*/
#define EVERY_LOG_MAX_LENGTH    100000

class CLogFile
{
public:

	CLogFile();
	virtual ~CLogFile();

	enum LogLevel {
		  nolog     =      0,          //不记录日志到文件
          error     =      1,          // 错误日志
          warning   =      2,          // 警告日志
		  trace     =      3,          // 跟踪日志
		  };

	//清楚所有日志内容
	virtual bool Clear();

	//设置日志级别
	virtual bool SetLogLevel(int nLevel);

	//获得日志级别
	virtual int  GetLogLevel();

	//设置日志存放路径
	virtual bool SetLogPath(const char * pszPath);

	//设置日志文件大小(参数单位:Kb)
	virtual bool SetLogSize(long nSize);

	
	//打开文件
	virtual BOOL Open();
	virtual BOOL Open(const char * pszFileName);

	//关闭文件
	virtual bool Close();

	//按照指定的级别格式化输入
	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);	

	//按照指定的级别输入字符串
	virtual void Add(const char * pchLog,int nCurLevel= CLogFile::trace);
	
	//按照指定的长度记录指定缓冲区的内容
	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = CLogFile::trace);

private:
	CXJFile   	*m_File;
	void		Flush( const char* data,int nCurLevel);
	void        Flush( const char* data,int nLen,int nCurLevel);

	//该日志输出级别
	int m_nLevel;
	
	//文件名
    char m_FileName[FILE_NAME_MAX_LEN];

	//文件路径
	char m_szLogPath[FILE_NAME_MAX_LEN];

	//互斥体
	CLock     m_Lock;

	//文件当前大小(单位:字节)
	long      m_nCurrenSize;

	//文件指定可以达到的最大值(单位:字节)
	long      m_nMaxSize;
};

#endif 