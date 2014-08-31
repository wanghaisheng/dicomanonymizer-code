// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DicomAnonym.h"
#include "ReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog


CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDlg)
	m_sReport = _T("");
	//}}AFX_DATA_INIT
}

void CReportDlg::SetData(const CAnonymizer::Dictionary& dict)
{
	m_sReport = "Original\tAnonymized\r\n";

	// Sort them:
	std::list<CAnonymizer::NamePair>::iterator pList;
	CAnonymizer::Dictionary::const_iterator pDict;
	for (pDict = dict.begin(); pDict != dict.end(); pDict++)
	{
		const CString& sNewName = pDict->m_sName;
		
		for (pList = m_List.begin(); pList != m_List.end(); pList++)
		{
			if (sNewName <= pList->m_sName)
			{
				break;
			}
		}
		m_List.insert(pList, *pDict);
	}

	// Display them:
	for (pList = m_List.begin(); pList != m_List.end(); pList++)
	{
		m_sReport = m_sReport + pList->m_sName + '\t' + pList->m_sAnonymized + "\r\n";
	}
}


void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDlg)
	DDX_Text(pDX, IDC_EDIT1, m_sReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	//{{AFX_MSG_MAP(CReportDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDlg message handlers
