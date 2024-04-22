// SendView.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "SendView.h"
#include "util.h"

#include "cuser.h"

/////////////////////////////////////////////////////////////////////////////
// CSendView

IMPLEMENT_DYNCREATE(CSendView, CEditView)

CSendView::CSendView()
{
	iScrollCmd = 0;
	iCurrCmdPos = 0;
	f = NULL;

}

CSendView::~CSendView()
{
    delete f;
}


BEGIN_MESSAGE_MAP(CSendView, CEditView)
	//{{AFX_MSG_MAP(CSendView)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendView drawing

void CSendView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSendView diagnostics

#ifdef _DEBUG
void CSendView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSendView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSendView message handlers

BOOL CSendView::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL ret = CEditView::PreCreateWindow(cs);
	cs.style = AFX_WS_DEFAULT_VIEW | ES_NOHIDESEL | ES_AUTOHSCROLL;
	return ret;
}



int CSendView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CEdit &ewnd = GetEditCtrl();
	f = ReallyCreateFont(NULL, (char *)LPCSTR(((CPconwApp*)AfxGetApp())->FontName), _T("Regular"), ((CPconwApp*)AfxGetApp())->FontSize , 0);
	if(f != NULL)
		ewnd.SetFont(f);
	ewnd.LimitText(250);
	return 0;
}

void CSendView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar != VK_RETURN) || (nRepCnt!=1))
	{
				
		if(nChar == VK_ESCAPE) 
		{
			CString strText;
			strText=_T("");
			GetEditCtrl().SetWindowText(strText);	
			return;
		}
		if(nChar == VK_TAB)
		{
			CString strText;
			char *pcTabUser;
			pcTabUser=NULL;

			pcTabUser=((CPconwApp*)AfxGetApp())->GetNextTabUser();
			if(pcTabUser==NULL) 
			{
				MessageBeep((unsigned int) -1);
				return;
			}
			if(strlen(pcTabUser)==0) return;
			strText = _T("/msg \"");
			strText += pcTabUser;
			strText += _T("\" ");
			GetEditCtrl().SetWindowText(strText);	
			int len = GetWindowTextLength();
			GetEditCtrl().SetSel(len,len);
			return;		
		}

		
		CEditView::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	else
	{
			
		CPconwDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		
		CString strText;
		GetEditCtrl().GetWindowText(strText);
		CmdHist[iCurrCmdPos] = strText;
		iCurrCmdPos++;
		if(iCurrCmdPos==MAX_OLD_COMMANDS) iCurrCmdPos=0;
		iScrollCmd=iCurrCmdPos;
		CString l_blank;
		l_blank.Empty();
		GetEditCtrl().SetWindowText(l_blank);
		pDoc->SetSendText(strText);

		
		
		
	}
		
}

void CSendView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//VK_PRIOR
	//VK_NEXT
	if(nChar == VK_PRIOR) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SendPageKey(nChar);
			return;
		}
	if(nChar == VK_NEXT) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SendPageKey(nChar);
			return;
		}
	if(nChar == VK_F2) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F2key));				
			return;
		}
	if(nChar == VK_F3) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F3key));				
			return;
		}
	if(nChar == VK_F4) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F4key));				
			return;
		}
	if(nChar == VK_F5) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F5key));				
			return;
		}
	if(nChar == VK_F6) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F6key));				
			return;
		}
	if(nChar == VK_F7) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F7key));				
			return;
		}
	if(nChar == VK_F8) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F8key));				
			return;
		}
	if(nChar == VK_F9) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F9key));				
			return;
		}
	if(nChar == VK_F10) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F10key));				
			return;
		}
	if(nChar == VK_F11) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F11key));				
			return;
		}
	if(nChar == VK_F12) 
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F12key));				
			return;
		}
	if(nChar == VK_DOWN)
		{
			int iOldPos;
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			CString strText;
			
			//GetEditCtrl().GetWindowText(strText);
			//CmdHist[iScrollCmd] = strText;
			if(iScrollCmd==iCurrCmdPos) return;
			iOldPos = iScrollCmd;
			iScrollCmd++;
			if(iScrollCmd==MAX_OLD_COMMANDS) iScrollCmd=0;
			
			strText = CmdHist[iScrollCmd];
			if(strText.IsEmpty())  
			{
				iScrollCmd = iOldPos;
				MessageBeep((unsigned int)-1);
				return;
			}
			GetEditCtrl().SetWindowText(strText);
			int len = GetWindowTextLength();
			GetEditCtrl().SetSel(len,len);
			return;
		}
		if(nChar == VK_UP)
		{
			int iOldPos;
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			
			CString strText;
			iOldPos = iScrollCmd;
			iScrollCmd--;
			if(iScrollCmd<0) iScrollCmd=(MAX_OLD_COMMANDS-1);

			strText = CmdHist[iScrollCmd];
			if(strText.IsEmpty())  
			{
				iScrollCmd = iOldPos;
				MessageBeep((unsigned int)-1);
				return;
			}
			GetEditCtrl().SetWindowText(strText);
			int len = GetWindowTextLength();
			GetEditCtrl().SetSel(len,len);
			return; 
		}
	
	
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSendView::OnMyChar(UINT nchar)
{
	//CWnd *l_ParentWnd;
	//l_ParentWnd=GetParent();
	if(nchar) OnChar(nchar,1,0);
	SetFocus();
	//else if(l_ParentWnd) l_ParentWnd->BringWindowToTop();
}

CString CSendView::ExpandString(CString OrigStr)
{
	CString m_TmpStra,m_TmpStrb;
	CString l_KeyIP,l_KeyLongIP,l_KeyUser;
	l_KeyIP.LoadString(IDS_KEYIP);
	l_KeyLongIP.LoadString(IDS_KEYLONGIP);
	l_KeyUser.LoadString(IDS_KEYUSER);

	m_TmpStra = _T("");
	m_TmpStrb = _T("");
	int iStart;
	m_TmpStra=OrigStr;
	unsigned long l_UserIP;
	union s_IPa
	{
		unsigned long lIP;
		unsigned char cIP[4];
	}IpAddr;
	
	unsigned int IPa,IPb,IPc,IPd;
	

	do
	{
		iStart=m_TmpStra.Find(l_KeyUser);
		if(iStart!=-1) 
		{
			m_TmpStrb=m_TmpStra.Left(iStart);
			m_TmpStrb += ((CPconwDoc *)GetDocument())->m_SelectedString;
			m_TmpStrb += m_TmpStra.Mid(iStart+l_KeyUser.GetLength());	
			m_TmpStra=m_TmpStrb;
		}
	}while(iStart!=-1);
	
	do
	{
		iStart=m_TmpStra.Find(l_KeyIP);
		if(iStart!=-1) 
		{
			CString l_DotIP;
			CUser *u;
			//lookup  the user, get the IP, and convert it to ascii	dotted	
			((CPconwApp*)AfxGetApp())->Users.Lookup(((CPconwDoc *)GetDocument())->m_SelectedString,(class CObject*&)u);
			if(!u) break;
			l_UserIP=u->GetUserIP();						
			IpAddr.lIP=l_UserIP;
			IPa=IpAddr.cIP[0];
			IPb=IpAddr.cIP[1];
			IPc=IpAddr.cIP[2];
			IPd=IpAddr.cIP[3];

			l_DotIP.Format("%d.%d.%d.%d",IPa,IPb,IPc,IPd);
			m_TmpStrb=m_TmpStra.Left(iStart);
			m_TmpStrb += l_DotIP;
			m_TmpStrb += m_TmpStra.Mid(iStart+l_KeyIP.GetLength());	
			m_TmpStra=m_TmpStrb;
		}
	}while(iStart!=-1);
	do
	{
		iStart=m_TmpStra.Find(l_KeyLongIP);
		if(iStart!=-1) 
		{
			CString l_LongIP;
			CUser *u;
			((CPconwApp*)AfxGetApp())->Users.Lookup(((CPconwDoc *)GetDocument())->m_SelectedString,(class CObject*&)u);
			if(!u) break;
			
			l_UserIP=ntohl(u->GetUserIP());
			l_LongIP.Format("%u",l_UserIP);
			//lookup  the user, get the IP, and convert it to ascii long
			m_TmpStrb=m_TmpStra.Left(iStart);
			m_TmpStrb += l_LongIP;
			m_TmpStrb += m_TmpStra.Mid(iStart+l_KeyLongIP.GetLength());	
			m_TmpStra=m_TmpStrb;
		}
	}while(iStart!=-1);
	
	return m_TmpStra;

}

CPconwDoc * CSendView::GetDocument(void)
{
	return (CPconwDoc*)m_pDocument;
}

void CSendView::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
	if(GetDocument()==((CPconwApp*)AfxGetApp())->m_ChannelDoc) pCmdUI->Enable(FALSE);
}

//GetSystemMenu

BOOL CSendView::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->message==WM_SYSKEYDOWN)&&(pMsg->wParam==VK_F10))
		{
			CPconwDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->SetSendText(ExpandString(((CPconwApp*)AfxGetApp())->F10key));				
			return TRUE;
		}	
	return CEditView::PreTranslateMessage(pMsg);
}

void CSendView::OnMyKeyDown(UINT nChar)
{
	SetFocus();
	OnKeyDown(nChar,1,0);
}
