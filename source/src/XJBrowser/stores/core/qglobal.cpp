/****************************************************************************
**
** Copyright (C) 1992-2005 Trolltech AS. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "StdAfx.h"

//#include "qplatformdefs.h"

//#include <qstring.h>
#include <string.h>

#include "qglobal.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>

#if defined(Q_CC_MSVC) && !defined(Q_OS_TEMP)
#include <crtdbg.h>
#endif

/*!
    \macro Q_DECLARE_TYPEINFO(Type, Flag)
    \relates <QtGlobal>

    You can use this macro to specify information about a custom type
    \a Type. With accurate type information, Qt's \l{generic
    containers} can choose appropriate storage methods and algorithms.

    \a Flag can be one of the following:

    \list
    \o \c Q_PRIMITIVE_TYPE specifies that \a Type is a POD (plain old
       data) type with no constructor or destructor.
    \o \c Q_MOVABLE_TYPE specifies that \a Type has a constructor
       and/or a destructor but can be moved in memory using \c
       memcpy().
    \o \c Q_COMPLEX_TYPE (the default) specifies that \a Type has
       constructors and/or a destructor and that it may not be moved
       in memory.
    \endlist

    Example of a "primitive" type:

    \code
        struct Point2D
        {
            int x;
            int y;
        };

        Q_DECLARE_TYPEINFO(Point2D, Q_PRIMITIVE_TYPE);
    \endcode
    
    Example of a movable type:

    \code
        class Point2D
        {
        public:
            Point2D() { data = new int[2]; }
            Point2D(const Point2D &other) { ... }
            ~Point2D() { delete[] data; }

            Point2D &operator=(const Point2D &other) { ... }

            int x() const { return data[0]; }
            int y() const { return data[1]; }

        private:
            int *data;
        };

        Q_DECLARE_TYPEINFO(Point2D, Q_MOVABLE_TYPE);
    \endcode
*/

/*!
    \class QFlag
    \brief The QFlag class is a helper data type for QFlags.

    It is equivalent to a plain \c int, except with respect to
    function overloading and type conversions. You should never need
    to use it in your applications.

    \sa QFlags
*/

/*!
    \fn QFlag::QFlag(int value)

    Constructs a QFlag object that stores the given \a value.
*/

/*!
    \fn QFlag::operator int() const

    Returns the value stored by the QFlag object.
*/

/*!
    \class QFlags
    \brief The QFlags class provides a type-safe way of storing
    OR-combinations of enum values.

    \mainclass
    \ingroup tools

    The QFlags<Enum> class is a template class, where Enum is an enum
    type. QFlags is used throughout Qt for storing combinations of
    enum values.

    The traditional C++ approach for storing OR-combinations of enum
    values is to use a \c int or \c uint variable. The inconvenient
    with that approach is that there's no type checking at all; any
    enum value can be OR'd with any other enum value and passed on to
    a function that takes a \c int or \c uint.

    Qt uses QFlags to provide type safety. For example, the
    Qt::Alignment type is simply a typedef for
    QFlags<Qt::AlignmentFlag>. QLabel::setAlignment() takes a
    Qt::Alignment parameter, which means that any combination of
    Qt::AlignmentFlag values is legal:

    \code
        label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    \endcode

    If you try to pass a value from another enum, the compiler will
    report an error.

    If you want to use QFlags for your own enum types, you must use
    the Q_DECLARE_FLAGS() and Q_DECLARE_OPERATORS_FOR_FLAGS().
    For example:

    \code
        class MyClass
        {
        public:
            enum Option {
                NoOptions = 0x0,
                ShowTabs = 0x1,
                ShowAll = 0x2,
                SqueezeBlank = 0x4
            };
            Q_DECLARE_FLAGS(Options, Option)
            ...
        };

        Q_DECLARE_OPERATORS_FOR_FLAGS(MyClass::Options)
    \endcode

    You can then use the \c MyClass::Options type to store
    combinations of \c MyClass::Option values.

    A sensible naming convension for enum types and associated QFlags
    types is to give a singular name to the enum type (e.g., \c
    Option) and a plural name to the QFlags type (e.g., \c Options).
    When a singular name is desired for the QFlags type (e.g., \c
    Alignment), you can use \c Flag as the suffix for the enum type
    (e.g., \c AlignmentFlag).

    \sa QFlag
*/

/*!
    \typedef QFlags::enum_type

    Typedef for the Enum template type.
*/

/*!
    \fn QFlags::QFlags(const QFlags &other)

    Constructs a copy of \a other.
*/

/*!
    \fn QFlags::QFlags(Enum flag)

    Constructs a QFlags object storing the given \a flag.
*/

/*!
    \fn QFlags::QFlags(Zero zero)

    Constructs a QFlags object with no flags set. \a zero must be a
    literal 0 value.
*/

/*!
    \fn QFlags::QFlags(QFlag value)

    Constructs a QFlags object initialized with the given integer \a
    value.

    The QFlag type is a helper type. By using it here instead of \c
    int, we effectively ensure that arbitrary enum values cannot be
    cast to a QFlags, whereas untyped enum values (i.e., \c int
    values) can.
*/

/*!
    \fn QFlags &QFlags::operator=(const QFlags &other)

    Assigns \a other to this object and returns a reference to this
    object.
*/

/*!
    \fn QFlags &QFlags::operator&=(int mask)

    Performs a bitwise AND operation with \a mask and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator&(), operator|=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator&=(uint mask)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator|=(QFlags other)

    Performs a bitwise OR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator|(), operator&=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator|=(Enum other)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator^=(QFlags other)

    Performs a bitwise XOR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator^(), operator&=(), operator|=()
*/

/*!
    \fn QFlags &QFlags::operator^=(Enum other)

    \overload
*/

/*!
    \fn QFlags::operator int() const

    Returns the value stored in the QFlags object as an integer.
*/

/*!
    \fn QFlags QFlags::operator|(QFlags other) const

    Returns a QFlags object containing the result of the bitwise OR
    operation on this object and \a other.

    \sa operator|=(), operator^(), operator&(), operator~()
*/

/*!
    \fn QFlags QFlags::operator|(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator^(QFlags other) const

    Returns a QFlags object containing the result of the bitwise XOR
    operation on this object and \a other.

    \sa operator^=(), operator&(), operator|(), operator~()
*/

/*!
    \fn QFlags QFlags::operator^(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(int mask) const

    Returns a QFlags object containing the result of the bitwise AND
    operation on this object and \a mask.

    \sa operator&=(), operator|(), operator^(), operator~()
*/

/*!
    \fn QFlags QFlags::operator&(uint mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(Enum mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator~() const

    Returns a QFlags object that contains the bitwise negation of
    this objec;t.

    \sa operator&(), operator|(), operator^()
*/

/*!
    \fn bool QFlags::operator!() const

    Returns true if no flag is set (i.e., if the value stored by the
    QFlags object is 0); otherwise returns false.
*/

/*!
    \macro Q_DECLARE_FLAGS(Flags, Enum)
    \relates QFlags

    The Q_DECLARE_FLAGS() macro expands to

    \code
        typedef QFlags<Enum> Flags;
    \endcode

    \a Enum is the name of an existing enum type, whereas \a Flags is
    the name of the QFlags<\e{Enum}> typedef.

    See the QFlags documentation for details.

    \sa Q_DECLARE_OPERATORS_FOR_FLAGS()
*/

/*!
    \macro Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)
    \relates QFlags

    The Q_DECLARE_OPERATORS_FOR_FLAGS() macro declares global \c
    operator|() functions for \a Flags, which is of type QFlags<T>.

    See the QFlags documentation for details.

    \sa Q_DECLARE_FLAGS()
*/

/*!
    \headerfile <QtGlobal>
    \title Global Qt Declarations

    \brief The <QtGlobal> header file provides basic declarations and is included by all other Qt headers.

    \sa <QtAlgorithms>
*/

/*!
    \typedef qreal
    \relates <QtGlobal>

    Typedef for \c double.
*/

/*! \typedef uchar
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned char}.
*/

/*! \typedef ushort
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned short}.
*/

/*! \typedef uint
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned int}.
*/

/*! \typedef ulong
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned long}.
*/

/*! \typedef qint8
    \relates <QtGlobal>

    Typedef for \c{signed char}. This type is guaranteed to be 8-bit
    on all platforms supported by Qt.
*/

/*! 
    \typedef quint8
    \relates <QtGlobal>

    Typedef for \c{unsigned char}. This type is guaranteed to
    be 8-bit on all platforms supported by Qt.
*/

/*! \typedef qint16
    \relates <QtGlobal>

    Typedef for \c{signed short}. This type is guaranteed to be
    16-bit on all platforms supported by Qt.
*/

/*! 
    \typedef quint16
    \relates <QtGlobal>

    Typedef for \c{unsigned short}. This type is guaranteed to
    be 16-bit on all platforms supported by Qt.
*/

/*! \typedef qint32
    \relates <QtGlobal>

    Typedef for \c{signed int}. This type is guaranteed to be 32-bit
    on all platforms supported by Qt.
*/

/*! 
    \typedef quint32
    \relates <QtGlobal>

    Typedef for \c{unsigned int}. This type is guaranteed to
    be 32-bit on all platforms supported by Qt.
*/

/*! \typedef qint64
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This type
    is guaranteed to be 64-bit on all platforms supported by Qt.

    Literals of that type can be created using the Q_INT64_C() macro:

    \code
        qint64 value = Q_INT64_C(932838457459459);
    \endcode

    \sa Q_INT64_C(), quint64
*/

/*! 
    \typedef quint64
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This type is guaranteed to be 64-bit on all platforms
    supported by Qt.

    Literals of that type can be created using the Q_UINT64_C()
    macro:

    \code
        quint64 value = Q_UINT64_C(932838457459459);
    \endcode

    \sa Q_UINT64_C(), qint64
*/

/*! \macro qint64 Q_INT64_C(literal)
    \relates <QtGlobal>

    Wraps the signed 64-bit integer \a literal in a
    platform-independent way. For example:

    \code
        qint64 value = Q_INT64_C(932838457459459);
    \endcode

    \sa qint64, Q_UINT64_C()
*/

/*! \macro quint64 Q_UINT64_C(literal)
    \relates <QtGlobal>

    Wraps the unsigned 64-bit integer \a literal in a
    platform-independent way. For example:

    \code
        quint64 value = Q_UINT64_C(932838457459459);
    \endcode

    \sa quint64, Q_INT64_C()
*/

/*! \typedef qlonglong
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This is
    the same as \l qint64.

    \sa Q_INT64_C(), qulonglong
*/

/*! 
    \typedef qulonglong
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This is the same as \l quint64.

    \sa Q_UINT64_C(), qlonglong
*/

/*! \fn const T &qAbs(const T &value)
    \relates <QtGlobal>

    Returns the absolute value of \a value.
*/

/*! \fn int qRound(double value)
    \relates <QtGlobal>

    Rounds \a value up to the nearest integer.
*/

/*! \fn qint64 qRound64(double value)
    \relates <QtGlobal>

    Rounds \a value up to the nearest 64-bit integer.
*/

/*! \fn const T &qMin(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the minimum of \a value1 and \a value2.

    \sa qMax(), qBound()
*/

/*! \fn const T &qMax(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the maximum of \a value1 and \a value2.

    \sa qMin(), qBound()
*/

/*! \fn const T &qBound(const T &min, const T &value, const T &max)
    \relates <QtGlobal>

    Returns \a value bounded by \a min and \a max. This is equivalent
    to qMax(\a min, qMin(\a value, \a max)).

    \sa qMin(), qMax()
*/

/*!
    \typedef Q_INT8
    \relates <QtGlobal>
    \compat

    Use \l qint8 instead.
*/

/*!
    \typedef Q_UINT8
    \relates <QtGlobal>
    \compat

    Use \l quint8 instead.
*/

/*!
    \typedef Q_INT16
    \relates <QtGlobal>
    \compat

    Use \l qint16 instead.
*/

/*!
    \typedef Q_UINT16
    \relates <QtGlobal>
    \compat

    Use \l quint16 instead.
*/

/*!
    \typedef Q_INT32
    \relates <QtGlobal>
    \compat

    Use \l qint32 instead.
*/

/*!
    \typedef Q_UINT32
    \relates <QtGlobal>
    \compat

    Use \l quint32 instead.
*/

/*!
    \typedef Q_INT64
    \relates <QtGlobal>
    \compat

    Use \l qint64 instead.
*/

/*!
    \typedef Q_UINT64
    \relates <QtGlobal>
    \compat

    Use \l quint64 instead.
*/

/*!
    \typedef Q_LLONG
    \relates <QtGlobal>
    \compat

    Use \l qint64 instead.
*/

/*!
    \typedef Q_ULLONG
    \relates <QtGlobal>
    \compat

    Use \l quint64 instead.
*/

/*!
    \typedef Q_LONG
    \relates <QtGlobal>
    \compat

    Use \c{void *} instead.
*/

/*!
    \typedef Q_ULONG
    \relates <QtGlobal>
    \compat

    Use \c{void *} instead.
*/

/*! \fn bool qSysInfo(int *wordSize, bool *bigEndian)
    \relates <QtGlobal>

    Use QSysInfo::WordSize and QSysInfo::ByteOrder instead.
*/

/*!
    \fn bool qt_winUnicode()
    \relates <QtGlobal>

    Use !(QSysInfo::WindowsVersion & QSysInfo::WV_DOS_based) instead.

    \sa QSysInfo
*/

/*!
    \fn int qWinVersion()
    \relates <QtGlobal>

    Use QSysInfo::WindowsVersion instead.

    \sa QSysInfo
*/

/*!
    \fn int qMacVersion()
    \relates <QtGlobal>

    Use QSysInfo::MacintoshVersion instead.

    \sa QSysInfo
*/

/*!
    \macro QT_VERSION
    \relates <QtGlobal>

    This macro expands a numeric value of the form 0xMMNNPP (MM =
    major, NN = minor, PP = patch) that specifies Qt's version
    number. For example, if you compile your application against Qt
    4.1.2, the QT_VERSION macro will expand to 0x040102.

    You can use QT_VERSION to use the latest Qt features where
    available. For example:

    \code
        #if QT_VERSION >= 0x040100
            QIcon icon = style()->standardIcon(QStyle::SP_TrashIcon);
        #else
            QPixmap pixmap = style()->standardPixmap(QStyle::SP_TrashIcon);
            QIcon icon(pixmap);
        #endif
    \endcode

    \sa QT_VERSION_STR, qVersion()
*/

/*!
    \macro QT_VERSION_STR
    \relates <QtGlobal>

    This macro expands to a string that specifies Qt's version number
    (for example, "4.1.2"). This is the version against which the
    application is compiled.

    \sa qVersion(), QT_VERSION
*/

/*!
    \relates <QtGlobal>

    Returns the version number of the Qt runtime as a string (for
    example, "4.1.2"). This may be a different version than the
    version against which the application was compiled.

    \sa QT_VERSION_STR
*/
#define QT_VERSION_STR   "4.0.1"
const char *qVersion()
{
    return QT_VERSION_STR;
}

bool qSharedBuild()
{
#ifdef QT_SHARED
    return true;
#else
    return false;
#endif
}

/*****************************************************************************
  System detection routines
 *****************************************************************************/

/*!
    \class QSysInfo
    \brief The QSysInfo class provides information about the system.

    \list
    \o \l WordSize specifies the size of a pointer for the platform
       on which the application is compiled.
    \o \l ByteOrder specifies whether the platform is big-endian or
       little-endian.
    \o \l WindowsVersion specifies the version of the Windows operating
       system on which the application is run (Windows only)
    \o \l MacintoshVersion specifies the version of the Macintosh
       operating system on which the application is run (Mac only).
    \endlist

    Some constants are defined only on certain platforms. You can use
    the preprocessor symbols Q_WS_WIN and Q_WS_MAC to test that
    the application is compiled under Windows or Mac.

    \sa QLibraryInfo
*/

/*!
    \variable QSysInfo::WordSize
    \brief the size in bits of a pointer for the platform on which
           the application is compiled (32 or 64)
*/

/*!
    \variable QSysInfo::WindowsVersion
    \brief the version of the Windows operating system on which the
           application is run (Windows only)
*/

/*!
    \variable QSysInfo::MacintoshVersion
    \brief the version of the Macintosh operating system on which
           the application is run (Mac only).
*/

/*!
    \enum QSysInfo::Endian

    \value BigEndian  Big-endian byte order (also called Network byte order)
    \value LittleEndian  Little-endian byte order
    \value ByteOrder  Equals BigEndian or LittleEndian, depending on
                      the platform's byte order.
*/

/*!
    \enum QSysInfo::WinVersion

    This enum provides symbolic names for the various versions of the
    Windows operating system. On Windows, the
    QSysInfo::WindowsVersion variable gives the version of the system
    on which the application is run.

    MS-DOS-based versions:

    \value WV_32s   Windows 3.1 wth Win 32s
    \value WV_95    Windows 95
    \value WV_98    Windows 98
    \value WV_Me    Windows Me

    NT-based versions:

    \value WV_NT    Windows NT
    \value WV_2000  Windows 2000
    \value WV_XP    Windows XP
    \value WV_2003  Windows XP 2003

    CE-based versions:

    \value WV_CE    Windows CE
    \value WV_CENET Windows CE .NET

    The following masks can be used for testing whether a Windows
    version is MS-DOS-based, NT-based, or CE-based:

    \value WV_DOS_based MS-DOS-based version of Windows
    \value WV_NT_based  NT-based version of Windows
    \value WV_CE_based  CE-based version of Windows

    \sa MacVersion
*/

/*!
    \enum QSysInfo::MacVersion

    This enum provides symbolic names for the various versions of the
    Macintosh operating system. On Mac, the
    QSysInfo::MacintoshVersion variable gives the version of the
    system on which the application is run.

    \value MV_9        Mac OS 9 (unsupported)
    \value MV_10_0     Mac OS X 10.0
    \value MV_10_1     Mac OS X 10.1
    \value MV_10_2     Mac OS X 10.2
    \value MV_10_3     Mac OS X 10.3
    \value MV_10_4     Mac OS X 10.4
    \value MV_Unknown  An unknown and currently unsupported platform

    \value MV_CHEETAH  Apple codename for MV_10_0
    \value MV_PUMA     Apple codename for MV_10_1
    \value MV_JAGUAR   Apple codename for MV_10_2
    \value MV_PANTHER  Apple codename for MV_10_3
    \value MV_TIGER    Apple codename for MV_10_4

    \sa WinVersion
*/

/*!
    \macro Q_OS_DARWIN
    \relates <QtGlobal>

    Defined on Darwin OS (synonym for Q_OS_MAC).
*/

/*!
    \macro Q_OS_MSDOS
    \relates <QtGlobal>

    Defined on MS-DOS and Windows.
*/

/*!
    \macro Q_OS_OS2
    \relates <QtGlobal>

    Defined on OS/2.
*/

/*!
    \macro Q_OS_OS2EMX
    \relates <QtGlobal>

    Defined on XFree86 on OS/2 (not PM).
*/

/*!
    \macro Q_OS_WIN32
    \relates <QtGlobal>

    Defined on Win32 (Windows 95/98/ME and Windows NT/2000/XP).
*/

/*!
    \macro Q_OS_CYGWIN
    \relates <QtGlobal>

    Defined on Cygwin.
*/

/*!
    \macro Q_OS_SOLARIS
    \relates <QtGlobal>

    Defined on Sun Solaris.
*/

/*!
    \macro Q_OS_HPUX
    \relates <QtGlobal>

    Defined on HP-UX.
*/

/*!
    \macro Q_OS_ULTRIX
    \relates <QtGlobal>

    Defined on DEC Ultrix.
*/

/*!
    \macro Q_OS_LINUX
    \relates <QtGlobal>

    Defined on Linux.
*/

/*!
    \macro Q_OS_FREEBSD
    \relates <QtGlobal>

    Defined on FreeBSD.
*/

/*!
    \macro Q_OS_NETBSD
    \relates <QtGlobal>

    Defined on NetBSD.
*/

/*!
    \macro Q_OS_OPENBSD
    \relates <QtGlobal>

    Defined on OpenBSD.
*/

/*!
    \macro Q_OS_BSDI
    \relates <QtGlobal>

    Defined on BSD/OS.
*/

/*!
    \macro Q_OS_IRIX
    \relates <QtGlobal>

    Defined on SGI Irix.
*/

/*!
    \macro Q_OS_OSF
    \relates <QtGlobal>

    Defined on HP Tru64 UNIX.
*/

/*!
    \macro Q_OS_SCO
    \relates <QtGlobal>

    Defined on SCO OpenServer 5.
*/

/*!
    \macro Q_OS_UNIXWARE
    \relates <QtGlobal>

    Defined on UnixWare 7, Open UNIX 8.
*/

/*!
    \macro Q_OS_AIX
    \relates <QtGlobal>

    Defined on AIX.
*/

/*!
    \macro Q_OS_HURD
    \relates <QtGlobal>

    Defined on GNU Hurd.
*/

/*!
    \macro Q_OS_DGUX
    \relates <QtGlobal>

    Defined on DG/UX.
*/

/*!
    \macro Q_OS_RELIANT
    \relates <QtGlobal>

    Defined on Reliant UNIX.
*/

/*!
    \macro Q_OS_DYNIX
    \relates <QtGlobal>

    Defined on DYNIX/ptx.
*/

/*!
    \macro Q_OS_QNX
    \relates <QtGlobal>

    Defined on QNX.
*/

/*!
    \macro Q_OS_QNX6
    \relates <QtGlobal>

    Defined on QNX RTP 6.1.
*/

/*!
    \macro Q_OS_LYNX
    \relates <QtGlobal>

    Defined on LynxOS.
*/

/*!
    \macro Q_OS_BSD4
    \relates <QtGlobal>

    Defined on Any BSD 4.4 system.
*/

/*!
    \macro Q_OS_UNIX
    \relates <QtGlobal>

    Defined on Any UNIX BSD/SYSV system.
*/

/*!
    \macro Q_CC_SYM
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Digital Mars C/C++ (used to be Symantec C++).
*/

/*!
    \macro Q_CC_MWERKS
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Metrowerks CodeWarrior.
*/

/*!
    \macro Q_CC_MSVC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Microsoft Visual C/C++, Intel C++ for Windows.
*/

/*!
    \macro Q_CC_BOR
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Borland/Turbo C++.
*/

/*!
    \macro Q_CC_WAT
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Watcom C++.
*/

/*!
    \macro Q_CC_GNU
    \relates <QtGlobal>

    Defined if the the application is compiled using
    GNU C++.
*/

/*!
    \macro Q_CC_COMEAU
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Comeau C++.
*/

/*!
    \macro Q_CC_EDG
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Edison Design Group C++.
*/

/*!
    \macro Q_CC_OC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    CenterLine C++.
*/

/*!
    \macro Q_CC_SUN
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Forte Developer, or Sun Studio C++.
*/

/*!
    \macro Q_CC_MIPS
    \relates <QtGlobal>

    Defined if the the application is compiled using
    MIPSpro C++.
*/

/*!
    \macro Q_CC_DEC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    DEC C++.
*/

/*!
    \macro Q_CC_HPACC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    HP aC++.
*/

/*!
    \macro Q_CC_USLC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    SCO OUDK and UDK.
*/

/*!
    \macro Q_CC_CDS
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Reliant C++.
*/

/*!
    \macro Q_CC_KAI
    \relates <QtGlobal>

    Defined if the the application is compiled using
    KAI C++.
*/

/*!
    \macro Q_CC_INTEL
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Intel C++ for Linux, Intel C++ for Windows.
*/

/*!
    \macro Q_CC_HIGHC
    \relates <QtGlobal>

    Defined if the the application is compiled using
    MetaWare High C/C++.
*/

/*!
    \macro Q_CC_PGI
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Portland Group C++.
*/

/*!
    \macro Q_CC_GHS
    \relates <QtGlobal>

    Defined if the the application is compiled using
    Green Hills Optimizing C++ Compilers.
*/

/*!
    \macro Q_OS_MACX
    \relates <QtGlobal>

    Defined for Mac OS X.
*/

/*!
    \macro Q_OS_MAC9
    \relates <QtGlobal>

    Defined for Mac OS 9.
*/

/*!
    \macro Q_OS_QWS
    \relates <QtGlobal>

    Defined for Qt/Embedded.
*/

/*!
    \macro Q_OS_WIN32
    \relates <QtGlobal>

    Defined for 32-bit Windows.
*/

/*!
    \macro Q_OS_X11
    \relates <QtGlobal>

    Defined for the X Window System.
*/

/*!
    \macro Q_OS_PM
    \relates <QtGlobal>
    \internal

    Defined for unsupported.
*/

/*!
    \macro Q_OS_WIN16
    \relates <QtGlobal>
    \internal

    Defined for unsupported.
*/

#if !defined(Q_BYTE_ORDER) && defined(QT_BUILD_QMAKE)
// needed to bootstrap qmake
static const unsigned int qt_one = 1;
const int QSysInfo::ByteOrder = ((*((unsigned char *) &qt_one) == 0) ? BigEndian : LittleEndian);
#endif

#if !defined(QWS) && defined(Q_OS_MAC)

#include <private/qcore_mac_p.h>
#include "qnamespace.h"

// This function has descended from Apple Source Code (FSpLocationFromFullPath),
// but changes have been made. [Creates a minimal alias from the full pathname]
Q_CORE_EXPORT OSErr qt_mac_create_fsspec(const QString &file, FSSpec *spec)
{
    FSRef fref;
    QByteArray utfs = file.toUtf8();
    OSErr ret = FSPathMakeRef((const UInt8 *)utfs.data(), &fref, NULL);
    if(ret == noErr)
        ret = FSGetCatalogInfo(&fref, kFSCatInfoNone, NULL, NULL, spec, NULL);
    return ret;
}

Q_CORE_EXPORT void qt_mac_to_pascal_string(QString s, Str255 str, TextEncoding encoding=0, int len=-1)
{
    if(len == -1)
        len = s.length();
#if 0
    UnicodeMapping mapping;
    mapping.unicodeEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault,
                                                 kTextEncodingDefaultVariant,
                                                 kUnicode16BitFormat);
    mapping.otherEncoding = (encoding ? encoding : );
    mapping.mappingVersion = kUnicodeUseLatestMapping;

    UnicodeToTextInfo info;
    OSStatus err = CreateUnicodeToTextInfo(&mapping, &info);
    if(err != noErr) {
        qDebug("Qt: internal: Unable to create pascal string '%s'::%d [%ld]",
               s.left(len).latin1(), (int)encoding, err);
        return;
    }
    const int unilen = len * 2;
    const UniChar *unibuf = (UniChar *)s.unicode();
    ConvertFromUnicodeToPString(info, unilen, unibuf, str);
    DisposeUnicodeToTextInfo(&info);
#else
    Q_UNUSED(encoding);
    CFStringGetPascalString(QCFString(s), str, 256, CFStringGetSystemEncoding());
#endif
}

Q_CORE_EXPORT QString qt_mac_from_pascal_string(const Str255 pstr) {
    return QCFString(CFStringCreateWithPascalString(0, pstr, CFStringGetSystemEncoding()));
}

static QSysInfo::MacVersion macVersion()
{
    long gestalt_version;
    if (Gestalt(gestaltSystemVersion, &gestalt_version) == noErr) {
        if (gestalt_version >= 0x1040 && gestalt_version < 0x1050)
            return QSysInfo::MV_10_4;
        else if (gestalt_version >= 0x1030 && gestalt_version < 0x1040)
            return QSysInfo::MV_10_3;
        else if (gestalt_version >= 0x1020 && gestalt_version < 0x1030)
            return QSysInfo::MV_10_2;
        else if (gestalt_version >= 0x1010 && gestalt_version < 0x1020)
            return QSysInfo::MV_10_1;
        else if (gestalt_version >= 0x1000 && gestalt_version < 0x1010)
            return QSysInfo::MV_10_0;
    }
    return QSysInfo::MV_Unknown;
}
const QSysInfo::MacVersion QSysInfo::MacintoshVersion = macVersion();
#elif defined(Q_OS_WIN32) || defined(Q_OS_CYGWIN) || defined(Q_OS_TEMP)

//#include "qt_windows.h"

#endif


/*!
    \macro void Q_ASSERT(bool test)
    \relates <QtGlobal>

    Prints a warning message containing the source code file name and
    line number if \a test is false.

    Q_ASSERT() is useful for testing pre- and post-conditions
    during development. It does nothing if \c QT_NO_DEBUG was defined
    during compilation.

    Example:

    \code
        // File: div.cpp

        #include <QtGlobal>

        int divide(int a, int b)
        {
            Q_ASSERT(b != 0);
            return a / b;
        }
    \endcode

    If \c b is zero, the Q_ASSERT statement will output the following
    message using the qFatal() function:

    \code
        ASSERT: "b == 0" in file div.cpp, line 7
    \endcode

    \sa Q_ASSERT_X(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_ASSERT_X(bool test, const char *where, const char *what)
    \relates <QtGlobal>

    Prints the message \a what together with the location \a where,
    the source file name and line number if \a test is false.

    Q_ASSERT_X is useful for testing pre- and post-conditions during
    development. It does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \code
        // File: div.cpp

        #include <QtGlobal>

        int divide(int a, int b)
        {
            Q_ASSERT_X(b != 0, "divide", "division by zero");
            return a / b;
        }
    \endcode

    If \c b is zero, the Q_ASSERT_X statement will output the following
    message using the qFatal() function:

    \code
        ASSERT failure in divide: "division by zero", file div.cpp, line 7
    \endcode

    \sa Q_ASSERT(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_CHECK_PTR(void *p)
    \relates <QtGlobal>

    If \a p is 0, prints a warning message containing the source code file
    name and line number, saying that the program ran out of memory.

    Q_CHECK_PTR does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \code
        int *a;

        Q_CHECK_PTR(a = new int[80]);   // WRONG!

        a = new (nothrow) int[80];      // Right
        Q_CHECK_PTR(a);
    \endcode

    \sa qWarning(), {Debugging Techniques}
*/


/*
  The Q_CHECK_PTR macro calls this function if an allocation check
  fails.
*/
void qt_check_pointer(const char *n, int l)
{
    //qWarning("In file %s, line %d: Out of memory", n, l);
}

/*
  The Q_ASSERT macro calls this this function when the test fails.
*/
void qt_assert(const char *assertion, const char *file, int line)
{
    //qFatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

/*
  The Q_ASSERT_X macro calls this this function when the test fails.
*/
void qt_assert_x(const char *where, const char *what, const char *file, int line)
{
    //qFatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}

unsigned int qt_int_sqrt(unsigned int n)
{
    // n must be in the range 0...UINT_MAX/2-1
    if (n >= (UINT_MAX>>2)) {
        unsigned int r = 2 * qt_int_sqrt(n / 4);
        unsigned int r2 = r + 1;
        return (n >= r2 * r2) ? r2 : r;
    }
    uint h, p= 0, q= 1, r= n;
    while (q <= n)
        q <<= 2;
    while (q != 1) {
        q >>= 2;
        h= p + q;
        p >>= 1;
        if (r >= h) {
            p += q;
            r -= h;
        }
    }
    return p;
}

#if defined(qMemCopy)
#  undef qMemCopy
#endif
#if defined(qMemSet)
#  undef qMemSet
#endif

void *qMalloc(size_t size) { return ::malloc(size); }
void qFree(void *ptr) { ::free(ptr); }
void *qRealloc(void *ptr, size_t size) { return ::realloc(ptr, size); }
void *qMemCopy(void *dest, const void *src, size_t n) { return memcpy(dest, src, n); }
void *qMemSet(void *dest, int c, size_t n) { return memset(dest, c, n); }


static QtMsgHandler handler = 0;                // pointer to debug handler
static const int QT_BUFFER_LENGTH = 8192;       // internal buffer length

#ifdef Q_CC_MWERKS
#include <CoreServices/CoreServices.h>
extern bool qt_is_gui_used;
static void mac_default_handler(const char *msg)
{
    if (qt_is_gui_used) {
        Str255 pmsg;
        qt_mac_to_pascal_string(msg, pmsg);
        DebugStr(pmsg);
    } else {
        fprintf(stderr, msg);
    }
}
#endif // Q_CC_MWERKS



QtMsgHandler qInstallMsgHandler(QtMsgHandler h)
{
    QtMsgHandler old = handler;
    handler = h;
    return old;
}

void qWarning(const char *, ...)
{

}


#if defined(QT3_SUPPORT) && !defined(QT_NO_SETTINGS)
#include <qlibraryinfo.h>
static const char *qInstallLocation(QLibraryInfo::LibraryLocation loc)
{
    static QByteArray ret;
    ret = QLibraryInfo::location(loc).toLatin1();
    return ret.constData();
}
const char *qInstallPath()
{
    return qInstallLocation(QLibraryInfo::PrefixPath);
}
const char *qInstallPathDocs()
{
    return qInstallLocation(QLibraryInfo::DocumentationPath);
}
const char *qInstallPathHeaders()
{
    return qInstallLocation(QLibraryInfo::HeadersPath);
}
const char *qInstallPathLibs()
{
    return qInstallLocation(QLibraryInfo::LibrariesPath);
}
const char *qInstallPathBins()
{
    return qInstallLocation(QLibraryInfo::BinariesPath);
}
const char *qInstallPathPlugins()
{
    return qInstallLocation(QLibraryInfo::PluginsPath);
}
const char *qInstallPathData()
{
    return qInstallLocation(QLibraryInfo::DataPath);
}
const char *qInstallPathTranslations()
{
    return qInstallLocation(QLibraryInfo::TranslationsPath);
}
const char *qInstallPathSysconf()
{
    return qInstallLocation(QLibraryInfo::SettingsPath);
}
#endif
