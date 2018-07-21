// MemSet.h: interface for the CMemSet class.
//
//////////////////////////////////////////////////////////////////////
/********************************************************************
*	filename: 	MemSet-�ڴ��
*	author:		yinzhehong
*	created:	2006/10/23
*	purpose:  	���ڴ�ŴӴ��̱���������ݣ���ѯ���
*               �����ڴ����ݹ���
*   version:    1.0
*********************************************************************/

#ifndef AFX_MEMSET_H__B7D5B489_1EB2_421B_8727_651C954B95A8__INCLUDED_
#define AFX_MEMSET_H__B7D5B489_1EB2_421B_8727_651C954B95A8__INCLUDED_

#include "define.h"
#include "MemField.h"
#include "XJLock.h"
#include "MemRow.h"

typedef char ** SQL_ROW;		/* ÿһ�еļ�¼Ϊһ���ַ������飺*/

/**
 * @defgroup 
 * @{
 */
/**
 * @brief    CMemSet       ���ݷ��ʺ��ŵļ��ϣ�Ҳ���ڴ�����ݴ�ŵļ���                
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
 *  @brief   	SetTableName	 ���ñ����ַ��� 
 *  @param 		[Out]a param of Type  const char *  �����ַ���
 *  @return 	int 
 */
	int SetTableName(const char * pTbName);

/*
 *  @brief   	SetValue	 �����ֶ��������ֶε�ֵ 
 *  @param 		[In]a param of Type  const char *  �ֶε�Ӣ����
 *  @param 		[In]a param of Type  const char*  ��Ҫ���õ�ֵ
 *  @return 	int 
 */
	int SetValue(const char* name, const char* value);

/*
 *  @brief   	SetValue	 �����ֶ�λ�������ֶε�ֵ 
 *  @param 		[In]a param of Type  UINT  �ֶε�λ�ã��ڼ��У�
 *  @param 		[In]a param of Type  const char*  ��Ҫ���õ�ֵ
 *  @return 	int 
 */
	int SetValue(UINT nField, const char* value);

public:
/*
 *  @brief   	GetMemFieldNum	 ��ȡ�ֶ���Ŀ 
 *  @return 	UINT �ֶ���Ŀ
 */
	UINT GetMemFieldNum();

/*
 *  @brief   	GetMemRowNum	 ��ü�¼�������� 
 *  @return 	UINT ��¼��������
 */
	UINT GetMemRowNum();

/*
 *  @brief   	GetCurRowNum	 �õ���ǰ������������ 
 *  @return 	UINT ��ǰ������������
 */
	UINT GetCurRowNum();

/*
 *  @brief   	GetTableName	 ��ñ����ַ��� 
 *  @return 	char * �����ַ���
 */
	char * GetTableName();

/*
 *  @brief   	GetFieldName	 ����ֶ������ַ��� 
 *  @param 		[In]a param of Type  UINT  �ֶ�λ�ã��ڼ��У�
 *  @return 	char * �ֶ������ַ���
 */
	char* GetFieldName(UINT nField);

/*
 *  @brief   	GetFieldType	 ����ֶ��������� 
 *  @param 		[In]a param of Type  UINT  �ֶ�λ�ã��ڼ��У�
 *  @return 	int 
 */
	int GetFieldType(UINT nField);

/*
 *  @brief   	GetValue	 �����ֶ�λ�û���ֶε�ֵ 
 *  @param 		[In]a param of Type  UINT  �ֶ�λ�ã��ڼ��У�
 *  @return 	char * �ֶε�ֵ��ʧ��ΪNULL
 */
	char * GetValue(UINT nField);

/*
 *  @brief   	GetValue	 �����ֶ�������ֶε�ֵ�����ִ�Сд�� 
 *  @param 		[In]a param of Type  const char*  �ֶ�Ӣ����
 *  @return 	char * �ֶε�ֵ��ʧ��ΪNULL
 */
	char * GetValue(const char* pFieldname);

/*
 *  @brief   	GetRealDataValue	 �����ֶ�������ֶε�ֵ������Сд�� 
 *  @param 		[In]a param of Type  const char*  �ֶ�Ӣ����
 *  @return 	char * �ֶε�ֵ��ʧ��ΪNULL
 */
	char * GetRealDataValue(const char* pLowerFieldname);

/*
 *  @brief   	GetRealValue	 �����ֶ�����ȡĳһ�е�ֵ������Сд��
 *  @param 		[In]a param of Type  const char*  �ֶ�Ӣ����
 *  @param 		[In]a param of Type  UINT  �к�
 *  @return 	char * �ֶε�ֵ��ʧ��ΪNULL
 */
	char * GetRealValue(const char* pFieldname, UINT nRow);

/*
 *  @brief   	GetRealNumValue	 �����кŻ�ȡĳһ�е�ֵ 
 *  @param 		[In]a param of Type  UINT  �к�
 *  @param 		[In]a param of Type  UINT  �к�
 *  @return 	char * �ֶε�ֵ��ʧ��ΪNULL
 */
	char * GetRealNumValue(UINT nRow, UINT nField);

/*
 *  @brief   	MoveFirst	 �ƶ�����һ�� 
 *  @return 	int 0 �ɹ� -1 ʧ��
 */
	int	MoveFirst();

/*
 *  @brief   	MoveNext	 �ƶ�����һ�� 
 *  @return 	int 
 */
	int MoveNext();

/*
 *  @brief   	MoveForward	 ��ǰ�ƶ�һ�� 
 *  @return 	int 0
 */
	int MoveForward();

/*
 *  @brief   	MoveNextTest	 �����ƶ�һ��,��ס��ǰλ�� 
 *  @return 	int 
 */
	int MoveNextTest();
	
/*
 *  @brief   	MoveToUserRow	 �ƶ����û�ָ������ 
 *  @param 		[In]a param of Type  int  ָ������
 *  @return 	int 
 */
	int MoveToUserRow(int nRownum);

public:
/*
 *  @brief   	GetFieldLength	 ��ȡ�ֶγ��ȣ������� 
 *  @param 		[In]a param of Type  UINT  �к�
 *  @return 	int 
 */
	int GetFieldLength(UINT nField);

/*
 *  @brief   	FreeData	 �ͷ��ڴ� 
 *  @param 		[In]a param of Type  bool  �Ƿ��ͷ��ֶζ���
 *  @return 	int 
 */
	int FreeData(bool bFreeAll);

public:
/*
 *  @brief   	FreeFieldlistonly	 ��ɾ���ֶ���Ϣ�б� 
 *  @return 	int 
 */
	int FreeFieldlistonly();

public:
/*
 *  @brief   	GetRow	 ���к�ȡ��������ָ�� 
 *  @param 		[In]a param of Type  int  �к�
 *  @return 	SQL_ROW ������ָ��
 */
	SQL_ROW GetRow(int nposition);

/*
 *  @brief   	GetCurRow	 �õ���ǰ������ָ�� 
 *  @return 	SQL_ROW ��ǰ������ָ��
 */
	SQL_ROW GetCurRow();

/*
 *  @brief   	SetLifeTime	 �������ݼ��������� 
 *  @param 		[In]a param of Type  UINT  ���ݼ���������
 *  @return 	int 
 */
	int SetLifeTime(UINT nLiftTime);

/*
 *  @brief   	GetLifeTime	 �õ���ǰ������ʱ�� 
 *  @return 	UINT ��ǰ������ʱ��
 */
    UINT GetLifeTime();

public:
/*
 *  @brief   	AddRow	 ���һ������ 
 *  @param 		[In]a param of Type  SQL_ROW  ������
 *  @return 	int 
 */
	int AddRow(SQL_ROW row);

/*
 *  @brief   	AddRows	 ��Ӷ������� 
 *  @param 		[In]a param of Type  CMemSet*  �����б�
 *  @return 	int 0
 */
	int AddRows(CMemSet* pMemSet);

/*
 *  @brief   	AtatchField	 �����ֶ����� 
 *  @param 		[In]a param of Type  CMemField*  �ֶ��б�
 *  @param 		[In]a param of Type  UINT  �ֶ���
 *  @return 	int 0���ɹ� -1��ʧ��
 */
	int AtatchField(CMemField* pMemFildList, UINT nFieldNum);

/*
 *  @brief   	AtatchRow	 ���������� 
 *  @param 		[In]a param of Type  CMemRow*  ������ָ��
 *  @param 		[In]a param of Type  UINT  ��������
 *  @return 	int 0���ɹ� -1��ʧ��
 */
	int AtatchRow(CMemRow* pMemRowList, UINT nRowNum);

/*
 *  @brief   	GetRowLength	 �������ݵ��ܳ��� 
 *  @param 		[In]a param of Type  int  ����
 *  @return 	int �ܳ���
 */
	int GetRowLength(int nRowNum);

private:
/*
 *  @brief   	ColoneRow	 ���������� 
 *  @param 		[In]a param of Type  SQL_ROW  ��������������
 *  @return 	SQL_ROW 
 */
	SQL_ROW ColoneRow(SQL_ROW pSourceRow);
	
/*
 *  @brief   	StringToLower	 �ַ�����С�� 
 *  @param 		[In]a param of Type  char*  ��������ַ���
 *  @return 	string ��С������ַ���
 */
	string StringToLower(char* p_val);

private:
/**	\brief ���ݼ���*/
	vector<SQL_ROW> m_listData;

/**	\brief ��ǰ��ָ��*/
	SQL_ROW m_CurmemRow;

/**	\brief �ֶθ���*/
	UINT m_nFieldNum;

/**	\brief ��ǰ�к�*/
	UINT m_nCurRow;

/**	\brief ������*/
	UINT m_nRowCount;

/**	\brief �ֶ��б�*/
	CMemField* m_pFiledList;

/**	\brief ������������*/
	UINT m_nMaxRow;

/**	\brief ����*/
	char m_chTablename[200];

/**	\brief ����ʱ�䣨δʹ�ã�*/
	UINT m_nExistTime;

/**	\brief ���б�*/
	CMemRow* m_pRowList;

/**	\brief ����Ŀ*/
	UINT m_nRowNum;

/**	\brief ������  0: ���� 1���޸� 2��ɾ������δ��*/
	int m_nDirtyType;

/**	\brief ��*/
	CXJLock m_lockMemsetAccess;
};

#endif
