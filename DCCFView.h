#if !defined(AFX_DCCFVIEW_H__1AAD1CC3_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DCCFVIEW_H__1AAD1CC3_22B6_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DCCFView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCCFView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#define DCC_PACKETSIZE 1024
class CDCCFView : public CFormView
{
protected:
	CDCCFView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCCFView)
	CDCCSocket * m_XferSocket;
	CDCCSocket * m_ListenSocket;
	char m_SendBuff[DCC_PACKETSIZE+1];

// Form Data
public:
	//{{AFX_DATA(CDCCFView)
	enum { IDD = IDD_DCC_FILE };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CString m_FileName;
	CString m_NewTitle;
	unsigned int m_FileLen;
	UINT m_LenSent;
	CFile m_DCCFile;
	int m_DCCMode;
	void ConnectDCC(UINT ip, short socket,CString FileName,CString User,UINT size);
	int ListenDCC(CString User,CString FileName);

	void OnDCCClose(void);
	void OnConnectXfer(void);
	void OnReceive(void);
	void OnAcceptConn(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCFView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCCFView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDCCFView)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCCFVIEW_H__1AAD1CC3_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
