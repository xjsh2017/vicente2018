/********************************************************************
	created:	2008/10/08
	created:	8:10:2008   0:33
	file base:	XJNet_Stream
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef CXJNET_STREAM_H
#define CXJNET_STREAM_H
#pragma   warning   (disable   :   4275) 
#include "Net.h"

/**
 * @defgroup 	CXJNet_Stream	类说明
 * @{
 */
/**
 * @brief       为CNet定义的通信角色的facade类，封装"数据模式"CNet支持的数据传输机制
 *				支持数据的发送与接收
 *				支持"阻塞"、"定时I/O"操作
 *				初始化由"主动角色"CXJNet_Connector或"被动角色"CXJNet_Acceptor的对象完成
 * @author      qingch
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */


class CXJNet_Stream
{
public:
    /*
     *  @brief   	构造函数
     *  @return 	 
     */

    CXJNet_Stream();
	
    CXJNet_Stream(const char* pLogPath , int pLogLevel = CLogFile::error);
    /*
     *  @brief   	析构函数
     *  @return 	 
     */

    virtual ~CXJNet_Stream();
    /*
     *  @brief   	获取CNet资源
     *  @return 	CNet& 数据模式使用的CNet对象的引用
     */

	CNet& GetNet();
    /*
     *  @brief   	获取本地地址信息 
     *  @param 		[Out]NETADDR*  地址对象指针
     *  @return 	int 0-成功 -1-失败
     */

	int GetLocalAddr(NETADDR* pAddr);
    /*
     *  @brief   	获取对端地址信息
     *  @param 		[Out]NETADDR*  地址对象指针
     *  @return 	int 0-成功 -1-失败
     */

	int GetRemoteAddr(NETADDR* pAddr) ;
    /*
     *  @brief   	设置CNet各选项
     *  @param 		[In]int  要设置的选项名称
     *  @param 		[In]int  选项值,设置超时时为超时时间(ms)
		                     其它选项时清0
     *  @param 		[In]int  保留
     *  @return 	bool true-成功 false-失败
     */

	bool SetOptions(int nFlag,int nTime,int Reserve=0); 
    /*
     *  @brief   	发送指定缓冲区中指定字节的数据
     *  @param 		[In]a param of Type  void *  指定的发送缓冲区指针
     *  @param 		[In]a param of Type  int     指定发送字节数
     *  @param 		[In]a param of Type  int     超时时间
     *  @return 	int 返回实际发出的字节数 (<=nLen)
     */

    int  Write(void * pBuf,int nLen,int nTimeout=-1);
    /*
     *  @brief   	读取指定字节的数据到指定缓冲区中
     *  @param 		[In]a param of Type  void *  存放接收数据的缓冲区
     *  @param 		[In]a param of Type  int     指定读取字节数
     *  @param 		[In]a param of Type  int     超时时间
     *  @return 	int 返回实际读到的字节数 (<=nLen)
     */

	int  Read(void * pBuf,int nLen,int nTimeout=-1);

    /*
     *  @brief   	打开流对象 
     *  @return 	bool true-成功 false-失败
     */

	virtual bool Open();

    /*
     *  @brief   	关闭流对象 
     *  @return 	bool true-成功 false-失败
     */

	virtual bool Close();


    /*
	*  @brief   	设置日志路径
	*  @param 		[In]a param of Type  const char *  日志路径
	*  @return		void
	*/
	
	void SetLogPath( const char * pszPath );
protected:
    /*
     *  @brief   	读操作前置操作 
     *  @param 		[In]a param of Type  int  设定的超时时间
     *  @return 	bool true-成功 false-失败
     */

	bool SharedReadStart(int nTimeout);
    /*
     *  @brief   	读操作后置操作
     *  @param 		[In]a param of Type  int  设定的超时时间
     *  @return 	bool true-成功 false-失败
     */

	bool SharedReadFinished(int nTimeout);
    /*
     *  @brief   	写操作前置操作
     *  @param 		[In]a param of Type  int  设定的超时时间
     *  @return 	bool true-成功 false-失败
     */

	bool SharedWriteStart(int nTimeout);
    /*
     *  @brief   	写操作后置操作
     *  @param 		[In]a param of Type  int  设定的超时时间
     *  @return 	bool true-成功 false-失败
     */

	bool SharedWriteFinished(int nTimeout);
    
    int  SendToPeer(void * pBuf,int nLen);
    
	int  RecvFromPeer(void * pBuf,int nLen);
private:
	CXJNet_Stream(const CXJNet_Stream&);
	CXJNet_Stream& operator=(const CXJNet_Stream&);
	/** @brief           读取超时*/
	int m_iReadTimeOut;
	/** @brief           发送超时*/
	int m_iWriteTimeOut;
	/** @brief           CNET对象*/
	CNet	m_Net;

};
/** @} */ //OVER
#endif // CXJNET_STREAM_H
