#if !defined(AFX_HMXNUMEDIT_H__F5CF2723_FC89_11D4_BD28_0060520365FB__INCLUDED_)
#define AFX_HMXNUMEDIT_H__F5CF2723_FC89_11D4_BD28_0060520365FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "HMXEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CHMXNumEdit window

class CHMXNumEdit : public CHMXEdit
{
// Construction
public:
	CHMXNumEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHMXNumEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool GetEngFormat();
	bool SetEngFormat( bool bEngFormat = true );
	virtual ~CHMXNumEdit();

	// Generated message map functions
protected:
	bool m_bEngFormat;
	//{{AFX_MSG(CHMXNumEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMXNUMEDIT_H__F5CF2723_FC89_11D4_BD28_0060520365FB__INCLUDED_)
