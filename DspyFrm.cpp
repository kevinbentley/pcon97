// DspyFrm.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "DspyFrm.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspyFrm

IMPLEMENT_DYNCREATE(CDspyFrm, CMDIChildWnd)

CDspyFrm::CDspyFrm()
{
}

CDspyFrm::~CDspyFrm()
{
}


BEGIN_MESSAGE_MAP(CDspyFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDspyFrm)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspyFrm message handlers

BOOL CDspyFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	MoveWindow(0,0,510,370);
	
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CDspyFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	CString l_Title;
	GetWindowText(l_Title);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.SetSelTab(l_Title);
	
}
