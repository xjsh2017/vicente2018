#ifndef _QCARDBASE_H
#define _QCARDBASE_H

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

#define _countof(arr)  (sizeof(arr)/sizeof(arr[0]))

namespace qf {
	extern inline char*		qf_priv(char *pszIn, int idx, int &offset);
	extern inline int		qf_count(char *pszIn, const char &delim = ',');
	extern inline int		qf_trim(char *strIn, char *strOut);
	extern inline char*		qf_arr_at(char *pszIn, char *pszOut, int iLen, int idx
									, const char &delim = ',');
	extern inline char*		qf_matrix_at(char *pszIn, char *pszOut, int iLen
									, int iRow, int iCol
									, const char &delimRow = ','
									, const char &delimCol = ';');
};


//»ù´¡Êý¾Ý¿¨
class CCardBase
{
public:
	CCardBase(){ m_uLen = c_uMaxLen; }
	virtual ~CCardBase(){}
	virtual int FRead(char *pszLine, size_t s){return 0; }
	virtual int FWrite(fstream &fs){ return 0; }
	/*
	virtual void GetName(char *pszName, int iLen){}
	virtual void GetNameTrimR(char *pszName, int iLen){}
	virtual void GetName2(char *pszName, int iLen){}
	virtual void GetName2TrimR(char *pszName, int iLen){}
	virtual void GetZone(char *pName, int iLen){}
	virtual void SetGenP(double dVal){}
	virtual double GetGenP() const{ return 0; }
	virtual double GetR() const { return 0; }
	virtual void SetR(double dVal){}
	virtual double GetX() const { return 0; }
	virtual void SetX(double dVal) {}
	virtual double GetG() const { return 0; }
	virtual void SetG(double dVal){}
	virtual double GetB() const { return 0; }
	virtual void SetB(double dVal){}*/
	const size_t GetLen() const { return m_uLen; }
	const static size_t c_uMaxLen;

protected:
	size_t m_uLen;
private:
};

#endif
