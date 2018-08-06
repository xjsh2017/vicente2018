#ifndef _QMEMTABLE_H
#define _QMEMTABLE_H

#include "qbytearraymatrix.h"

class QMemTablePrivate;
class QMemTable
{
private:
	QMemTablePrivate*	d_ptr;

public:
	QMemTable();
	QMemTable(int nTableID);
	QMemTable(const char* table_name, int nType = 0);
	~QMemTable();

	QByteArray FWrite(const char *pszFilePath = NULL);
	
	BOOL		LoadInfo(int nTableID);
	BOOL		LoadInfo(const char* table_name, int nNameType = 0);
	BOOL		LoadData(const char* sql_stmt);
	BOOL		LoadData(QByteArray &sql_stmt);
	BOOL		LoadDataAll();

	BOOL		SaveData();
	
	int			GetTableID();
	QByteArray	GetTableName(int nNameType = 0);

	int			GetFieldCount();
	QByteArray	GetFieldName(int fieldID, int nType = 0);
	int			GetFieldIndex(const char *szFieldName, int nNameType = 0);
	int			GetFieldDataType(int fieldID);

	QByteArray	GetFieldValue(int iRow, int iCol);
	QByteArray	GetFieldValue(int iRow, const char *szFieldName, int nNameType = 0);
	QByteArray	GetFieldValue(QByteArrayMatrix keyVals, int iCol);
	QByteArray	GetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, int nNameType = 0);

	int			GetKeyCount();
	QByteArray	GetKeyValue(int iRow, int idx);
	QByteArray	GetKeyName(int idx);
	int			GetValueRowIndex(QByteArrayMatrix &keyVals);

	int			GetRecordCount();
	int			GetRowCount();

	BOOL		SetFieldValue(int iRow, int iCol, QByteArray val);
	BOOL		SetFieldValue(int iRow, const char *szFieldName, QByteArray val);
	BOOL		SetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, QByteArray val);

private:
};

#endif // _QMEMTABLE_H
