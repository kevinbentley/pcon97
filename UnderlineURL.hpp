void D_VIEW_CLASS::UnderlineURLs(int iStart,LPCSTR szText)
{
	int l_ofs=0;
	int l_URLBegin;
	char * l_result;
	CString l_strNewText,l_tmpstr;
	l_strNewText = szText;
	l_strNewText.MakeLower();
	int isbMin,isbMax;
	l_URLBegin=l_strNewText.Find("http:");
	while(l_URLBegin!=-1)
	{
		for(int i=l_URLBegin;i<l_strNewText.GetLength();i++)
		{
			if((l_strNewText[i]==' ') || (l_strNewText[i]==')'))
			{
			
				goto skiphttp;
			}
		}
		
	
skiphttp:
		
		//Set the text to underlined here....
		
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		else
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		
		l_ofs+=i;
		CHARFORMAT vw_cf;
				
		vw_cf.dwMask=CFM_UNDERLINE|CFM_COLOR;
		vw_cf.crTextColor=((CPconwApp*)AfxGetApp())->colorURL;
		vw_cf.dwEffects=CFE_UNDERLINE;
		GetRichEditCtrl().SetSelectionCharFormat(vw_cf);
		l_tmpstr=l_strNewText.Right(l_strNewText.GetLength()-(i));
		l_strNewText=l_tmpstr;
		l_URLBegin=l_strNewText.Find("http:");
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int l;
			l=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(l,l);
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);

		
	}
    l_ofs=0;
    l_strNewText = szText;
	l_strNewText.MakeLower();
	
	l_URLBegin=l_strNewText.Find("ftp:");
	while(l_URLBegin!=-1)
	{
		for(int i=l_URLBegin;i<l_strNewText.GetLength();i++)
		{
			if((l_strNewText[i]==' ') || (l_strNewText[i]==')'))
			{
			
				goto skipftp;
			}
		}
		
	
skipftp:
		
		//Set the text to underlined here....
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		else
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		
		l_ofs+=i;
		CHARFORMAT vw_cf;
				
		vw_cf.dwMask=CFM_UNDERLINE|CFM_COLOR;
		vw_cf.crTextColor=((CPconwApp*)AfxGetApp())->colorURL;
		vw_cf.dwEffects=CFE_UNDERLINE;
		GetRichEditCtrl().SetSelectionCharFormat(vw_cf);
		l_tmpstr=l_strNewText.Right(l_strNewText.GetLength()-(i));
		l_strNewText=l_tmpstr;
		l_URLBegin=l_strNewText.Find("ftp:");
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int l;
			l=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(l,l);
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);

		
	}
	l_ofs=0;
	l_strNewText = szText;
	l_strNewText.MakeLower();
	
	l_URLBegin=l_strNewText.Find("finger:");
	while(l_URLBegin!=-1)
	{
		for(int i=l_URLBegin;i<l_strNewText.GetLength();i++)
		{
			if((l_strNewText[i]==' ') || (l_strNewText[i]==')'))
			{
			
				goto skipfinger;
			}
		}
		
	
skipfinger:
		
		//Set the text to underlined here....
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		else
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		
		l_ofs+=i;
		CHARFORMAT vw_cf;
				
		vw_cf.dwMask=CFM_UNDERLINE|CFM_COLOR;
		vw_cf.crTextColor=((CPconwApp*)AfxGetApp())->colorURL;
		vw_cf.dwEffects=CFE_UNDERLINE;
		GetRichEditCtrl().SetSelectionCharFormat(vw_cf);
		l_tmpstr=l_strNewText.Right(l_strNewText.GetLength()-(i));
		l_strNewText=l_tmpstr;
		l_URLBegin=l_strNewText.Find("finger:");
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int l;
			l=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(l,l);
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);

		
	}
    l_ofs=0;
    l_strNewText = szText;
	l_strNewText.MakeLower();
	l_URLBegin=l_strNewText.Find("getfile:");
	while(l_URLBegin!=-1)
	{
		for(int i=l_URLBegin;i<l_strNewText.GetLength();i++)
		{
			if((l_strNewText[i]==' ') || (l_strNewText[i]==')'))
			{
			
				goto skipGetFile;
			}
		}
		
	
skipGetFile:
		
		//Set the text to underlined here....
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		else
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		
		l_ofs+=i;
		CHARFORMAT vw_cf;
				
		vw_cf.dwMask=CFM_UNDERLINE|CFM_COLOR;
		vw_cf.crTextColor=((CPconwApp*)AfxGetApp())->colorURL;
		vw_cf.dwEffects=CFE_UNDERLINE;
		GetRichEditCtrl().SetSelectionCharFormat(vw_cf);
		l_tmpstr=l_strNewText.Right(l_strNewText.GetLength()-(i));
		l_strNewText=l_tmpstr;
		l_URLBegin=l_strNewText.Find("getfile:");
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int l;
			l=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(l,l);
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);

		
	}
    l_ofs=0;
    l_strNewText = szText;
	l_strNewText.MakeLower();
	
	l_URLBegin=l_strNewText.Find("mailto:");
	while(l_URLBegin!=-1)
	{
		for(int i=l_URLBegin;i<l_strNewText.GetLength();i++)
		{
			if((l_strNewText[i]==' ') || (l_strNewText[i]==')'))
			{
			
				goto skipmailto;
			}
		}
		
	
skipmailto:
		
		//Set the text to underlined here....
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		else
		{
			GetRichEditCtrl().HideSelection(TRUE,FALSE);
			if(!l_ofs) GetRichEditCtrl().SetSel(iStart+l_URLBegin,iStart+i);
			else GetRichEditCtrl().SetSel(l_ofs+iStart+l_URLBegin,iStart+l_ofs+i);
		}
		
		l_ofs+=i;
		CHARFORMAT vw_cf;
				
		vw_cf.dwMask=CFM_UNDERLINE|CFM_COLOR;
		vw_cf.crTextColor=((CPconwApp*)AfxGetApp())->colorURL;
        vw_cf.dwEffects=CFE_UNDERLINE;
		GetRichEditCtrl().SetSelectionCharFormat(vw_cf);
		l_tmpstr=l_strNewText.Right(l_strNewText.GetLength()-(i));
		l_strNewText=l_tmpstr;
		l_URLBegin=l_strNewText.Find("mailto:");
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int l;
			l=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(l,l);
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);

		
	}



}

void D_VIEW_CLASS::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetRichEditCtrl().GetSel(l_iStart,l_iEnd);
	POINT l_point;
    l_point.x=point.x;
    l_point.y=point.y;
    CHARFORMAT l_cf;
	int isbMin,isbMax;
    LONG charPos = ::SendMessage(GetRichEditCtrl().m_hWnd,EM_CHARFROMPOS,0,(LPARAM)&l_point);
    GetRichEditCtrl().HideSelection(TRUE,FALSE);
	GetRichEditCtrl().GetSel(l_iStart,l_iEnd);
	GetRichEditCtrl().SetSel(charPos,charPos);
    GetRichEditCtrl().GetSelectionCharFormat(l_cf);
    if(l_cf.dwEffects&CFE_UNDERLINE)
    {
		int l_iUrlStart,l_iURLEnd;
		l_iUrlStart=charPos;
		l_iURLEnd=charPos;
		
		while((l_cf.dwEffects&CFE_UNDERLINE)&&(l_iUrlStart>=0))
		{
			l_iUrlStart--;
			GetRichEditCtrl().SetSel(l_iUrlStart,l_iURLEnd);
			GetRichEditCtrl().GetSelectionCharFormat(l_cf);
		}
		l_iUrlStart++;	
		GetRichEditCtrl().SetSel(l_iUrlStart,l_iURLEnd);
		while((CFM_UNDERLINE&GetRichEditCtrl().GetSelectionCharFormat(l_cf))&&(l_iURLEnd!=GetRichEditCtrl().GetTextLength()))
		{
			l_iURLEnd++;
			GetRichEditCtrl().SetSel(l_iUrlStart,l_iURLEnd);
		}
		CString l_strLaunchURL;
		l_iURLEnd--;
		GetRichEditCtrl().SetSel(l_iUrlStart,l_iURLEnd);
		
		l_strLaunchURL=GetRichEditCtrl().GetSelText();
		GetScrollRange(SB_VERT,&isbMin,&isbMax);
		if((((CPconwApp*)AfxGetApp())->m_Frozen)||(GetScrollPos(SB_VERT)!=isbMax))
		{
			GetRichEditCtrl().SetSel(l_iStart,l_iEnd);
		}
		else 
		{
			int len=GetRichEditCtrl().GetTextLength();
			GetRichEditCtrl().SetSel(len,len);
		}
		
		if(strncmp(l_strLaunchURL,"finger:",7)==0)
		{
			CString l_fingercmd;
			l_fingercmd.Format("/finger %s",l_strLaunchURL.Right(l_strLaunchURL.GetLength()-7));
			((CPconwApp*)AfxGetApp())->SendMsg(l_fingercmd);
			CRichEditView::OnLButtonDown(nFlags, point);
			return;
		}
		if(strncmp(l_strLaunchURL,"GetFile:",7)==0)
		{
			CString l_getcmd;
			CString l_User,l_File;
			int l_iAtSign;
			l_iAtSign=l_strLaunchURL.Find("@");
			if(l_iAtSign==-1) 
			{
				CRichEditView::OnLButtonDown(nFlags, point);
				return;
			}
			l_User=l_strLaunchURL.Right(l_strLaunchURL.GetLength()-(l_iAtSign+1));
			l_File=l_strLaunchURL.Mid(8,l_iAtSign-8);
			l_getcmd.Format("/ctcp %s FILEREQ %s",l_User,l_File);
			((CPconwApp*)AfxGetApp())->SendMsg(l_getcmd);
			CRichEditView::OnLButtonDown(nFlags, point);
			return;
		}
		GetRichEditCtrl().HideSelection(FALSE,FALSE);
		ShellExecute(NULL,"open",LPCSTR(l_strLaunchURL),NULL,".",SW_SHOW);

	}
	
	CRichEditView::OnLButtonDown(nFlags, point);
}


void D_VIEW_CLASS::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	POINT l_point;
    l_point.x=point.x;
    l_point.y=point.y;
    CHARFORMAT l_cf;
    LONG charPos = ::SendMessage(GetRichEditCtrl().m_hWnd,EM_CHARFROMPOS,0,(LPARAM)&l_point);
    GetRichEditCtrl().HideSelection(TRUE,FALSE);
	GetRichEditCtrl().GetSel(l_iStart,l_iEnd);
	GetRichEditCtrl().SetSel(charPos,charPos);
    GetRichEditCtrl().GetSelectionCharFormat(l_cf);
	
	if((l_cf.dwEffects&CFE_UNDERLINE))
    {
        //SetCursor(l_hc);     
		m_URLCursor=TRUE;    
    }
	else
	{
		//MessageBeep(MB_OK);
		m_URLCursor=FALSE;
	}

    GetRichEditCtrl().SetSel(l_iStart,l_iEnd);

	GetRichEditCtrl().HideSelection(FALSE,FALSE);
    
	CRichEditView::OnMouseMove(nFlags, point);

}

BOOL D_VIEW_CLASS::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	    	
	
	if(!m_URLCursor)
	{
		return CRichEditView::OnSetCursor(pWnd, nHitTest, message);
	}
	else 
	{
		SetCursor(l_hc);	
		//MessageBeep(-1);
		return TRUE;
	}

}
