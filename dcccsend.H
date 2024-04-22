// DCCCSend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendChat view

#define MAX_OLD_COMMANDS 30


class CDCCCSend : public CEditView
{
protected:
	CDCCCSend();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCCCSend)

// Attributes
public:
	void OnMyKeyDown(UINT nChar);
	CString m_TmpStr;
	CDCCCDoc * GetDocument(void);
	CString ExpandString(CString OrigStr);
	void OnMyChar(UINT nchar);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCCSend)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCCCSend();
	CFont *f;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDCCCSend)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int iScrollCmd;
	int iCurrCmdPos;
	CString CmdHist[MAX_OLD_COMMANDS];
	
};

/////////////////////////////////////////////////////////////////////////////
