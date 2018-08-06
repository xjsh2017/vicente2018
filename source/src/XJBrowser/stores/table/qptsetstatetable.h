#ifndef _QPTSETSTATETABLE_H
#define _QPTSETSTATETABLE_H

#include "qmemtable.h"

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

class QPTSetStateTable : public QMemTable
{
public:
	QPTSetStateTable();
	~QPTSetStateTable();
	
public:	
	BOOL				ReLoad();
	BOOL				Save(const char *pszFilePath = NULL);
	
	int					GetType();
	int					GetStateID();
	QByteArray			GetPTID();
	int					GetCPUID();
	int					GetZoneID();
	int					GetFlags();
	QByteArray			GetRunnerUserID();
	QByteArray			GetOperUserID();
	QByteArray			GetMonitorUserID();

	/** @brief           工作流程*/
	QByteArrayMatrix	GetWorkFlow();
	QByteArrayMatrix	GetDefaultWorkFlow();
	/** @brief           工作日志*/
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

	int					GetHangoutReasonType(const char* pszHangoutReason);
	QByteArray			GetHangoutReasonName(int nHangoutReasonType);

public:

	BOOL	Next(const char *card_data, const char *log_data);
	BOOL	Next(int nNextStateID, const char* szUserID, int nFlag = 0);
	BOOL	Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag = 0);
	BOOL	Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag = 0);

	void		Next_0(const char *pszUserID);
	void		Next_1(const char *pszUserID, const char *pt_id, const char * pszHangoutReason);
	
	//private:
	void		Next_PTSet_State_2(int nCPU, int nZone, const char *szUserID
		, const MODIFY_LIST &arrModifyList
		, const PT_SETTING_LIST &arrSetting);
	void		Next_PTSet_State_3();
	void		Next_PTSet_State_4();
	void		Next_PTSet_State_5();
};

extern const char* PTSET_KEYNAME;


#endif // QPTSETCARD_H
