#pragma once

const int EMPTY        = -1;
const int CARAVEL      = 0;
const int SCOUT        = 1;
const int HARBINGER    = 2;
const int ENDEAVOUR    = 4;

const int LIST_SIZE = 1000;
const int MAX_PIECE_TYPES = 100;
const int MAX_CARGO = 10;

const int MAX_ROUTE_LENGTH = 11;

struct cargoHold{
	//either piece or cargo is EMPTY
	int piece;
	int cargo;
	//quantity is only applicable to cargo
	int quantity;
};

class Piece : public CObject
{
public:
	Piece(void);
	~Piece(void);
	void UpdatePiece(int passed_type, int passed_owner, CPoint position);
	void initPiece(void);
	int getAttStr(void);
	int getDefStr(void);
	void Serialize(CArchive &ar);
	void setSleepStatus(int);
	int isAsleep(void);
	void setPatrol(CPoint destination);
	bool onPatrol();
	void initRoute(bool empty_both = true);
	void initOldRoute(void);
	int getRoutePtr(void);
	bool setNextRoutePt(CPoint);
	CPoint getNextRoutePt(void);
	bool hasRoute(void);
	bool circRoute(void);
	void copyToOldRoute(void);
	CPoint getPrevPos(void);
	bool posInRoute(CPoint pos);
	CPoint peekRoutePoint(int pt_number);
	void setStartPoint(CPoint);

	//data items to be saved
	int piece_type;
	int piece_owner;
	int piece_moves;		//moves remaining for this piece in the current turn	
	int piece_terrain;		//type of terrain piece moves on - LAND or SEA
	
	//Possibly a struct with offensive, defensive, bonus values?	
	int att_strength;		//initially from attributes, but modifiable for individual piece
	int def_strength;
	CPoint piece_pos;
	CPoint prev_pos;
	CPoint destination;		//may need to be a list of destinations, to facilitate navigation
	//patrol points
	CPoint patrol_point1;
	CPoint patrol_point2;
	int dest_patrol_point;	//the point to travel to
	bool on_patrol;

	cargoHold cargo_holds[MAX_CARGO];
	int carried_by;

	char direction;			//LEFT or RIGHT
	int killed;				//if non-zero, then represents the 'date' the piece was killed
	int asleep;

	CPoint route[MAX_ROUTE_LENGTH];
	CPoint old_route[MAX_ROUTE_LENGTH];
	int route_start_ptr;
	int route_end_ptr;

protected:
	DECLARE_SERIAL(Piece)
};

class PieceList : public CObject
{
public:
	//constructors/destructors/initialisers
	PieceList(void);
	~PieceList(void);
	void initAttributes(void);
	void initPieces(void);

	//access funtions
	void GetPosition(int piece, int &x, int &y);
	CPoint GetPosition(int piece);
	int GetActive(void);
	int GetMoves(int piece);
	int IsEmpty(int piece_number);
	int GetOwner(int piece_number);
	int GetLoS(int piece_number);
	int GetNextEmpty();
	bool IsValidPieceType(int type);
	bool seaType(int type);
	bool isShip(int piece_number);
	char getTagType(int piece_number);

	//utility functions
	int FindPiece(int &owner, int x, int y);
	bool ValidPieceNum(int);

	//update functions
	void ResetMoves(int owner);
	int ReduceMoves(int piece, int moves);
	void ZeroMoves(int piece);
	int GetNextPiece(int owner);
	int loopNextPiece(int current_owner, int current_piece);
	void UpdateActive(int new_active);
	void SetPosition(int piece, int x, int y);
	int addPiece(int owner, int type, int x = -1, int y = -1);
	bool killPiece(int piece_number, int turn_number);
	int capturePiece(int captor, int captured_piece, int current_date);
	void initPieceAttributes(int count);
	int addPiecetoCargo(int carrier, int passenger);
	int getPieceCargo(int carrier, int hold_nbr);
	bool removePieceCargo(int carrier, int removed_piece);
	int getCarrier(int passenger);
	int getPieceType(int piece_nbr);
	int getAvailPieceHolds(int dest_piece);
	char *getName(int attacker);
	int IsKilled(int piece_number);
	void reclaimPieceSlots(int current_date);
	void setDestination(int piece, CPoint dest);
	CPoint getDestination(int piece);
	CPoint getPatrolPoint(int piece, int pt_number);
	void removePatrol(int piece);
	CPoint getDestPatrolPoint(int piece);
	void reverseDestPatrolPoint(int piece);
	bool onPatrol(int piece);
	bool hasDestination(int piece);
	void removeDestination(int piece);
	char getPieceDir(int piece_number);
	void setPieceDir(int piece_number, char piece_dir);
	void updateDir(int piece_number, int old_x, int new_x);
	void disembarkPiece(int piece_nbr);
	void putToSleep(int piece_nbr);
	void wakeOnBoard(int piece_nbr);

	//overrides
	void Serialize(CArchive &ar);

	//data items to be saved with each game
	Piece List_of_pieces[LIST_SIZE];
	int list_number;		//		(not used currently)
	int active_piece;
	int allocated_pieces;	//		(not used currently)
	int static list_count;	//		(not used currently)

	//fixed or default data items - same for everygame
	//these are the values attributable to a generic 'type' of piece
	struct Attributes{
		char* name;
		int   move_type;
		//attack, defense, bonuses....(access function based on passed parameters)
		int   att_strength;
		int   def_strength;
		int   moves;
		int   line_of_sight;
		int   num_cargo_holds;		
	};
	Attributes Piece_attributes[MAX_PIECE_TYPES];
protected:
	DECLARE_SERIAL(PieceList)
};