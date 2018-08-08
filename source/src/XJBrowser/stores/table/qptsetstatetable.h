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

class QPTSetCard1 : public QByteArrayMatrix
{
public:
	QPTSetCard1();
	QPTSetCard1(const char *);
	QPTSetCard1(const char *, const char *delim_row, const char *delim_col);
	
	virtual QByteArray FWrite(const char* pszFilePath = NULL);

	int		GetType();
	void	SetType(int nType);
	int		GetStateID();
	void	SetStateID(int nID);
	QByteArray	GetPTID();
	void	SetPTID(const char *pt_id);
	int		GetCPUID();
	void	SetCPUID(int nCPUID);
	int		GetZoneID();
	void	SetZoneID(int nZoneID);
	int		GetFlags();
	void	SetFlags(int nFlags);
};

class QLogTable1 : public QByteArrayMatrix
{
public:
	QLogTable1();
	QLogTable1(const char *);
	QLogTable1(const char *, const char *delim_row, const char *delim_col);
	
	virtual QByteArray FWrite(const char* pszFilePath = NULL);
	
	int		GetRecordCount();
	QByteArray	GetRecord(int iRow);
	
	QByteArray& Revert(int iRow);
	QByteArray& Insert(char *pszRecord, int iLen = -1);

};

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
	
	int					GetType();
	QByteArray			GetTypeName();
	int					GetStateID();
	QByteArray			GetPTID();
	int					GetCPUID();
	int					GetZoneID();
	int					GetFlags();
	QByteArray			GetRunnerUserID();
	QByteArray			GetOperUserID();
	QByteArray			GetMonitorUserID();
	QByteArray			GetStateUserID(int nStateID);
	QByteArray			GetWorkFlowStep(int nStateID);

	/** @brief           ��������*/
	QByteArrayMatrix	GetWorkFlow(int nTagOutType = -1);
	QByteArrayMatrix	GetDefaultWorkFlow(int nTagOutType = XJ_TAGOUT_PTVALVSET);
	/** @brief           ������־*/
	QByteArrayMatrix	GetLogs();
	
	void				SetType(int nType);
	void				SetStateID(int nID);
	void				SetPTID(const char *pt_id);
	void				SetCPUID(int nCPUID);
	void				SetZoneID(int nZoneID);
	void				SetFlags(int nFlags);

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
	BOOL		ReLoad();
	BOOL		ReLoad(const MODIFY_LIST &arrModifyList, const PT_SETTING_LIST &arrSetting);
	BOOL		Save(const char *pszFilePath = NULL);

	QPTSetStateTable*	m_pState;
	
};

extern const char* TAGOUT_KEYNAME;
extern const char* PTVALVSET_KEYNAME;
extern const char* PTZONESET_KEYNAME;
extern const char* PTSOFTSET_KEYNAME;


#endif // QPTSETCARD_H