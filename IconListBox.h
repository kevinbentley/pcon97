#if !defined(AFX_ICONLISTBOX_H__553684A3_854C_11D1_BA39_00A0C95C1A08__INCLUDED_)
#define AFX_ICONLISTBOX_H__553684A3_854C_11D1_BA39_00A0C95C1A08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IconListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconListBox window

class CIconListBox : public CListBox
{
// Construction
public:
	CIconListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconListBox)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONLISTBOX_H__553684A3_854C_11D1_BA39_00A0C95C1A08__INCLUDED_)
