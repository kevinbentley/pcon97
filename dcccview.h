
//
/////////////////////////////////////////////////////////////////////////////

class CDCCCView : public CRichEditView
{
protected: // create from serialization only
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CDCCCView();
	DECLARE_DYNCREATE(CDCCCView)

// Attributes
public:

	void SetNewLogName(LPCSTR FileName);
	BOOL GetLogState(void);
	void ToggleLogging(void);
	void UnderlineURLs(int iStart,LPCSTR szText);
	int SetDCCCOptions(int Mode);
    void OnMyKeyDown(UINT nChar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetNewFont(void);
	BOOL IsProfane(LPCTSTR Msg);
	void SetColor (COLORREF NewColor);
	void Message(LPCTSTR lpszMessage);
	CDCCCDoc* GetDocument();
	afx_msg void OnFileClearwindow();	
	
	CString m_TCPBuffA,m_TCPBuffB;
	BOOL m_AutologInit;
	BOOL m_URLCursor;
	CString m_LogName;	
	CFile m_LogFile;
	HCURSOR l_hc;
	HCURSOR l_hcaret;
	BOOL m_Logging;
	
	CFont *f;
	long l_iEnd;
	long l_iStart;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnChatClose(void);
	void ConnectDCCChat(UINT ip,short socket);
	void OnConnectChat(void);
	void OnReceive(void);
	void SendMsg(CString strMsg);
	void ParseDCCData(CString Line);
	
	CDCCSocket * m_DCCChatSocket;
	void OnAcceptConn(void);
	virtual ~CDCCCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDCCCView)
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
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in pconwView.cpp
inline CDCCCDoc* CDCCCView::GetDocument()
   { return (CDCCCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
