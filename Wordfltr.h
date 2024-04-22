// WordFltr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWordFltr dialog

class CWordFltr : public CDialog
{
// Construction
public:
	CRegistry registry;
	CString m_CurSel;
	CWordFltr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWordFltr)
	enum { IDD = IDD_DLGHIGHLIGHT };
	CClrStatic	m_statSampleTxt;
	CStringArray string_array;
	CListBox	m_lbWords;
	CString	m_NewText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordFltr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWordFltr)
	afx_msg void OnSelchangeWords();
	virtual void OnOK();
	afx_msg void OnAdd2list();
	afx_msg void OnDeleteitm();
	afx_msg void OnSetcolor();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
