#ifndef __IPX_H__
#define __IPX_H__

#include "gbl.h"
#include "kahn.h" 

#define NODE_ADDR_LEN         6
#define MAX_FRAGMENT_COUNT    2
#define DRIVER_WORKSPACE      12
#define NETWORK_ADDR_LENGTH   4

//#define MAX_IPX_PACKET        576
#define MAX_IPX_PACKET        1200           // give us a little more room

/*--------------------------------------------------------------------------*/
/* Maximum number of frames that we can send from a delta frame.            */
/*--------------------------------------------------------------------------*/
#define MAX_SEQ_DELTA_FRAMES  10

typedef struct ecb {
   struct ecb far *nextecb;                  //
   void (far *eventserviceroutine)();
   BYTE inuseflag;
   BYTE completioncode;
   WORD socketnumber;
   BYTE unionstuff[4];
   BYTE driverworkspace[DRIVER_WORKSPACE];
   BYTE immediateaddress[NODE_ADDR_LEN];
   WORD fragmentcount;
   struct {
      void far *ptr;
      WORD      length;
   } FragmentDescriptor[MAX_FRAGMENT_COUNT];
} ECB, *PECB, far *FPECB;

typedef struct socketq {
   FPECB ecb;
   WORD socket;
   WORD ecbcount;
   WORD ecbdec;
   WORD ecbkilled;
} SOCKETQ, *PSOCKETQ;

#define ECB_INUSE                   0xFE
#define ECB_COMPLETE                0x00

#define ECBRC_SUCCESS               0
#define ECBRC_CANCELLED             0xFC
#define ECBRC_PACKETOVERFLOW        0xFD
#define ECBRC_PACKETNOTDELIVERABLE  0xFE
#define ECBRC_SOCKETCLOSED          0xFF
#define ECBRC_CANNOTCANCELECB       0xF9
#define ECBRC_ECBISNOTBUSY          0xFF

typedef struct ipx_header {
   WORD checksum;
   WORD length;
   BYTE transportcontrol;
   BYTE packettype;
   BYTE destinationnetwork[NETWORK_ADDR_LENGTH];
   BYTE destinationnode[NODE_ADDR_LEN];
   WORD destinationsocket;
   BYTE sourcenetwork[NETWORK_ADDR_LENGTH];
   BYTE sourcenode[NODE_ADDR_LEN];
   WORD sourcesocket;
} IPX_HEADER, *PIPX_HEADER, far *FPIPX_HEADER;


#define IPXRC_OK                    0x00
#define IPXRC_SOCKETOPENED          0xFF
#define IPXRC_BADSOCKET             0xFF
#define IPXRC_SOCKETTABLEFULL       0xFE

WORD received_IPX_packet(PIPX_HEADER pipxh, PBYTE data, int length, int localdrivermode);

WORD extract_compressed_data(FPBYTE fragptr, PSKAHN_HDR pskhdr, WORD length, 
         PIPX_HEADER pipxh, PULONG addr);
WORD packit(FPBYTE data, WORD len, PBYTE newbuff);
WORD unpackit(PBYTE data, WORD len, FPBYTE curr);
WORD deltaframe(FPBYTE curr, WORD curr_len, PBYTE newbuff, PKAHN_CLIENT pkc);
PKAHN_CLIENT get_kahn_client(PULONG paddr);
WORD build_compressed_frame(FPECB fpecb, PULONG paddr, FPBYTE fdata,
                           WORD length, PBYTE buffer);
PKAHN_CLIENT get_kahn_client(PULONG paddr);

#endif      // __IPX_H__

