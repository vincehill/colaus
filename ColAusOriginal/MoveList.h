#pragma once

//maximum values constants
const int MAX_MOVES = 1000;
const int MAX_PLAYERSM = 10;

//constants for different types of moves/events
const int MOVE    = 1;
const int ATTACK  = 2;
const int CAPTURE = 3;
const int VICTORY = 4;	//losing piece is destroyed
const int DEFEAT  = 5;	//winning piece is destroyed
const int DISBAND = 6;
const int EMERGE  = 7;

//actual structure of each move
struct sMove{
	bool active;
	CPoint from;
	CPoint to;
	int piece_number;
	int move_type;

	//0 indicates a player has not seen the move yet
	int seen_by[MAX_PLAYERSM];
};

class CMoveList : public CObject
{
public:
	CMoveList(void);
	CMoveList(int);
	~CMoveList(void);
	void Serialize(CArchive &ar);

	//data members
	int next_ptr;
	int display_ptr[MAX_PLAYERSM];
	sMove moveList[MAX_MOVES];
	int num_players;

	void initMoves(void);
	void initMove(int);
	void reclaimMoves(void);
	void initDisplayPtr(int player_num);
	bool setNumPlayers(int num_players);

	bool isActive(int move_num);
	//gets details on the specified move number
	void getMoveInfo(int move_number, CPoint &from , CPoint &to , int &piece, int &type);
	//gets the next move number
	int getNextMoveNumber(void);
	//get details on the next move to display - returns move number
	int getNextMoveToDisplay(int curr_player, 
					         CPoint &mfrom, CPoint &mto, 
				             int &piece, int &type);
	//returns the move number allocated to the new move added
	int addMoveInfo(CPoint from, CPoint to, 
		            int piece, int curr_player, int passed_move_type = MOVE);
	//updates values of the move specified
	bool setMoveInfo(int move_num, CPoint from, CPoint to, 
		             int piece, int curr_player, int move_type);
	//returns true if the specified player has seen a move
	bool seenByPlayer(int move_num, int player_num);
	//returns true if all players have seen the move
	bool seenByAll(int move_num);
	//mark a move as having been seen by a particular player
	void updateSeenBy(int move_num, int player_num);
	//mark all moves as having been seen by a particular player
	void updateAllSeenBy(int player_num);

protected:
	DECLARE_SERIAL(CMoveList)
};
