/********************************************************************
	created:	2008/10/07
	created:	7:10:2008   23:49
	file base:	MessageLog
	file ext:	cpp
	author:		qingch
	purpose:	
*********************************************************************/

#include "MessageLog.h"

/************************************
 �� �� ���� CompareDiskFileTime
 ���ܸ�Ҫ�� �Ƚ��ļ�1��������ʱ���Ƿ���ļ�2����
 �� �� ֵ:  bool true:�� false:��
 ��    ���� const FILE_DES_STRUCT & lpFileTime1 �ļ�1�Ľṹ
 ��    ���� const FILE_DES_STRUCT & lpFileTime2 �ļ�2�Ľṹ
************************************/
bool CompareDiskFileTime(const FILE_DES_STRUCT &lpFileTime1, const FILE_DES_STRUCT &lpFileTime2 )
{
	if (lpFileTime1.tFileTime < lpFileTime2.tFileTime)
	{
		return true;
	}
	return false;
}

/*************************************************************
 �� �� ��:   CMessageLog
 ���ܸ�Ҫ:   ���캯��
 �� �� ֵ:   
**************************************************************/
CMessageLog::CMessageLog()
:m_iLogSaveDays(10),m_strLogRootPath("Xj_Log/"),m_strLogName("MessageLog"),m_Lock("CMessageLog")
{
	bzero(m_szLogPath,FILE_NAME_MAX_LEN);

	//get sys time
	SYSTIME sysTime;
	GetSysTime(sysTime);

	char  szTemp[20] = "";

  	//��ʼ�������������������Ŀ¼
  	sprintf(szTemp,"%04d-%02d-%02d",sysTime.wYear,sysTime.wMonth,sysTime.wDay);
  	m_strRelativePath = szTemp;

	//��ʼ����Сʱ�������ļ���
	bzero(szTemp,20);
	sprintf(szTemp,"%02dʱ.txt",sysTime.wHour);
	m_strFileName = szTemp;
}

/*************************************************************
 �� �� ��:   ~CMessageLog
 ���ܸ�Ҫ:   ��������
 �� �� ֵ:   
**************************************************************/
CMessageLog::~CMessageLog()
{

}

/*************************************************************
 �� �� ��:   Open
 ���ܸ�Ҫ:   ����־��������ָ��host����Ĭ��ΪMessageLog
 �� �� ֵ:   BOOL true-�ɹ� false-ʧ��
**************************************************************/
BOOL CMessageLog::Open()
{
	CLockUp localLock(&m_Lock);
// 	if (!Close())
// 		return FALSE;

	string strFullPath(GetFormatPath(m_strLogRootPath));
	strFullPath += GetFormatPath(m_strLogName);
	strFullPath += GetFormatPath(m_strRelativePath);

	if (strFullPath.size() >= FILE_NAME_MAX_LEN)
	{
		printf("���ɵ���־·������\n");
		return FALSE;
	}

	bzero(m_szLogPath,FILE_NAME_MAX_LEN);
	sprintf(m_szLogPath,"%s",strFullPath.c_str());
	CLogFile::SetLogPath(m_szLogPath);

	return (CLogFile::Open(m_strFileName.c_str()));
}

/*************************************************************
 �� �� ��:   Open
 ���ܸ�Ҫ:   ����־�������ɲ���ָ��host��
 �� �� ֵ:   BOOL true-�ɹ� false-ʧ��
 ��    ��:   const char * pszHostName ָ����host��
**************************************************************/
BOOL CMessageLog::Open( const char * pszHostName )
{
	CLockUp localLock(&m_Lock);
	//������־�ļ�������
	m_strLogName = pszHostName;
	return Open();
}

/*************************************************************
 �� �� ��:   SetLogPath
 ���ܸ�Ҫ:	 ������־��Ŀ¼���������ɵ���־�ļ��м��ļ�����ĸ�Ŀ¼
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   const char * pszPath ��Ÿ�Ŀ¼
**************************************************************/
bool CMessageLog::SetLogPath( const char * pszPath )
{
	CLockUp localLock(&m_Lock);
	m_strLogRootPath = pszPath;
	return true;
}

bool CMessageLog::SetHostName( const char * pHostName )
{
	CLockUp localLock(&m_Lock);
	//������־�ļ�������
	m_strLogName = pHostName;	
	/*
	* ----- commented by qingch   8/30/2009 ------
  	return (Open()==TRUE)?true:false;	
	*/
	return true;
}

/*************************************************************
 �� �� ��:   SharedOperateStart
 ���ܸ�Ҫ:   ����ǰ��׼������������ʱ����¸�Ŀ¼�����ļ�����
			�ļ����仯ʱ���´��ļ�
 �� �� ֵ:   BOOL true-�ɹ� false-ʧ��
**************************************************************/
BOOL CMessageLog::SharedOperateStart()
{
	CLockUp localLock(&m_Lock);
	bool bPathChanged(false);
	bool bNameChanged(false);
	//get sys time
	SYSTIME sysTime;
	GetSysTime(sysTime);

	char  szTemp[20] = "";	
	sprintf(szTemp,"%04d-%02d-%02d",sysTime.wYear,sysTime.wMonth,sysTime.wDay);

	string strTempFullPath(GetFormatPath(m_strLogRootPath));
	strTempFullPath += GetFormatPath(m_strLogName);
	strTempFullPath += GetFormatPath(szTemp);

	if (strcmp(strTempFullPath.c_str(),m_szLogPath) != 0){//ȫ·�������ı� �����´� ��ɨ���ļ��� ������־
  		//�������������ɵ���Ŀ¼��
  		m_strRelativePath = szTemp;
		bPathChanged = true;
	}

	//��Сʱ���ɵ��ļ���
	bzero(szTemp,20);
	sprintf(szTemp,"%02dʱ.txt",sysTime.wHour);
	//�ļ����仯�����´�
	if (strcmp(m_strFileName.c_str(),szTemp) != 0)
	{
		//�����ļ���
		m_strFileName = szTemp;
		bNameChanged = true;
	}

	BOOL bResult(TRUE);
	if (bPathChanged || bNameChanged)
	{
		bResult = Open();
		if (bPathChanged && bResult)
		{
			//ɨ�衢������
			DoScanAndClean();
		}
	}
	return bResult;
}

/*************************************************************
 �� �� ��:   FormatAdd
 ���ܸ�Ҫ:   ����ָ���ļ����ʽ������
 �� �� ֵ:   int �ɹ�����д����������ʧ�ܷ���-1
 ��    ��:   int nCurLevel             ��־����
 ��    ��:   const char * formatString Ҫд���format��
 ��    ��:   ...                       �ɱ�����б�
**************************************************************/
int CMessageLog::FormatAdd( int nCurLevel,const char* formatString, ... )
{
	// if the format string is NULL ,return 
	if (formatString == NULL)
	{
		return -1;
	}

	if (!SharedOperateStart())
		return -1;	
	
	va_list argList;

	// Set va_list to the beginning of optional arguments
	va_start(argList, formatString);

	const char * ptr = formatString;
	char * str = NULL;

	//save the max len of the formatstring
	int    nMaxLen = 0;

	while(*ptr != '\0')
	{
		str = NULL;

		if(*ptr == '%')
		{
			switch(*(ptr+1))
			{
			case 's':
			case 'S':
				str = va_arg(argList,char*);

				if( NULL == str)
					nMaxLen ++;
				else
					nMaxLen += strlen(str);
				ptr++;
				break;

			case 'c':
			case 'C':
				va_arg(argList,char);
				nMaxLen +=2;
				ptr++;
				break;

			case 'd':
			case 'D':
				va_arg(argList, int);
				nMaxLen +=11;
				ptr++;
				break;

			case 'u':
			case 'U':
				va_arg(argList, unsigned int);
				nMaxLen +=10;
				ptr++;
				break;

			case 'l':
			case 'L':
				ptr++;
				if(*(ptr+1) == 'd')
				{
					va_arg(argList, long);
					nMaxLen +=11;
				}
				else if(*(ptr+1) == 'u')
				{
					va_arg(argList, unsigned long);
					nMaxLen +=10;
				}
				ptr++;
				break;
			case 'f':
			case 'F':
				va_arg(argList, double);
				nMaxLen += 31;
				ptr++;
				break;
			case 'x':
			case 'X':
				va_arg(argList, void*);
				nMaxLen += 2*sizeof(void*);
				ptr++;
				break;
			default:
				nMaxLen+=1;
			}
		} //  if(*ptr == '%')
		else
		{
			nMaxLen +=1;
		}
		// Increment pointer..
		ptr++;
	}

	// end va_list
	va_end(argList);

    string strDes=""; 
	strDes.resize(nMaxLen);

	try{
		// get parament
		va_start(argList, formatString);  
		
		vsprintf((char*)strDes.data(), formatString, argList);

		va_end(argList);

		CLogFile::Add(strDes.c_str(),nCurLevel);

		return nMaxLen;
	}
	catch(...)
	{
		return 0;
	}
}

/*************************************************************
 �� �� ��:   Add
 ���ܸ�Ҫ:   ����ָ���ļ��������ַ���
 �� �� ֵ:   void
 ��    ��:   const char * pchLog   ������ַ���
 ��    ��:   int nCurLevel         ��־����
**************************************************************/
void CMessageLog::Add( const char * pchLog,int nCurLevel/*= CLogFile::trace*/ )
{
	if (!SharedOperateStart())
		return ;	
	CLogFile::Add(pchLog,nCurLevel);	
}

/*************************************************************
 �� �� ��:   AddFixLen
 ���ܸ�Ҫ:   /����ָ���ĳ��ȼ�¼ָ��������������
 �� �� ֵ:   void
 ��    ��:   const char * pchLog	Ҫд����ַ���
 ��    ��:   int nLen				���ݳ���
 ��    ��:   int nCurLevel			������־����
**************************************************************/
void CMessageLog::AddFixLen( const char * pchLog,int nLen,int nCurLevel /*= CLogFile::trace*/ )
{
	if (!SharedOperateStart())
		return ;	
	CLogFile::AddFixLen(pchLog,nLen,nCurLevel);		
}

/*************************************************************
 �� �� ��:   SetLogSaveDays
 ���ܸ�Ҫ:   ������־��������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   int pSaveDays ��־��������
**************************************************************/
bool CMessageLog::SetLogSaveDays( int pSaveDays )
{
	if ((pSaveDays<1)||(pSaveDays>30))
	{
		return false;
	}
	m_iLogSaveDays = pSaveDays;
	return true;
}

/*************************************************************
 �� �� ��:   DoScanAndClean
 ���ܸ�Ҫ:   ��־�ļ�ɨ�衢������
 �� �� ֵ:   void
**************************************************************/
void CMessageLog::DoScanAndClean()
{
	deque<FILE_DES_STRUCT> m_DirList;

    ScanLogPath(m_DirList);
	sort(m_DirList.begin(),m_DirList.end(),CompareDiskFileTime);

	while(m_DirList.size() > m_iLogSaveDays){
		CleanDir(m_DirList.front().strFilePath.c_str());
		m_DirList.pop_front();
	}
}

/*************************************************************
 �� �� ��:   ScanLogPath
 ���ܸ�Ҫ:   ɨ�豾��־����·���µİ���洢����־�ļ������
 �� �� ֵ:   void
 ��    ��:   deque<string> & m_DirList ɨ��õ����ļ����б�
**************************************************************/
void CMessageLog::ScanLogPath( deque<FILE_DES_STRUCT> &m_DirList )
{
    string strPath(GetFormatPath(m_strLogRootPath));
	strPath += GetFormatPath(m_strLogName);
    FILE_DES_STRUCT fileDescNode;
#ifdef OS_WINDOWS
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;
	
    string DirSpec(strPath);
	DirSpec += "*";
	hFind = FindFirstFile(DirSpec.c_str(), &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf ("Invalid file handle. Error is %u ", GetLastError());
		return ;
	}
	else{
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
			&& strcmp(FindFileData.cFileName, ".") != 0
			&& strcmp(FindFileData.cFileName, "..") != 0)
		{  
			string strDir(strPath);
			strDir += FindFileData.cFileName;
            fileDescNode.strFilePath = strDir;
            /*fileDescNode.tFileTime = (FindFileData.ftCreationTime.dwHighDateTime << 32)\
                + FindFileData.ftCreationTime.dwLowDateTime;*/
	      ULARGE_INTEGER uul;
				uul.HighPart = FindFileData.ftCreationTime.dwHighDateTime;
				uul.LowPart = FindFileData.ftCreationTime.dwLowDateTime;
				fileDescNode.tFileTime = uul.QuadPart/10000000-11644473600;
			                
			m_DirList.push_back(fileDescNode);
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
				&& strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0)
			{ //�ҵ�Ŀ¼
				string strDir(strPath);
				strDir += FindFileData.cFileName;
                fileDescNode.strFilePath = strDir;
                /*fileDescNode.tFileTime = (FindFileData.ftCreationTime.dwHighDateTime << 32)\
                + FindFileData.ftCreationTime.dwLowDateTime;*/
	      ULARGE_INTEGER uul;
				uul.HighPart = FindFileData.ftCreationTime.dwHighDateTime;
				uul.LowPart = FindFileData.ftCreationTime.dwLowDateTime;
				fileDescNode.tFileTime = uul.QuadPart/10000000-11644473600;
			        
				m_DirList.push_back(fileDescNode);
			}
		}
		
		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES)
		{
			printf ("FindNextFile error. Error is %u ", dwError);
			return;
		}
	}
#endif

#ifdef OS_LINUX
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp = opendir(strPath.c_str())) == NULL) {
        printf("cannot open directory: %s\n", strPath.c_str());
        return;
    }
    while((entry = readdir(dp)) != NULL) {
        string strDir(strPath);
        strDir += entry->d_name;
        if (lstat(strDir.c_str(),&statbuf) < 0){
            perror("lstat��������,����ԭ��:");
        }
        else{
            if(S_ISDIR(statbuf.st_mode)
               && (strcmp(".",entry->d_name) != 0)
               && (strcmp("..",entry->d_name) != 0)) {
                /**//* Found a directory, but ignore . and .. */
                fileDescNode.strFilePath = strDir;
                fileDescNode.tFileTime = statbuf.st_ctime;
                m_DirList.push_back(fileDescNode);
            }
        }
    }
    closedir(dp);
#endif

	return ;
}

/*************************************************************
 �� �� ��:   CleanDir
 ���ܸ�Ҫ:   ���ָ���ļ���
 �� �� ֵ:   void
 ��    ��:   const string & pPathName ָ�����ļ���·��
**************************************************************/
void CMessageLog::CleanDir( const string& pPathName )
{
	string strPathName(GetFormatPath(pPathName));
#ifdef OS_WINDOWS
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;

	string DirSpec(strPathName);
    DirSpec += "*";
	hFind = FindFirstFile(DirSpec.c_str(), &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf ("Invalid file handle. Error is %u ", GetLastError());
		return ;
	}
	else{
        if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			string strFileName = strPathName + FindFileData.cFileName;
            //�ҵ��ļ� ɾ��
			if (0 == remove(strFileName.c_str()))
			{
				printf("ɾ���ļ�:%s\n",strFileName.c_str());
			} 
			else
			{
				printf("ɾ���ļ�:%s ʧ��\n",strFileName.c_str());
			}
        }
        else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
			&& strcmp(FindFileData.cFileName, ".") != 0
			&& strcmp(FindFileData.cFileName, "..") != 0)
		{  
			string strTempPath = strPathName + FindFileData.cFileName;
			CleanDir(strTempPath);
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0)
		{
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
			{
				string strFileName = strPathName + FindFileData.cFileName;
				//�ҵ��ļ� ɾ��
				if (0 == remove(strFileName.c_str()))
				{
					printf("ɾ���ļ�:%s\n",strFileName.c_str());
				} 
				else
				{
					printf("ɾ���ļ�:%s ʧ��\n",strFileName.c_str());
				}
			}
			else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
				&& strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0)
			{ //�ҵ�Ŀ¼
				string strTempPath = strPathName + FindFileData.cFileName;
				CleanDir(strTempPath);
			}
		}
		
		dwError = GetLastError();
		FindClose(hFind);

		if (dwError != ERROR_NO_MORE_FILES)
		{
			printf ("FindNextFile error. Error is %u ", dwError);
			return;
		}

        if (0 == _rmdir(pPathName.c_str()))
        {
            printf("ɾ���ļ���:%s\n",pPathName.c_str());
        } 
        else
        {
            printf("ɾ���ļ���:%s ʧ��\n",pPathName.c_str());
	    }
	}
#endif

#ifdef OS_LINUX
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp = opendir(strPathName.c_str())) == NULL) {
        printf("cannot open directory: %s\n", strPathName.c_str());
        return;
    }

    while((entry = readdir(dp)) != NULL) {
        string strFileNodeName = strPathName + entry->d_name;
        if (lstat(strFileNodeName.c_str(),&statbuf) < 0){
            perror("lstat��������,����ԭ��:");
        }
        else{
            if(!S_ISDIR(statbuf.st_mode)){
                //�ҵ��ļ� ɾ��
                if (0 == remove(strFileNodeName.c_str()))
                {
                    printf("ɾ���ļ�:%s\n",strFileNodeName.c_str());
                } 
                else
                {
                    printf("ɾ���ļ�:%s ʧ��\n",strFileNodeName.c_str());
			    }
            }
            else if((strcmp(".",entry->d_name) != 0)
                && (strcmp("..",entry->d_name) != 0)) {
                /**//* Found a directory, but ignore . and .. */
                    CleanDir(strFileNodeName);
            }
        }
    }

    closedir(dp);

    if (0 == rmdir(pPathName.c_str()))
    {
        printf("ɾ���ļ���:%s\n",pPathName.c_str());
    } 
    else
    {
        printf("ɾ���ļ���:%s ʧ��\n",pPathName.c_str());
	}
#endif

	return ;	
}