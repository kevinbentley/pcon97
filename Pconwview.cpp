// pconwView.cpp : implementation of the CPconwView class
//

#include "stdafx.h"
#include "pconw.h"
#include "util.h"

#include "pconwDoc.h"
#include "pconwView.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CPconwView

IMPLEMENT_DYNCREATE(CPconwView, CRichEditView)

BEGIN_MESSAGE_MAP(CPconwView, CRichEditView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CPconwView)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(IDC_CLEARWINDOW, OnClearwindow)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDC_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_COMMAND(ID_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPconwView construction/destruction

CPconwView::CPconwView()
{
	m_Logging=FALSE;
	m_AutologInit=FALSE;
    m_URLCursor=FALSE;
    l_hc = ((CPconwApp*)AfxGetApp())->LoadCursor(IDC_CURURL);//IDC_CURURL//SetCursor
	l_hcaret = LoadCursor(NULL,IDC_IBEAM);
}

CPconwView::~CPconwView()
{
	if(m_Logging) m_LogFile.Close();
    delete f;
}

BOOL CPconwView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL ret = CRichEditView::PreCreateWindow(cs);
	cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | 
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY;


	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CPconwView drawing

void CPconwView::OnDraw(CDC* pDC)
{
	CPconwDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GetRichEditCtrl().SetBackgroundColor(FALSE,((CPconwApp*)AfxGetApp())->colorBACK);
}

/////////////////////////////////////////////////////////////////////////////
// CPconwView printing

BOOL CPconwView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPconwView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPconwView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPconwView diagnostics

#ifdef _DEBUG
void CPconwView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CPconwView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CPconwDoc* CPconwView::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPconwDoc)));//Also it could be CChatDoc
	return (CPconwDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPconwView message handlers

void CPconwView::Message(LPCTSTR lpszMessage)
{
	int len;
	//if(((CPconwApp*)AfxGetApp())->m_bNewFont) SetNewFont();
	if(!m_AutologInit) 
	{
		if(((CPconwApp*)AfxGetApp())->m_bAutoLog) 
		{
			m_Logging=FALSE;
			ToggleLogging();
		}
		m_AutologInit=TRUE;
	}
	if(m_Logging)
	{
		//Write line to disk
		m_LogFile.Write(lpszMessage,strlen(lpszMessage));
		m_LogFile.Write("\n",sizeof("\n"));
		
	}

	if(((CPconwApp*)AfxGetApp())->ProfanityFilter)
	{
		if(IsProfane(lpszMessage)) return;
	}
	
	GetRichEditCtrl().SetBackgroundColor(FALSE,((CPconwApp*)AfxGetApp())->colorBACK);
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	
	int isbMin,isbMax;
	GetScrollRange(SB_VERT,&isbMin,&isbMax);
	if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax)) 
	{
		len = GetWindowTextLength();
		GetRichEditCtrl().HideSelection(TRUE,FALSE);
		GetRichEditCtrl().ReplaceSel(strTemp);
		
		
		UnderlineURLs(len,lpszMessage);
		GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		GetRichEditCtrl().HideSelection(FALSE,FALSE);
		return;
	}
	len = GetWindowTextLength();
	GetRichEditCtrl().HideSelection(FALSE,FALSE);
	GetRichEditCtrl().SetSel(len,len);
	GetRichEditCtrl().ReplaceSel(strTemp);
	UnderlineURLs(len,lpszMessage);
}

int CPconwView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CHARFORMAT nfcf;
	LOGFONT nflf;

	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRichEditCtrl &ewnd = GetRichEditCtrl();
	f = ReallyCreateFont(NULL, (char *)LPCSTR(((CPconwApp*)AfxGetApp())->FontName), _T("Regular"), ((CPconwApp*)AfxGetApp())->FontSize , 0);
	if(f != NULL)
	{
		ewnd.SetFont(f);
        f->GetLogFont(&nflf);
		nfcf.dwMask=CFM_FACE|CFM_SIZE;
		nfcf.dwEffects=0;
		nfcf.yHeight=nflf.lfHeight;
		nfcf.bCharSet=nflf.lfCharSet;
		nfcf.bPitchAndFamily=nflf.lfPitchAndFamily;
		strcpy(nfcf.szFaceName,nflf.lfFaceName);
		GetRichEditCtrl().SetDefaultCharFormat(nfcf);
	}
	if(((CPconwApp*)AfxGetApp())->m_ChannelDoc==GetDocument())
	{
		CMenu *sysMenu;
		sysMenu=GetSystemMenu(FALSE);
		if(sysMenu) sysMenu->EnableMenuItem(WM_MDIDESTROY,FALSE);
	}	
    //if(((CPconwApp*)AfxGetApp())->m_bAutoLog) ToggleLogging();
	return 0; 	
}

void CPconwView::SetColor (COLORREF NewColor)
{
	CHARFORMAT vw_cf;
	LOGFONT nflf;
	int len = GetWindowTextLength();
	int isbMin,isbMax;
	GetScrollRange(SB_VERT,&isbMin,&isbMax);
	if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax)) 
	{
		GetRichEditCtrl().HideSelection(TRUE,FALSE);
	}
	else GetRichEditCtrl().HideSelection(FALSE,FALSE);
	GetRichEditCtrl().SetSel(len,len);
	f->GetLogFont(&nflf);
	vw_cf.dwMask=CFM_FACE|CFM_SIZE|CFM_COLOR;
	vw_cf.yHeight=nflf.lfHeight;
	vw_cf.bCharSet=nflf.lfCharSet;
	vw_cf.bPitchAndFamily=nflf.lfPitchAndFamily;
	strcpy(vw_cf.szFaceName,nflf.lfFaceName);
	//GetRichEditCtrl().SetDefaultCharFormat(nfcf);
	vw_cf.crTextColor = NewColor;
	SetCharFormat(vw_cf);
	//GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
}

BOOL CPconwView::IsProfane(LPCTSTR Msg)
{
	CString LowerMsg;

	LowerMsg = Msg;
	LowerMsg.MakeLower();

	if(LowerMsg.Find("fuck")==-1)
	{
		if(LowerMsg.Find("shit")==-1) return FALSE;
		else return TRUE;
	}
	else return TRUE;
}


void CPconwView::SetNewFont(void)
{
	CHARFORMAT nfcf;
	LOGFONT nflf;
	//GetParent
	CRichEditCtrl &ewnd = GetRichEditCtrl();
	f = ReallyCreateFont(NULL, (char *)LPCSTR(((CPconwApp*)AfxGetApp())->FontName), _T("Regular"), ((CPconwApp*)AfxGetApp())->FontSize , 0);
	if(f != NULL)
	{
		ewnd.SetFont(f);
		f->GetLogFont(&nflf);
		nfcf.dwMask=CFM_FACE|CFM_SIZE;
		nfcf.dwEffects=0;
		nfcf.yHeight=nflf.lfHeight;
		nfcf.bCharSet=nflf.lfCharSet;
		nfcf.bPitchAndFamily=nflf.lfPitchAndFamily;
		strcpy(nfcf.szFaceName,nflf.lfFaceName);
		GetRichEditCtrl().SetDefaultCharFormat(nfcf);
		//Hide the cursor
		GetRichEditCtrl().HideSelection(TRUE,FALSE);
		//Save the current selection
		GetRichEditCtrl().GetSel(l_iStart,l_iEnd);
		//highlight everything
		GetRichEditCtrl().SetSel(0,-1);
		//set the new format...
		GetRichEditCtrl().SetSelectionCharFormat(nfcf);
		//set the old selections
		GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		//show the cursor
		GetRichEditCtrl().HideSelection(FALSE,FALSE);
	}
}

void CPconwView::OnDropFiles(HDROP hDropInfo) 
{
	//Supports only one file!
	CPconwDoc *ThisDocument;
	CString DraggedFile;
	ThisDocument = GetDocument();
	if(!ThisDocument) return;
			
   UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

   for (UINT iFile = 0; iFile < nFiles; iFile++)
   {
      TCHAR szFileName[_MAX_PATH];

      ::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
	
      DraggedFile.Format("/offer %s",szFileName);
   }
   ::DragFinish(hDropInfo);
   
   ThisDocument->SetSendText((char *)LPCSTR(DraggedFile));
}

void CPconwView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CPconwDoc* ThisDocument;
	ThisDocument=GetDocument();
	ThisDocument->ActivateSendVw(nChar);
}



void CPconwView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_PCONW_VIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL CPconwView::PreTranslateMessage(MSG* pMsg)
{
	// CG: This block was added by the Pop-up Menu component
	{
		// Shift+F10: show pop-up menu.
		if ((((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) && // If we hit a key and
			(pMsg->wParam == VK_F10) && (GetKeyState(VK_SHIFT) & ~1)) != 0) ||	// it's Shift+F10 OR
			(pMsg->message == WM_CONTEXTMENU))									// Natural keyboard key
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}
		if(pMsg->message==WM_RBUTTONDOWN)
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);
			CPoint pt(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
            pt.Offset(rect.TopLeft());
			OnContextMenu (this,pt);

		}
		if(pMsg->message==WM_LBUTTONUP)
		{
			GetRichEditCtrl().Copy();
		}
		if((pMsg->message==WM_SYSKEYDOWN)&&(pMsg->wParam==VK_F10))
		{
			OnKeyDown(VK_F10,0,0);
			return TRUE;
		}

	}

	return CRichEditView::PreTranslateMessage(pMsg);
}

void CPconwView::OnEditCopy() 
{
	GetRichEditCtrl().Copy();	
}

void CPconwView::OnClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");
	
}

void CPconwView::OnFileClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");	
}

void CPconwView::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CPconwView::ToggleLogging(void)
{
	if(m_Logging)
	{
		m_LogFile.Close();
	//	delete m_LogFile;
//		m_LogFile = NULL;
		m_Logging = FALSE;
	}
	else
	{
		CPconwDoc* ThisDocument;
		ThisDocument=GetDocument();
		m_LogName=ThisDocument->m_Winname;

		//m_LogFile = new CFile;
		//if(m_LogFile) 
		{
			
			TRY
			{
			m_LogFile.Open(m_LogName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite);
            m_LogFile.Seek(0,CFile::end);
			m_Logging=TRUE;
			//CFileException e;
			//if(m_LogFile->Open(m_LogName,CFile::modeWrite|CFile::typeText,&e)) 
			//else MessageBox("Unable to Create Log File","Error",MB_OK|MB_ICONEXCLAMATION);
			}
			CATCH( CFileException, e )
			{
				CString l_tmpmsgerr,l_tmpmsgbl;
				l_tmpmsgerr.LoadString(IDS_STRBADLOG);
				l_tmpmsgbl.LoadString(IDS_STRERROR);
				MessageBox(l_tmpmsgerr,l_tmpmsgbl,MB_OK|MB_ICONEXCLAMATION);
				//m_LogFile=NULL;
				m_Logging=FALSE;
				return;
			}
			END_CATCH  
		}
	/*	else
		{
				MessageBox("Unable to Create Log File","Error",MB_OK|MB_ICONEXCLAMATION);
				//m_LogFile=NULL;
				m_Logging=FALSE;
				return;
		}
	*/	

	}
}

BOOL CPconwView::GetLogState(void)
{
	return m_Logging;
}

void CPconwView::SetNewLogName(LPCSTR FileName)
{
	if(m_Logging) m_LogFile.Close();
//	delete m_LogFile;
	m_LogName=FileName;
	m_LogFile.Open(m_LogName,CFile::modeWrite);
/*
	TRY
	{
	//m_LogFile = new CFile(FileName,CFile::modeWrite);
	}
	CATCH( CFileException, e )
	{
		MessageBox("Unable to Create Log File","Error",MB_OK|MB_ICONEXCLAMATION);
		m_LogFile=NULL;
	}
	END_CATCH
*/
}

void CPconwView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar)
	{
	case VK_F2:
	case VK_F3:
	case VK_F4:
	case VK_F5:
	case VK_F6:
	case VK_F7:
	case VK_F8:
	case VK_F9:
	case VK_F10:
	case VK_F11:
	case VK_F12:
		CPconwDoc* ThisDocument;
		ThisDocument=GetDocument();
		ThisDocument->SendSysKey(nChar);
		break;
	default:
		CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	

}

void CPconwView::OnMyKeyDown(UINT nChar)
{
	CRichEditView::OnKeyDown(nChar,0,0);
}

#define D_VIEW_CLASS CPconwView
//UnderlineURL and OnLButtonDown functions included in the file below.
#include "UnderlineURL.hpp"

