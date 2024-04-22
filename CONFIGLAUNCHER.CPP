// ConfigLauncher.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"

#include "AbortScan.h"

#include "ConfigLauncher.h"
#include "LaunchNewTitle.h"
#include "CancelThread.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigLauncher dialog


CConfigLauncher::CConfigLauncher(CWnd* pParent ,LaunchItems *LaunchPtr)
	: CDialog(CConfigLauncher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigLauncher)
	m_FileName = _T("");
	m_FilePath = _T("");
	m_ProgName = _T("");
	m_FileParms = _T("");
	m_bCloseChat = FALSE;
	m_bConfirmLaunch = FALSE;
	//}}AFX_DATA_INIT
	m_Launchees=LaunchPtr;
	SelGame=-1;
	m_iCurrGame=-1;
    m_bFirstRun=FALSE;
    char *token;
    char l_Line[255];
    CStdioFile l_GetGames;
	if(!l_GetGames.Open("games.txt",CFile::modeRead|CFile::shareExclusive)) 
	{
		AfxMessageBox(IDS_STRNOAUTOSCAN,MB_OK|MB_ICONSTOP);
		FirstGame = new ScanGame;
		CurrGame=FirstGame;
		CurrGame->Next=NULL;
		return;
	}


    FirstGame = new ScanGame;
    CurrGame=FirstGame;
    l_GetGames.ReadString(l_Line,254);
    while(l_GetGames.ReadString(l_Line,254))
    {
           token=strtok(l_Line,",");
           if(token) CurrGame->Name=token;
           token=strtok(NULL,",");
           if(token) CurrGame->FindFile=token;
           token=strtok(NULL,",");
           if(token) CurrGame->UseFile=token;
           token=strtok(NULL,",");
           if(token) CurrGame->Parms=token;
           token=strtok(NULL,",");
           if(token) CurrGame->Icon=token;
           CurrGame->Icon.TrimRight();//Get rid of \n
           CurrGame->Next=new ScanGame;
           CurrGame=CurrGame->Next;
           CurrGame->Next=NULL;
    }
    
}


void CConfigLauncher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigLauncher)
	DDX_Control(pDX, IDC_CLOSECHAT, m_CloseButton);
	DDX_Control(pDX, IDC_CONFIRMLNCH, m_ConfirmButton);
	DDX_Control(pDX, IDC_PROGICON, m_GameIcon);
	DDX_Control(pDX, IDC_LIST1, m_Games);
	DDX_Text(pDX, IDC_FILENAME1, m_FileName);
	DDX_Text(pDX, IDC_PROGDIR, m_FilePath);
	DDX_Text(pDX, IDC_PROGNAME, m_ProgName);
	DDX_Text(pDX, IDC_PROGPARMS, m_FileParms);
	DDX_Check(pDX, IDC_CLOSECHAT, m_bCloseChat);
	DDX_Check(pDX, IDC_CONFIRMLNCH, m_bConfirmLaunch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigLauncher, CDialog)
	//{{AFX_MSG_MAP(CConfigLauncher)
	ON_BN_CLICKED(IDC_ITEMADD, OnItemadd)
	ON_BN_CLICKED(IDC_ITEMDEL, OnItemdel)
	ON_BN_CLICKED(IDC_CHOOSEICON, OnChooseicon)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_FILEFIND, OnFilefind)
	ON_EN_KILLFOCUS(IDC_FILENAME1, OnKillfocusFilename1)
	ON_EN_KILLFOCUS(IDC_PROGDIR, OnKillfocusProgdir)
	ON_EN_KILLFOCUS(IDC_PROGNAME, OnKillfocusProgname)
	ON_EN_SETFOCUS(IDC_PROGNAME, OnSetfocusProgname)
	ON_BN_CLICKED(IDC_FINDGAMES, OnFindgames)
	ON_BN_CLICKED(IDC_CONFIRMLNCH, OnConfirmlnch)
	ON_BN_CLICKED(IDC_CLOSECHAT, OnClosechat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigLauncher message handlers

void CConfigLauncher::OnOK() 
{
	int GameNum;
	UpdateData(TRUE);
	//Save the previous settings first.
	if(m_iCurrGame!=-1) 
		for(GameNum=0;GameNum<20;GameNum++)
		{
			if(m_Launchees[GameNum].ProgName.GetLength())
			{
				if((m_Launchees[GameNum].ProgName.Compare(m_ProgName)==0)&&(m_iCurrGame!=GameNum))
				{
					//Duplicate, set focus & give warning
					//IDS_STRDUPTITLE
					//LoadString
					CString l_tmpmsg,l_tmpstrerr;
					l_tmpmsg.LoadString(IDS_STRDUPTITLE);
					l_tmpstrerr.LoadString(IDS_STRERROR);
					MessageBox(l_tmpmsg,l_tmpstrerr,MB_OK);
					CWnd * l_EditBox;
					l_EditBox=GetDlgItem(IDC_PROGNAME);
					if(l_EditBox) l_EditBox->SetFocus();
					return;
				}
			}
		}
	
	if(m_iCurrGame!=-1)
	{
		m_Launchees[m_iCurrGame].ProgName=m_ProgName;
		m_Launchees[m_iCurrGame].FileParms=m_FileParms;
		m_Launchees[m_iCurrGame].FilePath=m_FilePath;
		m_Launchees[m_iCurrGame].FileName=m_FileName;
		m_Launchees[m_iCurrGame].FileIcon=m_IconPath;
		m_iCurrGame=-1;
	}	
	CDialog::OnOK();
}

void CConfigLauncher::OnCancel() 
{
	m_iCurrGame=-1;
	CDialog::OnCancel();
}

void CConfigLauncher::OnItemadd() 
{
	int GameNum;
	
	//search for empty slot
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.IsEmpty())
		{
			m_iCurrGame=GameNum;
			break;
		}
	}
	if(GameNum>=20)
	{
		//No Room
		CString l_tmpstr,l_tmpstrerr;
		l_tmpstr.LoadString(IDS_STRMAXITEM);
		l_tmpstrerr.LoadString(IDS_STRERROR);
		MessageBox(l_tmpstr,l_tmpstrerr,MB_OK);
		return;
	}
	//Get the program title
	CLaunchNewTitle l_GetTitle;

	if(l_GetTitle.DoModal()==IDCANCEL) 
	{
		m_iCurrGame=-1;
		OnSelchangeList1();
		m_Games.SetCurSel(0);
		return;
	}
	
	//Check for dup. title
		for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(l_GetTitle.m_NewTitle)==0)
		{
			//This title is already used
			CString l_tmpstr,l_tmpstrerr;
			l_tmpstr.LoadString(IDS_STRDUPTITLE);
			l_tmpstrerr.LoadString(IDS_STRERROR);
			MessageBox(l_tmpstr,l_tmpstrerr,MB_OK);
//			OnSelchangeList1();
//			m_Games.SetCurSel(0);
			return;
		}
	}
		
	m_ProgName=l_GetTitle.m_NewTitle;
	//add it to the main dialog....
	SetDlgItemText(IDC_PROGNAME,m_ProgName);
	SetDlgItemText(IDC_PROGDIR,"");
	SetDlgItemText(IDC_FILENAME1,"");
	SetDlgItemText(IDC_PROGPARMS,"");
	m_IconPath="";
	m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	
	m_Games.SetCurSel(m_Games.AddString(m_ProgName));

	m_Launchees[m_iCurrGame].ProgName=m_ProgName;
	m_Launchees[m_iCurrGame].FileParms=m_FileParms;
	m_Launchees[m_iCurrGame].FilePath=m_FilePath;
	m_Launchees[m_iCurrGame].FileName=m_FileName;
	m_Launchees[m_iCurrGame].FileIcon=m_IconPath;

}

void CConfigLauncher::OnItemdel() 
{
	int l_iSaveGameNum=m_iCurrGame;
	//Remove Item from List
	m_Games.DeleteString(SelGame);
	//Select another item
	//OnSelchangeList1();
	if(m_iCurrGame!=-1) 
    {
	    m_Launchees[l_iSaveGameNum].ProgName.Empty();
	    m_Launchees[l_iSaveGameNum].FileName.Empty();
	    m_Launchees[l_iSaveGameNum].FilePath.Empty();
	    m_Launchees[l_iSaveGameNum].FileIcon.Empty();
	    m_Launchees[l_iSaveGameNum].FileParms.Empty();
    }
	m_iCurrGame=-1;
	m_Games.SetCurSel(0);
	OnSelchangeList1();
	
}

void CConfigLauncher::OnChooseicon() 
{
	HICON myIcon;
	if(m_iCurrGame==-1) return;
	//char szFilter[] = "Files with Icons(*.exe;*.ico;*.dll) | *.exe; *.dll; *.ico ||";
	CString l_strFilter;
	CString l_IconFile;
	
	l_strFilter.LoadString(IDS_STRFILEFILTER);

	l_IconFile=m_IconPath;
	if(l_IconFile.IsEmpty()) l_IconFile.LoadString(IDS_STRFILEEXT);//="*.exe;*.ico;*.dll";
	CFileDialog l_IconChooser(TRUE,NULL,l_IconFile,OFN_HIDEREADONLY,l_strFilter);
	l_IconChooser.DoModal();
	myIcon = ExtractIcon(AfxGetInstanceHandle(),l_IconChooser.GetPathName(),0);
	m_GameIcon.SetIcon(myIcon);
	m_IconPath=l_IconChooser.GetPathName();	
}


void CConfigLauncher::OnSelchangeList1() 
{
	CString l_SelectedGame;
	HINSTANCE l_thisInst=AfxGetInstanceHandle();
	int GameNum;
	
	UpdateData(TRUE);
	//Save the previous settings first.
	if(m_iCurrGame!=-1)
	{
		
		for(GameNum=0;GameNum<20;GameNum++)
		{
			if(m_Launchees[GameNum].ProgName.GetLength())
			{
				if((m_Launchees[GameNum].ProgName.Compare(m_ProgName)==0)&&(m_iCurrGame!=GameNum))
				{
					//Duplicate, set focus & give warning
					CString l_tmpstr,l_tmpstrerr;
					l_tmpstr.LoadString(IDS_STRDUPTITLE);
					l_tmpstrerr.LoadString(IDS_STRERROR);
					MessageBox(l_tmpstr,l_tmpstrerr,MB_OK);

					CWnd * l_EditBox;
					l_EditBox=GetDlgItem(IDC_PROGNAME);
					if(l_EditBox) l_EditBox->SetFocus();
					return;
				}
			}
		}
				
		m_Launchees[m_iCurrGame].ProgName=m_ProgName;
		m_Launchees[m_iCurrGame].FileParms=m_FileParms;
		m_Launchees[m_iCurrGame].FilePath=m_FilePath;
		m_Launchees[m_iCurrGame].FileName=m_FileName;
		m_Launchees[m_iCurrGame].FileIcon=m_IconPath;
	}
	
	
	SelGame=m_Games.GetCurSel();
	if(SelGame!=LB_ERR) m_Games.GetText(SelGame,l_SelectedGame);
	m_iCurrGame=-1;

	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(l_SelectedGame)==0)
		{
			m_iCurrGame=GameNum;
			break;
		}
	}
	if(m_iCurrGame==-1) 
	{
		SetDlgItemText(IDC_PROGNAME,"");
		SetDlgItemText(IDC_PROGDIR,"");
		SetDlgItemText(IDC_FILENAME1,"");
		SetDlgItemText(IDC_PROGPARMS,"");
		return;
	}

	SetDlgItemText(IDC_PROGNAME,m_Launchees[GameNum].ProgName);
	SetDlgItemText(IDC_PROGPARMS,m_Launchees[GameNum].FileParms);
	SetDlgItemText(IDC_PROGDIR,m_Launchees[GameNum].FilePath);
	SetDlgItemText(IDC_FILENAME1,m_Launchees[GameNum].FileName);
	m_IconPath=m_Launchees[GameNum].FileIcon;
	HICON myIcon;
	myIcon = ExtractIcon(AfxGetInstanceHandle(),m_Launchees[GameNum].FileIcon,0);
	if(myIcon) m_GameIcon.SetIcon(myIcon);
	else m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	
}

BOOL CConfigLauncher::OnInitDialog() 
{
	int GameNum;
	
	CDialog::OnInitDialog();
	
	//Scan through m_Launchees, finding anything in use, and add the item to
	//the listbox
	m_Games.ResetContent();
	
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(!m_Launchees[GameNum].ProgName.IsEmpty())
		{
			m_Games.AddString(m_Launchees[GameNum].ProgName);
		}
	
	}
	if(m_strAutoSel.IsEmpty())
	{	
		m_Games.SetCurSel(0);
		OnSelchangeList1();
	}
	else
	{
		m_Games.SetCurSel(m_Games.FindStringExact(0,m_strAutoSel));		
		OnSelchangeList1();
		m_strAutoSel.Empty();
	}
	
	if(((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch)
	{
		m_ConfirmButton.SetCheck(1);
	}
	else
	{
		m_ConfirmButton.SetCheck(0);
	}
	
	if(((CPconwApp *)AfxGetApp())->m_ibCloseChat)
	{
		m_CloseButton.SetCheck(1);
	}
	else
	{
		m_CloseButton.SetCheck(0);
	}
    if(m_bFirstRun) OnFindgames();
	
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConfigLauncher::ReadRegistry(void)
{
    int GameNum=0;
    BOOL l_FirstRun=FALSE;
//********************************	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "CloseChat", ((CPconwApp *)AfxGetApp())->m_ibCloseChat ) != TRUE )
   {
      ((CPconwApp *)AfxGetApp())->m_ibCloseChat=1;
	  registry.SetValue( "CloseChat",((CPconwApp *)AfxGetApp())->m_ibCloseChat);
   }
   if ( registry.GetValue( "ConfirmLaunch", ((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch ) != TRUE )
   {
      ((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch=1;
	  registry.SetValue( "ConfirmLaunch",((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch);
   }
//********************************




	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
   
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
      l_FirstRun = TRUE;
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
  // if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;	
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   ////registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   //registry.Close();
   GameNum++;
   //if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return TRUE;
   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return TRUE;
	  }
	  else return TRUE;	  
    }

   if ( registry.GetValue( "ProgramName", m_Launchees[GameNum].ProgName ) != TRUE )
   {
      m_Launchees[GameNum].ProgName.Empty();
	  registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
   }
   if ( registry.GetValue( "FileName", m_Launchees[GameNum].FileName ) != TRUE )
   {
      m_Launchees[GameNum].FileName.Empty();
	  registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
   }
   if ( registry.GetValue( "FilePath", m_Launchees[GameNum].FilePath ) != TRUE )
   {
      m_Launchees[GameNum].FilePath.Empty();
	  registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
   }
   if ( registry.GetValue( "FileParms", m_Launchees[GameNum].FileParms ) != TRUE )
   {
      m_Launchees[GameNum].FileParms.Empty();
	  registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
   }
   if ( registry.GetValue( "FileIcon", m_Launchees[GameNum].FileIcon ) != TRUE )
   {
      m_Launchees[GameNum].FileIcon.Empty();
	  registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
   }
   registry.Close();
   GameNum++;
   if(l_FirstRun) return FALSE;
   else return TRUE;
}

void CConfigLauncher::WriteRegistry(void)
{
	int GameNum=0;
//***********************************************
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
   
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
    {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
    }

 
	registry.SetValue( "CloseChat",((CPconwApp *)AfxGetApp())->m_ibCloseChat);
 	registry.SetValue( "ConfirmLaunch",((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch);
 
//***********************************************	
	
	
	
	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game01",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game02",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;	
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game03",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game04",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game05",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game06",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game07",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game08",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game09",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game10",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;

	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game11",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game12",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game13",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game14",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game15",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game16",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game17",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game18",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game19",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	//registry.Close();
	GameNum++;
	//if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE ) return;
	if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\LaunchPad\\Game20",
                      (CRegistry::CreatePermissions)(CRegistry::permissionRead | CRegistry::permissionSetValue)
                     ) != TRUE ) return;
	  }
	  else return;	  
   }
	registry.SetValue( "ProgramName", m_Launchees[GameNum].ProgName );
	registry.SetValue( "FileParms", m_Launchees[GameNum].FileParms );
	registry.SetValue( "FileIcon", m_Launchees[GameNum].FileIcon );
	registry.SetValue( "FilePath", m_Launchees[GameNum].FilePath );
	registry.SetValue( "FileName", m_Launchees[GameNum].FileName );
	registry.Close();

}

void CConfigLauncher::OnFilefind() 
{
	UpdateData(TRUE);
	CString l_strFilePath,l_strFileDir,l_strFileTitle;
	if(m_FileName.GetLength()) l_strFilePath.Format("%s\\%s",m_FilePath,m_FileName);
	else l_strFilePath.Format("%s\\*.exe",m_FilePath);
	char szFilter[] = "All files (*.*) | *.* ||";
	CFileDialog l_FileChooser(TRUE,NULL,l_strFilePath,OFN_HIDEREADONLY,szFilter);
	l_FileChooser.DoModal();
	//Separate the file from the directory
	SetDlgItemText(IDC_FILENAME1,l_FileChooser.GetFileName());
	
	l_strFileTitle=l_FileChooser.GetFileName();
	l_strFilePath=l_FileChooser.GetPathName();
	l_strFileDir=l_strFilePath.Left(l_strFilePath.GetLength()-(l_strFileTitle.GetLength()-3));
	
	int l_lastbackslash=0;
	for(int i=0;i<l_strFilePath.GetLength();i++)
	{
		if(l_strFilePath[i]=='\\') l_lastbackslash=i;
	}
	l_strFileDir=l_strFilePath.Left(l_lastbackslash);
	l_strFileTitle=l_strFilePath.Right(l_strFilePath.GetLength()-(l_lastbackslash+1));	
	
	
	
	SetDlgItemText(IDC_PROGDIR,l_strFileDir);
	SetDlgItemText(IDC_PROGPARMS,GetParms(l_strFileTitle));
	HICON myIcon;
	myIcon = ExtractIcon(AfxGetInstanceHandle(),l_strFilePath,0);
	if(myIcon) m_GameIcon.SetIcon(myIcon);
	else m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	m_IconPath=l_strFilePath;
	
}

void CConfigLauncher::OnKillfocusFilename1() //WM_KILLFOCUS
{
	//Validate that the file exists!
	UpdateData(TRUE);
	CString l_strFilePath;
	l_strFilePath.Format("%s\\%s",m_FilePath,m_FileName);

	CFile l_FileExist;
	if(l_FileExist.Open(l_strFilePath,CFile::modeRead))
	{
		l_FileExist.Close();
		return;
	}
	else 
	{
		//Display a MessageBox with unable to open file message
		CString l_strFileExist,l_strerr;
		l_strFileExist.LoadString(IDS_STRFILENOEXIST);
		l_strerr.LoadString(IDS_STRERROR);
		MessageBox(l_strFileExist,l_strerr,MB_OK|MB_ICONEXCLAMATION);
		l_FileExist.Abort();
		return;
	}
}

void CConfigLauncher::OnKillfocusProgdir() 
{
	UpdateData(TRUE);
	if(m_FilePath[m_FilePath.GetLength()-1]=='\\') m_FilePath=m_FilePath.Left(m_FilePath.GetLength()-1);
	SetDlgItemText(IDC_PROGDIR,m_FilePath);
	
}

void CConfigLauncher::OnKillfocusProgname() 
{
	
	UpdateData(TRUE);
	//Check for duplicate titles
	int GameNum;
	
	//search for empty slot
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if((m_Launchees[GameNum].ProgName.Compare(m_ProgName)==0)&&(m_iCurrGame!=GameNum))
		{
			//Duplicate, set focus & give warning
			CString l_tmpmsg,l_tmpstrerr;
			l_tmpmsg.LoadString(IDS_STRDUPTITLE);
			l_tmpstrerr.LoadString(IDS_STRERROR);
			MessageBox(l_tmpmsg,l_tmpstrerr,MB_OK);
			
			CWnd * l_EditBox;
			l_EditBox=GetDlgItem(IDC_PROGNAME);
			if(l_EditBox) l_EditBox->SetFocus();
			return;
		}
	}
	//Delete the old.....
	//m_SaveTitle
	m_Games.DeleteString(SelGame);
	//Add the new....
	//m_ProgName
	SelGame=m_Games.AddString(m_ProgName);
	m_Games.SetCurSel(SelGame);
}

void CConfigLauncher::AddDraggedFile(CString &FileName)
{
	//Take the filename...use it as the title & icon also...
	//Then DoModal()
	int GameNum;
	
	//search for empty slot
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.IsEmpty())
		{
			m_iCurrGame=GameNum;
			goto skip;
		}
	}
skip:
	if(GameNum>=20)
	{
		//No Room
		CString l_tmpstr,l_tmpstrerr;
		l_tmpstr.LoadString(IDS_STRMAXITEM);
		l_tmpstrerr.LoadString(IDS_STRERROR);
		MessageBox(l_tmpstr,l_tmpstrerr,MB_OK);
		return;
	}
		
	//Check for dup. title
		for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(FileName)==0)
		{
			//This title is already used
			CString l_tmpmsg,l_tmpstrerr;
			l_tmpmsg.LoadString(IDS_STRDUPTITLE);
			l_tmpstrerr.LoadString(IDS_STRERROR);
			MessageBox(l_tmpmsg,l_tmpstrerr,MB_OK);
			
//			OnSelchangeList1();
//			m_Games.SetCurSel(0);
			return;
		}
	}
		
	m_ProgName=FileName;
	//add it to the main dialog....
	//SetDlgItemText(IDC_PROGNAME,FileName);
	
	//split up the file & path
	CString l_Path,l_File;
	int l_lastbackslash=0;
	for(int i=0;i<FileName.GetLength();i++)
	{
		if(FileName[i]=='\\') l_lastbackslash=i;
	}
	l_Path=FileName.Left(l_lastbackslash);
	l_File=FileName.Right(FileName.GetLength()-(l_lastbackslash+1));	
	
	
	//SetDlgItemText(IDC_PROGDIR,l_Path);
	//SetDlgItemText(IDC_FILENAME1,l_File);
	//SetDlgItemText(IDC_PROGPARMS,GetParms(l_File));
	m_IconPath=FileName;
	
	//HICON myIcon;
	//myIcon = ExtractIcon(AfxGetInstanceHandle(),m_IconPath,0);
	//if(myIcon) m_GameIcon.SetIcon(myIcon);
	//else m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	
	//m_Games.SetCurSel(m_Games.AddString(m_ProgName));

	m_Launchees[m_iCurrGame].ProgName=m_ProgName;
	m_Launchees[m_iCurrGame].FileParms=m_FileParms;
	m_Launchees[m_iCurrGame].FilePath=l_Path;
	m_Launchees[m_iCurrGame].FileName=l_File;
	m_Launchees[m_iCurrGame].FileIcon=m_IconPath;
	m_iCurrGame=-1;
	
	m_strAutoSel=m_ProgName;
	if(DoModal()!=IDCANCEL) WriteRegistry();	
	else ReadRegistry();
}

CString CConfigLauncher::GetParms(CString &FileName)
{
	if(FileName.Compare("war2.exe")==0) return CString("pkt:8");
	if(FileName.Compare("duke3d.exe")==0) return CString("/f4");
	return CString("");
}

void CConfigLauncher::OnSetfocusProgname() 
{
	UpdateData(TRUE);
	m_SaveTitle=m_ProgName;	
}


//*******************************************************************
// FindFile - find files matching a pattern files on specified drives
//
// paramaters:
//             drives   - An array of drive letters to search.
//             pattern  - A file name pattern.
//
// returns:
//             Number of files found matching pattern.
//             If unable to add a name to listbox, number is returned as
//             a negative.
//
//*******************************************************************
long CConfigLauncher::FindFile(char *drives, char *pattern)
{
    char     tree[3];
    int      nfiles;
    long      rc;
	int		file_count;

    rc = 0;
    file_count = 0;
    tree[1] = ':';
    tree[2] = 0;
    while (*drives)
    {
        tree[0] = *drives++;
        nfiles = DoADir(tree, "*.*", pattern);
        if (nfiles >= 0)
            file_count += nfiles;
        else
        {
            file_count -= nfiles;
            rc = -file_count;
            break;
        }
    }

    return(rc);
}

//*******************************************************************
// DoADir - search a directory for files matching a file name pattern
//          recursivly search sub directories
//
// paramaters:
//             patternp  - A path to search.
//             patternn  - A file name pattern to use to find directories.
//             include   - A file name pattern to use to select files.
//
// returns:
//             Number of files found matching include.
//             If unable to add a name to listbox, number is returned as
//             a negative.
//
//*******************************************************************
int CConfigLauncher::DoADir(char *patternp, char *patternn, char *include)
{
    char          patternw[MAX_PATH];
    char          npatternp[MAX_PATH];
    char          buf[256];
    int           mfiles;
    int           have_subs;
    int           nfiles;
    LONG          lrc;
    struct _finddata_t  fileinfo;
	CString l_GamePath,l_GameIcon,l_GameTitle;

    strcpy(patternw, patternp);
    strcat(patternw, "\\");

    strcat(patternw, patternn);

    mfiles = 0;
    have_subs = 0;

    int isearchhandle = _findfirst(patternw, &fileinfo);
	if (isearchhandle)
    {
        do
        {
            if (fileinfo.attrib & _A_SUBDIR)  // subdirectory
            {
                if (fileinfo.name[0] != '.')  // ignore . and ..
                    have_subs = 1;
            }
            else                              // file
            {
                
			    
                CurrGame=FirstGame;
                
                while(CurrGame)
                {
                    //CurrGame->FindFile.MakeLower();
                        //CString
                    if(0==stricmp(LPCSTR(CurrGame->FindFile),fileinfo.name)) 
                    {
                        if(CurrGame->Icon.IsEmpty()) l_GameIcon.Format("%s\\%s",patternp,CurrGame->UseFile);
                        else l_GameIcon.Format("%s%s",((CPconwApp *)AfxGetApp())->m_PconwPath,CurrGame->Icon);
                        l_GamePath.Format("%s\\%s",patternp,CurrGame->UseFile);
                        AutoAddGame(l_GamePath,CurrGame->Name,l_GameIcon,CurrGame->Parms);
                    }
                    CurrGame = CurrGame->Next;
                }

			
                
                /*
                if(stricmp("war2.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEWAR2);
					AutoAddGame(l_GamePath,l_GameTitle,CString("war2.ico"),CString("pkt:8"));
				}
				if(stricmp("descentr.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEDESCENT);
					AutoAddGame(l_GamePath,l_GameTitle,CString("descent.ico"),CString("-DynamicSockets"));
				}
				if(stricmp("descent2.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMED2);
					AutoAddGame(l_GamePath,l_GameTitle,CString("descent2.ico"),CString("-noplayermessages -shortpackets -packets 5"));
				}
				if(stricmp("quake.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEQUAKE);
					AutoAddGame(l_GamePath,l_GameTitle,CString("quake.ico"),CString(""));
				}
				if(stricmp("duke3d.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,"setup.exe");
					l_GameTitle.LoadString(IDS_STRGAMEDUKE3D);
					AutoAddGame(l_GamePath,l_GameTitle,CString("duke3d.ico"),CString("/f4"));
				}
				if(stricmp("doom2.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,"setup.exe");
					l_GameTitle.LoadString(IDS_STRGAMEDOOM2);
					AutoAddGame(l_GamePath,l_GameTitle,CString("doom2.ico"),CString(""));
				}
				if(stricmp("Descent II Win95.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);//
					l_GameTitle.LoadString(IDS_STRGAMED2WIN95);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
				if(stricmp("ra.exe",fileinfo.name)==0)
				{
					l_GameIcon.Format("%s\\%s",patternp,"ra95.exe");
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMERADOS);
					AutoAddGame(l_GamePath,l_GameTitle,l_GameIcon,CString(""));
				}
				if(stricmp("ra95.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMERA95);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
                if(stricmp("netmech.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMENETMECH);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
                if(stricmp("racing.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEBRR);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
                if(stricmp("pool.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEVP);
					AutoAddGame(l_GamePath,l_GameTitle,CString(""),CString(""));
				}
                if(stricmp("c&c.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMECC);
					AutoAddGame(l_GamePath,l_GameTitle,CString(""),CString(""));
				}
                if(stricmp("tv.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,"setup.exe");
					l_GameTitle.LoadString(IDS_STRGAMETV);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
                if(stricmp("netipx.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEOMF);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString("netarena -l 0 file0001"));
				}
                
                if(stricmp("linksls.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMELINKLS);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
				}
                if(stricmp("d2demo.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMED2DEMO);
					AutoAddGame(l_GamePath,l_GameTitle,CString(""),CString(""));
				}
                if(stricmp("diablo.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEDIABLO);
					AutoAddGame(l_GamePath,l_GameTitle,CString(""),CString(""));
				}
                if(stricmp("a10cuba.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEA10);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                if(stricmp("ssteel.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMESS);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                if(stricmp("nascar2.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMENASCAR);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                if(stricmp("topgun.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRTOPGUN);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                if(stricmp("risk.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMERISK);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                if(stricmp("monsterx.exe",fileinfo.name)==0)
				{
					l_GamePath.Format("%s\\%s",patternp,fileinfo.name);
					l_GameTitle.LoadString(IDS_STRGAMEMTM);
					AutoAddGame(l_GamePath,l_GameTitle,l_GamePath,CString(""));
        		}
                

            */
			
			}
		MSG msg;
		while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))	DispatchMessage(&msg);
		if(((CPconwApp *)AfxGetApp())->g_iCancel) return 0;
		}
        while (!_findnext(isearchhandle,&fileinfo));
    }

    if (have_subs)
    {
        int isearchhandle2 = _findfirst(patternw, &fileinfo);
		if (isearchhandle2)
        {
            do
            {
                if (fileinfo.attrib & _A_SUBDIR)  // subdirectory
                {
                    if (fileinfo.name[0] != '.')  // ignore . and ..
                    {
                        strcpy(npatternp, patternp);
                        strcat(npatternp, "\\");
                        strcat(npatternp, fileinfo.name);

                        nfiles = DoADir(npatternp, patternn, include);

                        if (nfiles >= 0)
                            mfiles += nfiles;
                        else
                        {
                                                        mfiles -= nfiles;
                            return(-mfiles); // error return
                        }
                    }
                }
			MSG msg;
			while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))	DispatchMessage(&msg);			
			if(((CPconwApp *)AfxGetApp())->g_iCancel) return 0;
		}
            while (!_findnext(isearchhandle2,&fileinfo));
      }
   }


        return(mfiles);
}

//*******************************************************************
// FindDrives - find floppy and hard drives in system
//
// paramaters:
//             fdrives - An array to hold floppy drive letters.
//             hdrives - An array to hold hard drive letters.
//
//*******************************************************************
void CConfigLauncher::FindDrives(char *fdrives, char *hdrives)
{
        unsigned int  savedriveno;
		  unsigned int  maxdriveno;
		  unsigned int  driveno;
		  unsigned int  drivetype;
		  int           nfdrives = 0;
		  int           nhdrives = 0;
		  char driveroot[4];

		  savedriveno = _getdrive() + 1;               // save current drive
		  maxdriveno = 26; //_chdrive(savedriveno - 1);     // find max drive in system


		  for (driveno=0; driveno<maxdriveno; driveno++)
					 {
					 strcpy(driveroot, " :\\");
					 driveroot[0] = (char)('A'+driveno);
					 drivetype = GetDriveType((LPCSTR)driveroot);
					 if ((drivetype == 0)||(drivetype==1))
								continue;
					 else
								if (drivetype == DRIVE_REMOVABLE)
								{
                        fdrives[nfdrives++] = (char) (driveno + 'A');
                        }
                        else
                                {
                                hdrives[nhdrives++] = (char) (driveno + 'A');
                                }
                }

        fdrives[nfdrives] = 0;
        hdrives[nhdrives] = 0;

		  _chdrive(savedriveno - 1);                  // reset current drive
}

//*******************************************************************

//*******************************************************************
// FnMatch - test if a file name matches a file name pattern.
//           handles * and ? wildcard characters.
//
// paramaters:
//             pat   - A file name pattern (ie. xyz?.*)
//             name  - A file name to test against pat (ie. xyz1.c)
//
// returns:
//             1 - if match
//             0 - if not a match
//
//*******************************************************************
int CConfigLauncher::FnMatch(char *pat, char *name)
{
    int   match;
    int   ndone;
    char *cpp;
    char *cpn;

    cpp = pat;
    cpn = name;
    match = 1;
    ndone = 1;
    while (ndone)
    {
        switch (*cpp)
        {
            case '*':
                // skip to next letter
                cpp++;

                // skip to next letter
                cpn = strchr(cpn, *cpp);
                if (cpn == NULL)
                {
                    cpn = name;
                    while (*cpn)
                         cpn++;
                }
                break;

            case '?':
                cpp++;
                cpn++;
                break;

            case 0:
                if (*cpn != 0)
                    match = 0;
                ndone = 0;
                break;

            default:
                if (tolower(*cpp) == tolower(*cpn))
                {
                    cpp++;
                    cpn++;
                }
                else
                {
                    match = 0;
                    ndone = 0;
                }
                break;
        }
    }

    return(match);
}



//*******************************************************************




void CConfigLauncher::OnFindgames() 
{
	char strFdisks[26];
	char strflopdsks[8];
	char strScanDisk[4];
	CString SearchMsg;
	CAbortScan l_Abort;

	//Find all fixed disks.
	FindDrives(strflopdsks,strFdisks);
	//loop on all drives
	for(int iLoopa=0;iLoopa<strlen(strFdisks);iLoopa++)
	{
		//Do you want to search drive x:?
		if(strFdisks[iLoopa]=='\0') return;
		sprintf(strScanDisk,"%c",strFdisks[iLoopa]);
		
		CString l_tmpmsg1,l_tmpmsg2;
		l_tmpmsg1.LoadString(IDS_STRSCAN1);
		l_tmpmsg2.LoadString(IDS_STRSCAN2);
		
		SearchMsg.Format("%s %s %s",l_tmpmsg1,strScanDisk,l_tmpmsg2);
		if(MessageBox(SearchMsg,"Find Games",MB_YESNO)==IDYES)
		{
			((CPconwApp *)AfxGetApp())->g_iCancel=0;
			l_Abort.Create(IDD_ABORTSCAN,this);//
			FindFile(strScanDisk,"*.exe");	
			l_Abort.DestroyWindow();
		}
		
	}
	
}

void CConfigLauncher::AutoAddGame (CString &FileName,CString &Title,CString &Icon,CString &Parms)
{
	int GameNum;
	
	//search for empty slot
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.IsEmpty())
		{
			m_iCurrGame=GameNum;
			goto skip;
		}
	}
skip:
	if(GameNum>=20)
	{
		//No Room
		ResetSelItem();
		return;
	}
		
	//Check for dup. title
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(Title)==0)
		{
			//This title is already used
			ResetSelItem();
			return;
		}
	}
	//Check for dup. exe
	for(GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(FileName)==0)
		{
			//This exe is already used
			ResetSelItem();
			return;
		}
	}
		
	m_ProgName=Title;
	//add it to the main dialog....
	//SetDlgItemText(IDC_PROGNAME,FileName);
	
	//split up the file & path
	CString l_Path,l_File;
	int l_lastbackslash=0;
	for(int i=0;i<FileName.GetLength();i++)
	{
		if(FileName[i]=='\\') l_lastbackslash=i;
	}
	l_Path=FileName.Left(l_lastbackslash);
	l_File=FileName.Right(FileName.GetLength()-(l_lastbackslash+1));	
	
	
	//SetDlgItemText(IDC_PROGDIR,l_Path);
	//SetDlgItemText(IDC_FILENAME1,l_File);
	//SetDlgItemText(IDC_PROGPARMS,GetParms(l_File));
	if(Icon.GetLength()) m_IconPath=Icon;
	else m_IconPath=FileName;
	m_FileParms=Parms;

	//HICON myIcon;
	//myIcon = ExtractIcon(AfxGetInstanceHandle(),m_IconPath,0);
	//if(myIcon) m_GameIcon.SetIcon(myIcon);
	//else m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	
	//m_Games.SetCurSel(m_Games.AddString(m_ProgName));

	m_Launchees[m_iCurrGame].ProgName=m_ProgName;
	m_Launchees[m_iCurrGame].FileParms=m_FileParms;
	m_Launchees[m_iCurrGame].FilePath=l_Path;
	m_Launchees[m_iCurrGame].FileName=l_File;
	m_Launchees[m_iCurrGame].FileIcon=m_IconPath;

	//Add the item to the list box
	//select it
	//
	//SetDlgItemText(IDC_PROGNAME,m_ProgName);
	//SetDlgItemText(IDC_PROGDIR,l_Path);
	//SetDlgItemText(IDC_FILENAME1,l_File);
	//SetDlgItemText(IDC_PROGPARMS,m_FileParms);
	m_Games.AddString(m_ProgName);
	//m_Games.SetCurSel(m_Games.AddString(m_ProgName));
	//   m_GameIcon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
	m_iCurrGame=-1;
	//OnSelchangeList1();
	ResetSelItem();

	
}

void CConfigLauncher::ResetSelItem (void)
{

	CString l_strTemp;
	SelGame=m_Games.GetCurSel();
	if(SelGame!=LB_ERR) m_Games.GetText(SelGame,l_strTemp);
	else SelGame=-1;
	
	for(int GameNum=0;GameNum<20;GameNum++)
	{
		if(m_Launchees[GameNum].ProgName.Compare(l_strTemp)==0)
		{
			m_iCurrGame=GameNum;
			return;
		}
	}



}

void CConfigLauncher::OnConfirmlnch() 
{
	UpdateData(TRUE);
	if(m_bConfirmLaunch)
	{
		((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch=1;
	}
	else
	{
		((CPconwApp *)AfxGetApp())->m_ibConfirmLaunch=0;
	}
	
}

void CConfigLauncher::OnClosechat() 
{
	UpdateData(TRUE);
	if(m_bCloseChat)
	{
		((CPconwApp *)AfxGetApp())->m_ibCloseChat=1;
	}
	else
	{
		((CPconwApp *)AfxGetApp())->m_ibCloseChat=0;
	}
	
}

CConfigLauncher::~CConfigLauncher()
{
    ScanGame *l_Cleanup;
    CurrGame=FirstGame;
    l_Cleanup = CurrGame->Next;
    while(CurrGame)
    {
        delete CurrGame;
        CurrGame=l_Cleanup;
        if(CurrGame) l_Cleanup = CurrGame->Next;
    }

}
