#if !defined(COMTRADEPARSEENGINE_H_)
#define COMTRADEPARSEENGINE_H_

#include "../../common/define.h"
#include "../../common/Interface/ComtradeDefine.h"
#include "../../common/XJString.h"
#include "../../common/xjlib.h"

#ifdef OS_WINDOWS
#define LIBNAME_COMTRADEPARSE "XJComtrade.dll"
#else 
#define LIBNAME_COMTRADEPARSE "libXJComtrade.so"
#endif

//定义名以ComtradeParse的缩写cp开头
typedef int		(*cp_LoadFile)(char*, char*, XJHANDLE&); //打开文件
typedef void	(*cp_CloseFile)(XJHANDLE); //关闭文件
typedef void	(*cp_GetCFGAttr)(XJHANDLE, ComtradeCFG&); //取得CFG文件记录的波形数据配置属性
typedef int		(*cp_GetRatesCount)(XJHANDLE); //取得采样频率总数
typedef void	(*cp_GetRateSample)(XJHANDLE, int, float&, int&); //取得指定采样频率索引的采样频率和采样点数
typedef float	(*cp_GetRate)(XJHANDLE, int); //取得指定采样点的采样频率
typedef int		(*cp_GetChannelCFG)(XJHANDLE, int, ComtradeChannel&);//取得指定通道的配置信息
typedef int		(*cp_GetChannelType)(XJHANDLE, int);//取得指定通道的类型
typedef int		(*cp_GetTimeRelativeTrigger)(XJHANDLE, int);//取得指定采样点相对于故障触发时刻的时间
typedef int		(*cp_GetTimeRelativeStart)(XJHANDLE, int);//取得指定采样点相对于采样开始时刻的时间
typedef float	(*cp_GetRealValue)(XJHANDLE, int, int, int);//取得指定通道指定采样点的真实值
typedef int		(*cp_GetRecordValue)(XJHANDLE, int, int);//取得指定通道指定采样点的记录值
typedef int		(*cp_GetTotalSamples)(XJHANDLE);//取得采样点总数
typedef int		(*cp_GetTriggerTime)(XJHANDLE);//取得故障触发时刻相对于开始采样时刻的相对时间
typedef int		(*cp_GetTotalTime)(XJHANDLE);//取得总时间
typedef float	(*cp_GetMaxValue)(XJHANDLE,int,int,int);//取得指定通道最大值
typedef void	(*cp_GetCTPT)(XJHANDLE, float&, float&);//取得CTPT
typedef void	(*cp_SetCTPT)(XJHANDLE, float, float);//设置CTPT
typedef double	(*cp_GetTimeRelativeTriggerMSEC)(XJHANDLE, int);//取得指定采样点相对于故障触发时刻的时间，返回微秒值 
typedef double	(*cp_GetTimeRelativeStartMSEC)(XJHANDLE, int);//取得指定采样点相对于采样开始时刻的时间，返回微秒值
typedef HdrObj*	(*cp_GetHdrInfoObject)(XJHANDLE);
typedef std::string	(*cp_GetHdrInfoJSON)(XJHANDLE);

class ComtradeParseEngine
{
public:
	ComtradeParseEngine();
	virtual ~ComtradeParseEngine();
//Attribute
public:
	/** @brief           动态库句柄*/
	XJHANDLE	m_hModule;
private:
	/** @brief           打开文件接口*/
	cp_LoadFile m_pLoadFile;
	/** @brief           关闭文件接口*/
	cp_CloseFile m_pCloseFile;
	/** @brief           取得CFG文件记录的波形数据配置属性接口*/
	cp_GetCFGAttr m_pGetCFGAttr;
	/** @brief           取得采样频率总数接口*/
	cp_GetRatesCount m_pGetRatesCount;
	/** @brief           取得指定采样频率索引的采样频率和采样点数接口*/
	cp_GetRateSample m_pGetRateSample;
	/** @brief           取得指定采样点的采样频率*/
	cp_GetRate m_pGetRate;
	/** @brief           取得指定通道的配置信息*/
	cp_GetChannelCFG m_pGetChannelCFG;
	/** @brief           取得指定通道的类型*/
	cp_GetChannelType m_pGetChannelType;
	/** @brief           取得指定采样点相对于故障触发时刻的时间*/
	cp_GetTimeRelativeTrigger m_pTimeRelativeTrigger;
	/** @brief           取得指定采样点相对于采样开始时刻的时间*/
	cp_GetTimeRelativeStart m_pTimeRelativeStart;
	/** @brief           取得指定通道指定采样点的真实值*/
	cp_GetRealValue m_pGetRealValue;
	/** @brief           取得采样点总数*/
	cp_GetTotalSamples m_pGetTotalSamples;
	/** @brief           取得故障触发时刻相对于开始采样时刻的相对时间*/
	cp_GetTriggerTime m_pGetTriggerTime;
	/** @brief           取得总时间*/
	cp_GetTotalTime m_pGetTotalTime;
	/** @brief           取得指定通道最大值*/
	cp_GetMaxValue m_pGetMaxValue;
	/** @brief           取得指定通道指定采样点的记录值*/
	cp_GetRecordValue m_pGetRecordValue;
	/** @brief           设置CTPT变比*/
	cp_SetCTPT m_pSetCTPT;
	/** @brief           取得CTPT变比*/
	cp_GetCTPT m_pGetCTPT;
	cp_GetTimeRelativeTriggerMSEC m_pGetTimeRelativeTriggerMSEC;
	cp_GetTimeRelativeStartMSEC m_pGetTimeRelativeStartMSEC;
	cp_GetHdrInfoObject m_pGetHdrInfoObject;
	cp_GetHdrInfoJSON m_pGetHdrInfoJSON;

//Operations
public:
/*
 *  @brief   	LoadCPLib	 装载XJComtrade.dll 
 *  @return 	bool 装载成功返回TRUE,失败返回FALSE
 */
	bool xjcp_LoadCPLib();
public:
/*
 *  @brief   	XJLoadFile	 载入波形文件 
 *  @param 		[In]a param of Type  char*  波形文件路径
 *  @param 		[Out]a param of Type  char*&  错误信息
 *  @param 		[Out]a param of Type  int&  生成的波形文件数据对象指针
 *  @return 	int 载入成功返回>=0, 失败返回<0
 */
	int xjcp_LoadFile(char* sFilePath, char* sError, XJHANDLE& hComtrade);

/*
 *  @brief   	XJCloseFile	 关闭波形文件 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @return 	void
 */
	void xjcp_CloseFile(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetCFGAttr	 取得CFG文件记录的波形数据配置属性 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[Out]a param of Type  ComtradeCFG&  波形数据配置属性
 *  @return 	void 
 */
	void xjcp_GetCFGAttr(XJHANDLE hComtrade, ComtradeCFG& cfgAttr);

/*
 *  @brief   	XJGetRatesCount	 取得采样频率数
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @return 	int 采样频率数 
 */
	int	xjcp_GetRatesCount(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetRateSample	 取得指定采样频率索引的采样频率和采样点数 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定采样频率索引.索引从0开始
 *	@param		[Out]a param of Type float& 采样频率
 *	@param		[Out]a param of Type int& 采样频率下的采样点个数
 *  @return 	void 
 */
	void xjcp_GetRateSample(XJHANDLE hComtrade, int nRateIndex, float& fSampleRate, int& nSampelCount);

/*
*  @brief   	XJGetRate	 取得指定采样点的采样频率 
*  @param 		[In]a param of Type  int  指定波形文件数据对象指针
*  @param 		[In]a param of Type  int  指定采样点
 *  @return 	float 采样频率 
 */
	float xjcp_GetRate(XJHANDLE hComtrade, int nSamplePos);

/*
 *  @brief   	XJGetChannelCFG	 取得指定通道的配置信息 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定通道索引.索引从0开始
 *	@param		[Out]a param of Type ComtradeAChannel& 通道配置信息结构体
 *  @return 	int  通道类型.1表示模拟量通道，0表示开关量通道
 */
	int	xjcp_GetChannelCFG(XJHANDLE hComtrade, int nCn, ComtradeChannel& chl);

/*
 *  @brief   	XJGetChannelType	 取得指定通道的类型 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定通道索引.索引从0开始
 *  @return 	int  通道类型.1表示模拟量通道，0表示开关量通道
 */
	int	xjcp_GetChannelType(XJHANDLE hComtrade, int nCn);

/*
 *  @brief   	XJGetTimeRelativeTrigger	 取得指定采样点相对于故障触发时刻的时间 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *  @return 	int  相对于故障触发时刻的时间（毫秒）
 */
	int	xjcp_GetTimeRelativeTrigger(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetTimeRelativeStart	 取得指定采样点相对于采样开始时刻的时间 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *  @return 	int  相对于采样开始时刻的时间（毫秒）
 */
	int	xjcp_GetTimeRelativeStart(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetRealValue	 取得指定通道指定采样点的真实值
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定通道索引.索引从0开始
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *	@param		[In]a param of Type  int  指定需要的值的类型，默认为二次值.0为二次值，1为一次值
 *  @return 	float  指定通道指定采样点的真实值
 */
	float xjcp_GetRealValue(XJHANDLE hComtrade, int nCn, int nSampleNo, int nDataType = 0);

/*
 *  @brief   	XJGetRecordValue	 取得指定通道指定采样点的记录值
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定通道索引.索引从0开始
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *  @return		int  指定通道指定采样点的记录值
 */
	int xjcp_GetRecordValue(XJHANDLE hComtrade, int nCn, int nSampleNo);

/*
 *  @brief   	XJGetTotalSamples	 取得采样点总数 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @return 	int  采样点总数
 */
	int	xjcp_GetTotalSamples(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetTriggerTime	 取得故障触发时刻相对于开始采样时刻的相对时间
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @return 	int  故障触发时刻相对于开始采样时刻的相对时间（毫秒）
 */
	int	xjcp_GetTriggerTime(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetTotalTime	 取得总时间 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @return 	int  总时间（毫秒）
 */
	int	xjcp_GetTotalTime(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetMaxValue	 取得指定通道最大值
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定通道索引.索引从0开始
 *	@param		[In]a param of Type  int  指定需要的值的类型，0为二次值，1为一次值
 *	@param		[In]a param of Type  int  指定是否取公共最大值. 0表示取公共最大值,1表示各通道自己的最大值
 *  @return 	float  最大值
 */
	float xjcp_GetMaxValue(XJHANDLE hComtrade, int nCn, int nDataType, int nGroup = 0);

/*
 *  @brief   	XJGetCTPT	 取得CTPT变比
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[Out]a param of Type  float CT变比
 *	@param		[Out]a param of Type  float PT变比
 *  @return 	void
 */
	void xjcp_GetCTPT(XJHANDLE hComtrade, float& fCt, float& fPT);

/*
 *  @brief   	XJSetCTPT	 设置CTPT变比
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  float CT变比
 *	@param		[In]a param of Type  float PT变比
 *  @return 	void
 */
	void xjcp_SetCTPT(XJHANDLE hComtrade, float fCT, float fPT);

/*
 *  @brief   	XJGetTimeRelativeTrigger	 取得指定采样点相对于故障触发时刻的时间，返回微秒值 
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *  @return 	double  相对于故障触发时刻的时间（微秒）
 */
	double xjcp_GetTimeRelativeTriggerMSEC(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetTimeRelativeStart	 取得指定采样点相对于采样开始时刻的时间，返回微秒值
 *  @param 		[In]a param of Type  int  指定波形文件数据对象指针
 *  @param 		[In]a param of Type  int  指定采样点索引.索引从0开始
 *  @return 	double  相对于采样开始时刻的时间（微秒）
 */
	double xjcp_GetTimeRelativeStartMSEC(XJHANDLE hComtrade, int nSampleNo);

	HdrObj* xjcp_GetHdrInfoObject(XJHANDLE hComtrade);

	std::string xjcp_GetHdrInfoJSON(XJHANDLE hComtrade);
};
#endif
