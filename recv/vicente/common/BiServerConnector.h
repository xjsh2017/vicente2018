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
* @defgroup    CBiServerConnector ˫������������
* @{
*/

/**
* @brief       ˫���������������ɱ���˫��˫����Ϣ���ṩ���ӷ�������˫��˫������
* @author      ����
* @version     ver1.0
* @date        9:9:2008
*
* example
* @code
*     STRUCT_DESTINATION_SERVER pDestInfo;
*	  CBiServerConnector pConnect(pDestInfo); 
*     CSTTPStream pStream("hostname",true); 
*	  if(!pConnect.connect(pStream)){
*        //����ʧ�ܵĴ���
*	  }
*     else{
*        //���ӳɹ���Ĵ����緢��STTP����
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
	 * @brief        ���캯��
	 * @param[in]    pDestServer ����Ŀ��������Ϣ�Ľṹ��
	 * @return       ��
	 */
	CBiServerConnector(const STRUCT_DESTINATION_SERVER& pDestServer , const char* pLogPath = "Xj_Log/");
	/**
	* @brief        ��������
	* @param[in]    ��
	* @return       ��
	*/
	~CBiServerConnector();
    //##ModelId=489F08560271
    /**
    * @brief         ���ӷ����
    * @param[in]     pStream �������ӵ�SttpStream��������
    * @param[In]     nTimeout��ʱʱ�䣬��λs
	* @param[In]     pError ������Ϣ
	* @param[In]	 int Option ����ѡ�����ڿ�������˳��
    * @return        �Ƿ����ӳɹ� true-�ɹ� false-ʧ��
    */
    bool connect(CXJNet_Stream& pStream , int nTimeout=-1,char* pError=NULL,int Option=0);
    /**
    * @brief         ��ȡ���߱�־
    * @return       ���߷��������
    */	
	int GetOnlineServer();
    /**
    * @brief         ��ȡ�Ƿ�Ϊ˫��������־
    * @return       true: �� fasle: ��
    */	
	bool GetHasReservedServer();
  private:
	CBiServerConnector(const CBiServerConnector&);
	CBiServerConnector& operator= (const CBiServerConnector&);

    //##ModelId=489F09DD0251
	/** @brief           �������*/
    CBiNetServerConnector* m_pServerA;

    //##ModelId=489F09E50290
	/** @brief           �������*/
    CBiNetServerConnector* m_pServerB;

    //##ModelId=489F09EA02CE
	/** @brief           �Ƿ��б��÷����*/
    bool m_bHasReservedServer;

	/** @brief           ���߷�������־*/
    int m_iOnLineServer;

};

/** @} */ //OVER

#endif // STTPConnector_h__

