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
�� �� ��: xjws_LoadWSLib
���ܸ�Ҫ: װ��XJComtrade.dll 
�� �� ֵ: װ�سɹ�����TRUE,ʧ�ܷ���FALSE
��    ��: param1 
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
		printf("����WaveShape��̬��ʧ��!");
		return false;
	}
	return true;
}

/****************************************************
Date:2012/3/26  Author:LYH
������:   LoadWaveFile	
����ֵ:   int	����ɹ�����>=0, ʧ�ܷ���<0
���ܸ�Ҫ: ���벨���ļ�
����: char * sFilePath	�����ļ�·��
����: char * sError	������Ϣ
����: XJHANDLE & hWaveShape	���ɵĲ����ļ����ݶ���ָ��
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
������:   DrawShape	
����ֵ:   int	
���ܸ�Ҫ: ����ͼ��
����: cairo_t * cr	���ƻ���
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   CloseHandle	
����ֵ:   void	
���ܸ�Ҫ: �ر�ָ���ĵ�����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetCanvasRange	
����ֵ:   void	
���ܸ�Ҫ: ���û�����С
����: int nStartX	������ʼλ�ú�����
����: int nStartY	������ʼλ��������
����: int nWidth	������ȣ����ڻ�С��0ʱ��ֵ��������
����: int nHeight	�����߶ȣ����ڻ�С��0ʱ��ֵ��������
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   GetFullSize	
����ֵ:   void	
���ܸ�Ҫ: ȡ��Ҫ��ȫ��ʾͼ������Ĵ�С
����: int & nWidth	���
����: int & nHeight	�߶�
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetShapeDirection	
����ֵ:   void	
���ܸ�Ҫ: ����ͼ�η���
����: int nDire	ͼ�η���.0Ϊ����1Ϊ����.����Ϊ����ֵʱ����0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetIsChlUseColor	
����ֵ:   void	
���ܸ�Ҫ: ͨ���Ƿ���λ����ɫ��ʾ
����: int nUseCol	ͨ���Ƿ����ɫ��ʾ��0-������ɫ����ʱֻ��һ������ɫ(Ĭ��Ϊ��ɫ)��һ������ɫ(Ĭ��Ϊ��ɫ)��1-����ɫ����ʱ����Ĭ��Ϊ��ɫ,����ɫĬ��Ϊ��ɫ.����Ϊ����ֵʱ����1����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetCanvasBkColor	
����ֵ:   void	
���ܸ�Ҫ: ���ñ���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetCanvasStrockColor	
����ֵ:   void	
���ܸ�Ҫ: ���û���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Gֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Bֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetColorPhaseA	
����ֵ:   void	
���ܸ�Ҫ: ����A��ͨ���Ļ���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetColorPhaseB	
����ֵ:   void	
���ܸ�Ҫ: ����B��ͨ���Ļ���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetColorPhaseC	
����ֵ:   void	
���ܸ�Ҫ: ����C��ͨ���Ļ���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetColorPhaseC	
����ֵ:   void	
���ܸ�Ҫ: ����C��ͨ���Ļ���ɫ
����: int nR	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nG	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: int nB	RGB��ɫ�е�Rֵ����Χ��0-255��������Χ��ֵ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetFontInfo	
����ֵ:   void	
���ܸ�Ҫ: ����������Ϣ
����: char * sFontFamily	������
����: int nFontSize	�����С
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetTimeRate	
����ֵ:   void	
���ܸ�Ҫ: ����ʱ���������ʾÿ����Ҫռ�ö�������
����: double dTimeRate	ʱ�������С��0.1ʱ��0.1����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetDataType	
����ֵ:   void	
���ܸ�Ҫ: ����ȡֵ��ʽ
����: int nDataType	ָ��ȡֵ����.0Ϊ����ֵ��1Ϊһ��ֵ.ֵ�Ƿ�ʱ��0����
����: int nMaxDataType	ָ���Ƿ�ȡ�������ֵ. 0��ʾȡ�������ֵ,1��ʾ��ͨ���Լ������ֵ.ֵ�Ƿ�ʱ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetDIStyle	
����ֵ:   void	
���ܸ�Ҫ: ���ÿ�����ͨ����ʾ���
����: int nDIStyle	������ͼ�λ��Ʒ��0-���Σ�1-����.ֵ�Ƿ�ʱ��0����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetChlHeight	
����ֵ:   void	
���ܸ�Ҫ: ����ͨ��ͼ�θ߶ȣ�������ʾʱ�����ʹ��
����: int nAIHeight	ģ����ͨ��ͼ�θ߶ȣ�ֵС�ڵ���0ʱ��30����
����: int nDIHeight	������ͨ��ͼ�θ߶ȣ�ֵС�ڵ���0ʱ��20����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetRangeBySample	
����ֵ:   void	
���ܸ�Ҫ: ʹ�ò���������ͼ�λ��Ʒ�Χ����Χ0��(�ܲ�������-1)
����: int nStartSample	��ʼ���ƵĲ�����.������Χ��0����
����: int nEndSample	�������ƵĲ�����.������Χ��(�ܲ�������-1)����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   SetRangeByTime	
����ֵ:   void	
���ܸ�Ҫ: ʹ��ʱ������ͼ�λ��Ʒ�Χ����ʱ��Ϊ��Թ��ϴ���ʱ��ĺ���ֵʱ��
����: int nStartTime	��ʼ���Ƶ�ʱ��.������Χ��0����
����: int nEndTime	�������Ƶ�ʱ��.������Χ�����ʱ�䴦��
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_ExportToPDF	
����ֵ:   int	�ɹ�����0��ʧ�ܷ���С��0
���ܸ�Ҫ: ����ͼ�ε�PDF�ļ�
����: char * filepath	�ļ�ȫ·��
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_PrintShape	
����ֵ:   int	
���ܸ�Ҫ: 
����: XJHANDLE hWaveShape	
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
������:   xjws_WaveStartPosDrawing	
����ֵ:   void	
���ܸ�Ҫ: ������ʼ����λ��
����: int & nX	��ʼλ��X����
����: int & nY	��ʼλ��Y����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_GetTimeRate	
����ֵ:   double	ʱ���������ʾÿ����Ҫռ�ö�������
���ܸ�Ҫ: ȡ��ʱ�����
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_GetShapeDirection	
����ֵ:   int	ͼ�η���.0Ϊ����1Ϊ����
���ܸ�Ҫ: ȡ��ͼ�η���
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_GetTriggerTime	
����ֵ:   int	���ϴ���ʱ������ڲ�����ʼʱ�̵���Ժ���ʱ��
���ܸ�Ҫ: ȡ�ù��ϴ���ʱ������ڲ�����ʼʱ�̵���Ժ���ʱ��
����: XJHANDLE hWaveShape	ָ���ĵ�����
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
������:   xjws_SetNameStyle	
����ֵ:   void	
���ܸ�Ҫ: 
����: int nNameStyle	
����: XJHANDLE hWaveShape	
*****************************************************/
void WaveShapeEngine::xjws_SetNameStyle( int nNameStyle, XJHANDLE hWaveShape )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetNameStyle == NULL)
		return;
	m_pSetNameStyle(nNameStyle, hWaveShape);
}

