#if !defined(AFX_HMXLISTCTRL_H__8FDBDC80_7464_11D5_9628_00B0D015B123__INCLUDED_)
#define AFX_HMXLISTCTRL_H__8FDBDC80_7464_11D5_9628_00B0D015B123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMXListCtrl window

class CHMXListCtrl : public CListCtrl
{
// Construction
public:
	CHMXListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXListCtrl)
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool SetToolTipText(const CString& sText, bool bActivate = true);
	bool ActivateToolTip(bool bEnable = true);
	bool SetFontBold( bool bBold = true);
	bool SetFontItalic( bool bItalic = true);
	bool SetFontHeight( int nHeight );
	bool SetFontFaceName( const CString& sFaceName );
	bool EnableEditing(bool bEnableEditing = true);
	bool GetTextFont(LOGFONT* plgfnt);
	bool SetTextFont(LONG nHeight, bool bBold, bool bItalic, const CString& sFaceName);
	bool SetTextFont(const LOGFONT& lgfnt);
	bool GetTextClr(COLORREF &clr);
	bool SetTextClr(COLORREF clr);
	bool GetBkClr(COLORREF &clr);
	bool SetBkClr(COLORREF clr);
	virtual ~CHMXListCtrl();

	// Generated message map functions
protected:
	bool InitToolTip();
	CToolTipCtrl m_tt;
	bool m_bEnableEditing;
	CFont m_fntText;
	COLORREF m_clrBkClr;
	COLORREF m_clrTextClr;
	//{{AFX_MSG(CHMXListCtrl)
	//}}AFX_MSG
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXLISTCTRL_H__8FDBDC80_7464_11D5_9628_00B0D015B123__INCLUDED_)
