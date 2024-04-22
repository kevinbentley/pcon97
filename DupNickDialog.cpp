// DupNickDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "DupNickDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CDupNickDialog dialog


CDupNickDialog::CDupNickDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDupNickDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDupNickDialog)
	m_NewNick = _T("");
	//}}AFX_DATA_INIT
}


void CDupNickDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDupNickDialog)
	DDX_Text(pDX, IDC_NEWNICK, m_NewNick);
	DDV_MaxChars(pDX, m_NewNick, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDupNickDialog, CDialog)
	//{{AFX_MSG_MAP(CDupNickDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDupNickDialog message handlers

BOOL CDupNickDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_NEWNICK,m_OldNick);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDupNickDialog::OnOK() 
{
	UpdateData(TRUE);
	m_ChosenNick=m_NewNick;	
	CDialog::OnOK();
}
