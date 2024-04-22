// util.cpp
#include "stdafx.h"
#include "util.h"

/////
// Utility functions
//
static TCHAR szBold[]       = _T("Bold") ;
static TCHAR szItalic[]      = _T("Italic") ;
static TCHAR szBoldItalic[]  = _T("Bold Italic") ;
static TCHAR szRegular[]     = _T("Regular") ;

extern "C"
int EXPORT CALLBACK
    fnEnumReallyEx( LPENUMLOGFONT lpLogFont,
                LPNEWTEXTMETRIC lpTextMetrics,
                int nFontType,
                LPENUMLOGFONT lpELF ) ;

 /****************************************************************
 *  HFONT WINAPI
 *    ReallyCreateFontEx( HDC hDC, LPTSTR lpszFace,
 *                        LPTSTR lpszStyle, int nPointSize, UINT uiFlags )
 *
 *  Description: 
 *
 *    Creates a font based on facename, pointsize, and style.
 *
 *    Uses 3.1 API's to correctly select TT fonts.
 *
 *    HDC   hDC         the target DC (optional)
 *
 *    LPTSTR lpszFace    pointer the facename (required)
 *
 *    LPTSTR lpszStyle   pointer to the style (like "Demibold Italic")
 *                      (optional).
 *
 *    int nPointSize  size in points (required)
 *
 *    UINT  uiFlags      Flags, same as for ReallyCreateFont() except
 *                      RCF_BOLD and RCF_ITALIC are ignored if lpszStyle
 *                      is not NULL.
 *
 *  Comments:
 *
 ****************************************************************/


CFont* ReallyCreateFont( HDC hDC, LPTSTR lpszFace, LPTSTR lpszStyle, UINT nPointSize, UINT uiFlags )
{
   ENUMLOGFONT  elf ;
   FONTENUMPROC lpfn ;
   HDC          hdcCur ;
   
   if (!(lpfn = (FONTENUMPROC)MakeProcInstance( (FARPROC)fnEnumReallyEx, AfxGetInstanceHandle() )))
      return NULL ;

   // if the DC wasn't specified then use the display...
   //
   if (!hDC)
   {
      if (!(hdcCur = GetDC( NULL )))
      {
         FreeProcInstance( (FARPROC)lpfn ) ;
         return FALSE ;
      }
   }
   else
      hdcCur = hDC ;

   if (hdcCur)
   {
      /*
       * EnumFontFamilies takes the family name as the second param.
       * For example the family name might be "Lucida Sans" and the
       * style might be "Demibold Roman".
       *
       * The last parameter is app. defined.  In our case we pass a
       * structure that has a LOGFONT and a char array as it's members.
       * We put the style in the char array, and when this function
       * returns the LOGFONT has the logfont for the font we want to create.
       */
      if (lpszStyle)
         lstrcpy( (LPTSTR)elf.elfStyle, lpszStyle ) ;
      else
        switch( uiFlags & ~(RCF_NODEFAULT | RCF_STRIKEOUT | RCF_UNDERLINE) )
        {
            case RCF_BOLD:
                lstrcpy( (LPTSTR)elf.elfStyle, szBold ) ;
            break ;

            case RCF_ITALIC:
                lstrcpy( (LPTSTR)elf.elfStyle, szItalic ) ;
            break ;

            case RCF_BOLD | RCF_ITALIC:
                lstrcpy( (LPTSTR)elf.elfStyle, szBold ) ;
                lstrcat( (LPTSTR)elf.elfStyle, _T(" ") ) ;
                lstrcat( (LPTSTR)elf.elfStyle, szItalic ) ;
            break ;

            default:
                lstrcpy( (LPTSTR)elf.elfStyle, szRegular ) ;
                lpszStyle = (LPTSTR)elf.elfStyle ;
            break ;
        }

      if (lpszFace)
         EnumFontFamilies( hdcCur, lpszFace, lpfn, (LPARAM)(LPVOID)&elf ) ;

      if (!lpszFace || lstrcmpi( elf.elfLogFont.lfFaceName, lpszFace))
      {
         if ((uiFlags & RCF_NODEFAULT) == RCF_NODEFAULT)
         {
            FreeProcInstance( (FARPROC)lpfn ) ;

            if (hdcCur != hDC)
               ReleaseDC( NULL, hdcCur ) ;

            return NULL ;

         }
         else
            GetObject( GetStockObject( SYSTEM_FONT ),
                       sizeof( LOGFONT ), (LPTSTR)&elf.elfLogFont ) ;
      }

      //
      // See pages 4-48, 4-49, of Reference Vol. 1 for explaination
      // of why we set lfWidth to 0
      //
      elf.elfLogFont.lfWidth = 0 ;

      //
      // The equation for calculating point size based on font
      // height (a point is 1/72 of an inch) is:
      //
      //
      //    pt = (height * 72) / number of pixels in the Y direction
      //
      // Using GetTextMetrics() you should calculate height as:
      //
      //    height = tm.tmHeight - tm.tmInternalLeading
      //
      // This is because Windows' interpretation of a font height
      // is different from everyone else's.
      //
      //
      elf.elfLogFont.lfHeight = -MulDiv( nPointSize,
                             GetDeviceCaps( hdcCur, LOGPIXELSY),
                             72 )  ;

      elf.elfLogFont.lfStrikeOut = (BYTE)((uiFlags & RCF_STRIKEOUT) == RCF_STRIKEOUT) ;
      elf.elfLogFont.lfUnderline = (BYTE)((uiFlags & RCF_UNDERLINE) == RCF_UNDERLINE) ;

      if (!lpszStyle)
      {
         elf.elfLogFont.lfWeight    = (uiFlags & RCF_BOLD) ? FW_BOLD : FW_NORMAL ;
         elf.elfLogFont.lfItalic    = (BYTE)((uiFlags & RCF_ITALIC) == RCF_ITALIC) ;
      }

   }
   FreeProcInstance( (FARPROC)lpfn ) ;

   if (hdcCur != hDC)
      ReleaseDC( NULL, hdcCur ) ;

   CFont * pFont = new CFont ;
   pFont->CreateFontIndirect( &elf.elfLogFont ) ;
          
   return pFont ;

} /* ReallyCreateFontEx()  */

/****************************************************************
 *  int CALLBACK
 *    fnEnumReallyEx( LPLOGFONT lpLogFont,
 *                  LPTEXTMETRIC lpTextMetrics,
 *                  int nFontType, LPLOGFONT lpLF )
 *
 *  Description: 
 *
 *    Call back for EnumFonts and ReallySelectFontEx().
 *
 *    DO NOT FORGET TO EXPORT!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *  Comments:
 *
 ****************************************************************/
extern "C"
int EXPORT CALLBACK
fnEnumReallyEx( LPENUMLOGFONT lpELFin,
                LPNEWTEXTMETRIC lpTextMetrics,
                int nFontType,
                LPENUMLOGFONT lpELF )
{
   /* If we are not true type, then we will only be called once,
    * in in this case we need to compare the weight and italic
    * members of the TM struct to some strings.
    */
   if (!(nFontType & TRUETYPE_FONTTYPE))
   {
      lpELF->elfLogFont = lpELFin->elfLogFont ;

       // HACK TO FIX WindowsJ bug
       //
       LOGFONT lf ;
       GetObject( GetStockObject( SYSTEM_FONT ),
                           sizeof( LOGFONT ), (LPTSTR)&lf ) ;
       lpELF->elfLogFont.lfCharSet = lf.lfCharSet;

      /* Parse szStyle.  It may be something like "Bold Italic" or
       * "Demibold Italic".  Let's be somewhat smart about this.
       *
       * If "Demibold Italic" is passed in then he wants bold and
       * italic.
       *
       * We want to search szStyle for the word "bold".  Got a better
       * search strategy, go ahead and put it here (but tell me about it!).
       *
       * WARNING!!!
       *   The style string is language dependent.
       */
      if (lpELF->elfStyle)
      {
         /*
          * Assume "Regular"
          */
         lpELF->elfLogFont.lfWeight = FW_NORMAL ;
         lpELF->elfLogFont.lfItalic = FALSE ;

         if (!lstrcmpi( (LPTSTR)lpELF->elfStyle, szBold ))
         {
            lpELF->elfLogFont.lfWeight = FW_BOLD ;
            return 0 ;
         }

         if (!lstrcmpi( (LPTSTR)lpELF->elfStyle, szItalic))
         {
            lpELF->elfLogFont.lfItalic = TRUE ;
            return 0 ;
         }

         if (!lstrcmpi( (LPTSTR)lpELF->elfStyle, szBoldItalic ))
         {
            lpELF->elfLogFont.lfWeight = FW_BOLD ;
            lpELF->elfLogFont.lfItalic = TRUE ;
            return 0 ;
         }
      }

      return 0 ;  /* stop the enumeration */

   }

   /* We now know we have a TT font.  For each style in the
    * family passed in, we will get here.
    *
    * If we get an exact match copy it into lpELF and return.
    * Otherwise get as close as possible.
    */
   if (0==lstrcmpi( (LPTSTR)lpELF->elfStyle, (LPTSTR)lpELFin->elfStyle ))
   {
      *lpELF = *lpELFin;
      return 0;
   }

   lpELF->elfLogFont = lpELFin->elfLogFont ;

   return 1 ;

}/* fnEnumReallyEx() */


