// UserPing.cpp : implementation file
//

#include "stdafx.h"
#include "pconw.h"
#include "UserPing.h"


/////////////////////////////////////////////////////////////////////////////
// CUserPing


CUserPing::CUserPing()
{
	State = UP_NOACTION;
	NumPings = 0;
	TotalPing = 0;
}


CUserPing::~CUserPing()
{
}




/////////////////////////////////////////////////////////////////////////////
// CUserPing commands

//returns rating of 0-5 5 Being the lowest ping, and 0 being no ping response
int CUserPing::GetPingRating()
{
	DWORD l_ping;

	if(TotalPing==0) return 0;
	l_ping = TotalPing/NumPings;
	if(l_ping < 200) return 5;
	if(l_ping < 400) return 4;
	if(l_ping < 600) return 3;
	if(l_ping < 800) return 2;
//	if(l_ping < 1000) return 1;
	return 1;

}

void CUserPing::AddPing(DWORD NewPing)
{
	NumPings++;
	State = UP_RECEIVEDPONG;
	TotalPing += NewPing;
}

CUserPing::CUserPing(BOOL dummy)
{
	State = UP_NOACTION;
	NumPings = 0;
	TotalPing = 0;
}
