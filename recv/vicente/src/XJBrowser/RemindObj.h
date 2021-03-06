#ifndef _REMINDOBJ_H_
#define	_REMINDOBJ_H_
#include "StdAfx.h"

class CRemindContent
{
public:
	CRemindContent(CString sTypeName, CString sContent);
	~CRemindContent();
private:
	/** @brief			内容*/
	CString m_sContent;		
	/** @brief			类型名*/
	CString	m_sTypeName;	
	/** @brief			触发时间*/
	DWORD	m_dwTriggerTime;
	
public:
	CString GetContent() const { return m_sContent; };	
	void SetContent(CString Content) { m_sContent = Content; };
	CString GetTypeName() const { return m_sTypeName; };	
	void SetTypeName(CString TypeName) { m_sTypeName = TypeName; };
	DWORD GetTriggerTime() const { return m_dwTriggerTime; };	
	void SetTriggerTime(DWORD TriggerTime) { m_dwTriggerTime = TriggerTime; };
};
/********************************************************************
	created:	2013/10/09
	filename: 	RemindObj.h
	author:		LYH
	purpose:	表现一个提醒对象
*********************************************************************/
class CRemindObj
{
public:
	CRemindObj(CString sTypeName);
	~CRemindObj();
private:

	/** @brief			内容*/
	CString m_sContent;	
	/** @brief			类型名*/
	CString	m_sTypeName;
	/** @brief			用于当前显示的内容*/
	CString m_sDisplayContent;
	/** @brief			当前显示内容在总内容中的位置*/
	int m_nCurIndex;

	/** @brief			图形范围*/
	CRect	m_rcBound;
	/** @brief			类型名图形宽度*/
	int		m_nTypeNameWidth;

	/** @brief			类型名图形框颜色*/
	COLORREF	m_colTypeName;
	/** @brief			内容图形框颜色*/
	COLORREF	m_colContent;
	/** @brief			字体*/
	CFont		m_ftFont;

	/** @brief			内容对象链表*/
	CPtrList	m_listContent;
	

	/** @brief			是否激活*/
	BOOL	m_bActive;
	
public:
	CString GetContent() const { return m_sContent; };	
	void SetContent(CString Content) { m_sContent = Content; };
	CString GetTypeName() const { return m_sTypeName; };	
	void SetTypeName(CString TypeName) { m_sTypeName = TypeName; };
	void SetBound(int x, int y, int w, int h) {m_rcBound.SetRect(x, y, x+w, y+h); };
	CRect GetBound(){ return m_rcBound; };
	BOOL GetActive() const { return m_bActive; };	
	void SetActive(BOOL Active) { m_bActive = Active; };

	void Draw(CDC* pDC);

	void AddContent(CRemindContent* pRC);
	void RemoveHead();
	
	void Update(DWORD nElapse);

	/*
	* @brief	IsMouseOn	鼠标是否在对象图形范围内
	* @param	[In]a param of Type CPoint pt 鼠标位置		
	* @return   BOOL	在对象图形范围内返回TRUE,否则返回FALSE
	*/
	BOOL IsMouseOn(CPoint pt){return m_rcBound.PtInRect(pt);};

private:
	BOOL IsChinese(CString str);
};


#endif