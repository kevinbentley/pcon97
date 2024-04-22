#if !defined(AFX_DCCFDOC_H__1AAD1CC1_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_DCCFDOC_H__1AAD1CC1_22B6_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DCCFDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCCFDoc document

class CDCCFDoc : public CDocument
{
protected:
	CDCCFDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCCFDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCCFDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDCCFDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDCCFDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCCFDOC_H__1AAD1CC1_22B6_11D1_98E3_00A0249AB02A__INCLUDED_)
