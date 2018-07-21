# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=NSViews - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to NSViews - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "NSViews - Win32 Release" && "$(CFG)" !=\
 "NSViews - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "NSViews.mak" CFG="NSViews - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NSViews - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NSViews - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "NSViews - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "NSViews - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\NSViews.dll"

CLEAN : 
	-@erase "$(INTDIR)\NSFlexConstraints.obj"
	-@erase "$(INTDIR)\NSFlexDialog.obj"
	-@erase "$(INTDIR)\NSFlexFormView.obj"
	-@erase "$(INTDIR)\NSFlexMDIChildWnd.obj"
	-@erase "$(INTDIR)\NSFlexPropertyPage.obj"
	-@erase "$(INTDIR)\NSFlexPropertySheet.obj"
	-@erase "$(INTDIR)\NSFlexPropertySheetView.obj"
	-@erase "$(INTDIR)\NSViews.obj"
	-@erase "$(INTDIR)\NSViews.pch"
	-@erase "$(INTDIR)\NSViews.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\NSViews.dll"
	-@erase "$(OUTDIR)\NSViews.exp"
	-@erase "$(OUTDIR)\NSViews.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS"\
 /Fp"$(INTDIR)/NSViews.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/NSViews.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/NSViews.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/NSViews.pdb" /machine:I386 /def:".\NSViews.def"\
 /out:"$(OUTDIR)/NSViews.dll" /implib:"$(OUTDIR)/NSViews.lib" 
DEF_FILE= \
	".\NSViews.def"
LINK32_OBJS= \
	"$(INTDIR)\NSFlexConstraints.obj" \
	"$(INTDIR)\NSFlexDialog.obj" \
	"$(INTDIR)\NSFlexFormView.obj" \
	"$(INTDIR)\NSFlexMDIChildWnd.obj" \
	"$(INTDIR)\NSFlexPropertyPage.obj" \
	"$(INTDIR)\NSFlexPropertySheet.obj" \
	"$(INTDIR)\NSFlexPropertySheetView.obj" \
	"$(INTDIR)\NSViews.obj" \
	"$(INTDIR)\NSViews.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\NSViews.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\NSViews.dll" "$(OUTDIR)\NSViews.bsc"

CLEAN : 
	-@erase "$(INTDIR)\NSFlexConstraints.obj"
	-@erase "$(INTDIR)\NSFlexConstraints.sbr"
	-@erase "$(INTDIR)\NSFlexDialog.obj"
	-@erase "$(INTDIR)\NSFlexDialog.sbr"
	-@erase "$(INTDIR)\NSFlexFormView.obj"
	-@erase "$(INTDIR)\NSFlexFormView.sbr"
	-@erase "$(INTDIR)\NSFlexMDIChildWnd.obj"
	-@erase "$(INTDIR)\NSFlexMDIChildWnd.sbr"
	-@erase "$(INTDIR)\NSFlexPropertyPage.obj"
	-@erase "$(INTDIR)\NSFlexPropertyPage.sbr"
	-@erase "$(INTDIR)\NSFlexPropertySheet.obj"
	-@erase "$(INTDIR)\NSFlexPropertySheet.sbr"
	-@erase "$(INTDIR)\NSFlexPropertySheetView.obj"
	-@erase "$(INTDIR)\NSFlexPropertySheetView.sbr"
	-@erase "$(INTDIR)\NSViews.obj"
	-@erase "$(INTDIR)\NSViews.pch"
	-@erase "$(INTDIR)\NSViews.res"
	-@erase "$(INTDIR)\NSViews.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\NSViews.bsc"
	-@erase "$(OUTDIR)\NSViews.dll"
	-@erase "$(OUTDIR)\NSViews.exp"
	-@erase "$(OUTDIR)\NSViews.ilk"
	-@erase "$(OUTDIR)\NSViews.lib"
	-@erase "$(OUTDIR)\NSViews.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/NSViews.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/NSViews.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/NSViews.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\NSFlexConstraints.sbr" \
	"$(INTDIR)\NSFlexDialog.sbr" \
	"$(INTDIR)\NSFlexFormView.sbr" \
	"$(INTDIR)\NSFlexMDIChildWnd.sbr" \
	"$(INTDIR)\NSFlexPropertyPage.sbr" \
	"$(INTDIR)\NSFlexPropertySheet.sbr" \
	"$(INTDIR)\NSFlexPropertySheetView.sbr" \
	"$(INTDIR)\NSViews.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\NSViews.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/NSViews.pdb" /debug /machine:I386 /def:".\NSViews.def"\
 /out:"$(OUTDIR)/NSViews.dll" /implib:"$(OUTDIR)/NSViews.lib" 
DEF_FILE= \
	".\NSViews.def"
LINK32_OBJS= \
	"$(INTDIR)\NSFlexConstraints.obj" \
	"$(INTDIR)\NSFlexDialog.obj" \
	"$(INTDIR)\NSFlexFormView.obj" \
	"$(INTDIR)\NSFlexMDIChildWnd.obj" \
	"$(INTDIR)\NSFlexPropertyPage.obj" \
	"$(INTDIR)\NSFlexPropertySheet.obj" \
	"$(INTDIR)\NSFlexPropertySheetView.obj" \
	"$(INTDIR)\NSViews.obj" \
	"$(INTDIR)\NSViews.res" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\NSViews.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "NSViews - Win32 Release"
# Name "NSViews - Win32 Debug"

!IF  "$(CFG)" == "NSViews - Win32 Release"

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\NSViews.cpp
DEP_CPP_NSVIE=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSViews.obj" : $(SOURCE) $(DEP_CPP_NSVIE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSViews.obj" : $(SOURCE) $(DEP_CPP_NSVIE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSViews.sbr" : $(SOURCE) $(DEP_CPP_NSVIE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSViews.def

!IF  "$(CFG)" == "NSViews - Win32 Release"

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS"\
 /Fp"$(INTDIR)/NSViews.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\NSViews.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "BUILD_NSVIEWS"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/NSViews.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\NSViews.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSViews.rc
DEP_RSC_NSVIEW=\
	".\res\NSViews.rc2"\
	

"$(INTDIR)\NSViews.res" : $(SOURCE) $(DEP_RSC_NSVIEW) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexPropertySheet.cpp
DEP_CPP_NSFLE=\
	".\NSFlexConstraintList.h"\
	".\NSFlexConstraints.h"\
	".\NSFlexMDIChildWnd.h"\
	".\NSFlexPropertySheet.h"\
	".\NSViewsAfxExt.h"\
	".\NSViewsMessages.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexPropertySheet.obj" : $(SOURCE) $(DEP_CPP_NSFLE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexPropertySheet.obj" : $(SOURCE) $(DEP_CPP_NSFLE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexPropertySheet.sbr" : $(SOURCE) $(DEP_CPP_NSFLE) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexDialog.cpp
DEP_CPP_NSFLEX=\
	".\NSFlexConstraintList.h"\
	".\NSFlexConstraints.h"\
	".\NSFlexDialog.h"\
	".\NSViewsAfxExt.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexDialog.obj" : $(SOURCE) $(DEP_CPP_NSFLEX) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexDialog.obj" : $(SOURCE) $(DEP_CPP_NSFLEX) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexDialog.sbr" : $(SOURCE) $(DEP_CPP_NSFLEX) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexPropertyPage.cpp
DEP_CPP_NSFLEXP=\
	".\NSFlexConstraintList.h"\
	".\NSFlexConstraints.h"\
	".\NSFlexPropertyPage.h"\
	".\NSFlexPropertySheet.h"\
	".\NSViewsAfxExt.h"\
	".\NSViewsMessages.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexPropertyPage.obj" : $(SOURCE) $(DEP_CPP_NSFLEXP) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexPropertyPage.obj" : $(SOURCE) $(DEP_CPP_NSFLEXP) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexPropertyPage.sbr" : $(SOURCE) $(DEP_CPP_NSFLEXP) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexMDIChildWnd.cpp
DEP_CPP_NSFLEXM=\
	".\NSFlexMDIChildWnd.h"\
	".\NSViewsAfxExt.h"\
	".\NSViewsMessages.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_NSFLEXM) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_NSFLEXM) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexMDIChildWnd.sbr" : $(SOURCE) $(DEP_CPP_NSFLEXM) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexFormView.cpp
DEP_CPP_NSFLEXF=\
	".\NSFlexConstraintList.h"\
	".\NSFlexConstraints.h"\
	".\NSFlexFormView.h"\
	".\NSFlexMDIChildWnd.h"\
	".\NSViewsAfxExt.h"\
	".\NSViewsMessages.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexFormView.obj" : $(SOURCE) $(DEP_CPP_NSFLEXF) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexFormView.obj" : $(SOURCE) $(DEP_CPP_NSFLEXF) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexFormView.sbr" : $(SOURCE) $(DEP_CPP_NSFLEXF) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexPropertySheetView.cpp
DEP_CPP_NSFLEXPR=\
	".\NSFlexConstraints.h"\
	".\NSFlexMDIChildWnd.h"\
	".\NSFlexPropertyPage.h"\
	".\NSFlexPropertySheet.h"\
	".\NSFlexPropertySheetView.h"\
	".\NSViewsAfxExt.h"\
	".\NSViewsMessages.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexPropertySheetView.obj" : $(SOURCE) $(DEP_CPP_NSFLEXPR)\
 "$(INTDIR)" "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexPropertySheetView.obj" : $(SOURCE) $(DEP_CPP_NSFLEXPR)\
 "$(INTDIR)" "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexPropertySheetView.sbr" : $(SOURCE) $(DEP_CPP_NSFLEXPR)\
 "$(INTDIR)" "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSFlexConstraints.cpp
DEP_CPP_NSFLEXC=\
	".\NSFlexConstraintList.h"\
	".\NSFlexConstraints.h"\
	".\NSViewsAfxExt.h"\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "NSViews - Win32 Release"


"$(INTDIR)\NSFlexConstraints.obj" : $(SOURCE) $(DEP_CPP_NSFLEXC) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"


"$(INTDIR)\NSFlexConstraints.obj" : $(SOURCE) $(DEP_CPP_NSFLEXC) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"

"$(INTDIR)\NSFlexConstraints.sbr" : $(SOURCE) $(DEP_CPP_NSFLEXC) "$(INTDIR)"\
 "$(INTDIR)\NSViews.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\README.txt

!IF  "$(CFG)" == "NSViews - Win32 Release"

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LICENSE.txt

!IF  "$(CFG)" == "NSViews - Win32 Release"

!ELSEIF  "$(CFG)" == "NSViews - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
