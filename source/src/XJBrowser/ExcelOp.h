#ifndef _EXCELOP_H
#define _EXCELOP_H
#include "StdAfx.h"
#include "excel.h"
#include "DeviceObj.h"
#include "FaultEvent.h"
#include "FaultReport.h"
#include "XJBrowser.h"
#include "GlobalFunc.h"


const int CELL1_LENGTH = 3;
const int CELL2_LENGTH = 4;
const int CELL3_LENGTH = 4;
const int CELL4_LENGTH = 6;
const int CELL_ALL_LENGTH = CELL1_LENGTH + CELL2_LENGTH + CELL3_LENGTH + CELL4_LENGTH;

const int TITLE_FONT_SIZE = 25;
const int BODY_FONT_SIZE = 13;

struct _Font  
{
	CString strValue;     //��ʾֵ
	bool    bBold;        //�Ƿ����
	int     size;         //�����С
	bool    bCenter;
	_Font(CString str)
	{
		strValue = str;
		bBold = false;
		size = 12;
		bCenter = true;
	}
};

typedef struct _Font _Font;

typedef struct  
{
	int showrows;    //��ʾ��Ԫ�߱���������
	int col;         //��ǰ���еڼ���
	int cells;       //�߱�������Ԫ��
    _Font* font;
	bool  bBorder;    //�Ƿ���ʾ�߿�
	int  cellType;
}_Cell;

struct _Level
{
	int rows;   //������(����ʾ�о߱�������)
	int cols;   //������
	CPtrList cellList; //��ʾ��
	_Level(int x, int y):rows(x), cols(y){}
	_Level(){}
};


typedef struct _Level _Level;

_Cell* AddCellToList(_Level* level, 
					 CString strValue,
					 bool    bBold,        
                     int     size,
					 bool    bCenter,
					 int showrows,    //��ʾ��Ԫ�߱���������
					 int col,         //��ǰ���еڼ���
					 int cells,       //�߱�������Ԫ��
					 bool  bBorder = 1,    //�Ƿ���ʾ�߿�
					 int  cellType = 0,
					 int  listPos = 0);
void AddTitle();
void AddBasicInfo(CFaultReport* pReport);
int AddFaultProfile(_Level* level, CSecObj* pSecObj, CFaultReport* pReport);
void GetTempSignArray(CPtrArray& listTempSign, CFaultReport* pReport, CSecObj* pSecObj);
void GetTempPtOscArray(CPtrArray& listTempSign, CFaultReport* pReport, CSecObj* pSecObj);
CString CalReletiveTime(CFaultEvent* pEventFirst, CFaultEvent* pEvent);
int AddFaultAction(_Level* level, CSecObj* pSecObj, CFaultReport* pReport);
int AddFaultOSC(_Level* level, CSecObj* pSecObj, CFaultReport* pReport);
void AddProtectInfo(CSecObj* pSecObj, CFaultReport* pReport);
void AddProtectsInfo(CFaultReport* pReport);
int AddOSC(_Level* level, CPtrArray& listWROSC);
int AddPriDeviceOSC(_Level* level, CPtrArray& listWROSC);
int AddStationOSC(_Level* level, CPtrArray& listWROSC);
void AddOSCFile(CFaultReport* pReport);
void ParserReport(CFaultReport* pReport);
void MergeCells(int startRow, int startCol, _Cell* cell, _Worksheet& oSheet, COleVariant& vOpt);
void CalNowPos(int& Row, int& col, _Level* level, _Cell* cell, _Cell* lastCell);
void CalNowPosV2(int& Row, int& col, _Level* level, _Cell* cell, _Cell* lastCell);
void ParserCell(_Worksheet& oSheet, _Level* level, int& iCurrentRows, COleVariant& vOpt);
void ParserLevel(_Worksheet& oSheet, COleVariant& vOpt);
void ExportByExcel(CString strPath);
void ClearCellList(CPtrList* cellList);
void ClearLevelList();
void ExportWaveFile(CString strDir, CFaultReport* pReport);
void CopyProWaveFile(CString strDir, CFaultReport* pReport);
void CopyOSCWaveFile(CString strDir, CFaultReport* pReport);
#endif