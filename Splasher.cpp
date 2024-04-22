/*
Module : SPLASHER.CPP
Purpose: A splash screen component for MFC 4.x which uses a DIB bitmap
         instead of a resource. Palette management code is also included
         so that the bitmap will display using its own optimized palette.
         It also used as a UI thread so that UI of the splash screen 
         remains responsive.
Created: PJN / 15-11-1996
History: None

Copyright (c) 1996 by PJ Naughter.  
All rights reserved.

*/


//////////////////// Includes ///////////////////
#include "stdafx.h"
//#include "resource.h"
#include "Splasher.h"



//////////////////// Defines ////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MOUSE_MOVE 0xF012



BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
	//{{AFX_MSG_MAP(CSplashThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)



CSplashThread::CSplashThread()
{
}


CSplashThread::~CSplashThread()
{
}


CString CSplashThread::GetHomeDirectory() const
{
  TCHAR sFilename[_MAX_PATH];
  TCHAR sDrive[_MAX_DRIVE];
  TCHAR sDir[_MAX_DIR];
  TCHAR sFname[_MAX_FNAME];
  TCHAR sExt[_MAX_EXT];
  GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
  _tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

  CString rVal(CString(sDrive) + CString(sDir));
  int nLen = rVal.GetLength();
  if (rVal.GetAt(nLen-1) != _T('\\'))
    rVal += _T("\\");

  return rVal;
}



BOOL CSplashThread::InitInstance()
{
	if (!m_SplashScreen.Create(GetHomeDirectory() + "pcon97.dat"))
    TRACE("Splash Bitmap not found\n");
  m_pMainWnd = &m_SplashScreen;

	return TRUE;
}


void CSplashThread::HideSplash()
{
  m_SplashScreen.SetOKToClose();
  m_SplashScreen.SendMessage(WM_CLOSE);
}






BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
  //{{AFX_MSG_MAP(CSplashWnd)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_CLOSE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CSplashWnd::CSplashWnd()
{
  m_bOKToClose = FALSE;
  m_pDIB = NULL;
  m_pPal = NULL;         
}


CSplashWnd::~CSplashWnd()
{
  if (m_pDIB) 
  {
    delete m_pDIB;
    m_pDIB = NULL;
  }

  if (m_pPal) 
  {
    delete m_pPal;
    m_pPal = NULL;
  }

  //added these lines to destroy our invisible owner when we're done
  if (m_wndOwner.m_hWnd != NULL)
    m_wndOwner.DestroyWindow();
}


BOOL CSplashWnd::Create(const CString& sDIBFileName)
{                   
  BOOL bSuccess = TRUE;

  //try to load the dib first, before we resort to the resource bitmap
  CFile f;
  
  if (f.Open(sDIBFileName, CFile::modeRead | CFile::shareExclusive))
  {
    // Load the DIB from the file.
    ASSERT(m_pDIB == NULL);
    m_pDIB = new CDIB;
    if (m_pDIB)
    {
      if (!m_pDIB->Load(&f)) 
      { 
        TRACE("Failed to load splash\n");
        bSuccess = FALSE;
      }   
      else
      {
        // Create the palette from the DIB.
        ASSERT(m_pPal == NULL);
        m_pPal = new CDIBPal;
        
        if (m_pPal)
        {
          if (!m_pPal->Create(m_pDIB)) 
          {   
            TRACE("Create palette failed from Splash, ugliness to follow.\n");
            delete m_pPal;
            m_pPal = NULL;
            //can still use it as it may be a true color image
          }

          m_wndWidth = m_pDIB->GetWidth();
          m_wndHeight = m_pDIB->GetHeight();
        }                
        else
          bSuccess = FALSE;
      }
    } 
    else
      bSuccess = FALSE;
  }

  //Modify the owner window of the splash screen to be an invisible WS_POPUP 
  //window so that the splash screen does not appear in the task bar
  if (!m_wndOwner.CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP,
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
    return FALSE;

  bSuccess = bSuccess && CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
                           _T(""), WS_POPUP | WS_VISIBLE, 0, 0, m_wndWidth, m_wndHeight, 
                           m_wndOwner.GetSafeHwnd(), NULL);

  return bSuccess;
}


int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  CenterWindow();
  BringWindowToTop();
  SetForegroundWindow();

  return 0;
}


void CSplashWnd::OnPaint()
{
  CPaintDC dc(this);

  // Select and realize the palette.
  // Note: always use TRUE to do a background
  // realization here.
  CPalette* pOldPal = dc.SelectPalette(m_pPal, TRUE);
  dc.RealizePalette();
    
  // Draw the DIB.
  m_pDIB->Draw(&dc, 0, 0);
    
  // Restore the DC.
  dc.SelectPalette(pOldPal, FALSE);
}

                                                                    
//This message is an optional extra, If you do not want the splash screen
//to be dragable then remove this function                                                                    
void CSplashWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
  //Fake a Window drag
  SendMessage(WM_LBUTTONUP);
  SendMessage(WM_SYSCOMMAND, MOUSE_MOVE);
}


void CSplashWnd::OnClose() 
{
  if (m_bOKToClose) 
    CWnd::OnClose();
}


// Select and realise our palette
BOOL CSplashWnd::SelRelPal(BOOL bForceBkgnd)
{
  // We are going active, so realize our palette.
  CDC* pDC = GetDC();

  CPalette* pOldPal = pDC->SelectPalette(m_pPal, bForceBkgnd);
  UINT u = pDC->RealizePalette();
  pDC->SelectPalette(pOldPal, TRUE);
  pDC->RealizePalette();
  ReleaseDC(pDC);

  // If any colors have changed or we are in the
  // background, repaint the lot.
  if (u || bForceBkgnd) 
    InvalidateRect(NULL, TRUE); // Repaint.
  
  return (BOOL) u; // TRUE if some colors changed.
}


void CSplashWnd::OnPaletteChanged(CWnd* pFocusWnd)
{
  // See if the change was caused by us and ignore it if not.
  if (pFocusWnd != this) 
    SelRelPal(TRUE); // Realize in the background. 
}


BOOL CSplashWnd::OnQueryNewPalette()
{
  return SelRelPal(FALSE); // Realize in the foreground.
}


