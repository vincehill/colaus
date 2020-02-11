// HMXFormView.cpp : implementation file
//

/********************************************************************

	created:	2001/10/25
	file:		HMXFormView
	author:		Massimo Colurcio
	homepage:	http://www.softhor.com/developmentarea
	email:		m.colurcio@softhor.com
	thanks to:	an unknown programmer for bitmap drawing
	
	purpose:	want a form with background bitmap or color?

*********************************************************************/


#include "stdafx.h"
#include "HMXFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXFormView

CHMXFormView::CHMXFormView(UINT nIDD)
	: CFormView(nIDD)
{
	CommonConstructor();
}

CHMXFormView::~CHMXFormView()
{
	m_brsBkGnd.DeleteObject();
}

void CHMXFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHMXFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHMXFormView, CFormView)
	//{{AFX_MSG_MAP(CHMXFormView)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
//	ON_WM_NCPAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXFormView diagnostics

#ifdef _DEBUG
void CHMXFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHMXFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CHMXFormView::CommonConstructor()
{
	m_nType = BITMAP_TILE;
	m_nBkGnd = BKGND_NONE;
	m_brsBkGnd.CreateSolidBrush( m_clrBkGnd );
}

/********************************************************************

	created:	2001/10/25
	in:			uResource, nType
	out:		none
	return:		TRUE is success 
  
	purpose:	set bitmap from resource
	
*********************************************************************/

bool CHMXFormView::SetBitmap(UINT uResource, int nType /*BITMAP_TILE*/)
{
	bool bRet;
	m_nType = nType;
	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	m_nBkGnd = BKGND_IMAGE;
	bRet = m_bmpBkGnd.LoadResource(uResource)?true:false;
	Invalidate();
	return bRet;
}

/********************************************************************

	created:	2001/10/25
	in:			sFileName, nType
	out:		none
	return:		TRUE if success 
  
	purpose:	set bitmap from BMP file
	
*********************************************************************/

bool CHMXFormView::SetBitmap(const CString& sFileName, int nType /*BITMAP_TILE*/)
{
	bool bRet;
	m_nType = nType;
	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	m_nBkGnd = BKGND_IMAGE;
	bRet = m_bmpBkGnd.Load(sFileName)?true:false;
	Invalidate();
	return bRet;
}

/********************************************************************

	created:	2001/10/25
	in:			clr
	out:		none
	return:		always TRUE
  
	purpose:	set background color
	
*********************************************************************/

bool CHMXFormView::SetBkClr( COLORREF clr )
{
	m_clrBkGnd = clr;
	m_brsBkGnd.DeleteObject();
	m_brsBkGnd.CreateSolidBrush( m_clrBkGnd );
	m_nBkGnd = BKGND_COLOR;
	
	Invalidate();
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		background color
  
	purpose:	get background color
	
*********************************************************************/

bool CHMXFormView::GetBkClr( COLORREF clr)
{
	clr = m_clrBkGnd;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CHMXFormView message handlers

/********************************************************************

	created:	2001/10/25
	in:			pDC, see CWnd::OnDraw
	out:		none
	return:		none
  
	purpose:	draw background color or background bitmap
	
*********************************************************************/

void CHMXFormView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rc;
	GetClientRect(rc);
	int x = 0, y = 0;
	
	switch( m_nBkGnd ) {
	case BKGND_COLOR:
		pDC->FillRect( &rc, &m_brsBkGnd);
		break;
	case BKGND_IMAGE:
		if(m_bmpBkGnd.GetPixelPtr() != 0) {
			ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
			
			switch(m_nType) {
			case BITMAP_CENTER:
				// center the bitmap
				CFormView::OnDraw(pDC);
				x = (rc.Width() - m_bmpBkGnd.GetWidth()) / 2;
				y = (rc.Height() - m_bmpBkGnd.GetHeight()) / 2;
				m_bmpBkGnd.DrawDIB(pDC, x, y);
				break;
				
			case BITMAP_STRETCH:
				// stretch bitmap so it will best fit to the dialog
				m_bmpBkGnd.DrawDIB(pDC, 0, 0, rc.Width(), rc.Height());
				break;
				
			default:
				// tile the bitmap
				while(y < rc.Height()) {
					while(x < rc.Width()) {
						m_bmpBkGnd.DrawDIB(pDC, x, y);
						x += m_bmpBkGnd.GetWidth();
					}
					x = 0;
					y += m_bmpBkGnd.GetHeight();
				}
				break;
			}
		}
		break;
	default:
		break;
	}
	
	return;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		TRUE if success
  
	purpose:	handling palette changing
	
*********************************************************************/

BOOL CHMXFormView::OnQueryNewPalette() 
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
	return CHMXFormView::OnQueryNewPalette();
}

/********************************************************************

	created:	2001/10/25
	in:			see CWnd::OnPaletteChanged
	out:		see CWnd::OnPaletteChanged
	return:		none
  
	purpose:	handling palette changing
	
*********************************************************************/

void CHMXFormView::OnPaletteChanged(CWnd* pFocusWnd) 
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
		CHMXFormView::OnPaletteChanged(pFocusWnd);
}

/********************************************************************

	created:	2001/10/25
	in:			see CWnd::OnMouseMove
	out:		see CWnd::OnMouseMove
	return:		none
  
	purpose:	use this method to allow window movements dragging
				the window clicking the entire client area
	
*********************************************************************/

void CHMXFormView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//
	// Special Thanks to "Jens Rahm" <jens_rahm@hotmail.com>
	//
	if( nFlags == MK_LBUTTON ) {
		::ReleaseCapture();
		GetParentFrame()->SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	
	CFormView::OnMouseMove(nFlags, point);
}
