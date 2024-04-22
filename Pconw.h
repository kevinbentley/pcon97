#ifndef _pconw_header_
#define _pconw_header_
// pconw.h : main header file for the PCONW application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "sharemsg.h"		// include shared memory file
#include "pconwDoc.h"
#include "cfilexfr.h"
#include "ConfigLauncher.h"
//Splash screen stuff
#include "mmsystem.h"

#include "dib.h"
#include "dibpal.h"
#include "splasher.h"

#include "ControlSocket.h"
#include "NewMessage.h"

#include "DynChannelList.h"

#include "UserPing.h"
/////////////////////////////////////////////////////////////////////////////
// CPconwApp:
// See pconw.cpp for the implementation of this class
//

#define MAXTCPBUFF 500
#define MAXOFFERED 10

typedef struct TabUser 
{
	char User[20];
	TabUser *Next;
}TabUser;



class CPconwApp : public CWinApp
{
public:
	void ClearNewMessage(CString & Title);
	void NewMessage(CString &Title);
	DWORD m_BeepOnPrivate;
	unsigned int m_iSocketTimeout;
	BOOL m_bSocketConnected;
	DWORD m_IRCServerPort;
	CString m_IRCServerName;
	CString m_OfferedFiles[MAXOFFERED];
//	BOOL m_InParseData;
	BOOL m_bInRemove;
	BOOL m_bInAdd;
	BOOL RemoveMDIFrame(CString &Title);
	BOOL AddMDIFrame(CFrameWnd *Wnd,CDocument *Doc,CString &Title);
	void ParseControlData(PBYTE data, int datalen, CString& sendingip, UINT recvport);
	CControlSocket * m_ControlSocket;
	void ProccessControlData(void);
	char * GetWordNum(int num,CString &l_String);
	DWORD m_bShowChanList;
	CSplashThread* pSplashThread;
	int m_iSplashGone;
	CSplashWnd m_SplashWnd;
	void ParseKahnData(LPCSTR Line);
	//int m_TCPCurrPos;
	//char m_TCPBufferB[MAXTCPBUFF*2];
	//char m_TCPBufferA[MAXTCPBUFF*2];
	CString m_TCPBuffA,m_TCPBuffB;
	CString m_PconwPath;
	~CPconwApp();
	DWORD m_bAutoLog;
	CString m_CmdPart;
	CStringList m_ChannelList;
	CStringList m_IgnoreList;
	CStringList m_slChannelListData;
	CDynChannelList m_DynChannelList;

	BOOL CheckAddUser(LPCSTR User,long IP);
	CMultiDocTemplate * m_ChatDocTemplate;
	CMultiDocTemplate * m_FingerDocTemplate;
	CMultiDocTemplate * m_DCCCDocTemplate;
	CMultiDocTemplate * m_DCCFDocTemplate;
	CMultiDocTemplate * m_DSPYFDocTemplate;
	BOOL m_bDSpyRunning;
	DWORD m_ibCloseChat;
	DWORD m_ibConfirmLaunch;
	void LoadCommandStrings (void);
	void LaunchItem(int ItemNum);
	void DraggedLaunchItem(CString &DragFile);
	void AddDraggedLaunchItems(CStringList *Files);
	void ActivateChannel(void);
	int m_ChanRefresh;
	CChannelList * m_ChanList;
	CString m_strSpam;
	UINT m_WhoTimer;
	void FileXferIdle(void);
	DWORD m_bSeperatePrivate;
	CWnd * m_MainChannelWnd;
	void ActivateMainWnd(void);
	void SetNewFont(void);
	BOOL m_bClosing;
	void OpenQueryWindow(LPCSTR Name);
	BOOL m_bNewFont;
	void SendMsg(CString& strText,LPCSTR Window=NULL);
	CString CurrSelUser;
	CString m_Channel;
	void ParseKahnData(PBYTE data, int datalen, CString& sendingip, UINT recvport);
	void ProcessPendingRead(void);
	char * GetNextTabUser(void);
	void AddTabUser(const char *User);
	void GetFilters(void);
	DWORD GetColor (CString &cs_Text);
	CString m_strHandle;
	BOOL ConnectSocket(void);
	CMapStringToOb m_WindowMap;
	void DispatchMessage(LPCSTR Message,DWORD Color,LPCSTR User);
	
	LaunchItems m_LaunchItems[20];
	
	CString m_CreateNewDocTitle;
	CDocument *m_ChannelDoc;
	CRegistry registry;
	DWORD colorBACK;
	DWORD colorACT;
	DWORD colorNORM;
	DWORD colorPRIV;
    DWORD colorURL;
	DWORD colorSTAT;
	BOOL ProfanityFilter;
	BOOL m_bAway;
	DWORD FontSize;
	CString FontName;
	BOOL m_Frozen;
	CString DefaultChannel;
	CString LeaveMsg;
	CString F2key;
	CString F3key;
	CString F4key;
	CString F5key;
	CString F6key;
	CString F7key;
	CString F8key;
	CString F9key;
	CString F10key;
	CString F11key;
	CString F12key;
	CStatusBar*  m_MainStatusBar;
	CToolBar*    m_MainToolBar;
	CToolBar*    m_LaunchPadBar;
	PSHAREMEM psharemem;
	HANDLE hMapObject;
	CMapStringToOb Users;
	CChatSocket* m_pSocket;
	CFileXfer XferFiles;
	CString m_strAwayMsg;
	TabUser *FirstTabUser,*CurrTabUser,*TempTabUser;
	DWORD m_bShowJoin;
	DWORD m_bSpam;
	CConfigLauncher *m_LaunchCfgDlg;
	int g_iCancel;

	CString m_CmdMsg;
	CString m_CmdWho;
	CString m_CmdUsers;
	CString m_CmdPing;
	CString m_CmdHelp;
	CString m_CmdIgnore;
	CString m_CmdMotd;
	CString m_CmdKill;
	CString m_CmdGet;
	CString m_CmdOffer;
	CString m_CmdCancelGet;
	CString m_CmdWhois;
	CString m_CmdNoprivate;
	CString m_CmdBan;
	CString m_CmdUnban;
	CString m_CmdBlist;
	CString m_CmdDeoffer;
	CString m_CmdProgress;
	CString m_CmdShowoffers;
	CString m_CmdJoin;
	CString m_CmdMe;
	CString m_CmdAway;
	CString m_CmdSubnet;


	typedef struct SFilters {
	CString cs_Word;
	DWORD dw_Color;
	SFilters *Next;
	}SFilters;
	SFilters *FirstFilter,*CurrFilter;
	void GetDefaults(void);
	CPconwApp();

	CMapStringToOb m_WindowList;
	CNewMessage m_NewMessage;
	CMapStringToOb m_UserPings;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPconwApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPconwApp)
	afx_msg void OnAppAbout();
	afx_msg void OnListUsers();
	afx_msg void OnStopscroll();
	afx_msg void OnUpdateStopscroll(CCmdUI* pCmdUI);
	afx_msg void OnSetaway();
	afx_msg void OnUpdateSetaway(CCmdUI* pCmdUI);
	afx_msg void OnServerInfo();
	afx_msg void OnViewToolbar();
	afx_msg void OnSetpreferences();
	afx_msg void OnJoinsel();
	afx_msg void OnLaunchCfg();
	afx_msg void OnPreferencesColors();
	afx_msg void OnPreferencesFont();
	afx_msg void OnPreferencesHighlighting();
	afx_msg void OnPreferencesFunctionkeys();
	afx_msg void OnHomepage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMultiDocTemplate* m_pDocTemplate;
	CString m_strRejectMsg;
	BOOL m_bRejecting;
};


/////////////////////////////////////////////////////////////////////////////
#endif
