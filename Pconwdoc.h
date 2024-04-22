#ifndef _pconwdoc_header_
#define _pconwdoc_header_
// pconwDoc.h : interface of the CPconwDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CPconwDoc : public CDocument
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CPconwDoc)

// Attributes
public:
	BOOL UserPart(LPCSTR User);
	void UserJoin(LPCSTR User);
	void SendPageKey(UINT nchar);
	void SendSysKey(UINT nchar);
	void SetNewFont(void);
	void SetNewTitle(LPCSTR Title,CString StrippedTitle);
	void ActivateSendVw(UINT nchar);
	void SetSendText(LPCSTR Text);
	void DisplayMsg(LPCTSTR lpszText,DWORD l_Color);
	CString m_Winname;
	CString m_Title;

// Operations
public:
	CPconwDoc();
	CPconwDoc(CString& Winname);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPconwDoc)
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
	CString m_ChanMode;
	void SetChannelMode(CString Mode);
	CString m_SelectedString;
	BOOL IsUserInChannel(CString &User);
	void ChangeNick(CString & p_newnick, CString & p_oldnick);
	void ChangeUserMode(CString &Modes,CString &Users);
	virtual ~CPconwDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPconwDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif