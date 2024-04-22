// ColorTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "ColorTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CColorTabCtrl

CColorTabCtrl::CColorTabCtrl()
{
}

CColorTabCtrl::~CColorTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CColorTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CColorTabCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorTabCtrl message handlers

HBRUSH CColorTabCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

HBRUSH CColorTabCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CColorTabCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CTabCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
	MessageBeep(-1);

	CRect rect = lpDrawItemStruct->rcItem;
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex <0) return; 
	BOOL bSelected=(nTabIndex=GetCurSel()); 
	char label[64]; 
	TC_ITEM tci; 
	tci.mask=TCIF_TEXT|TCIF_IMAGE; 
	tci.pszText=label;
	tci.cchTextMax=63;
	if (!GetItem(nTabIndex, &tci )) return; 
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CPconwApp *MotherApp;
	CNewMessage *l_NewMessage;
	MotherApp = (CPconwApp *)AfxGetApp();
	if (!pDC) return;	
	int nSavedDC = pDC->SaveDC();
	
	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
	rect.top += ::GetSystemMetrics(SM_CYEDGE);	
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));	
	// Draw image
	CImageList* pImageList = GetImageList();	
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

	
	if(!MotherApp->m_WindowList.Lookup(label,(CObject*&)l_NewMessage))
	{
		bSelected= TRUE; //Just a kludge to make the next statement simpler
	}
	//	l_NewMessage->UnRead
	
	if (bSelected || (l_NewMessage->UnRead==FALSE)) 
	{
	//	pDC->SetTextColor(m_crSelColour);
//		pDC->SelectObject(&m_SelFont);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} 
	else {
		pDC->SetTextColor(RGB(255,0,0));		
//		pDC->SelectObject(&m_UnselFont);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}
	pDC->RestoreDC(nSavedDC);

}

void CColorTabCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTabCtrl::PreSubclassWindow();
	
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

int CColorTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1) 
		return -1;
	ModifyStyle(0, TCS_OWNERDRAWFIXED);		
	return 0;
}
