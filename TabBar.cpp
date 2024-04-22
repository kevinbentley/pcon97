// TabBar.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "TabBar.h"
#include "NewMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TCBUFFERSIZE 300

/////////////////////////////////////////////////////////////////////////////
// CTabBar dialog


CTabBar::CTabBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()//CTabBar::IDD)
{
	//{{AFX_DATA_INIT(CTabBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabBar)
	DDX_Control(pDX, IDC_TAB2, m_ColorTab);
	//}}AFX_DATA_MAP
}//CTabCtrl


BEGIN_MESSAGE_MAP(CTabBar, CDialogBar)
	//{{AFX_MSG_MAP(CTabBar)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, OnSelchangeTab2)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabBar message handlers

void CTabBar::OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TC_ITEM l_TC;
	char szBuffer[TCBUFFERSIZE];
	l_TC.cchTextMax=TCBUFFERSIZE;
	int l_Sel;
	l_TC.pszText = szBuffer;
	l_TC.mask = TCIF_TEXT;
	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	ASSERT (m_TabCtrl);
	l_Sel = m_TabCtrl->GetCurSel();
	m_TabCtrl->GetItem(l_Sel,&l_TC);
	
	
	CView *l_pActivateView;
	CDocument *l_pActiveDoc;
	CMDIChildWnd* l_ActivateFrame;
	POSITION l_posa;
	
	POSITION pos;
	CString key;
	for( pos = ((CPconwApp*)AfxGetApp())->m_WindowMap.GetStartPosition(); pos != NULL; )
	{
		((CPconwApp*)AfxGetApp())->m_WindowMap.GetNextAssoc( pos, key, (CObject*&)l_pActiveDoc );
		if(key.CompareNoCase(szBuffer)==0)
		{
			l_posa=l_pActiveDoc->GetFirstViewPosition();
			l_pActivateView = l_pActiveDoc->GetNextView(l_posa);
			l_ActivateFrame=(CMDIChildWnd*)l_pActivateView->GetParentFrame();
			l_ActivateFrame->MDIActivate();
			//l_ActivateFrame->ShowWindow(SW_RESTORE);
			((CPconwApp*)AfxGetApp())->ClearNewMessage(CString(szBuffer));
			return;
		}
					
	}
	*pResult = 0;
}

void CTabBar::InsertTab(CString & strTab)
{
	TC_ITEM l_TC;
	int iNumItems;
	l_TC.pszText = (char *)LPCSTR(strTab);
	l_TC.mask = TCIF_TEXT;
	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	ASSERT (m_TabCtrl);
	iNumItems = m_TabCtrl->GetItemCount();
	m_TabCtrl->InsertItem(iNumItems,&l_TC);//Makes it add to the end
	m_TabCtrl->SetCurSel(iNumItems);//Selects the current item


}

void CTabBar::OnSize(UINT nType, int cx, int cy) 
{
	//MessageBeep(-1);
	CDialogBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CTabBar::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialogBar::OnChildNotify(message, wParam, lParam, pLResult);
}

void CTabBar::ReSizeTabBar()
{
	//MessageBeep(-1);
	if(!IsWindow(m_hWnd)) return;
	CFrameWnd * l_ParentFrame;
	CRect l_RECT;
	CSize l_OldSize(m_sizeDefault);
	CSize l_NewSize;


	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	ASSERT (m_TabCtrl);
	l_ParentFrame = GetDockingFrame();
	ASSERT (l_ParentFrame);
	l_ParentFrame->GetClientRect(&l_RECT);
	l_NewSize = l_RECT.Size();
	l_NewSize.cy = 25;//l_OldSize.cy;
	m_sizeDefault= l_NewSize;
	l_RECT.left=3;
	l_RECT.top=0;
	m_TabCtrl->MoveWindow(&l_RECT);
	l_ParentFrame->RecalcLayout();

}
//CMapStringtoOb

void CTabBar::RemoveTab(CString & strRemove)
{
	char szBuffer[TCBUFFERSIZE];
	TC_ITEM l_TC;
	l_TC.cchTextMax=TCBUFFERSIZE;
	l_TC.pszText = szBuffer;
	l_TC.mask = TCIF_TEXT;
	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	for(int i=0;i<=m_TabCtrl->GetItemCount();i++)
	{
		m_TabCtrl->GetItem(i,&l_TC);
		if(strRemove.CompareNoCase(l_TC.pszText)==0)
		{
			m_TabCtrl->DeleteItem(i);
			return;
		}
	}
}

void CTabBar::NewMessage(CString &Title)
{
/*
	//m_NewMessage
	CNewMessage * l_NewMessage;

	m_WindowList.Lookup(Title,(CObject*&)l_NewMessage);
	if(l_NewMessage)
	{
		l_NewMessage->UnRead=TRUE;	
	}
	else
	{
		m_WindowList.SetAt(Title,(CObject*&)new CNewMessage(TRUE));
	}
*/
}



int CTabBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
	return 0;
}

void CTabBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if(IDC_TAB2==nIDCtl)
	{
		m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
		
		/*CRect rect = lpDrawItemStruct->rcItem;
		int nTabIndex = lpDrawItemStruct->itemID;
		if (nTabIndex <0) return; 
		BOOL bSelected=(nTabIndex=m_TabCtrl->GetCurSel()); 
		char label[64]; 
		TC_ITEM tci; 
		tci.mask=TCIF_TEXT|TCIF_IMAGE; 
		tci.pszText=label;
		tci.cchTextMax=63;
		if (!m_TabCtrl->GetItem(nTabIndex, &tci )) return; 
		CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		
		if (!pDC) return;	
		int nSavedDC = pDC->SaveDC();
		*/
		CPconwApp *MotherApp;
		CNewMessage *l_NewMessage;
		MotherApp = (CPconwApp *)AfxGetApp();

		CRect rect = lpDrawItemStruct->rcItem;
		int nTabIndex = lpDrawItemStruct->itemID;
		if (nTabIndex < 0) return;
		BOOL bSelected = (nTabIndex == m_TabCtrl->GetCurSel());

		char label[64];
		TC_ITEM tci;
		tci.mask = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText = label;     
		tci.cchTextMax = 63;    	
		if (!m_TabCtrl->GetItem(nTabIndex, &tci )) return;

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		if (!pDC) return;
		int nSavedDC = pDC->SaveDC();
		// For some bizarre reason the rcItem you get extends above the actual
		// drawing area. We have to workaround this "feature".
		rect.top += ::GetSystemMetrics(SM_CYEDGE);	
		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));	//RGB(255,255,255));//
/*
		// Draw image
		CImageList* pImageList = m_TabCtrl->GetImageList();	
		if (pImageList && tci.iImage >= 0) 
		{
			rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin
			// Get height of image so we 		
			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);		
			CRect ImageRect(info.rcImage);
			int nYpos = rect.top;
			pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
			rect.left += ImageRect.Width();	
		}	
*/
		
		if(!MotherApp->m_WindowList.Lookup(label,(CObject*&)l_NewMessage))
		{
			bSelected= TRUE; //Just a kludge to make the next statement simpler
			//ASSERT (0);//Shouldn't ever come to this...
			//return;
		}

		//	l_NewMessage->UnRead
		
		if (bSelected || (l_NewMessage->UnRead==FALSE)) 
		{
		//	pDC->SetTextColor(m_crSelColour);
	//		pDC->SelectObject(&m_SelFont);
			rect.top -= ::GetSystemMetrics(SM_CYEDGE);
			pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		} 
		else 
		{
			pDC->SetTextColor(RGB(255,0,0));		
	//		pDC->SelectObject(&m_UnselFont);
			pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
		}
		pDC->RestoreDC(nSavedDC);
		return;
	}
	CDialogBar::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

CString CTabBar::GetSelTab()
{
	TC_ITEM l_TC;
	char szBuffer[TCBUFFERSIZE];
	l_TC.cchTextMax=TCBUFFERSIZE;
	int l_Sel;
	l_TC.pszText = szBuffer;
	l_TC.mask = TCIF_TEXT;
	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	ASSERT (m_TabCtrl);
	l_Sel = m_TabCtrl->GetCurSel();
	m_TabCtrl->GetItem(l_Sel,&l_TC);
	return CString(szBuffer);
}

void CTabBar::SetSelTab(CString & Title)
{
	char szBuffer[TCBUFFERSIZE];
	TC_ITEM l_TC;
	l_TC.cchTextMax=TCBUFFERSIZE;
	l_TC.pszText = szBuffer;
	l_TC.mask = TCIF_TEXT;
	m_TabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB2);
	for(int i=0;i<=m_TabCtrl->GetItemCount();i++)
	{
		m_TabCtrl->GetItem(i,&l_TC);
		if((l_TC.pszText==NULL)||(l_TC.pszText[0]==NULL)) break;
		if(Title.CompareNoCase(l_TC.pszText)==0)
		{
			m_TabCtrl->SetCurSel(i);
			((CPconwApp*)AfxGetApp())->ClearNewMessage(Title);
			return;
		}
	}
}
