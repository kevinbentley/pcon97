#if !defined(AFX_COLORTABCTRL_H__82168984_835C_11D1_BA39_00A0C95C1A08__INCLUDED_)
#define AFX_COLORTABCTRL_H__82168984_835C_11D1_BA39_00A0C95C1A08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorTabCtrl window

class CColorTabCtrl : public CTabCtrl
{
// Construction
public:
	CColorTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorTabCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorTabCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORTABCTRL_H__82168984_835C_11D1_BA39_00A0C95C1A08__INCLUDED_)
