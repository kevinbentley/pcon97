// FingerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "FingerDoc.h"
#include "FingerSocket.h"
#include "FingerView.h"
#include "FingerFrame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc

IMPLEMENT_DYNCREATE(CFingerDoc, CDocument)

CFingerDoc::CFingerDoc()
{
	m_strFinger=((CPconwApp*)AfxGetApp())->m_CreateNewDocTitle;//This is the finger request!
	
	m_strFingerTitle.Format("Finger: %s",m_strFinger);
	SetTitle(m_strFingerTitle);
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(m_strFingerTitle,this);
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_strFingerTitle);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.InsertTab(m_strFingerTitle);
}

BOOL CFingerDoc::OnNewDocument()
{
	
	if (!CDocument::OnNewDocument())
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

CFingerDoc::~CFingerDoc()
{
}


BEGIN_MESSAGE_MAP(CFingerDoc, CDocument)
	//{{AFX_MSG_MAP(CFingerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc diagnostics

#ifdef _DEBUG
void CFingerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFingerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc serialization

void CFingerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}




void CFingerDoc::SetTitle(CString & WindowTitle)
{
	CString l_strFingerTitle;
	m_strFinger=WindowTitle;
	l_strFingerTitle.Format("Finger: %s",WindowTitle);
//	SetTitle(l_strFingerTitle);
	CDocument::SetTitle(WindowTitle);
}

void CFingerDoc::OnCloseDocument() 
{
	CString l_strFingerTitle;
	((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_strFingerTitle);
	((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(m_strFingerTitle);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.RemoveTab(m_strFingerTitle);
	//l_strFingerTitle.Format("Finger: %s",m_strFinger);
	//loop for a while....
	/*
	for(int i=0;i<100;i++)
	{
		if(((CPconwApp*)AfxGetApp())->RemoveMDIFrame(m_strFinger))
		{
			i=200;	
			break;
		}
	}	
	*/
	CDocument::OnCloseDocument();
}

void CFingerDoc::ActivateSendVw(UINT nchar)
{
	CFingerView *pSendView;
	CFingerFrame *pChildFrame;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CFingerView)))
		{
			//pSendView=(CFingerSend *)pView;
			//pSendView->OnMyChar(nchar);
			//((CFrameWnd*)AfxGetMainWnd())->SetActiveView(pSendView,TRUE);
			pChildFrame=(CFingerFrame *)pSendView->GetParentFrame();
			if(pChildFrame)
			{
				pChildFrame->MDIActivate();
				pSendView->SetForegroundWindow();
			}
		}	
	}
}

