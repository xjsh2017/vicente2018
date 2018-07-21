/********************************************************************
	created:	2010/05/25
	created:	25:5:2010   20:50
	filename: 	\VICENTE\CODE\COMMON\XJTime.h
	file path:	\VICENTE\CODE\COMMON
	file base:	XJTime
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef XJTime_h__
#define XJTime_h__

#pragma warning(disable : 4275)//屏蔽4275错误

#include "define.h"

/**
 * @defgroup 	CXJTime	时间处理类
 * @{
 */
/**
 * @brief       该类用于ipofas系统中各种时间格式的处理及转换
                可利用SYSTIME、time_t的时间构造
                可使用AssignTimeString接口 输入各种格式的时间字符串
                可使用GetTimeString接口 输出为各种格式的时间字符串
                提供 赋值(=) 小于(<) 等于(==) DiffTime操作
*  @author      qingch
 * @version     ver1.0
 * @date        03/06/10

 *
 * example
 * @code
 *    // 举例说明类如何使用
      1. 如果已有一个SYSTIME对象需要转换
            SYSTIME tSysTime;//如何设置值省去
            CXJTime tTime(tSysTime);
            char chSttpTime26[27] = "";
            strncpy(chSttpTime26,tTime.GetTimeString(CXJTime::STTP26Time).c_str,26);
            //转成其他格式的时间请输入对应的类型
      2. 如果已有一个time_t对象需要转换
            time_t tTTime;//如何设置值省去
            CXJTime tTime(tTTime);
            char chSttpTime26[27] = "";
            strncpy(chSttpTime26,tTime.GetTimeString(CXJTime::STTP26Time).c_str,26);
            //转成其他格式的时间请输入对应的类型
      3. 如果有一个时间字符串需要转换
            char chTime[27] = "2010-06-02 16:03:55.009001";
            CXJTime tTime;
            tTime.AssignTimeString(chTime,CXJTime::STTP26Time);
            char ch103Time7[7] = "";
            strncpy(ch103Time7,tTime.GetTimeString(CXJTime::CP56Time2a).c_str,7);
            //转成其他格式的时间请输入对应的类型
 * @endcode
 */
/** @} */ //OVER

class CXJTime
{
public:
    enum FormatType{
        STTP12Time = 0,/*STTP报文中的12位时间，格式：“yymmddHHMMSS”*/
        STTP14Time = 1,/*STTP报文中的14位时间，格式：“yyyymmddHHMMSS”*/
        STTP15Time = 2,/*STTP报文中的15位时间，格式：“YYMMDDmmhhss+3位毫秒数”*/
        STTP19Time = 3,/*STTP报文中的19位时间，格式：“ yyyy-mm-dd HH:MM:SS”*/
        STTP26Time = 4,/*STTP报文中的26位时间，格式：“  yyyy-mm-dd HH:MM:SS.6位ms”*/
        CP32Time2a = 5,/*103中的4位时间，{毫秒，分，备用1，无效，时，备用2，夏时制}*/
        CP56Time2a = 6,/*103中的7位时间，{毫秒，分，备用1，无效，时，备用2，夏时制，月的某天，星期的某天，月，备用3，年，备用4}*/
    };

    /*
     *  @brief		CXJTime	 默认构造，所有域初始化为0 
     *  @return		 
     */

    CXJTime();
    /*
     *  @brief		CXJTime	 拷贝构造 
     *  @param	[In]pXJTime  用于拷贝构造的另一个CXJTime对象
     *  @return		 
     */

    CXJTime(CXJTime& pXJTime);
    /*
     *  @brief		CXJTime	 构造函数，以一个SYSTIME对象进行构造 
     *  @param	[In]pSYSTime  用于构造的SYSTIME对象
     *  @return		 
     */

    CXJTime(SYSTIME& pSYSTime);
    /*
     *  @brief		CXJTime	 构造函数，以一个time_t对象进行构造  
     *  @param	[In]pTTime  用于构造的time_t对象
     *  @return		 
     */

    CXJTime(time_t& pTTime);

    /*
     *  @brief		~CXJTime	 析构函数 
     *  @return		 
     */

    ~CXJTime();

    /*
     *  @brief		operator=	 赋值操作 
     *  @param	[In]pXJTime  用于赋值的另一个CXJTime对象
     *  @return		 
     */
    CXJTime& operator= (CXJTime& pTime);

    /*
     *  @brief		GetTimeString	 获取时间字符串 
     *  @param	[In]pType  预获取的字符串的类型，使用enum FormatType的值
     *  @return		string 时间字符串结果
     */

    string GetTimeString(int pType);
    /*
     *  @brief		AssignTimeString	 使用时间字符串赋值 
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @param	[In]pType  输入的时间字符串的类型，使用enum FormatType的值
     *  @return		bool true-成功 false-失败（无效的类型）
     */

    bool AssignTimeString(string pTimeStr, int pType);

    /*
     *  @brief		Year	 获取年 
     *  @return		u_short 
     */

    u_short Year() const;
    /*
     *  @brief		Year	 设置年 
     *  @param	[In]pYear  输入的年
     *  @return		void 
     */

    void Year(u_short pYear);
    /*
     *  @brief		Month	 获取月 
     *  @return		u_short 
     */

    u_short Month() const;
    /*
     *  @brief		Month	 设置月 
     *  @param	[In]pMonth  输入的月
     *  @return		void 
     */

    void Month(u_short pMonth);
    /*
     *  @brief		Day	 获取日 
     *  @return		u_short 
     */

    u_short Day() const;
    /*
     *  @brief		Day	 设置日 
     *  @param	[In]pDay  输入的日
     *  @return		void 
     */

    void Day(u_short pDay);
    /*
     *  @brief		Hour	 获取时 
     *  @return		u_short 
     */

    u_short Hour() const;
    /*
     *  @brief		Hour	 设置时 
     *  @param	[In]pHour  输入的时
     *  @return		void 
     */

    void Hour(u_short pHour);
    /*
     *  @brief		Minute	 获取分 
     *  @return		u_short 
     */

    u_short Minute() const;
    /*
     *  @brief		Minute	 设置分 
     *  @param	[In]pMinute  输入的分
     *  @return		void 
     */

    void Minute(u_short pMinute);
    /*
     *  @brief		Second	 获取秒 
     *  @return		u_short 
     */

    u_short Second() const;
    /*
     *  @brief		Second	 设置秒 
     *  @param	[In]pSecond  输入的秒
     *  @return		void 
     */

    void Second(u_short pSecond);
    /*
     *  @brief		MilliSecond	 获取毫秒 
     *  @return		double 
     */

    double MilliSecond() const;
    /*
     *  @brief		MilliSecond	 设置毫秒 
     *  @param	[In]pMilliSecond  输入的毫秒
     *  @return		void 
     */

    void MilliSecond(double pMilliSecond);
    /*
     *  @brief		TimeZone	 获取时区 
     *  @return		int 
     */

    int TimeZone() const;
    /*
     *  @brief		TimeZone	 设置时区 
     *  @param	[In]val  输入的时区
     *  @return		void 
     */

    void TimeZone(int val);

    /*
     *  @brief		DiffTime	 比较输入的CXJTime对象与自身的时间差
     *  @param	[In]pTime  输入的CXJTime对象
     *  @return		double 时间差，单位秒，毫秒以小数形式体现，如（100.01s）
     */

    double DiffTime(CXJTime& pTime);

    /*
     *  @brief		operator<	小于操作，返回是否比输入的CXJTime对象小
     *  @param	[In]pTime  输入的CXJTime对象
     *  @return		bool 
     */
    bool operator< (CXJTime& pTime);
    /*
     *  @brief		operator==	 等于操作，返回是否与输入的CXJTime对象相等
     *  @param	[In]pTime  输入的CXJTime对象
     *  @return		bool
     */
    bool operator== (CXJTime& pTime);
    /*
     *  @brief		Time_t	 返回Time_t类型的时间 
     *  @return		time_t 
     */

    time_t Time_t();
protected:
    /*
     *  @brief		ConvertSttpTime12ToTM	 将STTP12位时间字符串转换为tm结构 
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertSttpTime12ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime14ToTM	 将STTP14位时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertSttpTime14ToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertSttpTime15ToTM	 将STTP15位时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertSttpTime15ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime19ToTM	 将STTP19位时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertSttpTime19ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime26ToTM	 将STTP26位时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertSttpTime26ToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertCP32Time2aToTM	 将103CP32Time2a时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertCP32Time2aToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertCP56Time2aToTM	 将103CP56Time2a时间字符串转换为tm结构  
     *  @param	[In]pTimeStr  输入的时间字符串
     *  @return		void 
     */

    void ConvertCP56Time2aToTM( string &pTimeStr);   
private:
    /**	\brief  时区*/
    int         m_iTimeZone;

    /**	\brief  tm格式的时间值*/
    struct tm   m_Time;

    /**	\brief  毫秒，可以小数形式记录比毫秒小的单位的值，如100.1*/
    double      m_dMilliSecond;
};


#endif // XJTime_h__