// olorSetup.cpp : implementation file
//

#include "stdafx.h"

#include <wfc.h>

#include "ClrStatic.h"

//#include "clredit.h"
#include "olorSetup.h"
#include "WordFltr.h"

#define SAMPLETEXT "Sample Text"


/////////////////////////////////////////////////////////////////////////////
// ColorSetup dialog


ColorSetup::ColorSetup(CWnd* pParent /*=NULL*/)
	: CDialog(ColorSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(ColorSetup)
	//}}AFX_DATA_INIT

	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      TRACE( "Can't connect to registry\n" );
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
	
   
   if ( registry.GetValue( "Action", m_dwAction ) != TRUE )
   {
      m_dwAction = PALETTERGB(255,0,0);
	  registry.SetValue( "Action", m_dwAction );
   }
   if ( registry.GetValue( "Normal", m_dwNormal ) != TRUE )
   {
      m_dwNormal = PALETTERGB(0,0,0);
	  registry.SetValue( "Normal", m_dwNormal );
   }
   if ( registry.GetValue( "Private", m_dwPrivate ) != TRUE )
   {
      m_dwPrivate = PALETTERGB(0,0,102);
	  registry.SetValue( "Private", m_dwPrivate );
   }
   if ( registry.GetValue( "Status", m_dwStatus ) != TRUE )
   {
      m_dwStatus = PALETTERGB(255,0,0);
	  registry.SetValue( "Status", m_dwStatus );
   }
   if ( registry.GetValue( "Background", m_dwBackground ) != TRUE )
   {
      m_dwBackground = PALETTERGB(255,255,255);
	  registry.SetValue( "Background", m_dwBackground );
   }
   if ( registry.GetValue( "URL", m_dwURL ) != TRUE )
   {
      m_dwURL = PALETTERGB(0,0,255);
	  registry.SetValue( "URL", m_dwURL);
   }


   registry.Close();
   m_Action.m_clrForeground=m_dwAction;
   m_Action.m_clrBackground=m_dwBackground;
   m_Status.m_clrForeground=m_dwStatus;
   m_Status.m_clrBackground=m_dwBackground;
   m_Private.m_clrForeground=m_dwPrivate;
   m_Private.m_clrBackground=m_dwBackground;
   m_Normal.m_clrForeground=m_dwNormal;
   m_Normal.m_clrBackground=m_dwBackground;
   m_Background.m_clrForeground=m_dwNormal;
   m_Background.m_clrBackground=m_dwBackground;
   m_URL.m_clrForeground=m_dwURL;
   m_URL.m_clrBackground=m_dwBackground;
   
}



void ColorSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ColorSetup)
	DDX_Control(pDX, IDC_URLSAMP, m_URL);
	DDX_Control(pDX, IDC_STATUSSAMP, m_Status);
	DDX_Control(pDX, IDC_PRIVATESAMP, m_Private);
	DDX_Control(pDX, IDC_NORMALSAMP, m_Normal);
	DDX_Control(pDX, IDC_BACKSAMP, m_Background);
	DDX_Control(pDX, IDC_ACTIONSAMP, m_Action);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ColorSetup, CDialog)
	//{{AFX_MSG_MAP(ColorSetup)
	ON_BN_CLICKED(IDC_STATUSTXT, OnStatustxt)
	ON_BN_CLICKED(IDC_PRIVATETXT, OnPrivatetxt)
	ON_BN_CLICKED(IDC_NORMTXT, OnNormtxt)
	ON_BN_CLICKED(IDC_BACKGROUND, OnBackground)
	ON_BN_CLICKED(IDC_ACTIONTXT, OnActiontxt)
	ON_BN_CLICKED(IDC_CFGHIGHLIGHT, OnCfghighlight)
	ON_BN_CLICKED(IDC_COLORURL, OnColorurl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColorSetup message handlers

void ColorSetup::OnStatustxt() 
{
	CColorDialog dlgStsColor(m_dwStatus,0,this);
	dlgStsColor.DoModal();
	m_dwStatus=dlgStsColor.GetColor();
	m_Status.m_clrForeground=m_dwStatus;
	UpdateData();
	RedrawWindow();
	
}

void ColorSetup::OnPrivatetxt() 
{
	CColorDialog dlgPrvColor(m_dwPrivate,0,this);
	dlgPrvColor.DoModal();
	m_dwPrivate=dlgPrvColor.GetColor();
	m_Private.m_clrForeground=m_dwPrivate;
	UpdateData();
	RedrawWindow();
	
}

void ColorSetup::OnNormtxt() 
{
	CColorDialog dlgNrmColor(m_dwNormal,0,this);
	dlgNrmColor.DoModal();
	m_dwNormal=dlgNrmColor.GetColor();
	m_Normal.m_clrForeground=m_dwNormal;
	UpdateData();
	RedrawWindow();
	
}

void ColorSetup::OnBackground() 
{
	CColorDialog dlgBkgColor(m_dwBackground,0,this);

	dlgBkgColor.DoModal();
	m_dwBackground=dlgBkgColor.GetColor();
	m_Private.m_clrBackground=m_dwBackground;
	m_Status.m_clrBackground=m_dwBackground;
	m_Action.m_clrBackground=m_dwBackground;
	m_Normal.m_clrBackground=m_dwBackground;
	m_Background.m_clrBackground=m_dwBackground;
	m_URL.m_clrBackground=m_dwBackground;
	UpdateData();
	RedrawWindow();
	
}

void ColorSetup::OnActiontxt() 
{
	CColorDialog dlgActColor(m_dwAction,0,this);
	dlgActColor.DoModal();
	m_dwAction=dlgActColor.GetColor();
	m_Action.m_clrForeground=m_dwAction;
	UpdateData();
	RedrawWindow();
	
}

void ColorSetup::OnCfghighlight() 
{
	CWordFltr WordFilterDlg;
	WordFilterDlg.DoModal();
	
}

void ColorSetup::OnOK() 
{
	
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      TRACE( "Can't connect to registry\n" );
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
			TRACE( "Can't open key\n" );
		    return;
		}
				
	  }
	  else
	  {
		TRACE( "Can't open key\n" );
		return;
	  }
   }
   registry.SetValue("Normal",m_dwNormal);
   registry.SetValue("Action",m_dwAction);
   registry.SetValue("Status",m_dwStatus);
   registry.SetValue("Private",m_dwPrivate);
   registry.SetValue("Background",m_dwBackground);
   registry.SetValue("URL",m_dwURL);
   registry.Close();
   ((CPconwApp*)AfxGetApp())->colorBACK=m_dwBackground;
   ((CPconwApp*)AfxGetApp())->colorNORM=m_dwNormal;
   ((CPconwApp*)AfxGetApp())->colorACT=m_dwAction;
   ((CPconwApp*)AfxGetApp())->colorPRIV=m_dwPrivate;
   ((CPconwApp*)AfxGetApp())->colorSTAT=m_dwStatus;
   ((CPconwApp*)AfxGetApp())->colorURL=m_dwURL;
   CDialog::OnOK();
}

BOOL ColorSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Status.SetWindowText(SAMPLETEXT);
	m_Private.SetWindowText(SAMPLETEXT);
	m_Background.SetWindowText(SAMPLETEXT);
	m_Action.SetWindowText(SAMPLETEXT);
	m_Normal.SetWindowText(SAMPLETEXT);
	m_URL.SetWindowText(SAMPLETEXT);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ColorSetup::OnColorurl() 
{
	CColorDialog dlgPrvColor(m_dwURL,0,this);
	dlgPrvColor.DoModal();
	m_dwURL=dlgPrvColor.GetColor();
	m_Private.m_clrForeground=m_dwPrivate;
	UpdateData();
	RedrawWindow();
	
}
