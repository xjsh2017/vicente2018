// SttpMsgParser.cpp: implementation of the CSttpMsgParser class.
//
//////////////////////////////////////////////////////////////////////

#include "SttpMsgParser.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSttpMsgParser::CSttpMsgParser()
{
	m_pMsg=NULL ;
	m_pMsgHdr=NULL ;
}

CSttpMsgParser::CSttpMsgParser(STTPMSG *pMsg)
{
	m_pMsg=pMsg ;
	m_pMsgHdr=&pMsg->MsgHdr;
}


CSttpMsgParser::~CSttpMsgParser()
{

}
//
/**	\brief	GetMsgID function                       ������Ϣ�ı��
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::GetMsgID(char* strID)
{
	memcpy(strID, m_pMsgHdr->m_byteMsgID, 5) ;
	strID[5]='\0' ;
}

UINT CSttpMsgParser::GetMsgID()
{
	char chtemp[6];	
	GetMsgID(chtemp);
	return atoi(chtemp);
}
//
/**	\brief	SetMsgID function   ������Ϣ�ı��
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgID(char* strID)
{
	memcpy(m_pMsgHdr->m_byteMsgID,strID,5) ;
}

void CSttpMsgParser::SetMsgID( UINT msgId )
{
	sprintf((char*)m_pMsgHdr->m_byteMsgID,"%05d",msgId);	
}
/**	\brief	GetMsgHdr function   ������Ϣ��ͷ
	\param	strID	a parameter of type char*
	\return	char*
*/
char* CSttpMsgParser::GetMsgHdr()
{
	return (char*)m_pMsgHdr ;
}

/**	\brief	SetMsgHdr function   д����Ϣ��ͷ
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgHdr(char* strMsgHdr)
{
	memcpy(m_pMsgHdr, strMsgHdr, sizeof(STTPMSGHDR)) ;
}

/**	\brief	GetMsgLength function   ������Ϣ�Ŀɱ䲿�ֵĳ���
	\param	strID	a parameter of type char*
	\return	UINT
*/
UINT CSttpMsgParser::GetMsgLength()
{
/*	BYTE bTemp = m_pMsgHdr->m_byteMsgLength;
    char chHex;
	chHex = bTemp;
	return strtol(&chHex,NULL,10);
*/
	return strtol((char*)m_pMsgHdr->m_byteMsgLength,NULL,10);
}

/**	\brief	SetMsgLength function   ������Ϣ�ɱ䲿�ֵĳ���
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgLength(UINT nLeng)
{
	char strTemp[16];
	sprintf(strTemp,"%-5d",nLeng);
	memcpy(m_pMsgHdr->m_byteMsgLength,strTemp,5);
}

/**	\brief	GetMsgLengthType function   ȡ����Ϣ�ĳ�������
	\param	strID	a parameter of type char*
	\return	char
*/
char CSttpMsgParser::GetMsgLengthType()
{
	return (char)m_pMsgHdr->m_byteMsgLengthType ;
}

/**	\brief	SetMsgLengthType function   ������Ϣ���ȵ�����
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgLengthType(char cType)
{
   m_pMsgHdr->m_byteMsgLengthType=(BYTE)cType ;
}

/**	\brief	GetMsgType function   ȡ����Ϣ������
	\param	strID	a parameter of type char*
	\return	char
*/
char CSttpMsgParser::GetMsgType()
{
	return m_pMsgHdr->m_byteMsgType ;
}

/**	\brief	SetMsgType function   ������Ϣ������
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgType(char cType)
{
	m_pMsgHdr->m_byteMsgType = (BYTE)cType ;
}

/**	\brief	SetMsgBody function   ������Ϣ�ɱ䲿��
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::SetMsgBody(char* strBody)
{
	memcpy(m_pMsg->MsgBody, strBody,GetMsgLength()) ;
}

/**	\brief	GetMsgBody function   ������Ϣ�ɱ䲿��
	\param	strID	a parameter of type char*
	\return	void
*/
char* CSttpMsgParser::GetMsgBody()
{
	return m_pMsg->MsgBody ;
}

/**	\brief	Attach function   ��STTPMSGճ��CSttpMsgParser�������STTPMSG���д���
	\param	strID	a parameter of type char*
	\return	void
*/
void CSttpMsgParser::Attach(STTPMSG *pMsg)
{
	m_pMsg= pMsg ;
	m_pMsgHdr=&pMsg->MsgHdr ;
}


/**	\brief	ArrayToDouble function   ��ͨ��STTP�յ����ֽ������ֵת����˫������
	\param	strID	a parameter of type char*
	\return	double
*/
double CSttpMsgParser::ArrayToDouble(char *szByte, int iLength)
{
	char * pByte;
	char strTemp[16];
	memcpy(strTemp,szByte,iLength);
	strTemp[iLength]='\0';
	return strtod(strTemp,&pByte);
}

/**	\brief	ArrayToHex function   ��ͨ��STTP�յ����ֽ�����ת��Ϊʮ��������
	\param	strID	a parameter of type char*
	\return	UINT
*/
UINT CSttpMsgParser::ArrayToHex(char *byte, int nCount)
{
	if(nCount>8)
		return 0;
	else
	{
		char ptempbyte[9];
		memset(ptempbyte,0,9);
		memcpy(ptempbyte,byte,nCount);
		return strtol(ptempbyte,NULL,16);
	}
}

/**	\brief	ArrayToInt function   ��ͨ��STTP�յ����ֽ�����ת��Ϊ������
	\param	strID	a parameter of type char*
	\return	int
*/
int CSttpMsgParser::ArrayToInt(char *byte, int nCount)
{
	
	if(nCount>16)
		return 0;
	else
	{
		char ptempbyte[17];
		memset(ptempbyte,0,17);
		memcpy(ptempbyte,byte,nCount);
		return strtol(ptempbyte,NULL,10);
	}

}

/**	\brief	GetMsgEndFlag function   �õ����ĺ�����־֡
	\param	strID	a parameter of type char*
	\return	bool  false ��ʾ�޺���֡,true��ʾ�к���֡
*/
bool CSttpMsgParser::GetMsgEndFlag()
{
	if(m_pMsgHdr->m_byteMsgVersion == '1')
	{
		return true;
	}
	else if(m_pMsgHdr->m_byteMsgVersion == '0')
	{
		return false;
	}
	else
	{
		return false;
	}
}

//add by yinzhehong 20061031
/**	\brief	SetMsgEndFlag function   ���ñ��ĺ�����־֡
	\param	bEndFlag	a parameter of type bool  false ��ʾ�޺���֡,true��ʾ�к���֡
	\return	void  
*/
void CSttpMsgParser::SetMsgEndFlag(bool bEndFlag)
{
	if(bEndFlag)
	{
		m_pMsgHdr->m_byteMsgVersion = '1';
	}
	else
	{
		m_pMsgHdr->m_byteMsgVersion = '0';
	}
}
/**	\brief	SetMsgNums function    ������Ϣ����֡��   
	\param	nNums	   UINT        ��Χ��1-9
	\return	void
*/
void CSttpMsgParser::SetMsgNums(UINT nNums)
{
	if(nNums > 9 || nNums < 1)
	{
		return ;
	}
	m_pMsgHdr->m_byteMsgType = nNums + 48;
}
/**	\brief	GetMsgNums function �õ���Ϣ����֡��
	\return	UINT	
*/
UINT CSttpMsgParser::GetMsgNums()
{
	 //TRACE("bytemsgType:%s/n",m_pMsgHdr->m_byteMsgType);
//            GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));
    BYTE bTemp = m_pMsgHdr->m_byteMsgType;
    char chHex[2]="";
	bzero(chHex,sizeof(chHex));
	memcpy(chHex,&bTemp,1);

	return strtol(chHex,NULL,10);
}
/**	\brief	SetMsgCurNum function  ���õ�ǰ֡,�ڴ��䶨ֵʱʹ��
	\param	nNums	UINT
	\return	void
*/
void CSttpMsgParser::SetMsgCurNum(UINT nNum)
{
	if(nNum > 9 || nNum < 1)
	{
		return ;
	}
	m_pMsgHdr->m_byteMsgLengthType = nNum+48;
}
/**	\brief	GetMsgCurNum function �õ���ǰ֡��
	\return	UINT                
*/
UINT CSttpMsgParser::GetMsgCurNum()
{
	BYTE bTemp = m_pMsgHdr->m_byteMsgLengthType;
    char chHex[2]="";
	bzero(chHex,sizeof(chHex));
	memcpy(chHex,&bTemp,1);
    return strtol(chHex,NULL,10);
}
/**	\brief	SetRII function��������Ϣ��ʶ��
	\param	nRiiNum	 UINT������Ϣ��ʶ��š���0-255
	\return	void	
*/
void CSttpMsgParser::SetMsgRII(UINT nRiiNum)
{
	if(nRiiNum > 999)
		return ;
	char strTemp[4];
	sprintf(strTemp,"%-3d",nRiiNum);
	memcpy(m_pMsgHdr->m_byteMsgSubsetId,strTemp,3);
}
/**	\brief	GetRii function���õ���Ϣ��ʶ��
	\return	UINT����������������Ϣ��ʶ��š���0-255
*/
UINT CSttpMsgParser::GetMsgRII()
{
    return 	ArrayToInt((char*) m_pMsgHdr->m_byteMsgSubsetId,3);
//	return strtol((char*)m_pMsgHdr->m_byteMsgSubsetId,NULL,10);
}

/**	\brief	  IntToArray function ������תΪ�ַ���
	\param	l	a parameter of type UINT
	\param	ch	a parameter of type char *
	\param	length	a parameter of type int
	\return	void	
*/
void CSttpMsgParser::IntToArray(UINT l, char *ch, int length)
{
	if(length>16)
		return;
	else
	{
		//Ҫ�����д��ʽΪ�����󿿣����㲿���ÿո�����磨"89  "��
		char strFormat[6],strTemp[16];
		strcpy(strFormat,"%-");
//		sprintf()
//		itoa(length,strFormat+2,10);
		Itoa(length,strFormat+2,10);//��˽�е�
		strcpy(strFormat+3+(length/10),"d");
		//��д��ʽ���ַ����Ĺ���
		sprintf(strTemp,strFormat,l);
		memcpy(ch,strTemp,_min( length, 16 ));
	};
}
/**	\brief	DoubleToArray function ��������תΪ�ַ���
	\param	l	a parameter of type UINT
	\param	ch	a parameter of type char *
	\param	length	a parameter of type int
	\return	void	
*/
void CSttpMsgParser::DoubleToArray(double fDouble, char *szByte, int iLength)
{
	char strTemp[16];
	sprintf(strTemp,"%f",fDouble);
	memcpy(szByte,strTemp,iLength);
}
/**	\brief	Itoa function                      ������תΪ�ַ���
	\param	l	a parameter of type UINT
	\param	ch	a parameter of type char *
	\param	length	a parameter of type int
	\return	void	
*/
char* CSttpMsgParser::Itoa(int value, char *str, int radix)
{
    int  rem = 0;
    int  pos = 0;
	//char ch  = '!';
	do
	{
		char ch  = '!';
		rem = value % radix;
		value /= radix;
		if(16 == radix)
		{
			if(rem >= 10 && rem <= 15)
			{
				switch(rem)
				{
				case 10:
					ch = 'a';
					break;
				case 11:
					ch = 'b';
					break;
				case 12:
					ch = 'c';
					break;
				case 13:
					ch = 'd';
					break;
				case 14:
					ch = 'e';
					break;
				case 15:
					ch = 'f';
					break;
				}
			}
		}
		if('!' == ch)
		{
			str[pos++] = (char) (rem + 0x30);
		}
		else
		{
			str[pos++] = ch;
		}
	}while( value != 0);
    str[pos] = '\0';
	return Strrev(str);
}
char* CSttpMsgParser::Strrev(char *szT)
{
	if(!szT)                     // ������Ŀմ�.
		return "";
	int i = strlen(szT);
	int t = !(i%2)?1:0;          // ��鴮����
	for(int j = i-1,k=0; j> (i/2 -t); j--)
	{
		char ch = szT[j];
		szT[j]  = szT[k];
		szT[k++] = ch;
	}
	return szT;
}
