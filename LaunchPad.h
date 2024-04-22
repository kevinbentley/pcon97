// LaunchPad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLaunchPad window

class CLaunchPad : public CToolBar
{
// Construction
public:
	CLaunchPad();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunchPad)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLaunchPad();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLaunchPad)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
