// FKeySetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFKeySetup dialog

class CFKeySetup : public CDialog
{
// Construction
public:
	CRegistry registry;
	CFKeySetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFKeySetup)
	enum { IDD = IDD_FKEYSETUP };
	CString	m_F10;
	CString	m_F11;
	CString	m_F12;
	CString	m_F2;
	CString	m_F3;
	CString	m_F4;
	CString	m_F5;
	CString	m_F6;
	CString	m_F7;
	CString	m_F8;
	CString	m_F9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFKeySetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFKeySetup)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
