#if !defined(AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_)
#define AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBEngine.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDBEngine document

/**
 * @defgroup 	CDBEngine	���ݿ�����
 * @{
 */
/**
 * @brief       �������ݿ����DLL�ӿ�����
*  @author      LYH
 * @version     ver1.0
 * @date        27/09/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA803AA
class CDBEngine : public CDocument
{
public:
	//##ModelId=49B87BA803BA
	CDBEngine();           // protected constructor used by dynamic creation
	//##ModelId=49B87BA803BB
	~CDBEngine();
// Attributes
public:
	/** @brief           ��̬����*/
	//##ModelId=49B87BA803BC
	HMODULE	m_hModule;

// Operations
public:

  int XJSelectBlogData( int TableCode, BLOB_SQL_DATA& sql, char* sError, CMemSet* pMem );

  int XJSelectRealData(int TableName , SQL_DATA& pSQLDATA , REALDATA_CONDITION& pData, char* Msg,  CMemSet * p_rst);
/*
 *  @brief   	XJSelectData	 ��ѯ�ӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @param 		[Out]a param of Type  CMemSet*  ���ݼ�
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803BD
	int	XJSelectData(int TableCode, SQL_DATA& sql, char* sError, CMemSet* pMem);

// 	int (*UpdateData)(int, SQL_DATA&, char*);	//��������
// 	int (*InsertData)(int, SQL_DATA&, char*);	//д����
// 	int (*DeleteData)(int, SQL_DATA&, char*);	//ɾ������

/*
 *  @brief   	XJUpdateData	 ���½ӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803C2
	int XJUpdateData(int TableCode, SQL_DATA& sql, char* sError);


/*
 *  @brief   	XJUpdateBlogData	 ���½ӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803C2
	int XJUpdateBlogData(int TableCode, BLOB_SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertData	 д���ݽӿ�
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803CA
	int XJInsertData(int TableCode, SQL_DATA& sql, char* sError);

	int XJInsertBlobData(int TableCode, BLOB_SQL_DATA& sql, char* sError);

/*
 *  @brief   	XJInsertMutiData	 ����д���� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  ��ѯ�����ṹ
 *  @param 		[In]a param of Type  char*  �������
 *  @return 	int �ɹ�����1, ʧ�ܷ���0
 */
	//##ModelId=49B87BA803CE
	int XJInsertMutiData(int TableName ,STTP_FULL_DATA& pRealData ,char* Msg);

/*
 *  @brief   	XJDeleteData	 ɾ�����ݽӿ� 
 *  @param 		[In]a param of Type  int  ���ݱ����
 *  @param 		[In]a param of Type  SQL_DATA&  ��ѯ�����ṹ
 *  @param 		[Out]a param of Type  char*  �������
 *  @return 	int �ɹ�����1,ʧ�ܷ���0
 */
	//##ModelId=49B87BA803D2
	int XJDeleteData(int TableCode, SQL_DATA& sql, char* sError);

/*
 *  @brief   	SetField	 ���ò�ѯ�ֶ� 
 *  @param 		[Out]a param of Type  SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  Field&  �ֶνṹ
 *  @param 		[In]a param of Type  CString  �ֶ�����
 *  @param 		[In]a param of Type  int  �ֶ�����
 *  @param		[In]a param of Type  CString �ֶ�ֵ
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetField(SQL_DATA& sql, Field& field, CString sName, int nType, CString sValue = "");


/*
 *  @brief   	SetField	 ���ò�ѯ�ֶ� 
 *  @param 		[Out]a param of Type  BLOB_SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  Field&  �ֶνṹ
 *  @param 		[In]a param of Type  CString  �ֶ�����
 *  @param 		[In]a param of Type  int  �ֶ�����
 *  @param		[In]a param of Type  CString �ֶ�ֵ
 *  @return 	void 
 */
	//##ModelId=49B87BA803D6
	void SetBlobField(BLOB_SQL_DATA& sql, BLOB_FIELD& field, CString sName, int nType, CString sValue = "");


/*
 *  @brief   	SetCondition	 ���ò�ѯ���� 
 *  @param 		[Out]a param of Type  SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  Condition&  �����ṹ
 *  @param 		[In]a param of Type  CString  ��������
 *  @param 		[In]a param of Type  int  ��������
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetCondition(SQL_DATA& sql, Condition& con, CString sContent, int nType = 0);
	
/*
 *  @brief   	SetCondition	 ���ò�ѯ���� 
 *  @param 		[Out]a param of Type  BLOB_SQL_DATA&  ��ѯ�ṹ
 *  @param 		[In]a param of Type  BLOB_Condition&  �����ṹ
 *  @param 		[In]a param of Type  CString  ��������
 *  @param 		[In]a param of Type  int  ��������
 *  @return 	void 
 */
	//##ModelId=49B87BA803DD
	void SetBlobCondition(BLOB_SQL_DATA& sql, BLOB_CONDITION& con, CString sContent, int nType = 0);


/*
 *  @brief   	XJConnect	 �������ݿ�
 *  @param 		[In]a param of Type  int  ģ����
 *  @param 		[In]a param of Type  char*  ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
 *  @return 	int 0-ʧ��, 1-�ɹ�
 */
	//##ModelId=49B87BA803E2
	int XJConnect(int pName, char* Msg);

/*
 *  @brief   	XJSetLogPath	 ������־·�� 
 *  @param 		[In]a param of Type  const char*  ��־·��(ȫ·��:ͳһ·��/ģ����)
 *  @param 		[In]a param of Type  char*  ִ�иýӿڷ��ص���Ϣ(һ��Ϊ������Ϣ)
 *  @return 	int 0-ʧ��, 1-�ɹ�
 */
	//##ModelId=49B87BA803E5
	int XJSetLogPath(const char* LogPath, char* Msg);

/*
 *  @brief   	XJCloseSession	 ��Ҫ�ĺ�������˵������ 
 *  @param 		[In/Out]a param of Type  CString&  �������˵��
 *  @return 	int 
 */
	int XJCloseSession(CString& sLog);

	/*
	*  @brief   	XJExecuteSql	 ֱ��ִ�б�׼��SQL��� 
	*  @param 		[In]a param of Type  MutiSQL_DATA&  ����һ��SQL���Ľṹ��
	*  @param 		[Out]a param of Type  char*  ִ�в�ѯ�������ص�ִ����Ϣ
	*  @param 		[Out]a param of Type  CMemSet *  ���صĲ�ѯ���ݼ�
	*  @return 	int �ɹ�����1,ʧ�ܷ���0
	*/
	int XJExecuteSql(MutiSQL_DATA& pMutiSQLDATA , char* Msg, CMemSet * p_rst);
public:
/*
 *  @brief   	LoadDBLib	 װ��XJDBFacade.dll 
 *  @return 	BOOL װ�سɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90002
	BOOL LoadDBLib();

	/*
	*  @brief   	FreeDBLib	 ж��XJDBFacade.dll 
	*  @return 	void 
	*/
	void FreeDBLib();

// Implementation
public:

	// Generated message map functions
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBENGINE_H__BEF12AD2_5DB9_46EF_9CE5_36C7A217BE47__INCLUDED_)
