// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "pconw.h"

#include "ChildFrm.h"
#include "pconwDoc.h"
#include "pconwView.h"
#include "SendView.h"
#include "ChanUList.h"
#include "util.h"

#include "mainfrm.h"

#define USERLISTWIDTH 137
#define EDITHEIGHT 30


/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_PAINT()
	ON_WM_MDIACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_COMMAND(ID_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_LOG, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOG, OnUpdateFileSave)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_iViewCreated=FALSE;
	if(!((CPconwApp*)AfxGetApp())->m_MainChannelWnd) ((CPconwApp*)AfxGetApp())->m_MainChannelWnd=this;
	
}

CChildFrame::CChildFrame(CString& WindName)
{
	m_iViewCreated=FALSE;
	m_WindowName=WindName;

	
}

CChildFrame::~CChildFrame()
{
	m_iViewCreated=FALSE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if(m_wndSplitter2.CreateStatic(this,1,2))
		if(m_wndSplitter.CreateStatic((CWnd *)&m_wndSplitter2,2,1,WS_CHILD|WS_VISIBLE|WS_BORDER,m_wndSplitter2.IdFromRowCol(0,0)))
	{
		CRect rect;
		GetClientRect(&rect);
		CSize size = rect.Size();
		size.cy-=EDITHEIGHT;
        size.cx-=USERLISTWIDTH;
	
	if (m_wndSplitter2.CreateView(0,1,RUNTIME_CLASS(CChanUList),CSize(0,0),pContext))
		if (m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CPconwView),size,pContext))
		{
			if (m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CSendView),CSize(0,0),pContext))
				
					{
						m_wndSplitter.SetRowInfo(0,size.cy,1);
						m_wndSplitter.SetRowInfo(1,EDITHEIGHT,0);
                        //m_wndSplitter.SetColumnInfo(0,size.cx,0);
						//m_wndSplitter.SetColumnInfo(1,USERLISTWIDTH,0);
                        m_wndSplitter2.SetColumnInfo(1,USERLISTWIDTH,0);
                        m_wndSplitter2.SetColumnInfo(0,size.cx,0);
						m_iViewCreated=TRUE;
						SetActiveView((CView*)m_wndSplitter.GetPane(1,0));
						
						//RECT rClient;
						//AfxGetMainWnd()->GetClientRect(&rClient);
						
						//OnSize(TRUE); 
						return TRUE;	
					}
		}
	}
	return FALSE;
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::RecalcLayout(BOOL bNotify) 
{
	CRect rect;
	GetClientRect(&rect);
	CSize size = rect.Size();
	size.cy-=EDITHEIGHT;
	size.cx-=USERLISTWIDTH;
	
	if(m_iViewCreated)
	{
		m_wndSplitter.SetRowInfo(0,size.cy,0);
		m_wndSplitter.SetRowInfo(1,EDITHEIGHT,0);
        //m_wndSplitter.SetColumnInfo(0,size.cx,0);
        
        m_wndSplitter2.SetColumnInfo(1,USERLISTWIDTH,0);
        m_wndSplitter2.SetColumnInfo(0,size.cx,0);
	}
	CMDIChildWnd::RecalcLayout(bNotify);
}


void CChildFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	SetWindowText("#kahn");
	
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	CString l_Title;
	l_Title = ((CPconwDoc*)GetActiveDocument())->m_Winname;
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.SetSelTab(l_Title);
}

void CChildFrame::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CChildFrame::OnFileClearwindow() 
{
	CPconwView *ChatterView;
	ChatterView=(CPconwView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) ChatterView->OnFileClearwindow();	
}

void CChildFrame::OnDestroy() 
{
	RECT l_wndsize;
    CString l_strValue;

    WINDOWPLACEMENT l_wndplc;
	GetWindowPlacement(&l_wndplc);
	//if((l_wndplc.showCmd & SW_MINIMIZE))
	//{
	if((l_wndplc.showCmd == SW_MINIMIZE))
	{
		WritePrivateProfileString("ChatWnd","Minimized","TRUE","pcon97.ini");	
	}
	else
	{
		WritePrivateProfileString("ChatWnd","Minimized","FALSE","pcon97.ini");	
	}
	if((l_wndplc.showCmd == SW_MAXIMIZE))
	{
		WritePrivateProfileString("ChatWnd","Maximized","TRUE","pcon97.ini");	
	}
	else
	{
		WritePrivateProfileString("ChatWnd","Maximized","FALSE","pcon97.ini");	
	}
	
	if((l_wndplc.showCmd != SW_MAXIMIZE)&&(l_wndplc.showCmd != SW_MINIMIZE))
	{
		GetWindowRect(&l_wndsize);
		
		GetParent()->ScreenToClient(&l_wndsize);
		
		l_strValue.Format("%d",l_wndsize.top);
		WritePrivateProfileString("ChatWnd","Top",LPCSTR(l_strValue),"pcon97.ini");
		l_strValue.Format("%d",l_wndsize.bottom);
		WritePrivateProfileString("ChatWnd","Bottom",LPCSTR(l_strValue),"pcon97.ini");
		l_strValue.Format("%d",l_wndsize.left);
		WritePrivateProfileString("ChatWnd","Left",LPCSTR(l_strValue),"pcon97.ini");
		l_strValue.Format("%d",l_wndsize.right);
		WritePrivateProfileString("ChatWnd","Right",LPCSTR(l_strValue),"pcon97.ini");
	}
//	}
    CMDIChildWnd::OnDestroy();
		
}

void CChildFrame::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::PostNcDestroy();
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CFont *f;
    LOGFONT l_lf;
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

	l_Rect.top=GetPrivateProfileInt("ChatWnd","Top",0,"pcon97.ini");
	l_Rect.bottom=GetPrivateProfileInt("ChatWnd","Bottom",162,"pcon97.ini");
	l_Rect.left=GetPrivateProfileInt("ChatWnd","Left",0,"pcon97.ini");
	l_Rect.right=GetPrivateProfileInt("ChatWnd","Right",511,"pcon97.ini");
	
	char szBuffer[100];
	GetPrivateProfileString("ChatWnd","Minimized","FALSE",szBuffer,100,"pcon97.ini");
	if(stricmp(szBuffer,"TRUE")==0)
	{
		bMin = TRUE;
	}
	GetPrivateProfileString("ChatWnd","Maximized","TRUE",szBuffer,100,"pcon97.ini");
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

void CChildFrame::OnFileSave() 
{
	CPconwView *ChatterView;
	ChatterView=(CPconwView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) ChatterView->ToggleLogging();
	//Get pane 0,0
	//Call ToggleLogging()
	
}

void CChildFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	BOOL l_logstat;
	CPconwView *ChatterView;
	ChatterView=(CPconwView *)m_wndSplitter.GetPane(0,0);
	if(ChatterView) l_logstat=ChatterView->GetLogState();
	pCmdUI->SetCheck(l_logstat);
	//Get pane 0,0
	//call GetLogState()
	//if TRUE, pCmdUI->SetCheck(TRUE);
	//else pCmdUI->SetCheck(FALSE);	
}

void CChildFrame::OnClose() 
{
		
	CMDIChildWnd::OnClose();
}
