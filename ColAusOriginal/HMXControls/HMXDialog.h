#if !defined(AFX_HMXDIALOG_H__79797642_772B_11D5_BD28_0060520365FB__INCLUDED_)
#define AFX_HMXDIALOG_H__79797642_772B_11D5_BD28_0060520365FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXDialog.h : header file
//

#include "dib256.h"

/////////////////////////////////////////////////////////////////////////////
// CHMXDialog dialog

class CHMXDialog : public CDialog
{
// Construction
public:
	CHMXDialog(UINT uResourceID, CWnd* pParent = NULL);
	CHMXDialog(LPCTSTR pszResourceID, CWnd* pParent = NULL);
	CHMXDialog();
	
	enum {
		BITMAP_TILE = 0,		// tile the bitmap to fill the dialog
		BITMAP_STRETCH,			// stretch the bitmap so it fits to the dialog
		BITMAP_CENTER			// center the bitmap inside the dialog
	};
	
	enum {
		BKGND_NONE = 0,			// bkgnd default
		BKGND_COLOR,			// bkgnd is a color
		BKGND_IMAGE				// bkgnd is an image
	};

// Attributes
public:
	bool	SetBitmap(const CString& sFileName, int Type = CHMXDialog::BITMAP_TILE);
	bool	SetBitmap(UINT uResource, int Type = CHMXDialog::BITMAP_TILE);
	bool	SetBkClr(COLORREF clr);
	bool	GetBkClr(COLORREF& clr);
	
// Dialog Data
protected:
	//{{AFX_DATA(CHMXDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDIBitmap	m_bmpBkGnd;
	int			m_nType;		// see enum above
	COLORREF	m_clrBkGnd;
	CBrush		m_brsBkGnd;
	int			m_nBkGnd;
	
	// Generated message map functions
	//{{AFX_MSG(CHMXDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	CommonConstruct();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXDIALOG_H__79797642_772B_11D5_BD28_0060520365FB__INCLUDED_)
