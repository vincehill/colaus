#if !defined(AFX_HMXCHILDFRAME_H__E28D6720_B9B3_11D5_9628_00B0D015B123__INCLUDED_)
#define AFX_HMXCHILDFRAME_H__E28D6720_B9B3_11D5_9628_00B0D015B123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HMXChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHMXChildFrame frame

class CHMXChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CHMXChildFrame)
protected:
	CHMXChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXChildFrame)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHMXChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CHMXChildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXCHILDFRAME_H__E28D6720_B9B3_11D5_9628_00B0D015B123__INCLUDED_)
