// setupdlg.cpp : implementation file
//

#include "stdafx.h"
#include <wfc.h>


#include "setupdlg.h"
//#include "clredit.h"
#include "ClrStatic.h"
#include "olorsetup.h" //ColorSetup
#include "fkeysetup.h" //FKEY config
#include "util.h"




/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog


CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	m_strChannel = _T("");
	m_strHandle = _T("");
	m_strServer = _T("");
	m_ProfanityFilterEnabled = FALSE;
	m_csQuitMsg = _T("");
	m_bAutoLog = FALSE;
	m_iServerPort = 0;
	m_ServerName = _T("");
	m_BeepOnPrivate = FALSE;
	//}}AFX_DATA_INIT

	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
			//TRACE( "Can't open key\n" );
		    //ReportError( registry.GetErrorCode() );
			return;
		}
				
	  }
	  else
	  {
		//TRACE( "Can't open key\n" );
		//ReportError( registry.GetErrorCode() );
		return;
	  }
   }
	

   
   CString TempString;
	
   if ( registry.GetValue( "ProfanityFilter", TempString ) != TRUE )
   {
      //TRACE( "registry.GetValue failed\n" );
      m_ProfanityFilterEnabled=FALSE;
	  registry.SetValue( "ProfanityFilter", "FALSE" );
   }
   else
   {
      if ( TempString.Compare( "TRUE" ) == 0 )
      {
         m_ProfanityFilterEnabled=TRUE;
      }
      else
      {
         m_ProfanityFilterEnabled=FALSE;
      }
   }  
   
   if ( registry.GetValue( "Channel", m_strChannel ) != TRUE )
   {
      //TRACE( "registry.GetValue failed\n" );
      m_strChannel = _T("#kahn");
	  registry.SetValue( "Channel", m_strChannel );
   }
   
   if ( registry.GetValue( "QuitMsg", m_csQuitMsg ) != TRUE )
   {
      //TRACE( "registry.GetValue failed\n" );
      m_csQuitMsg = _T("Leaving");
	  registry.SetValue( "QuitMsg", m_csQuitMsg );
   }
   if ( registry.GetValue( "PrivateWindows", ((CPconwApp*)AfxGetApp())->m_bSeperatePrivate ) != TRUE )
   {
      	  registry.SetValue( "PrivateWindows", ((CPconwApp*)AfxGetApp())->m_bSeperatePrivate );
   }
   if(((CPconwApp*)AfxGetApp())->m_bSeperatePrivate) m_bPrivate=TRUE;
   else m_bPrivate=FALSE;
   
   if ( registry.GetValue( "SpamFilter", ((CPconwApp*)AfxGetApp())->m_bSpam ) != TRUE )
   {
	  registry.SetValue( "SpamFilter", ((CPconwApp*)AfxGetApp())->m_bSpam );
   }
   if(((CPconwApp*)AfxGetApp())->m_bSpam)m_bSpamFilter=TRUE;
   else m_bSpamFilter=FALSE;
   if ( registry.GetValue( "ShowJoinPart", ((CPconwApp*)AfxGetApp())->m_bShowJoin ) != TRUE )
   {
	  registry.SetValue( "ShowJoinPart", ((CPconwApp*)AfxGetApp())->m_bShowJoin);
   }
   if(((CPconwApp*)AfxGetApp())->m_bShowJoin) m_bShowJoinPart=TRUE;
   else m_bShowJoinPart=FALSE;
   //********************************************************* 
   if ( registry.GetValue( "AutoLog", ((CPconwApp*)AfxGetApp())->m_bAutoLog ) != TRUE )
   {
	  registry.SetValue( "AutoLog",0);
   }
   if(((CPconwApp*)AfxGetApp())->m_bAutoLog) m_bAutoLog=TRUE;
   else m_bAutoLog=FALSE;
   if(((CPconwApp*)AfxGetApp())->m_BeepOnPrivate) m_BeepOnPrivate=TRUE;
   else m_BeepOnPrivate=FALSE;
   //BeepOnPrivate
   //*********************************************************
   registry.Close();	
   

}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_FONTDESC, m_FontDesc);
	DDX_Text(pDX, IDC_CHANNEL, m_strChannel);
	DDV_MaxChars(pDX, m_strChannel, 30);
	DDX_Check(pDX, IDC_ENABLEPROFANITYFILTER, m_ProfanityFilterEnabled);
	DDX_Text(pDX, IDC_QUITMSG, m_csQuitMsg);
	DDV_MaxChars(pDX, m_csQuitMsg, 50);
	DDX_Check(pDX, IDC_AUTOLOG, m_bAutoLog);
	DDX_Text(pDX, IDC_CHATPORT, m_iServerPort);
	DDV_MinMaxUInt(pDX, m_iServerPort, 0, 9999);
	DDX_Text(pDX, IDC_CHATSERVER, m_ServerName);
	DDV_MaxChars(pDX, m_ServerName, 200);
	DDX_Check(pDX, IDC_BEEPONMESSAGE, m_BeepOnPrivate);
	DDX_Check(pDX, IDC_PRIVCHAT, m_bPrivate);
	DDX_Check(pDX, IDC_SPAMFLTR, m_bSpamFilter);
	DDX_Check(pDX, IDC_SHOWJOINPART, m_bShowJoinPart);
	//}}AFX_DATA_MAP
	//DDX_Check(pDX, IDC_SHOWJOINPART, m_bShowJoinPart);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_BN_CLICKED(IDC_SETCOLORS, OnSetcolors)
	ON_BN_CLICKED(IDC_SETFKEYS, OnSetfkeys)
	ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
	ON_BN_CLICKED(IDC_SETLAUNCH, OnSetlaunch)
	ON_BN_CLICKED(IDC_SHOWJOINPART, OnShowjoinpart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

void CSetupDlg::OnSetcolors() 
{
	ColorSetup CLRSet;
	CLRSet.DoModal();	
}

void CSetupDlg::OnSetfkeys() 
{
	CFKeySetup FKEYSet;
	FKEYSet.DoModal();
	
}

void CSetupDlg::OnOK() 
{
	CString l_tmpstr;
	UpdateData(TRUE);

	((CPconwApp*)AfxGetApp())->m_IRCServerName=m_ServerName;
	((CPconwApp*)AfxGetApp())->m_IRCServerPort=m_iServerPort;

    if(m_strChannel[0]!='#') 
    {
        l_tmpstr = _T("#");
        l_tmpstr += m_strChannel;
        m_strChannel=l_tmpstr;
    }
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
			//TRACE( "Can't open key\n" );
		    //ReportError( registry.GetErrorCode() );
			return;
		}
				
	  }
	  else
	  {
		//TRACE( "Can't open key\n" );
		//ReportError( registry.GetErrorCode() );
		return;
	  }
    }
	else
	{
		if(m_bPrivate)((CPconwApp*)AfxGetApp())->m_bSeperatePrivate=1;
		else ((CPconwApp*)AfxGetApp())->m_bSeperatePrivate=0;
		
		if(m_bSpamFilter)((CPconwApp*)AfxGetApp())->m_bSpam=1;
		else ((CPconwApp*)AfxGetApp())->m_bSpam=0;

		if(m_bShowJoinPart)((CPconwApp*)AfxGetApp())->m_bShowJoin=1;
		else ((CPconwApp*)AfxGetApp())->m_bShowJoin=0;
		
        if(m_bAutoLog) ((CPconwApp*)AfxGetApp())->m_bAutoLog=1;
        else ((CPconwApp*)AfxGetApp())->m_bAutoLog=0;

        registry.SetValue( "AutoLog", ((CPconwApp*)AfxGetApp())->m_bAutoLog);
		registry.SetValue( "QuitMsg", m_csQuitMsg );
		registry.SetValue( "Channel", m_strChannel );
		registry.SetValue( "PrivateWindows", ((CPconwApp*)AfxGetApp())->m_bSeperatePrivate );
		if(m_ProfanityFilterEnabled) registry.SetValue( "ProfanityFilter", "TRUE" );
		else registry.SetValue( "ProfanityFilter", "FALSE" );
		registry.SetValue( "ShowJoinPart", ((CPconwApp*)AfxGetApp())->m_bShowJoin);
		registry.SetValue( "SpamFilter", ((CPconwApp*)AfxGetApp())->m_bSpam );
		m_BeepOnPrivate=((CPconwApp*)AfxGetApp())->m_BeepOnPrivate;
		registry.SetValue( "BeepOnPrivate", ((CPconwApp*)AfxGetApp())->m_BeepOnPrivate);
		registry.SetValue( "ChatServer", ((CPconwApp*)AfxGetApp())->m_IRCServerName );
		registry.SetValue( "ChatServerPort", ((CPconwApp*)AfxGetApp())->m_IRCServerPort );
		registry.Close();	
	}
	((CPconwApp*)AfxGetApp())->ProfanityFilter=m_ProfanityFilterEnabled;
	((CPconwApp*)AfxGetApp())->LeaveMsg=m_csQuitMsg;
	//((CChatDoc*)GetDefaults();
	CDialog::OnOK();
}


void CSetupDlg::OnSetfont() 
{
   CFont *l_cf;
   LOGFONT logfnt;
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
			//TRACE( "Can't open key\n" );
		    //ReportError( registry.GetErrorCode() );
			return;
		}
				
	  }
	  else
	  {
		//TRACE( "Can't open key\n" );
		//ReportError( registry.GetErrorCode() );
		return;
	  }
   }

   if ( registry.GetValue( "FontName", ((CPconwApp*)AfxGetApp())->FontName ) != TRUE )
   {
      ((CPconwApp*)AfxGetApp())->FontName = _T("Courier New");
	  registry.SetValue( "FontName", ((CPconwApp*)AfxGetApp())->FontName);
   }
   if ( registry.GetValue( "FontSize", ((CPconwApp*)AfxGetApp())->FontSize ) != TRUE )
   {
      ((CPconwApp*)AfxGetApp())->FontSize = 8;
	  registry.SetValue( "FontSize", ((CPconwApp*)AfxGetApp())->FontSize);
   }
   l_cf=ReallyCreateFont(NULL, (char *)LPCSTR(((CPconwApp*)AfxGetApp())->FontName), _T("Regular"), ((CPconwApp*)AfxGetApp())->FontSize , 0);
   l_cf->GetLogFont(&logfnt);
   CFontDialog dlg_Font(&logfnt,CF_SCREENFONTS);
   dlg_Font.DoModal();
   ((CPconwApp*)AfxGetApp())->FontName=dlg_Font.GetFaceName();
   ((CPconwApp*)AfxGetApp())->FontSize=.1*dlg_Font.GetSize();
   registry.SetValue( "FontName", ((CPconwApp*)AfxGetApp())->FontName);
   registry.SetValue( "FontSize", ((CPconwApp*)AfxGetApp())->FontSize);
   registry.Close();		
   m_CSFontDesc.Format("%s (%d)",((CPconwApp*)AfxGetApp())->FontName,((CPconwApp*)AfxGetApp())->FontSize);
   SetDlgItemText(IDC_FONTDESC,m_CSFontDesc);
   ((CPconwApp*)AfxGetApp())->SetNewFont();
	   //m_bNewFont=TRUE;
}

BOOL CSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CSFontDesc.Format("%s (%d)",((CPconwApp*)AfxGetApp())->FontName,((CPconwApp*)AfxGetApp())->FontSize);
	SetDlgItemText(IDC_FONTDESC,m_CSFontDesc);
	SetDlgItemText(IDC_CHATSERVER,((CPconwApp*)AfxGetApp())->m_IRCServerName);//m_ServerName
	CString l_itoa;//m_IRCServerPort
	l_itoa.Format("%u",((CPconwApp*)AfxGetApp())->m_IRCServerPort);
	SetDlgItemText(IDC_CHATPORT,l_itoa);//m_iServerPort

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDlg::OnSetlaunch() 
{
	// TODO: Add your control notification handler code here
	
}

void CSetupDlg::OnShowjoinpart() 
{
	
}
