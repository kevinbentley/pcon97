// dibpal.h : header file
//
// CDIBPal class
//

#ifndef __DIBPAL__
#define __DIBPAL__

#include "dib.h"

class CDIBPal : public CPalette
{
public:
    CDIBPal();
    ~CDIBPal();
    BOOL Create(CDIB *pDIB);            // Create from a DIB
    int GetNumColors();                 // Get the number of colors
                                        // in the palette.
    void Draw(CDC* pDC, CRect* pRect, BOOL bBkgnd = FALSE); 
    BOOL SetSysPalColors();
    BOOL Load(char* pszFileName = NULL);
    BOOL Load(CFile* fp);  
    BOOL Load(UINT hFile);
    BOOL Load(HMMIO hmmio);
    BOOL Save(CFile* fp);  
    BOOL Save(UINT hFile);
    BOOL Save(HMMIO hmmio);
};

#endif // __DIBPAL__

