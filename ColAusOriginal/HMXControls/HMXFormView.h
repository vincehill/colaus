#if !defined(AFX_HMXFORMVIEW_H__FFF79864_7716_11D5_BD28_0060520365FB__INCLUDED_)
#define AFX_HMXFORMVIEW_H__FFF79864_7716_11D5_BD28_0060520365FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXFormView.h : header file
//

#include "dib256.h"

/////////////////////////////////////////////////////////////////////////////
// CHMXFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CHMXFormView : public CFormView
{
protected:
	CHMXFormView();           // protected constructor used by dynamic creation
	CHMXFormView(UINT nIDD);
	
// Form Data
public:
	//{{AFX_DATA(CHMXFormView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
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
	
	bool SetBkClr( COLORREF clr );
	bool GetBkClr( COLORREF clr );
	bool SetBitmap(const CString& sFileName, int Type = CHMXFormView::BITMAP_TILE);
	bool SetBitmap(UINT uResource, int Type = CHMXFormView::BITMAP_TILE);
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXFormView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL
	virtual afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	
// Implementation
protected:
	virtual ~CHMXFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	int			m_nBkGnd;
	COLORREF	m_clrBkGnd;
	CBrush		m_brsBkGnd;
	CDIBitmap	m_bmpBkGnd;
	int			m_nType;		// see enum above
	
	// Generated message map functions
	//{{AFX_MSG(CHMXFormView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CommonConstructor();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXFORMVIEW_H__FFF79864_7716_11D5_BD28_0060520365FB__INCLUDED_)
