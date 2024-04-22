#ifndef _CNewMessage_Header_
#define _CNewMessage_Header_
//CNewMessage


class CNewMessage : public CObject

{

public:
	CNewMessage() { UnRead = FALSE;};
	CNewMessage(BOOL State) { UnRead = State;};
	BOOL UnRead;

};
#endif