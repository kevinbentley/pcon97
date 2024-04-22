#if !defined(AFX_DSPYFRM_H__EE334C61_24A8_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DSPYFRM_H__EE334C61_24A8_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DspyFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDspyFrm frame

class CDspyFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDspyFrm)
protected:
	CDspyFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspyFrm)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDspyFrm();

	// Generated message map functions
	//{{AFX_MSG(CDspyFrm)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPYFRM_H__EE334C61_24A8_11D1_98E3_00A0249AB02A__INCLUDED_)
