// AbortScan.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "AbortScan.h"



/////////////////////////////////////////////////////////////////////////////
// CAbortScan dialog


CAbortScan::CAbortScan(CWnd* pParent /*=NULL*/)
	: CDialog(CAbortScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbortScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iCanceled=0;
	((CPconwApp *)AfxGetApp())->g_iCancel=0;
}


void CAbortScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbortScan)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbortScan, CDialog)
	//{{AFX_MSG_MAP(CAbortScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbortScan message handlers

void CAbortScan::OnCancel() 
{
	m_iCanceled=1;
	((CPconwApp *)AfxGetApp())->g_iCancel=1;
}
