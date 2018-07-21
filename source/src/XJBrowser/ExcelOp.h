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
	CString strValue;     //显示值
	bool    bBold;        //是否粗体
	int     size;         //字体大小
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
	int showrows;    //显示单元具备几行行数
	int col;         //当前层中第几列
	int cells;       //具备几个单元格
    _Font* font;
	bool  bBorder;    //是否显示边框
	int  cellType;
}_Cell;

struct _Level
{
	int rows;   //总行数(即显示列具备的行数)
	int cols;   //总列数
	CPtrList cellList; //显示列
	_Level(int x, int y):rows(x), cols(y){}
	_Level(){}
};


typedef struct _Level _Level;

_Cell* AddCellToList(_Level* level, 
					 CString strValue,
					 bool    bBold,        
                     int     size,
					 bool    bCenter,
					 int showrows,    //显示单元具备几行行数
					 int col,         //当前层中第几列
					 int cells,       //具备几个单元格
					 bool  bBorder = 1,    //是否显示边框
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