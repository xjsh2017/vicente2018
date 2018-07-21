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
 * @defgroup 	CXJNet_Acceptor	��˵��
 * @{
 */
/**
 * @brief       ΪCNet����ı������ӽ�ɫ��facade�࣬�������ԶԵ����Ӷ˵����ӣ�
 *              �������ӽ������ʼ��CXJNet_Stream���󡣸�����ṩ�������ӽ�ɫ
 *				�����ڱ������ӵ�CNet�������������Ӻ������շ���
 * @author      qingch
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

class CXJNet_Acceptor
{
public:
    /*
     *  @brief   	���캯��
     *  @return 	 
     */

    CXJNet_Acceptor();

	CXJNet_Acceptor(const char* pLogPath , int pLogLevel = CLogFile::error);
    /*
     *  @brief   	��������
     *  @return 	 
     */

    virtual ~CXJNet_Acceptor();
    /*
     *  @brief   	Open ��ʼ������ģʽ
     *  @param 		[In]const NETADDR&  ���ڱ��������ĵ�ַ
     *  @return 	bool true-�ɹ� false-ʧ��
     */

    /*
     *  @brief   	Open ��ʼ������ģʽ 
     *  @param 		[In]u_short       �����˿�
     *  @param 		[In]const char *  ����IP
     *  @return 	bool rue-�ɹ� false-ʧ��
     */

	bool Open(u_short port,const char * host=NULL  , int nQueue = SOMAXCONN);
    /*
     *  @brief   	Accept ����������
     *  @param 		[Out]CXJNet_Stream&  ���ӽ������������ݴ����stream����
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Accept(CXJNet_Stream& pNew_stream);
    /*
     *  @brief   	�رձ�������
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Close();

    /*
	*  @brief   	������־·��
	*  @param 		[In]a param of Type  const char *  ��־·��
	*  @return		void
	*/
	
	void SetLogPath( const char * pszPath );
protected:
private:
	CXJNet_Acceptor(const CXJNet_Acceptor&);
	CXJNet_Acceptor& operator= (const CXJNet_Acceptor&);
	/** @brief           CNet����*/
	CNet	m_Net;
};

#endif // CXJNET_ACCEPTOR_H
