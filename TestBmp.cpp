// TestBmp.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "TestBmp.h"

/////////////////////////////////////////////////////////////////////////////
// CTestBmp dialog


CTestBmp::CTestBmp(CWnd* pParent /*=NULL*/)
	: CDialog(CTestBmp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestBmp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestBmp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestBmp)
	DDX_Control(pDX, IDC_REALICON, m_RealIcon);
	DDX_Control(pDX, IDC_PICTURE1, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestBmp, CDialog)
	//{{AFX_MSG_MAP(CTestBmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBmp message handlers

void CTestBmp::SetBmp (HBITMAP mybmp)
{
	m_bmp=mybmp;
	

}

BOOL CTestBmp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	l_ConvertImage.Create(32,32,FALSE,20,20);


	l_Icon=ExtractIcon(AfxGetInstanceHandle(),m_IconName,0);
	
	//l_Icon=AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);
	//l_inum=l_ConvertImage.Add(l_Icon);

	//l_ConvertImage.GetImageInfo(l_inum,&l_ii);
		
	//l_bmp.Attach(l_ii.hbmImage);
	
	//m_bmp=(HBITMAP)l_bmp.GetSafeHandle();
	
	


	CDCIcon.CreateCompatibleDC(NULL);
	
	BOOL l_btest=CDCIcon.DrawIcon(1,1,l_Icon);
	
	m_RealIcon.SetIcon(l_Icon);        
    m_MemDC.CreateCompatibleDC(NULL);
	newBitmap.CreateCompatibleBitmap(&m_MemDC, 33, 33);
	pOldBitmap = m_MemDC.SelectObject(&newBitmap);

//CStatic//GetDC
    // lx, ly is the left upper point of B on the screen
    l_btest=m_MemDC.BitBlt(1, 1, 32, 32, &CDCIcon, 1, 1, SRCCOPY);

	m_bmp=(HBITMAP)newBitmap.GetSafeHandle();

	m_Picture.SetBitmap(m_bmp);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestBmp::SetIconName (LPCSTR IconName)
{

	m_IconName = IconName;

}
