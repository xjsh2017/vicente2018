// LogFile.cpp: implementation of the CString class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "LogFile.h"


//////////////////////////////////////////////////////////////////////////
//                  Class  CLogFile   实现                                    
//////////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
	m_File = NULL;
	bzero(m_FileName,FILE_NAME_MAX_LEN);
	bzero(m_szLogPath,FILE_NAME_MAX_LEN);

	//默认日志级别为 error级
	m_nLevel = CLogFile::error;

	// 默认日志文件大小
	m_nMaxSize  = MAX_LOG_FILE_SIZE;

	// 当前日志大小
	m_nCurrenSize = 0;

	//fprintf(stdout,"default log level=%d(error level) \n",m_nLevel);

	//为CLock指定输出调试信息时的名称
	m_Lock.init("CLogFile");
}


CLogFile::~CLogFile()
{
	// close file, if still opened
	Close();
	m_Lock.del();
}

/*************************************************************
 函 数 名：Open()
 功能概要：默认以日期作为文件名打开
 返 回 值: BOOL 
           FALSE : 失败
		   TRUE  : 成功
 参    数：无
***************************************************************/
BOOL CLogFile::Open()
{
    SYSTIME sysTime;

    GetSysTime(sysTime);
 
	char chFileName[FILE_NAME_MAX_LEN] = "";

	// create file name
    sprintf(chFileName,"%04d%02d%02d.txt",sysTime.wYear, sysTime.wMonth, sysTime.wDay);

    return (CLogFile::Open(chFileName));
}

/*************************************************************
 函 数 名：Open()
 功能概要：打开指定文件名称的文件
 返 回 值: BOOL 
           FALSE : 失败
		   TRUE  : 成功
 参    数：
           const char * pszFileName :指定的文件名
***************************************************************/
BOOL CLogFile::Open(const char * pszFileName)
{
	// get ownership
	if( !m_Lock.lock()){
		printf("get ownership failed when open log file:%s\n",pszFileName);
		return FALSE;
	}

	// if already open close !
	if (m_File != NULL)
		Close();

	int nPathLen = strlen(m_szLogPath);
	if(nPathLen > 0)
	{
		//create save path
		CreateSavePath(m_szLogPath);
	}

	//save filename
	bzero(m_FileName,FILE_NAME_MAX_LEN);
	int nNameLen =( ( strlen(pszFileName) > FILE_NAME_MAX_LEN ) ? FILE_NAME_MAX_LEN : strlen(pszFileName) );
	strncpy(m_FileName, pszFileName,nNameLen);

	//path + name
    char chFullPath[FILE_NAME_MAX_LEN] = "";
	sprintf(chFullPath,"%s%s",m_szLogPath,pszFileName);

	// new memory	
	m_File = new CXJFile;
	if(m_File == NULL){
		m_Lock.unlock();
		fprintf(stderr,"allocate memory for CXJFile failed when open '%s'\n",m_FileName);
		return FALSE;
	}

    // open file
	if (!m_File->Open(chFullPath,CXJFile::modeWrite|CXJFile::modeCreate))
    {	
		// failed to open file
        fprintf(stderr,"can't open log file'%s'\n",m_FileName);
        delete m_File;
        m_File = NULL;

		m_Lock.unlock();
		return FALSE;
    }

	// init the current size
	m_nCurrenSize = m_File->GetLength();

	m_Lock.unlock();
	
	return TRUE;
}

/*************************************************************
 函 数 名：Close()
 功能概要：关闭文件
 返 回 值: 关闭成功或失败
 参    数：无
***************************************************************/
bool CLogFile::Close()
{
	// get ownership
	if( !m_Lock.lock()){
		printf("get ownership failed when close log file:%s\n",m_FileName);
		return false;
	}
	
    // file not open...
	if (m_File == NULL){
		m_Lock.unlock();	
		return true;
	}

	// close file
	m_File->Close();

	// delete class
    delete m_File;
    m_File = NULL;

	//释放互斥体使用权
    m_Lock.unlock();

	return true;
}

/*************************************************************
 函 数 名：Flush()
 功能概要：写入日志
 返 回 值: void
 参    数：
           const char * data : 要写入的内容
		   nCurLevel         : 当前日志级别
***************************************************************/
void CLogFile::Flush(const char *data,int nCurLevel)
{
	// file not opened
	if (m_File == NULL)
		return;

	// assert the file exist
	//path + name
    char chFullPath[FILE_NAME_MAX_LEN] = "";
	sprintf(chFullPath,"%s%s",m_szLogPath,m_FileName);

	if(xj_pathfile_exist(chFullPath) != 0){
        
		printf("file:%s not exist,recreate\n",chFullPath);

		Close();

		char chFileName[FILE_NAME_MAX_LEN] = "";
		sprintf(chFileName,"%s",m_FileName);

		if(!CLogFile::Open(chFileName))
			return;

		printf("file:%s recreate success\n",chFileName);
	}

	//get sys time
	char  szDate[20] = "";
	SYSTIME sysTime;
	
	GetSysTime(sysTime);
	sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,\
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	// get log level
	char szLogLevel[20]="";
	switch(nCurLevel)
	{
	case CLogFile::error:        
		strcpy(szLogLevel,"错误级:");
		break;
	case CLogFile::trace:
		strcpy(szLogLevel,"跟踪级:");
		break;
	case CLogFile::warning:
		strcpy(szLogLevel,"警告级:");
		break;
    default:
		strcpy(szLogLevel,"无级别:");
		break;
	}


	// get ownership
    if( !m_Lock.lock() ){
		//printf("get ownership failed when wirte log to %s,give up wirte\n",m_FileName);
		return;
	}
	
	// 防止文件被其他线程关闭
	if (m_File == NULL){
		m_Lock.unlock();	
		return;
	}

	// assert the file size if valid
	if(m_nCurrenSize >= m_nMaxSize){
		if (!Clear()){
			m_Lock.unlock();
			return;
		}
	}

	UINT n =0;

	try{
// 		// write the log level
// 		n = m_File->Write(szLogLevel,strlen(szLogLevel));
// 		m_nCurrenSize += n;
// 
// 		// write time to file
// 		n = m_File->Write(szDate,19);
// 		m_nCurrenSize += n;
// 
// 		// write ": " to file
// 		n = m_File->Write(":  ",3);
// 		m_nCurrenSize += n;
// 		
// 		// write data to file
// 		n = m_File->Write(data, strlen(data));
// 		m_nCurrenSize += n;
// 		
// 		// write \r\n to file
// 		n = m_File->Write("\r\n",2);
// 		m_nCurrenSize += n;

		string strContext(szLogLevel);
		strContext += szDate;
		strContext += ":  ";
		strContext += data;
		strContext += "\r\n";
		n = m_File->Write(strContext.c_str(),strContext.size());
		m_File->Flush();
		m_nCurrenSize += n;
	}
	catch(...)
	{
		m_Lock.unlock();
		return;
	}

	//release ownership
    m_Lock.unlock();
}

/*************************************************************
 函 数 名：Flush()
 功能概要：写入指定长度的日志
 返 回 值: void
 参    数：
           const char * data : 要写入的内容
		   nLen      : 日志长度
		   nCurLevel : 当前日志级别
***************************************************************/
void CLogFile::Flush( const char* data,int nLen,int nCurLevel)
{
	// file not opened
	if (m_File == NULL)
		return;

	// assert the file exist

	//path + name
    char chFullPath[FILE_NAME_MAX_LEN] = "";
	sprintf(chFullPath,"%s%s",m_szLogPath,m_FileName);

	if(xj_pathfile_exist(chFullPath) != 0){
        
		printf("file:%s not exist,recreate\n",chFullPath);

		Close();

		char chFileName[FILE_NAME_MAX_LEN] = "";
		sprintf(chFileName,"%s",m_FileName);

		if(!CLogFile::Open(chFileName))
			return;

		printf("file:%s recreate success\n",chFileName);
	}

	//get sys time
	char  szDate[20] = "";
	SYSTIME sysTime;
	
	GetSysTime(sysTime);
	sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear,sysTime.wMonth,sysTime.wDay,\
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	// get log level
	char szLogLevel[20]="";
	switch(nCurLevel)
	{
	case CLogFile::error:        
		strcpy(szLogLevel,"错误级:");
		break;
	case CLogFile::trace:
		strcpy(szLogLevel,"跟踪级:");
		break;
	case CLogFile::warning:
		strcpy(szLogLevel,"警告级:");
		break;
    default:
		strcpy(szLogLevel,"无级别:");
		break;
	}


	// get ownership
    if( !m_Lock.lock() ){ 
		//printf("get ownership failed when wirte log to %s,give up wirte\n",m_FileName);
		return;
	}

	// 防止文件被其他线程关闭
	if (m_File == NULL){
		m_Lock.unlock();	
		return;
	}

	// assert the file size if valid
	if(m_nCurrenSize >= m_nMaxSize){
		if (!Clear()){
			m_Lock.unlock();
			return;
		}
	}

	UINT n =0;

	try{
// 		// write the log level
// 		n = m_File->Write(szLogLevel,strlen(szLogLevel));
// 		m_nCurrenSize += n;
// 
// 		// write time to file
// 		n = m_File->Write(szDate,19);
// 		m_nCurrenSize += n;
// 
// 		// write ": " to file
// 		n = m_File->Write(":  ",3);
// 		m_nCurrenSize += n;
// 		
// 		// write data to file
// 		n = m_File->Write(data, nLen);
//         m_nCurrenSize += n;
// 		
// 		// write \r\n to file
// 		n = m_File->Write("\r\n",2);
// 		m_nCurrenSize += n;
		
		string strContext(szLogLevel);
		strContext += szDate;
		strContext += ":  ";
		strContext.append(data,nLen);
		strContext += "\r\n";
		n = m_File->Write(strContext.c_str(),strContext.size());
		m_File->Flush();
		m_nCurrenSize += n;
	}
	catch(...)
	{
		m_Lock.unlock();
		return;
	}
	
	//release ownership
    m_Lock.unlock();
}

/*************************************************************
 函 数 名：FormatAdd()
 功能概要：格式化写入(仅支持常用的s,d,l,u,f,c格式)
 返 回 值: int 成功返回写入直接数据，失败返回-1
 参    数：
           int          nCurLevel    : 该条日志的级别
           const char * formatString : 要写入的format串
		   ...                       : 可变参数列表
***************************************************************/
int CLogFile::FormatAdd(int nCurLevel,const char * formatString, ...)
{
	// if the format string is NULL ,return 
	if (formatString == NULL)
	{
		return -1;
	}
		
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

	// allocate memory
	//char * pchar = new char[nMaxLen+1];

	//if(pchar == NULL)
	//	return 0;
	nMaxLen += 255;       // 防止特殊情况长度计算错误；
    string strDes=""; 
	strDes.resize(nMaxLen); 

	try{
		// get parament
		va_start(argList, formatString);  
		
		// format
		//vsprintf(pchar, formatString, argList);
        /*
        * ----- commented by qingch   3/12/2009 ------
          vsprintf((char*)strDes.c_str(), formatString, argList);
        */
		vsprintf((char*)&strDes[0], formatString, argList);
		// if the curlevel >= setted level,not write 
		// the number of level more bigger indicate the level is more lower
		if(nCurLevel <= m_nLevel)
		{
			// tty output	
			//printf("%s\n",pchar);
			printf("%s\n",strDes.c_str());
			
			// write file
			//Flush(pchar);
			Flush(strDes.c_str(),nCurLevel);
		}
		
		//if(pchar != NULL)
		//	delete [] pchar;
		
		va_end(argList);
	}
	catch(...)
	{
		return 0;
	}

	return nMaxLen;
}

/*************************************************************
 函 数 名：SetLogLevel()
 功能概要：设置日志级别
 返 回 值: bool 设置成功或失败
 参    数：
           int nLevel : 当前日志级别
**************************************************************/
bool CLogFile::SetLogLevel(int nLevel)
{
	// get ownership
    if( !m_Lock.lock() )
	{	
		return false;
	}

	m_nLevel = nLevel;

	m_Lock.unlock();

	return true;
}

/*************************************************************
 函 数 名：GetLogLevel()
 功能概要：获得设置的日志级别
 返 回 值: 
           int : 日志级别
 参    数：无
**************************************************************/
int CLogFile::GetLogLevel()
{
	int nLogLevel = -1;
	// get ownership
    if( !m_Lock.lock() )
	{	
		return nLogLevel;
	}

	nLogLevel = m_nLevel;

	m_Lock.unlock();

	return nLogLevel;
}

/*************************************************************
 函 数 名：SetLogPath()
 功能概要：设置日志存放路径
 返 回 值: bool 设置成功或失败
 参    数：
           const char * pszPath : 路径名
**************************************************************/
bool CLogFile::SetLogPath(const char * pszPath)
{
	// get ownership
    if( !m_Lock.lock() )
	{	
		return false;
	}

	bzero(m_szLogPath,FILE_NAME_MAX_LEN);

	strcpy(m_szLogPath,pszPath);
	const char * pchar = m_szLogPath;

	int nlen = strlen(m_szLogPath);
	if(nlen > 0)
	{
		if( (strncmp( (pchar+nlen-1),"/",1) != 0) && 
			(strncmp( (pchar+nlen-1),"\\",1) != 0) )
		{
			m_szLogPath[nlen]='/';
			m_szLogPath[nlen+1] = '\0';
		}
	}

	m_Lock.unlock();

	return true;
}

/*************************************************************
 函 数 名：SetLogSize()
 功能概要：设置日志文件大小(单位:Kb)
 返 回 值: bool 设置成功或失败
 参    数：
**************************************************************/
bool CLogFile::SetLogSize(long nSize)
{
	// get ownership
    if( !m_Lock.lock() )
	{	
		return false;
	}

	if(nSize >= 0)
		m_nMaxSize = nSize * 1024;	

	m_Lock.unlock();

	return true;
}

/*************************************************************
 函 数 名：Clear()
 功能概要：清除日志内容
 返 回 值: bool 清除成功或失败
 参    数：无
**************************************************************/
bool CLogFile::Clear()
{
	// get ownership
    if( !m_Lock.lock() )
	{	
		return false;
	}

    // close file before deleting
    if (m_File == NULL)
	{
		m_Lock.unlock();
		return false;
	}

	// close file
	m_File->Close();

	/*
	* ----- commented by qingch   6/1/2009 ------
  	// delete class
      delete m_File;
      m_File = NULL;
	*/

	//path + fileName
    char chFullPath[FILE_NAME_MAX_LEN] = "";
	sprintf(chFullPath,"%s%s",m_szLogPath,m_FileName);

    /*
    * ----- commented by qingch   6/1/2009 ------
      // remove log file
      if(remove(chFullPath) < 0)
  	{
  		m_Lock.unlock();
  		fprintf(stderr,"del log file %s failed,reason:%s\n",chFullPath,strerror(errno));
  		return false;
  	
	  }
	*/

    // open file
	if (!m_File->Open(chFullPath,CXJFile::modeCreate))
    {	
		// failed to open file
        fprintf(stderr,"can't open log file'%s'\n",m_FileName);
		m_Lock.unlock();
		return FALSE;
    }

	// init the current size
	m_nCurrenSize = m_File->GetLength();

	m_Lock.unlock();


    /*
    * ----- commented by qingch   6/1/2009 ------
      // create new file
      bzero(chFullPath,FILE_NAME_MAX_LEN);
  	strcpy(chFullPath,m_FileName);
  	Open(chFullPath);  
    */

	return true;
}

/*************************************************************
 函 数 名：Add()
 功能概要：按指定的级别写入字符串
 返 回 值: void
 参    数：
           const char * pchLog : 要写入的字符串
		   int       nCurLevle : 该条日志级别
**************************************************************/
void CLogFile::Add(const char * pchLog,int nCurLevel)
{
	
    // if the curlevel >= setted level,not write 
	// the number of level more bigger indicate the level is more lower
	if(nCurLevel <= m_nLevel)
	{
		//tty output
	    printf("%s\n",pchLog);

		Flush(pchLog,nCurLevel);
	}
}

/*************************************************************
 函 数 名：AddFixLen()
 功能概要：按照指定的长度记录指定缓冲区的内容
 返 回 值: void
 参    数：
           const char * pchLog : 要写入的字符串
		   int       nLen      : 内容长度
		   int       nCurLevle : 该条日志级别
**************************************************************/
void CLogFile::AddFixLen(const char * pchLog,int nLen,int nCurLevel)
{
	if( (nLen <= 0) || (pchLog == NULL))
		return ;

	// if the curlevel >= setted level,not write 
	// the number of level more bigger indicate the level is more lower
	if(nCurLevel <= m_nLevel)
	{
		//tty output
	    printf("%s\n",pchLog);

		Flush(pchLog,nLen,nCurLevel);
	}

}