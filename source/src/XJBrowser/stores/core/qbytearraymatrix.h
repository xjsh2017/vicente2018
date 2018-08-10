#ifndef _QBYTEARRAYMATRIX_H
#define _QBYTEARRAYMATRIX_H

#include "qbytearray.h"

class QByteArrayMatrix : public QByteArray
{
public:
	QByteArrayMatrix();
	QByteArrayMatrix(const char *);
	QByteArrayMatrix(const char *, const char *delim_row, const char *delim_col);
	
	virtual void resizeMatrix(int iRows, int iCols, QByteArray val);
	virtual void expandMatrix(int iAddCols, QByteArray val);
	virtual void expandRow(int iRow, int iTotalCols, QByteArray val);

	virtual int FRead(char *pszLine, size_t s = -1)
	{
		clear();
		this->append(pszLine);
		
		return 0;
	}
	virtual QByteArray FWrite(const char* pszFilePath = NULL);
	virtual int AppendRow(const char *pszLine)
	{
		if (GetRows() > 0) 
			this->append(m_delim_row);
		this->append(pszLine);

		return GetRows();
	}
	virtual int AppendRow(const QByteArray &s)
	{
		if (GetRows() > 0) 
			this->append(m_delim_row);
		this->append(s);
		
		return GetRows();
	}
	virtual int AppendRow(const QByteArrayMatrix &s)
	{
		if (GetRows() > 0) 
			this->append(m_delim_row);
		this->append(s);
		
		return GetRows();
	}
	virtual int AppendField(const char *pszLine, bool bFirst = false)
	{
		if (!bFirst)
			this->append(m_delim_col);
		this->append(pszLine);
		
		return 0;
	}
	void	AppendRowDelim(){
		this->append(m_delim_row);
	}
	void	AppendColDelim(){
		this->append(m_delim_col);
	}
	
	int					GetRows();
	int					GetRowCount() { return GetRows(); }
	int					GetRecordCount() { return GetRows(); }
	QByteArray			GetRowData(int iRow);
	QByteArrayMatrix	GetRow(int iRow);
	QByteArrayMatrix	GetRecord(int iRow) { return GetRow(iRow); }

	int					GetCols(int iRow = -1);
	int					GetColCount(int iRow = -1) { return GetCols(iRow); }
	int					GetFieldCount() { return GetCols(-1); }
	QByteArray			GetColData(int iCol);
	QByteArrayMatrix	GetCol(int iCol);
	QByteArrayMatrix	GetField(int iCol);
	QByteArray			GetFieldValue(int iRow, int iCol);
	
	// depreciated:
	QByteArray			GetValue(int iRow, int iCol) { return GetFieldValue(iRow, iCol); }

	void				GetRowBytePos(int iRow, int &from, int &end);
	void				GetBytePos(int iRow, int iCol, int &from, int &end);

	void				SetFieldValue(int iRow, int iCol, const char *val);
	void				SetFieldValue(int iRow, int iCol, const QByteArray &s);
	
	const QByteArray&	GetDelimRow() const { return m_delim_row; }
	const QByteArray&	GetDelimCol() const { return m_delim_col; }
	void				SetDelimRow(QByteArray delim) { m_delim_row = delim; }
	void				SetDelimCol(QByteArray delim) { m_delim_col = delim; }

	int					valueIndexOf(QByteArray &s, int from = 0);
	int					valueIndexOf(const char *pszVal, int from = 0);
	int					valueIndexOf(int numVal, int from = 0);

	int					valueCount(QByteArray &s);
	int					valueCount(const char *pszVal);
	int					valueCount(int numVal);

	bool				valueContains(QByteArray &s);
	bool				valueContains(const char *pszVal);
	bool				valueContains(int numVal);

private:
	QByteArray	m_delim_row;
	QByteArray	m_delim_col;
	int			m_from;
	int			m_end;
};

#endif // _QBYTEARRAYMATRIX_H
