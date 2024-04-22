// SendPrvMsg.cpp : implementation file
//

#include "stdafx.h"
//#include "chatter.h"

#include "SendPrvMsg.h"

/////////////////////////////////////////////////////////////////////////////
// SendPrvMsg dialog


SendPrvMsg::SendPrvMsg(CWnd* pParent /*=NULL*/)
	: CDialog(SendPrvMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SendPrvMsg)
	m_Message = _T("");
	//}}AFX_DATA_INIT
}


void SendPrvMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SendPrvMsg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Text(pDX, IDC_EDIT1, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SendPrvMsg, CDialog)
	//{{AFX_MSG_MAP(SendPrvMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SendPrvMsg message handlers
