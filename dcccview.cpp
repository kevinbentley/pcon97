// pconwView.cpp : implementation of the CDCCCView class
//

#include "stdafx.h"
#include "pconw.h"
#include "util.h"

#include "DCCCDoc.h"
#include "DCCCFrm.h"
#include "DCCCSend.h"
#include "DCCCView.h"

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CDCCCView

IMPLEMENT_DYNCREATE(CDCCCView, CRichEditView)

BEGIN_MESSAGE_MAP(CDCCCView, CRichEditView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CDCCCView)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(IDC_CLEARWINDOW, OnClearwindow)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLEARWINDOW, OnUpdateFileClearwindow)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(IDC_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_COMMAND(ID_FILE_CLEARWINDOW, OnFileClearwindow)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCCView construction/destruction

CDCCCView::CDCCCView()
{
	m_DCCChatSocket=NULL;
	m_Logging=FALSE;
	m_AutologInit=FALSE;
	l_hc = ((CPconwApp*)AfxGetApp())->LoadCursor(IDC_CURURL);//IDC_CURURL
	l_hcaret = ((CPconwApp*)AfxGetApp())->LoadStandardCursor(IDC_IBEAM);
}
CDCCCView::~CDCCCView()
{
	if(m_Logging) m_LogFile.Close();
    delete f;
}

BOOL CDCCCView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	BOOL ret = CRichEditView::PreCreateWindow(cs);
	
	cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | 
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY;

	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// CDCCCView drawing

void CDCCCView::OnDraw(CDC* pDC)
{
	CDCCCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GetRichEditCtrl().SetBackgroundColor(FALSE,((CPconwApp*)AfxGetApp())->colorBACK);
}

/////////////////////////////////////////////////////////////////////////////
// CDCCCView printing

BOOL CDCCCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDCCCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDCCCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDCCCView diagnostics

#ifdef _DEBUG
void CDCCCView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CDCCCView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CDCCCDoc* CDCCCView::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDCCCDoc)));//Also it could be CDCCCDoc
	return (CDCCCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCCCView message handlers

void CDCCCView::Message(LPCTSTR lpszMessage)
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

int CDCCCView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void CDCCCView::SetColor (COLORREF NewColor)
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

BOOL CDCCCView::IsProfane(LPCTSTR Msg)
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


void CDCCCView::SetNewFont(void)
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

void CDCCCView::OnDropFiles(HDROP hDropInfo) 
{
	//Supports only one file!
	CDCCCDoc *ThisDocument;
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

void CDCCCView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDCCCDoc* ThisDocument;
	ThisDocument=GetDocument();
	ThisDocument->ActivateSendVw(nChar);
}



void CDCCCView::OnContextMenu(CWnd*, CPoint point)
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

BOOL CDCCCView::PreTranslateMessage(MSG* pMsg)
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

void CDCCCView::OnEditCopy() 
{
	GetRichEditCtrl().Copy();	
}

void CDCCCView::OnClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");
	
}

void CDCCCView::OnFileClearwindow() 
{
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().ReplaceSel("");	
}

void CDCCCView::OnUpdateFileClearwindow(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}

void CDCCCView::ToggleLogging(void)
{
	if(m_Logging)
	{
		m_LogFile.Close();
		m_Logging = FALSE;
	}
	else
	{
		CDCCCDoc* ThisDocument;
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

BOOL CDCCCView::GetLogState(void)
{
	return m_Logging;
}

void CDCCCView::SetNewLogName(LPCSTR FileName)
{
	if(m_Logging) m_LogFile.Close();
	m_LogName=FileName;
	m_LogFile.Open(m_LogName,CFile::modeWrite);

}

void CDCCCView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
		CDCCCDoc* ThisDocument;
		ThisDocument=GetDocument();
		ThisDocument->SendSysKey(nChar);
		break;
	default:
		CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
	

}

void CDCCCView::OnMyKeyDown(UINT nChar)
{
	CRichEditView::OnKeyDown(nChar,0,0);
}

#define D_VIEW_CLASS CDCCCView
//UnderlineURL, OnLButtonDown, etc functions included in the file below.
#include "UnderlineURL.hpp"



void CDCCCView::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
	SetColor(((CPconwApp*)AfxGetApp())->colorSTAT);
	Message("Attempting to establish DCC Connection...");

	
}

void CDCCCView::OnAcceptConn()
{
	m_DCCChatSocket = new CDCCSocket(this,DCC_CHAT_LISTEN);//DCC_CHAT_ACCEPT

	((CDCCCDoc*)GetDocument())->m_ListenSocket->Accept(*m_DCCChatSocket);
	m_DCCChatSocket->AsyncSelect(FD_READ|FD_CLOSE);
	SetColor(((CPconwApp*)AfxGetApp())->colorSTAT);
	Message("Connection established...");
}

int CDCCCView::SetDCCCOptions(int Mode)
{
	if(Mode==CHAT_INITIATE)
	{
		unsigned int port;
		CString l_SockAddr;
		struct in_addr IpAddressStruct;
		((CDCCCDoc*)GetDocument())->m_ListenSocket = new CDCCSocket(this,DCC_CHAT_LISTEN);
		((CDCCCDoc*)GetDocument())->m_ListenSocket->Create(0,SOCK_STREAM,FD_ACCEPT);
		((CDCCCDoc*)GetDocument())->m_ListenSocket->GetSockName(l_SockAddr,port);
		//int l_error = ((CDCCCDoc*)GetDocument())->m_ListenSocket->GetLastError();
		((CDCCCDoc*)GetDocument())->m_ListenSocket->Listen(1);

		return port;
	}
	//if(Mode==CHAT_INITIATE)
	//{

	//	return NULL;
	//}
	return NULL;

}

void CDCCCView::OnReceive()
{
	char l_BuffA[2000];
	int l_Len;
	for(int j=0;j<2000;j++) l_BuffA[j]=NULL;
	do
	{
		l_Len=m_DCCChatSocket->Receive(&l_BuffA,1999);
		if(l_Len>=1)
		{
			l_BuffA[l_Len]=NULL;
			m_TCPBuffA+=l_BuffA;
		}

	}while(l_Len>0);
	//Now Dispatch....

	int l_PosA=0;
	int	l_PosB=0;
	CString l_DispatchStr;
	int l_StrLen=m_TCPBuffA.GetLength();

	for(int i=0;i<l_StrLen;i++)
	{
		if((m_TCPBuffA[i]==0x0d)||(m_TCPBuffA[i]==0x0a))
		{
			l_PosB=i;//Skip the CR
			//ASSERT(l_PosB-l_PosA);
			l_DispatchStr.Empty();
			l_DispatchStr=m_TCPBuffA.Mid(l_PosA,l_PosB-l_PosA);
			//l_PosA=i+2;//Next char after the CR
			l_PosA=i++;
			//l_DispatchStr += "\n";
			l_DispatchStr.TrimLeft();
			l_DispatchStr.TrimRight();
			ParseDCCData(l_DispatchStr);
		}
	}
	m_TCPBuffB.Empty();
	m_TCPBuffB=m_TCPBuffA.Mid(l_PosA,m_TCPBuffA.GetLength()-l_PosA);
	m_TCPBuffA=m_TCPBuffB;


	/*
	//send the data to the view
	char l_BuffA[2000];
	int l_Len;
	l_Len=m_DCCChatSocket->Receive(&l_BuffA,1999);
	if(l_Len>=1)
	{
		//
		int l_ilen;
		l_BuffA[l_Len]=0;
		l_ilen = GetWindowTextLength();
		GetRichEditCtrl().SetSel(l_ilen,l_ilen);
		GetRichEditCtrl().ReplaceSel(l_BuffA);
		UnderlineURLs(l_ilen,l_BuffA);
		//CSocket
	}*/
}

void CDCCCView::OnDestroy() 
{
	if(m_DCCChatSocket) delete m_DCCChatSocket;
	if(((CDCCCDoc*)GetDocument())->m_ListenSocket) delete ((CDCCCDoc*)GetDocument())->m_ListenSocket;
	CRichEditView::OnDestroy();

}

void CDCCCView::ParseDCCData(CString Line)
{
		CString l_FmtLine,l_Peer,l_strTest;
		int l_ilen;
		
		l_strTest=Line.Left(7);
		CDCCCDoc* ThisDocument;
		ThisDocument=GetDocument();
		l_Peer=ThisDocument->m_Winname;
		if(Line.IsEmpty()) return;
		if(l_strTest.Compare("\x01\ACTION")==0)
		{
			l_strTest=Line.Mid(7,Line.GetLength()-8);

			l_FmtLine.Format("* %s%s",l_Peer,l_strTest);
			l_FmtLine.TrimLeft();
			l_FmtLine.TrimRight();
			SetColor(((CPconwApp*)AfxGetApp())->colorACT);
			Message(LPCSTR(l_FmtLine));
		}
		else 
		{
			l_FmtLine.Format("<%s> %s",l_Peer,Line);
			l_FmtLine.TrimLeft();
			l_FmtLine.TrimRight();
			SetColor(((CPconwApp*)AfxGetApp())->colorNORM);
			Message(LPCSTR(l_FmtLine));
		}
}

void CDCCCView::SendMsg(CString strMsg)
{
	CString l_FmtLine,l_strTest,l_strCR;
	int l_ilen;
	if(!m_DCCChatSocket) return;
	if(strMsg.IsEmpty()) return;
	l_strCR=strMsg;
	l_strCR+="\n";
	m_DCCChatSocket->Send(LPCSTR(l_strCR),l_strCR.GetLength());
		
	l_strTest=strMsg.Left(7);
	CDCCCDoc* ThisDocument;
	ThisDocument=GetDocument();
		
	if(l_strTest.Compare("\x01\ACTION")==0)
	{
		l_strTest=strMsg.Mid(7,strMsg.GetLength()-8);

		l_FmtLine.Format("* %s%s",((CPconwApp*)AfxGetApp())->m_strHandle,l_strTest);
		l_FmtLine.TrimLeft();
		l_FmtLine.TrimRight();
		SetColor(((CPconwApp*)AfxGetApp())->colorACT);
		Message(LPCSTR(l_FmtLine));
	}
	else 
	{
		l_FmtLine.Format("<%s> %s",((CPconwApp*)AfxGetApp())->m_strHandle,strMsg);
		l_FmtLine.TrimLeft();
		l_FmtLine.TrimRight();
		SetColor(((CPconwApp*)AfxGetApp())->colorNORM);
		Message(LPCSTR(l_FmtLine));
	}


}

void CDCCCView::OnConnectChat()
{
	SetColor(((CPconwApp*)AfxGetApp())->colorSTAT);
	Message("Connection established...");
}

void CDCCCView::ConnectDCCChat(UINT ip, short socket)
{
	UINT l_nIP;
	l_nIP=htonl(ip);
	struct in_addr IpAddressStruct;
	memcpy(&IpAddressStruct.S_un.S_addr,&l_nIP,4);
	m_DCCChatSocket = new CDCCSocket(this,DCC_CHAT_LISTEN);
	m_DCCChatSocket->Create(0,SOCK_STREAM,FD_READ|FD_CLOSE|FD_CONNECT);
	m_DCCChatSocket->Connect(inet_ntoa(IpAddressStruct),socket);
	//SetColor(((CPconwApp*)AfxGetApp())->colorSTAT);
	//Message("Attempting to establish DCC Connection...");
	
}

void CDCCCView::OnChatClose()
{
	SetColor(((CPconwApp*)AfxGetApp())->colorSTAT);
	Message("Chat Connection closed.");
}
