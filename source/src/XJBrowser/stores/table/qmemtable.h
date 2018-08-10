#ifndef _QMEMTABLE_H
#define _QMEMTABLE_H

#include "qbytearraymatrix.h"
#include "XJStoreDefine.h"

class QMemTablePrivate;
class QMemTable
{
private:
	QMemTablePrivate*	d_ptr;

public:
	QMemTable();
	QMemTable(int nTableID);
	QMemTable(const char* table_name, int nNameType = XJ::LANG_ENG);
	~QMemTable();

	QByteArray FWrite(const char *pszFilePath = NULL);
	
	BOOL		LoadInfo(int nTableID);
	BOOL		LoadInfo(const char* table_name, int nNameType = XJ::LANG_ENG);
	BOOL		LoadData(const char* sql_stmt);
	BOOL		LoadData(QByteArray &sql_stmt);
	BOOL		LoadDataAll();

	BOOL		SaveData();
	
	int			GetTableID();
	QByteArray	GetTableName(int nNameType = XJ::LANG_ENG);

	int			GetFieldCount();
	QByteArray	GetFieldName(int iCol, int nNameType = XJ::LANG_ENG);
	int			GetFieldIndex(const char *szFieldName, int nNameType = XJ::LANG_ENG);
	int			GetFieldDataType(int iCol);

	QByteArray	GetFieldValue(int iRow, int iCol);
	QByteArray	GetFieldValue(int iRow, const char *szFieldName, int nNameType = XJ::LANG_ENG);
	QByteArray	GetFieldValue(QByteArrayMatrix keyVals, int iCol);
	QByteArray	GetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, int nNameType = XJ::LANG_ENG);

	int			GetKeyCount();
	QByteArray	GetKeyValue(int iRow, int idx);
	QByteArray	GetKeyName(int idx);
	int			GetRowIndex(QByteArrayMatrix &keyVals);
	int			GetRowIndex(const char* keyVals);

	int			GetRecordCount();
	int			GetRowCount();

	BOOL		SetFieldValue(int iRow, int iCol, QByteArray val);
	BOOL		SetFieldValue(int iRow, const char *szFieldName, QByteArray val);
	BOOL		SetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, QByteArray val);
	BOOL		SetFieldValue(const char* keyVals, const char *szFieldName, QByteArray val);

	BOOL		HasField(const char* fieldName, int nNameType = XJ::LANG_ENG);
	BOOL		HasField(const QByteArray &fieldName, int nNameType = XJ::LANG_ENG);
	BOOL		AddField(const char* fieldName, FIELD_TYPE_ENUM enFieldType, QByteArray initVal);

	int			AddRow(QByteArrayMatrix keyVals);

private:
};

#endif // _QMEMTABLE_H
