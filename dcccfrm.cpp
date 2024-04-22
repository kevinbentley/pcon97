// CDCCCFrm
//

#include "stdafx.h"
#include "pconw.h"

#include "DCCCDoc.h"
#include "DCCCFrm.h"
#include "DCCCSend.h"
#include "DCCCView.h"

#include "mainfrm.h"

#define USERLISTWIDTH 135
#define EDITHEIGHT 30

/////////////////////////////////////////////////////////////////////////////
// CDCCCFrm

IMPLEMENT_DYNCREATE(CDCCCFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CDCCCFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDCCCFrm)
	ON_WM_PAINT()
	ON_WM_MDIACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_COMMAND(ID_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_LOG, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOG, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCCFrm construction/destruction

CDCCCFrm::CDCCCFrm()
{
	m_iViewCreated=FALSE;
	if(!((CPconwApp*)AfxGetApp())->m_MainChannelWnd) ((CPconwApp*)AfxGetApp())->m_MainChannelWnd=this;
	
}

CDCCCFrm::CDCCCFrm(CString& WindName)
{
	m_iViewCreated=FALSE;
	m_WindowName=WindName;
	
}

CDCCCFrm::~CDCCCFrm()
{
	m_iViewCreated=FALSE;
}

BOOL CDCCCFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDCCCFrm diagnostics

#ifdef _DEBUG
void CDCCCFrm::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CDCCCFrm::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCCCFrm message handlers

BOOL CDCCCFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (m_wndSplitter.CreateStatic(this,2,1))
	{
		CRect rect;
		GetClientRect(&rect);
		CSize size = rect.Size();
		size.cy-=EDITHEIGHT;
	
		if (m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CDCCCView),size,pContext))
		{
			if (m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CDCCCSend),CSize(0,0),pContext))
				
					{
						m_wndSplitter.SetRowInfo(0,size.cy,1);
						m_wndSplitter.SetRowInfo(1,EDITHEIGHT,0);
						
						m_iViewCreated=TRUE;
						SetActiveView((CView*)m_wndSplitter.GetPane(1,0));
						
						RECT rClient;
						AfxGetMainWnd()->GetClientRect(&rClient);

						//ShowWindow(SW_SHOWMAXIMIZED);
						//OnSize(SIZE_RESTORED,rClient.right,rClient.bottom);//GetClientRect
						return TRUE;	
					}
		}
	}
	return FALSE;
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CDCCCFrm::RecalcLayout(BOOL bNotify) 
{
	CRect rect;
	GetClientRect(&rect);
	CSize size = rect.Size();
	size.cy-=EDITHEIGHT;
	
	
	if(m_iViewCreated)
	{
		m_wndSplitter.SetRowInfo(0,size.cy,0);
		m_wndSplitter.SetRowInfo(1,EDITHEIGHT,0);
	}
	CMDIChildWnd::RecalcLayout(bNotify);
}


void CDCCCFrm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	SetWindowText("#kahn");
	
}

void CDCCCFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	CString l_Title;
	GetWindowText(l_Title);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.SetSelTab(l_Title);
	
}

void CDCCCFrm::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDCCCFrm::OnFileClearwindow() 
{
	CDCCCView *ChatterView;
	ChatterView=(CDCCCView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) ChatterView->OnFileClearwindow();	
}

void CDCCCFrm::OnDestroy() 
{
	
	RECT l_wndsize;
    CString l_strValue;

    WINDOWPLACEMENT l_wndplc;
	GetWindowPlacement(&l_wndplc);
	if((l_wndplc.showCmd == SW_MINIMIZE))
	{
		WritePrivateProfileString("PrivWnd","Minimized","TRUE","pcon97.ini");	
	}
	else
	{
		WritePrivateProfileString("PrivWnd","Minimized","FALSE","pcon97.ini");	
	}
	if((l_wndplc.showCmd ==SW_MAXIMIZE))
	{
		WritePrivateProfileString("PrivWnd","Maximized","TRUE","pcon97.ini");	
	}
	else
	{
		WritePrivateProfileString("PrivWnd","Maximized","FALSE","pcon97.ini");	
	}

	if((l_wndplc.showCmd != SW_MAXIMIZE)&&(l_wndplc.showCmd != SW_MINIMIZE))
	{

		GetWindowRect(&l_wndsize);
			
		GetParent()->ScreenToClient(&l_wndsize);
		l_strValue.Format("%d",l_wndsize.top);
		WritePrivateProfileString("PrivWnd","Top",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.bottom);
		WritePrivateProfileString("PrivWnd","Bottom",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.left);
		WritePrivateProfileString("PrivWnd","Left",LPCSTR(l_strValue),"pcon97.ini");

		l_strValue.Format("%d",l_wndsize.right);
		WritePrivateProfileString("PrivWnd","Right",LPCSTR(l_strValue),"pcon97.ini");
	}
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDCCCFrm::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::PostNcDestroy();//CFtp
}

int CDCCCFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInst;
	HICON AppIcon;
    hInst = AfxGetInstanceHandle();
	AppIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDR_PCONWTYPE));
    SendMessage(WM_SETICON, TRUE, (LPARAM)AppIcon);
	SetIcon(AppIcon,TRUE);
	SetIcon(AppIcon,FALSE);

	BOOL bMin,bMax;
	bMin = FALSE;
	bMax = FALSE;

	RECT l_Rect;

	l_Rect.top=GetPrivateProfileInt("PrivWnd","Top",162,"pcon97.ini");
	l_Rect.bottom=GetPrivateProfileInt("PrivWnd","Bottom",324,"pcon97.ini");
	l_Rect.left=GetPrivateProfileInt("PrivWnd","Left",0,"pcon97.ini");
	l_Rect.right=GetPrivateProfileInt("PrivWnd","Right",511,"pcon97.ini");

	char szBuffer[100];
	GetPrivateProfileString("PrivWnd","Minimized","FALSE",szBuffer,100,"pcon97.ini");
	if(stricmp(szBuffer,"TRUE")==0)
	{
		bMin = TRUE;
	}
	GetPrivateProfileString("PrivWnd","Maximized","TRUE",szBuffer,100,"pcon97.ini");
	if(stricmp(szBuffer,"TRUE")==0)
	{
		bMax = TRUE;
	}

	if(bMin)
	{
		MoveWindow(&l_Rect);
		ShowWindow(SW_SHOWMINIMIZED);
	}
	else if(bMax)
	{
		MoveWindow(&l_Rect);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
	else 
	{

		MoveWindow(&l_Rect);
	}
	return 0;
}

void CDCCCFrm::OnFileSave() 
{
	CDCCCView *ChatterView;
	ChatterView=(CDCCCView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) ChatterView->ToggleLogging();
	//Get pane 0,0
	//Call ToggleLogging()
	
}

void CDCCCFrm::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	BOOL l_logstat;
	CDCCCView *ChatterView;
	ChatterView=(CDCCCView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) l_logstat=ChatterView->GetLogState();
	pCmdUI->SetCheck(l_logstat);
	//Get pane 0,0
	//call GetLogState()
	//if TRUE, pCmdUI->SetCheck(TRUE);
	//else pCmdUI->SetCheck(FALSE);	
}
void CDCCCFrm::OnClose() 
{
	//MessageBeep(-1);
	CMDIChildWnd::OnClose();
}
