#if !defined(AFX_HMXSHAPEDFORMVIEW_H__D338EEA0_BD98_11D5_9628_00B0D015B123__INCLUDED_)
#define AFX_HMXSHAPEDFORMVIEW_H__D338EEA0_BD98_11D5_9628_00B0D015B123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXShapedFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMXShapedFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CHMXShapedFormView : public CFormView
{
protected:
	CHMXShapedFormView();           // protected constructor used by dynamic creation
	CHMXShapedFormView(UINT nIDD, const CString& sFileRgn = "", COLORREF clr = RGB(0,0,0));
	
// Form Data
public:
	//{{AFX_DATA(CHMXShapedFormView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	bool Dismiss();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXShapedFormView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HRGN BitmapRegion(HBITMAP hBitmap,COLORREF clsTransp=RGB(0,0,0),bool bTransp=true);
	CString m_sFileRgn;		// this string will keep the BMP filename;
	HBITMAP m_hBmp;			// This is the handle to the bitmap which stores the picture
	HBITMAP m_hPrevBmp;		// This handle is to store the previous bitmap selected on
							// the memory context. It should normally be NULL, but it
							// doesn't harm anyone to take care of such little precautions!
	HDC m_hMemDC;			// This is the handle to the memory context where we copy first
							// the bitmap. When its time to paint the background, we copy
							// the bitmap from the memory context to the device context!
	HRGN m_hRegion;			// The clipping region that makes this cool effect!
	BITMAP m_bmInfo;		// This bitmap hepls us to know the size of the loaded bitmap!
	COLORREF m_clrTransp;	// This is the trasparent color for Region
	virtual ~CHMXShapedFormView();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHMXShapedFormView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CommonConstructor();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXSHAPEDFORMVIEW_H__D338EEA0_BD98_11D5_9628_00B0D015B123__INCLUDED_)
