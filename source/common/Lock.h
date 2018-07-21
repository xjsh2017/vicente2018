//
// Lock.h
//
// This is a multithreading lock Operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef LOCK_H_HEADER_INCLUDED_BAD5A27F
#define LOCK_H_HEADER_INCLUDED_BAD5A27F

#include "define.h"

//##ModelId=4519DA5403C4
//##Documentation
//## 互斥体类，解决线程间的排他访问
class CLock
{
  public:
	//构造函数
	 CLock();

	//析构函数
	~CLock();

	//初始化锁
    //##ModelId=4519E3D00375
    BOOL init(char * phost = NULL);

	//删除锁
    //##ModelId=4519E3E701F2
    BOOL del();

	//阻塞等待直到取得拥有权
    //##ModelId=4519E3F40088
    BOOL lock();

	//无论是否取得拥有权,调用马上返回
	BOOL trylock();

	//让出拥有权
    //##ModelId=4519E40E003F
    BOOL unlock();

  private:
    //##ModelId=4519E338018C
    MUTEX m_mutex;

	//该mutex所属的宿主
	char  ch_host[FILE_NAME_MAX_LEN];

	//初始化标志
	BOOL  bInit;

};



#endif /* LOCK_H_HEADER_INCLUDED_BAD5A27F */
