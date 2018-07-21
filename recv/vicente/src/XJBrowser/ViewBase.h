// ***************************************************************
//  ViewBase   version:  1.0    date: 10:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �̳���FormView, �ṩ��̬�ı�ؼ�λ�ù���
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_)
#define AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewBase.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CViewBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B860205
struct ctrlPos{
	//##ModelId=49B87B860214
	UINT	nID;		//�ؼ�ID
	//##ModelId=49B87B860215
	float	fWidth;		//�ؼ����/�ͻ������
	//##ModelId=49B87B860216
	float	fHeight;	//�ؼ��߶�/�ͻ����߶�
	//##ModelId=49B87B860217
	int		nType;		//�仯����
	//##ModelId=49B87B860218
	int		nID_Left;	//�������ĸ��ؼ�λ�ò���
	//##ModelId=49B87B860219
	int		Left_Dis;	//�����Ծ���
	//##ModelId=49B87B860222
	int		nID_Top;	//�ϱ�����ĸ��ؼ�λ�ò���
	//##ModelId=49B87B860223
	int		Top_Dis;	//�ϱ���Ծ���
	//##ModelId=49B87B860224
	int		nID_Right;	//�ұ�����ĸ��ؼ�λ�ò���
	//##ModelId=49B87B860225
	int		Right_Dis;	//�ұ���Ծ���
	//##ModelId=49B87B860226
	int		nID_Bottom;	//�±�����ĸ��ؼ�λ�ò���
	//##ModelId=49B87B860227
	int		Bottom_Dis;	//�±���Ծ���
};

//���λ�ö���
int const	left_client = -1;	//�ͻ������λ��
int const	right_client = -2;	//�ͻ����ұ�λ��
int const	top_client = -3;	//�ͻ����ϱ�λ��
int const	bottom_client = -4; //�ͻ����±�λ��
int const	mid_client = -5;	//�ͻ����м�λ��,����
int const	null_effect = -99;	//������Ч

//��ȸ߶ȸı������
int const	change_null = 0;	//��ȸ߶ȶ�����
int const	change_width = 1;	//��ȸı�
int const	change_height = 2;	//�߶ȸı�
int	const	change_all	= 3;	//���ı�


//##ModelId=49B87B860242
class CViewBase : public CFormView
{
protected:
	//##ModelId=49B87B860244
	CViewBase();           // protected constructor used by dynamic creation
	//##ModelId=49B87B860245
	CViewBase(UINT nIDTemplate);
	DECLARE_DYNCREATE(CViewBase)

// Form Data
public:
	//{{AFX_DATA(CViewBase)
	enum { IDD = 0 };
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B860248
	CArray<ctrlPos,ctrlPos> posCtrl;
	//##ModelId=49B87B860254
	UINT m_nDlgID;
	//##ModelId=49B87B860255
	CSize szDlg;
	/** @brief           ��¼ҳ���Ƿ���������ϸ��Ϣ*/
	//##ModelId=49B87B860256
	BOOL m_bLoadData;
	/** @brief           �Զ��ٻ�����*/
	CString		m_sCyc;

// Operations
public:
/*
 *  @brief   	SetCyc	 �����Զ��������� 
 *  @param 		[In]a param of Type  CString  ��һ������ʱ��
 *  @param 		[In]a param of Type  CString  ����
 *  @return 	void 
 */
	//##ModelId=49B87BB20138
	virtual void SetCyc(CString sStartTime, CString sCyc){} ;

/*
 *  @brief   	MoveCtrl	�ƶ��ؼ�λ��,����MFC��MoveWindow 
 *  @param 		[In]a param of Type  UINT  Ҫ�ƶ��Ŀؼ���ID
 *  @param 		[In]a param of Type  int  ��λ��,���������������ؼ�ID,���Ǵ˿ؼ�right + ƫ����
 *  @param 		[In]a param of Type  int  �Ϸ�λ��,���������������ؼ�ID,���Ǵ˿ؼ�bottom + ƫ����
 *  @param 		[In]a param of Type  int  �ҷ�λ��,���������������ؼ�ID,���Ǵ˿ؼ�left - ƫ����
 *  @param 		[In]a param of Type  int  �·�λ��,���������������ؼ�ID,���Ǵ˿ؼ�top - ƫ����
 *	@param		[In]a param of Type	 int  ƫ����
 *  @return 	void 
 */
	//##ModelId=49B87B860257
	void MoveCtrl(UINT nID, int nLeft, int nTop, int nRight, int nBottom, int offset = 10);
/*
 *  @brief   	RegulateControlSize	 �����ؼ�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B860263
	void RegulateControlSize();
/*
 *  @brief   	RecordRate	 ��¼�ؼ����� 
 *  @param 		[In/]a param of Type  UINT  Ҫ��¼�Ŀؼ���ID
 *  @param 		[In]a param of Type  int  �ı�����
 *  @param 		[In]a param of Type  int  �����Ե�λ��
 *  @param 		[In]a param of Type  int  �ϱ���Ե�λ��
 *  @param 		[In]a param of Type  int  �ұ���Ե�λ��
 *  @param 		[In]a param of Type  int  �±���Ե�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B860264
	void RecordRate(UINT nID, int nType = 0, int nLeft = null_effect, 
		int nTop = null_effect, int nRight = null_effect, int nBottom = null_effect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewBase)
	public:
	//##ModelId=49B87B860271
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B860275
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B860277
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B860280
	virtual ~CViewBase();
#ifdef _DEBUG
	//##ModelId=49B87B860282
	virtual void AssertValid() const;
	//##ModelId=49B87B860284
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewBase)
	//##ModelId=49B87B860290
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87B860295
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_)
