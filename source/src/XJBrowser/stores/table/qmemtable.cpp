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
	QMemTablePrivate(const char* table_name, int nNameType = XJ::LANG_ENG);
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
	 *  @param 		[In] table_name	表名 
	 *  @param 		[In] nNameType	表名类型：英文、中文、法文、德文等
	 *  @return 	BOOL
	 */
	BOOL		LoadInfo(const char* table_name, int nNameType = XJ::LANG_ENG, bool bForceReLoad = false);
	BOOL		LoadData(const char* sql_stm);
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

	int			GetKeyCount();
	QByteArray	GetKeyValue(int iRow, int idx);
	QByteArray	GetKeyName(int idx, int nNameType = XJ::LANG_ENG);
	int			GetKeyIndex(int idx);
	int			GetRowIndex(QByteArrayMatrix &keyVals);

	int			GetRecordCount();
	int			GetRowCount();

	int			GetNextFieldIndex();

	BOOL		SetFieldValue(int iRow, int iCol, QByteArray val);
	BOOL		SetFieldValue(int iRow, const char *szFieldName, QByteArray val);
	
	BOOL		HasField(const char* fieldName, int nNameType = XJ::LANG_ENG);
	BOOL		HasField(const QByteArray &fieldName, int nNameType = XJ::LANG_ENG);
	BOOL		AddField(const char* fieldName, FIELD_TYPE_ENUM enFieldType, QByteArray initVal);

	int			AddRowData(QByteArrayMatrix keyVals);
	
public: // SQL DML DDL
	QByteArray	BuildSQL_UPDATE(int iRow);
	QByteArray	BuildSQL_INSERT(int iRow);
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

QMemTablePrivate::QMemTablePrivate(const char* table_name, int nNameType/* = XJ::LANG_ENG*/)
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

	m_info_table_define.SetDelimRow("$$!");
	m_info_table_define.SetDelimCol("##!");

	m_info_field.SetDelimRow("$$!");
	m_info_field.SetDelimCol("##!");
	
	m_info_field_define.SetDelimRow("$$!");
	m_info_field_define.SetDelimCol("##!");

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

BOOL QMemTablePrivate::LoadInfo(const char* table_name, int nNameType/* = XJ::LANG_ENG*/, bool bForceReLoad/* = false*/)
{
	BOOL bReturn = FALSE;

	if (!table_name || QByteArray(table_name).trimmed().isEmpty())
		return FALSE;
	
	bool bReLoad = false;
	if (qstrcmp(table_name,GetTableName().constData()) != 0)
		bReLoad = true;	// 表名不一致，重载表信息

	if (bForceReLoad)
		bReLoad = true;

	if (!bReLoad)
		return FALSE;
	
	CString strSQL;

// 	QByteArray msg;
// 	msg << "reload table: " << table_name;
// 	AfxMessageBox(msg.constData());
	
	strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = %d"
		, 10000);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_table_define))
		return bReturn;

	strSQL.Format("SELECT * FROM tb_field_info WHERE table_id = %d"
		, 10001);
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_info_field_define))
		return bReturn;
	
	switch(nNameType){
	case XJ::LANG_ENG: // english name
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
	case XJ::LANG_CHS:	// chinese name
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

	if (!HasField("is_modified"))
		AddField("is_modified", DBI_FIELD_TYPE_INT, QByteArray::number(0));

	return TRUE;
}

BOOL QMemTablePrivate::LoadDataAll()
{
	BOOL bReturn = FALSE;

	CString strSQL;
	
	strSQL.Format("SELECT * FROM %s", GetTableName().constData());
	if (!ExcuteSQL(strSQL.GetBuffer(0), m_data))
		return bReturn;

	if (!HasField("is_modified"))
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

		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));

		CString strSQL;
		if (1 == is_modified){
			//AfxMessageBox("UPDATE ..");
			strSQL = BuildSQL_UPDATE(i).constData();
			MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		}else if(2 == is_modified){
			//AfxMessageBox("Insert .. ");
			strSQL = BuildSQL_INSERT(i).constData();
			MutiSql.Funtype = EX_STTP_FUN_TYPE_ADD;
		}
		
		//AfxMessageBox(strSQL);
		//continue;
		WriteLog(strSQL);
		
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		memset(sError, '\0', MAXMSGLEN);
 		
// 		QByteArray cc(GetTime());
// 		cc << "\n"
// 			<< strSQL.GetBuffer(0)
// 			<< "\n\n";
// 		fstream fs;
// 		fs.open("c:/sql.txt", std::fstream::out | std::fstream::app);
// 		//FWrite(fs);
// 		fs.write(cc.constData(), cc.count());

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
			m_data.SetFieldValue(i, nColIdx, QByteArray("0"));
		}
		else
		{
			str.Format("QMemTablePrivate::SaveData, 更新失败, 原因为: %s", sError);
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
	
	//WriteLog(strSQL);
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
		//WriteLog(str, XJ_LOG_LV3);
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

QByteArray QMemTablePrivate::GetTableName(int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray sReturn;
	
	switch (nNameType){
	case XJ::LANG_ENG:
		return m_info_table.GetFieldValue(1, 2);
		break;
	case XJ::LANG_CHS:
		return m_info_table.GetFieldValue(1, 3);
		break;
	}
	
	return sReturn;
}

int	QMemTablePrivate::GetFieldCount()
{
	return m_info_field.GetRows();
}

QByteArray QMemTablePrivate::GetFieldName(int iCol, int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray sReturn;
	
	switch (nNameType){
	case XJ::LANG_ENG:
		return m_info_field.GetFieldValue(iCol, 3);
		break;
	case XJ::LANG_CHS:
		return m_info_field.GetFieldValue(iCol, 4);
		break;
	}
	
	return sReturn;
}

int QMemTablePrivate::GetFieldIndex(const char *szFieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	int nReturn = -1;

	if (!szFieldName || QByteArray(szFieldName).isEmpty())
		return nReturn;

	int nRows = m_info_field.GetRows();
	int nColIdx = 2;
	
	int nNameIdx = -1;
	switch (nNameType){
	case XJ::LANG_ENG:
		nNameIdx = 3;
		break;
	case XJ::LANG_CHS:
		nNameIdx = 4;
		break;
	default:
		nNameIdx = 3;
	}

	for (int i = 1; i <= nRows; i++){
		if (m_info_field.GetFieldValue(i, nNameIdx) != szFieldName)
			continue;

		return m_info_field.GetFieldValue(i, nColIdx).toInt();
	}
	
	return nReturn;
}

int	QMemTablePrivate::GetFieldDataType(int iCol)
{
	return m_info_field.GetFieldValue(iCol, 5).toInt();
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

QByteArray QMemTablePrivate::GetKeyName(int idx, int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray s;

	int iCol = -1;
	switch (nNameType){
	case XJ::LANG_ENG:
		iCol = 3;
		break;
	case XJ::LANG_CHS:
		iCol = 4;
		break;
	default:
		iCol = 3;
	}
	
	GetKeyCount();
	if (idx > m_key_row_idx.size() || idx < 1)
		return s;
	
	int nRowIdx = m_key_row_idx[idx - 1];

	return m_info_field.GetFieldValue(nRowIdx, iCol);
}

int QMemTablePrivate::GetKeyIndex(int idx)
{
	BOOL nReturn = -1;
	if (idx < 1)
		return nReturn;

	return GetFieldIndex(GetKeyName(idx).constData());
}

int	QMemTablePrivate::GetRowIndex(QByteArrayMatrix &keyVals)
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
		QByteArrayMatrix tmp = GetFieldValue(iRow, nColIdx);
		int nFlag = tmp.GetRowData(1).toInt();
		if (nFlag == 0)
			nFlag = 1;
		QByteArray fldsChangeCol = tmp.GetRowData(2);
		if (!fldsChangeCol.contains(QByteArray::number(iCol))){
			if (!fldsChangeCol.isEmpty())
				fldsChangeCol << ",";
			fldsChangeCol << iCol;
		}
		QByteArrayMatrix newVal;
		newVal << nFlag << ";" << fldsChangeCol;
		m_data.SetFieldValue(iRow, nColIdx, newVal);
	}
	m_data.SetFieldValue(iRow, iCol, val);

	return TRUE;
}

BOOL QMemTablePrivate::SetFieldValue(int iRow, const char *szFieldName, QByteArray val)
{
	int iCol = GetFieldIndex(szFieldName);
	if (-1 == iCol)
		return FALSE;
 	
	return SetFieldValue(iRow, iCol, val);
}

BOOL QMemTablePrivate::HasField(const char* fieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	BOOL bReturn = TRUE;

	if (-1 == GetFieldIndex(fieldName, nNameType))
		return FALSE;

	return bReturn;
}

BOOL QMemTablePrivate::HasField(const QByteArray &fieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	return HasField(fieldName.constData(), nNameType);
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
		<< fieldName << m_info_field.GetDelimCol()
		<< fieldName;
	m_info_field.AppendRow(line);
	
	m_data.expandMatrix(1, initVal);

	return bReturn;
}

int QMemTablePrivate::AddRowData(QByteArrayMatrix keyVals)
{
	int nReturn = -1;

	nReturn = GetRowIndex(keyVals);
	if (GetRowIndex(keyVals) > 0)
		return nReturn;

	int nKeyCount = GetKeyCount();
	int nIdx_From = 1;
	QByteArrayMatrix val;
	for (int i = 1; i <= nKeyCount; i++){
		int nKeyIdx = GetKeyIndex(i);
		for (int j = nIdx_From; j < nKeyIdx; j++){
			if (j != 1)
				val << m_data.GetDelimCol();
			val << "";
		}
		if (nKeyIdx != 1)
			val << m_data.GetDelimCol();
		val << keyVals.GetFieldValue(1, i);

		nIdx_From = nKeyIdx + 1;
	}

	int nAddRowIdx = m_data.AppendRow(val);
	int nColIdx = GetFieldIndex("is_modified");
	m_data.SetFieldValue(nAddRowIdx, nColIdx, QByteArray("2"));
	
	return nAddRowIdx;
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

QByteArray QMemTablePrivate::BuildSQL_INSERT(int iRow)
{
	// INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
	QByteArray baSQL;
	
	QByteArrayMatrix val = GetFieldValue(iRow, "is_modified");
	if (val.GetFieldValue(1, 1).toInt() != 2)
		return baSQL;
	
	baSQL << "INSERT INTO " << GetTableName();
	
	baSQL << " ( ";
	int nKeyCount = GetKeyCount();
	for (int i = 1; i <= nKeyCount; i++){
		if (i != 1)
			baSQL << ",";
		
		baSQL << GetKeyName(i);
	}
	if (nKeyCount > 0)
		baSQL << ",";

	int nFieldChangedCount = val.GetCols(2);
	for (int i = 1; i <= nFieldChangedCount; i++){
		int nCol = val.GetFieldValue(2, i).toInt();
		if (0 == nCol)
			continue;
		
		if (i != 1)
			baSQL << ",";
		
		baSQL << GetFieldName(nCol);
	}
	baSQL << " ) ";

	baSQL << " VALUES ( ";
	for (int i = 1; i <= nKeyCount; i++){
		if (i != 1)
			baSQL << ",";
		
		baSQL << "'" << GetKeyValue(iRow, i) << "'";
	}
	if (nKeyCount > 0)
		baSQL << ",";
	for (int i = 1; i <= nFieldChangedCount; i++){
		int nCol = val.GetFieldValue(2, i).toInt();
		if (0 == nCol)
			continue;
		
		if (i != 1)
			baSQL << ",";
		
		baSQL << "'" << GetFieldValue(iRow, nCol) << "'";
	}
	baSQL << " ) ";
	
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

QMemTable::QMemTable(const char* table_name, int nNameType/* = XJ::LANG_ENG*/)
	: d_ptr(new QMemTablePrivate(table_name, nNameType))
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
 		<< "Record Count: " << d_ptr->m_data.GetRowCount() << "\n\n"
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

BOOL QMemTable::LoadInfo(const char* table_name, int nNameType/* = XJ::LANG_ENG*/)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadInfo(table_name, nNameType);
}

BOOL QMemTable::LoadData(const char* sql_stmt)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->LoadData(sql_stmt);
}

BOOL QMemTable::LoadData(QByteArray &sql_stmt)
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;
	
	return d_ptr->LoadData(sql_stmt.constData());
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

QByteArray QMemTable::GetTableName(int nNameType/* = XJ::LANG_ENG*/)
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

QByteArray QMemTable::GetFieldName(int iCol, int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray sReturn;

	if (NULL == d_ptr)
		return sReturn;

	return d_ptr->GetFieldName(iCol, nNameType);
}

int QMemTable::GetFieldIndex(const char *szFieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetFieldIndex(szFieldName, nNameType);
}

int	QMemTable::GetFieldDataType(int iCol)
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->GetFieldDataType(iCol);
}

QByteArray QMemTable::GetFieldValue(int iRow, int iCol)
{
	QByteArray sReturn;
	
	if (NULL == d_ptr)
		return sReturn;
	
	return d_ptr->GetFieldValue(iRow, iCol);
}

QByteArray QMemTable::GetFieldValue(int iRow, const char *szFieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray sReturn;
	
	if (NULL == d_ptr)
		return sReturn;

	return d_ptr->GetFieldValue(iRow, szFieldName, nNameType);
}

QByteArray QMemTable::GetFieldValue(QByteArrayMatrix keyVals, int iCol)
{
	QByteArray s;
	int iRow = GetRowIndex(keyVals);
	if (iRow < 1 || iRow > GetRowCount())
		return s;

	return GetFieldValue(iRow, iCol);
}

QByteArray QMemTable::GetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	QByteArray s;
	int iRow = GetRowIndex(keyVals);
	if (iRow < 1 || iRow > GetRowCount())
		return s;
	
	return GetFieldValue(iRow, szFieldName, nNameType);
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

int	QMemTable::GetRowIndex(QByteArrayMatrix &keyVals)
{	
	if (NULL == d_ptr)
		return -1;
	
	return d_ptr->GetRowIndex(keyVals);
}

int	QMemTable::GetRowIndex(const char* keyVals)
{	
	if (NULL == d_ptr)
		return -1;
	
	return d_ptr->GetRowIndex(QByteArrayMatrix(keyVals));
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

BOOL QMemTable::SetFieldValue(QByteArrayMatrix keyVals, const char *szFieldName, QByteArray val)
{
	if (NULL == d_ptr)
		return FALSE;

	int iRow = GetRowIndex(keyVals);
	SetFieldValue(iRow, szFieldName, val);
}

BOOL QMemTable::SetFieldValue(const char* keyVals, const char *szFieldName, QByteArray val)
{
	if (NULL == d_ptr)
		return FALSE;
	
	int iRow = GetRowIndex(keyVals);
	SetFieldValue(iRow, szFieldName, val);
}

BOOL QMemTable::HasField(const char* fieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->HasField(fieldName, nNameType);
}

BOOL QMemTable::HasField(const QByteArray &fieldName, int nNameType/* = XJ::LANG_ENG*/)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->HasField(fieldName.constData(), nNameType);
}

BOOL QMemTable::AddField(const char* fieldName, FIELD_TYPE_ENUM enFieldType, QByteArray initVal)
{
	if (NULL == d_ptr)
		return FALSE;
	
	return d_ptr->AddField(fieldName, enFieldType, initVal);
}

int QMemTable::AddRow(QByteArrayMatrix keyVals)
{
	if (NULL == d_ptr)
		return -1;

	return d_ptr->AddRowData(keyVals);
}