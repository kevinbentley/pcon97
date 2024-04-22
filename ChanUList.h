// ChanUList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChanUList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "IconListBox.h"

class CChanUList : public CFormView
{
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CChanUList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChanUList)

// Form Data
public:
	BOOL m_bInit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL DelChanUser(LPCSTR User);
	void AddChanUser(LPCSTR User);
	//{{AFX_DATA(CChanUList)
	enum { IDD = IDD_CHANULIST };
	CIconListBox	m_lbUserList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CDC m_dcCompatible;
	CBitmap m_LagBitmaps;
	HICON m_hiconUnknown;
	void KickUser();
	void TakeVoice();
	void TakeOps(void);
	void GrantVoice(void);
	void GrantOps(void);
	BOOL IsUserHere(CString & User);
	void ChangeNick(CString &newnick, CString &oldnick);
	void ChangeUserMode(CString &mode,CString &User);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChanUList)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChanUList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CChanUList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWhoisuser();
	afx_msg void OnSendmsg();
	afx_msg void OnPing();
	afx_msg void OnGetuserfile();
	afx_msg void OnDblclkList1();
	afx_msg void OnPopupToggleignore();
	afx_msg void OnUpdatePopupToggleignore(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeList1();
	afx_msg void OnPaint();
	afx_msg void OnUpdateTakeops(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTakevoice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrantops(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrantvoice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKickuser(CCmdUI* pCmdUI);
	afx_msg void OnDccchat();
	afx_msg void OnDccsend();
	afx_msg void OnUdpping();
	afx_msg void OnIgnoreuser();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
