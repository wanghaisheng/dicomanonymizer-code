; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDicomAnonymDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DicomAnonym.h"

ClassCount=5
Class1=CDicomAnonymApp
Class2=CDicomAnonymDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CListCtrlX
Resource3=IDD_DICOMANONYM_DIALOG
Class5=CReportDlg
Resource4=IDD_REPORT
Resource5=IDR_MENU1

[CLS:CDicomAnonymApp]
Type=0
HeaderFile=DicomAnonym.h
ImplementationFile=DicomAnonym.cpp
Filter=N

[CLS:CDicomAnonymDlg]
Type=0
HeaderFile=DicomAnonymDlg.h
ImplementationFile=DicomAnonymDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_COMBO1

[CLS:CAboutDlg]
Type=0
HeaderFile=DicomAnonymDlg.h
ImplementationFile=DicomAnonymDlg.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DICOMANONYM_DIALOG]
Type=1
Class=CDicomAnonymDlg
ControlCount=18
Control1=IDC_EDIT1,edit,1350631552
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631941
Control4=IDC_STATIC1,static,1342308352
Control5=IDC_BUTTON2,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_EDIT3,edit,1350631552
Control9=IDC_BUTTON3,button,1342242816
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CHECK1,button,1342242819
Control13=IDC_CHECK2,button,1342242819
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT4,edit,1350631552
Control16=IDC_EDIT5,edit,1350631552
Control17=IDC_COMBO1,combobox,1344339971
Control18=IDC_BUTTON4,button,1342242816

[CLS:CListCtrlX]
Type=0
HeaderFile=ListCtrlX.h
ImplementationFile=ListCtrlX.cpp
BaseClass=CListCtrl
Filter=W

[DLG:IDD_REPORT]
Type=1
Class=CReportDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1350633668

[CLS:CReportDlg]
Type=0
HeaderFile=ReportDlg.h
ImplementationFile=ReportDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_FINISHED_SCAN
Command2=ID_FINISHED_ANONYMIZE
CommandCount=2

