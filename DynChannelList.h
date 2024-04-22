#if !defined(AFX_DYNCHANNELLIST_H__618C03A3_8449_11D1_BA39_00A0C95C1A08__INCLUDED_)
#define AFX_DYNCHANNELLIST_H__618C03A3_8449_11D1_BA39_00A0C95C1A08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DynChannelList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDynChannelList dialog

class CDynChannelList : public CDialog
{
// Construction
public:
	char * GetWordNum(int num, CString & l_String);
	void RefreshList(CStringList &slChannels);
	CDynChannelList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDynChannelList)
	enum { IDD = IDD_CHANNELLIST };
	CListBox	m_lbChannels;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynChannelList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDynChannelList)
	afx_msg void OnJoinchannel();
	afx_msg void OnDblclkList1();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNCHANNELLIST_H__618C03A3_8449_11D1_BA39_00A0C95C1A08__INCLUDED_)
