// pconwView.cpp : implementation of the CChatView class
//

#include "stdafx.h"
#include "pconw.h"
#include "util.h"

#include "chatDoc.h"
#include "chatView.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CChatView

IMPLEMENT_DYNCREATE(CChatView, CRichEditView)

BEGIN_MESSAGE_MAP(CChatView, CRichEditView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CChatView)
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
// CChatView construction/destruction

CChatView::CChatView()
{
	m_Logging=FALSE;
	m_AutologInit=FALSE;
	l_hc = ((CPconwApp*)AfxGetApp())->LoadCursor(IDC_CURURL);//IDC_CURURL
	l_hcaret = ((CPconwApp*)AfxGetApp())->LoadStandardCursor(IDC_IBEAM);
}
CChatView::~CChatView()
{
	if(m_Logging) m_LogFile.Close();
    delete f;
}

BOOL CChatView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	

	BOOL ret = CRichEditView::PreCreateWindow(cs);
	
/*
	WNDCLASS wndcls;

    if (cs.lpszClass != NULL &&
            GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls))
    {
        // register a very similar WNDCLASS
        cs.lpszClass = AfxRegisterWndClass(
                            wndcls.style,

                            NULL,  // <-- NULL default cursor!!!!
                            //wndcls.hCursor,

                            wndcls.hbrBackground,
                            wndcls.hIcon);
    }

	
	*/
	cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | 
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY;

	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CChatView drawing

void CChatView::OnDraw(CDC* pDC)
{
	CChatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GetRichEditCtrl().SetBackgroundColor(FALSE,((CPconwApp*)AfxGetApp())->colorBACK);
}

/////////////////////////////////////////////////////////////////////////////
// CChatView printing

BOOL CChatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CChatView diagnostics

#ifdef _DEBUG
void CChatView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CChatView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CChatDoc* CChatView::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChatDoc)));//Also it could be CChatDoc
	return (CChatDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatView message handlers

void CChatView::Message(LPCTSTR lpszMessage)
{
//	HWND l_hwnd1 = GetRichEditCtrl().m_hWnd;
	//DWORD l_dw1 = SetClassLong(m_hWnd,GCL_HCURSOR,NULL);//GetRichEditCtrl().
	//l_dw1 = SetClassLong(GetRichEditCtrl().m_hWnd,GCL_HCURSOR,(long)l_hc);
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
	if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax)) //GetScrollPos(SB_VERT)
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

int CChatView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void CChatView::SetColor (COLORREF NewColor)
{
	CHARFORMAT vw_cf;
	LOGFONT nflf;
	int len = GetWindowTextLength();
	GetRichEditCtrl().GetSel(l_iStart,l_iEnd);
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

BOOL CChatView::IsProfane(LPCTSTR Msg)
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


void CChatView::SetNewFont(void)
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

void CChatView::OnDropFiles(HDROP hDropInfo) 
{
	//Supports only one file!
	CChatDoc *ThisDocument;
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

void CChatView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CChatDoc* ThisDocument;
	ThisDocument=GetDocument();
	ThisDocument->ActivateSendVw(nChar);
}



void CChatView::OnContextMenu(CWnd*, CPoint point)
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

BOOL CChatView::PreTranslateMessage(MSG* pMsg)
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

void CChatView::OnEditCopy() 
{
	GetRichEditCtrl().Copy();	
}

void CChatView::OnClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");
	
}

void CChatView::OnFileClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");	
}

void CChatView::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CChatView::ToggleLogging(void)
{
	if(m_Logging)
	{
		m_LogFile.Close();
		m_Logging = FALSE;
	}
	else
	{
		CChatDoc* ThisDocument;
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
			}
			CATCH( CFileException, e )
			{
				CString l_tmpmsgerr,l_tmpmsgbl;
				l_tmpmsgerr.LoadString(IDS_STRBADLOG);
				l_tmpmsgbl.LoadString(IDS_STRERROR);
				MessageBox(l_tmpmsgerr,l_tmpmsgbl,MB_OK|MB_ICONEXCLAMATION);
				m_Logging=FALSE;
				return;
			}
			END_CATCH  
		}
	

	}
}

BOOL CChatView::GetLogState(void)
{
	return m_Logging;
}

void CChatView::SetNewLogName(LPCSTR FileName)
{
	if(m_Logging) m_LogFile.Close();
	m_LogName=FileName;
	m_LogFile.Open(m_LogName,CFile::modeWrite);

}

void CChatView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
		CChatDoc* ThisDocument;
		ThisDocument=GetDocument();
		ThisDocument->SendSysKey(nChar);
		break;
	default:
		CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	

}

void CChatView::OnMyKeyDown(UINT nChar)
{
	CRichEditView::OnKeyDown(nChar,0,0);
}

#define D_VIEW_CLASS CChatView
//UnderlineURL and OnLButtonDown functions included in the file below.
#include "UnderlineURL.hpp"

