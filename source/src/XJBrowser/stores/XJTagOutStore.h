#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "qbytearraymatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CXJTagOutStore

/**
 * @defgroup 	CXJTagOutStore	���������ڴ�����
 * @{
 */ 
/**
 * @brief       ��������ʱ���õ������ݽṹ
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

class CXJTagOutStorePrivate;
class CXJTagOutStore : public CXJRootStore
{
private:
	static CXJTagOutStore*	m_pInstance;
	CXJTagOutStorePrivate*	d_ptr;

public:
	static CXJTagOutStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJTagOutStore();           // protected constructor used by dynamic creation
	~CXJTagOutStore();

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
	BOOL	SaveState(const char *pszFilePath = NULL);
	BOOL	SaveData(const char *pszFilePath = NULL);
	
	/*
	 *  @brief   	CheckStore	 ���״̬���������
	 *  @param 		void
	 *  @return 	BOOL		FALSE - ���ü��ʧ��;
							    TRUE  - ���ü��ɹ�
	 */
	BOOL	Check();
	
	BOOL	RevertModify();

	/*
	 *  @brief   	AddManOperator	 ����˹�������־ 
	 *  @param 		[In]a param of Type  CString  FunID  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  Act	 ������������
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID, int nOperType = OPER_SUCCESS, CString strMsg = CString());

	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	QByteArray		GetFuncID(int nStateID);
	QByteArray		GetSubFuncID(int nStateID);

	int				GetTagOutRowIdx(int nTagOutType = XJ_TAGOUT_UNDEFINE);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
