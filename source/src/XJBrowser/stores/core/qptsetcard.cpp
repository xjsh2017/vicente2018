// DBEngine.cpp : implementation file
//

#include "stdafx.h"
#include "qptsetcard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CPTSetCard::reset() 
{
	m_cEnd = '\0';
	memset((char*)&m_szType, ' ', strlen(m_szType));
}

int CPTSetCard::FRead(char *pszLine, size_t s)
{
	reset();
	
	size_t uLen = strlen(pszLine);
	m_uLen = strlen(m_szType);
	m_uLen = uLen > m_uLen ? m_uLen : uLen;
	memcpy((char*)&m_szType, pszLine, m_uLen);
	
	return 0;
}

// int CPTSetCard::FWrite(fstream &fs)
// { 
// 	fs.write((char*)&m_szType, m_uLen); 
// 	return 0; 
// }

const char* CPTSetCard::GetValue() const 
{ 
	return m_szType; 
}

int CPTSetCard::GetType() const
{
	char szOut[3] = { 0 }; 
	strncpy(szOut, m_szType, sizeof(m_szType));
	return atoi(szOut);
}

void CPTSetCard::SetType(int nType)
{
	char szFormat[10] = { 0 };
	sprintf(szFormat, "%-2d", nType);
	strncpy(m_szType, szFormat, sizeof(m_szType));
}

int CPTSetCard::GetStateID() const
{
	char szOut[3] = { 0 }; 
	strncpy(szOut, m_szID, sizeof(m_szID));
	return atoi(szOut);
}

void CPTSetCard::SetStateID(int nID)
{
	char szFormat[10] = { 0 };
	sprintf(szFormat, "%-2d", nID);
	strncpy(m_szID, szFormat, sizeof(m_szID));
}

void CPTSetCard::GetPTID(char *pt_id, int iLen)
{
	int iTemp = sizeof(m_szPT_ID);
	iLen = iTemp > iLen ? iLen : iTemp;
	strncpy(pt_id, m_szPT_ID, iLen);
}

void CPTSetCard::SetPTID(char *pt_id)
{
	memset((char*)&m_szPT_ID, ' ', sizeof(m_szPT_ID));
	int iLen = sizeof(m_szPT_ID);
	if (strlen(pt_id) < iLen)
		iLen = strlen(pt_id);
	//cout << iLen << endl;
	//strncpy(m_szPT_ID, pt_id, min(sizeof(m_szPT_ID), strlen(pt_id)));
	strncpy(m_szPT_ID, pt_id, iLen);
}

int CPTSetCard::GetCPUID()
{
	char szOut[10] = { 0 }; 
	strncpy(szOut, m_szCPU_ID, sizeof(m_szCPU_ID));
	return atoi(szOut);
}

void CPTSetCard::SetCPUID(int nCPUID)
{
	char szFormat[10] = { 0 };
	sprintf(szFormat, "%-2d", nCPUID);
	strncpy(m_szCPU_ID, szFormat, sizeof(m_szCPU_ID));
}

int CPTSetCard::GetZoneID()
{
	char szOut[10] = { 0 }; 
	strncpy(szOut, m_szZone_ID, sizeof(m_szZone_ID));
	return atoi(szOut);
}

void CPTSetCard::SetZoneID(int nZoneID)
{
	char szFormat[10] = { 0 };
	sprintf(szFormat, "%-2d", nZoneID);
	strncpy(m_szZone_ID, szFormat, sizeof(m_szZone_ID));
}

int CPTSetCard::GetFlags() const
{
	char szOut[10] = { 0 }; 
	strncpy(szOut, m_szFlags, sizeof(m_szFlags));
	return atoi(szOut);
}
void CPTSetCard::SetFlags(int nFlags)
{
	char szFormat[10] = { 0 };
	sprintf(szFormat, "%-5d", nFlags);
	strncpy(m_szFlags, szFormat, sizeof(m_szFlags));
}

BOOL CPTSetCard::RevertRecords(int idx)
{
	return TRUE;
}

void CPTSetCard::FPrint()
{
	cout << "CPTSetCard: " << endl;
	cout << "----------------------------" << endl;
	cout << "| Value    | [" << GetValue() << "]" << endl;
	cout << "| Len      |  " << GetLen() << endl;
	cout << "----------------------------" << endl;
	cout << "| Type     | " << GetType() << endl;
	cout << "| State ID | " << GetStateID() << endl;
	char szValue[PT_ID_LEN + 1]={0};
	GetPTID(szValue, sizeof(szValue));
	cout << "| PT    ID | " << szValue << endl;
	cout << "| CPU   ID | " << GetCPUID() << endl;
	cout << "| Zone  ID | " << GetZoneID() << endl;
	cout << "| Flags    | " << GetFlags() << endl;
	cout << "----------------------------" << endl;
	
	cout << endl;
}

///////////////////////////////////////////////////////////////

//char* CLogCard::GetLog(int idx, char *pszRecord, int iLen)
//template<size_t N>
char* CLogCard::GetLogData(int iRow, char* pszOut, int iLen)
{
	char *pReturn = pszOut;
	
	char szReserve1[MAX_FIELD_VALE_LEN + 1]={0};
	strncpy(szReserve1, data, sizeof(szReserve1));
	
	size_t nLen = strlen(szReserve1);
	int i = 0;
	int j = nLen - 1;
	
	while(szReserve1[j] == ' ') 
		--j;
	data[j + 1] = '\0';
	//cout << "j = " << j << endl;
	
	while(i < j){
		if (szReserve1[i] == ';')
			szReserve1[i] = '\0';
		
		++i;
	}
	
	char *p = szReserve1;
	int offset = 0;
	p = qf::qf_priv(p, iRow, offset);
	
	nLen = strlen(p);
	nLen = nLen > iLen - 1 ? iLen - 1 : nLen;
	strncpy(pReturn, p, nLen);
	
// 		cout << "offset = " << offset << endl;
// 		cout << "p = " << p << endl;
// 		cout << "pszRecord = " << pszRecord << endl;
	return pReturn;
}

//template<size_t N>
int CLogCard::GetLogCount()
{
	int nCount = 0;
	int i = 0;
	size_t nLen = strlen(data);
	while(i < nLen){
		if (data[i] == ';')
			nCount++;
		
		++i;
	}
	nCount += 1;
	
	return nCount;
}

//template<size_t N>
BOOL CLogCard::RevertTo(int idx)
{
	return TRUE;
}

//template<size_t N>
BOOL CLogCard::Prepend()
{
	
	return TRUE;
}

//template<size_t N>
BOOL CLogCard::Append(char *pszRecord, int iLen)
{
	if (NULL == pszRecord)
		return FALSE;
	
	size_t nLen = strlen(pszRecord);
	
	int j = strlen(data) - 1;
	while(data[j] == ' ') 
		--j;
	
	data[j + 1] = ';';
	size_t nRightCap = sizeof(data) - j - 2;
	if (nLen > nRightCap)
		return FALSE;
	
	//cout << "nRightCap = " << nRightCap << endl;
	//cout << "nLen = " << nLen << endl;
	strncpy(data + j + 2, pszRecord, nLen);
	
	return TRUE;
}

//template<size_t N>
void CLogCard::PopFront()
{
	
}

//template<size_t N>
void CLogCard::PopBack(char *pszRecord, int iLen)
{
	if (NULL == pszRecord)
		return;
	
	size_t nLen = strlen(pszRecord);
	
	int j = strlen(data) - 1;
	while(data[j] == ' ') 
		--j;
	
	data[j + 1] = ';';
	size_t nRightCap = sizeof(data) - j - 2;
	if (nLen > nRightCap)
		return;
	cout << "nRightCap = " << nRightCap << endl;
	cout << "nLen = " << nLen << endl;
	strncpy(data + j + 2, pszRecord, nLen);
	
	cout << strlen(data) << endl;
	cout << sizeof(data) << endl;
	cout << data << endl;
}

//template<size_t N>
int CLogCard::GetDataLen()
{
	int j = strlen(data) - 1;
	while(data[j] == ' ') 
		--j;

	return j + 1;
}

//template<size_t N>
void CLogCard::Print()
{
	cout << "CLogCard: " << endl;
	cout << "----------------------------" << endl;
	cout << "Value    | [" << GetValue() << "]" << endl;
	cout << "Capacity |  " << GetLen() << endl;
	cout << "Len      |  " << GetDataLen() << endl;
	cout << "----------------------------" << endl;

	int nCount = GetLogCount();
	cout << "Log Count : " << nCount << endl;
	
	char szRecord[MAX_FIELD_VALE_LEN]={0};
	for (int i = 0; i < nCount; i++){
		memset(szRecord, 0, sizeof(szRecord));
		char *p = GetLogData(i, szRecord, sizeof(szRecord));
		cout << i << ": " << p << endl;
	}

	cout << "----------------------------" << endl;
	cout << endl;
}

CMemTable::CMemTable()
	: m_delim_row(';'), m_delim_col(',')
{

}

CMemTable::CMemTable(const char *pszIn)
	: QByteArray(pszIn), m_delim_row(';'), m_delim_col(',')
{

}

CMemTable::CMemTable(const char *pszIn, const char *delim_row, const char *delim_col)
	: QByteArray(pszIn), m_delim_row(*delim_row), m_delim_col(*delim_col)
{

}

int CMemTable::GetRecordCount()
{
	return GetRows();
}

int CMemTable::GetRows()
{
	return this->count(m_delim_row) + 1;
}

int CMemTable::GetFieldCount()
{
	return GetCols();
}

int CMemTable::GetCols()
{
	int index = this->indexOf(m_delim_row);
	return this->left(index).count(m_delim_col) + 1;
}

QByteArray	CMemTable::GetRecord(int iRow)
{
	int from = 0;
	int end = 0;

	int rows = GetRecordCount();
	
	if (iRow < 1 || iRow > rows)
		return QByteArray();

	for (int i = 1; i <= iRow; i++){
		end = indexOf(m_delim_row, from);

		if (i != iRow)
			from = end + 1;
	}

	if (iRow == this->GetRecordCount()){
		from = lastIndexOf(m_delim_row) + 1;
		end = this->count();
	}

	if (end < from)
		return QByteArray();

	return this->mid(from, end - from);
}


QByteArray	CMemTable::GetFiled(int iRow, int iCol)
{
	int from = 0;
	int end = 0;
	
	int rows = GetRecordCount();
	int cols = GetFieldCount();

	if (iRow < 1 || iCol < 1 || iRow > rows || iCol > cols)
		return QByteArray();

	QByteArray record = GetRecord(iRow);
	for (int i = 1; i <= iCol; i++){
		end = record.indexOf(m_delim_col,from);
		
		if (i != iCol)
			from = end + 1;
	}

	if (iCol == record.count(m_delim_col) + 1){
		from = record.lastIndexOf(m_delim_col) + 1;
		end = record.count();
	}
	
	if (end < from)
		return QByteArray();
	
	return record.mid(from, end - from);
}

QByteArray& CMemTable::Revert(int iRow)
{
	int from = 0;
	int end = count();
	
	int rows = GetRecordCount();
	
	if (iRow < 1 || iRow > rows)
		return *this;
	
	for (int i = 1; i <= iRow; i++){
		from = indexOf(m_delim_row, from);
		
		if (i != iRow)
			from = from + 1;
	}

	return remove(from, end - from);
}

QByteArray& CMemTable::Insert(char *pszRecord, int iLen)
{
	QByteArray byte(";");
	byte += pszRecord;
	return append(byte);
}


/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

