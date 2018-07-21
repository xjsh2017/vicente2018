/********************************************************************
	created:	2008/09/22
	created:	22:9:2008   14:51
	file base:	MessageLog
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef MessageLog_h__
#define MessageLog_h__
#pragma warning (disable   :   4275)
#include "LogFile.h"
#include "XJLock.h"
#include "XJString.h"

typedef struct _FILE_DES_STRUCT
{
    string   strFilePath;
    time_t   tFileTime;
}FILE_DES_STRUCT;

/**
 * @defgroup 	CMessageLog	类说明
 * @{
 */
/**
 * @brief       报文日志类，以指定的hostname创建目录，在该目录下保存日志，
 *              日志按日期形成子目录，按小时生成文件。主要用于保存报文。
 * @author      qingch
 * @version     ver1.0
 * @date        07/10/08

 *
 * example
 * @code
 *    
 * @endcode
 */


class CMessageLog  :public CLogFile
{
public:
    /*
     *  @brief   	构造函数
     *  @return 	 
     */

	CMessageLog();
    /*
     *  @brief   	析构函数
     *  @return 	 
     */

	virtual ~CMessageLog();
    /*
     *  @brief   	打开日志方法
     *  @return 	BOOL true-成功 false-失败
     */

	virtual BOOL Open();
    /*
     *  @brief   	打开日志方法
     *  @param 		[In]const char *  指定的Host名称
     *  @return 	BOOL true-成功 false-失败
     */

	virtual BOOL Open(const char * pszHostName);
    /*
     *  @brief   	设置日志根目录
     *  @param 		[In]const char *  指定的根目录
     *  @return 	bool true-成功 false-失败
     */

	virtual bool SetLogPath(const char * pszPath);
    /*
     *  @brief   	FUNCTIONNAME	 简要的函数功能说明文字 
     *  @param 		[In/Out]a param of Type  const char *  输入参数说明
     *  @return 	bool 
     */

	virtual bool SetHostName( const char * pHostName);
    /*
     *  @brief   	设置日志保存天数
     *  @param 		[In]int  日志保存天数
     *  @return 	bool true-成功 false-失败
     */

	virtual bool SetLogSaveDays(int pSaveDays);
    /*
     *  @brief   	按照指定的级别格式化输入
     *  @param 		[In]int nCurLevel 日志级别
     *  @param 		[In]const char*  要写入的format串
     *  @param 		[In]...  可变参数列表
     *  @return 	int 成功返回写入数据数，失败返回-1
     */

	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);	
	
    /*
     *  @brief   	按指定的级别写入字符串
     *  @param 		[In]const char *  输入的字符串
     *  @param 		[In]int           日志级别
     *  @return 	void 
     */

	virtual void Add(const char * pchLog,int nCurLevel= CLogFile::trace);
	
    /*
     *  @brief   	按照指定的长度记录指定缓冲区的内容
     *  @param 		[In]const char *  要写入的字符串
     *  @param 		[In]int  内容长度
     *  @param 		[In]int  该条日志级别
     *  @return 	void 
     */

	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = CLogFile::trace);
protected:
    /*
     *  @brief   	操作前的准备工作
     *  @return 	BOOL true-成功 false-失败
     */

	BOOL SharedOperateStart();
    /*
     *  @brief   	日志文件扫描、清理工作
     *  @return 	void 
     */

	void DoScanAndClean();
	
    /*
     *  @brief   	扫描本日志对象路径下的按天存储的日志文件夹情况 
     *  @param 		[Out]a param of Type  deque<string> &  扫描得到的文件夹列表
     *  @return 	void 
     */

	void ScanLogPath( deque<FILE_DES_STRUCT> &m_DirList );
    /*
     *  @brief   	清除指定文件夹 
     *  @param 		[In]a param of Type  const string&  指定的文件夹路径
     *  @return 	void 
     */

	void CleanDir(const string& pPathName);
private:
	CMessageLog (CMessageLog&);
	CMessageLog& operator = (CMessageLog&);

	/** @brief  		 日志保存天数 */
	int	  m_iLogSaveDays;
	/** @brief  		 日志文件路径 */
	char   m_szLogPath[FILE_NAME_MAX_LEN];

	/** @brief  		 以小时生成的文件名 */
	string m_strFileName;

	/** @brief  		 文件根目录 */
	string m_strLogRootPath;

	/** @brief  		 报文日志文件夹名称*/
	string m_strLogName;

  	/** @brief  		 以日期生成的子目录名 */
  	string m_strRelativePath;

	/** @brief 文件锁 */
	CXJLock	m_Lock;
};
/** @} */ //OVER
#endif // MessageLog_h__
