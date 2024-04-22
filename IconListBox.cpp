// IconListBox.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "IconListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconListBox

CIconListBox::CIconListBox()
{
}

CIconListBox::~CIconListBox()
{
}


BEGIN_MESSAGE_MAP(CIconListBox, CListBox)
	//{{AFX_MSG_MAP(CIconListBox)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconListBox message handlers

void CIconListBox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//	CListBox *l_lbUsers = (CListBox *) GetDlgItem(IDC_LIST1);
//		ASSERT(l_lbUsers);
		CString l_strLine = (LPCSTR)lpDrawItemStruct->itemData;
		DWORD l_dwPing;
//		l_strLine = GetText();
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		//COLORREF cr = (COLORREF)lpDrawItemStruct->itemData; // RGB in item data

		if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		{
			// Paint the color item in the color requested
//			CBrush br(cr);
			pDC->TextOut(17,0,l_strLine);
		//	pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
		}

		if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
			(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
		{
			// item has been selected - hilite frame
		//	COLORREF crHilite = RGB(255-GetRValue(cr),
		//					255-GetGValue(cr), 255-GetBValue(cr));
		//	CBrush br(crHilite);
			pDC->TextOut(17,0,l_strLine);
		//	pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
		}

		if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
			(lpDrawItemStruct->itemAction & ODA_SELECT))
		{
			// Item has been de-selected -- remove frame
		//	CBrush br(cr);
			pDC->TextOut(17,0,l_strLine);
		//	pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
		}	
//	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
