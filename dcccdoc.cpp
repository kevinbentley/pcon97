/// chatDoc.cpp : implementation of the CDCCCDoc class
// This document class is used for private chat and server messages
//
//

#include "stdafx.h"
#include "pconw.h"

#include "DCCSocket.h"

#include "DCCCDoc.h"
#include "DCCCFrm.h"
#include "DCCCSend.h"
#include "DCCCView.h"

#include "MainFrm.h"


//((CPconwApp*)AfxGetApp())->
/////////////////////////////////////////////////////////////////////////////
// CDCCCDoc

IMPLEMENT_DYNCREATE(CDCCCDoc, CDocument)

BEGIN_MESSAGE_MAP(CDCCCDoc, CDocument)
	//{{AFX_MSG_MAP(CDCCCDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCCDoc construction/destruction

CDCCCDoc::CDCCCDoc(CString& Winname)
{
	m_Winname=Winname;
	//m_objWindows

}

CDCCCDoc::CDCCCDoc()
{
	m_ListenSocket = NULL;
	
	if(((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle.IsEmpty()) ((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle="BADNEWS!";
	m_Winname=((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle;
	
	SetPathName(m_Winname,FALSE);
	m_FmtTitle.Format("DCC Chat %s",m_Winname);
	SetTitle(m_FmtTitle);
    ((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(m_FmtTitle,this);
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_FmtTitle);

	((CMainFrame *)AfxGetMainWnd())->m_TabBar.InsertTab(m_FmtTitle);
	
}


CDCCCDoc::~CDCCCDoc()
{
}

BOOL CDCCCDoc::OnNewDocument()
{

	CString msg;
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	if(((CPconwApp*)AfxGetApp())->m_ChannelDoc==this) //This is the server window....
	{
		msg.LoadString(IDS_CHAT_HELP1);
		((CPconwApp*)AfxGetApp())->DispatchMessage(LPCSTR(msg),((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	}
	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CDCCCDoc serialization


void CDCCCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDCCCDoc diagnostics

#ifdef _DEBUG
void CDCCCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCCCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCCCDoc commands

void CDCCCDoc::OnCloseDocument() 
{
	
	if(((CPconwApp*)AfxGetApp())->m_bClosing==FALSE)
	{
		((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_FmtTitle);
		((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(m_FmtTitle);
		((CMainFrame *)AfxGetMainWnd())->m_TabBar.RemoveTab(m_Winname);
	}
	
	CDocument::OnCloseDocument();
	
}

BOOL CDCCCDoc::SaveModified() 
{
	return TRUE;	
	//return CDocument::SaveModified();
}

void CDCCCDoc::DisplayMsg(LPCTSTR lpszText,DWORD l_Color)
{
	CDCCCView * pDCCCView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
		{
			pDCCCView=(CDCCCView *)pView;
			pDCCCView->SetColor(l_Color);
			pDCCCView->Message(lpszText);			
		}	
	}
	return;
}

void CDCCCDoc::SetSendText(LPCSTR Text)
{
	
#ifndef IRCMODE	
	CString strSendMessage;
	if(((CPconwApp*)AfxGetApp())->m_ChannelDoc!=this)
	{
		if(Text[0]!='/') 
		{
			strSendMessage.Format("/pmsg \"%s\" :%s",m_Winname,Text);
			((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage);
		}
		else 
		{
			strSendMessage=Text;
			
			((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage,LPCSTR(m_Winname));

		}

	}
	
	else 
	{
		strSendMessage=Text;
		((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage);
	}
#endif

#ifdef IRCMODE	
	CString strSendMessage;
	if(Text[0]!='/')
	{
		strSendMessage=Text;
		CDCCCView *pDCCCView;
		for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
		{
			CView* pView = GetNextView(pos);
			
			if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
			{
				pDCCCView=(CDCCCView *)pView;
				pDCCCView->SendMsg(strSendMessage);
			}	
		}
		return;

	}
	else
	{
		CString l_MeTest,l_strTmp;
		l_strTmp=Text;
		l_MeTest=l_strTmp.Left(3);
		if(l_MeTest.CompareNoCase("/me")==0)
		{
			CString l_FmtAction;
			l_FmtAction.Format("\01\ACTION %s\x01",l_strTmp.Right(l_strTmp.GetLength()-4));
			CDCCCView *pDCCCView;
			for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
			{
				CView* pView = GetNextView(pos);
				
				if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
				{
					pDCCCView=(CDCCCView *)pView;
					pDCCCView->SendMsg(l_FmtAction);
				}	
			}
			return;
		}
		strSendMessage=Text;
		((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage);
		return;
	}
	

#endif
	
}

void CDCCCDoc::DeleteContents() 
{
	//	CChatView *pChatView;	

//		CMsg msg;
	//	CString strTemp;
/*
		if (strTemp.LoadString(IDS_DISCONNECT))
		{
			msg.m_bClose = TRUE;
			msg.m_strText = m_strHandle + strTemp;
			msg.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
		}
	
*/	
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		CDCCCView * pDCCCView;
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
		{
			pDCCCView=(CDCCCView *)pView;
			CHARFORMAT cf;
			pDCCCView = (CDCCCView*)pView;
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = ((CPconwApp*)AfxGetApp())->colorSTAT;
			pDCCCView->SetCharFormat(cf);
			pDCCCView->GetRichEditCtrl().SetWindowText(_T(""));
		}	
	}
		
	CDocument::DeleteContents();
	
}

void CDCCCDoc::ActivateSendVw(UINT nchar)
{
	CDCCCSend *pDCCSendV;
	CDCCCFrm *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCSend)))
		{
			pDCCSendV=(CDCCCSend *)pView;
			pDCCSendV->OnMyChar(nchar);
			//((CFrameWnd*)AfxGetMainWnd())->SetActiveView(pSendView,TRUE);
			pChildFrame=(CDCCCFrm *)pDCCSendV->GetParentFrame();
			if(pChildFrame)
			{
				pChildFrame->MDIActivate();
				pDCCSendV->SetForegroundWindow();
			}
		}	
	}
}

void CDCCCDoc::SetNewTitle(LPCSTR Title)
{
	m_Winname=Title;
	SetPathName(m_Winname,FALSE);
	SetTitle(m_Winname);
	
	CDCCCView *pDCCCView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
		{
			pDCCCView=(CDCCCView *)pView;
			pDCCCView->SetNewLogName(LPCSTR(m_Winname));
		}	
	}
	
	
	
}

BOOL CDCCCDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if((((CPconwApp*)AfxGetApp())->m_ChannelDoc==this)&&(((CPconwApp*)AfxGetApp())->m_bClosing==FALSE))
	{
		//MessageBeep(-1);
		return FALSE;
	}
	else 
	{
		return TRUE;
	}
	//return TRUE;
}

void CDCCCDoc::SetNewFont(void)
{
	CDCCCView *pDCCCView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
		{
			pDCCCView=(CDCCCView *)pView;
			pDCCCView->SetNewFont();
		}	
	}
}

void CDCCCDoc::SendSysKey(UINT nchar)
{
	CDCCCSend *pDCCSendV;
	CDCCCFrm *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCSend)))
		{
			pDCCSendV=(CDCCCSend *)pView;
			pDCCSendV->OnMyKeyDown(nchar);			
		}	
	}
}

void CDCCCDoc::SendPageKey(UINT nchar)
{
	CDCCCView *pDCCCView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CDCCCView)))
		{
			pDCCCView=(CDCCCView *)pView;
			pDCCCView->OnMyKeyDown(nchar);
		}	
	}
}

int CDCCCDoc::SetDCCCOptions(int Mode)
{
	//int port;
	//CString l_SockAddr;
	
	//m_ListenSocket = new CDCCSocket(this,DCC_LISTEN);

	//m_ListenSocket->Create(0,SOCK_STREAM,FD_READ|FD_ACCEPT|FD_CONNECT|FD_CLOSE);

	//m_ListenSocket->GetSockName(l_SockAddr,port);

	return 0;
}
