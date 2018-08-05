#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "qptsetcard.h"

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

/**
 * @defgroup 	CXJPTSetStore	定值修改内存数据
 * @{
 */ 
/**
 * @brief       定值修改时所用到的数据结构
 * @author      WPS
 * @version     ver1.0
 * @date        09/07/18

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

struct PT_SETTING_DATA{
	PT_SETTING* pts;
	QByteArray reserve1;	// 原值
	QByteArray reserve2;	// 备用
	QByteArray reserve3;	// 修改值
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
	/** @brief           动态库句柄*/
	HMODULE	m_hModule;

// Operations
public:
	/*
	 *  @brief   	ReLoadStore	 获取状态机状态 
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL	ReLoad();	
	BOOL	ReLoadStore();

	/*
	 *  @brief   	SaveStore	 保存状态机 
	 *  @param 		[In]a param of Type  CString  功能模块ID
	 *  @param 		[In]a param of Type  CString  操作动作描述
	 *  @return 	PTSETSTORE	
	 */
	BOOL	Save();
	
	/*
	 *  @brief   	CheckStore	 检查状态机配置情况
	 *  @param 		void
	 *  @return 	int		-1: 配置检查失败;
							0: 配置存在;
							1: 配置不存在;
							10: 配置不存在，创建失败;
							11: 配置不存在，创建成功	
	 */
	int		Check();

	/* 控制卡 */
	QPTSetCard*	GetCard();
	/* 日志 */
	QLogTable*	GetLog();
	/* 存储 */
	PT_SETTING_DATA_LIST& GetStoreData();
	/* 工作流程 */
	QByteArrayMatrix& GetWorkFlow();

	BOOL	Next(const char *card_data, const char *log_data);
	BOOL	Next(int nNextStateID, const char* szUserID, int nFlag = 0);
	BOOL	Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag = 0);
	BOOL	Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag = 0);
	
	BOOL	SaveRecallToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL	SaveModifyToDB(CString &sPTID, const MODIFY_LIST &arrModifyList);
	BOOL	RevertModify();

	QByteArrayMatrix GetDefaultWorkFlow();

	/*
	 *  @brief   	AddManOperator	 添加人工操作日志 
	 *  @param 		[In]a param of Type  CString  FunID  功能模块ID
	 *  @param 		[In]a param of Type  CString  Act	 操作动作描述
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID);

	CString		GetUserTypeName(CString sUserGroupID);
	CString		GetUserTypeName(int nUserType);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	CString		GetFuncID(int nStateID);

	void		Next_0();
	void		Next_1(const char *pt_id);

//private:
	void		Next_PTSet_State_2(int nCPU, int nZone, const char *szUserID
									, const MODIFY_LIST &arrModifyList
									, const PT_SETTING_LIST &arrSetting);
	void		Next_PTSet_State_3();
	void		Next_PTSet_State_4();
	void		Next_PTSet_State_5();

// Implementation
public:

	// Generated message map functions
protected:
};

extern const char* PTSET_KEYNAME;

#endif // !defined(_XJPTSETSTORE_H)
