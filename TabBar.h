#if !defined(AFX_TABBAR_H__BF009261_8144_11D1_BA39_00A0C95C1A08__INCLUDED_)
#define AFX_TABBAR_H__BF009261_8144_11D1_BA39_00A0C95C1A08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabBar.h : header file
//

#include "ColorTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTabBar dialog

class CTabBar : public CDialogBar
{
// Construction
public:
	void SetSelTab(CString &Title);
	CString GetSelTab(void);
	void NewMessage(CString &Title);
	void RemoveTab(CString &strRemove);
	void ReSizeTabBar(void);
	void InsertTab(CString &strTab);
	CTabCtrl * m_TabCtrl;
	CTabBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabBar)
	enum { IDD = IDD_TABVIEW };
	CColorTabCtrl	m_ColorTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabBar)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabBar)
	afx_msg void OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBAR_H__BF009261_8144_11D1_BA39_00A0C95C1A08__INCLUDED_)
