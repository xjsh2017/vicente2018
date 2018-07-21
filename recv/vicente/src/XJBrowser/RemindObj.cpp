#include "RemindObj.h"
#include "xjbrowser.h"


/****************************************************
Date:2013/10/9  Author:LYH
函数名:   CRemindContent	
返回值:   	
功能概要: 
参数: CString sTypeName	
参数: CString sContent	
*****************************************************/
CRemindContent::CRemindContent( CString sTypeName, CString sContent )
{
	m_sTypeName = sTypeName;
	m_sContent = sContent;
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   ~CRemindContent	
返回值:   	
功能概要: 
*****************************************************/
CRemindContent::~CRemindContent()
{
	
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   CRemindObj	
返回值:   	
功能概要: 
*****************************************************/
CRemindObj::CRemindObj(CString sTypeName)
{
	m_sContent = "";	
	m_sTypeName = sTypeName;

	m_bActive = FALSE;

	m_rcBound.SetRectEmpty();
	m_nTypeNameWidth = 60;

	m_colTypeName = RGB(230,173,0);
	if(m_sTypeName == StringFromID(IDS_TOOLBAR_ACTION))
		m_colContent = RGB(255, 0, 0);
	else if(m_sTypeName == StringFromID(IDS_TOOLBAR_ALARM))
		m_colContent = RGB(130,148,73);
	else if(m_sTypeName == StringFromID(IDS_TOOLBAR_OSC))
		m_colContent = RGB(171,181,218);
	else if(m_sTypeName == StringFromID(IDS_TOOLBAR_FAULTREPORT))
		m_colContent = RGB(229,229,0);

	m_listContent.RemoveAll();
	m_nCurIndex = 0;

	VERIFY(m_ftFont.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,					// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					StringFromID(IDS_DEFAULT_FONT)));                 // lpszFacename
}


/****************************************************
Date:2013/10/9  Author:LYH
函数名:   ~CRemindObj	
返回值:   	
功能概要: 
*****************************************************/
CRemindObj::~CRemindObj()
{
	POSITION pos = m_listContent.GetHeadPosition();
	while(pos)
	{
		CRemindContent* rc = (CRemindContent*)m_listContent.GetNext(pos);
		if(rc)
			delete rc;
	}
	m_listContent.RemoveAll();
}


/****************************************************
Date:2013/10/9  Author:LYH
函数名:   Draw	
返回值:   void	
功能概要: 
参数: CDC * pDC	
*****************************************************/
void CRemindObj::Draw( CDC* pDC )
{
	if(pDC == NULL)
		return;
	
	CPen contentpen(PS_SOLID, 5, RGB(0, 0, 0));
	CPen* pOldPen = NULL;
	if(m_bActive)
		pOldPen = pDC->SelectObject(&contentpen);
	CBrush contentbrush(m_colContent);
	CBrush* pOldBursh = pDC->SelectObject(&contentbrush);
	pDC->Rectangle(m_rcBound);
	pDC->SelectObject(pOldBursh);
	if(m_bActive)
		pDC->SelectObject(pOldPen);
	
	CRect rcTypeName = m_rcBound;
	rcTypeName.right = rcTypeName.left+m_nTypeNameWidth;
	CBrush typebrush(m_colTypeName);
	pOldBursh = pDC->SelectObject(&typebrush);
	pDC->Rectangle(rcTypeName);

	pDC->SetBkMode(TRANSPARENT);

	CString sDisplay = "";
	sDisplay.Format("%s(%d)", m_sTypeName, m_listContent.GetCount());
	pDC->DrawText(sDisplay, &rcTypeName, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	CRect rcContent = m_rcBound;
	rcContent.left = rcTypeName.left+m_nTypeNameWidth;

	pDC->DrawText(m_sDisplayContent, 
		&rcContent, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   AddContent	
返回值:   void	
功能概要: 
参数: CRemindContent * pRC	
*****************************************************/
void CRemindObj::AddContent( CRemindContent* pRC )
{
	if(pRC == NULL)
		return;
	CString strlog;
	//strlog.Format("[%s]提醒对象,新增内容, 新内容为:%s", GetTypeName(), pRC->GetContent());
	//WriteLog(strlog);
	pRC->SetTriggerTime(GetTickCount());
	if(g_RemindMaxCount > 0 && m_listContent.GetCount() >= g_RemindMaxCount)
	{
		RemoveHead();
		strlog.Format("[%s]提醒对象,新增内容,链表已满,删除第一个元素", GetTypeName(), pRC->GetContent());
		WriteLog(strlog);
	}
	int nCurLength = m_sContent.GetLength();
	m_listContent.AddTail(pRC);
	if(m_sContent != "")
		m_sContent += " ";
	m_sContent += pRC->GetContent();
	m_nCurIndex = nCurLength;
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   RemoveHead	
返回值:   void	
功能概要: 
*****************************************************/
void CRemindObj::RemoveHead()
{
	if(m_listContent.GetCount() == 0)
		return;
	CRemindContent* rc = (CRemindContent*)m_listContent.GetHead();
	if(!rc)
		return;
	CString sHeadContent = rc->GetContent();
	if(m_sContent.GetLength() <= sHeadContent.GetLength())
	{
		m_sContent = "";
		m_nCurIndex = 0;
	}
	else if(m_sContent.GetLength() > sHeadContent.GetLength())
	{
		m_sContent.Delete(0, sHeadContent.GetLength()+1);
		if(m_nCurIndex <= sHeadContent.GetLength())
			m_nCurIndex = 0;
		else
			m_nCurIndex -= (sHeadContent.GetLength()+1);
	}
	delete rc;
	m_listContent.RemoveHead();
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   Update	
返回值:   void	
功能概要: 
*****************************************************/
void CRemindObj::Update(DWORD nElapse)
{
	int ncount = 0;
	DWORD nstart = GetTickCount();
	while(m_listContent.GetHeadPosition() != NULL)
	{
		if( GetTickCount() - nstart >= nElapse )
			break;
		CRemindContent* rc = (CRemindContent*)m_listContent.GetHead();
		if(GetTickCount() - rc->GetTriggerTime() > 1000*60*g_RemindTime)
		{
			RemoveHead();
			ncount++;
			if( ncount%5 == 0)
				Sleep(2);
		}
		else
			break;
	}
	m_sDisplayContent = m_sContent;
	m_sDisplayContent.Delete(0, m_nCurIndex);

	m_sDisplayContent = m_sDisplayContent.Left(256);

	if(IsChinese(m_sDisplayContent))
		m_nCurIndex += 2;
	else
		m_nCurIndex++;

	if(m_nCurIndex >= m_sContent.GetLength()-1)
		m_nCurIndex = 0;
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   IsChinese	
返回值:   BOOL	
功能概要: 
参数: CString str	
*****************************************************/
BOOL CRemindObj::IsChinese( CString str )
{
	TBYTE ucHigh, ucLow;

	if(str.GetLength() < 2)
		return FALSE;
	
    if ( (TBYTE)str[0] < 0x80 )
		return FALSE;
    ucHigh = (TBYTE)str[0];
    ucLow  = (TBYTE)str[1];
    if ( ucHigh < 0xa1 || ucLow < 0xa1)
		return FALSE;
    return TRUE;
}


