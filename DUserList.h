// DUserList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDUserList dialog

class CDUserList : public CDialogBar
{
// Construction
public:
	CString ExpandString(CString OrigStr);
	CDUserList();   // standard constructor

	void RemoveExactItem (char *Item);
	void AddItem (char *Item);
	CString m_TmpStr;
	//{{AFX_DATA(CDUserList)
	enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDUserList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);

	// Generated message map functions
	//{{AFX_MSG(CDUserList)
	afx_msg void OnPing();
	afx_msg void OnSendmsg();
	afx_msg void OnDblclkList1();
	afx_msg void OnToggleIgnore();
	afx_msg void OnSelchangeList1();
	afx_msg void OnGetuserfile();
	afx_msg void OnWhoisuser();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateWhoisuser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSendmsg(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePing(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGetuserfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupToggleignore(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
