#ifndef _setup_dialog_header_
#define _setup_dialog_header_
/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog

class CSetupDlg : public CDialog
{
// Construction
public:
	BOOL m_bShowJoinPart;
	BOOL m_bSpamFilter;
	BOOL m_bPrivate;
	CString m_CSFontDesc;
	CRegistry registry;
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_SETUP };
	CStatic	m_FontDesc;
	CString	m_strChannel;
	CString	m_strHandle;
	CString	m_strServer;
	BOOL	m_ProfanityFilterEnabled;
	CString	m_csQuitMsg;
	BOOL	m_bAutoLog;
	UINT	m_iServerPort;
	CString	m_ServerName;
	BOOL	m_BeepOnPrivate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupDlg)
	afx_msg void OnSetcolors();
	afx_msg void OnSetfkeys();
	virtual void OnOK();
	afx_msg void OnSetfont();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetlaunch();
	afx_msg void OnShowjoinpart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif