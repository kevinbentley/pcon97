// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows 95 Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxrich.h>

#include <afxsock.h>		// MFC socket extensions
#include <afxcview.h>
#include <afxadv.h>

#include <winsock.h>
#include <wsipx.h>
#include <wsnwlink.h>
#include <afxsock.h>

#include <wfc.h>

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>

typedef struct _LaunchItems
{
	CString ProgName;
	CString FileName;
	CString FilePath;
	CString FileParms;
	CString FileIcon;
	TBBUTTON Button;
	CBitmap BitMap;
	//IMAGEINFO ImgInfo;
	HICON hIcon;
	ICONINFO IconInfo;


	int	iBitMapNum;
}LaunchItems;

#include "chatsock.h"

#include "ChannelList.h"
#include "pconw.h"


//Splash screen stuff
#include "mmsystem.h"

#include "dib.h"
#include "dibpal.h"
#include "splasher.h"

#define CHAT_INITIATE 1
#define CHAT_ACCEPT 2

#define DCC_CHAT_LISTEN 4

#define DCC_CHAT_CALL 8

#define DCC_CHAT_ACCEPT 16

#define DCC_FILE_LISTEN 32
#define DCC_FILE_CONNECT 64

