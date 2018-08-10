#ifndef _QPTSETSTATETABLE_H
#define _QPTSETSTATETABLE_H

#include "XJStoreDefine.h"
#include "qmemtable.h"

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


class QPTSetDataTable;
class QPTSetStateTable : public QMemTable
{
public:
	QPTSetStateTable();
	~QPTSetStateTable();
	
public:	
	BOOL				ReLoad();
	BOOL				Save(const char *pszFilePath = NULL);
	BOOL				Check(int nTagOutType);
	
	QByteArray			GetTagOutValue(int iCol);
	int					GetType();
	QByteArray			GetTypeName();
	int					GetLastType();
	int					GetLastTypeName();
	int					GetStateID();
	QByteArray			GetPTID();
	int					GetCPUID();
	int					GetZoneID();
	int					GetFlags();
	int					GetTagOutRowIdx(int nTagOutType = XJ_TAGOUT_UNDEFINE);
	QByteArray			GetRunnerUserID();
	QByteArray			GetOperUserID();
	QByteArray			GetMonitorUserID();

	QByteArray			GetWorkFlowUserID(int nTagOutType, int nStateID);

	/** @brief           ��������*/
	QByteArrayMatrix	GetWorkFlow(int nTagOutType = -1);
	QByteArrayMatrix	GetDefaultWorkFlow(int nTagOutType = XJ_TAGOUT_PTVALVSET);
	/** @brief           ������־*/
	QByteArrayMatrix	GetLogs();
	
	void				SetTagOutValue(int iCol, QByteArray &s);
	void				SetType(int nType);
	void				SetLastType(int nType);
	void				SetStateID(int nID);
	void				SetPTID(const char *pt_id);
	void				SetCPUID(int nCPUID);
	void				SetZoneID(int nZoneID);
	void				SetFlags(int nFlags);

	void				SetWorkFlowUserID(int nTagOutType, int nStateID, const char* pszUserID);
	void				SetWorkFlowUserID(int nTagOutType, int nStateID, QByteArray &baUserID);

	QByteArrayMatrix	AddLog(int nStateID, const char *pszUserID, int nAddType = 0);
	QByteArrayMatrix	GetLog(int nStateID);
	QByteArrayMatrix	GetLog(int nStateID, int &iRow);

	int					GetTagOutReasonType(const char* pszTagOutReason);
	int					GetTagOutReasonTypeByState(int nStateID);

	QByteArray			GetTagOutReasonName(int pszTagOutReason);
	QByteArray			GetTagOutReasonNameByState(int nStateID);

public:

	BOOL		Next(const char *card_data, const char *log_data);
	BOOL		Next(int nNextStateID, const char* szUserID, int nFlag = 0);
	BOOL		Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag = 0);
	BOOL		Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag = 0);
	
//private:
	void		Next_0(const char *pszUserID);
	void		Next_1(const char *pszUserID, const char *pt_id, const char * pszHangoutReason);

	void		Next_PTSet_State_2(int nCPU, int nZone, const char *pszUserID
								, const MODIFY_LIST &arrModifyList
								, const PT_SETTING_LIST &arrSetting);
	void		Next_PTSet_State_3(const char *pszUserID);
	void		Next_PTSet_State_4(const char *pszUserID);
	void		Next_PTSet_State_5(const char *pszUserID);

	void		Next_PTSET_ZONE_STATE_2(int nCPU, int nZone, const char *pszUserID
								, const MODIFY_LIST &arrModifyList
								, const PT_SETTING_LIST &arrSetting);
	void		Next_PTSET_ZONE_STATE_3(const char *pszUserID);
	void		Next_PTSET_ZONE_STATE_4(const char *pszUserID);
	void		Next_PTSET_ZONE_STATE_5(const char *pszUserID);

	void		RevertTo_PTSet_State_1(int nFrom_PTSetStateID, const char* pszUserID, QByteArray &strMsg = QByteArray());

public:
	QPTSetDataTable*	m_pData;

};

class QPTSetDataTable : public QMemTable
{
public:
	QPTSetDataTable();
	~QPTSetDataTable();
	
public:	
	BOOL		ReLoad(QByteArray &pt_id = QByteArray());
	BOOL		ReLoad(const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting);
	BOOL		ReLoad(QByteArray &pt_id, int nCPU, int nZone, const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting);
	BOOL		Save(const char *pszFilePath = NULL);		

	BOOL		RevertModifiy();	// �޸�ֵ����ձ���
	BOOL		SaveModify();	// �޸�ֵ�滻ԭֵ����

	void		UnitTest_01();

	QPTSetStateTable*	m_pState;
	
};

class QPTZoneDataTable : public QMemTable
{
public:
	QPTZoneDataTable();
	~QPTZoneDataTable();
	
public:	
	BOOL		ReLoad(QByteArray &pt_id = QByteArray());
	BOOL		ReLoad(QByteArray &pt_id, int nCPU, int nZone, QByteArray &newZoneValue);
	BOOL		Save(const char *pszFilePath = NULL);		
	
	BOOL		RevertModifiy();	// �޸�ֵ����ձ���
	BOOL		SaveModify();	// �޸�ֵ�滻ԭֵ����
	
	void		UnitTest_01();
	
	QPTSetStateTable*	m_pState;
	
};

extern const char* TAGOUT_KEYNAME;
extern const char* PTVALVSET_KEYNAME;
extern const char* PTZONESET_KEYNAME;
extern const char* PTSOFTSET_KEYNAME;

extern const int COL_WORKFLOW_TAGOUT_ID;
extern const int COL_WORKFLOW_STATE_ID;
extern const int COL_WORKFLOW_USERGROUP_ID;
extern const int COL_WORKFLOW_ENABLE;
extern const int COL_WORKFLOW_USER_ID;

#endif // QPTSETCARD_H
