#if !defined(AFX_HMXSHAPEDDIALOG_H__FAC576C4_CA7F_11D5_BD28_0060520365FB__INCLUDED_)
#define AFX_HMXSHAPEDDIALOG_H__FAC576C4_CA7F_11D5_BD28_0060520365FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HMXShapedDialog.h : header file
//

#include "dib256.h"

/////////////////////////////////////////////////////////////////////////////
// CHMXShapedDialog dialog

class CHMXShapedDialog : public CDialog
{
// Construction
public:
	CHMXShapedDialog(UINT uResourceID, CWnd* pParent = NULL);
	CHMXShapedDialog(LPCTSTR pszResourceID, CWnd* pParent = NULL);
	CHMXShapedDialog();
	
// Attributes
public:
	bool SetRegionFile(const CString& sFileRgn, COLORREF clrTransp = RGB( 0, 0, 0) );
	
// Dialog Data
	//{{AFX_DATA(CHMXShapedDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXShapedDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	
	// Generated message map functions
	//{{AFX_MSG(CHMXShapedDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	CommonConstruct();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXSHAPEDDIALOG_H__FAC576C4_CA7F_11D5_BD28_0060520365FB__INCLUDED_)
