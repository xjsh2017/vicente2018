#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "qbytearraymatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

/**
 * @defgroup 	CXJPTSetStore	��ֵ�޸��ڴ�����
 * @{
 */ 
/**
 * @brief       ��ֵ�޸�ʱ���õ������ݽṹ
 * @author      WPS
 * @version     ver1.0
 * @date        09/07/18

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

class QPTSetStateTable;

class QPTSetDataTable;
class QPTSetZoneDataTable;
class QPTSetSBDataTable;

class CXJPTSetStorePrivate;
class CXJPTSetStore : public CXJRootStore
{
private:
	static CXJPTSetStore*	m_pInstance;
	CXJPTSetStorePrivate*	d_ptr;

public:
	static CXJPTSetStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJPTSetStore();           // protected constructor used by dynamic creation
	~CXJPTSetStore();

// Attributes
public:
	/** @brief           ��̬����*/
	HMODULE	m_hModule;

public:
	/*
	 *  @brief   	ReLoadStore	 ��������״̬�� 
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL	ReLoadState();

	/** @brief           ״̬��*/
	QPTSetStateTable*	GetState();

	/** @brief           ��ֵ�޸�����*/
	QPTSetDataTable*	GetPTSetData();

// Operations
public:
	/*
	 *  @brief   	SaveStore	 ����״̬�� 
	 *  @param 		[In]a param of Type  CString  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  ������������
	 *  @return 	PTSETSTORE	
	 */
	BOOL	Save();
	BOOL	Save(const char *pszFilePath/* = NULL*/);
	
	/*
	 *  @brief   	CheckStore	 ���״̬���������
	 *  @param 		void
	 *  @return 	int		-1: ���ü��ʧ��;
							0: ���ô���;
							1: ���ò�����;
							10: ���ò����ڣ�����ʧ��;
							11: ���ò����ڣ������ɹ�	
	 */
	int		Check();
	
	BOOL	RevertModify();

	/*
	 *  @brief   	AddManOperator	 ����˹�������־ 
	 *  @param 		[In]a param of Type  CString  FunID  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  Act	 ������������
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID);

	CString		GetUserTypeName(CString sUserGroupID);
	CString		GetUserTypeName(int nUserType);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	CString		GetFuncID(int nStateID);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
