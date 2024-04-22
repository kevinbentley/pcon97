#ifndef _chatdoc_header_
#define _chatdoc_header_
// pconwDoc.h : interface of the CChatDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CChatDoc : public CDocument
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CChatDoc)

// Attributes
public:
	void SendPageKey(UINT nchar);
	void SendSysKey(UINT nchar);
	void SetNewFont(void);
	void SetNewTitle(LPCSTR Title);
	void ActivateSendVw(UINT nchar);
	void SetSendText(LPCSTR Text);
	void DisplayMsg(LPCTSTR lpszText,DWORD l_Color);
	CString m_Winname;

// Operations
public:
	CChatDoc();
	CChatDoc(CString& Winname);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChatDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChatDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif