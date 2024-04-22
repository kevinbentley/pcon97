// GetFileDlg.cpp : implementation file
//

#include "stdafx.h"

#include "GetFileDlg.h"




/////////////////////////////////////////////////////////////////////////////
// CGetFileDlg dialog


CGetFileDlg::CGetFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetFileDlg)
	m_GetNum = _T("0");
	m_GetFile1Name = _T("");
	//}}AFX_DATA_INIT
}


void CGetFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetFileDlg)
	DDX_Text(pDX, IDC_OFFERNUM, m_GetNum);
	DDV_MaxChars(pDX, m_GetNum, 1);
	DDX_Text(pDX, IDC_FILE1NAME, m_GetFile1Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetFileDlg, CDialog)
	//{{AFX_MSG_MAP(CGetFileDlg)
	ON_BN_CLICKED(IDC_GETFILENAME, OnGetfilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetFileDlg message handlers

void CGetFileDlg::OnGetfilename() 
{
	UpdateData(TRUE);
	CFileDialog FileDlg(TRUE,NULL,m_GetFile1Name,OFN_PATHMUSTEXIST,NULL,this);
	if(IDCANCEL==FileDlg.DoModal()) return;
	m_GetFile1Name=FileDlg.GetPathName();
	SetDlgItemText(IDC_FILE1NAME,m_GetFile1Name);
	UpdateData(TRUE);   //CEdit
	
}

void CGetFileDlg::OnOK() 
{
	CString cs_MsgCommand;
	UpdateData(TRUE);
	if(m_GetFile1Name.IsEmpty()) return;
	if(m_GetNum.IsEmpty()) return;
		
	CDialog::OnOK();
}
