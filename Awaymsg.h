// AwayMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAwayMsg dialog

class CAwayMsg : public CDialog
{
// Construction
public:
	CAwayMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAwayMsg)
	enum { IDD = IDD_AWAYMSG };
	CString	m_AwayMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAwayMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAwayMsg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
