// DBEngine.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "DBEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int (*SelectData)(int, SQL_DATA&, char*, CMemSet*); //查询数据
int (*UpdateData)(int, SQL_DATA&, char*);	//更新数据
int (*UpdateBlogData)(int, BLOB_SQL_DATA&, char*);	//更新数据
int (*InsertData)(int, SQL_DATA&, char*);	//写数据
int (*InsertBlobData)(int, BLOB_SQL_DATA&, char*);
int (*DeleteData)(int, SQL_DATA&, char*);	//删除数据
int (*InsertMutiData)(int, STTP_FULL_DATA&, char*); //批量写数据
int	(*Connect)(int, char*); //连接数据库接口
int (*SetLogPath)(const char*, char*); //设置日志路径接口
int (*CloseSession)(char*); 
int (*ExecuteSql)(MutiSQL_DATA&, char*, CMemSet*); //直接执行SQL语句操作
int (*SelectRealData)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *);

/////////////////////////////////////////////////////////////////////////////
// CDBEngine

//##ModelId=49B87BA803BA
CDBEngine::CDBEngine()
{
	LoadDBLib();
}

//##ModelId=49B87BA803BB
CDBEngine::~CDBEngine()
{
	FreeLibrary(m_hModule);
	m_hModule = NULL;
}

//##ModelId=49B87BA90002
BOOL CDBEngine::LoadDBLib()
{
	m_hModule = LoadLibrary("XJDBFacade.dll");
	if(m_hModule != NULL)
	{
		SelectData = (int(*)(int,SQL_DATA&, char*, CMemSet*))GetProcAddress(m_hModule, "SelectData"); //查询数据
		UpdateData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "UpdateData");	//更新数据
		UpdateBlogData = (int (*)(int, BLOB_SQL_DATA&, char*))GetProcAddress(m_hModule, "BlobUpdateData");	//更新数据
		InsertData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "InsertData");	//写数据
		DeleteData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "DeleteData");	//删除数据
		InsertMutiData = (int (*)(int, STTP_FULL_DATA&, char*))GetProcAddress(m_hModule, "InsertMutiData"); //批量写数据
		Connect = (int(*)(int, char*))GetProcAddress(m_hModule, "Connect"); //连接数据库
		SetLogPath = (int(*)(const char*, char*))GetProcAddress(m_hModule, "SetLogPath"); //设置日志路径
		CloseSession = (int(*)(char*))GetProcAddress(m_hModule, "CloseSession");
		ExecuteSql = (int(*)(MutiSQL_DATA&, char*, CMemSet*))GetProcAddress(m_hModule, "ExecuteSql"); //直接执行SQL语句操作
		SelectRealData = (int(*)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *))GetProcAddress(m_hModule, "RealDataSelect");//实时库查询
		InsertBlobData = (int (*)(int, BLOB_SQL_DATA&, char*))GetProcAddress(m_hModule, "BlobInsertData");
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：XJSelectData()
 功能概要：调用数据平台DLL库接口执行查询
 返 回 值: 成功返回1,失败返回0
 参    数：param1   表代码
           Param2   查询条件结构体
		   Param3	错误信息
		   Param4	返回的查询结果数据集
**************************************************************/
//##ModelId=49B87BA803BD
int CDBEngine::XJSelectData( int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem )
{
	if(m_hModule == NULL)
		return 0;
	if(SelectData == NULL)
		return 0;
	return SelectData(TableCode, sql, sError, pMem);
}

/*************************************************************
 函 数 名：SetField()
 功能概要：设置查询字段
 返 回 值: void
 参    数：param1   查询结构
           Param2   字段结构
		   Param3	字段名称
		   Param4	字段类型
**************************************************************/
//##ModelId=49B87BA803D6
void CDBEngine::SetField(SQL_DATA& sql, Field& field, CString sName, int nType, CString sValue)
{
	bzero(&field, sizeof(Field));
	CString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
	{
		int vlen = sValue.GetLength();

		strncpy(field.FieldValue, sValue, vlen>sizeof(field.FieldValue)?sizeof(field.FieldValue):vlen);
	}
	sql.Fieldlist.push_back(field);
}

/*************************************************************
 函 数 名：SetField()
 功能概要：设置查询字段
 返 回 值: void
 参    数：param1   查询结构
           Param2   字段结构
		   Param3	字段名称
		   Param4	字段类型
**************************************************************/
//##ModelId=49B87BA803D6
void CDBEngine::SetBlobField(BLOB_SQL_DATA& sql, BLOB_FIELD& field, CString sName, int nType, CString sValue)
{
	memset(&field, 0, sizeof(field));
	CString str = sName;
	str.MakeUpper();
    field.strBlobFieldName = str.GetBuffer(0);
	field.nBlobFieldType = nType;
	if(sValue != "")
	{
		field.strBlobFieldValue = sValue.GetBuffer(0);
	}
	sql.BlobFieldlist.push_back(field);
}



/*************************************************************
 函 数 名：SetCondition()
 功能概要：设置查询条件
 返 回 值: void
 参    数：param1   查询结构
           Param2   条件结构
		   Param3	条件内容
		   Param4	条件类型
**************************************************************/
//##ModelId=49B87BA803DD
void CDBEngine::SetCondition(SQL_DATA& sql, Condition& con, CString sContent, int nType/* = 0*/)
{
	bzero(&con, sizeof(Condition));
	CString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}

/*************************************************************
 函 数 名：SetCondition()
 功能概要：设置查询条件
 返 回 值: void
 参    数：param1   查询结构
           Param2   条件结构
		   Param3	条件内容
		   Param4	条件类型
**************************************************************/
//##ModelId=49B87BA803DD
void CDBEngine::SetBlobCondition(BLOB_SQL_DATA& sql, BLOB_CONDITION& con, CString sContent, int nType/* = 0*/)
{
	//bzero(&con, sizeof(Condition));
	CString str = sContent;
	con.strBlobConditionContent = str.GetBuffer(0);
	con.nBlobConditionType = nType;
	sql.Blob_Conditionlist.push_back(con);
}

/*************************************************************
 函 数 名：XJUpdateData()
 功能概要：通过数据库接口库更新数据
 返 回 值: 成功返回1,失败返回0
 参    数：param1	数据表代号
		   Param2	数据查询结构
		   Param3	错误输出
**************************************************************/
//##ModelId=49B87BA803C2
int CDBEngine::XJUpdateData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(UpdateData == NULL)
		return 0;
	return UpdateData(TableCode, sql, sError);
}

/*************************************************************
 函 数 名：XJUpdateBlogData()
 功能概要：通过数据库接口库更新数据
 返 回 值: 成功返回1,失败返回0
 参    数：param1	数据表代号
		   Param2	数据查询结构
		   Param3	错误输出
**************************************************************/
//##ModelId=49B87BA803C2
int CDBEngine::XJUpdateBlogData( int TableCode, BLOB_SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(UpdateBlogData == NULL)
		return 0;
	return UpdateBlogData(TableCode, sql, sError);
}

/*************************************************************
 函 数 名：XJInsertData()
 功能概要：通过数据库接口库写数据
 返 回 值: 成功返回1,失败返回0
 参    数：param1	数据表代号
		   Param2	数据查询结构
		   param3	错误输出
**************************************************************/
//##ModelId=49B87BA803CA
int CDBEngine::XJInsertData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(InsertData == NULL)
		return 0;
	return InsertData(TableCode, sql, sError);
}

/*************************************************************
 函 数 名：XJDeleteData()
 功能概要：通过数据库接口库删除数据
 返 回 值: 成功返回1,失败返回0
 参    数：param1	数据表代号
		   Param2	数据查询结构
		   Param3	错误输出
**************************************************************/
//##ModelId=49B87BA803D2
int CDBEngine::XJDeleteData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(DeleteData == NULL)
		return 0;
	return DeleteData(TableCode, sql, sError);
}

/*************************************************************
 函 数 名：XJInsertMutiData()
 功能概要：批量写数据
 返 回 值: 成功返回1, 失败返回0
 参    数：param1	数据表代号
		   Param2	查询条件结构
		   Param3	错误输出
**************************************************************/
//##ModelId=49B87BA803CE
int CDBEngine::XJInsertMutiData( int TableName ,STTP_FULL_DATA& pRealData ,char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(InsertMutiData == NULL)
		return 0;
	return InsertMutiData(TableName, pRealData, Msg);
}

/*************************************************************
 函 数 名：XJConnect()
 功能概要：连接数据库
 返 回 值: 0-失败, 1-成功
 参    数：param1	模块名
		   Param2	执行该接口返回的信息(一般为错误信息)
**************************************************************/
//##ModelId=49B87BA803E2
int CDBEngine::XJConnect( int pName, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(Connect == NULL)
		return 0;
	return Connect(pName, Msg);
}

/*************************************************************
 函 数 名：XJSetLogPath()
 功能概要：设置日志路径
 返 回 值: 0-失败, 1-成功
 参    数：param1	日志路径(全路径:统一路径/模块编号)
		   Param2	执行该接口返回的信息(一般为错误信息)
**************************************************************/
//##ModelId=49B87BA803E5
int CDBEngine::XJSetLogPath( const char* LogPath, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(SetLogPath == NULL)
		return 0;
	return SetLogPath(LogPath, Msg);
}

/*************************************************************
 函 数 名: XJCloseSession()
 功能概要: 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
int CDBEngine::XJCloseSession( CString& sLog )
{
	if(m_hModule == NULL)
		return 0;
	if(CloseSession == NULL)
		return 0;
	char* pStr = sLog.GetBuffer(0);
	return CloseSession(pStr);
}

/*************************************************************
函 数 名：XJExecuteSql()
功能概要：直接执行标准的SQL语句
返 回 值: 成功返回1,失败返回0
参    数：param1	放置一条SQL语句的结构体
Param2	执行查询操作返回的执行信息
Param3	返回的查询数据集
**************************************************************/
int CDBEngine::XJExecuteSql( MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(ExecuteSql == NULL)
		return 0;
	return ExecuteSql(pMutiSQLDATA, Msg, p_rst);
}

/*************************************************************
CreateDate: 30:12:2009     Author:LYH
函 数 名: FreeDBLib()
功能概要: 卸载XJDBFacade.dll 
返 回 值: 
参    数: param1 
Param2 
**************************************************************/
void CDBEngine::FreeDBLib()
{
	if(m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

int CDBEngine::XJSelectRealData( int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(SelectRealData == NULL)
		return 0;
	return SelectRealData(TableName, pSQLDATA, pData, Msg, p_rst);
}

int CDBEngine::XJInsertBlobData(int TableCode, BLOB_SQL_DATA& sql, char* sError)
{
	if(!m_hModule)
		return 0;
	if(!InsertBlobData)
		return 0;
	return InsertBlobData(TableCode, sql, sError);
}
