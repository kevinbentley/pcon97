// FingerSocket.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "FingerSocket.h"
#include "FingerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerSocket

CFingerSocket::CFingerSocket(CWnd *Owner)
{
	m_OwnerWnd=Owner;
}

CFingerSocket::~CFingerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CFingerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CFingerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CFingerSocket member functions

void CFingerSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnConnect(nErrorCode);
	((CFingerView *)m_OwnerWnd)->ConnectFinger();
}

void CFingerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnReceive(nErrorCode);
	((CFingerView *)m_OwnerWnd)->ReceiveFinger();
}

void CFingerSocket::OnClose(int nErrorCode) 
{
	CAsyncSocket::OnClose(nErrorCode);
	((CFingerView *)m_OwnerWnd)->CloseFinger();
}
