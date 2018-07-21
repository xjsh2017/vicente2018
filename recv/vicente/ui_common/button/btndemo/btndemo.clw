; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBtndemoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "btndemo.h"

ClassCount=3
Class1=CBtndemoApp
Class2=CBtndemoDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_BTNDEMO_DIALOG
Resource4=IDR_TOOLBAR1
Resource5=IDR_MENU1

[CLS:CBtndemoApp]
Type=0
HeaderFile=btndemo.h
ImplementationFile=btndemo.cpp
Filter=N

[CLS:CBtndemoDlg]
Type=0
HeaderFile=btndemoDlg.h
ImplementationFile=btndemoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_MENU_1

[CLS:CAboutDlg]
Type=0
HeaderFile=btndemoDlg.h
ImplementationFile=btndemoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_BTNDEMO_DIALOG]
Type=1
Class=CBtndemoDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_MENU_1
Command2=ID_MENU_2
Command3=ID_MENU_7
Command4=ID_MENU_4
Command5=ID_MENU_5
Command6=ID_MENU_6
Command7=ID_MENU_3
CommandCount=7

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_MENU_1
Command2=ID_MENU_2
Command3=ID_MENU_3
Command4=ID_MENU_4
Command5=ID_MENU_5
Command6=ID_MENU_6
Command7=ID_MENU_7
CommandCount=7

