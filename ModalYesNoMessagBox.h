#if !defined(AFX_MODALYESNOMESSAGBOX_H__E75C1BE1_23D3_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_MODALYESNOMESSAGBOX_H__E75C1BE1_23D3_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ModalYesNoMessagBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModalYesNoMessagBox dialog

class CModalYesNoMessagBox : public CDialog
{
// Construction
public:
	CString m_MessageText;
	CString m_MessageTitle;
	CModalYesNoMessagBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModalYesNoMessagBox)
	enum { IDD = IDD_YESNODIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModalYesNoMessagBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModalYesNoMessagBox)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODALYESNOMESSAGBOX_H__E75C1BE1_23D3_11D1_98E3_00A0249AB02A__INCLUDED_)
