#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "core/qbytearray.h"

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

/** @brief			װ�ù���*/
int const XJ_OPER_HANGOUT = 101;
/** @brief			װ��ȡ������*/
int const XJ_OPER_UNHANGOUT = 102;

/** @brief			��ֵ�޸�*/
int const XJ_OPER_PTSET = 201;
/** @brief			��ֵ�޸�ԤУ*/
int const XJ_OPER_PTSET_PRECORRECTION = 202;
/** @brief			��ֵ�޸�ִ��*/
int const XJ_OPER_PTSET_EXECUTE = 203;
/** @brief			��ֵ�޸�:�޸ĺ˶�*/
int const XJ_OPER_PTSET_STATE_2 = 204;
/** @brief			��ֵ�޸�:����*/
int const XJ_OPER_PTSET_STATE_3 = 205;
/** @brief			��ֵ�޸�:��֤*/
int const XJ_OPER_PTSET_STATE_4 = 206;
/** @brief			��ֵ�޸�:�޸����*/
int const XJ_OPER_PTSET_STATE_5 = 207;

/** @brief			��ֵ���޸�*/
int const XJ_OPER_PTZONESET = 301;
/** @brief			��ֵ���޸�ԤУ*/
int const XJ_OPER_PTZONESET_PRECORRECTION = 302;
/** @brief			��ֵ���޸�ִ��*/
int const XJ_OPER_PTZONESET_EXECUTE = 303;

/** @brief			��ѹ���޸�*/
int const XJ_OPER_PTSOFTSET = 401;
/** @brief			��ѹ��Ͷ��ԤУ*/
int const XJ_OPER_PTSOFTSET_PRECORRECTION = 402;
/** @brief			��ѹ��Ͷ��ִ��*/
int const XJ_OPER_PTSOFTSET_EXECUTE = 403;

struct PT_SETTING_DATA{
	PT_SETTING* pts;
	QByteArray reserve1;	// ԭֵ
	QByteArray reserve2;	// ����
	QByteArray reserve3;	// �޸�ֵ
	BOOL bMod;
	
	PT_SETTING_DATA::PT_SETTING_DATA(){
		bMod = FALSE;
		pts = NULL;
		reserve1 = "";
		reserve2 = "";
		reserve3 = "";
	}
};
typedef CTypedPtrArray<CPtrArray, PT_SETTING_DATA*>		PT_SETTING_DATA_LIST;
typedef CTypedPtrArray<CPtrArray, PT_SETTING*>			PT_SETTING_LIST;

class QPTSetCard;
class QLogTable;
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
	 *  @brief   	ReLoadStore	 ��ȡ״̬��״̬ 
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL	ReLoad();	
	BOOL	ReLoadStore();

	/*
	 *  @brief   	SaveStore	 ����״̬�� 
	 *  @param 		[In]a param of Type  CString  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  ������������
	 *  @return 	PTSETSTORE	
	 */
	BOOL	Save();
	
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

	/* ���ƿ� */
	QPTSetCard*	GetCard();
	/* ��־ */
	QLogTable*	GetLog();
	
	BOOL	Next(const char *card_data, const char *log_data);
	BOOL	Next(int nNextStateID, const char* szUserID, int nFlag = 0);
	BOOL	Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag = 0);
	BOOL	Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag = 0);
	
	BOOL	SaveRecallToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL	SaveModifyToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL	RevertModify();

	BOOL	SetCardFlags(int nFlag);

	/*
	 *  @brief   	AddManOperator	 ����˹�������־ 
	 *  @param 		[In]a param of Type  CString  FunID  ����ģ��ID
	 *  @param 		[In]a param of Type  CString  Act	 ������������
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID);

	CString		GetUserGroupNameByID(CString sUserGroupID);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	CString		GetFuncID(int nStateID);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
