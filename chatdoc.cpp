/// chatDoc.cpp : implementation of the CChatDoc class
// This document class is used for private chat and server messages
//
//

#include "stdafx.h"
#include "pconw.h"

#include "chatDoc.h"
#include "ChatFrm.h"
#include "Sendchat.h"
#include "chatView.h"

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CChatDoc

IMPLEMENT_DYNCREATE(CChatDoc, CDocument)

BEGIN_MESSAGE_MAP(CChatDoc, CDocument)
	//{{AFX_MSG_MAP(CChatDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDoc construction/destruction

CChatDoc::CChatDoc(CString& Winname)
{
	m_Winname=Winname;
	//m_objWindows

}

CChatDoc::CChatDoc()
{
	if(((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle.IsEmpty()) ((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle="BADNEWS!";
	m_Winname=((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle;
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(m_Winname,this);
	SetPathName(m_Winname,FALSE);
	SetTitle(m_Winname);
    //((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_Winname);

	((CMainFrame *)AfxGetMainWnd())->m_TabBar.InsertTab(m_Winname);
	
}


CChatDoc::~CChatDoc()
{
}

BOOL CChatDoc::OnNewDocument()
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
// CChatDoc serialization


void CChatDoc::Serialize(CArchive& ar)
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
// CChatDoc diagnostics

#ifdef _DEBUG
void CChatDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChatDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatDoc commands

void CChatDoc::OnCloseDocument() 
{
	
	if(((CPconwApp*)AfxGetApp())->m_bClosing==FALSE)
	{
		((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_Winname);
		((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(m_Winname);
		((CMainFrame *)AfxGetMainWnd())->m_TabBar.RemoveTab(m_Winname);
	}
	
	CDocument::OnCloseDocument();
	
}

BOOL CChatDoc::SaveModified() 
{
	return TRUE;	
	//return CDocument::SaveModified();
}

void CChatDoc::DisplayMsg(LPCTSTR lpszText,DWORD l_Color)
{
	CChatView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			pChatView=(CChatView *)pView;
			pChatView->SetColor(l_Color);
			pChatView->Message(lpszText);			
		}	
	}
	return;
}

void CChatDoc::SetSendText(LPCSTR Text)
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
	if(((CPconwApp*)AfxGetApp())->m_ChannelDoc!=this)
	{
		strSendMessage=Text;
		((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage,LPCSTR(m_Winname));
	}
	else 
	{
		strSendMessage=Text;
		((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage);
	}
#endif
	
}

void CChatDoc::DeleteContents() 
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
		CChatView * pChatView;
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			pChatView=(CChatView *)pView;
			CHARFORMAT cf;
			CChatView* pChatView = (CChatView*)pView;
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = ((CPconwApp*)AfxGetApp())->colorSTAT;
			pChatView->SetCharFormat(cf);
			pChatView->GetRichEditCtrl().SetWindowText(_T(""));
		}	
	}
		
	CDocument::DeleteContents();
	
}

void CChatDoc::ActivateSendVw(UINT nchar)
{
	CSendChat *pSendView;
	CChatFrame *pChildFrame;


	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CSendChat)))
		{
			pSendView=(CSendChat *)pView;
			pSendView->OnMyChar(nchar);
			//((CFrameWnd*)AfxGetMainWnd())->SetActiveView(pSendView,TRUE);
			pChildFrame=(CChatFrame *)pSendView->GetParentFrame();
			if(pChildFrame)
			{
				pChildFrame->MDIActivate();
				pSendView->SetForegroundWindow();
			}
		}	
	}
}

void CChatDoc::SetNewTitle(LPCSTR Title)
{
	m_Winname=Title;
	SetPathName(m_Winname,FALSE);
	SetTitle(m_Winname);
	
	CChatView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			pChatView=(CChatView *)pView;
			pChatView->SetNewLogName(LPCSTR(m_Winname));
		}	
	}
	
	
	
}

BOOL CChatDoc::CanCloseFrame(CFrameWnd* pFrame) 
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

void CChatDoc::SetNewFont(void)
{
	CChatView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			pChatView=(CChatView *)pView;
			pChatView->SetNewFont();
		}	
	}
}

void CChatDoc::SendSysKey(UINT nchar)
{
	CSendChat *pSendView;
	CChatFrame *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CSendChat)))
		{
			pSendView=(CSendChat *)pView;
			pSendView->OnMyKeyDown(nchar);			
		}	
	}
}

void CChatDoc::SendPageKey(UINT nchar)
{
	CChatView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			pChatView=(CChatView *)pView;
			pChatView->OnMyKeyDown(nchar);
		}	
	}
}
