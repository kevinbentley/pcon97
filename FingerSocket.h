#if !defined(AFX_FINGERSOCKET_H__62EDB904_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_FINGERSOCKET_H__62EDB904_08E3_11D1_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FingerSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CFingerSocket command target

class CFingerSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CFingerSocket(CWnd *Owner);
	virtual ~CFingerSocket();

// Overrides
public:
	CWnd * m_OwnerWnd;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFingerSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFingerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERSOCKET_H__62EDB904_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
