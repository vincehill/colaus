
#pragma once

#define DSTCOPY 0x00AA0029

extern BOOL WINAPI MyMaskBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
    HDC hdcSrc, int nXSrc, int nYSrc,
    HBITMAP hbmMask, int xMask, int yMask,
    DWORD dwRop );

extern void DisplayLastError(void);