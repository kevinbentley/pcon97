// ChanUList.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "ChanUList.h"
#include "GetFileDlg.h"
#include "resource.h"
#include "cuser.h"
#include "KickPrompt.h"
#include "SendView.h"
#include "ChildFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CChanUList

IMPLEMENT_DYNCREATE(CChanUList, CFormView)

CChanUList::CChanUList()
	: CFormView(CChanUList::IDD)
{
	//{{AFX_DATA_INIT(CChanUList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_bInit=TRUE;
	//m_hiconUnknown = (HICON) AfxGetApp()->LoadIcon(IDR_PCONWTYPE);//LoadImage(AfxGetInstanceHandle(),"d:\\dev\\pcon97\\res\\pconwdoc.ico",IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	//m_hiconUnknown = (HICON) LoadImage(AfxGetInstanceHandle(),"d:\\dev\\pcon97\\res\\pconwdoc.ico",IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	//ASSERT(m_hiconUnknown);
	m_LagBitmaps.LoadBitmap(IDR_LAGOMETER);
	m_dcCompatible.CreateCompatibleDC(NULL);
	m_dcCompatible.SelectObject(&m_LagBitmaps);
	

/*	
HANDLE LoadImage( HINSTANCE hinst, 
 // handle of the instance that contains the image 
 
LPCTSTR lpszName, 
 // name or identifier of image 
 
UINT uType, 
 // type of image 
 
int cxDesired, 
 // desired width 
 
int cyDesired, 
 // desired height 
 
UINT fuLoad 
 // load flags 
 
); 
 

CDC

CBitMap



*/
}

CChanUList::~CChanUList()
{
	m_LagBitmaps.DeleteObject();
}

void CChanUList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChanUList)
	DDX_Control(pDX, IDC_LIST1, m_lbUserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChanUList, CFormView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CChanUList)
	ON_WM_SIZE()
	ON_COMMAND(IDC_WHOISUSER, OnWhoisuser)
	ON_COMMAND(IDC_SENDMSG, OnSendmsg)
	ON_COMMAND(IDC_PING, OnPing)
	ON_COMMAND(IDC_GETUSERFILE, OnGetuserfile)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_COMMAND(ID_POPUP_TOGGLEIGNORE, OnPopupToggleignore)
	ON_UPDATE_COMMAND_UI(ID_POPUP_TOGGLEIGNORE, OnUpdatePopupToggleignore)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_TAKEOPS, OnUpdateTakeops)
	ON_UPDATE_COMMAND_UI(ID_TAKEVOICE, OnUpdateTakevoice)
	ON_UPDATE_COMMAND_UI(ID_GRANTOPS, OnUpdateGrantops)
	ON_UPDATE_COMMAND_UI(ID_GRANTVOICE, OnUpdateGrantvoice)
	ON_UPDATE_COMMAND_UI(ID_KICKUSER, OnUpdateKickuser)
	ON_COMMAND(IDM_DCCCHAT, OnDccchat)
	ON_COMMAND(IDM_DCCSEND, OnDccsend)
	ON_COMMAND(IDM_UDPPING, OnUdpping)
	ON_COMMAND(IDM_IGNOREUSER, OnIgnoreuser)
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_GRANTOPS,GrantOps)
	ON_COMMAND(ID_GRANTVOICE,GrantVoice)
	ON_COMMAND(ID_TAKEOPS,TakeOps)
	ON_COMMAND(ID_TAKEVOICE,TakeVoice)
	ON_COMMAND(ID_KICKUSER, KickUser)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanUList diagnostics

#ifdef _DEBUG
void CChanUList::AssertValid() const
{
	CFormView::AssertValid();
}

void CChanUList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChanUList message handlers

void CChanUList::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
    if(IsWindow(m_lbUserList.m_hWnd)) m_lbUserList.MoveWindow(0,0,cx-10,cy-10,TRUE);
/*
    DWORD l_lpSize;
    l_lpSize=(cy*0xffff)+cx;
    int blaha=LOWORD(l_lpSize);
    blaha=HIWORD(l_lpSize);
    //CListBox
    m_lbUserList.SendMessage(WM_SIZE,nType,l_lpSize);
    
 


    DWORD l_lpSize;
    l_lpSize=(cx*0xffff)+cy;
    int blaha=LOWORD(l_lpSize);
    blaha=HIWORD(l_lpSize);
    SendDlgItemMessage(IDC_LIST1,WM_SIZE,nType,l_lpSize);
    CListBox *l_Userlist;
    l_Userlist = (CListBox *)GetDlgItem(IDC_LIST1);
    if(IsWindow(l_Userlist->m_hWnd))
    {
        DWORD l_lpSize;
        l_lpSize=(cx*0xffff)+cy;
        int blaha=LOWORD(l_lpSize);
        blaha=HIWORD(l_lpSize);
        l_Userlist->SendMessage(WM_SIZE,nType,l_lpSize);
    }
*/	
}

void CChanUList::AddChanUser(LPCSTR User)
{
#ifndef IRCMODE
    if(LB_ERR==m_lbUserList.FindStringExact(0,User)) m_lbUserList.AddString(User);
#endif
#ifdef IRCMODE
	CString l_StrRealNick,l_StrNick,l_StrNickV,l_StrNickO;
	l_StrRealNick = User;
	if((l_StrRealNick[0]=='@')||(l_StrRealNick[0]=='+'))
	{
		l_StrNick = l_StrRealNick.Right(l_StrRealNick.GetLength()-1); 
		l_StrNickV = "+"+l_StrNick;
		l_StrNickO = "@"+l_StrNick;
		if((LB_ERR==m_lbUserList.FindStringExact(0,l_StrNick)) &&(LB_ERR==m_lbUserList.FindStringExact(0,l_StrNickO)) &&(LB_ERR==m_lbUserList.FindStringExact(0,l_StrNickV)))
		{
			m_lbUserList.AddString(User);
			return;
		}
	}
	if(LB_ERR==m_lbUserList.FindStringExact(0,User)) m_lbUserList.AddString(User);

#endif
}

BOOL CChanUList::DelChanUser(LPCSTR User)
{
#ifndef IRCMODE	
    m_lbUserList.DeleteString(m_lbUserList.FindStringExact(0,User));
	return TRUE;
#endif

#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",User);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);
		return TRUE;

	}
	else 
	{
		l_strTest.Format("+%s",User);
		l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
		if(l_lbResult!=LB_ERR)
		{
			((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);
			return TRUE;
	
		}
		else
		{
			l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,User);
			if(l_lbResult!=LB_ERR)
			{
				((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);		
				return TRUE;
			}
		}
	}
	return FALSE;
#endif
}

void CChanUList::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_CHAN_ULIST));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL CChanUList::PreTranslateMessage(MSG* pMsg)
{
	// CG: This block was added by the Pop-up Menu component
	{
		// Shift+F10: show pop-up menu.
		//if ((((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) && // If we hit a key and
		//	(pMsg->wParam == VK_F10) && (GetKeyState(VK_SHIFT) & ~1)) != 0) ||	// it's Shift+F10 OR
		if(	(pMsg->message == WM_CONTEXTMENU))									// Natural keyboard key
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}
		/*else
		{
			if((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)&&
				(
					(pMsg->wParam == VK_F2)||
					(pMsg->wParam == VK_F3)||
					(pMsg->wParam == VK_F4)||
					(pMsg->wParam == VK_F5)||
					(pMsg->wParam == VK_F6)||
					(pMsg->wParam == VK_F7)||
					(pMsg->wParam == VK_F8)||
					(pMsg->wParam == VK_F9)||
					(pMsg->wParam == VK_F10)||
					(pMsg->wParam == VK_F11)||
					(pMsg->wParam == VK_F12)
				)				
				)
			{
				//Send the message to the send text window.
				
			}
		}*/
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CChanUList::OnWhoisuser() 
{
	CString cs_WhoisCommand,cs_UserName,l_Whois;
	l_Whois.LoadString(IDS_CMDWHOIS);
	int ilb_Result;
#ifdef	IRCMODE
//((CPconwDoc *)GetDocument())->m_SelectedString
	cs_WhoisCommand = _T("/");
	cs_WhoisCommand += l_Whois;
	cs_WhoisCommand += " ";
	cs_WhoisCommand += ((CPconwDoc *)GetDocument())->m_SelectedString;
	((CPconwApp*)AfxGetApp())->SendMsg(cs_WhoisCommand);
#endif
#ifndef	IRCMODE
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	cs_WhoisCommand = _T("/");
	cs_WhoisCommand += l_Whois;
	cs_WhoisCommand += _T(" \"");
	cs_WhoisCommand += cs_UserName;
	cs_WhoisCommand += _T("\"");
	((CPconwApp*)AfxGetApp())->SendMsg(cs_WhoisCommand);
#endif
	
}

void CChanUList::OnSendmsg() 
{
//	CString cs_MsgCommand,cs_UserName;
	//SendPrvMsg DlgPrvMsg;
	//int ilb_Result;
	
	//ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	//if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	//else return;
	
    ((CPconwApp*)AfxGetApp())->OpenQueryWindow(LPCSTR(((CPconwDoc *)GetDocument())->m_SelectedString));

	
}

void CChanUList::OnPing() 
{
	CString cs_PingCommand,cs_UserName;
	CString l_Ping;
	l_Ping.LoadString(IDS_CMDPING);
	int ilb_Result;
#ifdef IRCMODE
	//ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	//if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	//else return;
	//((CPconwDoc *)GetDocument())->m_SelectedString
	cs_PingCommand = _T("/");
	cs_PingCommand += l_Ping;
	cs_PingCommand += " ";
	cs_PingCommand += ((CPconwDoc *)GetDocument())->m_SelectedString;
	((CPconwApp*)AfxGetApp())->SendMsg(cs_PingCommand);
#endif
#ifndef	IRCMODE

	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;
	cs_PingCommand = _T("/");
	cs_PingCommand += l_Ping;
	cs_PingCommand += _T(" \"");
	cs_PingCommand += cs_UserName;
	cs_PingCommand += _T("\"");
	((CPconwApp*)AfxGetApp())->SendMsg(cs_PingCommand);

#endif
	
}

void CChanUList::OnGetuserfile() 
{
	CGetFileDlg l_GetFileDlg;
	if(IDCANCEL==l_GetFileDlg.DoModal()) return;
	CString cs_MsgCommand,l_Get;
	l_Get.LoadString(IDS_CMDGET);
	
	if(l_GetFileDlg.m_GetFile1Name.IsEmpty()) return;
	
	cs_MsgCommand =_T("/");
	cs_MsgCommand+=l_Get;
	cs_MsgCommand+=_T(" \"");
	cs_MsgCommand+=((CPconwDoc *)GetDocument())->m_SelectedString;
	cs_MsgCommand+=_T("\" ");
	cs_MsgCommand+=l_GetFileDlg.m_GetNum;
	cs_MsgCommand+=_T(" ");
	cs_MsgCommand+=l_GetFileDlg.m_GetFile1Name;
	
	((CPconwApp*)AfxGetApp())->SendMsg(cs_MsgCommand);
	
}

void CChanUList::OnDblclkList1() 
{
	OnSendmsg();
	
}

void CChanUList::OnPopupToggleignore() 
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

void CChanUList::OnUpdatePopupToggleignore(CCmdUI* pCmdUI) 
{
    CUser *uFrom;	
    CString cs_UserName;
    int ilb_Result;

    ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) ((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,cs_UserName);
	else return;


	if(((CPconwApp*)AfxGetApp())->Users.Lookup(cs_UserName, (class CObject*&)uFrom))
		{
		if(uFrom->IsIgnored()) pCmdUI->SetCheck(TRUE);
        else pCmdUI->SetCheck(FALSE);
		}	
}

void CChanUList::OnSelchangeList1() 
{
#ifdef IRCMODE	
    int ilb_Result;
	CString l_tmpkey,l_StrRealNick;
	CSendView *pSendView;
	CChildFrame *pChildFrame;
	pSendView=NULL;
	for(POSITION pos=GetDocument()->GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetDocument()->GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CSendView)))
		{
			pSendView=(CSendView *)pView;
			break;
		}	
	}

	ASSERT(pSendView);
    l_tmpkey.LoadString(IDS_KEYUSER);
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) 
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,l_StrRealNick);
		if(l_StrRealNick[0]=='@')
		{
			((CPconwDoc *)GetDocument())->m_SelectedString = l_StrRealNick.Right(l_StrRealNick.GetLength()-1)	;
			if(((CPconwDoc *)GetDocument())->m_SelectedString.Compare(l_tmpkey)==0) ((CPconwDoc *)GetDocument())->m_SelectedString=_T("LUSER");
			pSendView->SetFocus();			
			return;
		}
		if(l_StrRealNick[0]=='+')
		{
			((CPconwDoc *)GetDocument())->m_SelectedString = l_StrRealNick.Right(l_StrRealNick.GetLength()-1)	;
			if(((CPconwDoc *)GetDocument())->m_SelectedString.Compare(l_tmpkey)==0) ((CPconwDoc *)GetDocument())->m_SelectedString=_T("LUSER");
			pSendView->SetFocus();			
			return;
		}
		((CPconwDoc *)GetDocument())->m_SelectedString = l_StrRealNick;
		if(((CPconwDoc *)GetDocument())->m_SelectedString.Compare(l_tmpkey)==0) ((CPconwDoc *)GetDocument())->m_SelectedString=_T("LUSER");
		pSendView->SetFocus();			

		
	}
    else 
	{
		pSendView->SetFocus();			
		return;
	}
	//if(((CPconwDoc *)GetDocument())->m_SelectedString.Compare(l_tmpkey)==0) ((CPconwDoc *)GetDocument())->m_SelectedString=_T("LUSER");
#endif

#ifndef IRCMODE
    int ilb_Result;
	CString l_tmpkey,l_StrRealNick;
    l_tmpkey.LoadString(IDS_KEYUSER);
	ilb_Result=((CListBox*)GetDlgItem(IDC_LIST1))->GetCurSel();
	if(LB_ERR!=ilb_Result) 
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->GetText(ilb_Result,l_StrRealNick);
		((CPconwDoc *)GetDocument())->m_SelectedString = l_StrRealNick;
		if(((CPconwDoc *)GetDocument())->m_SelectedString.Compare(l_tmpkey)==0) ((CPconwDoc *)GetDocument())->m_SelectedString=_T("LUSER");
		return;
	}
	else return;
#endif
}

void CChanUList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	RECT l_size;
    
	if(m_bInit) 
    {
        GetClientRect(&l_size);
		if(IsWindow(m_lbUserList.m_hWnd)) m_lbUserList.MoveWindow(0,0,l_size.right-10,l_size.bottom-10,TRUE);
        m_bInit=FALSE;
    }
	
	// Do not call CFormView::OnPaint() for painting messages
}

void CChanUList::ChangeUserMode(CString & mode, CString & User)
{

	CString l_Nick,l_strTest;
	BOOL l_bVmode=FALSE;
	BOOL l_bOmode=FALSE;

	int l_lbResult;
	//remove all possible instances of the user (user, +user, @user)
	l_strTest.Format("@%s",User);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);
		l_bOmode=TRUE;

	}
	else 
	{
		l_strTest.Format("+%s",User);
		l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
		if(l_lbResult!=LB_ERR)
		{
			((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);
			l_bVmode=TRUE;
	
		}
		else
		{
			l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,User);
			if(l_lbResult!=LB_ERR)
			{
				((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);		
			}
		}
	}
	//
	//Add the user back with new mode
	if(mode[0]=='+')
	{
		if((mode[1]=='o')||(mode[1]=='O'))
		{
			l_Nick.Format("@%s",User);
		}
		if((mode[1]=='v')||(mode[1]=='V'))
		{
			l_Nick.Format("+%s",User);
			if(l_bOmode) l_Nick.Format("@%s",User);
		}

	}
	else
	{
		l_Nick=User;
		if((mode[1]!='v')&&(mode[1]!='V')) if(l_bVmode) l_Nick.Format("+%s",User);
		if((mode[1]!='o')&&(mode[1]!='O')) if(l_bOmode) l_Nick.Format("@%s",User);

	}
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(l_Nick);		

}

void CChanUList::ChangeNick(CString & newnick, CString & oldnick)
{
	//remove all possible instances of the user (user, +user, @user)
	CString l_Nick,l_strTest,l_mode;
	if(newnick[0]==':')
	{
		l_strTest=newnick.Right(newnick.GetLength()-1);
		newnick=l_strTest;
	}
	int l_lbResult;	l_mode="   ";
	//remove all possible instances of the user (user, +user, @user)
	l_strTest.Format("@%s",oldnick);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		l_mode="+o";
		((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);

	}
	else 
	{
		l_strTest.Format("+%s",oldnick);
		l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
		if(l_lbResult!=LB_ERR)
		{
			l_mode="+v";
			((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);
	
		}
		else
		{
			l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,oldnick);
			if(l_lbResult!=LB_ERR)
			{
				l_mode="  ";
				((CListBox*)GetDlgItem(IDC_LIST1))->DeleteString(l_lbResult);		
			}
		}
	}
	//
	//Add the user back with new nick
	if(l_mode[0]=='+')
	{
		if((l_mode[1]=='o')||(l_mode[1]=='O'))
		{
			l_Nick.Format("@%s",newnick);
		}
		if((l_mode[1]=='v')||(l_mode[1]=='V'))
		{
			l_Nick.Format("+%s",newnick);
		}

	}
	else
	{
		l_Nick=newnick;
	}
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(l_Nick);		
	
}

BOOL CChanUList::IsUserHere(CString & User)
{
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",User);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		return TRUE;

	}
	else 
	{
		l_strTest.Format("+%s",User);
		l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
		if(l_lbResult!=LB_ERR)
		{
			return TRUE;
	
		}
		else
		{
			l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,User);
			if(l_lbResult!=LB_ERR)
			{
				return TRUE;
			}
		}
	}
	return FALSE;

}

void CChanUList::GrantOps()
{
	CString l_cmd;
	((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/mode %s +o %s",((CPconwDoc *)GetDocument())->m_Winname,((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);

}

void CChanUList::GrantVoice()
{
	CString l_cmd;
	((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/mode %s +v %s",((CPconwDoc *)GetDocument())->m_Winname,((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
}

void CChanUList::TakeOps()
{
	CString l_cmd;
	((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/mode %s -o %s",((CPconwDoc *)GetDocument())->m_Winname,((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
}

void CChanUList::TakeVoice()
{
	CString l_cmd;
	((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/mode %s -v %s",((CPconwDoc *)GetDocument())->m_Winname,((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
}

void CChanUList::KickUser()
{
	//Make a dialog for the kick message.
	CKickPrompt l_KickPrompt;
	l_KickPrompt.DoModal();
	CString l_cmd;
	((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/kick %s %s %s",((CPconwDoc *)GetDocument())->m_Winname,((CPconwDoc *)GetDocument())->m_SelectedString,l_KickPrompt.m_KickMsg);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);

}

void CChanUList::OnUpdateTakeops(CCmdUI* pCmdUI) 
{
#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",((CPconwApp*)AfxGetApp())->m_strHandle);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		pCmdUI->Enable(TRUE);

	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
#endif	
}

void CChanUList::OnUpdateTakevoice(CCmdUI* pCmdUI) 
{
#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",((CPconwApp*)AfxGetApp())->m_strHandle);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		pCmdUI->Enable(TRUE);

	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
#endif	
}

void CChanUList::OnUpdateGrantops(CCmdUI* pCmdUI) 
{
#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",((CPconwApp*)AfxGetApp())->m_strHandle);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		pCmdUI->Enable(TRUE);

	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
#endif
}

void CChanUList::OnUpdateGrantvoice(CCmdUI* pCmdUI) 
{
#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",((CPconwApp*)AfxGetApp())->m_strHandle);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		pCmdUI->Enable(TRUE);

	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
#endif
}

void CChanUList::OnUpdateKickuser(CCmdUI* pCmdUI) 
{
#ifdef IRCMODE	
	CString l_strTest;
	BOOL l_lbResult;
	l_strTest.Format("@%s",((CPconwApp*)AfxGetApp())->m_strHandle);
	l_lbResult=((CListBox*)GetDlgItem(IDC_LIST1))->FindStringExact(0,l_strTest);
	if(l_lbResult!=LB_ERR)
	{
		pCmdUI->Enable(TRUE);

	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
#endif
}

void CChanUList::OnDccchat() 
{
	CString l_cmd;
	//((CPconwDoc *)GetDocument())->m_Winname;
	l_cmd.Format("/dcc chat %s",((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
	
}

void CChanUList::OnDccsend() 
{
	CString l_cmd;
	CFileDialog l_FileDlg(TRUE);
	if(l_FileDlg.DoModal()==IDOK)
	{
		((CPconwDoc *)GetDocument())->m_Winname;
		l_cmd.Format("/dcc send %s %s",((CPconwDoc *)GetDocument())->m_SelectedString,l_FileDlg.GetPathName());
		((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
	}
	
}

void CChanUList::OnUdpping() 
{
	//IDM_UDPPING
	CString l_cmd;
	l_cmd.Format("/tping %s",((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);
}

void CChanUList::OnIgnoreuser() 
{
	CString l_cmd;
	l_cmd.Format("/ignore %s",((CPconwDoc *)GetDocument())->m_SelectedString);
	((CPconwApp*)AfxGetApp())->SendMsg(l_cmd);	
}

void CChanUList::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}

void CChanUList::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default


	
	//Change resource to owner drawn fixed & has strings

	if(nIDCtl==IDC_LIST1)
	{
		CListBox *l_lbUsers = (CListBox *) GetDlgItem(IDC_LIST1);
		ASSERT(l_lbUsers);
		CString l_StrNick;
		int l_iRating;
		l_iRating = 0;
		CString l_strLine;// = (LPCSTR)lpDrawItemStruct->itemData;
		DWORD l_dwPing;
		l_lbUsers->GetText(lpDrawItemStruct->itemID,l_strLine);
		if(l_strLine[0]=='@')
		{
			l_StrNick = LPCSTR(l_strLine)+1;
		}
		else if(l_strLine[0]=='+')
		{
			l_StrNick = LPCSTR(l_strLine)+1;
		}
		else
		{
			l_StrNick = l_strLine;
		}
		
		
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		//COLORREF cr = (COLORREF)lpDrawItemStruct->itemData; // RGB in item data
		CUserPing *l_UserPing;
		
		if(((CPconwApp*)AfxGetApp())->m_UserPings.Lookup(l_StrNick,(class CObject*&)l_UserPing))
		{
			l_iRating = l_UserPing->GetPingRating();
		}
		if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		{
			// Paint the item
			pDC->SetBkColor(RGB(255,255,255));
			pDC->SetTextColor(RGB(0,0,0));
			pDC->BitBlt(lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,16,16,&m_dcCompatible,l_iRating*16,0,SRCCOPY);
			pDC->TextOut(lpDrawItemStruct->rcItem.left+17,lpDrawItemStruct->rcItem.top,l_strLine);
		}

		if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
			(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
		{
			// item has been selected - hilite frame
			COLORREF crHilite = RGB(0,0,255);//all Blue!
			CBrush br(crHilite);
			pDC->SetBkColor(crHilite);
			pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
			pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->TextOut(lpDrawItemStruct->rcItem.left+17,lpDrawItemStruct->rcItem.top,l_strLine);
			pDC->BitBlt(lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,16,16,&m_dcCompatible,l_iRating*16,0,SRCCOPY);
			
			
		}

		if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
			(lpDrawItemStruct->itemAction & ODA_SELECT))
		{
			// Item has been de-selected -- remove frame
			CBrush br(RGB(255,255,255));//GetLogBrush( LOGBRUSH* pLogBrush );
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,255,255));
			pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
			pDC->TextOut(lpDrawItemStruct->rcItem.left+17,lpDrawItemStruct->rcItem.top,l_strLine);
			pDC->BitBlt(lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,16,16,&m_dcCompatible,l_iRating*16,0,SRCCOPY);
			pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
		}
		return;
	}
	


	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CChanUList::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	//Not currently implemented..
	if(nIDCtl==IDC_LIST1)
	{

		return;
	}
	
	CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CChanUList::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CChanUList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	/*
	NMHDR * l_pnotify;
	TOOLTIPTEXT * l_pToolTipInfo;

	l_pnotify=(NMHDR *)lParam;
	
	//Just in case
	if(!l_pnotify) return CFormView::OnNotify(wParam, lParam, pResult);

	if((l_pnotify->code==TTN_NEEDTEXT)||(l_pnotify->code==IDD_CHANULIST))
	{

		if(l_pnotify->idFrom==IDC_LIST1)
		{
		l_pToolTipInfo = (TOOLTIPTEXT *)l_pnotify;
		l_pToolTipInfo->hinst==NULL;
		
		strcpy(l_pToolTipInfo->szText,"Tool Tip");
		return TRUE;
		}
	}

	
	n*/
	return CFormView::OnNotify(wParam, lParam, pResult);
}
