// HMXShapedFormView.cpp : implementation file
//

/********************************************************************

	created:	2001/10/25
	file:		HMXShapedFormView
	author:		Massimo Colurcio
	homepage:	http://www.softhor.com/developmentarea
	email:		m.colurcio@softhor.com
	thanks to:	
	
	purpose:	use this class to create bitmap shaped formview

*********************************************************************/


#include "stdafx.h"
#include "HMXShapedFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXShapedFormView

CHMXShapedFormView::CHMXShapedFormView(UINT nIDD, const CString& sFileRgn, COLORREF clr)
	: CFormView(nIDD)
{
	if( !sFileRgn.IsEmpty() ) {
		m_sFileRgn = sFileRgn;
		m_clrTransp = clr;
	}
	
	CommonConstructor();
}

CHMXShapedFormView::~CHMXShapedFormView()
{
}

void CHMXShapedFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHMXShapedFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHMXShapedFormView, CFormView)
	//{{AFX_MSG_MAP(CHMXShapedFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXShapedFormView diagnostics

#ifdef _DEBUG
void CHMXShapedFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHMXShapedFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CHMXShapedFormView::CommonConstructor()
{
	// do nothing... now
}

void CHMXShapedFormView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	BitBlt( pDC->m_hDC, 0, 0, m_bmInfo.bmWidth, m_bmInfo.bmHeight, m_hMemDC, 0, 0, SRCCOPY);
	
	return;
}

/********************************************************************

	created:	2001/10/25
	in:			lpCreateStructure
	out:		none
	return:		see CWnd::OnCreate
	
	purpose:	overrides this funcion to create the region at
				this time. 

	Special thanks to David Gallardo Llopis. He wrote
	the biggest part of this function

*********************************************************************/

int CHMXShapedFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if( m_sFileRgn.IsEmpty() )
		return 0;
	
	// The name of the bitmap to load (if not in the same directory as the application, provide full path!)
	// If the bitmap is to be load from the resources, the name stands for the ressource-string identification
	
	// If you want to load the bitmap from file 
	m_hBmp=(HBITMAP) LoadImage(lpCreateStruct->hInstance,m_sFileRgn,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if( !m_hBmp )
		return 0;
	
	// If you want to load the bitmap from the resources 
	// Uncomment the next line, and comment the previous "LoadImage()" line!
	// hImage=(HBITMAP) LoadImage(lpCreateStruct->hInstance,name,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	// Let's make this function do the hard work. You pass it the handle of the bitmap,
	// the color in the bitmap to be transparent, and a third boolean parameter that
	// tells the function whether the passed color is to be interpreted as the transparent
	// color or as the opaque color
	
	m_hRegion=BitmapRegion( m_hBmp, m_clrTransp, true);
	
	// If there was no problem getting the region, we make it the current clipping region
	// of the dialog's window
	if(m_hRegion) {
		
		//
		// THE MAGIC ONE
		//
		// I spent 2 days to find this solution. I Hope it's definitive :)
		//
		OffsetRgn(	m_hRegion, 
					GetSystemMetrics( SM_CXFRAME ), 
					GetSystemMetrics( SM_CXFRAME ) );
		
		GetParent()->SetWindowRgn( m_hRegion, TRUE);
	}
	
	// We ask the bitmap for its size...
	GetObject(m_hBmp,sizeof(m_bmInfo),&m_bmInfo);
	
	// At last, we create a display-compatible memory context!
	m_hMemDC=CreateCompatibleDC(NULL);
	
	m_hPrevBmp=(HBITMAP)SelectObject(m_hMemDC,m_hBmp);
	
	return 0;
}

/********************************************************************

	created:	2001/10/25
	in:			hBitmap, clrTransp, bTransp
	out:		none
	return:		region handle
	
	purpose:	creates regin from bitmap

*********************************************************************/

HRGN CHMXShapedFormView::BitmapRegion(HBITMAP hBitmap,COLORREF clrTransp, bool bTransp)
{
	// We create an empty region
	HRGN hRegion = NULL;
	
	// If the passed bitmap is NULL, go away!
	if(!hBitmap) 
		return hRegion;
	
	// We create a memory context for working with the bitmap
	// The memory context is compatible with the display context (screen)
	HDC hMemDC=CreateCompatibleDC(NULL);
	
	// If no context is created, go away, too!
	if(!hMemDC) 
		return hRegion;
		
	// Computation of the bitmap size
	BITMAP bmBitmap;
	
	GetObject(hBitmap, sizeof(bmBitmap), &bmBitmap);
	
	// In order to make the space for the region, we
	// create a bitmap with 32bit depth color and with the
	// size of the loaded bitmap!
	BITMAPINFOHEADER RGB32BITSBITMAPINFO=
	{ 
		sizeof(BITMAPINFOHEADER), 
		bmBitmap.bmWidth, 
		bmBitmap.bmHeight, 
		1,32,BI_RGB,0,0,0,0,0 
	};
	
	// Here is the pointer to the bitmap data
	VOID *pBits;
	
	// With the previous information, we create the new bitmap!
	HBITMAP hNewBitmap;
	hNewBitmap=CreateDIBSection(hMemDC,
								(BITMAPINFO *)&RGB32BITSBITMAPINFO,
								DIB_RGB_COLORS,&pBits,NULL,0);

	// If the creation process succeded...
	if(hNewBitmap) {
		// We select the bitmap onto the created memory context
		// and then we store the previosly selected bitmap on this context!
		HBITMAP hPrevBmp=(HBITMAP) SelectObject(hMemDC,hNewBitmap);
		
		// We create another device context compatible with the first!
		HDC hDC=CreateCompatibleDC(hMemDC);
		
		// If success...
		if(hDC) {		
			// We compute the number of bytes per row that the bitmap contains, rounding to 32 bit-multiples
			BITMAP bmNewBitmap;
			
			GetObject(hNewBitmap,sizeof(bmNewBitmap),&bmNewBitmap);
				
			while(bmNewBitmap.bmWidthBytes % 4) 
				bmNewBitmap.bmWidthBytes++;
			
			// Copy of the original bitmap on the memory context!
			HBITMAP hPrevBmpOrg=(HBITMAP) SelectObject(hDC,hBitmap);
			BitBlt(hMemDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,hDC,0,0,SRCCOPY);

			// In order to optimize the code, we don't call the GDI each time we
			// find a transparent pixel. We use a RGN_DATA structure were we store
			// consecutive rectangles, until we have a large amount of them and then we crete
			// the composed region with ExtCreateRgn(), combining it with the main region.
			// Then we begin again initializing the RGN_DATA structure and doing another
			// iteration, until the entire bitmap is analyzed.

			// Also, in order to not saturate the Windows API with calls for reserving
			// memory, we wait until NUMRECT rectangles are stores in order to claim
			// for another NUMRECT memory space!
#define NUMRECT	100			
			DWORD nMaxRect = NUMRECT;
			
			// We create the memory data
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*nMaxRect));
			RGNDATA *pData=(RGNDATA*) GlobalLock(hData);
			pData->rdh.dwSize=sizeof(RGNDATAHEADER);
			pData->rdh.iType=RDH_RECTANGLES;
			pData->rdh.nCount=pData->rdh.nRgnSize=0;
			SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
			
			// We study each pixel on the bitmap...
			BYTE *pbPixels=(BYTE*) bmNewBitmap.bmBits+(bmNewBitmap.bmHeight-1)*bmNewBitmap.bmWidthBytes;
			
			// Main loop
			for(int nRow=0;nRow<bmBitmap.bmHeight;nRow++) 
			{
				// Horizontal loop
				for(int nCol=0;nCol<bmBitmap.bmWidth;nCol++)
				{		
					// We optimized searching for adyacent transparent pixels!
					int nXo=nCol;
					LONG *plPixel=(LONG*) pbPixels+nCol;

					while(nCol<bmBitmap.bmWidth) 
					{
						bool bInRange=false;

						// Massimo wrote:
						//
						//		Could someone explain me why I have to 
						//		compare colors in that way?????
						//
						//		Blue/Red inversion??????
						//

						// If the color is that indicated as transparent...
						//
/*

						This is the original code...

						if(	GetRValue(*plPixel)==GetRValue(clrTransp) &&
							GetGValue(*plPixel)==GetGValue(clrTransp) &&
							GetBValue(*plPixel)==GetBValue(clrTransp) )
							bInRange=true;

*/
						if(	GetBValue(*plPixel)==GetRValue(clrTransp) &&
							GetGValue(*plPixel)==GetGValue(clrTransp) &&
							GetRValue(*plPixel)==GetBValue(clrTransp) )
							bInRange=true;

						if((bTransp) && (bInRange)) 
							break;

						if((!bTransp) && (!bInRange)) 
							break;

						plPixel++;
						nCol++;
					} // while (nCol < bm.bmWidth) 		
					
					if(nCol>nXo) 
					{
						// We add the rectangle (Xo,Row),(Column,Row+1) to the region

						// If the number of rectangles is greater then NUMRECT, we claim
						// another pack of NUMRECT memory places!
						if (pData->rdh.nCount>=nMaxRect)
						{
							GlobalUnlock(hData);
							nMaxRect+=NUMRECT;
							hData=GlobalReAlloc(hData,sizeof(RGNDATAHEADER)+(sizeof(RECT)*nMaxRect),GMEM_MOVEABLE);
							pData=(RGNDATA *)GlobalLock(hData);					
						}		
						
						RECT *pRect=(RECT*) &pData->Buffer;
						SetRect(&pRect[pData->rdh.nCount], nXo, nRow, nCol, nRow+1);		
								
						if( nXo<pData->rdh.rcBound.left) 
							pData->rdh.rcBound.left=nXo;

						if( nRow<pData->rdh.rcBound.top) 
							pData->rdh.rcBound.top=nRow;

						if(nCol>pData->rdh.rcBound.right) 
							pData->rdh.rcBound.right = nCol;
						
						if(nRow+1>pData->rdh.rcBound.bottom) 
							pData->rdh.rcBound.bottom=nRow+1;
		
						pData->rdh.nCount++;																	

						// In Win95/08 there is a limitation on the maximum number of
						// rectangles a RGN_DATA can store (aprox. 4500), so we call
						// the API for a creation and combination with the main region
						// each 2000 rectangles. This is a good optimization, because
						// instead of calling the routines for combining for each new
						// rectangle found, we call them every 2000 rectangles!!!
						if(pData->rdh.nCount==2000)
						{						
							HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER) + (sizeof(RECT) * nMaxRect),pData);
							if (hNewRegion) {
								// Si ya existe la región principal,sumamos la nueva,
								// si no,entonces de momento la principal coincide con
								// la nueva región.
								if (hRegion) {
									CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);									
									DeleteObject(hNewRegion);
								} else
									hRegion=hNewRegion;
							}
							// Volvemos a comenzar la suma de rectángulos
							pData->rdh.nCount=0;
							SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
						}			
					
					} // if ( nCol > nXo)
				} // for (int  nCol ...)

				// Nueva Row. Lo del negativo se debe a que el bitmap está invertido
				// verticalmente.
				pbPixels -= bmNewBitmap.bmWidthBytes;
			
			} // for (int Row...)			

			// Una vez finalizado el proceso,procedemos a la fusión de la
			// región remanente desde la última fusión hasta el final			
			HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*nMaxRect),pData);

			if(hNewRegion) 
			{
				// If the main region does already exist, we add the new one,
				if(hRegion)
				{														
					CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);
					DeleteObject(hNewRegion);
				}
				else
					// if not, we consider the new one to be the main region at first!
					hRegion=hNewRegion;				
			}						
			
			// We free the allocated memory and the rest of used ressources
			GlobalFree(hData);
			SelectObject(hDC,hPrevBmpOrg);
			DeleteDC(hDC);
			
		}// if (hDC) 

		SelectObject(hMemDC,hPrevBmp);
		DeleteDC(hMemDC);
	} //if (hNewBitmap)	

	return hRegion;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		none
  
	purpose:	do a lot of useful stuffs
	
*********************************************************************/

void CHMXShapedFormView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	// Simply select the previous bitmap on the memory context...
	SelectObject(m_hMemDC, m_hPrevBmp);	
	
	// ... and delete the bitmap the we loaded at the beginning
	if(m_hBmp) 
		DeleteObject(m_hBmp);
}

/********************************************************************

	created:	2001/10/25
	in:			see CWnd::OnMouseMove
	out:		see CWnd::OnMouseMove
	return:		none
  
	purpose:	use this method to allow window movements dragging
				the window clicking the entire client area
	
*********************************************************************/

void CHMXShapedFormView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// special thanks to Jens Rams <jens_rahm@hotmail.com>
	if(nFlags == MK_LBUTTON) {
		ReleaseCapture();
		GetParentFrame()->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CFormView::OnMouseMove(nFlags, point);
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		true if successful
  
	purpose:	use this method to close window... 
				dont forget you havent the close button :)
	
*********************************************************************/

bool CHMXShapedFormView::Dismiss()
{
	return GetParentFrame()->DestroyWindow()?true:false;
}
