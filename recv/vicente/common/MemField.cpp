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
	m_nFieldType = 2;       //�ֶ���������
	m_nFlags     = 0;       //��־ֵ
}
CMemField::~CMemField()
{

}
/**	\brief	SetFieldName function               �����ֶ�����
	\param	s	a parameter of type const char *
	\return	void
*/
void CMemField::SetFieldName(const char *s)
{
	memset(m_strName,0,sizeof(m_strName)); 
	strncpy(m_strName,s,sizeof(m_strName)-1);
}
/**	\brief	The CMemField::GetFieldName function  �õ��ֶ�����
	\return	char*	
*/
char* CMemField::GetFieldName()
{
	return m_strName;
}
/**	\brief	SetFieldLength function               �����ֶ����ݳ���
	\param	nLength	a parameter of type UINT
	\return	void	
*/
void CMemField::SetFieldLength(UINT nLength)
{
	m_nLength = nLength;
}
/**	\brief	GetFieldLength function               �õ��ֶ����ݳ���
	\return	UINT	
*/
UINT CMemField::GetFieldLength()
{
	return m_nLength;
}
/**	\brief	SetFieldType function  �����ֶ����ݵ�����
	\param	nFieldType	a parameter of type UINT
	\return	int	
*/
int CMemField::SetFieldType(UINT nFieldType)
{
	m_nFieldType = nFieldType;
	return 0;
}
/**	\brief	GetFieldType function	�õ��ֶ����ݵ�����
	\return	UINT	
*/
UINT CMemField::GetFieldType()
{
	return m_nFieldType;
}
/**	\brief	SetFieldFlag function   �����ֶεı�־ֵ
	\param	nFlag	a parameter of type UINT
	\return	int	
*/
int CMemField::SetFieldFlag(UINT nFlag)
{
	m_nFlags = nFlag;
	return 0;
}
/**	\brief	GetFieldFlag function   �õ��ֶεı�־ֵ
	\return	UINT	
*/
UINT CMemField::GetFieldFlag()
{
	return m_nFlags;
}
