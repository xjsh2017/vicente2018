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
 * @defgroup 	CXJNet_Connector	��˵��
 * @{
 */
/**
 * @brief       ΪCNet�����������ɫ��facade�࣬��װ"��������ģʽ"CNet֧�ֵ��������ӻ���
 *				ʹ�ô�����������socket��Դ�������ӣ����ӳɹ����ʼ����������֧�����ӳ�ʱ
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
     *  @brief   	���캯��
     *  @return 	 
     */

    CXJNet_Connector();
    /*
     *  @brief   	��������
     *  @return 	 
     */

    virtual ~CXJNet_Connector();
    /*
     *  @brief   	���ӷ����
     *  @param 		[Out]a param of Type  CXJNet_Stream&  ���ڽ������ӵ�������
     *  @param 		[In]a param of Type  const char *  �����IP
     *  @param 		[In]a param of Type  int  ����˶˿ں�
     *  @param 		[In]a param of Type  int  ���ض˿ں�
     *  @param 		[In]a param of Type  int  ��ʱʱ�䣬��λs
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Connect(CXJNet_Stream& pNew_stream,const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);
	
    /*
     *  @brief   	���Ӻ��ò���
     *  @param 		[Out]a param of Type  CXJNet_Stream&  ���ڽ������ӵ�������
     *  @param 		[In]a param of Type  int  ��ʱʱ�䣬��λs
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool ShardConnectFinish( CXJNet_Stream& pNew_stream ,int nTimeout, bool bResult);
	
    /*
     *  @brief   	����ǰ�ò��� 
     *  @param 		[Out]a param of Type  CXJNet_Stream &  ���ڽ������ӵ�������
     *  @param 		[In]a param of Type  int  ��ʱʱ�䣬��λs
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SharedConnectStart( CXJNet_Stream& pNew_stream ,int nTimeout);
protected:

private:
};
/** @} */ //OVER
#endif // CXJNET_CONNECTOR_H
