// MemField.cpp: implementation of the CMemField class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "MemField.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemField::CMemField()
{
	m_nLength = 0;
	memset(m_strName,'\0',sizeof(m_strName)); 
	m_nFieldType = 2;       //字段数据类型
	m_nFlags     = 0;       //标志值
}
CMemField::~CMemField()
{

}
/**	\brief	SetFieldName function               设置字段名称
	\param	s	a parameter of type const char *
	\return	void
*/
void CMemField::SetFieldName(const char *s)
{
	memset(m_strName,0,sizeof(m_strName)); 
	strncpy(m_strName,s,sizeof(m_strName)-1);
}
/**	\brief	The CMemField::GetFieldName function  得到字段名称
	\return	char*	
*/
char* CMemField::GetFieldName()
{
	return m_strName;
}
/**	\brief	SetFieldLength function               设置字段数据长度
	\param	nLength	a parameter of type UINT
	\return	void	
*/
void CMemField::SetFieldLength(UINT nLength)
{
	m_nLength = nLength;
}
/**	\brief	GetFieldLength function               得到字段数据长度
	\return	UINT	
*/
UINT CMemField::GetFieldLength()
{
	return m_nLength;
}
/**	\brief	SetFieldType function  设置字段数据的类型
	\param	nFieldType	a parameter of type UINT
	\return	int	
*/
int CMemField::SetFieldType(UINT nFieldType)
{
	m_nFieldType = nFieldType;
	return 0;
}
/**	\brief	GetFieldType function	得到字段数据的类型
	\return	UINT	
*/
UINT CMemField::GetFieldType()
{
	return m_nFieldType;
}
/**	\brief	SetFieldFlag function   设置字段的标志值
	\param	nFlag	a parameter of type UINT
	\return	int	
*/
int CMemField::SetFieldFlag(UINT nFlag)
{
	m_nFlags = nFlag;
	return 0;
}
/**	\brief	GetFieldFlag function   得到字段的标志值
	\return	UINT	
*/
UINT CMemField::GetFieldFlag()
{
	return m_nFlags;
}
