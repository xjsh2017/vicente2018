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
 * @defgroup 	CMessageLog	��˵��
 * @{
 */
/**
 * @brief       ������־�࣬��ָ����hostname����Ŀ¼���ڸ�Ŀ¼�±�����־��
 *              ��־�������γ���Ŀ¼����Сʱ�����ļ�����Ҫ���ڱ��汨�ġ�
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
     *  @brief   	���캯��
     *  @return 	 
     */

	CMessageLog();
    /*
     *  @brief   	��������
     *  @return 	 
     */

	virtual ~CMessageLog();
    /*
     *  @brief   	����־����
     *  @return 	BOOL true-�ɹ� false-ʧ��
     */

	virtual BOOL Open();
    /*
     *  @brief   	����־����
     *  @param 		[In]const char *  ָ����Host����
     *  @return 	BOOL true-�ɹ� false-ʧ��
     */

	virtual BOOL Open(const char * pszHostName);
    /*
     *  @brief   	������־��Ŀ¼
     *  @param 		[In]const char *  ָ���ĸ�Ŀ¼
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	virtual bool SetLogPath(const char * pszPath);
    /*
     *  @brief   	FUNCTIONNAME	 ��Ҫ�ĺ�������˵������ 
     *  @param 		[In/Out]a param of Type  const char *  �������˵��
     *  @return 	bool 
     */

	virtual bool SetHostName( const char * pHostName);
    /*
     *  @brief   	������־��������
     *  @param 		[In]int  ��־��������
     *  @return 	bool true-�ɹ� false-ʧ��
     */

	virtual bool SetLogSaveDays(int pSaveDays);
    /*
     *  @brief   	����ָ���ļ����ʽ������
     *  @param 		[In]int nCurLevel ��־����
     *  @param 		[In]const char*  Ҫд���format��
     *  @param 		[In]...  �ɱ�����б�
     *  @return 	int �ɹ�����д����������ʧ�ܷ���-1
     */

	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);	
	
    /*
     *  @brief   	��ָ���ļ���д���ַ���
     *  @param 		[In]const char *  ������ַ���
     *  @param 		[In]int           ��־����
     *  @return 	void 
     */

	virtual void Add(const char * pchLog,int nCurLevel= CLogFile::trace);
	
    /*
     *  @brief   	����ָ���ĳ��ȼ�¼ָ��������������
     *  @param 		[In]const char *  Ҫд����ַ���
     *  @param 		[In]int  ���ݳ���
     *  @param 		[In]int  ������־����
     *  @return 	void 
     */

	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = CLogFile::trace);
protected:
    /*
     *  @brief   	����ǰ��׼������
     *  @return 	BOOL true-�ɹ� false-ʧ��
     */

	BOOL SharedOperateStart();
    /*
     *  @brief   	��־�ļ�ɨ�衢������
     *  @return 	void 
     */

	void DoScanAndClean();
	
    /*
     *  @brief   	ɨ�豾��־����·���µİ���洢����־�ļ������ 
     *  @param 		[Out]a param of Type  deque<string> &  ɨ��õ����ļ����б�
     *  @return 	void 
     */

	void ScanLogPath( deque<FILE_DES_STRUCT> &m_DirList );
    /*
     *  @brief   	���ָ���ļ��� 
     *  @param 		[In]a param of Type  const string&  ָ�����ļ���·��
     *  @return 	void 
     */

	void CleanDir(const string& pPathName);
private:
	CMessageLog (CMessageLog&);
	CMessageLog& operator = (CMessageLog&);

	/** @brief  		 ��־�������� */
	int	  m_iLogSaveDays;
	/** @brief  		 ��־�ļ�·�� */
	char   m_szLogPath[FILE_NAME_MAX_LEN];

	/** @brief  		 ��Сʱ���ɵ��ļ��� */
	string m_strFileName;

	/** @brief  		 �ļ���Ŀ¼ */
	string m_strLogRootPath;

	/** @brief  		 ������־�ļ�������*/
	string m_strLogName;

  	/** @brief  		 ���������ɵ���Ŀ¼�� */
  	string m_strRelativePath;

	/** @brief �ļ��� */
	CXJLock	m_Lock;
};
/** @} */ //OVER
#endif // MessageLog_h__
