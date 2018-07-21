#include "WaveShapeEngine.h"


////////////////////////////////////////////////////////////////
// CPEngine

WaveShapeEngine::WaveShapeEngine()
{
	m_hModule = NULL;

	m_pLoadWaveFile = NULL;
	m_pDrawShape = NULL;
	m_pCloseWaveHandle = NULL;
	m_pSetCanvasRange = NULL;
	m_pGetFullSize = NULL;
	m_pSetShapeDirection = NULL;
	m_pSetIsChlUseColor = NULL;
	m_pSetCanvasBkColor = NULL;
	m_pSetCanvasStrockColor = NULL;
	m_pSetColorPhaseA = NULL;
	m_pSetColorPhaseB = NULL;
	m_pSetColorPhaseC = NULL;
	m_pSetColorPhaseN = NULL;
	m_pSetFontInfo = NULL;
	m_pSetTimeRate = NULL;
	m_pSetDataType = NULL;
	m_pSetDIStyle = NULL;
	m_pSetChlHeight = NULL;
	m_pSetRangeBySample = NULL;
	m_pSetRangeByTime = NULL;
	m_pExportToPDF = NULL;
	m_pPrintShape = NULL;
	m_pSetNameStyle = NULL;
}

WaveShapeEngine::~WaveShapeEngine()
{
	if(m_hModule != NULL)
	{
		xj_free_library(m_hModule);
		m_hModule = NULL;
		m_pLoadWaveFile = NULL;
		m_pDrawShape = NULL;
		m_pCloseWaveHandle = NULL;
		m_pSetCanvasRange = NULL;
		m_pGetFullSize = NULL;
		m_pSetShapeDirection = NULL;
		m_pSetIsChlUseColor = NULL;
		m_pSetCanvasBkColor = NULL;
		m_pSetCanvasStrockColor = NULL;
		m_pSetColorPhaseA = NULL;
		m_pSetColorPhaseB = NULL;
		m_pSetColorPhaseC = NULL;
		m_pSetColorPhaseN = NULL;
		m_pSetFontInfo = NULL;
		m_pSetTimeRate = NULL;
		m_pSetDataType = NULL;
		m_pSetDIStyle = NULL;
		m_pSetChlHeight = NULL;
		m_pSetRangeBySample = NULL;
		m_pSetRangeByTime = NULL;
		m_pExportToPDF = NULL;
		m_pPrintShape = NULL;
		m_pSetNameStyle = NULL;
	}
}

/************************************
函 数 名: xjws_LoadWSLib
功能概要: 装载XJComtrade.dll 
返 回 值: 装载成功返回TRUE,失败返回FALSE
参    数: param1 
		  param2 
************************************/
bool WaveShapeEngine::xjws_LoadWSLib()
{
	m_hModule = xj_load_library(LIBNAME_WAVESHAPE);
	if(m_hModule != NULL)
	{
		m_pLoadWaveFile = (int(*)(char* sFilePath, char* sError, XJHANDLE& hWaveShape))xj_get_addr(m_hModule, "LoadWaveFile");
		m_pDrawShape = (int(*)(cairo_surface_t * pSur, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "DrawShape");
		m_pCloseWaveHandle = (void(*)(XJHANDLE hWaveShape))xj_get_addr(m_hModule, "CloseWaveHandle");
		m_pSetCanvasRange = (void(*)(int nStartX, int nStartY, int nWidth, int nHeight, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetCanvasRange");
		m_pGetFullSize = (void(*)(int& nWidth, int& nHeight, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "GetFullSize");
		m_pSetShapeDirection = (void(*)(int nDire, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetShapeDirection");
		m_pSetIsChlUseColor = (void(*)(int nUseCol, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetIsChlUseColor");
		m_pSetCanvasBkColor = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetCanvasBkColor");
		m_pSetCanvasStrockColor = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetCanvasStrockColor");
		m_pSetColorPhaseA = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetColorPhaseA");
		m_pSetColorPhaseB = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetColorPhaseB");
		m_pSetColorPhaseC = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetColorPhaseC");
		m_pSetColorPhaseN = (void(*)(int nR, int nG, int nB, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetColorPhaseN");
		m_pSetFontInfo = (void(*)(char* sFontFamily, int nFontSize, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetFontInfo");
		m_pSetTimeRate = (void(*)(double dTimeRate, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetTimeRate");
		m_pSetDataType = (void(*)(int nDataType, int nMaxDataType, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetDataType");
		m_pSetDIStyle = (void(*)(int nDIStyle, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetDIStyle");
		m_pSetChlHeight = (void(*)(int nAIHeight, int nDIHeight, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetChlHeight");
		m_pSetRangeBySample = (void(*)(int nStartSample, int nEndSample, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetRangeBySample");
		m_pSetRangeByTime = (void(*)(int nStartTime, int nEndTime, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "SetRangeByTime");
		m_pExportToPDF = (int(*)(char* filepath, XJHANDLE hWaveShape))xj_get_addr(m_hModule, "ExportToPDF");
		m_pPrintShape = (int(*)(XJHANDLE))xj_get_addr(m_hModule, "PrintShape");
		m_pWaveStartPosDrawing=(void(*)(int&,int&,XJHANDLE))xj_get_addr(m_hModule, "WaveStartPosDrawing");
		m_pGetTimeRate = (double(*)(XJHANDLE))xj_get_addr(m_hModule, "GetTimeRate");
		m_pGetShapeDirection=(int(*)(XJHANDLE))xj_get_addr(m_hModule, "GetShapeDirection");
		m_pGetTriggerTime=(int(*)(XJHANDLE))xj_get_addr(m_hModule, "GetTriggerTime");
		m_pSetNameStyle=(void(*)(int,XJHANDLE))xj_get_addr(m_hModule, "SetNameStyle");
	}
	else
	{
		printf("载入WaveShape动态库失败!");
		return false;
	}
	return true;
}

/****************************************************
Date:2012/3/26  Author:LYH
函数名:   LoadWaveFile	
返回值:   int	载入成功返回>=0, 失败返回<0
功能概要: 载入波形文件
参数: char * sFilePath	波形文件路径
参数: char * sError	错误信息
参数: XJHANDLE & hWaveShape	生成的波形文件数据对象指针
*****************************************************/
int WaveShapeEngine::xjws_LoadWaveFile( char* sFilePath, char* sError, XJHANDLE& hWaveShape )
{
	if(m_hModule == NULL)
		return -1;
	if(m_pLoadWaveFile == NULL)
		return -1;
	return m_pLoadWaveFile(sFilePath, sError, hWaveShape);
}

/****************************************************
Date:2012/3/26  Author:LYH
函数名:   DrawShape	
返回值:   int	
功能概要: 绘制图形
参数: cairo_t * cr	绘制环境
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
int WaveShapeEngine::xjws_DrawShape( cairo_surface_t * pSur, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return -1;
	if(m_pDrawShape == NULL)
		return -1;
	return m_pDrawShape(pSur, hWaveShape);
}

/****************************************************
Date:2012/3/27  Author:LYH
函数名:   CloseHandle	
返回值:   void	
功能概要: 关闭指定文档对象
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_CloseWaveHandle( XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pCloseWaveHandle == NULL)
		return;
	m_pCloseWaveHandle(hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetCanvasRange	
返回值:   void	
功能概要: 设置画布大小
参数: int nStartX	画布起始位置横坐标
参数: int nStartY	画布起始位置纵坐标
参数: int nWidth	画布宽度，等于或小于0时此值不起作用
参数: int nHeight	画布高度，等于或小于0时此值不起作用
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetCanvasRange( int nStartX, int nStartY, int nWidth, int nHeight, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetCanvasRange == NULL)
		return;
	m_pSetCanvasRange(nStartX, nStartY, nWidth, nHeight, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   GetFullSize	
返回值:   void	
功能概要: 取得要完全显示图形所需的大小
参数: int & nWidth	宽度
参数: int & nHeight	高度
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_GetFullSize( int& nWidth, int& nHeight, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pGetFullSize == NULL)
		return;
	m_pGetFullSize(nWidth, nHeight, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetShapeDirection	
返回值:   void	
功能概要: 设置图形方向
参数: int nDire	图形方向.0为横向，1为纵向.设置为其他值时将按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetShapeDirection( int nDire, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetShapeDirection == NULL)
		return;
	m_pSetShapeDirection(nDire, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetIsChlUseColor	
返回值:   void	
功能概要: 通道是否按相位分颜色显示
参数: int nUseCol	通道是否分颜色显示。0-不分颜色，此时只有一个背景色(默认为黑色)和一个画笔色(默认为白色)；1-分颜色，此时背景默认为黑色,画笔色默认为黑色.设置为其他值时将按1处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetIsChlUseColor( int nUseCol, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetIsChlUseColor == NULL)
		return;
	m_pSetIsChlUseColor(nUseCol, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetCanvasBkColor	
返回值:   void	
功能概要: 设置背景色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetCanvasBkColor( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetCanvasBkColor == NULL)
		return;
	m_pSetCanvasBkColor(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetCanvasStrockColor	
返回值:   void	
功能概要: 设置画笔色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetCanvasStrockColor( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetCanvasStrockColor == NULL)
		return;
	m_pSetCanvasStrockColor(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetColorPhaseA	
返回值:   void	
功能概要: 设置A相通道的画笔色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetColorPhaseA( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetColorPhaseA == NULL)
		return;
	m_pSetColorPhaseA(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetColorPhaseB	
返回值:   void	
功能概要: 设置B相通道的画笔色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetColorPhaseB( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetColorPhaseB == NULL)
		return;
	m_pSetColorPhaseB(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetColorPhaseC	
返回值:   void	
功能概要: 设置C相通道的画笔色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetColorPhaseC( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetColorPhaseC == NULL)
		return;
	m_pSetColorPhaseC(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetColorPhaseC	
返回值:   void	
功能概要: 设置C相通道的画笔色
参数: int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nG	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: int nB	RGB颜色中的R值，范围：0-255，超出范围的值按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetColorPhaseN( int nR, int nG, int nB, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetColorPhaseN == NULL)
		return;
	m_pSetColorPhaseN(nR, nG, nB, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetFontInfo	
返回值:   void	
功能概要: 设置字体信息
参数: char * sFontFamily	字体名
参数: int nFontSize	字体大小
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetFontInfo( char* sFontFamily, int nFontSize, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetFontInfo == NULL)
		return;
	m_pSetFontInfo(sFontFamily, nFontSize, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetTimeRate	
返回值:   void	
功能概要: 设置时间比例，表示每毫秒要占用多少像素
参数: double dTimeRate	时间比例，小于0.1时按0.1处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetTimeRate( double dTimeRate, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetTimeRate == NULL)
		return;
	m_pSetTimeRate(dTimeRate, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetDataType	
返回值:   void	
功能概要: 设置取值方式
参数: int nDataType	指定取值类型.0为二次值，1为一次值.值非法时按0处理
参数: int nMaxDataType	指定是否取公共最大值. 0表示取公共最大值,1表示各通道自己的最大值.值非法时按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetDataType( int nDataType, int nMaxDataType, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetDataType == NULL)
		return;
	m_pSetDataType(nDataType, nMaxDataType, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetDIStyle	
返回值:   void	
功能概要: 设置开关量通道显示风格
参数: int nDIStyle	开关量图形绘制风格。0-矩形，1-连线.值非法时按0处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetDIStyle( int nDIStyle, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetDIStyle == NULL)
		return;
	m_pSetDIStyle(nDIStyle, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetChlHeight	
返回值:   void	
功能概要: 设置通道图形高度，纵向显示时做宽度使用
参数: int nAIHeight	模拟量通道图形高度，值小于等于0时按30处理
参数: int nDIHeight	开关量通道图形高度，值小于等于0时按20处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetChlHeight( int nAIHeight, int nDIHeight, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetChlHeight == NULL)
		return;
	m_pSetChlHeight(nAIHeight, nDIHeight, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetRangeBySample	
返回值:   void	
功能概要: 使用采样点设置图形绘制范围，范围0到(总采样点数-1)
参数: int nStartSample	起始绘制的采样点.超出范围按0处理
参数: int nEndSample	结束绘制的采样点.超出范围按(总采样点数-1)处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetRangeBySample( int nStartSample, int nEndSample, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetRangeBySample == NULL)
		return;
	m_pSetRangeBySample(nStartSample, nEndSample, hWaveShape);
}

/****************************************************
Date:2012/5/16  Author:LYH
函数名:   SetRangeByTime	
返回值:   void	
功能概要: 使用时间设置图形绘制范围，此时间为相对故障触发时间的毫秒值时间
参数: int nStartTime	起始绘制的时间.超出范围按0处理
参数: int nEndTime	结束绘制的时间.超出范围按最大时间处理
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_SetRangeByTime( int nStartTime, int nEndTime, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetRangeByTime == NULL)
		return;
	m_pSetRangeByTime(nStartTime, nEndTime, hWaveShape);
}

/****************************************************
Date:2012/5/31  Author:LYH
函数名:   xjws_ExportToPDF	
返回值:   int	成功返回0，失败返回小于0
功能概要: 导出图形到PDF文件
参数: char * filepath	文件全路径
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
int WaveShapeEngine::xjws_ExportToPDF( char* filepath, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return -9;
	if(m_pExportToPDF == NULL)
		return -9;
	return m_pExportToPDF(filepath, hWaveShape);
}

/****************************************************
Date:2012/6/5  Author:LYH
函数名:   xjws_PrintShape	
返回值:   int	
功能概要: 
参数: XJHANDLE hWaveShape	
*****************************************************/
int WaveShapeEngine::xjws_PrintShape( XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return -9;
	if(m_pPrintShape == NULL)
		return -9;
	return m_pPrintShape(hWaveShape);
}

/****************************************************
Date:2012/9/17  Author:LYH
函数名:   xjws_WaveStartPosDrawing	
返回值:   void	
功能概要: 波形起始绘制位置
参数: int & nX	起始位置X坐标
参数: int & nY	起始位置Y坐标
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
void WaveShapeEngine::xjws_WaveStartPosDrawing( int& nX, int& nY, XJHANDLE hWaveShape )
{
	nX = 0;
	nY = 0;
	if(m_hModule == NULL)
		return;
	if(m_pWaveStartPosDrawing == NULL)
		return;
	m_pWaveStartPosDrawing(nX, nY, hWaveShape);
}

/****************************************************
Date:2012/9/17  Author:LYH
函数名:   xjws_GetTimeRate	
返回值:   double	时间比例，表示每毫秒要占用多少像素
功能概要: 取得时间比例
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
double WaveShapeEngine::xjws_GetTimeRate( XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTimeRate == NULL)
		return 0;
	return m_pGetTimeRate(hWaveShape);
}

/****************************************************
Date:2012/9/17  Author:LYH
函数名:   xjws_GetShapeDirection	
返回值:   int	图形方向.0为横向，1为纵向
功能概要: 取得图形方向
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
int WaveShapeEngine::xjws_GetShapeDirection( XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetShapeDirection == NULL)
		return 0;
	return m_pGetShapeDirection(hWaveShape);
}

/****************************************************
Date:2012/9/20  Author:LYH
函数名:   xjws_GetTriggerTime	
返回值:   int	故障触发时刻相对于采样开始时刻的相对毫秒时间
功能概要: 取得故障触发时刻相对于采样开始时刻的相对毫秒时间
参数: XJHANDLE hWaveShape	指定文档对象
*****************************************************/
int WaveShapeEngine::xjws_GetTriggerTime( XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTriggerTime == NULL)
		return 0;
	return m_pGetTriggerTime(hWaveShape);
}

/****************************************************
Date:2012/11/25  Author:LYH
函数名:   xjws_SetNameStyle	
返回值:   void	
功能概要: 
参数: int nNameStyle	
参数: XJHANDLE hWaveShape	
*****************************************************/
void WaveShapeEngine::xjws_SetNameStyle( int nNameStyle, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetNameStyle == NULL)
		return;
	m_pSetNameStyle(nNameStyle, hWaveShape);
}

