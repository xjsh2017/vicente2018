#if !defined(AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_)
#define AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBEngine.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDBEngine document

/**
 * @defgroup 	CDBEngine	数据库引擎
 * @{
 */
/**
 * @brief       创建数据库访问DLL接口连接
*  @author      LYH
 * @version     ver1.0
 * @date        27/09/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA803AA
class CDBEngine : public CDocument
{
public:
	//##ModelId=49B87BA803BA
	CDBEngine();           // protected constructor used by dynamic creation
	//##ModelId=49B87BA803BB
	~CDBEngine();
// Attributes
public:
	/** @brief           动态库句柄*/
	//##ModelId=49B87BA803BC
	HMODULE	m_hModule;

// Operations
public:

  int XJSelectBlogData( int TableCode, BLOB_SQL_DATA& sql, char* sError, CMemSet* pMem );

  int XJSelectRealData(int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg,  CMemSet * p_rst);
/*
 *  @brief   	XJSelectData	 查询接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @param 		[Out]a param of Type  CMemSet*  数据集
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803BD
	int	XJSelectData(int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem);

// 	int (*UpdateData)(int, SQL_DATA&, char*);	//更新数据
// 	int (*InsertData)(int, SQL_DATA&, char*);	//写数据
// 	int (*DeleteData)(int, SQL_DATA&, char*);	//删除数据

/*
 *  @brief   	XJUpdateData	 更新接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803C2
	int XJUpdateData(int TableCode, SQL_DATA& sql, char* sError);


/*
 *  @brief   	XJUpdateBlogData	 更新接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803C2
	int XJUpdateBlogData(int TableCode, BLOB_SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertData	 写数据接口
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803CA
	int XJInsertData(int TableCode, SQL_DATA& sql, char* sError);

	int XJInsertBlobData(int TableCode, BLOB_SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertMutiData	 批量写数据 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  查询条件结构
 *  @param 		[In]a param of Type  char*  错误输出
 *  @return 	int 成功返回1, 失败返回0
 */
	//##ModelId=49B87BA803CE
	int XJInsertMutiData(int TableName ,STTP_FULL_DATA& pRealData ,char* Msg);

/*
 *  @brief   	XJDeleteData	 删除数据接口 
 *  @param 		[In]a param of Type  int  数据表代号
 *  @param 		[In]a param of Type  SQL_DATA&  查询条件结构
 *  @param 		[Out]a param of Type  char*  错误输出
 *  @return 	int 成功返回1,失败返回0
 */
	//##ModelId=49B87BA803D2
	int XJDeleteData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	SetField	 设置查询字段 
 *  @param 		[Out]a param of Type  SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  Field&  字段结构
 *  @param 		[In]a param of Type  CString  字段名称
 *  @param 		[In]a param of Type  int  字段类型
 *  @param		[In]a param of Type  CString 字段值
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetField(SQL_DATA& sql, Field& field, CString sName, int nType, CString sValue = "");


/*
 *  @brief   	SetField	 设置查询字段 
 *  @param 		[Out]a param of Type  BLOB_SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  Field&  字段结构
 *  @param 		[In]a param of Type  CString  字段名称
 *  @param 		[In]a param of Type  int  字段类型
 *  @param		[In]a param of Type  CString 字段值
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetBlobField(BLOB_SQL_DATA& sql, BLOB_FIELD& field, CString sName, int nType, CString sValue = "");


/*
 *  @brief   	SetCondition	 设置查询条件 
 *  @param 		[Out]a param of Type  SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  Condition&  条件结构
 *  @param 		[In]a param of Type  CString  条件内容
 *  @param 		[In]a param of Type  int  条件类型
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetCondition(SQL_DATA& sql, Condition& con, CString sContent, int nType = 0);
	
/*
 *  @brief   	SetCondition	 设置查询条件 
 *  @param 		[Out]a param of Type  BLOB_SQL_DATA&  查询结构
 *  @param 		[In]a param of Type  BLOB_Condition&  条件结构
 *  @param 		[In]a param of Type  CString  条件内容
 *  @param 		[In]a param of Type  int  条件类型
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetBlobCondition(BLOB_SQL_DATA& sql, BLOB_CONDITION& con, CString sContent, int nType = 0);


/*
 *  @brief   	XJConnect	 连接数据库
 *  @param 		[In]a param of Type  int  模块名
 *  @param 		[In]a param of Type  char*  执行该接口返回的信息(一般为错误信息)
 *  @return 	int 0-失败, 1-成功
 */
	//##ModelId=49B87BA803E2
	int XJConnect(int pName, char* Msg);

/*
 *  @brief   	XJSetLogPath	 设置日志路径 
 *  @param 		[In]a param of Type  const char*  日志路径(全路径:统一路径/模块编号)
 *  @param 		[In]a param of Type  char*  执行该接口返回的信息(一般为错误信息)
 *  @return 	int 0-失败, 1-成功
 */
	//##ModelId=49B87BA803E5
	int XJSetLogPath(const char* LogPath, char* Msg);

/*
 *  @brief   	XJCloseSession	 简要的函数功能说明文字 
 *  @param 		[In/Out]a param of Type  CString&  输入参数说明
 *  @return 	int 
 */
	int XJCloseSession(CString& sLog);

	/*
	*  @brief   	XJExecuteSql	 直接执行标准的SQL语句 
	*  @param 		[In]a param of Type  MutiSQL_DATA&  放置一条SQL语句的结构体
	*  @param 		[Out]a param of Type  char*  执行查询操作返回的执行信息
	*  @param 		[Out]a param of Type  CMemSet *  返回的查询数据集
	*  @return 	int 成功返回1,失败返回0
	*/
	int XJExecuteSql(MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst);
public:
/*
 *  @brief   	LoadDBLib	 装载XJDBFacade.dll 
 *  @return 	BOOL 装载成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90002
	BOOL LoadDBLib();

	/*
	*  @brief   	FreeDBLib	 卸载XJDBFacade.dll 
	*  @return 	void 
	*/
	void FreeDBLib();

// Implementation
public:

	// Generated message map functions
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_)
