// DCCFDoc.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"

#include "DCCFDoc.h"
//#include "DCCFView.h"
//#include "DCCFFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCCFDoc

IMPLEMENT_DYNCREATE(CDCCFDoc, CDocument)

CDCCFDoc::CDCCFDoc()
{
}

BOOL CDCCFDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDCCFDoc::~CDCCFDoc()
{
}


BEGIN_MESSAGE_MAP(CDCCFDoc, CDocument)
	//{{AFX_MSG_MAP(CDCCFDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCFDoc diagnostics

#ifdef _DEBUG
void CDCCFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCCFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCCFDoc serialization

void CDCCFDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CDCCFDoc commands
