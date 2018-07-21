// DBEngineCP.h: interface for the DBEngineCP class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(DBFACADEENGINE_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_)
#define DBFACADEENGINE_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../common/define.h"
#include "../../common/xjlib.h"
#include "../../common/MemSet.h"
#include "../../common/SttpDefine.h"
#include "../../common/XJString.h"

#ifdef OS_WINDOWS
#define LIBNAME_DBFACADE "XJDBFacade.dll"
#else 
#define LIBNAME_DBFACADE "libXJDBFacade.so"
#endif

//��������DBFacade����дdbf��ͷ
typedef int (*dbf_SelectData)(int, SQL_DATA&, char*, CMemSet*); //��ѯ����
typedef int (*dbf_UpdateData)(int, SQL_DATA&, char*);	//��������
typedef int (*dbf_InsertData)(int, SQL_DATA&, char*);	//д����
typedef int (*dbf_DeleteData)(int, SQL_DATA&, char*);	//ɾ������
typedef int (*dbf_InsertMultiData)(int, STTP_FULL_DATA&, char*); //����д����
typedef int	(*dbf_Connect)(int, char*); //�������ݿ�ӿ�
typedef int (*dbf_SetLogPath)(const char*, char*); //������־·���ӿ�
typedef int (*dbf_CloseSession)(char*); //�ر�����
typedef int (*dbf_ExecuteSql)(MutiSQL_DATA&, char*, CMemSet*); //ֱ��ִ��SQL������
typedef int (*dbf_SelectRealData)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *);//��ѯʵʱ����

class DBFacadeEngine  
{
public:
	DBFacadeEngine();
	virtual ~DBFacadeEngine();

private:
	/** @brief           ��̬����*/
	XJHANDLE	m_hModule;

private:
	dbf_SelectData m_pSelectData;//��ѯ����
	dbf_UpdateData m_pUpdateData;	//��������
	dbf_InsertData m_pInsertData;	//д����
	dbf_DeleteData m_pDeleteData;	//ɾ������
	dbf_ExecuteSql m_pExecuteSql; //ֱ��ִ��SQL������
	dbf_InsertMultiData m_pInsertMultiData; //����д����
	dbf_Connect m_pConnect; //�������ݿ�ӿ�
	dbf_SetLogPath m_pSetLogPath; //������־·���ӿ�
	dbf_CloseSession m_pCloseSession; //�ر�����
	dbf_SelectRealData m_pSelectRealData;//��ѯʵʱ����

private:
	/*
	* @brief	xjdbf_LoadDBFLib	װ��XJDBFacade.dll
	* @return   bool	װ�سɹ�����true,ʧ�ܷ���false
	*/
	bool xjdbf_LoadDBFLib();
public:
	/*
 *  @brief   	XJSelectData	 ��ѯ�ӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @param 		[Out]a param of Type  CMemSet*  ���ݼ�
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803BD
	int	xjdbf_SelectData(int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem);

/*
 *  @brief   	XJUpdateData	 ���½ӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803C2
	int xjdbf_UpdateData(int TableCode, SQL_DATA& sql, char* sError);


/*
 *  @brief   	XJInsertData	 д���ݽӿ�
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803CA
	int xjdbf_InsertData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertMutiData	 ����д���� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  ��ѯ�����ṹ
 *  @param 		[In]a param of Type  char*  �������
 *  @return 	int �ɹ�����1, ʧ�ܷ���0
 */
	//##ModelId=49B87BA803CE
	int xjdbf_InsertMutiData(int TableName ,STTP_FULL_DATA& pRealData ,char* Msg);

/*
 *  @brief   	XJDeleteData	 ɾ�����ݽӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803D2
	int xjdbf_DeleteData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJConnect	 �������ݿ�
 *  @param 		[In]a param of Type  int  ģ����
 *  @param 		[In]a param of Type  char*  ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
 *  @return 	int 0-ʧ��, 1-�ɹ�
 */
	//##ModelId=49B87BA803E2
	int xjdbf_Connect(int pName, char* Msg);

/*
 *  @brief   	XJSetLogPath	 ������־·�� 
 *  @param 		[In]a param of Type  const char*  ��־·��(ȫ·��:ͳһ·��/ģ����)
 *  @param 		[In]a param of Type  char*  ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
 *  @return 	int 0-ʧ��, 1-�ɹ�
 */
	//##ModelId=49B87BA803E5
	int xjdbf_SetLogPath(const char* LogPath, char* Msg);

/*
 *  @brief   	XJCloseSession	 �ر����� 
 *  @param 		[Out]a param of Type  CString&  ������Ϣ
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	int xjdbf_CloseSession(char* sLog);

	/*
	*  @brief   	XJExecuteSql	 ֱ��ִ�б�׼��SQL��� 
	*  @param 		[In]a param of Type  MutiSQL_DATA&  ����һ��SQL���Ľṹ��
	*  @param 		[Out]a param of Type  char*  ִ�в�ѯ�������ص�ִ����Ϣ
	*  @param 		[Out]a param of Type  CMemSet *  ���صĲ�ѯ���ݼ�
	*  @return 	int �ɹ�����1,ʧ�ܷ���0
	*/
	int xjdbf_ExecuteSql(MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst);
	
	/*
	* @brief	XJSelectRealData	��ʵʱ���ѯ����	
	* @param	[In]a param of Type int TableName ���ݱ���		
	* @param	[In]a param of Type SQL_DATA & pSQLDATA	����һ��SQL���Ľṹ��	
	* @param	[In]a param of Type REALDATA_CONDITION & pData ʵʱ������λ����Ϣ		
	* @param	[In]a param of Type char * Msg	ִ�в�ѯ�������ص�ִ����Ϣ	
	* @param	[In]a param of Type CMemSet * p_rst	���صĲ�ѯ���ݼ�	
	* @return   int	�ɹ�����1,ʧ�ܷ���0
	*/
	int xjdbf_SelectRealData(int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg,  CMemSet * p_rst);

	/*
 *  @brief   	SetField	 ���ò�ѯ�ֶ� 
 *  @param 		[Out]a param of Type  SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  Field&  �ֶνṹ
 *  @param 		[In]a param of Type  CXJString  �ֶ�����
 *  @param 		[In]a param of Type  int  �ֶ�����
 *  @param		[In]a param of Type  CXJString �ֶ�ֵ
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetField(SQL_DATA& sql, Field& field, CXJString sName, int nType, CXJString sValue = "");

	void SetField(SQL_DATA& sql, CXJString sName, int nType, CXJString sValue = "");


/*
 *  @brief   	SetCondition	 ���ò�ѯ���� 
 *  @param 		[Out]a param of Type  SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  Condition&  �����ṹ
 *  @param 		[In]a param of Type  CXJString  ��������
 *  @param 		[In]a param of Type  int  ��������
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetCondition(SQL_DATA& sql, Condition& con, CXJString sContent, int nType = 0);

	void SetCondition(SQL_DATA& sql, CXJString sContent, int nType = 0);

};

#endif // !defined(AFX_DBENGINECP_H__EA0F4F60_E3B6_4025_96F8_B879134E76A1__INCLUDED_)
