//
// String.h
//
// This is a string operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//


#ifndef XJString_h__
#define XJString_h__

#include "define.h"

using namespace std;

class CXJString : public std::string  
{	
public:

	CXJString(const CXJString& string1);
	CXJString(const char *ch);
	CXJString(const char ch);
	
	CXJString();

	int Delete( int nIndex, int nCount = 1 );

	int Insert( int nIndex, const char * pstr );
	int Insert( int nIndex, char ch );

	int Remove( char ch );

	void MakeReverse( );

	int Find( char ch ) const;
	int Find( const char * lpszSub ) const;
	int Find( char ch, int nStart ) const;
	int Find( const char * pstr, int nStart ) const;

	int ReverseFind( char ch ) const;

	int FindOneOf( const char * lpszCharSet ) const;

	int Format(const char* pstrFormat, ... );

	int GetLength() const;

	CXJString Left(int nCount) const;
	CXJString Right(int nCount) const;

	CXJString Mid(int nFirst) const;
	CXJString Mid( int nFirst, int nCount) const;

	CXJString& operator=(const std::string str);
	CXJString& operator=(char ch);
	CXJString& operator =( const char * lpsz );
	CXJString& operator=(const CXJString& str);
	void MakeUpper();
	void MakeLower();
	void MakeLower(CXJString & str);
	void MakeUpper(CXJString & str);

	bool IsEmpty( ) const;

	void Empty( );

	char GetAt( int nIndex ) const;

	char operator []( int nIndex ) const;

	void SetAt( int nIndex, char ch );

	operator const char * ( ) const;

	int Compare( const char * lpsz ) const;
	int Compare( const CXJString& string1 ) const;
	int CompareNoCase( const char * lpsz ) const;
	int CompareNoCase( const CXJString& string1 ) const;

	void TrimLeft( );
	void TrimLeft( char chTarget );
	void TrimLeft( char * pchar);
	
	void TrimRight( );
	void TrimRight( char chTarget );
	void TrimRight( char * pchar );

	void Replace( char chOld, char chNew );
	void Replace(const char* chOld,const char* chNew );
	
	char * GetBuffer( int nMinBufLength );

	void ReleaseBuffer( int nNewLength = -1 );

	friend CXJString operator + (const CXJString& string1, const CXJString& string2)
	{
		CXJString str;
		str.append(string1);
		str.append(string2);
		return str;
	}
	friend CXJString operator + ( const CXJString& string1, char ch )
	{
		CXJString str;
		str.append(string1);
		str.insert(str.end(),ch);
		return str;
	}
	friend CXJString operator + ( const CXJString& string1, char* ch )
	{
		CXJString str;
		str.append(string1);
		str.append(ch);
		return str;
	}
};
#endif // XJString_h__

