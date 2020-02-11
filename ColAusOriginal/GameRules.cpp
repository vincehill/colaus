#include "StdAfx.h"
#include "gamerules.h"

GameRules::GameRules(void)
{
	//initialise Game Rules
	//(may eventually read them from a text file
	number_of_countries = 5;

	CountryRules[0].name_noun = "England";
	CountryRules[0].name_adjt = "English";
	CountryRules[0].starting_location.SetPoint(100, 50);
	CountryRules[1].name_noun = "Holland";
	CountryRules[1].name_adjt = "Dutch";
	CountryRules[1].starting_location.SetPoint(0, 10);
	CountryRules[2].name_noun = "France";
	CountryRules[2].name_adjt = "French";
	CountryRules[2].starting_location.SetPoint(0, 50);
	CountryRules[3].name_noun = "Spain";
	CountryRules[3].name_adjt = "Spanish";
	CountryRules[3].starting_location.SetPoint(0, 5);
	CountryRules[4].name_noun = "Portugal";
	CountryRules[4].name_adjt = "Portugese";
	CountryRules[4].starting_location.SetPoint(0, 15);
}

GameRules::~GameRules(void)
{
}

CPoint GameRules::getStartPos(int country)
{
	return CountryRules[country].starting_location;
}

char *GameRules::getAdjCountry(int country)
{
	return CountryRules[country].name_adjt;
}