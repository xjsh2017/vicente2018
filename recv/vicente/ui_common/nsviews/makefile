include cpp/variables.def

PROJECT = nsviews
TYPE = library
SOURCE = dllmain_nsviews.cpp NSFlexConstraints.cpp NSFlexDialog.cpp \
  NSFlexFormView.cpp NSFlexMDIChildWnd.cpp NSFlexPropertyPage.cpp \
  NSFlexPropertySheet.cpp NSFlexPropertySheetView.cpp NSViews.rc StdAfx.cpp 
NO_COMPILE = true
TARGETS = i_nsviews.lib
# Note: the target cannot be called nsviews.dll due to the software license.
# This is not as big a deal with this only generating a static library,
# but should be remembered if this is turned back into a dll again.
DEFINITIONS += BUILD_NSVIEWS USE_HOOPLE_DLLS
USE_MFC = true

include cpp/rules.def
