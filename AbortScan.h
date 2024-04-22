// AbortScan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbortScan dialog

class CAbortScan : public CDialog
{
// Construction
public:
	int m_iCanceled;
	CAbortScan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAbortScan)
	enum { IDD = IDD_ABORTSCAN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbortScan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbortScan)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
