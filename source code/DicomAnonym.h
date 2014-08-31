// DicomAnonym.h : main header file for the DICOMANONYM application
//

#if !defined(AFX_DICOMANONYM_H__E15BF813_43F2_4510_AA2B_411B0D924C0E__INCLUDED_)
#define AFX_DICOMANONYM_H__E15BF813_43F2_4510_AA2B_411B0D924C0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDicomAnonymApp:
// See DicomAnonym.cpp for the implementation of this class
//

class CDicomAnonymApp : public CWinApp
{
public:
	CDicomAnonymApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDicomAnonymApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDicomAnonymApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICOMANONYM_H__E15BF813_43F2_4510_AA2B_411B0D924C0E__INCLUDED_)
