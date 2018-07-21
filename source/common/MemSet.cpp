// MemSet.cpp: implementation of the CMemSet class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#pragma warning(disable:4786)
#include "MemSet.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemSet::CMemSet():m_lockMemsetAccess("Lock RealMemSetData")
{
	memset(m_chTablename, '\0', sizeof(m_chTablename));//表名
	m_pFiledList = NULL;//字段指针
	m_nFieldNum  = 0;//字段数目
	m_CurmemRow  = NULL;//当前行指针
	m_nCurRow    = 0;//当前是第几行
	m_nRowCount  = 0;//一共多少行数据
    m_nMaxRow    = 1000000;//最大存放行
	m_nExistTime = 0;//表存在的时间
	m_pRowList   = NULL;
	m_nRowNum    = 0;
	m_nDirtyType = 0;

	m_listData.clear();
}

//************************************
// 函数名:    ~CMemSet
// 功能概述:  析构
// 返回值:   
//************************************
CMemSet::~CMemSet()
{
	//释放数据
	FreeData(true);
}

//************************************
// 函 数 名:  FreeData
// 功能概要:  释放数据
// 访问权限:  public 
// 返 回 值:  int 0:成功
// 参    数:  bool bFreeAll 是否清空全部，if true 字段定义也将被释放
//************************************
int CMemSet::FreeData(bool bFreeAll)
{
	SQL_ROW pTempRowdata = NULL;
    vector<SQL_ROW>::iterator itRowitor = m_listData.begin();
	for (; itRowitor != m_listData.end(); itRowitor++)
	{
		pTempRowdata = *itRowitor;
		if (pTempRowdata == NULL)
		{
			--m_nRowCount;
			continue;
		}

		for(int j = 0; j < m_nFieldNum; j++)
		{
			if(pTempRowdata[j] != NULL)
			{
				delete [] pTempRowdata[j]; //删除每行数据
				pTempRowdata[j] = NULL;
			}			
		}
		
		if (pTempRowdata != NULL)
		{
			delete[] pTempRowdata;
			pTempRowdata = NULL;
		}
		
		--m_nRowCount;
	}
	m_listData.clear();

	m_nCurRow   = 0;
	m_CurmemRow = NULL;
	
	if(bFreeAll)//释放字段定义
	{
		if(m_pFiledList != NULL)
		{
			delete []m_pFiledList;
			m_pFiledList = NULL;
		}
		m_nFieldNum = 0; 
	}

	if (bFreeAll)
	{
		if (m_pRowList != NULL)
		{
			delete []m_pRowList;
			m_pRowList = NULL;
		}
	}

	return 0;
}

//************************************
// 函 数 名:  MoveFirst
// 功能概要:  移动到第一行
// 访问权限:  public 
// 返 回 值:  int 0 成功 -1 失败
//************************************
int CMemSet::MoveFirst()
{
	if(m_nRowCount == 0 )
	{
		return -1;
	}

// 	if(m_nCurRow == 0)
// 	{
// 		return 0;
// 	}

	m_CurmemRow = (SQL_ROW) m_listData.at(0);
	m_nCurRow = 0;

	return 0;
}

//************************************
// 函 数 名:  MoveNext
// 功能概要:  移动到下一行
// 访问权限:  public 
// 返 回 值:  int 
//************************************
int CMemSet::MoveNext()
{
	if(m_nCurRow >= m_nRowCount)
	{
		return -1;
	}

	m_nCurRow++;

	if (m_nCurRow >= m_listData.size())
	{
		MoveFirst();//回头
	}
	else
	{
		m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	}
	
	return m_nCurRow;//返回的是当前的行数
}

//************************************
// 函 数 名:  MoveNextTest
// 功能概要:  往下移动一行,记住当前位置
// 访问权限:  public 
// 返 回 值:  int 当前行
//************************************
int CMemSet::MoveNextTest()
{
	if(m_nCurRow >= m_nRowCount)
	{
		return -1;
	}

	m_nCurRow++;

	if (m_nCurRow >= m_listData.size())
	{
		MoveFirst();//回头
	}
	else
	{
		m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	}

	return m_nCurRow;
}

//************************************
// 函 数 名:  MoveForward
// 功能概要:  向前移动一行
// 访问权限:  public 
// 返 回 值:  int 0
//************************************
int CMemSet::MoveForward()
{
	if(m_nCurRow == 0 || m_nRowCount == 0)
	{
		return 0;
	}

	m_nCurRow--;

	if (m_nCurRow < 0)
	{
		return -1;
	}

	if (m_nCurRow >= m_listData.size())
	{
		return -1;
	}

	m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);

	return 0;
}

//************************************
// 函 数 名:  MoveToUserRow
// 功能概要:  移动到用户指定的行
// 访问权限:  public 
// 返 回 值:  int 
// 参    数:  int nRownum 指定的行
//************************************
int CMemSet::MoveToUserRow( int nRownum )
{
	if((nRownum >= m_nRowCount) || (nRownum < 0))
	{
		return -1;
	}

	if (nRownum >= m_listData.size())
	{
		return -1;
	}
	
	m_nCurRow = nRownum;//当前行标志
	
	m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	
	return m_nCurRow;
}

//************************************
// 函 数 名:  GetValue
// 功能概要:  根据字段名获得字段的值（区分大小写）
// 访问权限:  public 
// 返 回 值:  char* 字段的值，失败为NULL
// 参    数:  const char * pFieldname 字段英文名
//************************************
char* CMemSet::GetValue(const char* pFieldname)
{
	if(pFieldname == NULL || m_CurmemRow == NULL)
	{
		return NULL;
	}

	int nField = -1;

	for(int i = 0; i < m_nFieldNum; i++)//得到字段所在编号
	{
		if (m_pFiledList[i].GetFieldName() != NULL)
		{
			if(strcmp(m_pFiledList[i].GetFieldName(), pFieldname) == 0)
			{
				nField = i;
				break;
			}
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return GetValue(nField);
}

//************************************
// 函 数 名:  GetRealDataValue
// 功能概要:  根据字段名获得字段的值（适配小写）
// 访问权限:  public 
// 返 回 值:  char* 字段的值，失败为NULL
// 参    数:  const char * pLowerFieldname 字段英文名
//************************************
char* CMemSet::GetRealDataValue(const char* pLowerFieldname)
{
	if(pLowerFieldname == NULL || m_CurmemRow == NULL)
	{
		return NULL;
	}

	int nField = -1;
	char* pFieldName = NULL;
	string strMemsetLowerFieldname = "";
	string strLowerFieldname = StringToLower((char*)pLowerFieldname);

	for(int i = 0; i < m_nFieldNum; i++)//得到字段所在编号
	{
		pFieldName = m_pFiledList[i].GetFieldName();
		if (pFieldName == NULL)
		{
			continue;
		}

		strMemsetLowerFieldname = StringToLower(pFieldName);
		if(strcmp(strMemsetLowerFieldname.c_str(), strLowerFieldname.c_str()) == 0)
		{
			nField = i;
			break;
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return 	GetValue(nField);
}

//************************************
// 函 数 名:  GetValue
// 功能概要:  根据字段位置获得字段的值
// 访问权限:  public 
// 返 回 值:  char* 字段的值，失败为NULL
// 参    数:  UINT nField 字段位置（第几列）
//************************************
char* CMemSet::GetValue(UINT nField)
{
	if(nField >= m_nFieldNum || m_CurmemRow == NULL)
	{
		return NULL;
	}

	return m_CurmemRow[nField];//返回该值的字符串指针
}

//************************************
// 函 数 名:  GetRealValue
// 功能概要:  根据字段名获取某一行的值（适配小写）
// 访问权限:  public 
// 返 回 值:  char* 字段的值，失败为NULL
// 参    数:  const char * pFieldname 字段英文名
// 参    数:  UINT nRow 行号
//************************************
char* CMemSet::GetRealValue(const char* pFieldname, UINT nRow)
{
	int nField = -1;
	if (pFieldname == NULL)
	{
		return NULL;
	}
	
	CMemField* pTempFieldList = m_pFiledList;
	string strMemsetLowerFieldname = "";

	for(int i = 0; i < m_nFieldNum; i++)//得到字段所在编号
	{
		if (pTempFieldList[i].GetFieldName() == NULL)
		{
			continue;
		}

		strMemsetLowerFieldname = StringToLower(pTempFieldList[i].GetFieldName());//最小化
		if (strMemsetLowerFieldname.size() > 0)
		{
			if(strcmp(strMemsetLowerFieldname.c_str(), pFieldname) == 0)
			{
				nField = i;
				break;
			}
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return GetRealNumValue(nRow, nField);
}

//************************************
// 函 数 名:  GetRealNumValue
// 功能概要:  根据列号获取某一行的值
// 访问权限:  public 
// 返 回 值:  char* 字段的值，失败为NULL
// 参    数:  UINT nRow 行号
// 参    数:  UINT nField 列号
//************************************
char* CMemSet::GetRealNumValue(UINT nRow, UINT nField)
{
	if(nField >= m_nFieldNum)
	{
		return NULL;
	}
	
	CLockUp lockup(&m_lockMemsetAccess);//加锁


	if (nRow >= m_listData.size())
	{
		return NULL;
	}

	SQL_ROW pTempRow = (SQL_ROW) m_listData.at(nRow);//获取值
	if (pTempRow != NULL)
	{
		return pTempRow[nField];
	}

	return NULL;
}

//************************************
// 函数名:    SetValue
// 功能概述:  根据字段名称设置当前行某个字段的值
// 返回值:   int 0:成功
// 参数: const char * name 字段名
// 参数: const char * value 需要设置的值
//************************************
int CMemSet::SetValue(const char *name,const char *value)
{
	int nField = -1;
	if(name == NULL || m_CurmemRow == NULL)
	{
		return -1;
	}
	//得到字段所在编号
	char* pFieldName = NULL;
	CMemField * m_temp_field = m_pFiledList;
	string strLowerField = "";

	for(int i = 0; i < m_nFieldNum; i++)
	{
		pFieldName = m_temp_field[i].GetFieldName();
		if (pFieldName != NULL)
		{
			strLowerField = StringToLower(pFieldName);//最小化
			if (strLowerField.size() > 0)
			{
				if(strcmp(strLowerField.c_str(), name) == 0)
				{
					nField = i;
					break;
				}
			}
		}
	}
	if(nField == -1)
	{
		return -1;
	}
	//根据字段序号设置当前行某个字段的值
	return SetValue(nField,value);
}

//************************************
// 函数名:    SetValue
// 功能概述:  根据字段序号设置当前行某个字段的值
// 返回值:   int 0:成功
// 参数: UINT nField 字段序号
// 参数: const char * value 字段值
//************************************
int CMemSet::SetValue(UINT nField,const char* value)
{
	if(nField >= m_nFieldNum || m_CurmemRow == NULL)
	{
		return NULL;
	}

	CLockUp lockUp(&m_lockMemsetAccess);

	if (m_CurmemRow[nField] != NULL)
	{
		delete []m_CurmemRow[nField];
		m_CurmemRow[nField] = NULL;
	}
	if(value != NULL)//赋为非空值
	{
		int nLength = strlen(value);
		if(nLength > 0)
		{
			char* newvale = new char[nLength + 1];
			if (newvale != NULL)
			{
				memset(newvale, '\0', (nLength+1) );
				memcpy(newvale, value, nLength);
				m_CurmemRow[nField] = newvale;
			}
		}
	}
	return 0;
}

//************************************
// 函 数 名:  AddRow
// 功能概要:  增加一行数据到末尾
// 访问权限:  public 
// 返 回 值:  int 增加后的数据行数
// 参    数:  SQL_ROW row 数据行
//************************************
int CMemSet::AddRow(SQL_ROW row)
{
	CLockUp lockUp(&m_lockMemsetAccess);

	if(m_nRowCount == 0)
	{
		m_CurmemRow = row;
	}
	m_listData.push_back(row);//添加到末尾
	m_nRowCount++;
	
	return m_nRowCount;
}

//************************************
// 函 数 名:  AtatchField
// 功能概要:  增加字段描述
// 访问权限:  public 
// 返 回 值:  int 0：成功 -1：失败
// 参    数:  CMemField * pMemFildList 字段列表
// 参    数:  UINT nFieldNum 字段数
//************************************
int CMemSet::AtatchField(CMemField *pMemFildList, UINT nFieldNum)
{
	if(pMemFildList == NULL || nFieldNum == 0)
	{
		return -1;
	}

	if(m_pFiledList != NULL)
	{
		delete []m_pFiledList;
		m_pFiledList = NULL;
	}

	m_pFiledList = pMemFildList;
	m_nFieldNum  = nFieldNum;

	return 0;
}

//************************************
// 函 数 名:  SetLifeTime
// 功能概要:  设置内存表生存时间
// 访问权限:  public 
// 返 回 值:  int 0:成功
// 参    数:  UINT nLiftTime 生存时间
//************************************
int CMemSet::SetLifeTime(UINT nLiftTime)
{
	m_nExistTime = nLiftTime;

	return 0;
}

//************************************
// 函 数 名:  GetLifeTime
// 功能概要:  得到表当前的生存时间
// 访问权限:  public 
// 返 回 值:  UINT 当前的生存时间
//************************************
UINT CMemSet::GetLifeTime()
{
	return m_nExistTime;
}

//************************************
// 函 数 名:  GetMemRowNum
// 功能概要:  得到记录集行数
// 访问权限:  public 
// 返 回 值:  UINT 行数
//************************************
UINT CMemSet::GetMemRowNum()
{
	return m_nRowCount;
}

//************************************
// 函 数 名:  GetMemFieldNum
// 功能概要:  得到列数(字段数)
// 访问权限:  public 
// 返 回 值:  UINT 列数
//************************************
UINT CMemSet::GetMemFieldNum()
{
	return m_nFieldNum;
}

//************************************
// 函 数 名:  GetFieldType
// 功能概要:  得到指定列的数据类型
// 访问权限:  public 
// 返 回 值:  int 字段数据类型
// 参    数:  UINT nField 字段序号
//************************************
int CMemSet::GetFieldType(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return -1;
	}

	return m_pFiledList[nField].GetFieldType();
}

//************************************
// 函 数 名:  GetFieldName
// 功能概要:  得到指定列的列名称
// 访问权限:  public 
// 返 回 值:  char* 字段名
// 参    数:  UINT nField 列序号
//************************************
char* CMemSet::GetFieldName(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return NULL;
	}

	return m_pFiledList[nField].GetFieldName();
}

//************************************
// 函 数 名:  AddRows
// 功能概要:  在表后追加表数据,此方法为拷贝方式添加
// 访问权限:  public 
// 返 回 值:  int 0
// 参    数:  CMemSet * pMemSet 数据集
//************************************
int CMemSet::AddRows(CMemSet *pMemSet)
{
	if(pMemSet == NULL)
	{
		return -1;
	}

	if(pMemSet->GetMemRowNum() == 0 || pMemSet->GetMemFieldNum() == 0)//无数据
	{
		return 0;
	}

	SQL_ROW	memRow = NULL;
	for(int i = 0; i < pMemSet->GetMemRowNum(); i++)
	{
		memRow = ColoneRow(pMemSet->GetRow(i));
		if(memRow != NULL)
		{
			AddRow(memRow);
		}
	}

	return 0;
}

//************************************
// 函 数 名:  GetRow
// 功能概要:  得到指定的行数据
// 访问权限:  public 
// 返 回 值:  SQL_ROW 数据行 0 <= nposition < rowcount
// 参    数:  int nposition 位置
//************************************
SQL_ROW CMemSet::GetRow(int nposition)
{
	if((nposition < 0) || nposition > (m_nRowCount -1))
	{
		return NULL;
	}

	return (SQL_ROW) m_listData.at(nposition);
}

//************************************
// 函 数 名:  ColoneRow
// 功能概要:  克隆一行数据
// 访问权限:  private 
// 返 回 值:  SQL_ROW 数据行
// 参    数:  SQL_ROW pSourceRow 复制的数据行
//************************************
SQL_ROW CMemSet::ColoneRow(SQL_ROW pSourceRow)
{
	if(pSourceRow == NULL || m_nFieldNum == 0)
	{
		return NULL;
	}

	SQL_ROW	memRow  = new  char* [m_nFieldNum];//new出列存放数据
	if(memRow == NULL)
	{
		return NULL;
	}

	char* pFieldData = NULL;
	UINT nFieldLength = 0;
	for(UINT i = 0; i < m_nFieldNum; i++)//先将数据指针赋空，然后再根据实际长度new内存，new完后再行删除
	{
		memRow[i] = NULL;              //指针中的值默认为空
		if(pSourceRow[i] != NULL)
		{
			pFieldData   = NULL;
			nFieldLength = strlen(pSourceRow[i]);
			pFieldData   = new char[nFieldLength + 1];
			if(pFieldData != NULL)
			{
				memset(pFieldData, '\0', (nFieldLength+1));
				memcpy(pFieldData, pSourceRow[i], nFieldLength);//拷贝数据
				memRow[i] = pFieldData;                       //放入数组
			}
		}		
	}

	return memRow;
}

//************************************
// 函 数 名:  AtatchRow
// 功能概要:  加入数据行
// 访问权限:  public 
// 返 回 值:  int 0：成功 -1：失败
// 参    数:  CMemRow * pMemRowList 数据行指针 
// 参    数:  UINT nRowNum 数据行数
//************************************
int CMemSet::AtatchRow(CMemRow* pMemRowList, UINT nRowNum)
{
	if (pMemRowList == NULL || nRowNum == 0)
	{
		return -1;
	}

	if (m_pRowList != NULL)
	{
		delete []m_pRowList;
		m_pRowList = NULL;
	}

	m_pRowList = pMemRowList;
	m_nRowNum  = nRowNum;
	
	return 0;
}

//************************************
// 函 数 名:  GetRowLength
// 功能概要:  获取行的数据长度
// 访问权限:  public 
// 返 回 值:  int 总长度
// 参    数:  int nRowNum 行数
//************************************
int CMemSet::GetRowLength(int nRowNum)
{
	if(nRowNum < 0 || nRowNum > m_nRowNum)
	{
		return -1;
	}
	return m_pRowList[nRowNum].GetRowLength();
}

//************************************
// 函 数 名:  GetFieldLength
// 功能概要:  获取字段长度（废弃）
// 访问权限:  public 
// 返 回 值:  int 
// 参    数:  UINT nField 列号
//************************************
int CMemSet::GetFieldLength(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return -1;
	}

	return m_pFiledList[nField].GetFieldLength();
}

//************************************
// 函 数 名:  SetTableName
// 功能概要:  设置表名称
// 访问权限:  public 
// 返 回 值:  int 0 成功，1 参数无效
// 参    数:  const char * pTbName 表名字
//************************************
int CMemSet::SetTableName(const char* pTbName)
{
	if (pTbName == NULL)
	{
		return -1;
	}
	
	if(strlen(pTbName) > 199)//长度超过199
	{
		return -1;
	}

	memset(m_chTablename, '\0', sizeof(m_chTablename));
	memcpy(m_chTablename, pTbName, strlen(pTbName));
	
	return 0;
}

//************************************
// 函 数 名:  GetTableName
// 功能概要:  得到表名
// 访问权限:  public 
// 返 回 值:  char * 表名
//************************************
char* CMemSet::GetTableName()
{
	return m_chTablename;
}

//************************************
// 函 数 名:  StringToLower
// 功能概要:  字符串最小化
// 访问权限:  private 
// 返 回 值:  string 最小化后的字符串
// 参    数:  char * p_val 待处理的字符串
//************************************
string CMemSet::StringToLower(char* p_val)
{
	string strLowerStr = "";
	
	if (p_val == NULL)
	{
		return strLowerStr;
	}
	
	char* p = p_val;
	for (p = p_val; *p; p++)
	{
		if ((*p >= ' ') && (*p < 128))
		{
			strLowerStr += tolower(*p);
		}
		else
		{
			strLowerStr += *p;
		}
	}
	return strLowerStr;
}

//************************************
// 函 数 名:  FreeFieldlistonly
// 功能概要:  仅删除字段信息列表
// 访问权限:  public 
// 返 回 值:  int 
//************************************
int CMemSet::FreeFieldlistonly()
{
	m_nRowCount = 0;
	m_nCurRow   = 0;
	m_CurmemRow = NULL;
	m_listData.clear();
	if(m_pFiledList != NULL)
	{
		delete []m_pFiledList;
		m_pFiledList = NULL;
	}

	m_nFieldNum = 0; 
	if (m_pRowList != NULL)
	{
		delete []m_pRowList;
		m_pRowList = NULL;
	}

	return 0;
}

//************************************
// 函 数 名:  GetCurRow
// 功能概要:  得到当前数据行指针
// 访问权限:  public 
// 返 回 值:  SQL_ROW 当前数据行指针
//************************************
SQL_ROW CMemSet::GetCurRow()
{
	return m_CurmemRow;
}

//************************************
// 函 数 名:  GetCurRowNum
// 功能概要:  得到当前遍历到的行数
// 访问权限:  public 
// 返 回 值:  UINT 当前遍历到的行数
//************************************
UINT CMemSet::GetCurRowNum()
{
	return m_nCurRow;
}

