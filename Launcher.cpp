// CLauncher.cpp : implementation file
//CControlBar//CToolBar
//

#include "stdafx.h"
#include "pconw.h"
#include "launcher.h"

//#include "TestBmp.h"


//#define	LAUNCH_CFG	WM_USER+500
//#define	LAUNCH_01	WM_USER+501
//#define	LAUNCH_02	WM_USER+502
//#define	LAUNCH_03	WM_USER+503

/////////////////////////////////////////////////////////////////////////////
// CDUserList dialog


CLauncher::CLauncher()
	: CToolBar()
{
	//{{AFX_DATA_INIT(CLauncher)
	//}}AFX_DATA_INIT
}




BEGIN_MESSAGE_MAP(CLauncher, CToolBar)
	//{{AFX_MSG_MAP(CLauncher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//	DragAcceptFiles(TRUE);
BOOL CLauncher::Create( CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	BOOL rvalue;
	int l_iStr;
	TBBUTTON l_button;

	rvalue=CToolBar::Create(pParentWnd,dwStyle,nID);
	DragAcceptFiles(TRUE);
//	SetSizes(CSize(38,38),CSize(32,32));

	m_ToolBarCtlA = &GetToolBarCtrl();
	l_iStr=m_ToolBarCtlA->AddBitmap(1,IDB_BITMAP1);

	l_button.iBitmap=l_iStr;
	l_button.idCommand=LAUNCH_CFG;
	l_button.fsState=TBSTATE_ENABLED;
	l_button.fsStyle=TBSTYLE_BUTTON;
	l_button.iString=NULL;
	m_ToolBarCtlA->SetButtonSize(CSize(40,40));
	m_ToolBarCtlA->SetBitmapSize(CSize(32,32));
	m_ToolBarCtlA->AddButtons(1,&l_button);
	m_ToolBarCtlA->EnableButton(LAUNCH_CFG,TRUE);
	m_ToolBarCtlA->AutoSize();
	//CBitmap
	
	
	return rvalue;
}

BOOL CLauncher::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_DROPFILES)
	{
	   UINT nFiles = ::DragQueryFile((HDROP)pMsg->wParam, (UINT)-1, NULL, 0);
	   for (UINT iFile = 0; iFile < nFiles; iFile++)
	   {
		  TCHAR szFileName[_MAX_PATH];
		  ::DragQueryFile((HDROP)pMsg->wParam, iFile, szFileName, _MAX_PATH);
		  m_strlstDragged.AddTail(szFileName);
		  //MessageBox(szFileName,szFileName,MB_OK);
	   }
	   ::DragFinish((HDROP)pMsg->wParam);
	   
	   //Go through the list, calling the code to add this item.....
	   POSITION pos;
	   pos=m_strlstDragged.GetHeadPosition();
	   //((CPconwApp*)AfxGetApp())->DraggedLaunchItem(m_strlstDragged.GetAt(pos));
	   while(pos)
	   {
			((CPconwApp*)AfxGetApp())->DraggedLaunchItem(m_strlstDragged.GetNext(pos));
	   }	   
	   //m_strlstDragged.RemoveAll();
	   return TRUE;
	}
	

	return CToolBar::PreTranslateMessage(pMsg);
}

void CLauncher::RefreshToolBar(LaunchItems *LaunchPtr)
{

//	CImageList l_ConvertImage;
//	CBitmap l_ConversionBitMap,*l_bmpptr;
//	BITMAP l_hbitmap;
//	IMAGEINFO l_imageinfo;
//	BYTE *bmpBuff=NULL;

	
//	l_ConvertImage.Create(32,32,FALSE,20,20);
	//Scan through LaunchPtr[x]. members 
	
		//For each item, read the icon into a CImageList
		//Then read the bitmap, and add it to the toolbar, storing the bitmap #
		//Create the button, using the above bitmap, and the title for the tooltip
		for(int x=0;x<20;x++)//*****************************x<20
		{
			if(LaunchPtr[x].ProgName.GetLength())
			{
				int iTempImageNum;
	//			l_bmpptr = new CBitmap;

				LaunchPtr[x].hIcon = ExtractIcon(AfxGetInstanceHandle(),LaunchPtr[x].FileIcon,0);
				if(!LaunchPtr[x].hIcon) LaunchPtr[x].hIcon=AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);
				
				GetIconInfo(LaunchPtr[x].hIcon,&LaunchPtr[x].IconInfo);
	//			LaunchPtr[x].BitMap = new CBitmap;
				LaunchPtr[x].BitMap.Detach();
				LaunchPtr[x].BitMap.Attach(LaunchPtr[x].IconInfo.hbmColor);
				int iblah=m_ToolBarCtlA->AddBitmap(1,&LaunchPtr[x].BitMap);

				//int iblah=m_ToolBarCtlA->AddBitmap(1,LaunchPtr[x].BitMap);


				LaunchPtr[x].Button.iBitmap=iblah;
				LaunchPtr[x].Button.idCommand=IDM_LAUNCH01+x;
				
				LaunchPtr[x].Button.fsState=TBSTATE_ENABLED;
				LaunchPtr[x].Button.fsStyle=TBSTYLE_BUTTON;
				LaunchPtr[x].Button.iString=NULL;				
				
				m_ToolBarCtlA->AddButtons(1,&LaunchPtr[x].Button);
				m_ToolBarCtlA->EnableButton(IDM_LAUNCH01+x,TRUE);
//				delete l_bmpptr;


			}
		
		}
	m_ToolBarCtlA->AutoSize();

	CToolTipCtrl *l_ToolTip;
	l_ToolTip=m_ToolBarCtlA->GetToolTips();
	if(l_ToolTip)
	{
		l_ToolTip->SetDelayTime(50);
	}


}


/*
				iTempImageNum=l_ConvertImage.Add(LaunchPtr[x].hIcon);
				
		
				BOOL l_btest=l_ConvertImage.GetImageInfo(iTempImageNum,&LaunchPtr[x].ImgInfo);
				ASSERT(l_btest);
				//At this point, we have an imageinfo struct, with an HBITMAP pointer.
				l_btest=l_bmpptr->Attach(LaunchPtr[x].ImgInfo.hbmImage);
				//Now, we have a temparary CBitmap object.


				CTestBmp l_ctstbmp;
				//l_ctstbmp.SetBmp((HBITMAP)LaunchPtr[x].BitMap->GetSafeHandle());
				l_ctstbmp.SetIconName(LPCSTR(LaunchPtr[x].FileIcon));
				l_ctstbmp.DoModal();
				//l_bmpptr=l_ConversionBitMap.FromHandle(LaunchPtr[x].ImgInfo.hbmImage);
											
				//l_ConversionBitMap=l_imageinfo.hbmImage;//GetBitmapBits
				
				//l_btest=l_bmpptr->Attach(LaunchPtr[x].ImgInfo.hbmImage);
				//l_bmpptr->GetObject(sizeof(l_hbitmap),&l_hbitmap);

				//(l_hbitmap.bmWidthBytes*l_hbitmap.bmHeight)//This is the number of bytes in the bitmap
				//	CSize l_bmpDim;
				//	bmpBuff = new BYTE[l_hbitmap.bmWidthBytes*l_hbitmap.bmHeight];
				//	ASSERT(bmpBuff);

				//	DWORD itest1 = l_bmpptr->GetBitmapBits(l_hbitmap.bmWidthBytes*l_hbitmap.bmHeight,bmpBuff);
					//l_bmpDim=l_bmpptr.GetBitmapDimension();
				//	LaunchPtr[x].BitMap.CreateBitmapIndirect(&l_hbitmap);
				//	LaunchPtr[x].BitMap.SetBitmapDimension(l_hbitmap.bmWidth,l_hbitmap.bmHeight);
				//	itest1 = LaunchPtr[x].BitMap.SetBitmapBits(l_hbitmap.bmWidthBytes*l_hbitmap.bmHeight,l_hbitmap.bmBits);
					
				//	bmpBuff=NULL;
				
				//	ASSERT(l_btest);
				delete bmpBuff;

				//::SendMessage(AfxGetMainWnd()->m_hWnd,BM_SETIMAGE,0,(LPARAM) (HANDLE)LaunchPtr[x].hIcon);
				//WM_//BUTTON//WM_SETICON//STM_SETICON//SetIcon

//				CBitmap l_bmp1,l_bmp2;
//				CDC l_cdc;
//
//				l_cdc.CreateCompatibleDC(NULL);
//				l_cdc.DrawIcon(0,0,LaunchPtr[x].hIcon);
//				l_cdc.GetCurrentBitmap();			
//				l_cdc.SelectObject();




  */