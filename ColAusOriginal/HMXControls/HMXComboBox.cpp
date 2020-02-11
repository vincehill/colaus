// HMXCombobox.cpp : implementation file
//

/********************************************************************

	created:	2001/10/25
	file:		HMXComboBox
	author:		Massimo Colurcio
	homepage:	http://www.softhor.com/developmentarea
	email:		m.colurcio@softhor.com
	
	purpose:	new kind of CComboBox class

	special thanks to Davide Calabro' (tooltip from BtnST)

*********************************************************************/

#include "stdafx.h"
#include "HMXComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXCombobox

CHMXComboBox::CHMXComboBox()
{
	m_bEnableEditing = true;
	m_clrBkClr = ::GetSysColor(COLOR_WINDOW);
	m_clrTextClr = ::GetSysColor(COLOR_WINDOWTEXT);
	m_brsBkGnd.CreateSolidBrush(m_clrBkClr);	
}

CHMXComboBox::~CHMXComboBox()
{
	m_brsBkGnd.DeleteObject();
	m_fntText.DeleteObject();
}


BEGIN_MESSAGE_MAP(CHMXComboBox, CComboBox)
	//{{AFX_MSG_MAP(CHMXComboBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXComboBox message handlers

HBRUSH CHMXComboBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetBkColor(m_clrBkClr);
	pDC->SetTextColor(m_clrTextClr);
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return (HBRUSH)m_brsBkGnd;
}

HBRUSH CHMXComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SetBkColor( m_clrBkClr );
	pDC->SetTextColor( m_clrTextClr );
	
	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)m_brsBkGnd;
}

/********************************************************************

	created:	2001/10/25
	in:			clr
	out:		none
	return:		always true;
	
	purpose:	set background color

*********************************************************************/

bool CHMXComboBox::SetBkClr(COLORREF clr)
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

bool CHMXComboBox::GetBkClr(COLORREF & clr)
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

bool CHMXComboBox::SetTextClr(COLORREF clr)
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

bool CHMXComboBox::GetTextClr(COLORREF & clr)
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

bool CHMXComboBox::SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName )
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

bool CHMXComboBox::SetTextFont( const LOGFONT& lgfnt )
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

bool CHMXComboBox::GetTextFont( LOGFONT* plgfnt)
{
	GetFont()->GetObject( sizeof(LOGFONT), (void*)plgfnt);
	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			bEditing
	out:		none
	return:		always true
	
	purpose:	enable / diable editing

*********************************************************************/

bool CHMXComboBox::EnableEditing(bool bEnableEditing)
{
	// TODO: Add your specialized code here and/or call the base class
	m_bEnableEditing = bEnableEditing;

	return true;
}

LRESULT CHMXComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !m_bEnableEditing )
		if( message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK || message == WM_KEYDOWN || message == WM_VKEYTOITEM )
			return FALSE;
	
	return CComboBox::WindowProc(message, wParam, lParam);
}

/********************************************************************

	created:	2001/10/25
	in:			bBold
	out:		none
	return:		always true
	
	purpose:	set font bold

*********************************************************************/

bool CHMXComboBox::SetFontBold( bool bBold )
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

bool CHMXComboBox::SetFontItalic( bool bItalic )
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

bool CHMXComboBox::SetFontHeight( int nHeight )
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

bool CHMXComboBox::SetFontFaceName( const CString& sFaceName )
{
	LOGFONT lgfnt;
	
	GetTextFont( &lgfnt );
	strcpy( lgfnt.lfFaceName, sFaceName );
	
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

bool CHMXComboBox::InitToolTip()
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

bool CHMXComboBox::SetToolTipText(const CString& sText, bool bActivate)
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

bool CHMXComboBox::ActivateToolTip(bool bActivate)
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

BOOL CHMXComboBox::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_tt.RelayEvent(pMsg);
	
	return CComboBox::PreTranslateMessage(pMsg);
} 

