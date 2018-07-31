#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"

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

#define PT_ID_LEN 20

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

// Operations
public:
	/*
	 *  @brief   	AddManOperator	 ����˹�������־ 
	 *  @param 		[In]a param of Type  CString  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  ������������
	 *  @return 	void
	 */
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult  = 0, int num = -1);
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult  = 0, int num  = -1);
	
	/*
	 *  @brief   	GetStatus	 ��ȡ״̬��״̬ 
	 *  @param 		void
	 *  @return 	CPTSetCard* �����޸Ŀ�ָ��	
	 */
	CXJPTSetStore* GetStore();

	/*
	 *  @brief   	SaveStore	 ��ȡ״̬��״̬ 
	 *  @param 		[In]a param of Type  CString  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  ������������
	 *  @return 	PTSETSTORE	
	 */
	BOOL SaveStore();

	BOOL NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sRecords = CString());
	BOOL RevertPTSetModState(int nRevertStateID, int nFlag = 0);
	BOOL SetRevertModifyValueFlag(int nFlag);
	CString GetUserIDByState(int nState, CString &sRecords = CString());
	BOOL SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag = 0);
	
	/*
	 *  @brief   	GetStatus	 ���״̬���������
	 *  @param 		void
	 *  @return 	int		-1: ���ü��ʧ��;
							0: ���ô���;
							1: ���ò�����;
							10: ���ò����ڣ�����ʧ��;
							11: ���ò����ڣ������ɹ�	
	 */
	int			CheckStore();
	
	CString		GetUserGroupNameByID(CString sUserGroupID);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
