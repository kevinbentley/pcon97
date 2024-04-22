// ClrStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClrStatic window

class CClrStatic : public CStatic
{
// Construction
public:
	DWORD m_clrBackground;
	DWORD m_clrForeground;
	CClrStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClrStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClrStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClrStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
