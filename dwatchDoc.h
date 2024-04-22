// dwatchDoc.h : interface of the CDwatchDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DWATCHDOC_H__7741E90C_CBE2_11D0_98E1_00A0249AB02A__INCLUDED_)
#define AFX_DWATCHDOC_H__7741E90C_CBE2_11D0_98E1_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CDwatchDoc : public CDocument
{
protected: // create from serialization only
	CDwatchDoc();
	DECLARE_DYNCREATE(CDwatchDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDwatchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDwatchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDwatchDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DWATCHDOC_H__7741E90C_CBE2_11D0_98E1_00A0249AB02A__INCLUDED_)
