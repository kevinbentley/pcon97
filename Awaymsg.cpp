// AwayMsg.cpp : implementation file
//

#include "stdafx.h"

#include "AwayMsg.h"


/////////////////////////////////////////////////////////////////////////////
// CAwayMsg dialog


CAwayMsg::CAwayMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CAwayMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAwayMsg)
	m_AwayMsg = _T("");
	//}}AFX_DATA_INIT
}


void CAwayMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAwayMsg)
	DDX_Text(pDX, IDC_AWAYMSG, m_AwayMsg);
	DDV_MaxChars(pDX, m_AwayMsg, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAwayMsg, CDialog)
	//{{AFX_MSG_MAP(CAwayMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAwayMsg message handlers
