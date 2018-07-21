// File.cpp: implementation of the CXJFile class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "XJFile.h"

//////////////////////////////////////////////////////////////////////////
//                  Class  CXJFile   ʵ��                                    
//////////////////////////////////////////////////////////////////////////
CXJFile::CXJFile()
{
	pfile = NULL;

	memset(m_chError,0,FILE_ERROR_MAX_LEN);

	memset(m_chName,0,FILE_NAME_MAX_LEN);
}

CXJFile::~CXJFile()
{
	if(pfile != NULL)
		fclose(pfile);

	pfile = NULL;
}

/*************************************************************
 �� �� ����Open()
 ���ܸ�Ҫ�����ļ�
 �� �� ֵ: BOOL  TRUE:�ɹ� FALSE:ʧ��
 ��    ����const char * name   �ļ�����
           UINT  nOpenFlags    �򿪱�־
		   #define modeCreate                0          //0000 0000 
           #define modeRead                  1          //0000 0001
           #define modeWrite                 2          //0000 0010
           #define modeReadWrite             4          //0000 0100
		   #define modeNoTruncate            8          //0000 1000
***************************************************************/
BOOL CXJFile::Open(const char *name, UINT nOpenFlags)
{
	if(name == NULL)
	{
		memset(m_chError,0,FILE_ERROR_MAX_LEN);
		strcpy(m_chError,"File Name Is NULL,Can't Open");
        printf("%s\n",m_chError);
		return FALSE;
	}
	
	//save file name
	if( sizeof(name) > FILE_NAME_MAX_LEN )
	{
		memset(m_chError,0,FILE_ERROR_MAX_LEN);
		sprintf(m_chError,"file Name len =%d,too leng",sizeof(name));
        printf("%s\n",m_chError);
		return FALSE;
	}
	sprintf(m_chName,"%s",name);
    
	if(pfile != NULL)
	{
		memset(m_chError,0,FILE_ERROR_MAX_LEN);
		sprintf(m_chError,"the File '%s' has been Opend,please close it Firstly",name);
		printf("%s\n",m_chError);
		return FALSE;
	}
    
	switch(nOpenFlags)
	{
	case 0:    
		/*Opens an empty file for both reading and writing. If the given file 
		  exists, its contents are destroyed; creates the file first if it 
		  doesn��t exist. */
		pfile = fopen(name,"w+b");
		break;
	case 1:      
		/*Opens for reading. If the file does not exist or cannot be found, 
		  the fopen call fails*/
		pfile = fopen(name,"rb");
		break;
	case 2:
	case 8:
		/*Opens for writing at the end of the file (appending) without removing 
		  the EOF marker before writing new data to the file; creates the file 
		  first if it doesn��t exist*/
		pfile = fopen(name,"ab");
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		/*Opens for both reading and writing. (The file must exist.) */
		pfile = fopen(name,"r+b");
		break;
	default:
		/*Opens for reading and appending; the appending operation includes the 
		  removal of the EOF marker before new data is written to the file and 
		  the EOF marker is restored after writing is complete; creates the file 
		  first if it doesn��t exist*/
		pfile = fopen(name,"a+b");
		break;
	}

	if(pfile == NULL)
	{
		memset(m_chError,0,FILE_ERROR_MAX_LEN);
		sprintf(m_chError,"open %s failed,reason:%s(errno:%d)",name,strerror(errno),errno);
        printf("%s\n",m_chError);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����Read()
 ���ܸ�Ҫ�����ļ��ж�ȡָ�����ֽ���Ŀ
 �� �� ֵ: UINT ʵ�ʶ�ȡ�����ֽ���
 ��    ����void * lpbuf  ��Ŷ�ȡ�����ݵĻ�����
           UINT   nCount ��ȡ���ֽ���,ʵ�ʷ���ֵ����С�ڸ�ֵ

***************************************************************/
UINT CXJFile::Read(void * lpbuf, UINT nCount)
{
	UINT nRead = 0;
	if(pfile == NULL) 
	{
        memset(m_chError,0,FILE_ERROR_MAX_LEN);
		sprintf(m_chError,"read from %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return 0;
	}

	if(!feof(pfile))
	{
		//if( fgets((char*)lpbuf,nCount,pfile) == NULL)
		nRead = fread((char*)lpbuf,sizeof(char),nCount,pfile);
		if(ferror(pfile))
		{
			sprintf(m_chError,"read from %s error,reason:%s",m_chName,strerror(errno));
			printf("%s\n",m_chError);
			Flush();
			return nRead;
		}
		
		//����д����ת��ʱ��Ҫ����һ��fflush��fseek��fsetpos
		//Flush();
		return  nRead;//(strlen((char*)lpbuf)+1);	
	}
    
	//file pointer is end
	sprintf(m_chError,"the file %s's pointer has been eof,can't read",m_chName);
	printf("%s\n",m_chError);
	return 0;
}

/*************************************************************
 �� �� ����Write()
 ���ܸ�Ҫ�����ļ���д��ָ����Ŀ������
 �� �� ֵ: UINT ʵ��д���ļ��е�������Ŀ
 ��    ����const void * lpbuf  Ҫд������ݻ�����
           UINT  nCount        Ҫд������ݵ���Ŀ
***************************************************************/
UINT CXJFile::Write(const void * lpBuf,UINT nCount)
{
	UINT nwrite = 0;

	if(pfile == NULL) 
	{
        memset(m_chError,0,FILE_ERROR_MAX_LEN);
		sprintf(m_chError,"write to %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return 0;
	}

	//write data
    nwrite = fwrite(lpBuf,sizeof(char),nCount,pfile);
	if( int n=ferror(pfile) )
	{
		sprintf(m_chError,"write to %s error,reason:%s",m_chName,strerror(errno));
	    printf("%s\n",m_chError);
		Flush();
		return nwrite;
	}
		
	//����д����ת��ʱ��Ҫ����һ��fflush��fseek��fsetpos
	Flush();
	return nwrite;	
} 

/*************************************************************
 �� �� ����Flush()
 ���ܸ�Ҫ��ǿ�ƽ���������д���ļ�
 �� �� ֵ: BOOL
 ��    ������
***************************************************************/
BOOL CXJFile::Flush()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fflush %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return FALSE;
	}

	if(fflush(pfile) == 0)
		return TRUE;
	else{
	    sprintf(m_chError,"fflush %s error,reason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
	}
	return FALSE;
}

/*************************************************************
 �� �� ����Close()
 ���ܸ�Ҫ���ر��ļ�
 �� �� ֵ: BOOL
 ��    ������
 �޸���ʷ��
***************************************************************/
BOOL CXJFile::Close()
{
	if(pfile == NULL) 
	{
		return TRUE;
	}

	if(fclose(pfile) == 0)
	{
		pfile = NULL;
		return TRUE;
	}
	else{
	    sprintf(m_chError,"fclose %s error,reason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
	}
	return FALSE;
}

/*************************************************************
 �� �� ����Seek()
 ���ܸ�Ҫ���ƶ��ļ�ָ�뵽ָ����λ��
 �� �� ֵ: BOOL  �ɹ���ʧ��
 ��    ����long lOff   ƫ����
           UINT nFrom  ƫ����ʼ��ַ
***************************************************************/
BOOL CXJFile::Seek(long lOff,UINT nFrom)
{
	int nFlag = 0;
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return FALSE;
	}
    
	//�ж�position
	switch(nFrom)
	{
	case CXJFile::begin:
		nFlag = SEEK_SET;
		break;
	case CXJFile::current:
		nFlag = SEEK_CUR;
		break;
	case CXJFile::end:
		nFlag = SEEK_END;
		break;
	default:
		sprintf(m_chError,"fseek %s failed because the second argument is error",m_chName);
		printf("%s\n",m_chError);
		return FALSE;
	}
      
	//fseek
	if(fseek(pfile,lOff,nFlag) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek %s %ld failed,reason:%s",m_chName,lOff,strerror(errno));
        printf("%s\n",m_chError);
	}
	return FALSE;
}

/*************************************************************
 �� �� ����SeekToBegin()
 ���ܸ�Ҫ���ƶ��ļ�ָ�뵽�ļ�����ʼ��ַ
 �� �� ֵ: BOOL
 ��    ������
***************************************************************/
BOOL CXJFile::SeekToBegin()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek to begien %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return FALSE;
	}

	//set file position to begin
    if(fseek(pfile,0,SEEK_SET) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek to begin %s error,reason:%s",m_chName,strerror(errno));
        printf("%s\n",m_chError);
	}
	return FALSE;
}

/*************************************************************
 �� �� ����SeekToEnd()
 ���ܸ�Ҫ���ƶ��ļ�ָ�뵽�ļ�����λ��
 �� �� ֵ: BOOL
 ��    ������
***************************************************************/
BOOL CXJFile::SeekToEnd()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek to end %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return FALSE;
	}

	//set file position to end
    if(fseek(pfile,0,SEEK_END) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek to end %s error,reason:%s",m_chName,strerror(errno));
        printf("%s\n",m_chError);
	}

	return FALSE;
}

/*************************************************************
 �� �� ����GetLength()
 ���ܸ�Ҫ������ļ��ܳ���
 �� �� ֵ: long �ļ������ֽ���
 ��    ������
***************************************************************/
long CXJFile::GetLength()
{
	long num = 0;

	int r=0;

	//���ڱ����ļ���ǰλ�õı���
	fpos_t  curpos;

	if(pfile == NULL) 
	{
		sprintf(m_chError,"get %s length failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return -1;
	}
    
	//���浱ǰ�ļ�λ��
	if( fgetpos( pfile, &curpos ) != 0 )
	{
		sprintf(m_chError,"get file cur position failed when get %s's length,\
               rason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
		return -1;		    
	}

    //�ƶ��ļ����ײ�
	if( !SeekToBegin() )
	{
		return -1;
	}

	// �����λ��ƫ��
	long n_begin = ftell(pfile);

	// �ƶ����ļ�β��
	if( !SeekToEnd() )
	{
		fsetpos(pfile,&curpos);

		return -1;
	}

	// ���βλ��ƫ��
	long n_end = ftell(pfile);

	// ����ļ�����
	num = n_end - n_begin;

	//�ƶ��ļ�ָ�뵽ԭ��λ��
	if( fsetpos(pfile,&curpos) != 0)
	{
		sprintf(m_chError,"set file cur position failed when get %s's length,\
               rason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
		return num;		  
	}

	return num;
}	

/*************************************************************
 �� �� ����GetError()
 ���ܸ�Ҫ����ô���ԭ��
 �� �� ֵ: char * ��������
 ��    ������
***************************************************************/
char *CXJFile::GetError()
{
	return m_chError;
}
