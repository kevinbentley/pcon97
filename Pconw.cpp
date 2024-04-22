// pconw.cpp : Defines the class behaviors for the application.
//


//********************************************************
//********************************************************
//  Make sure you include the .\wfc directory for include
//  Files.
//********************************************************
//********************************************************

//Use this for IRC compatable mode


#include "stdafx.h"
#include "ControlSocket.h"
#include "chatsock.h"


#include "pconw.h"
#include "kali.h"
#include "setupdlg.h"
#include "ChildFrm.h"
#include "GetFileDlg.h"
#include "SendPrvMsg.h"
#include "DUserList.h"
#include "MainFrm.h"
#include "ChatFrm.h"
#include "chatDoc.h"
#include "Sendchat.h"
#include "chatView.h"

#include "ClrStatic.h"
#include "util.h"
#include "setupdlg.h"
#include "olorSetup.h"
#include "WordFltr.h"
#include "fkeysetup.h" //FKEY config

#include "pconwView.h"
#include "cuser.h"
#include "awaymsg.h"

#include "FingerSocket.h"
#include "FingerDoc.h"
#include "FingerView.h"
#include "FingerFrame.h"

#include "FingerThread.h"

#include "DCCSocket.h"

#include "DCCCDoc.h"
#include "DCCCFrm.h"
#include "DCCCSend.h"
#include "DCCCView.h"

#include "DCCFDoc.h"
#include "DCCFView.h"
#include "DCCFFrm.h"

#include "GameView.h"
#include "dwatchDoc.h"
#include "DspyFrm.h"

#include "ModalYesNoMessagBox.h"
#include "DupNickDialog.h"

#include <process.h>


#define WHOTIMEOUT 5

/////////////////////////////////////////////////////////////////////////////
// CPconwApp

BEGIN_MESSAGE_MAP(CPconwApp, CWinApp)
	//{{AFX_MSG_MAP(CPconwApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_LIST_USERS, OnListUsers)
	ON_COMMAND(IDC_STOPSCROLL, OnStopscroll)
	ON_UPDATE_COMMAND_UI(IDC_STOPSCROLL, OnUpdateStopscroll)
	ON_COMMAND(IDC_SETAWAY, OnSetaway)
	ON_UPDATE_COMMAND_UI(IDC_SETAWAY, OnUpdateSetaway)
	ON_COMMAND(ID_SERVER_INFO, OnServerInfo)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(IDC_SETPREFERENCES, OnSetpreferences)
	ON_COMMAND(IDC_JOINSEL, OnJoinsel)
	ON_COMMAND(LAUNCH_CFG, OnLaunchCfg)
	ON_COMMAND(ID_PREFERENCES_COLORS, OnPreferencesColors)
	ON_COMMAND(ID_PREFERENCES_FONT, OnPreferencesFont)
	ON_COMMAND(ID_PREFERENCES_HIGHLIGHTING, OnPreferencesHighlighting)
	ON_COMMAND(ID_PREFERENCES_FUNCTIONKEYS, OnPreferencesFunctionkeys)
	ON_COMMAND(IDM_HOMEPAGE, OnHomepage)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPconwApp construction

CPconwApp::CPconwApp()
{
	m_iSplashGone=0;
	m_iSocketTimeout=0;
	m_ChannelDoc=NULL;
	FirstFilter = new SFilters;
	CurrFilter = FirstFilter;
	FirstFilter->Next=NULL;
	FirstFilter->cs_Word= _T("");
	m_Frozen=FALSE;
	m_bRejecting = FALSE;
	m_bAway = FALSE;
	FirstTabUser = new TabUser;
	FirstTabUser->Next = NULL;
	FirstTabUser->User[0] = 0;
	CurrTabUser = FirstTabUser;
	m_bNewFont=FALSE;
	m_bClosing=FALSE;
	m_MainChannelWnd=NULL;
	m_WhoTimer=WHOTIMEOUT;
	m_ChanList=NULL;
	m_ChanRefresh=10;
	g_iCancel=0;
//	m_TCPCurrPos=0;
	m_bInAdd=0;
	m_bInRemove=0;	
	m_bDSpyRunning=0;
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPconwApp object
 
CPconwApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CPconwApp initialization

BOOL CPconwApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

  pSplashThread = (CSplashThread*) AfxBeginThread(RUNTIME_CLASS(CSplashThread));
  if (pSplashThread == NULL)
  {
    AfxMessageBox("Failed to create splash screen", MB_OK | MB_ICONSTOP);
    return FALSE;
  }
  ASSERT(pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));
   


GetDefaults();
m_CmdMsg.LoadString(IDS_CMDMSG);
m_CmdWho.LoadString(IDS_CMDWHO);
m_CmdUsers.LoadString(IDS_CMDUSERS);
m_CmdPing.LoadString(IDS_CMDPING);
m_CmdHelp.LoadString(IDS_CMDHELP);
m_CmdIgnore.LoadString(IDS_CMDIGNORE);
m_CmdMotd.LoadString(IDS_CMDMOTD);
m_CmdKill.LoadString(IDS_CMDKILL);
m_CmdGet.LoadString(IDS_CMDGET);
m_CmdOffer.LoadString(IDS_CMDOFFER);
m_CmdCancelGet.LoadString(IDS_CMDCANCELGET);
m_CmdWhois.LoadString(IDS_CMDWHOIS);
m_CmdNoprivate.LoadString(IDS_CMDNOPRIVATE);
m_CmdBan.LoadString(IDS_CMDBAN);
m_CmdUnban.LoadString(IDS_CMDUNBAN);
m_CmdBlist.LoadString(IDS_CMDBLIST);
m_CmdDeoffer.LoadString(IDS_CMDDEOFFER);
m_CmdProgress.LoadString(IDS_CMDPROGRESS);
m_CmdShowoffers.LoadString(IDS_CMDSHOWOFFERS);
m_CmdJoin.LoadString(IDS_CMDJOIN);
m_CmdMe.LoadString(IDS_CMDME);
m_CmdAway.LoadString(IDS_CMDAWAY);
m_CmdSubnet.LoadString(IDS_CMDSUBNET);
m_CmdPart.LoadString(IDS_CMDPART);

CString l_Argv0;
l_Argv0=GetCommandLine();
m_PconwPath=l_Argv0.Left(l_Argv0.ReverseFind('\\')+1);
if(m_PconwPath[0]=='\"')
{
	CString l_StripQuote;
	l_StripQuote=m_PconwPath.Right(m_PconwPath.GetLength()-1);
	m_PconwPath=l_StripQuote;
}


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	hMapObject = OpenFileMapping (FILE_MAP_READ,
	                            FALSE,
	                            KAHN_SHAREMEM_NAME);
	if (!hMapObject)
	{
   		CString l_tmpmsgkcce,l_tmpmsgerr;
		l_tmpmsgerr.LoadString(IDS_STRERROR);
		l_tmpmsgkcce.LoadString(IDS_STRKAHNCCERR);
		::MessageBox(NULL,l_tmpmsgkcce,l_tmpmsgerr, MB_OK | MB_ICONEXCLAMATION);
		return(FALSE);
	}

	psharemem = (PSHAREMEM) MapViewOfFile (hMapObject,
                                 FILE_MAP_WRITE,
                                 0, 0, 0);
	
	if (!psharemem)
	{
  		CString l_tmpmsgkcce,l_tmpmsgerr;
		l_tmpmsgerr.LoadString(IDS_STRMME);
		l_tmpmsgkcce.LoadString(IDS_STRKAHNCCERR);
		::MessageBox (NULL,l_tmpmsgkcce,l_tmpmsgkcce, MB_OK | MB_ICONEXCLAMATION);
		return(FALSE);
	}
	
	if(IsWindow(psharemem->hChat))
	{
		
		BringWindowToTop(psharemem->hChat);
		SetForegroundWindow(psharemem->hChat);
		return FALSE;
	}
	

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CPconwDoc),
		RUNTIME_CLASS(CChildFrame), // Channels only
		RUNTIME_CLASS(CPconwView));
	AddDocTemplate(pDocTemplate);

	m_pDocTemplate=pDocTemplate; //This one is used for channel windows only.

	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CChatDoc),
		RUNTIME_CLASS(CChatFrame), // Chat frame (private msg, server msg, and finger)
		RUNTIME_CLASS(CChatView));
	AddDocTemplate(pDocTemplate);

	m_ChatDocTemplate=pDocTemplate; //This one is used for private chat windows and server windows
	
	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CFingerDoc),
		RUNTIME_CLASS(CFingerFrame), // Finger Window
		RUNTIME_CLASS(CFingerView));
	AddDocTemplate(pDocTemplate);

	m_FingerDocTemplate=pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CDCCCDoc),
		RUNTIME_CLASS(CDCCCFrm), // DCC Chat Window
		RUNTIME_CLASS(CDCCCView));
	AddDocTemplate(pDocTemplate);

	m_DCCCDocTemplate=pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CDCCFDoc),
		RUNTIME_CLASS(CDCCFFrm), // DCC Chat Window
		RUNTIME_CLASS(CDCCFView));
	AddDocTemplate(pDocTemplate);

	m_DCCFDocTemplate=pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(
		IDR_PCONWTYPE,
		RUNTIME_CLASS(CDwatchDoc),
		RUNTIME_CLASS(CDspyFrm), // DCC Chat Window
		RUNTIME_CLASS(CGameView));
	AddDocTemplate(pDocTemplate);

	m_DSPYFDocTemplate=pDocTemplate;

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	
	//****************************************************
	//Create the server window.
	//****************************************************
	m_CreateNewDocTitle = "Server";
	CChatFrame *ChannelWindow;
	m_ChannelDoc=m_ChatDocTemplate->CreateNewDocument();
    m_WindowMap.SetAt("Server",m_ChannelDoc);

	//2 of these happen here:
	//First-chance exception in Pconw.exe (KERNEL32.DLL): 0xC0000005: Access Violation.
	ChannelWindow=(CChatFrame *)m_ChatDocTemplate->CreateNewFrame(m_ChannelDoc,NULL);
	m_ChatDocTemplate->InitialUpdateFrame((CFrameWnd *)ChannelWindow,m_ChannelDoc,TRUE);
	m_CreateNewDocTitle.Empty();
	//****************************************************
	//****************************************************
#ifndef IRCMODE	
	//****************************************************
	//Create the Default chat window.
	//****************************************************
	CPconwDoc *l_FirstChanDoc;
	CChildFrame *l_ChatFrame;
	m_CreateNewDocTitle = DefaultChannel;

	l_FirstChanDoc=(CPconwDoc *)m_pDocTemplate->CreateNewDocument();
	
	m_ChannelList.AddHead(DefaultChannel);
	
	CString l_tmplower;
	l_tmplower=DefaultChannel;
	l_tmplower.MakeLower();
	//m_WindowMap.SetAt(l_tmplower,l_FirstChanDoc);
	
	l_ChatFrame=(CChildFrame *)m_pDocTemplate->CreateNewFrame((CDocument *)l_FirstChanDoc,NULL);
	m_pDocTemplate->InitialUpdateFrame((CFrameWnd *)l_ChatFrame,l_FirstChanDoc,TRUE);
	m_CreateNewDocTitle.Empty();    
	//****************************************************
	//****************************************************
	//Add myself to the first channel
    l_FirstChanDoc->UserJoin(psharemem->nickname);

#endif //IRCMODE	
	
		
	psharemem->hChat=m_pMainWnd->m_hWnd;

	m_LaunchCfgDlg = new CConfigLauncher(pMainFrame,(LaunchItems *)&m_LaunchItems);
	//returns FALSE if it didn't find the key.....
    if(!m_LaunchCfgDlg->ReadRegistry())
    {
        CString l_strConfigLaunchpad;
        l_strConfigLaunchpad.LoadString(IDS_STRFIRSTLAUNCH);
        
        //This would be a good time to scan for games
        if(MessageBox(pMainFrame->m_hWnd,LPCSTR(l_strConfigLaunchpad),_T(""),MB_YESNO)==IDYES)
        {
           m_LaunchCfgDlg->m_bFirstRun=TRUE;
           if(m_LaunchCfgDlg->DoModal()!=IDCANCEL) m_LaunchCfgDlg->WriteRegistry();	
	       else m_LaunchCfgDlg->ReadRegistry();
		   m_LaunchCfgDlg->m_bFirstRun=FALSE;
        }

    }//Commandline
	
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	
	pMainFrame->RefreshLaunchBar();
	
	//Create the hidden modeless channel list box
	m_DynChannelList.Create(IDD_CHANNELLIST,pMainFrame);
	m_DynChannelList.ShowWindow(SW_HIDE);
	
	ConnectSocket();
	//m_SplashWnd.BringWindowToTop();
//	pSplashThread->HideSplash();    //Bring down the splash screen
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPconwApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPconwApp commands

void CPconwApp::GetDefaults(void)
{
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      return;
   }

   if ( registry.Open( "SYSTEM\\SOFTWARE\\StarGate\\Kahn97\\Chat\\Colors",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\Colors"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\Colors",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			return;
		}
				
	  }
	  else
	  {
		return;
	  }
    }
	
   

   if ( registry.GetValue( "Action", colorACT ) != TRUE )
   {
      colorACT = RGB(255,0,0);
	  registry.SetValue( "Action", colorACT );
   }
   if ( registry.GetValue( "Normal", colorNORM ) != TRUE )
   {
      colorNORM = RGB(0,0,0);
	  registry.SetValue( "Normal", colorNORM );
   }
   if ( registry.GetValue( "Private", colorPRIV ) != TRUE )
   {
      colorPRIV = RGB(0,0,102);
	  registry.SetValue( "Private", colorPRIV );
   }
   if ( registry.GetValue( "Status", colorSTAT ) != TRUE )
   {
      colorSTAT = RGB(255,0,0);
	  registry.SetValue( "Status", colorSTAT );
   }
   if ( registry.GetValue( "Background", colorBACK ) != TRUE )
   {
      colorBACK = RGB(255,255,255);
	  registry.SetValue( "Background", colorBACK );
   }
   if ( registry.GetValue( "URL", colorURL) != TRUE )
   {
      colorURL = PALETTERGB(0,0,255);
	  registry.SetValue( "URL", colorURL);
   }
	

   registry.Close();

   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      return;
   }

   if ( registry.Open( "SYSTEM\\SOFTWARE\\StarGate\\Kahn97\\Chat\\FKeys",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\FKeys"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\FKeys",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			return;
		}
				
	  }
	  else
	  {
		return;
	  }
    }
	
   
   if ( registry.GetValue( "F2", F2key ) != TRUE )
   {
      F2key.LoadString(IDS_STRFKEY2);
	  registry.SetValue( "F2", F2key );
   }
   if ( registry.GetValue( "F3", F3key ) != TRUE )
   {
      F3key.LoadString(IDS_STRFKEY3);
	  registry.SetValue( "F3", F3key );
   }
   if ( registry.GetValue( "F4", F4key ) != TRUE )
   {
      F4key.LoadString(IDS_STRFKEY4);
	  registry.SetValue( "F4", F4key );
   }
   if ( registry.GetValue( "F5", F5key ) != TRUE )
   {
      F5key.LoadString(IDS_STRFKEY5);
	  registry.SetValue( "F5", F5key );
   }
   if ( registry.GetValue( "F6", F6key ) != TRUE )
   {
      F6key.LoadString(IDS_STRFKEY6);
	  registry.SetValue( "F6", F6key );
   }
   if ( registry.GetValue( "F7", F7key ) != TRUE )
   {
      F7key.LoadString(IDS_STRFKEY7);
	  registry.SetValue( "F7", F7key );
   }
   if ( registry.GetValue( "F8", F8key ) != TRUE )
   {
      F8key.LoadString(IDS_STRFKEY8);
	  registry.SetValue( "F8", F8key );
   }
   if ( registry.GetValue( "F9", F9key ) != TRUE )
   {
      F9key.LoadString(IDS_STRFKEY9);
	  registry.SetValue( "F9", F9key );
   }
   if ( registry.GetValue( "F10", F10key ) != TRUE )
   {
      F10key.LoadString(IDS_STRFKEY10);
	  registry.SetValue( "F10", F10key );
   }
   if ( registry.GetValue( "F11", F11key ) != TRUE )
   {
      F11key.LoadString(IDS_STRFKEY11);
	  registry.SetValue( "F11", F11key );
   }
   if ( registry.GetValue( "F12", F12key ) != TRUE )
   {
      F12key.LoadString(IDS_STRFKEY12);
	  registry.SetValue( "F12", F12key );
   }
   registry.Close();
   
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      return;
   }

   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			return;
		}
				
	  }
	  else
	  {
		return;
	  }
	
   }
   if ( registry.GetValue( "FontName", FontName ) != TRUE )
   {
      FontName.LoadString(IDS_STRDFTFONT);
	  registry.SetValue( "FontName", FontName);
   }
   if ( registry.GetValue( "FontSize", FontSize ) != TRUE )
   {
      FontSize = 8;
	  registry.SetValue( "FontSize", FontSize);
   }

   if ( registry.GetValue( "Channel", DefaultChannel ) != TRUE )
   {
      DefaultChannel.LoadString(IDS_STRDFTCHAN);
	  registry.SetValue( "Channel", DefaultChannel );
   }
   
   if ( registry.GetValue( "QuitMsg", LeaveMsg ) != TRUE )
   {
      LeaveMsg.LoadString(IDS_STRLEAVMSG);
	  registry.SetValue( "QuitMsg", LeaveMsg );
   }
   if ( registry.GetValue( "PrivateWindows", m_bSeperatePrivate ) != TRUE )
   {
      m_bSeperatePrivate = 1;
	  registry.SetValue( "PrivateWindows", m_bSeperatePrivate );
   }
   if ( registry.GetValue( "SpamFilter", m_bSpam ) != TRUE )
   {
      m_bSpam = 1;
	  registry.SetValue( "SpamFilter", m_bSpam );
   }
   if ( registry.GetValue( "ShowJoinPart", m_bShowJoin ) != TRUE )
   {
      m_bShowJoin = 0;
	  registry.SetValue( "ShowJoinPart", m_bShowJoin );
   }
   if ( registry.GetValue( "AutoLog", ((CPconwApp*)AfxGetApp())->m_bAutoLog ) != TRUE )
   {
	  registry.SetValue( "AutoLog",0);
   }
   
   if ( registry.GetValue( "ShowChanList",m_bShowChanList) != TRUE )
   {
		registry.SetValue( "ShowChanList",1);
		m_bShowChanList=1;
   }
   if ( registry.GetValue( "ChatServer",m_IRCServerName) != TRUE )
   {
		registry.SetValue( "ChatServer","stargatenetworks.vservers.com");
		m_IRCServerName="stargatenetworks.vservers.com";
   }
   if ( registry.GetValue( "ChatServerPort",m_IRCServerPort) != TRUE )
   {
		registry.SetValue( "ChatServerPort",6969);
		m_IRCServerPort=6969;
   }
   if ( registry.GetValue( "BeepOnPrivate",m_BeepOnPrivate) != TRUE )
   {
		registry.SetValue( "BeepOnPrivate",1);
		m_BeepOnPrivate=1;
   }

   
   //m_BeepOnPrivate
   registry.Close();
	
   if(strnicmp(LPCSTR(LeaveMsg),"Killed",6)==0) LeaveMsg.LoadString(IDS_STRLEAVMSG);

   GetFilters();

}

void CPconwApp::DispatchMessage(LPCSTR Message,DWORD Color,LPCSTR User=NULL)
{
	if(m_bClosing) return;
	CPconwDoc *p_tgtDoc;
	CChatDoc *p_tgtChatDoc;
	LPCSTR l_User;
	if(User)
	{
		NewMessage(CString(User));
	}
	else
	{
		NewMessage(CString("Server"));
	}
	//if(!m_bSeperatePrivate) l_User=NULL;
//	else l_User=User;
    l_User=User;
	//This is a system Message
    if(((!l_User) || (User[0]==0)) || ((!m_bSeperatePrivate) && (User[0]!='#') ) )
	{
		p_tgtChatDoc=(CChatDoc *)m_ChannelDoc;
		p_tgtChatDoc->DisplayMsg(Message,Color);	
		return;
	}
	//Window already here, just print
	CObject *l_Doc;
	CString l_StrUser;
	l_StrUser=User;
	//l_StrUser.MakeLower();
	POSITION pos;
	CString key;
	BOOL l_bFound=FALSE;
	for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
	{
		((CPconwApp*)AfxGetApp())->m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
		if(key.CompareNoCase(l_StrUser)==0)
		{
			l_bFound=TRUE;
			break;
		}
					
	}
	
	
	if(l_bFound)
	{
		if(l_Doc->IsKindOf(RUNTIME_CLASS(CChatDoc))) 
		{
			p_tgtChatDoc=(CChatDoc*)l_Doc;
			p_tgtChatDoc->DisplayMsg(Message,Color);
		}
		if(l_Doc->IsKindOf(RUNTIME_CLASS(CPconwDoc))) 
		{
			p_tgtDoc=(CPconwDoc*)l_Doc;
			p_tgtDoc->DisplayMsg(Message,Color);
		}
		
	}
	//Create new window, and print it.
	else
	{
		if(User[0]=='#')
		{
			return;
		}
		m_CreateNewDocTitle = User;
		CChatFrame *ChannelWindow;
		p_tgtChatDoc=(CChatDoc *)m_ChatDocTemplate->CreateNewDocument();

		ChannelWindow=(CChatFrame *)m_ChatDocTemplate->CreateNewFrame((CDocument *)p_tgtChatDoc,NULL);

		//Just in case
		if(!ChannelWindow) 
		{
			CString l_tmpmsgnwe,l_tmpmsgerr;
			l_tmpmsgnwe.LoadString(IDS_STRNEWWINERR);
			l_tmpmsgerr.LoadString(IDS_STRERROR);

			MessageBox(NULL,l_tmpmsgnwe,l_tmpmsgerr,MB_ICONEXCLAMATION);
			p_tgtChatDoc=(CChatDoc *)m_ChannelDoc;
			p_tgtChatDoc->DisplayMsg(Message,Color);	
            return;
		}
		
		//m_WindowMap.SetAt(l_StrUser,p_tgtChatDoc);
		m_pDocTemplate->InitialUpdateFrame((CFrameWnd *)ChannelWindow,p_tgtChatDoc,TRUE);
		//m_CreateNewDocTitle.Empty();
		p_tgtChatDoc->DisplayMsg(Message,Color);	
	}
}









BOOL CPconwApp::ConnectSocket(void)
{

//This function will establish the logon protocol, etc, once connected.
CString msg;
	
    if(psharemem->nickname[0]=='#')
    {
        m_strHandle = _T("$");
        m_strHandle = psharemem->nickname+1;
    }
	else m_strHandle = psharemem->nickname;
	m_Channel = DefaultChannel;

//New TCP based chat
#ifdef IRCMODE
	DispatchMessage("Attempting to connect to chat server....",colorSTAT);
	//allocate the socket	
	m_pSocket = new CChatSocket();
	//Connect to the server
	m_pSocket->Create();
	//m_IRCServerName
	//m_IRCServerPort
	if(m_pSocket->Connect(m_IRCServerName,m_IRCServerPort))
	{
		AfxMessageBox("Connect Failed!!!");
	}
	m_pSocket->GetLastError();

	m_ControlSocket = new CControlSocket;

	if(!m_ControlSocket->Create(0x08AA, SOCK_DGRAM, FD_READ, "127.0.0.1"))
	{
		CString l_tmpmsgso,l_tmpmsgerr;
		l_tmpmsgso.LoadString(IDS_STRNONEWSOCK);
		l_tmpmsgerr.LoadString(IDS_STRERROR);
		::MessageBox(NULL,l_tmpmsgso,l_tmpmsgerr, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	//Init file xfer system once converted
	//XferFiles.InitFileXfer();
	//Add myself to the default channel
//	if(m_ChanList) m_ChanList->ProcessJoin(m_strHandle,m_Channel);	

	return TRUE;	
#endif //IRCMODE
//Here is the old UDP stuff
#ifndef IRCMODE
	
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	
	BYTE sendbuff[100];

	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));	
	 
	
//	CString msg;
	

	m_pSocket = new CChatSocket();
	if(!m_pSocket->Create(0x08AA, SOCK_DGRAM, FD_READ, "127.0.0.1"))
	{
		CString l_tmpmsgso,l_tmpmsgerr;
		l_tmpmsgso.LoadString(IDS_STRNONEWSOCK);
		l_tmpmsgerr.LoadString(IDS_STRERROR);
		::MessageBox(NULL,l_tmpmsgso,l_tmpmsgerr, MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//
	// Ok a new user is on chat, send our sigon message
	//


	m_Channel = DefaultChannel;
//	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
	pchatpkthdr->type = SIGNON;
	pchatpkthdr->ipaddress = 0;
	strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
	strcpy(pchatpkthdr->channel,LPCSTR(DefaultChannel));
	
	m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

	msg.LoadString(IDS_CHAT_HELP1);
	DispatchMessage(LPCSTR(msg),colorSTAT);

	pchatpkthdr->type = USERS_RESPOND;
	strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
	strcpy(pchatpkthdr->channel,LPCSTR(DefaultChannel));
	m_Channel = DefaultChannel;
	msg.LoadString(IDS_QUERYING_CHAT);
	DispatchMessage(msg,colorSTAT);
	m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);

  
	if(m_ChanList) m_ChanList->ProcessJoin(m_strHandle,m_Channel);	
	//Init File Transfer subsystem
	XferFiles.InitFileXfer();
	
	//Send Motd request
	pchatpkthdr->type = SERVER_COMMAND;
	pchatpkthdr->ipaddress = 0;
	pchatpktdata->framedata.textframe.text[0] = KALI_MOTD_REQ;
	pchatpktdata->framedata.textframe.size = 1;
	msg.LoadString(IDS_REQUESTING_SERVER_ID);
	DispatchMessage(msg,colorSTAT);
	m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
		pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
		0x08A9, "127.0.0.1", 0);

	return TRUE;
#endif //NOT IRCMODE
}

#define IN_BUFFLEN	500
void CPconwApp::ProcessPendingRead(void)
{
//New TCP based chat
#ifdef IRCMODE
	//CString m_TCPBuffA,m_TCPBuffB;
	char l_BuffA[2000];
	int l_Len;
	for(int j=0;j<2000;j++) l_BuffA[j]=NULL;
	do
	{
		l_Len=m_pSocket->Receive(&l_BuffA,1999);
		if(l_Len>=1)
		{
			l_BuffA[l_Len]=NULL;
			m_TCPBuffA+=l_BuffA;
		}

	}while(l_Len>0);
	
	if(!m_bInAdd)
	{
		//Now Dispatch....
		m_bInAdd=TRUE;
		int l_PosA=0;
		int	l_PosB=0;
		CString l_DispatchStr;
		int l_StrLen=m_TCPBuffA.GetLength();

		for(int i=0;i<l_StrLen;i++)
		{
			if((m_TCPBuffA[i]==0x0d)||(m_TCPBuffA[i]==0x0a))
			{
				l_PosB=i;//Skip the CR
				//ASSERT(l_PosB-l_PosA);
				l_DispatchStr.Empty();
				l_DispatchStr=m_TCPBuffA.Mid(l_PosA,l_PosB-l_PosA);
				//l_PosA=i+2;//Next char after the CR
				l_PosA=i++;
				//l_DispatchStr += "\n";
				l_DispatchStr.TrimLeft();
				l_DispatchStr.TrimRight();
				ParseKahnData(LPCSTR(l_DispatchStr));
			}
		}
		m_TCPBuffB.Empty();
		m_TCPBuffB=m_TCPBuffA.Mid(l_PosA,m_TCPBuffA.GetLength()-l_PosA);
		m_TCPBuffA=m_TCPBuffB;
		m_bInAdd=FALSE;
	}

#endif //IRCMODE

//Here is the old UDP stuff
#ifndef IRCMODE
	
	unsigned char buffer[1500];
	CString recvip;
	UINT recvport;
	int bytesreceived;

	bytesreceived = m_pSocket->ReceiveFrom( buffer, 1500, recvip, recvport, 0);
	if(bytesreceived != 0)
	{
		ParseKahnData((PBYTE)buffer, bytesreceived, recvip, recvport);
	}

#endif 
}

void CPconwApp::ParseKahnData(PBYTE data, int datalen, CString& sendingip, UINT recvport)
{

//New TCP based chat
#ifdef IRCMODE
#endif //IRCMODE

//Here is the old UDP stuff
#ifndef IRCMODE

	POSITION pos;
	DWORD t = GetTickCount();
	unsigned char sendbuff[1800];
	static WORD seq = 0;
	char seps[] = "\n";
	char *token;
	char strIP[20];
	CString msg, msg2;
	CString PartMsg;
	CUser *uFrom,*u;
	PCHAT_PACKET_HDR pchatpkthdr, plocalchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata, plocalchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	plocalchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	plocalchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	BOOL l_bValidChannel;
    CPconwDoc *l_Doc;

	CString l_ChannelRsp;

	union s_IPa
	{
		unsigned long lIP;
		unsigned char cIP[4];
	}IpAddr;

	IpAddr.lIP=pchatpkthdr->ipaddress;
	unsigned int IPa,IPb,IPc,IPd;
	IPa=IpAddr.cIP[0];
	IPb=IpAddr.cIP[1];
	IPc=IpAddr.cIP[2];
	IPd=IpAddr.cIP[3];

	if(m_bClosing) return;

	//****************************************************
	//	Check if the channel is one we have active.
	//	Set the flag if it is.....
	//****************************************************
	if(m_ChannelList.Find(pchatpkthdr->channel))
	{
		l_bValidChannel=TRUE;
	}
	else
	{
		l_bValidChannel=FALSE;
	}


	if(m_pSocket != NULL)
	{
		if(Users.Lookup(pchatpkthdr->name, (class CObject*&)uFrom))
		{
		if(uFrom->IsIgnored()) return;
		}
		CString l_lowerchan;
		// Figure out what the client wants to do with this packet
		switch(pchatpkthdr->type)
		{
			case DISPLAY_DATA:
				*data = 0x0a;
				token = strtok((char*)data+1, seps);
				while(token != NULL)
				{
					DispatchMessage(token,colorSTAT);
					token = strtok(NULL, seps);
				}
				//DispatchMessage(" ",colorSTAT);
				break;

			case NORMAL_TEXT:
			//	CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress);
				if(!Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					CString su = pchatpkthdr->name;
					u = new CUser(su);
					u->SetUserIP(pchatpkthdr->ipaddress);
					Users.SetAt(pchatpkthdr->name, u);
				}
				
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserJoin(pchatpkthdr->name);
		        }
				if(l_bValidChannel)
				{
					CString strText1;
					strText1=pchatpktdata->framedata.textframe.text;
					strText1.TrimLeft();
					strText1.TrimRight();
					if (strText1.Find("\n")!=-1) strText1=strText1.SpanExcluding("\n");
					
					msg = pchatpkthdr->name;
					msg += _T(": ");
					msg += strText1;
					if(m_bSpam)
					{
						if(m_strSpam.Compare(msg)!=0)
						{
							DispatchMessage(msg,GetColor(msg),pchatpkthdr->channel);
						}
					}
					else DispatchMessage(msg,GetColor(msg),pchatpkthdr->channel);
					m_strSpam=msg;
				}
				break;

			case HIGH_TEXT:
				//	CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress);
				if(!Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					CString su = pchatpkthdr->name;
					u = new CUser(su);
					u->SetUserIP(pchatpkthdr->ipaddress);
					Users.SetAt(pchatpkthdr->name, u);
				}
				
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserJoin(pchatpkthdr->name);
		        }
				if(l_bValidChannel)
				{
					CString strText1;
					strText1=pchatpktdata->framedata.textframe.text;
					strText1.TrimLeft();
					strText1.TrimRight();
					if (strText1.Find("\n")!=-1) strText1=strText1.SpanExcluding("\n");
					
					
					msg = _T("* ");
					msg += pchatpkthdr->name;
					msg += _T(" ");
					msg += strText1;
					if(m_bSpam)
					{
						if(m_strSpam.Compare(msg)!=0)
						{
							DispatchMessage(msg,colorACT,pchatpkthdr->channel);
						}
					}
					else DispatchMessage(msg,colorACT,pchatpkthdr->channel);
					m_strSpam=msg;
					
				}
				break;
			case PRIV_MSG:
			//	CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress);
				//((CMainFrame*)m_pMainWnd)->dlgUsers.AddItem((char *)User);
				if(!Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					CString su = pchatpkthdr->name;
					u = new CUser(su);
					u->SetUserIP(pchatpkthdr->ipaddress);
					Users.SetAt(pchatpkthdr->name, u);
				
				}
				if (m_bRejecting)
				{
					//We don't want to hear from you.
					if(pchatpkthdr->ipaddress==0) return;
					pchatpkthdr->type = HIGH_TEXT;
					pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
					strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
					strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
					strcpy(pchatpktdata->framedata.textframe.text, m_strRejectMsg);
					pchatpktdata->framedata.textframe.size = m_strRejectMsg.GetLength()+1;
					m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
						pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
						0x08A9, "127.0.0.1", 0);
					return;
				}
				if(pchatpkthdr->ipaddress==0) return;
				msg = _T("<");
				msg += pchatpkthdr->name;
				msg += _T("> ");
				msg += pchatpktdata->framedata.textframe.text;
				MessageBeep((unsigned int)-1);
				
				DispatchMessage(msg,colorPRIV,pchatpkthdr->name);
				AddTabUser(pchatpkthdr->name);
				if(m_bAway) 
				{
				pchatpkthdr->type = HIGH_TEXT;
				pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
				strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
				strcpy(pchatpktdata->framedata.textframe.text, m_strAwayMsg);
				pchatpktdata->framedata.textframe.size = m_strAwayMsg.GetLength()+1;
				m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				
				}
				break;
			
			case USERS_RESPOND:
			//	CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress);
				//	Add the code to respond for ALL valid channels
				//m_ChannelList
				//CStringList
				
				pos=m_ChannelList.GetHeadPosition();
				while(pos)
				{
					//l_ChannelRsp=m_ChannelList.GetAt(pos);
					l_ChannelRsp=m_ChannelList.GetNext(pos);
					plocalchatpkthdr->type = USERS_RESPONSE;
					if(pchatpkthdr->ipaddress==0) return;
					plocalchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
					strcpy(plocalchatpkthdr->name, LPCSTR(m_strHandle));
					strcpy(plocalchatpkthdr->channel, l_ChannelRsp);
					plocalchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
					m_pSocket->SendTo(plocalchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
				}
				
				if(m_bAway) 
				{
				pchatpkthdr->type = HIGH_TEXT;
				pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
				strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
				strcpy(pchatpktdata->framedata.textframe.text, m_strAwayMsg);
				pchatpktdata->framedata.textframe.size = m_strAwayMsg.GetLength()+1;
				m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				
				}
				
				break;	

			case USERS_RESPONSE:
	
               	
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();

	            if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserJoin(pchatpkthdr->name);
		        }

                
                //Users.Lookup(pchatpkthdr->name, (class CObject*&)u)
				//if(CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress))
				if(m_ChanList) m_ChanList->ProcessJoin(pchatpkthdr->name,pchatpkthdr->channel);
				if(Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					if(m_WhoTimer<=WHOTIMEOUT)
					{
						msg = pchatpkthdr->name;
						msg2.LoadString(IDS_USER_ON);
						msg += msg2;
						msg += pchatpkthdr->channel;
						sprintf(strIP,"(%d.%d.%d.%d)",IPa,IPb,IPc,IPd);
						msg += strIP;
						DispatchMessage(msg,colorSTAT);
					}
					
					
					u->SetUserIP(pchatpkthdr->ipaddress);
					
				}
				else
				{
					
					msg = pchatpkthdr->name;
					msg2.LoadString(IDS_USER_ON);
					msg += msg2;
					msg += pchatpkthdr->channel;
					sprintf(strIP,"(%d.%d.%d.%d)",IPa,IPb,IPc,IPd);
					msg += strIP;
					if(m_bShowJoin) DispatchMessage(msg,colorSTAT);
											
					CString su = pchatpkthdr->name;
					u = new CUser(su);
					u->SetUserIP(pchatpkthdr->ipaddress);
					Users.SetAt(pchatpkthdr->name, u);
					
					//((CMainFrame*)m_pMainWnd)->dlgUsers.AddItem(pchatpkthdr->name);
					//AfxGetMainWnd

				}
				break;

			case PING_REQUEST:
			//	CheckAddUser(pchatpkthdr->name,pchatpkthdr->ipaddress);
				if(pchatpkthdr->ipaddress==0) return;
				plocalchatpkthdr->type = PING_RESPONSE;
				plocalchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
				strcpy(plocalchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(plocalchatpkthdr->channel, m_Channel);
				plocalchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
				//support for DOS's Ping scheme, until I change this one.
				strcpy(plocalchatpktdata->framedata.textframe.text,pchatpktdata->framedata.textframe.text);
				m_pSocket->SendTo(plocalchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
				break;

			case PING_RESPONSE:
			 	t = GetCurrentTime();
				if(Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					CString l_tmpmsgpr;
					l_tmpmsgpr.LoadString(IDS_STRPINGRSP);
					sprintf((PCHAR)sendbuff, "%s %s %ldms",LPCSTR(l_tmpmsgpr), pchatpkthdr->name, *(DWORD *)pchatpkthdr->channel);
					DispatchMessage((PCHAR)sendbuff,colorSTAT);
				}
				break;

			case SIGNON:
                
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserJoin(pchatpkthdr->name);
		        }
				if(m_ChanList) m_ChanList->ProcessJoin(pchatpkthdr->name,pchatpkthdr->channel);
				if(Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					if(m_WhoTimer<=WHOTIMEOUT)
					{
						msg = pchatpkthdr->name;
						msg2.LoadString(IDS_USER_ON);
						msg += msg2;
						msg += pchatpkthdr->channel;
						sprintf(strIP,"(%d.%d.%d.%d)",IPa,IPb,IPc,IPd);
						msg += strIP;
						DispatchMessage(msg,colorSTAT);
					}
					u->SetUserIP(pchatpkthdr->ipaddress);
					
				}
				else
				{
					
					msg = pchatpkthdr->name;
					msg2.LoadString(IDS_USER_ON);
					msg += msg2;
					msg += pchatpkthdr->channel;
					sprintf(strIP,"(%d.%d.%d.%d)",IPa,IPb,IPc,IPd);
					msg += strIP;
					if(m_bShowJoin) DispatchMessage(msg,colorSTAT);
					
					CString su = pchatpkthdr->name;
					u = new CUser(su);
					u->SetUserIP(pchatpkthdr->ipaddress);
					Users.SetAt(pchatpkthdr->name, u);

					//((CMainFrame*)m_pMainWnd)->dlgUsers.AddItem(pchatpkthdr->name);
				
				  }

				if(l_bValidChannel)
				{
					XferFiles.ShowOffers(pchatpkthdr->ipaddress);
					sprintf(strIP,"(%d.%d.%d.%d)",IPa,IPb,IPc,IPd);
					CString l_tmpmsgij;
					l_tmpmsgij.LoadString(IDS_STRISJOINING);
					wsprintf((char*)sendbuff,"%s %s %s %s",pchatpkthdr->name,LPCSTR(l_tmpmsgij), pchatpkthdr->channel,strIP);
					DispatchMessage((char*)sendbuff,colorSTAT);
				if(m_bAway) 
					{
					if(pchatpkthdr->ipaddress==0) break;
					pchatpkthdr->type = HIGH_TEXT;
					pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
					strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
					strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
					strcpy(pchatpktdata->framedata.textframe.text, m_strAwayMsg);
					pchatpktdata->framedata.textframe.size = m_strAwayMsg.GetLength()+1;
					m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
						pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
						0x08A9, "127.0.0.1", 0);
					
					}
					
				}

				break;

			case SIGNOFF:
                
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserPart(pchatpkthdr->name);
		        }
				

				if(Users.Lookup(pchatpkthdr->name, (class CObject*&)u))
				{
					CString strKilled;
					if(!strcmp(pchatpkthdr->channel, LPCSTR(m_Channel)))
					{
						if(m_ChanList) m_ChanList->ProcessPart(pchatpkthdr->name,pchatpkthdr->channel);
						CString l_tmpmsgil;
						l_tmpmsgil.LoadString(IDS_STRISLEAVING);
						wsprintf((char*)sendbuff,"%s %s (%s)", pchatpkthdr->name,LPCSTR(l_tmpmsgil),pchatpktdata->framedata.textframe.text);
						if(m_bShowJoin) DispatchMessage((char*)sendbuff,colorSTAT);
						else if(strnicmp(pchatpktdata->framedata.textframe.text,"Killed by",9)==0) DispatchMessage((char*)sendbuff,colorSTAT);
					}
				//	else
				//	{
				//		if(strnicmp(pchatpktdata->framedata.textframe.text,"Killed by",8)==0) 
				//		{
				//			if(m_ChanList) m_ChanList->ProcessPart(pchatpkthdr->name,pchatpkthdr->channel);
				//			wsprintf((char*)sendbuff,"%s is leaving chat (%s)", pchatpkthdr->name,pchatpktdata->framedata.textframe.text);
				//			if(m_bShowJoin) DispatchMessage((char*)sendbuff,colorSTAT);
				//		}
				//	}
					//((CMainFrame*)m_pMainWnd)->dlgUsers.RemoveExactItem(pchatpkthdr->name);
					Users.RemoveKey(pchatpkthdr->name);
					delete u;
				}
			
				break;
			case PART_CHANNEL:
				l_lowerchan=pchatpkthdr->channel;
				l_lowerchan.MakeLower();
                
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
	            {
		            l_Doc->UserPart(pchatpkthdr->name);
		        }
				
                {
					if(m_ChanList) m_ChanList->ProcessPart(pchatpkthdr->name,pchatpkthdr->channel);
					CString l_tmpmsgilc;
					l_tmpmsgilc.LoadString(IDS_STRLVGCHAN);
					PartMsg.Format("%s %s %s",pchatpkthdr->name,LPCSTR(l_tmpmsgilc),pchatpkthdr->channel);
					DispatchMessage((char*)LPCSTR(PartMsg),colorSTAT);
				}
				break;


			case FILEREQ:
				if(pchatpkthdr->ipaddress==0) break;
				XferFiles.ProcessFileReq((char *)pchatpkthdr);
				break;
			
			case FILEINFO:
				if(pchatpkthdr->ipaddress==0) break;
				XferFiles.ProcessFileInfo((char *)pchatpkthdr);
				break;
			
			case FILEFRAG:
				if(pchatpkthdr->ipaddress==0) break;
				XferFiles.ProcessFileFrag((char *)pchatpkthdr);
				break;
			
			case FILEACK:
				if(pchatpkthdr->ipaddress==0) break;
				XferFiles.ProcessAck((char *)pchatpkthdr);
				break;
			case FILEFIN:
				if(pchatpkthdr->ipaddress==0) break;
				XferFiles.ProcessFileFin((char *)pchatpkthdr);
				break;

//FILEREQ

//			BINARY_DATA:
//			BINARY_REQUEST:

			default:
				break; 
		}
	}

#endif //NOT IRCMODE
}

int CPconwApp::ExitInstance() 
{
#ifndef IRCMODE
	//AfxMessageBox("CPconwApp::ExitInstance Entered....");
	BYTE sendbuff[1200];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	
	
	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
			
	pchatpkthdr->type = SIGNOFF;
	pchatpkthdr->ipaddress = 0;

	if (m_pSocket != NULL)
	{
		psharemem->hChat=NULL;
		strcpy(pchatpktdata->framedata.textframe.text,LPCSTR(LeaveMsg));
		pchatpktdata->framedata.textframe.size = LeaveMsg.GetLength()+1;
		strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
		strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
		m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
		pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
		0x08A9, "127.0.0.1", 0);
		
		BYTE Buffer[50];
		m_pSocket->ShutDown();

		while(m_pSocket->Receive(Buffer,50) > 0);
	}
#endif
#ifdef IRCMODE
	CString l_leavemsg;
	l_leavemsg.Format("quit :%s\n",LeaveMsg);
	if(m_pSocket)
	{
		m_pSocket->Send(LPCSTR(l_leavemsg),l_leavemsg.GetLength());
		m_pSocket->ShutDown(2);
		m_pSocket->Close();
	}
	if(m_ControlSocket)
	{
		m_ControlSocket->ShutDown(2);
		m_ControlSocket->Close();
		delete m_ControlSocket;
	}

#endif
	delete m_pSocket;
	m_pSocket = NULL;
    //Delete lots of stuff
    POSITION pos;
    CString l_key;
    CUser *u;
    pos=Users.GetStartPosition();
    while (pos)
    {
        Users.GetNextAssoc(pos,l_key,(class CObject*&)u);
        delete u;
        Users.RemoveKey(l_key);
    }
	CNewMessage *l_NewMessage;
    pos = m_WindowList.GetStartPosition();
	while (pos)
    {
        m_WindowList.GetNextAssoc(pos,l_key,(class CObject*&)l_NewMessage);
        delete l_NewMessage;
        m_WindowList.RemoveKey(l_key);
    }
//m_UserPings
	CUserPing *l_UserPing;
    pos = m_UserPings.GetStartPosition();
	while (pos)
    {
        m_UserPings.GetNextAssoc(pos,l_key,(class CObject*&)l_UserPing);
        delete l_UserPing;
        m_UserPings.RemoveKey(l_key);
    }
    
	CurrTabUser = FirstTabUser;

    while(CurrTabUser)
    {
        TempTabUser=CurrTabUser;
        CurrTabUser=CurrTabUser->Next;
        delete TempTabUser;
    }
    
    SFilters *TempFilter;
    CurrFilter=FirstFilter;
	
	while(CurrFilter)
	{
		TempFilter=CurrFilter;
		CurrFilter=CurrFilter->Next;
        delete TempFilter;
	}
        
    //Channel list
    delete m_LaunchCfgDlg;
	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
	{
      return CWinApp::ExitInstance();
	}

	if ( registry.Open("SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
	{
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat"))
	  {
		if(registry.Open("SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			return CWinApp::ExitInstance();
		}
				
	  }
	  else
	  {
		return CWinApp::ExitInstance();
	  }
    }
	//Write the reg...
	registry.SetValue( "ShowChanList",m_bShowChanList);
      
	//close the reg....
	registry.Close();	
	
	return CWinApp::ExitInstance();
}

void CPconwApp::SendMsg(CString& strText,LPCSTR Window)
{
//Proccess chat commands & send data
	CPconwDoc *l_Doc;	
	CString msg;
	BYTE sendbuff[1500];
	BYTE localbuff[500];
	PCHAR token;
	char seps[] = "\n \t";
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
    
    CString l_ChannelRsp;
    POSITION pos;

//New TCP based chat
#ifdef IRCMODE
	CString l_Message;	
	l_Message = strText;// + "\n";
	if((Window) && (strText[0]!='/'))
	{
		CString l_NewMessage;
		l_NewMessage.Format("PRIVMSG %s :%s\n",Window,l_Message);
		m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
		
		l_NewMessage.Format("<%s> %s",m_strHandle,l_Message);
		if(Window[0]!='#')
		{
			DispatchMessage(l_NewMessage,colorPRIV,Window);
		}
		else
		{
			DispatchMessage(l_NewMessage,GetColor(l_NewMessage),Window);//
		}

		return;
	}
	else
	{	
		if(strText[0]=='/')
		{
			CString l_StrTmp,l_StrCmd,l_StrParm,l_StrWork;	
			int l_iEndCmd;
			l_StrTmp = l_Message.Right(l_Message.GetLength()-1);
			l_Message = l_StrTmp;
			l_iEndCmd = l_Message.Find(' ');
			l_StrCmd = l_Message.Left(l_iEndCmd);
			l_StrWork=l_Message.Mid(l_iEndCmd+1,l_Message.GetLength()-l_iEndCmd);
			l_Message=l_StrWork;
			if(l_StrCmd.IsEmpty())
			{
				//Only a command, no parms.
				l_StrCmd=l_Message;
			}
			//Start parsing commands
			if(l_StrCmd.CompareNoCase("users")==0)
			{
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pchatpktdata->framedata.textframe.text[0] = KALI_QUERY_SERVER;
				pchatpktdata->framedata.textframe.size = 1;
				msg.LoadString(IDS_QUERYING_SERVER);
				DispatchMessage(msg,colorSTAT);
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;				

			}

		/*	if(l_StrCmd.CompareNoCase("serverkill")==0)
			{
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pchatpktdata->framedata.textframe.text[0] = 0x62;
				pchatpktdata->framedata.textframe.text[1] = '1';
				pchatpktdata->framedata.textframe.text[2] = '4';
				pchatpktdata->framedata.textframe.text[3] = '2';
				pchatpktdata->framedata.textframe.text[4] = 0;

				pchatpktdata->framedata.textframe.size = 1;
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;				
			}
*/
			if(l_StrCmd.CompareNoCase("swhois")==0)
			{
				S_Whois pktwhois;

				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pktwhois.command=KALI_WHOIS_REQ;
				
				strcpy(pktwhois.nick,l_Message);
				
				memcpy(pchatpktdata->framedata.textframe.text,&pktwhois,sizeof(S_Whois));
				pchatpktdata->framedata.textframe.size = l_Message.GetLength()+2;
				msg=_T("** Requesting user information...");
				DispatchMessage(msg,colorSTAT);
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;				
			}
			
			if(l_StrCmd.CompareNoCase("sban")==0)
			{
				S_kill pktkill;
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pktkill.command=KALI_BAN_USER;
				strcpy(pktkill.nick,l_Message);
				memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
				pchatpktdata->framedata.textframe.size = 18;
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;
			}
			if(l_StrCmd.CompareNoCase("sunban")==0)
			{
				S_kill pktkill;
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pktkill.command=KALI_BAN_CLEAR;
				strcpy(pktkill.nick,l_Message);
				memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
				pchatpktdata->framedata.textframe.size = 18;
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;
			}

			if(l_StrCmd.CompareNoCase("sblist")==0)
			{
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pchatpktdata->framedata.textframe.text[0] = KALI_BAN_LIST;
				pchatpktdata->framedata.textframe.size = 1;
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;
			}			
				
			if(l_StrCmd.CompareNoCase("skill")==0)
			{
				S_kill pktkill;
				pktkill.reason[0]=NULL;
				CString tempstr,tmpusername,tmpmsg;
				tmpusername=GetWordNum(0,l_Message);
				if(tmpusername[0]=='\"')
				{
					//Extract the middle, leaving off the quotes.
					int l_iQuote;
					tempstr=l_Message.Right(l_Message.GetLength()-1);
					l_iQuote=tempstr.Find("\"");
					if(l_iQuote>=0)
					{
						//Get the rest of the nick, and the reason
						tmpusername=tempstr.Left(l_iQuote);
						tmpmsg=tempstr.Right(tempstr.GetLength()-2-tmpusername.GetLength());
						if(tmpmsg[0]==':')
						{
							CString l_Safe;
							l_Safe=tmpmsg.Right(tmpmsg.GetLength()-1);
							tmpmsg=l_Safe;
						}
					}
					
				}
				else
				{
					CString l_FixString;
					l_FixString.Format(":%s",l_Message);
					tmpmsg=GetWordNum(1,l_FixString);
				
				}
					
				pchatpkthdr->type = SERVER_COMMAND;
				pchatpkthdr->ipaddress = 0;
				pktkill.command=KALI_KILL_USER;
				strcpy(pktkill.nick,tmpusername);
				if(!tmpmsg.IsEmpty()) strncpy(pktkill.reason,tmpmsg,199);
				memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
				pchatpktdata->framedata.textframe.size = tmpmsg.GetLength()+19;
				
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
					pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;			
			}
			if(l_StrCmd.CompareNoCase("ignore")==0)
			{
				CString l_Ignoree;
				CString l_IgnoreMsg;
				POSITION posignore;
				l_Ignoree = GetWordNum(0,l_Message);
				//Lookup to see if this user is already
				//in the Ignore list (m_IgnoreList)
				posignore=m_IgnoreList.Find(l_Ignoree);
				if(posignore==0)
				{
					m_IgnoreList.AddHead(l_Ignoree);//
					l_IgnoreMsg.Format("** Ignoreing user %s.",l_Ignoree);
					DispatchMessage(l_IgnoreMsg,colorSTAT);
					return;
				}
				//else, stop ignoring
				else
				{
					m_IgnoreList.RemoveAt(posignore);
					l_IgnoreMsg.Format("** No longer ignoreing user %s.",l_Ignoree);
					DispatchMessage(l_IgnoreMsg,colorSTAT);
					return;
				}
			}


			if(l_StrCmd.CompareNoCase("dcc")==0)
			{
				CString l_DCCCmd;
				l_DCCCmd=GetWordNum(0,l_Message);

				if(l_DCCCmd.CompareNoCase("chat")==0)
				{
					CString l_strTarget;
					int l_iListenPort;
					int l_iMyIp;
					l_iMyIp=psharemem->myip;
					CDCCCDoc *l_DCCCDoc;
					CDCCCFrm *l_DCCCFrame;
					l_strTarget = GetWordNum(1,l_Message);
					m_CreateNewDocTitle=l_strTarget;

					l_DCCCDoc=(CDCCCDoc *)m_DCCCDocTemplate->CreateNewDocument();
					l_DCCCFrame=(CDCCCFrm *)m_DCCCDocTemplate->CreateNewFrame((CDocument *)l_DCCCDoc,NULL);
					m_DCCCDocTemplate->InitialUpdateFrame((CFrameWnd *)l_DCCCFrame,l_DCCCDoc,TRUE);
					//Set the options here....
					//get the view
					POSITION pos;
					pos = l_DCCCDoc->GetFirstViewPosition();
					CDCCCView* pFirstView = (CDCCCView*)l_DCCCDoc->GetNextView( pos );
					l_iListenPort = pFirstView->SetDCCCOptions(CHAT_INITIATE);			
					
					//Send the private message to the target here....
					
					m_CreateNewDocTitle.Empty();
					
					l_StrWork.Format("PRIVMSG %s :\x001\DCC CHAT chat %u %u\x001\n",l_strTarget,htonl(l_iMyIp),l_iListenPort);
					l_Message = l_StrWork;
					m_pSocket->Send(LPCSTR(l_Message),l_Message.GetLength());

					return;
				}
				if(l_DCCCmd.CompareNoCase("send")==0)
				{
					CString l_strTarget,l_FileName;
					int l_iListenPort;
					int l_iMyIp;
					l_iMyIp=psharemem->myip;
					CDCCFDoc *l_DCCFDoc;
					CDCCFFrm *l_DCCFFrame;
					l_strTarget = GetWordNum(1,l_Message);
					CString l_tmpparse;
					int l_iFindName;
					l_iFindName=l_Message.Find(' ');
					if(l_iFindName==-1) return;
					l_tmpparse = l_Message.Right(l_Message.GetLength()-l_iFindName);
					l_Message = l_tmpparse;
					l_Message.TrimLeft();
					l_iFindName=l_Message.Find(' ');
					if(l_iFindName==-1) return;
					
					if(l_Message[l_Message.GetLength()-1]=='\"')
					{
						l_tmpparse=l_Message.Left(l_Message.GetLength()-1);
						l_Message=l_tmpparse;
					}
 					l_Message.TrimLeft();
					l_FileName = l_Message.Right(l_Message.GetLength()-l_iFindName);
					l_FileName.TrimLeft();
					CString l_StripFileName;
					int l_iLastSlash;
					l_iLastSlash=l_FileName.ReverseFind('\\');
					if(l_iLastSlash!=-1)
					{
						l_StripFileName = l_FileName.Right(l_FileName.GetLength()-(l_iLastSlash+1));
					}
					else
					{
						l_StripFileName = l_FileName;
					}
					
					
					CFile l_CFSize;
					int l_iFileSize;

					if(l_CFSize.Open(l_FileName,CFile::modeRead|CFile::shareDenyNone))
					{
						l_iFileSize=l_CFSize.GetLength();
						l_CFSize.Abort();
						m_CreateNewDocTitle=l_strTarget;
						l_DCCFDoc=(CDCCFDoc *)m_DCCFDocTemplate->CreateNewDocument();
						l_DCCFFrame=(CDCCFFrm *)m_DCCFDocTemplate->CreateNewFrame((CDocument *)l_DCCFDoc,NULL);
						m_DCCFDocTemplate->InitialUpdateFrame((CFrameWnd *)l_DCCFFrame,l_DCCFDoc,TRUE);
						//Set the options here....
						//get the view
						POSITION pos;
						pos = l_DCCFDoc->GetFirstViewPosition();
						CDCCFView* pFirstView = (CDCCFView*)l_DCCFDoc->GetNextView( pos );
						l_iListenPort = pFirstView->ListenDCC(l_strTarget,l_FileName);
						//Send the private message to the target here....
						m_CreateNewDocTitle.Empty();
						
						l_StrWork.Format("PRIVMSG %s :\x001\DCC send %s %u %u %u\x001\n",l_strTarget,l_StripFileName,htonl(l_iMyIp),l_iListenPort,l_iFileSize);
						l_Message = l_StrWork;
						m_pSocket->Send(LPCSTR(l_Message),l_Message.GetLength());

						return;
					}
					else
					{
						MessageBox(AfxGetMainWnd()->m_hWnd,"Unable to open file!","Error",MB_OK);
						return;
					}
					
				}
			}			
			
			if(l_StrCmd.CompareNoCase("finger")==0)
			{
				
				CString l_strLowerFinger;
				l_strLowerFinger.Format("Finger: %s",l_Message);
				l_strLowerFinger.MakeLower();
				m_CreateNewDocTitle=l_Message;
				CFingerDoc *l_FingerDoc;
				CFingerFrame *l_FingerFrame;
				l_FingerDoc=(CFingerDoc *)m_FingerDocTemplate->CreateNewDocument();
				l_FingerFrame=(CFingerFrame *)m_FingerDocTemplate->CreateNewFrame((CDocument *)l_FingerDoc,NULL);
				m_pDocTemplate->InitialUpdateFrame((CFrameWnd *)l_FingerFrame,l_FingerDoc,TRUE);
				m_CreateNewDocTitle.Empty();
				return;
			}		

			if(l_StrCmd.CompareNoCase("join")==0)
			{
				
				CString l_JoinChan;
				l_JoinChan=l_Message;
				l_StrWork.Format("JOIN %s\n",l_Message);
				l_Message = l_StrWork;
				m_pSocket->Send(LPCSTR(l_Message),l_Message.GetLength());
				//Get The Channel Mode....
				l_StrWork.Format("MODE %s\n",l_JoinChan);
				l_Message = l_StrWork;
				m_pSocket->Send(LPCSTR(l_Message),l_Message.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("kill")==0)
			{
				CString l_NewMessage;
				l_NewMessage.Format("kill :%s\n",l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			//This is only for /query type messages.
			if(l_StrCmd.CompareNoCase("pmsg")==0)
			{
				//what I really need to do is get the nick out of the /msg
				//and replace it with privmsg				
				if(!Window) 
				{
					return;
				}

				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%s\n",Window,l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("msg")==0)
			{
				//what I really need to do is get the nick out of the /msg
				//and replace it with privmsg				
				CString l_NewMessage,l_strmsg1,l_strmsg2;
				CString l_StrParm1,l_StrParm2,l_StrWork1;
				l_strmsg1=GetWordNum(0,l_Message);
				l_strmsg2=GetWordNum(1,l_Message);

				
				int l_iEndParm1;
				l_iEndParm1=l_Message.Find(' ');
				l_StrWork1=l_Message.Right(l_Message.GetLength()-l_iEndParm1-1);
				l_Message=l_StrWork1;

				l_NewMessage.Format("PRIVMSG %s :%s\n",l_strmsg1,l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("%s: *%s* %s",m_strHandle,l_strmsg1,l_Message);
				DispatchMessage(l_NewMessage,colorPRIV,Window);
				return;
			}
			if(l_StrCmd.CompareNoCase("notice")==0)
			{
				//what I really need to do is get the nick out of the /msg
				//and replace it with privmsg				
				CString l_NewMessage,l_strmsg1,l_strmsg2;
				CString l_StrParm1,l_StrParm2,l_StrWork1;
				l_strmsg1=GetWordNum(0,l_Message);
				l_strmsg2=GetWordNum(1,l_Message);

				
				int l_iEndParm1;
				l_iEndParm1=l_Message.Find(' ');
				l_StrWork1=l_Message.Right(l_Message.GetLength()-l_iEndParm1-1);
				l_Message=l_StrWork1;

				l_NewMessage.Format("NOTICE %s :%s\n",l_strmsg1,l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("%s: NOTICE -%s- %s",m_strHandle,l_strmsg1,l_Message);
				DispatchMessage(l_NewMessage,colorACT,l_strmsg1);
				return;
			}

			if(l_StrCmd.CompareNoCase("me")==0)
			{
				if(!Window) return;
				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%cACTION %s%c\n",Window,0x01,l_Message,0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("* %s %s",m_strHandle,l_Message);
				DispatchMessage(l_NewMessage,colorACT,Window);
				return;

			}
			if(l_StrCmd.CompareNoCase("ctcp")==0)
			{
				//if(!Window) return;
				CString l_NewMessage;
				CString l_Target,l_CTCPCmd;
				l_Target=GetWordNum(0,l_Message);
				//l_CTCPCmd=GetWordNum(1,l_Message);
				l_NewMessage.Format("PRIVMSG %s :%c%s%c\n",l_Target,0x01,l_Message.Right(l_Message.GetLength()-(l_Target.GetLength()+1)),0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("%s: CTCP *%s* %s",m_strHandle,l_Target,l_Message.Right(l_Message.GetLength()-(l_Target.GetLength()+1)));
				DispatchMessage(l_NewMessage,colorACT,Window);
				return;

			}
			if(l_StrCmd.CompareNoCase("offer")==0)
			{
				//
				CString l_FileName;
				CFile l_cfTest;
				int l_iOfferNum;
				int i;
				for(i=0;i<MAXOFFERED;i++)
				{
					//
					if(m_OfferedFiles[i].IsEmpty())
					{
						//here's a spot!
						break;
					}
				}
				if(i==MAXOFFERED) 
				{
					//woops, no more offers available.
					CString l_ErrMsg;
					l_ErrMsg.Format("** No more files can be offered! (Max. of %u)",MAXOFFERED);
					DispatchMessage(l_ErrMsg,colorSTAT,NULL);
					return;
				}
				l_FileName=l_Message;
				if(l_cfTest.Open(l_FileName,CFile::modeRead|CFile::shareDenyNone))
				{
					//put it in the slot
					l_cfTest.Abort();
					m_OfferedFiles[i]=l_FileName;
					//Advertise it in all channels.
					POSITION pos;
					CString key,l_StripFileName;
					int l_iLastSlash;
					l_iLastSlash=l_FileName.ReverseFind('\\');
					if(l_iLastSlash!=-1)
					{
						l_StripFileName = l_FileName.Right(l_FileName.GetLength()-(l_iLastSlash+1));
					}
					else
					{
						l_StripFileName = l_FileName;
					}		
					for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
					{
						m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
						if (key[0]=='#')
						{
							//Msg that you aren't away
							//Send a message to each channel open
							CString l_NewMessage;
							l_NewMessage.Format("PRIVMSG %s :%cACTION is offering file: GetFile:%s@%s%c\n",key,0x01,l_StripFileName,m_strHandle,0x01);
							m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
							l_NewMessage.Format("* %s is offering file: GetFile:%s@%s",m_strHandle,l_StripFileName,m_strHandle);
							DispatchMessage(l_NewMessage,colorACT,key);							
						}
					}
					return;

				}
				else
				{
					//File doesn't quite exist...
					CString l_ErrMsg;
					l_ErrMsg.Format("** Unable to open file: %s",l_FileName);
					DispatchMessage(l_ErrMsg,colorSTAT,NULL);
					return;
				}
				return;

			}
			if(l_StrCmd.CompareNoCase("deoffer")==0)
			{
				CString l_FileName;
				l_FileName=GetWordNum(0,l_Message);
				CFile l_cfTest;
				int l_iOfferNum;
				int i;
				for(i=0;i<MAXOFFERED;i++)
				{
					//
					if(m_OfferedFiles[i].CompareNoCase(l_FileName)==0)
					{
						//here it is
						m_OfferedFiles[i].Empty();
						CString l_ErrMsg;
						l_ErrMsg.Format("** File no longer being offered");
						DispatchMessage(l_ErrMsg,colorSTAT,NULL);
						break;
					}
				}
				if(i==MAXOFFERED) 
				{
					//woops, no more offers available.
					CString l_ErrMsg;
					l_ErrMsg.Format("** That file is not being offered!");
					DispatchMessage(l_ErrMsg,colorSTAT,NULL);
					return;
				}
				return;
			}
			if(l_StrCmd.CompareNoCase("showoffers")==0)
			{
				//Advertise it in all channels.
				POSITION pos;
				CString key;
				int i;
				for(i=0;i<MAXOFFERED;i++)
				{
					//
					if(!m_OfferedFiles[i].IsEmpty())
					{
						CString l_StripFileName;
						int l_iLastSlash;
						l_iLastSlash=m_OfferedFiles[i].ReverseFind('\\');
						if(l_iLastSlash!=-1)
						{
							l_StripFileName = m_OfferedFiles[i].Right(m_OfferedFiles[i].GetLength()-(l_iLastSlash+1));
						}
						else
						{
							l_StripFileName = m_OfferedFiles[i];
						}		
						//here's a file!
						for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
						{
							m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
							if (key[0]=='#')
							{
								//Msg that you aren't away
								//Send a message to each channel open
								CString l_NewMessage;
								l_NewMessage.Format("PRIVMSG %s :%cACTION is offering file: GetFile:%s@%s%c\n",key,0x01,l_StripFileName,m_strHandle,0x01);
								m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
								l_NewMessage.Format("* %s is offering file: GetFile:%s@%s",m_strHandle,l_StripFileName,m_strHandle);
								DispatchMessage(l_NewMessage,colorACT,key);							
							}
						}
					}
				}		
			return;
			}
			
			if(l_StrCmd.CompareNoCase("ping")==0)
			{
				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%cPING %d%c\n",l_Message,0x01,GetTickCount(),0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("** Pinging %s\n",l_Message);
				DispatchMessage(l_NewMessage,colorSTAT,NULL);
				return;
			}
			if(l_StrCmd.CompareNoCase("tping")==0)
			{
				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%cUDPPING%c\n",l_Message,0x01,0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("** Pinging %s\n",l_Message);
				DispatchMessage(l_NewMessage,colorSTAT,NULL);
				return;
			}
			if(l_StrCmd.CompareNoCase("help")==0)
			{
				//CString l_NewMessage;
				//l_NewMessage.Format("For help and a list of all the Commands, please go to: http://stargatenetworks.com/help/pcon97cm.html");
				//DispatchMessage(l_NewMessage,colorSTAT,NULL);
				ShellExecute(NULL,"open","http://stargatenetworks.com/help/pcon97cm.html","","",SW_SHOWNORMAL);
				return;
			}
			if(l_StrCmd.CompareNoCase("quit")==0)
			{
				CString l_NewMessage;
				if(l_Message.IsEmpty()) l_Message = LeaveMsg;
				l_NewMessage.Format("QUIT :%s\n",l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("list")==0)
			{
				CString l_NewMessage;
				l_NewMessage.Format("LIST \n");
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("whois")==0)
			{
				CString l_NewMessage;
				l_NewMessage.Format("whois %s\n",l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("who")==0)
			{
				CString l_NewMessage,l_WhoParms;
				l_WhoParms=GetWordNum(1,l_Message);
				if(l_WhoParms.IsEmpty())
				{
					l_NewMessage.Format("WHO *\n");
				}
				else
				{
					l_NewMessage.Format("WHO %s\n",l_WhoParms);
				}
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("nick")==0)
			{
				CString l_NewMessage;
			//	m_strHandle=l_Message;
				l_NewMessage.Format("nick %s\n",l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			if(l_StrCmd.CompareNoCase("kick")==0)
			{
				//Need to parse out nick & message, then add the ':' to the message
				if((!Window) || (l_Message[0]=='#'))
				{
					CString l_StrParm1,l_StrParm2,l_StrReason,l_StrWork1;
					int l_iEndParm1;
					//get the channel
					l_StrParm1=l_Message.SpanExcluding(" ");
					l_iEndParm1=l_Message.Find(' ');
					l_StrWork1=l_Message.Right(l_Message.GetLength()-l_iEndParm1-1);
					l_Message=l_StrWork1;
					//l_StrParm2=l_Message.SpanExcluding(" ");
					//get User
					l_StrParm2=GetWordNum(0,l_Message);
					//Skip to the reason
					l_iEndParm1=l_Message.Find(' ');
					if(l_iEndParm1!=-1)
					{
						l_StrReason.Format(":%s",l_Message.Right(l_Message.GetLength()-l_iEndParm1-1));
					}
					else
					{
						l_StrReason.Empty();
					}
					CString l_NewMessage;
					l_NewMessage.Format("kick %s %s %s\n",l_StrParm1,l_StrParm2,l_StrReason);
					m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
					return;

				}
				else
				{
					CString l_StrParm1,l_StrReason;
					int l_iEndParm1;
					l_StrParm1=l_Message.SpanExcluding(" ");
					l_iEndParm1=l_Message.Find(' ');
					if(l_iEndParm1!=-1)
					{
						l_StrReason.Format(":%s",l_Message.Right(l_Message.GetLength()-l_iEndParm1));
					}
					else
					{
						l_StrReason.Empty();
					}
					CString l_NewMessage;
					l_NewMessage.Format("kick %s %s %s\n",Window,l_StrParm1,l_StrReason);
					m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
					return;
				}

			}
			//Topic
			if(l_StrCmd.CompareNoCase("topic")==0)
			{
				//Need to parse out nick & message, then add the ':' to the message
				if((!Window) || (l_Message[0]=='#'))
				{
					CString l_StrParm1,l_StrParm2,l_StrReason,l_StrWork1;
					int l_iEndParm1;
					//get the channel
					l_StrParm1=l_Message.SpanExcluding(" ");
					l_iEndParm1=l_Message.Find(' ');
					l_StrWork1=l_Message.Right(l_Message.GetLength()-l_iEndParm1-1);
					l_Message=l_StrWork1;
					
					//Skip to the reason
					//l_iEndParm1=l_Message.Find(' ');
					if(l_iEndParm1!=-1)
					{
						l_StrReason.Format(":%s",l_Message);
					}
					else
					{
						l_StrReason.Empty();
					}
					CString l_NewMessage;
					l_NewMessage.Format("topic %s %s\n",l_StrParm1,l_StrReason);
					m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
					return;

				}
				else
				{
					CString l_NewMessage;
					l_NewMessage.Format("topic %s :%s\n",Window,l_Message);
					m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
					return;
				}

			}
			if(l_StrCmd.CompareNoCase("mode")==0)
			{
				//if(!Window) return;
				CString l_NewMessage;
				l_NewMessage.Format("MODE %s\n",l_Message);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				return;
			}
			
		CString l_Default;
		l_Default.Format("%s %s\n",l_StrCmd,l_Message);
		m_pSocket->Send(LPCSTR(l_Default),l_Default.GetLength());

//		CString l_StrError;
//		l_StrError.LoadString(IDS_UNKNOWN_COMMAND);
//		DispatchMessage(l_StrError,colorSTAT,NULL);
		//MessageBeep(-1);	
		
		}
		
	}





#endif //IRCMODE
//Here is the old UDP stuff
#ifndef IRCMODE	
	
	
//	m_CmdPart; //****NEW****
//	m_CmdMsg;
//	m_CmdWho;
//	m_CmdUsers;
//	m_CmdPing;
//	m_CmdHelp;
//	m_CmdIgnore;
//	m_CmdMotd;
//	m_CmdKill;
//	m_CmdGet;
//	m_CmdOffer;
//	m_CmdCancelGet;
//	m_CmdWhois;
//	m_CmdNoprivate;
//	m_CmdBan;
//	m_CmdUnban;
//	m_CmdBlist;
//	m_CmdDeoffer;
//	m_CmdProgress;
//	m_CmdShowoffers;
//	m_CmdJoin;
//	m_CmdMe;
//	m_CmdAway;
//	m_CmdSubnet;
//	m_CmdPart;


	if(*LPCSTR(strText) == '.' || *LPCSTR(strText) == '/')
	{
		strcpy((PCHAR)localbuff, LPCSTR(strText));
		token = strtok((char*)localbuff, seps);

		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdDeoffer),m_CmdDeoffer.GetLength()))
		{
			CString tempstr;
			CString l_tmpmsgaafb;
			l_tmpmsgaafb.LoadString(IDS_STRAASTFILEB);
			token = strtok(NULL, seps);
			if(!token) return;

			XferFiles.Deoffer(atoi(token));
			tempstr = l_tmpmsgaafb;
			tempstr += token;
			CString l_tmpmsgnfo;
			l_tmpmsgnfo.LoadString(IDS_STRNOFILEOFF);
			tempstr += _T("] ");
			tempstr +=l_tmpmsgnfo;
			DispatchMessage(tempstr,colorSTAT);
			return;

		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdCancelGet),m_CmdCancelGet.GetLength()))
		{
			XferFiles.CancelGet();
			return;
		}
		
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdOffer),m_CmdOffer.GetLength()))
		{
			int OfferAsNum;
			CString tempstr;
			//char num[5];
			token +=m_CmdOffer.GetLength()+2;
			//token = strtok(NULL, seps);
			//if(!token) return;

			OfferAsNum=XferFiles.OfferFile(token);
			if(OfferAsNum==0) 
			{
				CString l_tmpmsgf,l_tmpmsgnf;
				l_tmpmsgf.LoadString(IDS_STRASTFILE);
				l_tmpmsgnf.LoadString(IDS_STRFILENF);
				tempstr = l_tmpmsgf;
				tempstr += token;
				tempstr += l_tmpmsgnf;
				DispatchMessage(tempstr,colorSTAT);
				//return;
			}
			if(OfferAsNum==-1) 
			{
				tempstr.LoadString(IDS_STRNOFILEOFFSLT);
				DispatchMessage(tempstr,colorSTAT);
				return;
			}
			if(Window)
			{
				if(Window[0]!='#')
				{
					CUser *u;
					CString TmpStr;
					if(Users.Lookup(LPCSTR(Window),(class CObject*&)u))
					{
						XferFiles.ShowOffers(u->GetUserIP());
						return;
					}
					else
					{
						TmpStr.LoadString(IDS_STRNOUSER);
						DispatchMessage(TmpStr,colorSTAT,Window);
						return;
					}

				}
			}
			if(Window) if(Window[0]=='#')
			{
				XferFiles.ShowOffers(Window);// Add Channel (Window)
			}
			else
			{
				if(Window)
				{
					CUser *u;
					if(Users.Lookup(Window, (class CObject*&)u))
					XferFiles.ShowOffers((long)u->GetUserIP());
				}

			}
			
			return;
			

		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdProgress),m_CmdProgress.GetLength()))
		{
			XferFiles.Progress();
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdShowoffers),m_CmdShowoffers.GetLength()))
		{
			if(Window) if(Window[0]=='#')
			{
				XferFiles.ShowOffers(Window);// Add Channel (Window)
			}
			else
			{
				if(Window)
				{
					CUser *u;
					if(Users.Lookup(Window, (class CObject*&)u))
					XferFiles.ShowOffers((long)u->GetUserIP());
				}

			}
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdGet),m_CmdGet.GetLength()))
		{
			CString GetFrom,GetNum,GetFileName,TmpStr;
			CUser *u;
			
			if(LPCSTR(strText)[2+m_CmdGet.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			
			if(!token) return;
			GetFrom=token;
			token = strtok(NULL, seps);
			if(!token) 
			{
				//print message that you must specify a offer number
				TmpStr.LoadString(IDS_STRSPECFILENAME);
				DispatchMessage(TmpStr,colorSTAT);
				return;
			}
			GetNum=token;
			token = strtok(NULL, seps);
			if(!token) 
			{
				//print message that the file name must be specified
				TmpStr.LoadString(IDS_STRSPECSAVENAME);
				DispatchMessage(TmpStr,colorSTAT);
				return;
			}
			GetFileName=token;

			//void CFileXfer::GetFile(char *User,int Offernum,char *FileName,long IP)
			if(Users.Lookup(LPCSTR(GetFrom), (class CObject*&)u))
			{
				XferFiles.GetFile((char *)LPCSTR(GetFrom),atoi(LPCSTR(GetNum)),(char *)LPCSTR(GetFileName),u->GetUserIP());
				return;
			}
			else
			{
				//print message that the file name must be specified
				TmpStr.LoadString(IDS_STRNOUSER);
				DispatchMessage(TmpStr,colorSTAT);
				return;
			}
			
			//display a message that the file is being requested.
			CString l_tmpmsggb,l_tmpmsgibr;
			l_tmpmsggb.LoadString(IDS_STRASTFILEBRK);
			l_tmpmsgibr.LoadString(IDS_STRFILERQFROM);
			TmpStr = l_tmpmsggb;
			TmpStr += GetNum;
			TmpStr += l_tmpmsgibr;
			TmpStr += GetFrom + _T(".");
			DispatchMessage(TmpStr,colorSTAT);
			return;

		}

		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdMe),m_CmdMe.GetLength()))
		{
			
			if(strcmp(LPCSTR(strText)+1+m_CmdMe.GetLength(),"")==0) return;
			CString tempstr,tmpusername,tmpmsg;
			tempstr = _T("* ");
			tempstr += m_strHandle;
			tempstr += _T(" ");
			tempstr += LPCSTR(strText)+2+m_CmdMe.GetLength();

			
			
			DispatchMessage(tempstr,colorACT,Window);
			
			
			pchatpkthdr->type = HIGH_TEXT;
			
			pchatpkthdr->ipaddress = 0;
			strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
			strcpy(pchatpkthdr->channel,Window);
			strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(strText)+2+m_CmdMe.GetLength());
			pchatpktdata->framedata.textframe.size = strText.GetLength()-(1+m_CmdMe.GetLength());
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
			

		}
		
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdAway),m_CmdAway.GetLength()))
		{
			CString tempstr;
			if(strcmp(LPCSTR(strText)+1+m_CmdAway.GetLength(),"")!=0) 
			{
				CString l_tmpmsgia;
				l_tmpmsgia.LoadString(IDS_STRISAWAY);
				m_bAway = TRUE;
				tempstr = _T("* ");
				tempstr += m_strHandle;
				tempstr += _T(" ");
				tempstr += l_tmpmsgia;
				tempstr += LPCSTR(strText)+2+m_CmdAway.GetLength();
				tempstr += _T(")");
				m_strAwayMsg = l_tmpmsgia;
				m_strAwayMsg += LPCSTR(strText)+2+m_CmdAway.GetLength();
				m_strAwayMsg += _T(")");
				
			}
			else 
			{
//Scan through all channels, and report the away message
                pos=m_ChannelList.GetHeadPosition();
				while(pos)
				{
                    l_ChannelRsp=m_ChannelList.GetNext(pos);
                    CString l_tmpmsgib;
				    l_tmpmsgib.LoadString(IDS_STRISBACK);
				    m_bAway=FALSE;
				    tempstr = _T("* ");
				    tempstr += m_strHandle;
				    tempstr += _T(" ");
				    tempstr += l_tmpmsgib;
				    DispatchMessage(tempstr,colorACT,LPCSTR(l_ChannelRsp));//add channell
				    tempstr = l_tmpmsgib;
				    pchatpkthdr->type = HIGH_TEXT;
				    pchatpkthdr->ipaddress = 0;
				    strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
				    strcpy(pchatpkthdr->channel,LPCSTR(l_ChannelRsp));
				    strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(tempstr));
				    pchatpktdata->framedata.textframe.size = tempstr.GetLength()+1;
				    m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				    pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
						    0x08A9, "127.0.0.1", 0);
                }
				return;
				
			}
//Scan through all channels, and report the away message			
            pos=m_ChannelList.GetHeadPosition();
			while(pos)
			{
                l_ChannelRsp=m_ChannelList.GetNext(pos);
                DispatchMessage(tempstr,colorACT,LPCSTR(l_ChannelRsp));//add channell
			    pchatpkthdr->type = HIGH_TEXT;
			    pchatpkthdr->ipaddress = 0;
			    strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
			    strcpy(pchatpkthdr->channel,LPCSTR(l_ChannelRsp));
			    strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(m_strAwayMsg));
			    pchatpktdata->framedata.textframe.size = m_strAwayMsg.GetLength()+1;
			    m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			    pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					    0x08A9, "127.0.0.1", 0);
            }
			return;
			
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdNoprivate),m_CmdNoprivate.GetLength()))
		{
			CString tempstr;
			if(strcmp(LPCSTR(strText)+1+m_CmdNoprivate.GetLength(),"")!=0) 
			{ 
				CString l_tmpmsgrm;
				l_tmpmsgrm.LoadString(IDS_STRREJECTING);
				m_bRejecting = TRUE;
				tempstr = l_tmpmsgrm;
				tempstr += LPCSTR(strText)+2+m_CmdNoprivate.GetLength();
				tempstr += _T(")");
				CString l_tmpmsgir;
				l_tmpmsgir.LoadString(IDS_STRISREJECTING);
				m_strRejectMsg = l_tmpmsgir;
				m_strRejectMsg += LPCSTR(strText)+2+m_CmdNoprivate.GetLength();
				m_strRejectMsg += _T(")");
				
			}
			else 
			{
				m_bRejecting=FALSE;
				tempstr.LoadString(IDS_STRNOTREJECTING);
			}
			DispatchMessage(tempstr,colorSTAT);
			return;
		}
		//m_CmdIgnore
		if(_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdIgnore),m_CmdIgnore.GetLength())==0)
		{
			CUser *u;
			if(LPCSTR(strText)[2+m_CmdIgnore.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			if(Users.Lookup(token, (class CObject*&)u))
			{
				if(!u->IsIgnored())
				{
					CString l_tmpmsgu,l_tmpmsgibi;
					l_tmpmsgu.LoadString(IDS_STRUSER);
					l_tmpmsgibi.LoadString(IDS_STRISBEINGIGN);
					u->IgnoreUser(TRUE);
					sprintf((PCHAR)sendbuff,"%s %s %s",LPCSTR(l_tmpmsgu),token,LPCSTR(l_tmpmsgibi));
					DispatchMessage((PCHAR)sendbuff,colorSTAT); 
				}
				else
				{
					u->IgnoreUser(FALSE);
					CString l_tmpmsgu,l_tmpmsgibi;
					l_tmpmsgu.LoadString(IDS_STRUSER);
					l_tmpmsgibi.LoadString(IDS_STRNOTIGN);
					sprintf((PCHAR)sendbuff,"%s %s %s",LPCSTR(l_tmpmsgu),token,LPCSTR(l_tmpmsgibi));
					DispatchMessage((PCHAR)sendbuff,colorSTAT); 
				}
				return;
			}
			else
			{
				CString l_tmpmsgunp;
				l_tmpmsgunp.LoadString(IDS_STRNOUSER);
				sprintf((PCHAR)sendbuff,"%s",LPCSTR(l_tmpmsgunp));
				DispatchMessage((PCHAR)sendbuff,colorSTAT); 
				return;
			}
		}
		
		//m_CmdMsg
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdMsg),m_CmdMsg.GetLength()))
		{
			CUser *u;
			CString tempstr,tmpusername,tmpmsg;
			if(LPCSTR(strText)[5]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			if(Users.Lookup(token, (class CObject*&)u))
			{
				tmpusername=token;
				if(u->GetUserIP()==0)
				{
					CString l_tmpmsgcst,l_tmpmsguia;
					l_tmpmsgcst.LoadString(IDS_STRCANTSENDTO);
					l_tmpmsguia.LoadString(IDS_STRUNKNOWNIP);
					tempstr.Format("%s %s%s",l_tmpmsgcst,tmpusername,l_tmpmsguia);
					DispatchMessage(tempstr,colorSTAT,tmpusername);
					return;
				}
				
				AddTabUser(LPCSTR(tmpusername));

				token = strtok(NULL, seps);
				if(LPCSTR(strText)[2+m_CmdMsg.GetLength()]=='\"') tmpmsg = (LPCSTR(strText)+(m_CmdMsg.GetLength()+5+tmpusername.GetLength()));
				else tmpmsg = (LPCSTR(strText)+(m_CmdMsg.GetLength()+3+tmpusername.GetLength()));
				tempstr = _T("-") + tmpusername;
				tempstr += _T("- ") + tmpmsg;
				DispatchMessage(tempstr,colorPRIV,tmpusername);
				pchatpkthdr->type = PRIV_MSG;
				pchatpkthdr->ipaddress = u->GetUserIP();
				strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
				strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(tmpmsg));//LPCSTR(strText)+(6+tmpusername.GetLength()));
				pchatpktdata->framedata.textframe.size = strText.GetLength();
				m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
				return;
			}
			else
				{
					CString l_tmpmsgunp;
					l_tmpmsgunp.LoadString(IDS_STRNOUSER);
					sprintf((PCHAR)sendbuff,"%s",LPCSTR(l_tmpmsgunp));
					DispatchMessage((PCHAR)sendbuff,colorSTAT,token); 
					return;
				}
			
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdKill),m_CmdKill.GetLength()))
		{
			S_kill pktkill;
			pktkill.reason[0]=NULL;
			CString tempstr,tmpusername,tmpmsg;
			if(LPCSTR(strText)[2+m_CmdKill.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			tmpusername=token;
			token = strtok(NULL, seps);
			if(LPCSTR(strText)[2+m_CmdKill.GetLength()]=='\"') tmpmsg = (LPCSTR(strText)+(m_CmdKill.GetLength()+5+tmpusername.GetLength()));
			else tmpmsg = (LPCSTR(strText)+(m_CmdKill.GetLength()+3+tmpusername.GetLength()));
			
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pktkill.command=KALI_KILL_USER;
			strcpy(pktkill.nick,tmpusername);
			if(!tmpmsg.IsEmpty()) strncpy(pktkill.reason,tmpmsg,199);
			memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
			pchatpktdata->framedata.textframe.size = tmpmsg.GetLength()+19;
			//msg.LoadString(IDS_QUERYING_SERVER);
			//DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdBan),m_CmdBan.GetLength()))
		{
			S_kill pktkill;
			pktkill.reason[0]=NULL;
			CString tempstr,tmpusername;
			if(LPCSTR(strText)[2+m_CmdBan.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			tmpusername=token;
			
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pktkill.command=KALI_BAN_USER;
			strcpy(pktkill.nick,tmpusername);
			memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
			pchatpktdata->framedata.textframe.size = 18;
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdUnban),m_CmdUnban.GetLength()))
		{
			S_kill pktkill;
			pktkill.reason[0]=NULL;
			CString tempstr,tmpusername;
			//if(LPCSTR(strText)[5]=='\"') token = strtok(NULL, "\"");
			token = strtok(NULL, seps);
			if(!token) return;
			tmpusername=token;
			
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pktkill.command=KALI_BAN_CLEAR;
			strcpy(pktkill.nick,tmpusername);
			memcpy(pchatpktdata->framedata.textframe.text,&pktkill,sizeof(S_kill));
			pchatpktdata->framedata.textframe.size = 18;
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdWhois),m_CmdWhois.GetLength()))
		{
			S_Whois pktwhois;
			
			CString tempstr,tmpusername,tmpmsg;
			if(LPCSTR(strText)[2+m_CmdWhois.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			tmpusername=token;
			token = strtok(NULL, seps);
			if(LPCSTR(strText)[2+m_CmdWhois.GetLength()]=='\"') tmpmsg = (LPCSTR(strText)+(m_CmdWhois.GetLength()+5+tmpusername.GetLength()));
			else tmpmsg = (LPCSTR(strText)+(m_CmdWhois.GetLength()+3+tmpusername.GetLength()));

			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pktwhois.command=KALI_WHOIS_REQ;
			strcpy(pktwhois.nick,tmpusername);
			
			memcpy(pchatpktdata->framedata.textframe.text,&pktwhois,sizeof(S_Whois));
			pchatpktdata->framedata.textframe.size = tmpusername.GetLength()+2;
			msg=_T("** Requesting user information...");
			DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		if(!_strnicmp(LPCSTR(strText)+1,LPCSTR(m_CmdSubnet),m_CmdSubnet.GetLength()))
		{
			S_Subnet Subnetpkt;
			CString tmpsubnet,tmppasswd;

			token = strtok(NULL, seps);
			if(!token) tmpsubnet="";
			else tmpsubnet=token;
			
			token = strtok(NULL, seps);
			if(token) tmppasswd=token;
			else tmppasswd="";
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			Subnetpkt.command=KALI_CHANGESUB;
			strncpy(Subnetpkt.subnet,tmpsubnet,31);
			strncpy(Subnetpkt.password,tmppasswd,16);
			memcpy(pchatpktdata->framedata.textframe.text,&Subnetpkt,sizeof(S_Subnet));
			pchatpktdata->framedata.textframe.size = sizeof(S_Subnet);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
	
		if(!strcmp(LPCSTR(strText)+1,LPCSTR(m_CmdHelp)))
		{
			
			msg.LoadString(IDS_CHAT_HELP2A);
			DispatchMessage(LPCSTR(msg),colorSTAT);
			msg.LoadString(IDS_CHAT_HELP3);
			DispatchMessage(LPCSTR(msg),colorSTAT);
			msg.LoadString(IDS_CHAT_HELP4);
			DispatchMessage(LPCSTR(msg),colorSTAT);
			//IDS_CHAT_HELP4
			return;
		}

		//m_CmdUsers
		if(!stricmp(LPCSTR(strText)+1, LPCSTR(m_CmdUsers)))
		{
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pchatpktdata->framedata.textframe.text[0] = KALI_QUERY_SERVER;
			pchatpktdata->framedata.textframe.size = 1;
			msg.LoadString(IDS_QUERYING_SERVER);
			DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		//Start parsing commands
		if(!stricmp(LPCSTR(strText)+1,"skill"))
		{
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pchatpktdata->framedata.textframe.text[0] = 0x62;
			pchatpktdata->framedata.textframe.text[1] = '1';
			pchatpktdata->framedata.textframe.text[2] = '4';
			pchatpktdata->framedata.textframe.text[3] = '2';
			pchatpktdata->framedata.textframe.text[4] = 0;

			pchatpktdata->framedata.textframe.size = 1;
			msg.LoadString(IDS_QUERYING_SERVER);
			DispatchMessage(msg,colorSTAT);
			m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;				

		}
		//m_CmdWho
		if(!stricmp(LPCSTR(strText)+1, LPCSTR(m_CmdWho)))
		{
			m_WhoTimer=0;			
			pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
			pchatpkthdr->type = USERS_RESPOND;
			pchatpkthdr->ipaddress = psharemem->myip;
			strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
			strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
			msg.LoadString(IDS_QUERYING_CHAT);
			DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
			return;
		}

		if(!stricmp(LPCSTR(strText)+1, LPCSTR(m_CmdMotd)))
		{
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pchatpktdata->framedata.textframe.text[0] = KALI_MOTD_REQ;
			pchatpktdata->framedata.textframe.size = 1;
			msg.LoadString(IDS_REQUESTING_SERVER_ID);
			DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		if(!stricmp(LPCSTR(strText)+1, LPCSTR(m_CmdBlist)))
		{
			pchatpkthdr->type = SERVER_COMMAND;
			pchatpkthdr->ipaddress = 0;
			pchatpktdata->framedata.textframe.text[0] = KALI_BAN_LIST;
			pchatpktdata->framedata.textframe.size = 1;
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			return;
		}
		//m_CmdPart
		if(!strnicmp(LPCSTR(strText)+1, LPCSTR(m_CmdPart),m_CmdPart.GetLength()))
		{

            token = strtok(NULL, seps);
			CString l_tmplower;
			//Get the document.....and blow it up....
			l_tmplower = token;
			l_tmplower.MakeLower();
			CPconwDoc *p_tgtDoc;
			
			if(!m_WindowMap.Lookup(l_tmplower,(CObject*&)p_tgtDoc)) return;
			p_tgtDoc->UserPart(m_strHandle);
            p_tgtDoc->OnCloseDocument();
			if(m_ChanList) m_ChanList->ProcessPart(m_strHandle,l_tmplower);	
			return;
		}
		
		
		
		if(!stricmp(token+1,LPCSTR(m_CmdJoin)))
		{
			
			CString tempstr,oldchannel,partmsg,lowerchan;

			if(m_ChannelList.Find(pchatpkthdr->channel))
			{
				//Dork, you are already in that channel!
				msg.LoadString(IDS_CHANNEL_ALREADY);
				msg += m_Channel;
				DispatchMessage(msg,colorSTAT);
				return;

			}
			
            //if(m_ChanList) m_ChanList->ProcessPart(m_strHandle,m_Channel);	
			token = strtok(NULL, seps);
			if(token == NULL)
			{
				msg.LoadString(IDS_CHANNEL_SELECTED);
				msg += m_Channel;
				DispatchMessage(msg,colorSTAT);
				return;
			}
			if(strlen(token) > 7)
			{
				msg.LoadString(IDS_CHANNEL_TOO_BIG);
				DispatchMessage(msg,colorSTAT);
				return;
			}
			if(token[0]!='#') 
			{
				msg.LoadString(IDS_CHANNEL_NEEDS_POUND);
				DispatchMessage(msg,colorSTAT);
				return;
			}
			
			
			/*
            oldchannel = m_Channel;

			CString l_tmpmsglc;
			l_tmpmsglc.LoadString(IDS_STRLVCHAN);
			tempstr = l_tmpmsglc;
			tempstr += oldchannel;
			DispatchMessage(tempstr,colorSTAT);
			//partmsg = _T("is leaving channel ");
			partmsg = oldchannel;
			m_Channel=token;
			pchatpkthdr->type = PART_CHANNEL;
			pchatpkthdr->ipaddress = 0;
			strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
			strcpy(pchatpkthdr->channel, LPCSTR(oldchannel));
			strcpy(pchatpktdata->framedata.textframe.text, LPCSTR(partmsg));
			pchatpktdata->framedata.textframe.size = partmsg.GetLength()+1;
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
			
			
			*/
			
			
			oldchannel = m_Channel;
			m_Channel = token;
			XferFiles.SetChannel(LPCSTR(m_Channel));			
			
			CPconwDoc *l_FirstChanDoc;
			//*****************************************************************
			//*****************************************************************
			CChildFrame *ChannelWindow;
			CPconwDoc *l_NewChanDoc;
			m_CreateNewDocTitle = token;

			l_NewChanDoc=(CPconwDoc *)m_pDocTemplate->CreateNewDocument();
			
			lowerchan=token;
			lowerchan.MakeLower();
			m_ChannelList.AddHead(lowerchan);
			
			m_WindowMap.SetAt(lowerchan,l_NewChanDoc);
			
			ChannelWindow=(CChildFrame *)m_pDocTemplate->CreateNewFrame((CDocument *)l_NewChanDoc,NULL);
			m_pDocTemplate->InitialUpdateFrame((CFrameWnd *)ChannelWindow,l_NewChanDoc,TRUE);
			m_CreateNewDocTitle.Empty();
            lowerchan=token;
			lowerchan.MakeLower();
			if(m_WindowMap.Lookup(lowerchan,(CObject*&)l_Doc))
            {
	            l_Doc->UserJoin(m_strHandle);
            }
			//*****************************************************************
			//*****************************************************************
			
			
			pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
			pchatpkthdr->type = SIGNON;
			pchatpkthdr->ipaddress = 0;
			strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
			strcpy(pchatpkthdr->channel, token);
			msg.LoadString(IDS_SWITCHING_CHANNELS);
			msg += token;
			DispatchMessage(msg,colorSTAT);
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
			
			//((CPconwDoc*)m_ChannelDoc)->SetNewTitle(m_Channel);
			if(m_ChanList) m_ChanList->ProcessJoin(m_strHandle,token);	
			return;
		}

		if(0==stricmp(token+1,LPCSTR(m_CmdPing)))
		{
			if(LPCSTR(strText)[2+m_CmdPing.GetLength()]=='\"') token = strtok(NULL, "\"");
			else token = strtok(NULL, seps);
			if(!token) return;
			CUser *u;
			CString key;
			if(0==strcmp(token, "*"))
			{
				// time to ping everybody
				POSITION pos = Users.GetStartPosition();
				while(pos != NULL)
				{
					Users.GetNextAssoc(pos, key, (class CObject*&)u);
					pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
					pchatpkthdr->type = PING_REQUEST;
					pchatpkthdr->ipaddress = u->GetUserIP();
					strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
					strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
					m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
					u->startclock();
				}
			}
			else
			{
				if(Users.Lookup(token, (class CObject*&)u))
				{
					pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
					pchatpkthdr->type = PING_REQUEST;
					pchatpkthdr->ipaddress = u->GetUserIP();
					strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
					strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
		
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
					u->startclock();
				}
				else
				{
					CString l_tmpmsgunp;
					l_tmpmsgunp.LoadString(IDS_STRNOUSER);
					sprintf((PCHAR)sendbuff,"%s",LPCSTR(l_tmpmsgunp));
					DispatchMessage((PCHAR)sendbuff,colorSTAT); 
				}
			}
			return;
		}

		msg.LoadString(IDS_UNKNOWN_COMMAND);
		DispatchMessage(msg,colorSTAT);
	}
	else
	{
		if(!strText.IsEmpty())
		{
			if(!Window) return;
            CString tempstr;
			tempstr = m_strHandle + _T(": ") + strText;
			DispatchMessage(tempstr,GetColor(tempstr),Window);

			pchatpkthdr->type = NORMAL_TEXT;
			pchatpkthdr->ipaddress = 0;
			strcpy(pchatpkthdr->name, m_strHandle);
			strcpy(pchatpkthdr->channel, Window);

			strcpy(pchatpktdata->framedata.textframe.text, strText);
			pchatpktdata->framedata.textframe.size = strText.GetLength() + 1;
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
		}
	}

#endif //NOT IRCMODE
}

void CPconwApp::AddTabUser(const char *User)
{
	CurrTabUser = FirstTabUser;
	strcpy(CurrTabUser->User,User);

	do
	{
		if(CurrTabUser != FirstTabUser && (strcmp(User,CurrTabUser->User)==0)) return;
		
		if(CurrTabUser->Next==NULL)
		{
			//this one is empty, allocate  & fill in.
			CurrTabUser->Next = new TabUser;
			CurrTabUser = CurrTabUser->Next;
			CurrTabUser->Next = NULL;
			strcpy(CurrTabUser->User,User);
			CurrTabUser = FirstTabUser;
			return;
		}
		CurrTabUser = CurrTabUser->Next;
	}while(1);
}

char * CPconwApp::GetNextTabUser(void)
{
	char *ReturnValue;

	ReturnValue = CurrTabUser->User;
	if(CurrTabUser->Next==NULL) CurrTabUser = FirstTabUser;
	else CurrTabUser = CurrTabUser->Next;
	return ReturnValue;
	
}

DWORD CPconwApp::GetColor (CString &cs_Text)
{
	
	CString cs_Lower;
	cs_Lower = cs_Text;
	cs_Lower.MakeLower();
	
	if(FirstFilter->Next)
	{
		CurrFilter=FirstFilter->Next;
		
		do
		{
			if(cs_Lower.Find(CurrFilter->cs_Word)!=-1)
			{
				return CurrFilter->dw_Color;
			}
			
			CurrFilter=CurrFilter->Next;;
		}while(CurrFilter);		
	}
	return colorNORM;
}

void CPconwApp::GetFilters(void)
{
	SFilters *TmpFilter;
	//typedef struct SFilters {
	//CString cs_Word;
	//DWORD dw_Color;
	//SFilters *Next;
	//}*FirstFilter,*CurrFilter;

	if(FirstFilter->Next)
	{
		CurrFilter=FirstFilter->Next;
		//walk the list, and delete everyone
		do
		{
			TmpFilter=CurrFilter->Next;
			delete CurrFilter;
			CurrFilter=TmpFilter;
		}while(CurrFilter);		
	}
   CurrFilter=FirstFilter;

   CRegistry registry;
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      TRACE( "Can't connect to registry\n" );
      return;
   }

   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			TRACE( "Can't open key\n" );
		    //ReportError( registry.GetErrorCode() );
			return;
		}
				
	  }
	  else
	  {
		TRACE( "Can't open key\n" );
		//ReportError( registry.GetErrorCode() );
		return;
	  }
   }
   
  // string_array.RemoveAll();
   CString tmpstring(""),tmpstring2("");
   registry.QueryInfo();
   int sknum = registry.GetNumberOfValues();
   DWORD tmpdword = NULL;
   int thistype;


   for(int iloop=0;iloop<sknum;iloop++)
   {
	   CurrFilter->Next= new SFilters;
	   CurrFilter=CurrFilter->Next;
	   CurrFilter->Next=NULL;
	   registry.EnumerateValues(iloop,tmpstring,(CRegistry::KeyValueTypes &)thistype,NULL,tmpdword);	
	   //Add to the list....
	   CurrFilter->cs_Word=tmpstring;
	   registry.GetValue(tmpstring,CurrFilter->dw_Color);
   }
   registry.Close();
}

void CPconwApp::OnListUsers() 
{
	CString lusers;
	lusers.LoadString(IDS_STRWHO);
	SendMsg(lusers);
}

void CPconwApp::OnStopscroll() 
{
	if(m_Frozen) m_Frozen=FALSE;
	else m_Frozen=TRUE;	
}

void CPconwApp::OnUpdateStopscroll(CCmdUI* pCmdUI) 
{
	if(m_Frozen) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
	
}

void CPconwApp::OnSetaway() 
{
	CAwayMsg dlgAwayMsg;
	//We are already away
	CPconwDoc *l_Doc;
	CString tempstr,key;
	if(m_bAway) 
	{
		m_bAway=FALSE;
		POSITION pos;
		CString key;
				
		for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
		{
			m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
			if (key[0]=='#')
			{
				//Msg that you aren't away
				CString l_tmpmsgib;
				l_tmpmsgib.LoadString(IDS_STRISBACK);
				tempstr = l_tmpmsgib;	
				//Send a message to each channel open saying that I am not away
				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%cACTION %s%c\n",key,0x01,tempstr,0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("* %s %s",m_strHandle,tempstr);
				DispatchMessage(l_NewMessage,colorACT,key);
				tempstr.Format("away\n");
				m_pSocket->Send(LPCSTR(tempstr),tempstr.GetLength());

			}
		}
	}
	else
	{
		if(dlgAwayMsg.DoModal()==IDCANCEL) return;
		
		m_bAway=TRUE;
		CString l_tmpmsgia;
		POSITION pos;
		CString key;
				
		for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
		{
			m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
			if (key[0]=='#')//l_Doc->IsKindOf(RUNTIME_CLASS(CPconwDoc)))
			{
				l_tmpmsgia.LoadString(IDS_STRISAWAY);
				//tempstr = _T("/me ");
				tempstr = l_tmpmsgia;
				tempstr += dlgAwayMsg.m_AwayMsg;
				tempstr += _T(")");
				//Send a message to each channel open saying that I am away
				CString l_NewMessage;
				l_NewMessage.Format("PRIVMSG %s :%cACTION %s%c\n",key,0x01,tempstr,0x01);
				m_pSocket->Send(LPCSTR(l_NewMessage),l_NewMessage.GetLength());
				l_NewMessage.Format("* %s %s",m_strHandle,tempstr);
				DispatchMessage(l_NewMessage,colorACT,key);
			}
		}
		tempstr.Format("away %s\n",dlgAwayMsg.m_AwayMsg);
		m_pSocket->Send(LPCSTR(tempstr),tempstr.GetLength());
	}


	
}

void CPconwApp::OnUpdateSetaway(CCmdUI* pCmdUI) 
{
	if(m_bAway) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
	
}

void CPconwApp::OnServerInfo() 
{
	CString serverinf;
	serverinf.LoadString(IDS_STRLUSERS);
	SendMsg(serverinf);
}

void CPconwApp::OnViewToolbar() 
{
	// TODO: Add your command handler code here
	
}

void CPconwApp::OnSetpreferences() 
{
	CSetupDlg SetupDialog;
	SetupDialog.DoModal();	
}

void CPconwApp::OpenQueryWindow(LPCSTR Name)
{
	CChatDoc *p_tgtDoc;
	//Window already here, just print
	CString l_lowername;
	l_lowername=Name;
	l_lowername.MakeLower();
	if(m_WindowMap.Lookup(l_lowername,(CObject *&)p_tgtDoc))
	{
		//((CMainFrame*)m_pMainWnd)->MDIActivate()
		//p_tgtDoc->ActivateSendVw(0);
		p_tgtDoc->DisplayMsg("",colorBACK);	
		
	}
	//Create new window, and print it.
	else
	{
		m_CreateNewDocTitle = Name;
		CChildFrame *ChannelWindow;
		p_tgtDoc=(CChatDoc *)m_ChatDocTemplate->CreateNewDocument();

		ChannelWindow=(CChildFrame *)m_ChatDocTemplate->CreateNewFrame((CDocument *)p_tgtDoc,NULL);
		//Just in case
		if(!ChannelWindow) 
		{
			CString l_tmpmsgwerr,l_tmpmsgerr;
			l_tmpmsgwerr.LoadString(IDS_STRNEWWINERR);
			l_tmpmsgerr.LoadString(IDS_STRERROR);
			MessageBox(NULL,l_tmpmsgwerr,l_tmpmsgerr,MB_ICONERROR);
			p_tgtDoc=(CChatDoc *)m_ChannelDoc;
			p_tgtDoc->DisplayMsg("",colorBACK);	
		}
		
		//m_WindowMap.SetAt(l_lowername,p_tgtDoc);
		m_ChatDocTemplate->InitialUpdateFrame((CFrameWnd *)ChannelWindow,p_tgtDoc,TRUE);
		//m_CreateNewDocTitle.Empty();//CDocTemplate//SetActi
		p_tgtDoc->DisplayMsg("",colorBACK);	
		//p_tgtDoc->ActivateSendVw(0);
	}
}

void CPconwApp::SetNewFont(void)
{
	CString strKey;
	POSITION POS;
	CPconwDoc *p_tgtDoc;
	CChatDoc *p_tgtChat;

	((CPconwDoc*)m_ChannelDoc)->SetNewFont();

	for(POS=m_WindowMap.GetStartPosition();POS!=NULL;)
	{
		m_WindowMap.GetNextAssoc( POS, strKey, (CObject*&)p_tgtDoc);
		if(strKey[0]=='#')
		{
			p_tgtDoc->SetNewFont();	
		}
		else
		{
			p_tgtChat=(CChatDoc *)p_tgtDoc;
			p_tgtChat->SetNewFont();
		}
	}

}

//This function has changed since it was created, thus the funky name
void CPconwApp::ActivateMainWnd(void)
{
	CString Name;
	Name = CurrSelUser;
	CPconwDoc *p_tgtDoc;
	//Window already here, activate it.
	CString l_lowerchan;
	l_lowerchan=Name;
	l_lowerchan.MakeLower();
	if(m_WindowMap.Lookup(l_lowerchan,(CObject *&)p_tgtDoc))
	{
		p_tgtDoc->ActivateSendVw(0);			
	}
	//Window not created, go to channel window
	else
	{
		((CChildFrame*)m_MainChannelWnd)->MDIActivate();
		m_MainChannelWnd->BringWindowToTop();
	}
}

void CPconwApp::FileXferIdle(void)
{

//	unsigned int m_iSocketTimeout;
//	BOOL m_bSocketConnected //300
	if(!m_bSocketConnected)
	{
		if(m_iSocketTimeout>300)
		{
			m_iSocketTimeout=0;
			//woop! houston, we have a problem...no connect to the old irc server.
			::MessageBox(m_pMainWnd->m_hWnd,"Unable to connect to the chat server you specified.\nPCon will attempt to connect you to a public (Not controlled by StarGate Networks) chat Server.\n You May want to check the chat server settings under preferences.","Error Connecting to chat Server",MB_ICONSTOP|MB_OK);
			if(m_pSocket)
			{
				m_pSocket->ShutDown(2);
				m_pSocket->Close();
				delete m_pSocket;
				m_pSocket = new CChatSocket();
				//Connect to the server
				m_pSocket->Create();
				if(m_pSocket->Connect("irc.dal.net",6667))
				{
					AfxMessageBox("Connect Failed!!!");
				}
				m_iSocketTimeout=0;
			}

		}
		else
		{
			m_iSocketTimeout++;
		}
	}

	if(m_iSplashGone==8) 
	{
	//	m_SplashWnd.DestroyWindow();
		pSplashThread->HideSplash();    //Bring down the splash screen
	//	m_pMainWnd->SetForegroundWindow();
		
		::BringWindowToTop(m_pMainWnd->m_hWnd);
		::SetForegroundWindow(m_pMainWnd->m_hWnd);
		::ShowWindow(m_pMainWnd->m_hWnd, SW_RESTORE);

//		m_ChanList->Init();
		m_iSplashGone++;
	}
	else
	{
		if(m_iSplashGone<=8)
		{
			m_iSplashGone++;
		}
	}
//	if(m_ChanRefresh>=10) 
//	{
//		if(m_ChanList) 
        {
     //       m_ChanList->Refresh();
        }
//		m_ChanRefresh=0;
//	}
	//else m_ChanRefresh++;
	
	if(m_WhoTimer<=WHOTIMEOUT)
	{
		m_WhoTimer++;
	}
/*

	if(m_ChanRefresh>=200)  //4*300ms
	{
		m_ChanRefresh=0;
		
		POSITION pos;
		CUserPing *l_UserPing;
		CString l_key;
		pos = m_UserPings.GetStartPosition();
		while (pos)
		{
			m_UserPings.GetNextAssoc(pos,l_key,(class CObject*&)l_UserPing);
			//Look for someone who hasn't been pinged yet (NumPings==0)
			if(l_UserPing->State==UP_NOACTION)
			{
				
				l_UserPing->State=UP_SENTPING;
				//Send the ping request...
				CString l_spingstring;//lame name  ;)
				l_spingstring.Format("/tping %s",l_key);
				SendMsg(l_spingstring,NULL);
				return;
			}
			//If everyone is pinged, start over, ping #1 and mark #2 needing to be pinged... (State)
			
		}
	}
	else m_ChanRefresh++;

*/
#ifndef	IRCMODE
	XferFiles.EnterIdle();
#endif

}


//When I add multi channel support, pass the channel to jump to.
void CPconwApp::ActivateChannel(void)
{
	((CChildFrame*)m_MainChannelWnd)->MDIActivate();
	m_MainChannelWnd->BringWindowToTop();
}

void CPconwApp::OnJoinsel() 
{
	m_ChanList->JoinSelected();	
}

void CPconwApp::AddDraggedLaunchItems(CStringList *Files)
{

	//*********NOT USED*******************
	//Parse the files....check for exe,bat,com,or (??lnk)
	//find an empty slot
	//put info in m_LaunchItems[x]
	//Tell the launchpad to add an icon.


}

void CPconwApp::OnLaunchCfg() 
{
	if(m_LaunchCfgDlg->DoModal()!=IDCANCEL) m_LaunchCfgDlg->WriteRegistry();	
	else m_LaunchCfgDlg->ReadRegistry();
	((CMainFrame *)m_pMainWnd)->RefreshLaunchBar();
}

void CPconwApp::DraggedLaunchItem(CString &DragFile)
{
	m_LaunchCfgDlg->AddDraggedFile(DragFile);
	((CMainFrame *)m_pMainWnd)->RefreshLaunchBar();
}


void CPconwApp::LaunchItem(int ItemNum)
{
	if(m_LaunchItems[ItemNum].ProgName.GetLength())
	{
		CString l_FilePath,l_WinExec,l_Progmsg,l_tmpmsgsga,l_tmpmsgsgb,l_tmpmsglt;
		l_tmpmsglt.LoadString(IDS_STRLQT);
		l_tmpmsgsga.LoadString(IDS_STRLNCHQST);
		l_tmpmsgsgb.LoadString(IDS_STRQUESTIONMK);
		l_Progmsg.Format("%s %s %s",l_tmpmsgsga,m_LaunchItems[ItemNum].ProgName,l_tmpmsgsgb);

		if(m_ibConfirmLaunch)
		{
			if(MessageBox(m_pMainWnd->m_hWnd,LPCSTR(l_Progmsg),LPCSTR(l_tmpmsglt),MB_YESNO)==IDYES)
			{
				//l_WinExec.Format("back2chat.exe %s\\%s %s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName,m_LaunchItems[ItemNum].FileParms);
				//l_FilePath.Format("%s\\%s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName);
				//spawn the program
				//CreateProcess((char *)LPCSTR(l_FilePath),(char *)LPCSTR(m_LaunchItems[ItemNum].FileParms),NULL,NULL,FALSE,DETACHED_PROCESS,NULL,LPCSTR(m_LaunchItems[ItemNum].FilePath),NULL,NULL);
				
				if(m_ibCloseChat)
				{
					//l_WinExec.Format("back2chat.exe %s\\%s %s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName,m_LaunchItems[ItemNum].FileParms);
					//WinExec(LPCSTR(l_WinExec),SW_SHOW);
                    WinExec("back2cht.exe",SW_SHOW);
					ShellExecute(NULL,"open",LPCSTR(m_LaunchItems[ItemNum].FileName),LPCSTR(m_LaunchItems[ItemNum].FileParms),LPCSTR(m_LaunchItems[ItemNum].FilePath),SW_SHOWNORMAL);
                    //*****************************************
                    //Load Back2chat with parms to reload pconw
                    //*****************************************
                    
                    PostQuitMessage(0);
				}
				else
				{
					//l_WinExec.Format("%s\\%s %s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName,m_LaunchItems[ItemNum].FileParms);
					//WinExec(LPCSTR(l_WinExec),SW_SHOW);
                    ShellExecute(NULL,"open",LPCSTR(m_LaunchItems[ItemNum].FileName),LPCSTR(m_LaunchItems[ItemNum].FileParms),LPCSTR(m_LaunchItems[ItemNum].FilePath),SW_SHOWNORMAL);
				}
			}
		}
		else
		{
			if(m_ibCloseChat)
			{
				//l_WinExec.Format("back2chat.exe %s\\%s %s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName,m_LaunchItems[ItemNum].FileParms);
				//WinExec(LPCSTR(l_WinExec),SW_SHOW);
                WinExec("back2cht.exe",SW_SHOW);
				ShellExecute(NULL,"open",LPCSTR(m_LaunchItems[ItemNum].FileName),LPCSTR(m_LaunchItems[ItemNum].FileParms),LPCSTR(m_LaunchItems[ItemNum].FilePath),SW_SHOWNORMAL);
				//*****************************************
                //Load Back2chat with parms to reload pconw
                //*****************************************
                PostQuitMessage(0);
			}
			else
			{
				//l_WinExec.Format("%s\\%s %s",m_LaunchItems[ItemNum].FilePath,m_LaunchItems[ItemNum].FileName,m_LaunchItems[ItemNum].FileParms);
				//WinExec(LPCSTR(l_WinExec),SW_SHOW);
                ShellExecute(NULL,"open",LPCSTR(m_LaunchItems[ItemNum].FileName),LPCSTR(m_LaunchItems[ItemNum].FileParms),LPCSTR(m_LaunchItems[ItemNum].FilePath),SW_SHOWNORMAL);
			}

		}
		/*
		char savdir[256];
		_getcwd(savdir,255);
		_chdir(LPCSTR(m_LaunchItems[ItemNum].FilePath));
		
		//_spawnl(_P_NOWAIT,LPCSTR(l_FilePath),LPCSTR(m_LaunchItems[ItemNum].FileParms),NULL);
		_chdir(savdir);
		*/
		
		//later, spawn small launcher app, to unload chat & reload.
	}
}

void CPconwApp::LoadCommandStrings (void)
{

}

BOOL CPconwApp::CheckAddUser(LPCSTR User,long IP)
{
	//When I use this function, the app crashes hard...I don't know why....
	
	CUser *u;
	BOOL l_rcode;

	l_rcode=Users.Lookup(User, (class CObject*&)u);
	
	if(!l_rcode)
	{
//		((CMainFrame*)m_pMainWnd)->dlgUsers.AddItem((char *)User);
		CString su = User;
		u = new CUser(su);
		u->SetUserIP(IP);
		Users.SetAt(User, u);
	}

	return l_rcode;
}

CPconwApp::~CPconwApp()
{

}

void CPconwApp::ParseKahnData(LPCSTR Line)
{

	//m_InParseData=TRUE;
	CPconwDoc *l_Doc;	
	CString l_StrPrefix;
	CString l_StrMessage;
	CString l_StrCommand;
	CString l_StrParm;
	CString l_StrWork,l_StrTmpa,l_StrNick,l_StrCTCPCmd;
	
	
	BYTE sendbuff[1500];
	BYTE localbuff[500];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));

	l_StrMessage=Line;
	if(m_bClosing) return;
	if(l_StrMessage.IsEmpty()) return;
	if(m_bSpam)
	{
		if(m_strSpam.Compare(l_StrMessage)==0)
		{
			return;
		}
	}
	m_strSpam=l_StrMessage;
	//Check Message for prefix
	if(l_StrMessage[0]==':')
	{
		int l_iEndPrefix;
		l_iEndPrefix=l_StrMessage.Find(' ');
		if(l_iEndPrefix==-1)
		{
			//Error, no space after prefix
			AfxMessageBox("Unable to parse line.");
			DispatchMessage(l_StrMessage,colorSTAT,NULL);
			return;
		}
		l_StrPrefix=l_StrMessage.Mid(1,l_iEndPrefix-1);
		l_StrWork=l_StrMessage.Mid(l_iEndPrefix+1,l_StrMessage.GetLength()-l_iEndPrefix);
		l_StrMessage=l_StrWork;
		l_StrTmpa=l_StrPrefix.SpanExcluding("!");
		l_StrNick=l_StrTmpa.Left(l_StrTmpa.GetLength());
	}
	int l_iEndCommand;
	l_iEndCommand = l_StrMessage.Find(' ');
	l_StrCommand=l_StrMessage.Left(l_iEndCommand);
	l_StrWork=l_StrMessage.Mid(l_iEndCommand+1,l_StrMessage.GetLength()-l_iEndCommand);
	l_StrMessage=l_StrWork;
	
	if(l_StrCommand.CompareNoCase("part")==0)
	{
		//Get the channel
		if(l_StrMessage[0]==':')
		{
			l_StrWork=l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage=l_StrWork;
		}	
		
		//remove all the users.....
		CString l_lowerchanF;
		l_lowerchanF=l_StrMessage;
		//l_lowerchanF.MakeLower();
		if((m_WindowMap.Lookup(l_lowerchanF,(CObject*&)l_Doc))&&(l_StrNick!=m_strHandle))
		{
			CString l_StrChannel;
			l_StrChannel=l_StrMessage;
			l_Doc->UserPart(l_StrNick);
			//add message about the join
			if(m_bShowJoin)
			{
				l_StrMessage.Format("%s has left channel %s.",l_StrNick,l_StrMessage);
				DispatchMessage(l_StrMessage,colorSTAT,l_StrChannel);
			}
		}
		
		//
		
		//Check to see if the user is left in any channel, remove him if not.
		POSITION pos;
		CString key;
		BOOL l_bUserInChan,l_bStillAround;
		l_bStillAround = FALSE;
		CUserPing *l_UserPing;
		
		for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
		{
			m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
			if (key[0]=='#')
			{
				l_bUserInChan=l_Doc->IsUserInChannel(l_StrNick);
				if(l_bUserInChan) 
				{
					l_bStillAround=TRUE;
				}
			}
			
		}
		if(l_bStillAround)
		{
			if(m_UserPings.Lookup(l_StrNick,(class CObject*&)l_UserPing))
			{
				delete l_UserPing;
				m_UserPings.RemoveKey(l_StrNick);
			}
		}
		
		return;
	}
	if(l_StrCommand.CompareNoCase("join")==0)
	{
		
		//Get the channel
		if(l_StrMessage[0]==':')
		{
			l_StrWork=l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage=l_StrWork;
		}

		//If it is a new channel....open the window....
		CString l_lowerchanG;
		l_lowerchanG=l_StrMessage;
		l_lowerchanG.MakeLower();
		if(!m_WindowMap.Lookup(l_lowerchanG,(CObject*&)l_Doc))
		{
			CPconwDoc *l_FirstChanDoc;
			//*****************************************************************
			//*****************************************************************
			CChildFrame *ChannelWindow;
			CPconwDoc *l_NewChanDoc;
			m_CreateNewDocTitle = l_StrMessage;

			l_NewChanDoc=(CPconwDoc *)m_pDocTemplate->CreateNewDocument();
			
			m_ChannelList.AddHead(l_StrMessage);
			
			//l_StrMessage.MakeLower();
			//m_WindowMap.SetAt(l_StrMessage,l_NewChanDoc);
			
			ChannelWindow=(CChildFrame *)m_pDocTemplate->CreateNewFrame((CDocument *)l_NewChanDoc,NULL);
			m_pDocTemplate->InitialUpdateFrame((CFrameWnd *)ChannelWindow,l_NewChanDoc,TRUE);
			m_CreateNewDocTitle.Empty();
		}
				
		//Add all the users.....
		CString l_lowerchanH;
		l_lowerchanH=l_StrMessage;
		l_lowerchanH.MakeLower();
		CUserPing *l_UserPing;
		
		if(!m_UserPings.Lookup(l_StrNick,(class CObject*&)l_UserPing))
		{
			l_UserPing = new CUserPing(TRUE);
			m_UserPings.SetAt(l_StrNick,l_UserPing);
		}
		
		if(m_WindowMap.Lookup(l_lowerchanH,(CObject*&)l_Doc))
		{
			CString l_StrChannel;
			l_StrChannel=l_StrMessage;
			if(m_strHandle!=l_StrNick)
			{
				l_Doc->UserJoin(l_StrNick);
			}
			//add message about the join
			if(m_bShowJoin)
			{
				l_StrMessage.Format("%s has joined channel %s.",l_StrNick,l_StrMessage);
				DispatchMessage(l_StrMessage,colorSTAT,l_StrChannel);
			}
			
		}
		return;
	}
	
	
	if(l_StrCommand.CompareNoCase("privmsg")==0)
	{
		CString l_StrDestination;
		int l_iEndDestination;
		l_iEndDestination=l_StrMessage.Find(' ');
		l_StrDestination=l_StrMessage.Left(l_iEndDestination);
		l_StrWork=l_StrMessage.Mid(l_iEndDestination+1,l_StrMessage.GetLength()-l_iEndDestination);
		l_StrMessage=l_StrWork;
		POSITION posignore;
		//Lookup to see if this user is
		//in the Ignore list (m_IgnoreList)
		posignore=m_IgnoreList.Find(l_StrNick);
		if(posignore!=NULL)
		{
			return;
		}		
		//Get rid of the ':'
		if(l_StrMessage[0]==':')
		{
			l_StrWork=l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage=l_StrWork;
		}
		
		if(l_StrMessage[0]==0x01)
		{
			//This is a ctcp message
			//Strip the 0x01 off of the end.
			l_StrWork=l_StrMessage.Mid(1,l_StrMessage.GetLength()-2);
			l_StrMessage=l_StrWork;
			int l_iEndCTCPCmd;
			l_iEndCTCPCmd=l_StrMessage.Find(' ');
			if(l_iEndCTCPCmd!=-1)
			{
				l_StrCTCPCmd=l_StrMessage.Left(l_iEndCTCPCmd);
				l_StrWork=l_StrMessage.Mid(l_iEndCTCPCmd+1,l_StrMessage.GetLength()-l_iEndCTCPCmd);
			}
			else
			{
				l_StrCTCPCmd=l_StrMessage.Left(l_StrMessage.GetLength());
				l_StrMessage.Empty();
			}

			l_StrMessage=l_StrWork;
			if(l_StrCTCPCmd.Compare("FILEREQ")==0)
			{
				int i;
				CString l_FileReq;
				for(i=0;i<MAXOFFERED;i++)
				{
					CString l_StripFileName;
					int l_iLastSlash;
					l_iLastSlash=m_OfferedFiles[i].ReverseFind('\\');
					if(l_iLastSlash!=-1)
					{
						l_StripFileName = m_OfferedFiles[i].Right(m_OfferedFiles[i].GetLength()-(l_iLastSlash+1));
					}
					else
					{
						l_StripFileName = m_OfferedFiles[i];
					}
					if(l_StripFileName.CompareNoCase(l_StrMessage)==0)
					{
						l_FileReq.Format("/dcc send %s %s",l_StrNick,m_OfferedFiles[i]);
						SendMsg(l_FileReq,NULL);
						return;
					}
				}
				return;
			}
			if(l_StrCTCPCmd.Compare("ACTION")==0)
			{
				CString l_StrAction;
				l_StrAction.Format("* %s %s",l_StrNick,l_StrMessage);
				if(l_StrDestination[0]=='#')
				{
					DispatchMessage(l_StrAction,colorACT,l_StrDestination);
				}
				else
				{
					DispatchMessage(l_StrAction,colorACT,l_StrNick);
				}
				return;
			}
			if(l_StrCTCPCmd.CompareNoCase("DCC")==0)
			{
				//get the sub command
				CString l_DCCCmd;
				 l_DCCCmd = GetWordNum(0,l_StrMessage);
				if(l_DCCCmd.CompareNoCase("CHAT")==0)
				{
					//Prompt the user to see if they want to chat....
					CString l_chatmsg;
					l_chatmsg.Format("%s is requesting a DCC chat. Do you want to accept?",l_StrNick);

					if(MessageBox(AfxGetMainWnd()->m_hWnd,LPCSTR(l_chatmsg),"DCC Chat",MB_YESNO)==IDYES)
					{
						
						CDCCCDoc *l_DCCCDoc;
						CDCCCFrm *l_DCCCFrame;
						m_CreateNewDocTitle=l_StrNick;
						//convert the ip and socket to integers
						l_DCCCDoc=(CDCCCDoc *)m_DCCCDocTemplate->CreateNewDocument();
						l_DCCCFrame=(CDCCCFrm *)m_DCCCDocTemplate->CreateNewFrame((CDocument *)l_DCCCDoc,NULL);
						m_DCCCDocTemplate->InitialUpdateFrame((CFrameWnd *)l_DCCCFrame,l_DCCCDoc,TRUE);
						//Set the options here....
						//get the view
						POSITION pos;
						pos = l_DCCCDoc->GetFirstViewPosition();
						CDCCCView* pFirstView = (CDCCCView*)l_DCCCDoc->GetNextView( pos );
						
						m_CreateNewDocTitle.Empty();
						CString l_strip,l_strsocket;
						UINT l_ip;
						short l_socket;
						l_strip=GetWordNum(2,l_StrMessage);
						l_ip=atol(LPCSTR(l_strip));
						l_strsocket=GetWordNum(3,l_StrMessage);
						l_socket=atoi(LPCSTR(l_strsocket));
						
						pFirstView->ConnectDCCChat(l_ip,l_socket);
						
					}
				}
				if(l_DCCCmd.CompareNoCase("send")==0)
				{
					//Prompt the user to see if they want to chat....
					CString l_chatmsg,l_SendFile,l_Size;
					l_SendFile=GetWordNum(1,l_StrMessage);
					l_Size=GetWordNum(4,l_StrMessage);
					unsigned int l_iSize=atoi(LPCSTR(l_Size));
					l_chatmsg.Format("%s is sending you a file called %s. Do you want to receive it?",l_StrNick,l_SendFile);
					CModalYesNoMessagBox l_MessageBox;
					l_MessageBox.m_MessageTitle = "DCC File";
					l_MessageBox.m_MessageText = l_chatmsg;

					if(l_MessageBox.DoModal()==IDOK)
					{
						CFileDialog *l_OpenFileDlg;
						l_OpenFileDlg = new CFileDialog(FALSE,NULL,LPCSTR(l_SendFile));
						
						if(l_OpenFileDlg->DoModal()==IDOK)
						{
							CDCCFDoc *l_DCCFDoc;
							CDCCFFrm *l_DCCFFrame;
							m_CreateNewDocTitle=l_StrNick;
							//convert the ip and socket to integers
							l_DCCFDoc=(CDCCFDoc *)m_DCCFDocTemplate->CreateNewDocument();
							l_DCCFFrame=(CDCCFFrm *)m_DCCFDocTemplate->CreateNewFrame((CDocument *)l_DCCFDoc,NULL);
							m_DCCFDocTemplate->InitialUpdateFrame((CFrameWnd *)l_DCCFFrame,l_DCCFDoc,TRUE);
							//Set the options here....
							//get the view
							POSITION pos;
							pos = l_DCCFDoc->GetFirstViewPosition();
							CDCCFView* pFirstView = (CDCCFView*)l_DCCFDoc->GetNextView( pos );
							
							m_CreateNewDocTitle.Empty();
							CString l_strip,l_strsocket;
							UINT l_ip;
							short l_socket;
							l_strip=GetWordNum(2,l_StrMessage);
							l_ip=atol(LPCSTR(l_strip));
							l_strsocket=GetWordNum(3,l_StrMessage);
							l_socket=atoi(LPCSTR(l_strsocket));
							
							pFirstView->ConnectDCC(l_ip,l_socket,l_OpenFileDlg->GetPathName(),l_StrNick,l_iSize);
						}
						
					}
				}

				return;
			}
			if(l_StrCTCPCmd.Compare("PING")==0)
			{
				CString l_StrPing;
				
				l_StrPing.Format("NOTICE %s :%cPING %s%c\n",l_StrNick,0x01,l_StrMessage,0x01);
				SendMsg(l_StrPing,NULL);
				m_pSocket->Send(LPCSTR(l_StrPing),l_StrPing.GetLength());
				//DispatchMessage(l_StrPing,colorACT,NULL);
				return;
			}
//UDPPING
			if(l_StrCTCPCmd.Compare("UDPPING")==0)
			{
				CString l_StrPing;
				
				l_StrPing.Format("NOTICE %s :%cUDPPING %u%c\n",l_StrNick,0x01,psharemem->myip,0x01);
				SendMsg(l_StrPing,NULL);
				m_pSocket->Send(LPCSTR(l_StrPing),l_StrPing.GetLength());
				//DispatchMessage(l_StrPing,colorACT,NULL);
				return;
			}
			if(l_StrCTCPCmd.Compare("VERSION")==0)
			{
				CString l_StrPing;
				l_StrPing.Format("NOTICE %s :\001VERSION The Players Connection '97. By Kevin Bentley\001\n",l_StrNick);
				SendMsg(l_StrPing,NULL);
				m_pSocket->Send(LPCSTR(l_StrPing),l_StrPing.GetLength());
				//DispatchMessage(l_StrPing,colorACT,NULL);
				return;
			}
			l_StrWork.Format("CTCP %s :%s",l_StrNick,l_StrCTCPCmd);
			l_StrMessage=l_StrWork;
			DispatchMessage(l_StrMessage,colorSTAT,NULL);
			
			return;
		}
		//Channel Message
		if(l_StrDestination[0]=='#')
		{
			l_StrWork=l_StrMessage;//.Right(l_StrMessage.GetLength()-1);
			l_StrMessage.Format("<%s> %s",l_StrNick,l_StrWork);
			DispatchMessage(l_StrMessage,GetColor(l_StrMessage),l_StrDestination);
			return;
		}
		//Private Message
		else
		{
			l_StrWork=l_StrMessage;//.Right(l_StrMessage.GetLength()-1);
			if(m_BeepOnPrivate) MessageBeep(-1);
			l_StrMessage.Format("<%s> %s",l_StrNick,l_StrWork);
			DispatchMessage(l_StrMessage,colorPRIV,l_StrNick);
			return;

		}
		
	}
	if(l_StrCommand.CompareNoCase("notice")==0)
	{
		CString l_StrDestination;
		int l_iEndDestination;
		l_iEndDestination=l_StrMessage.Find(' ');
		l_StrDestination=l_StrMessage.Left(l_iEndDestination);
		l_StrWork=l_StrMessage.Mid(l_iEndDestination+1,l_StrMessage.GetLength()-l_iEndDestination);
		l_StrMessage=l_StrWork;
		POSITION posignore;
		//Lookup to see if this user is
		//in the Ignore list (m_IgnoreList)
		posignore=m_IgnoreList.Find(l_StrNick);
		if(posignore!=NULL)
		{
			return;
		}
		
		//Get rid of the ':'
		if(l_StrMessage[0]==':')
		{
			l_StrWork=l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage=l_StrWork;
		}
		if(l_StrMessage[0]==0x01)
		{
			//This is a ctcp message
			//Strip the 0x01 off of the end.
			l_StrWork=l_StrMessage.Mid(1,l_StrMessage.GetLength()-2);
			l_StrMessage=l_StrWork;
			int l_iEndCTCPCmd;
			l_iEndCTCPCmd=l_StrMessage.Find(' ');
			if(l_iEndCTCPCmd!=-1)
			{
				l_StrCTCPCmd=l_StrMessage.Left(l_iEndCTCPCmd);
				l_StrWork=l_StrMessage.Mid(l_iEndCTCPCmd+1,l_StrMessage.GetLength()-l_iEndCTCPCmd);
			}
			else
			{
				l_StrCTCPCmd=l_StrMessage.Left(l_StrMessage.GetLength());
				l_StrMessage.Empty();
			}

			l_StrMessage=l_StrWork;
			
			if(l_StrCTCPCmd.Compare("PING")==0)
			{
				CString l_StrAction;
				CString l_Time;
				UINT l_iTime,l_iAdjTime;
				l_Time=l_StrMessage;
				l_iTime=atoi(LPCSTR(l_StrMessage));
				l_iAdjTime=GetTickCount()-l_iTime;
				l_StrAction.Format("IRC Ping time from %s is %ums",l_StrNick,l_iAdjTime);
				DispatchMessage(l_StrAction,colorACT,NULL);
				return;
			}
			if(l_StrCTCPCmd.Compare("UDPPING")==0)
			{
				DWORD l_ip;
				l_ip=atoi(LPCSTR(l_StrMessage));
				pchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;			// point to send buffer
				pchatpkthdr->type = PING_REQUEST;
				pchatpkthdr->ipaddress = l_ip;
				strcpy(pchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(pchatpkthdr->channel, LPCSTR(m_Channel));
				m_ControlSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);				
				return;
			}
		}
		//Channel Notice
		if(l_StrDestination[0]=='#')
		{
			l_StrWork=l_StrMessage;
			l_StrMessage.Format("* <%s> %s",l_StrNick,l_StrWork);
			DispatchMessage(l_StrMessage,colorACT,l_StrDestination);
			return;
		}
		//Private Notice
		else
		{
			if(m_bSocketConnected)
			{
				l_StrWork=l_StrMessage;
				l_StrMessage.Format("*<%s> %s",l_StrNick,l_StrWork);
				if(m_BeepOnPrivate) MessageBeep(-1);
				DispatchMessage(l_StrMessage,colorSTAT,NULL);
				return;
			}
			else 
			{
				return;
			}

		}
		
	}
	if(l_StrCommand.CompareNoCase("ping")==0)
	{
		//Respond to server....
		
		//DispatchMessage("* Ping from server.",colorSTAT,NULL);
		l_StrWork.Format("PONG %s\n",l_StrMessage);
		m_pSocket->Send(LPCSTR(l_StrWork),l_StrWork.GetLength());
		//DispatchMessage(l_StrWork,colorSTAT,NULL);
		return;

	}
	if(l_StrCommand.Compare("353")==0)
	{
		int l_ChannelStart,l_iNamesStart;
		int l_iNextName;
		CString l_StrChannel;
		CString l_StrNames;
		CString l_StrDispatchMsg;
		CString l_StrSaveNames;
		CUserPing *l_UserPing;

		l_ChannelStart = l_StrMessage.Find(' ')+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_ChannelStart);
		l_StrMessage = l_StrWork;
		
		l_StrChannel = l_StrMessage.SpanExcluding(" ");
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-(l_StrChannel.GetLength()+1));
		l_StrMessage = l_StrWork;

		l_StrChannel = l_StrMessage.SpanExcluding(" ");
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-(l_StrChannel.GetLength()+1));
		l_StrMessage = l_StrWork;

		if(l_StrMessage[0]==':')
		{
			l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage = l_StrWork;
		}
		l_StrSaveNames = l_StrMessage;
		//l_iNamesStart = l_StrMessage
		//Scan 
		//RPL_NAMREPLY
		//Parse all the names out, and add to the list.
		//Add all the users.....
		l_StrMessage+=" ";
		l_StrChannel.MakeLower();
		while((l_iNextName=l_StrMessage.Find(' '))>0)
		{
			//Get the part up to the space
			l_StrNames = l_StrMessage.Left(l_iNextName);
			
			
			if(!m_UserPings.Lookup(l_StrNames,(class CObject*&)l_UserPing))
			{
				l_UserPing = new CUserPing(TRUE);
				m_UserPings.SetAt(l_StrNames,l_UserPing);
			}

			
			if(m_WindowMap.Lookup(l_StrChannel,(CObject*&)l_Doc))
			{
				l_Doc->UserJoin(l_StrNames);						
			}
			l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iNextName-1);
			l_StrMessage = l_StrWork;

		}
		//Dispatch message!
		l_StrDispatchMsg.Format("%s - %s",l_StrChannel,l_StrSaveNames);
		DispatchMessage(l_StrDispatchMsg,colorSTAT,NULL);
		return;
	}
	if(l_StrCommand.CompareNoCase("QUIT")==0)
	{
		//Someone quit...find out who....
		//Get the channel
		if(l_StrMessage[0]==':')
		{
			l_StrWork=l_StrMessage.Right(l_StrMessage.GetLength()-1);
			l_StrMessage=l_StrWork;
		}	
		l_StrMessage.Format("%s has quit IRC (%s).",l_StrNick,l_StrMessage);
		//remove the user from any channel they might be in....
		POSITION pos;
		CString key;
		BOOL l_bUserInChan;
		
		for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
		{
			m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
			if (key[0]=='#')//l_Doc->IsKindOf(RUNTIME_CLASS(CPconwDoc)))
			{
				l_bUserInChan=l_Doc->UserPart(l_StrNick);
				if(l_bUserInChan) 
					if(m_bShowJoin)
					{
						DispatchMessage(l_StrMessage,colorSTAT,key);
					}
			}
			
		}
		CUserPing *l_UserPing;
		
		if(m_UserPings.Lookup(l_StrNick,(class CObject*&)l_UserPing))
		{
			delete l_UserPing;
			m_UserPings.RemoveKey(l_StrNick);
		}
		
		DispatchMessage(l_StrMessage,colorSTAT,NULL);
		return;

	}
	if(l_StrCommand.CompareNoCase("mode")==0)
	{
		CString l_target,l_Modes;
		int l_iNumModes;
		//get the object of the mode
		l_target=GetWordNum(0,l_StrMessage);
		//get the mode
		l_Modes=GetWordNum(1,l_StrMessage);
		//Get the number of modes
		CString l_ModeReport;
		if(l_Modes.GetLength())
		{
			l_ModeReport.Format("%s set mode %s",l_StrNick,l_StrMessage);
			DispatchMessage(l_ModeReport,colorSTAT,NULL);
		}
		else
		{
			return;
		}
		if((l_target[0]=='#')&&((l_Modes[1]=='o')||(l_Modes[1]=='O')||(l_Modes[1]=='v')||(l_Modes[1]=='V')))
		{
			l_iNumModes=l_Modes.GetLength()-1;
			CString l_modifier;//+ or -
			l_modifier=l_Modes;
			int l_ihack=0; //I am not proud of this.....
			for(int j=0;j<l_iNumModes;j++)
			{
				//Need to account for -o+b user user!user@host
				
				CString l_tmpMode;
				CString l_lowerchan;
				CString l_Usermode;
				if((l_Modes[j+1]=='-')||(l_Modes[j+1]=='+'))
				{
					l_modifier=l_Modes[j+1];
					j++;
					l_ihack++;

				}
				l_tmpMode.Format("%c%c",l_modifier[0],l_Modes[1+j]);
				l_Usermode=GetWordNum((2+j)-l_ihack,l_StrMessage);
				l_lowerchan=l_target;
				l_lowerchan.MakeLower();
				
				if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
				{
					//Set topic
					l_Doc->ChangeUserMode(l_tmpMode,l_Usermode);
				}								
			}
			
		}
		if(l_target[0]=='#')
		{
			CString l_StrWork;
			l_StrWork.Format("MODE %s\n",l_target);
			m_pSocket->Send(LPCSTR(l_StrWork),l_StrWork.GetLength());				
		}
		return;

	}
	if(l_StrCommand.CompareNoCase("kick")==0)
	{
		//get the channel
		CString l_target,l_Reason,l_Kickee;
		l_target=GetWordNum(0,l_StrMessage);
		//get the get the username
		l_Kickee=GetWordNum(1,l_StrMessage);
		//get the reason
		l_Reason=GetWordNum(2,l_StrMessage);
		//Check to see if it is me....
		if(m_strHandle==l_Kickee)
		{
			//Close that document window for the channel I was kicked from.
			CString l_KickNotice;
			l_KickNotice.Format("You have been kicked from %s with the reason %s",l_target,l_Reason);
			AfxMessageBox(l_KickNotice);
			DispatchMessage(l_KickNotice,colorSTAT,NULL);
			CString l_tmplower;
			//Get the document.....and blow it up....
			l_tmplower = l_target;
			l_tmplower.MakeLower();
			CPconwDoc *p_tgtDoc;
			
			if(!m_WindowMap.Lookup(l_tmplower,(CObject*&)p_tgtDoc)) return;
			//p_tgtDoc->UserPart(m_strHandle);
            p_tgtDoc->OnCloseDocument();
			//if(m_ChanList) m_ChanList->ProcessPart(m_strHandle,l_tmplower);				
		}
		else
		{
			CString l_lowerchan;
			l_lowerchan=l_target;
			l_lowerchan.MakeLower();
			if(m_WindowMap.Lookup(l_lowerchan,(CObject*&)l_Doc))
			{
				CString l_StrChannel;
				l_StrChannel=l_target;
				l_Doc->UserPart(l_Kickee);
				//add message about the kick
				l_StrMessage.Format("%s has kicked %s from %s (%s).",l_StrNick,l_Kickee,l_StrChannel,l_Reason);
				DispatchMessage(l_StrMessage,colorSTAT,l_StrChannel);				
			}
		}
		return;

	}
	if(l_StrCommand.CompareNoCase("nick")==0)
	{
		POSITION pos;
		CString key,l_NewNick;
		BOOL l_bUserInChan;
		if(l_StrMessage[0]!=':')
		{
			l_NewNick=l_StrMessage;
		}
		else
		{
			l_NewNick=l_StrMessage.Right(l_StrMessage.GetLength()-1);
		}
		if(l_StrNick==m_strHandle) m_strHandle = l_NewNick;
		l_StrMessage.Format("%s is now known as %s",l_StrNick,l_NewNick);
		for( pos = m_WindowMap.GetStartPosition(); pos != NULL; )
		{
			m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_Doc );
			if (key[0]=='#')
			{
				if(l_Doc->IsUserInChannel(l_StrNick)) DispatchMessage(l_StrMessage,colorSTAT,key);
				if(l_Doc->IsUserInChannel(l_StrNick)) l_Doc->ChangeNick(l_NewNick,l_StrNick);
			}
			
		}
		return;
		
		

	}
	if(l_StrCommand.Compare("332")==0)
	{
		CString l_ChanTopic;
		int l_iEndChannel;
		l_iEndChannel = l_StrMessage.Find(" ") + 1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		l_StrMessage = l_StrWork;
		l_ChanTopic = l_StrMessage.SpanExcluding(" ");
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		if(l_StrWork[0]==':')
		{
			CString l_StrTmpTopic;

			l_StrTmpTopic=l_StrWork.Right(l_StrWork.GetLength()-1);
			l_StrWork = l_StrTmpTopic;

		}
		l_StrMessage.Format("%s - Topic: %s",l_ChanTopic,l_StrWork);
		CString l_lowerchanI;
		CString l_dispatchtitle;
		l_lowerchanI=l_ChanTopic;
		l_lowerchanI.MakeLower();
		if(m_WindowMap.Lookup(l_lowerchanI,(CObject*&)l_Doc))
			{
				//Set topic
				l_Doc->SetNewTitle(l_StrMessage,l_StrWork);						
				//Always inform the user....
				if(l_StrWork.GetLength()) 
				{
					l_dispatchtitle.Format("%s sets the topic to: %s",l_StrNick,l_StrWork);
				}
				DispatchMessage(l_dispatchtitle,colorSTAT,l_ChanTopic);

			}		
		
		return;
	}
	if(l_StrCommand.CompareNoCase("topic")==0)
	{
		CString l_ChanTopic;
		int l_iEndChannel;
		
		l_ChanTopic = l_StrMessage.SpanExcluding(" ");
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		if(l_StrWork[0]==':')
		{
			CString l_StrTmpTopic;

			l_StrTmpTopic=l_StrWork.Right(l_StrWork.GetLength()-1);
			l_StrWork = l_StrTmpTopic;

		}
		l_StrMessage.Format("%s - Topic: %s",l_ChanTopic,l_StrWork);
		CString l_lowerchanJ;
		l_lowerchanJ=l_ChanTopic;
		l_lowerchanJ.MakeLower();
		if(m_WindowMap.Lookup(l_lowerchanJ,(CObject*&)l_Doc))
			{
				//Set topic
				l_Doc->SetNewTitle(l_StrMessage,l_StrWork);						
			}
		CString l_topicmsg;
		l_topicmsg.Format("** %s has changed the topic of channel %s to: %s",l_StrNick,l_ChanTopic,l_StrWork);
		DispatchMessage(l_topicmsg,colorSTAT,l_ChanTopic);
		return;

	}
	if(l_StrCommand.Compare("331")==0)
	{
		CString l_ChanTopic;
		int l_iEndChannel;
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		l_StrMessage = l_StrWork;
		l_ChanTopic = l_StrMessage.SpanExcluding(" ");
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		if(l_ChanTopic[0]==':')
		{
			CString l_StrTmpTopic;

			l_StrTmpTopic=l_ChanTopic.Right(l_ChanTopic.GetLength()-1);
			l_ChanTopic = l_StrTmpTopic;

		}
		l_StrMessage.Format("%s - No Topic",l_ChanTopic);
		//No topic is set, clear it.
		CString l_lowerchanK;
		l_lowerchanK=l_ChanTopic;
		l_lowerchanK.MakeLower();
		if(m_WindowMap.Lookup(l_lowerchanK,(CObject*&)l_Doc))
			{
				//Set topic
				l_Doc->SetNewTitle(l_StrMessage,l_StrWork);						
			}
		return;
	}
	if(l_StrCommand.Compare("372")==0)
	{
		//MOTD
		CString l_MOTD;
		l_MOTD=GetWordNum(1,l_StrMessage);
		DispatchMessage(l_MOTD,colorSTAT,NULL);
		return;		
	}
	//a bunch of stuff to ignore....
	if( (l_StrCommand.Compare("329")==0)||
		(l_StrCommand.Compare("366")==0)||
		(l_StrCommand.Compare("333")==0)||
		(l_StrCommand.Compare("001")==0)||
		(l_StrCommand.Compare("002")==0)||
		(l_StrCommand.Compare("003")==0)||
		(l_StrCommand.Compare("004")==0)||
		(l_StrCommand.Compare("251")==0)||
		(l_StrCommand.Compare("252")==0)||
		(l_StrCommand.Compare("253")==0)||
		(l_StrCommand.Compare("254")==0)||
		(l_StrCommand.Compare("255")==0)||
		(l_StrCommand.Compare("265")==0)||
		(l_StrCommand.Compare("266")==0)||
		(l_StrCommand.Compare("375")==0)||
		(l_StrCommand.Compare("366")==0)
	  )

	{
		return;		
	}
	if(l_StrCommand.Compare("376")==0)
	{
		if(!m_bSocketConnected)
		{
			m_bSocketConnected=TRUE;
			CString l_SendMessage;
			//We are really connected, send the default messages.....
			l_SendMessage.Format("JOIN %s\n",DefaultChannel); 
			m_pSocket->Send(LPCSTR(l_SendMessage),l_SendMessage.GetLength());
			l_SendMessage.Format("MODE %s\n",DefaultChannel); 
			m_pSocket->Send(LPCSTR(l_SendMessage),l_SendMessage.GetLength());
			return;		
		}
	}
	if( (l_StrCommand.Compare("433")==0)||
		(l_StrCommand.Compare("431")==0)||
		(l_StrCommand.Compare("432")==0)
	  )
	{
		//Bad Nickname....
		CDupNickDialog l_BadNick;
		CString l_NewNick;
		l_BadNick.m_OldNick=m_strHandle;
		l_BadNick.DoModal();
		m_strHandle=l_BadNick.m_ChosenNick;
		l_NewNick.Format("NICK %s\n",m_strHandle);
		m_pSocket->Send(LPCSTR(l_NewNick),l_NewNick.GetLength());
		return;		
	}
	if(l_StrCommand.Compare("321")==0)
	{
		//Clear CStringList
		m_slChannelListData.RemoveAll();

		return;
	}
	if(l_StrCommand.Compare("322")==0)
	{
		//Add to CStringArray
		m_slChannelListData.AddTail(l_StrMessage);
		return;
	}
	if(l_StrCommand.Compare("323")==0)
	{
		//Thats all, send the string to the channel list...
		m_DynChannelList.RefreshList(m_slChannelListData);
		m_DynChannelList.ShowWindow(SW_RESTORE);
		return;
	}

	if(l_StrCommand.Compare("324")==0)
	{
		//channel modes....		
		CPconwDoc *l_Doc;	
		CString l_ChanMode;
		CString l_Channel;
		char * l_pstr;
		l_Channel= GetWordNum(1,l_StrMessage);
		l_pstr = GetWordNum(2,l_StrMessage);
		if((l_pstr[0]!=NULL)&&(l_pstr[1]!=NULL)&&l_pstr) l_ChanMode.Format("%s",l_pstr);
		else return;
		if(m_WindowMap.Lookup(l_Channel,(CObject*&)l_Doc))
		{
				//Set topic
				l_Doc->SetChannelMode(l_ChanMode);
		}
		else
		{
			CString l_FormatMode;
			l_FormatMode.Format("Mode: %s %s",l_Channel,l_ChanMode);
			DispatchMessage(l_ChanMode,colorSTAT,NULL);
		}
		return;		
	}
	if(l_StrCommand.Compare("352")==0)
	{
		//who stuff
		int l_iEndChannel;
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		l_StrMessage = "User: ";
		l_StrMessage +=	l_StrWork;
		DispatchMessage(l_StrMessage,colorSTAT,NULL);
		return;		
	}
	if((l_StrCommand.Compare("311")==0)||
	   (l_StrCommand.Compare("313")==0)||
	   (l_StrCommand.Compare("312")==0)||
	   (l_StrCommand.Compare("317")==0)||
	   (l_StrCommand.Compare("318")==0)||
	   (l_StrCommand.Compare("319")==0)||
	   (l_StrCommand.Compare("307")==0)
	  )
	{
		//whois stuff
		int l_iEndChannel;
		l_iEndChannel = l_StrMessage.Find(" ")+1;
		l_StrWork = l_StrMessage.Right(l_StrMessage.GetLength()-l_iEndChannel);
		l_StrMessage = "User Info: ";
		l_StrMessage +=	l_StrWork;
		DispatchMessage(l_StrMessage,colorSTAT,NULL);
		return;		
	}
	

//#ifdef DEBUG
	l_StrWork.Format("%s",Line);
	DispatchMessage(l_StrWork,colorSTAT,NULL);
//#endif


}
/*

CDocument

  */

void CPconwApp::OnPreferencesColors() 
{
	ColorSetup CLRSet;
	CLRSet.DoModal();		
}

void CPconwApp::OnPreferencesFont() 
{
   CFont *l_cf;
   LOGFONT logfnt;
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      return;
   }
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) 
		{
			return;
		}
				
	  }
	  else
	  {
		return;
	  }
   }

   if ( registry.GetValue( "FontName",FontName ) != TRUE )
   {
      FontName = _T("Courier New");
	  registry.SetValue( "FontName",FontName);
   }
   if ( registry.GetValue( "FontSize",FontSize ) != TRUE )
   {
      FontSize = 8;
	  registry.SetValue( "FontSize",FontSize);
   }
   l_cf=ReallyCreateFont(NULL, (char *)LPCSTR(FontName), _T("Regular"), FontSize , 0);
   l_cf->GetLogFont(&logfnt);
   CFontDialog dlg_Font(&logfnt,CF_SCREENFONTS);
   dlg_Font.DoModal();
   FontName=dlg_Font.GetFaceName();
   FontSize=.1*dlg_Font.GetSize();
   registry.SetValue( "FontName", FontName);
   registry.SetValue( "FontSize", FontSize);
   registry.Close();		
   SetNewFont();

	
}

void CPconwApp::OnPreferencesHighlighting() 
{
	CWordFltr WordFilterDlg;
	WordFilterDlg.DoModal();	
}

void CPconwApp::OnPreferencesFunctionkeys() 
{
	CFKeySetup FKEYSet;
	FKEYSet.DoModal();	
}

char * CPconwApp::GetWordNum(int num, CString & l_String)
{
	char seps[20] = " \n\r\t";
	
	char *ptokstr,*token,*strstart;
	static char strreturn[300];
	ptokstr = new char[l_String.GetLength()+2];
	strstart = ptokstr;
	
	ASSERT(ptokstr);
	
	strcpy(ptokstr,LPCSTR(l_String));
	
	token=strtok(ptokstr,seps);

	for(int i=0;i!=num;i++)
	{
		token=strtok(NULL,seps);
	}
	//l_tmpstrB should now have what we want.
	if(token)
	{
		strcpy(strreturn,token);
	}
	else
	{
		return "";
	}
	//check for the ':' char....
	if(token[0]==':')
	{
		//Its not pretty, but it works, return the rest of the string
		strcpy(strreturn,LPCSTR(l_String)+((token-strstart)+1));
	}
	
	delete [] ptokstr;
	//return the appropriate response.
	return strreturn;
}
//This is for server user lists, or server whois, also udp pings in IRCMODE
void CPconwApp::ProccessControlData()
{
	unsigned char buffer[1500];
	CString recvip;
	UINT recvport;
	int bytesreceived;

	bytesreceived = m_ControlSocket->ReceiveFrom( buffer, 1500, recvip, recvport, 0);
	if(bytesreceived != 0)
	{
		ParseControlData((PBYTE)buffer, bytesreceived, recvip, recvport);
	}

}
//This is for server user lists, or server whois, also udp pings in IRCMODE
void CPconwApp::ParseControlData(PBYTE data, int datalen, CString & sendingip, UINT recvport)
{
	POSITION pos;
	DWORD t = GetTickCount();
	unsigned char sendbuff[1800];
	static WORD seq = 0;
	char seps[] = "\n";
	char *token;
	char strIP[20];
	CString msg, msg2;
	CUser *uFrom,*u;
	PCHAT_PACKET_HDR pchatpkthdr, plocalchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata, plocalchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	plocalchatpkthdr = (PCHAT_PACKET_HDR)sendbuff;
	plocalchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	CPconwDoc *l_Doc;

	switch(pchatpkthdr->type)
		{
			case DISPLAY_DATA:
				*data = 0x0a;
				token = strtok((char*)data+1, seps);
				while(token != NULL)
				{
					DispatchMessage(token,colorSTAT);
					token = strtok(NULL, seps);
				}
				break;
			case PING_REQUEST:
			
				if(pchatpkthdr->ipaddress==0) return;
				plocalchatpkthdr->type = PING_RESPONSE;
				strcpy(plocalchatpkthdr->name, LPCSTR(m_strHandle));
				strcpy(plocalchatpkthdr->channel, "      ");
				plocalchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
				//support for DOS's Ping scheme, until I change this one.
				strcpy(plocalchatpktdata->framedata.textframe.text,pchatpktdata->framedata.textframe.text);
				m_ControlSocket->SendTo(plocalchatpkthdr, sizeof(CHAT_PACKET_HDR), 0x08A9, "127.0.0.1", 0);
				break;

			case PING_RESPONSE:
			 	
				CString l_tmpmsgpr;
				DWORD l_pingrsp;
				CString l_StrNick(pchatpkthdr->name);
				l_tmpmsgpr.LoadString(IDS_STRPINGRSP);
				l_pingrsp = *(DWORD *)pchatpkthdr->channel;
				sprintf((PCHAR)sendbuff, "%s %s %ldms",LPCSTR(l_tmpmsgpr), pchatpkthdr->name, *(DWORD *)pchatpkthdr->channel);
				DispatchMessage((PCHAR)sendbuff,colorSTAT);				
				//add their response to the ping list
				CUserPing *l_UserPing;
		
				if(m_UserPings.Lookup(l_StrNick,(class CObject*&)l_UserPing))
				{
					l_UserPing->AddPing(l_pingrsp);
				}
				break;
		}

}

BOOL CPconwApp::AddMDIFrame(CFrameWnd * FrmWnd, CDocument * Doc, CString & Title)
{
	return TRUE;
}


BOOL CPconwApp::RemoveMDIFrame(CString & Title)
{
	return TRUE;	

}

void CPconwApp::OnHomepage() 
{
	ShellExecute(NULL,"open","http://stargatenetworks.com","","",SW_SHOWNORMAL);	
}

void CPconwApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	ShellExecute(NULL,"open","http://stargatenetworks.com/help/pcon97cm.html","","",SW_SHOWNORMAL);

}

void CPconwApp::NewMessage(CString & Title)
{
	CNewMessage * l_NewMessage;
	
	if(m_WindowList.Lookup(Title,(CObject*&)l_NewMessage))
	{
		if(!l_NewMessage->UnRead)
		{
			if(((CMainFrame *)AfxGetMainWnd())->m_TabBar.GetSelTab().CompareNoCase(Title)!=0)
			{
				l_NewMessage->UnRead=TRUE;	
				((CMainFrame *)AfxGetMainWnd())->m_TabBar.RedrawWindow();
			}
		}
	}
	else
	{
		if(((CMainFrame *)AfxGetMainWnd())->m_TabBar.GetSelTab().CompareNoCase(Title)!=0)
			{
				m_WindowList.SetAt(Title,(CObject*&)new CNewMessage(TRUE));
				((CMainFrame *)AfxGetMainWnd())->m_TabBar.RedrawWindow();
			}
		else
		{
			m_WindowList.SetAt(Title,(CObject*&)new CNewMessage(FALSE));
			((CMainFrame *)AfxGetMainWnd())->m_TabBar.RedrawWindow();
		}
	}
}

void CPconwApp::ClearNewMessage(CString & Title)
{
	CNewMessage * l_NewMessage;
	
	if(m_WindowList.Lookup(Title,(CObject*&)l_NewMessage))
	{
		l_NewMessage->UnRead=FALSE;	
	}
	else
	{
		m_WindowList.SetAt(Title,(CObject*&)new CNewMessage(FALSE));
	}
}
