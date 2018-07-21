// MemField.h: interface for the CMemField class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_MEMFIELD_H__775B354B_DA9D_49C4_838F_A988AE7CAB56__INCLUDED_
#define AFX_MEMFIELD_H__775B354B_DA9D_49C4_838F_A988AE7CAB56__INCLUDED_
//字段
//#include "db_constant.h"
#include "EntityContainer.h"
/**
 * @defgroup 
 * @{
 */
/**
 * @brief    CMemField     内存表字段描述                    
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/18
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
class CMemField
{
public:
	CMemField();
	virtual ~CMemField();
	
public:
	void  SetFieldName(const char* s);
	char* GetFieldName();
	
	UINT  GetFieldType();
	int   SetFieldType(UINT nFieldType);

	UINT  GetFieldLength();
	void  SetFieldLength(UINT nLength);
	
	UINT  GetFieldFlag();
	int   SetFieldFlag(UINT nFlag);
private:
    char  m_strName[50];      //字段名称
	UINT  m_nLength;          //字段长度
	UINT  m_nFieldType;       //字段数据类型
	UINT  m_nFlags;           //标志值，键类型标识别
};
#endif
