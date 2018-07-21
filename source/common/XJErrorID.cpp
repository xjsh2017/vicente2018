/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   10:49
	file base:	XJErrorID
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/

#include "XJErrorID.h"

char* CommErrMsg[] = {
	/* 0 */ "No error",
	/* 1 */ " Memeroy is not enough ",
	/* 2 */ "Failed in Loading lib",
	/* 3 */ " Failed in Getting Function Address",
	/* 4 */ "Runtime Exception",
	/* 5 */ "Invalid Address or Point",
	/* 6 */ "Failed in Creating Thread",
	/* 7 */ " Failed in Ending Thread ",
	/* 8 */ "Verify Failed",
	/* 9 */ "Can't find the function point",
	/* 10 */ "Sttp translat to Data Failed",
	/* 11 */ "Data translat to Sttp Failed",
};


char* BusinessErrMsg[] = {
	/* 0 */ "No error",
	/* 1 */ "主站与该子站通讯中断",
	/* 2 */ "该子站正在初始化配置",
	/* 3 */ "下发到该子站的命令无效",
	/* 4 */ "该子站回复操作失败",
	/* 5 */ "该子站的回复无效",
	/* 6 */ "接受该子站的回复超时",
	/* 7 */ "对该子站的数据处理失败",
	/* 8 */ "短链接连接异常",
	/* 9 */ "无效的命令数据",
	/* 10 */ "Sttp客户端缓存已满",
	/* 11 */ "Sttp客户端与服务端连接中断",
	/* 12 */ "客户端接收超时",
	/* 13 */ "发布命令失败，子站规约库可能未启动",
};

char* OtherErrMsg[] = {
	/* 0 */ "No error",
};

char* getErrorMsg(int pErrorNo)
{
	if ((pErrorNo < MYDEF_ERROR_START_NO) || (pErrorNo > MYDEF_ERROR_END_NO))
	{
		return "No Error";
	}
	else {
		int offSet(0);
		if (pErrorNo <= COM_ERROR_END_NO){ /* 通用错误 */
			offSet = pErrorNo - COM_ERROR_START_NO;
			if (offSet > 11 )
			{
				return "No Error";
			} 
			else
			{
				return CommErrMsg[offSet];
			}
		}
		else if (pErrorNo <= BUSINESS_ERROR_END_NO){ /* 业务错误 */
			offSet = pErrorNo - BUSINESS_ERROR_START_NO;
			if (offSet > 13 )
			{
				return "No Error";
			} 
			else
			{
				return BusinessErrMsg[offSet];
			}
		}
		else { /* 其他错误 */
			offSet = pErrorNo - OTHER_ERROR_START_NO;
			if (offSet > 0 )
			{
				return "No Error";
			} 
			else
			{
				return OtherErrMsg[offSet];
			}
		}
	}
}