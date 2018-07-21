// XJString.cpp: implementation of the CXJString class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "XJString.h"

//////////////////////////////////////////////////////////////////////////
//                  Class  CXJString   实现                                    
//////////////////////////////////////////////////////////////////////////

CXJString::CXJString(const CXJString& string1)
{
	this->append(string1);
}
CXJString::CXJString(const char *ch)
{
	*this=ch;
//	this->append(ch);
}
CXJString::CXJString(const char ch)
{
	*this += ch;
}

CXJString::CXJString()
{}

/*********************************************************

   Deletes a character or characters from a string.

*********************************************************/
int CXJString::Delete( int nIndex, int nCount)
{
	this->erase(nIndex,nCount);
	return this->GetLength();
}
/*********************************************************

   Inserts a single character or a substring at the given 
   index within the string.

*********************************************************/
int CXJString::Insert( int nIndex, const char * pstr )
{
	this->insert(nIndex,pstr);
	return this->GetLength();	
}
int CXJString::Insert( int nIndex, char ch )
{	
	CXJString strTmp(ch);
	this->insert(nIndex,strTmp);
	strTmp.Empty();
	return this->GetLength();
}
/*********************************************************

   Removes indicated characters from a string.

*********************************************************/
int CXJString::Remove( char ch )
{
	CXJString::iterator iter;
	int count = 0;
	for(iter = this->begin(); iter != this->end();iter ++)
	{
		if(*iter == ch)
		{
			this->erase(iter);count++;
		}
	}
	return count;
}
/*********************************************************

   Reverses the characters in this string.

*********************************************************/
void CXJString::MakeReverse( )
{
	CXJString strTmp;
	CXJString::iterator iter;
	iter=this->end();
	iter--;
	for(; iter != this->begin(); iter--)
	{
		strTmp += *iter;
	}
	strTmp += *iter;
	*this = strTmp;
	strTmp.Empty();
}
/*********************************************************

   Finds a character or substring inside a larger string.

*********************************************************/
int CXJString::Find( char ch ) const
{
	return this->find(ch);
}
int CXJString::Find( const char * lpszSub ) const
{
	return this->find(lpszSub);
}
int CXJString::Find( char ch, int nStart ) const
{
	return this->find(ch,nStart);
}
int CXJString::Find( const char * pstr, int nStart ) const
{
	return this->find(pstr,nStart);
}
/*********************************************************

   Finds a character inside a larger string; starts from the end.

*********************************************************/
int CXJString::ReverseFind( char ch ) const
{
	return this->find_last_of(ch);	
}
/*********************************************************

   Finds the first matching character from a set.

*********************************************************/
int CXJString::FindOneOf( const char * lpszCharSet ) const
{
	return this->find_first_of(lpszCharSet);
}
/*********************************************************

   Format the string as sprintf does.

*********************************************************/
int CXJString::Format(const char* pstrFormat, ... )
{///本函数仅仅支持ANSI标准字符集 '%[flags] [width] [.precision] [{h | l | I64 | L}]type'	 	
	assert(pstrFormat!=NULL);
	va_list argList;
	va_start(argList,pstrFormat); 
	int nMaxLen = 0;
	for (const char * p = pstrFormat; *p != '\0';p++ )
	{
		if (*p != '%' || *(++p) == '%')
		{// 如果不是'%'就直接累计长度，如果是'%%'也使长度加1
			nMaxLen += 1;
			continue;
		}
		int nItemLen = 0; //用来保存每个参数的长度
		int nWidth = 0; //用来保存每一项的宽度
		for (; *p != '\0'; p ++)
		{
			if (*p == '#')
				nMaxLen += 2;   // 处理 '0x'
			else if (*p == '*')
				nWidth = va_arg(argList, int);  //如：'%5f' 中的5
			else if (*p == '-' || *p == '+' || *p == '0'|| *p == ' ')
				;  //忽略该符号
			else // 不是标志字符就退出循环
				break;
		}
		if (nWidth == 0)
		{ //提取宽度
			nWidth = atoi(p);
			for (; *p != '\0' && isdigit(*p); p ++)
				;
		}
		assert(nWidth >= 0);//有效宽度
		int nPrecision = 0; //精度位数
		if (*p == '.')
		{
			p++;// 跳过 '.'字符 (宽度.精度)			
			if (*p == '*')
			{ //有参数给出
				nPrecision = va_arg(argList, int);
				p ++;// 取得精度，跳过字符
			}
			else
			{ //在格式串中有宽度
				nPrecision = atoi(p);
				for (; *p != '\0' && isdigit(*p);	p ++)
					;
			}
			assert(nPrecision >= 0);//有效宽度
		}
		switch (*p)
		{
		case 'h':     //short int 型
			p ++;
			break;
		case 'l':	 //long double 型
			p ++;
			break;
		case 'F':	 //近指针
		case 'N':	//远指针	
		case 'L':	//long double 型
			p++;
			break;
		}
		switch (*p)
		{
		case 'c':   //// 单个字符
		case 'C':
			nItemLen = 2;
			va_arg(argList,int);
			break;
		case 's':	//// 字符串
		case 'S':
			nItemLen = strlen(va_arg(argList, const char*));
			nItemLen = ((1) > (nItemLen)) ? (1) : (nItemLen);//如果是空串就使用1 即保存'\0'
			break;
		}
		if (nItemLen != 0)		
		{
			nItemLen = ((nItemLen) > (nWidth)) ? (nItemLen) : (nWidth);//使用大者
			if (nPrecision != 0)
				nItemLen = ((nItemLen) < (nPrecision)) ? (nItemLen) : (nPrecision);
		}
		else
		{
			switch (*p)
			{
			case 'd':    //整数的处理
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				va_arg(argList, int);
				nItemLen = 32;  //四字节
				nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者
				break;
			case 'e':	//浮点数
			case 'f':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 32;//四字节
				nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
				break;
			case 'p':	//指针
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
				break;
			case 'n':
				va_arg(argList, int*); //指向整数的指针,见BorlanderC++3.1库函数P352
				break;
			default:
				assert(false);  //不能处理的格式，给出警告
			}
		}
		nMaxLen += nItemLen;//把该项的长度累计
	}
	va_end(argList);
	va_start(argList, pstrFormat);  // 重新开始提取参数
	char* ch = new char[nMaxLen+1]; //分配内存
	vsprintf(ch, pstrFormat, argList);
	//assert(vsprintf(ch, pstrFormat, argList) <= nMaxLen);
    //首先情况原来缓存数据
	this->Empty();

	this->append(ch); //加到string的尾部
	delete[] ch;      //释放内存
	va_end(argList);
	return nMaxLen;
}
/*********************************************************

   Returns the number of characters in a CXJString object. For 
   multibyte characters, counts each 8-bit character; that is, 
   a lead and trail byte in one multibyte character are counted 
   as two characters.

*********************************************************/
int CXJString::GetLength() const
{
	return this->length();
}
/*********************************************************

   Extracts the left part of a string (like the Basic LEFT$ 
   function).

*********************************************************/
CXJString CXJString::Left(int nCount) const
{
	if (nCount <=0) 
		return CXJString("");	
	CXJString strTmp;
	strTmp = this->substr(0,nCount);
	return strTmp;
}
/*********************************************************

   Extracts the right part of a string (like the Basic RIGHT$ 
   function).

*********************************************************/
CXJString CXJString::Right(int nCount) const
{
	if (nCount <=0) 
		return CXJString("");
	CXJString strTmp;
	if (nCount > GetLength())
		strTmp = this->substr(0);
	else
		strTmp = this->substr(GetLength()-nCount);
	return strTmp;
}
/*********************************************************

   Extracts the middle part of a string (like the Basic MID$ 
   function).

*********************************************************/
CXJString CXJString::Mid(int nFirst) const
{
	CXJString strTmp;
	if (nFirst >= GetLength())
		return CXJString("");
	if (nFirst <= 0)
		strTmp = this->substr(0);
	else
		strTmp = this->substr(nFirst);
	return strTmp;		
}
CXJString CXJString::Mid( int nFirst, int nCount) const
{
	if (nCount <= 0) 
		return CXJString("");
	if (nFirst >= GetLength())
		return CXJString("");
	CXJString strTmp;
	if (nFirst <= 0)
		strTmp = this->substr(0,nCount);
	else
		strTmp = this->substr(nFirst,nCount);
	return strTmp;				
}
/*********************************************************

   Assigns a new value to a CXJString object.

*********************************************************/
CXJString& CXJString::operator=(const std::string str)
{
	if (this->compare(str) == 0) return *this;
	this->assign(str);
	return *this;
}
CXJString& CXJString::operator=(char ch)
{
	this->Empty();
	this->insert(this->begin(),ch);
	return *this;
}
CXJString& CXJString::operator =( const char * lpsz )
{
	this->Empty();
	this->append(lpsz);
	return *this;
}

CXJString& CXJString::operator=( const CXJString& str )
{
	if (this->compare(str) == 0) return *this;
	this->Empty();
	this->append(str.c_str());
	return *this;
}
/*********************************************************

   Converts all the characters in this string to uppercase 
   characters.

*********************************************************/
void CXJString::MakeUpper()
{
/*	std::transform(this->begin (),
		this->end (),this->begin (),
		toupper);*/

	char * pchar = (char*)this->c_str();

	for (int i =0; i< this->length(); i++)
	{
		*(pchar+i) = toupper(*(pchar+i));
	}
}

void CXJString::MakeUpper(CXJString & str)
{
	char * pchar = str.GetBuffer(str.GetLength());

	for (int i =0; i< str.GetLength(); i++)
	{
		*(pchar+i) = toupper(*(pchar+i));
	}

	str.ReleaseBuffer();

}

/*********************************************************

   Converts all the characters in this string to lowercase 
   characters.

*********************************************************/
void CXJString::MakeLower()
{
/*	std::transform(this->begin (),
		this->end (),this->begin (),
		tolower);*/
	char * pchar = (char*)this->c_str();

	for (int i =0; i< this->length(); i++)
	{
		*(pchar+i) = tolower(*(pchar+i));
	}
}

void CXJString::MakeLower(CXJString & str)
{
	char * pchar = str.GetBuffer(str.GetLength());

	for (int i =0; i< str.GetLength(); i++)
	{
		*(pchar+i) = tolower(*(pchar+i));
	}

	str.ReleaseBuffer();
}

/*********************************************************

   Tests whether a CXJString object contains no characters

*********************************************************/
bool CXJString::IsEmpty( ) const
{
	return this->empty(); 
}
/*********************************************************

   Forces a string to have 0 length

*********************************************************/
void CXJString::Empty( )
{
	this->erase(this->begin(),this->end());
}
/*********************************************************

   Returns the character at a given position.

*********************************************************/
char CXJString::GetAt( int nIndex ) const
{
	return this->at(nIndex);
}
/*********************************************************

   Returns the character at a given position — operator 
   substitution for GetAt.

*********************************************************/
char CXJString::operator []( int nIndex ) const
{
	return this->at(nIndex);
}
/*********************************************************

   Sets a character at a given position.

*********************************************************/
void CXJString::SetAt( int nIndex, char ch )
{
	this->at(nIndex) = ch;
}

/*********************************************************

   Directly accesses characters stored in a CXJString object 
   as a C-style string.

*********************************************************/
CXJString::operator const char *( ) const
{
	return this->c_str();
}
/*********************************************************

   Compares two strings (case sensitive).

*********************************************************/
int CXJString::Compare( const char * lpsz ) const
{
	CXJString str;
	str.append(lpsz);
	return this->compare(str);
}
int CXJString::Compare( const CXJString& string1 ) const
{
	return this->compare(string1);
}
/*********************************************************

   Compares two strings (case insensitive).

*********************************************************/
int CXJString::CompareNoCase( const char * lpsz ) const
{
	CXJString str,strThis,strSource;
	str.append(lpsz);

	strncpy(strSource.GetBuffer(str.GetLength()),
		    str.c_str(),str.GetLength());
	strSource.ReleaseBuffer();

	strncpy(strThis.GetBuffer(this->GetLength()),
		   this->c_str(),this->length());
	strThis.ReleaseBuffer();

	strSource.MakeLower(strSource);
	strThis.MakeLower(strThis);
	return strThis.compare(strSource);
}

int CXJString::CompareNoCase( const CXJString& string1 ) const
{
	CXJString str,strThis;
    
	strncpy(str.GetBuffer(string1.size()),
		   string1.c_str(),string1.length());
	str.ReleaseBuffer();

	strncpy(strThis.GetBuffer(this->GetLength()),
		   this->c_str(),this->length());
	strThis.ReleaseBuffer();

	str.MakeLower(str);
	strThis.MakeLower(strThis);
	return strThis.compare(str);
}
/*********************************************************

   Trim trailing whitespace characters from the string.

*********************************************************/
void CXJString::TrimRight( )
{
	TrimRight (' ');
}
/*********************************************************

   Trim leading whitespace characters from the string.

*********************************************************/
void CXJString::TrimLeft( )
{   
	TrimLeft(' ');
}
/*********************************************************

   Trim leading target characters from the string.

*********************************************************/
void CXJString::TrimLeft( char chTarget )
{
	std::string::size_type pos;
	pos = this->find_first_not_of(chTarget);
	if ((pos == npos)||(pos <= 0)) 
		return; 
	this->erase(this->begin(),this->begin()+pos);		
}
/*********************************************************

   Trim leading target string from the string.

*********************************************************/
void CXJString::TrimLeft( char * pchar)
{
	std::string::size_type pos;
	pos = this->find_first_not_of(pchar);
	if ((pos == npos)||(pos <= 0)) 
		return; 
	this->erase(this->begin(),this->begin()+pos);	
}
/*********************************************************

   Trim trailing target characters from the string.

*********************************************************/
void CXJString::TrimRight( char chTarget )
{
	std::string::size_type pos;
	pos = this->find_last_not_of(chTarget);
	++pos;
	if ((pos == npos)||(pos >= this->GetLength()))
		return; 
	this->erase(this->begin()+pos,this->end());		
}
/*********************************************************

   Trim trailing target string from the string.

*********************************************************/
void CXJString::TrimRight( char * pchar )
{
	/*std::string::size_type pos;
	pos = this->find_last_not_of(pchar);
	++pos;
	if (pos == this->GetLength())
		return; 
	this->erase(this->begin()+pos,this->end());	*/

	int nLen = strlen(pchar);
	if(nLen <=0 )
		return;
    if( strcmp((const char*)Right(nLen),pchar) == 0)
	{
		this->erase( this->begin()+(this->GetLength() - nLen),this->end());
	}
}
/*********************************************************

   Replaces indicated characters with other characters.

*********************************************************/
void CXJString::Replace( char chOld, char chNew )
{
	for(int i=0;i<this->GetLength();i++)
	{
		if (this->at(i) == chOld)
			this->at(i) = chNew;
	}		
}
void CXJString::Replace(const char* chOld,const char* chNew )
{
	int index = this->find(chOld);
	while (index > -1)
	{
		this->erase(index,strlen(chOld));
		this->insert(index,chNew);
		index = this->find(chOld);
	}
}		
/*********************************************************

   Returns a pointer to the characters in the CXJString

*********************************************************/
char * CXJString::GetBuffer( int nMinBufLength )
{
	if(nMinBufLength != 0)
		this->resize(nMinBufLength);

	return (char*)this->c_str();
}
/*********************************************************

   Releases control of the buffer returned by GetBuffer.

*********************************************************/
void CXJString::ReleaseBuffer( int nNewLength)
{
	this->TrimRight('\0');
}
