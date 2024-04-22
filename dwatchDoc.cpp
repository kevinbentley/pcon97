// dwatchDoc.cpp : implementation of the CDwatchDoc class
//

#include "stdafx.h"
//#include "dwatch.h"
#include "pconw.h"
#include "dwatchDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDwatchDoc

IMPLEMENT_DYNCREATE(CDwatchDoc, CDocument)

BEGIN_MESSAGE_MAP(CDwatchDoc, CDocument)
	//{{AFX_MSG_MAP(CDwatchDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDwatchDoc construction/destruction

CDwatchDoc::CDwatchDoc()
{
	CString l_Title;
	l_Title = "Descent(tm) Network Game Spy";
	SetTitle(l_Title);
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(l_Title,this);
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(l_Title);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.InsertTab(l_Title);
}

CDwatchDoc::~CDwatchDoc()
{
}

BOOL CDwatchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDwatchDoc serialization

void CDwatchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDwatchDoc diagnostics

#ifdef _DEBUG
void CDwatchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDwatchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDwatchDoc commands

void CDwatchDoc::OnCloseDocument() 
{
	((CPconwApp*)AfxGetApp())->m_bDSpyRunning=0;
	CString l_Title;
	l_Title = "Descent(tm) Network Game Spy";
	((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(l_Title);
	((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(l_Title);
	((CMainFrame *)AfxGetMainWnd())->m_TabBar.RemoveTab(l_Title);
	CDocument::OnCloseDocument();
}
