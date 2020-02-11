// HMXChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "HMXChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHMXChildFrame

IMPLEMENT_DYNCREATE(CHMXChildFrame, CMDIChildWnd)

CHMXChildFrame::CHMXChildFrame()
{
}

CHMXChildFrame::~CHMXChildFrame()
{
}


BEGIN_MESSAGE_MAP(CHMXChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CHMXChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHMXChildFrame message handlers

BOOL CHMXChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	cs.style = WS_CHILD;	// set the right child aspect & behaviour
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
