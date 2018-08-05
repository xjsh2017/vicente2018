// XJStoreDefine.cpp : implementation file
//

#include "XJStoreDefine.h"

#include <time.h>
#include <stdio.h>

QByteArray GetTime(int nType)
{
	time_t tt = time(NULL);
	struct tm *local;  
	local=localtime(&tt);  //获取当前系统时间  
	
	char szLine[256] = {0};
	char *pxq[]={"日","一","二","三","四","五","六"};
	
	if (nType == 0){
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec);
	}else if (1 == nType){
		sprintf(szLine, "%4d-%02d-%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday);
	}else{
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d 星期%s"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec
			, pxq[local->tm_wday]);
	}
	
	return QByteArray(szLine);
}

