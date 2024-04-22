#ifndef _CFileXfer_
#define _CFileXfer_

//CFileXfer header


#pragma pack(1)
// Maximum different files offered
#define MAXOFFERS 4
#define MAXINPROGRESS 5
#define DATASIZE 800

typedef struct sFileInProgress
{
	char User[17];			//Is "" if unused
	long IP;
	CFile *FileObj;			//Pointer to CFile
	long pack1ofs;			//Offset for next packet to send
	int pack1ackseq;		//Sequence # to expect an ack from
	long NextOfs;			//The offset to use for the next send packet.
	unsigned char pack1born;//Use this to resend every 2 seconds, if needed.
	unsigned char pack1resent;//keep track of retires
	unsigned char AllInBuff;//Everything is in the buffer, when all offsets are 0, done.
	int offernum;
	long FileSize;
	int reqsig;
	int FileFinAck;
}sFileInProgress;


typedef struct sFileReq 
{
	int reqsig;				//Id from the requestor
	int offernum;			//which file they want
}sFileReq;

typedef struct sFileFrag
{
	int Sequence;			//so you can ack it
	int fileident;			//keep track of which xfer this is.
	long offset;			//where in the file is this starting from?
	unsigned long crc;		//seeded crc
	int size;				//size of this fragment
	char data[DATASIZE+1];		//max we will use at this time is 1000
}sFileFrag;

typedef struct sFileInfo
{
	int Sequence;			//so you can ack it
	int reqsig;				//Id from the requestor
	int fileident;			//keep track of which xfer this is.
	long size;				//size of this file
	long CRC;				//crc....if 0, ignore...
	int packetsize;			//size of the packet...for future use...
}sFileInfo;

typedef struct sOfferedFiles
{
	CFile *CFileObject;
	long size;
	CString FileName;

}sOfferedFiles;

typedef struct sIncomingFile
{
	char User[17];
	int offernum;
	char FileName[255];
	CFile FileObject;
	long TotalSize;
	long Progress;
	int born;
	unsigned long SenderIP;
	long StartTime;
	//int FileFinAck;
}sIncomingFile;

#pragma pack()

class CFileXfer : public CObject
{
public:
	void ShowOffers(LPCSTR Channel);
	void ProcessFileFin(char *data);
	char Channel[17];
	void SetChannel(const char *l_Channel);
	void ShowOffers(long showIP);
	void Progress(void);
	char Handle[17];
	CPconwDoc * m_ChatDoc;
	void CancelGet (void);
	void DisplayMessage (char *Msg);
	CStatusBar * m_MainStatusBar;
	void EnterIdle(void);
	void ProcessFileFrag(char *data);
	void ProcessAck (char *data);
	void SendAck (int Seq,long IP);
	void ProcessFileInfo (char *data);
	void InitFileXfer(void);
	void ProcessFileReq (char *Data);
	void Deoffer(int Idx);
	CFileXfer();
	~CFileXfer();
	
	void GetFile(char *User,int Offernum,char *FileName,long IP); //Send the request, and start listening
	int OfferFile(char *FileName);	//return 1 on success, 0 on file not found, -1 on out of offer slots

protected:
	//all the available files
	sOfferedFiles AvailFiles[MAXOFFERS];
	sFileInProgress InProgressFiles[MAXINPROGRESS];
	sIncomingFile IncomingFile;
private:
	CString *m_Channel;
	CString *m_strHandle;
	CChatSocket *m_pSocket;
	unsigned int Sequence;

};


#endif