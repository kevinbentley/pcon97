//
// CUser.cpp
//
// Implements the CUser class
//

#include "stdafx.h"
#include "CUser.h"

IMPLEMENT_DYNAMIC(CUser, CObject);

//
// Constructor
CUser::CUser(CString& UName)
{
	UserName = UName;
	Ignored = FALSE;
}

//
// startclock
void CUser::startclock()
{
	msecTime = GetTickCount();
}

//
// stopclock
//
int CUser::stopclock()
{
	DWORD nowtime = GetTickCount();
	return(nowtime - msecTime);
}

void CUser::IgnoreUser(BOOL Ignore)
{
	Ignored = Ignore;
}


BOOL CUser::IsIgnored (void)
{
	return Ignored;
}
