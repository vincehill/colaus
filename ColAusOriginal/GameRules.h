#pragma once
//maximum number of different countries able to be catered for
const int MAX_COUNTRIES = 5;

class GameRules
{
public:
	GameRules(void);
	~GameRules(void);

	//access members
	CPoint getStartPos(int country);
	char *getAdjCountry(int country);

private:
	struct countriesRuleBase{
		char* name_noun;
		char* name_adjt;
		//(Percentage value, not actual map coords)
		CPoint starting_location;
	};
	int number_of_countries;
	countriesRuleBase CountryRules[MAX_COUNTRIES];
};
