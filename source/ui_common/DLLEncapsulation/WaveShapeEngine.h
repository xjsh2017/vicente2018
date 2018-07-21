#if !defined(WAVESHAPEENGINE_H_)
#define WAVESHAPEENGINE_H_

 #include "../../common/define.h"
 #include "../../common_open_source/cairo_include/cairo.h"
 #include "../../common/xjlib.h"


#ifdef OS_WINDOWS
#define LIBNAME_WAVESHAPE "XJWaveShapeWIN.dll"
#else 
#define LIBNAME_WAVESHAPE "libXJWaveShape.so"
#endif

//定义名以WaveShape的缩写ws开头
typedef int (*ws_LoadWaveFile)(char* sFilePath, char* sError, XJHANDLE& hWaveShape);
typedef int (*ws_DrawShape)(cairo_surface_t * pSur, XJHANDLE hWaveShape);
typedef void (*ws_CloseWaveHandle)(XJHANDLE hWaveShape);
typedef void (*ws_SetCanvasRange)(int nStartX, int nStartY, int nWidth, int nHeight, XJHANDLE hWaveShape);
typedef void (*ws_GetFullSize)(int& nWidth, int& nHeight, XJHANDLE hWaveShape);
typedef void (*ws_SetShapeDirection)(int nDire, XJHANDLE hWaveShape);
typedef void (*ws_SetIsChlUseColor)(int nUseCol, XJHANDLE hWaveShape);
typedef void (*ws_SetCanvasBkColor)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetCanvasStrockColor)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetColorPhaseA)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetColorPhaseB)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetColorPhaseC)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetColorPhaseN)(int nR, int nG, int nB, XJHANDLE hWaveShape);
typedef void (*ws_SetFontInfo)(char* sFontFamily, int nFontSize, XJHANDLE hWaveShape);
typedef void (*ws_SetTimeRate)(double dTimeRate, XJHANDLE hWaveShape);
typedef void (*ws_SetDataType)(int nDataType, int nMaxDataType, XJHANDLE hWaveShape);
typedef void (*ws_SetDIStyle)(int nDIStyle, XJHANDLE hWaveShape);
typedef void (*ws_SetChlHeight)(int nAIHeight, int nDIHeight, XJHANDLE hWaveShape);
typedef void (*ws_SetRangeBySample)(int nStartSample, int nEndSample, XJHANDLE hWaveShape);
typedef void (*ws_SetRangeByTime)(int nStartTime, int nEndTime, XJHANDLE hWaveShape);
typedef int (*ws_ExportToPDF)(char* filepath, XJHANDLE hWaveShape);
typedef int (*ws_PrintShape)(XJHANDLE hWaveShape);
typedef void (*ws_WaveStartPosDrawing)(int& nX, int& nY, XJHANDLE hWaveShape);
typedef double (*ws_GetTimeRate)(XJHANDLE hWaveShape);
typedef int (*ws_GetShapeDirection)(XJHANDLE hWaveShape);
typedef int (*ws_GetTriggerTime)(XJHANDLE hWaveShape);
typedef void (*ws_SetNameStyle)(int, XJHANDLE);

class WaveShapeEngine
{
public:
	WaveShapeEngine();
	virtual ~WaveShapeEngine();
//Attribute
public:
	/** @brief           动态库句柄*/
	XJHANDLE	m_hModule;
private:
	ws_LoadWaveFile m_pLoadWaveFile;
	ws_DrawShape m_pDrawShape;
	ws_CloseWaveHandle m_pCloseWaveHandle;
	ws_SetCanvasRange m_pSetCanvasRange;
	ws_GetFullSize m_pGetFullSize;
	ws_SetShapeDirection m_pSetShapeDirection;
	ws_SetIsChlUseColor m_pSetIsChlUseColor;
	ws_SetCanvasBkColor m_pSetCanvasBkColor;
	ws_SetCanvasStrockColor m_pSetCanvasStrockColor;
	ws_SetColorPhaseA m_pSetColorPhaseA;
	ws_SetColorPhaseB m_pSetColorPhaseB;
	ws_SetColorPhaseC m_pSetColorPhaseC;
	ws_SetColorPhaseN m_pSetColorPhaseN;
	ws_SetFontInfo m_pSetFontInfo;
	ws_SetTimeRate m_pSetTimeRate;
	ws_SetDataType m_pSetDataType;
	ws_SetDIStyle m_pSetDIStyle;
	ws_SetChlHeight m_pSetChlHeight;
	ws_SetRangeBySample m_pSetRangeBySample;
	ws_SetRangeByTime m_pSetRangeByTime;
	ws_ExportToPDF m_pExportToPDF;
	ws_PrintShape m_pPrintShape;
	ws_WaveStartPosDrawing m_pWaveStartPosDrawing;
	ws_GetTimeRate m_pGetTimeRate;
	ws_GetShapeDirection m_pGetShapeDirection;
	ws_GetTriggerTime m_pGetTriggerTime;
	ws_SetNameStyle m_pSetNameStyle;

//Operations
public:
/*
 *  @brief   	LoadWSLib	 装载XJWaveShape.dll 
 *  @return 	bool 装载成功返回TRUE,失败返回FALSE
 */
	bool xjws_LoadWSLib();
public:
	/*
 *  @brief   	LoadFile	 载入波形文件 
 *  @param 		[In]a param of Type  char*  波形文件路径
 *  @param 		[Out]a param of Type  char*  错误信息
 *  @param 		[Out]a param of Type  int&  生成的波形文件数据对象指针
 *  @return 	int 载入成功返回>=0, 失败返回<0
 */
	int xjws_LoadWaveFile(char* sFilePath, char* sError, XJHANDLE& hWaveShape);

/*
	* @brief	DrawShape	绘制图形
	* @param	[In]a param of Type cairo_t * cr	绘制环境	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   int	
*/	
	int xjws_DrawShape(cairo_surface_t * pSur, XJHANDLE hWaveShape);

	/*
	* @brief	CloseWaveHandle	关闭指定文档对象
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_CloseWaveHandle(XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasRange	设置画布大小
	* @param	[In]a param of Type int nStartX	画布起始位置横坐标	
	* @param	[In]a param of Type int nStartY	画布起始位置纵坐标	
	* @param	[In]a param of Type int nWidth	画布宽度，等于或小于0时此值不起作用,图形将以实际宽度来绘制
	* @param	[In]a param of Type int nHeight	画布高度，等于或小于0时此值不起作用，图形将以已经高度来绘制
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetCanvasRange(int nStartX, int nStartY, int nWidth, int nHeight, XJHANDLE hWaveShape);

	/*
	* @brief	GetFullSize	取得要完全显示图形所需的大小
	* @param	[In]a param of Type int & nWidth	宽度	
	* @param	[In]a param of Type int & nHeight	高度	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_GetFullSize(int& nWidth, int& nHeight, XJHANDLE hWaveShape);

	/*
	* @brief	SetShapeDirection	设置图形方向
	* @param	[In]a param of Type int nDire	图形方向.0为横向，1为纵向.设置为其他值时将按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetShapeDirection(int nDire, XJHANDLE hWaveShape);

	/*
	* @brief	SetIsChlUseColor	通道是否按相位分颜色显示
	* @param	[In]a param of Type int nUseCol	通道是否分颜色显示。0-不分颜色，此时只有一个背景色(默认为黑色)和一个画笔色(默认为白色)；1-分颜色，此时背景默认为黑色,画笔色默认为黑色。设置为其他值时将按1处理
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetIsChlUseColor(int nUseCol, XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasBkColor	设置背景色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetCanvasBkColor(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasStrockColor	设置画笔色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象		
	* @return   void	
	*/
	void xjws_SetCanvasStrockColor(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseA	设置A相通道的画笔色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象		
	* @return   void	
	*/
	void xjws_SetColorPhaseA(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseB	设置B相通道的画笔色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象		
	* @return   void	
	*/
	void xjws_SetColorPhaseB(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseC	设置C相通道的画笔色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象		
	* @return   void	
	*/
	void xjws_SetColorPhaseC(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseN	设置N相通道的画笔色
	* @param	[In]a param of Type int nR	RGB颜色中的R值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nG	RGB颜色中的G值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type int nB	RGB颜色中的B值，范围：0-255，超出范围的值按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象		
	* @return   void	
	*/
	void xjws_SetColorPhaseN(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetFontInfo	设置字体信息
	* @param	[In]a param of Type char * sFontFamily	字体名	
	* @param	[In]a param of Type int nFontSize	字体大小
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象
	* @return   void	
	*/
	void xjws_SetFontInfo(char* sFontFamily, int nFontSize, XJHANDLE hWaveShape);

	/*
	* @brief	SetTimeRate	设置时间比例，表示每毫秒要占用多少像素
	* @param	[In]a param of Type double dTimeRate	时间比例，小于0.1时按0.1处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetTimeRate(double dTimeRate, XJHANDLE hWaveShape);

	/*
	* @brief	SetDataType	设置取值方式
	* @param	[In]a param of Type int nDataType	指定取值类型.0为二次值，1为一次值.值非法时按0处理	
	* @param	[In]a param of Type int nMaxDataType	指定是否取公共最大值. 0表示取公共最大值,1表示各通道自己的最大值.值非法时按0处理
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetDataType(int nDataType, int nMaxDataType, XJHANDLE hWaveShape);

	/*
	* @brief	SetDIStyle	设置开关量通道显示风格
	* @param	[In]a param of Type int nDIStyle	开关量图形绘制风格。0-矩形，1-连线.值非法时按0处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetDIStyle(int nDIStyle, XJHANDLE hWaveShape);

	/*
	* @brief	SetChlHeight	设置通道图形高度，纵向显示时做宽度使用
	* @param	[In]a param of Type int nAIHeight	模拟量通道图形高度，值小于等于0时按30处理	
	* @param	[In]a param of Type int nDIHeight	开关量通道图形高度，值小于等于0时按20处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetChlHeight(int nAIHeight, int nDIHeight, XJHANDLE hWaveShape);

	/*
	* @brief	SetRangeBySample	使用采样点设置图形绘制范围，范围0到(总采样点数-1)
	* @param	[In]a param of Type int nStartSample	起始绘制的采样点.超出范围按0处理	
	* @param	[In]a param of Type int nEndSample		结束绘制的采样点.超出范围按(总采样点数-1)处理
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetRangeBySample(int nStartSample, int nEndSample, XJHANDLE hWaveShape);

	/*
	* @brief	SetRangeByTime	使用时间设置图形绘制范围，此时间为相对故障触发时间的毫秒值时间
	* @param	[In]a param of Type int nStartTime	起始绘制的时间.超出范围按开始采样时间处理	
	* @param	[In]a param of Type int nEndTime	结束绘制的时间.超出范围按最大时间处理	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_SetRangeByTime(int nStartTime, int nEndTime, XJHANDLE hWaveShape);

	/*
	* @brief	xjws_ExportToPDF	导出图形到PDF文件
	* @param	[In]a param of Type char * filepath	文件全路径	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   int	成功返回0，失败返回小于0
	*/
	int xjws_ExportToPDF(char* filepath, XJHANDLE hWaveShape);

	/*
	* @brief	xjws_PrintShape	打印图形
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   int	成功返回0，失败返回小于0
	*/
	int xjws_PrintShape(XJHANDLE hWaveShape);

	/*
	* @brief	WaveStartPosDrawing	波形起始绘制位置
	* @param	[In]a param of Type int & nX 起始位置X坐标		
	* @param	[In]a param of Type int & nY 起始位置Y坐标		
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   void	
	*/
	void xjws_WaveStartPosDrawing(int& nX, int& nY, XJHANDLE hWaveShape);

	/*
	* @brief	GetTimeRate	取得时间比例
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   double	时间比例，表示每毫秒要占用多少像素
	*/
	double xjws_GetTimeRate(XJHANDLE hWaveShape);

	/*
	* @brief	SetShapeDirection	取得图形方向	
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   int	图形方向.0为横向，1为纵向
	*/
	int xjws_GetShapeDirection(XJHANDLE hWaveShape);

	/*
	* @brief	xjws_GetTriggerTime	取得故障触发时刻相对于采样开始时刻的相对毫秒时间
	* @param	[In]a param of Type XJHANDLE hWaveShape	指定文档对象	
	* @return   int	故障触发时刻相对于采样开始时刻的相对毫秒时间
	*/
	int xjws_GetTriggerTime(XJHANDLE hWaveShape);

	void xjws_SetNameStyle(int nNameStyle, XJHANDLE hWaveShape);
};
#endif
