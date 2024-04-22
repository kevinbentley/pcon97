// ChannelList.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "ChannelList.h"
#include "chatDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CChannelList dialog



CChannelList::CChannelList(CWnd* pParent /*=NULL*/)
{
	m_bFocus=FALSE;
	iValid=FALSE;
	m_bSelectedChan=FALSE;
	sFirstChanList = new struct sChannelList;
	sFirstChanList->NumUsers=0;
	sCurrChanList=sFirstChanList;
#ifndef IRCMODE
	sCurrChanList->ChanName="#descnt";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#ra";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;
#endif

#ifdef IRCMODE
	sCurrChanList->ChanName="#descent";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#redalert";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;
#endif
	sCurrChanList->ChanName="#kahn";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#war2";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#c&c";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#duke3d";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#su27";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#atf";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#doom";
	sCurrChanList->NumUsers=0;
	sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;

	sCurrChanList->ChanName="#help";
	sCurrChanList->NumUsers=0;
	//sCurrChanList->Next = new struct sChannelList;
	//sCurrChanList=sCurrChanList->Next;
	sCurrChanList->Next=NULL;
}


void CChannelList::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChannelList, CDialogBar)
	//{{AFX_MSG_MAP(CChannelList)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnSetfocusCombo1)
	ON_CBN_DBLCLK(IDC_COMBO1, OnDblclkCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnKillfocusCombo1)
	ON_LBN_DBLCLK(IDC_COMBO1, OnDblclkCombo1)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelList message handlers

void CChannelList::ProcessJoin(LPCSTR User,LPCSTR Chan)
{
	if(Chan[0]==0) return;
	sCurrChanList=sFirstChanList;
	do
	{
		if(sCurrChanList->ChanName.Compare(Chan)==0)
		{
			if(sCurrChanList->Users.Find(User)==NULL) sCurrChanList->Users.AddTail(User);
			//sCurrChanList->NumUsers++;
			return;
		}
		sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
	//not found, add it to the end
	sCurrChanList=sFirstChanList;
	while(sCurrChanList->Next) sCurrChanList=sCurrChanList->Next;
	sCurrChanList->Next = new struct sChannelList;
    sCurrChanList=sCurrChanList->Next;
	sCurrChanList->ChanName=Chan;
	sCurrChanList->NumUsers=1;
	sCurrChanList->Next=NULL;


}

void CChannelList::ProcessPart(LPCSTR User,LPCSTR Chan)
{
	sCurrChanList=sFirstChanList;
	do
	{
		if(sCurrChanList->ChanName.Compare(Chan)==0)
		{
			if(sCurrChanList->Users.Find(User)) sCurrChanList->Users.RemoveAt(sCurrChanList->Users.Find(User));
			//sCurrChanList->NumUsers--;
			return;
		}
		sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
}

BOOL CChannelList::OnInitDialog() 
{
//	CDialogBar::OnInitDialog();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CChannelList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	else
	{

	return 0;
	}
	
}

void CChannelList::Init(void)
{
	CString FmtChan;
	CListBox *ChannelListBox;
	int l_ilbFind;
	sCurrChanList=sFirstChanList;
	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
	do
	{
		if(sCurrChanList==NULL) break;
		//sCurrChanList->Users.GetCount()
#ifndef IRCMODE
		if(sCurrChanList->NumUsers!= -1) FmtChan.Format("(%d) %s",sCurrChanList->Users.GetCount(),sCurrChanList->ChanName);
        else FmtChan.Format("* %s",sCurrChanList->ChanName);
#endif
#ifdef IRCMODE
		//Later on use /list to populate with REAL IRC channel list & count
		//if(sCurrChanList->NumUsers!= -1) FmtChan.Format("%s",sCurrChanList->ChanName);
        //else FmtChan.Format("* %s",sCurrChanList->ChanName);
		FmtChan.Format("%s",sCurrChanList->ChanName);
#endif
	//	FmtChan.Format("(%d) %s",sCurrChanList->NumUsers,sCurrChanList->ChanName);
		//if(sCurrChanList->ChanName.Compare(((CPconwApp*)AfxGetApp())->m_Channel)==0) SelChan=FmtChan;
		
		l_ilbFind=ChannelListBox->FindStringExact(0,FmtChan);
		if(l_ilbFind==LB_ERR) 
		{
			ChannelListBox->AddString(FmtChan);
		}
		sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
}


void CChannelList::Refresh(void)
{
#ifdef IRCMODE
	return;
#endif
	CListBox *ChannelListBox;
	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
	CString FmtChan;
	CString SelChan,strSelected;
	int l_sel = ChannelListBox->GetCurSel();
    if(l_sel!=LB_ERR) ChannelListBox->GetText(l_sel,strSelected);

	if(m_bFocus) return;
	ChannelListBox->ResetContent();
	sCurrChanList=sFirstChanList;
	do
	{
		if(sCurrChanList==NULL) break;
		//sCurrChanList->Users.GetCount()
#ifndef IRCMODE
		if(sCurrChanList->NumUsers!= -1) FmtChan.Format("(%d) %s",sCurrChanList->Users.GetCount(),sCurrChanList->ChanName);
        else FmtChan.Format("* %s",sCurrChanList->ChanName);
#endif
#ifdef IRCMODE
		//Later on use /list to populate with REAL IRC channel list & count
		//if(sCurrChanList->NumUsers!= -1) FmtChan.Format("%s",sCurrChanList->ChanName);
        //else FmtChan.Format("* %s",sCurrChanList->ChanName);
		FmtChan.Format("%s",sCurrChanList->ChanName);
#endif
	//	FmtChan.Format("(%d) %s",sCurrChanList->NumUsers,sCurrChanList->ChanName);
		if(sCurrChanList->ChanName.Compare(((CPconwApp*)AfxGetApp())->m_Channel)==0) SelChan=FmtChan;
		ChannelListBox->AddString(FmtChan);
		sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
	
    if(l_sel!=LB_ERR) ChannelListBox->SetSel(ChannelListBox->FindStringExact(0,strSelected));
	
    //if(!m_bSelectedChan) 
	//{
	//ChannelListBox->SelectString(0,SelChan);
	//	m_bSelectedChan=TRUE;
	//}
	//iValid=TRUE;
}

void CChannelList::OnSelchangeCombo1() 
{
	//Set the active document, based on the new selection
    CChatDoc *l_Activatme;
    CPconwDoc *l_ActivateChan;
    CString strJoinMsg,strSelected;
	CString l_tmpmsgj;
	l_tmpmsgj.LoadString(IDS_CMDJOIN);
	CListBox *ChannelListBox;
	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
	int iChop;
//MDIActivate();
	ChannelListBox->GetText(ChannelListBox->GetCurSel(),strSelected);
#ifndef IRCMODE
	if(strSelected[0]=='*') 
    {
        CString l_strLower;//l_Activatme
		l_strLower=strSelected;
		l_strLower.MakeLower();
        if(((CPconwApp*)AfxGetApp())->m_WindowMap.Lookup(LPCSTR(l_strLower)+2,(CObject*&)l_Activatme))
        {
            l_Activatme->ActivateSendVw(0);
        }
        //This is a private chat window, just activate it.....
        return;
    }
    
	iChop=strSelected.Find(')')+1;
	strJoinMsg.Format("%s",strSelected.Right(strSelected.GetLength()-(iChop+1)));
#endif 

#ifdef IRCMODE
	strJoinMsg=strSelected;

//	((CChildFrame*)m_MainChannelWnd)->MDIActivate();
//	m_MainChannelWnd->BringWindowToTop();
//CDocument
	CView *l_pActivateView;
	CDocument *l_pActiveDoc;
	CMDIChildWnd* l_ActivateFrame;//GetParentFrame( ) 
	POSITION l_posa;
//	if(((CPconwApp*)AfxGetApp())->m_WindowMap.Lookup(strJoinMsg,(CObject*&)l_pActiveDoc))
//    {
//        l_posa=l_pActiveDoc->GetFirstViewPosition();
//		l_pActivateView = l_pActiveDoc->GetNextView(l_posa);
///		l_ActivateFrame=(CMDIChildWnd*)l_pActivateView->GetParentFrame();
//		l_ActivateFrame->MDIActivate();

   // }
	POSITION pos;
	CString key;
	for( pos = ((CPconwApp*)AfxGetApp())->m_WindowMap.GetStartPosition(); pos != NULL; )
	{
		((CPconwApp*)AfxGetApp())->m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_pActiveDoc );
		if(key.CompareNoCase(strJoinMsg)==0)
		{
			l_posa=l_pActiveDoc->GetFirstViewPosition();
			l_pActivateView = l_pActiveDoc->GetNextView(l_posa);
			l_ActivateFrame=(CMDIChildWnd*)l_pActivateView->GetParentFrame();
			l_ActivateFrame->MDIActivate();
			l_ActivateFrame->ShowWindow(SW_RESTORE);
			return;
		}
					
	}
	return;

#endif
    if(((CPconwApp*)AfxGetApp())->m_WindowMap.Lookup(strJoinMsg,(CObject*&)l_ActivateChan))
    {
        l_ActivateChan->ActivateSendVw(0);
    }
	
}

void CChannelList::OnSetfocusCombo1() 
{
	m_bFocus=TRUE;
	if(iValid) ((CPconwApp*)AfxGetApp())->ActivateChannel();			
}

void CChannelList::OnDblclkCombo1() 
{
    JoinSelected();	
}

void CChannelList::OnKillfocusCombo1() 
{
	m_bFocus=FALSE;
}

void CChannelList::JoinSelected(void)
{
	CChatDoc *l_Activatme;
    CString strJoinMsg,strSelected;
	CString l_tmpmsgj;
	l_tmpmsgj.LoadString(IDS_CMDJOIN);
	CListBox *ChannelListBox;
	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
	int iChop;

	ChannelListBox->GetText(ChannelListBox->GetCurSel(),strSelected);
	if(strSelected.GetLength()) if(strSelected[0]=='*') 
    {
        //l_Activatme
        if(((CPconwApp*)AfxGetApp())->m_WindowMap.Lookup(LPCSTR(strSelected)+2,(CObject*&)l_Activatme))
        {
            l_Activatme->ActivateSendVw(0);
        }
        //This is a private chat window, just activate it.....
        return;
    }
    iChop=strSelected.Find(')')+1;
	strJoinMsg.Format("/%s %s",l_tmpmsgj,strSelected.Right(strSelected.GetLength()-iChop));
	((CPconwApp*)AfxGetApp())->SendMsg(strJoinMsg);	
	//if(iValid) 
	//((CPconwApp*)AfxGetApp())->ActivateChannel();
}



void CChannelList::AddWindow(LPCSTR Window)
{
    CListBox *ChannelListBox;
	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
	
	if(Window[0]==0) return;
	
#ifdef IRCMODE
	if(ChannelListBox->FindStringExact(0,Window)==LB_ERR)
	{
		ChannelListBox->AddString(Window);
	}
	return;
#endif
	
	sCurrChanList=sFirstChanList;
	do
	{
		if(sCurrChanList->ChanName.Compare(Window)==0)
		{
			return;
		}
	sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
	
    //not found, add it to the end
	sCurrChanList=sFirstChanList;
	while(sCurrChanList->Next) sCurrChanList=sCurrChanList->Next;
	
    sCurrChanList->Next = new struct sChannelList;
	sCurrChanList=sCurrChanList->Next;
    sCurrChanList->ChanName=Window;
	sCurrChanList->NumUsers=-1;//-1 means it is a window, not a channel
	sCurrChanList->Next=NULL;




}

void CChannelList::CloseWindow(LPCSTR Window)
{
  
#ifdef IRCMODE
//	CListBox *ChannelListBox;
//	ChannelListBox = (CListBox *)GetDlgItem(IDC_COMBO1);
//	int l_isel;
//	l_isel=ChannelListBox->FindStringExact(0,Window);
//	ChannelListBox->DeleteString(l_isel);
	return;
#endif	
	
	CString l_FmtWindow;
	l_FmtWindow.Format("* %s",Window);
	sCurrChanList=sFirstChanList;
    sChannelList *l_stmp=sFirstChanList;
	do
	{
		if(sCurrChanList->ChanName.Compare(Window)==0)
		{
			//Delete this structure
        //    if(sCurrChanList!=sFirstChanList)
        //    {
                l_stmp->Next=sCurrChanList->Next;
                delete sCurrChanList;
				
			    return;
        //    }
        //    else
        //    {
        //        sCurrChanList=sCurrChanList->Next;
        //        delete sCurrChanList;
        //    }
		}
	
        l_stmp=sCurrChanList;
		sCurrChanList=sCurrChanList->Next;
	}while(sCurrChanList);
	
}
//CDockState

CChannelList::~CChannelList()
{
    sCurrChanList=sFirstChanList;

    while(sCurrChanList)
    {
    sTempChanList = sCurrChanList;
	sCurrChanList=sCurrChanList->Next;
    delete sTempChanList;
    }
}

void CChannelList::OnClose() 
{
	CDialogBar::OnClose();
}

void CChannelList::OnDestroy() 
{
	CDialogBar::OnDestroy();	
}
