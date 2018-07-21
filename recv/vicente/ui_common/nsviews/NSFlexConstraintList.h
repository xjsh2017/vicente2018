// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexConstraintList_h__
#define __NSFlexConstraintList_h__

class CNSFlexConstraint
{

  public:

    CNSFlexConstraint(UINT nControlID,
        const CNSFlexHorizontalConstraint& HorizontalConstraint,
        const CNSFlexVerticalConstraint& VerticalConstraint);

    ~CNSFlexConstraint();

    void SetBasePosition(int nBaseLeft, int nBaseTop, int nBaseRight, 
        int nBaseBottom);

    BOOL Init(const CWnd* pContainerWnd, int nClientWidth, int nClientHeight);

    UINT GetControlID() const;

    int GetBaseLeft() const;
    int GetBaseTop() const;
    int GetBaseRight() const;
    int GetBaseBottom() const;

    float GetLeftLocator() const;
    float GetTopLocator() const;
    float GetRightLocator() const;
    float GetBottomLocator() const;

  private:

    UINT m_nControlID;

    int m_nBaseLeft;
    int m_nBaseTop;
    int m_nBaseRight;
    int m_nBaseBottom;

    CNSFlexHorizontalConstraint* m_pHorizontalConstraint;
    CNSFlexVerticalConstraint* m_pVerticalConstraint;
    
};

inline UINT CNSFlexConstraint::GetControlID() const
{
    return m_nControlID;
}

inline int CNSFlexConstraint::GetBaseLeft() const
{
    return m_nBaseLeft;
}

inline int CNSFlexConstraint::GetBaseTop() const
{
    return m_nBaseTop;
}

inline int CNSFlexConstraint::GetBaseRight() const
{
    return m_nBaseRight;
}

inline int CNSFlexConstraint::GetBaseBottom() const
{
    return m_nBaseBottom;
}

inline float CNSFlexConstraint::GetLeftLocator() const
{
    return m_pHorizontalConstraint->GetLeft();
}

inline float CNSFlexConstraint::GetTopLocator() const
{
    return m_pVerticalConstraint->GetTop();
}

inline float CNSFlexConstraint::GetRightLocator() const
{
    return m_pHorizontalConstraint->GetRight();
}

inline float CNSFlexConstraint::GetBottomLocator() const
{
    return m_pVerticalConstraint->GetBottom();
}


class CNSFlexConstraintList
{

  public:

    CNSFlexConstraintList(CWnd* pContainerWnd);

    virtual ~CNSFlexConstraintList(); 

    void AddConstraint(UINT nControlID,
        const CNSFlexHorizontalConstraint& HorizontalConstraint,
        const CNSFlexVerticalConstraint& VerticalConstraint);

#ifdef _DEBUG

    BOOL AssertAllFlexControlsExist();

#endif

    void PositionControls(int nClientWidth, int nClientHeight);

    void SetClientBaseSize(int nClientBaseWidth, int nClientBaseHeight);

  private:

    void InitConstraints();

    CWnd* m_pContainerWnd;

    int m_nClientBaseWidth;
    int m_nClientBaseHeight;

    CPtrList m_FlexConstraints;

    BOOL m_bConstraintsInitialized;

};

#endif
                                   
