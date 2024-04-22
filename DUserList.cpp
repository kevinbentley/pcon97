// DUserList.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "GetFileDlg.h"
#include "SendPrvMsg.h"
#include "DUserList.h"



/////////////////////////////////////////////////////////////////////////////
// CDUserList dialog


CDUserList::CDUserList()
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDUserList)
	//}}AFX_DATA_INIT
}


void CDUserList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDUserList)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDUserList, CDialogBar)
	//{{AFX_MSG_MAP(CDUserList)
		ON_COMMAND(IDC_PING, OnPing)
		ON_COMMAND(IDC_SENDMSG, OnSendmsg)
		ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
		ON_COMMAND(ID_POPUP_TOGGLEIGNORE, OnToggleIgnore)
		ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
		ON_COMMAND(IDC_GETUSERFILE, OnGetuserfile)
		ON_COMMAND(IDC_WHOISUSER, OnWhoisuser)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDC_WHOISUSER, OnUpdateWhoisuser)
	ON_UPDATE_COMMAND_UI(IDC_SENDMSG, OnUpdateSendmsg)
	ON_UPDATE_COMMAND_UI(IDC_PING, OnUpdatePing)
	ON_UPDATE_COMMAND_UI(IDC_GETUSERFILE, OnUpdateGetuserfile)
	ON_UPDATE_COMMAND_UI(ID_POPUP_TOGGLEIGNORE, OnUpdatePopupToggleignore)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDUserList message handlers
void CDUserList::AddItem (char *Item)
{
	CListBox *l_userlistbox;
	//m_lbUsers.AddString(Item);
	//TRACE("IDC_LIST1 = %x",IDC_LIST1);
	l_userlistbox=(CListBox*)GetDlgItem(IDC_LIST1);
	//TRACE("l_userlistbox=%X",l_userlistbox);
	if(l_userlistbox) l_userlistbox->AddString(Item);
}

void CDUserList::RemoveExactItem (char *Item)
{
	((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,Item));
}

void CDUserList::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component
	int ilb_Result;

	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR==ilb_Result) return;
	
	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_USER_LIST));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	//while (pWndPopupOwner->GetStyle() & WS_CHILD)
	//	pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL CDUserList::PreTranslateMessage(MSG* pMsg)
{
	// CG: This block was added by the Pop-up Menu component
	{
		// Shift+F10: show pop-up menu.
		if ((((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) && // If we hit a key and
			(pMsg->wParam == VK_F10) && (GetKeyState(VK_SHIFT) & ~1)) != 0) ||	// it's Shift+F10 OR
			(pMsg->message == WM_CONTEXTMENU))									// Natural keyboard key
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}
		if(pMsg->message==WM_RBUTTONDOWN)
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);
			CPoint pt(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
            pt.Offset(rect.TopLeft());
			OnContextMenu (this,pt);

		}
	}
/*	if(pMsg->message==WM_SYSKEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_F2:
				OnKeyDown(pMsg->wParam,1,0);
				return TRUE;

			case VK_F3:
				OnKeyDown(pMsg->wParam,1,0);
				return TRUE;
			default:
				break;
			}

			
		}	
		*/

	return CDialogBar::PreTranslateMessage(pMsg);
}

void CDUserList::OnPing() 
{
	CString cs_PingCommand,cs_UserName;
	CString l_Ping;
	l_Ping.LoadString(IDS_CMDPING);
	int ilb_Result;
	
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	cs_PingCommand = _T("/");
	cs_PingCommand += l_Ping;
	cs_PingCommand += _T(" \"");
	cs_PingCommand += cs_UserName;
	cs_PingCommand += _T("\"");
	((CPconwApp*)AfxGetApp())->SendMsg(cs_PingCommand);

}

void CDUserList::OnSendmsg() 
{
	
	
	CString cs_MsgCommand,cs_UserName;
	SendPrvMsg DlgPrvMsg;
	int ilb_Result;
	
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	
	 ((CPconwApp*)AfxGetApp())->OpenQueryWindow(LPCSTR(cs_UserName));

	/*
    cs_MsgCommand = _T("/msg \"");
	cs_MsgCommand += cs_UserName;
	cs_MsgCommand += _T("\" ");
	DlgPrvMsg.DoModal();
	if(DlgPrvMsg.m_Message.IsEmpty()) return;
	else cs_MsgCommand += DlgPrvMsg.m_Message;

	//ThisDocument = ((CChatDoc *)((CPconwApp*)AfxGetApp())->m_MainDoc);
	//if(!ThisDocument) return;
	//ThisDocument->SetSendText((char *)LPCSTR(cs_MsgCommand));
	//ThisDocument->ActivateSendVw(0);
	((CPconwApp*)AfxGetApp())->SendMsg(cs_MsgCommand);
	*/
}

void CDUserList::OnDblclkList1() 
{
	OnSendmsg();
}


void CDUserList::OnToggleIgnore() 
{
	CString cs_UserName,IgnoreMsg,l_Ignore;
	l_Ignore.LoadString(IDS_CMDIGNORE);
	//CChatDoc *ThisDocument;
	int ilb_Result;

	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	//ThisDocument = ((CChatDoc *)((CPconwApp*)AfxGetApp())->m_MainDoc);
	//if(!ThisDocument) return;
	IgnoreMsg = _T("/");
	IgnoreMsg += l_Ignore;
	IgnoreMsg += _T(" \"");
	IgnoreMsg += cs_UserName;	
	IgnoreMsg += _T("\"");
	//ThisDocument->SetSendText((char *)LPCSTR(IgnoreMsg));
	//ThisDocument->ActivateSendVw(0);
	((CPconwApp*)AfxGetApp())->SendMsg(IgnoreMsg);

}

void CDUserList::OnSelchangeList1() 
{
	int ilb_Result;
	//CChatDoc *ThisDocument;
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,((CPconwApp*)AfxGetApp())->CurrSelUser);
	else return;
	if(((CPconwApp*)AfxGetApp())->CurrSelUser.Compare("$USER")==0) ((CPconwApp*)AfxGetApp())->CurrSelUser=_T("LUSER");
	//ThisDocument = ((CChatDoc *)((CPconwApp*)AfxGetApp())->m_MainDoc);
	//if(!ThisDocument) return;
	//ThisDocument->ActivateSendVw(0);
	//if(((CPconwApp*)AfxGetApp())->m_MainChannelWnd) ((CPconwApp*)AfxGetApp())->m_MainChannelWnd->BringWindowToTop();
	((CPconwApp*)AfxGetApp())->ActivateMainWnd();
	
}

void CDUserList::OnGetuserfile() 
{
	//CChatDoc *ThisDocument;
	CGetFileDlg l_GetFileDlg;
	if(IDCANCEL==l_GetFileDlg.DoModal()) return;
	CString cs_MsgCommand,l_Get;
	l_Get.LoadString(IDS_CMDGET);
	
	if(l_GetFileDlg.m_GetFile1Name.IsEmpty()) return;
	
	cs_MsgCommand =_T("/");
	cs_MsgCommand+=l_Get;
	cs_MsgCommand+=_T(" \"");
	cs_MsgCommand+=((CPconwApp*)AfxGetApp())->CurrSelUser;
	cs_MsgCommand+=_T("\" ");
	cs_MsgCommand+=l_GetFileDlg.m_GetNum;
	cs_MsgCommand+=_T(" ");
	cs_MsgCommand+=l_GetFileDlg.m_GetFile1Name;
	
	((CPconwApp*)AfxGetApp())->SendMsg(cs_MsgCommand);
}

void CDUserList::OnWhoisuser() 
{
	CString cs_WhoisCommand,cs_UserName,l_Whois;
	l_Whois.LoadString(IDS_CMDWHOIS);
	int ilb_Result;
	
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	cs_WhoisCommand = _T("/");
	cs_WhoisCommand += l_Whois;
	cs_WhoisCommand += _T(" \"");
	cs_WhoisCommand += cs_UserName;
	cs_WhoisCommand += _T("\"");
	((CPconwApp*)AfxGetApp())->SendMsg(cs_WhoisCommand);
}

void CDUserList::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);	
//	if(::IsWindow(GetDlgItem(IDC_LIST1)->m_hWnd)) ((CListBox*)GetDlgItem(IDC_LIST1))->MoveWindow(0,0,cx,cy);	
}


void CDUserList::OnUpdateWhoisuser(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDUserList::OnUpdateSendmsg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDUserList::OnUpdatePing(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDUserList::OnUpdateGetuserfile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDUserList::OnUpdatePopupToggleignore(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

CString CDUserList::ExpandString(CString OrigStr)
{
	
	CString l_User;
	l_User.LoadString(IDS_KEYUSER);
	m_TmpStr = _T("");
	int iStart;

	iStart=OrigStr.Find(l_User);
	if(iStart==-1) 
	{
		m_TmpStr = OrigStr;
		return m_TmpStr;
	}

	m_TmpStr=OrigStr.Left(iStart);
	m_TmpStr += ((CPconwApp*)AfxGetApp())->CurrSelUser;
    m_TmpStr += OrigStr.Mid(iStart+l_User.GetLength());

	return m_TmpStr;

}



void CDUserList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CPconwDoc* pDoc;
	pDoc=(CPconwDoc*)((CPconwApp*)AfxGetApp())->m_ChannelDoc;
	
	if(nChar == VK_F2) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F2key));				
			return;
		}
	if(nChar == VK_F3) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F3key));				
			return;
		}
	if(nChar == VK_F4) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F4key));				
			return;
		}
	if(nChar == VK_F5) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F5key));				
			return;
		}
	if(nChar == VK_F6) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F6key));				
			return;
		}
	if(nChar == VK_F7) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F7key));				
			return;
		}
	if(nChar == VK_F8) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F8key));				
			return;
		}
	if(nChar == VK_F9) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F9key));				
			return;
		}
	if(nChar == VK_F10) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F10key));				
			return;
		}
	if(nChar == VK_F11) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F11key));				
			return;
		}
	if(nChar == VK_F12) 
		{
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F12key));				
			return;
		}
	
	CDialogBar::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDUserList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialogBar::OnNotify(wParam, lParam, pResult);
}

BOOL CDUserList::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if(message==WM_SYSKEYDOWN)
	{
		switch(wParam)
		{
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
			CPconwDoc* ThisDocument;
			ThisDocument=(CPconwDoc*)((CPconwApp*)AfxGetApp())->m_ChannelDoc;
			ThisDocument->SendSysKey(wParam);
			return CDialogBar::OnChildNotify(message, wParam, lParam, pLResult);
		default:
			break;

		}
	}
	
	return CDialogBar::OnChildNotify(message, wParam, lParam, pLResult);
}
