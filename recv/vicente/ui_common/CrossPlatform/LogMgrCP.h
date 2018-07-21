// LogMgrCP.h: interface for the LogMgrCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_)
#define AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../common/LogFile.h"
#include "../../common/XJLock.h"
#include "../../common/MessageLog.h"
#include "../../common/XJString.h"

const int log_level_error     =      1;          // ������־
const int log_level_warning   =      2;          // ������־
const int log_level_trace     =      3;          // ������־
void WriteLog_CP(CXJString sLog, int nLogLevel = 3);

/**
 * @brief    MaintenanceData  ��ƽ̨�汾��־����               
 * @author   lyh
 * @version   1.0
 * @date      2012/7/3
 */
class LogMgrCP  
{
private:
	LogMgrCP();
public:
	virtual ~LogMgrCP();

///////////////////ʵ�ֵ���ģʽ/////////////////////////////
private:
	/** @brief			���ྲ̬����*/
	static LogMgrCP* m_instance;
	/** @brief			����getInstance��������*/
	static CXJLock* m_InstanceLock;
public:
	/*
	* @brief	getInstance	ȡ��LogMgrCP����ָ��
	* @return   LogMgrCP*	LogMgrCP����ָ��
	*/
	static LogMgrCP* getInstance();
///////////////////ʵ�ֵ���ģʽ end//////////////////////////

private:
	/** @brief			��־�ļ�*/
	CMessageLog m_File;
public:
	/*
	* @brief	SetLogPath	������־�ļ�·��
	* @param	[In]a param of Type CXJString sPath	·��	
	* @param	[In]a param of Type CXJString sFolder ��־�ļ�������		
	* @return   void	
	*/
	void SetLogPath(CXJString sPath = "./", CXJString sFolder = "Temp_Log");

	/*
	* @brief	WriteLog	д��־����,���ⲿ����
	* @param	[In]a param of Type CXJString sLog	��־�ַ���	
	* @param	[In]a param of Type int nLogLevel	��־�ȼ�	
	* @return   void	
	*/
	void WriteLog(CXJString sLog, int nLogLevel);

	/*
	* @brief	SetLogLevel	������־�ļ��ļ�¼�ȼ�
	* @param	[In]a param of Type int nLogLevel ��־�ȼ�		
	* @return   void	
	*/
	void SetLogLevel(int nLogLevel);
};

#endif // !defined(AFX_LOGMGRCP_H__0467ADF6_952D_4172_8CEF_6538751BB770__INCLUDED_)
