// DBEngine.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "DBEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int (*SelectData)(int, SQL_DATA&, char*, CMemSet*); //��ѯ����
int (*UpdateData)(int, SQL_DATA&, char*);	//��������
int (*UpdateBlogData)(int, BLOB_SQL_DATA&, char*);	//��������
int (*InsertData)(int, SQL_DATA&, char*);	//д����
int (*InsertBlobData)(int, BLOB_SQL_DATA&, char*);
int (*DeleteData)(int, SQL_DATA&, char*);	//ɾ������
int (*InsertMutiData)(int, STTP_FULL_DATA&, char*); //����д����
int	(*Connect)(int, char*); //�������ݿ�ӿ�
int (*SetLogPath)(const char*, char*); //������־·���ӿ�
int (*CloseSession)(char*); 
int (*ExecuteSql)(MutiSQL_DATA&, char*, CMemSet*); //ֱ��ִ��SQL������
int (*SelectRealData)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *);

/////////////////////////////////////////////////////////////////////////////
// CDBEngine

//##ModelId=49B87BA803BA
CDBEngine::CDBEngine()
{
	LoadDBLib();
}

//##ModelId=49B87BA803BB
CDBEngine::~CDBEngine()
{
	FreeLibrary(m_hModule);
	m_hModule = NULL;
}

//##ModelId=49B87BA90002
BOOL CDBEngine::LoadDBLib()
{
	m_hModule = LoadLibrary("XJDBFacade.dll");
	if(m_hModule != NULL)
	{
		SelectData = (int(*)(int,SQL_DATA&, char*, CMemSet*))GetProcAddress(m_hModule, "SelectData"); //��ѯ����
		UpdateData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "UpdateData");	//��������
		UpdateBlogData = (int (*)(int, BLOB_SQL_DATA&, char*))GetProcAddress(m_hModule, "BlobUpdateData");	//��������
		InsertData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "InsertData");	//д����
		DeleteData = (int (*)(int, SQL_DATA&, char*))GetProcAddress(m_hModule, "DeleteData");	//ɾ������
		InsertMutiData = (int (*)(int, STTP_FULL_DATA&, char*))GetProcAddress(m_hModule, "InsertMutiData"); //����д����
		Connect = (int(*)(int, char*))GetProcAddress(m_hModule, "Connect"); //�������ݿ�
		SetLogPath = (int(*)(const char*, char*))GetProcAddress(m_hModule, "SetLogPath"); //������־·��
		CloseSession = (int(*)(char*))GetProcAddress(m_hModule, "CloseSession");
		ExecuteSql = (int(*)(MutiSQL_DATA&, char*, CMemSet*))GetProcAddress(m_hModule, "ExecuteSql"); //ֱ��ִ��SQL������
		SelectRealData = (int(*)(int, SQL_DATA&, REALDATA_CONDITION&, char*,  CMemSet *))GetProcAddress(m_hModule, "RealDataSelect");//ʵʱ���ѯ
		InsertBlobData = (int (*)(int, BLOB_SQL_DATA&, char*))GetProcAddress(m_hModule, "BlobInsertData");
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����XJSelectData()
 ���ܸ�Ҫ����������ƽ̨DLL��ӿ�ִ�в�ѯ
 �� �� ֵ: �ɹ�����1,ʧ�ܷ���0
 ��    ����param1   �����
           Param2   ��ѯ�����ṹ��
		   Param3	������Ϣ
		   Param4	���صĲ�ѯ������ݼ�
**************************************************************/
//##ModelId=49B87BA803BD
int CDBEngine::XJSelectData( int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem )
{
	if(m_hModule == NULL)
		return 0;
	if(SelectData == NULL)
		return 0;
	return SelectData(TableCode, sql, sError, pMem);
}

/*************************************************************
 �� �� ����SetField()
 ���ܸ�Ҫ�����ò�ѯ�ֶ�
 �� �� ֵ: void
 ��    ����param1   ��ѯ�ṹ
           Param2   �ֶνṹ
		   Param3	�ֶ�����
		   Param4	�ֶ�����
**************************************************************/
//##ModelId=49B87BA803D6
void CDBEngine::SetField(SQL_DATA& sql, Field& field, CString sName, int nType, CString sValue)
{
	bzero(&field, sizeof(Field));
	CString str = sName;
	str.MakeUpper();
	strncpy(field.FieldName, str, str.GetLength());
	field.FieldType = nType;
	if(sValue != "")
	{
		int vlen = sValue.GetLength();

		strncpy(field.FieldValue, sValue, vlen>sizeof(field.FieldValue)?sizeof(field.FieldValue):vlen);
	}
	sql.Fieldlist.push_back(field);
}

/*************************************************************
 �� �� ����SetField()
 ���ܸ�Ҫ�����ò�ѯ�ֶ�
 �� �� ֵ: void
 ��    ����param1   ��ѯ�ṹ
           Param2   �ֶνṹ
		   Param3	�ֶ�����
		   Param4	�ֶ�����
**************************************************************/
//##ModelId=49B87BA803D6
void CDBEngine::SetBlobField(BLOB_SQL_DATA& sql, BLOB_FIELD& field, CString sName, int nType, CString sValue)
{
	memset(&field, 0, sizeof(field));
	CString str = sName;
	str.MakeUpper();
    field.strBlobFieldName = str.GetBuffer(0);
	field.nBlobFieldType = nType;
	if(sValue != "")
	{
		field.strBlobFieldValue = sValue.GetBuffer(0);
	}
	sql.BlobFieldlist.push_back(field);
}



/*************************************************************
 �� �� ����SetCondition()
 ���ܸ�Ҫ�����ò�ѯ����
 �� �� ֵ: void
 ��    ����param1   ��ѯ�ṹ
           Param2   �����ṹ
		   Param3	��������
		   Param4	��������
**************************************************************/
//##ModelId=49B87BA803DD
void CDBEngine::SetCondition(SQL_DATA& sql, Condition& con, CString sContent, int nType/* = 0*/)
{
	bzero(&con, sizeof(Condition));
	CString str = sContent;
	strncpy(con.ConditionContent, str, str.GetLength());
	con.ConditionType = nType;
	sql.Conditionlist.push_back(con);
}

/*************************************************************
 �� �� ����SetCondition()
 ���ܸ�Ҫ�����ò�ѯ����
 �� �� ֵ: void
 ��    ����param1   ��ѯ�ṹ
           Param2   �����ṹ
		   Param3	��������
		   Param4	��������
**************************************************************/
//##ModelId=49B87BA803DD
void CDBEngine::SetBlobCondition(BLOB_SQL_DATA& sql, BLOB_CONDITION& con, CString sContent, int nType/* = 0*/)
{
	//bzero(&con, sizeof(Condition));
	CString str = sContent;
	con.strBlobConditionContent = str.GetBuffer(0);
	con.nBlobConditionType = nType;
	sql.Blob_Conditionlist.push_back(con);
}

/*************************************************************
 �� �� ����XJUpdateData()
 ���ܸ�Ҫ��ͨ�����ݿ�ӿڿ��������
 �� �� ֵ: �ɹ�����1,ʧ�ܷ���0
 ��    ����param1	���ݱ����
		   Param2	���ݲ�ѯ�ṹ
		   Param3	�������
**************************************************************/
//##ModelId=49B87BA803C2
int CDBEngine::XJUpdateData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(UpdateData == NULL)
		return 0;
	return UpdateData(TableCode, sql, sError);
}

/*************************************************************
 �� �� ����XJUpdateBlogData()
 ���ܸ�Ҫ��ͨ�����ݿ�ӿڿ��������
 �� �� ֵ: �ɹ�����1,ʧ�ܷ���0
 ��    ����param1	���ݱ����
		   Param2	���ݲ�ѯ�ṹ
		   Param3	�������
**************************************************************/
//##ModelId=49B87BA803C2
int CDBEngine::XJUpdateBlogData( int TableCode, BLOB_SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(UpdateBlogData == NULL)
		return 0;
	return UpdateBlogData(TableCode, sql, sError);
}

/*************************************************************
 �� �� ����XJInsertData()
 ���ܸ�Ҫ��ͨ�����ݿ�ӿڿ�д����
 �� �� ֵ: �ɹ�����1,ʧ�ܷ���0
 ��    ����param1	���ݱ����
		   Param2	���ݲ�ѯ�ṹ
		   param3	�������
**************************************************************/
//##ModelId=49B87BA803CA
int CDBEngine::XJInsertData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(InsertData == NULL)
		return 0;
	return InsertData(TableCode, sql, sError);
}

/*************************************************************
 �� �� ����XJDeleteData()
 ���ܸ�Ҫ��ͨ�����ݿ�ӿڿ�ɾ������
 �� �� ֵ: �ɹ�����1,ʧ�ܷ���0
 ��    ����param1	���ݱ����
		   Param2	���ݲ�ѯ�ṹ
		   Param3	�������
**************************************************************/
//##ModelId=49B87BA803D2
int CDBEngine::XJDeleteData( int TableCode, SQL_DATA& sql, char* sError )
{
	if(m_hModule == NULL)
		return 0;
	if(DeleteData == NULL)
		return 0;
	return DeleteData(TableCode, sql, sError);
}

/*************************************************************
 �� �� ����XJInsertMutiData()
 ���ܸ�Ҫ������д����
 �� �� ֵ: �ɹ�����1, ʧ�ܷ���0
 ��    ����param1	���ݱ����
		   Param2	��ѯ�����ṹ
		   Param3	�������
**************************************************************/
//##ModelId=49B87BA803CE
int CDBEngine::XJInsertMutiData( int TableName ,STTP_FULL_DATA& pRealData ,char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(InsertMutiData == NULL)
		return 0;
	return InsertMutiData(TableName, pRealData, Msg);
}

/*************************************************************
 �� �� ����XJConnect()
 ���ܸ�Ҫ���������ݿ�
 �� �� ֵ: 0-ʧ��, 1-�ɹ�
 ��    ����param1	ģ����
		   Param2	ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
**************************************************************/
//##ModelId=49B87BA803E2
int CDBEngine::XJConnect( int pName, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(Connect == NULL)
		return 0;
	return Connect(pName, Msg);
}

/*************************************************************
 �� �� ����XJSetLogPath()
 ���ܸ�Ҫ��������־·��
 �� �� ֵ: 0-ʧ��, 1-�ɹ�
 ��    ����param1	��־·��(ȫ·��:ͳһ·��/ģ����)
		   Param2	ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
**************************************************************/
//##ModelId=49B87BA803E5
int CDBEngine::XJSetLogPath( const char* LogPath, char* Msg )
{
	if(m_hModule == NULL)
		return 0;
	if(SetLogPath == NULL)
		return 0;
	return SetLogPath(LogPath, Msg);
}

/*************************************************************
 �� �� ��: XJCloseSession()
 ���ܸ�Ҫ: 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
int CDBEngine::XJCloseSession( CString& sLog )
{
	if(m_hModule == NULL)
		return 0;
	if(CloseSession == NULL)
		return 0;
	char* pStr = sLog.GetBuffer(0);
	return CloseSession(pStr);
}

/*************************************************************
�� �� ����XJExecuteSql()
���ܸ�Ҫ��ֱ��ִ�б�׼��SQL���
�� �� ֵ: �ɹ�����1,ʧ�ܷ���0
��    ����param1	����һ��SQL���Ľṹ��
Param2	ִ�в�ѯ�������ص�ִ����Ϣ
Param3	���صĲ�ѯ���ݼ�
**************************************************************/
int CDBEngine::XJExecuteSql( MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(ExecuteSql == NULL)
		return 0;
	return ExecuteSql(pMutiSQLDATA, Msg, p_rst);
}

/*************************************************************
CreateDate: 30:12:2009     Author:LYH
�� �� ��: FreeDBLib()
���ܸ�Ҫ: ж��XJDBFacade.dll 
�� �� ֵ: 
��    ��: param1 
Param2 
**************************************************************/
void CDBEngine::FreeDBLib()
{
	if(m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

int CDBEngine::XJSelectRealData( int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg, CMemSet * p_rst )
{
	if(m_hModule == NULL)
		return 0;
	if(SelectRealData == NULL)
		return 0;
	return SelectRealData(TableName, pSQLDATA, pData, Msg, p_rst);
}

int CDBEngine::XJInsertBlobData(int TableCode, BLOB_SQL_DATA& sql, char* sError)
{
	if(!m_hModule)
		return 0;
	if(!InsertBlobData)
		return 0;
	return InsertBlobData(TableCode, sql, sError);
}
