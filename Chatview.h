// chatview.h : interface of the CChatView class
//
/////////////////////////////////////////////////////////////////////////////

class CChatView : public CRichEditView
{
protected: // create from serialization only
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CChatView();
	DECLARE_DYNCREATE(CChatView)

// Attributes
public:
	HCURSOR l_hc;
	HCURSOR l_hcaret;
	void UnderlineURLs(int iStart,LPCSTR szText);
    BOOL m_AutologInit;
	BOOL m_URLCursor;
	void OnMyKeyDown(UINT nChar);
	CString m_LogName;
	void SetNewLogName(LPCSTR FileName);
	BOOL GetLogState(void);
	void ToggleLogging(void);
	CFile m_LogFile;
	BOOL m_Logging;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetNewFont(void);
	BOOL IsProfane(LPCTSTR Msg);
	void SetColor (COLORREF NewColor);
	void Message(LPCTSTR lpszMessage);
	CChatDoc* GetDocument();
	afx_msg void OnFileClearwindow();
	CFont *f;
	long l_iEnd;
	long l_iStart;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChatView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnClearwindow();
	afx_msg void OnUpdateFileClearwindow(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in pconwView.cpp
inline CChatDoc* CChatView::GetDocument()
   { return (CChatDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
