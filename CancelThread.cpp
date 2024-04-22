// CancelThread.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"

#include "AbortScan.h"

#include "CancelThread.h"




/////////////////////////////////////////////////////////////////////////////
// CCancelThread

IMPLEMENT_DYNCREATE(CCancelThread, CWinThread)

CCancelThread::CCancelThread()
{
}

CCancelThread::~CCancelThread()
{
}

BOOL CCancelThread::InitInstance()
{
	//m_AbortDlg.Create(IDD_ABORTSCAN);
	m_AbortDlg.DoModal();
	return TRUE;
}

int CCancelThread::ExitInstance()
{
	//m_AbortDlg.DestroyWindow();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCancelThread, CWinThread)
	//{{AFX_MSG_MAP(CCancelThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCancelThread message handlers
