// HMXDialog.cpp : implementation file
//

/********************************************************************

	created:	2001/10/25
	file:		HMXDialog
	author:		Massimo Colurcio
	homepage:	http://www.softhor.com/developmentarea
	email:		m.colurcio@softhor.com
	thanks to:	an unknown programmer for bitmap drawing
	
	purpose:	want a dialog with background bitmap or color?
	  
*********************************************************************/


#include "stdafx.h"
#include "HMXDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXDialog dialog


CHMXDialog::CHMXDialog()
{
	CommonConstruct();
}


CHMXDialog::CHMXDialog(UINT uResource, CWnd* pParent /*=NULL*/)
	: CDialog(uResource, pParent)
{
	CommonConstruct();
}


CHMXDialog::CHMXDialog(LPCTSTR pszResource, CWnd* pParent /*=NULL*/)
	: CDialog(pszResource, pParent)
{
	CommonConstruct();
}


void CHMXDialog::CommonConstruct()
{
	m_nType = BITMAP_TILE;
	m_nBkGnd = BKGND_NONE;
}

void CHMXDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHMXDialog)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/********************************************************************

	created:	2001/10/25
	in:			uResource, nType
	out:		none
	return:		true if success 
  
	purpose:	set bitmap from resource
	
*********************************************************************/

bool CHMXDialog::SetBitmap(UINT uResource, int nType /*BITMAP_TILE*/)
{
	bool bRet;
	m_nType = nType;
	m_nBkGnd = BKGND_IMAGE;
	bRet = m_bmpBkGnd.LoadResource(uResource)?true:false;
	Invalidate();
	return bRet;
}

/********************************************************************

	created:	2001/10/25
	in:			sFileName, nType
	out:		none
	return:		true if success 
  
	purpose:	set bitmap from BMP file
	
*********************************************************************/

bool CHMXDialog::SetBitmap(const CString& sFileName, int nType /*BITMAP_TILE*/)
{
	bool bRet;
	m_nType = nType;
	m_nBkGnd = BKGND_IMAGE;
	bRet = m_bmpBkGnd.Load(sFileName)?true:false;
	Invalidate();
	return bRet;
}

/********************************************************************

	created:	2001/10/25
	in:			clr
	out:		none
	return:		always true
  
	purpose:	set background color
	
*********************************************************************/

bool CHMXDialog::SetBkClr(COLORREF clr)
{
	m_clrBkGnd = clr;
	m_brsBkGnd.DeleteObject();
	m_brsBkGnd.CreateSolidBrush(m_clrBkGnd);
	m_nBkGnd = BKGND_COLOR;
	Invalidate();

	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		background color
	return:		none
  
	purpose:	get background color
	
*********************************************************************/

bool CHMXDialog::GetBkClr(COLORREF& clr)
{
	clr = m_clrBkGnd;

	return true;
}

BEGIN_MESSAGE_MAP(CHMXDialog, CDialog)
	//{{AFX_MSG_MAP(CHMXDialog)
	ON_WM_ERASEBKGND()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXDialog message handlers

/********************************************************************

	created:	2001/10/25
	in:			pDC, see CWnd::OnEraseBkgnd
	out:		none
	return:		none
  
	purpose:	erase background painting with 
				color or background bitmap
	
*********************************************************************/

BOOL CHMXDialog::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(rc);
	int x = 0, y = 0;
	BOOL bRet;

	switch( m_nBkGnd ) {
	case BKGND_NONE:
		//
		// no background
		//
		bRet = CDialog::OnEraseBkgnd(pDC);
		break;
	case BKGND_COLOR:
		//
		// color background
		//
		pDC->FillRect( &rc, &m_brsBkGnd);
		bRet = TRUE;
		break;
	case BKGND_IMAGE:
		//
		// image background
		//
		if(m_bmpBkGnd.GetPixelPtr() != 0) {
			
			switch(m_nType) {
			case BITMAP_CENTER:
				//
				// center the bitmap
				//
				CDialog::OnEraseBkgnd(pDC);
				x = (rc.Width() - m_bmpBkGnd.GetWidth()) / 2;
				y = (rc.Height() - m_bmpBkGnd.GetHeight()) / 2;
				m_bmpBkGnd.DrawDIB(pDC, x, y);
				break;
				
			case BITMAP_STRETCH:
				//
				// stretch bitmap so it will best fit to the dialog
				//
				m_bmpBkGnd.DrawDIB(pDC, 0, 0, rc.Width(), rc.Height());
				break;
				
			case BITMAP_TILE:
				//
				// tile the bitmap
				//
				while(y < rc.Height()) {
					while(x < rc.Width()) {
						m_bmpBkGnd.DrawDIB(pDC, x, y);
						x += m_bmpBkGnd.GetWidth();
					}
					x = 0;
					y += m_bmpBkGnd.GetHeight();
				}
				bRet = TRUE;
				break;
			default:
				bRet = CDialog::OnEraseBkgnd(pDC);
				break;
			}
		} else
			// no bitmap set, behave like a normal dialog
			bRet = CDialog::OnEraseBkgnd(pDC);
		break;
	default:
		bRet = CDialog::OnEraseBkgnd(pDC);
		break;
	}

	return bRet;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		TRUE if success
  
	purpose:	handling palette changing
	
*********************************************************************/

BOOL CHMXDialog::OnQueryNewPalette() 
{
	CPalette * pPal = m_bmpBkGnd.GetPalette();
	if( pPal != 0 && GetSafeHwnd() != 0 ) {
		CClientDC dc(this);
		CPalette * pOldPalette = dc.SelectPalette(pPal, FALSE);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);
		
		if (nChanged == 0)
			return FALSE;
		
		Invalidate();
		return TRUE;
	}
	return CDialog::OnQueryNewPalette();
}

/********************************************************************

  created:	2001/10/25
  in:			see CWnd::OnPaletteChanged
  out:		see CWnd::OnPaletteChanged
  return:		none
  
	purpose:	handling palette changing
	
*********************************************************************/

void CHMXDialog::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CPalette * pPal = m_bmpBkGnd.GetPalette();
	if( pPal != 0 && GetSafeHwnd() != 0 && pFocusWnd != this && ! IsChild(pFocusWnd) ) {
		CClientDC dc(this);
		CPalette * pOldPalette = dc.SelectPalette(pPal, TRUE);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);
		
		if( nChanged )
			Invalidate();
	} else
		CDialog::OnPaletteChanged(pFocusWnd);
}

/********************************************************************

	created:	2001/10/25
	in:			see CWnd::OnMouseMove
	out:		see CWnd::OnMouseMove
	return:		none
  
	purpose:	use this method to allow window movements dragging
				the window clicking the entire client area
				
*********************************************************************/

void CHMXDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//
	// Special Thanks to "Jens Rahm" <jens_rahm@hotmail.com>
	//
	if( nFlags == MK_LBUTTON ) {
		::ReleaseCapture();
		SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
