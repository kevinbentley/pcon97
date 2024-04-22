#ifndef _ConfigLauncher_Header_
#define _ConfigLauncher_Header_
// ConfigLauncher.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigLauncher dialog

class CConfigLauncher : public CDialog
{
// Construction
public:
	virtual ~CConfigLauncher();
	typedef struct ScanGame
    {
        CString Name;
        CString FindFile;
        CString UseFile;
        CString Parms;
        CString Icon;
        ScanGame *Next;
    };
    ScanGame *FirstGame,*CurrGame;
    BOOL m_bFirstRun;
	void ResetSelItem (void);
	int FnMatch(char *pat, char *name);
	void FindDrives(char *fdrives, char *hdrives);
	int DoADir(char *patternp, char *patternn, char *include);
	long FindFile(char *drives, char *pattern);
	CString m_SaveTitle;
	CString GetParms(CString &FileName);
	CString m_strAutoSel;
	void AddDraggedFile(CString &FileName);
	int SelGame;
	int m_iCurrGame;
	void WriteRegistry(void);
	BOOL ReadRegistry(void);
	CConfigLauncher(CWnd* pParent = NULL,LaunchItems *LaunchPtr=NULL);   // standard constructor
	LaunchItems *m_Launchees;
	CRegistry registry;
	CString m_IconPath;
	//class CAbortScan *l_Abort; 
// Dialog Data
	//{{AFX_DATA(CConfigLauncher)
	enum { IDD = IDD_LAUNCHCONFIG };
	CButton	m_CloseButton;
	CButton	m_ConfirmButton;
	CStatic	m_GameIcon;
	CListBox	m_Games;
	CString	m_FileName;
	CString	m_FilePath;
	CString	m_ProgName;
	CString	m_FileParms;
	BOOL	m_bCloseChat;
	BOOL	m_bConfirmLaunch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigLauncher)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigLauncher)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemadd();
	afx_msg void OnItemdel();
	afx_msg void OnChooseicon();
	afx_msg void OnSelchangeList1();
	virtual BOOL OnInitDialog();
	afx_msg void OnFilefind();
	afx_msg void OnKillfocusFilename1();
	afx_msg void OnKillfocusProgdir();
	afx_msg void OnKillfocusProgname();
	afx_msg void OnSetfocusProgname();
	afx_msg void OnFindgames();
	afx_msg void OnConfirmlnch();
	afx_msg void OnClosechat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AutoAddGame (CString &FileName,CString &Title,CString &Icon,CString &Parms);
};

#endif
