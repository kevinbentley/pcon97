// LaunchNewTitle.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "LaunchNewTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaunchNewTitle dialog


CLaunchNewTitle::CLaunchNewTitle(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunchNewTitle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaunchNewTitle)
	m_NewTitle = _T("");
	//}}AFX_DATA_INIT
}


void CLaunchNewTitle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaunchNewTitle)
	DDX_Text(pDX, IDC_TITLE, m_NewTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaunchNewTitle, CDialog)
	//{{AFX_MSG_MAP(CLaunchNewTitle)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaunchNewTitle message handlers
