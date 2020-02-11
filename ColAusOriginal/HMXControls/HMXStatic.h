#if !defined(AFX_HMXSTATIC_H__2850C041_7611_11D5_9628_00B0D015B123__INCLUDED_)
#define AFX_HMXSTATIC_H__2850C041_7611_11D5_9628_00B0D015B123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMXStatic window

class CHMXStatic : public CStatic
{
// Construction
public:
	CHMXStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXStatic)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool SetToolTipText(const CString& sText, bool bActivate = true);
	bool ActivateToolTip(bool bEnable = true);
	bool GetTransparent();
	bool SetTransparent( bool bTransparent = true);
	bool SetFontRotation( int nAngle );
	bool SetFontBold(bool bBold = true);
	bool SetFontItalic( bool bItalic = true);
	bool SetFontHeight( int nHeight );
	bool SetFontFaceName( const CString& sFaceName );
	bool GetTextFont( LOGFONT* plgfnt);
	bool SetTextFont( LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName );
	bool SetTextFont( const LOGFONT& lgfnt );
	bool GetTextClr(COLORREF &clr);
	bool SetTextClr( COLORREF clr );
	bool GetBkClr(COLORREF &clr);
	bool SetBkClr( COLORREF clr );
	virtual ~CHMXStatic();

protected:
	bool InitToolTip();
	CToolTipCtrl m_tt;
	CFont m_fntText;
	COLORREF m_clrTextClr;
	COLORREF m_clrBkClr;
	CBrush m_brsBkGnd;
	CBrush m_brsHollow;
	bool m_bTransparent;

	// Generated message map functions
protected:
	//{{AFX_MSG(CHMXStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXSTATIC_H__2850C041_7611_11D5_9628_00B0D015B123__INCLUDED_)
