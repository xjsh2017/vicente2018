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

/** @brief            数据库访问接口库名称定义*/
#define LIB_DBFACADE_NAME              "libXJDBFacade.so"

#endif

#ifdef OS_WINDOWS

/** @brief            数据库访问接口库名称定义*/
#define LIB_DBFACADE_NAME              "XJDBFacade.dll"

#endif

/** @brief            DBFacade中Connect函数定义*/
typedef int (*PFUNCONNECT)(int ,char* );

/** @brief            DBFacade中SetLogCofig函数定义*/
typedef int (*PFUNSETLOGCONFIG)(char*,char* );

/** @brief            DBFacade中SelectData函数定义*/
typedef int (*PFUNSELECT)(int , SQL_DATA& , char* , CMemSet* );

/** @brief            DBFacade中MultiSelectData函数定义*/
typedef int (*PFUNMULTISELECT)(char* , MutiSQL_DATA& , char* );

/** @brief            DBFacade中UpdateData函数定义*/
typedef int (*PFUNUPDATE)(int , SQL_DATA& , char*);

/** @brief            DBFacade中InsertData函数定义*/
typedef int (*PFUNINSERT)(int , SQL_DATA& , char*);

/** @brief            DBFacade中DeleteData函数定义*/
typedef int (*PFUNDELETE)(int , SQL_DATA& , char*);

/** @brief            DBFacade中ExecuteSql函数定义*/
typedef int (*PFUNEXECUTESQL)(MutiSQL_DATA& , char* , CMemSet * );

/** @brief            DBFacade中RealDataInsert函数定义*/
typedef int (*PFUNRDINSERT)(STTP_FULL_DATA& , char* );

/** @brief            DBFacade中RealDataSelect函数定义*/
typedef int (*PFUNRDSELECT)(int,SQL_DATA&,REALDATA_CONDITION&,char*,CMemSet*);

/** @brief            DBFacade中InsertMultiData函数定义*/
typedef int (*PFUNINSERTMULTIDATA)(int,STTP_FULL_DATA&,char*);


/**
 * @defgroup 	CXJDBFacade	类说明
 * @{
 */
/**
 * @brief       加载，使用DBFacade动态库的类
*  @author      qingch
 * @version     ver1.0
 * @date        13/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

class CXJDBFacade
{
public:
    /*
     *  @brief   	构造函数 
     *  @param 		[In]a param of Type  CLogFile*  传入的日志对象指针
     *  @return 	 
     */

    CXJDBFacade(CLogFile* pLogFile);
    /*
     *  @brief   	析构函数	 
     *  @return 	virtual 
     */

    virtual ~CXJDBFacade();
    /*
     *  @brief   	加载动态库和各函数指针
     *  @return 	int 0-成功 其他-错误代码
     */

	int LoadLib();
    /*
     *  @brief   	静态库查询函数
     *  @param 		[In]a param of Type  int  数据库表编号
     *  @param 		[In]a param of Type  SQL_DATA&  输入的查询参数
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @param 		[Out]a param of Type  CMemSet*  查询到的结果数据集
     *  @return 	bool true-成功 false-失败
     */

	bool Select( int pTableNo , SQL_DATA& pSqlData , char* pError , CMemSet* pMemSet );
    /*
     *  @brief   	静态库更新函数
     *  @param 		[In]a param of Type  int  数据库表编号
     *  @param 		[In]a param of Type  SQL_DATA&  输入的查询参数
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @return 	bool true-成功 false-失败
     */

	bool Update( int pTableNo , SQL_DATA& pSqlData , char* pError );
    /*
     *  @brief   	静态库插入函数
     *  @param 		[In]a param of Type  int  数据库表编号
     *  @param 		[In]a param of Type  SQL_DATA&  输入的查询参数
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @return 	bool true-成功 false-失败
     */

	bool Insert( int pTableNo , SQL_DATA& pSqlData , char* pError );

    /*
     *  @brief   	静态库删除函数
     *  @param 		[In]a param of Type  int  数据库表编号
     *  @param 		[In]a param of Type  SQL_DATA&  输入的查询参数
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @return 	bool true-成功 false-失败
     */

	bool Delete( int pTableNo , SQL_DATA& pSqlData , char* pError );

    /*
     *  @brief   	静态库执行SQL函数
     *  @param 		[In]a param of Type  MutiSQL_DATA&  放置一条SQL语句的结构体
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @param 		[Out]a param of Type  CMemSet*  查询到的结果数据集
     *  @return 	bool true-成功 false-失败
     */

	bool ExecuteSql( MutiSQL_DATA pMutiSQLDATA  , char* pError , CMemSet* pMemSet );
    /*
     *  @brief   	实时库查询函数
     *  @param 		[In]a param of Type  int  数据库表编号
     *  @param 		[In]a param of Type  SQL_DATA&  输入的查询参数
     *  @param 		[In]a param of Type  REALDATA_CONDITION&  实时库查询条件参数
     *  @param 		[Out]a param of Type  char*  错误信息
     *  @param 		[Out]a param of Type  CMemSet*  查询到的结果数据集
     *  @return 	bool true-成功 false-失败
     */

	bool RDSelect( int pTableNo , SQL_DATA& pSqlData , REALDATA_CONDITION& pCondition , char* pError , CMemSet* pMemSet );

    /*
     *  @brief   	实时库插入函数
     *  @param 		[In]a param of Type  STTP_FULL_DATA&  写数据库集合
     *  @param 		[Out]a param of Type  char*  执行操作返回的信息
     *  @return 	bool true-成功 false-失败
     */

	bool RDInsert( STTP_FULL_DATA& pRealData, char* pError);
	
    /*
     *  @brief		InsertMultiData	 批量插入记录 
     *  @param		[In]TableName  表名编号
     *  @param		[In]pRealData  保存数据的FUlldata
     *  @param		[Out]pError  出错字符串
     *  @return		bool true-成功 false-失败
     */

	bool InsertMultiData(int TableName ,STTP_FULL_DATA& pRealData ,char* pError);
	/**
	* @brief			Connect 连接数据平台
	* @param 			[In]int iModelNO 使用者登录模块ID
	* @param 			[In]char* pError 返回错误码
	* @return			bool true:成功 false:失败
	* @notes	无
	* @sample	无
	*/
	bool Connect( int iModelNO , char* pError );
	/**
	* @brief			SetLogPath 设置日志路径
	* @param 			[In]int LogPath 传入的日志路径
	* @param 			[In]char* Msg 返回错误码
	* @return			bool 1:成功 0:失败
	* @notes	无
	* @sample	无
	*/
	bool SetLogPath( char* LogPath,char* Msg );
protected:
private:
	/** @brief           数据访问接口库handle*/
	XJHANDLE			     m_hLibXJDBFacade;
	
	/** @brief           数据库静态库select函数指针*/
	PFUNSELECT			 m_pFunSelect;
	
	/** @brief           数据库静态库Update函数指针*/
	PFUNUPDATE			 m_pFunUpdate;
	
	/** @brief           数据库静态库insert函数指针*/
	PFUNINSERT		     m_pFunInsert;
	
	/** @brief           数据库静态库Delete函数指针*/
	PFUNDELETE		     m_pFunDelete;

	/** @brief           数据库静态库ExecuteSql函数指针*/
	PFUNEXECUTESQL		 m_pFunExecuteSql;
	
	/** @brief           数据库实时库select函数指针*/
	PFUNRDSELECT		 m_pFunRDSelect;

	/** @brief           数据库实时库insert函数指针*/
	PFUNRDINSERT		 m_pFunRDInsert;
	
	/** @brief           InsertMultiData函数指针*/
	PFUNINSERTMULTIDATA  m_pFunInsertMultiData;
	
	/** @brief           数据库静态库Connect函数指针*/
	PFUNCONNECT			 m_pFunConnect;

	/** @brief           数据库静态库SetLogPath函数指针*/
	PFUNSETLOGCONFIG     m_pFunSetLogPath;

	/** @brief           日志对象指针*/
	CLogFile*		     m_pLogFile;
};

#endif // CXJDBFACADE_H
