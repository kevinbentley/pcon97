// chatsock.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1994 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
//#include <wfc.h>
//#include "chatter.h"

#include "chatsock.h"
//#include "chatdoc.h"

#include <stddef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//IMPLEMENT_DYNAMIC(CChatSocket, CAsyncSocket)


/////////////////////////////////////////////////////////////////////////////
// CChatSocket

CChatSocket::CChatSocket()
{
}

CChatSocket::~CChatSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CChatSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatSocket member functions

void CChatSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnReceive(nErrorCode);
	((CPconwApp*)AfxGetApp())->ProcessPendingRead();
}

void CChatSocket::OnConnect(int nErrorCode) 
{
	
	if(nErrorCode!=0)
	{
		CAsyncSocket::OnConnect(nErrorCode);
		return;
	}
	//MessageBeep(-1);
	CString l_Message;
	CString l_SN,l_strtmp;
	CString l_Email;
	l_strtmp=((CPconwApp*)AfxGetApp())->psharemem->serial;
	l_Email=((CPconwApp*)AfxGetApp())->psharemem->email;
	if(l_strtmp.GetLength())
	{
		l_SN=l_strtmp.Right(8);
	}
	else
	{
		l_SN="UnRegged";
	}
	//Throw the user & stuff
	l_Message.Format("USER %s %s %s :%s\n",l_SN,l_SN,l_SN,l_Email);
	Send(LPCSTR(l_Message),l_Message.GetLength());
	
	l_Message.LoadString(IDS_IRCCONNECTED);
	((CPconwApp*)AfxGetApp())->DispatchMessage(l_Message,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	
	l_Message.Format("NICK %s\n",((CPconwApp*)AfxGetApp())->m_strHandle); 
	Send(LPCSTR(l_Message),l_Message.GetLength());
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CChatSocket::OnClose(int nErrorCode) 
{
	CString msg;
	//MessageBeep(-1);
	msg.LoadString(IDS_DISCONNECTIRC);
	((CPconwApp*)AfxGetApp())->DispatchMessage(msg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	if(!((CPconwApp*)AfxGetApp())->m_bClosing) ::MessageBox(((CPconwApp*)AfxGetApp())->m_pMainWnd->m_hWnd,LPCSTR(msg),"Disconnected From Server",MB_ICONSTOP);
	//Close the windows?
	CAsyncSocket::OnClose(nErrorCode);
}
