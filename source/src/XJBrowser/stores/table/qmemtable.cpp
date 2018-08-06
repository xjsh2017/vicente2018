// qmemtable.cpp : implementation file
//

#include "stdafx.h"
#include "qmemtable.h"

#include "XJBrowser.h"
#include "XJStoreDefine.h"

////////////////////////////////////////////////////////////
// QMemTablePrivate
// 
class QMemTablePrivate 
{
public:
	QMemTablePrivate();
	QMemTablePrivate(int nTableID);
	QMemTablePrivate(const char* table_name, int nNameType = 0);
    ~QMemTablePrivate();
	
	/** @brief           表ID*/
	int					m_nTableID;

	/** @brief           表信息*/
	QByteArrayMatrix	m_info_table;
	/** @brief           表信息定义信息 10000*/
	QByteArrayMatrix	m_info_table_define;

	/** @brief           表字段信息*/
	QByteArrayMatrix	m_info_field;
	/** @brief           表字段定义信息 10001*/
	QByteArrayMatrix	m_info_field_define;

	/** @brief           表数据（全部、局部）单表数据大小最大不超过 4G */
	QByteArrayMatrix	m_data;

	/** @brief           主键字段在数据表中位置*/
	vector<int>			m_key_col_idx;	
	/** @brief           主键字段在字段定义表中的位置*/
	vector<int>			m_key_row_idx;

public:
	void		init();
	/*
	 *  @brief   	LoadInfo	 从tb_table_info、tb_field_info 载入表信息、字段信息 
	 *  @param 		[In] nTableID	表数字ID
	 *  @return 	BOOL
	 */
	BOOL		LoadInfo(int nTableID);
	/*
	 *  @brief   	LoadInfo	 从tb_table_info、tb_field_info 载入表信息、字段信息 
	 *  @param 		[In] nTableID	表数字ID
	 *  @return 	BOOL
	 */
	BOOL		LoadInfo(const char* table_name, int nType = 0);
	BOOL		LoadData(const char* sql_stm);
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

	int			GetKeyCount();
	QByteArray	GetKeyValue(int iRow, int idx);
	QByteArray	GetKeyName(int idx, int nNameType = 0);
	int			GetValueRowIndex(QByteArrayMatrix &keyVals);

	int			GetRecordCount();
	int			GetRowCount();

	int			GetNextFieldIndex();

	BOOL		SetFieldValue(int iRow, int iCol, QByteArray val);
	BOOL		SetFieldValue(int iRow, const char *szFieldName, QByteArray val);
	
	BOOL		AddField(const char* fieldName, FIELD_TYPE_ENUM enFieldType, QByteArray initVal);
	
public: // SQL DML DDL
	QByteArray	BuildSQL_UPDATE(int iRow);
	BOOL		ExcuteSQL(const char* sql_stmt, QByteArrayMatrix &content);

};

QMemTablePrivate::QMemTablePrivate()
{
	init();
}

QMemTablePrivate::QMemTablePrivate(int nTableID)
{
	init();
	m_nTableID = nTableID;
}

QMemTablePrivate::QMemTablePrivate(const char* table_name, int nType/* = 0*/)
{
	init();
}

QMemTablePrivate::~QMemTablePrivate()
{
}

void QMemTablePrivate::init()
{
	m_nTableID = -1;

	m_info_table.clear();
	m_info_table_define.clear();
	m_info_field.clear();
	m_info_field_define.clear();
	m_data.clear();
	
	m_info_table.SetDelimRow("$$!");
	m_info_table.SetDelimCol("##!");

	m_info_field.SetDelimRow("$$!");
	m_info_field.SetDelimCol("##!");

	m_data.SetDelimRow("$$!");
	m_data.SetDelimCol("##!");
}

BOOL QMemTablePrivate::LoadInfo(int nTableID)
{
	BOOL bReturn = FALSE;

	CString strSQL;

	strSQL.Format("SELECT*FROM tb_field_info WHERE table_id = %d"
		, 10000);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table_define))
		return bReturn;
	
	strSQL.Format("SELECT*FROM tb_field_info WHERE table_id = %d"
		, 10001);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field_define))
		return bReturn;

	// 载入表信息
	strSQL.Format("SELECT*FROM tb_table_info WHERE table_id = %d"
		, nTableID);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table))
		return bReturn;

	// 载入字段信息
	strSQL.Format("SELECT*FROM tb_field_info WHERE table_id = %d"
		, nTableID);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field))
		return bReturn;
	
	m_nTableID = nTableID;

	return TRUE;
}

BOOL QMemTablePrivate::LoadInfo(const char* table_name, int nNameType/* = 0*/)
{
	BOOL bReturn = FALSE;
	
	CString strSQL;
	
	strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = %d"
		, 10000);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table_define))
		return bReturn;

	strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = %d"
		, 10001);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field_define))
		return bReturn;
	
	switch(nNameType){
	case 0: // english name
		{
			// 载入表信息
			strSQL.Format("SELECT * FROM tb_table_info WHERE table_id = (SELECT table_id FROM tb_table_info WHERE table_name_eng = '%s')"
				, table_name);
			if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table))
				return bReturn;
			
			// 载入字段信息
			strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = (SELECT table_id FROM tb_table_info WHERE table_name_eng = '%s')"
				, table_name);
			if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field))
				return bReturn;
		}
		break;
	case 1:	// chinese name
		{
			// 载入表信息
			strSQL.Format("SELECT * FROM tb_table_info WHERE table_id = (SELECT table_id FROM tb_table_info WHERE table_name_chn = '%s')"
				, table_name);
			if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table))
				return bReturn;
			
			// 载入字段信息
			strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = (SELECT table_id FROM tb_table_info WHERE table_name_chn = '%s')"
				, table_name);
			if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field))
				return bReturn;
		}
	}
	
	m_nTableID = m_info_table.GetFieldValue(1, 1).toInt();
	
	return TRUE;
}

BOOL QMemTablePrivate::LoadData(const char* sql_stm)
{
	BOOL bReturn = FALSE;
	if (NULL == sql_stm)
		return bReturn;
	
	CString strSQL;
	
	strSQL.Format("%s", sql_stm);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_data))
		return bReturn;

	return TRUE;
}

BOOL QMemTablePrivate::LoadDataAll()
{
	BOOL bReturn = FALSE;

	CString strSQL;
	
	strSQL.Format("SELECT * FROM %s", GetTableName().constData());
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_data))
		return bReturn;

	AddField("is_modified", DBI_FIELD_TYPE_INT, QByteArray::number(0));
	
	return TRUE;
}

BOOL QMemTablePrivate::SaveData()
{
	BOOL bReturn = FALSE;

	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	
	char * sError = NULL;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nRowCount = GetRowCount();
	for(int i = 1; i <= nRowCount; i++)
	{
		CMemSet	mem;
		int nResult;

		QByteArrayMatrix val = GetFieldValue(i, "is_modified");
		int is_modified = val.GetFieldValue(1, 1).toInt();
		if (0 == is_modified)
			continue;
		
		CString strSQL = BuildSQL_UPDATE(i).constData();
		//AfxMessageBox(strSQL);
		//continue;
		
		WriteLog(strSQL);
		//AfxMessageBox(strSQL);
		
		//进行查询
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = dbEngine.XJExecuteSql(MutiSql, sError, &mem);
		}
		catch (...)
		{
			str.Format("QMemTablePrivate::SaveData, 更新失败");
			WriteLog(str);
			DELETE_POINTERS(sError);
			//AfxMessageBox(str);
			
			return FALSE;
		}
		
		if(nResult == 1)
		{	
			str.Format("QMemTablePrivate::SaveData, 更新成功");
			WriteLog(str);
			//AfxMessageBox(str);

			// 取消更新标志
			int nColIdx = GetFieldIndex("is_modified");
			m_data.SetFieldValue(i, nColIdx, QByteArray(""));
		}
		else
		{
			str.Format("QMemTablePrivate::SaveData, 更新失败, 原因为%s", sError);
			WriteLog(str);
			//AfxMessageBox(str);
		}
		
		mem.FreeData(true);
	}
	DELETE_POINTERS(sError);
	
	return TRUE;
}

BOOL QMemTablePrivate::ExcuteSQL(const char* sql_stmt, QByteArrayMatrix &content)
{
	BOOL bReturn = FALSE;

	if (NULL == sql_stmt)
		return bReturn;

	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	
	char * sError = NULL;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet	mem;
	int nResult;
	CString strSQL;
	MutiSQL_DATA MutiSql;
	
	// 载入表信息
	strSQL.Format("%s"
		, sql_stmt);
	
	WriteLog(strSQL);
	//AfxMessageBox(strSQL);
	
	//进行查询
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	memset(sError, '\0', MAXMSGLEN);
	
	try
	{
		nResult = dbEngine.XJExecuteSql(MutiSql, sError, &mem);
	}
	catch (...)
	{
		str.Format("QMemTablePrivate::ExcuteSQL, 查询失败");
		WriteLog(str);
		DELETE_POINTERS(sError);
		//AfxMessageBox(str);
		
		return FALSE;
	}
	if(nResult == 1)
	{	
		int nRowCount = mem.GetMemRowNum();
		int nFieldCount = mem.GetMemFieldNum();
		
		str.Format("QMemTablePrivate::ExcuteSQL, 读取到%d条数据", nRowCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		content.clear();
		mem.MoveFirst();
		for (int i = 0; i < nRowCount; i++)
		{
			char * val = NULL;

			if (i != 0)
				content.AppendRowDelim();

			for (int j = 0; j < nFieldCount; j++){
				val = mem.GetValue((UINT)j);
				content.AppendField(val, j==0);
			}
			
			mem.MoveNext();
		}
	}
	else
	{
		str.Format("QMemTablePrivate::ExcuteSQL, 查询失败, 原因为%s", sError);
		WriteLog(str);
		//AfxMessageBox(str);
	}
	mem.FreeData(true);
	
	DELETE_POINTERS(sError);

	return TRUE;
}

int QMemTablePrivate::GetTableID()
{
	return m_nTableID;
}

QByteArray QMemTablePrivate::GetTableName(int nNameType/* = 0*/)
{
	QByteArray sReturn;
	
	switch (nNameType){
	case 0:
		return m_info_table.GetFieldValue(1, 2);
		break;
	case 1:
		return m_info_table.GetFieldValue(1, 3);
		break;
	}
	
	return sReturn;
}

int	QMemTablePrivate::GetFieldCount()
{
	return m_info_field.GetRows();
}

QByteArray QMemTablePrivate::GetFieldName(int fieldID, int nType)
{
	QByteArray sReturn;
	
	switch (nType){
	case 0:	// english name
		return m_info_field.GetFieldValue(fieldID, 3);
		break;
	case 1:	// chinese name
		return m_info_field.GetFieldValue(fieldID, 4);
		break;
	}
	
	return sReturn;
}

int QMemTablePrivate::GetFieldIndex(const char *szFieldName, int nNameType/* = 0*/)
{
	int nReturn = -1;

	if (!szFieldName || QByteArray(szFieldName).isEmpty())
		return nReturn;

	int nRows = m_info_field.GetRows();
	int nColIdx = 2;
	int nNameIdx = 3;
	if (nNameType != 0)
		nNameIdx = 4;

	for (int i = 1; i <= nRows; i++){
		if (m_info_field.GetFieldValue(i, nNameIdx) != szFieldName)
				continue;

		return m_info_field.GetFieldValue(i, nColIdx).toInt();
	}
	
	return nReturn;
}

int	QMemTablePrivate::GetFieldDataType(int fieldID)
{
	return m_info_field.GetFieldValue(fieldID, 5).toInt();
}

QByteArray QMemTablePrivate::GetFieldValue(int iRow, int iCol)
{
	return m_data.GetFieldValue(iRow, iCol);
}

QByteArray QMemTablePrivate::GetFieldValue(int iRow, const char *szFieldName, int nNameType)
{
	int iCol = GetFieldIndex(szFieldName, nNameType);
	return m_data.GetFieldValue(iRow, iCol);
}

int QMemTablePrivate::GetKeyCount()
{
	int nReturn = 0;
	
	m_key_col_idx.clear();
	m_key_row_idx.clear();
	
	int nKeyIdx = 7;
	int nFieldID = 2;
	
	int nFieldCount = GetFieldCount();
	for (int i = 1; i <= nFieldCount; i++){
		int is_key = m_info_field.GetFieldValue(i, nKeyIdx).toInt();
		if (is_key == 1){
			nReturn ++;
			int nColIdx = m_info_field.GetFieldValue(i, nFieldID).toInt();
			m_key_col_idx.push_back(nColIdx);
			m_key_row_idx.push_back(i);
		}
	}
	
	return nReturn;
}

QByteArray QMemTablePrivate::GetKeyValue(int iRow, int idx)
{
	QByteArray s;

	GetKeyCount();
	if (idx > m_key_col_idx.size() || idx < 1)
		return s;

	int nFieldIdx = m_key_col_idx[idx - 1];

	return GetFieldValue(iRow, nFieldIdx);
}

QByteArray QMemTablePrivate::GetKeyName(int idx, int nNameType/* = 0*/)
{
	QByteArray s;

	int iCol = 3;
	if (nNameType != 0)
		iCol = 4;
	
	GetKeyCount();
	if (idx > m_key_row_idx.size() || idx < 1)
		return s;
	
	int nRowIdx = m_key_row_idx[idx - 1];

	return m_info_field.GetFieldValue(nRowIdx, iCol);
}

int	QMemTablePrivate::GetValueRowIndex(QByteArrayMatrix &keyVals)
{
	int nReturn = -1;
	
	int nRows = GetRowCount();
	int nKeyCount = GetKeyCount();
	
	for (int i = 1; i <= nRows; i++){
		bool bFound = true;
		if (nKeyCount != keyVals.GetFieldCount()){
			bFound = false;
			continue;
		}
		
		for (int j = 1; j <= nKeyCount; j++){
			if (GetKeyValue(i, j) != keyVals.GetFieldValue(1, j)){
				bFound = false;
				break;
			}
		}
		
		if (bFound)
			return i;
	}
	
	return nReturn;
}

int	QMemTablePrivate::GetRecordCount()
{
	return m_data.GetRows();
}

int	QMemTablePrivate::GetRowCount()
{
	return m_data.GetRows();
}

int	QMemTablePrivate::GetNextFieldIndex()
{
	int nNextIdx = -1;

	int nRows = m_info_field.GetRows();
	for (int i = 1; i <= nRows; i++)
	{
		int nFieldIdx = m_info_field.GetFieldValue(i, 2).toInt();
		if (nNextIdx < nFieldIdx)
			nNextIdx = nFieldIdx;
	}

	return nNextIdx + 1;
}

BOOL QMemTablePrivate::SetFieldValue(int iRow, int iCol, QByteArray val)
{
	if (val != GetFieldValue(iRow, iCol)){
		int nColIdx = GetFieldIndex("is_modified");
		QByteArrayMatrix val = GetFieldValue(iRow, nColIdx);
		QByteArray fldsChangeCol = val.GetRowData(2);
		if (!fldsChangeCol.contains(QByteArray::number(iCol))){
			if (!fldsChangeCol.isEmpty())
				fldsChangeCol << ",";
			fldsChangeCol << iCol;
		}
		QByteArrayMatrix newVal;
		newVal << 1 << ";" << fldsChangeCol;
		m_data.SetFieldValue(iRow, nColIdx, newVal);
// 		SetFieldValue(iRow, "is_modified", QByteArray::number(1));
	}
	m_data.SetFieldValue(iRow, iCol, val);
	return TRUE;
}

BOOL QMemTablePrivate::SetFieldValue(int iRow, const char *szFieldName, QByteArray val)
{
	int iCol = GetFieldIndex(szFieldName);
	if (-1 == iCol)
		return FALSE;

	if (val != GetFieldValue(iRow, iCol)){
		int nColIdx = GetFieldIndex("is_modified");
		QByteArrayMatrix val = GetFieldValue(iRow, nColIdx);
		QByteArray fldsChangeCol = val.GetRowData(2);
		if (!fldsChangeCol.contains(QByteArray::number(iCol))){
			if (!fldsChangeCol.isEmpty())
				fldsChangeCol << ",";
			fldsChangeCol << iCol;
		}
		QByteArrayMatrix newVal;
		newVal << 1 << ";" << fldsChangeCol;
		
		m_data.SetFieldValue(iRow, nColIdx, newVal);
// 		SetFieldValue(iRow, "is_modified", QByteArray::number(1));
	}
 	m_data.SetFieldValue(iRow, iCol, val);
	return TRUE;
}

BOOL QMemTablePrivate::AddField(const char* fieldName, FIELD_TYPE_ENUM enFieldType, QByteArray initVal)
{
	BOOL bReturn = FALSE;

	// 10005##!1##!user_id##!用户编号##!2##!16##!1##!0##!0##!1##!0##!1##!0##!100##!2##!2##!##!0##!0##!##!##!##!0##!0$$!
	//char szLine[256] = {0};
	//sprintf("%");
	QByteArray line;
	//line.append(QByteArray::number(m_nTableID)).append();

	line << m_nTableID << m_info_field.GetDelimCol()
		<< GetNextFieldIndex() << m_info_field.GetDelimCol()
		<< fieldName;
	m_info_field.AppendRow(line);
	
	m_data.expandMatrix(1, initVal);

	return bReturn;
}

QByteArray QMemTablePrivate::BuildSQL_UPDATE(int iRow)
{
	QByteArray baSQL;
	
	QByteArrayMatrix val = GetFieldValue(iRow, "is_modified");
	if (val.GetFieldValue(1, 1).toInt() != 1)
		return baSQL;

	baSQL << "UPDATE " << GetTableName();
	baSQL << " SET ";

	int nFieldChangedCount = val.GetCols(2);
	for (int i = 1; i <= nFieldChangedCount; i++){
		int nCol = val.GetFieldValue(2, i).toInt();
		if (0 == nCol)
			continue;

		if (i != 1)
			baSQL << ",";

		baSQL << GetFieldName(nCol) << " = '" << GetFieldValue(iRow, nCol) << "'";
	}
	baSQL << " ";

	int nKeyCount = GetKeyCount();
	if (nKeyCount > 0)
		baSQL << "WHERE ";
	for (int i = 1; i <= nKeyCount; i++){
		if (i != 1)
			baSQL << " AND ";

		baSQL << GetKeyName(i) << " = '" << GetKeyValue(iRow, i) << "'";
	}

	return baSQL;
}

////////////////////////////////////////////////////////////
// QMemTable
// 

QMemTable::QMemTable()
	: d_ptr(new QMemTablePrivate)
{
}

QMemTable::QMemTable(int nTableID)
	: d_ptr(new QMemTablePrivate(nTableID))
{
}

QMemTable::QMemTable(const char* table_name, int nType/* = 0*/)
	: d_ptr(new QMemTablePrivate(table_name, nType))
{
}

QMemTable::~QMemTable()
{
	DELETE_POINTER(d_ptr);
}

QByteArray QMemTable::FWrite(const char *pszFilePath)
{
	QByteArrayMatrix &info_table = d_ptr->m_info_table;
	QByteArrayMatrix &info_field = d_ptr->m_info_field;
	
	QByteArrayMatrix &info_table_define = d_ptr->m_info_table_define;
	QByteArrayMatrix &info_field_define = d_ptr->m_info_field_define;

	QByteArrayMatrix &data = d_ptr->m_data;

	QByteArray out;
	
	int i, j;
	Q_UNUSED(j);
	out << "[ Time ]: " << GetTime() << "\n";
	
	out << "\n--------------[ Table Info  ]--------------\n\n"
		<< "[ Table Definition  ] : " << "\n"
		<< d_ptr->m_info_table_define << "\n\n"
		<< "[ Table Data  ] : " << "\n"
		<< d_ptr->m_info_table << "\n\n"
		<< "[ Table ID    ] : " << GetTableID() << "\n"
		<< "[ Table Name  ] : " << GetTableName() << "\n";
		
	out << "\n--------------[ Table Field ]--------------\n\n"
		<< "[ Field Definition  ] : " << "\n"
		<< d_ptr->m_info_field_define << "\n\n"
		<< "[ Field Data  ] : " << "\n"
		<< d_ptr->m_info_field << "\n\n"
		<< "[ Field Count ] : " << GetFieldCount() << "\n"
		<< "[ Field Names ] : " << "\n";
	out << d_ptr->m_info_field.GetCol(3).FWrite();

	out << "\n--------------[ Table Keys ]--------------\n\n"
		<< "[ Key Count ] : " << GetKeyCount() << "\n"
		<< "[ Key Name  ] : " << "\n";

	for (i = 1; i <= GetKeyCount(); i++){
		if( i != 1)
			out << "  |  ";
		else
			out << "|  ";

		out << GetKeyName(i);
	}
	out << "  |\n";

	out << "The 3rd Row Key Values: \n";
	for (i = 1; i <= GetKeyCount(); i++){
		if( i != 1)
			out << "  |  ";
		else
			out << "|  ";
		
		out << GetKeyValue(3, i);
	}
	out << "  |\n";

	
	out << "\n--------------[ Table Data  ]--------------\n\n"
		<< d_ptr->m_data.FWrite() << "\n";
	
	if (NULL == pszFilePath || QByteArray(pszFilePath).isEmpty()){
		cout << out.constData() << endl;
	}else{
		if (NULL == pszFilePath)
			return out;
		
		fstream fs;
		fs.open(pszFilePath, std::fstream::out/* | std::fstream::app*/);
		//FWrite(fs);
				
		fs.write(out.data(), out.count());
	}
	
	return out;
}

BOOL QMemTable::LoadInfo(int nTableID)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadInfo(nTableID);
}

BOOL QMemTable::LoadInfo(const char* table_name, int nNameType/* = 0*/)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadInfo(table_name, nNameType);
}

BOOL QMemTable::LoadData(const char* sql_stm)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadData(sql_stm);
}

BOOL QMemTable::LoadDataAll()
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadDataAll();
}

BOOL QMemTable::SaveData()
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;
	
	return d_ptr->SaveData();
}

int QMemTable::GetTableID()
{
	int nReturn = -1;
	if (NULL == d_ptr)
		return nReturn;

	return d_ptr->GetTableID();
}

QByteArray QMemTable::GetTableName(int nNameType/* = 0*/)
{
	QByteArray sReturn;
	if (NULL == d_ptr)
		return sReturn;
	
	return d_ptr->GetTableName(nNameType);
}

int	QMemTable::GetFieldCount()
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetFieldCount();
}

QByteArray QMemTable::GetFieldName(int fieldID, int nType)
{
	QByteArray sReturn;

	if (NULL == d_ptr)
		return sReturn;

	return d_ptr->GetFieldName(fieldID, nType);
}

int QMemTable::GetFieldIndex(const char *szFieldName, int nNameType)
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetFieldIndex(szFieldName, nNameType);
}

int	QMemTable::GetFieldDataType(int fieldID)
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetFieldDataType(fieldID);
}

QByteArray QMemTable::GetFieldValue(int iRow, int iCol)
{
	QByteArray sReturn;
	
	if (NULL == d_ptr)
		return sReturn;
	
	return d_ptr->GetFieldValue(iRow, iCol);
}

QByteArray QMemTable::GetFieldValue(int iRow, const char *szFieldName, int nNameType)
{
	QByteArray sReturn;
	
	if (NULL == d_ptr)
		return sReturn;

	return d_ptr->GetFieldValue(iRow, szFieldName, nNameType);
}

int QMemTable::GetKeyCount()
{
	int nReturn = 0;

	d_ptr->m_key_col_idx.clear();

	int nKeyIdx = 7;
	int nFieldID = 2;

	int nFieldCount = GetFieldCount();
	for (int i = 1; i <= nFieldCount; i++){
		int is_key = d_ptr->m_info_field.GetFieldValue(i, nKeyIdx).toInt();
		if (is_key == 1){
			nReturn ++;
			int nColIdx = d_ptr->m_info_field.GetFieldValue(i, nFieldID).toInt();
			d_ptr->m_key_col_idx.push_back(nColIdx);
		}
	}

	return nReturn;
}

QByteArray QMemTable::GetKeyValue(int iRow, int idx)
{
	QByteArray s;
	if (NULL == d_ptr)
		return s;

	return d_ptr->GetKeyValue(iRow, idx);
}

QByteArray QMemTable::GetKeyName(int idx)
{
	QByteArray s;
	if (NULL == d_ptr)
		return s;

	return d_ptr->GetKeyName(idx);
}

int	QMemTable::GetValueRowIndex(QByteArrayMatrix &keyVals)
{	
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetValueRowIndex(keyVals);
}

int	QMemTable::GetRecordCount()
{
	if (NULL == d_ptr)
		return -1;
	
	return d_ptr->GetRecordCount();
}

int	QMemTable::GetRowCount()
{
	if (NULL == d_ptr)
		return -1;
	
	return d_ptr->GetRowCount();
}

BOOL QMemTable::SetFieldValue(int iRow, int iCol, QByteArray val)
{
	if (NULL == d_ptr)
		return FALSE;

	return d_ptr->SetFieldValue(iRow, iCol, val);
}

BOOL QMemTable::SetFieldValue(int iRow, const char *szFieldName, QByteArray val)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->SetFieldValue(iRow, szFieldName, val);
}