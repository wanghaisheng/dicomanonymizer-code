#if !defined(AFX_REPORTDLG_H__B3FA7159_805B_4C84_B9F0_1DD42F767E48__INCLUDED_)
#define AFX_REPORTDLG_H__B3FA7159_805B_4C84_B9F0_1DD42F767E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDlg.h : header file
//

#include "Anonymizer.h"
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CReportDlg dialog

class CReportDlg : public CDialog
{
// Construction
public:
	CReportDlg(CWnd* pParent = NULL);   // standard constructor

	void SetData(const CAnonymizer::Dictionary& dict);


// Dialog Data
	//{{AFX_DATA(CReportDlg)
	enum { IDD = IDD_REPORT };
	CString	m_sReport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	std::list<CAnonymizer::NamePair> m_List;

	// Generated message map functions
	//{{AFX_MSG(CReportDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDLG_H__B3FA7159_805B_4C84_B9F0_1DD42F767E48__INCLUDED_)
