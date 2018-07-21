// DBEngineCP.cpp: implementation of the DBEngineCP class.
//
//////////////////////////////////////////////////////////////////////

#include "DBFacadeEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBFacadeEngine::DBFacadeEngine()
{
	m_hModule = NULL;
	m_pSelectData = NULL;//查询数据
	m_pUpdateData = NULL;	//更新数据
	m_pInsertData = NULL;	//写数据
	m_pDeleteData = NULL;	//删除数据
	m_pExecuteSql = NULL; //直接执行SQL语句操作
	m_pInsertMultiData = NULL; //批量写数据
	m_pConnect = NULL; //连接数据库接口
	m_pSetLogPath = NULL; //设置日志路径接口
	m_pCloseSession = NULL; //关闭连接
	m_pSelectRealData = NULL;//查询实时数据

	xjdbf_LoadDBFLib();
}

DBFacadeEngine::~DBFacadeEngine()
{
	if(m_hModule != NULL)
	{
		xj_free_library(m_hModule);
		m_pSelectData = NULL;//查询数据
		m_pUpdateData = NULL;	//更新数据
		m_pInsertData = NULL;	//写数据
		m_pDeleteData = NULL;	//删除数据
		m_pExecuteSql = NULL; //直接执行SQL语句操作
		m_pInsertMultiData = NULL; //批量写数据
		m_pConnect = NULL; //连接数据库接口
		m_pSetLogPath = NULL; //设置日志路径接口
		m_pCloseSession = NULL; //关闭连接
		m_pSelectRealData = NULL;//查询实时数据
	}
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_LoadDBFLib	
返回值:   bool	装载成功返回true,失败返回false
功能概要: 装载XJDBFacade.dll
*****************************************************/
bool DBFacadeEngine::xjdbf_LoadDBFLib()
{
	m_hModule = xj_load_library(LIBNAME_DBFACADE);

	if(m_hModule == NULL)
		return false;

	m_pSelectData = (int(*)(int,SQL_DATA&, char*, CMemSet*))xj_get_addr(m_hModule, "SelectData");//查询数据
	m_pUpdateData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "UpdateData");	//更新数据
	m_pInsertData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "InsertData");	//写数据
	m_pDeleteData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "DeleteData");	//删除数据
	m_pExecuteSql = (int(*)(MutiSQL_DATA&, char*, CMemSet*))xj_get_addr(m_hModule, "ExecuteSql"); //直接执行SQL语句操作
	m_pInsertMultiData = (int (*)(int, STTP_FULL_DATA&, char*))xj_get_addr(m_hModule, "InsertMutiData"); //批量写数据
	m_pConnect = (int(*)(int, char*))xj_get_addr(m_hModule, "Connect"); //连接数据库接口
	m_pSetLogPath = (int(*)(const char*, char*))xj_get_addr(m_hModule, "SetLogPath"); //设置日志路径接口
	m_pCloseSession = (int(*)(char*))xj_get_addr(m_hModule, "CloseSession"); //关闭连接
	m_pSelectRealData = (int(*)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *))xj_get_addr(m_hModule, "RealDataSelect");//查询实时数据

	return true;
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_SelectData	
返回值:   int	成功返回1,失败返回0
功能概要: 查询接口
参数: int TableCode	数据表代号
参数: SQL_DATA & sql 查询条件结构	
参数: char * sError	错误输出
参数: CMemSet * pMem 数据集	
*****************************************************/
int DBFacadeEngine::xjdbf_SelectData( int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSelectData == NULL)
		return 0;
	return m_pSelectData(TableCode, sql, sError, pMem);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_UpdateData	
返回值:   int	成功返回1,失败返回0
功能概要: 更新接口
参数: int TableCode	数据表代号
参数: SQL_DATA & sql 查询条件结构	
参数: char * sError	错误输出
*****************************************************/
int DBFacadeEngine::xjdbf_UpdateData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pUpdateData == NULL)
		return 0;
	return m_pUpdateData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_InsertData	
返回值:   int 成功返回1,失败返回0	
功能概要: 写数据接口
参数: int TableCode	数据表代号
参数: SQL_DATA & sql 查询条件结构	
参数: char * sError	错误输出
*****************************************************/
int DBFacadeEngine::xjdbf_InsertData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pInsertData == NULL)
		return 0;
	return m_pInsertData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_InsertMutiData	
返回值:   int 成功返回1, 失败返回0	
功能概要: 批量写数据
参数: int TableName	数据表代号
参数: STTP_FULL_DATA & pRealData 查询条件结构	
参数: char * Msg 错误输出	
*****************************************************/
int DBFacadeEngine::xjdbf_InsertMutiData( int TableName ,STTP_FULL_DATA& pRealData ,char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pInsertMultiData == NULL)
		return 0;
	return m_pInsertMultiData(TableName, pRealData, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_DeleteData	
返回值:   int 成功返回1,失败返回0	
功能概要: 删除数据接口
参数: int TableCode	数据表代号
参数: SQL_DATA & sql 查询条件结构	
参数: char * sError	错误输出
*****************************************************/
int DBFacadeEngine::xjdbf_DeleteData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pDeleteData == NULL)
		return 0;
	return m_pDeleteData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_Connect	
返回值:   int	0-失败, 1-成功
功能概要: 连接数据库
参数: int pName	模块名
参数: char * Msg 执行该接口返回的信息(一般为错误信息)	
*****************************************************/
int DBFacadeEngine::xjdbf_Connect( int pName, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pConnect == NULL)
		return 0;
	return m_pConnect(pName, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_SetLogPath	
返回值:   int	
功能概要: 设置日志路径
参数: const char * LogPath 日志路径(全路径:统一路径/模块名)	
参数: char * Msg 执行该接口返回的信息(一般为错误信息)	
*****************************************************/
int DBFacadeEngine::xjdbf_SetLogPath( const char* LogPath, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSetLogPath == NULL)
		return 0;
	return m_pSetLogPath(LogPath, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_CloseSession	
返回值:   int 成功返回1,失败返回0	
功能概要: 关闭连接
参数: CString & sLog 返回信息	
*****************************************************/
int DBFacadeEngine::xjdbf_CloseSession( char* sLog )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pCloseSession == NULL)
		return 0;
	return m_pCloseSession(sLog);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_ExecuteSql	
返回值:   int 成功返回1,失败返回0	
功能概要: 直接执行标准的SQL语句
参数: MutiSQL_DATA & pMutiSQLDATA 放置一条SQL语句的结构体	
参数: char * Msg 执行查询操作返回的执行信息	
参数: CMemSet * p_rst 返回的查询数据集	
*****************************************************/
int DBFacadeEngine::xjdbf_ExecuteSql( MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pExecuteSql == NULL)
		return 0;
	return m_pExecuteSql(pMutiSQLDATA, Msg, p_rst);
}

/****************************************************
Date:2012/7/5  Author:LYH
函数名:   xjdbf_SelectRealData	
返回值:   int 成功返回1,失败返回0	
功能概要: 从实时库查询数据
参数: int TableName	数据表编号
参数: SQL_DATA & pSQLDATA 放置一条SQL语句的结构体	
参数: REALDATA_CONDITION & pData 实时库数据位置信息	
参数: char * Msg 执行查询操作返回的执行信息	
参数: CMemSet * p_rst 返回的查询数据集	
*****************************************************/
int DBFacadeEngine::xjdbf_SelectRealData( int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSelectRealData == NULL)
		return 0;
	return m_pSelectRealData(TableName, pSQLDATA, pData, Msg, p_rst);
}

/****************************************************
Date:2012/7/9  Author:LYH
函数名:   SetField	
返回值:   void	
功能概要: 设置查询字段
参数: SQL_DATA & sql 查询结构	
参数: Field & field	字段结构
参数: CXJString sName 字段名称	
参数: int nType	字段类型
参数: CXJString sValue	字段值
*****************************************************/
void DBFacadeEngine::SetField( SQL_DATA& sql, Field& field, CXJString sName, int nType, CXJString sValue /*= ""*/ )
{
	bzero(&field, sizeof(Field));
	CXJString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
		strncpy(field.FieldValue, sValue, sValue.GetLength());
	sql.Fieldlist.push_back(field);
}

void DBFacadeEngine::SetField(SQL_DATA& sql, CXJString sName, int nType, CXJString sValue /*= ""*/)
{
	Field field;
	bzero(&field, sizeof(Field));
	CXJString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
		strncpy(field.FieldValue, sValue, sValue.GetLength());
	sql.Fieldlist.push_back(field);
}

/****************************************************
Date:2012/7/9  Author:LYH
函数名:   SetCondition	
返回值:   void	
功能概要: 设置查询条件
参数: SQL_DATA & sql 查询结构	
参数: Condition & con 条件结构	
参数: CXJString sContent 条件内容	
参数: int nType	条件类型
*****************************************************/
void DBFacadeEngine::SetCondition( SQL_DATA& sql, Condition& con, CXJString sContent, int nType /*= 0*/ )
{
	bzero(&con, sizeof(Condition));
	CXJString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}

void DBFacadeEngine::SetCondition(SQL_DATA& sql, CXJString sContent, int nType /*= 0*/)
{
	Condition con;
	bzero(&con, sizeof(Condition));
	CXJString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}