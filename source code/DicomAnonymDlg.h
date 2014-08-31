// DicomAnonymDlg.h : header file
//

#if !defined(AFX_DICOMANONYMDLG_H__4BE96028_A633_4EA5_8CCD_549C150DC964__INCLUDED_)
#define AFX_DICOMANONYMDLG_H__4BE96028_A633_4EA5_8CCD_549C150DC964__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcmn.h>
#include "Anonymizer.h"

/////////////////////////////////////////////////////////////////////////////
// CDicomAnonymDlg dialog

class CDicomAnonymDlg : public CDialog
{
// Construction
public:
	CDicomAnonymDlg(CWnd* pParent = NULL);	// standard constructor
	~CDicomAnonymDlg();

// Dialog Data
	//{{AFX_DATA(CDicomAnonymDlg)
	enum { IDD = IDD_DICOMANONYM_DIALOG };
	CComboBox	m_Combo1;
	CListCtrl	m_List1;
	CString	m_sStartFolder;
	CString	m_sPrefix;
	int		m_nStart;
	BOOL	m_bEraseInstitution;
	BOOL	m_bUseNumber;
	CString	m_sTagLo;
	CString	m_sTagHi;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDicomAnonymDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Column numbers:
	enum { kNewVal=0, kOldVal };

	CAnonymizer m_Anonym;
	bool m_bEditing;

	void InitDicomTagComboBox();
	void UpdateList();
	void InsertListColumn(int num, int size, CString title);
	void SetListItem(int index, int sub, CString str);
	DWORD GetPrimaryTag();

	// About thread:
	int m_nProgress;
	bool m_bThreadRunning;
	bool m_bIgnoreErrors;

	static UINT ScanThread(LPVOID dummy);
	void ScanThread();
	static UINT AnonymizeThread(LPVOID dummy);
	void AnonymizeThread();

	// Generated message map functions
	//{{AFX_MSG(CDicomAnonymDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonScanFolder();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAssign();
	afx_msg void OnFinishedScan();
	afx_msg void OnFinishedAnonymize();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnEndLabelEditList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICOMANONYMDLG_H__4BE96028_A633_4EA5_8CCD_549C150DC964__INCLUDED_)
