// HMXStatic.cpp : implementation file
//

/********************************************************************

	created:	2001/10/25
	file:		HMXStatic
	author:		Massimo Colurcio
	homepage:	http://www.softhor.com/developmentarea
	email:		m.colurcio@softhor.com
	
	purpose:	more control over static text

	special thanks to Davide Calabro' (tooltip from BtnST)

*********************************************************************/

#include "stdafx.h"
#include "HMXStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXStatic

CHMXStatic::CHMXStatic()
{
	m_bTransparent = true;
	m_clrBkClr = ::GetSysColor(COLOR_WINDOW);
	m_clrTextClr = ::GetSysColor(COLOR_WINDOWTEXT);
	m_brsBkGnd.CreateSolidBrush(m_clrBkClr);
	m_brsHollow.CreateStockObject(HOLLOW_BRUSH);
}

CHMXStatic::~CHMXStatic()
{
	m_brsBkGnd.DeleteObject();
	m_brsHollow.DeleteObject();
	m_fntText.DeleteObject();
}


BEGIN_MESSAGE_MAP(CHMXStatic, CStatic)
	//{{AFX_MSG_MAP(CHMXStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXStatic message handlers

HBRUSH CHMXStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO: Change any attributes of the DC here
	if( m_bTransparent )
 		pDC->SetBkMode( TRANSPARENT );
	else {
		pDC->SetBkMode( OPAQUE );
		pDC->SetBkColor(m_clrBkClr);
	}
	pDC->SetTextColor(m_clrTextClr);

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	if( m_bTransparent )
		return (HBRUSH)m_brsHollow;
	else
		return (HBRUSH)m_brsBkGnd;
}

BOOL CHMXStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if( m_bTransparent )
		pDC->SetBkMode( TRANSPARENT );
	else
		pDC->SetBkColor(m_clrBkClr);

	return CStatic::OnEraseBkgnd(pDC);
}

/////////////////////////////////////////////////////////////////////////////

/********************************************************************

	created:	2001/10/25
	in:			clr
	out:		none
	return:		always true;
	
	purpose:	set background color

*********************************************************************/

bool CHMXStatic::SetBkClr(COLORREF clr)
{
	m_clrBkClr = clr;
	m_brsBkGnd.DeleteObject();
	m_brsBkGnd.CreateSolidBrush(m_clrBkClr);
	Invalidate();

	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		clr
	return:		always true
	
	purpose:	return background color

*********************************************************************/

bool CHMXStatic::GetBkClr(COLORREF & clr)
{
	clr = m_clrBkClr;
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			clr
	out:		none
	return:		always true;
	
	purpose:	set Text color

*********************************************************************/

bool CHMXStatic::SetTextClr(COLORREF clr)
{
	m_clrTextClr = clr;
	Invalidate();

	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		clr
	return:		always true
	
	purpose:	get text color

*********************************************************************/

bool CHMXStatic::GetTextClr(COLORREF & clr)
{
	clr = m_clrTextClr;
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			nHeight, bBold, bItalic, sFaceName
	out:		none
	return:		always true
	
	purpose:	set the font for the control

*********************************************************************/

bool CHMXStatic::SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName )
{
	LOGFONT lgfnt;

	lgfnt.lfHeight			= -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
	lgfnt.lfWidth			= 0;
	lgfnt.lfEscapement		= 0;
	lgfnt.lfOrientation		= 0;
	lgfnt.lfWeight			= bBold?FW_BOLD:FW_DONTCARE;
	lgfnt.lfItalic			= bItalic?TRUE:FALSE;
	lgfnt.lfUnderline		= FALSE;
	lgfnt.lfStrikeOut		= FALSE;
	lgfnt.lfCharSet			= DEFAULT_CHARSET;
	lgfnt.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lgfnt.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lgfnt.lfQuality			= DEFAULT_QUALITY;
	lgfnt.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	strcpy( lgfnt.lfFaceName, sFaceName );

	return SetTextFont( lgfnt );

}

/********************************************************************

	created:	2001/10/25
	in:			lgFont
	out:		none
	return:		always true
	
	purpose:	set the font for the control

*********************************************************************/

bool CHMXStatic::SetTextFont( const LOGFONT& lgfnt )
{
	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect( &lgfnt );
	SetFont( &m_fntText, TRUE );

	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		lgFont
	return:		always true
	
	purpose:	get text font

*********************************************************************/

bool CHMXStatic::GetTextFont( LOGFONT* plgfnt)
{
	GetFont()->GetObject( sizeof(LOGFONT), (void*)plgfnt);
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			bTransparent
	out:		none
	return:		always true
	
	purpose:	set transparency

*********************************************************************/

bool CHMXStatic::SetTransparent( bool bTransparent /* = true */)
{
	m_bTransparent = bTransparent;
	Invalidate();
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		transparency
	
	purpose:	get transparency

*********************************************************************/

bool CHMXStatic::GetTransparent()
{
	return m_bTransparent;
}

/********************************************************************

	created:	2001/10/25
	in:			bBold
	out:		none
	return:		always true
	
	purpose:	set font bold

*********************************************************************/

bool CHMXStatic::SetFontBold( bool bBold )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	lgfnt.lfWeight	= bBold?FW_BOLD:FW_DONTCARE; 
	
	SetTextFont( lgfnt );
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			bItalic
	out:		none
	return:		always true
	
	purpose:	set the font italic

*********************************************************************/

bool CHMXStatic::SetFontItalic( bool bItalic )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	lgfnt.lfItalic	= bItalic ? TRUE : FALSE; 
	
	SetTextFont( lgfnt );
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			nHeight
	out:		none
	return:		always true
	
	purpose:	set the font height

*********************************************************************/

bool CHMXStatic::SetFontHeight( int nHeight )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	lgfnt.lfHeight	= -MulDiv(nHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);    
	lgfnt.lfWidth	= 0; 
	
	SetTextFont( lgfnt );
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			sFaceName
	out:		none
	return:		always true
	
	purpose:	set the font face name

*********************************************************************/

bool CHMXStatic::SetFontFaceName( const CString& sFaceName )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	strcpy( lgfnt.lfFaceName, sFaceName );
	
	SetTextFont( lgfnt );
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			nAngle
	out:		none
	return:		always true
	
	purpose:	set the rotation of fonts
				works only with TT fonts

*********************************************************************/

bool CHMXStatic::SetFontRotation( int nAngle )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	lgfnt.lfEscapement	= long(nAngle)*10;
	lgfnt.lfOrientation	= long(nAngle)*10;
	SetTextFont( lgfnt );
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		always true
	
	purpose:	init tooltip

*********************************************************************/

bool CHMXStatic::InitToolTip()
{
	if (m_tt.m_hWnd == NULL) {
		m_tt.Create(this);
		m_tt.Activate(true);
		m_tt.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	}
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			sText, bActivate
	out:		none
	return:		always true
	
	purpose:	set tooltip text

*********************************************************************/

bool CHMXStatic::SetToolTipText(const CString& sText, bool bActivate)
{
	InitToolTip(); 
	
	// If there is no tooltip defined then add it
	if (m_tt.GetToolCount() == 0)
	{
		CRect rect; 
		GetClientRect(rect);
		m_tt.AddTool(this, sText, rect, 1);
	}
	
	m_tt.UpdateTipText(sText, this, 1);
	m_tt.Activate(bActivate);
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			bActivate
	out:		none
	return:		always true
	
	purpose:	activate/deactivate tooltip

*********************************************************************/

bool CHMXStatic::ActivateToolTip(bool bActivate)
{
	if (m_tt.GetToolCount() == 0)
		return false;
	
	// Activate tooltip
	m_tt.Activate(bActivate);
	
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			see CWnd::PretanslateMessage
	out:		see CWnd::PretanslateMessage
	return:		see CWnd::PretanslateMessage
	
	purpose:	let tooltip works

*********************************************************************/

BOOL CHMXStatic::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_tt.RelayEvent(pMsg);
	
	return CStatic::PreTranslateMessage(pMsg);
} 
