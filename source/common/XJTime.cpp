/********************************************************************
	created:	2010/05/25
	created:	25:5:2010   20:50
	filename: 	\VICENTE\CODE\COMMON\XJTime.cpp
	file path:	\VICENTE\CODE\COMMON
	file base:	XJTime
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/

#include "XJTime.h"

/*************************************************************
 函 数 名:	CXJTime
 功能概要:	默认构造，所有域初始化为0 
 返 回 值:	
*************************************************************/
CXJTime::CXJTime()
:m_iTimeZone(8),m_dMilliSecond(0)
{
    m_Time.tm_year   = 0;
    m_Time.tm_mon    = 0;
    m_Time.tm_mday   = 0;
    m_Time.tm_yday   = 0;
    m_Time.tm_wday   = 0;
    m_Time.tm_hour   = 0;
    m_Time.tm_min    = 0;
    m_Time.tm_sec    = 0;
	m_Time.tm_isdst  = 0;
}       

/*************************************************************
 函 数 名:	CXJTime
 功能概要:	拷贝构造
 返 回 值:	
 参    数:	pXJTime 用于拷贝构造的另一个CXJTime对象
*************************************************************/
CXJTime::CXJTime( CXJTime& pXJTime )
:m_iTimeZone(8),m_Time(pXJTime.m_Time),m_dMilliSecond(pXJTime.m_dMilliSecond)
{

}

/*************************************************************
 函 数 名:	CXJTime
 功能概要:	构造函数，以一个SYSTIME对象进行构造 
 返 回 值:	
 参    数:	pSYSTime 用于构造的SYSTIME对象
*************************************************************/
CXJTime::CXJTime( SYSTIME& pSYSTime )
:m_iTimeZone(8)
{
    m_Time.tm_year   = pSYSTime.wYear-1900;               
    m_Time.tm_mon    = pSYSTime.wMonth-1;              
    m_Time.tm_mday   = pSYSTime.wDay;                
    m_Time.tm_yday   = pSYSTime.wDayOfYear-1;          
    m_Time.tm_wday   = pSYSTime.wDayOfWeek;          
    m_Time.tm_hour   = pSYSTime.wHour;               
    m_Time.tm_min    = pSYSTime.wMinute;             
    m_Time.tm_sec    = pSYSTime.wSecond + (pSYSTime.wMilliseconds/1000);       
	m_Time.tm_isdst  = 0;	
    m_dMilliSecond   = pSYSTime.wMilliseconds % 1000;    
    mktime(&m_Time);
}

/*************************************************************
 函 数 名:	CXJTime
 功能概要:	构造函数，以一个time_t对象进行构造
 返 回 值:	
 参    数:	pTTime 用于构造的time_t对象
*************************************************************/
CXJTime::CXJTime( time_t& pTTime )
:m_iTimeZone(8)
{
    //tm 结构
	struct tm *newtime  = NULL;  
    
    pTTime += m_iTimeZone*60*60;
    newtime = gmtime(&pTTime);
    
    memcpy((char*)&m_Time,(char*)newtime,sizeof(struct tm));

    m_dMilliSecond = 0;       
}

/*************************************************************
 函 数 名:	~CXJTime
 功能概要:	析构函数
 返 回 值:	
*************************************************************/
CXJTime::~CXJTime()
{
    
}

/*************************************************************
 函 数 名:	operator=
 功能概要:	赋值操作
 返 回 值:	CXJTime& 
 参    数:	pTime 用于赋值的另一个CXJTime对象
*************************************************************/
CXJTime& CXJTime::operator=( CXJTime& pTime )
{
    if (this != &pTime)
    {
        m_Time.tm_year     = pTime. m_Time.tm_year;         
        m_Time.tm_mon      = pTime. m_Time.tm_mon;          
        m_Time.tm_mday     = pTime. m_Time.tm_mday;         
        m_Time.tm_yday     = pTime. m_Time.tm_yday;     
        m_Time.tm_wday     = pTime. m_Time.tm_wday;      
        m_Time.tm_hour     = pTime. m_Time.tm_hour;         
        m_Time.tm_min      = pTime. m_Time.tm_min;          
        m_Time.tm_sec      = pTime. m_Time.tm_sec;    
		m_Time.tm_isdst	   = pTime. m_Time.tm_isdst;	
        m_dMilliSecond     = pTime.m_dMilliSecond;   
        m_iTimeZone        = pTime.m_iTimeZone;
    }
    return *this;
} 

/*************************************************************
 函 数 名:	GetTimeString
 功能概要:	获取时间字符串
 返 回 值:	std::string  时间字符串结果
 参    数:	pType 预获取的字符串的类型，使用enum FormatType的值
*************************************************************/
std::string CXJTime::GetTimeString( int pType ) 
{
    mktime(&m_Time);
    string strDest;
    switch (pType)
    {
    case STTP12Time:
        {
            strDest.resize(14);
            sprintf((char*)&strDest[0],"%02d%02d%02d%02d%02d%02d",
                Year(),Month(),Day(),Hour(),Minute(),Second());
            strDest.erase(0,2);
        }
        break;
    case STTP14Time:
        {
            strDest.resize(14);
            sprintf((char*)&strDest[0],"%04d%02d%02d%02d%02d%02d",
                Year(),Month(),Day(),Hour(),Minute(),Second());
        }
        break;
    case STTP15Time:
        {
            int iTemp = m_dMilliSecond;
            strDest.resize(17);
            sprintf((char*)&strDest[0],"%02d%02d%02d%02d%02d%02d%03d",
                Year(),Month(),Day(),Hour(),Minute(),Second(),iTemp);
            strDest.erase(0,2);
        }
        break;
    case STTP19Time:
        {
            strDest.resize(19);
            sprintf((char*)&strDest[0],"%04d-%02d-%02d %02d:%02d:%02d",
                Year(),Month(),Day(),Hour(),Minute(),Second());
        }    
        break;
    case STTP26Time:
        {
            int iTemp = m_dMilliSecond * 1000;
            strDest.resize(26);
            sprintf((char*)&strDest[0],"%04d-%02d-%02d %02d:%02d:%02d.%06d",
                Year(),Month(),Day(),Hour(),Minute(),Second(),iTemp);
        }    
        break;
    case CP32Time2a:
        {
            int iMilliSecond = m_Time.tm_sec*1000 + m_dMilliSecond;
            strDest.resize(4);
            strDest[0] = iMilliSecond % 256;
            strDest[1] = iMilliSecond / 256;
            strDest[2] = m_Time.tm_min & 0x3f;
            strDest[3] = m_Time.tm_hour & 0x1f;
        }
        break;
    case CP56Time2a:
        {
            int iMilliSecond = m_Time.tm_sec*1000 + m_dMilliSecond;
            strDest.resize(7);
            BYTE b = iMilliSecond % 256;
            strDest[0] = iMilliSecond % 256;
            b = iMilliSecond / 256;
            strDest[1] = iMilliSecond / 256;
            strDest[2] = m_Time.tm_min & 0x3f;
            strDest[3] = m_Time.tm_hour & 0x1f;
            BYTE bWeek = (m_Time.tm_wday==0)?7:m_Time.tm_wday;
            bWeek = bWeek<<5;
            bWeek = (bWeek|(m_Time.tm_mday&0x1f));
            strDest[4] = bWeek;
            strDest[5] = (m_Time.tm_mon + 1) & 0x0f;
			int year = Year();
			/*
			The definition of the year field of the CP56Time2a in IEC 60870-5-4
			clause 6.8 is:
			Years := UI7 <0..99>
			*/
            strDest[6] = ((year>=2000)?year-2000:year-1900) & 0x7f;
        }
        break;
    default:
        strDest.resize(0);
        break;
    }
    return strDest;
}

/*************************************************************
 函 数 名:	AssignTimeString
 功能概要:	使用时间字符串赋值
 返 回 值:	bool true-成功 false-失败（无效的类型）
 参    数:	pTimeStr 输入的时间字符串
 参    数:	pType 输入的时间字符串的类型，使用enum FormatType的值
*************************************************************/
bool CXJTime::AssignTimeString( string pTimeStr, int pType )
{
    bool bResult(true);
    switch (pType)
    {
    case STTP12Time:
        ConvertSttpTime12ToTM(pTimeStr);
        break;
    case STTP14Time:
        ConvertSttpTime14ToTM(pTimeStr);
        break;
    case STTP15Time:
        ConvertSttpTime15ToTM(pTimeStr);
        break;
    case STTP19Time:
        ConvertSttpTime19ToTM(pTimeStr);
        break;
    case STTP26Time:
        ConvertSttpTime26ToTM(pTimeStr);
        break;
    case CP32Time2a:
        ConvertCP32Time2aToTM(pTimeStr);
        break;
    case CP56Time2a:
        ConvertCP56Time2aToTM(pTimeStr);
        break;
    default:
        bResult = false;
        break;
    }
    mktime(&m_Time);
    return bResult;
}

/*************************************************************
 函 数 名:	Year
 功能概要:	获取年
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Year() const
{
	return m_Time.tm_year + 1900;
    //return (m_Time.tm_year<70)?m_Time.tm_year + 2000:m_Time.tm_year + 1900;
}

/*************************************************************
 函 数 名:	Year
 功能概要:	设置年
 返 回 值:	void
 参    数:	pYear 输入的年
*************************************************************/
void CXJTime::Year( u_short pYear )
{
    m_Time.tm_year = (pYear<1900)?0:(pYear-1900);
	//m_Time.tm_year = (pYear<2000)?(pYear-1900):(pYear-1900);
}

/*************************************************************
 函 数 名:	Month
 功能概要:	获取月
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Month() const
{
    return m_Time.tm_mon + 1;
}

/*************************************************************
 函 数 名:	Month
 功能概要:	设置月
 返 回 值:	void
 参    数:	pMonth 输入的月
*************************************************************/
void CXJTime::Month( u_short pMonth )
{
    m_Time.tm_mon = (pMonth<1)?0:(pMonth-1);
}

/*************************************************************
 函 数 名:	Day
 功能概要:	获取日
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Day() const
{
    return m_Time.tm_mday;
}

/*************************************************************
 函 数 名:	Day
 功能概要:	设置日
 返 回 值:	void
 参    数:	pDay 输入的日
*************************************************************/
void CXJTime::Day( u_short pDay )
{
    m_Time.tm_mday = pDay;
}

/*************************************************************
 函 数 名:	Hour
 功能概要:	获取时
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Hour() const
{
    return m_Time.tm_hour;
}

/*************************************************************
 函 数 名:	Hour
 功能概要:	设置时
 返 回 值:	void
 参    数:	pHour 输入的时
*************************************************************/
void CXJTime::Hour( u_short pHour )
{
    m_Time.tm_hour = pHour; 
}

/*************************************************************
 函 数 名:	Minute
 功能概要:	获取分
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Minute() const
{
    return m_Time.tm_min;
}

/*************************************************************
 函 数 名:	Minute
 功能概要:	设置分
 返 回 值:	void
 参    数:	pMinute 输入的分
*************************************************************/
void CXJTime::Minute( u_short pMinute )
{
    m_Time.tm_min = pMinute;
}

/*************************************************************
 函 数 名:	Second
 功能概要:	获取秒
 返 回 值:	u_short
*************************************************************/
u_short CXJTime::Second() const
{
    return m_Time.tm_sec;
}

/*************************************************************
 函 数 名:	Second
 功能概要:	设置秒
 返 回 值:	void
 参    数:	pSecond 输入的秒
*************************************************************/
void CXJTime::Second( u_short pSecond )
{
    m_Time.tm_sec = pSecond;
}

/*************************************************************
 函 数 名:	MilliSecond
 功能概要:	获取毫秒
 返 回 值:	double
*************************************************************/
double CXJTime::MilliSecond() const
{
   return m_dMilliSecond;
}

/*************************************************************
 函 数 名:	MilliSecond
 功能概要:	设置毫秒
 返 回 值:	void
 参    数:	pMilliSecond 输入的毫秒
*************************************************************/
void CXJTime::MilliSecond( double pMilliSecond )
{
    double tSec = pMilliSecond / 1000.0;
    int iSec = tSec;
    m_Time.tm_sec += iSec;
    m_dMilliSecond = (tSec - iSec) * 1000.0;
}

/*************************************************************
 函 数 名:	TimeZone
 功能概要:	获取时区
 返 回 值:	int
*************************************************************/
int CXJTime::TimeZone() const
{
    return m_iTimeZone;
}

/*************************************************************
 函 数 名:	TimeZone
 功能概要:	设置时区
 返 回 值:	void
 参    数:	val 输入的时区
*************************************************************/
void CXJTime::TimeZone( int val )
{
    m_iTimeZone = ((val>=0)&&(val<24))?val:0;
}

/*************************************************************
 函 数 名:	DiffTime
 功能概要:	比较输入的CXJTime对象与自身的时间差
 返 回 值:	double 时间差，单位秒，毫秒以小数形式体现，如（100.01s）
 参    数:	pTime 输入的CXJTime对象
*************************************************************/
double CXJTime::DiffTime( CXJTime& pTime )
{
    time_t tSour = mktime(&m_Time);
    time_t tDest = mktime(&pTime.m_Time);

    return (difftime(tSour,tDest)+((m_dMilliSecond - pTime.m_dMilliSecond) / 1000.0));
}

/*************************************************************
 函 数 名:	operator<
 功能概要:	小于操作，返回是否比输入的CXJTime对象小
 返 回 值:	bool
 参    数:	pTime 输入的CXJTime对象
*************************************************************/
bool CXJTime::operator<( CXJTime& pTime )
{
    time_t tSour = mktime(&m_Time);
    time_t tDest = mktime(&pTime.m_Time);
    if ((tSour<tDest)||((tSour==tDest)&&(m_dMilliSecond<pTime.m_dMilliSecond)))
    {
        return true;
    } 
    else
    {
        return false;
    }
}

/*************************************************************
 函 数 名:	operator==
 功能概要:	等于操作，返回是否与输入的CXJTime对象相等
 返 回 值:	bool
 参    数:	pTime 输入的CXJTime对象
*************************************************************/
bool CXJTime::operator==( CXJTime& pTime )
{
    time_t tSour = mktime(&m_Time);
    time_t tDest = mktime(&pTime.m_Time);
    if ((tSour == tDest) && (fabs(m_dMilliSecond-pTime.m_dMilliSecond)<1e-6))
    {
        return true;
    } 
    else
    {
       return false;
    }
}

/*************************************************************
 函 数 名:	Time_t
 功能概要:	返回Time_t类型的时间
 返 回 值:	time_t
*************************************************************/
time_t CXJTime::Time_t() 
{
    return mktime(&m_Time);
}


/*************************************************************
 函 数 名:	ConvertSttpTime12ToTM
 功能概要:	将STTP12位时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertSttpTime12ToTM( string &pTimeStr)
{
    pTimeStr.resize(12);
    m_Time.tm_year = atoi(pTimeStr.substr(0,2).c_str()); 
    if (m_Time.tm_year<70)
    {
        m_Time.tm_year += 100;
    }
    m_Time.tm_mon = atoi(pTimeStr.substr(2,2).c_str())-1;
    m_Time.tm_mday = atoi(pTimeStr.substr(4,2).c_str());
    m_Time.tm_hour = atoi(pTimeStr.substr(6,2).c_str());
    m_Time.tm_min= atoi(pTimeStr.substr(8,2).c_str());
    m_Time.tm_sec = atoi(pTimeStr.substr(10,2).c_str());
    m_dMilliSecond = 0;
}

/*************************************************************
 函 数 名:	ConvertSttpTime14ToTM
 功能概要:	将STTP14位时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertSttpTime14ToTM( string &pTimeStr)
{
    pTimeStr.resize(14);
    m_Time.tm_year = atoi(pTimeStr.substr(0,4).c_str())-1900; 
    m_Time.tm_mon = atoi(pTimeStr.substr(4,2).c_str())-1;
    m_Time.tm_mday = atoi(pTimeStr.substr(6,2).c_str());
    m_Time.tm_hour = atoi(pTimeStr.substr(8,2).c_str());
    m_Time.tm_min= atoi(pTimeStr.substr(10,2).c_str());
    m_Time.tm_sec = atoi(pTimeStr.substr(12,2).c_str());
    m_dMilliSecond = 0;
}

/*************************************************************
 函 数 名:	ConvertSttpTime15ToTM
 功能概要:	将STTP15位时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertSttpTime15ToTM( string &pTimeStr)
{
    pTimeStr.resize(15);
    m_Time.tm_year = atoi(pTimeStr.substr(0,2).c_str()); 
    if (m_Time.tm_year<70)
    {
        m_Time.tm_year += 100;
    }
    m_Time.tm_mon = atoi(pTimeStr.substr(2,2).c_str())-1;
    m_Time.tm_mday = atoi(pTimeStr.substr(4,2).c_str());
    m_Time.tm_hour = atoi(pTimeStr.substr(6,2).c_str());
    m_Time.tm_min= atoi(pTimeStr.substr(8,2).c_str());
    m_Time.tm_sec = atoi(pTimeStr.substr(10,2).c_str());
    m_dMilliSecond = atoi(pTimeStr.substr(12,3).c_str());
}

/*************************************************************
 函 数 名:	ConvertSttpTime19ToTM
 功能概要:	将STTP19位时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertSttpTime19ToTM( string &pTimeStr )
{
    pTimeStr.resize(19);
    m_Time.tm_year = atoi(pTimeStr.substr(0,4).c_str())-1900; 
    m_Time.tm_mon = atoi(pTimeStr.substr(5,2).c_str())-1;
    m_Time.tm_mday = atoi(pTimeStr.substr(8,2).c_str());
    m_Time.tm_hour = atoi(pTimeStr.substr(11,2).c_str());
    m_Time.tm_min= atoi(pTimeStr.substr(14,2).c_str());
    m_Time.tm_sec = atoi(pTimeStr.substr(17,2).c_str());
    m_dMilliSecond = 0;
}

/*************************************************************
 函 数 名:	ConvertSttpTime26ToTM
 功能概要:	将STTP26位时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertSttpTime26ToTM( string &pTimeStr )
{
    pTimeStr.resize(26);
    m_Time.tm_year = atoi(pTimeStr.substr(0,4).c_str())-1900; 
    m_Time.tm_mon = atoi(pTimeStr.substr(5,2).c_str())-1;
    m_Time.tm_mday = atoi(pTimeStr.substr(8,2).c_str());
    m_Time.tm_hour = atoi(pTimeStr.substr(11,2).c_str());
    m_Time.tm_min= atoi(pTimeStr.substr(14,2).c_str());
    m_Time.tm_sec = atoi(pTimeStr.substr(17,2).c_str());
    int iTemp = atoi(pTimeStr.substr(20,6).c_str());
    m_dMilliSecond = iTemp/1000.0;
}

/*************************************************************
 函 数 名:	ConvertCP32Time2aToTM
 功能概要:	将103CP32Time2a时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertCP32Time2aToTM( string &pTimeStr )
{
    pTimeStr.resize(4);
    BYTE l = pTimeStr[0];
    BYTE h = pTimeStr[1];
    int iMilliSec = l + h*256;
    m_dMilliSecond = iMilliSec % 1000;
    m_Time.tm_sec = iMilliSec / 1000;
    m_Time.tm_min = pTimeStr[2] & 0x3f;
    m_Time.tm_hour = pTimeStr[3] & 0x1f;    

    time_t tNow;
    time(&tNow);
    struct tm *nowtime  = NULL;  
    
    tNow += m_iTimeZone*60*60;
    nowtime = gmtime(&tNow);
    m_Time.tm_mday = nowtime->tm_mday;
    m_Time.tm_mon = nowtime->tm_mon -1 ;
    m_Time.tm_year = nowtime->tm_year;
}

/*************************************************************
 函 数 名:	ConvertCP56Time2aToTM
 功能概要:	将103CP56Time2a时间字符串转换为tm结构
 返 回 值:	void
 参    数:	pTimeStr 输入的时间字符串
*************************************************************/
void CXJTime::ConvertCP56Time2aToTM( string &pTimeStr )
{
   pTimeStr.resize(7);
   BYTE l = pTimeStr[0];
   BYTE h = pTimeStr[1];
   int iMilliSec = l + h*256;
   m_dMilliSecond = iMilliSec % 1000;
   m_Time.tm_sec = iMilliSec / 1000;
   m_Time.tm_min = pTimeStr[2] & 0x3f;
   m_Time.tm_hour = pTimeStr[3] & 0x1f;
   m_Time.tm_mday = pTimeStr[4] & 0x1f;
   m_Time.tm_mon = (pTimeStr[5] & 0x0f) -1;
   int year = pTimeStr[6] & 0x7f;
   m_Time.tm_year = (year<70)?100+year:year;
}
