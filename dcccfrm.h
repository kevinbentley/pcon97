// ChildFrm.h : interface of the CDCCCFrm class
//
/////////////////////////////////////////////////////////////////////////////

class CDCCCFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDCCCFrm)
public:
	CString m_WindowName;
	CSplitterWnd m_wndSplitter;
	CDCCCFrm();
	CDCCCFrm(CString& WindName);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCCFrm)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDCCCFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	BOOL m_iViewCreated;
	//{{AFX_MSG(CDCCCFrm)
	afx_msg void OnPaint();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnUpdateFileClearwindow(CCmdUI* pCmdUI);
	afx_msg void OnFileClearwindow();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
