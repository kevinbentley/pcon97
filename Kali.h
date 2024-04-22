#ifndef __kali_h__
#define __kali_h__

//#include "gbl.h"
//#include "ipx.h"

//
// Kali UDP port numbers
#define KALI_PORT          0x08A0         // network byte order port id 
#define KALI_PORT_REV      0xA008         // intel byte port id

typedef struct kali_hdr 
{
   BYTE  kali_type;                        // Kali Frame type
} KALI_HDR, *PKALI_HDR;

typedef struct kali_encap_hdr
{
   WORD kali_frame_num;                    // Kali frame number
   WORD  kali_dest_socket;                 // Kali destination socket
   WORD  kali_src_socket;                  // Kali source socket
} KALI_ENCAP_HDR, *PKALI_ENCAP_HDR;

typedef struct kali_bcast_encap_hdr
{
   BYTE  kali_orig_ip[4];                  // Kali frame number
   WORD  kali_orig_ip_socket;              // Kali destination socket
} KALI_BCAST_ENCAP_HDR, *PKALI_BCAST_ENCAP_HDR;

#define  KALI_MGMT_LEN  sizeof(KALI_HDR)
//
// Kali packet types (kahn) redefines
//
#define  KALI_LOGON_REQ             0x0a
#define  KALI_LOGON_RESP            0x8a
#define  KALI_MOTD_REQ              0x09
#define  KALI_QUERY_SERVER          0x04
#define  KALI_DISPLAY_STRING        0x01
#define  KALI_DISPLAY_STRING_LAST   0x02
#define  KALI_SERVER_TASK_REQ       0x06
#define  KALI_SERVER_TASK_RESP      0x86
#define  KALI_LOGOFF_REQ            0x05
#define  KALI_LOGOFF_RESP           0x85

#define  KALI_DATA_PACKET           0x07

#define  KALI_BROADCAST_TO_SERVER   0x08
#define  KALI_BROADCAST_FROM_SERVER 0x88

#define  KALI_TIMEOUT_PACKET        0xF0
#define  KALI_NICKNAME_COLLISION    0xF1
#define  KALI_VSN_FULL              0xF2

#define  KALI_COMPRESS_ACK          0x8F

typedef struct kali_logon {
   BYTE  nickname[16];                       // nickname of the user
   BYTE  name[32];                           // user name
   BYTE  serial[16];                         // 16 byte serial number
   BYTE  authnum[16];                        // 16 byte authorization number
   BYTE  email[80];                          // email address
   
} KALI_LOGON, *PKALI_LOGON;


#define     KALI_MGMT      0
#define     KALI_DATA      1
#define     KALI_ENCAPDATA 2

//void kali_packet_receive(PIP_HDR pip, PUDP_HDR pudp, PKALI_HDR pkhdr, PBYTE ipaddr);


typedef struct kali_client {
   struct kali_client *next;                 // pointer to next client
   BYTE  addr[4];                            // ip address of this client
   WORD  socket;                             // the socket opened 
   BYTE  process[16];                        // array for the process name
} KALI_CLIENT, *PKALI_CLIENT;

//void kali_packet_receive(PIP_HDR pip, PUDP_HDR pudp, PKALI_HDR pkhdr, PBYTE ipaddr);
WORD kali_send_logon_request(PBYTE ip);
ip_revcopy(char *dest, char *src);
_fip_revcopy(char *dest, char *src);

#define KALI_SESSION_ACTIVE  0x01
#define KALI_SESSION_CLOSED  0x00

#define KM_SERVER 0x01

typedef struct kahn_key
{
   char serial[17];
   char username[33];
   char subkey[40];
} KAHN_KEY, *PKAHN_KEY;

#endif // __kali_h_

