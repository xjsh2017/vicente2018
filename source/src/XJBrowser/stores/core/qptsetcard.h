#ifndef _QPTSETCARD_H
#define _QPTSETCARD_H

#include "qcardbase.h"
#include "qbytearraymatrix.h"

#define PT_ID_LEN 20
#define MAX_FIELD_VALE_LEN 256

//* ��ֵ�޸Ŀ��ƿ�
class CPTSetCard : public CCardBase
{
public:
	CPTSetCard():CCardBase(){ reset();}
	~CPTSetCard(){}

	void reset();
	virtual int		FRead(char *pszLine, size_t s);
	//virtual int		FWrite(fstream &fs);
	virtual void	FPrint();

	const char* GetValue() const;
	const char* data() const;

	int		GetType() const;
	void	SetType(int nType);
	int		GetStateID() const;
	void	SetStateID(int nID);
	void	GetPTID(char *pt_id, int iLen);
	void	SetPTID(char *pt_id);
	int		GetCPUID();
	void	SetCPUID(int nCPUID);
	int		GetZoneID();
	void	SetZoneID(int nZoneID);
	int		GetFlags() const;
	void	SetFlags(int nFlags);
	char*	GetRecord(int idx, char *pszRecord, int iLen);
	BOOL	RevertRecords(int idx);

protected:

private:
	// ����: 0 - ��ֵ�޸ģ�1 - ��ֵ���л���2 - ��ѹ��Ͷ��
	char m_szType[2];
	// ״̬��ID
	char m_szID[2];
	// ����ID
	char m_szPT_ID[PT_ID_LEN];	
	// CPU��
	char m_szCPU_ID[2];
	// ��ֵ����
	char m_szZone_ID[2];		
	// ��־λ			
	char m_szFlags[5];
	// ����λ
	char m_cEnd;
};

class CLogCard:CCardBase
{
public:
	CLogCard():CCardBase(){ reset(); }
	~CLogCard(){}

	void reset() 
	{
		m_cEnd = '\0';
		memset((char*)&data, ' ', strlen(data));
	}

	virtual int FRead(char *pszLine, size_t s)
	{
		reset();
		
		size_t uLen = strlen(pszLine);
		m_uLen = strlen(data);
		m_uLen = uLen > m_uLen ? m_uLen : uLen;
		memcpy((char*)&data, pszLine, m_uLen);

		return 0;
	}
	virtual int FWrite(fstream &fs){ fs.write((char*)&data, m_uLen); return 0; }

	const char* GetValue() const { return data; }

	int		GetLogCount();
	int		GetDataLen();
	char*	GetLogData(int iRow, char *pszOut, int iLen);
	char*	GetLogData(int iRow, int iCol, char *pszOut, int iLen);

	BOOL	RevertTo(int idx);
	BOOL	Prepend();
	BOOL	Append(char *pszRecord, int iLen);
	void	PopFront();
	void	PopBack(char *pszRecord, int iLen);
	
	void	Print();

public:
	// �����ֶ�1
	char data[MAX_FIELD_VALE_LEN]; 
	//char data[N]; 
	char m_cEnd;
};

class QPTSetCard : public QByteArrayMatrix
{
public:
	QPTSetCard();
	QPTSetCard(const char *);
	QPTSetCard(const char *, const char *delim_row, const char *delim_col);
	
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

class QLogTable : public QByteArrayMatrix
{
public:
	QLogTable();
	QLogTable(const char *);
	QLogTable(const char *, const char *delim_row, const char *delim_col);
	
	virtual QByteArray FWrite(const char* pszFilePath = NULL);
	
	int		GetRecordCount();
	QByteArray	GetRecord(int iRow);
	
	QByteArray& Revert(int iRow);
	QByteArray& Insert(char *pszRecord, int iLen = -1);

};

#endif // QPTSETCARD_H
