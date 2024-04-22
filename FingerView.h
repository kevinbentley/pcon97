#if !defined(AFX_FINGERVIEW_H__62EDB903_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_FINGERVIEW_H__62EDB903_08E3_11D1_98E1_00A0249AB02A__INCLUDED_

#include "FingerSocket.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FingerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFingerView view

class CFingerView : public CRichEditView
{
protected: // create from serialization only
	CFingerView();
	DECLARE_DYNCREATE(CFingerView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFingerView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void CloseFinger(void);
	long l_iStart;
	long l_iEnd;
	char m_szHostBuffer[MAXGETHOSTSTRUCT+1];
	CString m_strHost;
	CString m_strUser;
	
	HCURSOR l_hc;
	BOOL m_URLCursor;
	void ReceiveFinger();
	void ConnectFinger();
	void UnderlineURLs(int iStart,LPCSTR szText);
	CFingerSocket *m_FingerSocket;
	HOSTENT *m_hostEnt;
	virtual ~CFingerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Generated message map functions
	//{{AFX_MSG(CFingerView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERVIEW_H__62EDB903_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
