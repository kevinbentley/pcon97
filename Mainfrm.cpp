// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "pconw.h"

#include "ChildFrm.h"
#include "GetFileDlg.h"
#include "SendPrvMsg.h"
#include "DUserList.h"

#include "dwatchDoc.h"

#include "MainFrm.h"
#include "ChannelList.h"
#include "TabBar.h"

#include "DspyFrm.h"

//m_GameLauncher
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(IDC_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(LAUNCH_CFG, OnUpdateCfg)
	ON_COMMAND(LAUNCH_CFG, OnCfg)
	ON_COMMAND(IDM_LAUNCH01, OnLaunch01)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH01, OnUpdateLaunch01)
	ON_COMMAND(IDM_LAUNCH02, OnLaunch02)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH02, OnUpdateLaunch02)
	ON_COMMAND(IDM_LAUNCH03, OnLaunch03)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH03, OnUpdateLaunch03)
	ON_COMMAND(IDM_LAUNCH04, OnLaunch04)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH04, OnUpdateLaunch04)
	ON_COMMAND(IDM_LAUNCH05, OnLaunch05)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH05, OnUpdateLaunch05)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH06, OnUpdateLaunch06)
	ON_COMMAND(IDM_LAUNCH06, OnLaunch06)
	ON_COMMAND(IDM_LAUNCH07, OnLaunch07)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH07, OnUpdateLaunch07)
	ON_COMMAND(IDM_LAUNCH08, OnLaunch08)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH08, OnUpdateLaunch08)
	ON_COMMAND(IDM_LAUNCH09, OnLaunch09)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH09, OnUpdateLaunch09)
	ON_COMMAND(IDM_LAUNCH10, OnLaunch10)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH10, OnUpdateLaunch10)
	ON_COMMAND(IDM_LAUNCH11, OnLaunch11)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH11, OnUpdateLaunch11)
	ON_COMMAND(IDM_LAUNCH12, OnLaunch12)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH12, OnUpdateLaunch12)
	ON_COMMAND(IDM_LAUNCH13, OnLaunch13)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH13, OnUpdateLaunch13)
	ON_COMMAND(IDM_LAUNCH14, OnLaunch14)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH14, OnUpdateLaunch14)
	ON_COMMAND(IDM_LAUNCH15, OnLaunch15)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH15, OnUpdateLaunch15)
	ON_COMMAND(IDM_LAUNCH16, OnLaunch16)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH16, OnUpdateLaunch16)
	ON_COMMAND(IDM_LAUNCH17, OnLaunch17)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH17, OnUpdateLaunch17)
	ON_COMMAND(IDM_LAUNCH18, OnLaunch18)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH18, OnUpdateLaunch18)
	ON_COMMAND(IDM_LAUNCH19, OnLaunch19)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH19, OnUpdateLaunch19)
	ON_COMMAND(IDM_LAUNCH20, OnLaunch20)
	ON_UPDATE_COMMAND_UI(IDM_LAUNCH20, OnUpdateLaunch20)
	ON_COMMAND(IDM_LAUNCHEXPLORER, OnLaunchexplorer)
	ON_UPDATE_COMMAND_UI(IDM_SHOWCHAN, OnUpdateShowchan)
	ON_COMMAND(IDM_SHOWCHAN, OnShowchan)
	ON_COMMAND(ID_MENUDSPY, OnMenudspy)
	ON_UPDATE_COMMAND_UI(ID_MENUDSPY, OnUpdateMenudspy)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	IDM_LAUNCHEXPLORER,
		ID_SEPARATOR,
	IDC_FILE_CLEARWINDOW,
	ID_FILE_LOG,
//	ID_LIST_USERS,
	ID_SERVER_INFO,
		ID_SEPARATOR,
	ID_APP_ABOUT,
		ID_SEPARATOR,
	IDC_SETPREFERENCES,
	IDC_STOPSCROLL,
	IDC_SETAWAY,
		ID_SEPARATOR,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
		ID_SEPARATOR,
	IDM_SHOWCHAN,
		ID_SEPARATOR,
	ID_MENUDSPY
	
};

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_GameLauncher=NULL;	
}

CMainFrame::~CMainFrame()
{
    delete m_GameLauncher;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//
	if (!m_wndToolBar.Create(this,WS_CHILD | WS_VISIBLE | CBRS_TOP,0x4272) ||
		m_wndToolBar.GetToolBarCtrl().AddBitmap(sizeof(buttons)/sizeof(UINT), IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		return -1;      // fail to create
	}
//!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		//TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);

	//	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	EnableDocking(CBRS_ALIGN_ANY);
	
	
	//SetWindowText("The Player's Connection for Windows");
	((CPconwApp*)AfxGetApp())->m_MainStatusBar=&m_wndStatusBar;
	((CPconwApp*)AfxGetApp())->m_MainToolBar=&m_wndToolBar;
	

//	dlgChannelList.Create(this,IDD_CHANNELBAR,CBRS_RIGHT,0x4270);
//	dlgChannelList.EnableDocking(CBRS_ALIGN_ANY);
//	((CPconwApp*)AfxGetApp())->m_ChanList=&dlgChannelList;

	
	/*
	m_GameLauncher.Create(this);
	m_GameLauncher.SetBarStyle(m_GameLauncher.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		
	m_GameLauncher.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	
	((CPconwApp*)AfxGetApp())->m_LaunchPadBar=&m_GameLauncher;
*/
	//DockControlBar(&m_GameLauncher,AFX_IDW_DOCKBAR_BOTTOM);

  
	DockControlBar(&m_wndToolBar);
//	DockControlBarLeftOf(&dlgChannelList,&m_wndToolBar);
//	DockControlBar(&dlgChannelList);

    HINSTANCE hInst;
	HICON AppIcon;
    hInst = AfxGetInstanceHandle();
	AppIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
    SendMessage(WM_SETICON,TRUE,(LPARAM)AppIcon);
	SetIcon(AppIcon,TRUE);
	SetIcon(AppIcon,FALSE);

	//ShowWindow(SW_SHOWMAXIMIZED);

   	RECT l_Rect;


    l_Rect.top=GetPrivateProfileInt("ParentWnd","Top",0,"pcon97.ini");
    l_Rect.bottom=GetPrivateProfileInt("ParentWnd","Bottom",470,"pcon97.ini");//470
    l_Rect.left=GetPrivateProfileInt("ParentWnd","Left",0,"pcon97.ini");
    l_Rect.right=GetPrivateProfileInt("ParentWnd","Right",636,"pcon97.ini");//636
    MoveWindow(&l_Rect);
    //ShowWindow(SW_SHOWNORMAL);

	RefreshLaunchBar();

	//
	WritePrivateProfileString("pconw.ini-Bar5","Visible","1","pcon97.ini");
	WritePrivateProfileString("pconw.ini-Bar6","Visible","1","pcon97.ini");
    CDockState l_DockState;
   // l_DockState.LoadState("pcon97.ini");
   // SetDockState(l_DockState);
	
	//((CPconwApp *)AfxGetMainWnd())->m_SplashWnd.SetOKToClose();

	//((CPconwApp *)AfxGetMainWnd())->m_SplashWnd.DestroyWindow();
/*
	if(((CPconwApp*)AfxGetApp())->m_bShowChanList==0)
	{
		dlgChannelList.ShowWindow(SW_HIDE);
		RecalcLayout(TRUE);
	}
	else
	{
		dlgChannelList.ShowWindow(SW_NORMAL);
		RecalcLayout(TRUE);
	}
*/
	m_TabBar.Create(this,IDD_TABVIEW,CBRS_TOP,0x4269);
	
	m_TabBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_TabBar,AFX_IDW_DOCKBAR_TOP);
	m_TabBar.ReSizeTabBar();
//	m_TabBar.SubclassDlgItem(IDC_TAB2,this);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	m_Timer=SetTimer(0x142,300,NULL);
	//((CPconwApp *)AfxGetMainWnd())->m_SplashWnd.SetOKToClose();
	//((CPconwApp *)AfxGetMainWnd())->m_SplashWnd.DestroyWindow();
	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);
}

//CDocManager//CreateNewFrame

void CMainFrame::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnClose() 
{
	
	CDockState l_DockState;
	GetDockState(l_DockState);
    l_DockState.SaveState("pcon97.ini");

//ShowWindow
    KillTimer(m_Timer);
	((CPconwApp*)AfxGetApp())->m_bClosing=TRUE;
	
	WINDOWPLACEMENT l_wndplc;
	GetWindowPlacement(&l_wndplc);
	if(!(l_wndplc.showCmd & SW_MINIMIZE))
	{	
		RECT l_wndsize;
		CString l_strValue;

		GetWindowRect(&l_wndsize);

		l_strValue.Format("%d",l_wndsize.top);
		WritePrivateProfileString("ParentWnd","Top",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.bottom);
		WritePrivateProfileString("ParentWnd","Bottom",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.left);
		WritePrivateProfileString("ParentWnd","Left",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.right);
		WritePrivateProfileString("ParentWnd","Right",LPCSTR(l_strValue),"pcon97.ini");
	}
	CMDIFrameWnd::OnClose();
	
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	((CPconwApp*)AfxGetApp())->FileXferIdle();	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnUpdateCfg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnCfg() 
{
	return;
	
}

void CMainFrame::RefreshLaunchBar(void)
{
	
    
    if(m_GameLauncher) 
    {
        CDockState l_DockState;
	    GetDockState(l_DockState);
        l_DockState.SaveState("pcon97.ini");
        delete m_GameLauncher;
    }
	m_GameLauncher = new CLauncher;
	//m_GameLauncher->DestroyWindow();
	m_GameLauncher->Create(this,WS_CHILD | WS_VISIBLE | CBRS_TOP,0x4271);
	m_GameLauncher->SetBarStyle(m_GameLauncher->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		
	//m_GameLauncher->ModifyStyle(0, TBSTYLE_FLAT);
	m_GameLauncher->EnableDocking(CBRS_ALIGN_ANY);
	
	((CPconwApp*)AfxGetApp())->m_LaunchPadBar=m_GameLauncher;

	m_GameLauncher->RefreshToolBar((LaunchItems *)((CPconwApp *)AfxGetApp())->m_LaunchItems);
	DockControlBar(m_GameLauncher,AFX_IDW_DOCKBAR_BOTTOM);
	ShowControlBar(m_GameLauncher,TRUE,FALSE);
    CDockState l_DockState;
   //l_DockState.LoadState("pcon97.ini");
   // SetDockState(l_DockState);

	
}

void CMainFrame::OnLaunch01() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(0);
	
}

void CMainFrame::OnUpdateLaunch01(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch02() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(1);
	
}

void CMainFrame::OnUpdateLaunch02(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch03() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(2);
	
}

void CMainFrame::OnUpdateLaunch03(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch04() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(3);
	
}

void CMainFrame::OnUpdateLaunch04(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch05() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(4);
	
}

void CMainFrame::OnUpdateLaunch05(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch06() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(5);
	
}

void CMainFrame::OnUpdateLaunch06(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch07() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(6);
	
}

void CMainFrame::OnUpdateLaunch07(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch08() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(7);
	
}

void CMainFrame::OnUpdateLaunch08(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch09() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(8);
	
}

void CMainFrame::OnUpdateLaunch09(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch10() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(9);
	
}

void CMainFrame::OnUpdateLaunch10(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch11() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(10);
	
}

void CMainFrame::OnUpdateLaunch11(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch12() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(11);
	
}

void CMainFrame::OnUpdateLaunch12(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch13() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(12);
	
}

void CMainFrame::OnUpdateLaunch13(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch14() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(13);
	
}

void CMainFrame::OnUpdateLaunch14(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch15() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(14);
	
}

void CMainFrame::OnUpdateLaunch15(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch16() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(15);
	
}

void CMainFrame::OnUpdateLaunch16(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch17() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(16);
	
}

void CMainFrame::OnUpdateLaunch17(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch18() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(17);
	
}

void CMainFrame::OnUpdateLaunch18(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch19() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(18);
	
}

void CMainFrame::OnUpdateLaunch19(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CMainFrame::OnLaunch20() 
{
	((CPconwApp*)AfxGetApp())->LaunchItem(19);
	
}

void CMainFrame::OnUpdateLaunch20(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}




BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR * l_pnotify;
	TOOLTIPTEXT * l_pToolTipInfo;

	l_pnotify=(NMHDR *)lParam;
	
	//Just in case
	if(!l_pnotify) return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);

	if(l_pnotify->code==TTN_NEEDTEXT)
	{

		if((l_pnotify->idFrom>=IDM_LAUNCH01) && (l_pnotify->idFrom<=IDM_LAUNCH20))
		{
		l_pToolTipInfo = (TOOLTIPTEXT *)l_pnotify;
		l_pToolTipInfo->hinst==NULL;
		
		strcpy(l_pToolTipInfo->szText,((LaunchItems *)&((CPconwApp *)AfxGetApp())->m_LaunchItems[l_pToolTipInfo->hdr.idFrom-IDM_LAUNCH01])->ProgName);
		return TRUE;
		}
	}
	return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
}

void CMainFrame::OnLaunchexplorer() 
{
	ShellExecute(m_hWnd,"explore",".",NULL,NULL,SW_SHOWNORMAL);
	
}

void CMainFrame::OnUpdateShowchan(CCmdUI* pCmdUI) 
{

/*	if(((CPconwApp*)AfxGetApp())->m_bShowChanList)
	{
		//Check the menu item
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		//Uncheck the menu item
		pCmdUI->SetCheck(FALSE);
	}
*/	
}

void CMainFrame::OnShowchan() 
{

	((CPconwApp*)AfxGetApp())->SendMsg(CString("/list"));
/*
	if(((CPconwApp*)AfxGetApp())->m_bShowChanList)
	{
		((CPconwApp*)AfxGetApp())->m_bShowChanList=0;
		dlgChannelList.ShowWindow(SW_HIDE);
		RecalcLayout(TRUE);
	}
	else
	{
		((CPconwApp*)AfxGetApp())->m_bShowChanList=1;
		dlgChannelList.ShowWindow(SW_NORMAL);
		RecalcLayout(TRUE);
	}
*/	
}

void CMainFrame::OnMenudspy() 
{
	if(((CPconwApp*)AfxGetApp())->m_bDSpyRunning)
	{
		m_DSpyDoc->OnCloseDocument();
		((CPconwApp*)AfxGetApp())->m_bDSpyRunning=0;
	}
	else
	{
		((CPconwApp*)AfxGetApp())->m_bDSpyRunning=1;
		CDwatchDoc *l_DDspyDoc;
		CDspyFrm *l_DspyFrame;
		
		l_DDspyDoc=(CDwatchDoc *)((CPconwApp*)AfxGetApp())->m_DSPYFDocTemplate->CreateNewDocument();
		l_DspyFrame=(CDspyFrm *)((CPconwApp*)AfxGetApp())->m_DSPYFDocTemplate->CreateNewFrame((CDocument *)l_DDspyDoc,NULL);
		((CPconwApp*)AfxGetApp())->m_DSPYFDocTemplate->InitialUpdateFrame((CFrameWnd *)l_DspyFrame,l_DDspyDoc,TRUE);
		m_DSpyDoc=l_DDspyDoc;
	}
	
}

void CMainFrame::OnUpdateMenudspy(CCmdUI* pCmdUI) 
{
	if(((CPconwApp*)AfxGetApp())->m_bDSpyRunning)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}

	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	m_TabBar.ReSizeTabBar();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	CMDIFrameWnd::ActivateFrame(nCmdShow);
}
