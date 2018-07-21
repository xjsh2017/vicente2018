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

#pragma warning(disable : 4275)//����4275����

#include "define.h"

/**
 * @defgroup 	CXJTime	ʱ�䴦����
 * @{
 */
/**
 * @brief       ��������ipofasϵͳ�и���ʱ���ʽ�Ĵ���ת��
                ������SYSTIME��time_t��ʱ�乹��
                ��ʹ��AssignTimeString�ӿ� ������ָ�ʽ��ʱ���ַ���
                ��ʹ��GetTimeString�ӿ� ���Ϊ���ָ�ʽ��ʱ���ַ���
                �ṩ ��ֵ(=) С��(<) ����(==) DiffTime����
*  @author      qingch
 * @version     ver1.0
 * @date        03/06/10

 *
 * example
 * @code
 *    // ����˵�������ʹ��
      1. �������һ��SYSTIME������Ҫת��
            SYSTIME tSysTime;//�������ֵʡȥ
            CXJTime tTime(tSysTime);
            char chSttpTime26[27] = "";
            strncpy(chSttpTime26,tTime.GetTimeString(CXJTime::STTP26Time).c_str,26);
            //ת��������ʽ��ʱ���������Ӧ������
      2. �������һ��time_t������Ҫת��
            time_t tTTime;//�������ֵʡȥ
            CXJTime tTime(tTTime);
            char chSttpTime26[27] = "";
            strncpy(chSttpTime26,tTime.GetTimeString(CXJTime::STTP26Time).c_str,26);
            //ת��������ʽ��ʱ���������Ӧ������
      3. �����һ��ʱ���ַ�����Ҫת��
            char chTime[27] = "2010-06-02 16:03:55.009001";
            CXJTime tTime;
            tTime.AssignTimeString(chTime,CXJTime::STTP26Time);
            char ch103Time7[7] = "";
            strncpy(ch103Time7,tTime.GetTimeString(CXJTime::CP56Time2a).c_str,7);
            //ת��������ʽ��ʱ���������Ӧ������
 * @endcode
 */
/** @} */ //OVER

class CXJTime
{
public:
    enum FormatType{
        STTP12Time = 0,/*STTP�����е�12λʱ�䣬��ʽ����yymmddHHMMSS��*/
        STTP14Time = 1,/*STTP�����е�14λʱ�䣬��ʽ����yyyymmddHHMMSS��*/
        STTP15Time = 2,/*STTP�����е�15λʱ�䣬��ʽ����YYMMDDmmhhss+3λ��������*/
        STTP19Time = 3,/*STTP�����е�19λʱ�䣬��ʽ���� yyyy-mm-dd HH:MM:SS��*/
        STTP26Time = 4,/*STTP�����е�26λʱ�䣬��ʽ����  yyyy-mm-dd HH:MM:SS.6λms��*/
        CP32Time2a = 5,/*103�е�4λʱ�䣬{���룬�֣�����1����Ч��ʱ������2����ʱ��}*/
        CP56Time2a = 6,/*103�е�7λʱ�䣬{���룬�֣�����1����Ч��ʱ������2����ʱ�ƣ��µ�ĳ�죬���ڵ�ĳ�죬�£�����3���꣬����4}*/
    };

    /*
     *  @brief		CXJTime	 Ĭ�Ϲ��죬�������ʼ��Ϊ0 
     *  @return		 
     */

    CXJTime();
    /*
     *  @brief		CXJTime	 �������� 
     *  @param	[In]pXJTime  ���ڿ����������һ��CXJTime����
     *  @return		 
     */

    CXJTime(CXJTime& pXJTime);
    /*
     *  @brief		CXJTime	 ���캯������һ��SYSTIME������й��� 
     *  @param	[In]pSYSTime  ���ڹ����SYSTIME����
     *  @return		 
     */

    CXJTime(SYSTIME& pSYSTime);
    /*
     *  @brief		CXJTime	 ���캯������һ��time_t������й���  
     *  @param	[In]pTTime  ���ڹ����time_t����
     *  @return		 
     */

    CXJTime(time_t& pTTime);

    /*
     *  @brief		~CXJTime	 �������� 
     *  @return		 
     */

    ~CXJTime();

    /*
     *  @brief		operator=	 ��ֵ���� 
     *  @param	[In]pXJTime  ���ڸ�ֵ����һ��CXJTime����
     *  @return		 
     */
    CXJTime& operator= (CXJTime& pTime);

    /*
     *  @brief		GetTimeString	 ��ȡʱ���ַ��� 
     *  @param	[In]pType  Ԥ��ȡ���ַ��������ͣ�ʹ��enum FormatType��ֵ
     *  @return		string ʱ���ַ������
     */

    string GetTimeString(int pType);
    /*
     *  @brief		AssignTimeString	 ʹ��ʱ���ַ�����ֵ 
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @param	[In]pType  �����ʱ���ַ��������ͣ�ʹ��enum FormatType��ֵ
     *  @return		bool true-�ɹ� false-ʧ�ܣ���Ч�����ͣ�
     */

    bool AssignTimeString(string pTimeStr, int pType);

    /*
     *  @brief		Year	 ��ȡ�� 
     *  @return		u_short 
     */

    u_short Year() const;
    /*
     *  @brief		Year	 ������ 
     *  @param	[In]pYear  �������
     *  @return		void 
     */

    void Year(u_short pYear);
    /*
     *  @brief		Month	 ��ȡ�� 
     *  @return		u_short 
     */

    u_short Month() const;
    /*
     *  @brief		Month	 ������ 
     *  @param	[In]pMonth  �������
     *  @return		void 
     */

    void Month(u_short pMonth);
    /*
     *  @brief		Day	 ��ȡ�� 
     *  @return		u_short 
     */

    u_short Day() const;
    /*
     *  @brief		Day	 ������ 
     *  @param	[In]pDay  �������
     *  @return		void 
     */

    void Day(u_short pDay);
    /*
     *  @brief		Hour	 ��ȡʱ 
     *  @return		u_short 
     */

    u_short Hour() const;
    /*
     *  @brief		Hour	 ����ʱ 
     *  @param	[In]pHour  �����ʱ
     *  @return		void 
     */

    void Hour(u_short pHour);
    /*
     *  @brief		Minute	 ��ȡ�� 
     *  @return		u_short 
     */

    u_short Minute() const;
    /*
     *  @brief		Minute	 ���÷� 
     *  @param	[In]pMinute  ����ķ�
     *  @return		void 
     */

    void Minute(u_short pMinute);
    /*
     *  @brief		Second	 ��ȡ�� 
     *  @return		u_short 
     */

    u_short Second() const;
    /*
     *  @brief		Second	 ������ 
     *  @param	[In]pSecond  �������
     *  @return		void 
     */

    void Second(u_short pSecond);
    /*
     *  @brief		MilliSecond	 ��ȡ���� 
     *  @return		double 
     */

    double MilliSecond() const;
    /*
     *  @brief		MilliSecond	 ���ú��� 
     *  @param	[In]pMilliSecond  ����ĺ���
     *  @return		void 
     */

    void MilliSecond(double pMilliSecond);
    /*
     *  @brief		TimeZone	 ��ȡʱ�� 
     *  @return		int 
     */

    int TimeZone() const;
    /*
     *  @brief		TimeZone	 ����ʱ�� 
     *  @param	[In]val  �����ʱ��
     *  @return		void 
     */

    void TimeZone(int val);

    /*
     *  @brief		DiffTime	 �Ƚ������CXJTime�����������ʱ���
     *  @param	[In]pTime  �����CXJTime����
     *  @return		double ʱ����λ�룬������С����ʽ���֣��磨100.01s��
     */

    double DiffTime(CXJTime& pTime);

    /*
     *  @brief		operator<	С�ڲ����������Ƿ�������CXJTime����С
     *  @param	[In]pTime  �����CXJTime����
     *  @return		bool 
     */
    bool operator< (CXJTime& pTime);
    /*
     *  @brief		operator==	 ���ڲ����������Ƿ��������CXJTime�������
     *  @param	[In]pTime  �����CXJTime����
     *  @return		bool
     */
    bool operator== (CXJTime& pTime);
    /*
     *  @brief		Time_t	 ����Time_t���͵�ʱ�� 
     *  @return		time_t 
     */

    time_t Time_t();
protected:
    /*
     *  @brief		ConvertSttpTime12ToTM	 ��STTP12λʱ���ַ���ת��Ϊtm�ṹ 
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertSttpTime12ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime14ToTM	 ��STTP14λʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertSttpTime14ToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertSttpTime15ToTM	 ��STTP15λʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertSttpTime15ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime19ToTM	 ��STTP19λʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertSttpTime19ToTM( string &pTimeStr);
    /*
     *  @brief		ConvertSttpTime26ToTM	 ��STTP26λʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertSttpTime26ToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertCP32Time2aToTM	 ��103CP32Time2aʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertCP32Time2aToTM( string &pTimeStr);   
    /*
     *  @brief		ConvertCP56Time2aToTM	 ��103CP56Time2aʱ���ַ���ת��Ϊtm�ṹ  
     *  @param	[In]pTimeStr  �����ʱ���ַ���
     *  @return		void 
     */

    void ConvertCP56Time2aToTM( string &pTimeStr);   
private:
    /**	\brief  ʱ��*/
    int         m_iTimeZone;

    /**	\brief  tm��ʽ��ʱ��ֵ*/
    struct tm   m_Time;

    /**	\brief  ���룬����С����ʽ��¼�Ⱥ���С�ĵ�λ��ֵ����100.1*/
    double      m_dMilliSecond;
};


#endif // XJTime_h__