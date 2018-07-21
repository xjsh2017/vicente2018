/********************************************************************
	created:	2008/09/09
	created:	9:9:2008   14:06
	file base:	BiServerConnector
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef STTPConnector_h__
#define STTPConnector_h__
#pragma   warning   (disable   :   4275)

#include "BiNetServerConnector.h"
//#include "STTPStream.h"
#include "XJNet_Stream.h"

const int SERVER_ONLINE_UNK = -1;
const int SERVER_ONLINE_A = 0;
const int SERVER_ONLINE_B = 1;

/**
* @defgroup    CBiServerConnector 双服务器连接器
* @{
*/

/**
* @brief       双服务器连接器，可保存双机双网信息，提供连接方法处理双机双网连接
* @author      程清
* @version     ver1.0
* @date        9:9:2008
*
* example
* @code
*     STRUCT_DESTINATION_SERVER pDestInfo;
*	  CBiServerConnector pConnect(pDestInfo); 
*     CSTTPStream pStream("hostname",true); 
*	  if(!pConnect.connect(pStream)){
*        //连接失败的处理
*	  }
*     else{
*        //连接成功后的处理，如发送STTP报文
*        STTPMSG pMsg;
*        pStream.SendSTTPMessage(pMsg)
*	  }
* @endcode
*/

//##ModelId=489F07D0008C
class CBiServerConnector
{
  public:
	/**
	 * @brief        构造函数
	 * @param[in]    pDestServer 包含目标服务端信息的结构体
	 * @return       无
	 */
	CBiServerConnector(const STRUCT_DESTINATION_SERVER& pDestServer , const char* pLogPath = "Xj_Log/");
	/**
	* @brief        析构函数
	* @param[in]    无
	* @return       无
	*/
	~CBiServerConnector();
    //##ModelId=489F08560271
    /**
    * @brief         连接服务端
    * @param[in]     pStream 用于连接的SttpStream对象引用
    * @param[In]     nTimeout超时时间，单位s
	* @param[In]     pError 错误消息
	* @param[In]	 int Option 连接选项用于控制连接顺序
    * @return        是否连接成功 true-成功 false-失败
    */
    bool connect(CXJNet_Stream& pStream , int nTimeout=-1,char* pError=NULL,int Option=0);
    /**
    * @brief         获取在线标志
    * @return       在线服务器编号
    */	
	int GetOnlineServer();
    /**
    * @brief         获取是否为双服务器标志
    * @return       true: 真 fasle: 假
    */	
	bool GetHasReservedServer();
  private:
	CBiServerConnector(const CBiServerConnector&);
	CBiServerConnector& operator= (const CBiServerConnector&);

    //##ModelId=489F09DD0251
	/** @brief           主服务端*/
    CBiNetServerConnector* m_pServerA;

    //##ModelId=489F09E50290
	/** @brief           备服务端*/
    CBiNetServerConnector* m_pServerB;

    //##ModelId=489F09EA02CE
	/** @brief           是否有备用服务端*/
    bool m_bHasReservedServer;

	/** @brief           在线服务器标志*/
    int m_iOnLineServer;

};

/** @} */ //OVER

#endif // STTPConnector_h__

