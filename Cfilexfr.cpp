//CFileXfer Class


#include "stdafx.h"

#include "chatsock.h"

#include "pconw.h"
#include "..\kahn95\kali.h"

#include "packets.h"
#include "CFileXfr.h"


CFileXfer::CFileXfer()
{
	int i;
	for(i=0;i<MAXOFFERS;i++) AvailFiles[i].size=0;
	for(i=0;i<MAXINPROGRESS;i++) InProgressFiles[i].User[0]=0;
	IncomingFile.User[0]=0;
	IncomingFile.TotalSize=-1;
	Sequence = 1;
}
	



CFileXfer::~CFileXfer()
{
	
	for(int i=0;i<MAXOFFERS;i++) if(AvailFiles[i].size!=0) 
    {
        AvailFiles[i].CFileObject->Close();
        delete AvailFiles[i].CFileObject;

    }

}
	

//Send the request, and start listening
void CFileXfer::GetFile(char *User,int Offernum,char *FileName,long IP)
{
	
	char databuff[1100];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)&databuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	sFileReq GetFileReq;

	if(IncomingFile.User[0]!=0)
	{
		//already getting a file
		CString l_tmpmsg;
		l_tmpmsg.LoadString(IDS_STRXFERINPRGS);
		((CPconwApp*)AfxGetApp())->DispatchMessage(l_tmpmsg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
		//DisplayMessage((char *)LPCSTR(l_tmpmsg));
		return;
	}
	
	GetFileReq.offernum=Offernum;
	GetFileReq.reqsig=0; //later for multipile requests.

	strcpy(IncomingFile.User,User);
	strcpy(IncomingFile.FileName,FileName);
	IncomingFile.StartTime=GetCurrentTime();
	IncomingFile.born=0; //check again, and resend if no fileinfo in 2 seconds.
	IncomingFile.offernum=Offernum;
	IncomingFile.TotalSize=0;
	//IncomingFile.FileFinAck=0;
	IncomingFile.SenderIP=IP;
	pchatpkthdr->type = FILEREQ;
	
	pchatpkthdr->ipaddress = IP;
	strcpy(pchatpkthdr->name, Handle);
	strcpy(pchatpkthdr->channel, Channel);
	
	memcpy(pchatpktdata->framedata.textframe.text,&GetFileReq,sizeof(sFileReq) );
	
	CString l_tmprqmsg;
	l_tmprqmsg.LoadString(IDS_STRRQDILE);
	m_MainStatusBar->SetPaneText(0,l_tmprqmsg);
	
	pchatpktdata->framedata.textframe.size = sizeof(sFileReq);
	m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
		pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
		0x08A9, "127.0.0.1", 0);

	return;
}




//return Offernum on success, 0 on file not found, -1 on out of offer slots
int CFileXfer::OfferFile(char *FileName)	
{
	int OfferIndex=0;
	int i;

	for(i=0;i<MAXOFFERS;i++)
	{
		if(AvailFiles[i].size==0) 
		{
			OfferIndex=i+1;
			i=MAXOFFERS;
			break;
		}
	
	}
	//-1 on out of offer slots	
	if(OfferIndex==0) return -1;
	
	AvailFiles[OfferIndex-1].CFileObject = new CFile;
	//0 on file not found
	if(!AvailFiles[OfferIndex-1].CFileObject->Open(FileName,CFile::modeRead)) return 0;
	
	AvailFiles[OfferIndex-1].size=AvailFiles[OfferIndex-1].CFileObject->GetLength();
	AvailFiles[OfferIndex-1].FileName = FileName;
	return OfferIndex;
}


void CFileXfer::Deoffer(int Idx)
{
	if(AvailFiles[Idx].size==0) return;
	AvailFiles[Idx].CFileObject->Close();
	delete AvailFiles[Idx].CFileObject;
	AvailFiles[Idx].size=0;
	return;
}

void CFileXfer::ProcessFileReq (char *data)
{
	int i;
	int iXferNum=-1;
	sFileReq *ReqPacket;
	sFileInfo AnswerFileInfo;
	
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));

	ReqPacket=(sFileReq *)pchatpktdata->framedata.textframe.text;
	char ReqMsg[80];

	//first check to get a free in progress slot
	for(i=0;i<MAXINPROGRESS;i++) 
		if(InProgressFiles[i].User[0]==0)
		{
			iXferNum=i;
			i=MAXINPROGRESS;
			break;
		}

	if((AvailFiles[ReqPacket->offernum].size>0) && (iXferNum!=-1))
	{
		
		//set up struct, and send file_info
		CString l_tmprqmsgA;
		l_tmprqmsgA.LoadString(IDS_STRREQFILE);
		sprintf(ReqMsg,"** %s %s [%d].",pchatpkthdr->name,l_tmprqmsgA,ReqPacket->offernum);
		//DisplayMessage(ReqMsg);
		((CPconwApp*)AfxGetApp())->DispatchMessage(ReqMsg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
		
		strcpy(InProgressFiles[iXferNum].User,pchatpkthdr->name);
		InProgressFiles[iXferNum].FileObj=AvailFiles[ReqPacket->offernum].CFileObject;
		InProgressFiles[iXferNum].pack1ofs=0;
		InProgressFiles[iXferNum].AllInBuff=0;
		InProgressFiles[iXferNum].NextOfs=0;
		InProgressFiles[iXferNum].pack1ackseq=Sequence;
		InProgressFiles[iXferNum].pack1born=0;
		InProgressFiles[iXferNum].FileFinAck=0;
		InProgressFiles[iXferNum].IP=pchatpkthdr->ipaddress;
		AnswerFileInfo.Sequence=Sequence;
		Sequence++;

		AnswerFileInfo.size=AvailFiles[ReqPacket->offernum].size; //size of the file
		InProgressFiles[i].FileSize=AvailFiles[ReqPacket->offernum].size;;
		AnswerFileInfo.reqsig=ReqPacket->reqsig; //for the requestor...you asked for it...
		InProgressFiles[i].reqsig=ReqPacket->reqsig;
		AnswerFileInfo.fileident=iXferNum; //number to use to refer to this xfer later.
		AnswerFileInfo.CRC=0;
		pchatpkthdr->type = FILEINFO;
		
		pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
		strcpy(pchatpkthdr->name, Handle);
		strcpy(pchatpkthdr->channel, Channel);
		
		memcpy(pchatpktdata->framedata.textframe.text,&AnswerFileInfo,sizeof(sFileInfo) );
		
		pchatpktdata->framedata.textframe.size = sizeof(sFileInfo);
		m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
			0x08A9, "127.0.0.1", 0);


	}
	else
	{
		//send a packet saying invalid request
		AnswerFileInfo.Sequence=Sequence;
		Sequence++;

		AnswerFileInfo.size=0;
		AnswerFileInfo.reqsig=ReqPacket->reqsig;
		
		pchatpkthdr->type = FILEINFO;
		
		pchatpkthdr->ipaddress = pchatpkthdr->ipaddress;
		strcpy(pchatpkthdr->name, Handle);
		strcpy(pchatpkthdr->channel, Channel);
		
		memcpy(pchatpktdata->framedata.textframe.text,&AnswerFileInfo,sizeof(sFileInfo) );
		
		pchatpktdata->framedata.textframe.size = sizeof(sFileInfo);
		m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
			0x08A9, "127.0.0.1", 0);

	}

}

void CFileXfer::InitFileXfer(void)
{
	m_MainStatusBar = ((CPconwApp*)AfxGetApp())->m_MainStatusBar;
	m_pSocket = ((CPconwApp*)AfxGetApp())->m_pSocket;
	strcpy(Handle,LPCSTR(((CPconwApp*)AfxGetApp())->m_strHandle));
	strcpy(Channel,LPCSTR(((CPconwApp*)AfxGetApp())->m_Channel));
	//m_ChatDoc = ChatDoc;
	
}

void CFileXfer::ProcessFileInfo (char *data)
{
	int AckSeq;
	long AckIP;
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	
	sFileInfo *IncomingFileInfo=(sFileInfo *)pchatpktdata->framedata.textframe.text;
	
	//make sure we are expecting this.....
	if(IncomingFile.TotalSize!=0) return;
	
	if(IncomingFileInfo->size==0)
	{
		CString l_tmpmsgfna;
		l_tmpmsgfna.LoadString(IDS_STRFILENA);
		m_MainStatusBar->SetPaneText(0,l_tmpmsgfna);
		IncomingFile.TotalSize=0;
		IncomingFile.User[0]=0;
		return;
	}

	IncomingFile.FileObject.Open(IncomingFile.FileName,CFile::modeCreate|CFile::modeWrite);
	
	AckSeq=IncomingFileInfo->Sequence;
	AckIP=pchatpkthdr->ipaddress;

	//fill in the rest of the data
	IncomingFile.TotalSize=IncomingFileInfo->size;
	//Display receiving file info in statusbar
	
	CString l_tmpmsgrfi;
	l_tmpmsgrfi.LoadString(IDS_STRRCVFILEINFO);
	m_MainStatusBar->SetPaneText(0,l_tmpmsgrfi);
	//CStatusBar
	SendAck (AckSeq,AckIP);
	return;
}

void CFileXfer::SendAck (int Seq,long IP)
{
	char databuff[1100];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)&databuff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));	
	
	//Should work.....
	pchatpkthdr->type = FILEACK;
	pchatpkthdr->ipaddress = IP;
	strcpy(pchatpkthdr->name, Handle);
	strcpy(pchatpkthdr->channel, Channel);
	memcpy(pchatpktdata->framedata.textframe.text,&Seq,sizeof(Seq) );
	pchatpktdata->framedata.textframe.size = sizeof(Seq);
	m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
		pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
		0x08A9, "127.0.0.1", 0);

	return;
}

void CFileXfer::ProcessAck (char *data)
{
	char Buff[1500];
	long IP;
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));	
	sFileFrag FileFragment;
	int i;
	int xxx;
	char DoneMsg[60];
	
	//Search through outgoing files for this ack....see if it is time do do something.
	int *AckNum = (int *)pchatpktdata->framedata.textframe.text;

	for(i=0;i<MAXINPROGRESS;i++) 
	{
													 
		if((*AckNum==InProgressFiles[i].pack1ackseq) && (InProgressFiles[i].User[0]!=0))
		{
		//check to see if we need to bother....
		if(InProgressFiles[i].AllInBuff==1) 
		{
			if(InProgressFiles[i].FileFinAck)
			{
				//This means we are done...stop sending....
				CString l_tmpmsgfxfrto,l_tmpmsgcomplete;
				l_tmpmsgfxfrto.LoadString(IDS_STRFXFRTO);
				l_tmpmsgcomplete.LoadString(IDS_STRCOMPLETE);
				sprintf(DoneMsg,"** %s %s %s.",LPCSTR(l_tmpmsgfxfrto),InProgressFiles[i].User,LPCSTR(l_tmpmsgcomplete));
				//DisplayMessage(DoneMsg);

				InProgressFiles[i].User[0]=0;
				InProgressFiles[i].pack1ackseq=0;
				InProgressFiles[i].pack1ofs=0; //never resend on the timer if pack1ofs==0
				return;
			}
			//FileFinAck
			//if FileFinAck is set, close the file....
			else 
			{
			//send a file complete packet
			pchatpkthdr->ipaddress=IP;
			pchatpkthdr->type = FILEFIN;
			FileFragment.Sequence=Sequence;
			strcpy(pchatpkthdr->name, Handle);
			strcpy(pchatpkthdr->channel, Channel);
			
			pchatpktdata->framedata.textframe.size = sizeof(sFileFrag)-DATASIZE;
			
			m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
				0x08A9, "127.0.0.1", 0);
			InProgressFiles[i].FileFinAck=1;
			return;
			}

			
		}

		//Read the next chunk, and send away
		InProgressFiles[i].FileObj->Seek(InProgressFiles[i].NextOfs,CFile::begin);
		FileFragment.size=InProgressFiles[i].FileObj->Read(&FileFragment.data,DATASIZE);
		FileFragment.Sequence=Sequence;
		FileFragment.offset=InProgressFiles[i].NextOfs;
		InProgressFiles[i].pack1ackseq=Sequence;
		Sequence++;
		InProgressFiles[i].NextOfs+=FileFragment.size;
		InProgressFiles[i].pack1born=0;
		InProgressFiles[i].pack1resent=0;
		//if the amount read is smaller than DATASIZE
		if(FileFragment.size<DATASIZE) InProgressFiles[i].AllInBuff=1;
		
		
		IP = pchatpkthdr->ipaddress;
		
		pchatpkthdr=(PCHAT_PACKET_HDR)&Buff;
		pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));	
		
		memcpy(pchatpktdata->framedata.textframe.text,&FileFragment,sizeof(sFileFrag) );
		
		pchatpkthdr->ipaddress=IP;
		pchatpkthdr->type = FILEFRAG;
		strcpy(pchatpkthdr->name, Handle);
		strcpy(pchatpkthdr->channel, Channel);
		
		pchatpktdata->framedata.textframe.size = sizeof(sFileFrag);
		
		xxx=m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
			pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
			0x08A9, "127.0.0.1", 0);
		}
	}
}

void CFileXfer::ProcessFileFrag(char *data)
{
	int AckSeq;
	long AckIP;
	CString donemsg;  //CFile
	char filesize[40];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	
	sFileFrag *IncomingFrag=(sFileFrag*)pchatpktdata->framedata.textframe.text;
	
	if(IncomingFile.User[0]==0) return;
	//if(strcmp(IncomingFile.User,pchatpkthdr->name)!=0) return;
	
	//this should account for very lagged packets, which we have already received
	//if(IncomingFile.FileObject.GetPosition()>IncomingFrag->offset) return;

	IncomingFile.born=0;
	IncomingFile.FileObject.Seek(IncomingFrag->offset,CFile::begin);
	IncomingFile.FileObject.Write(IncomingFrag->data,IncomingFrag->size);
	
	AckSeq=IncomingFrag->Sequence;
	AckIP=pchatpkthdr->ipaddress;
	
	sprintf(filesize,"%d",IncomingFrag->offset);
	donemsg = IncomingFile.FileName;
	donemsg += _T(": ");
	donemsg += filesize;
	
	CString l_tmpmsgbr;
	l_tmpmsgbr.LoadString(IDS_STRBYTERCV);
	
	donemsg += _T(" ");
	donemsg += l_tmpmsgbr;
	m_MainStatusBar->SetPaneText(0,LPCSTR(donemsg));

	SendAck (AckSeq,AckIP);
}

void CFileXfer::EnterIdle(void)
{
	char Buff[1500];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)&Buff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));	
	sFileFrag FileFragment;
	sFileInfo AnswerFileInfo;
	int i;
	char DispMsg[60];

	//check incoming...time out if no fragment for 60 seconds
	if(IncomingFile.User[0]!=0)
	{
		IncomingFile.born++;
		if(IncomingFile.born>=240)
		{
			IncomingFile.FileObject.Abort();
			IncomingFile.User[0]=0;
			CString l_tmpmsgfxto;
			l_tmpmsgfxto.LoadString(IDS_STRFILEXFRTO);
			//DisplayMessage((char *)LPCSTR(l_tmpmsgfxto));
			((CPconwApp*)AfxGetApp())->DispatchMessage(l_tmpmsgfxto,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
			l_tmpmsgfxto.LoadString(IDS_STRTIMEOUT);
			m_MainStatusBar->SetPaneText(0,l_tmpmsgfxto);
		}

	}

	//Scan through all the inprogress files, resending when needed.
	//also, expire after excessive resends.
		for(i=0;i<MAXINPROGRESS;i++) 
		{
			if(InProgressFiles[i].User[0]!=0)
			{
				InProgressFiles[i].pack1born++;
				if(InProgressFiles[i].pack1born>=8)
				{
					InProgressFiles[i].pack1born==0;
					if(InProgressFiles[i].pack1resent>100)
					{
						//if the pack1resent feild > 20, abort xfer
						CString l_tmpmsgfxfrto,l_tmpmsgcomplete;
						l_tmpmsgfxfrto.LoadString(IDS_STRFXFRTO);
						l_tmpmsgcomplete.LoadString(IDS_STRTO);
						sprintf(DispMsg,"** %s %s %s.",LPCSTR(l_tmpmsgfxfrto),InProgressFiles[i].User,LPCSTR(l_tmpmsgcomplete));
												
						//DisplayMessage(DispMsg);
						((CPconwApp*)AfxGetApp())->DispatchMessage(DispMsg,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);

						InProgressFiles[i].User[0]=0;
						//print canceled message
						break;
					}
					//increment the pack1resent field
					InProgressFiles[i].pack1resent++;
					
					//if pack1ofs == 0...and NextOfs = 0, send file info.
					if((InProgressFiles[i].pack1ofs==0) && (InProgressFiles[i].NextOfs==0))
					{
					//resend fileinfo
						AnswerFileInfo.size=InProgressFiles[i].FileSize; //size of the file
						AnswerFileInfo.reqsig=InProgressFiles[i].reqsig; //for the requestor...you asked for it...
						AnswerFileInfo.fileident=i; //number to use to refer to this xfer later.
						pchatpkthdr->type = FILEINFO;
						
						pchatpkthdr->ipaddress = InProgressFiles[i].IP;
						strcpy(pchatpkthdr->name, Handle);
						strcpy(pchatpkthdr->channel, Channel);
						
						memcpy(pchatpktdata->framedata.textframe.text,&AnswerFileInfo,sizeof(sFileInfo) );
						
						pchatpktdata->framedata.textframe.size = sizeof(sFileInfo);
						m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
							pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
							0x08A9, "127.0.0.1", 0);
						return;
					}
					if(InProgressFiles[i].FileFinAck)
					{
						//send a file complete packet
						pchatpkthdr->ipaddress=InProgressFiles[i].IP;
						pchatpkthdr->type = FILEFIN;
						FileFragment.Sequence=Sequence;
						strcpy(pchatpkthdr->name, Handle);
						strcpy(pchatpkthdr->channel, Channel);
						
						pchatpktdata->framedata.textframe.size = sizeof(sFileFrag)-DATASIZE;
						
						m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
							pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
							0x08A9, "127.0.0.1", 0);
						return;
					}

					//Reread the file chunk, and send away
					InProgressFiles[i].FileObj->Seek(InProgressFiles[i].pack1ofs,CFile::begin);
					FileFragment.size=InProgressFiles[i].FileObj->Read(&FileFragment.data,DATASIZE);
					FileFragment.Sequence=InProgressFiles[i].pack1ackseq;
					FileFragment.offset=InProgressFiles[i].pack1ofs;
										
					memcpy(pchatpktdata->framedata.textframe.text,&FileFragment,sizeof(sFileFrag) );
					
					pchatpkthdr->ipaddress=InProgressFiles[i].IP;
					pchatpkthdr->type = FILEFRAG;
					strcpy(pchatpkthdr->name, Handle);
					strcpy(pchatpkthdr->channel, Channel);
					
					pchatpktdata->framedata.textframe.size = sizeof(sFileFrag);
					m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
						pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
						0x08A9, "127.0.0.1", 0);

				}
			}
		}
}

void CFileXfer::DisplayMessage (char *Msg)
{
	DWORD tmpclr;
	tmpclr=((CPconwApp*)AfxGetApp())->colorSTAT;
	ASSERT(FALSE);
	m_ChatDoc->DisplayMsg(Msg,tmpclr);

}

void CFileXfer::CancelGet (void)
{
	
	CString l_tmpmsgnf;
	l_tmpmsgnf.LoadString(IDS_STRNOFILE);
	if(IncomingFile.User[0]==0) ((CPconwApp*)AfxGetApp())->DispatchMessage(l_tmpmsgnf,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
		//DisplayMessage((char *)LPCSTR(l_tmpmsgnf));
	else
	{
			//send cancel message
			
			//if(IncomingFrag->offset!=0) 
			IncomingFile.FileObject.Abort();
			IncomingFile.User[0]=0;
			CString l_tmpmsgcancelxfr;
			l_tmpmsgcancelxfr.LoadString(IDS_STRCANCELXFR);
			//DisplayMessage((char *)LPCSTR(l_tmpmsgcancelxfr));
			((CPconwApp*)AfxGetApp())->DispatchMessage(l_tmpmsgcancelxfr,((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
			l_tmpmsgcancelxfr.LoadString(IDS_STRDWNLDABORT);
			m_MainStatusBar->SetPaneText(0,l_tmpmsgcancelxfr);
			
	}
}

void CFileXfer::Progress(void)
{
	char szProgress[80];
	int i;
	for(i=0;i<MAXINPROGRESS;i++) 
	{
		if(InProgressFiles[i].User[0]!=0)
		{
			CString l_tmpmsga,l_tmpmsgb;
			l_tmpmsga.LoadString(IDS_STRSENT);
			l_tmpmsgb.LoadString(IDS_STRBYTESTO);
			sprintf(szProgress,"** %s %d %s %s.",LPCSTR(l_tmpmsga),InProgressFiles[i].NextOfs,LPCSTR(l_tmpmsgb),InProgressFiles[i].User);
			//DisplayMessage(szProgress);
			((CPconwApp*)AfxGetApp())->DispatchMessage(CString(szProgress),((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
		}
	}
}

void CFileXfer::ShowOffers(long showIP)
{
	char Buff[1500];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)&Buff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	char szOffers[80];
	int i;
	for(i=0;i<MAXOFFERS;i++)
		{
			if(AvailFiles[i].size!=0) 
			{
				CString l_tmpmsgio,l_tmpmsgaf;
				
				l_tmpmsgio.LoadString(IDS_STRISOFFER);
				l_tmpmsgaf.LoadString(IDS_STRASFILE);
				
				sprintf(szOffers,"* %s %s %s %s [%d].",Handle,LPCSTR(l_tmpmsgio),LPCSTR(AvailFiles[i].FileName),LPCSTR(l_tmpmsgaf),i);
				if(!showIP) ((CPconwApp*)AfxGetApp())->DispatchMessage(szOffers,((CPconwApp*)AfxGetApp())->colorACT,NULL);
					//((CPconwDoc*)((CPconwApp*)AfxGetApp())->m_ChannelDoc)->DisplayMsg(szOffers,((CPconwApp*)AfxGetApp())->colorACT);
				//sprintf(szOffers,"is offering %s as file [%d].",LPCSTR(AvailFiles[i].FileName),i);
				sprintf(szOffers,"%s %s %s [%d].",LPCSTR(l_tmpmsgio),LPCSTR(AvailFiles[i].FileName),LPCSTR(l_tmpmsgaf),i);
				//*******^^^
				pchatpkthdr->type = HIGH_TEXT;
				pchatpkthdr->ipaddress = showIP;
				strcpy(pchatpkthdr->name, Handle);
				strcpy(pchatpkthdr->channel, Channel);
				strcpy(pchatpktdata->framedata.textframe.text,szOffers);
				pchatpktdata->framedata.textframe.size = strlen(szOffers)+1;
				m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
			}
		
		}
}

void CFileXfer::ProcessFileFin(char *data)
{
	long XferTime;

	long Cps;

	int AckSeq;
	long AckIP;
	CString donemsg;
	char filesize[40];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)data;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	
	sFileFrag *IncomingFrag=(sFileFrag*)pchatpktdata->framedata.textframe.text;
	
	//verify that a file is being received by this dude...
	////IncomingFile.SenderIP
	if(IncomingFile.User[0]==0) return;
	if(pchatpkthdr->ipaddress!=IncomingFile.SenderIP) return;


	//close the file, etc.
	XferTime=.001*(GetCurrentTime()-IncomingFile.StartTime);
	if(XferTime)Cps=IncomingFile.FileObject.GetLength()/XferTime;
	else Cps=IncomingFile.FileObject.GetLength()/1;
	//this is the end
	CString l_tmpmsgb,l_tmpmsgbps;
	
	l_tmpmsgb.LoadString(IDS_STRBYTES);
	l_tmpmsgbps.LoadString(IDS_STRBYTESSEC);

	sprintf(filesize,"%d %s %d %s.",IncomingFile.FileObject.GetLength(),LPCSTR(l_tmpmsgb),Cps,LPCSTR(l_tmpmsgbps));
	IncomingFile.FileObject.Close();
	IncomingFile.User[0]=0;
	donemsg = IncomingFile.FileName;
	CString l_tmpmsgrs;
	l_tmpmsgrs.LoadString(IDS_STRRCVSCS);
	
	donemsg += _T(" ");
	donemsg += l_tmpmsgrs;
	donemsg += filesize;
	
	m_MainStatusBar->SetPaneText(0,LPCSTR(donemsg));

	//send the ack
	AckSeq=IncomingFrag->Sequence;
	AckIP=pchatpkthdr->ipaddress;

	SendAck (AckSeq,AckIP);

}

void CFileXfer::SetChannel(const char *l_Channel)
{
	strcpy(Channel,l_Channel);
}

void CFileXfer::ShowOffers(LPCSTR l_Channel)
{
	char Buff[1500];
	PCHAT_PACKET_HDR pchatpkthdr;
	PCHAT_PACKET_DATA pchatpktdata;
	pchatpkthdr = (PCHAT_PACKET_HDR)&Buff;
	pchatpktdata = (PCHAT_PACKET_DATA)((PBYTE)pchatpkthdr + sizeof(CHAT_PACKET_HDR));
	char szOffers[80];
	int i;
	for(i=0;i<MAXOFFERS;i++)
		{
			if((AvailFiles[i].size!=0) && (l_Channel))
			{
				CString l_tmpmsgio,l_tmpmsgaf;
				
				l_tmpmsgio.LoadString(IDS_STRISOFFER);
				l_tmpmsgaf.LoadString(IDS_STRASFILE);
				
				sprintf(szOffers,"* %s %s %s %s [%d].",Handle,LPCSTR(l_tmpmsgio),LPCSTR(AvailFiles[i].FileName),LPCSTR(l_tmpmsgaf),i);
				//if(!showIP) ((CPconwDoc*)((CPconwApp*)AfxGetApp())->m_ChannelDoc)->DisplayMsg(szOffers,((CPconwApp*)AfxGetApp())->colorACT);
				((CPconwApp*)AfxGetApp())->DispatchMessage(szOffers,((CPconwApp*)AfxGetApp())->colorACT,l_Channel);

				//sprintf(szOffers,"is offering %s as file [%d].",LPCSTR(AvailFiles[i].FileName),i);
				sprintf(szOffers,"%s %s %s [%d].",LPCSTR(l_tmpmsgio),LPCSTR(AvailFiles[i].FileName),LPCSTR(l_tmpmsgaf),i);
				//*******^^^
				pchatpkthdr->type = HIGH_TEXT;
				pchatpkthdr->ipaddress = 0;
				strcpy(pchatpkthdr->name, Handle);
				strcpy(pchatpkthdr->channel, l_Channel);
				strcpy(pchatpktdata->framedata.textframe.text,szOffers);
				pchatpktdata->framedata.textframe.size = strlen(szOffers)+1;
				m_pSocket->SendTo(pchatpkthdr, sizeof(CHAT_PACKET_HDR) + 
				pchatpktdata->framedata.textframe.size + sizeof(pchatpktdata->framedata.textframe.size),
					0x08A9, "127.0.0.1", 0);
			}
		
		}
}
