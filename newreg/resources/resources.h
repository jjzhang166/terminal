// resources.h : main header file for the RESOURCES application
//

#if !defined(AFX_RESOURCES_H__1A0533D1_88AF_4926_B0EC_1AEB2DA9A6A6__INCLUDED_)
#define AFX_RESOURCES_H__1A0533D1_88AF_4926_B0EC_1AEB2DA9A6A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
/////////////////////////////////////////////////////////////////////////////
// CResourcesApp:
// See resources.cpp for the implementation of this class
//

class CResourcesApp: public CWinApp {
public:
	CResourcesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourcesApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CResourcesApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCES_H__1A0533D1_88AF_4926_B0EC_1AEB2DA9A6A6__INCLUDED_)
