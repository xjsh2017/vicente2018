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
 函 数 名:   CXJDBFacade
 功能概要:   构造函数
 返 回 值:   
 参    数:   CLogFile * pLogFile 日志对象指针
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
 函 数 名:   ~CXJDBFacade
 功能概要:   析构函数
 返 回 值:   
**************************************************************/
CXJDBFacade::~CXJDBFacade()
{
	xj_free_library(m_hLibXJDBFacade);
}

/*************************************************************
 函 数 名:   LoadLib
 功能概要:   加载动态库和各函数指针
 返 回 值:   int 0-成功 其他-错误代码
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
函 数 名:   Select
功能概要:   静态库查询函数
返 回 值:   bool true-成功 false-失败
参    数:   int pTableNo 数据库表编号
参    数:   SQL_DATA & pSqlData 输入的查询参数
参    数:   char * pError 错误信息
参    数:   CMemSet * pMemSet 查询到的结果数据集
**************************************************************/
bool CXJDBFacade::Select( int pTableNo , SQL_DATA& pSqlData , char* pError , CMemSet* pMemSet )
{
	if (m_pFunSelect != NULL){
		return (m_pFunSelect(pTableNo,pSqlData,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(SelectData)");
		return false;
	}	
}

/*************************************************************
函 数 名:   Update
功能概要:   静态库更新函数
返 回 值:   bool true-成功 false-失败
参    数:   int pTableNo 数据库表编号
参    数:   SQL_DATA & pSqlData 输入的查询参数
参    数:   char * pError 错误信息
**************************************************************/
bool CXJDBFacade::Update( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunUpdate != NULL){
		return (m_pFunUpdate(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(UpdateData)");
		return false;
	}		
}

/*************************************************************
函 数 名:   Insert
功能概要:   静态库插入函数
返 回 值:   bool true-成功 false-失败
参    数:   int pTableNo 数据库表编号
参    数:   SQL_DATA & pSqlData 输入的查询参数
参    数:   char * pError 错误信息
**************************************************************/
bool CXJDBFacade::Insert( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunInsert != NULL){
		return (m_pFunInsert(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(InsertData)");
		return false;
	}		
}

/*************************************************************
函 数 名:   Delete
功能概要:   静态库删除函数
返 回 值:   bool true-成功 false-失败
参    数:   int pTableNo 数据库表编号
参    数:   SQL_DATA & pSqlData 输入的查询参数
参    数:   char * pError 错误信息
**************************************************************/
bool CXJDBFacade::Delete( int pTableNo , SQL_DATA& pSqlData , char* pError )
{
	if (m_pFunDelete != NULL){
		return (m_pFunDelete(pTableNo,pSqlData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(DeleteData)");
		return false;
	}	
}

/*************************************************************
函 数 名:   ExecuteSql
功能概要:   静态库执行SQL函数
返 回 值:   bool true-成功 false-失败
参    数:   MutiSQL_DATA pMutiSQLDATA 放置一条SQL语句的结构体
参    数:   char * pError 错误信息
参    数:   CMemSet * pMemSet 查询到的结果数据集
**************************************************************/
bool CXJDBFacade::ExecuteSql( MutiSQL_DATA pMutiSQLDATA , char* pError , CMemSet* pMemSet )
{
	if (m_pFunExecuteSql != NULL){
		return (m_pFunExecuteSql(pMutiSQLDATA,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(ExecuteSql)");
		return false;
	}		
}

/*************************************************************
函 数 名:   RDSelect
功能概要:   实时库查询函数
返 回 值:   bool true-成功 false-失败
参    数:   int pTableNo 数据库表编号
参    数:   SQL_DATA & pSqlData 输入的查询参数
参    数:   REALDATA_CONDITION & pCondition 实时库查询条件参数
参    数:   char * pError 错误信息
参    数:   CMemSet * pMemSet 查询到的结果数据集
**************************************************************/
bool CXJDBFacade::RDSelect( int pTableNo , SQL_DATA& pSqlData , REALDATA_CONDITION& pCondition , char* pError , CMemSet* pMemSet )
{
	if (m_pFunRDSelect != NULL){
		return (m_pFunRDSelect(pTableNo,pSqlData,pCondition,pError,pMemSet) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(RealDataSelect)");
		return false;
	}
}

/*************************************************************
 函 数 名:   RDInsert
 功能概要:   实时库插入函数
 返 回 值:   bool true-成功 false-失败
 参    数:   STTP_FULL_DATA & pRealData 写数据库集合
 参    数:   char * pError 执行操作返回的信息
**************************************************************/
bool CXJDBFacade::RDInsert( STTP_FULL_DATA& pRealData, char* pError )
{
	if (m_pFunRDInsert != NULL){
		return (m_pFunRDInsert(pRealData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(RealDataInsert)");
		return false;
	}	
}

/************************************
 函 数 名:    InsertMultiData
 功能概要:    批量插入记录
 返 回 值:    bool true-成功 false-失败
 参    数:    int TableName 表名编号
 参    数:    STTP_FULL_DATA & pRealData 保存数据的FUlldata
 参    数:    char * pError 出错字符串
 ************************************/
bool CXJDBFacade::InsertMultiData( int TableName ,STTP_FULL_DATA& pRealData ,char* pError )
{
	if (m_pFunInsertMultiData != NULL){
		return (m_pFunInsertMultiData(TableName,pRealData,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(InsertMultiData)");
		return false;
	}		
}

/*************************************************************
函 数 名:   Connect
功能概要:   静态库连接函数
返 回 值:   bool true-成功 false-失败
参    数:   int iModelNO 模块号
参    数:   char * pError 错误信息
**************************************************************/
bool CXJDBFacade::Connect( int iModelNO , char* pError )
{
	if (m_pFunConnect != NULL){
		return (m_pFunConnect(iModelNO,pError) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(SelectData)");
		return false;
	}	
}

/*************************************************************
函 数 名:   SetLogPath
功能概要:   静态库设置日志路径函数
返 回 值:   bool true-成功 false-失败
参    数:   char * LogPath 日志路径
参    数:   char * Msg 错误信息
**************************************************************/
bool CXJDBFacade::SetLogPath( char* LogPath,char* Msg )
{
	if (m_pFunSetLogPath != NULL){
		return (m_pFunSetLogPath(LogPath,Msg) == 1)?true:false;	
	}	
	else{
		m_pLogFile->FormatAdd(CLogFile::error,"无效的函数指针(SetLogPath)");
		return false;
	}	
}
