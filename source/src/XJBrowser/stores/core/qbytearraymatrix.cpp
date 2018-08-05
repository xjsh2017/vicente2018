// qbytearraymatrix.cpp : implementation file
//

#include "stdafx.h"
#include "qbytearraymatrix.h"

#include "XJStoreDefine.h"


/////////////////////////////////////////////////////////////
// QByteArrayMatrix
//
QByteArrayMatrix::QByteArrayMatrix()
	: m_delim_row(";"), m_delim_col(",")
{

}

QByteArrayMatrix::QByteArrayMatrix(const char *pszIn)
	: QByteArray(pszIn), m_delim_row(";"), m_delim_col(",")
{

}

QByteArrayMatrix::QByteArrayMatrix(const char *pszIn, const char *delim_row, const char *delim_col)
	: QByteArray(pszIn), m_delim_row(delim_row), m_delim_col(delim_col)
{

}

void QByteArrayMatrix::resizeMatrix(int iRows, int iCols, QByteArray val)
{

}

void QByteArrayMatrix::expandMatrix(int iAddCols, QByteArray val)
{
	int nRows = GetRowCount();
	int nCols = GetCols();

	if (iAddCols <1)
		return;

	QByteArray tmp;
	for (int i = 1; i <= nRows; i++){
		int nRowCols = GetCols(i);

		tmp = val;
		for (int j = nRowCols; j < nCols + iAddCols - 1; j++)
			tmp.prepend(m_delim_col);

		if (nCols > 0)
			tmp.prepend(m_delim_col);

		QByteArray varLastCol = GetFieldValue(i, nCols);
		tmp.prepend(varLastCol);

		SetFieldValue(i, nCols, tmp);
	}
}

QByteArray QByteArrayMatrix::FWrite(const char *pszFilePath)
{
	QByteArray out;

#define endl "\n"

	int i,j;
	out << "[ Time ]: " << GetTime() << endl
		<< "----------------------------" << endl
		<< "| Data     | [" << constData() << "]" << endl
		<< "| Len      |  " << count() << endl
		<< "| Rows     |  " << GetRows() << endl
		<< "| Cols     |  " << GetCols(0);

	if (GetCols(-1) == GetCols(0))
		out << endl;
	else
		out << " ~ " << GetCols(-1) << endl;
	out << "----------------------------" << endl;
	
	for (i = 1; i <= GetRowCount(); i++){
		if (i != 1)
			out << "  |" << endl;
		
		for (j = 1; j <= GetFieldCount(); j++){
			if (j != 1)
				out << "  |  ";
			else
				out << "|  ";
			
			out << GetFieldValue(i, j);
		}
	}
	out << "  |" << endl;
	out << "----------------------------" << endl;
	
	out << endl;

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

int QByteArrayMatrix::GetRows()
{
	if (isEmpty())
		return 0;

	return this->count(m_delim_row) + 1;
}

int QByteArrayMatrix::GetCols(int iRow/* = -1*/)
{
	if (isEmpty())
		return 0;
	
	int nReturn = 0;

	if (iRow > 0){ // 获取某一行的实际字段个数
		QByteArray rowData = GetRowData(iRow);

		if (rowData.isEmpty())
			return 0;

		return rowData.count(m_delim_col) + 1;
	}

	if (iRow == -1){ // 行字段个数最多的字段数
		for (int i = 1; i <= GetRowCount(); i++)
			nReturn = qMax(nReturn, GetCols(i));
		
		return nReturn;
	}else if (iRow == 0){ // 行字段个数最少的字段数
		nReturn = GetCols(-1);
		for (int i = 1; i <= GetRowCount(); i++)
			nReturn = qMin(nReturn, GetCols(i));
		
		return nReturn;
	}

	return nReturn;
}

QByteArrayMatrix QByteArrayMatrix::GetRow(int iRow)
{
	QByteArray row_data = GetRowData(iRow);

	return QByteArrayMatrix(row_data.constData(), m_delim_row.constData(), m_delim_col.constData());
}

QByteArray QByteArrayMatrix::GetRowData(int iRow)
{
	GetRowBytePos(iRow, m_from, m_end);
	
	return this->mid(m_from, m_end - m_from);
}

QByteArrayMatrix QByteArrayMatrix::GetCol(int iCol)
{
	QByteArrayMatrix s("", m_delim_row, m_delim_col);

	int nRows = GetRowCount();
	for (int i = 1; i <= nRows; i++){
		if (i != 1)
			s << m_delim_col;
		s << GetFieldValue(i, iCol);
	}

	return s;
}

QByteArrayMatrix QByteArrayMatrix::GetField(int iCol)
{
	return GetCol(iCol);
}

void QByteArrayMatrix::GetRowBytePos(int iRow, int &from, int &end)
{
	from = 0;
	end = 0;

	if (iRow < 1)
		return;
	
	for (int i = 1; i < iRow; i++){
		from = indexOf(m_delim_row, from);

		if (from == -1){
			from = 0;
			return;
		}
		
		if (iRow > 1)
			from += m_delim_row.count();
	}
	end = indexOf(m_delim_row, from);
	if (-1 == end)
		end = count();
}

void QByteArrayMatrix::GetBytePos(int iRow, int iCol, int &from, int &end)
{
	from = 0;
	end = 0;

	if (iRow < 1 || iCol < 1)
		return;

	GetRowBytePos(iRow, from, end);
		
	int from_min = from;
	int end_max = end;
	
	for (int j = 1; j < iCol; j++){	
		from = indexOf(m_delim_col, from);

		if (from == -1){
			from = from_min;
			end = from_min;
			return;
		}
		
 		if (iCol > 1)
 			from += m_delim_col.count();
 	}
 	int end2 = indexOf(m_delim_col, from);
	end = qMin(end, end2);
	if (-1 == end)
		end = count();

	if (from > end_max)
		from = end_max;
	if (end > end_max)
		end = end_max;
}

QByteArray	QByteArrayMatrix::GetFieldValue(int iRow, int iCol)
{
	GetBytePos(iRow, iCol, m_from, m_end);
	
	return mid(m_from, m_end - m_from);
}

void QByteArrayMatrix::SetFieldValue(int iRow, int iCol, const char *val)
{
	GetBytePos(iRow, iCol, m_from, m_end);
	replace(m_from, m_end - m_from, val);
}

void QByteArrayMatrix::SetFieldValue(int iRow, int iCol, const QByteArray &s)
{
	GetBytePos(iRow, iCol, m_from, m_end);
	replace(m_from, m_end - m_from, s);
}

