// DCCFView.cpp : implementation file
//
#define DCC_PACKETSIZE 1024
#include "stdafx.h"
#include "pconw.h"
#include "DCCSocket.h"
#include "DCCFView.h"



/////////////////////////////////////////////////////////////////////////////
// CDCCFView

IMPLEMENT_DYNCREATE(CDCCFView, CFormView)

CDCCFView::CDCCFView()
	: CFormView(CDCCFView::IDD)
{
	//{{AFX_DATA_INIT(CDCCFView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DCCMode=0;
	m_XferSocket=NULL;
	m_ListenSocket=NULL;
}

CDCCFView::~CDCCFView()
{
}

void CDCCFView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCCFView)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCCFView, CFormView)
	//{{AFX_MSG_MAP(CDCCFView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCCFView diagnostics

#ifdef _DEBUG
void CDCCFView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDCCFView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCCFView message handlers

void CDCCFView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	//m_XferSocket
	// TODO: Add your specialized code here and/or call the base class
	
}

int CDCCFView::ListenDCC(CString User,CString FileName)
{
	//CString l_NewTitle;
	m_NewTitle.Format("DCC Send: %s",User);
	GetDocument()->SetTitle(m_NewTitle);
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(m_NewTitle,GetDocument());
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_NewTitle);
	//IDC_DCC_FILENAME
	//IDC_DCC_PROGRESS
	SetDlgItemText(IDC_DCC_FILENAME,FileName);
	SetDlgItemText(IDC_DCC_PROGRESS,"0 bytes sent");

	unsigned int port;
	CString l_SockAddr;
	struct in_addr IpAddressStruct;
	m_ListenSocket = new CDCCSocket(this,DCC_FILE_LISTEN);
	m_ListenSocket->Create(0,SOCK_STREAM,FD_ACCEPT);
	m_ListenSocket->GetSockName(l_SockAddr,port);
	//->GetLastError();
	m_FileName=FileName;
	m_ListenSocket->Listen(1);

	m_DCCMode=DCC_FILE_LISTEN;

	if(m_DCCFile.Open(FileName,CFile::modeRead|CFile::shareDenyNone))
	{
		m_FileLen=m_DCCFile.GetLength();
		//int testa;
		//testa=m_FileLen%65536;
		//if(m_FileLen>65535)
		//{

		//}
		m_Progress.SetRange(0,m_FileLen/1024);
	}
	else
	{
		//Can't send the file if we can't open it.
		return -1;
	}

	return port;
}

void CDCCFView::ConnectDCC(UINT ip, short socket,CString FileName,CString User,UINT size)
{
	//This is actually receive mode.....
	
	
	m_NewTitle.Format("DCC Receive: %s",User);
	GetDocument()->SetTitle(m_NewTitle);
	((CPconwApp*)AfxGetApp())->m_WindowMap.SetAt(m_NewTitle,GetDocument());
	//((CPconwApp*)AfxGetApp())->m_ChanList->AddWindow(m_NewTitle);
	UINT l_nIP;
	l_nIP=htonl(ip);
	struct in_addr IpAddressStruct;
	memcpy(&IpAddressStruct.S_un.S_addr,&l_nIP,4);
	m_XferSocket = new CDCCSocket(this,DCC_FILE_LISTEN);
	m_XferSocket->Create(0,SOCK_STREAM,FD_READ|FD_CLOSE|FD_CONNECT);
	m_XferSocket->Connect(inet_ntoa(IpAddressStruct),socket);
	m_DCCMode=DCC_FILE_CONNECT;
	SetDlgItemText(IDC_DCC_FILENAME,FileName);
	m_FileName=FileName;
	SetDlgItemText(IDC_DCC_PROGRESS,"0 bytes received");
	m_Progress.SetRange(0,size/1024);
	m_FileLen=size;
	m_LenSent=0;
	if(m_DCCFile.Open(FileName, CFile::modeCreate | CFile::modeWrite))
	{
		//
	}
		
}

void CDCCFView::OnDCCClose(void)
{
	//Send a message to the server window saying 
	//that the xfer is complete

	m_DCCFile.Abort();
	//close the doc
	CString l_FmtXferCompletion;
	if(m_FileLen)
	{
		if(m_FileLen==m_LenSent)
		{
			l_FmtXferCompletion.Format("** Transfer of file %s completed succesfully",m_FileName);
		}
		else
		{
			l_FmtXferCompletion.Format("** Transfer of file %s terminated unsuccesfully",m_FileName);
		}

	}
	else
	{
		l_FmtXferCompletion.Format("** DCC File transfer of file %s closed.",m_FileName);
	}
	
	((CPconwApp*)AfxGetApp())->DispatchMessage(LPCSTR(l_FmtXferCompletion),((CPconwApp*)AfxGetApp())->colorSTAT,NULL);


	GetDocument()->OnCloseDocument();

}

void CDCCFView::OnConnectXfer(void)
{
	//we should start receiving now
	

}

void CDCCFView::OnReceive(void)
{
#define RCVBUFFSIZE	2048
	char l_RcvBuff[RCVBUFFSIZE];
	//If we are in receive mode, write to the file, and send ACK
	if(m_DCCMode==DCC_FILE_CONNECT)
	{
		int l_rcvlen = m_XferSocket->Receive(&l_RcvBuff,RCVBUFFSIZE);
		int l_iAck;
		//l_RcvBuff
		m_DCCFile.Write(l_RcvBuff,l_rcvlen);
		m_LenSent+=l_rcvlen;
		l_iAck=htonl(m_LenSent);
		m_XferSocket->Send(&l_iAck,sizeof(int));
		CString l_Progress;
		l_Progress.Format("%u Bytes received...",m_LenSent);
		SetDlgItemText(IDC_DCC_PROGRESS,l_Progress);
		m_Progress.SetPos(m_LenSent/1024);
		return;
	}
	//in Send mode, make sure we have been acked as much as we sent
	//and send the next chunk.
	
	if(m_DCCMode==DCC_FILE_LISTEN)
	{
		char l_cbuff[200];
		unsigned int l_iAck,l_tmp;
		int l_rcvlen = m_XferSocket->Receive(&l_tmp,sizeof(int));
		l_iAck=ntohl(l_tmp);
		ASSERT(l_rcvlen==4);
		if(l_iAck==m_LenSent)
		{
			if(m_DCCFile.GetLength()==m_LenSent)
			{
				CString l_FmtXferCompletion;
				//The file is sent. Close the document.
				l_FmtXferCompletion.Format("** Transfer of file %s completed succesfully",m_FileName);
				((CPconwApp*)AfxGetApp())->DispatchMessage(LPCSTR(l_FmtXferCompletion),((CPconwApp*)AfxGetApp())->colorSTAT,NULL);
				GetDocument()->OnCloseDocument();
				return;
			}
			else
			{
				//Get the next packet and send it.
				int l_ReadLen;
				CString l_Progress;
				l_Progress.Format("%u Bytes sent...",l_iAck);
				SetDlgItemText(IDC_DCC_PROGRESS,l_Progress);
				l_ReadLen=m_DCCFile.Read(m_SendBuff,DCC_PACKETSIZE);
				m_XferSocket->Send(m_SendBuff,l_ReadLen);
				m_LenSent+=l_ReadLen;
				m_Progress.SetPos(m_LenSent/1024);
			}
			return;
		}
	}
	
	

}
void CDCCFView::OnAcceptConn(void)
{
	//make the real socket
	m_XferSocket = new CDCCSocket(this,DCC_FILE_LISTEN);

	m_ListenSocket->Accept(*m_XferSocket);
	m_XferSocket->AsyncSelect(FD_READ|FD_CLOSE);

	//send the first chunk
	
	//DCC_PACKETSIZE
	int l_ReadLen;
	m_FileLen = m_DCCFile.GetLength();
	l_ReadLen=m_DCCFile.Read(m_SendBuff,DCC_PACKETSIZE);
	m_LenSent=l_ReadLen;
	if(l_ReadLen==DCC_PACKETSIZE)
	{
		m_XferSocket->Send(m_SendBuff,l_ReadLen);
	}
	else
	{
		//that was one small file....
		m_XferSocket->Send(m_SendBuff,l_ReadLen);
	}

}

void CDCCFView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	((CPconwApp*)AfxGetApp())->m_WindowMap.RemoveKey(m_NewTitle);
	((CPconwApp*)AfxGetApp())->m_ChanList->CloseWindow(m_NewTitle);
	if(m_XferSocket) delete m_XferSocket;
	if(m_ListenSocket) delete m_ListenSocket;
	
}
