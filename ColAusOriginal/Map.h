#pragma once

//#include "BitmapStore.h"

const int MAX_WIDTH  = 200;
const int MAX_HEIGHT = 200;

const int SQ_WIDTH  = 48;
const int SQ_HEIGHT = 48;

//const int DISP_WIDTH  = 16;
//const int DISP_HEIGHT = 14;

const int NUM_RANDOM_TYPES  = 100;

const int NUM_TERRAIN_TYPES = 9;
const int TUNDRA = 0;
const int DESERT = 1;
const int PLAINS = 2;
const int PRAIRIE = 3;
const int GRASSLAND = 4;
const int SAVANNAH = 5;
const int MARSH = 6;
const int SWAMP = 7;
const int ICE = 8;


const int LAND = 0;
const int SEA  = 1;
//percentage value above which is snowy mountains
const int SNOWLINE = 95;
const int HILLLINE = 90;
const int SEALEVEL = 74;

const int NORIVER = 0;
const int MNRIVER = 1;
const int MJRIVER = 2;

const int NO_PIECE = -1;

class Mapsquare: public CObject
{
public:
	//constructors/destructor
	Mapsquare(void);
	Mapsquare(int);
	~Mapsquare(void);

	//fuctions
	void init(void);
	void copyTerrain(Mapsquare *sourceSquare);

	//data members
	int terrain;
	int land_terrain_type;
	int random_number;
	int altitude;
	bool explored;
	bool recently_seen;  
	bool forest;
	int river;
	int resource;
	int piece;

	//CObject overridden functions
	void Serialize(CArchive &ar);

protected:
	DECLARE_SERIAL(Mapsquare)
};

/*-----------------------------------------------------------------------------------*/

class Map: public CObject
{
public:
	Map(void);
	~Map(void);

	//paramaterised map creation (might replace int's with a struct later)
	int randomMap(int size_x,
		          int size_y,
			      int temperature,
			      int rainfall,
			      int cont_size,
			      int altitude,
			      int land_points,
				  int forest,
				  int fertility);
	BOOL createMap(char *inBuffer);
	BOOL populateSquare(int i, int j, char);

	//map creationn routines
	void initLandTerrainTypes(void);
	void initAltitudes(void);
	void initForests(void);
	void removeDiagonals(void);
	
	//initialisation routines
	void initMessages(void);
	void initTables(void);
	
	//utility routines
	const Map &operator=(const Map &);
	int updateDisplaySize(int disp_x, int disp_y);
	void initMap(void);

	//player routines
	void updateExplored(CPoint loc, int line_of_sight);
	void Map::initRecentlySeen(void);
	void updateRecentlySeen(CPoint loc, int line_of_sight);
	void synchMapTerrain(Map *MasterMap, CPoint new_loc, int line_of_sight);

	//map creation utilitiy functions
	int coastalScore(int x, int y);
	int u_mountScore(int x, int y);
	int u_mountSquare(int x, int y);
	int u_forestScore(int x, int y);
	int u_riversScore(int x, int y);
	int u_hillScore(int x, int y);
	int u_mthillScore(int x, int y);
	int u_landcount(int x, int y, int radius = 1);
	int u_terrCount(int x, int y, int terr);
	int u_seaCount(int x, int y, int radius);
	int u_riverCount(int x, int y, int radius);
	int u_hillCount(int x, int y, int radius);
	int u_forestCount(int x, int y, int radius);
	bool u_boundsCheck(int x, int y);
	void u_boundsTrim(CPoint &);
	void u_boundsTrim(CPoint &pt, CRect area);
	void u_extendRiver(int i, int j);
	void u_extendRiver2(int i, int j);
	void u_extendRiver3(int i, int j, int length);
	void u_varyEdges(int x);
	//area based map generation functions
	void seedlandArea(CRect area, int density_pct);
	void createlandArea(CRect area, int land_size);
	void removeLand(CRect area, int probability);
	void reduceMtDensity(CRect area, int probability);
	void reduceMtNumber(CRect area, int probability);
	void increaseMtNumber(CRect area, int probability, int cluster);

	//access functions
	bool isSeaSquare(int x, int y);
	bool hasRiver(int x, int y);
	int  getPiece(int x, int y);
	bool isRecentlySeen(int x, int y);
	bool mapMoveOk(CPoint old_loc, CPoint new_loc);

	int  processMove(UINT nChar, int &x, int &y);

	//update functions
	void copyMoves(Map &right);
	void updatePiecePresence(int, int, int, int);
	int  addPiecetoSquare(int active_piece, CPoint new_loc);
	int  removePiecefromSquare(CPoint new_loc);

	//CObject overridden functions
	void Serialize(CArchive &ar);

	//data members that should be serialized
	Mapsquare Land[MAX_WIDTH][MAX_HEIGHT];
	int MAP_WIDTH;
	int MAP_HEIGHT;

	//constant data members that are always the same
	int coast_table[16];
	int mount_table[16];
	int forest_table[16];
	int rivers_table[16];

	//error handling
	char *Messages[100];
	int active_messages[100];

	int MAP_DISP_WIDTH;
	int MAP_DISP_HEIGHT;
	int	mtemp;
	int mrainfall;
	int mcont_size;
	int maltitude;
	int mforest;
	int mfertility;
	int mland_points;

	BOOL move_key_pressed;

protected:
	DECLARE_SERIAL(Map)
};


