#if !defined(AFX_FINGERFRAME_H__62EDB902_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_FINGERFRAME_H__62EDB902_08E3_11D1_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FingerFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFingerFrame frame

class CFingerFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFingerFrame)
protected:
	CFingerFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFingerFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFingerFrame();

	// Generated message map functions
	//{{AFX_MSG(CFingerFrame)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERFRAME_H__62EDB902_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
