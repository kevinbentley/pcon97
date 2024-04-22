// FKeySetup.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"

#include <wfc.h>

#include "FKeySetup.h"


/////////////////////////////////////////////////////////////////////////////
// CFKeySetup dialog


CFKeySetup::CFKeySetup(CWnd* pParent /*=NULL*/)
	: CDialog(CFKeySetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFKeySetup)
	m_F10 = _T("");
	m_F11 = _T("");
	m_F12 = _T("");
	m_F2 = _T("");
	m_F3 = _T("");
	m_F4 = _T("");
	m_F5 = _T("");
	m_F6 = _T("");
	m_F7 = _T("");
	m_F8 = _T("");
	m_F9 = _T("");
	//}}AFX_DATA_INIT

   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      TRACE( "Can't connect to registry\n" );
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
	
   
   if ( registry.GetValue( "F2", m_F2 ) != TRUE )
   {
      m_F2.LoadString(IDS_STRFKEY2);
	  registry.SetValue( "F2", m_F2 );
   }
   if ( registry.GetValue( "F3", m_F3 ) != TRUE )
   {
      m_F3.LoadString(IDS_STRFKEY3);
	  registry.SetValue( "F3", m_F3 );
   }
   if ( registry.GetValue( "F4", m_F4 ) != TRUE )
   {
      m_F4.LoadString(IDS_STRFKEY4);
	  registry.SetValue( "F4", m_F4 );
   }
   if ( registry.GetValue( "F5", m_F5 ) != TRUE )
   {
      m_F5.LoadString(IDS_STRFKEY5);
	  registry.SetValue( "F5", m_F5 );
   }
   if ( registry.GetValue( "F6", m_F6 ) != TRUE )
   {
      m_F6.LoadString(IDS_STRFKEY6);
	  registry.SetValue( "F6", m_F6 );
   }
   if ( registry.GetValue( "F7", m_F7 ) != TRUE )
   {
      m_F7.LoadString(IDS_STRFKEY7);
	  registry.SetValue( "F7", m_F7 );
   }
   if ( registry.GetValue( "F8", m_F8 ) != TRUE )
   {
      m_F8.LoadString(IDS_STRFKEY8);
	  registry.SetValue( "F8", m_F8 );
   }
   if ( registry.GetValue( "F9", m_F9 ) != TRUE )
   {
      m_F9.LoadString(IDS_STRFKEY9);
	  registry.SetValue( "F9", m_F9 );
   }
   if ( registry.GetValue( "F10", m_F10 ) != TRUE )
   {
      m_F10.LoadString(IDS_STRFKEY10);
	  registry.SetValue( "F10", m_F10 );
   }
   if ( registry.GetValue( "F11", m_F11 ) != TRUE )
   {
      m_F11.LoadString(IDS_STRFKEY11);
	  registry.SetValue( "F11", m_F11 );
   }
   if ( registry.GetValue( "F12", m_F12 ) != TRUE )
   {
      m_F12.LoadString(IDS_STRFKEY12);
	  registry.SetValue( "F12", m_F12 );
   }
  
   registry.Close();


}


void CFKeySetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFKeySetup)
	DDX_Text(pDX, IDC_F10KEY, m_F10);
	DDV_MaxChars(pDX, m_F10,200);
	DDX_Text(pDX, IDC_F11KEY, m_F11);
	DDV_MaxChars(pDX, m_F11,200);	
	DDX_Text(pDX, IDC_F12KEY, m_F12);
	DDV_MaxChars(pDX, m_F12,200);
	DDX_Text(pDX, IDC_F2KEY, m_F2);
	DDV_MaxChars(pDX, m_F2,200);
	DDX_Text(pDX, IDC_F3KEY, m_F3);
	DDV_MaxChars(pDX, m_F3,200);
	DDX_Text(pDX, IDC_F4KEY, m_F4);
	DDV_MaxChars(pDX, m_F4,200);
	DDX_Text(pDX, IDC_F5KEY, m_F5);
	DDV_MaxChars(pDX, m_F5,200);
	DDX_Text(pDX, IDC_F6KEY, m_F6);
	DDV_MaxChars(pDX, m_F6,200);
	DDX_Text(pDX, IDC_F7KEY, m_F7);
	DDV_MaxChars(pDX, m_F7,200);
	DDX_Text(pDX, IDC_F8KEY, m_F8);
	DDV_MaxChars(pDX, m_F8,200);
	DDX_Text(pDX, IDC_F9KEY, m_F9);
	DDV_MaxChars(pDX, m_F9,200);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFKeySetup, CDialog)
	//{{AFX_MSG_MAP(CFKeySetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFKeySetup message handlers

void CFKeySetup::OnOK() 
{

   UpdateData(TRUE);
   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
			TRACE( "Can't open key\n" );
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
	
   registry.SetValue( "F2", m_F2 );
   registry.SetValue( "F3", m_F3 );
   registry.SetValue( "F4", m_F4 );
   registry.SetValue( "F5", m_F5 );
   registry.SetValue( "F6", m_F6 );
   registry.SetValue( "F7", m_F7 );
   registry.SetValue( "F8", m_F8 );
   registry.SetValue( "F9", m_F9 );
   registry.SetValue( "F10", m_F10 );
   registry.SetValue( "F11", m_F11 );
   registry.SetValue( "F12", m_F12 );
   ((CPconwApp*)AfxGetApp())->F2key=m_F2;
   ((CPconwApp*)AfxGetApp())->F3key=m_F3;
   ((CPconwApp*)AfxGetApp())->F4key=m_F4;
   ((CPconwApp*)AfxGetApp())->F5key=m_F5;
   ((CPconwApp*)AfxGetApp())->F6key=m_F6;
   ((CPconwApp*)AfxGetApp())->F7key=m_F7;
   ((CPconwApp*)AfxGetApp())->F8key=m_F8;
   ((CPconwApp*)AfxGetApp())->F9key=m_F9;
   ((CPconwApp*)AfxGetApp())->F10key=m_F10;
   ((CPconwApp*)AfxGetApp())->F11key=m_F11;
   ((CPconwApp*)AfxGetApp())->F12key=m_F12;


   registry.Close();
   
   CDialog::OnOK();
}
