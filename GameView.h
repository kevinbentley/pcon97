#if !defined(AFX_GAMEVIEW_H__98591C01_CC49_11D0_883A_3C8B00C10000__INCLUDED_)
#define AFX_GAMEVIEW_H__98591C01_CC49_11D0_883A_3C8B00C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GameView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef _d2pack_header_
#define _d2pack_header_

#pragma pack(1)
#define LITE_INFO_TYPE 43	//2b
#define GET_BIG_GAME_INFO 44//2c
#define PLAYERSINFO 45		//2d
#define PID_GAME_LIST 56	//38
#define BIG_GAME_INFO 57	//39 - The big game info

#define NETGAME_NAME_LEN 15
#define MISSION_NAME_LEN 25
#define CALLSIGN_LEN 8
#define MAX_PLAYERS 8

#define NETGAME_ANARCHY 0
#define GAME_TEAM_ANARCHY 1
#define GAME_ROBOT_ANARCHY 2
#define GAME_COOPERATIVE 3
#define GAME_CAPTURE_FLAG 4
#define GAME_HOARD 5
#define GAME_TEAM_HOARD 6

#define NETGAME_FLAG_HOARD 8
#define NETGAME_FLAG_TEAM_HOARD 16
#define NETGAME_FLAG_REALLY_ENDLEVEL 32
#define NETGAME_FLAG_REALLY_FORMING 64

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned long fix;

typedef struct lite_info 
{
	int unknownpad;
	
	ubyte type; //always 43
	int Security;
	char game_name[NETGAME_NAME_LEN+1];
	char mission_title[MISSION_NAME_LEN+1];
	char mission_name[9];
	int levelnum;
	ubyte gamemode;
	ubyte RefusePlayers;
	ubyte difficulty;
	ubyte game_status;
	ubyte numplayers;
	ubyte max_numplayers;
	ubyte numconnected;
	ubyte game_flags;
	ubyte protocol_version;
	ubyte version_major;
	ubyte version_minor;
	ubyte team_vector;
} lite_info;

typedef struct netplayer_info 
{
	char callsign[CALLSIGN_LEN+1];
	ubyte server[4];
	ubyte node[6];
	ubyte version_major;
	ubyte version_minor;
	ubyte comp_type;// enum comp_type computer_type;
	byte connected;
	ushort socket;
	ubyte rank;

} netplayer_info;
typedef struct AllNetPlayers_info
{
	int unknownpad;
	
	char type; // always 45
	int Security;
	struct netplayer_info players[MAX_PLAYERS+4];
} AllNetPlayers_info;


typedef struct netgame_info 
{
	int unknownpad;
	ubyte type; //always 57
	int Security;
	char game_name[NETGAME_NAME_LEN+1];
	char mission_title[MISSION_NAME_LEN+1];
	char mission_name[9];
	int levelnum;
	ubyte gamemode; // See the GAME_ defines above
	ubyte RefusePlayers;
	ubyte difficulty;
	ubyte game_status;
	ubyte numplayers;
	ubyte max_numplayers;
	ubyte numconnected;
	ubyte game_flags;
	ubyte protocol_version;
	ubyte version_major;
	ubyte version_minor;
	ubyte team_vector; // bit=1 if red

	short DoMegas:1;
	short DoSmarts:1;
	short DoFusions:1;
	short DoHelix:1;
	short DoPhoenix:1;
	short DoAfterburner:1;
	short DoInvulnerability:1;
	short DoCloak:1;
	short DoGauss:1;
	short DoVulcan:1;
	short DoPlasma:1;
	short DoOmega:1;
	short DoSuperLaser:1;
	short DoProximity:1;
	short DoSpread:1;
	short DoSmartMine:1;
	short DoFlash:1;
	short DoGuided:1;
	short DoEarthShaker:1;
	short DoMercury:1;
	short Allow_marker_view:1;
	short AlwaysLighting:1; 
	short DoAmmoRack:1;
	short DoConverter:1;
	short DoHeadlight:1;
	short DoHoming:1;
	short DoLaserUpgrade:1;
	short DoQuadLasers:1;
	short ShowAllNames:1;
	short BrightPlayers:1;

	char team_name[2][CALLSIGN_LEN+1];
	int locations[MAX_PLAYERS];
	short kills[MAX_PLAYERS][MAX_PLAYERS];
	ushort segments_checksum;
	short team_kills[2];
	short killed[MAX_PLAYERS];
	short player_kills[MAX_PLAYERS];
	int KillGoal;
	fix PlayTimeAllowed;
	fix level_time;
	int control_invul_time;
	int monitor_vector;
	int player_score[MAX_PLAYERS];
	ubyte player_flags[MAX_PLAYERS];
	short PacketsPerSec;
	ubyte ShortPackets;

} netgame_info;

typedef struct sequence_packet 
{
	int unknownpad;
	ubyte type;
	int Security;
	ubyte pad1[3];
	netplayer_info player;
} sequence_packet;



typedef struct D2Game
{
		int	KeepAlive;
		BOOL FullGameInfo;
		AllNetPlayers_info	Players;
		netgame_info	Game;
		SOCKADDR_IPX	Master;

}D2Game;


#define CONNECT_DISCONNECTED		0	//(Player was in game but left)
#define CONNECT_PLAYING			1	//(Player is in game and playing)
#define CONNECT_WAITING			2	//(Player is waiting to start the next level)
#define CONNECT_DIED_IN_MINE		3	//(Player died in mine)
#define CONNECT_FOUND_SECRET		4	//(Player exited mine via secret exit)
#define CONNECT_ESCAPE_TUNNEL		5	//(Player is flying thru escape tunnel)
#define CONNECT_END_MENU		6	//(Player is looking at the kill list or bonus)

// Values for game_status field:
#define NETSTAT_MENU		0
#define NETSTAT_PLAYING		1
#define NETSTAT_BROWSING	2
#define NETSTAT_WAITING		3
#define NETSTAT_STARTING	4
#define NETSTAT_ENDLEVEL	5

// Values for game_flags field
#define NETGAME_FLAG_CLOSED 	1
#define NETGAME_FLAG_SHOW_ID	2 // Not currently in use
#define NETGAME_FLAG_SHOW_MAP 	4

// Values for gamemode field
//#define NETGAME_ANARCHY		0
//#define NETGAME_TEAM_ANARCHY	1
//#define NETGAME_ROBOT_ANARCHY	2
//#define NETGAME_COOPERATIVE	3



typedef struct D1_netplayer_info {
	char		callsign[9];
	ubyte		server[4];
	ubyte		node[6];
	ushort	socket;
   char     connect;
 }D1_netplayer_info;

typedef struct D1_netplayer_req {
	char		callsign[10];
	ubyte		server[4];
	ubyte		node[6];
	ushort	socket;
 }D1_netplayer_req;

typedef struct D1_sequence_packet {
	long		type; //36decimal
	D1_netplayer_req	player;
 }D1_sequence_packet;

typedef struct D1_netgame_info {
	long		type; // Always 37 decimal for this packet type
	char		packet_type; //$ if request, % if answer
	char		game_name[16];
	char		team_name[2][9];
	ubyte		gamemode;// (see NETGAME_XXX defines)
	ubyte		difficulty;// (0-4)
	ubyte 	game_status;
	ubyte		numplayers; // how many active players
	ubyte		max_numplayers; //(4 or 8, depending on mode)
	ubyte		game_flags;// (see NETGAME_FLAG_XXX defines)
	D1_netplayer_info	players[8];
	long		locations[8]; // starting locations (not always valid)
	short		kills[8][8]; // array of who-killed-who
	long		levelnum; // -1 = secret #1
	ubyte		protocol_version; // 2 for Registered
	ubyte		team_vector; // bitvector, 0 = blue, 1 = red.
	ushort	segments_checksum;
	short		team_kills[2];
	short		killed[8];
	short		kills1[8];
	fix		level_time;
	fix		control_invul_time; // while level time < this, no damage to controlcen
	long 		monitor_vector; // which monitors are exploded, 32-bit bitvector
	long		player_score[8]; // valid only during coop games
	ubyte		player_flags[8]; // can get info about keys players have
	char		mission_name[9]; // filename of mission
	char		mission_title[22]; // title of mission
} D1_netgame_info;

typedef struct D1Game
{
		int	KeepAlive;
		BOOL FullGameInfo;
	//	AllNetPlayers_info	Players;
		D1_netgame_info	Game;
		SOCKADDR_IPX	Master;

}D1Game;

//#pragma pack(0)
#endif


class CGameView : public CFormView
{
protected:
	CGameView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGameView)

// Form Data
public:
	//{{AFX_DATA(CGameView)
	enum { IDD = IDD_LISTS };
	CEdit	m_ecAddress;
	CButton	m_bShort;
	CEdit	m_ecPPS;
	CListBox	m_ecPlayerName;
	CEdit	m_ecMode;
	CEdit	m_ecRank;
	CEdit	m_ecScore;
	CEdit	m_ecMission;
	CEdit	m_ecMaxPlayers;
	CEdit	m_ecLevel;
	CEdit	m_ecKills2;
	CEdit	m_ecGameName;
	CEdit	m_ecKills;
	CEdit	m_exDifficulty;
	CEdit	m_ecCurPlyrs;
	int		m_iSocket;
	CString	m_strSelPlayer;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	D1Game * FindD1Game(char * GameName);
	void AddD1Game();
	void SendD1Seq();
	BOOL m_GameVerD2;
	AllNetPlayers_info m_Players;
	void AddAllD2Players(void);
	void FillD2Game(void);
	void SendD2Seq();
	void AddD2Game();
	D2Game * FindD2Game(char *GameName);
	int m_CountToRefresh;
	SOCKADDR_IPX m_MyIPXAddr,m_RcvIPXAddr,m_BcastIPX;
	SOCKET m_WSock;
	WSADATA m_wsadata;
	CString m_MyNick;
	short m_RealSocket;
	UINT m_RefreshTimer;
	D2Game *m_D2Ptr;
	D1Game *m_D1Ptr;
	CPtrList m_D2Games;
	CPtrList m_D2Players;
	CPtrList m_D1Games;
	CPtrList m_D1Players;
	netplayer_info m_CurrRcvPlr;
	netgame_info m_CurrGameInfo;
	D1_netgame_info m_D1CurrGameInfo;
	TIMEVAL TimeVal;
	sequence_packet m_Seq,*m_IncomingSeq;
	D1_sequence_packet m_D1Seq,*m_D1IncomingSeq;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGameView)
	afx_msg void OnRefresh();
	afx_msg void OnDeltaposSocketspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSocket();
	afx_msg void OnRadipx();
	afx_msg void OnRadip();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDii();
	afx_msg void OnDi();
	afx_msg void OnSelchangeGamelist();
	afx_msg void OnSelchangePlayerlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__98591C01_CC49_11D0_883A_3C8B00C10000__INCLUDED_)
