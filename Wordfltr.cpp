// WordFltr.cpp : implementation file
//

#include "stdafx.h"

#include "ClrStatic.h"
#include "WordFltr.h"


/////////////////////////////////////////////////////////////////////////////
// CWordFltr dialog


CWordFltr::CWordFltr(CWnd* pParent /*=NULL*/)
	: CDialog(CWordFltr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWordFltr)
	m_NewText = _T("");
	//}}AFX_DATA_INIT

   ;


}


void CWordFltr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWordFltr)
	DDX_Control(pDX, IDC_SAMPLECLR, m_statSampleTxt);
	DDX_Control(pDX, IDC_WORDS, m_lbWords);
	DDX_Text(pDX, IDC_NEWTEXT, m_NewText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWordFltr, CDialog)
	//{{AFX_MSG_MAP(CWordFltr)
	ON_LBN_SELCHANGE(IDC_WORDS, OnSelchangeWords)
	ON_BN_CLICKED(IDC_ADD2LIST, OnAdd2list)
	ON_BN_CLICKED(IDC_DELETEITM, OnDeleteitm)
	ON_BN_CLICKED(IDC_SETCOLOR, OnSetcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWordFltr message handlers

void CWordFltr::OnSelchangeWords() 
{
	m_lbWords.GetText(m_lbWords.GetCurSel(),m_CurSel);

	m_statSampleTxt.m_clrBackground=((CPconwApp*)AfxGetApp())->colorBACK;
	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
   registry.GetValue(m_CurSel,m_statSampleTxt.m_clrForeground);
   registry.Close();
	
   UpdateData(TRUE);
	RedrawWindow();
	
}
void CWordFltr::OnOK() 
{
	((CPconwApp*)AfxGetApp())->GetFilters();
	CDialog::OnOK();
}

void CWordFltr::OnAdd2list() 
{
	
	
	UpdateData(TRUE);
	
	if(m_lbWords.FindStringExact(0,m_NewText)!=LB_ERR) 
	{
		MessageBox("That word is already in the list\n(Note: Words are NOT case sensitive!)","Word not added");
		return;
	}


	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
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
   m_NewText.MakeLower();
   registry.SetValue(m_NewText,m_statSampleTxt.m_clrForeground);
   m_lbWords.SetCurSel(m_lbWords.AddString(m_NewText));
   OnSelchangeWords();
   registry.Close();
	
}

void CWordFltr::OnDeleteitm() 
{
	if(m_CurSel.IsEmpty()) return;
	m_lbWords.DeleteString(m_lbWords.FindStringExact(0,m_CurSel));
	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      return;
   }

   if ( registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters",
	   (CRegistry::CreatePermissions)(CRegistry::permissionAllAccess)
                     ) != TRUE )
   {
      if(registry.Create("SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters"))
	  {
		if(registry.Open( "SOFTWARE\\StarGate\\Kahn97\\Chat\\Filters",
                      (CRegistry::CreatePermissions)(CRegistry::permissionAllAccess)
                     ) != TRUE ) 
		{
			return;
		}
				
	  }
	  else return;
	     }
   registry.DeleteValue(m_CurSel);
   
   registry.Close();
	
}

void CWordFltr::OnSetcolor() 
{
	CColorDialog l_ColorDlg(m_statSampleTxt.m_clrForeground,0,this);
	l_ColorDlg.DoModal();
	m_statSampleTxt.m_clrForeground=l_ColorDlg.GetColor();
	UpdateData();
	RedrawWindow();
	if(m_CurSel.IsEmpty()) return;

   if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
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
			return;
		}
				
	  }
	  else return;
	     }
   registry.SetValue(m_CurSel,m_statSampleTxt.m_clrForeground);
   registry.Close();
	


	
}

BOOL CWordFltr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if ( registry.Connect( CRegistry::keyLocalMachine ) != TRUE )
   {
      //TRACE( "Can't connect to registry\n" );
      return TRUE;
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
			//TRACE( "Can't open key\n" );
		    //ReportError( registry.GetErrorCode() );
			return TRUE;
		}
				
	  }
	  else
	  {
		//TRACE( "Can't open key\n" );
		//ReportError( registry.GetErrorCode() );
		return TRUE;
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
	   registry.EnumerateValues(iloop,tmpstring,(CRegistry::KeyValueTypes &)thistype,NULL,tmpdword);	
	   m_lbWords.AddString(tmpstring);
   }
   
   m_statSampleTxt.m_clrBackground=((CPconwApp*)AfxGetApp())->colorBACK;
   m_statSampleTxt.m_clrForeground=((CPconwApp*)AfxGetApp())->colorNORM;

   registry.Close();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
