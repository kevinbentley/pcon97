#if !defined(AFX_DCCFFRM_H__1AAD1CC2_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DCCFFRM_H__1AAD1CC2_22B6_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DCCFFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCCFFrm frame

class CDCCFFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDCCFFrm)
protected:
	CDCCFFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCFFrm)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCCFFrm();

	// Generated message map functions
	//{{AFX_MSG(CDCCFFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCCFFRM_H__1AAD1CC2_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
