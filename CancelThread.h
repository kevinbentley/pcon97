// CancelThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CCancelThread thread

class CCancelThread : public CWinThread
{
	DECLARE_DYNCREATE(CCancelThread)
protected:

// Attributes
public:
	CCancelThread();           // protected constructor used by dynamic creation
	CAbortScan m_AbortDlg;
	virtual ~CCancelThread();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCancelThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCancelThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
