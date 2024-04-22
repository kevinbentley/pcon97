//
// Sharemsg.h
//
// This file contains the structures used to share memory between the Kahn Control Center and
// the Kahn Win95 chat program. 
//
#ifndef __sharemsg_h__
#define __sharemsg_h__

#include "packets.h"

#define	 CHAT_PACKET						0x01
#define  DISPLAY_PACKET						0x02

#define KAHN_SHAREMEM_NAME					"kahn_shared_mem"

typedef struct _sharemem
{
	ULONG myip;
	char  nickname[16];
	HWND  hChat;							// handle to chat
	DWORD version;							// Version of this kahn client
											// 99 for .99 100 for 1.0
	char  serial[17];						// sz serial number
	char  email[256];						// sz email address	
//	ENCAPPACKET recv[NUM_PACKETS_SHARED];
//	ENCAPPACKET send[NUM_PACKETS_SHARED];
} SHAREMEM, *PSHAREMEM;

#endif		// __sharemsg_h__
