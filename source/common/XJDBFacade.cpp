/********************************************************************
	created:	2008/10/13
	created:	13:10:2008   15:41
	file base:	XJDBFacade
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "XJDBFacade.h"

/*************************************************************
 �� �� ��:   CXJDBFacade
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
 ��    ��:   CLogFile * pLogFile ��־����ָ��
**************************************************************/
CXJDBFacade::CXJDBFacade(CLogFile* pLogFile)
:m_hLibXJDBFacade(NULL),m_pFunSelect(NULL),m_pFunUpdate(NULL),m_pFunInsert(NULL),
 m_pFunDelete(NULL),m_pFunExecuteSql(NULL),m_pFunRDSelect(NULL),m_pFunRDInsert(NULL),
 m_pFunInsertMultiData(NULL),m_pFunConnect(NULL),m_pFunSetLogPath(NULL),
 m_pLogFile(pLogFile)
{
    //ctor
}

/*************************************************************
 �� �� ��:   ~CXJDBFacade
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   
**************************************************************/
CXJDBFacade::~CXJDBFacade()
{
	xj_free_library(m_hLibXJDBFacade);
}

/*************************************************************
 �� �� ��:   LoadLib
 ���ܸ�Ҫ:   ���ض�̬��͸�����ָ��
 �� �� ֵ:   int 0-�ɹ� ����-�������
**************************************************************/
int CXJDBFacade::LoadLib()
{
	m_hLibXJDBFacade = xj_load_library(LIB_DBFACADE_NAME);
	if (m_hLibXJDBFacade != NULL)
	{
		m_pFunConnect = (PFUNCONNECT) xj_get_addr(m_hLibXJDBFacade,
			"Connect");
		if (m_pFunConnect == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function Connect failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}

		m_pFunSetLogPath = (PFUNSETLOGCONFIG) xj_get_addr(m_hLibXJDBFacade,
			"SetLogPath");
		if (m_pFunSetLogPath == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function Connect failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}

		m_pFunSelect = (PFUNSELECT) xj_get_addr(m_hLibXJDBFacade,
			"SelectData");
		if (m_pFunSelect == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function SelectData failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}	
		m_pFunUpdate = (PFUNUPDATE) xj_get_addr(m_hLibXJDBFacade,
			"UpdateData");
		if (m_pFunUpdate == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function UpdateData failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}	
		m_pFunInsert = (PFUNINSERT) xj_get_addr(m_hLibXJDBFacade,
			"InsertData");
		if (m_pFunInsert == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function InsertData failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}	
		m_pFunDelete = (PFUNDELETE) xj_get_addr(m_hLibXJDBFacade,
			"DeleteData");
		if (m_pFunDelete == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function DeleteData failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}
		m_pFunExecuteSql = (PFUNEXECUTESQL) xj_get_addr(m_hLibXJDBFacade,
			"ExecuteSql");
		if (m_pFunExecuteSql == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function ExecuteSql failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}		
		m_pFunRDSelect = (PFUNRDSELECT) xj_get_addr(m_hLibXJDBFacade,
			"RealDataSelect");
		if (m_pFunRDSelect == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function RealDataSelect failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}	
		m_pFunRDInsert = (PFUNRDINSERT) xj_get_addr(m_hLibXJDBFacade,
			"RealDataInsert");
		if (m_pFunRDInsert == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function RealDataInsert failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}	
		m_pFunInsertMultiData = (PFUNINSERTMULTIDATA) xj_get_addr(m_hLibXJDBFacade,
			"InsertMutiData");
		if (m_pFunRDInsert == NULL) {
			m_pLogFile->FormatAdd(CLogFile::error,"(%s)load function InsertMutiData failed",LIB_DBFACADE_NAME);
			return R_GET_FUNADDR_FAILED;
		}		
	} 
	else
	{
		m_pLogFile->Add("LoadDBFacadeLib failed",CLogFile::error);
		return R_LOAD_LIB_FAILED;
	}	

	m_pLogFile->Add("LoadDBFacadeLib and function success",CLogFile::trace);
	return 0;	
}

/*************************************************************
�� �� ��:   Select
���ܸ�Ҫ:   ��̬���ѯ����
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int pTableNo ���ݿ����
��    ��:   SQL_DATA & pSqlData ����Ĳ�ѯ����
��    ��:   char * pError ������Ϣ
��    ��:   CMemSet * pMemSet ��ѯ���Ľ�����ݼ�
**************************************************************/
bool CXJDBFacade::Select( int pTableNo , SQL_DATA& pSqlData , char* pError , CMemSet* pMemSet )
{
	if (m_pFunSelect != NULL){
		return (m_pFunSelect(pTableNo,pSqlData,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(SelectData)");
		return false;
	}	
}

/*************************************************************
�� �� ��:   Update
���ܸ�Ҫ:   ��̬����º���
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int pTableNo ���ݿ����
��    ��:   SQL_DATA & pSqlData ����Ĳ�ѯ����
��    ��:   char * pError ������Ϣ
**************************************************************/
bool CXJDBFacade::Update( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunUpdate != NULL){
		return (m_pFunUpdate(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(UpdateData)");
		return false;
	}		
}

/*************************************************************
�� �� ��:   Insert
���ܸ�Ҫ:   ��̬����뺯��
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int pTableNo ���ݿ����
��    ��:   SQL_DATA & pSqlData ����Ĳ�ѯ����
��    ��:   char * pError ������Ϣ
**************************************************************/
bool CXJDBFacade::Insert( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunInsert != NULL){
		return (m_pFunInsert(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(InsertData)");
		return false;
	}		
}

/*************************************************************
�� �� ��:   Delete
���ܸ�Ҫ:   ��̬��ɾ������
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int pTableNo ���ݿ����
��    ��:   SQL_DATA & pSqlData ����Ĳ�ѯ����
��    ��:   char * pError ������Ϣ
**************************************************************/
bool CXJDBFacade::Delete( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunDelete != NULL){
		return (m_pFunDelete(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(DeleteData)");
		return false;
	}	
}

/*************************************************************
�� �� ��:   ExecuteSql
���ܸ�Ҫ:   ��̬��ִ��SQL����
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   MutiSQL_DATA pMutiSQLDATA ����һ��SQL���Ľṹ��
��    ��:   char * pError ������Ϣ
��    ��:   CMemSet * pMemSet ��ѯ���Ľ�����ݼ�
**************************************************************/
bool CXJDBFacade::ExecuteSql( MutiSQL_DATA pMutiSQLDATA , char* pError , CMemSet* pMemSet )
{
	if (m_pFunExecuteSql != NULL){
		return (m_pFunExecuteSql(pMutiSQLDATA,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(ExecuteSql)");
		return false;
	}		
}

/*************************************************************
�� �� ��:   RDSelect
���ܸ�Ҫ:   ʵʱ���ѯ����
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int pTableNo ���ݿ����
��    ��:   SQL_DATA & pSqlData ����Ĳ�ѯ����
��    ��:   REALDATA_CONDITION & pCondition ʵʱ���ѯ��������
��    ��:   char * pError ������Ϣ
��    ��:   CMemSet * pMemSet ��ѯ���Ľ�����ݼ�
**************************************************************/
bool CXJDBFacade::RDSelect( int pTableNo , SQL_DATA& pSqlData , REALDATA_CONDITION& pCondition , char* pError , CMemSet* pMemSet )
{
	if (m_pFunRDSelect != NULL){
		return (m_pFunRDSelect(pTableNo,pSqlData,pCondition,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(RealDataSelect)");
		return false;
	}
}

/*************************************************************
 �� �� ��:   RDInsert
 ���ܸ�Ҫ:   ʵʱ����뺯��
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   STTP_FULL_DATA & pRealData д���ݿ⼯��
 ��    ��:   char * pError ִ�в������ص���Ϣ
**************************************************************/
bool CXJDBFacade::RDInsert( STTP_FULL_DATA& pRealData, char* pError )
{
	if (m_pFunRDInsert != NULL){
		return (m_pFunRDInsert(pRealData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(RealDataInsert)");
		return false;
	}	
}

/************************************
 �� �� ��:    InsertMultiData
 ���ܸ�Ҫ:    ���������¼
 �� �� ֵ:    bool true-�ɹ� false-ʧ��
 ��    ��:    int TableName �������
 ��    ��:    STTP_FULL_DATA & pRealData �������ݵ�FUlldata
 ��    ��:    char * pError �����ַ���
 ************************************/
bool CXJDBFacade::InsertMultiData( int TableName ,STTP_FULL_DATA& pRealData ,char* pError )
{
	if (m_pFunInsertMultiData != NULL){
		return (m_pFunInsertMultiData(TableName,pRealData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(InsertMultiData)");
		return false;
	}		
}

/*************************************************************
�� �� ��:   Connect
���ܸ�Ҫ:   ��̬�����Ӻ���
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   int iModelNO ģ���
��    ��:   char * pError ������Ϣ
**************************************************************/
bool CXJDBFacade::Connect( int iModelNO , char* pError )
{
	if (m_pFunConnect != NULL){
		return (m_pFunConnect(iModelNO,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(SelectData)");
		return false;
	}	
}

/*************************************************************
�� �� ��:   SetLogPath
���ܸ�Ҫ:   ��̬��������־·������
�� �� ֵ:   bool true-�ɹ� false-ʧ��
��    ��:   char * LogPath ��־·��
��    ��:   char * Msg ������Ϣ
**************************************************************/
bool CXJDBFacade::SetLogPath( char* LogPath,char* Msg )
{
	if (m_pFunSetLogPath != NULL){
		return (m_pFunSetLogPath(LogPath,Msg) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"��Ч�ĺ���ָ��(SetLogPath)");
		return false;
	}	
}
