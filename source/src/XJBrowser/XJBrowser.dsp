# Microsoft Developer Studio Project File - Name="XJBrowser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XJBrowser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XJBrowser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XJBrowser.mak" CFG="XJBrowser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XJBrowser - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XJBrowser - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XJBrowser - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../XJBrowser" /I "./pages" /I "./pages/pt" /I "./stores" /I "./stores/core" /I "./stores/table" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OS_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib winmm.lib Mpr.lib Psapi.lib ../../common_open_source/cairo_bin/cairo.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\bin\XJBrowser\XJBrowser.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "XJBrowser - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../XJBrowser" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OS_WINDOWS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib winmm.lib Mpr.lib Psapi.lib ../../common_open_source/cairo_bin/cairo.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\bin\XJBrowser\XJBrowserD.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "XJBrowser - Win32 Release"
# Name "XJBrowser - Win32 Debug"
# Begin Group "Sources"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "pages"

# PROP Default_Filter ""
# Begin Group "pt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pages\pt\DlgDataCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgOperHis.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgTagOutSet.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgValidateUser.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\HangoutWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\PTSetProgView.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\pt\PTSetStateItem.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\pages\PTSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\pages\PTSoftBoard.cpp
# End Source File
# End Group
# Begin Group "stores"

# PROP Default_Filter ""
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\stores\core\qbytearray.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\core\qbytearraymatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\core\qbytearraymatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\core\qcardbase.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\core\qglobal.cpp
# End Source File
# End Group
# Begin Group "table"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\stores\table\qmemtable.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\table\qptsetstatetable.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\stores\XJRootStore.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\XJStoreDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\XJTagOutStore.cpp
# End Source File
# Begin Source File

SOURCE=.\stores\XJUserStore.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlgEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ACResultSel.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoCallFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoCallResult.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoResultFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchDetailSettingView.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchDetailView.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchSel.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchSelectView.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchSettingView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CJControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CJDockBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CJDockContext.cpp
# End Source File
# Begin Source File

SOURCE=.\CJFlatButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CJFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CJMDIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CJMiniDockFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CJSizeDockBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CJTabctrlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CommFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CommWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\CompareComboBox\CompareComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ContextDockBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CrossDef.cpp
# End Source File
# Begin Source File

SOURCE=.\CrossFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CrossPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomStyleSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DataEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\DBEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\ddvue.cpp
# End Source File
# Begin Source File

SOURCE=.\Department.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceList.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceView.cpp
# End Source File
# Begin Source File

SOURCE=.\DisDevFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_DevRunStatusUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddOsc.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgApp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoCallStop.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDetailStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDraft.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEventProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEventSignal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaultEventAdd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaultReport.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGMarked.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOCSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtnote.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelFault.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSettingDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSignalList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStationProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStationSvg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStatusHis.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgValidateID.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWork.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DownLoadView.cpp
# End Source File
# Begin Source File

SOURCE=.\DownOscFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DownOscView.cpp
# End Source File
# Begin Source File

SOURCE=.\DraftDocument.cpp
# End Source File
# Begin Source File

SOURCE=.\DraftFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DraftList.cpp
# End Source File
# Begin Source File

SOURCE=.\DraftView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\EntityContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\excel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelOp.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\FaultReport.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\globalfun.cpp
# End Source File
# Begin Source File

SOURCE=.\HideHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HisValueMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCellEdit.h
# End Source File
# Begin Source File

SOURCE=.\ListCellListSel.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Lock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LogView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualFaultShorten.cpp
# End Source File
# Begin Source File

SOURCE=.\MDITabs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\MemField.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\MemRow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\MemSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\MessageLog.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgView.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OXHookWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OXItemTip.cpp
# End Source File
# Begin Source File

SOURCE=.\OXItemTipWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\OXSplashWnd\OXSplashWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\OXSplashWnd\OXSplashWndDIB.cpp
# End Source File
# Begin Source File

SOURCE=.\OXTreeCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\OXTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OXTreeEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\OXTreeHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\OXTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSysBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintCap.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PTAction.cpp
# End Source File
# Begin Source File

SOURCE=.\PTAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\PTAnalog.cpp
# End Source File
# Begin Source File

SOURCE=.\PTDigital.cpp
# End Source File
# Begin Source File

SOURCE=.\PTDistance.cpp
# End Source File
# Begin Source File

SOURCE=.\PTFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PTGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\PTHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\PTNote.cpp
# End Source File
# Begin Source File

SOURCE=.\PTOSC.cpp
# End Source File
# Begin Source File

SOURCE=.\PTOSCList.cpp
# End Source File
# Begin Source File

SOURCE=.\PTSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\PushMsgProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\RemindObj.cpp
# End Source File
# Begin Source File

SOURCE=.\RemindView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\SoeFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\StationInfoFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SttpEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Subclass.cpp
# End Source File
# Begin Source File

SOURCE=.\SysAlarmFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageInfoCase.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSettingPagePTProp.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSettingSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\ThemeHelperST.cpp
# End Source File
# Begin Source File

SOURCE=.\TLCDragWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TLCDropWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TLHDragWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TLHDropWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTipWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListColumnInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListComboCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListDC.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListResource.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListStaticCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeListTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewAction.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewActionDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewAlarmDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewComm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewCommMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewCross.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewCrossDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewDI.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewDistance.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewFault.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewFaultDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewFaultQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewFaultSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewOSC.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewPTComm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewPTRun.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSOE.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSOEDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewStaInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewStationInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSysAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSysAlarmDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveRecFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveRecGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveRecOSC.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveRecSetting.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\DLLEncapsulation\WaveShapeEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\XJBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\XJBrowser.rc
# End Source File
# Begin Source File

SOURCE=.\XJBrowser_update_history.cpp
# End Source File
# Begin Source File

SOURCE=.\XJBrowserDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\XJBrowserView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XJErrorID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XJFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\xjlib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XJLock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\XJString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\XPStyleButtonST.cpp
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "pages "

# PROP Default_Filter ".h"
# Begin Group "pt "

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\pages\pt\DlgDataCheck.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgOperHis.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgTagOutSet.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\DlgValidateUser.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\HangoutWnd.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\PTSetProgView.h
# End Source File
# Begin Source File

SOURCE=.\pages\pt\PTSetStateItem.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\pages\PTSetting.h
# End Source File
# Begin Source File

SOURCE=.\pages\PTSoftBoard.h
# End Source File
# End Group
# Begin Group "stores "

# PROP Default_Filter ""
# Begin Group "core "

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\stores\core\qbytearray.h
# End Source File
# Begin Source File

SOURCE=.\stores\core\qbytearraymatcher.h
# End Source File
# Begin Source File

SOURCE=.\stores\core\qbytearraymatrix.h
# End Source File
# Begin Source File

SOURCE=.\stores\core\qcardbase.h
# End Source File
# Begin Source File

SOURCE=.\stores\core\qglobal.h
# End Source File
# End Group
# Begin Group "table "

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\stores\table\qmemtable.h
# End Source File
# Begin Source File

SOURCE=.\stores\table\qptsetstatetable.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\stores\XJRootStore.h
# End Source File
# Begin Source File

SOURCE=.\stores\XJStoreDefine.h
# End Source File
# Begin Source File

SOURCE=.\stores\XJTagOutStore.h
# End Source File
# Begin Source File

SOURCE=.\stores\XJUserStore.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlgEx.h
# End Source File
# Begin Source File

SOURCE=.\ACResultSel.h
# End Source File
# Begin Source File

SOURCE=.\ActionFrame.h
# End Source File
# Begin Source File

SOURCE=.\AlarmFrame.h
# End Source File
# Begin Source File

SOURCE=.\AutoCallFrame.h
# End Source File
# Begin Source File

SOURCE=.\AutoCallResult.h
# End Source File
# Begin Source File

SOURCE=.\AutoResultFrame.h
# End Source File
# Begin Source File

SOURCE=.\BatchDetailSettingView.h
# End Source File
# Begin Source File

SOURCE=.\BatchDetailView.h
# End Source File
# Begin Source File

SOURCE=.\BatchFrame.h
# End Source File
# Begin Source File

SOURCE=.\BatchSel.h
# End Source File
# Begin Source File

SOURCE=.\BatchSelectView.h
# End Source File
# Begin Source File

SOURCE=.\BatchSettingView.h
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\BCMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CJ60Lib.h
# End Source File
# Begin Source File

SOURCE=.\CJControlBar.h
# End Source File
# Begin Source File

SOURCE=.\CJDockBar.h
# End Source File
# Begin Source File

SOURCE=.\CJDockContext.h
# End Source File
# Begin Source File

SOURCE=.\CJFlatButton.h
# End Source File
# Begin Source File

SOURCE=.\CJFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\CJMDIFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\CJMiniDockFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\CJSizeDockBar.h
# End Source File
# Begin Source File

SOURCE=.\CJTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=.\CommFrame.h
# End Source File
# Begin Source File

SOURCE=.\CommonFile.h
# End Source File
# Begin Source File

SOURCE=.\CommWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\CompareComboBox\CompareComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ContextDockBar.h
# End Source File
# Begin Source File

SOURCE=.\CoolMenu.h
# End Source File
# Begin Source File

SOURCE=.\CrossDef.h
# End Source File
# Begin Source File

SOURCE=.\CrossFrame.h
# End Source File
# Begin Source File

SOURCE=.\CrossPoint.h
# End Source File
# Begin Source File

SOURCE=.\CtrlButton.h
# End Source File
# Begin Source File

SOURCE=.\CustomStyleSetting.h
# End Source File
# Begin Source File

SOURCE=.\DataEngine.h
# End Source File
# Begin Source File

SOURCE=.\DBEngine.h
# End Source File
# Begin Source File

SOURCE=.\ddvue.h
# End Source File
# Begin Source File

SOURCE=..\..\common\define.h
# End Source File
# Begin Source File

SOURCE=.\Department.h
# End Source File
# Begin Source File

SOURCE=.\DeviceList.h
# End Source File
# Begin Source File

SOURCE=.\DeviceObj.h
# End Source File
# Begin Source File

SOURCE=.\DeviceView.h
# End Source File
# Begin Source File

SOURCE=.\DisDevFrame.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_DevRunStatusUpdate.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddOsc.h
# End Source File
# Begin Source File

SOURCE=.\DlgApp.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoCallStop.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgCheck.h
# End Source File
# Begin Source File

SOURCE=.\DlgDetailStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgDraft.h
# End Source File
# Begin Source File

SOURCE=.\DlgEventProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgEventSignal.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportProcess.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaultEventAdd.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaultReport.h
# End Source File
# Begin Source File

SOURCE=.\DLGMarked.h
# End Source File
# Begin Source File

SOURCE=.\DlgOCSet.h
# End Source File
# Begin Source File

SOURCE=.\pages\DlgOperHis.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtnote.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelFault.h
# End Source File
# Begin Source File

SOURCE=.\DlgSettingDetail.h
# End Source File
# Begin Source File

SOURCE=.\DlgSignalList.h
# End Source File
# Begin Source File

SOURCE=.\DlgStationProp.h
# End Source File
# Begin Source File

SOURCE=.\DlgStationSvg.h
# End Source File
# Begin Source File

SOURCE=.\DlgStatusHis.h
# End Source File
# Begin Source File

SOURCE=.\DlgValidateID.h
# End Source File
# Begin Source File

SOURCE=.\DlgWork.h
# End Source File
# Begin Source File

SOURCE=.\DownloadFrame.h
# End Source File
# Begin Source File

SOURCE=.\DownLoadView.h
# End Source File
# Begin Source File

SOURCE=.\DownOscFrame.h
# End Source File
# Begin Source File

SOURCE=.\DownOscView.h
# End Source File
# Begin Source File

SOURCE=.\DraftDocument.h
# End Source File
# Begin Source File

SOURCE=.\DraftFrame.h
# End Source File
# Begin Source File

SOURCE=.\DraftList.h
# End Source File
# Begin Source File

SOURCE=.\DraftView.h
# End Source File
# Begin Source File

SOURCE=..\..\common\EntityContainer.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# Begin Source File

SOURCE=.\excel.h
# End Source File
# Begin Source File

SOURCE=.\ExcelOp.h
# End Source File
# Begin Source File

SOURCE=.\FaultEvent.h
# End Source File
# Begin Source File

SOURCE=.\FaultFrame.h
# End Source File
# Begin Source File

SOURCE=.\FaultReport.h
# End Source File
# Begin Source File

SOURCE=.\GeoView.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFunc.h
# End Source File
# Begin Source File

SOURCE=.\HideHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\HisValueMgr.h
# End Source File
# Begin Source File

SOURCE=.\ListCellEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCellListSel.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\Lock.h
# End Source File
# Begin Source File

SOURCE=..\..\common\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\LogMgr.h
# End Source File
# Begin Source File

SOURCE=.\LogView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManualFaultShorten.h
# End Source File
# Begin Source File

SOURCE=.\MDITabs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\MemField.h
# End Source File
# Begin Source File

SOURCE=..\..\common\MemRow.h
# End Source File
# Begin Source File

SOURCE=..\..\common\MemSet.h
# End Source File
# Begin Source File

SOURCE=..\..\common\MessageLog.h
# End Source File
# Begin Source File

SOURCE=.\MsgView.h
# End Source File
# Begin Source File

SOURCE=.\MtVerify.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OXBaseSubclassedComboBox.h
# End Source File
# Begin Source File

SOURCE=.\OXDllExt.h
# End Source File
# Begin Source File

SOURCE=.\OXHookWnd.h
# End Source File
# Begin Source File

SOURCE=.\OXItemTip.h
# End Source File
# Begin Source File

SOURCE=.\OXItemTipWnd.h
# End Source File
# Begin Source File

SOURCE=.\OXMainRes.h
# End Source File
# Begin Source File

SOURCE=.\OXTreeCombo.h
# End Source File
# Begin Source File

SOURCE=.\OXTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OXTreeEdit.h
# End Source File
# Begin Source File

SOURCE=.\OXTreeHeader.h
# End Source File
# Begin Source File

SOURCE=.\OXTreeItem.h
# End Source File
# Begin Source File

SOURCE=.\PageSysBrowser.h
# End Source File
# Begin Source File

SOURCE=.\PaintCap.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\PrintListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Psapi.h
# End Source File
# Begin Source File

SOURCE=.\PTAction.h
# End Source File
# Begin Source File

SOURCE=.\PTAlarm.h
# End Source File
# Begin Source File

SOURCE=.\PTAnalog.h
# End Source File
# Begin Source File

SOURCE=.\PTDigital.h
# End Source File
# Begin Source File

SOURCE=.\PTDistance.h
# End Source File
# Begin Source File

SOURCE=.\PTFrame.h
# End Source File
# Begin Source File

SOURCE=.\PTGeneral.h
# End Source File
# Begin Source File

SOURCE=.\PTHistory.h
# End Source File
# Begin Source File

SOURCE=.\PTNote.h
# End Source File
# Begin Source File

SOURCE=.\PTOSC.h
# End Source File
# Begin Source File

SOURCE=.\PTOSCList.h
# End Source File
# Begin Source File

SOURCE=.\PTSheet.h
# End Source File
# Begin Source File

SOURCE=.\PushMsgProcess.h
# End Source File
# Begin Source File

SOURCE=.\RemindObj.h
# End Source File
# Begin Source File

SOURCE=.\RemindView.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScrollViewEx.h
# End Source File
# Begin Source File

SOURCE=.\SoeFrame.h
# End Source File
# Begin Source File

SOURCE=.\StationInfoFrame.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\common\SttpDefine.h
# End Source File
# Begin Source File

SOURCE=.\SttpEngine.h
# End Source File
# Begin Source File

SOURCE=.\Subclass.h
# End Source File
# Begin Source File

SOURCE=.\SysAlarmFrame.h
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageGeneral.h
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageInfoCase.h
# End Source File
# Begin Source File

SOURCE=.\SysSettingPagePTProp.h
# End Source File
# Begin Source File

SOURCE=.\SysSettingPageStatus.h
# End Source File
# Begin Source File

SOURCE=.\SysSettingSheet.h
# End Source File
# Begin Source File

SOURCE=.\TabView.h
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\ThemeHelperST.h
# End Source File
# Begin Source File

SOURCE=.\TLCDragWnd.h
# End Source File
# Begin Source File

SOURCE=.\TLCDropWnd.h
# End Source File
# Begin Source File

SOURCE=.\TLHDragWnd.h
# End Source File
# Begin Source File

SOURCE=.\TLHDropWnd.h
# End Source File
# Begin Source File

SOURCE=.\ToolTipWnd.h
# End Source File
# Begin Source File

SOURCE=.\TreeListColumnInfo.h
# End Source File
# Begin Source File

SOURCE=.\TreeListComboCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListDC.h
# End Source File
# Begin Source File

SOURCE=.\TreeListEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListItem.h
# End Source File
# Begin Source File

SOURCE=.\TreeListResource.h
# End Source File
# Begin Source File

SOURCE=.\TreeListStaticCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TreeListTipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\ViewAction.h
# End Source File
# Begin Source File

SOURCE=.\ViewActionDetail.h
# End Source File
# Begin Source File

SOURCE=.\ViewAlarm.h
# End Source File
# Begin Source File

SOURCE=.\ViewAlarmDetail.h
# End Source File
# Begin Source File

SOURCE=.\ViewBase.h
# End Source File
# Begin Source File

SOURCE=.\ViewComm.h
# End Source File
# Begin Source File

SOURCE=.\ViewCommMap.h
# End Source File
# Begin Source File

SOURCE=.\ViewCross.h
# End Source File
# Begin Source File

SOURCE=.\ViewCrossDetail.h
# End Source File
# Begin Source File

SOURCE=.\ViewDI.h
# End Source File
# Begin Source File

SOURCE=.\ViewDistance.h
# End Source File
# Begin Source File

SOURCE=.\ViewFault.h
# End Source File
# Begin Source File

SOURCE=.\ViewFaultDetail.h
# End Source File
# Begin Source File

SOURCE=.\ViewFaultQuery.h
# End Source File
# Begin Source File

SOURCE=.\ViewFaultSheet.h
# End Source File
# Begin Source File

SOURCE=.\ViewOSC.h
# End Source File
# Begin Source File

SOURCE=.\ViewPTComm.h
# End Source File
# Begin Source File

SOURCE=.\ViewPTRun.h
# End Source File
# Begin Source File

SOURCE=.\ViewSOE.h
# End Source File
# Begin Source File

SOURCE=.\ViewSOEDetail.h
# End Source File
# Begin Source File

SOURCE=.\ViewStaInfo.h
# End Source File
# Begin Source File

SOURCE=.\ViewStationInfo.h
# End Source File
# Begin Source File

SOURCE=.\ViewSysAlarm.h
# End Source File
# Begin Source File

SOURCE=.\ViewSysAlarmDetail.h
# End Source File
# Begin Source File

SOURCE=.\WaveRecFrame.h
# End Source File
# Begin Source File

SOURCE=.\WaveRecGeneral.h
# End Source File
# Begin Source File

SOURCE=.\WaveRecOSC.h
# End Source File
# Begin Source File

SOURCE=.\WaveRecSetting.h
# End Source File
# Begin Source File

SOURCE=.\XJBrowser.h
# End Source File
# Begin Source File

SOURCE=.\XJBrowserDoc.h
# End Source File
# Begin Source File

SOURCE=.\XJBrowserView.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XJErrorID.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XJFile.h
# End Source File
# Begin Source File

SOURCE=..\..\common\xjlib.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XJLock.h
# End Source File
# Begin Source File

SOURCE=..\..\common\XJString.h
# End Source File
# Begin Source File

SOURCE=..\..\ui_common\button\XPStyleButtonST.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ack.ico
# End Source File
# Begin Source File

SOURCE=.\res\BACK.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_bkgnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_bkgnd2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_bkgnd3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_checkbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_def_head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_def_sort.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_def_tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bm_lockbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\control.ico
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_button.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_checkbox.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur_cross.cur
# End Source File
# Begin Source File

SOURCE=.\res\CUR_DROP.CUR
# End Source File
# Begin Source File

SOURCE=.\res\CUR_HORZ.CUR
# End Source File
# Begin Source File

SOURCE=.\res\cur_lockbox.cur
# End Source File
# Begin Source File

SOURCE=.\res\CUR_SIZE.CUR
# End Source File
# Begin Source File

SOURCE=.\res\cur_split.cur
# End Source File
# Begin Source File

SOURCE=.\res\CUR_STOP.CUR
# End Source File
# Begin Source File

SOURCE=.\res\CUR_VERT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\draw_wait.bmp
# End Source File
# Begin Source File

SOURCE=.\res\event.ico
# End Source File
# Begin Source File

SOURCE=.\res\fiswaveA.ico
# End Source File
# Begin Source File

SOURCE=.\res\gcvA.ico
# End Source File
# Begin Source File

SOURCE=.\res\greenlight.ico
# End Source File
# Begin Source File

SOURCE=.\res\guowang.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BITMAP_TREE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BITMAP_TREE1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BITMAP_TREE2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BUTTON_IMAGES.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDC_HSPLITBAR.cur
# End Source File
# Begin Source File

SOURCE=.\res\IDC_MAPTOOL_PAN.cur
# End Source File
# Begin Source File

SOURCE=.\res\IDC_VSPLITBAR.cur
# End Source File
# Begin Source File

SOURCE=.\res\IDR_GIF_WAIT.gif
# End Source File
# Begin Source File

SOURCE=.\res\key.ico
# End Source File
# Begin Source File

SOURCE=.\res\key1.ico
# End Source File
# Begin Source File

SOURCE=.\res\login.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\modify.ico
# End Source File
# Begin Source File

SOURCE=.\res\nan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nan_logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nanwang.bmp
# End Source File
# Begin Source File

SOURCE=.\res\print.ico
# End Source File
# Begin Source File

SOURCE=.\res\redlight.ico
# End Source File
# Begin Source File

SOURCE=.\res\refresh.ico
# End Source File
# Begin Source File

SOURCE=.\res\Search1.ico
# End Source File
# Begin Source File

SOURCE=.\res\start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\statuslight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Suntech.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tick.ico
# End Source File
# Begin Source File

SOURCE=.\res\tool1_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool1_nor.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolBmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolBmp2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\viewlog.ico
# End Source File
# Begin Source File

SOURCE=.\res\wndbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\xj.bmp
# End Source File
# Begin Source File

SOURCE=.\res\XJBrowser.ico
# End Source File
# Begin Source File

SOURCE=.\res\XJBrowser.rc2
# End Source File
# Begin Source File

SOURCE=.\res\XJBrowserDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\zoom_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoom_n.bmp
# End Source File
# Begin Source File

SOURCE=".\res\另存.ico"
# End Source File
# Begin Source File

SOURCE=".\res\中国南方电网.jpg"
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\greenlight.png
# End Source File
# Begin Source File

SOURCE=.\res\nan.png
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\redlight.png
# End Source File
# Begin Source File

SOURCE=.\xjbrowser.Manifest
# End Source File
# End Target
# End Project
# Section XJBrowser : {62274DEE-B5BF-48F8-910F-4F797CCBCD96}
# 	2:21:DefaultSinkHeaderFile:ddvue.h
# 	2:16:DefaultSinkClass:CDDVue
# End Section
# Section XJBrowser : {A1919AAD-5EF2-48E5-A8E1-2822EF1FA1F5}
# 	2:5:Class:CDDVue
# 	2:10:HeaderFile:ddvue.h
# 	2:8:ImplFile:ddvue.cpp
# End Section
