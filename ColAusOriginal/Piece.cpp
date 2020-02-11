#include "StdAfx.h"
#include "piece.h"
#include "map.h"

IMPLEMENT_SERIAL(Piece, CObject, 0)

Piece::Piece(void)
{
	piece_type = EMPTY;
	piece_owner = EMPTY;
	piece_moves = EMPTY;
	piece_terrain = EMPTY;
	att_strength = EMPTY;
	def_strength = EMPTY;
	piece_pos.x = -1;
	piece_pos.y = -1;
	prev_pos.SetPoint(-1,-1);
	killed = -1;
	direction = 'L';
	asleep = 0;
	on_patrol = false;
	initRoute();
}

void Piece::UpdatePiece(int type, int owner, CPoint pos)
{
	piece_type = type;
	piece_owner = owner;
	piece_pos = pos;
}

void Piece::initPiece(void)
{
	piece_type = -1;
	piece_owner = EMPTY;
	piece_moves = -1;
	piece_terrain = -1;
	att_strength = -1;
	def_strength = -1;
	piece_pos.x = piece_pos.y = -1;
	prev_pos.x = prev_pos.y = -1;
	carried_by = -1;
	destination.x = destination.y = -1;
	patrol_point1.x = patrol_point1.y = -1;
	patrol_point2.x = patrol_point2.y = -1;
	dest_patrol_point = 0;
	on_patrol = false;

	//blank out all possible cargo holds
	for (int i = 0; i < MAX_CARGO; i++)
	{
		this->cargo_holds[i].cargo = EMPTY;
		this->cargo_holds[i].piece = EMPTY;
		this->cargo_holds[i].quantity = 0;
	}
	killed = 0;
	direction = 'L';
	asleep = 0;
	initRoute();
}

int Piece::getAttStr(void)
{
	return att_strength;
}

int Piece::getDefStr(void)
{
	return def_strength;
}


Piece::~Piece(void)
{
}

void Piece::Serialize(CArchive &ar)
{
	if  (ar.IsStoring())
	{
		ar << att_strength;		
		ar << def_strength;
		ar << piece_pos.x << piece_pos.y;
		ar << destination.x << destination.y;	
		ar << patrol_point1.x << patrol_point1.y;
		ar << patrol_point2.x << patrol_point2.y;
		ar << dest_patrol_point;
		ar << on_patrol;
		ar << carried_by << direction << killed;	
		for (int i = 0; i < MAX_CARGO; i++)
		{
			ar << cargo_holds[i].cargo 
			   << cargo_holds[i].piece 
			   << cargo_holds[i].quantity;
		}
		ar << piece_type << piece_owner << piece_moves << piece_terrain;
		ar << asleep;		
	}
	else
	{
		ar >> att_strength;		
		ar >> def_strength;
		ar >> piece_pos.x >> piece_pos.y;
		ar >> destination.x >> destination.y;	
		ar >> patrol_point1.x >> patrol_point1.y;
		ar >> patrol_point2.x >> patrol_point2.y;
		ar >> dest_patrol_point;
		ar >> on_patrol;
		ar >> carried_by >> direction >> killed;	
		for (int i = 0; i < MAX_CARGO; i++)
			ar >> cargo_holds[i].cargo 
			   >> cargo_holds[i].piece 
			   >> cargo_holds[i].quantity;
		ar >> piece_type >> piece_owner >> piece_moves >> piece_terrain;
		ar >> asleep;
	}
}

void Piece::setSleepStatus(int piece_status)
{
	asleep = piece_status;
}

int Piece::isAsleep(void)
{
	return asleep;
}

void Piece::setPatrol(CPoint dest)
{
	patrol_point1.SetPoint(piece_pos.x, piece_pos.y);
	patrol_point2.SetPoint(dest.x, dest.y);
	dest_patrol_point = 2;
	on_patrol = true;
	initRoute();
}

bool Piece::onPatrol()
{
	return on_patrol;
}

void Piece::initRoute(bool empty_both)
{
	if (empty_both)
		initOldRoute();		
	else
		copyToOldRoute();

	for (int i = 0; i < MAX_ROUTE_LENGTH; i++)
	{
		route[i].SetPoint(-1,-1);
	}

	route_start_ptr = -1;
	route_end_ptr = -1;
}

void Piece::initOldRoute(void)
{
	for (int i = 0; i < MAX_ROUTE_LENGTH; i++)
		old_route[i].SetPoint(-1,-1);
}

void Piece::copyToOldRoute(void)
{
	for (int i = 0; i < MAX_ROUTE_LENGTH; i++)
		old_route[i].SetPoint(route[i].x,route[i].y);
}

int Piece::getRoutePtr(void)
{
	return route_start_ptr;
}

bool Piece::hasRoute(void)
{
	if ( (route_start_ptr >= 1)
	&&   (route_start_ptr < route_end_ptr)
	&&   (route_start_ptr < MAX_ROUTE_LENGTH - 1) )
		return true;
	else
		return false;
}

bool Piece::setNextRoutePt(CPoint new_point)
{
	if (route_end_ptr < MAX_ROUTE_LENGTH - 1)
	{
		if (route_start_ptr < 1)
			route_start_ptr = 1;
		if (route_end_ptr < 1)
			route_end_ptr = 1;
		route[route_end_ptr].SetPoint(new_point.x,new_point.y);
		route_end_ptr++;
		return true;
	}
	else
		return false;
}

CPoint Piece::getNextRoutePt(void)
{
	CPoint return_pt(-1,-1);

	if ( (route_start_ptr < MAX_ROUTE_LENGTH) 
	&&   (route_start_ptr >= 1)
	&&   (route_start_ptr < route_end_ptr) )
	{
		return_pt = route[route_start_ptr];
		route_start_ptr++;
	}

	return return_pt;
}

CPoint Piece::peekRoutePoint(int pt_number)
{
	CPoint err_pt(-1,-1);

	if ((pt_number < 0) || (pt_number > MAX_ROUTE_LENGTH))
		return err_pt;
	else
		return route[pt_number];
}

void Piece::setStartPoint(CPoint start_pt)
{
	route[0].SetPoint(start_pt.x,start_pt.y);
}

bool Piece::circRoute(void)
{
	//return true if there is at least one shared non-null point between the
	//new route and the old one
	CPoint null_pt(-1,-1);

	if (!(hasRoute()))
		return false;

	for (int i = 0; i < MAX_ROUTE_LENGTH; i++)
		for (int j = 1; j < MAX_ROUTE_LENGTH; j++)
			if  ( (old_route[i] == route[j])
			&&    (route[j] != null_pt) )
				return true;

	return false;
}

bool Piece::posInRoute(CPoint pos)
{
	//returns true if the passed point is in the current route list
	if (!(hasRoute()))
		return false;

	for (int i = 0; i < MAX_ROUTE_LENGTH; i++)
		if (route[i] == pos)
			return true;

	return false;
}

CPoint Piece::getPrevPos(void)
{
	return prev_pos;
}

/*-----------------Piece List-----------------------------------*/

IMPLEMENT_SERIAL(PieceList, CObject, 0)

int PieceList::list_count = 0;

PieceList::PieceList(void)
{
	list_number = list_count;
	list_count++;

	active_piece = -1;
	initAttributes();
}

PieceList::~PieceList(void)
{
}

void PieceList::initAttributes()
{
	//initialise the attributes of each generic type of piece
	for (int count = 0; count < MAX_PIECE_TYPES; count++)
	{
		Piece_attributes[count].name = "Unallocated";
		Piece_attributes[count].move_type = -1;
		Piece_attributes[count].moves = -1;
		Piece_attributes[count].att_strength = -1;
		Piece_attributes[count].def_strength = -1;
		Piece_attributes[count].line_of_sight = -1;
		Piece_attributes[count].num_cargo_holds  = 0;
	}

	//sets values for each 'type' of piece
	Piece_attributes[0].name = "Caravel";
	Piece_attributes[0].move_type = SEA;
	Piece_attributes[0].moves = 120;
	Piece_attributes[0].att_strength = 0;
	Piece_attributes[0].def_strength = 2;
	Piece_attributes[0].line_of_sight = 1;
	Piece_attributes[0].num_cargo_holds = 1;

	Piece_attributes[1].name = "Scout";
	Piece_attributes[1].move_type = LAND;
	Piece_attributes[1].moves = 6;
	Piece_attributes[1].att_strength = 1;
	Piece_attributes[1].def_strength = 1;
	Piece_attributes[1].line_of_sight = 2;
	Piece_attributes[1].num_cargo_holds = 0;

	Piece_attributes[2].name = "Transport";
	Piece_attributes[2].move_type = SEA;
	Piece_attributes[2].moves = 18;
	Piece_attributes[2].att_strength = 0;
	Piece_attributes[2].def_strength = 4;
	Piece_attributes[2].line_of_sight = 2;
	Piece_attributes[2].num_cargo_holds = 8;

	Piece_attributes[3].name = "Trader";
	Piece_attributes[3].move_type = SEA;
	Piece_attributes[3].moves = 30;
	Piece_attributes[3].att_strength = 0;
	Piece_attributes[3].def_strength = 2;
	Piece_attributes[3].line_of_sight = 2;
	Piece_attributes[3].num_cargo_holds = 2;

	Piece_attributes[4].name = "Brig";
	Piece_attributes[4].move_type = SEA;
	Piece_attributes[4].moves = 24;
	Piece_attributes[4].att_strength = 2;
	Piece_attributes[4].def_strength = 3;
	Piece_attributes[4].line_of_sight = 2;
	Piece_attributes[4].num_cargo_holds = 2;

	Piece_attributes[5].name = "Frigate";
	Piece_attributes[5].move_type = SEA;
	Piece_attributes[5].moves = 24;
	Piece_attributes[5].att_strength = 4;
	Piece_attributes[5].def_strength = 6;
	Piece_attributes[5].line_of_sight = 2;
	Piece_attributes[5].num_cargo_holds = 4;

	allocated_pieces = 4;

}

bool PieceList::IsValidPieceType(int type)
{	
	if (Piece_attributes[type].move_type == -1)
		return false;
	else
		return true;
}

bool PieceList::seaType(int type)
{	
	if (Piece_attributes[type].move_type == SEA)
		return true;
	else
		return false;
}

bool PieceList::isShip(int piece_number)
{
	int this_piece_type = List_of_pieces[piece_number].piece_type;

	return seaType(this_piece_type);
}

void PieceList::SetPosition(int piece, int x, int y)
{
	if (ValidPieceNum(piece))
	{
		List_of_pieces[piece].prev_pos.x = List_of_pieces[piece].piece_pos.x;
		List_of_pieces[piece].prev_pos.y = List_of_pieces[piece].piece_pos.y;
		List_of_pieces[piece].piece_pos.x = x;
		List_of_pieces[piece].piece_pos.y = y;
	}
}

void PieceList::GetPosition(int piece, int &x, int &y)
{
	if (!ValidPieceNum(piece))
	{
		x = y = -1;
	}
	else
	{
		x = List_of_pieces[piece].piece_pos.x;
		y = List_of_pieces[piece].piece_pos.y;
	}
	//if this piece is being carried by another, then return carrier's location
	int carrier = List_of_pieces[piece].carried_by;
	if  (carrier != EMPTY)
	{
		x = List_of_pieces[carrier].piece_pos.x;
		y = List_of_pieces[carrier].piece_pos.y;
	}
}

CPoint PieceList::GetPosition(int piece)
{
	CPoint temp;
	if (!ValidPieceNum(piece))
	{
		temp.x = temp.y = -1;
		return temp;
	}
	//if this piece is being carried by another, then return carrier's location
	int carrier = List_of_pieces[piece].carried_by;
	if  (carrier != EMPTY)
	{
		return List_of_pieces[carrier].piece_pos;
	}
	else
		return List_of_pieces[piece].piece_pos;
}

int PieceList::GetActive()
{
	return active_piece;
}

int PieceList::GetMoves(int piece)
{
	//returns the number of moves the piece has this turn
	if (!ValidPieceNum(piece))
		return 0;
	else
		return List_of_pieces[piece].piece_moves;
}

int PieceList::FindPiece(int &owner, int x, int y)
{
	int count;
	for (count = 0; count < LIST_SIZE; count++)
		if (   (List_of_pieces[count].piece_owner != EMPTY)
			&& (List_of_pieces[count].piece_pos.x == x)
			&& (List_of_pieces[count].piece_pos.y == y))
			{
				owner = List_of_pieces[count].piece_owner;
				return count;
			}
	return EMPTY;
}

void PieceList::ResetMoves(int owner)
{
	//resets to max move points all pieces for this owner
	for (int count = 0; count < LIST_SIZE; count++)
		if ( (owner == List_of_pieces[count].piece_owner)
		//don't reset moves for killed pieces
		&&   (!IsKilled(count)) )
			List_of_pieces[count].piece_moves = 
			    Piece_attributes[List_of_pieces[count].piece_type].moves;
}

void PieceList::initPieceAttributes(int piece_number)
{
	//resets all attributes of an individual piece to the default
	//values for that piece type
	List_of_pieces[piece_number].piece_moves = 
		Piece_attributes[List_of_pieces[piece_number].piece_type].moves;
	List_of_pieces[piece_number].att_strength = 
		Piece_attributes[List_of_pieces[piece_number].piece_type].att_strength;
	List_of_pieces[piece_number].def_strength = 
		Piece_attributes[List_of_pieces[piece_number].piece_type].def_strength;
	List_of_pieces[piece_number].piece_terrain = 
		Piece_attributes[List_of_pieces[piece_number].piece_type].move_type;
}

int PieceList::ReduceMoves(int piece, int moves)
{
	if (!ValidPieceNum(piece))
		return 0;

	if (List_of_pieces[piece].piece_moves >= moves)
	{
		List_of_pieces[piece].piece_moves -= moves;
		if (List_of_pieces[piece].piece_moves < 0)
			List_of_pieces[piece].piece_moves = 0;
		return 1;
	}
	
	return 0;
}

void PieceList::ZeroMoves(int piece)
{
	//sets to zero the number of moves remaining to the piece
	if (ValidPieceNum(piece))
		List_of_pieces[piece].piece_moves = 0;
}

int PieceList::GetNextPiece(int current_owner)
{
	//returns the next piece available to move for this owner
	//-1 is returned if there are no pieces left with available moves
	int starting_place = active_piece;
	for (int i = (active_piece + 1); i < LIST_SIZE; i++)
		if ( (List_of_pieces[i].piece_owner == current_owner) 
		&&   (List_of_pieces[i].piece_moves > 0) 
		&&   (!IsKilled(i)) 
		&&   (!List_of_pieces[i].isAsleep()) )
			return i;
	if  (active_piece > -1)
		for (int i = 0; i <= active_piece; i++)
			if ( (List_of_pieces[i].piece_owner == current_owner) 
			&&   (List_of_pieces[i].piece_moves > 0) 
			&&   (!IsKilled(i)) 
			&&   (!List_of_pieces[i].isAsleep()) )
				return i;
	return -1;
}

int PieceList::addPiecetoCargo(int carrier, int passenger)
{
	//returns 0 if the piece was added okay
	//returns 1 if either of the pieces does not exist(?)
	//returns 2 if this piece cannot carry cargo
	//returns 3 if there are not cargo spaces available
	//returns 4 if either piece reference is invalid

	if ( (!ValidPieceNum(carrier))
	||   (!ValidPieceNum(passenger)) )
		return 4;

	//return error if either piece does not exist
	if ( (IsEmpty(carrier))
	||   (IsEmpty(passenger)) )
		return 1;
	//return error if the carrier is not able to carry cargo
	int carrier_type = List_of_pieces[carrier].piece_type;
	int nbr_holds = Piece_attributes[carrier_type].num_cargo_holds;
	if (nbr_holds == 0)
		return 2;
	//find next available cargo hold
	int next_hold = -1;
	for (int i = 0; i < nbr_holds; i++)
		if ( (List_of_pieces[carrier].cargo_holds[i].piece == EMPTY)
		&&   (List_of_pieces[carrier].cargo_holds[i].cargo == EMPTY) )
		{
		    next_hold = i;
			i = nbr_holds + 1;
		}
	//return error if there are no empty holds available
	if (next_hold == -1)
		return 3;
	//all validation passed, so all aboard!
	List_of_pieces[carrier].cargo_holds[next_hold].piece = passenger;
	List_of_pieces[passenger].carried_by = carrier;
	//return 'OK' status
	return 0;
}

int PieceList::getAvailPieceHolds(int dest_piece)
{
	//returns the number of cargo holds availale
	//returns  0 if the piece has no spare holds
	//returns -1 if the piece does not exist(?)
	//returns -2 if this piece cannot carry Piece cargo

	//return error if either piece does not exist
	if  ( (IsEmpty(dest_piece))
	||    (!ValidPieceNum(dest_piece)) )
		return -1;
	//return error if the carrier is not able to carry cargo
	int carrier_type = List_of_pieces[dest_piece].piece_type;
	int nbr_holds = Piece_attributes[carrier_type].num_cargo_holds;
	if (nbr_holds == 0)
		return -2;
	//find next available cargo hold
	int empty_counter = 0;
	for (int i = 0; i < nbr_holds; i++)
		if ( (List_of_pieces[dest_piece].cargo_holds[i].piece == EMPTY)
		&&   (List_of_pieces[dest_piece].cargo_holds[i].cargo == EMPTY) )
		{
		    empty_counter++;
		}
	return empty_counter;
}

void PieceList::disembarkPiece(int piece_nbr)
{
	//return if validation fails
	if (IsEmpty(piece_nbr))
		return;

	if (!ValidPieceNum(piece_nbr))
		return;

	int carrier = List_of_pieces[piece_nbr].carried_by;
	List_of_pieces[piece_nbr].carried_by = NO_PIECE;

	if (IsEmpty(carrier))
		return;
	else
	{
		//update carrier so the cargo hold becomes vacant
		//replace with getCargoHolds(carrier);
		int piece_type = getPieceType(carrier);
		int nbr_holds = Piece_attributes[piece_type].num_cargo_holds;
		for (int i = 0; (i < nbr_holds); i++)
			if  (List_of_pieces[carrier].cargo_holds[i].piece == piece_nbr)
			{
				List_of_pieces[carrier].cargo_holds[i].piece = NO_PIECE;
				i = nbr_holds + 1;
			}
	}
}

int PieceList::getCarrier(int passenger)
{
	//returns -1 if this piece is not being carried by another
	//otherwise returns the piece number of the carrier
	if (!ValidPieceNum(passenger))
		return -1;
	else
		return List_of_pieces[passenger].carried_by;
}

int PieceList::getPieceType(int piece_nbr)
{
	if (ValidPieceNum(piece_nbr))
		return List_of_pieces[piece_nbr].piece_type;
	else
		return -1;
}

int PieceList::getPieceCargo(int carrier, int hold_nbr)
{
	//returns -1 if the piece does not exist
	//returns -2 if the piece is not of a type that holds cargo
	//returns -3 if there is no cargo in this hold
	//returns -4 if there is cargo, but it is not a piece
	//otherwise returns the piece number of the carried piece

	//return error if the piece does not exist or the reference is invalid
	if  ( (IsEmpty(carrier))
	||    (!ValidPieceNum(carrier)) )
		return -1;
	//return error if the piece has been killed
	if  (IsKilled(carrier))
		return -5;
	//return error if the carrier is not able to carry cargo
	int carrier_type = List_of_pieces[carrier].piece_type;
	int nbr_holds = Piece_attributes[carrier_type].num_cargo_holds;
	if (nbr_holds == 0)
		return -2;
	//return error if there is no cargo in the hold, or it is not a piece
	if (List_of_pieces[carrier].cargo_holds[hold_nbr].piece == EMPTY)
        if (List_of_pieces[carrier].cargo_holds[hold_nbr].cargo == 0) 
			return -3;
		else
			return -4;
	//validation passed, so return the piece number in the hold
	return List_of_pieces[carrier].cargo_holds[hold_nbr].piece;
}

int PieceList::loopNextPiece(int current_owner, int current_piece)
{
	//returns the next piece belonging to this owner
	//-1 is returned if there are no pieces left 
	int starting_place;

	if (current_piece > LIST_SIZE)
		return -1;
	else
		starting_place = current_piece;

	for (int i = (starting_place + 1); i < LIST_SIZE; i++)
		if ( (List_of_pieces[i].piece_owner == current_owner) 
		&&   (!IsKilled(i)) )
			return i;
	return -1;
}

void PieceList::UpdateActive(int new_active)
{
	active_piece = new_active;
	//wake the piece up
	if (ValidPieceNum(active_piece))
		List_of_pieces[new_active].setSleepStatus(FALSE);
}

int PieceList::IsEmpty(int piece_number)
{
	if (List_of_pieces[piece_number].piece_owner == EMPTY)
		return 1;
	else
		return 0;
}

int PieceList::IsKilled(int piece_number)
{
	return List_of_pieces[piece_number].killed;
}

int PieceList::GetOwner(int piece_number)
{
	if (ValidPieceNum(piece_number))
		return List_of_pieces[piece_number].piece_owner;
	else
		return -1;
}

bool PieceList::ValidPieceNum(int piece_num)
{
	if ((piece_num >= 0) && (piece_num < LIST_SIZE))
		return true;
	else
		return false;
}

int PieceList::GetLoS(int piece_number)
{
	if (ValidPieceNum(piece_number))
	{
		int p_type = List_of_pieces[piece_number].piece_type;
		return Piece_attributes[p_type].line_of_sight;
	}
	else
		return -1;
}

int PieceList::addPiece(int owner, int type, int x, int y)
{
	//creates a new piece, with default location at -1, -1, i.e. off the map board
	//returns the PieceList index, or -1 if there are no spare spots
	bool spare_slot_found = false;
	CPoint position;
	position.x = x;
	position.y = y;
	int count;
	//bug: piece with slot of 'zero' is not being displayed...until a legal move
	for (count = 1; 
		((count < LIST_SIZE) && (!spare_slot_found)); 
		 count++)
		if (List_of_pieces[count].piece_owner == EMPTY)
		{
			List_of_pieces[count].initPiece();
			List_of_pieces[count].UpdatePiece(type, owner, position);
			initPieceAttributes(count);
			List_of_pieces[count].killed = 0;
			spare_slot_found = true;
		}
	if (spare_slot_found)
		return (count - 1);
	else
		return -1;
}

bool PieceList::killPiece(int victim, int current_date)
{
	//removes a piece from the list, returns false if the piece requested does not exist

	if ( (List_of_pieces[victim].piece_owner == EMPTY)
	||   (!ValidPieceNum(victim)) )
		return false;

	//any pieces being carried also are killed instantaneously (drowned if on ship)
	for (int i = 0; i < MAX_CARGO; i++)
	{
		int cargo_piece = getPieceCargo(victim, i);
		if (cargo_piece > 0)
			List_of_pieces[cargo_piece].initPiece();
	}
	//if the piece is being carried, then remove from carrier
	int carrier = getCarrier(victim);
	removePieceCargo(carrier, victim);

	//attributes are not initialised until piece has been killed for a whole turn
	//this allows it to be displayed to other players if required
	//List_of_pieces[victim].initPiece();
	List_of_pieces[victim].killed = current_date;

	return true;
}

bool PieceList::removePieceCargo(int carrier, int removed_piece)
{
	if ( (!ValidPieceNum(carrier))
	||   (!ValidPieceNum(removed_piece)) )
		return false;

	for (int i = 0; i < MAX_CARGO; i++)
	{
		int cargo_piece = getPieceCargo(carrier, i);
		if (cargo_piece == removed_piece)
		{
			List_of_pieces[carrier].cargo_holds[i].piece = NO_PIECE;
			return true;
		}
	}
	return false;
}

int PieceList::capturePiece(int captor, int captured_piece, int current_date)
{
	//copy the captured piece to a new slot, transfer cargo, 
	//then 'kill' the old captured piece
	//returns the new piece number for the captured piece

	if ( (!ValidPieceNum(captor))
	||   (!ValidPieceNum(captured_piece)) )
		return -1;

	CPoint pos;
	pos = GetPosition(captured_piece);
	int new_piece = addPiece(captor,
		                     getPieceType(captured_piece),
		                     pos.x, pos.y);

	//set moves to zero, att_strength to zero, def_strength to one
	List_of_pieces[new_piece].att_strength = 0;
	List_of_pieces[new_piece].def_strength = 1;
	ZeroMoves(new_piece);

	//update any pieces being carried also
	for (int i = 0; i < MAX_CARGO; i++)
	{
		int cargo_piece = getPieceCargo(captured_piece, i);
		if (cargo_piece > 0)
		{
			//generage new cargo piece, and then 'kill' old piece
			int new_cargo_piece = addPiece(captor, getPieceType(cargo_piece), -1, -1);
			addPiecetoCargo(new_piece, new_cargo_piece);
			List_of_pieces[cargo_piece].killed = current_date;

			//List_of_pieces[cargo_piece].piece_owner = captor;
			ZeroMoves(new_cargo_piece);
			//captured piece cannot attack until strength is replenished, by visiting
			//a port of the captor and being 'repaired'
			List_of_pieces[new_cargo_piece].att_strength = 0;
		}
	}

	List_of_pieces[captured_piece].killed = current_date;

	return new_piece;
}

void PieceList::initPieces(void)
{
	//initialises the PieceList structure prior to commencing a new game
	active_piece = -1;
	list_count = 1;
	list_number = 1;
	for (int i = 0; i < LIST_SIZE; i++)
		List_of_pieces[i].initPiece();
}

int PieceList::GetNextEmpty()
{
	//return the next available slot, or -1 if the list is full
	for (int i = 0; i < LIST_SIZE; i++)
		if (this->IsEmpty(i))
			return i;
	return -1;
}

void PieceList::reclaimPieceSlots(int current_date)
{
	int kill_date;
	for (int i = 0; i < LIST_SIZE; i++)
	{
		kill_date = IsKilled(i);
		if ( (kill_date != 0) 
		&&   ((current_date - kill_date) > 1) )
			List_of_pieces[i].initPiece();
	}
}

char *PieceList::getName(int attacker)
{
	int type = List_of_pieces[attacker].piece_type;
	return Piece_attributes[type].name;
}

void PieceList::setDestination(int piece, CPoint dest)
{
	if (ValidPieceNum(piece))
	{
		List_of_pieces[piece].destination = dest;
		List_of_pieces[piece].initRoute();
	}
	//List_of_pieces[piece].initRoute();
}
	
CPoint PieceList::getDestination(int piece)
{
	return List_of_pieces[piece].destination;
}

CPoint PieceList::getPatrolPoint(int piece, int pt_number)
{
	if (ValidPieceNum(piece))
	{
		if (pt_number == 1)
			return List_of_pieces[piece].patrol_point1;
		else
			return List_of_pieces[piece].patrol_point2;
	}
	CPoint error_point(-1, -1);
	return error_point;
}

bool PieceList::hasDestination(int piece)
{
	if ((List_of_pieces[piece].destination.x < 0)
	||  (List_of_pieces[piece].destination.y < 0))
		return false;
	else
		return true;
}

void PieceList::removeDestination(int piece)
{
	if  (ValidPieceNum(piece))
	{
		List_of_pieces[piece].destination.SetPoint(-1, -1);
		List_of_pieces[piece].initRoute();
	}
}

char PieceList::getPieceDir(int piece_number)
{
	return List_of_pieces[piece_number].direction;
}

void PieceList::setPieceDir(int piece_number, char piece_dir)
{
	if  (ValidPieceNum(piece_number))
		List_of_pieces[piece_number].direction = piece_dir;
}

void PieceList::updateDir(int piece_number, int old_x, int new_x)
{
	if (!ValidPieceNum(piece_number))
		return;

	//update orientation of piece
	int dir_offset = new_x - old_x;
	if (dir_offset > 0)
		setPieceDir(piece_number, 'R');
	else if (dir_offset < 0)
		setPieceDir(piece_number, 'L');
}

char PieceList::getTagType(int piece_number)
{
	if (!ValidPieceNum(piece_number))
		return 0;

	if (hasDestination(piece_number))
		return 'G';

	if (List_of_pieces[piece_number].isAsleep())
		return 'S';

	if (List_of_pieces[piece_number].onPatrol())
		return 'P';

	//if the piece has no status corresponding to a tag
	return 0;
}

void PieceList::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << list_number << active_piece << allocated_pieces << list_count;	
	}
	else
	{
		ar >> list_number >> active_piece >> allocated_pieces >> list_count;	
	}
	for (int i = 0; i < LIST_SIZE; i++)
		List_of_pieces[i].Serialize(ar);
}

void PieceList::putToSleep(int piece_nbr)
{
	if  (ValidPieceNum(piece_nbr))
		List_of_pieces[piece_nbr].asleep = TRUE;
}

void PieceList::wakeOnBoard(int piece_nbr)
{
	if (!ValidPieceNum(piece_nbr))
		return;

	//wake all pieces on board
	for (int i = 0; i < MAX_CARGO; i++)
	{
		int cargo_piece = getPieceCargo(piece_nbr, i);
		if (cargo_piece > 0)
			List_of_pieces[cargo_piece].setSleepStatus(FALSE);
	}
}

void PieceList::removePatrol(int piece_nbr)
{
	if (!ValidPieceNum(piece_nbr))
		return;

	List_of_pieces[piece_nbr].on_patrol = false;
	List_of_pieces[piece_nbr].patrol_point1.SetPoint(-1,-1);
	List_of_pieces[piece_nbr].patrol_point2.SetPoint(-1,-1);
	List_of_pieces[piece_nbr].dest_patrol_point = 0;
}

CPoint PieceList::getDestPatrolPoint(int piece_nbr)
{
	CPoint error_point(-1,-1);

	if (!ValidPieceNum(piece_nbr))
		return error_point;

	if  (List_of_pieces[piece_nbr].dest_patrol_point == 1)
		return List_of_pieces[piece_nbr].patrol_point1;
	else if (List_of_pieces[piece_nbr].dest_patrol_point == 2)
		return List_of_pieces[piece_nbr].patrol_point2;
	else
		return error_point;
}

void PieceList::reverseDestPatrolPoint(int piece_nbr)
{
	if (!ValidPieceNum(piece_nbr))
		return;
	
	if (!List_of_pieces[piece_nbr].onPatrol())
		return;

	if  (List_of_pieces[piece_nbr].dest_patrol_point == 1)
		List_of_pieces[piece_nbr].dest_patrol_point++;
	else
		List_of_pieces[piece_nbr].dest_patrol_point = 1;
}

bool PieceList::onPatrol(int piece_nbr)
{
	if (!ValidPieceNum(piece_nbr))
		return false;

	return List_of_pieces[piece_nbr].onPatrol();
}