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
 * @defgroup 	CSTTPStream	��˵��
 * @{
 */
/**
 * @brief       STTP���ݴ�����
*  @author      qingch
 * @version     ver1.0
 * @date        08/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */


class CSTTPStream : public CXJNet_Stream  
{
public:
    /*
     *  @brief   	���캯�� 
     *  @param 		[In]a param of Type  const char*  �ͻ���,������־��Ŀ¼�Ѹÿͻ�������
     *  @param 		[In]a param of Type  bool         �Ƿ񱣴汨��
     *  @return 	 
     */

	CSTTPStream(const char* pSzHost,bool pLogFlag,const char* pSzLogPath="../Xj_Log/");
    /*
     *  @brief   	��������
     *  @return 	 
     */

	virtual ~CSTTPStream();

    /*
     *  @brief   	�������� 
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	virtual bool Open();

    /*
     *  @brief   	�ر������� 
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	virtual bool Close();

    /*
     *  @brief   	����һ��STTP����
     *  @param 		[In]a param of Type  const STTPMSG*  �����͵�STTP����
     *  @return 	int >0-�ɹ����͵�ʵ�ʳ��� <=0-����ʧ�� 
     */

	int SendSTTPMessage(const STTPMSG* pMsg);
	
    /*
     *  @brief   	����һ��STTP����
     *  @param 		[Out]a param of Type  STTPMSG*  �����յ���STTP����
     *  @return 	int >0-�ɹ����յ�ʵ�ʳ��� <=0-����ʧ��
     */

	int RecvSTTPMessage(STTPMSG* pMsg);
    /*
     *  @brief   	������־·��
     *  @param 		[In]a param of Type  const char *  ��־·��
     *  @return 	bool True-�ɹ� False-ʧ��
     */

	bool SetLogPath( const char * pszPath );

    /*
     *  @brief   	���ÿͻ���
     *  @param 		[In]a param of Type  const char *  �ͻ���,������־��Ŀ¼�Ѹÿͻ�������
     *  @return 	bool True-�ɹ� False-ʧ��
     */

	bool SetHostName( const char * pHostName);
private:
    /*
     *  @brief   	������ĵ���־�ļ�
     *  @param 		[In]a param of Type  const STTPMSG *  ������ı���
     *  @param 		[In]a param of Type  const LOG_BUFFER_HEAD&  �������־ͷ
     *  @return 	void 
     */

	void  PrintSttpMsg(const STTPMSG * pMsg,const LOG_BUFFER_HEAD& pHead);
	
    /*
     *  @brief   	д������־��Ϣ
     *  @param 		[In]a param of Type  const char*  ���������־
     *  @return 	void 
     */

	void WriteErrorLog(const char* chLog );
    /*
     *  @brief   	д������־
     *  @param 		[In]a param of Type  const char*  ������ı���
     *  @param 		[In]a param of Type  int  ���ĳ���
     *  @return 	void 
     */

	void WriteMessageLog(const string& strLog, int iSizeOfStrLog );

	/** @brief  			�Ƿ��¼��־��� */
	bool					m_bLog;
	/** @brief  			������־���� */
	CMessageLog				m_LogFile;
	/** @brief  			������ */
	CXJLock					m_RecvLock;
	/** @brief  			������ */
	CXJLock					m_SendLock;
	/** @brief  			������ */
	CXJLock					m_LogLock;

};
/** @} */ //OVER
#endif // STTPStream_h__



