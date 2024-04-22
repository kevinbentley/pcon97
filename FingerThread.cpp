// FingerThread.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "FingerDoc.h"
#include "FingerSocket.h"
#include "FingerView.h"
#include "FingerFrame.h"

#include "FingerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerThread

IMPLEMENT_DYNCREATE(CFingerThread, CWinThread)

CFingerThread::CFingerThread()
{
}

//CBounceThread::CFingerThread(HWND hwndParent) : m_hwndParent(hwndParent)
//{
//}


CFingerThread::~CFingerThread()
{
}



BOOL CFingerThread::InitInstance()
{
//	CWnd* pParent = CWnd::FromHandle(m_hwndParent);
//	CRect rect;
//	pParent->GetClientRect(&rect);
//
//	BOOL bReturn = m_wndFinger.Create(_T("FingerChildWnd"),
//		WS_CHILD | WS_VISIBLE, rect, pParent);

	// It is important to set CWinThread::m_pMainWnd to the user interface
	// window.  This is required so that when the m_pMainWnd is destroyed,
	// the CWinThread is also automatically destroyed.  For insight into
	// how the CWinThread is automatically destroyed when the m_pMainWnd
	// window is destroyed, see the implementation of CWnd::OnNcDestroy
	// in wincore.cpp of the MFC sources.

//	if (bReturn)
//		m_pMainWnd = &m_wndFinger;

	//return bReturn;
	
	
	
	//((CPconwApp*)AfxGetApp())->
	return TRUE;
}

int CFingerThread::ExitInstance()
{
	
	
	
	
	
	//loop for a while....
//	for(int i=0;i<100;i++)
//	{
//		if(((CPconwApp*)AfxGetApp())->RemoveMDIFrame(m_strFormatTitle))
//		{
//			i=200;
//			break;
//		}
//	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CFingerThread, CWinThread)
	//{{AFX_MSG_MAP(CFingerThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerThread message handlers

void CFingerThread::InitWindow(CString &Title)
{
	CFingerDoc *l_FingerDoc;
	CFingerFrame *l_FingerFrame;
	
	l_FingerDoc=(CFingerDoc *)((CPconwApp*)AfxGetApp())->m_FingerDocTemplate->CreateNewDocument();
	//Set the title before creating the view, because settitle sets the finger host.
	l_FingerDoc->SetTitle(Title);
	l_FingerFrame=(CFingerFrame *)((CPconwApp*)AfxGetApp())->m_FingerDocTemplate->CreateNewFrame((CDocument *)l_FingerDoc,NULL);
	((CPconwApp*)AfxGetApp())->m_FingerDocTemplate->InitialUpdateFrame((CFrameWnd *)l_FingerFrame,l_FingerDoc,TRUE);			
	
	m_strFormatTitle.Format("Finger: %s",Title);
	//loop for a while....
	for(int i=0;i<100;i++)
	{
		if(((CPconwApp*)AfxGetApp())->AddMDIFrame(l_FingerFrame,l_FingerDoc,m_strFormatTitle))
		{
			i=200;	
			break;
		}
	}

}
//WSAsyncSelect