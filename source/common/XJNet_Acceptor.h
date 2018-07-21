/********************************************************************
	created:	2008/10/06
	created:	6:10:2008   1:07
	file base:	XJNet_Acceptor
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef CXJNET_ACCEPTOR_H
#define CXJNET_ACCEPTOR_H
#pragma   warning   (disable   :   4275) 
#include "Net.h"
#include "XJNet_Stream.h"

/**
 * @defgroup 	CXJNet_Acceptor	类说明
 * @{
 */
/**
 * @brief       为CNet定义的被动连接角色的facade类，接受来自对等连接端的连接，
 *              并在连接建立后初始化CXJNet_Stream对象。该类仅提供被动连接角色
 *				避免在被动连接的CNet上误用主动连接和数据收发。
 * @author      qingch
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

class CXJNet_Acceptor
{
public:
    /*
     *  @brief   	构造函数
     *  @return 	 
     */

    CXJNet_Acceptor();

	CXJNet_Acceptor(const char* pLogPath , int pLogLevel = CLogFile::error);
    /*
     *  @brief   	析构函数
     *  @return 	 
     */

    virtual ~CXJNet_Acceptor();
    /*
     *  @brief   	Open 初始化被动模式
     *  @param 		[In]const NETADDR&  用于被动侦听的地址
     *  @return 	bool true-成功 false-失败
     */

    /*
     *  @brief   	Open 初始化被动模式 
     *  @param 		[In]u_short       侦听端口
     *  @param 		[In]const char *  本地IP
     *  @return 	bool rue-成功 false-失败
     */

	bool Open(u_short port,const char * host=NULL  , int nQueue = SOMAXCONN);
    /*
     *  @brief   	Accept 接受新连接
     *  @param 		[Out]CXJNet_Stream&  连接建立后用于数据传输的stream对象
     *  @return 	bool true-成功 false-失败
     */

	bool Accept(CXJNet_Stream& pNew_stream);
    /*
     *  @brief   	关闭被动连接
     *  @return 	bool true-成功 false-失败
     */

	bool Close();

    /*
	*  @brief   	设置日志路径
	*  @param 		[In]a param of Type  const char *  日志路径
	*  @return		void
	*/
	
	void SetLogPath( const char * pszPath );
protected:
private:
	CXJNet_Acceptor(const CXJNet_Acceptor&);
	CXJNet_Acceptor& operator= (const CXJNet_Acceptor&);
	/** @brief           CNet对象*/
	CNet	m_Net;
};

#endif // CXJNET_ACCEPTOR_H
