// GetWordNum(int num,CString &Word)
// Extract a single word from a string, separated by whitespace
// num is the zero based word to retrieve.
// l_String is the total String to extract from
// Return value is the word extracted, or "" if out of bounds
// If the extracted word begins with a ':' return the rest of the String, 
// starting with the char after the ':'

CString & GetWordNum(int num,CString &l_String)
{
	char seps[20] = " \n\r\t";
	CString l_tmpstrA,l_tmpstrB;
	l_tmpstrA=l_String;
	l_tmpstrB=strtok(LPCSTR(l_tmpstrA),seps);

	for(int i=0;i!=num;i++)
	{
		l_tmpstrB=strtok(NULL,seps);
	}
	//l_tmpstrB should now have what we want.
	
	//check for the ':' char.....l_tmpstrA ?
	if(l_tmpstrB[0]==':')
	{
		l_tmpstrB += " ";
		l_tmpstrB += l_tmpstrA;
	}
	
	//return the appropriate response.
	
	return l_tmpstrB;
}

