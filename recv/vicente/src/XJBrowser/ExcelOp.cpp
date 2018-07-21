#include "ExcelOp.h"

CPtrList level_List;

_Cell* AddCellToList(_Level* level, 
					 CString strValue,
					 bool    bBold,        
                     int     size,
					 bool    bCenter,
					 int showrows,    //显示单元具备几行行数
					 int col,         //当前层中第几列
					 int cells,       //具备几个单元格
					 bool  bBorder,    //是否显示边框
					 int  cellType,
					 int  listPos)
{
	try
	{
		_Font* font = new _Font(strValue);
		MYASSERT_NULL(font);
		font->bBold = bBold;
		font->size = size;
		font->bCenter = bCenter;
		_Cell* cell = new _Cell;
		MYASSERT_NULL(cell);
		cell->font = font;
		cell->showrows = showrows;
		cell->col = col;
		cell->cells = cells;
		cell->bBorder = bBorder;
		cell->cellType = cellType;
		if (listPos)
		{
			level->cellList.AddHead(cell);
		}
		else
		{
			level->cellList.AddTail(cell);
		}
		return cell;
	}
	catch (...)
	{
		return NULL;
	}
}

//--------------------------------------------------------------------------------------------

void AddTitle()
{
	ClearLevelList();
	_Level* level = new _Level(1, 1);
	MYASSERT(level);
	 AddCellToList(level, StringFromID(IDS_FAULT_REPORT), 1, TITLE_FONT_SIZE, 1, 1, 1, CELL_ALL_LENGTH, 0);
	level_List.AddTail(level);
}

void AddBasicInfo(CFaultReport* pReport)
{
	_Level* level = new _Level(1, 1);
	MYASSERT(level);
	AddCellToList(level, StringFromID(IDS_FAULT_STATION1)+":", 0, BODY_FONT_SIZE, 1, 1, 1, CELL1_LENGTH);

	CString str = "";
	if (pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}	
	AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 2, CELL2_LENGTH);
	AddCellToList(level, StringFromID(IDS_FAULT_STATION2)+":", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
	
	str = "";
	if (pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
	level_List.AddTail(level);

	level = new _Level(1,1);
	MYASSERT(level);
	AddCellToList(level, StringFromID(IDS_FAULT_PHASE)+":", 0, BODY_FONT_SIZE, 1, 1, 1, CELL1_LENGTH);
	AddCellToList(level, pReport->GetFaultType(), 0, BODY_FONT_SIZE, 1, 1, 2, CELL2_LENGTH);
	AddCellToList(level, StringFromID(IDS_FAULT_LOCATION)+":", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
	AddCellToList(level, pReport->m_sFaultDistance, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
	level_List.AddTail(level);

	level = new _Level(1,1);
	MYASSERT(level);
	AddCellToList(level, "", 0, BODY_FONT_SIZE, 1, 1, 1, CELL_ALL_LENGTH);
	level_List.AddTail(level);
}

int AddFaultProfile(_Level* level, CSecObj* pSecObj, CFaultReport* pReport)
{
	MYASSERT_NULL(pSecObj);
	MYASSERT_NULL(pReport);
	//统计总行数
	int allRows = 0;
	_Cell* cell = AddCellToList(level, StringFromID(IDS_FAULT_OVERVIEW), 0, BODY_FONT_SIZE, 1, allRows, 2, CELL2_LENGTH);
	MYASSERT_NULL(cell);
	//从第二，三列开始添加
	AddCellToList(level, StringFromID(IDS_MODEL_SUBSTATION)+":", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
	AddCellToList(level, pSecObj->m_pStation->m_sName, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
	++allRows;


	AddCellToList(level, StringFromID(IDS_FAULT_DEVICE)+":", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
	AddCellToList(level, pSecObj->m_pOwner->m_sName, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
	++allRows;


	AddCellToList(level, StringFromID(IDS_FAULT_TIME)+":", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);

	CPtrArray  m_listTempSign;
	CFaultEvent* pEvent = NULL;
	GetTempSignArray(m_listTempSign, pReport, pSecObj);
	if(m_listTempSign.GetSize() > 0)
	{
		pEvent = (CFaultEvent*)m_listTempSign[0];
		MYASSERT_NULL(pEvent);
	}

	CString sTime;
	if (pEvent != NULL)
	{	
		sTime.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	}
	else
	{
		sTime.Format("%s", pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S"));
	}
	AddCellToList(level, sTime, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH, 1, 1);
	++allRows;

	for (int i = 0; i < pReport->m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)pReport->m_listChr[i];
		MYASSERT_CONTINUE(pEvent);
		if (pSecObj->m_sID == pEvent->m_pSec->m_sID)
		{

			AddCellToList(level, pEvent->m_sEventDefName, 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
			CString str;
			str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
			AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
			++allRows;
		}
	}

	cell->showrows = allRows;
	return allRows;
}


void GetTempSignArray(CPtrArray& listTempSign, CFaultReport* pReport, CSecObj* pSecObj)
{
	for(int i = 0; i < pReport->m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)pReport->m_listSign.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecObj)
				listTempSign.Add(pEvent);
		}
	}
}



CString CalReletiveTime(CFaultEvent* pEventFirst, CFaultEvent* pEvent)
{
	CString sReturn = "";
	//判断是否事件
	if(pEvent->m_nType != XJ_FAULT_EVENT_CHR && pEvent->m_nType != XJ_FAULT_EVENT_SING && pEvent->m_nType != XJ_FAULT_EVENT_ALARM)
	{
		return "";
	}
	if(pEventFirst == NULL)
	{
		return "";
	}
	//int nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
	int nOrgMs = pEventFirst->m_nmsStart;
	//CTimeSpan tmSpan = pEvent->m_tmSign - m_pReport->m_tmFaultTime;
	CTimeSpan tmSpan = pEvent->m_tmSign - pEventFirst->m_tmStart;
	LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
	//转为毫秒值
	LONG nMs = nSEC* 1000;
	LONG nCurMs = pEvent->m_nmsSign + nMs;
	//计算差值
	LONG nDis = nCurMs - nOrgMs;
	sReturn.Format("%d "+StringFromID(IDS_COMMON_MILLISECOND), nDis);
	return sReturn;
}


int AddFaultAction(_Level* level, CSecObj* pSecObj, CFaultReport* pReport)
{
	int rowNum = 0;
	CPtrArray  m_listTempSign;
	GetTempSignArray(m_listTempSign, pReport, pSecObj);
	if(m_listTempSign.GetSize() <= 0)
		return 0;
	CFaultEvent* pEventFirst = (CFaultEvent*)m_listTempSign[0];
	MYASSERT_NULL(pEventFirst);

	_Cell* cell = AddCellToList(level, StringFromID(IDS_EVENTTYPE_ACTION), 0, BODY_FONT_SIZE, 1, 1, 2, CELL2_LENGTH);
	MYASSERT_NULL(cell);

	for (int i = 0; i < m_listTempSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listTempSign[i];
		MYASSERT_CONTINUE(pEvent);
		AddCellToList(level, CalReletiveTime(pEventFirst, pEvent), 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH);
		
		CString str;
		str += pEvent->m_sEventDefName;
		str += " ";
		str += pEvent->m_sEventContent == "1" ? StringFromID(IDS_CASE_ACTION) : StringFromID(IDS_CASE_RETURN);
		AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
		++rowNum;
	}

	cell->showrows = rowNum;
	return rowNum;
}

int AddFaultOSC(_Level* level, CSecObj* pSecObj, CFaultReport* pReport)
{
	int rowNum = 0;

	CPtrArray  m_listTempSign;
	GetTempPtOscArray(m_listTempSign, pReport, pSecObj);
	if(m_listTempSign.GetSize() <= 0)
		return 0;

	CFaultEvent* pEventFirst = (CFaultEvent*)m_listTempSign[0];
	MYASSERT_NULL(pEventFirst);

	_Cell* cell = AddCellToList(level, StringFromID(IDS_EVENTTYPE_OSC), 0, BODY_FONT_SIZE, 1, 1, 2, CELL2_LENGTH);
	MYASSERT_NULL(cell);

	for (int i = 0; i < m_listTempSign.GetSize(); i ++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listTempSign[i];
		MYASSERT_CONTINUE(pEvent);
		CString str;
		str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH, 1, 1);
		
		str = pEvent->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
		{
			str.Delete(0, nRFind+1);
		}
		AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 4, CELL4_LENGTH);
		++rowNum;
	}

	if (rowNum == 0)
	{
		AddCellToList(level, "", 0, BODY_FONT_SIZE, 1, 1, 3, CELL3_LENGTH + CELL4_LENGTH);
		++rowNum;
	}
	cell->showrows = rowNum;
	return rowNum;
}

void AddProtectInfo(CSecObj* pSecObj, CFaultReport* pReport)
{
	_Level* level = new _Level;
	MYASSERT(level);
	int rowsNum = 0;
	rowsNum += AddFaultProfile(level, pSecObj, pReport);
	rowsNum += AddFaultAction(level, pSecObj, pReport);
	rowsNum += AddFaultOSC(level, pSecObj, pReport);

	AddCellToList(level, pSecObj->m_sName, 0, BODY_FONT_SIZE, 1, rowsNum, 1, CELL1_LENGTH, 1, 0, 1);

	level->cols = 4;
	level->rows = rowsNum;	
	level_List.AddTail(level);
}

void AddProtectsInfo(CFaultReport* pReport)
{
	for(int i  = 0; i < pReport->m_listSec.GetSize(); i++)
	{
		CSecObj* pObj = (CSecObj*)pReport->m_listSec.GetAt(i);
		MYASSERT_CONTINUE(pObj);
		AddProtectInfo(pObj, pReport);
	}
}

int AddOSC(_Level* level, CPtrArray& listWROSC)
{
	if (listWROSC.GetSize() == 0)
	{
		return 0; 
	}
	int rowNum = 0;
	for (int i = 0; i < listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)listWROSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		CString str;
		str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 4, CELL3_LENGTH, 1, 1);
		
		str = pEvent->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
		{
			str.Delete(0, nRFind+1);
		}
		AddCellToList(level, str, 0, BODY_FONT_SIZE, 1, 1, 5, CELL4_LENGTH);
		++rowNum;
	}
	return rowNum;
}


int AddPriDeviceOSC(_Level* level, CPtrArray& listWROSC)
{
	if (listWROSC.GetSize() == 0)
	{
		return 0; 
	}
	int rowNum = 0;
	CFaultEvent* pEvent = (CFaultEvent*)listWROSC.GetAt(0);
	MYASSERT_NULL(pEvent);
	CString strPriDeviceName = pEvent->m_pSec->m_sName;
	_Cell* cell = AddCellToList(level, strPriDeviceName, 0, BODY_FONT_SIZE, 1, 1, 3, CELL2_LENGTH);
	MYASSERT_NULL(cell);
	CPtrArray  samePriArray;
	CPtrArray  tempArray;
	for (int i  = 0; i < listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)listWROSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		if (pEvent->m_pSec->m_sName == strPriDeviceName)
		{
			samePriArray.Add(pEvent);
		}
		else
		{
			tempArray.Add(pEvent);
		}
	}
	
	cell->showrows = AddOSC(level, samePriArray);
	rowNum += cell->showrows;
	rowNum += AddPriDeviceOSC(level, tempArray);
	return rowNum;
}



int AddStationOSC(_Level* level, CPtrArray& listWROSC)
{
	if (listWROSC.GetSize() == 0)
	{
		return 0; 
	}
	int rowNum = 0;
	CFaultEvent* pEvent = (CFaultEvent*)listWROSC.GetAt(0);
	MYASSERT_NULL(pEvent);
	CString strStationName  = pEvent->m_pSec->m_pStation->m_sName;
	_Cell* cell = AddCellToList(level, strStationName, 0, BODY_FONT_SIZE, 1, 1, 2, CELL1_LENGTH%2 == 0 ? CELL1_LENGTH/2 : (CELL1_LENGTH + 1)/2);
	MYASSERT_NULL(cell);
	CPtrArray  sameStationArray;
	CPtrArray  tempArray;
	for (int i  = 0; i < listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)listWROSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		if (pEvent->m_pSec->m_pStation->m_sName == strStationName)
		{
			sameStationArray.Add(pEvent);
		}
		else
		{
			tempArray.Add(pEvent);
		}
	}

	cell->showrows = AddPriDeviceOSC(level, sameStationArray);
	rowNum += cell->showrows;
	rowNum += AddStationOSC(level, tempArray);
	return rowNum;
}

void AddOSCFile(CFaultReport* pReport)
{
	_Level* level = new _Level;
	MYASSERT(level);
	_Cell* cell = AddCellToList(level, StringFromID(IDS_FAULT_RECORDER_OSC), 0, BODY_FONT_SIZE, 1, 1, 1, CELL1_LENGTH%2 == 0 ? CELL1_LENGTH/2 : (CELL1_LENGTH - 1)/2);
	MYASSERT(cell);

	cell->showrows = AddStationOSC(level, pReport->m_listWROSC);
	level->cols = 5;

	if (cell->showrows == 0)
	{
		cell->cells = CELL1_LENGTH;
		cell->showrows = 1;
		level->cols = 2;
		AddCellToList(level, "", 0, BODY_FONT_SIZE, 1, 1, 2, CELL_ALL_LENGTH - CELL1_LENGTH);
	}
	level->rows = cell->showrows;
	level_List.AddTail(level);
}

void ParserReport(CFaultReport* pReport)
{
	AddTitle();
	AddBasicInfo(pReport);
	AddProtectsInfo(pReport);
	AddOSCFile(pReport);
}

void MergeCells(int startRow, int startCol, _Cell* cell, _Worksheet& oSheet, COleVariant& vOpt)
{
	Range   r;
	Range unionRange;
	r.AttachDispatch(oSheet.GetCells());   
	unionRange.AttachDispatch(r.GetItem   (COleVariant((long)startRow),COleVariant((long)startCol)).pdispVal   ); 
	unionRange.AttachDispatch(unionRange.GetResize(COleVariant((long)cell->showrows),COleVariant((long)cell->cells))); 
	unionRange.Merge(COleVariant((long)0));       //合并单元格
	unionRange.SetWrapText(COleVariant((short)1));
	if (cell->cellType == 1)
	{
		unionRange.SetNumberFormat(COleVariant("YYYY-mm-dd HH:MM:SS.000"));
	}
	
	unionRange.SetValue2(COleVariant(cell->font->strValue));
	if (cell->font->bCenter)
	{
		//short num = cell->col/2;
		unionRange.SetHorizontalAlignment(COleVariant((short)3));
	}
//	unionRange.AutoFill(unionRange, 0);
	
	Font ft;
	ft = unionRange.GetFont();
	if (cell->font->bBold)
	{
		ft.SetBold(COleVariant((short)1));
	}
	ft.SetSize(COleVariant((short)cell->font->size));
	if (cell->bBorder)
	{
		unionRange.BorderAround(COleVariant((short)1),(long)2,(long)1, vOpt);
	}
	r.ReleaseDispatch();
	unionRange.ReleaseDispatch();
}

void CalNowPos(int& Row, int& col, _Level* level, _Cell* cell, _Cell* lastCell)
{
	col = 1;
	if (cell->col > 1)
	{
		col += CELL1_LENGTH;
	}
    if (cell->col > 2)
	{
		col += CELL2_LENGTH;
	}
	if (cell->col > 3)
	{
		col += CELL3_LENGTH;
	}

	if (lastCell != NULL && cell->col != lastCell->col && lastCell->showrows != level->rows && cell->col < lastCell->col)
	{
		Row += lastCell->showrows;
	}
}

void CalNowPosV2(int& Row, int& col, _Level* level, _Cell* cell, _Cell* lastCell)
{
	col = 1;
	if (cell->col > 1)
	{
		col += CELL1_LENGTH%2 == 0 ? CELL1_LENGTH/2 : (CELL1_LENGTH - 1)/2;
	}
    if (cell->col > 2)
	{
		col += CELL1_LENGTH%2 == 0 ? CELL1_LENGTH/2 : (CELL1_LENGTH + 1)/2;
	}
	if (cell->col > 3)
	{
		col += CELL2_LENGTH;
	}
	if (cell->col > 4)
	{
		col += CELL3_LENGTH;
	}
	
	if (lastCell != NULL && cell->col != lastCell->col && lastCell->showrows != level->rows && cell->col < lastCell->col)
	{
		Row += lastCell->showrows;
	}
}


void ParserCell(_Worksheet& oSheet, _Level* level, int& iCurrentRows, COleVariant& vOpt)
{
	int currentCol = 1;
	int currentRow = iCurrentRows;
	_Cell* lastCell = NULL;
	POSITION pos = level->cellList.GetHeadPosition();
	while (pos != NULL)
	{
		_Cell* cell = (_Cell*)level->cellList.GetNext(pos);
		MYASSERT_CONTINUE(cell);
		if (level->cols > 4)
		{
			CalNowPosV2(currentRow, currentCol, level, cell, lastCell);
		}
		else
		{
			CalNowPos(currentRow, currentCol, level, cell, lastCell);
		}
		MergeCells(currentRow, currentCol, cell, oSheet, vOpt);
		lastCell = cell;
	}
}

void ParserLevel(_Worksheet& oSheet, COleVariant& vOpt)
{
	
	POSITION pos = level_List.GetHeadPosition();
	int iCurrentRow = 1;
	while (pos != NULL)
	{
		_Level* level = (_Level*)level_List.GetNext(pos);
		ParserCell(oSheet, level, iCurrentRow, vOpt);
		iCurrentRow += level->rows;
	}
}

void ExportByExcel(CString strPath)
{

	CoInitialize(NULL); 
	COleVariant   vOpt((long)DISP_E_PARAMNOTFOUND,   VT_ERROR); 
	//启动EXCEL 
	_Application   oApp; 
	if(!oApp.CreateDispatch("Excel.Application")) 
	{ 
		AfxMessageBox( StringFromID(IDS_EXCEL_STARTFAIL)); 
		return; 
	} 
	//向工作簿中添加新工作表 
	Workbooks   oBooks   =   oApp.GetWorkbooks(); 
	_Workbook   oBook   =   oBooks.Add(vOpt); 
	Worksheets   oSheets   =   oBook.GetWorksheets(); 
	//获取第一张工作表 
	_Worksheet   oSheet   =   oSheets.GetItem(COleVariant((short)1)); 
	
	ParserLevel(oSheet, vOpt);
	CString sVersion = oApp.GetVersion();
	oBook.SaveAs(COleVariant(strPath),_variant_t((long)56),vOpt,vOpt,vOpt,vOpt,(long)1,vOpt,vOpt,vOpt,vOpt,vOpt);
// 	oBook.SaveAs(COleVariant(strPath),&vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,
//                     vOpt,vOpt,vOpt,vOpt,vOpt, vOpt,vOpt,vOpt,vOpt);
	oSheet.ReleaseDispatch();
	oSheets.ReleaseDispatch();
	oBook.Close(COleVariant((short)FALSE), vOpt, vOpt);
	oBooks.Close();
	oApp.Quit();
	CoUninitialize(); 

}

void ClearCellList(CPtrList* cellList)
{
	POSITION pos = cellList->GetHeadPosition();
	while (pos != NULL)
	{
		_Cell* cell = (_Cell*)cellList->GetNext(pos);
		MYASSERT_CONTINUE(cell);
		if (cell->font != NULL)
		{
			delete cell->font;
		}
		delete cell;
	}
	cellList->RemoveAll();
}



void ClearLevelList()
{
	POSITION pos = level_List.GetHeadPosition();
	while (pos != NULL)
	{
		_Level* level = (_Level*)level_List.GetNext(pos);
		MYASSERT_CONTINUE(level);
		ClearCellList(&level->cellList);
		delete level;
	}
	level_List.RemoveAll();
}

/****************************************************
Date:2012/4/5  Author:LYH
函数名:   GetTempPtOscArray	
返回值:   void	
功能概要: 
参数: CPtrArray & listTempSign	
参数: CFaultReport * pReport	
参数: CSecObj * pSecObj	
*****************************************************/
void GetTempPtOscArray( CPtrArray& listTempSign, CFaultReport* pReport, CSecObj* pSecObj )
{
	for(int i = 0; i < pReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)pReport->m_listPTOSC.GetAt(i);
		if(pEvent != NULL)
		{
			if(pEvent->m_pSec == pSecObj)
				listTempSign.Add(pEvent);
		}
	}
}

/****************************************************
Date:2012/9/10  Author:LYH
AmendDate:2012/10/9  Author:LY
函数名:   ExportWaveFile	
返回值:   void	
功能概要: 
参数: CString strDir	
参数: CFaultReport * pReport	
*****************************************************/
void ExportWaveFile( CString strDir, CFaultReport* pReport )
{
	if(strDir.IsEmpty())
		return;
	if(strDir.Right(1) != "\\")
		strDir += "\\";
	if(pReport == NULL)
		return;
	//CString sDir=strDir;		
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();


	CString sDir = strDir;
	CString sDir2 = strDir;
	CString filename;
	filename.Format("%s",pReport->m_sName);

	
	sDir += "保护\\";
	sDir2 += "录波器\\";
	CString sDir3;// = sDir + filename;
	CString sDir4;// = sDir2 + filename;

	if(CreateDirectory(sDir, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CString strLog;
			strLog.Format("创建目录失败,目录已存在:%s", sDir);
			WriteLog(strLog, XJ_LOG_LV3);
			/*ExportWaveFile( sDir, pReport);*/
		}
		else
		{
			CString strLog;
			sDir  += "\\";
			strLog.Format("创建目录成功:%s", sDir);
			WriteLog(strLog, XJ_LOG_LV3);
		}
// 		sDir3 = sDir + filename + "\\";
// 		if(CreateDirectory(sDir3, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
// 		{
// 			if(GetLastError() == ERROR_ALREADY_EXISTS)
// 			{
// 				CString strLog;
// 				strLog.Format("创建目录失败,目录已存在:%s", sDir3);
// 				WriteLog(strLog, XJ_LOG_LV3);
// 				//ExportWaveFile( sDir, pReport);
// 			}
// 			else
// 			{
// 				CString strLog;
// 				sDir3  += "\\";
// 				strLog.Format("创建目录成功:%s", sDir3);
// 				WriteLog(strLog, XJ_LOG_LV3);
// 			}
			CopyProWaveFile(sDir, pReport);
	//	}
	//	else
	//	{
// 			
// 				CString strLog;
// 				strLog.Format("创建目录失败:%s", sDir3);
// 				WriteLog(strLog, XJ_LOG_LV1);
// 		}

		
	}
	else
	{
		CString strLog;
		strLog.Format("创建目录失败:%s", sDir);
		WriteLog(strLog, XJ_LOG_LV1);	
	}



	if(CreateDirectory(sDir2, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CString strLog;
			strLog.Format("创建目录失败,目录已存在:%s", sDir2);
			WriteLog(strLog, XJ_LOG_LV3);
			//ExportWaveFile( sDir, pReport);
		}
		else
		{
			CString strLog;
			sDir2  += "\\";
			strLog.Format("创建目录成功:%s", sDir2);
			WriteLog(strLog, XJ_LOG_LV3);
		}
// 		sDir4 = sDir2 + filename + "\\";
// 		if(CreateDirectory(sDir4, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
// 		{
// 			if(GetLastError() == ERROR_ALREADY_EXISTS)
// 			{
// 				CString strLog;
// 				strLog.Format("创建目录失败,目录已存在:%s", sDir4);
// 				WriteLog(strLog, XJ_LOG_LV3);
// 				//ExportWaveFile( sDir, pReport);
// 			}
// 			else
// 			{
// 				CString strLog;
// 				sDir4  += "\\";
// 				strLog.Format("创建目录成功:%s", sDir4);
// 				WriteLog(strLog, XJ_LOG_LV3);
// 			}
			CopyOSCWaveFile(sDir2, pReport);
// 		}
// 		else
// 		{
// 			CString strLog;
// 			strLog.Format("创建目录失败:%s", sDir4);
// 			WriteLog(strLog, XJ_LOG_LV1);	
// 		}
	}
	else
	{
		CString strLog;
		strLog.Format("创建目录失败:%s", sDir2);
		WriteLog(strLog, XJ_LOG_LV1);	
	}
}
/****************************************************
Date:2012/11/21  Author:LY
函数名:   CopyProWaveFile	
返回值:   void	
功能概要:	复制文件
参数: CString strDir	
参数: CFaultReport * pReport	
*****************************************************/
void CopyProWaveFile( CString sDir3, CFaultReport* pReport )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	for(int i = 0; i < pReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)pReport->m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		//int nlength = pEvent->m_pSec->m_pStation->m_sID.GetLength();
		CString sStation = sDir3;
		sStation.Delete(sStation.Find("\\保护"),sStation.GetLength()-sStation.Find("\\保护"));
		while (sStation.Find("\\"))
		{
			int nlen = sStation.Find("\\") + 1;
			if (nlen == 0)
				break;
			sStation.Delete(0,nlen);
		}
		
		if (sStation != pEvent->m_pSec->m_pStation->m_sID)
			continue;
		
		CString strPath = pEvent->m_sEventDef;
		CString strLog;
		strLog.Format("录波文件路径:%s", strPath);
		WriteLog(strLog, XJ_LOG_LV3);
		//去后缀
		int nFind = strPath.ReverseFind('.');
		if(nFind != -1)
		{
			strPath = strPath.Left(nFind);
		}
		CString strFullPath = strDownDir + strPath;
		
		strLog.Format("查找文件路径:%s, downdir:%s, path:%s", strFullPath+".*", strDownDir, strPath);
		WriteLog(strLog, XJ_LOG_LV3);
		
		CFileFind filefind;
		CFileFind tempfind;
		BOOL bFind = filefind.FindFile(strFullPath+".*");
		if(!bFind)
		{
			CString strLog;
			strLog.Format("查找文件失败,路径:%s", strFullPath+".*");
			WriteLog(strLog, XJ_LOG_LV1);
		}
		while(bFind)
		{
			bFind = filefind.FindNextFile();
			if(!filefind.IsDots() && !filefind.IsDirectory())
			{
// 				CString sfillname = filefind.GetFileName();
// 				//去后缀
// 				int nFind = sfillname.ReverseFind('.');
// 				if(nFind != -1)
// 				{
// 					sfillname = sfillname.Left(nFind);
// 				}
				CString sDir5 = sDir3 + pEvent->m_pSec->m_sName/*sfillname*/;

				if(CreateDirectory(sDir5, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
				{
					if(GetLastError() == ERROR_ALREADY_EXISTS)
					{
						CString strLog;
						strLog.Format("创建目录失败,目录已存在:%s", sDir5);
						WriteLog(strLog, XJ_LOG_LV3);
						//ExportWaveFile( sDir, pReport);
					}
					else
					{
						CString strLog;
					
						strLog.Format("创建目录成功:%s", sDir5);
						WriteLog(strLog, XJ_LOG_LV3);
					}
						sDir5  += "\\";
					CString sOldPath = filefind.GetFilePath();
					CString sNewPath = sDir5 + filefind.GetFileName();
					if(!CopyFile(sOldPath, sNewPath, TRUE))
					{
						CString str;
						str.Format("复制文件失败,原始路径:%s, 新路径:%s", sOldPath, sNewPath);
						WriteLog(str, XJ_LOG_LV1);
					}
					else
					{
						CString str;
						str.Format("复制文件成功,原始路径:%s, 新路径:%s", sOldPath, sNewPath);
						WriteLog(str, XJ_LOG_LV3);
					}
				}
				else
				{
					CString strLog;
					strLog.Format("创建目录失败:%s", sDir5);
					WriteLog(strLog, XJ_LOG_LV1);	
				}

			}
		}
	}
}
/****************************************************
Date:2012/11/21  Author:LY
函数名:   CopyWaveFile	
返回值:   void	
功能概要:	复制文件
参数: CString strDir	
参数: CFaultReport * pReport	
*****************************************************/
void CopyOSCWaveFile( CString sDir4, CFaultReport* pReport )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	for(int k = 0; k < pReport->m_listWROSC.GetSize(); k++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)pReport->m_listWROSC.GetAt(k);
		if(pEvent == NULL)
			continue;

		int nlength = pEvent->m_pSec->m_pStation->m_sID.GetLength();
		CString sStation = sDir4;
		sStation.Delete(sStation.Find("\\录波器"),sStation.GetLength()-sStation.Find("\\录波器"));
	
		while (sStation.Find("\\"))
		{
			int nlen = sStation.Find("\\") + 1;
			if (nlen == 0)
				break;
			sStation.Delete(0,nlen);
		}
		
		if (sStation != pEvent->m_pSec->m_pStation->m_sID)
			continue;

		CString strPath = pEvent->m_sEventDef;
		CString strLog;
		strLog.Format("录波文件路径:%s", strPath);
		WriteLog(strLog, XJ_LOG_LV3);
		//去后缀
		int nFind = strPath.ReverseFind('.');
		if(nFind != -1)
		{
			strPath = strPath.Left(nFind);
		}
		CString strFullPath = strDownDir + strPath;
		strLog.Format("查找文件路径:%s, downdir:%s, path:%s", strFullPath+".*", strDownDir, strPath);
		WriteLog(strLog, XJ_LOG_LV3);
		
		CFileFind filefind;
		CFileFind tempfind;
		BOOL bFind = filefind.FindFile(strFullPath+".*");
		while(bFind)
		{
			bFind = filefind.FindNextFile();
			if(!filefind.IsDots()&&!filefind.IsDirectory())
			{
// 				CString sfillname = filefind.GetFileName();
// 				//去后缀
// 				int nFind = sfillname.ReverseFind('.');
// 				if(nFind != -1)
// 				{
// 					sfillname = sfillname.Left(nFind);
// 				}
				CString sDir6 = sDir4 + pEvent->m_pSec->m_sName/*sfillname*/;
				if(CreateDirectory(sDir6, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
				{
					if(GetLastError() == ERROR_ALREADY_EXISTS)
					{
						CString strLog;
						strLog.Format("创建目录失败,目录已存在:%s", sDir6);
						WriteLog(strLog, XJ_LOG_LV3);
						//ExportWaveFile( sDir, pReport);
					}
					else
					{
						CString strLog;
					
						strLog.Format("创建目录成功:%s", sDir6);
						WriteLog(strLog, XJ_LOG_LV3);
					}
					sDir6  += "\\";
					CString sOldPath = filefind.GetFilePath();
					CString sNewPath = sDir6 + filefind.GetFileName();
					if(!CopyFile(sOldPath, sNewPath, TRUE))
					{
						CString str;
						str.Format("复制文件失败,原始路径:%s, 新路径:%s", sOldPath, sNewPath);
						WriteLog(str, XJ_LOG_LV1);
					}
					else
					{
						CString str;
						str.Format("复制文件成功,原始路径:%s, 新路径:%s", sOldPath, sNewPath);
						WriteLog(str, XJ_LOG_LV1);
					}
				}
				else
				{
					CString strLog;
					strLog.Format("创建目录失败:%s", sDir6);
					WriteLog(strLog, XJ_LOG_LV1);	
				}

			}
		}
	}
}