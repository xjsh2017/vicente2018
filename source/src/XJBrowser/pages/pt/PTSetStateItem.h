#ifndef _PTSETMODSTATEITEM_H_
#define	_PTSETMODSTATEITEM_H_

#include "StdAfx.h"

/********************************************************************
	created:	2018/08/01
	filename: 	PTSetStateItem.h
	author:		WPS
	purpose:	����һ�����Ѷ���
*********************************************************************/

class CPTSetStateContent
{
public:
	CPTSetStateContent(CString sTypeName, CString sContent);
	~CPTSetStateContent();
private:
	/** @brief			����*/
	CString m_sContent;		
	/** @brief			������*/
	CString	m_sTypeName;		
	/** @brief			������2*/
	CString	m_sTypeName2;
	/** @brief			����ʱ��*/
	DWORD	m_dwTriggerTime;
	
public:
	CString GetContent() const { return m_sContent; };	
	CString GetTypeName() const { return m_sTypeName; };
	CString GetTypeName2() const { return m_sTypeName2; };	
	DWORD GetTriggerTime() const { return m_dwTriggerTime; };	

	void SetContent(CString Content) { m_sContent = Content; };
	void SetTypeName(CString TypeName) { m_sTypeName = TypeName; };
	void SetTypeName2(CString TypeName) { m_sTypeName2 = TypeName; };
	void SetTriggerTime(DWORD TriggerTime) { m_dwTriggerTime = TriggerTime; };
};

class CPTSetStateItem
{
public:
	CPTSetStateItem(CString sTypeName, int nPTSetStateID = -1);
	~CPTSetStateItem();
private:

	/** @brief			����*/
	CString m_sContent;	
	/** @brief			������*/
	CString	m_sTypeName;
	/** @brief			������2*/
	CString	m_sTypeName2;
	/** @brief			���ڵ�ǰ��ʾ������*/
	CString m_sDisplayContent;
	/** @brief			��ǰ��ʾ�������������е�λ��*/
	int m_nCurIndex;

	/** @brief			ͼ�η�Χ*/
	CRect	m_rcBound;
	/** @brief			������ͼ�ο��*/
	int		m_nTypeNameWidth;
	static int s_nTypeNameWidth;

	/** @brief			������ͼ�ο���ɫ*/
	COLORREF	m_colTypeName;
	/** @brief			����ͼ�ο���ɫ*/
	COLORREF	m_colContent;
	/** @brief			����*/
	CFont		m_ftFont;

	/** @brief			���ݶ�������*/
	CPtrList	m_listContent;
	

	/** @brief			�Ƿ񼤻�*/
	BOOL	m_bActive;

	/** @brief			�Ƿ���*/
	BOOL	m_bMarked;

	/** @brief			�Ƿ�ɼ�*/
	BOOL	m_bVisible;

	int		m_nCurPTSetState;
	int		m_nPTSetStateID;
	
public:
	CString GetContent() const { return m_sContent; };	
	void SetContent(CString Content) { m_sContent = Content; };
	CString GetTypeName() const { return m_sTypeName; };	
	void SetTypeName(CString TypeName) { m_sTypeName = TypeName; };
	void SetBound(int x, int y, int w, int h) {m_rcBound.SetRect(x, y, x+w, y+h); };
	CRect GetBound(){ return m_rcBound; };
	BOOL GetActive() const { return m_bActive; };	
	void SetActive(BOOL Active) { m_bActive = Active; };
	void SetCurPTSetState(int nState);
	void SetVisible(int visible) { m_bVisible = visible; }
	void SetMarked(BOOL bMarked) { m_bMarked = bMarked; }

	int GetPTSetState()  { return m_nPTSetStateID; }

	void SetTypeNameLength(int nLen) { m_nTypeNameWidth = nLen; };

	void Draw(CDC* pDC);

	void AddContent(CPTSetStateContent* pRC);
	void RemoveHead();
	
	void Update(DWORD nElapse);

	/*
	* @brief	IsMouseOn	����Ƿ��ڶ���ͼ�η�Χ��
	* @param	[In]a param of Type CPoint pt ���λ��		
	* @return   BOOL	�ڶ���ͼ�η�Χ�ڷ���TRUE,���򷵻�FALSE
	*/
	BOOL IsMouseOn(CPoint pt){return m_rcBound.PtInRect(pt);};

	int IsVisble() { return m_bVisible; }

private:
	BOOL IsChinese(CString str);
};


#endif