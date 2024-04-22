#ifndef _CDCCCDoc_header_
#define _CDCCCDoc_header_
//
/////////////////////////////////////////////////////////////////////////////

#include "DCCSocket.h"

class CDCCCDoc : public CDocument
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CDCCCDoc)

// Attributes
public:
	void SendPageKey(UINT nchar);
	void SendSysKey(UINT nchar);
	void SetNewFont(void);
	void SetNewTitle(LPCSTR Title);
	void ActivateSendVw(UINT nchar);
	void SetSendText(LPCSTR Text);
	void DisplayMsg(LPCTSTR lpszText,DWORD l_Color);
	int SetDCCCOptions(int Mode);
	CString m_Winname;

// Operations
public:
	CDCCCDoc();
	CDCCCDoc(CString& Winname);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCCDoc)
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
	CDCCSocket * m_ListenSocket;
	CString m_FmtTitle;
	virtual ~CDCCCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDCCCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif