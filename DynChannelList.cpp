// DynChannelList.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "DynChannelList.h"

/////////////////////////////////////////////////////////////////////////////
// CDynChannelList dialog


CDynChannelList::CDynChannelList(CWnd* pParent /*=NULL*/)
	: CDialog(CDynChannelList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynChannelList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDynChannelList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynChannelList)
	DDX_Control(pDX, IDC_LIST1, m_lbChannels);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDynChannelList, CDialog)
	//{{AFX_MSG_MAP(CDynChannelList)
	ON_BN_CLICKED(IDC_JOINCHANNEL, OnJoinchannel)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynChannelList message handlers

void CDynChannelList::OnJoinchannel() 
{
	int l_iSel;
	CString l_JoinChan,l_Cmd,l_Line;
	l_iSel = m_lbChannels.GetCurSel();
	if(l_iSel!=LB_ERR)
	{
		m_lbChannels.GetText(l_iSel,l_Line);
		l_JoinChan = l_Line.SpanExcluding("\t");
		l_Cmd.Format("/join %s",l_JoinChan);
		((CPconwApp*)AfxGetApp())->SendMsg(l_Cmd);
	}
	
}

void CDynChannelList::OnDblclkList1() 
{
	OnJoinchannel();
	
}

void CDynChannelList::OnOK() 
{
	//Just hide the window!
	ShowWindow(SW_HIDE);
	//CDialog::OnOK();
	
}

void CDynChannelList::OnCancel() 
{
	//Just hide the window!
	ShowWindow(SW_HIDE);
	//CDialog::OnCancel();
}

void CDynChannelList::RefreshList(CStringList & slChannels)
{
	CString l_Line;
	CString l_Channel,l_NumUsers,l_Topic;
	CString l_FormatItem;
	POSITION pos;
	m_lbChannels.ResetContent();
	pos=slChannels.GetHeadPosition();
	while(pos)
	{
		l_Line=slChannels.GetNext(pos);
		l_Channel = GetWordNum(1,l_Line);
		l_NumUsers = GetWordNum(2,l_Line);
		l_Topic = GetWordNum(3,l_Line);
		l_FormatItem.Format("%s\t%s\t%s",l_Channel,l_NumUsers,l_Topic);
		m_lbChannels.AddString(l_FormatItem);
	}
//CListBox
}


char * CDynChannelList::GetWordNum(int num, CString & l_String)
{
	char seps[20] = " \n\r\t";
	
	char *ptokstr,*token,*strstart;
	static char strreturn[300];
	ptokstr = new char[l_String.GetLength()+2];
	strstart = ptokstr;
	
	ASSERT(ptokstr);
	
	strcpy(ptokstr,LPCSTR(l_String));
	
	token=strtok(ptokstr,seps);

	for(int i=0;i!=num;i++)
	{
		token=strtok(NULL,seps);
	}
	//l_tmpstrB should now have what we want.
	if(token)
	{
		strcpy(strreturn,token);
	}
	else
	{
		return "";
	}
	//check for the ':' char....
	if(token[0]==':')
	{
		//Its not pretty, but it works, return the rest of the string
		strcpy(strreturn,LPCSTR(l_String)+((token-strstart)+1));
	}
	
	delete [] ptokstr;
	//return the appropriate response.
	return strreturn;
}

BOOL CDynChannelList::OnInitDialog() 
{
	int l_TabStops[2];
	l_TabStops[0] = 48;
	l_TabStops[1] = 72;
	CDialog::OnInitDialog();
	
	m_lbChannels.SetTabStops(2,(LPINT) &l_TabStops[0]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
