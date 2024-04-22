// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "Launcher.h"
#include "dwatchDoc.h"
#include "TabBar.h"	// Added by ClassView
#include "ChildFrm.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	void RefreshLaunchBar(void);
	CLauncher * m_GameLauncher;
	//CChannelList dlgChannelList;
	UINT m_Timer;
	CChildFrame * ChannelWindow;
	CMainFrame();


// Attributes
public:

// Operations
public:
	//CDUserList dlgUsers;
	void DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CTabBar m_TabBar;
	CDwatchDoc * m_DSpyDoc;
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateFileClearwindow(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateCfg(CCmdUI* pCmdUI);
	afx_msg void OnCfg();
	afx_msg void OnLaunch01();
	afx_msg void OnUpdateLaunch01(CCmdUI* pCmdUI);
	afx_msg void OnLaunch02();
	afx_msg void OnUpdateLaunch02(CCmdUI* pCmdUI);
	afx_msg void OnLaunch03();
	afx_msg void OnUpdateLaunch03(CCmdUI* pCmdUI);
	afx_msg void OnLaunch04();
	afx_msg void OnUpdateLaunch04(CCmdUI* pCmdUI);
	afx_msg void OnLaunch05();
	afx_msg void OnUpdateLaunch05(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLaunch06(CCmdUI* pCmdUI);
	afx_msg void OnLaunch06();
	afx_msg void OnLaunch07();
	afx_msg void OnUpdateLaunch07(CCmdUI* pCmdUI);
	afx_msg void OnLaunch08();
	afx_msg void OnUpdateLaunch08(CCmdUI* pCmdUI);
	afx_msg void OnLaunch09();
	afx_msg void OnUpdateLaunch09(CCmdUI* pCmdUI);
	afx_msg void OnLaunch10();
	afx_msg void OnUpdateLaunch10(CCmdUI* pCmdUI);
	afx_msg void OnLaunch11();
	afx_msg void OnUpdateLaunch11(CCmdUI* pCmdUI);
	afx_msg void OnLaunch12();
	afx_msg void OnUpdateLaunch12(CCmdUI* pCmdUI);
	afx_msg void OnLaunch13();
	afx_msg void OnUpdateLaunch13(CCmdUI* pCmdUI);
	afx_msg void OnLaunch14();
	afx_msg void OnUpdateLaunch14(CCmdUI* pCmdUI);
	afx_msg void OnLaunch15();
	afx_msg void OnUpdateLaunch15(CCmdUI* pCmdUI);
	afx_msg void OnLaunch16();
	afx_msg void OnUpdateLaunch16(CCmdUI* pCmdUI);
	afx_msg void OnLaunch17();
	afx_msg void OnUpdateLaunch17(CCmdUI* pCmdUI);
	afx_msg void OnLaunch18();
	afx_msg void OnUpdateLaunch18(CCmdUI* pCmdUI);
	afx_msg void OnLaunch19();
	afx_msg void OnUpdateLaunch19(CCmdUI* pCmdUI);
	afx_msg void OnLaunch20();
	afx_msg void OnUpdateLaunch20(CCmdUI* pCmdUI);
	afx_msg void OnLaunchexplorer();
	afx_msg void OnUpdateShowchan(CCmdUI* pCmdUI);
	afx_msg void OnShowchan();
	afx_msg void OnMenudspy();
	afx_msg void OnUpdateMenudspy(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
