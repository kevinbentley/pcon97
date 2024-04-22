// DCCSocket.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "DCCSocket.h"

#include "DCCCDoc.h"
#include "DCCCView.h"
#include "DCCFView.h"

/////////////////////////////////////////////////////////////////////////////
// CDCCSocket

CDCCSocket::CDCCSocket(CWnd *Owner,int ClassType)
{
	m_ClassType=ClassType;
	m_OwnerWnd=Owner;

}

CDCCSocket::~CDCCSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDCCSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CDCCSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDCCSocket member functions

void CDCCSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
	if(m_ClassType==DCC_CHAT_LISTEN)
	{
		((CDCCCView*)m_OwnerWnd)->OnChatClose();
	}
	if(m_ClassType==DCC_FILE_LISTEN)
	{
		((CDCCFView*)m_OwnerWnd)->OnDCCClose();
	}
}

void CDCCSocket::OnConnect(int nErrorCode) 
{
	CAsyncSocket::OnConnect(nErrorCode);
	if(m_ClassType==DCC_CHAT_LISTEN)
	{
		((CDCCCView*)m_OwnerWnd)->OnConnectChat();
	}
	if(m_ClassType==DCC_FILE_LISTEN)
	{
		((CDCCFView*)m_OwnerWnd)->OnConnectXfer();
	}
}

void CDCCSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnAccept(nErrorCode);

	if(m_ClassType==DCC_CHAT_LISTEN)
	{
		((CDCCCView*)m_OwnerWnd)->OnAcceptConn();
	}
	if(m_ClassType==DCC_FILE_LISTEN)
	{
		((CDCCFView*)m_OwnerWnd)->OnAcceptConn();
	}
}

void CDCCSocket::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
	if(m_ClassType==DCC_CHAT_LISTEN)
	{
		((CDCCCView*)m_OwnerWnd)->OnReceive();
	}
	if(m_ClassType==DCC_FILE_LISTEN)
	{
		((CDCCFView*)m_OwnerWnd)->OnReceive();
	}

	
}
