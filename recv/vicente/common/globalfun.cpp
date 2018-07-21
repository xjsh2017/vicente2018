#pragma warning(disable : 4275)
#pragma warning(disable : 4786)//����4786����
#include "define.h"
#include "XJString.h"

/*  ȫ�ֺ���ʵ��  */

/*************************************************************
 �� �� ����GetSysTime()
 ���ܸ�Ҫ�����ϵͳ��ǰʱ��
 �� �� ֵ: void
 ��    ����SYSTIME& curTime
***************************************************************/
void GetSysTime(SYSTIME & curTime)
{
	//��ű���ʱ��
	time_t ltime=0;

	//tm �ṹ
	struct tm today = {0};

	//ȡ�ñ���ʱ��
	time(&ltime);

#ifdef OS_WINDOWS
	struct tm *tmp_tm = NULL;
	tmp_tm = localtime(&ltime);//ʱ��ת��Ϊtm����
	today = *tmp_tm;
#else
	localtime_r(&ltime, &today);
#endif
	
    //ȡֵ
	curTime.wYear      = today.tm_year + 1900;  //tm ��year ����1900
	curTime.wMonth     = today.tm_mon + 1;      //tm ��month ����0
	curTime.wDay       = today.tm_mday;
	curTime.wHour      = today.tm_hour;
	curTime.wMinute    = today.tm_min ;
	curTime.wSecond    = today.tm_sec ;
	curTime.wDayOfWeek = today.tm_wday ;        //sunday is 0 ,monday is 1 ,and so on
    curTime.wDayOfYear = today.tm_yday + 1;     //tm�л���0��ʼ

	//��֧��ȥ����,��0
	curTime.wMilliseconds = 0;
}

/*************************************************************
 �� �� ����CreateSavePath()
 ���ܸ�Ҫ: ����·��
 �� �� ֵ: void
 ��    ����char * szPath   ·����
***************************************************************/
void CreateSavePath(char * szPath)  
{
	int nStart=0,nReturn = 0;
	CXJString strFilePath,strTempPath;

	strFilePath = szPath;

#ifdef OS_WINDOWS 
	nStart=strFilePath.Find('/');
	if(nStart < 0)  //����һ��Ŀ¼
	{
		nStart=strFilePath.Find('\\');
		if (nStart < 0)
		{
			::CreateDirectory(strFilePath,NULL); 
			return;
		}
		while(nStart+1)
		{
			if(nStart < 0) //��û�з���ʱ����-1��
			{
				::CreateDirectory(strFilePath,NULL); 
				return;
			}
			strTempPath=strFilePath.Left(nStart);
			
			//��·���������򴴽������������
			::CreateDirectory(strTempPath,NULL); 
			nStart=strFilePath.Find('\\',nStart+1);	
		}		
	}

	while(nStart+1)
	{
		if(nStart < 0) //��û�з���ʱ����-1��
		{
			::CreateDirectory(strFilePath,NULL); 
			return;
		}
		strTempPath=strFilePath.Left(nStart);

		//��·���������򴴽������������
		::CreateDirectory(strTempPath,NULL); 
		nStart=strFilePath.Find('/',nStart+1);	
	}
#endif

#ifdef OS_LINUX
    nStart=strFilePath.Find('/');
	if(nStart < 0)  //����һ��Ŀ¼
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
		if(nStart != 0 )  // ��Ϊ��·��ʱ��ʼ����
		{
			strTempPath=strFilePath.Left(nStart);
			
			//��·���������򴴽������������
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
 �� �� ����GetIniKey()
 ���ܸ�Ҫ: ���ָ�������ļ���ָ���ؼ��ֵ�ֵ
 �� �� ֵ: void
 ��    ����const char * filename  �����ļ�����
           const * title     ��Ŀ����
		   const * key       �ؼ�������
		   const * chResult  ȡ�õĽ��
***************************************************************/
void GetIniKey(const char *filename, const char *title,
				  const char *key,char *chResult)
{
	if(chResult == NULL)
	{
		fprintf(stderr,"the last parameter is NULL in GetIniKey()");
		return ;
	}

	//�ļ�ָ��
	FILE * fp = NULL;

	//��ʱ����
	char tmpLine[1024]="";         // ÿ������ַ���
	char every_line[1024]="";
	char tempKey[255]="";          // ������ʱ�ؼ���
	char tmpTitle[255]="";         // ������ʷ��Ŀ����
	char * pEveryLine = NULL;
   	char * pResult = NULL;

	//��ȡ�ļ�����ֵ
	char * pReturn = NULL;

	int i=0,j=0,k=0,nLen = 0;

	//��־һ����Ŀ�µ�key�Ƿ��Ѿ�ɨ����
	int flag = 0;

	char * tmp=NULL;
	
	//���ļ�
	if ((fp = fopen( filename, "r")) == NULL )
	{
		fprintf(stderr,"Open File %s Failed,Reason:%s \n",filename,strerror(errno));
		return;
	}

	//��ȡ����
	while (!feof(fp))
	{
		memset(tmpLine,0,1024);
		memset(every_line,0,1024);

		pReturn = fgets(every_line,1023,fp );    //��ȡһ��

		if ( pReturn == NULL )                //�ļ�������������
		{  
			break; 
		}
		
		nLen = strlen(every_line);
        pEveryLine = every_line;
        j = 0;

		// ȥ�����׿ո�(����һ����Ϊ�ո��ַ�ǰ�����пո�ȥ��,)
		for(i =0; i<nLen; i++)
		{
			if(every_line[i] == 0x20)
			{
				j++;         // �ո���Ŀ
			}
			else             // ���ֲ�Ϊ�ո�ʱ���˳���ѭ��
				break;
		}
        
		memcpy(tmpLine,pEveryLine+j,nLen-j);  // ���Ƴ��ո�����ַ�
        nLen = nLen - j;                      // �������ó���

		// ����ע���л����
		if( ( tmpLine[0] == ';') ||                           // ';'��ע�� 
		    ((tmpLine[0] == '/') && (tmpLine[1] == '/')) ||   // '//'ע��
		    ((tmpLine[0] == '/') && (tmpLine[1] == '*')) ||   // '/*'ע��
			((tmpLine[0] == '-') && (tmpLine[1] == '-')) ||   // '--'ע��
			((tmpLine[0] == '\r')&& (tmpLine[1] == '\n'))||   // windows �»��з�(�޿ո�Ŀ���)
			 (tmpLine[0] =='\n') )                            // linux/unix�»��з� (�޿ո�Ŀ���)
		{
			continue;
		}	

        // ȥ�����з�
		if( (nLen >= 1) && (tmpLine[nLen-1] == '\n') )      // windows\linux�»��з����Ϊ'\n'
			tmpLine[nLen-1] = 0;
		if( (nLen >= 2) && (tmpLine[nLen-2] == '\r') )      //  windows�»��з�Ϊ��'\n'֮ǰ���и�'\r'
			tmpLine[nLen-2] = 0;

		tmp = strchr(tmpLine, '=');
		
		//��һ�ζ��� '=' Ӧ���� ��Ŀ����
		if (( tmp != NULL )&&(flag == 1))
		{
			i= j = 0;
			memset(tempKey,0,255);

			//ȡ���ؼ���
			while(tmpLine[j] != '=')
			{
				if(tmpLine[j] != 0x20)                        //ȥ���ո�
				{
					tempKey[i]=tmpLine[j];
					i++;

					if(i > 254)
						break;
				}
				j++;
			}
			
	    	//����ؼ�����ͬ��ȡ��ֵ (ȥ������еĿո�)
			if(strcmp(tempKey,key) == 0)
			{
				nLen = strlen(tmp) - 1;
				tmp = tmp + 1;          // ���� '='��
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

				fclose ( fp );        // �ر��ļ�

				return ;
			}
		}
		else
		{
			//���flag=1,��˵��ǰһ��title�µ�key�Ѿ�����
			if( flag == 1)
			{
				fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n", \
					   title,key,filename);
				bzero(chResult,strlen(chResult));
				fclose(fp);
				return ;
			}
			memset(tmpTitle,0,255);

			// ������Ŀ����
			strcpy(tmpTitle,"[");
			strcat(tmpTitle,title);
			strcat(tmpTitle,"]");
			
			// ȷ�϶�������Ϣ�Ƿ�����Ŀ����
			if( strstr(tmpLine,tmpTitle) != NULL )
			{
				flag = 1;   // ��ʶ�ҵ�title
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
 �� �� ����EndThread()
 ���ܸ�Ҫ: �����߳�
 �� �� ֵ: void
 ��    ������
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
 �� �� ����MySleep()
 ���ܸ�Ҫ: ˯�ߵȴ� ֧�ֺ���ȴ�
 �� �� ֵ: void
 ��    ����
           nmillseconds : ������
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
 �� �� ����GotFileStatusInDir()
 ���ܸ�Ҫ: ����ָ��Ŀ¼�������ļ����ļ���Ϣ�������������ļ��м������ļ���
 �� �� ֵ: void
 ��    ����
           dir ·���� pFileStatusList, �ļ���Ϣ�ṹ������
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
	//��Ŀ¼
	printf("opendir<%s>\r\n",dir);
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    //����Ŀ¼���ļ�
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
		//ȡ���ļ�״̬
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
 �� �� ��:   AddField
 ���ܸ�Ҫ:	 ��ָ����SQL_DATA�����һ���ֶ�
 �� �� ֵ:   void
 ��    ��:   SQL_DATA & sql
 ��    ��:   const char * pFieldName �ֶ���
 ��    ��:   int nType �ֶ�����
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
 �� �� ��:   AddCondition
 ���ܸ�Ҫ:	 ��ָ����SQL_DATA�����һ������
 �� �� ֵ:   void
 ��    ��:   SQL_DATA & sql
 ��    ��:   const char * pContent ��������
 ��    ��:   int nType ��������
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
 �� �� ����Zero_SQL_DATA()
 ���ܸ�Ҫ�����SQL_DATA
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   SQL_DATA����
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
 �� �� ����Zero_RealData_Condition()
 ���ܸ�Ҫ����սṹ
 �� �� ֵ: ��
 ��    ����param1   �ṹ����
**************************************************************/
void Zero_RealData_Condition(REALDATA_CONDITION& pRealDataCondition)
{
	pRealDataCondition.IsUse      = false;//�Ƿ�ʹ��
	pRealDataCondition.station_id = "";//��վID
	pRealDataCondition.pt_id      = "";//����ID
	pRealDataCondition.cpu_code   = "";//CPU��
	pRealDataCondition.breaker_id = "";//����ID
	pRealDataCondition.reverse1   = "";//����
	pRealDataCondition.reverse2   = "";//����
}

/*************************************************************
 �� �� ��:   GetFormatPath
 ���ܸ�Ҫ:   ��ø�ʽ�����ļ�·���������е�б��ͳһΪ'/',���û��'/'
			ʱ����'/'
 �� �� ֵ:   std::string ��ʽ�����·��
 ��    ��:   string sourcePath ���ʽ����·��
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
 �� �� ��:   GetFileNameFromFullFileName
 ���ܸ�Ҫ:   �Ӵ�·�����ļ����л�ȡ�ļ���
 �� �� ֵ:   std::string	����·�����ļ���
 ��    ��:   string sFullFileName	��·�����ļ���
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
 �� �� ��:   GetFilePathFromFullFileName
 ���ܸ�Ҫ:   ��ȫ�ļ�������·���ģ��л�ȡ�ļ�·������ͳһΪ'/'
 �� �� ֵ:   std::string	�������ļ�·��
 ��    ��:   string sFullFileName	ȫ�ļ���
**************************************************************/
string GetFilePathFromFullFileName(string sFullFileName)
{
	string sPath;
	string tFullFileName(sFullFileName);
	//��ʽ�� ��'\'�滻Ϊ'/'
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
    
    memset( buf, '\0', sizeof(buf) );//��ʼ��buf,�������д�����뵽�ļ���
    stream = popen( chBuff, "r" ); //����ls ��l���������� ͨ���ܵ���ȡ����r����������FILE* stream
    if (stream==NULL)  
	{
		return false;
    }
	wstream = fopen(chPingFile, "w+"); //�½�һ����д���ļ�
    if (wstream==NULL)  
	{
		pclose(stream);
		return false;
    }	
    fread( buf, sizeof(char), sizeof(buf), stream); //���ո�FILE* stream����������ȡ��buf��
    fwrite( buf, 1, sizeof(buf), wstream );//��buf�е�����д��FILE    *wstream��Ӧ�����У�Ҳ��д���ļ���
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
			strstr(szTempBuff, "ʱ��=")!=NULL||
			strstr(szTempBuff, "ʱ��<")!=NULL )
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