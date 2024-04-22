#if !defined(AFX_CONTROLSOCKET_H__178E4401_07F0_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_CONTROLSOCKET_H__178E4401_07F0_11D1_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ControlSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CControlSocket command target

class CControlSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CControlSocket();
	virtual ~CControlSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CControlSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLSOCKET_H__178E4401_07F0_11D1_98E1_00A0249AB02A__INCLUDED_)
