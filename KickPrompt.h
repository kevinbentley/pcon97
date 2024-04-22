#if !defined(AFX_KICKPROMPT_H__0D21D7C1_05BB_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_KICKPROMPT_H__0D21D7C1_05BB_11D1_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KickPrompt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKickPrompt dialog

class CKickPrompt : public CDialog
{
// Construction
public:
	CKickPrompt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKickPrompt)
	enum { IDD = IDD_PROMPTKICK };
	CString	m_KickMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKickPrompt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKickPrompt)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KICKPROMPT_H__0D21D7C1_05BB_11D1_98E1_00A0249AB02A__INCLUDED_)
