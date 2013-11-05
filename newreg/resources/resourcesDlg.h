// resourcesDlg.h : header file
//

#if !defined(AFX_RESOURCESDLG_H__1167E459_4C3B_4190_8382_93D2971A5661__INCLUDED_)
#define AFX_RESOURCESDLG_H__1167E459_4C3B_4190_8382_93D2971A5661__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CResourcesDlg dialog

class CResourcesDlg: public CDialog {
// Construction
public:
	CResourcesDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
	//{{AFX_DATA(CResourcesDlg)
	enum {
		IDD = IDD_RESOURCES_DIALOG
	};
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourcesDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CResourcesDlg)
	virtual BOOL OnInitDialog();afx_msg
	void OnPaint();afx_msg
	HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCESDLG_H__1167E459_4C3B_4190_8382_93D2971A5661__INCLUDED_)
