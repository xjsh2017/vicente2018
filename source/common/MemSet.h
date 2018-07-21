// MemSet.h: interface for the CMemSet class.
//
//////////////////////////////////////////////////////////////////////
/********************************************************************
*	filename: 	MemSet-内存表
*	author:		yinzhehong
*	created:	2006/10/23
*	purpose:  	用于存放从磁盘表导出后的数据：查询结果
*               用于内存数据管理
*   version:    1.0
*********************************************************************/

#ifndef AFX_MEMSET_H__B7D5B489_1EB2_421B_8727_651C954B95A8__INCLUDED_
#define AFX_MEMSET_H__B7D5B489_1EB2_421B_8727_651C954B95A8__INCLUDED_

#include "define.h"
#include "MemField.h"
#include "XJLock.h"
#include "MemRow.h"

typedef char ** SQL_ROW;		/* 每一行的记录为一个字符串数组：*/

/**
 * @defgroup 
 * @{
 */
/**
 * @brief    CMemSet       数据访问后存放的集合，也是内存库数据存放的集合                
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/18
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
class CMemSet
{
public:
	CMemSet();
	virtual ~CMemSet();

public:
/*
 *  @brief   	SetTableName	 设置表名字符串 
 *  @param 		[Out]a param of Type  const char *  表名字符串
 *  @return 	int 
 */
	int SetTableName(const char * pTbName);

/*
 *  @brief   	SetValue	 根据字段名设置字段的值 
 *  @param 		[In]a param of Type  const char *  字段的英文名
 *  @param 		[In]a param of Type  const char*  需要设置的值
 *  @return 	int 
 */
	int SetValue(const char* name, const char* value);

/*
 *  @brief   	SetValue	 根据字段位置设置字段的值 
 *  @param 		[In]a param of Type  UINT  字段的位置（第几列）
 *  @param 		[In]a param of Type  const char*  需要设置的值
 *  @return 	int 
 */
	int SetValue(UINT nField, const char* value);

public:
/*
 *  @brief   	GetMemFieldNum	 获取字段数目 
 *  @return 	UINT 字段数目
 */
	UINT GetMemFieldNum();

/*
 *  @brief   	GetMemRowNum	 获得记录集总行数 
 *  @return 	UINT 记录集总行数
 */
	UINT GetMemRowNum();

/*
 *  @brief   	GetCurRowNum	 得到当前遍历到的行数 
 *  @return 	UINT 当前遍历到的行数
 */
	UINT GetCurRowNum();

/*
 *  @brief   	GetTableName	 获得表名字符串 
 *  @return 	char * 表名字符串
 */
	char * GetTableName();

/*
 *  @brief   	GetFieldName	 获得字段名称字符串 
 *  @param 		[In]a param of Type  UINT  字段位置（第几列）
 *  @return 	char * 字段名称字符串
 */
	char* GetFieldName(UINT nField);

/*
 *  @brief   	GetFieldType	 获得字段数据类型 
 *  @param 		[In]a param of Type  UINT  字段位置（第几列）
 *  @return 	int 
 */
	int GetFieldType(UINT nField);

/*
 *  @brief   	GetValue	 根据字段位置获得字段的值 
 *  @param 		[In]a param of Type  UINT  字段位置（第几列）
 *  @return 	char * 字段的值，失败为NULL
 */
	char * GetValue(UINT nField);

/*
 *  @brief   	GetValue	 根据字段名获得字段的值（区分大小写） 
 *  @param 		[In]a param of Type  const char*  字段英文名
 *  @return 	char * 字段的值，失败为NULL
 */
	char * GetValue(const char* pFieldname);

/*
 *  @brief   	GetRealDataValue	 根据字段名获得字段的值（适配小写） 
 *  @param 		[In]a param of Type  const char*  字段英文名
 *  @return 	char * 字段的值，失败为NULL
 */
	char * GetRealDataValue(const char* pLowerFieldname);

/*
 *  @brief   	GetRealValue	 根据字段名获取某一行的值（适配小写）
 *  @param 		[In]a param of Type  const char*  字段英文名
 *  @param 		[In]a param of Type  UINT  行号
 *  @return 	char * 字段的值，失败为NULL
 */
	char * GetRealValue(const char* pFieldname, UINT nRow);

/*
 *  @brief   	GetRealNumValue	 根据列号获取某一行的值 
 *  @param 		[In]a param of Type  UINT  行号
 *  @param 		[In]a param of Type  UINT  列号
 *  @return 	char * 字段的值，失败为NULL
 */
	char * GetRealNumValue(UINT nRow, UINT nField);

/*
 *  @brief   	MoveFirst	 移动到第一行 
 *  @return 	int 0 成功 -1 失败
 */
	int	MoveFirst();

/*
 *  @brief   	MoveNext	 移动到下一行 
 *  @return 	int 
 */
	int MoveNext();

/*
 *  @brief   	MoveForward	 向前移动一行 
 *  @return 	int 0
 */
	int MoveForward();

/*
 *  @brief   	MoveNextTest	 往下移动一行,记住当前位置 
 *  @return 	int 
 */
	int MoveNextTest();
	
/*
 *  @brief   	MoveToUserRow	 移动到用户指定的行 
 *  @param 		[In]a param of Type  int  指定的行
 *  @return 	int 
 */
	int MoveToUserRow(int nRownum);

public:
/*
 *  @brief   	GetFieldLength	 获取字段长度（废弃） 
 *  @param 		[In]a param of Type  UINT  列号
 *  @return 	int 
 */
	int GetFieldLength(UINT nField);

/*
 *  @brief   	FreeData	 释放内存 
 *  @param 		[In]a param of Type  bool  是否释放字段定义
 *  @return 	int 
 */
	int FreeData(bool bFreeAll);

public:
/*
 *  @brief   	FreeFieldlistonly	 仅删除字段信息列表 
 *  @return 	int 
 */
	int FreeFieldlistonly();

public:
/*
 *  @brief   	GetRow	 按行号取得数据行指针 
 *  @param 		[In]a param of Type  int  行号
 *  @return 	SQL_ROW 数据行指针
 */
	SQL_ROW GetRow(int nposition);

/*
 *  @brief   	GetCurRow	 得到当前数据行指针 
 *  @return 	SQL_ROW 当前数据行指针
 */
	SQL_ROW GetCurRow();

/*
 *  @brief   	SetLifeTime	 设置数据集的生命期 
 *  @param 		[In]a param of Type  UINT  数据集的生命期
 *  @return 	int 
 */
	int SetLifeTime(UINT nLiftTime);

/*
 *  @brief   	GetLifeTime	 得到表当前的生存时间 
 *  @return 	UINT 当前的生存时间
 */
    UINT GetLifeTime();

public:
/*
 *  @brief   	AddRow	 添加一行数据 
 *  @param 		[In]a param of Type  SQL_ROW  数据行
 *  @return 	int 
 */
	int AddRow(SQL_ROW row);

/*
 *  @brief   	AddRows	 添加多行数据 
 *  @param 		[In]a param of Type  CMemSet*  数据列表
 *  @return 	int 0
 */
	int AddRows(CMemSet* pMemSet);

/*
 *  @brief   	AtatchField	 增加字段描述 
 *  @param 		[In]a param of Type  CMemField*  字段列表
 *  @param 		[In]a param of Type  UINT  字段数
 *  @return 	int 0：成功 -1：失败
 */
	int AtatchField(CMemField* pMemFildList, UINT nFieldNum);

/*
 *  @brief   	AtatchRow	 加入数据行 
 *  @param 		[In]a param of Type  CMemRow*  数据行指针
 *  @param 		[In]a param of Type  UINT  数据行数
 *  @return 	int 0：成功 -1：失败
 */
	int AtatchRow(CMemRow* pMemRowList, UINT nRowNum);

/*
 *  @brief   	GetRowLength	 该行数据的总长度 
 *  @param 		[In]a param of Type  int  行数
 *  @return 	int 总长度
 */
	int GetRowLength(int nRowNum);

private:
/*
 *  @brief   	ColoneRow	 拷贝数据行 
 *  @param 		[In]a param of Type  SQL_ROW  待拷贝的数据行
 *  @return 	SQL_ROW 
 */
	SQL_ROW ColoneRow(SQL_ROW pSourceRow);
	
/*
 *  @brief   	StringToLower	 字符串最小化 
 *  @param 		[In]a param of Type  char*  待处理的字符串
 *  @return 	string 最小化后的字符串
 */
	string StringToLower(char* p_val);

private:
/**	\brief 数据集合*/
	vector<SQL_ROW> m_listData;

/**	\brief 当前行指针*/
	SQL_ROW m_CurmemRow;

/**	\brief 字段个数*/
	UINT m_nFieldNum;

/**	\brief 当前行号*/
	UINT m_nCurRow;

/**	\brief 总行数*/
	UINT m_nRowCount;

/**	\brief 字段列表*/
	CMemField* m_pFiledList;

/**	\brief 允许的最大行数*/
	UINT m_nMaxRow;

/**	\brief 表名*/
	char m_chTablename[200];

/**	\brief 存在时间（未使用）*/
	UINT m_nExistTime;

/**	\brief 行列表*/
	CMemRow* m_pRowList;

/**	\brief 行数目*/
	UINT m_nRowNum;

/**	\brief 脏类型  0: 新增 1：修改 2：删除，暂未用*/
	int m_nDirtyType;

/**	\brief 锁*/
	CXJLock m_lockMemsetAccess;
};

#endif
