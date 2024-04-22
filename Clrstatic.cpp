// ClrStatic.cpp : implementation file
//

#include "stdafx.h"

#include "ClrStatic.h"



/////////////////////////////////////////////////////////////////////////////
// CClrStatic

CClrStatic::CClrStatic()
{
}

CClrStatic::~CClrStatic()
{
}


BEGIN_MESSAGE_MAP(CClrStatic, CStatic)
	//{{AFX_MSG_MAP(CClrStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClrStatic message handlers

void CClrStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetTextColor(m_clrForeground);
	dc.SetBkColor(m_clrBackground);
	
	CString l_tmpsamptxt;
	l_tmpsamptxt.LoadString(IDS_STRSAMPTXT);
	dc.TextOut(0,0,l_tmpsamptxt);
	// Do not call CStatic::OnPaint() for painting messages
}
