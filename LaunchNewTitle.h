// LaunchNewTitle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLaunchNewTitle dialog

class CLaunchNewTitle : public CDialog
{
// Construction
public:
	CLaunchNewTitle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLaunchNewTitle)
	enum { IDD = IDD_NEWLAUNCHITEM };
	CString	m_NewTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunchNewTitle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLaunchNewTitle)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
