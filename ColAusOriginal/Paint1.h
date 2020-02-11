// Paint1.h : main header file for the Paint1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CPaint1App:
// See Paint1.cpp for the implementation of this class
//

class CColAusApp : public CWinApp
{
public:
	CColAusApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Application wide structures/objects  ??
	

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CColAusApp theApp;
