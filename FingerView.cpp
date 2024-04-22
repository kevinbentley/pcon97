// FingerView.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "FingerSocket.h"
#include "FingerView.h"
#include "FingerDoc.h"


#define WM_USER_HOSTLOOKUP	WM_USER+142
/////////////////////////////////////////////////////////////////////////////
// CFingerView

IMPLEMENT_DYNCREATE(CFingerView, CRichEditView)

CFingerView::CFingerView()
{
	m_URLCursor=FALSE;
    l_hc = ((CPconwApp*)AfxGetApp())->LoadCursor(IDC_CURURL);//IDC_CURURL//SetCursor
}

CFingerView::~CFingerView()
{
}

BEGIN_MESSAGE_MAP(CFingerView, CRichEditView)
	//{{AFX_MSG_MAP(CFingerView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerView diagnostics

#ifdef _DEBUG
void CFingerView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CFingerView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFingerView message handlers

void CFingerView::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
	CFingerDoc * l_Doc;
	CString l_UserHost,l_strtmp;
	int l_iAtSign;
	//m_strUser
	//m_strHost
	
	l_Doc=(CFingerDoc *)GetDocument();
	l_UserHost=l_Doc->m_strFinger;
	l_strtmp=l_UserHost.Right(l_UserHost.GetLength()-8);
	l_UserHost=l_strtmp;
	l_iAtSign=l_UserHost.Find("@");
	//parse the finger line
	if(l_iAtSign>=0)
	{
		m_strUser=l_UserHost.Left(l_iAtSign);
		m_strHost=l_UserHost.Right(l_UserHost.GetLength()-l_iAtSign-1);
	}
	else
	{
		//whoops!
	}
	m_FingerSocket = new CFingerSocket(this);
	//Create TCP socket 
	m_FingerSocket->Create();
	//connect to the host....
	HANDLE l_test = WSAAsyncGetHostByName(m_hWnd,WM_USER_HOSTLOOKUP,LPCSTR(m_strHost),m_szHostBuffer,MAXGETHOSTSTRUCT);
	
	//ASSERT(l_test);
	
	
	
}

void CFingerView::ConnectFinger()
{
	char szUser[255];
	m_strUser.TrimRight();
	sprintf(szUser,"%s\r\n",LPCSTR(m_strUser));
	//send the user part of the finger line
	
	m_FingerSocket->Send(szUser,strlen(szUser)+2);
}

void CFingerView::ReceiveFinger()
{
	
	//send the data to the view
	char l_BuffA[2000];
	int l_Len;
	l_Len=m_FingerSocket->Receive(&l_BuffA,1999);
	if(l_Len>=1)
	{
		//
		int l_ilen;
		l_BuffA[l_Len]=0;
		l_ilen = GetWindowTextLength();
		GetRichEditCtrl().SetSel(l_ilen,l_ilen);
		GetRichEditCtrl().ReplaceSel(l_BuffA);
		UnderlineURLs(l_ilen,l_BuffA);
		//CSocket
	}

}

BOOL CFingerView::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_USER_HOSTLOOKUP)
	{
		
		HOSTENT* Hostent;
		struct in_addr IpAddressStruct;
		Hostent = (HOSTENT *) m_szHostBuffer;
		if ( Hostent->h_addrtype == AF_INET )
        {
                memcpy(&IpAddressStruct.S_un.S_addr,Hostent->h_addr,4);
				char l_szIP[200];
				strcpy(l_szIP,inet_ntoa(IpAddressStruct));
				if(m_FingerSocket->Connect(l_szIP,79))
				{
					//Ooops
				}

		}
		
		return FALSE;
	}
	
	return CRichEditView::PreTranslateMessage(pMsg);
}


void CFingerView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// No char proccessing for us.....
	
	//CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CFingerView::CloseFinger()
{
	int l_ilen;
	
	l_ilen = GetWindowTextLength();
	GetRichEditCtrl().SetSel(l_ilen,l_ilen);
	GetRichEditCtrl().ReplaceSel("\nHost connection closed.");
}

void CFingerView::OnDestroy() 
{
	CRichEditView::OnDestroy();
	
	delete m_FingerSocket;
	
}


#define D_VIEW_CLASS CFingerView
//UnderlineURL and OnLButtonDown functions included in the file below.
#include "UnderlineURL.hpp"


