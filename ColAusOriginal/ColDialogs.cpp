// ColDialogs.cpp : implementation file
//

#include "stdafx.h"
//#include "Colmap.h"
#include "ColDialogs.h"
#include "Paint1Doc.h"
#include "iostream.h"
#include "fstream.h"

ofstream testOutFile1("ColDialogs.txt", ios::out);

//-----------------------------------------------------------------------
// ColDlg class implementation
//-----------------------------------------------------------------------

ColDlg::ColDlg()
{
}

ColDlg::ColDlg(UINT uResource, CWnd* pParent /*=NULL*/)
	: CHMXDialog(uResource, pParent)
{
}

ColDlg::ColDlg(LPCTSTR pszResource, CWnd* pParent /*=NULL*/)
	: CHMXDialog(pszResource, pParent)
{
}

ColDlg::~ColDlg()
{

}

BEGIN_MESSAGE_MAP(ColDlg, CHMXDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL ColDlg::OnInitDialog(void)
{
	CHMXDialog::OnInitDialog();
	
	CHMXDialog::SetBitmap(IDB_BACKGROUND, BITMAP_TILE);
	createFonts();



	return TRUE;
}

void ColDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void ColDlg::createFonts()
{
	dlgFont1.lfHeight = 24;
	dlgFont1.lfWidth = 0;
	dlgFont1.lfEscapement = 0;
	dlgFont1.lfOrientation = 0;
	dlgFont1.lfWeight = FW_NORMAL;
	dlgFont1.lfItalic = 0;
	dlgFont1.lfUnderline = 0;
	dlgFont1.lfStrikeOut = 0;
	dlgFont1.lfCharSet = ANSI_CHARSET;
	dlgFont1.lfOutPrecision = OUT_DEFAULT_PRECIS;
	dlgFont1.lfQuality = PROOF_QUALITY;
	dlgFont1.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(dlgFont1.lfFaceName, "Ariel");

	dlgFont2.lfHeight = 18;
	dlgFont2.lfWidth = 0;
	dlgFont2.lfEscapement = 0;
	dlgFont2.lfOrientation = 0;
	dlgFont2.lfWeight = FW_NORMAL;
	dlgFont2.lfItalic = 0;
	dlgFont2.lfUnderline = 0;
	dlgFont2.lfStrikeOut = 0;
	dlgFont2.lfCharSet = ANSI_CHARSET;
	dlgFont2.lfOutPrecision = OUT_DEFAULT_PRECIS;
	dlgFont2.lfQuality = PROOF_QUALITY;
	dlgFont2.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(dlgFont2.lfFaceName, "Ariel");
}

LOGFONT *ColDlg::getColFont(int font_nbr)
{
	switch (font_nbr)
	{
		case 1:
			return &dlgFont1;
		case 2:
			return &dlgFont2;
		default:
			return &dlgFont1;
	}
}

//----------------------------------------------------------------------
// GameOptsDialog dialog
//----------------------------------------------------------------------

IMPLEMENT_DYNAMIC(GameOptsDialog, CHMXDialog)
GameOptsDialog::GameOptsDialog(CWnd* pParent /*=NULL*/)
	: CHMXDialog(GameOptsDialog::IDD, pParent)
{
}

GameOptsDialog::~GameOptsDialog()
{
}

void GameOptsDialog::DoDataExchange(CDataExchange* pDX)
{
	CHMXDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AUTO_END_TURN, auto_end_turn);
	DDX_Check(pDX, IDC_GO_ANIMATE_PIECE_MOVES, animate_piece_moves);
	DDX_Control(pDX, IDCANCEL, go_cancel);
	DDX_Control(pDX, IDOK, go_ok);
	DDX_Control(pDX, IDC_SCREEN_RES, res_list_box);
}


BEGIN_MESSAGE_MAP(GameOptsDialog, CHMXDialog)
END_MESSAGE_MAP()


// GameOptsDialog message handlers



BOOL GameOptsDialog::OnInitDialog()
{
	CHMXDialog::OnInitDialog();
	CHMXDialog::SetBitmap(IDB_BACKGROUND, BITMAP_TILE);

	// TODO:  Add extra initialization here
	res_list_box.AddString("No change");
	res_list_box.AddString("600 x 800");
	res_list_box.AddString("800 x 1024");
	res_list_box.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GameOptsDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	res_selection = res_list_box.GetCurSel();

	CHMXDialog::OnOK();
}
// D:\Visual Studio Projects\Paint1\ColDialogs.cpp : implementation file
//

#include "stdafx.h"

#include "D:\Visual Studio Projects\Paint1\ColDialogs.h"


// AddPieceDlg dialog ----------------------------------------------------

IMPLEMENT_DYNAMIC(AddPieceDlg, CHMXDialog)
AddPieceDlg::AddPieceDlg(CWnd* pParent /*=NULL*/)
	: CHMXDialog(AddPieceDlg::IDD, pParent)
	, new_piece_type(0)
	, new_piece_owner(0)
	, new_loc_x(0)
	, new_loc_y(0)
{
}

AddPieceDlg::~AddPieceDlg()
{
}

void AddPieceDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT_TYPE, new_piece_type);
	DDX_Text(pDX, IDC_EDIT_OWNER, new_piece_owner);
	DDX_Text(pDX, IDC_EDIT_X, new_loc_x);
	DDX_Text(pDX, IDC_EDIT_Y, new_loc_y);

	CHMXDialog::DoDataExchange(pDX);
}

BOOL AddPieceDlg::OnInitDialog()
{
	CHMXDialog::OnInitDialog();
	CHMXDialog::SetBitmap(IDB_BACKGROUND, BITMAP_TILE);

	return TRUE;
}


BEGIN_MESSAGE_MAP(AddPieceDlg, CHMXDialog)
END_MESSAGE_MAP()


// AddPieceDlg message handlers
// D:\Visual Studio Projects\Paint1\ColDialogs.cpp : implementation file
//

#include "stdafx.h"
//#include "Colmap.h"
#include "D:\Visual Studio Projects\Paint1\ColDialogs.h"

//----------------------------------------------------------------
// ConfirmAttackDlg dialog
//----------------------------------------------------------------

IMPLEMENT_DYNAMIC(ConfirmAttackDlg, CHMXDialog)
ConfirmAttackDlg::ConfirmAttackDlg(CWnd* pParent /*=NULL*/)
	: CHMXDialog(ConfirmAttackDlg::IDD, pParent)
	, dlg_question(_T(""))
	, dlg_ok_string(_T(""))
{
}

ConfirmAttackDlg::~ConfirmAttackDlg()
{
}

BOOL ConfirmAttackDlg::OnInitDialog()
{
	CHMXDialog::OnInitDialog();
	CHMXDialog::SetBitmap(IDB_BACKGROUND, BITMAP_TILE);

	CWnd *pParentWnd = this;

	CRect rc;
	CHMXDialog::GetClientRect(&rc);
	rc.top = rc.bottom * 0.2;
	CPoint top_left(20, 20);
	CPoint bot_right;

	bot_right.x = rc.right - 20;
	bot_right.y = top_left.y + 200;
	rc.SetRect(top_left, bot_right);
	questionStatic.Create(_T(dlg_question), WS_CHILD|WS_VISIBLE|SS_CENTER, 
						   rc, pParentWnd);
	questionStatic.SetTextClr(RGB(255,255,255));
	updateFont(SmallFont, 18);
	SmallFont.lfWeight = FW_BOLD;
	questionStatic.SetTextFont(SmallFont);
	questionStatic.SetFontRotation( 0 );

	return TRUE;
}

void ConfirmAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CHMXDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_DIALOG_QUESTION, dlg_question);
	//DDV_MaxChars(pDX, dlg_question, 100);
	DDX_Text(pDX, IDOK, dlg_ok_string);
	DDV_MaxChars(pDX, dlg_ok_string, 100);
}


BEGIN_MESSAGE_MAP(ConfirmAttackDlg, CHMXDialog)
END_MESSAGE_MAP()

void ConfirmAttackDlg::updateCountry(char *country_text)
{
	dlg_question = "My lord, we are not currently at war with the ";
	dlg_question += country_text;
	dlg_question += ".  Should we really attack?";
	dlg_ok_string = "Yes, we shall crush the foul-smelling ";
	dlg_ok_string += country_text;
}

void ConfirmAttackDlg::updateFont(LOGFONT &logFont, int height)
{
	logFont.lfHeight = height;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(logFont.lfFaceName, "Ariel");
}


//------------------------------------------------------------------------------
// ResolveAttackDlg dialog
//------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(ResolveAttackDlg, CHMXDialog)
ResolveAttackDlg::ResolveAttackDlg(CWnd* pParent /*=NULL*/)
	: CHMXDialog(ResolveAttackDlg::IDD, pParent)
{
}

ResolveAttackDlg::~ResolveAttackDlg()
{
	AttackerPiece.DestroyWindow();
	DefenderPiece.DestroyWindow();
	//AttackerTerrain.DestroyWindow();
	//DefenderTerrain.DestroyWindow();
}

void ResolveAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CHMXDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATTACK_PIECE, AttackerPiece);
	DDX_Control(pDX, IDC_STATIC_TITLE, DlgTitle);
	//DDX_Control(pDX, IDC_ATT_DESC, AttackerDesc);
	DDX_Control(pDX, IDC_DEFEND_PIECE, DefenderPiece);
	//DDX_Control(pDX, IDC_DEF_DESC, DefenderDesc);

}

bool ResolveAttackDlg::updateBitmaps(HBITMAP h_attacker, HBITMAP h_defender)
{
	h_attacker_bm = h_attacker;
	h_defender_bm = h_defender;

	return true;
}

void ResolveAttackDlg::updateFont(LOGFONT &logFont, int height)
{
	logFont.lfHeight = height;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(logFont.lfFaceName, "Ariel");
}

BOOL ResolveAttackDlg::OnInitDialog(void)
{
	CHMXDialog::OnInitDialog();

	AttackerPiece.SetBitmap(h_attacker_bm);
	DefenderPiece.SetBitmap(h_defender_bm);
	//need to get locations of bitmaps, or preferably set dynamically
	//to ensure that dialog is always built correctly
	CRect attack_rect;
	//AttackerPiece.GetClientRect(attack_rect);
	AttackerPiece.GetWindowRect(attack_rect);
	
	testOutFile1 << "Attacker GetWindowRect:" 
		         << " (" << attack_rect.top << "," << attack_rect.left << "), " 
				 << " (" << attack_rect.bottom << "," << attack_rect.right << "), " << endl;

	CHMXDialog::SetBitmap(IDB_BACKGROUND, BITMAP_TILE);
	//DlgTitle.SetTransparent();
	DlgTitle.SetTextClr(RGB(255,255,255));
	updateFont(TitleFont, 24);
	DlgTitle.SetTextFont(TitleFont);
	DlgTitle.SetFontRotation( 0 );

	//set up descriptions of each piece
	CWnd *pParentWnd = this;

	CRect rc;
	CHMXDialog::GetClientRect(&rc);

	testOutFile1 << "CHMXDialog GetClientRect:" 
		         << " (" << rc.top << "," << rc.left << "), " 
				 << " (" << rc.bottom << "," << rc.right << "), " << endl;

	CRect rc1;
	CHMXDialog::GetWindowRect(&rc1);

	testOutFile1 << "CHMXDialog GetWindowRect:" 
		         << " (" << rc1.top << "," << rc1.left << "), " 
				 << " (" << rc1.bottom << "," << rc1.right << "), " << endl;

	rc.top = rc.bottom * 0.2;
	CPoint top_left(110, 55);
	CPoint bot_right;

	bot_right.x = top_left.x + 150;
	bot_right.y = top_left.y + 200;
	rc.SetRect(top_left, bot_right);
	
	AttackerDesc.Create(_T(att_desc), WS_CHILD|WS_VISIBLE|SS_LEFT, 
						   rc, pParentWnd);
	AttackerDesc.SetTextClr(RGB(190,190,190));
	updateFont(SmallFont, 18);
	SmallFont.lfWeight = FW_BOLD;
	AttackerDesc.SetTextFont(SmallFont);
	AttackerDesc.SetFontRotation( 0 );

	//att_str = "Strength: 01\nTerrain Bonus: 00%\nAttack Bonus: 50%";
	top_left.y += 25;
	bot_right.x = top_left.x + 150;
	bot_right.y = top_left.y + 75;
	rc.SetRect(top_left, bot_right);
	AttackerStr.Create(_T(att_str), WS_CHILD|WS_VISIBLE|SS_LEFT, 
						   rc, pParentWnd);
	AttackerStr.SetTextClr(RGB(255,255,0));
	//updateFont(SmallFont, 18);
	SmallFont.lfWeight = FW_NORMAL;
	AttackerStr.SetTextFont(SmallFont);
	AttackerStr.SetFontRotation( 0 );

	top_left.x += 240;
	top_left.y -= 25;
	bot_right.x = top_left.x + 150;
	bot_right.y -= 75;
	rc.SetRect(top_left, bot_right);
	DefenderDesc.Create(_T(def_desc), WS_CHILD|WS_VISIBLE|SS_LEFT, 
						   rc, pParentWnd);
	DefenderDesc.SetTextClr(RGB(190,190,190));
	//updateFont(SmallFont, 18);
	//SmallFont.lfUnderline = TRUE;
	SmallFont.lfWeight = FW_BOLD;
	DefenderDesc.SetTextFont(SmallFont);
	DefenderDesc.SetFontRotation( 0 );

	//def_str = "Strength: 02\nTerrain Bonus: 50%\nDefence Bonus: 00%";
	top_left.y += 25;
	bot_right.x = top_left.x + 150;
	bot_right.y = top_left.y + 75;
	rc.SetRect(top_left, bot_right);
	DefenderStr.Create(_T(def_str), WS_CHILD|WS_VISIBLE|SS_LEFT, 
						   rc, pParentWnd);
	DefenderStr.SetTextClr(RGB(255,255,0));
	//updateFont(SmallFont, 18);
	SmallFont.lfWeight = FW_NORMAL;
	DefenderStr.SetTextFont(SmallFont);
	DefenderStr.SetFontRotation( 0 );
	

	return TRUE;
}

//BEGIN_MESSAGE_MAP(ResolveAttackDlg, CHMXDialog)
BEGIN_MESSAGE_MAP(ResolveAttackDlg, CHMXDialog)
	ON_STN_CLICKED(IDC_ATTACK_PIECE, OnStnClickedAttackPiece)
END_MESSAGE_MAP()


// ResolveAttackDlg message handlers

void ResolveAttackDlg::OnStnClickedAttackPiece()
{
	// TODO: Add your control notification handler code here
}


//------------------------------------------------------------------------------
// CAboutDlg dialog used for App About
//------------------------------------------------------------------------------

CAboutDlg::CAboutDlg() : ColDlg(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	ColDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, DlgTitle);
}

BEGIN_MESSAGE_MAP(CAboutDlg, ColDlg)
END_MESSAGE_MAP()



BOOL CAboutDlg::OnInitDialog(void)
{
	ColDlg::OnInitDialog();

	DlgTitle.SetTextClr(RGB(255,255,255));
	//DlgTitle.updateFont(TitleFont);
	DlgTitle.SetTextFont(*(ColDlg::getColFont(1)));
	DlgTitle.SetFontRotation( 0 );

	return TRUE;
}


//---------------------------------------------------------------------
// ColMsgBox dialog
//---------------------------------------------------------------------

ColMsgBox::ColMsgBox(CWnd* pParent /*=NULL*/)
	: ColDlg(ColMsgBox::IDD, pParent)
{
}

ColMsgBox::~ColMsgBox()
{
}

void ColMsgBox::DoDataExchange(CDataExchange* pDX)
{
	ColDlg::DoDataExchange(pDX);
}

BOOL ColMsgBox::OnInitDialog(void)
{
	ColDlg::OnInitDialog();

	CWnd *pParentWnd = this;

	CRect rc;
	ColDlg::GetClientRect(&rc);
	rc.top = rc.bottom * 0.2;
	message_dynamic.Create(_T(message_str), WS_CHILD|WS_VISIBLE|SS_CENTER, 
						   rc, pParentWnd);
	message_dynamic.SetTextClr(RGB(255,255,255));
	message_dynamic.SetTextFont(*(ColDlg::getColFont(1)));
	message_dynamic.SetFontRotation( 0 );

	return TRUE;
}

BEGIN_MESSAGE_MAP(ColMsgBox, ColDlg)
END_MESSAGE_MAP()


// ColMsgBox message handlers
// D:\Visual Studio Projects\Paint1\ColDialogs.cpp : implementation file
//


//--------------------------------------------------------------------------
// NewGameDlg dialog
//--------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(NewGameDlg, ColDlg)

NewGameDlg::NewGameDlg(CWnd* pParent /*=NULL*/)
	: ColDlg(NewGameDlg::IDD, pParent)
{
}

NewGameDlg::~NewGameDlg()
{
}

void NewGameDlg::DoDataExchange(CDataExchange* pDX)
{
	ColDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_TITLE, DlgTitle);
}

BOOL NewGameDlg::OnInitDialog()
{
	ColDlg::OnInitDialog();

	DlgTitle.SetTextClr(RGB(255,255,255));
	//DlgTitle.updateFont(TitleFont);
	DlgTitle.SetTextFont(*(ColDlg::getColFont(1)));
	DlgTitle.SetFontRotation( 0 );

	CRect rc;
	ColDlg::GetClientRect(&rc);
	//rc.top = rc.bottom * 0.2;
	CWnd *pParentWnd = this;
	DlgBMList[0].bmStatic.Create(NULL, WS_CHILD|WS_VISIBLE|SS_CENTER, 
						   rc, pParentWnd);
	//DlgBMList[0].bmStatic.SetBitmap(DlgBMList[0].hBitmap);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL NewGameDlg::updateBitmaps(int bm_ref, HBITMAP bm_handle)
{
	DlgBMList[bm_ref].hBitmap = bm_handle;

	return TRUE;
}

BEGIN_MESSAGE_MAP(NewGameDlg, ColDlg)
END_MESSAGE_MAP()


// NewGameDlg message handlers


//------------------------------------------------------------------------
// CMapStatDialog dialog

IMPLEMENT_DYNAMIC(CMapStatDialog, CDialog)
CMapStatDialog::CMapStatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMapStatDialog::IDD, pParent)
{
}

CMapStatDialog::~CMapStatDialog()
{
}

void CMapStatDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Radio(pDX, IDC_NONE, display_value);
	//DDX_Radio(pDX, IDC_ALTITUDE, display_altitude);
	//DDX_Radio(pDX, IDC_TERRAIN_TYPE, display_terrain_type);

	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapStatDialog, CDialog)
END_MESSAGE_MAP()


// CMapStatDialog message handlers
