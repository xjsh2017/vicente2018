#include "vccommon_define.h"
#include "LogMgr.h"

void WriteLog(CString sLog, int nLogLevel/* = 3*/)
{
	CLogMgr* pMgr = CLogMgr::getInstance();
	if(pMgr != NULL)
		pMgr->WriteLog(sLog, nLogLevel);
}

void WriteLogTemp(CString sLog, int nLogLevel/* = 3*/)
{

}

/*************************************************************
 函 数 名：StringToTimeSttp15()
 功能概要：将15位的STTP时间字符串转化为CTime
 返 回 值: 转化成的CTime对象
 参    数：param1 时间字符串
		   Parsm2 返回毫秒
**************************************************************/
CTime StringToTimeSttp15(CString & sTime, int& nms)
{
	//字符串格式为YYMMDDhhmmss+3位毫秒数,如080803095654123
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() < 12 && strTime.GetLength() > 16)
		return tmTime;
	CString strTemp;

	int nt[6];

	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}

	//毫秒
	nms = atoi(strTime);

	//年加上2000,变成20XX年
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//判断日期
	int nDay = nt[2];
	if(nDay < 0 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：StringToTimeSttp12()
 功能概要：将12位的STTP时间字符串转化为CTime,12位的时间不带毫秒值
 返 回 值: 转化成的CTime对象
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTimeSttp12(CString & sTime)
{
	//字符串格式为YYMMDDhhmmss,如080803095654
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() != 12)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}
	
	//年加上2000,变成20XX年
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	int nDay = nt[2];
	if(nDay < 0 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：StringToTimeSttp26()
 功能概要：将26位的STTP时间字符串转化为CTime,毫秒值占6位
 返 回 值: CTime类型时间
 参    数：param1	时间字符串
		   Param2	毫秒值
**************************************************************/
CTime StringToTimeSttp26(CString & sTime, int& nms)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMMMM 如:2008-05-30 13:44:51.120005
	CString strTime = sTime;
	if(strTime.GetLength() < 23 && strTime.GetLength() >27)
		return tmTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//判断秒钟合法
		if(nSec < 0 || nSec > 60)
		return tmTime;

	//毫秒
	strTemp = strTime;
	strTemp.Delete(0, 20);
	if(strTemp.GetLength() > 3)
	{
		//去掉后面3位
		strTemp.Delete(strTemp.GetLength() - 3, 3);
	}
	//剩下6位毫秒值
	nms = atoi(strTemp);
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 函 数 名：StringToTime14()
 功能概要：将14数的时间字符串转换成时间对象
 返 回 值: CTime类型时间
 参    数：param1 时间字符串
		   Param2
**************************************************************/
CTime StringToTime14(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//时间格式为:YYYYMMDDhhmmss, 如:20080808010100
	CString strTime = sTime;
	if(strTime.GetLength() != 14)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		int nLength = 2;
		if(i == 0)
			nLength = 4;
		strTemp = strTime.Left(nLength);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, nLength);
	}

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//判断日期
	int nDay = nt[2];
	if(nDay < 0 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：XJToMS()
 功能概要：CXJString对象字符串转化为CString字符串
 返 回 值: 转化成CString对象
 参    数：param1	要转化的CXJString对象
**************************************************************/
CString XJToMS(string xjStr)
{
	int nLength = xjStr.length();
	char* a = new char[nLength+2];
	memset(a, 0, nLength+2);
	memcpy(a, xjStr.c_str(), nLength);
	CString str = a;
	delete [] a;
	return str;
}

string WChar2Ansi(LPCWSTR pwszSrc) 
{ 
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL); 
	if (nLen <= 0) return std::string(""); 
	char* pszDst = new char[nLen]; 
	if (NULL == pszDst) return std::string(""); 
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL); 
	pszDst[nLen -1] = 0; 
	std::string strTemp(pszDst); 
	delete [] pszDst; 
	return strTemp; 
}	
/*************************************************************
 函 数 名：MSToXJ()
 功能概要：CString对象字符串转化为CXJString字符串
 返 回 值: 转化成CXJString对象
 参    数：param1	要转化的CString对象
**************************************************************/
string MSToXJ(CString str)
{
	return (LPCSTR)str;
}

CTime StringToTimeWithMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMM 如:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//判断秒钟合法
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

CTime StringToTimeWithoutMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//19位字符串格式为YYYY-MM-DD HH:MM:SS 如:2008-05-30 13:44:51
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	strTemp = strTime;
	strTemp.Delete(0, pos+1);
	nSec = atoi(strTemp);
	//判断秒钟合法
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 函 数 名：StringToTime()
 功能概要：数据库中的时间字符串转化为时间.字符串格式为YYYY-MM-DD HH:MM:SS.MMMM
 返 回 值: 转化成的时间
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTime(CString& sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMM 如:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	strTime.TrimLeft();
	strTime.TrimRight();
	int nLength = strTime.GetLength();
	if(nLength <= 19)
	{
		//19位字符串格式为YYYY-MM-DD HH:MM:SS
		return StringToTimeWithoutMillisecond(strTime);
	}
	else if(nLength > 19)
	{
		return StringToTimeWithMillisecond(strTime);
	}
	return tmTime;
}

/*************************************************************
 函 数 名：ComplexExchange()
 功能概要：复数转换, 判断传入的字符串是否表示复数,如果是,转换成复数形式表示
 返 回 值: void
 参    数：param1	需转换的字符串	
		   Param2
**************************************************************/
CString	ComplexExchange(CString sStr)
{
	CString sReturn = sStr;
	//复数的字符串形式为: 实部&&&虚部
	int nPos = sStr.Find("&&", 0);
	if(nPos == -1)
	{
		//非复数
		return sReturn;
	}
	//复数
	CString strR = sStr.Left(nPos); //实部
	CString strC = sStr;
	strC.Delete(0, nPos + 2); //虚部
	sReturn.Format("%s+j%s", strR, strC);
	return sReturn;
}

/*************************************************************
 函 数 名：CompareComplex()
 功能概要：比较两个复数的大小
 返 回 值: 如果1比2大, 返回1. 1=2,返回0. 1<2,返回-1
 参    数：param1	复数1
		   Param2	复数2
**************************************************************/
int	CompareComplex(CString str1, CString str2)
{
	//判断数据合法,复数必然是: 实部+j虚部 形式
	int nPos1 = str1.Find('+', 0);
	int nPos2 = str2.Find('+', 0);

	if(nPos1 == -1 || nPos2 == -1)
	{
		//不合法,退出
		return 0;
	}
	
	CString strR1 = str1.Left(nPos1); //复数1的实部
	CString strC1 = str1;
	strC1.Delete(0, nPos1+1); //复数1的虚部
	CString strR2 = str2.Left(nPos2); //复数2的实部
	CString strC2 = str2;
	strC2.Delete(0, nPos2+1); //复数2的虚部
	//先比实部
	int R1 = atoi(strR1);
	int R2 = atoi(strR2);
	if(R1 > R2)
	{
		return 1;
	}
	else if(R1 < R2)
	{
		return -1;
	}
	else
	{
		//实部相等,比虚部
		int C1 = atoi(strC1);
		int C2 = atoi(strC2);
		if(C1 > C2)
		{
			return 1;
		}
		else if(C1 < C2)
		{
			return -1;
		}
		else 
		{
			return 0;
		}
	}
	return 0;
}