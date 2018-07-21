/********************************************************************
	created:	2008/10/08
	created:	8:10:2008   0:35
	file base:	XJNet_Connector
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef CXJNET_CONNECTOR_H
#define CXJNET_CONNECTOR_H
#pragma   warning   (disable   :   4275) 
#include "XJNet_Stream.h"

/**
 * @defgroup 	CXJNet_Connector	类说明
 * @{
 */
/**
 * @brief       为CNet定义的主动角色的facade类，封装"主动连接模式"CNet支持的主动连接机制
 *				使用传入的流对象的socket资源发起连接，连接成功后初始化该流对象，支持连接超时
 * @author      qingch
 * @version     ver1.0
 * @date        08/10/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */


class CXJNet_Connector
{
public:
    /*
     *  @brief   	构造函数
     *  @return 	 
     */

    CXJNet_Connector();
    /*
     *  @brief   	析构函数
     *  @return 	 
     */

    virtual ~CXJNet_Connector();
    /*
     *  @brief   	连接服务端
     *  @param 		[Out]a param of Type  CXJNet_Stream&  用于建立连接的流对象
     *  @param 		[In]a param of Type  const char *  服务端IP
     *  @param 		[In]a param of Type  int  服务端端口号
     *  @param 		[In]a param of Type  int  本地端口号
     *  @param 		[In]a param of Type  int  超时时间，单位s
     *  @return 	bool true-成功 false-失败
     */

	bool Connect(CXJNet_Stream& pNew_stream,const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);
	
    /*
     *  @brief   	连接后置操作
     *  @param 		[Out]a param of Type  CXJNet_Stream&  用于建立连接的流对象
     *  @param 		[In]a param of Type  int  超时时间，单位s
     *  @return 	bool true-成功 false-失败
     */

	bool ShardConnectFinish( CXJNet_Stream& pNew_stream ,int nTimeout, bool bResult);
	
    /*
     *  @brief   	连接前置操作 
     *  @param 		[Out]a param of Type  CXJNet_Stream &  用于建立连接的流对象
     *  @param 		[In]a param of Type  int  超时时间，单位s
     *  @return 	bool true-成功 false-失败
     */

	bool SharedConnectStart( CXJNet_Stream& pNew_stream ,int nTimeout);
protected:

private:
};
/** @} */ //OVER
#endif // CXJNET_CONNECTOR_H
