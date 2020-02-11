#pragma once


#include "Paint1.h"
#include "afxwin.h"
#include "HMXControls/HMXDialog.h"
#include "HMXControls/HMXStatic.h"
#include "d:\visual studio projects\paint1\hmxcontrols\hmxstatic.h"

//----------------------------------------------------------------------------
// ColDlg dialog - to be inherited from to create a 'colonial' look dialog
//----------------------------------------------------------------------------

class ColDlg : public CHMXDialog
{
	//DECLARE_DYNAMIC(ColDlg)

public:
	// standard constructors
	ColDlg(UINT uResourceID, CWnd* pParent = NULL);  
	ColDlg(LPCTSTR pszResourceED, CWnd* pParent = NULL);
	ColDlg();

	virtual ~ColDlg();

	//Overrides
protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:	
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	LOGFONT *getColFont(int font_nbr);
private:
	void createFonts(void);
	LOGFONT dlgFont1;
	LOGFONT dlgFont2;
};

//-----------------------------------------------------------------------
// GameOptsDialog dialog
//-----------------------------------------------------------------------

class GameOptsDialog : public CHMXDialog
{
	DECLARE_DYNAMIC(GameOptsDialog)

public:
	GameOptsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GameOptsDialog();

// Dialog Data
	enum { IDD = IDD_GAME_OPTS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int auto_end_turn;
	int res_selection;
	int animate_piece_moves;
	CButton go_cancel;
	CButton go_ok;
	CComboBox res_list_box;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
#pragma once

//----------------------------------------------------------------------------
// AddPieceDlg dialog
//----------------------------------------------------------------------------

class AddPieceDlg : public CHMXDialog
{
	DECLARE_DYNAMIC(AddPieceDlg)

public:
	AddPieceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddPieceDlg();

// Dialog Data
	enum { IDD = IDD_ADD_PIECE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	int new_piece_type;
	int new_piece_owner;
	int new_loc_x;
	int new_loc_y;
};
#pragma once

//----------------------------------------------------------------------------
// ConfirmAttackDlg dialog
//----------------------------------------------------------------------------

class ConfirmAttackDlg : public CHMXDialog
{
	DECLARE_DYNAMIC(ConfirmAttackDlg)

public:
	ConfirmAttackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConfirmAttackDlg();
	void updateCountry(char *);
	void updateFont(LOGFONT &logFont, int height);

// Dialog Data
	enum { IDD = IDD_CONFIRM_ATTACK };
	LOGFONT SmallFont;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	// stores the question to be displayed
	CHMXStatic questionStatic;
	CString dlg_question;
	CString dlg_ok_string;
};
#pragma once

//---------------------------------------------------------------------------
// ResolveAttackDlg dialog
//---------------------------------------------------------------------------

//class ResolveAttackDlg : public CDialog
class ResolveAttackDlg : public CHMXDialog
{
	DECLARE_DYNAMIC(ResolveAttackDlg)

public:
	ResolveAttackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ResolveAttackDlg();

	//set up functions for caller
	bool updateBitmaps(HBITMAP, HBITMAP);
	void updateFont(LOGFONT &, int height);

	CStatic attacker;
	HBITMAP h_attacker_bm;
	HBITMAP h_defender_bm;
	LOGFONT TitleFont;
	LOGFONT SmallFont;
	CString att_desc;
	CString def_desc;
	CString att_str;
	CString def_str;

// Dialog Data
	enum { IDD = IDD_RESOLVE_ATTACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedAttackPiece();
	CHMXStatic DlgTitle;
	CHMXStatic AttackerPiece;
	CHMXStatic AttackerDesc;
	CHMXStatic DefenderPiece;
	CHMXStatic DefenderDesc;
	CHMXStatic AttackerStr;
	CHMXStatic DefenderStr;
	//CHMXStatic AttackerPieceLabel;
	//CHMXStatic AttackerStrength;
	//CHMXStatic DefenderPieceLabel;
	//CHMXStatic DefenderStrength;
};

//---------------------------------------------------------------------------
// CAboutDlg dialog
//---------------------------------------------------------------------------

class CAboutDlg : public ColDlg
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	CHMXStatic DlgTitle;
};

#pragma once

//-----------------------------------------------------------------------------
// ColMsgBox dialog
//-----------------------------------------------------------------------------

class ColMsgBox : public ColDlg
{
//	DECLARE_DYNAMIC(ColMsgBox)

public:
	ColMsgBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~ColMsgBox();

// Dialog Data
	enum { IDD = IDD_COL_MSGBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString message_str;
	CHMXStatic message_dynamic;
};
#pragma once

//----------------------------------------------------------------------------
// NewGameDlg dialog
//----------------------------------------------------------------------------

struct sDlgStaticBM{
	HBITMAP hBitmap;
	CHMXStatic bmStatic;
};

class NewGameDlg : public ColDlg
{
	DECLARE_DYNCREATE(NewGameDlg)

public:
	NewGameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NewGameDlg();

// Dialog Data
	enum { IDD = IDD_NEW_GAME_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//functions
	BOOL updateBitmaps(int bm_ref, HBITMAP bm_handle);  //inherit from ColDlg

	CHMXStatic DlgTitle;	//inherit from ColDlg

	//should inherit this, so other classes can similarly derive
	sDlgStaticBM DlgBMList[10];  //inherit from ColDlg

};
#pragma once

//---------------------------------------------------------------------------
// CMapStatDialog dialog

class CMapStatDialog : public CDialog
{
	DECLARE_DYNAMIC(CMapStatDialog)

public:
	CMapStatDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapStatDialog();

// Dialog Data
	enum { IDD = IDD_MAP_STATS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int display_value;
};
