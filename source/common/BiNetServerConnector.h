/********************************************************************
	created:	2008/09/09
	created:	9:9:2008   14:05
	file base:	CBiNetServerConnector
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef STTPServer_h__
#define STTPServer_h__
#pragma   warning   (disable   :   4275)

#include "define.h"
#include "SttpDefine.h"
#include "Net.h"
#include "XJNet_Stream.h"
#include "XJNet_Connector.h"

/**
* @defgroup    CBiNetServerConnector 双网服务端连接器
* @{
*/

/**
* @brief       保存服务端的地址信息、并提供双IP连接方法
* @author      程清
* @version     ver1.0
* @date        9:9:2008
*
* example
* @code
*  
*
* @endcode
*/

//##ModelId=489F0A4A033C
class CBiNetServerConnector
{
  public:
	/**
	 * @brief        构造函数
	 * @param[in]    pServerInfo 包含服务端地址信息的结构体
	 * @return       无
	 */
	CBiNetServerConnector(const STRUCT_SERVER_INFO& pServerInfo , const char* pLogPath = "Xj_Log/");
	/**
	* @brief        析构函数
	* @param[in]    无
	* @return       无
	*/
	~CBiNetServerConnector();
    //##ModelId=489F0C110290
    /**
    * @brief         连接服务端
    * @param[in]     pNet 用于连接的net资源
    * @param[In]     nTimeout超时时间，单位s
    * @return        是否连接成功 true-成功 false-失败
    */
    bool connect(CXJNet_Stream& pStream , int nTimeout=-1, char* pError=NULL);

  private:
	CBiNetServerConnector(const CBiNetServerConnector&);
	CBiNetServerConnector& operator=(const CBiNetServerConnector&);
	/** @brief           是否有备用地址*/
	bool m_bHasReservedNet;
	/** @brief           主网络地址*/	  
    NETADDR m_NetAddrA;
	/** @brief           备网络地址*/
    NETADDR m_NetAddrB;
	/** @brief  		 日志记录 */
	//CLogFile m_LogFile;
	/** @brief  		 Net连接器 */
	CXJNet_Connector m_Connector;

};

/** @} */ //OVER

#endif // STTPServer_h__

