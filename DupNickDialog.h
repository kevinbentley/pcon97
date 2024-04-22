#if !defined(AFX_DUPNICKDIALOG_H__6212A161_26D6_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DUPNICKDIALOG_H__6212A161_26D6_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DupNickDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDupNickDialog dialog

class CDupNickDialog : public CDialog
{
// Construction
public:
	CString m_OldNick;
	CString m_ChosenNick;
	CDupNickDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDupNickDialog)
	enum { IDD = IDD_DUPNICK };
	CString	m_NewNick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDupNickDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDupNickDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUPNICKDIALOG_H__6212A161_26D6_11D1_98E3_00A0249AB02A__INCLUDED_)
