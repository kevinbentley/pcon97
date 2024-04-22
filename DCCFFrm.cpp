// DCCFFrm.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "DCCFFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCCFFrm

IMPLEMENT_DYNCREATE(CDCCFFrm, CMDIChildWnd)

CDCCFFrm::CDCCFFrm()
{
}

CDCCFFrm::~CDCCFFrm()
{
}


BEGIN_MESSAGE_MAP(CDCCFFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDCCFFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCFFrm message handlers

BOOL CDCCFFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	RECT l_Rect;

    l_Rect.top=5;
    l_Rect.bottom=135;
    l_Rect.left=5;
	l_Rect.right=220;
    //SetWindowPos(&wndTop,l_left,l_top,l_right,l_bottom,SWP_SHOWWINDOW|SWP_NOZORDER);
	MoveWindow(&l_Rect);
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
