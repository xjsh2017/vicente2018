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
* @defgroup    CBiNetServerConnector ˫�������������
* @{
*/

/**
* @brief       �������˵ĵ�ַ��Ϣ�����ṩ˫IP���ӷ���
* @author      ����
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
	 * @brief        ���캯��
	 * @param[in]    pServerInfo ��������˵�ַ��Ϣ�Ľṹ��
	 * @return       ��
	 */
	CBiNetServerConnector(const STRUCT_SERVER_INFO& pServerInfo , const char* pLogPath = "Xj_Log/");
	/**
	* @brief        ��������
	* @param[in]    ��
	* @return       ��
	*/
	~CBiNetServerConnector();
    //##ModelId=489F0C110290
    /**
    * @brief         ���ӷ����
    * @param[in]     pNet �������ӵ�net��Դ
    * @param[In]     nTimeout��ʱʱ�䣬��λs
    * @return        �Ƿ����ӳɹ� true-�ɹ� false-ʧ��
    */
    bool connect(CXJNet_Stream& pStream , int nTimeout=-1, char* pError=NULL);

  private:
	CBiNetServerConnector(const CBiNetServerConnector&);
	CBiNetServerConnector& operator=(const CBiNetServerConnector&);
	/** @brief           �Ƿ��б��õ�ַ*/
	bool m_bHasReservedNet;
	/** @brief           �������ַ*/	  
    NETADDR m_NetAddrA;
	/** @brief           �������ַ*/
    NETADDR m_NetAddrB;
	/** @brief  		 ��־��¼ */
	//CLogFile m_LogFile;
	/** @brief  		 Net������ */
	CXJNet_Connector m_Connector;

};

/** @} */ //OVER

#endif // STTPServer_h__

