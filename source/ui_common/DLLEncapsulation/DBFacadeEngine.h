// DBEngineCP.h: interface for the DBEngineCP class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(DBFACADEENGINE_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_)
#define DBFACADEENGINE_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../common/define.h"
#include "../../common/xjlib.h"
#include "../../common/MemSet.h"
#include "../../common/SttpDefine.h"
#include "../../common/XJString.h"

#ifdef OS_WINDOWS
#define LIBNAME_DBFACADE "XJDBFacade.dll"
#else 
#define LIBNAME_DBFACADE "libXJDBFacade.so"
#endif

//定义名以DBFacade的缩写dbf开头
typedef int (*dbf_SelectData)(int, SQL_DATA&, char*, CMemSet*); //查询数据
typedef int (*dbf_UpdateData)(int, SQL_DATA&, char*);	//更新数据
typedef int (*dbf_InsertData)(int, SQL_DATA&, char*);	//写数据
typedef int (*dbf_DeleteData)(int, SQL_DATA&, char*);	//删除数据
typedef int (*dbf_InsertMultiData)(int, STTP_FULL_DATA&, char*); //批量写数据
typedef int	(*dbf_Connect)(int, char*); //连接数据库接口
typedef int (*dbf_SetLogPath)(const char*, char*); //设置日志路径接口
typedef int (*dbf_CloseSession)(char*); //关闭连接
typedef int (*dbf_ExecuteSql)(MutiSQL_DATA&, char*, CMemSet*); //直接执行SQL语句操作
typedef int (*dbf_SelectRealData)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *);//查询实时数据

class DBFacadeEngine  
{
public:
	DBFacadeEngine();
	virtual ~DBFacadeEngine();

private:
	/** @brief           动态库句柄*/
	XJHANDLE	m_hModule;

private:
	dbf_SelectData m_pSelectData;//查询数据
	dbf_UpdateData m_pUpdateData;	//更新数据
	dbf_InsertData m_pInsertData;	//写数据
	dbf_DeleteData m_pDeleteData;	//删除数据
	dbf_ExecuteSql m_pExecuteSql; //直接执行SQL语句操作
	dbf_InsertMultiData m_pInsertMultiData; //批量写数据
	dbf_Connect m_pConnect; //连接数据库接口
	dbf_SetLogPath m_pSetLogPath; //设置日志路径接口
	dbf_CloseSession m_pCloseSession; //关闭连接
	dbf_SelectRealData m_pSelectRealData;//查询实时数据

private:
	/*
	* @brief	xjdbf_LoadDBFLib	装载XJDBFacade.dll
	* @return   bool	装载成功返回true,失败返回false
	*/
	bool xjdbf_LoadDBFLib();
public:
	/*
 *  @brief   	XJSelectData	 查询接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @param 		[Out]a param of Type  CMemSet*  数据集
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803BD
	int	xjdbf_SelectData(int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem);

/*
 *  @brief   	XJUpdateData	 更新接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803C2
	int xjdbf_UpdateData(int TableCode, SQL_DATA& sql, char* sError);


/*
 *  @brief   	XJInsertData	 写数据接口
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803CA
	int xjdbf_InsertData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertMutiData	 批量写数据 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  查询条件结构
 *  @param 		[In]a param of Type  char*  错误输出
 *  @return 	int 成功返回1, 失败返回0
 */
	//##ModelId=49B87BA803CE
	int xjdbf_InsertMutiData(int TableName ,STTP_FULL_DATA& pRealData ,char* Msg);

/*
 *  @brief   	XJDeleteData	 删除数据接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803D2
	int xjdbf_DeleteData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJConnect	 连接数据库
 *  @param 		[In]a param of Type  int  模块名
 *  @param 		[In]a param of Type  char*  执行该接口返回的信息(一般为错误信息)
 *  @return 	int 0-失败, 1-成功
 */
	//##ModelId=49B87BA803E2
	int xjdbf_Connect(int pName, char* Msg);

/*
 *  @brief   	XJSetLogPath	 设置日志路径 
 *  @param 		[In]a param of Type  const char*  日志路径(全路径:统一路径/模块名)
 *  @param 		[In]a param of Type  char*  执行该接口返回的信息(一般为错误信息)
 *  @return 	int 0-失败, 1-成功
 */
	//##ModelId=49B87BA803E5
	int xjdbf_SetLogPath(const char* LogPath, char* Msg);

/*
 *  @brief   	XJCloseSession	 关闭连接 
 *  @param 		[Out]a param of Type  CString&  返回信息
 *  @return 	int 成功返回1,失败返回0
 */
	int xjdbf_CloseSession(char* sLog);

	/*
	*  @brief   	XJExecuteSql	 直接执行标准的SQL语句 
	*  @param 		[In]a param of Type  MutiSQL_DATA&  放置一条SQL语句的结构体
	*  @param 		[Out]a param of Type  char*  执行查询操作返回的执行信息
	*  @param 		[Out]a param of Type  CMemSet *  返回的查询数据集
	*  @return 	int 成功返回1,失败返回0
	*/
	int xjdbf_ExecuteSql(MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst);
	
	/*
	* @brief	XJSelectRealData	从实时库查询数据	
	* @param	[In]a param of Type int TableName 数据表编号		
	* @param	[In]a param of Type SQL_DATA & pSQLDATA	放置一条SQL语句的结构体	
	* @param	[In]a param of Type REALDATA_CONDITION & pData 实时库数据位置信息		
	* @param	[In]a param of Type char * Msg	执行查询操作返回的执行信息	
	* @param	[In]a param of Type CMemSet * p_rst	返回的查询数据集	
	* @return   int	成功返回1,失败返回0
	*/
	int xjdbf_SelectRealData(int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg,  CMemSet * p_rst);

	/*
 *  @brief   	SetField	 设置查询字段 
 *  @param 		[Out]a param of Type  SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  Field&  字段结构
 *  @param 		[In]a param of Type  CXJString  字段名称
 *  @param 		[In]a param of Type  int  字段类型
 *  @param		[In]a param of Type  CXJString 字段值
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetField(SQL_DATA& sql, Field& field, CXJString sName, int nType, CXJString sValue = "");

	void SetField(SQL_DATA& sql, CXJString sName, int nType, CXJString sValue = "");


/*
 *  @brief   	SetCondition	 设置查询条件 
 *  @param 		[Out]a param of Type  SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  Condition&  条件结构
 *  @param 		[In]a param of Type  CXJString  条件内容
 *  @param 		[In]a param of Type  int  条件类型
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetCondition(SQL_DATA& sql, Condition& con, CXJString sContent, int nType = 0);

	void SetCondition(SQL_DATA& sql, CXJString sContent, int nType = 0);

};

#endif // !defined(AFX_DBENGINECP_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_)
