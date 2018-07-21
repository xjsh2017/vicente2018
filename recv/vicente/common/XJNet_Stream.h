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
 * @defgroup 	CXJNet_Stream	��˵��
 * @{
 */
/**
 * @brief       ΪCNet�����ͨ�Ž�ɫ��facade�࣬��װ"����ģʽ"CNet֧�ֵ����ݴ������
 *				֧�����ݵķ��������
 *				֧��"����"��"��ʱI/O"����
 *				��ʼ����"������ɫ"CXJNet_Connector��"������ɫ"CXJNet_Acceptor�Ķ������
 * @author      qingch
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */


class CXJNet_Stream
{
public:
    /*
     *  @brief   	���캯��
     *  @return 	 
     */

    CXJNet_Stream();
	
    CXJNet_Stream(const char* pLogPath , int pLogLevel = CLogFile::error);
    /*
     *  @brief   	��������
     *  @return 	 
     */

    virtual ~CXJNet_Stream();
    /*
     *  @brief   	��ȡCNet��Դ
     *  @return 	CNet& ����ģʽʹ�õ�CNet���������
     */

	CNet& GetNet();
    /*
     *  @brief   	��ȡ���ص�ַ��Ϣ 
     *  @param 		[Out]NETADDR*  ��ַ����ָ��
     *  @return 	int 0-�ɹ� -1-ʧ��
     */

	int GetLocalAddr(NETADDR* pAddr);
    /*
     *  @brief   	��ȡ�Զ˵�ַ��Ϣ
     *  @param 		[Out]NETADDR*  ��ַ����ָ��
     *  @return 	int 0-�ɹ� -1-ʧ��
     */

	int GetRemoteAddr(NETADDR* pAddr) ;
    /*
     *  @brief   	����CNet��ѡ��
     *  @param 		[In]int  Ҫ���õ�ѡ������
     *  @param 		[In]int  ѡ��ֵ,���ó�ʱʱΪ��ʱʱ��(ms)
		                     ����ѡ��ʱ��0
     *  @param 		[In]int  ����
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SetOptions(int nFlag,int nTime,int Reserve=0); 
    /*
     *  @brief   	����ָ����������ָ���ֽڵ�����
     *  @param 		[In]a param of Type  void *  ָ���ķ��ͻ�����ָ��
     *  @param 		[In]a param of Type  int     ָ�������ֽ���
     *  @param 		[In]a param of Type  int     ��ʱʱ��
     *  @return 	int ����ʵ�ʷ������ֽ��� (<=nLen)
     */

    int  Write(void * pBuf,int nLen,int nTimeout=-1);
    /*
     *  @brief   	��ȡָ���ֽڵ����ݵ�ָ����������
     *  @param 		[In]a param of Type  void *  ��Ž������ݵĻ�����
     *  @param 		[In]a param of Type  int     ָ����ȡ�ֽ���
     *  @param 		[In]a param of Type  int     ��ʱʱ��
     *  @return 	int ����ʵ�ʶ������ֽ��� (<=nLen)
     */

	int  Read(void * pBuf,int nLen,int nTimeout=-1);

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
	*  @brief   	������־·��
	*  @param 		[In]a param of Type  const char *  ��־·��
	*  @return		void
	*/
	
	void SetLogPath( const char * pszPath );
protected:
    /*
     *  @brief   	������ǰ�ò��� 
     *  @param 		[In]a param of Type  int  �趨�ĳ�ʱʱ��
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SharedReadStart(int nTimeout);
    /*
     *  @brief   	���������ò���
     *  @param 		[In]a param of Type  int  �趨�ĳ�ʱʱ��
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SharedReadFinished(int nTimeout);
    /*
     *  @brief   	д����ǰ�ò���
     *  @param 		[In]a param of Type  int  �趨�ĳ�ʱʱ��
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SharedWriteStart(int nTimeout);
    /*
     *  @brief   	д�������ò���
     *  @param 		[In]a param of Type  int  �趨�ĳ�ʱʱ��
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool SharedWriteFinished(int nTimeout);
    
    int  SendToPeer(void * pBuf,int nLen);
    
	int  RecvFromPeer(void * pBuf,int nLen);
private:
	CXJNet_Stream(const CXJNet_Stream&);
	CXJNet_Stream& operator=(const CXJNet_Stream&);
	/** @brief           ��ȡ��ʱ*/
	int m_iReadTimeOut;
	/** @brief           ���ͳ�ʱ*/
	int m_iWriteTimeOut;
	/** @brief           CNET����*/
	CNet	m_Net;

};
/** @} */ //OVER
#endif // CXJNET_STREAM_H
