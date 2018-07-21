// ***************************************************************
//  SttpEngine   version:  1.0    date: 25:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: STTP引擎,负责启动与关闭STTP客户端接口库,封装接口
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(XJNOTIFY_H_)
#define XJNOTIFY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataEngine.h"
#include "vccommon_define.h"

class CXJNotify : public CObject
{
public:	
	CXJNotify(CDataEngine* pDataEngine);
	virtual ~CXJNotify();

public:
	CDataEngine* m_pDataEngine;

public:
/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	GetDescribe	 获得事件描述 
 *  @return 	virtual CString 事件描述字符串
 */
	virtual CString GetDescribe();

/*
 *  @brief   	GetSimpleDescribe	 获得事件简要描述 
 *  @return 	virtual CString 事件简要描述字符串
 */
	virtual CString GetSimpleDescribe();

/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	virtual BOOL BuildReleation();

/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	virtual CXJNotify* Clone();

/*
 *  @brief   	GetFaultType	 将故障类型解释成文字
 *  @return 	virtual CString	 解释后的文字
 */
	virtual CString GetFaultType(CString sFaultType);

/*
 *  @brief   	GetTriggerType	 得到相别说明 
 *  @param 		[In]a param of Type  CString  相别缩写
 *  @return 	virtual CString 相别说明
 */
	virtual CString GetTriggerType(CString sTrigger);

	/*
	* @brief	GetNotifyType	取得通知类型
	* @return   int	通知类型
	*/
	virtual int GetNotifyType();
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
