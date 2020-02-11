#include "StdAfx.h"
#include "gamestats.h"

IMPLEMENT_SERIAL(GameStats, CObject, 0)

GameStats::GameStats(void)
{
	month = 1;
	year = 1750;
	active_player = 0;
	num_players = 4;
	initTables();
	auto_end_turn = true;
	res_selection = 0;
	start_of_turn = true;
	animate_piece_moves = true;
}

GameStats::GameStats(int pmonth, int pyear, int pplayer, int pnumplayers)
{
	month = pmonth;
	year = pyear;
	active_player = pplayer;
	num_players = pnumplayers;
	initTables();
	auto_end_turn = true;
	res_selection = 0;
	animate_piece_moves = true;
}

GameStats::~GameStats(void)
{
}

void GameStats::initTables(void)
{
	month_lit[0]  = "Jan";
	month_lit[1]  = "Feb";
	month_lit[2]  = "Mar";
	month_lit[3]  = "Apr";
	month_lit[4]  = "May";
	month_lit[5]  = "Jun";
	month_lit[6]  = "Jul";
	month_lit[7]  = "Aug";
	month_lit[8]  = "Sep";
	month_lit[9]  = "Oct";
	month_lit[10] = "Nov";
	month_lit[11] = "Dec";

}

void GameStats::incrementDate(void)
{
	if (month == 12)
	{
		month = 1;
		year++;
	}
	else
		month++;
}

char *GameStats::getMonthtxt(void)
{
	return month_lit[month - 1];
}

int GameStats::getDateInt(void)
{
	return ((year * 12) + month);
}

int GameStats::nextPlayer(void)
{
	//returns the number of the next player, and makes this player active
	active_player++;
	if (active_player >= num_players)
	{
		active_player = 0;
		incrementDate();
	}
	return active_player;
}

int GameStats::getActivePlayer(void)
{
	return active_player;
}

void GameStats::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << month << year;
		ar << active_player << num_players;
		ar << auto_end_turn << res_selection << start_of_turn << animate_piece_moves;
	}
	else
	{
		ar >> month >> year;
		ar >> active_player >> num_players;
		ar >> auto_end_turn >> res_selection >> start_of_turn >> animate_piece_moves;

	}
}

//-------------------PlayerStats----------------------------------

IMPLEMENT_SERIAL(PlayerStats, CObject, 0)

PlayerStats::PlayerStats(void)
{
	gold = 0;
	name = "Default";
	number = 1;
	initDiplStatus();
	computer_player = false;
	ai_algorithm = -1;
}

PlayerStats::~PlayerStats(void)
{
	
}

void PlayerStats::initDiplStatus(void)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
		DiplomacyStatus[i] = 1;
}

bool PlayerStats::atWarWith(int opponent)
{
	if (DiplomacyStatus[opponent] == 1)
		return false;
	else
		return true;
}

void PlayerStats::declareWarWith(int opponent)
{
	DiplomacyStatus[opponent] = 0;
}

char *PlayerStats::getName(void)
{
	return name;
}

int PlayerStats::getGold(void)
{
	return gold;
}

void PlayerStats::updateName(char *new_name)
{
	name = new_name;
}

void PlayerStats::add2Gold(int amount)
{
	gold += amount;
}

void PlayerStats::Serialize(CArchive &ar)
{
	CString *name_string;
	name_string = new CString;

	if (ar.IsStoring())
	{
		ar << number;
		ar << country;
		*name_string += name;
		ar << *name_string; 
		ar << gold;
		for (int i = 0; i < MAX_PLAYERS; i++)
			ar << DiplomacyStatus[i];
		ar << ai_algorithm;
		ar << computer_player;
	}
	else
	{
		ar >> number;
		ar >> country;
		ar >> *name_string;
		name = name_string->GetBuffer();
		ar >> gold;
		for (int i = 0; i < MAX_PLAYERS; i++)
			ar >> DiplomacyStatus[i];
		ar >> ai_algorithm;
		ar >> computer_player;
	}
}

bool PlayerStats::isComputerPlayer(void)
{
	return computer_player;
}

int PlayerStats::getAIAlgorithm(void)
{
	return ai_algorithm;
}

void PlayerStats::setPlayerType(bool passed_comp_player)
{
	computer_player = passed_comp_player;
}
	
void PlayerStats::setAIAlgorithm(int passed_algorithm)
{
	ai_algorithm = passed_algorithm;
}
