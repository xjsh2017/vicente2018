// DBEngineCP.cpp: implementation of the DBEngineCP class.
//
//////////////////////////////////////////////////////////////////////

#include "DBFacadeEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBFacadeEngine::DBFacadeEngine()
{
	m_hModule = NULL;
	m_pSelectData = NULL;//��ѯ����
	m_pUpdateData = NULL;	//��������
	m_pInsertData = NULL;	//д����
	m_pDeleteData = NULL;	//ɾ������
	m_pExecuteSql = NULL; //ֱ��ִ��SQL������
	m_pInsertMultiData = NULL; //����д����
	m_pConnect = NULL; //�������ݿ�ӿ�
	m_pSetLogPath = NULL; //������־·���ӿ�
	m_pCloseSession = NULL; //�ر�����
	m_pSelectRealData = NULL;//��ѯʵʱ����

	xjdbf_LoadDBFLib();
}

DBFacadeEngine::~DBFacadeEngine()
{
	if(m_hModule != NULL)
	{
		xj_free_library(m_hModule);
		m_pSelectData = NULL;//��ѯ����
		m_pUpdateData = NULL;	//��������
		m_pInsertData = NULL;	//д����
		m_pDeleteData = NULL;	//ɾ������
		m_pExecuteSql = NULL; //ֱ��ִ��SQL������
		m_pInsertMultiData = NULL; //����д����
		m_pConnect = NULL; //�������ݿ�ӿ�
		m_pSetLogPath = NULL; //������־·���ӿ�
		m_pCloseSession = NULL; //�ر�����
		m_pSelectRealData = NULL;//��ѯʵʱ����
	}
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_LoadDBFLib	
����ֵ:   bool	װ�سɹ�����true,ʧ�ܷ���false
���ܸ�Ҫ: װ��XJDBFacade.dll
*****************************************************/
bool DBFacadeEngine::xjdbf_LoadDBFLib()
{
	m_hModule = xj_load_library(LIBNAME_DBFACADE);

	if(m_hModule == NULL)
		return false;

	m_pSelectData = (int(*)(int,SQL_DATA&, char*, CMemSet*))xj_get_addr(m_hModule, "SelectData");//��ѯ����
	m_pUpdateData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "UpdateData");	//��������
	m_pInsertData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "InsertData");	//д����
	m_pDeleteData = (int (*)(int, SQL_DATA&, char*))xj_get_addr(m_hModule, "DeleteData");	//ɾ������
	m_pExecuteSql = (int(*)(MutiSQL_DATA&, char*, CMemSet*))xj_get_addr(m_hModule, "ExecuteSql"); //ֱ��ִ��SQL������
	m_pInsertMultiData = (int (*)(int, STTP_FULL_DATA&, char*))xj_get_addr(m_hModule, "InsertMutiData"); //����д����
	m_pConnect = (int(*)(int, char*))xj_get_addr(m_hModule, "Connect"); //�������ݿ�ӿ�
	m_pSetLogPath = (int(*)(const char*, char*))xj_get_addr(m_hModule, "SetLogPath"); //������־·���ӿ�
	m_pCloseSession = (int(*)(char*))xj_get_addr(m_hModule, "CloseSession"); //�ر�����
	m_pSelectRealData = (int(*)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *))xj_get_addr(m_hModule, "RealDataSelect");//��ѯʵʱ����

	return true;
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_SelectData	
����ֵ:   int	�ɹ�����1,ʧ�ܷ���0
���ܸ�Ҫ: ��ѯ�ӿ�
����: int TableCode	���ݱ����
����: SQL_DATA & sql ��ѯ�����ṹ	
����: char * sError	�������
����: CMemSet * pMem ���ݼ�	
*****************************************************/
int DBFacadeEngine::xjdbf_SelectData( int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSelectData == NULL)
		return 0;
	return m_pSelectData(TableCode, sql, sError, pMem);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_UpdateData	
����ֵ:   int	�ɹ�����1,ʧ�ܷ���0
���ܸ�Ҫ: ���½ӿ�
����: int TableCode	���ݱ����
����: SQL_DATA & sql ��ѯ�����ṹ	
����: char * sError	�������
*****************************************************/
int DBFacadeEngine::xjdbf_UpdateData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pUpdateData == NULL)
		return 0;
	return m_pUpdateData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_InsertData	
����ֵ:   int �ɹ�����1,ʧ�ܷ���0	
���ܸ�Ҫ: д���ݽӿ�
����: int TableCode	���ݱ����
����: SQL_DATA & sql ��ѯ�����ṹ	
����: char * sError	�������
*****************************************************/
int DBFacadeEngine::xjdbf_InsertData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pInsertData == NULL)
		return 0;
	return m_pInsertData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_InsertMutiData	
����ֵ:   int �ɹ�����1, ʧ�ܷ���0	
���ܸ�Ҫ: ����д����
����: int TableName	���ݱ����
����: STTP_FULL_DATA & pRealData ��ѯ�����ṹ	
����: char * Msg �������	
*****************************************************/
int DBFacadeEngine::xjdbf_InsertMutiData( int TableName ,STTP_FULL_DATA& pRealData ,char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pInsertMultiData == NULL)
		return 0;
	return m_pInsertMultiData(TableName, pRealData, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_DeleteData	
����ֵ:   int �ɹ�����1,ʧ�ܷ���0	
���ܸ�Ҫ: ɾ�����ݽӿ�
����: int TableCode	���ݱ����
����: SQL_DATA & sql ��ѯ�����ṹ	
����: char * sError	�������
*****************************************************/
int DBFacadeEngine::xjdbf_DeleteData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pDeleteData == NULL)
		return 0;
	return m_pDeleteData(TableCode, sql, sError);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_Connect	
����ֵ:   int	0-ʧ��, 1-�ɹ�
���ܸ�Ҫ: �������ݿ�
����: int pName	ģ����
����: char * Msg ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)	
*****************************************************/
int DBFacadeEngine::xjdbf_Connect( int pName, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pConnect == NULL)
		return 0;
	return m_pConnect(pName, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_SetLogPath	
����ֵ:   int	
���ܸ�Ҫ: ������־·��
����: const char * LogPath ��־·��(ȫ·��:ͳһ·��/ģ����)	
����: char * Msg ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)	
*****************************************************/
int DBFacadeEngine::xjdbf_SetLogPath( const char* LogPath, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSetLogPath == NULL)
		return 0;
	return m_pSetLogPath(LogPath, Msg);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_CloseSession	
����ֵ:   int �ɹ�����1,ʧ�ܷ���0	
���ܸ�Ҫ: �ر�����
����: CString & sLog ������Ϣ	
*****************************************************/
int DBFacadeEngine::xjdbf_CloseSession( char* sLog )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pCloseSession == NULL)
		return 0;
	return m_pCloseSession(sLog);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_ExecuteSql	
����ֵ:   int �ɹ�����1,ʧ�ܷ���0	
���ܸ�Ҫ: ֱ��ִ�б�׼��SQL���
����: MutiSQL_DATA & pMutiSQLDATA ����һ��SQL���Ľṹ��	
����: char * Msg ִ�в�ѯ�������ص�ִ����Ϣ	
����: CMemSet * p_rst ���صĲ�ѯ���ݼ�	
*****************************************************/
int DBFacadeEngine::xjdbf_ExecuteSql( MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pExecuteSql == NULL)
		return 0;
	return m_pExecuteSql(pMutiSQLDATA, Msg, p_rst);
}

/****************************************************
Date:2012/7/5  Author:LYH
������:   xjdbf_SelectRealData	
����ֵ:   int �ɹ�����1,ʧ�ܷ���0	
���ܸ�Ҫ: ��ʵʱ���ѯ����
����: int TableName	���ݱ���
����: SQL_DATA & pSQLDATA ����һ��SQL���Ľṹ��	
����: REALDATA_CONDITION & pData ʵʱ������λ����Ϣ	
����: char * Msg ִ�в�ѯ�������ص�ִ����Ϣ	
����: CMemSet * p_rst ���صĲ�ѯ���ݼ�	
*****************************************************/
int DBFacadeEngine::xjdbf_SelectRealData( int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pSelectRealData == NULL)
		return 0;
	return m_pSelectRealData(TableName, pSQLDATA, pData, Msg, p_rst);
}

/****************************************************
Date:2012/7/9  Author:LYH
������:   SetField	
����ֵ:   void	
���ܸ�Ҫ: ���ò�ѯ�ֶ�
����: SQL_DATA & sql ��ѯ�ṹ	
����: Field & field	�ֶνṹ
����: CXJString sName �ֶ�����	
����: int nType	�ֶ�����
����: CXJString sValue	�ֶ�ֵ
*****************************************************/
void DBFacadeEngine::SetField( SQL_DATA& sql, Field& field, CXJString sName, int nType, CXJString sValue /*= ""*/ )
{
	bzero(&field, sizeof(Field));
	CXJString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
		strncpy(field.FieldValue, sValue, sValue.GetLength());
	sql.Fieldlist.push_back(field);
}

void DBFacadeEngine::SetField(SQL_DATA& sql, CXJString sName, int nType, CXJString sValue /*= ""*/)
{
	Field field;
	bzero(&field, sizeof(Field));
	CXJString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
		strncpy(field.FieldValue, sValue, sValue.GetLength());
	sql.Fieldlist.push_back(field);
}

/****************************************************
Date:2012/7/9  Author:LYH
������:   SetCondition	
����ֵ:   void	
���ܸ�Ҫ: ���ò�ѯ����
����: SQL_DATA & sql ��ѯ�ṹ	
����: Condition & con �����ṹ	
����: CXJString sContent ��������	
����: int nType	��������
*****************************************************/
void DBFacadeEngine::SetCondition( SQL_DATA& sql, Condition& con, CXJString sContent, int nType /*= 0*/ )
{
	bzero(&con, sizeof(Condition));
	CXJString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}

void DBFacadeEngine::SetCondition(SQL_DATA& sql, CXJString sContent, int nType /*= 0*/)
{
	Condition con;
	bzero(&con, sizeof(Condition));
	CXJString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}