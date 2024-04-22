// GameView.cpp : implementation file
//

#include "stdafx.h"
//#include "dwatch.h"
#include "GameView.h"
//#include "dwatchDoc.h"
//#include "DspyFrm.h"
//#include "KaliRunning.h"
/////////////////////////////////////////////////////////////////////////////
// CGameView

IMPLEMENT_DYNCREATE(CGameView, CFormView)

CGameView::CGameView()
	: CFormView(CGameView::IDD)
{
	
	m_RefreshTimer = 0;
	m_CountToRefresh = 0;
	m_MyNick="Dspy";
	TimeVal.tv_sec=0;
	TimeVal.tv_usec=0;
	m_GameVerD2 = TRUE;
	
	if(WSAStartup(0x0101,&m_wsadata)!=0)
	{
		AfxMessageBox("Unable to initialize Winsock");
		PostQuitMessage(-1);
		return;
	}
	//Create socket
	m_WSock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX);
	if(m_WSock==INVALID_SOCKET)
	{
		AfxMessageBox("Unable to initialize Winsock IPX");
		PostQuitMessage(-1);
		return;
	}
	//m_MyIPXAddr

	m_RealSocket = 0x5130;

	memset(&m_MyIPXAddr,0,sizeof(m_MyIPXAddr));
	
	//if(!m_pSocket->Create(0x08AA, SOCK_DGRAM, FD_READ, "127.0.0.1"))


	m_MyIPXAddr.sa_socket = htons(m_RealSocket);
	m_MyIPXAddr.sa_family = AF_IPX;
	if (bind(m_WSock,(struct sockaddr *)&m_MyIPXAddr,sizeof(m_MyIPXAddr)) != 0)

        {
          AfxMessageBox("Unable to bind socket");
		  PostQuitMessage(-1);
          return;
        }
	
	sockaddr_ipx l_IPXAddr;  //_DATA
	int cbOpt = sizeof(l_IPXAddr);
	//l_IPXAddr.adapternum = 0;
	//getsockopt(m_WSock,NSPROTO_IPX,IPX_ADDRESS,(char *)&l_IPXAddr,&cbOpt);
	int bcast = TRUE;
	setsockopt (m_WSock, SOL_SOCKET, SO_BROADCAST, (char *) &bcast, sizeof(bcast) );
//A fatal exception 0E has occured at 0028:C2F894AA in VXD Kahn (01) + 00003126
	//
	getsockname(m_WSock,(sockaddr *)&l_IPXAddr,&cbOpt);
	//CString l_tmpstr;
	//l_tmpstr.Format("%x.%x.%x.%x.%x.%x",l_IPXAddr.sa_nodenum[5],l_IPXAddr.sa_nodenum[4],l_IPXAddr.sa_nodenum[3],l_IPXAddr.sa_nodenum[2],l_IPXAddr.sa_nodenum[1],l_IPXAddr.sa_nodenum[0]);
	//AfxMessageBox(l_tmpstr);
	
	m_Seq.type = PID_GAME_LIST;
	m_Seq.unknownpad=0x12;
	m_Seq.Security=0x73a6;
	m_Seq.player.version_major	= 0x86;
	m_Seq.player.version_minor=0x33;
	m_Seq.player.comp_type=0x10;
	m_Seq.player.connected=0x05;
	m_Seq.player.socket=0x00;
	m_Seq.player.rank=0x00;
	
	int l_iLoop;
	for(l_iLoop=0;l_iLoop<4;l_iLoop++) m_MyIPXAddr.sa_netnum[l_iLoop] = l_IPXAddr.sa_netnum[l_iLoop];
	for(l_iLoop=0;l_iLoop<6;l_iLoop++) m_MyIPXAddr.sa_nodenum[l_iLoop] = l_IPXAddr.sa_nodenum[l_iLoop];
	
	for(l_iLoop=0;l_iLoop<4;l_iLoop++) m_Seq.player.server[l_iLoop] = m_MyIPXAddr.sa_netnum[l_iLoop];
	for(l_iLoop=0;l_iLoop<6;l_iLoop++) m_Seq.player.node[l_iLoop] = m_MyIPXAddr.sa_nodenum[l_iLoop];
	strcpy(m_Seq.player.callsign,LPCSTR(m_MyNick));
	
	for(l_iLoop=0;l_iLoop<4;l_iLoop++) m_BcastIPX.sa_netnum[l_iLoop] = -1;
	for(l_iLoop=0;l_iLoop<6;l_iLoop++) m_BcastIPX.sa_nodenum[l_iLoop] = -1;
	m_BcastIPX.sa_family = AF_IPX;
	
	for(l_iLoop=0;l_iLoop<4;l_iLoop++) m_D1Seq.player.server[l_iLoop] = m_MyIPXAddr.sa_netnum[l_iLoop];
	for(l_iLoop=0;l_iLoop<6;l_iLoop++) m_D1Seq.player.node[l_iLoop] = m_MyIPXAddr.sa_nodenum[l_iLoop];
	
//	if(((l_IPXAddr.sa_nodenum[5]&0x000000ff)==0x08)&&((l_IPXAddr.sa_nodenum[4]&0x000000ff)==0xa5))
//	{
//		CKaliRunning l_KaliRun;
//		l_KaliRun.DoModal();
//	}
	
	m_D1Seq.player.socket = 0x5100; 
	strcpy(m_D1Seq.player.callsign,"$");
	strcat(m_D1Seq.player.callsign,LPCSTR(m_MyNick));
	m_D1Seq.type=36;
	
	//{{AFX_DATA_INIT(CGameView)
	m_iSocket = 0;
	m_strSelPlayer = _T("");
	//}}AFX_DATA_INIT
}

CGameView::~CGameView()
{
	closesocket(m_WSock);
	WSACleanup();	
/*	POSITION pos;
	pos=m_D2Games.GetHeadPosition();
	while(pos)
	{
		l_Game = (D2Game *)m_D2Games.GetNext(pos);//m_D2Games.GetAt(pos);
		delete(l_Game);
				
	}
	m_D2Games.RemoveAll();
	pos=m_D1Games.GetHeadPosition();
	while(pos)
	{
		l1_Game = (D1Game *)m_D1Games.GetNext(pos);//m_D2Games.GetAt(pos);
		delete(l1_Game);
				
	}
	m_D1Games.RemoveAll();
*/
}

void CGameView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameView)
	DDX_Control(pDX, IDC_ADDRESS, m_ecAddress);
	DDX_Control(pDX, IDC_CHECK1, m_bShort);
	DDX_Control(pDX, IDC_PPS, m_ecPPS);
	DDX_Control(pDX, IDC_PLAYERLIST, m_ecPlayerName);
	DDX_Control(pDX, IDC_MODE, m_ecMode);
	DDX_Control(pDX, IDC_RANK, m_ecRank);
	DDX_Control(pDX, IDC_SCORE, m_ecScore);
	DDX_Control(pDX, IDC_MISSION, m_ecMission);
	DDX_Control(pDX, IDC_MAXPLAYERS, m_ecMaxPlayers);
	DDX_Control(pDX, IDC_LEVEL, m_ecLevel);
	DDX_Control(pDX, IDC_KILLS2, m_ecKills2);
	DDX_Control(pDX, IDC_GAMENAME, m_ecGameName);
	DDX_Control(pDX, IDC_KILLS, m_ecKills);
	DDX_Control(pDX, IDC_DIFFICULTY, m_exDifficulty);
	DDX_Control(pDX, IDC_CURNUMPLAYERS, m_ecCurPlyrs);
	DDX_Text(pDX, IDC_SOCKET, m_iSocket);
	DDV_MinMaxInt(pDX, m_iSocket, -100, 100);
	DDX_LBString(pDX, IDC_PLAYERLIST, m_strSelPlayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameView, CFormView)
	//{{AFX_MSG_MAP(CGameView)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SOCKETSPIN, OnDeltaposSocketspin)
	ON_EN_CHANGE(IDC_SOCKET, OnChangeSocket)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DII, OnDii)
	ON_BN_CLICKED(IDC_DI, OnDi)
	ON_LBN_SELCHANGE(IDC_GAMELIST, OnSelchangeGamelist)
	ON_LBN_SELCHANGE(IDC_PLAYERLIST, OnSelchangePlayerlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//	ON_BN_CLICKED(IDC_RADIPX, OnRadipx)
//	ON_BN_CLICKED(IDC_RADIP, OnRadip)

/////////////////////////////////////////////////////////////////////////////
// CGameView diagnostics

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGameView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGameView message handlers

void CGameView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// Set the DII box to be checked. IDC_DI
	//CCheckbox
	CButton *l_D2Button;
	l_D2Button = (CButton *)GetDlgItem(IDC_DII);
	if(l_D2Button) l_D2Button->SetCheck(1);
	SetDlgItemText(IDC_SOCKET,"0");
	//Establish Socket & send initial req.
	m_RefreshTimer = SetTimer(0x147,100,NULL);
	SendD2Seq();
	
}

void CGameView::OnRefresh() 
{
	//m_GameVerD2 TRUE if D2
	if(m_GameVerD2==TRUE)
		SendD2Seq();
	else
		SendD1Seq();
	
}

void CGameView::OnDeltaposSocketspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(FALSE == UpdateData(TRUE)) 
	{
		return;
	}
	m_iSocket -= pNMUpDown->iDelta;
	CString l_strSock;
	l_strSock.Format("%d",m_iSocket);
	SetDlgItemText(IDC_SOCKET,l_strSock);
	*pResult = 0;
}

void CGameView::OnChangeSocket() 
{
	//Clear out the current games...
	POSITION pos;
	D2Game * l_Game;

	if(m_GameVerD2==TRUE)
	{
		pos=m_D2Games.GetHeadPosition();
		while(pos)
		{
			l_Game = (D2Game *)m_D2Games.GetNext(pos);//m_D2Games.GetAt(pos);
			delete(l_Game);
					
		}
		m_D2Games.RemoveAll();

		CListBox *l_GameListBox;
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			l_GameListBox->ResetContent();
		}
		// Recalc the socket based on which ver. we are looking at
		if(FALSE == UpdateData(TRUE)) 
		{
			return;
		}
		//Close the current socket
		closesocket(m_WSock);
		//Reopen the socket
		m_WSock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX);
		if(m_WSock==INVALID_SOCKET)
		{
			AfxMessageBox("Unable to initialize Winsock IPX");
			PostQuitMessage(-1);
			return;
		}
		//Figure out the new socket....
		m_RealSocket = 0x5130 + m_iSocket;
		m_MyIPXAddr.sa_socket = htons(m_RealSocket);
		if (bind(m_WSock,(struct sockaddr *)&m_MyIPXAddr,sizeof(m_MyIPXAddr)) != 0)
			{
			  AfxMessageBox("Unable to bind socket");
			  PostQuitMessage(-1);
			  return;
			}
		int bcast = TRUE;
		setsockopt (m_WSock, SOL_SOCKET, SO_BROADCAST, (char *) &bcast, sizeof(bcast) );
		SendD2Seq();
	}	
	else
	{
		D1Game * l1_Game;
		pos=m_D1Games.GetHeadPosition();
		while(pos)
		{
			l1_Game = (D1Game *)m_D1Games.GetNext(pos);//m_D2Games.GetAt(pos);
			delete(l1_Game);
					
		}
		m_D1Games.RemoveAll();

		CListBox *l_GameListBox;
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			l_GameListBox->ResetContent();
		}
		// Recalc the socket based on which ver. we are looking at
		if(FALSE == UpdateData(TRUE)) 
		{
			return;
		}
		//Close the current socket
		closesocket(m_WSock);
		//Reopen the socket
		m_WSock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX);
		if(m_WSock==INVALID_SOCKET)
		{
			AfxMessageBox("Unable to initialize Winsock IPX");
			PostQuitMessage(-1);
			return;
		}
		//Figure out the new socket....
		m_RealSocket = 0x5100 + m_iSocket;
		m_MyIPXAddr.sa_socket = htons(m_RealSocket);
		if (bind(m_WSock,(struct sockaddr *)&m_MyIPXAddr,sizeof(m_MyIPXAddr)) != 0)
			{
			  AfxMessageBox("Unable to bind socket");
			  PostQuitMessage(-1);
			  return;
			}
		int bcast = TRUE;
		setsockopt (m_WSock, SOL_SOCKET, SO_BROADCAST, (char *) &bcast, sizeof(bcast) );
		SendD1Seq();
	}
}

void CGameView::OnRadipx() 
{
	// TODO: Add your control notification handler code here
	
}

void CGameView::OnRadip() 
{
	// TODO: Add your control notification handler code here
	
}

void CGameView::OnTimer(UINT nIDEvent) 
{
	if(m_GameVerD2==TRUE)
	{
		char blah[200];
		int iTemp=0;
		int iSelect=1;
		fd_set SocketList;
		FD_ZERO(&SocketList);
		FD_SET(m_WSock, &SocketList);

		m_CountToRefresh++;
		if(m_CountToRefresh>=200)
		{
			SendD2Seq();
			m_CountToRefresh=0;
		}
		
		while((iSelect!=-1) && (iSelect!=0))
		{
			//Look for incoming data, and handle it.
			iSelect = select(0, &SocketList, (fd_set *)NULL, (fd_set *)NULL, &TimeVal);
			
			if(iSelect>0)
			{	
				
				memset(&m_CurrGameInfo,0,sizeof(m_CurrGameInfo));
				
				iTemp=sizeof(m_RcvIPXAddr);
				
				//need to define receive buffer and size.
				//also need to have buffer for received address.
				int iInSize = recvfrom(m_WSock, (char *)&m_CurrGameInfo, sizeof(m_CurrGameInfo), 0, (struct sockaddr *)&m_RcvIPXAddr, &iTemp);
				if(iInSize<=0) 
				{
				//	MessageBox(itoa(iInSize,blah,10));
					return;
				}
				//Parse the packet, depending on what it is....
				switch(m_CurrGameInfo.type)
				{
				case LITE_INFO_TYPE:
					//Use this to keep a game listbox
					AddD2Game();
					break;
				case PLAYERSINFO:
					//Match with full game info
					AddAllD2Players();
					break;
				case PID_GAME_LIST:
					//This is a request for game info, just catch their user info
					//m_IncomingSeq = (sequence_packet*)&m_CurrGameInfo;
					break;
				case BIG_GAME_INFO:
					//Full game info, update dialog box.
					//allocate new structure...
					FillD2Game();
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		int iTemp;
		int iSelect=1;
		fd_set SocketList;
		FD_ZERO(&SocketList);
		FD_SET(m_WSock, &SocketList);

		m_CountToRefresh++;
		if(m_CountToRefresh>=200)
		{
			SendD1Seq();
			m_CountToRefresh=0;
		}
		
		while((iSelect!=-1) && (iSelect!=0))
		{
			//Look for incoming data, and handle it.
			iSelect = select(0, &SocketList, (fd_set *)NULL, (fd_set *)NULL, &TimeVal);
			
			if(iSelect>0)
			{	
				memset(&m_D1CurrGameInfo,0,sizeof(m_D1CurrGameInfo));
				//need to define receive buffer and size.
				//also need to have buffer for received address.
				iTemp=sizeof(m_RcvIPXAddr);
				int iInSize = recvfrom(m_WSock, (char *)&m_D1CurrGameInfo, sizeof(m_D1CurrGameInfo), 0, (struct sockaddr *)&m_RcvIPXAddr, &iTemp);
				if(iInSize<=0) return;
				//Parse the packet, depending on what it is....
				switch(m_D1CurrGameInfo.packet_type)
				{
				case '%':
					//Use this to keep a game listbox
					//With D1, we have all the info we need in this packet.
					AddD1Game();
					break;
				case '$':
					//This is a request for game info, just catch their user info
					//m_IncomingSeq = (sequence_packet*)&m_CurrGameInfo;
					break;
				}
			}
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CGameView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnClose();
}

void CGameView::OnDestroy() 
{
	if(m_RefreshTimer) KillTimer(m_RefreshTimer);
	CFormView::OnDestroy();
}

void CGameView::OnDii() 
{
	m_GameVerD2=TRUE;
	OnChangeSocket();	
}

void CGameView::OnDi() 
{
	//Set the socket for D1
	m_GameVerD2=FALSE;
	OnChangeSocket();
}


//Finds the game names GameName, returns pointer to is, or NULL if not found.
D2Game * CGameView::FindD2Game(char * GameName)
{
	POSITION pos;
	D2Game * l_Game;

	pos=m_D2Games.GetHeadPosition();
	while(pos)
	{
		l_Game = (D2Game *)m_D2Games.GetNext(pos);//m_D2Games.GetAt(pos);
		if(stricmp(l_Game->Game.game_name,GameName)==0)
		{
			return l_Game;
		}
		
	}
	return NULL;
}

void CGameView::AddD2Game()
{
	POSITION pos;
	D2Game * l_Game;

	pos=m_D2Games.GetHeadPosition();
	while(pos)
	{
		l_Game = (D2Game *)m_D2Games.GetNext(pos);
		if(stricmp(l_Game->Game.game_name,m_CurrGameInfo.game_name)==0)
		{
			//We already have this game name.
			memcpy(&l_Game->Master,&m_RcvIPXAddr,sizeof(SOCKADDR_IPX));
			memcpy(&l_Game->Game,&m_CurrGameInfo,sizeof(netgame_info));
			l_Game->KeepAlive=2;
			l_Game->FullGameInfo = FALSE;
			return;
		}
		
	}
	m_D2Ptr = new D2Game;
	m_D2Games.AddTail(m_D2Ptr);
	memcpy(&m_D2Ptr->Master,&m_RcvIPXAddr,sizeof(SOCKADDR_IPX));
	memcpy(&m_D2Ptr->Game,&m_CurrGameInfo,sizeof(netgame_info));
	m_D2Ptr->KeepAlive=2;
	m_D2Ptr->FullGameInfo = FALSE;
	
	CListBox *l_GameListBox;
	l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
	if(l_GameListBox)
	{
		l_GameListBox->AddString(m_CurrGameInfo.game_name);
	}

}

void CGameView::OnSelchangeGamelist() 
{
	char l_blah[200];
	if(m_GameVerD2==TRUE)
	{
		//Send a req. to get the full game info to the game owner.
		//get the selection text....
		CListBox *l_GameListBox;
		D2Game * l_Game;
		char l_szGameName[NETGAME_NAME_LEN+2];
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}
		l_Game = FindD2Game(l_szGameName);
		ASSERT(l_Game);
		if(l_Game==NULL) 
			return;
		//send seq. to that address.....
		m_BcastIPX.sa_socket=htons(m_RealSocket);
		m_Seq.type = GET_BIG_GAME_INFO;
		//m_Seq.unknownpad++;
		m_Seq.Security=l_Game->Game.Security;

		int l_rc = sendto(m_WSock,(char *)&m_Seq,sizeof(m_Seq),0,(const struct sockaddr *)&l_Game->Master,sizeof(SOCKADDR_IPX));
		m_ecGameName.SetWindowText("Waiting....");
		m_ecMission.SetWindowText("");
		m_ecCurPlyrs.SetWindowText("");
		m_ecMaxPlayers.SetWindowText("");
		m_ecLevel.SetWindowText("");
		m_exDifficulty.SetWindowText("");
		m_ecMode.SetWindowText("");
		m_ecPPS.SetWindowText("");
		m_bShort.SetCheck(FALSE);
		m_ecPlayerName.ResetContent();
		m_ecScore.SetWindowText("");
		m_ecKills.SetWindowText("");
		m_ecKills2.SetWindowText("");
		m_ecAddress.SetWindowText("");
		m_ecRank.SetWindowText("");
	}
	else
	{
		
		//get the selection text....
		//then display the info
		CListBox *l_GameListBox;
		D1Game * l_Game;
		char l_szGameName[NETGAME_NAME_LEN+2];
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}
		l_Game = FindD1Game(l_szGameName);
		ASSERT(l_Game);
		if(l_Game==NULL) 
			return;
		
	//Fill in the controls in the view
	m_ecGameName.SetWindowText(l_Game->Game.game_name);
	m_ecMission.SetWindowText(l_Game->Game.mission_title);
	m_ecCurPlyrs.SetWindowText(itoa(l_Game->Game.numplayers,l_blah,10));
	m_ecMaxPlayers.SetWindowText(itoa(l_Game->Game.max_numplayers,l_blah,10));
	m_ecLevel.SetWindowText(itoa(l_Game->Game.levelnum,l_blah,10));
	switch(l_Game->Game.difficulty)
	{
		case 1:
			strcpy(l_blah,"Trainee");
			break;
		case 2:
			strcpy(l_blah,"Rookie");
			break;
		case 3:
			strcpy(l_blah,"Hotshot");
			break;
		case 4:
			strcpy(l_blah,"Ace");
			break;
		case 5:
			strcpy(l_blah,"Insane");
			break;
		default:
			strcpy(l_blah,"Unknown");	

	}
	m_exDifficulty.SetWindowText(l_blah);
	switch(l_Game->Game.gamemode)
	{
	case NETGAME_ANARCHY:
		strcpy(l_blah,"Anarchy");
		break;
	case GAME_TEAM_ANARCHY:
		strcpy(l_blah,"Team Anarchy");
		break;
	case GAME_ROBOT_ANARCHY:
		strcpy(l_blah,"Robot Anarchy");
		break;
	case GAME_COOPERATIVE:
		strcpy(l_blah,"Coop");
		break;
	default:
		strcpy(l_blah,"Unknown");	

	}
	if(l_Game->Game.game_flags&NETGAME_FLAG_CLOSED) strcat(l_blah," (Closed)");
	m_ecMode.SetWindowText(l_blah);
	m_ecPPS.SetWindowText("N/A");
	
	m_bShort.SetCheck(FALSE);

	//Now fill in the players.....

	m_ecPlayerName.ResetContent();
		
	
	for(int iLoop=0;iLoop<8;iLoop++)
	{
		if(l_Game->Game.players[iLoop].callsign[0]!=0)
			m_ecPlayerName.AddString(l_Game->Game.players[iLoop].callsign);
	}
	m_ecPlayerName.SetCurSel(0);
	OnSelchangePlayerlist();
	}
}

void CGameView::SendD2Seq()
{
	m_Seq.type = PID_GAME_LIST;
	m_BcastIPX.sa_socket=htons(m_RealSocket);
	int l_rc = sendto(m_WSock,(char *)&m_Seq,sizeof(m_Seq),0,(const struct sockaddr *)&m_BcastIPX,sizeof(SOCKADDR_IPX));
	if(l_rc==-1) 
	{
		l_rc = WSAGetLastError();

	}
}//10047

void CGameView::FillD2Game()
{
	char l_blah[200];
	if(m_Seq.Security != m_CurrGameInfo.Security) return;
	//take the new packet, fill in the structures,
	D2Game *l_Game;
	
	l_Game = FindD2Game(m_CurrGameInfo.game_name);
	ASSERT(l_Game);	
	if(l_Game==NULL)
		return;
	memcpy(&l_Game->Game,&m_CurrGameInfo,sizeof(netgame_info));
	

	//Fill in the controls in the view
	m_ecGameName.SetWindowText(m_CurrGameInfo.game_name);
	m_ecMission.SetWindowText(m_CurrGameInfo.mission_title);
	m_ecCurPlyrs.SetWindowText(itoa(m_CurrGameInfo.numplayers,l_blah,10));
	m_ecMaxPlayers.SetWindowText(itoa(m_CurrGameInfo.max_numplayers,l_blah,10));
	m_ecLevel.SetWindowText(itoa(m_CurrGameInfo.levelnum,l_blah,10));
	switch(m_CurrGameInfo.difficulty)
	{
	case 1:
		strcpy(l_blah,"Trainee");
		break;
	case 2:
		strcpy(l_blah,"Rookie");
		break;
	case 3:
		strcpy(l_blah,"Hotshot");
		break;
	case 4:
		strcpy(l_blah,"Ace");
		break;
	case 5:
		strcpy(l_blah,"Insane");
		break;
	default:
		strcpy(l_blah,"Unknown");	

	}
	m_exDifficulty.SetWindowText(l_blah);
	switch(m_CurrGameInfo.game_flags)
	{
	case NETGAME_ANARCHY:
		strcpy(l_blah,"Anarchy");
		break;
	case GAME_TEAM_ANARCHY:
		strcpy(l_blah,"Team Anarchy");
		break;
	case GAME_ROBOT_ANARCHY:
		strcpy(l_blah,"Robot Anarchy");
		break;
	case GAME_COOPERATIVE:
		strcpy(l_blah,"Coop");
		break;
	case GAME_CAPTURE_FLAG:
		strcpy(l_blah,"Capture the Flag");
		break;
	case GAME_HOARD:
		strcpy(l_blah,"Hoard");
		break;
	case GAME_TEAM_HOARD:
		strcpy(l_blah,"Team Hoard");
		break;
	case NETGAME_FLAG_HOARD:
		strcpy(l_blah,"Flag Hoard");
		break;
	case NETGAME_FLAG_TEAM_HOARD:
		strcpy(l_blah,"Flag Team Hoard");
		break;

	default:
		strcpy(l_blah,"Unknown");	

	}
	if(m_CurrGameInfo.RefusePlayers) strcat(l_blah," (Restricted)");
	m_ecMode.SetWindowText(l_blah);
	m_ecPPS.SetWindowText(itoa(m_CurrGameInfo.PacketsPerSec,l_blah,10));
	if(m_CurrGameInfo.ShortPackets)
		m_bShort.SetCheck(TRUE);
	else 
		m_bShort.SetCheck(FALSE);
	
}

void CGameView::AddAllD2Players()
{
	if(m_Seq.Security != m_CurrGameInfo.Security) return;
	m_ecPlayerName.ResetContent();
	memcpy(&m_Players,&m_CurrGameInfo,sizeof(m_Players));
	
	
	for(int iLoop=0;iLoop<12;iLoop++)
	{
		if(m_Players.players[iLoop].callsign[0]!=0)
			m_ecPlayerName.AddString(m_Players.players[iLoop].callsign);
	}
	m_ecPlayerName.SetCurSel(0);
	OnSelchangePlayerlist();
}

void CGameView::OnSelchangePlayerlist() 
{
	if(m_GameVerD2==TRUE)
	{
		//Scan through the list, and fill in the rest of the data.
		//m_strSelPlayer
		CString l_tmpstr;
		char l_Blah[200];
		UpdateData(TRUE);
		CListBox *l_GameListBox;
		char l_szGameName[NETGAME_NAME_LEN+2];
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}	
		D2Game *l_Game;
		l_Game = FindD2Game(l_szGameName);
		ASSERT(l_Game);	
		
		l_GameListBox = (CListBox *)GetDlgItem(IDC_PLAYERLIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}
		


		for(int iLoop=0;iLoop<8;iLoop++)
		{
			if(strcmp(m_Players.players[iLoop].callsign,LPCSTR(m_strSelPlayer))==0)
			{
				//Fill in all the stats.
				switch(m_Players.players[iLoop].rank)
				{
	

				case 0:
					l_tmpstr = "Unpatched";
					break;
				case 1:
					l_tmpstr = "Cadet";		
					break;
				case 2:
					l_tmpstr = "Ensign";
					break;
				case 3:
					l_tmpstr = "Lieutenant";
					break;
				case 4:
					l_tmpstr = "Lt. Commander";
					break;
				case 5:
					l_tmpstr = "Commander";
					break;
				case 6:
					l_tmpstr = "Captain";
					break;
				case 7:
					l_tmpstr = "Rear Admiral";
					break;
				case 8:
					l_tmpstr = "Admiral";
					break;
				case 9:
					l_tmpstr = "Demigod";
					break;
				default: 
					l_tmpstr = "Unknown";
				}
				m_ecRank.SetWindowText(l_tmpstr);

				m_ecScore.SetWindowText(itoa(l_Game->Game.player_score[iLoop],l_Blah,10));
				m_ecKills.SetWindowText(itoa(l_Game->Game.player_kills[iLoop],l_Blah,10));
				m_ecKills2.SetWindowText(itoa(l_Game->Game.killed[iLoop],l_Blah,10));
	//IP Address
				l_tmpstr.Format("%.1u.%.1u.%.1u.%.1u",m_Players.players[iLoop].node[3],m_Players.players[iLoop].node[2],m_Players.players[iLoop].node[1],m_Players.players[iLoop].node[0]);
				m_ecAddress.SetWindowText(l_tmpstr);
				

				return;
			}
		}
	}
	else
	{
	//Scan through the list, and fill in the rest of the data.
		//m_strSelPlayer
		CString l_tmpstr;
		char l_Blah[200];
		UpdateData(TRUE);
		CListBox *l_GameListBox;
		char l_szGameName[NETGAME_NAME_LEN+2];
		l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}	
		D1Game *l_Game;
		l_Game = FindD1Game(l_szGameName);
		ASSERT(l_Game);	
		if(l_Game==NULL) 
			return;
		l_GameListBox = (CListBox *)GetDlgItem(IDC_PLAYERLIST);
		if(l_GameListBox)
		{
			int l_lbSel = l_GameListBox->GetCurSel();
			if(l_lbSel != LB_ERR)
			{
				l_GameListBox->GetText(l_lbSel,l_szGameName);
			}
			else return;
		}
		


		for(int iLoop=0;iLoop<8;iLoop++)
		{
			if(strcmp(l_Game->Game.players[iLoop].callsign,LPCSTR(m_strSelPlayer))==0)
			{
				//Fill in all the stats.
				m_ecRank.SetWindowText("N/A");

				m_ecScore.SetWindowText(itoa(l_Game->Game.player_score[iLoop],l_Blah,10));
				m_ecKills.SetWindowText(itoa(l_Game->Game.kills1[iLoop],l_Blah,10));
				m_ecKills2.SetWindowText(itoa(l_Game->Game.killed[iLoop],l_Blah,10));
	//IP Address
				l_tmpstr.Format("%.1u.%.1u.%.1u.%.1u",m_Players.players[iLoop].node[3],m_Players.players[iLoop].node[2],m_Players.players[iLoop].node[1],m_Players.players[iLoop].node[0]);
				m_ecAddress.SetWindowText(l_tmpstr);
				
				return;
			}
		}

	}
}


void CGameView::SendD1Seq()
{
	
	m_BcastIPX.sa_socket=htons(m_RealSocket);
	int l_rc = sendto(m_WSock,(char *)&m_D1Seq,sizeof(m_Seq),0,(const struct sockaddr *)&m_BcastIPX,sizeof(SOCKADDR_IPX));
	if(l_rc==-1) 
	{
		l_rc = WSAGetLastError();

	}
}

void CGameView::AddD1Game()
{
	POSITION pos;
	D1Game * l_Game;

	pos=m_D1Games.GetHeadPosition();
	while(pos)
	{
		l_Game = (D1Game *)m_D1Games.GetNext(pos);
		if(stricmp(l_Game->Game.game_name,m_D1CurrGameInfo.game_name)==0)
		{
			//We already have this game name.
			memcpy(&l_Game->Master,&m_RcvIPXAddr,sizeof(SOCKADDR_IPX));
			memcpy(&l_Game->Game,&m_D1CurrGameInfo,sizeof(D1_netgame_info));
			l_Game->KeepAlive=2;
			l_Game->FullGameInfo = FALSE;
			return;
		}
		
	}
	m_D1Ptr = new D1Game;
	m_D1Games.AddTail(m_D1Ptr);
	memcpy(&m_D1Ptr->Master,&m_RcvIPXAddr,sizeof(SOCKADDR_IPX));
	memcpy(&m_D1Ptr->Game,&m_D1CurrGameInfo,sizeof(D1_netgame_info));
	m_D1Ptr->KeepAlive=2;
	m_D1Ptr->FullGameInfo = FALSE;
	
	CListBox *l_GameListBox;
	l_GameListBox = (CListBox *)GetDlgItem(IDC_GAMELIST);
	if(l_GameListBox)
	{
		l_GameListBox->AddString(m_D1CurrGameInfo.game_name);
	}

}

D1Game * CGameView::FindD1Game(char * GameName)
{
	POSITION pos;
	D1Game * l_Game;

	pos=m_D1Games.GetHeadPosition();
	while(pos)
	{
		l_Game = (D1Game *)m_D1Games.GetNext(pos);
		if(stricmp(l_Game->Game.game_name,GameName)==0)
		{
			return l_Game;
		}
		
	}
	return NULL;
}
