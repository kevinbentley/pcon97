// SendPrvMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SendPrvMsg dialog

class SendPrvMsg : public CDialog
{
// Construction
public:
	SendPrvMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SendPrvMsg)
	enum { IDD = IDD_PRIVATEMSG };
	CButton	m_Cancel;
	CButton	m_Ok;
	CString	m_Message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SendPrvMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SendPrvMsg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
