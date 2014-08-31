// DicomAnonymDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DicomAnonym.h"
#include "DicomAnonymDlg.h"
#include "ReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CDicomAnonymDlg* m_pDlg;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDicomAnonymDlg dialog

CDicomAnonymDlg::CDicomAnonymDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDicomAnonymDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDicomAnonymDlg)
	m_sStartFolder = _T("");
	m_sPrefix = _T("");
	m_nStart = 1;
	m_bEraseInstitution = FALSE;
	m_bUseNumber = TRUE;
	m_sTagLo = _T("0010");
	m_sTagHi = _T("0010");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bEditing = false;
	m_bThreadRunning = false;
	m_bIgnoreErrors = false;

	::CoInitialize(0);  // In order to use SHBrowseForFolder().
}

CDicomAnonymDlg::~CDicomAnonymDlg()
{
	::CoUninitialize();
}

void CDicomAnonymDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDicomAnonymDlg)
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_Control(pDX, IDC_LIST1, m_List1);
	DDX_Text(pDX, IDC_EDIT1, m_sStartFolder);
	DDX_Text(pDX, IDC_EDIT2, m_sPrefix);
	DDX_Text(pDX, IDC_EDIT3, m_nStart);
	DDX_Check(pDX, IDC_CHECK1, m_bEraseInstitution);
	DDX_Check(pDX, IDC_CHECK2, m_bUseNumber);
	DDX_Text(pDX, IDC_EDIT4, m_sTagLo);
	DDX_Text(pDX, IDC_EDIT5, m_sTagHi);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDicomAnonymDlg, CDialog)
	//{{AFX_MSG_MAP(CDicomAnonymDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonScanFolder)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonAssign)
	ON_COMMAND(ID_FINISHED_SCAN, OnFinishedScan)
	ON_COMMAND(ID_FINISHED_ANONYMIZE, OnFinishedAnonymize)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, OnEndLabelEditList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnClickList1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDicomAnonymDlg message handlers

BOOL CDicomAnonymDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CWinApp* pApp = AfxGetApp();
	m_sStartFolder = pApp->GetProfileString(_T("DicomAnonym"), _T("StartFolder"), _T(""));

	InsertListColumn(kOldVal, 350, _T("Old name"));
	InsertListColumn(kNewVal, 250, _T("New name"));
	int order[] = { kOldVal, kNewVal };
	m_List1.SetColumnOrderArray(2, order);
	
	m_pDlg = this;

	InitDicomTagComboBox();

	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDicomAnonymDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDicomAnonymDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDicomAnonymDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnButtonStart() 
{
	UpdateData();
	m_bThreadRunning = true;
	SetTimer(2, 200, NULL);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  // Disable "Anonymize All" button
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  // Disable "Scan" button
	AfxBeginThread(AnonymizeThread, NULL);
}

//------------------------------------------------------------------------------
void CDicomAnonymDlg::OnButtonScanFolder() 
{
	UpdateData();

	m_Anonym.SetPrimaryTag(GetPrimaryTag());

	if (!m_sStartFolder.IsEmpty())
	{
		m_bThreadRunning = true;
		SetTimer(1, 200, NULL);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);  // Disable "Anonymize All" button
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);  // Disable "Scan" button
		AfxBeginThread(ScanThread, NULL);
	}

}

//------------------------------------------------------------------------------
void CDicomAnonymDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	POSITION pos = m_List1.GetFirstSelectedItemPosition();
	if (pos == 0)
	{
		return;
	}
	int index = m_List1.GetNextSelectedItem(pos);
	ASSERT(index >= 0);
	if (m_bEditing)
	{
		return;
	}

	// Start editing:
	CRect rCell;
	m_List1.GetItemRect(index, &rCell, LVIR_LABEL);
	m_List1.ClientToScreen(&rCell);
	CPoint Cursor;
	::GetCursorPos(&Cursor);
	if (rCell.PtInRect(Cursor))
	{
		m_List1.EditLabel(index);
	}
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnButtonAssign() 
{
	UpdateData();
	m_Anonym.AssignNewNames(m_sPrefix, m_nStart, m_bUseNumber != FALSE);
	UpdateList();
}

//=========================================================================
void CDicomAnonymDlg::InitDicomTagComboBox()
{
#define ADD_TAG_ITEM(dw, name) index = m_Combo1.AddString(name); if (index >= 0) m_Combo1.SetItemData(index, dw);

	int index;
	ADD_TAG_ITEM(0x00100010, "Patient's Name")
	ADD_TAG_ITEM(0x00200010, "Patient ID")
	ADD_TAG_ITEM(0x10300008, "Study Description")
	ADD_TAG_ITEM(0x00700008, "Manufacturer")

	m_Combo1.SetCurSel(0);

#undef ADD_TAG_ITEM
}

//=========================================================================
void CDicomAnonymDlg::UpdateList()
{
	m_List1.DeleteAllItems();
	const CAnonymizer::Dictionary& dict = m_Anonym.GetDictionary();
	for (int i=0; i<dict.size(); i++)
	{
		SetListItem(i, kNewVal, dict[i].m_sAnonymized);
		SetListItem(i, kOldVal, dict[i].m_sName);
	}
}

//=========================================================================
void CDicomAnonymDlg::InsertListColumn(int num, int size, CString title)
{
	LV_COLUMN	lvc;

	lvc.mask	= LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvc.fmt		= LVCFMT_BITMAP_ON_RIGHT;
	lvc.cx		= size;
	lvc.pszText	= title.GetBuffer(title.GetLength()+1 );

	m_List1.InsertColumn(num, &lvc);
}

//=========================================================================
void CDicomAnonymDlg::SetListItem(int index, int sub, CString str)
{
	LV_ITEM		item;

	item.iItem		= index;
	item.iSubItem	= sub;
	item.pszText	= str.GetBuffer( str.GetLength()+1 );
	item.mask		= LVIF_TEXT | LVIF_IMAGE;

	if( index >= m_List1.GetItemCount() )
		m_List1.InsertItem( &item );
	else
		m_List1.SetItem( &item );
}

//=========================================================================
DWORD CDicomAnonymDlg::GetPrimaryTag()
{
	UpdateData();

	DWORD Lo, Hi;
	sscanf(m_sTagLo, "%x", &Lo);
	sscanf(m_sTagHi, "%x", &Hi);

	return (Hi << 16) + Lo;
}

//=========================================================================
UINT CDicomAnonymDlg::ScanThread(LPVOID dummy)
{
	m_pDlg->ScanThread();
	return 0;
}

//=========================================================================
void CDicomAnonymDlg::ScanThread()
{
	m_nProgress = 0;
	m_nProgress = m_Anonym.ScanFolder(m_sStartFolder, m_nProgress);
	PostMessage(WM_COMMAND, ID_FINISHED_SCAN);
}

//=========================================================================
UINT CDicomAnonymDlg::AnonymizeThread(LPVOID dummy)
{
	m_pDlg->AnonymizeThread();
	return 0;
}

//=========================================================================
void CDicomAnonymDlg::AnonymizeThread()
{
	bool bEraseInstitution = m_bEraseInstitution != 0;
	m_nProgress = 0;
	int nFiles = m_Anonym.GetFileList().size();
	for (int i=0; i<nFiles; i++)
	{
		m_nProgress = i+1;
		if ((m_Anonym.Anonymize(i, bEraseInstitution) == false) && !m_bIgnoreErrors)
		{
			// Error happened.
			int decision = AfxMessageBox(m_Anonym.GetLastErrMsg(), MB_ABORTRETRYIGNORE);
			if (decision == IDABORT)
			{
				PostMessage(WM_COMMAND, ID_FINISHED_ANONYMIZE);
				return;
			}
			else if (decision == IDIGNORE)
			{
				m_bIgnoreErrors = true;
			}
		}
	}
	PostMessage(WM_COMMAND, ID_FINISHED_ANONYMIZE);
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnFinishedScan() 
{
	m_bThreadRunning = false;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);  // Enable "Anonymize All" button
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);  // Enable "Scan" button

	// Populate the list control:
	OnButtonAssign();

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(_T("DicomAnonym"), _T("StartFolder"), m_sStartFolder);
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnFinishedAnonymize() 
{
	m_bThreadRunning = false;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);  // Enable "Anonymize All" button
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);  // Enable "Scan" button

	// Display a report:
	CReportDlg dlg;
	dlg.SetData(m_Anonym.GetDictionary());
	dlg.DoModal();
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnTimer(UINT nIDEvent) 
{
	static nLastID, nCountDown;
	
	// Use slow timer after 1 minute:
	if (nIDEvent != nLastID)
	{
		nCountDown = 60000 / 200;
		nLastID = nIDEvent;
	}
	if (nCountDown > 0)
	{
		nCountDown--;
		if (nCountDown == 0)
		{
			KillTimer(nIDEvent);
			SetTimer(nIDEvent, 1000, NULL);
		}
	}

	if (!m_bThreadRunning)
	{
		KillTimer(nIDEvent);
	}

	// Update the message:
	if (nIDEvent == 1)  // Scanning folder
	{
		CString sMsg;
		sMsg.Format(_T("%d files scanned."), m_nProgress);
		GetDlgItem(IDC_STATIC1)->SetWindowText(sMsg);
	}
	else if (nIDEvent == 2)  // Anonymizing
	{
		int nFiles = m_Anonym.GetFileList().size();
		CString sMsg;
		sMsg.Format("%d of %d anonymized.", m_nProgress, nFiles);
		GetDlgItem(IDC_STATIC1)->SetWindowText(sMsg);
	}
	
	CDialog::OnTimer(nIDEvent);
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnClose() 
{
	if (m_bThreadRunning)
	{
		if (AfxMessageBox("Discard running job and exit?", MB_YESNO) != IDYES)
		{
			return;
		}
	}
	
	CDialog::OnClose();
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnEndLabelEditList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// User has finished editing the new name.

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	if (pDispInfo->item.iItem < 0)
	{
		ASSERT(0); return;
	}

	if (pDispInfo->item.pszText)  // Changed
	{
		if (pNMHDR->idFrom == IDC_LIST1)
		{
			// Update the new string to the dictionary and the list item:
			int index = pDispInfo->item.iItem;
			CAnonymizer::NamePair pair = m_Anonym.GetDictionary()[index];
			pair.m_sAnonymized = pDispInfo->item.pszText;
			pair.m_sAnonymized = pair.m_sAnonymized.Left(pair.m_sName.GetLength());
			m_Anonym.SetDictionary(pair);
			SetListItem(index, kNewVal, pair.m_sAnonymized);
		}
	}

	*pResult = 0;
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnColumnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// User has clicked on the header of a column.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int column = pNMListView->iSubItem;
	m_Anonym.SortDictionary(column == kOldVal);
	UpdateList();
	
	*pResult = 0;
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnOK() 
{
	// To avoid closing the window when hitting Enter key
	//CDialog::OnOK();
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnSelchangeCombo1() 
{
	int index = m_Combo1.GetCurSel();
	DWORD tag = m_Combo1.GetItemData(index);
	m_sTagLo.Format("%04x", tag & 0xFFFF);
	m_sTagHi.Format("%04x", (tag >> 16) & 0xFFFF);
	UpdateData(FALSE);
}

//-------------------------------------------------------------------------
void CDicomAnonymDlg::OnButtonBrowse() 
{
	// Open "Browse for folders" dialog:

	LPMALLOC pMalloc = NULL;
	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	// set the bi's default values
	bi.hwndOwner = GetSafeHwnd();
	bi.lpszTitle = _T("Select the folder that contains DICOM files.");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = NULL;
	pidl = SHBrowseForFolder(&bi);
	if(pidl != NULL)
	{
		char path[MAX_PATH];
		SHGetPathFromIDList(pidl, path);
		m_sStartFolder = path;
		UpdateData(false);
	
		// free memory
		if(SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
}
