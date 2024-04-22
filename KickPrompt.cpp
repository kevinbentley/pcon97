// KickPrompt.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "KickPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKickPrompt dialog


CKickPrompt::CKickPrompt(CWnd* pParent /*=NULL*/)
	: CDialog(CKickPrompt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKickPrompt)
	m_KickMsg = _T("");
	//}}AFX_DATA_INIT
}


void CKickPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKickPrompt)
	DDX_Text(pDX, IDC_KICKMSG, m_KickMsg);
	DDV_MaxChars(pDX, m_KickMsg, 76);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKickPrompt, CDialog)
	//{{AFX_MSG_MAP(CKickPrompt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKickPrompt message handlers

void CKickPrompt::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
