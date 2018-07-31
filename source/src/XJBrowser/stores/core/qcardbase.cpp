#include "StdAfx.h"
#include "qcardbase.h"

//const size_t CCardBase::c_uMaxLen = 0x2800;                    //数据最大长度 10240(10K)
const size_t CCardBase::c_uMaxLen = 100;

namespace qf {

char *qf_priv(char *pszIn, int idx, int &offset)
{
	if (NULL == pszIn)
		return NULL;
	
	offset = 0;
	char *p = pszIn;
	for (int i = 0; i < idx; ++i)
	{
		offset += strlen(p) + 1;
		p = (p + strlen(p) + 1);
	}
	
	return p;
}

int qf_count(char *pszIn, const char &delim)
{
	if (NULL == pszIn)
		return 0;

	int nCount = 0;
	int i = 0;
	size_t nLen = strlen(pszIn);
	while(i < nLen){
		if (pszIn[i] == delim)
			nCount++;
		
		++i;
	}
	nCount += 1;
	
	return nCount;
}

char* qf_matrix_at(char *pszIn, char *pszOut, int iLen
				   , int iRow, int iCol
				   , const char &delimRow
				   , const char &delimCol)
{
	char *pReturn = pszOut;

	return pReturn;
}

char* qf_arr_at(char *pszIn, char *pszOut, int iLen, int idx, const char &delim)
{
	char *pReturn = pszOut;

	if (NULL == pszOut)
		return pReturn;

	memset(pszOut, 0, iLen);

	int i = 0;
	int index = 0;
	int offset_start = 0;
	int offset_end = 0;

	size_t nLen = strlen(pszIn);
	while(i <= nLen){
		if (pszIn[i] == delim || pszIn[i] == '\0'){
			offset_end = i - 1;
			if (index == idx){
				strncpy(pszOut, pszIn + offset_start, offset_end - offset_start + 1);

				return pReturn; 
			}

			offset_start = i + 1;
			index++;
		}
		
		++i;
	}

	return pReturn;
}

int qf_trim(char *strIn, char *strOut)
{
	
	int i, j ;
	
	i = 0;
	
	j = strlen(strIn) - 1;
	
	while(strIn[i] == ' ')
		++i;
	
	while(strIn[j] == ' ')
		--j;
	strncpy(strOut, strIn + i , j - i + 1);
	strOut[j - i + 1] = '\0';
	
	return strlen(strOut);
}



} // end of namespace qf