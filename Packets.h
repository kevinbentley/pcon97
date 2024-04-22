//
// This header defines the packets we send in the chat program
//

#ifndef __packets_h__
#define __packets_h__

#define  NORMAL_TEXT          0x01
#define  HIGH_TEXT            0x02 //Posture Message
#define  BINARY_DATA          0x03
#define  BINARY_REQUEST       0x04
#define  PING_REQUEST         0x05
#define  PING_RESPONSE        0x06
#define  USERS_RESPOND		  0x07
#define  USERS_RESPONSE		  0x08
#define  PRIV_MSG			  0x09
#define  PART_CHANNEL		  0x11
//Filexfer packet types
#define  FILEREQ			  0x0a
#define  FILELASTFRAG		  0x0b
#define  FILEFRAG			  BINARY_DATA
#define  FILEACK			  0x0d
#define  FILENAK			  0x0e
#define	 FILEINFO			  0x0f
#define	 FILEFIN			  0x10

#define  SIGNON               0xF1
#define  SIGNOFF              0xF2
#define  DISPLAY_DATA		  0xF0
#define  SERVER_COMMAND	 	  0xE0

#define  KALI_KILL_USER       0x20
#define  KALI_BAN_USER		  0x21
#define  KALI_BAN_CLEAR		  0x22
#define  KALI_BAN_LIST		  0x23

#define  KALI_WHOIS_REQ		  0x0c
#define  KALI_CHANGESUB       0x0b       



#define  MAX_TEXT_SIZE        132
#define  MAX_DATA_SIZE        1500

#define	NUMLAUNCHES	20

typedef unsigned long ULONG;
#pragma pack(1)



typedef struct _chat_packet_hdr
{
   BYTE  type;
   char  name[16];                  // name of person sending this packet
   char  channel[8];                // channel they are currently on
   ULONG ipaddress;					// header information

} CHAT_PACKET_HDR, *PCHAT_PACKET_HDR;

typedef struct _chat_packet_data
{
   union
   {
      struct
      {
         char filename[13];         // filename
         long totalsize;            // total size of the file
         long offset;               // offset into the file for this packet
         long length;               // length of the binary data
         BYTE data[MAX_DATA_SIZE];  // data
      }  binframe;
      struct
      {
   		 long	 size;				// size of packet data to send if server command
         char text[MAX_DATA_SIZE];  // text only packet
      } textframe;

   } framedata;

} CHAT_PACKET_DATA, *PCHAT_PACKET_DATA;

typedef struct _S_kill
{
	char command;
	char nick[16];
	char reason[200];
}S_kill;
typedef struct _S_Subnet
{
	char command;
	char subnet[32];
	char password[16];
}S_Subnet;

typedef struct _S_Whois
{
	char command;
	char nick[17];	
}S_Whois;


#pragma pack()
#endif      // __packets_h__
