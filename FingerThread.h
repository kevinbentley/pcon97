#if !defined(AFX_FINGERTHREAD_H__B6E68AA1_1711_11D1_98E3_00A0249AB02A__INCLUDED_)
#define AFX_FINGERTHREAD_H__B6E68AA1_1711_11D1_98E3_00A0249AB02A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FingerThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CFingerThread thread

class CFingerThread : public CWinThread
{
	DECLARE_DYNCREATE(CFingerThread)
protected:
	CFingerThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CFingerThread(HWND hwndParent);
// Operations
public:
	CString m_strFormatTitle;
	void InitWindow(CString &Title);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFingerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND m_hwndParent;
//	CFingerThread m_wndFinger;
	virtual ~CFingerThread();

	// Generated message map functions
	//{{AFX_MSG(CFingerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINGERTHREAD_H__B6E68AA1_1711_11D1_98E3_00A0249AB02A__INCLUDED_)
