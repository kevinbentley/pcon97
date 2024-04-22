#if !defined(AFX_DCCSOCKET_H__65381A01_2154_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DCCSOCKET_H__65381A01_2154_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DCCSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDCCSocket command target

class CDCCSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CDCCSocket(CWnd *Owner,int ClassType);
	virtual ~CDCCSocket();

// Overrides
public:
	int m_ClassType;
	CWnd *m_OwnerWnd;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDCCSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCCSOCKET_H__65381A01_2154_11D1_98E3_00A0249AB02A__INCLUDED_)
