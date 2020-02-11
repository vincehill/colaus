// HMXNumEdit.cpp: implementation of the CHMXNumEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HMXNumEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHMXNumEdit::CHMXNumEdit()
{
	m_bEngFormat = false;
}

CHMXNumEdit::~CHMXNumEdit()
{

}
/////////////////////////////////////////////////////////////////////////////
// CHMXNumEdit

BEGIN_MESSAGE_MAP(CHMXNumEdit, CHMXEdit)
	//{{AFX_MSG_MAP(CHMXNumEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillFocus)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXNumEdit message handlers

void CHMXNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if( isdigit(nChar) ||
		nChar == '+' ||
		nChar == '-' ||
		nChar == '.' ||
		nChar == (unsigned)*localeconv()->decimal_point ||
		nChar == 'e' ||
		nChar == 'E' ||
		nChar == VK_DELETE ||
		nChar == VK_BACK)
		if( nChar == '.' )
			if( nChar == UINT(*localeconv()->decimal_point))
				CHMXEdit::OnChar(nChar, nRepCnt, nFlags);
			else
				SendMessage( WM_CHAR, *localeconv()->decimal_point, 0);
		else
			CHMXEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CHMXNumEdit::OnKillFocus()
{
	// TODO: Add your control notification handler code here
	if( m_bEngFormat ) {
		CString sBuffer;
		double nVal;
		GetWindowText( sBuffer );
		if( !sBuffer.IsEmpty() ) {
			nVal = atof(sBuffer);
			sBuffer.Format("%g", nVal);
			SetWindowText( sBuffer);
		}
	}

	CHMXEdit::OnKillFocus();
}

/********************************************************************

	created:	2001/10/25
	in:			bEngFormat
	out:		none
	return:		always true
  
	purpose:	set 'Engineers' format
	
*********************************************************************/

bool CHMXNumEdit::SetEngFormat(bool bEngFormat)
{
	m_bEngFormat = bEngFormat;

	return true;
}

/********************************************************************

	created:	2001/10/25
	in:			none
	out:		none
	return:		bEngFormat
  
	purpose:	get 'Engineers' format
	
*********************************************************************/

bool CHMXNumEdit::GetEngFormat()
{
	return m_bEngFormat;
}

HBRUSH CHMXNumEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO: Change any attributes of the DC here
	return CHMXEdit::CtlColor(pDC, nCtlColor);
}
