#ifndef _ChannelList_header 
#define _ChannelList_header

#include "resource.h"


// ChannelList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelList dialog

class CChannelList : public CDialogBar
{
// Construction
public:
	~CChannelList();
	void CloseWindow(LPCSTR Window);
	void AddWindow(LPCSTR Window);
	BOOL m_bFocus;
	BOOL m_bSelectedChan;
	void JoinSelected(void);
	BOOL iValid;
	void Refresh(void);
	void Init(void);
	void ProcessPart(LPCSTR User,LPCSTR Chan);
	void ProcessJoin(LPCSTR User,LPCSTR Chan);
	CChannelList(CWnd* pParent = NULL);   // standard constructor

	struct sChannelList
	{
		CString ChanName;
		CStringList Users;
		int Idx;
		int NumUsers;
		struct sChannelList *Next;
	}*sFirstChanList,*sCurrChanList,*sTempChanList;


// Dialog Data
	//{{AFX_DATA(CChannelList)
	enum { IDD = IDD_CHANNELBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChannelList)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSetfocusCombo1();
	afx_msg void OnDblclkCombo1();
	afx_msg void OnKillfocusCombo1();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif _ChannelList_header