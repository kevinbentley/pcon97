// LaunchPad.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "LaunchPad.h"


/////////////////////////////////////////////////////////////////////////////
// CLaunchPad

CLaunchPad::CLaunchPad()
{
}

CLaunchPad::~CLaunchPad()
{
}


BEGIN_MESSAGE_MAP(CLaunchPad, CToolBar)
	//{{AFX_MSG_MAP(CLaunchPad)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLaunchPad message handlers

void CLaunchPad::OnDropFiles(HDROP hDropInfo) 
{
	MessageBeep(-1);
	
	CToolBar::OnDropFiles(hDropInfo);
}
