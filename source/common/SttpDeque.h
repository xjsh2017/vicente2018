//
// Deque.h
//
// This is a STTP deque Operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//
#ifndef INCLUDE_H_DEQUE
#define INCLUDE_H_DEQUE

#include "define.h"
#include "LogFile.h"
#include "Lock.h"
#include "SttpDefine.h"

using namespace std;

class CSttpDeque
{
public:

	//构造析构函数
	CSttpDeque(int m_Size = 50);
	virtual ~CSttpDeque();

	//删除头一个元素
	bool RemoveHead();
 
	//增加一个元素到尾部
	bool Insert(STTP_DEQUE_MSG &item); 

	//取出一个元素并删除
	bool Extract(STTP_DEQUE_MSG &item,int nIndex=0);

	//取出一个元素但不删除
	bool Fetch(STTP_DEQUE_MSG &item,int nIndex=0);

	//清空所有元素
	bool Clear();

	//确认该队列是否有元素
	bool IsEmpty() ;

	//返回该队列当前元素个数
	int size() ;

	//设置队列的最大容量
	bool SetMaxSize(UINT nNum);
	
	//设置日志级别
	void SetLogLevel(int nLevel);
	
	//修改元素内容
	bool SetAt(STTP_DEQUE_MSG item,int nIndex=0);
	
	//获得日志输出级别
	int         GetLogLevel();

	//设置日志路径
	void        SetLogPath(char *pPath);
	
	//初始化日志文件
	bool        SetLogFileName(char * pFileName);

private:

	//队列允许放入的最大元素个数
	 UINT                m_MaxSize;

	 //list STL
	 list <STTP_DEQUE_MSG>    m_mDeque;

	 //锁
	 CLock               m_Lock;

	 //日志记录
	 CLogFile            m_LogFile;

protected:

};

#endif