#pragma once

//#include "Paint1Doc.h"

const int MAX_PLAYERS = 6;

class GameStats : public CObject
{
public:
	GameStats(void);
	GameStats(int pmonth, int pyear, int pplayer, int pnumplayers);
	~GameStats(void);
	void initTables(void);
	void Serialize(CArchive &ar);

	//update functions
	void incrementDate(void);
	int nextPlayer(void);

	//display/access functions
	char *getMonthtxt(void);
	int getActivePlayer(void);
	int getDateInt(void);

	//data members specific to a given game
	int month;
	int year;
	int active_player;
	int num_players;
	//play/graphics options
	bool auto_end_turn;
	int res_selection;
	bool start_of_turn;
	bool animate_piece_moves;

	//constant data members that don't need to be saved
	char *month_lit[12];

protected:
	DECLARE_SERIAL(GameStats)
};

class PlayerStats : public CObject
{
public:
	//constructors/destructors/initialisation
	PlayerStats(void);
	~PlayerStats(void);
	void Serialize(CArchive &ar);
	void initDiplStatus(void);

	//update functions
	void updateName(char *);
	void add2Gold(int);
	void declareWarWith(int);
	void setPlayerType(bool computer_player);
	void setAIAlgorithm(int algorithm);

	//display/access functions
	char *getName(void);
	int getGold(void);
	bool atWarWith(int);
	bool isComputerPlayer(void);
	int getAIAlgorithm(void);

	//data members to be saved for each game
	int number;
	int country;
	char *name;
	int gold;
	int DiplomacyStatus[MAX_PLAYERS];
	bool computer_player;
	int ai_algorithm;

protected:
	DECLARE_SERIAL(PlayerStats)
};

