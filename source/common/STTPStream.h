/********************************************************************
	created:	2008/09/10
	created:	10:9:2008   23:14
	file base:	STTPStream
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef STTPStream_h__
#define STTPStream_h__
#pragma warning (disable   :   4275)

#include "define.h"
#include "Net.h"
#include "XJLock.h"
#include "SttpDefine.h"
#include "SttpMsgParser.h"
#include "XJNet_Stream.h"
#include "MessageLog.h"

/**
 * @defgroup 	CSTTPStream	类说明
 * @{
 */
/**
 * @brief       STTP数据传输类
*  @author      qingch
 * @version     ver1.0
 * @date        08/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */


class CSTTPStream : public CXJNet_Stream  
{
public:
    /*
     *  @brief   	构造函数 
     *  @param 		[In]a param of Type  const char*  客户名,报文日志的目录已该客户名命名
     *  @param 		[In]a param of Type  bool         是否保存报文
     *  @return 	 
     */

	CSTTPStream(const char* pSzHost,bool pLogFlag,const char* pSzLogPath="../Xj_Log/");
    /*
     *  @brief   	析构函数
     *  @return 	 
     */

	virtual ~CSTTPStream();

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
     *  @brief   	发送一条STTP报文
     *  @param 		[In]a param of Type  const STTPMSG*  待发送的STTP报文
     *  @return 	int >0-成功发送的实际长度 <=0-发送失败 
     */

	int SendSTTPMessage(const STTPMSG* pMsg);
	
    /*
     *  @brief   	接收一条STTP报文
     *  @param 		[Out]a param of Type  STTPMSG*  保存收到的STTP报文
     *  @return 	int >0-成功接收的实际长度 <=0-接收失败
     */

	int RecvSTTPMessage(STTPMSG* pMsg);
    /*
     *  @brief   	设置日志路径
     *  @param 		[In]a param of Type  const char *  日志路径
     *  @return 	bool True-成功 False-失败
     */

	bool SetLogPath( const char * pszPath );

    /*
     *  @brief   	设置客户名
     *  @param 		[In]a param of Type  const char *  客户名,报文日志的目录已该客户名命名
     *  @return 	bool True-成功 False-失败
     */

	bool SetHostName( const char * pHostName);
private:
    /*
     *  @brief   	输出报文到日志文件
     *  @param 		[In]a param of Type  const STTPMSG *  待输出的报文
     *  @param 		[In]a param of Type  const LOG_BUFFER_HEAD&  输出的日志头
     *  @return 	void 
     */

	void  PrintSttpMsg(const STTPMSG * pMsg,const LOG_BUFFER_HEAD& pHead);
	
    /*
     *  @brief   	写错误日志信息
     *  @param 		[In]a param of Type  const char*  待输出的日志
     *  @return 	void 
     */

	void WriteErrorLog(const char* chLog );
    /*
     *  @brief   	写报文日志
     *  @param 		[In]a param of Type  const char*  待输出的报文
     *  @param 		[In]a param of Type  int  报文长度
     *  @return 	void 
     */

	void WriteMessageLog(const string& strLog, int iSizeOfStrLog );

	/** @brief  			是否记录日志标记 */
	bool					m_bLog;
	/** @brief  			报文日志对象 */
	CMessageLog				m_LogFile;
	/** @brief  			接收锁 */
	CXJLock					m_RecvLock;
	/** @brief  			发送锁 */
	CXJLock					m_SendLock;
	/** @brief  			发送锁 */
	CXJLock					m_LogLock;

};
/** @} */ //OVER
#endif // STTPStream_h__



