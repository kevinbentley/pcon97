// ControlSocket.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "ControlSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlSocket

CControlSocket::CControlSocket()
{
}

CControlSocket::~CControlSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CControlSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CControlSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CControlSocket member functions

void CControlSocket::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
	((CPconwApp*)AfxGetApp())->ProccessControlData();
}
