// DBEngine.cpp : implementation file
//

#include "stdafx.h"
#include "qptsetcard.h"

#include <time.h> 

QByteArray GetTime(int nType)
{
	time_t tt = time(NULL);
	struct tm *local;  
	local=localtime(&tt);  //获取当前系统时间  
	
	char szLine[256] = {0};
	char *pxq[]={"日","一","二","三","四","五","六"};

	if (nType == 0){
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec);
	}else if (1 == nType){
		sprintf(szLine, "%4d-%02d-%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday);
	}else{
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d 星期%s"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec
			, pxq[local->tm_wday]);
	}

	return QByteArray(szLine);
}

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
	return (char*)&m_szType; 
}

const char* CPTSetCard::data() const 
{ 
	return (char*)&m_szType; 
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


/////////////////////////////////////////////////////////////
// QMatrixByteArray
//
QMatrixByteArray::QMatrixByteArray()
	: m_delim_row(";"), m_delim_col(",")
{

}

QMatrixByteArray::QMatrixByteArray(const char *pszIn)
	: QByteArray(pszIn), m_delim_row(";"), m_delim_col(",")
{

}

QMatrixByteArray::QMatrixByteArray(const char *pszIn, const char *delim_row, const char *delim_col)
	: QByteArray(pszIn), m_delim_row(delim_row), m_delim_col(delim_col)
{

}

int QMatrixByteArray::GetRows()
{
	return this->count(m_delim_row) + 1;
}

int QMatrixByteArray::GetCols()
{
	if (isEmpty())
		return 1;

	int index = this->indexOf(m_delim_row);
	if (index == -1)
		index = count() - 1;
	return this->left(index).count(m_delim_col) + 1;
}

QMatrixByteArray QMatrixByteArray::GetRow(int iRow)
{
	QByteArray row_data = GetRowData(iRow);

	return QMatrixByteArray(row_data.constData(), m_delim_row.constData(), m_delim_col.constData());
}

QByteArray	QMatrixByteArray::GetRowData(int iRow)
{
	int from = 0;
	int end = 0;
	
	int rows = GetRows();
	
	if (iRow < 1 || iRow > rows)
		return QByteArray();
	
	for (int i = 1; i <= iRow; i++){
		end = indexOf(m_delim_row, from);
		
		if (i != iRow)
			from = end + 1;
	}
	
	if (iRow == rows){
		from = lastIndexOf(m_delim_row) + 1;
		end = this->count();
	}
	
	if (end < from)
		return QByteArray();
	
	return this->mid(from, end - from);
}


QByteArray	QMatrixByteArray::GetFiled(int iRow, int iCol)
{
	m_from = 0;
	m_end = 0;
	
	int rows = GetRows();
	int cols = GetCols();

	if (iRow < 1 || iCol < 1 || iRow > rows || iCol > cols)
		return QByteArray();

	QByteArray record = GetRowData(iRow);
	for (int i = 1; i <= iCol; i++){
		m_end = record.indexOf(m_delim_col, m_from);
		
		if (i != iCol)
			m_from = m_end + 1;
	}

	if (iCol == record.count(m_delim_col) + 1){
		m_from = record.lastIndexOf(m_delim_col) + 1;
		m_end = record.count();
	}
	
	if (m_end < m_from)
		return QByteArray();
	
	return record.mid(m_from, m_end - m_from);
}

void QMatrixByteArray::SetFiled(int iRow, int iCol, const char *val)
{
	GetFiled(iRow, iCol);
	replace(m_from, m_end - m_from, val);
}

void QMatrixByteArray::SetFiled(int iRow, int iCol, const QByteArray &s)
{
	GetFiled(iRow, iCol);
	replace(m_from, m_end - m_from, s);
}

///////////////////////////////////////////////////////////////////
//  QPTSetCard
// 
QPTSetCard::QPTSetCard()
: QMatrixByteArray("", ";", ",")
{
	
}

QPTSetCard::QPTSetCard(const char *pszIn)
: QMatrixByteArray(pszIn, ";", ",")
{
	
}

QPTSetCard::QPTSetCard(const char *pszIn, const char *delim_row, const char *delim_col)
: QMatrixByteArray(pszIn, delim_row, delim_col)
{
	
}

int QPTSetCard::FWrite(int nType, const char *pszFilePath)
{
	if (nType == 0){
		cout << "[ " << GetTime().data() << " ]" << endl;
		cout << "----------------------------" << endl;
		cout << "| Value    | [" << GetValue() << "]" << endl;
		cout << "| Len      |  " << count() << endl;
		cout << "----------------------------" << endl;
		cout << "| Type     | " << GetType() << endl;
		cout << "| State ID | " << GetStateID() << endl;
		cout << "| PT    ID | " << GetPTID().data() << endl;
		cout << "| CPU   ID | " << GetCPUID() << endl;
		cout << "| Zone  ID | " << GetZoneID() << endl;
		cout << "| Flags    | " << GetFlags() << endl;
		cout << "----------------------------" << endl;
		
		cout << endl;
	}else if (nType == 1){
		if (NULL == pszFilePath)
			return -1;

		fstream fs;
		fs.open(pszFilePath, std::fstream::out/* | std::fstream::app*/);
		//FWrite(fs);

		QByteArray out;

		char szLine[256] = {0};
		sprintf(szLine, "[ %s ] : [%s]\n", GetTime().data(), data());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "Len", count());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "Type", GetType());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "State ID", GetStateID());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%s]\n", "PT ID", GetPTID().data());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "CPU ID", GetCPUID());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "Zone ID", GetZoneID());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));
		sprintf(szLine, "| %20s |  [%d]\n", "Flags", GetFlags());
		out.append(szLine);
		memset(szLine, 0, sizeof(szLine));


		fs.write(out.data(), out.count());
	}

	return 0;
}


int	QPTSetCard::GetType()
{
	return GetFiled(1, 1).toInt();
}

void QPTSetCard::SetType(int nType)
{
	SetFiled(1, 1, QByteArray::number(nType));
}

int	QPTSetCard::GetStateID()
{
	return GetFiled(1, 2).toInt();
}

void QPTSetCard::SetStateID(int nID)
{
	SetFiled(1, 2, QByteArray::number(nID));
}

QByteArray QPTSetCard::GetPTID()
{
	return GetFiled(1, 3);
}

void QPTSetCard::SetPTID(const char *pt_id)
{
	SetFiled(1, 3, QByteArray(pt_id));
}

int QPTSetCard::GetCPUID()
{
	return GetFiled(1, 4).toInt();
}

void QPTSetCard::SetCPUID(int nCPUID)
{
	SetFiled(1, 4, QByteArray::number(nCPUID));
}

int QPTSetCard::GetZoneID()
{
	return GetFiled(1, 5).toInt();
}

void QPTSetCard::SetZoneID(int nZoneID)
{
	SetFiled(1, 5, QByteArray::number(nZoneID));
}

int QPTSetCard::GetFlags()
{
	return GetFiled(1, 6).toInt();
}

void QPTSetCard::SetFlags(int nFlags)
{
	SetFiled(1, 6, QByteArray::number(nFlags));
}


/////////////////////////////////////////////////////////////
// QLogTable
//
QLogTable::QLogTable()
	: QMatrixByteArray("", ";", ",")
{

}

QLogTable::QLogTable(const char *pszIn)
	: QMatrixByteArray(pszIn, ";", ",")
{

}

QLogTable::QLogTable(const char *pszIn, const char *delim_row, const char *delim_col)
	: QMatrixByteArray(pszIn, delim_row, delim_col)
{

}

int QLogTable::GetRecordCount()
{
	return GetRows();
}

QByteArray	QLogTable::GetRecord(int iRow)
{
	return GetRowData(iRow);
}

QByteArray& QLogTable::Revert(int iRow)
{
	int from = 0;
	int end = count();
	
	int rows = GetRecordCount();
	
	if (iRow < 1 || iRow > rows)
		return *this;
	
	for (int i = 1; i <= iRow; i++){
		from = indexOf(GetDelimRow(), from);
		
		if (i != iRow)
			from = from + 1;
	}

	return remove(from, end - from);
}

QByteArray& QLogTable::Insert(char *pszRecord, int iLen)
{
	QByteArray byte(GetDelimRow());
	byte += pszRecord;
	return append(byte);
}

int QLogTable::FWrite(int nType, const char *pszFilePath)
{
	if (nType == 0){
		cout << "[ " << GetTime().data() << " ]" << endl;
		cout << "----------------------------" << endl;
		cout << "| Value    | [" << GetValue() << "]" << endl;
		cout << "| Len      |  " << count() << endl;
		cout << "----------------------------" << endl;
		cout << "| Record Count     | " << GetRecordCount() << endl;
		cout << "| Field Count      | " << GetFieldCount() << endl;
		cout << "----------------------------" << endl;

		for (int i = 1; i <= GetRecordCount(); i++){
			for (int j = 1; j <= GetFieldCount(); j++){
				if (j == 1)
					cout << "| ";
				cout << GetFiled(i,j).data() << " | ";
			}
			cout << endl;
		}
		
		cout << endl;
	}else if (nType == 1){
		if (NULL == pszFilePath)
			return -1;
		
		fstream fs;
		fs.open(pszFilePath, std::fstream::out/* | std::fstream::app*/);
		//FWrite(fs);
		
		QByteArray out;
		
		char szLine[256] = {0};
		sprintf(szLine, "[ %s ] : [%s]\n", GetTime().data(), data());
		out.append(szLine);
		
		fs.write(out.data(), out.count());
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

