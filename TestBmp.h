// TestBmp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestBmp dialog

class CTestBmp : public CDialog
{
// Construction
public:
	CString m_IconName;
	void SetIconName (LPCSTR IconName);
	HBITMAP m_bmp;
	void SetBmp (HBITMAP mybmp);
	CTestBmp(CWnd* pParent = NULL);   // standard constructor
	
	CImageList l_ConvertImage;
	HICON l_Icon;
	IMAGEINFO l_ii;
	int l_inum;
	CDC m_MemDC,CDCIcon;
    CBitmap newBitmap;
    CBitmap* pOldBitmap;
	CBitmap l_bmp;
// Dialog Data
	//{{AFX_DATA(CTestBmp)
	enum { IDD = IDD_DIALOG3 };
	CStatic	m_RealIcon;
	CStatic	m_Picture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestBmp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
