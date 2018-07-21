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
 函 数 名： CompareDiskFileTime
 功能概要： 比较文件1的最后访问时间是否比文件2的早
 返 回 值:  bool true:是 false:否
 参    数： const FILE_DES_STRUCT & lpFileTime1 文件1的结构
 参    数： const FILE_DES_STRUCT & lpFileTime2 文件2的结构
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
 函 数 名:   CMessageLog
 功能概要:   构造函数
 返 回 值:   
**************************************************************/
CMessageLog::CMessageLog()
:m_iLogSaveDays(10),m_strLogRootPath("Xj_Log/"),m_strLogName("MessageLog"),m_Lock("CMessageLog")
{
	bzero(m_szLogPath,FILE_NAME_MAX_LEN);

	//get sys time
	SYSTIME sysTime;
	GetSysTime(sysTime);

	char  szTemp[20] = "";

  	//初始化已日期命名的相对子目录
  	sprintf(szTemp,"%04d-%02d-%02d",sysTime.wYear,sysTime.wMonth,sysTime.wDay);
  	m_strRelativePath = szTemp;

	//初始化以小时命名的文件名
	bzero(szTemp,20);
	sprintf(szTemp,"%02d时.txt",sysTime.wHour);
	m_strFileName = szTemp;
}

/*************************************************************
 函 数 名:   ~CMessageLog
 功能概要:   析构函数
 返 回 值:   
**************************************************************/
CMessageLog::~CMessageLog()
{

}

/*************************************************************
 函 数 名:   Open
 功能概要:   打开日志方法，不指定host名，默认为MessageLog
 返 回 值:   BOOL true-成功 false-失败
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
		printf("生成的日志路径超长\n");
		return FALSE;
	}

	bzero(m_szLogPath,FILE_NAME_MAX_LEN);
	sprintf(m_szLogPath,"%s",strFullPath.c_str());
	CLogFile::SetLogPath(m_szLogPath);

	return (CLogFile::Open(m_strFileName.c_str()));
}

/*************************************************************
 函 数 名:   Open
 功能概要:   打开日志方法，由参数指定host名
 返 回 值:   BOOL true-成功 false-失败
 参    数:   const char * pszHostName 指定的host名
**************************************************************/
BOOL CMessageLog::Open( const char * pszHostName )
{
	CLockUp localLock(&m_Lock);
	//报文日志文件夹名称
	m_strLogName = pszHostName;
	return Open();
}

/*************************************************************
 函 数 名:   SetLogPath
 功能概要:	 设置日志根目录，设置生成的日志文件夹及文件保存的根目录
 返 回 值:   bool true-成功 false-失败
 参    数:   const char * pszPath 存放根目录
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
	//报文日志文件夹名称
	m_strLogName = pHostName;	
	/*
	* ----- commented by qingch   8/30/2009 ------
  	return (Open()==TRUE)?true:false;	
	*/
	return true;
}

/*************************************************************
 函 数 名:   SharedOperateStart
 功能概要:   操作前的准备工作，根据时间更新各目录名，文件名。
			文件名变化时重新打开文件
 返 回 值:   BOOL true-成功 false-失败
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

	if (strcmp(strTempFullPath.c_str(),m_szLogPath) != 0){//全路径发生改变 需重新打开 需扫描文件夹 清理日志
  		//更新以日期生成的子目录名
  		m_strRelativePath = szTemp;
		bPathChanged = true;
	}

	//以小时生成的文件名
	bzero(szTemp,20);
	sprintf(szTemp,"%02d时.txt",sysTime.wHour);
	//文件名变化，重新打开
	if (strcmp(m_strFileName.c_str(),szTemp) != 0)
	{
		//更新文件名
		m_strFileName = szTemp;
		bNameChanged = true;
	}

	BOOL bResult(TRUE);
	if (bPathChanged || bNameChanged)
	{
		bResult = Open();
		if (bPathChanged && bResult)
		{
			//扫描、清理工作
			DoScanAndClean();
		}
	}
	return bResult;
}

/*************************************************************
 函 数 名:   FormatAdd
 功能概要:   按照指定的级别格式化输入
 返 回 值:   int 成功返回写入数据数，失败返回-1
 参    数:   int nCurLevel             日志级别
 参    数:   const char * formatString 要写入的format串
 参    数:   ...                       可变参数列表
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
 函 数 名:   Add
 功能概要:   按照指定的级别输入字符串
 返 回 值:   void
 参    数:   const char * pchLog   输入的字符串
 参    数:   int nCurLevel         日志级别
**************************************************************/
void CMessageLog::Add( const char * pchLog,int nCurLevel/*= CLogFile::trace*/ )
{
	if (!SharedOperateStart())
		return ;	
	CLogFile::Add(pchLog,nCurLevel);	
}

/*************************************************************
 函 数 名:   AddFixLen
 功能概要:   /按照指定的长度记录指定缓冲区的内容
 返 回 值:   void
 参    数:   const char * pchLog	要写入的字符串
 参    数:   int nLen				内容长度
 参    数:   int nCurLevel			该条日志级别
**************************************************************/
void CMessageLog::AddFixLen( const char * pchLog,int nLen,int nCurLevel /*= CLogFile::trace*/ )
{
	if (!SharedOperateStart())
		return ;	
	CLogFile::AddFixLen(pchLog,nLen,nCurLevel);		
}

/*************************************************************
 函 数 名:   SetLogSaveDays
 功能概要:   设置日志保存天数
 返 回 值:   bool true-成功 false-失败
 参    数:   int pSaveDays 日志保存天数
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
 函 数 名:   DoScanAndClean
 功能概要:   日志文件扫描、清理工作
 返 回 值:   void
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
 函 数 名:   ScanLogPath
 功能概要:   扫描本日志对象路径下的按天存储的日志文件夹情况
 返 回 值:   void
 参    数:   deque<string> & m_DirList 扫描得到的文件夹列表
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
			{ //找到目录
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
            perror("lstat发生错误,错误原因:");
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
 函 数 名:   CleanDir
 功能概要:   清除指定文件夹
 返 回 值:   void
 参    数:   const string & pPathName 指定的文件夹路径
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
            //找到文件 删除
			if (0 == remove(strFileName.c_str()))
			{
				printf("删除文件:%s\n",strFileName.c_str());
			} 
			else
			{
				printf("删除文件:%s 失败\n",strFileName.c_str());
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
				//找到文件 删除
				if (0 == remove(strFileName.c_str()))
				{
					printf("删除文件:%s\n",strFileName.c_str());
				} 
				else
				{
					printf("删除文件:%s 失败\n",strFileName.c_str());
				}
			}
			else if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY
				&& strcmp(FindFileData.cFileName, ".") != 0
				&& strcmp(FindFileData.cFileName, "..") != 0)
			{ //找到目录
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
            printf("删除文件夹:%s\n",pPathName.c_str());
        } 
        else
        {
            printf("删除文件夹:%s 失败\n",pPathName.c_str());
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
            perror("lstat发生错误,错误原因:");
        }
        else{
            if(!S_ISDIR(statbuf.st_mode)){
                //找到文件 删除
                if (0 == remove(strFileNodeName.c_str()))
                {
                    printf("删除文件:%s\n",strFileNodeName.c_str());
                } 
                else
                {
                    printf("删除文件:%s 失败\n",strFileNodeName.c_str());
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
        printf("删除文件夹:%s\n",pPathName.c_str());
    } 
    else
    {
        printf("删除文件夹:%s 失败\n",pPathName.c_str());
	}
#endif

	return ;	
}