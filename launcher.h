// DUserList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDUserList dialog

class CLauncher : public CToolBar
{
// Construction
public:
	void RefreshToolBar(LaunchItems *LaunchPtr);
	CStringList m_strlstDragged;
	CToolBarCtrl *m_ToolBarCtlA;
	
	CLauncher();   // standard constructor

/*	
	//{{AFX_DATA(CLauncher)
	enum { IDD = IDD_LAUNCHER };
	//}}AFX_DATA
*/


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauncher)
	public:
	virtual BOOL Create( CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CLauncher)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
