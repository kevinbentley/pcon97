// olorSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ColorSetup dialog

class ColorSetup : public CDialog
{
// Construction
public:
	DWORD m_dwBackground;
	DWORD m_dwStatus;
	DWORD m_dwPrivate;
	DWORD m_dwNormal;
	DWORD m_dwAction;
    DWORD m_dwURL;
	CRegistry registry;
	ColorSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ColorSetup)
	enum { IDD = IDD_COLORSETUP };
	CClrStatic	m_URL;
	CClrStatic	m_Status;
	CClrStatic	m_Private;
	CClrStatic	m_Normal;
	CClrStatic	m_Background;
	CClrStatic	m_Action;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColorSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ColorSetup)
	afx_msg void OnStatustxt();
	afx_msg void OnPrivatetxt();
	afx_msg void OnNormtxt();
	afx_msg void OnBackground();
	afx_msg void OnActiontxt();
	afx_msg void OnCfghighlight();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnColorurl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
