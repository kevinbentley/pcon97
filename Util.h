// util.h

#ifndef _UTIL_H_
#define _UTIL_H_

/// Utilities   
#define RCF_NORMAL      0x0000
#define RCF_ITALIC      0x0001
#define RCF_UNDERLINE   0x0002
#define RCF_STRIKEOUT   0x0004
#define RCF_BOLD        0x0008
#define RCF_NODEFAULT   0x0010
#define RCF_TRUETYPEONLY 0x0011

CFont *ReallyCreateFont( HDC hDC, LPTSTR lpszFace, LPTSTR lpszStyle, UINT nPointSize, UINT uiFlags ) ;


#endif 

