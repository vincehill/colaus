#include "StdAfx.h"
#include "movelist.h"

IMPLEMENT_SERIAL(CMoveList, CObject, 0)

CMoveList::CMoveList(int number_of_players = 10)
{
	next_ptr = 0;
	if  (num_players <= MAX_PLAYERSM)
		num_players = number_of_players;
	else
		num_players = -1;
	initMoves();
	for (int i = 0; i < MAX_PLAYERSM; i++)
		initDisplayPtr(i);
}

CMoveList::~CMoveList(void)
{
}

CMoveList::CMoveList(void)
{
	next_ptr = 0;
	num_players = MAX_PLAYERSM;
	initMoves();
	for (int i = 0; i < MAX_PLAYERSM; i++)
		initDisplayPtr(i);
}

void CMoveList::initMoves(void)
{
	for (int i = 0; i < MAX_MOVES; i++)
		initMove(i);
	next_ptr = 0;
}

void CMoveList::initMove(int move_num)
{
	moveList[move_num].active = false;
	moveList[move_num].from.SetPoint(-1, -1);
	moveList[move_num].to.SetPoint(-1, -1);
	moveList[move_num].piece_number = -1;
	for (int i = 0; i < num_players; i++)
		moveList[move_num].seen_by[i] = FALSE;
}

void CMoveList::reclaimMoves(void)
{
	for (int i = 0; i < MAX_MOVES; i++)
		if  (seenByAll(i))
			initMove(i);
}

bool CMoveList::seenByPlayer(int move_num, int player_num)
{
	if (moveList[move_num].seen_by[player_num])
		return true;
	else
		return false;
}

bool CMoveList::seenByAll(int move_num)
{
	for (int i = 0; i < num_players; i++)
		if  (!moveList[move_num].seen_by[i])
			return false;
	return true;
}

int CMoveList::addMoveInfo(CPoint from, CPoint to, 
						   int piece_num, int curr_player, 
						   int passed_move_type)
{
	//add a move number to the MoveList, with the values specified
	//returns 0 to indicate there was a problem, otherwise returns the number of the move
	if  (curr_player >= num_players)
		return 0;
	int current_move = getNextMoveNumber();
	if (current_move < 0)
		return -1;
	if (setMoveInfo(current_move, from, to, piece_num, curr_player, passed_move_type))
	{
		updateSeenBy(current_move, curr_player);
		return current_move;
	}
	else
		return -1;
}

bool CMoveList::setMoveInfo(int move_num, 
							CPoint move_from, 
							CPoint move_to, 
							int piece, int curr_player, int passed_move_type)
{
	if  ((move_num < 0) || (move_num >= MAX_MOVES))
		return false;
	if  ((curr_player < 0) || (curr_player > num_players))
		return false;

	moveList[move_num].active = true;
	moveList[move_num].from = move_from;
	moveList[move_num].to = move_to;
	moveList[move_num].piece_number = piece;
	moveList[move_num].move_type = passed_move_type;

	for (int i = 0; i < num_players; i++)
		moveList[move_num].seen_by[i] = FALSE;

	return true;
}

//TODO:generalise so this is passed a boolean function, and returns the first move meeting the
//criteria in the function (e.g. 'bool isActive(int i)' or bool beenDisplayed(int i, int player)...
int CMoveList::getNextMoveNumber(void)
{
	//gets the next free slot for storing details about a move
	int starting_loc = next_ptr;
	
	for (int i = next_ptr; i < (MAX_MOVES - 1); i++)
		if  (!isActive(i))
		{
			next_ptr = i;
			return next_ptr;
		}

	//if we reach here then we need to start looking for moves at the beginning again
	for (int i = 0; i < starting_loc; i++)
		if  (!isActive(i))
		{
			next_ptr = i;
			return next_ptr;
		}

	//indicates there are no inactive slots available at all.....
	return -1;
}

void CMoveList::initDisplayPtr(int curr_player)
{
	display_ptr[curr_player] = 0;
}
	
int CMoveList::getNextMoveToDisplay(int curr_player, 
							        CPoint &mfrom, CPoint &mto, 
							        int &piece, int &type)
{
	//get details on the next move to display - returns move number

	if  ((curr_player < 0) || (curr_player >= num_players))
		return -1;

	int starting_loc = display_ptr[curr_player];
	//loop through moves, looking for the next one the player has not seen
	for (int i = display_ptr[curr_player]; i < MAX_MOVES; i++)
	{
		if  ( (!seenByPlayer(i, curr_player))
		&&    (isActive(i)) )
		{
			display_ptr[curr_player] = i;
			getMoveInfo(display_ptr[curr_player], mfrom, mto, piece, type);
			updateSeenBy(display_ptr[curr_player], curr_player);
			return display_ptr[curr_player];
		}
	}

	for (int i = 0; i < starting_loc; i++)
	{
		if  ( (!seenByPlayer(i, curr_player))
		&&    (isActive(i) ) )
		{
			display_ptr[curr_player] = i;
			getMoveInfo(display_ptr[curr_player], mfrom, mto, piece, type);
			updateSeenBy(display_ptr[curr_player], curr_player);
			return display_ptr[curr_player];
		}
	}

	return -1;
}

void CMoveList::updateAllSeenBy(int player_num)
{
	//mark all moves as having been seen by a particular player
	if  ((player_num < 0) || (player_num >= num_players))
		return;

	for (int i = 0; i < MAX_MOVES; i++)
	{
		if  ( (!seenByPlayer(i, player_num))
		&&    (isActive(i)) )
		{
			display_ptr[player_num] = i;
			//getMoveInfo(display_ptr[curr_player], mfrom, mto, piece, type);
			updateSeenBy(i, player_num);
		}
	}
	return;	
}

void CMoveList::getMoveInfo(int move_num, CPoint &mfrom, CPoint &mto, int &piece, int &type)
{
	mfrom = moveList[move_num].from;
	mto = moveList[move_num].to;
	piece = moveList[move_num].piece_number;
	type = moveList[move_num].move_type;
}

void CMoveList::updateSeenBy(int move_num, int player_num)
{
	moveList[move_num].seen_by[player_num] = TRUE;
}

bool CMoveList::isActive(int move_num)
{
	return moveList[move_num].active;
}

bool CMoveList::setNumPlayers(int num_playing)
{
	if ((num_playing < 0) || (num_playing > MAX_PLAYERSM))
		return false;
	else
		num_players = num_playing;
	return true;
}

void CMoveList::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << next_ptr << num_players;
		for (int i = 0; i < MAX_PLAYERSM; i++)
			ar << display_ptr[i];
		for (int i = 0; i < MAX_MOVES; i++)
		{
			ar << moveList[i].active;
			ar << moveList[i].from.x;
			ar << moveList[i].from.y;
			ar << moveList[i].move_type;
			ar << moveList[i].piece_number;
			for (int j = 0; j < MAX_PLAYERSM; j++)
				ar << moveList[i].seen_by[j];
			ar << moveList[i].to.x;
			ar << moveList[i].to.y;
		}
	}
	else
	{
		ar >> next_ptr >> num_players;
		for (int i = 0; i < MAX_PLAYERSM; i++)
			ar >> display_ptr[i];
		for (int i = 0; i < MAX_MOVES; i++)
		{
			ar >> moveList[i].active;
			ar >> moveList[i].from.x;
			ar >> moveList[i].from.y;
			ar >> moveList[i].move_type;
			ar >> moveList[i].piece_number;
			for (int j = 0; j < MAX_PLAYERSM; j++)
				ar >> moveList[i].seen_by[j];
			ar >> moveList[i].to.x;
			ar >> moveList[i].to.y;
		}
	}
}