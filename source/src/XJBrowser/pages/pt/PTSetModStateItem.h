#ifndef _PTSETMODSTATEITEM_H_
#define	_PTSETMODSTATEITEM_H_
#include "StdAfx.h"

class CPTSetModStateContent
{
public:
	CPTSetModStateContent(CString sTypeName, CString sContent);
	~CPTSetModStateContent();
private:
	/** @brief			����*/
	CString m_sContent;		
	/** @brief			������*/
	CString	m_sTypeName;	
	/** @brief			����ʱ��*/
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
	purpose:	����һ�����Ѷ���
*********************************************************************/
class CPTSetModStateItem
{
public:
	CPTSetModStateItem(CString sTypeName, int nPTSetStateID = -1);
	~CPTSetModStateItem();
private:

	/** @brief			����*/
	CString m_sContent;	
	/** @brief			������*/
	CString	m_sTypeName;
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

	void AddContent(CPTSetModStateContent* pRC);
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