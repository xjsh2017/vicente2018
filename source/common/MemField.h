// MemField.h: interface for the CMemField class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_MEMFIELD_H__775B354B_DA9D_49C4_838F_A988AE7CAB56__INCLUDED_
#define AFX_MEMFIELD_H__775B354B_DA9D_49C4_838F_A988AE7CAB56__INCLUDED_
//�ֶ�
//#include "db_constant.h"
#include "EntityContainer.h"
/**
 * @defgroup 
 * @{
 */
/**
 * @brief    CMemField     �ڴ���ֶ�����                    
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
    char  m_strName[50];      //�ֶ�����
	UINT  m_nLength;          //�ֶγ���
	UINT  m_nFieldType;       //�ֶ���������
	UINT  m_nFlags;           //��־ֵ�������ͱ�ʶ��
};
#endif
