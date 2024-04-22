// GetFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetFileDlg dialog

class CGetFileDlg : public CDialog
{
// Construction
public:
	CGetFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetFileDlg)
	enum { IDD = IDD_DIALOG2 };
	CString	m_GetNum;
	CString	m_GetFile1Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetFileDlg)
	afx_msg void OnGetfilename();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
