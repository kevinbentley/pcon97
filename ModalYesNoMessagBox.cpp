// ModalYesNoMessagBox.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "ModalYesNoMessagBox.h"


/////////////////////////////////////////////////////////////////////////////
// CModalYesNoMessagBox dialog


CModalYesNoMessagBox::CModalYesNoMessagBox(CWnd* pParent /*=NULL*/)
	: CDialog(CModalYesNoMessagBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModalYesNoMessagBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModalYesNoMessagBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModalYesNoMessagBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModalYesNoMessagBox, CDialog)
	//{{AFX_MSG_MAP(CModalYesNoMessagBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModalYesNoMessagBox message handlers

BOOL CModalYesNoMessagBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_MESSAGETEXT,m_MessageText);
	SetWindowText(m_MessageTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
