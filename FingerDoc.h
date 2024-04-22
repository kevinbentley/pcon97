#if !defined(AFX_FINGERDOC_H__62EDB901_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
#define AFX_FINGERDOC_H__62EDB901_08E3_11D1_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FingerDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFingerDoc document

class CFingerDoc : public CDocument
{
protected:
	CFingerDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFingerDoc)

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFingerDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void ActivateSendVw(UINT nchar);
	void SetTitle(CString &WindowTitle);
	CString m_strFinger;
	CString m_strFingerTitle;
	
	virtual ~CFingerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFingerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERDOC_H__62EDB901_08E3_11D1_98E1_00A0249AB02A__INCLUDED_)
