/*
Module : SPLASHER.H
Purpose: A splash screen component for MFC 4.x which uses a DIB bitmap
         instead of a resource. Palette management code is also included
         so that the bitmap will display using its own optimized palette
Created: PJN / 15-11-1996
History: None

Copyright (c) 1996 by PJ Naughter.  
All rights reserved.

*/


#ifndef _SPLASH_H__
#define _SPLASH_H__


//////////////////////////// Includes /////////////////////////////////////////


///////////////// Classes //////////////////////////
class CSplashWnd : public CWnd
{
public:
  CSplashWnd();
  ~CSplashWnd();

// Operations
  BOOL Create(const CString& sDIBFileName);
  void SetOKToClose() { m_bOKToClose = TRUE; };  
  
protected:
  //{{AFX_VIRTUAL(CSplashWnd)
  //}}AFX_VIRTUAL

  //{{AFX_MSG(CSplashWnd)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
  afx_msg BOOL OnQueryNewPalette();
  afx_msg void OnClose();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
  
  BOOL SelRelPal(BOOL bForceBkgnd);


  BOOL     m_bOKToClose;
  CDIB*    m_pDIB;
  CDIBPal* m_pPal;
  int      m_wndWidth;
  int      m_wndHeight;
  CBitmap  m_bitmap;
  CWnd     m_wndOwner;                   
};


class CSplashThread : public CWinThread
{
public:
  void HideSplash();

protected:
	CSplashThread();
  virtual ~CSplashThread();

	//{{AFX_VIRTUAL(CSplashThread)
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	DECLARE_DYNCREATE(CSplashThread)

	//{{AFX_MSG(CSplashThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

  CSplashWnd m_SplashScreen;

  CString GetHomeDirectory() const;
};


#endif


