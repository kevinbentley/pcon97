//
// CUser.h
//
// Implements the CUser class tat we used for pings and stuff

#ifndef __CUSER_H__
#define __CUSER_H__

class CUser : public CObject
{
	DECLARE_DYNAMIC(CUser);

public:
	void IgnoreUser(BOOL Ignore);
	BOOL IsIgnored (void);
	CUser(CString& UserName);
	inline LPCSTR GetUserName(){return(LPCSTR(UserName));}
	inline ULONG GetUserIP(){return(ip);}
	inline ULONG starttime(){return(msecTime);}
	void SetUserIP(ULONG newip){ip = newip;}
	
	void startclock();
	int  stopclock();

protected:
	BOOL Ignored;
	CString UserName;
	DWORD msecTime;
	ULONG ip;
private:

};


#endif
