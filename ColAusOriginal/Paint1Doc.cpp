// Paint1Doc.cpp : implementation of the CPaint1Doc class
//

#include "stdafx.h"
#include "Paint1.h"

#include "Paint1Doc.h"
#include "ColDialogs.h"
#include "GameRules.h"
#include "cSound.h"

#include "iostream.h"
#include "fstream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ofstream testOutFileDoc("Paint1Doc.txt", ios::out);

//GameRules RuleBase;

IMPLEMENT_DYNCREATE(CColAusDoc, CDocument)

BEGIN_MESSAGE_MAP(CColAusDoc, CDocument)

	//ON_COMMAND(ID_FILE_GAMEOPTIONS, OnFileGameoptions)
	ON_COMMAND(ID_TEST_SYNCH, OnTestSynch)
END_MESSAGE_MAP()


// CColAusDoc construction/destruction

CColAusDoc::CColAusDoc()
{
	// TODO: add one-time construction code here
	current_game = false;
	first_time = true;
}

CColAusDoc::~CColAusDoc()
{
}

BOOL CColAusDoc::OnOpenDocument(LPCTSTR lpszFileName)
{
	//This code is for testing the save game process

	//initialise all parts of the document
	MasterList.initPieces();
	MasterMap.initMap();
	for (int i = 0; i < MAX_PLAYERS; i++)
		PlayerMap[i].initMap();
	displayMoves.initMoves();

	BOOL Open_Result = CDocument::OnOpenDocument(lpszFileName);
	if  (Open_Result)
	{
		//createGame();
		current_game = true;
		int active_piece = MasterList.GetNextPiece(gameStats.active_player);
		MasterList.UpdateActive(active_piece);
		return TRUE;
	}
	else
	{
		//output some message to indicate an error occurred
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					  FORMAT_MESSAGE_FROM_SYSTEM | 
			          FORMAT_MESSAGE_IGNORE_INSERTS,
		              NULL,
		              GetLastError(),
	 	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		              (LPTSTR) &lpMsgBuf,
		              0, NULL);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		AfxMessageBox((LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return FALSE;
	}


	//This code is for opening an original Colonization map (not a saved file)

	m_csFileName = (CString *)0;
	if (INVALID_HANDLE_VALUE == 
			(m_hFile=CreateFile(lpszFileName, 
			                    GENERIC_READ, 
								0, 
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		AfxMessageBox(IDS_ERR_OPEN);
		return FALSE;
	}

	int file_read_size = 6 + (58 * 72);
	char inBuffer[8192];
	DWORD bytes_to_read = file_read_size;
	DWORD bytes_read;

	// Attempt a synchronous read operation. 
	BOOL readResult = ReadFile(m_hFile, &inBuffer, bytes_to_read, &bytes_read, NULL); 
	if (!readResult)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					  FORMAT_MESSAGE_FROM_SYSTEM | 
			          FORMAT_MESSAGE_IGNORE_INSERTS,
		              NULL,
		              GetLastError(),
	 	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		              (LPTSTR) &lpMsgBuf,
		              0, NULL);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		AfxMessageBox((LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return FALSE;
	}
	if (readResult && bytes_read == 0)
	{
		AfxMessageBox(IDS_ERR_EOF);
		return FALSE;
	}
	if (!validMapFile(inBuffer))
	{
		AfxMessageBox(IDS_ERR_INVALID);
		return FALSE;
	}

	if (!createMap(inBuffer, &MasterMap))
	{
		AfxMessageBox(IDS_ERR_MAP_CREATE);
		return FALSE;
	}

	//now create a Game, using the map created above
	//only needed when opening a map with no game data yet, need to have dialog to user
	//to get details of the game
	createGame();
	
	//some alternative code can be used when opening a ColAus game file
	//this->DeleteContents();
	//this->Serialize(ar);
	//above should be overridden to ensure each data class of CColAusDoc is serialized?
	//also need an identifying variable to identify the file as being a ColAus game, and
	//also the version number
	//OnOpenDocument has been overridden for reading map files
	//need to either call base class function for CArchive processing, or else
	//create a new menu option which does the above, e.g. select map from file
	//dialog in new game dialog, with one option being 'auto generate', and parameters
	//could then be supplied
	//CDocument::OnOpenDocument(lpszFileName);

	return TRUE;
}

BOOL CColAusDoc::validMapFile(char *inBuffer)
{
	char mapfilePrefix[6] = {0x3A, 0x00, 0x48, 0x00, 0x04, 0x00};

	for (int i = 0; i < 6; i++)
		if (mapfilePrefix[i] != inBuffer[i])
			return FALSE;
	return TRUE;
}

BOOL CColAusDoc::createMap(char *inBuffer, Map *theMap)
{
	theMap->initMap();
	if (theMap->createMap(inBuffer))
		return TRUE;
	else
		return FALSE;
}

BOOL CColAusDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if (first_time)
	{	
		first_time = false;

		midiJukeBox.ShuffleJukeBox(0);
		testOutFileDoc << "Current Directory for jukebox:" << midiJukeBox.s_CurDir.GetBuffer(50);
		testOutFileDoc << "File Name:" << midiJukeBox.getSongTitle(0);

		return TRUE;
	}
	
	NewGameDlg newGameDialog;
	HBITMAP bm_handle = NULL;

	newGameDialog.updateBitmaps(0, bm_handle);

	newGameDialog.DoModal();

	MasterMap.initMap();
	MasterMap.randomMap(100, 100, 75, 75, 50, 70, 85, 80, 60);

	createGame();

	return TRUE;
}

void CColAusDoc::createGame(void)
{
	//NB. Assumes that the MasterMap has already been created, 
	//either randomly, or by opening a map file
	gameStats.year = 1750;
	gameStats.month = 1;
	gameStats.active_player = 0;
	gameStats.start_of_turn = true;

	//much of the below should be obtained via a dialog box
	gameStats.num_players = 4;

	playerStats[0].country = 0;
	playerStats[1].country = 1;
	playerStats[2].country = 2;
	playerStats[3].country = 3;

	playerStats[1].add2Gold(100);
	playerStats[2].add2Gold(200);
	playerStats[3].add2Gold(300);

	playerStats[0].updateName("Human_Player_0");
	playerStats[1].updateName("Comp_Player_1");
	playerStats[2].updateName("Human_Player_2");
	playerStats[3].updateName("Comp_Player_3");

	playerStats[0].setPlayerType(false);
	playerStats[1].setPlayerType(true);
	playerStats[2].setPlayerType(false);
	playerStats[3].setPlayerType(true);

	playerStats[1].setAIAlgorithm(0);
	playerStats[3].setAIAlgorithm(0);

	//Initialise Player Maps and the Piece List
	for (int i = 0; i < gameStats.num_players; i++)
		PlayerMap[i].initMap();
	MasterList.initPieces();
	displayMoves.setNumPlayers(gameStats.num_players);

	//create initial piece(s)s for each player
	CPoint start_pos;
	for (int i = 0; i < gameStats.num_players; i++)
	{
		start_pos = RuleBase.getStartPos(playerStats[i].country);
		start_pos.x = ((MasterMap.MAP_WIDTH * start_pos.x) / 100);
		start_pos.y = ((MasterMap.MAP_HEIGHT * start_pos.y) / 100);
		if (start_pos.x >= MasterMap.MAP_WIDTH)
			start_pos.x--;
		if (start_pos.y >= MasterMap.MAP_HEIGHT)
			start_pos.y--;
		int carrier = MasterList.addPiece(i, 4, start_pos.x, start_pos.y);
		int passenger1 = MasterList.addPiece(i, 1);//no location, as on 1st ship
		MasterList.addPiecetoCargo(carrier, passenger1);
	}

	//Create some extra pieces, to make testing easier
	start_pos = RuleBase.getStartPos(0);
	start_pos.x = ((MasterMap.MAP_WIDTH * start_pos.x) / 100);
	start_pos.y = ((MasterMap.MAP_HEIGHT * start_pos.y) / 100);
	if (start_pos.x >= MasterMap.MAP_WIDTH)
		start_pos.x--;
	if (start_pos.y >= MasterMap.MAP_HEIGHT)
		start_pos.y--;
	for (int i = gameStats.num_players; i < (gameStats.num_players + 12); i++)
	{
		//start_pos.x--;
		start_pos.y--;
		MasterList.addPiece(i % gameStats.num_players, 4, start_pos.x, start_pos.y);
		MasterList.addPiece(i % gameStats.num_players, 3, start_pos.x - 2, start_pos.y);
		MasterList.addPiece(i % gameStats.num_players, 2, start_pos.x - 4, start_pos.y);
	}

	//not sure if any of this is needed.....?
	int active_piece = MasterList.GetNextPiece(gameStats.active_player);
	MasterList.UpdateActive(active_piece);

	//update MasterMap with pieces from the PieceList
	int i_x, i_y;
	i_x = i_y = 0;
	for (int i = 0; i < LIST_SIZE; i++)
	{
		MasterList.GetPosition(i, i_x, i_y);
		//if piece on map && piece not being carried
		if ( (MasterMap.u_boundsCheck(i_x, i_y))
		&&   (MasterList.getCarrier(i) == NO_PIECE) )
			MasterMap.Land[i_x][i_y].piece = i;
	}

	//prepare each player's map
	for (int i = 0; i < gameStats.num_players; i++)
	{
		//copy master map to each of the Player maps
		PlayerMap[i] = MasterMap;
		//update player maps to reflect initial explored positions
		updateAllExplored(i);
		refreshRecentlySeen(i);
		MasterList.ResetMoves(i);
		playerStats[i].initDiplStatus();
	}

	//intialise movelist
	this->displayMoves.initMoves();

	current_game = true;
}

Map *CColAusDoc::getActiveMap(void)
{
	int active_player = gameStats.getActivePlayer();

	if (active_player >= 0)
		return &(PlayerMap[active_player]);
	else
		return NULL;
}

char *CColAusDoc::getPlayerAdj(int player_number)
{
	int country = playerStats[player_number].country;
	return RuleBase.getAdjCountry(country);
}

void CColAusDoc::updateAllExplored(int player)
{
	int i_x, i_y;
	i_x = i_y = 0;
	CPoint i_loc;
	int line_of_sight;
	for (int i = 0; i < LIST_SIZE; i++)
	{
		if (MasterList.GetOwner(i) == player) 
		{
			MasterList.GetPosition(i, i_x, i_y);
			i_loc.SetPoint(i_x, i_y);
			line_of_sight = MasterList.GetLoS(i);
			PlayerMap[player].updateExplored(i_loc, line_of_sight);
		}
	}
}

// CColAusDoc serialization

void CColAusDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
	MasterMap.Serialize(ar);
	for (int i = 0; i < MAX_PLAYERS; i++)
		PlayerMap[i].Serialize(ar);
	displayMoves.Serialize(ar);
	MasterList.Serialize(ar);
	gameStats.Serialize(ar);
	for (int i = 0; i < MAX_PLAYERS; i++)
		playerStats[i].Serialize(ar);
}


// CColAusDoc diagnostics

#ifdef _DEBUG
void CColAusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColAusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CColAusDoc::synchPlayerMaps()
{
	//copy master map moves to each of the Player maps
	for (int i = 0; i < gameStats.num_players; i++)
		PlayerMap[i].copyMoves(MasterMap);
}

void CColAusDoc::synchMasterMap(int player)
{
	//propagates a player's moves to the master map
	MasterMap.copyMoves(PlayerMap[player]);
}

void CColAusDoc::endTurn(void)
{
	int cur_owner = gameStats.getActivePlayer();
	//int act_piece;

	//ensure no active piece remains flashing
	//MasterList.UpdateActive(EMPTY);

	//copy current player's moves to MasterMap
	synchMasterMap(cur_owner);

	//reclaim old slots on PieceList
	MasterList.reclaimPieceSlots(gameStats.getDateInt());
	//reclaim old slots on MoveList
	displayMoves.reclaimMoves();

	//wait for next player to confirm that he is ready 
	ColMsgBox msgDlg;

	msgDlg.message_str = "That is the end of ";
	msgDlg.message_str += playerStats[cur_owner].getName();
	msgDlg.message_str += "'s turn.";
	msgDlg.DoModal();
	
	//message handler to do this "OnStartNewTurn"?
	cur_owner = gameStats.nextPlayer();

	//loop to display all unseen moves (updating player's map as we proceed?)
	//this will be a ViewBased function......

	//copy moves on updated master map to all player's maps
	//This is not required, at least until moves are displayed to player
	//It should not be required, as all moves are added to the player's map
	//synchPlayerMaps();

	//Changes to the terrain should be propagated to the MasterMap immediately,
	//as each move the terrain is synchronised with the MasterMap...
	

	MasterList.ResetMoves(cur_owner);
	MasterList.UpdateActive(EMPTY);

	//TODO:don't need these to, as the view "StartMove" functoinality can do it
	//act_piece = MasterList.GetNextPiece(cur_owner);
	//MasterList.UpdateActive(act_piece);

	//refresh 'recently_seen' status of map for current player
	//do this before updating player's map, and update it if any pieces/colonies are lost
	refreshRecentlySeen(cur_owner);

	//animate moves on MoveList the player has not seen yet
	//.....updating MoveList as we go

	//housekeeping - reclaim space on movelist

	//triggers "StartMove" functionality on next execution of "OnDraw()"
	gameStats.start_of_turn = true;

	//this-Invalidate(FALSE);
}

void CColAusDoc::refreshRecentlySeen(int cur_owner)
{
	//reset all squares to recently_seen = false, then
	//loop through each players piece (later colonies) and update 
	//based on individual line of sight for each
	PlayerMap[cur_owner].initRecentlySeen();
	int i, line_of_sight, act_piece;
	CPoint i_loc;
	act_piece = MasterList.loopNextPiece(cur_owner, 0);
	//temporary code, replace with actual line of sight for this piece, FIXED
	i_loc = MasterList.GetPosition(act_piece); 
	line_of_sight = MasterList.GetLoS(act_piece);
	PlayerMap[cur_owner].updateRecentlySeen(i_loc, line_of_sight);
	//line_of_sight = MasterList.GetLoS(act_piece);
	for (i = act_piece; 
		 i >= 0; 
		 i = MasterList.loopNextPiece(cur_owner, i))
	{
		i_loc = MasterList.GetPosition(i); 
		line_of_sight = MasterList.GetLoS(i);
		PlayerMap[cur_owner].updateRecentlySeen(i_loc, line_of_sight);
	}
}

int CColAusDoc::addPiece(int new_piece_owner,
						 int new_piece_type,
						 CPoint new_loc)
{
	//owner should be validated by the Doc class
	//piece_type should be validated by PieceList class
	//location should be validated by Doc.Map class
	int new_piece_slot;
	if (new_piece_owner >= gameStats.num_players)
		return -1;
		
	new_piece_slot = MasterList.GetNextEmpty();
	if (new_piece_slot == -1)
		return -1;

	if (!MasterList.IsValidPieceType(new_piece_type))
		return -1;

	//Check map location is legal, and empty
	if (!(MasterMap.u_boundsCheck(new_loc.x, new_loc.y))
	||  (PlayerMap[new_piece_owner].Land[new_loc.x][new_loc.y].piece != EMPTY))
		return -1;

	bool sea_square = MasterMap.isSeaSquare(new_loc.x, new_loc.y);

	if ((sea_square)
	&&  (!MasterList.seaType(new_piece_type)))
		return -1;

	if ((!sea_square)
	&&  (MasterList.seaType(new_piece_type)))
		return -1;

	//add the piece to the list of pieces
	new_piece_slot = MasterList.addPiece(new_piece_owner,
	      			                     new_piece_type,
				       	                 new_loc.x,
					                     new_loc.y);
	//update master map with the added piece
	MasterMap.Land[new_loc.x][new_loc.y].piece = new_piece_slot;
	//update currently active map with the added piece
	PlayerMap[gameStats.active_player].Land[new_loc.x][new_loc.y].piece = new_piece_slot;

	//update the current view if this piece belongs to the current player
	if (new_piece_owner == gameStats.active_player)
	{
		int los = MasterList.GetLoS(new_piece_slot);
		PlayerMap[new_piece_owner].updateRecentlySeen(new_loc, los);
		PlayerMap[new_piece_owner].updateExplored(new_loc, los);
	}

	//update the MoveList
	displayMoves.addMoveInfo(new_loc, new_loc, new_piece_slot, new_piece_owner, EMERGE); 

	return new_piece_slot;
}

// CColAusDoc commands


void CColAusDoc::OnTestSynch()
{
	synchPlayerMaps();
}
