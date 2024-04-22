/// pconwDoc.cpp : implementation of the CPconwDoc class
//

#include "stdafx.h"
#include "pconw.h"

#include "pconwDoc.h"
#include "ChildFrm.h"
#include "pconwView.h"
#include "SendView.h"
#include "ChanUList.h"

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CPconwDoc

IMPLEMENT_DYNCREATE(CPconwDoc, CDocument)

BEGIN_MESSAGE_MAP(CPconwDoc, CDocument)
	//{{AFX_MSG_MAP(CPconwDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPconwDoc construction/destruction

CPconwDoc::CPconwDoc(CString& Winname)
{
	m_Winname=Winname;
	//m_objWindows

}


CPconwDoc::CPconwDoc()
{
	CString msg,l_LowerName;
	if(((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle.IsEmpty()) ((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle="BADNEWS!";
	m_Winname=((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle;
	l_LowerName=m_Winname;
	l_LowerName.MakeLower();
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(l_LowerName,this);
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_Winname);

	((CMainFrame *)AfxGetMainWnd())->m_TabBar.InsertTab(m_Winname);

	
	SetPathName(m_Winname,FALSE);
	SetTitle(m_Winname);

#ifdef IRCMODE
	msg.LoadString(IDS_JOININGCHANNEL);
	((CPconwApp*)AfxGetApp())->DispatchMessage(msg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
#endif	//#ifdef IRCMODE

#ifndef IRCMODE
	
    PCHAT_PACKET_HDR pchatpkthdr;
 
	BYTE sendbuff[100];
//	CString msg;

	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
	pchatpkthdr->type = SIGNON;
	pchatpkthdr->ipaddress = 0;
	strcpy(pchatpkthdr->name, LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
	strcpy(pchatpkthdr->channel,LPCSTR(m_Winname));
	//m_Channel = DefaultChannel;
	if(((CPconwApp*)AfxGetApp())->m_pSocket)((CPconwApp*)AfxGetApp())->m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

	//msg.LoadString(IDS_CHAT_HELP1);
	//((CPconwApp*)AfxGetApp())->DispatchMessage(LPCSTR(msg),((CPconwApp*)AfxGetApp())->colorSTAT);

	pchatpkthdr->type = USERS_RESPOND;
	strcpy(pchatpkthdr->name, LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
	strcpy(pchatpkthdr->channel,LPCSTR(m_Winname));
	//m_Channel = DefaultChannel;
	msg.LoadString(IDS_QUERYING_CHAT);
	((CPconwApp*)AfxGetApp())->DispatchMessage(msg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	if(((CPconwApp*)AfxGetApp())->m_pSocket)((CPconwApp*)AfxGetApp())->m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

#endif //#ifndef IRCMODE

	
}


CPconwDoc::~CPconwDoc()
{
}

BOOL CPconwDoc::OnNewDocument()
{


	if (!CDocument::OnNewDocument())
		return FALSE;
	else 
	{
#ifndef IRCMODE
		
		PCHAT_PACKET_HDR pchatpkthdr;
	 
		BYTE sendbuff[100];
		CString msg;

		pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
		pchatpkthdr->type = SIGNON;
		pchatpkthdr->ipaddress = 0;
		strcpy(pchatpkthdr->name, LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
		strcpy(pchatpkthdr->channel,LPCSTR(m_Winname));
		//m_Channel = DefaultChannel;
		if(((CPconwApp*)AfxGetApp())->m_pSocket)((CPconwApp*)AfxGetApp())->m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

		//msg.LoadString(IDS_CHAT_HELP1);
		//((CPconwApp*)AfxGetApp())->DispatchMessage(LPCSTR(msg),((CPconwApp*)AfxGetApp())->colorSTAT);

		pchatpkthdr->type = USERS_RESPOND;
		strcpy(pchatpkthdr->name, LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
		strcpy(pchatpkthdr->channel,LPCSTR(m_Winname));
		//m_Channel = DefaultChannel;
		msg.LoadString(IDS_QUERYING_CHAT);
		((CPconwApp*)AfxGetApp())->DispatchMessage(msg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
		if(((CPconwApp*)AfxGetApp())->m_pSocket)((CPconwApp*)AfxGetApp())->m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

		return TRUE;
#endif
#ifdef IRCMODE
		return TRUE;
#endif 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPconwDoc serialization


void CPconwDoc::Serialize(CArchive& ar)
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
// CPconwDoc diagnostics

#ifdef _DEBUG
void CPconwDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPconwDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPconwDoc commands

void CPconwDoc::OnCloseDocument() 
{
	CString l_tmpmsglc,tempstr,partmsg,m_Channel;
	partmsg = m_Winname;
	//CString l_tmpmsglc,tempstr,partmsg,m_Channel;
	l_tmpmsglc.LoadString(IDS_STRLVCHAN);
	tempstr = l_tmpmsglc;
	tempstr += m_Winname;
	((CPconwApp*)AfxGetApp())->DispatchMessage(tempstr,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_Winname);
	//((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(m_Winname);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.RemoveTab(m_Winname);

#ifdef IRCMODE
	CString l_StrClose;
	l_StrClose = "/part ";
	l_StrClose += m_Winname;
	((CPconwApp*)AfxGetApp())->SendMsg(l_StrClose,LPCSTR(m_Winname));


#endif
#ifndef IRCMODE
	BYTE sendbuff[1500];
	BYTE localbuff[500];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	
	//partmsg = _T("is leaving channel ");
	partmsg = m_Winname;
	m_Channel=m_Winname;
	pchatpkthdr->type = PART_CHANNEL;
	pchatpkthdr->ipaddress = 0;
	strcpy(pchatpkthdr->name, LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
	strcpy(pchatpkthdr->channel, LPCSTR(m_Winname));
	strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(partmsg));
	pchatpktdata->framedata.textframe.size = partmsg.GetLength()+1;
	((CPconwApp*)AfxGetApp())->m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
	pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
			0x08A9, "127.0.0.1", 0);
		
	
	//if(((CPconwApp*)AfxGetApp())->m_ChannelDoc==this) MessageBeep((UINT)-1);
	
#endif
	POSITION l_pos;
	//Remove from m_ChannelList
	CString l_strLower=m_Winname;
	l_strLower.MakeLower();
	if(m_Winname.GetLength())
	{
		l_pos=((CPconwApp*)AfxGetApp())->m_ChannelList.Find(l_strLower);
	}
	else
	{
		return;
	}
	if(!((CPconwApp*)AfxGetApp())->m_bClosing)	
	{
		//if(l_pos!=NULL) ((CPconwApp*)AfxGetApp())->m_ChannelList.RemoveAt(l_pos);

	}
		
	
	l_tmpmsglc.LoadString(IDS_STRLVCHAN);
	tempstr = l_tmpmsglc;
	tempstr += m_Winname;
	//((CPconwApp*)AfxGetApp())->DispatchMessage(tempstr,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
	//partmsg = _T("is leaving channel ");
	
	//((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_Winname);
	CDocument::OnCloseDocument();
}

BOOL CPconwDoc::SaveModified() 
{
	return TRUE;	
	//return CDocument::SaveModified();
}

void CPconwDoc::DisplayMsg(LPCTSTR lpszText,DWORD l_Color)
{
	CPconwView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CPconwView)))
		{
			pChatView=(CPconwView *)pView;
			pChatView->SetColor(l_Color);
			pChatView->Message(lpszText);			
		}	
	}
	return;
}

void CPconwDoc::SetSendText(LPCSTR Text)
{
	CString strSendMessage;

	strSendMessage=Text;
	((CPconwApp*)AfxGetApp())->SendMsg(strSendMessage,LPCSTR(m_Winname));

}

void CPconwDoc::DeleteContents() 
{
	//	CPconwView *pChatView;	

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
/*	
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		CPconwView * pChatView;
		if (pView->IsKindOf(RUNTIME_CLASS(CPconwView)))
		{
			pChatView=(CPconwView *)pView;
			CHARFORMAT cf;
			CPconwView* pChatView = (CPconwView*)pView;
			cf.dwMask = CFM_COLOR;
			cf.crTextColor = ((CPconwApp*)AfxGetApp())->colorSTAT;
			pChatView->SetCharFormat(cf);
			pChatView->GetRichEditCtrl().SetWindowText(_T(""));
		}	
	}
*/			
	CDocument::DeleteContents();
	
}

void CPconwDoc::ActivateSendVw(UINT nchar)
{
	CSendView *pSendView;
	CChildFrame *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CSendView)))
		{
			pSendView=(CSendView *)pView;
			pSendView->OnMyChar(nchar);
			//((CFrameWnd*)AfxGetMainWnd())->SetActiveView(pSendView,TRUE);
			pChildFrame=(CChildFrame *)pSendView->GetParentFrame();
			if(pChildFrame)
			{
				pChildFrame->MDIActivate();
				pSendView->SetForegroundWindow();
			}
		}	//CControlBar
	}
}

void CPconwDoc::SetNewTitle(LPCSTR Title,CString StrippedTitle)
{
	m_Title=StrippedTitle;
	SetPathName(m_Winname,FALSE);
	SetTitle(Title);
	/*
	CPconwView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CPconwView)))
		{
			pChatView=(CPconwView *)pView;
			//pChatView->SetNewLogName(Title);
		}	
	}
	*/
	
	
}

BOOL CPconwDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	return TRUE;
	//if((((CPconwApp*)AfxGetApp())->m_ChannelDoc==this)&&(((CPconwApp*)AfxGetApp())->m_bClosing==FALSE)) return FALSE;
	//else return TRUE;
}

void CPconwDoc::SetNewFont(void)
{
	CPconwView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CPconwView)))
		{
			pChatView=(CPconwView *)pView;
			pChatView->SetNewFont();
		}	
	}
}

void CPconwDoc::SendSysKey(UINT nchar)
{
	CSendView *pSendView;
	CChildFrame *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CSendView)))
		{
			pSendView=(CSendView *)pView;
			pSendView->OnMyKeyDown(nchar);			
		}	
	}
}

void CPconwDoc::SendPageKey(UINT nchar)
{
	CPconwView *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CPconwView)))
		{
			pChatView=(CPconwView *)pView;
			pChatView->OnMyKeyDown(nchar);
		}	
	}
}

void CPconwDoc::UserJoin(LPCSTR User)
{
    //Need to see if they 
	
	CChanUList *pChatView;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChanUList)))
		{
			pChatView=(CChanUList *)pView;
			pChatView->AddChanUser(User);
		}	
	}
}

BOOL CPconwDoc::UserPart(LPCSTR User)
{
    CChanUList *pChatView;
	BOOL l_RetCode;
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChanUList)))
		{
			pChatView=(CChanUList *)pView;
			l_RetCode = pChatView->DelChanUser(User);
			return l_RetCode;
		}	
	}
	return FALSE;
}

void CPconwDoc::ChangeUserMode(CString & Modes, CString & Users)
{
	CChanUList *pChatView;
	
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChanUList)))
		{
			pChatView=(CChanUList *)pView;
			pChatView->ChangeUserMode(Modes,Users);
		}	
	}
}

void CPconwDoc::ChangeNick(CString & p_newnick, CString & p_oldnick)
{
	CChanUList *pChatView;
	
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChanUList)))
		{
			pChatView=(CChanUList *)pView;
			pChatView->ChangeNick(p_newnick,p_oldnick);
		}	
	}
}

BOOL CPconwDoc::IsUserInChannel(CString & User)
{
	CChanUList *pChatView;
	BOOL l_bInChan;
	
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChanUList)))
		{
			pChatView = (CChanUList *)pView;
			l_bInChan= pChatView->IsUserHere(User);
			return l_bInChan;
		}	
	}
	return FALSE;
}

void CPconwDoc::SetChannelMode(CString Mode)
{
	CString l_Title;
	l_Title.Format("%s[%s] - Topic: %s",m_Winname,Mode,m_Title);
	m_ChanMode=Mode;
	SetNewTitle(LPCSTR(l_Title),m_Title);
}
