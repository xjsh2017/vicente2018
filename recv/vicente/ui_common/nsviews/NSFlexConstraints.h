// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSFlexConstraints_h__
#define __NSFlexConstraints_h__

#include "NSViewsAfxExt.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexHorizontalConstraint defines a horizontal position constraint for
// a control. The class includes virtual functions for constructing more
// complex derived constraint types.

class AFX_EXT_NSVIEWS CNSFlexHorizontalConstraint
{

  // --------------------------------------------------------------------------

  public:

    // Constructors.
    CNSFlexHorizontalConstraint(float fLeft = 0.0f, float fRight = 0.0f);
    CNSFlexHorizontalConstraint(const CNSFlexHorizontalConstraint& ref);
    //  fLeft - Position value associated with the left edge of a control.
    //    Value should be in the range of 0.0f to 1.0f.
    //  fRight - Position value associated with the right edge of a control.
    //    Value should be in the range of 0.0f to 1.0f.
    //  ref - Reference object for copy constructor.

    // Assignment operator.
    const CNSFlexHorizontalConstraint& operator=(
        CNSFlexHorizontalConstraint& ref);
    //  ref - Reference object.

    // Return a copy of me. Override for derived types.
    virtual CNSFlexHorizontalConstraint* CopyMe() const;

    // Destructor.
    virtual ~CNSFlexHorizontalConstraint();

    // Set constraint values. Override for derived types.
    virtual void Init(const CWnd* pContainerWnd, int nWidth);
    //  pContainerWnd - Pointer to window that contains controls associated
    //    with this constraint.
    //  nHeight - Height of window that contains controls associated with
    //    this constraint
    
    // Get the constraint values.
    float GetLeft() const { return m_fLeft; };
    float GetRight() const { return m_fRight; };

  // --------------------------------------------------------------------------

  protected:

    void SetLeft(float fLeft);
    void SetRight(float fRight);

  private:
    
    float m_fLeft;
    float m_fRight;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Useful constants for horizontal constraints:

extern const AFX_EXT_NSVIEWS CNSFlexHorizontalConstraint
    NSFlexHorizontallyFixed; // (0.0, 0.0)

extern const AFX_EXT_NSVIEWS CNSFlexHorizontalConstraint
    NSFlexShiftRight;        // (1.0, 1.0)

extern const AFX_EXT_NSVIEWS CNSFlexHorizontalConstraint
    NSFlexExpandRight;       // (0.0, 1.0)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// CNSFlexVerticalConstraint defines a vertical position constraint for
// a control. The class includes virtual functions for constructing more
// complex derived constraint types.

class AFX_EXT_NSVIEWS CNSFlexVerticalConstraint
{

  // --------------------------------------------------------------------------

  public:

    // Constructors.
    CNSFlexVerticalConstraint(float fTop = 0.0f, float fBottom = 0.0f);
    CNSFlexVerticalConstraint(const CNSFlexVerticalConstraint& ref);
    //  fTop - Position value associated with the top edge of a control.
    //    Value should be in the range of 0.0f to 1.0f.
    //  fBottom - Position value associated with the bottom edge of a control.
    //    Value should be in the range of 0.0f to 1.0f.
    //  ref - Reference object for copy constructor.

    // Assignment operator.
    const CNSFlexVerticalConstraint& operator=(
        CNSFlexVerticalConstraint& ref);
    //  ref - Reference object.

    // Return a copy of me. Override for derived types.
    virtual CNSFlexVerticalConstraint* CopyMe() const;

    // Destructor.
    virtual ~CNSFlexVerticalConstraint();

    // Set constraint values. Override for derived types.
    virtual void Init(const CWnd* pContainerWnd, int nHeight);
    //  pContainerWnd - Pointer to window that contains controls associated
    //    with this constraint.
    //  nHeight - Height of window that contains controls associated with
    //    this constraint
    
    // Get the constraint values.
    float GetTop() const { return m_fTop; };
    float GetBottom() const { return m_fBottom; };

  // --------------------------------------------------------------------------

  protected:

    void SetTop(float fTop);
    void SetBottom(float fBottom);

  private:
    
    float m_fTop;
    float m_fBottom;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Useful constants for vertical constraints:

extern const AFX_EXT_NSVIEWS CNSFlexVerticalConstraint
    NSFlexVerticallyFixed;   // (0.0, 0.0)

extern const AFX_EXT_NSVIEWS CNSFlexVerticalConstraint
    NSFlexShiftDown;         // (1.0, 1.0)

extern const AFX_EXT_NSVIEWS CNSFlexVerticalConstraint
    NSFlexExpandDown;        // (0.0, 1.0)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
