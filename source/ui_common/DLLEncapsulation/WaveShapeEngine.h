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

//��������WaveShape����дws��ͷ
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
	/** @brief           ��̬����*/
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
 *  @brief   	LoadWSLib	 װ��XJWaveShape.dll 
 *  @return 	bool װ�سɹ�����TRUE,ʧ�ܷ���FALSE
 */
	bool xjws_LoadWSLib();
public:
	/*
 *  @brief   	LoadFile	 ���벨���ļ� 
 *  @param 		[In]a param of Type  char*  �����ļ�·��
 *  @param 		[Out]a param of Type  char*  ������Ϣ
 *  @param 		[Out]a param of Type  int&  ���ɵĲ����ļ����ݶ���ָ��
 *  @return 	int ����ɹ�����>=0, ʧ�ܷ���<0
 */
	int xjws_LoadWaveFile(char* sFilePath, char* sError, XJHANDLE& hWaveShape);

/*
	* @brief	DrawShape	����ͼ��
	* @param	[In]a param of Type cairo_t * cr	���ƻ���	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   int	
*/	
	int xjws_DrawShape(cairo_surface_t * pSur, XJHANDLE hWaveShape);

	/*
	* @brief	CloseWaveHandle	�ر�ָ���ĵ�����
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_CloseWaveHandle(XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasRange	���û�����С
	* @param	[In]a param of Type int nStartX	������ʼλ�ú�����	
	* @param	[In]a param of Type int nStartY	������ʼλ��������	
	* @param	[In]a param of Type int nWidth	������ȣ����ڻ�С��0ʱ��ֵ��������,ͼ�ν���ʵ�ʿ��������
	* @param	[In]a param of Type int nHeight	�����߶ȣ����ڻ�С��0ʱ��ֵ�������ã�ͼ�ν����Ѿ��߶�������
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetCanvasRange(int nStartX, int nStartY, int nWidth, int nHeight, XJHANDLE hWaveShape);

	/*
	* @brief	GetFullSize	ȡ��Ҫ��ȫ��ʾͼ������Ĵ�С
	* @param	[In]a param of Type int & nWidth	���	
	* @param	[In]a param of Type int & nHeight	�߶�	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_GetFullSize(int& nWidth, int& nHeight, XJHANDLE hWaveShape);

	/*
	* @brief	SetShapeDirection	����ͼ�η���
	* @param	[In]a param of Type int nDire	ͼ�η���.0Ϊ����1Ϊ����.����Ϊ����ֵʱ����0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetShapeDirection(int nDire, XJHANDLE hWaveShape);

	/*
	* @brief	SetIsChlUseColor	ͨ���Ƿ���λ����ɫ��ʾ
	* @param	[In]a param of Type int nUseCol	ͨ���Ƿ����ɫ��ʾ��0-������ɫ����ʱֻ��һ������ɫ(Ĭ��Ϊ��ɫ)��һ������ɫ(Ĭ��Ϊ��ɫ)��1-����ɫ����ʱ����Ĭ��Ϊ��ɫ,����ɫĬ��Ϊ��ɫ������Ϊ����ֵʱ����1����
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetIsChlUseColor(int nUseCol, XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasBkColor	���ñ���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetCanvasBkColor(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetCanvasStrockColor	���û���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����		
	* @return   void	
	*/
	void xjws_SetCanvasStrockColor(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseA	����A��ͨ���Ļ���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����		
	* @return   void	
	*/
	void xjws_SetColorPhaseA(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseB	����B��ͨ���Ļ���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����		
	* @return   void	
	*/
	void xjws_SetColorPhaseB(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseC	����C��ͨ���Ļ���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����		
	* @return   void	
	*/
	void xjws_SetColorPhaseC(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetColorPhaseN	����N��ͨ���Ļ���ɫ
	* @param	[In]a param of Type int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����		
	* @return   void	
	*/
	void xjws_SetColorPhaseN(int nR, int nG, int nB, XJHANDLE hWaveShape);

	/*
	* @brief	SetFontInfo	����������Ϣ
	* @param	[In]a param of Type char * sFontFamily	������	
	* @param	[In]a param of Type int nFontSize	�����С
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����
	* @return   void	
	*/
	void xjws_SetFontInfo(char* sFontFamily, int nFontSize, XJHANDLE hWaveShape);

	/*
	* @brief	SetTimeRate	����ʱ���������ʾÿ����Ҫռ�ö�������
	* @param	[In]a param of Type double dTimeRate	ʱ�������С��0.1ʱ��0.1����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetTimeRate(double dTimeRate, XJHANDLE hWaveShape);

	/*
	* @brief	SetDataType	����ȡֵ��ʽ
	* @param	[In]a param of Type int nDataType	ָ��ȡֵ����.0Ϊ����ֵ��1Ϊһ��ֵ.ֵ�Ƿ�ʱ��0����	
	* @param	[In]a param of Type int nMaxDataType	ָ���Ƿ�ȡ�������ֵ. 0��ʾȡ�������ֵ,1��ʾ��ͨ���Լ������ֵ.ֵ�Ƿ�ʱ��0����
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetDataType(int nDataType, int nMaxDataType, XJHANDLE hWaveShape);

	/*
	* @brief	SetDIStyle	���ÿ�����ͨ����ʾ���
	* @param	[In]a param of Type int nDIStyle	������ͼ�λ��Ʒ��0-���Σ�1-����.ֵ�Ƿ�ʱ��0����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetDIStyle(int nDIStyle, XJHANDLE hWaveShape);

	/*
	* @brief	SetChlHeight	����ͨ��ͼ�θ߶ȣ�������ʾʱ�����ʹ��
	* @param	[In]a param of Type int nAIHeight	ģ����ͨ��ͼ�θ߶ȣ�ֵС�ڵ���0ʱ��30����	
	* @param	[In]a param of Type int nDIHeight	������ͨ��ͼ�θ߶ȣ�ֵС�ڵ���0ʱ��20����	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetChlHeight(int nAIHeight, int nDIHeight, XJHANDLE hWaveShape);

	/*
	* @brief	SetRangeBySample	ʹ�ò���������ͼ�λ��Ʒ�Χ����Χ0��(�ܲ�������-1)
	* @param	[In]a param of Type int nStartSample	��ʼ���ƵĲ�����.������Χ��0����	
	* @param	[In]a param of Type int nEndSample		�������ƵĲ�����.������Χ��(�ܲ�������-1)����
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetRangeBySample(int nStartSample, int nEndSample, XJHANDLE hWaveShape);

	/*
	* @brief	SetRangeByTime	ʹ��ʱ������ͼ�λ��Ʒ�Χ����ʱ��Ϊ��Թ��ϴ���ʱ��ĺ���ֵʱ��
	* @param	[In]a param of Type int nStartTime	��ʼ���Ƶ�ʱ��.������Χ����ʼ����ʱ�䴦��	
	* @param	[In]a param of Type int nEndTime	�������Ƶ�ʱ��.������Χ�����ʱ�䴦��	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_SetRangeByTime(int nStartTime, int nEndTime, XJHANDLE hWaveShape);

	/*
	* @brief	xjws_ExportToPDF	����ͼ�ε�PDF�ļ�
	* @param	[In]a param of Type char * filepath	�ļ�ȫ·��	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   int	�ɹ�����0��ʧ�ܷ���С��0
	*/
	int xjws_ExportToPDF(char* filepath, XJHANDLE hWaveShape);

	/*
	* @brief	xjws_PrintShape	��ӡͼ��
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   int	�ɹ�����0��ʧ�ܷ���С��0
	*/
	int xjws_PrintShape(XJHANDLE hWaveShape);

	/*
	* @brief	WaveStartPosDrawing	������ʼ����λ��
	* @param	[In]a param of Type int & nX ��ʼλ��X����		
	* @param	[In]a param of Type int & nY ��ʼλ��Y����		
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   void	
	*/
	void xjws_WaveStartPosDrawing(int& nX, int& nY, XJHANDLE hWaveShape);

	/*
	* @brief	GetTimeRate	ȡ��ʱ�����
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   double	ʱ���������ʾÿ����Ҫռ�ö�������
	*/
	double xjws_GetTimeRate(XJHANDLE hWaveShape);

	/*
	* @brief	SetShapeDirection	ȡ��ͼ�η���	
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   int	ͼ�η���.0Ϊ����1Ϊ����
	*/
	int xjws_GetShapeDirection(XJHANDLE hWaveShape);

	/*
	* @brief	xjws_GetTriggerTime	ȡ�ù��ϴ���ʱ������ڲ�����ʼʱ�̵���Ժ���ʱ��
	* @param	[In]a param of Type XJHANDLE hWaveShape	ָ���ĵ�����	
	* @return   int	���ϴ���ʱ������ڲ�����ʼʱ�̵���Ժ���ʱ��
	*/
	int xjws_GetTriggerTime(XJHANDLE hWaveShape);

	void xjws_SetNameStyle(int nNameStyle, XJHANDLE hWaveShape);
};
#endif
