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
 �� �� ��:	CXJTime
 ���ܸ�Ҫ:	Ĭ�Ϲ��죬�������ʼ��Ϊ0 
 �� �� ֵ:	
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
 �� �� ��:	CXJTime
 ���ܸ�Ҫ:	��������
 �� �� ֵ:	
 ��    ��:	pXJTime ���ڿ����������һ��CXJTime����
*************************************************************/
CXJTime::CXJTime( CXJTime& pXJTime )
:m_iTimeZone(8),m_Time(pXJTime.m_Time),m_dMilliSecond(pXJTime.m_dMilliSecond)
{

}

/*************************************************************
 �� �� ��:	CXJTime
 ���ܸ�Ҫ:	���캯������һ��SYSTIME������й��� 
 �� �� ֵ:	
 ��    ��:	pSYSTime ���ڹ����SYSTIME����
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
 �� �� ��:	CXJTime
 ���ܸ�Ҫ:	���캯������һ��time_t������й���
 �� �� ֵ:	
 ��    ��:	pTTime ���ڹ����time_t����
*************************************************************/
CXJTime::CXJTime( time_t& pTTime )
:m_iTimeZone(8)
{
    //tm �ṹ
	struct tm *newtime  = NULL;  
    
    pTTime += m_iTimeZone*60*60;
    newtime = gmtime(&pTTime);
    
    memcpy((char*)&m_Time,(char*)newtime,sizeof(struct tm));

    m_dMilliSecond = 0;       
}

/*************************************************************
 �� �� ��:	~CXJTime
 ���ܸ�Ҫ:	��������
 �� �� ֵ:	
*************************************************************/
CXJTime::~CXJTime()
{
    
}

/*************************************************************
 �� �� ��:	operator=
 ���ܸ�Ҫ:	��ֵ����
 �� �� ֵ:	CXJTime& 
 ��    ��:	pTime ���ڸ�ֵ����һ��CXJTime����
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
 �� �� ��:	GetTimeString
 ���ܸ�Ҫ:	��ȡʱ���ַ���
 �� �� ֵ:	std::string  ʱ���ַ������
 ��    ��:	pType Ԥ��ȡ���ַ��������ͣ�ʹ��enum FormatType��ֵ
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
 �� �� ��:	AssignTimeString
 ���ܸ�Ҫ:	ʹ��ʱ���ַ�����ֵ
 �� �� ֵ:	bool true-�ɹ� false-ʧ�ܣ���Ч�����ͣ�
 ��    ��:	pTimeStr �����ʱ���ַ���
 ��    ��:	pType �����ʱ���ַ��������ͣ�ʹ��enum FormatType��ֵ
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
 �� �� ��:	Year
 ���ܸ�Ҫ:	��ȡ��
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Year() const
{
	return m_Time.tm_year + 1900;
    //return (m_Time.tm_year<70)?m_Time.tm_year + 2000:m_Time.tm_year + 1900;
}

/*************************************************************
 �� �� ��:	Year
 ���ܸ�Ҫ:	������
 �� �� ֵ:	void
 ��    ��:	pYear �������
*************************************************************/
void CXJTime::Year( u_short pYear )
{
    m_Time.tm_year = (pYear<1900)?0:(pYear-1900);
	//m_Time.tm_year = (pYear<2000)?(pYear-1900):(pYear-1900);
}

/*************************************************************
 �� �� ��:	Month
 ���ܸ�Ҫ:	��ȡ��
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Month() const
{
    return m_Time.tm_mon + 1;
}

/*************************************************************
 �� �� ��:	Month
 ���ܸ�Ҫ:	������
 �� �� ֵ:	void
 ��    ��:	pMonth �������
*************************************************************/
void CXJTime::Month( u_short pMonth )
{
    m_Time.tm_mon = (pMonth<1)?0:(pMonth-1);
}

/*************************************************************
 �� �� ��:	Day
 ���ܸ�Ҫ:	��ȡ��
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Day() const
{
    return m_Time.tm_mday;
}

/*************************************************************
 �� �� ��:	Day
 ���ܸ�Ҫ:	������
 �� �� ֵ:	void
 ��    ��:	pDay �������
*************************************************************/
void CXJTime::Day( u_short pDay )
{
    m_Time.tm_mday = pDay;
}

/*************************************************************
 �� �� ��:	Hour
 ���ܸ�Ҫ:	��ȡʱ
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Hour() const
{
    return m_Time.tm_hour;
}

/*************************************************************
 �� �� ��:	Hour
 ���ܸ�Ҫ:	����ʱ
 �� �� ֵ:	void
 ��    ��:	pHour �����ʱ
*************************************************************/
void CXJTime::Hour( u_short pHour )
{
    m_Time.tm_hour = pHour; 
}

/*************************************************************
 �� �� ��:	Minute
 ���ܸ�Ҫ:	��ȡ��
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Minute() const
{
    return m_Time.tm_min;
}

/*************************************************************
 �� �� ��:	Minute
 ���ܸ�Ҫ:	���÷�
 �� �� ֵ:	void
 ��    ��:	pMinute ����ķ�
*************************************************************/
void CXJTime::Minute( u_short pMinute )
{
    m_Time.tm_min = pMinute;
}

/*************************************************************
 �� �� ��:	Second
 ���ܸ�Ҫ:	��ȡ��
 �� �� ֵ:	u_short
*************************************************************/
u_short CXJTime::Second() const
{
    return m_Time.tm_sec;
}

/*************************************************************
 �� �� ��:	Second
 ���ܸ�Ҫ:	������
 �� �� ֵ:	void
 ��    ��:	pSecond �������
*************************************************************/
void CXJTime::Second( u_short pSecond )
{
    m_Time.tm_sec = pSecond;
}

/*************************************************************
 �� �� ��:	MilliSecond
 ���ܸ�Ҫ:	��ȡ����
 �� �� ֵ:	double
*************************************************************/
double CXJTime::MilliSecond() const
{
   return m_dMilliSecond;
}

/*************************************************************
 �� �� ��:	MilliSecond
 ���ܸ�Ҫ:	���ú���
 �� �� ֵ:	void
 ��    ��:	pMilliSecond ����ĺ���
*************************************************************/
void CXJTime::MilliSecond( double pMilliSecond )
{
    double tSec = pMilliSecond / 1000.0;
    int iSec = tSec;
    m_Time.tm_sec += iSec;
    m_dMilliSecond = (tSec - iSec) * 1000.0;
}

/*************************************************************
 �� �� ��:	TimeZone
 ���ܸ�Ҫ:	��ȡʱ��
 �� �� ֵ:	int
*************************************************************/
int CXJTime::TimeZone() const
{
    return m_iTimeZone;
}

/*************************************************************
 �� �� ��:	TimeZone
 ���ܸ�Ҫ:	����ʱ��
 �� �� ֵ:	void
 ��    ��:	val �����ʱ��
*************************************************************/
void CXJTime::TimeZone( int val )
{
    m_iTimeZone = ((val>=0)&&(val<24))?val:0;
}

/*************************************************************
 �� �� ��:	DiffTime
 ���ܸ�Ҫ:	�Ƚ������CXJTime�����������ʱ���
 �� �� ֵ:	double ʱ����λ�룬������С����ʽ���֣��磨100.01s��
 ��    ��:	pTime �����CXJTime����
*************************************************************/
double CXJTime::DiffTime( CXJTime& pTime )
{
    time_t tSour = mktime(&m_Time);
    time_t tDest = mktime(&pTime.m_Time);

    return (difftime(tSour,tDest)+((m_dMilliSecond - pTime.m_dMilliSecond) / 1000.0));
}

/*************************************************************
 �� �� ��:	operator<
 ���ܸ�Ҫ:	С�ڲ����������Ƿ�������CXJTime����С
 �� �� ֵ:	bool
 ��    ��:	pTime �����CXJTime����
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
 �� �� ��:	operator==
 ���ܸ�Ҫ:	���ڲ����������Ƿ��������CXJTime�������
 �� �� ֵ:	bool
 ��    ��:	pTime �����CXJTime����
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
 �� �� ��:	Time_t
 ���ܸ�Ҫ:	����Time_t���͵�ʱ��
 �� �� ֵ:	time_t
*************************************************************/
time_t CXJTime::Time_t() 
{
    return mktime(&m_Time);
}


/*************************************************************
 �� �� ��:	ConvertSttpTime12ToTM
 ���ܸ�Ҫ:	��STTP12λʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertSttpTime14ToTM
 ���ܸ�Ҫ:	��STTP14λʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertSttpTime15ToTM
 ���ܸ�Ҫ:	��STTP15λʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertSttpTime19ToTM
 ���ܸ�Ҫ:	��STTP19λʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertSttpTime26ToTM
 ���ܸ�Ҫ:	��STTP26λʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertCP32Time2aToTM
 ���ܸ�Ҫ:	��103CP32Time2aʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
 �� �� ��:	ConvertCP56Time2aToTM
 ���ܸ�Ҫ:	��103CP56Time2aʱ���ַ���ת��Ϊtm�ṹ
 �� �� ֵ:	void
 ��    ��:	pTimeStr �����ʱ���ַ���
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
