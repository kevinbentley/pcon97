// FingerFrame.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "FingerFrame.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerFrame

IMPLEMENT_DYNCREATE(CFingerFrame, CMDIChildWnd)

CFingerFrame::CFingerFrame()
{
}

CFingerFrame::~CFingerFrame()
{
}


BEGIN_MESSAGE_MAP(CFingerFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFingerFrame)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerFrame message handlers

void CFingerFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	CString l_Title;
	GetWindowText(l_Title);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.SetSelTab(l_Title);	
}
