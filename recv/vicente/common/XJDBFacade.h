/********************************************************************
	created:	2008/10/13
	created:	13:10:2008   15:41
	file base:	XJDBFacade
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#pragma  warning	(disable : 4275) 

#ifndef CXJDBFACADE_H
#define CXJDBFACADE_H

#include "define.h"
#include "LogFile.h"
#include "MemSet.h"
#include "XJSttpNetAPI.h"
#ifdef OS_LINUX

/** @brief            ���ݿ���ʽӿڿ����ƶ���*/
#define LIB_DBFACADE_NAME              "libXJDBFacade.so"

#endif

#ifdef OS_WINDOWS

/** @brief            ���ݿ���ʽӿڿ����ƶ���*/
#define LIB_DBFACADE_NAME              "XJDBFacade.dll"

#endif

/** @brief            DBFacade��Connect��������*/
typedef int (*PFUNCONNECT)(int ,char* );

/** @brief            DBFacade��SetLogCofig��������*/
typedef int (*PFUNSETLOGCONFIG)(char*,char* );

/** @brief            DBFacade��SelectData��������*/
typedef int (*PFUNSELECT)(int , SQL_DATA& , char* , CMemSet* );

/** @brief            DBFacade��MultiSelectData��������*/
typedef int (*PFUNMULTISELECT)(char* , MutiSQL_DATA& , char* );

/** @brief            DBFacade��UpdateData��������*/
typedef int (*PFUNUPDATE)(int , SQL_DATA& , char*);

/** @brief            DBFacade��InsertData��������*/
typedef int (*PFUNINSERT)(int , SQL_DATA& , char*);

/** @brief            DBFacade��DeleteData��������*/
typedef int (*PFUNDELETE)(int , SQL_DATA& , char*);

/** @brief            DBFacade��ExecuteSql��������*/
typedef int (*PFUNEXECUTESQL)(MutiSQL_DATA& , char* , CMemSet * );

/** @brief            DBFacade��RealDataInsert��������*/
typedef int (*PFUNRDINSERT)(STTP_FULL_DATA& , char* );

/** @brief            DBFacade��RealDataSelect��������*/
typedef int (*PFUNRDSELECT)(int,SQL_DATA&,REALDATA_CONDITION&,char*,CMemSet*);

/** @brief            DBFacade��InsertMultiData��������*/
typedef int (*PFUNINSERTMULTIDATA)(int,STTP_FULL_DATA&,char*);


/**
 * @defgroup 	CXJDBFacade	��˵��
 * @{
 */
/**
 * @brief       ���أ�ʹ��DBFacade��̬�����
*  @author      qingch
 * @version     ver1.0
 * @date        13/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

class CXJDBFacade
{
public:
    /*
     *  @brief   	���캯�� 
     *  @param 		[In]a param of Type  CLogFile*  �������־����ָ��
     *  @return 	 
     */

    CXJDBFacade(CLogFile* pLogFile);
    /*
     *  @brief   	��������	 
     *  @return 	virtual 
     */

    virtual ~CXJDBFacade();
    /*
     *  @brief   	���ض�̬��͸�����ָ��
     *  @return 	int 0-�ɹ� ����-�������
     */

	int LoadLib();
    /*
     *  @brief   	��̬���ѯ����
     *  @param 		[In]a param of Type  int  ���ݿ����
     *  @param 		[In]a param of Type  SQL_DATA&  ����Ĳ�ѯ����
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @param 		[Out]a param of Type  CMemSet*  ��ѯ���Ľ�����ݼ�
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Select( int pTableNo , SQL_DATA& pSqlData , char* pError , CMemSet* pMemSet );
    /*
     *  @brief   	��̬����º���
     *  @param 		[In]a param of Type  int  ���ݿ����
     *  @param 		[In]a param of Type  SQL_DATA&  ����Ĳ�ѯ����
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Update( int pTableNo , SQL_DATA& pSqlData , char* pError );
    /*
     *  @brief   	��̬����뺯��
     *  @param 		[In]a param of Type  int  ���ݿ����
     *  @param 		[In]a param of Type  SQL_DATA&  ����Ĳ�ѯ����
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Insert( int pTableNo , SQL_DATA& pSqlData , char* pError );

    /*
     *  @brief   	��̬��ɾ������
     *  @param 		[In]a param of Type  int  ���ݿ����
     *  @param 		[In]a param of Type  SQL_DATA&  ����Ĳ�ѯ����
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool Delete( int pTableNo , SQL_DATA& pSqlData , char* pError );

    /*
     *  @brief   	��̬��ִ��SQL����
     *  @param 		[In]a param of Type  MutiSQL_DATA&  ����һ��SQL���Ľṹ��
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @param 		[Out]a param of Type  CMemSet*  ��ѯ���Ľ�����ݼ�
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool ExecuteSql( MutiSQL_DATA pMutiSQLDATA  , char* pError , CMemSet* pMemSet );
    /*
     *  @brief   	ʵʱ���ѯ����
     *  @param 		[In]a param of Type  int  ���ݿ����
     *  @param 		[In]a param of Type  SQL_DATA&  ����Ĳ�ѯ����
     *  @param 		[In]a param of Type  REALDATA_CONDITION&  ʵʱ���ѯ��������
     *  @param 		[Out]a param of Type  char*  ������Ϣ
     *  @param 		[Out]a param of Type  CMemSet*  ��ѯ���Ľ�����ݼ�
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool RDSelect( int pTableNo , SQL_DATA& pSqlData , REALDATA_CONDITION& pCondition , char* pError , CMemSet* pMemSet );

    /*
     *  @brief   	ʵʱ����뺯��
     *  @param 		[In]a param of Type  STTP_FULL_DATA&  д���ݿ⼯��
     *  @param 		[Out]a param of Type  char*  ִ�в������ص���Ϣ
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	bool RDInsert( STTP_FULL_DATA& pRealData, char* pError);
	
    /*
     *  @brief		InsertMultiData	 ���������¼ 
     *  @param		[In]TableName  �������
     *  @param		[In]pRealData  �������ݵ�FUlldata
     *  @param		[Out]pError  �����ַ���
     *  @return		bool true-�ɹ� false-ʧ��
     */

	bool InsertMultiData(int TableName ,STTP_FULL_DATA& pRealData ,char* pError);
	/**
	* @brief			Connect ��������ƽ̨
	* @param 			[In]int iModelNO ʹ���ߵ�¼ģ��ID
	* @param 			[In]char* pError ���ش�����
	* @return			bool true:�ɹ� false:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool Connect( int iModelNO , char* pError );
	/**
	* @brief			SetLogPath ������־·��
	* @param 			[In]int LogPath �������־·��
	* @param 			[In]char* Msg ���ش�����
	* @return			bool 1:�ɹ� 0:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool SetLogPath( char* LogPath,char* Msg );
protected:
private:
	/** @brief           ���ݷ��ʽӿڿ�handle*/
	XJHANDLE			     m_hLibXJDBFacade;
	
	/** @brief           ���ݿ⾲̬��select����ָ��*/
	PFUNSELECT			 m_pFunSelect;
	
	/** @brief           ���ݿ⾲̬��Update����ָ��*/
	PFUNUPDATE			 m_pFunUpdate;
	
	/** @brief           ���ݿ⾲̬��insert����ָ��*/
	PFUNINSERT		     m_pFunInsert;
	
	/** @brief           ���ݿ⾲̬��Delete����ָ��*/
	PFUNDELETE		     m_pFunDelete;

	/** @brief           ���ݿ⾲̬��ExecuteSql����ָ��*/
	PFUNEXECUTESQL		 m_pFunExecuteSql;
	
	/** @brief           ���ݿ�ʵʱ��select����ָ��*/
	PFUNRDSELECT		 m_pFunRDSelect;

	/** @brief           ���ݿ�ʵʱ��insert����ָ��*/
	PFUNRDINSERT		 m_pFunRDInsert;
	
	/** @brief           InsertMultiData����ָ��*/
	PFUNINSERTMULTIDATA  m_pFunInsertMultiData;
	
	/** @brief           ���ݿ⾲̬��Connect����ָ��*/
	PFUNCONNECT			 m_pFunConnect;

	/** @brief           ���ݿ⾲̬��SetLogPath����ָ��*/
	PFUNSETLOGCONFIG     m_pFunSetLogPath;

	/** @brief           ��־����ָ��*/
	CLogFile*		     m_pLogFile;
};

#endif // CXJDBFACADE_H
