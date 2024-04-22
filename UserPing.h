#if !defined(AFX_USERPING_H__52312081_85D3_11D1_BA39_00A0C95C1A08__INCLUDED_)
#define AFX_USERPING_H__52312081_85D3_11D1_BA39_00A0C95C1A08__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UserPing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserPing document
#define UP_NOACTION		0
#define UP_GETTINGHOST	1
#define UP_RESOLVING	2
#define UP_SENTPING		3
#define UP_RECEIVEDPONG	4

class CUserPing : public CObject
{
protected:
	       
// Attributes
public:
	CUserPing();   
	CString User;
	CString ResolvedIP;
	CString UnresolvedIP;
	DWORD	TotalPing;
	int		NumPings;
	int		State;

// Operations
public:


// Implementation
public:
	CUserPing (BOOL dummy);
	void AddPing(DWORD NewPing);
	int GetPingRating(void);
	virtual ~CUserPing();

protected:

};

#endif // !defined(AFX_USERPING_H__52312081_85D3_11D1_BA39_00A0C95C1A08__INCLUDED_)
