#pragma warning(disable : 4275)
#pragma warning(disable : 4786)//屏蔽4786错误
#include "define.h"
#include "XJString.h"

/*  全局函数实现  */

/*************************************************************
 函 数 名：GetSysTime()
 功能概要：获得系统当前时间
 返 回 值: void
 参    数：SYSTIME& curTime
***************************************************************/
void GetSysTime(SYSTIME & curTime)
{
	//存放本地时间
	time_t ltime=0;

	//tm 结构
	struct tm today = {0};

	//取得本地时间
	time(&ltime);

#ifdef OS_WINDOWS
	struct tm *tmp_tm = NULL;
	tmp_tm = localtime(&ltime);//时间转换为tm类型
	today = *tmp_tm;
#else
	localtime_r(&ltime, &today);
#endif
	
    //取值
	curTime.wYear      = today.tm_year + 1900;  //tm 中year 基于1900
	curTime.wMonth     = today.tm_mon + 1;      //tm 中month 基于0
	curTime.wDay       = today.tm_mday;
	curTime.wHour      = today.tm_hour;
	curTime.wMinute    = today.tm_min ;
	curTime.wSecond    = today.tm_sec ;
	curTime.wDayOfWeek = today.tm_wday ;        //sunday is 0 ,monday is 1 ,and so on
    curTime.wDayOfYear = today.tm_yday + 1;     //tm中基于0开始

	//不支持去毫秒,置0
	curTime.wMilliseconds = 0;
}

/*************************************************************
 函 数 名：CreateSavePath()
 功能概要: 创建路径
 返 回 值: void
 参    数：char * szPath   路径名
***************************************************************/
void CreateSavePath(char * szPath)  
{
	int nStart=0,nReturn = 0;
	CXJString strFilePath,strTempPath;

	strFilePath = szPath;

#ifdef OS_WINDOWS 
	nStart=strFilePath.Find('/');
	if(nStart < 0)  //仅有一级目录
	{
		nStart=strFilePath.Find('\\');
		if (nStart < 0)
		{
			::CreateDirectory(strFilePath,NULL); 
			return;
		}
		while(nStart+1)
		{
			if(nStart < 0) //当没有发现时返回-1；
			{
				::CreateDirectory(strFilePath,NULL); 
				return;
			}
			strTempPath=strFilePath.Left(nStart);
			
			//若路径不存在则创建，若存在则打开
			::CreateDirectory(strTempPath,NULL); 
			nStart=strFilePath.Find('\\',nStart+1);	
		}		
	}

	while(nStart+1)
	{
		if(nStart < 0) //当没有发现时返回-1；
		{
			::CreateDirectory(strFilePath,NULL); 
			return;
		}
		strTempPath=strFilePath.Left(nStart);

		//若路径不存在则创建，若存在则打开
		::CreateDirectory(strTempPath,NULL); 
		nStart=strFilePath.Find('/',nStart+1);	
	}
#endif

#ifdef OS_LINUX
    nStart=strFilePath.Find('/');
	if(nStart < 0)  //仅有一级目录
	{
		nReturn =mkdir((const char *)strFilePath,S_IRWXU | S_IRWXG | S_IRWXO); 
		if(nReturn < 0)
		{
			if(errno != EEXIST)
				fprintf(stderr,"create Directory '%s' failed,reason:%s \n",(const char *)strFilePath,strerror(errno));
		}
		return;
	}

	while(nStart+1)
	{
		if(nStart != 0 )  // 不为根路径时开始创建
		{
			strTempPath=strFilePath.Left(nStart);
			
			//若路径不存在则创建，若存在则打开
			nReturn =mkdir((const char *)strTempPath,S_IRWXU | S_IRWXG | S_IRWXO); 
			if(nReturn < 0)
			{
				if(errno != EEXIST){
					fprintf(stderr,"create Directory '%s' failed,reason:%s \n",(const char *)strFilePath,strerror(errno));
					return ;
				}
			}
		}
		nStart=strFilePath.Find('/',nStart+1);	
	}

#endif
}

/*************************************************************
 函 数 名：GetIniKey()
 功能概要: 获得指定配置文件中指定关键字的值
 返 回 值: void
 参    数：const char * filename  配置文件名称
           const * title     项目名称
		   const * key       关键字名称
		   const * chResult  取得的结果
***************************************************************/
void GetIniKey(const char *filename, const char *title,
				  const char *key,char *chResult)
{
	if(chResult == NULL)
	{
		fprintf(stderr,"the last parameter is NULL in GetIniKey()");
		return ;
	}

	//文件指针
	FILE * fp = NULL;

	//临时变量
	char tmpLine[1024]="";         // 每行最多字符数
	char every_line[1024]="";
	char tempKey[255]="";          // 保存临时关键字
	char tmpTitle[255]="";         // 保存历史项目名称
	char * pEveryLine = NULL;
   	char * pResult = NULL;

	//读取文件返回值
	char * pReturn = NULL;

	int i=0,j=0,k=0,nLen = 0;

	//标志一个项目下的key是否已经扫描完
	int flag = 0;

	char * tmp=NULL;
	
	//打开文件
	if ((fp = fopen( filename, "r")) == NULL )
	{
		fprintf(stderr,"Open File %s Failed,Reason:%s \n",filename,strerror(errno));
		return;
	}

	//读取内容
	while (!feof(fp))
	{
		memset(tmpLine,0,1024);
		memset(every_line,0,1024);

		pReturn = fgets(every_line,1023,fp );    //读取一行

		if ( pReturn == NULL )                //文件结束或发生错误
		{  
			break; 
		}
		
		nLen = strlen(every_line);
        pEveryLine = every_line;
        j = 0;

		// 去除行首空格(将第一个不为空格字符前的所有空格去除,)
		for(i =0; i<nLen; i++)
		{
			if(every_line[i] == 0x20)
			{
				j++;         // 空格数目
			}
			else             // 出现不为空格时则退出该循环
				break;
		}
        
		memcpy(tmpLine,pEveryLine+j,nLen-j);  // 复制除空格外的字符
        nLen = nLen - j;                      // 重新设置长度

		// 过滤注释行或空行
		if( ( tmpLine[0] == ';') ||                           // ';'号注释 
		    ((tmpLine[0] == '/') && (tmpLine[1] == '/')) ||   // '//'注释
		    ((tmpLine[0] == '/') && (tmpLine[1] == '*')) ||   // '/*'注释
			((tmpLine[0] == '-') && (tmpLine[1] == '-')) ||   // '--'注释
			((tmpLine[0] == '\r')&& (tmpLine[1] == '\n'))||   // windows 下换行符(无空格的空行)
			 (tmpLine[0] =='\n') )                            // linux/unix下换行符 (无空格的空行)
		{
			continue;
		}	

        // 去除换行符
		if( (nLen >= 1) && (tmpLine[nLen-1] == '\n') )      // windows\linux下换行符最后都为'\n'
			tmpLine[nLen-1] = 0;
		if( (nLen >= 2) && (tmpLine[nLen-2] == '\r') )      //  windows下换行符为在'\n'之前还有个'\r'
			tmpLine[nLen-2] = 0;

		tmp = strchr(tmpLine, '=');
		
		//第一次读到 '=' 应该是 项目名称
		if (( tmp != NULL )&&(flag == 1))
		{
			i= j = 0;
			memset(tempKey,0,255);

			//取出关键字
			while(tmpLine[j] != '=')
			{
				if(tmpLine[j] != 0x20)                        //去除空格
				{
					tempKey[i]=tmpLine[j];
					i++;

					if(i > 254)
						break;
				}
				j++;
			}
			
	    	//如果关键字相同则取其值 (去除结果中的空格)
			if(strcmp(tempKey,key) == 0)
			{
				nLen = strlen(tmp) - 1;
				tmp = tmp + 1;          // 跳过 '='号
                pResult = chResult;

				while( nLen > 0)
				{
					if( (*tmp) != 0x20 )
					{
						*pResult = *tmp;
						pResult ++;
					}

                    tmp ++;
					nLen --;
				}

				fclose ( fp );        // 关闭文件

				return ;
			}
		}
		else
		{
			//如果flag=1,则说明前一个title下的key已经读完
			if( flag == 1)
			{
				fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n", \
					   title,key,filename);
				bzero(chResult,strlen(chResult));
				fclose(fp);
				return ;
			}
			memset(tmpTitle,0,255);

			// 生成项目名称
			strcpy(tmpTitle,"[");
			strcat(tmpTitle,title);
			strcat(tmpTitle,"]");
			
			// 确认读到的信息是否有项目名称
			if( strstr(tmpLine,tmpTitle) != NULL )
			{
				flag = 1;   // 标识找到title
			}
		}
	}

	fclose ( fp );

	fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n",\
		    title,key,filename);
	bzero(chResult,strlen(chResult));
	return;
}

/*************************************************************
 函 数 名：EndThread()
 功能概要: 结束线程
 返 回 值: void
 参    数：无
***************************************************************/
void EndThread()
{
#ifdef OS_WINDOWS
				unsigned long code=0;
				ExitThread(code);
#endif

#ifdef OS_LINUX
				pthread_exit(NULL);
#endif
}

/*************************************************************
 函 数 名：MySleep()
 功能概要: 睡眠等待 支持毫秒等待
 返 回 值: void
 参    数：
           nmillseconds : 毫秒数
***************************************************************/
void MySleep(UINT nmilliseconds)
{
#ifdef OS_WINDOWS
	Sleep(nmilliseconds);
#endif

#ifdef OS_LINUX
     timeval tm;
     tm.tv_sec =  nmilliseconds / 1000;
     tm.tv_usec = (nmilliseconds % 1000) * 1000;
     int nret=select(0, 0, 0, 0, &tm);
#endif
}

/*************************************************************
 函 数 名：GotFileStatusInDir()
 功能概要: 查找指定目录下所有文件及文件信息（不包括下属文件夹及其中文件）
 返 回 值: void
 参    数：
           dir 路径， pFileStatusList, 文件信息结构体链表
***************************************************************/
void GetFileStatusInDir(char *dir, vector <FILE_STATUS_STRUCT> *pFileStatusList,int nLevel)
{
	pFileStatusList->clear();
// #ifdef OS_LINUX
//     DIR *dp;
//     struct dirent *entry;
//     struct stat statbuf;
// 	
//     if((dp = opendir(dir)) == NULL) {
//         fprintf(stderr,"cannot open directory: %s\n", dir);
//         return;
//     }
//     chdir(dir);
//     while((entry = readdir(dp)) != NULL) {
//         lstat(entry->d_name,&statbuf);
//         if(S_ISDIR(statbuf.st_mode)) 
// 		{
// 			// ignore Directory
// 			continue;																
//         }
//         else 
// 		{
// 			FILE_STATUS_STRUCT m_FileStatusStruct;
// 			memset(&m_FileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
// //			memset(m_FileStatusStruct.cFileName,0,100);
// 			memcpy(m_FileStatusStruct.cFileName,entry->d_name,100);
// 			m_FileStatusStruct.stWriteTime=statbuf.st_mtime;
// 			m_FileStatusStruct.nFileSize=statbuf.st_size;
// 			pFileStatusList->push_back(m_FileStatusStruct);
// 		}
//     }
// 	for(int i=0;i<nLevel;i++)
//     {
// 		chdir("..");
// 	}
// 	rewinddir(dp);
//    closedir(dp);
// #endif
#ifdef OS_LINUX
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
	char* full_path;
	
	if ((dir==NULL)||(pFileStatusList==NULL))
	{
		printf("GetFileStatusInDir error");
		return;
	}
	//打开目录
	printf("opendir<%s>\r\n",dir);
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    //遍历目录里文件
    while((entry = readdir(dp)) != NULL) {
        if ((strcmp(entry->d_name, ".") == 0)||strcmp(entry->d_name, "..") == 0)
			continue;
		int len = strlen(dir) + strlen( entry->d_name) + 10;
		full_path = new char[len];
		if (full_path==NULL)
		{
			return;
		}
		sprintf(full_path, "%s/%s", dir, entry->d_name);
		//printf(full_path);
		//取得文件状态
		int ret = stat( full_path, &statbuf);
		if (ret)
		{
			delete full_path;
			full_path = NULL;
			return;
		}
        
		if(S_ISDIR(statbuf.st_mode)) 
		{
			// ignore Directory
			delete full_path;
			continue;																
        }
        else 
		{
			FILE_STATUS_STRUCT m_FileStatusStruct;
			memset(&m_FileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
			strncpy(m_FileStatusStruct.cFileName,entry->d_name,100);
			m_FileStatusStruct.stWriteTime=statbuf.st_mtime;
			m_FileStatusStruct.nFileSize=statbuf.st_size;
			pFileStatusList->push_back(m_FileStatusStruct);
		}
		delete full_path;
    }
	
	printf("scan file system end!");
	rewinddir(dp);
    closedir(dp);
#endif
	
#ifdef OS_WINDOWS

	string path=dir;
	struct _finddata_t filefind;
    string curr=path+"*.*";
    int done=0,handle;
    if((handle=_findfirst(curr.c_str(),&filefind))==-1)return;
    while(!(done=_findnext(handle,&filefind)))
    {
        if(!strcmp(filefind.name,".."))continue;

        if ((_A_SUBDIR==filefind.attrib))
        {      
			//ignore Directory
			continue;
        }
        else 
        {
			FILE_STATUS_STRUCT m_FileStatusStruct;
			memset(&m_FileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
			
			strncpy(m_FileStatusStruct.cFileName,filefind.name,100);
			m_FileStatusStruct.nFileSize=filefind.size;
			m_FileStatusStruct.stWriteTime=filefind.time_write;
			pFileStatusList->push_back(m_FileStatusStruct);
        }
    }    
    _findclose(handle);     
#endif
}


int  GetFileStatus(char *filename, FILE_STATUS_STRUCT *pFileStatusStruct)
{
#ifdef OS_LINUX
	
    struct stat statbuf;
	
    if(lstat(filename,&statbuf)!=0)
		return 1;
	
	memset(pFileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
	strncpy(pFileStatusStruct->cFileName,GetFileNameFromFullFileName(filename).c_str(),100);
	pFileStatusStruct->stWriteTime=statbuf.st_mtime;
	pFileStatusStruct->nFileSize=statbuf.st_size;
	
#endif
	
#ifdef OS_WINDOWS
/*	struct _stat statbuf;

	int nRet=_stat(filename,&statbuf);
	//	if(_stat(filename,&statbuf)!=0)
	if(nRet!=0)
		return 1;
	
	memset(pFileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
	
	memcpy(pFileStatusStruct->cFileName,filename,100);
	pFileStatusStruct->stWriteTime=statbuf.st_atime;
	pFileStatusStruct->nFileSize=statbuf.st_size;*/

//	string path=dir;
	struct _finddata_t filefind;
 //   string curr=path+"*.*";
    int done=0,handle;
    if((handle=_findfirst(filename,&filefind))==-1)
		return 1;

	memset(pFileStatusStruct,0,sizeof(FILE_STATUS_STRUCT));
	
	memcpy(pFileStatusStruct->cFileName,filefind.name,100);
	pFileStatusStruct->nFileSize=filefind.size;
	pFileStatusStruct->stWriteTime=filefind.time_write;
	
    _findclose(handle);     

#endif

	return 0;
}

#ifndef MFC_ADO_USED_DEFINE

/*************************************************************
 函 数 名:   AddField
 功能概要:	 向指定的SQL_DATA中添加一个字段
 返 回 值:   void
 参    数:   SQL_DATA & sql
 参    数:   const char * pFieldName 字段名
 参    数:   int nType 字段类型
**************************************************************/
void AddField(SQL_DATA& sql, const char* pFieldName, int nType, const char* pFieldValue /*= NULL*/)
{
	Field field;
    bzero(&field, sizeof(Field));
	
    field.FieldType = nType;

    CXJString str(pFieldName);
    str.MakeUpper();
    strncpy(field.FieldName, str.c_str(), str.GetLength());
	
	if (NULL != pFieldValue){
		CXJString strValue(pFieldValue);
		//strValue.MakeUpper();
		strncpy(field.FieldValue, strValue.c_str(), strValue.GetLength());
	}
	
    sql.Fieldlist.push_back(field);
	
}

/*************************************************************
 函 数 名:   AddCondition
 功能概要:	 向指定的SQL_DATA中添加一个条件
 返 回 值:   void
 参    数:   SQL_DATA & sql
 参    数:   const char * pContent 条件内容
 参    数:   int nType 条件类型
**************************************************************/
void AddCondition(SQL_DATA& sql, const char* pContent, int nType /*= 0*/)

{
	Condition con;
    bzero(&con, sizeof(Condition));
	
    CXJString str(pContent);
	
    strncpy(con.ConditionContent, str.c_str(), str.GetLength());
	
	con.ConditionType = nType;

    sql.Conditionlist.push_back(con);
	
}

/*************************************************************
 函 数 名：Zero_SQL_DATA()
 功能概要：清空SQL_DATA
 返 回 值: 返回值说明
 参    数：param1   SQL_DATA引用
**************************************************************/
void Zero_SQL_DATA(SQL_DATA& pSqlData)
{
	int i = 0;
	//Field
	int FieldNum = pSqlData.Fieldlist.size();
	if (FieldNum > 0)
	{
		for (i = 0; i<FieldNum; i++)
		{
			pSqlData.Fieldlist[i].FieldType = 0;
			bzero(pSqlData.Fieldlist[i].FieldName,sizeof(pSqlData.Fieldlist[i].FieldName));
			bzero(pSqlData.Fieldlist[i].FieldValue,sizeof(pSqlData.Fieldlist[i].FieldValue));
		}
	}
	pSqlData.Fieldlist.clear();

	//Condition
	int ConditionNum = pSqlData.Conditionlist.size();
	if (ConditionNum > 0)
	{
		for (i= 0 ; i< ConditionNum; i++)
		{
			pSqlData.Conditionlist[i].ConditionType = 0;
			bzero(pSqlData.Conditionlist[i].ConditionContent,sizeof(pSqlData.Conditionlist[i].ConditionContent));
		}
	}
	pSqlData.Conditionlist.clear();
}

#endif

/*************************************************************
 函 数 名：Zero_RealData_Condition()
 功能概要：清空结构
 返 回 值: 无
 参    数：param1   结构引用
**************************************************************/
void Zero_RealData_Condition(REALDATA_CONDITION& pRealDataCondition)
{
	pRealDataCondition.IsUse      = false;//是否使用
	pRealDataCondition.station_id = "";//厂站ID
	pRealDataCondition.pt_id      = "";//保护ID
	pRealDataCondition.cpu_code   = "";//CPU号
	pRealDataCondition.breaker_id = "";//开关ID
	pRealDataCondition.reverse1   = "";//备用
	pRealDataCondition.reverse2   = "";//备用
}

/*************************************************************
 函 数 名:   GetFormatPath
 功能概要:   获得格式化的文件路径，将所有的斜杠统一为'/',最后没有'/'
			时加上'/'
 返 回 值:   std::string 格式化后的路径
 参    数:   string sourcePath 需格式化的路径
**************************************************************/
string GetFormatPath(string sourcePath)
{
	string::size_type stPos(0);
	//
	while(string::npos != (stPos = sourcePath.find('\\',stPos))){
		sourcePath.replace(stPos,1,"/");
	}

	if((!sourcePath.empty()) && ((*(sourcePath.end()-1) != '/')))
	{
		sourcePath += '/';
  	}
	return sourcePath;
}

/*************************************************************
 函 数 名:   GetFileNameFromFullFileName
 功能概要:   从带路径的文件名中获取文件名
 返 回 值:   std::string	不带路径的文件名
 参    数:   string sFullFileName	带路径的文件名
**************************************************************/
string GetFileNameFromFullFileName(string sFullFileName)
{
	string::size_type n = sFullFileName.find_last_of('\\', sFullFileName.size());
	if (string::npos != n){
		sFullFileName.erase(0, n+1);
	}

	n = sFullFileName.find_last_of('/', sFullFileName.size());
	if (string::npos != n){
		sFullFileName.erase(0, n+1 );
	}
	return sFullFileName;
}

/*************************************************************
 函 数 名:   GetFilePathFromFullFileName
 功能概要:   从全文件名（带路径的）中获取文件路径，并统一为'/'
 返 回 值:   std::string	处理后的文件路径
 参    数:   string sFullFileName	全文件名
**************************************************************/
string GetFilePathFromFullFileName(string sFullFileName)
{
	string sPath;
	string tFullFileName(sFullFileName);
	//格式化 将'\'替换为'/'
	string::size_type stPos(0);
	while(string::npos != (stPos = tFullFileName.find('\\',stPos))){
		tFullFileName.replace(stPos,1,"/");
	}
	
	string::size_type n = tFullFileName.find_last_of('/', tFullFileName.size());
	if (string::npos != n){
		sPath.assign(sFullFileName,0,n+1);
	}

	return sPath;
}

bool PingDestination(const string pIp)
{
    char chPingFile[256];
    memset(chPingFile,0,256);
    sprintf(chPingFile,"ping(%s).txt",pIp.c_str());
    
    char chBuff[256] = "";
#ifdef OS_WINDOWS
    sprintf(chBuff,"ping  %s -n 3 -w 5000>%s",pIp.c_str(),chPingFile);
    system(chBuff);
#endif

#ifdef OS_LINUX
    FILE   *stream;
    FILE   *wstream;
    char   buf[8192];
    
#ifdef OS_SUNUNIX
    sprintf(chBuff,"ping -s -I 1 %s 1024 3",pIp.c_str());   
#elif defined OS_AIX
    sprintf(chBuff,"ping -c 3 -w 5 %s",pIp.c_str());  
#elif defined OS_HPUNIX
    sprintf(chBuff,"ping %s -n 3 -m 5",pIp.c_str());  	
#else
    sprintf(chBuff,"ping %s -c 3",pIp.c_str());  
#endif
    
    memset( buf, '\0', sizeof(buf) );//初始化buf,以免后面写如乱码到文件中
    stream = popen( chBuff, "r" ); //将“ls －l”命令的输出 通过管道读取（“r”参数）到FILE* stream
    if (stream==NULL)  
	{
		return false;
    }
	wstream = fopen(chPingFile, "w+"); //新建一个可写的文件
    if (wstream==NULL)  
	{
		pclose(stream);
		return false;
    }	
    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf中
    fwrite( buf, 1, sizeof(buf), wstream );//将buf中的数据写到FILE    *wstream对应的流中，也是写到文件中
    pclose( stream );  
    fclose( wstream );
#endif
    FILE   *pFile=NULL;
    pFile = fopen(chPingFile, "r");
    if (pFile == NULL)
    {
        return false;
    }
    
    char szTempBuff[256];
    int nNum=0;
    while (fgets(szTempBuff, 256, pFile) != NULL)
    {
        if (strstr(szTempBuff, "time=")!=NULL||
			strstr(szTempBuff, "time<")!=NULL||
			strstr(szTempBuff, "时间=")!=NULL||
			strstr(szTempBuff, "时间<")!=NULL )
        {
            nNum=nNum+1;
        }
    }
    fclose(pFile);
    pFile=NULL;
    remove(chPingFile);
    if(nNum>=2)
    {
        return true;
    }
    else
    {
        return false;
	}
}

WORD ReverseWordEndian(WORD pSourse)
{
    SWAP_16(pSourse);
    return pSourse;
}

int ReverseIntEndian(int pSourse)
{
    SWAP_32(pSourse);
    return pSourse;
}

float ReverseFloatEndian(float pSourse)
{
    SWAP_32(pSourse);
    return pSourse;
}

string FormatGatewayList(string pGatewayA, string pGatewayB)
{
    if(pGatewayA.size()>15) 
        pGatewayA.resize(15);
    if(pGatewayB.size()>15) 
        pGatewayB.resize(15);

    string strGatewayList(pGatewayA);
    strGatewayList += ",";
    strGatewayList += pGatewayB;

    return strGatewayList;
}

void GetGatewayFromList(string pGatewayList, string& pGatewayA, string& pGatewayB)
{
    pGatewayA.resize(0);
    pGatewayB.resize(0);
    string::size_type st_pos = pGatewayList.find_first_of(',');
    if (st_pos != string::npos)
    {
        pGatewayA.assign(pGatewayList,0,st_pos);
        pGatewayB.assign(pGatewayList,st_pos+1,pGatewayList.size()-st_pos-1);
        if(pGatewayA.size()>15) 
            pGatewayA.resize(15);
        if(pGatewayB.size()>15) 
            pGatewayB.resize(15);
    }
}

bool GetFileExFileName(const string& pSrcFileName,string& strExtName)
{
	bool bReturn(false);
	string::size_type n = pSrcFileName.find_last_of('.');
	if (string::npos != n)	{
		strExtName = pSrcFileName.substr(n,pSrcFileName.size());
		bReturn = true;
	}
	return bReturn;
}