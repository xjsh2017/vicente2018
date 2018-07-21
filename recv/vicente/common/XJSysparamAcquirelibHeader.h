#ifndef XJSysparamAcquirelibHeader_h__
#define XJSysparamAcquirelibHeader_h__
//屏蔽告警消息
#pragma   warning   (disable   :   4786)
#pragma   warning   (disable   :   4275)

#include "define.h"//标准库的定义

/** @brief  		 动态库名称定义 */
#ifdef OS_WINDOWS
#define XJ_LIB_SYS_PARAM_ACQUIRE      "XJSysparamAcquirelib.dll"
#else
#define XJ_LIB_SYS_PARAM_ACQUIRE      "libXJSysparamAcquirelib.so"
#endif

/**	\brief	The _XJ_DISK_CAP_RESULT_ struct
磁盘容量结果信息
*/
typedef struct _XJ_DISK_CAP_RESULT_
{
	/**	\brief 磁盘序列号*/
	string strDiskName;
	
	/**	\brief 挂载点，在windows下为盘符*/
	string strMountedName;
	
	/**	\brief 已用百分比*/
	string strUsepercent;
	
	/**	\brief 总容量，单位：kb*/
	string strTotalsize;
	
	/**	\brief 剩余容量，单位：kb*/
	string strFreesize;
}XJ_DISK_CAP_RESULT;

/*****************************接口定义*****************************/
/*
 *  @brief   	Start	 开始，加载信息 
 *  @param 		[Out]a param of Type  char*  返回的附加信息
 *  @return 	int 
 */
	typedef	int (*gSyspararmlib_Start)(char*);

/*
 *  @brief   	End	 关闭，销毁资源 
 *  @param 		[Out]a param of Type  char*  返回的附加信息
 *  @return 	int 
 */
	typedef	int (*gSyspararmlib_End)(char*);

/*
 *  @brief   	SetLibLogparam	 设置日志参数 
 *  @param 		[In]a param of Type  char*  日志路径
 *  @param 		[In]a param of Type  int  日志级别
 *  @param 		[In]a param of Type  long  日志大小
 *  @return 	bool true：成功 false：失败
 */
	typedef	bool (*gSyspararmlib_SetLibLogparam)(char*, int /*= CLogFile::trace*/, long /*= 10240*/);

/*
 *  @brief   	GetDiskpartitionInfo	 获取磁盘上各分区的信息 
 *  @param 		[Out]a param of Type  vector<XJ_DISK_CAP_RESULT>&  磁盘上各分区的信息
 *  @return 	bool true：成功 false：失败
 */
	typedef	bool (*gSyspararmlib_GetDiskpartitionInfo)(vector<XJ_DISK_CAP_RESULT>&);

/*
 *  @brief   	GetDiskInfo	 获取硬盘信息 
 *  @param 		[Out]a param of Type  XJ_DISK_CAP_RESULT&  硬盘信息结构体
 *  @return 	bool true：成功 false：失败
 */
	typedef	bool (*gSyspararmlib_GetDiskInfo)(XJ_DISK_CAP_RESULT&);

/** @} */ //OVER
#endif // XJSysparamAcquirelibHeader_h__

