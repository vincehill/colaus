#include "StdAfx.h"
#include "bitmapstore.h"
#include "Paint1.h"
#include "resource.h"

#include "Paint1Doc.h"
#include "Paint1View.h"

//BitmapStore ColGraphics;

BitmapStore::BitmapStore(void)
{
	return_value = 0;

	if ((!pieces.LoadBitmap(IDB_PIECES))
	||  (!pieces_mask.LoadBitmap(IDB_PIECES_MASK)) 
	||  (!pieces_colors.LoadBitmap(IDB_PIECES_COLORS)) )
	{
		return_value = 99;
	}

	if ((!undiscovered.LoadBitmap(IDB_UNDISCOVERED))
	||	(!undisc_mask.LoadBitmap(IDB_UNDISCOVERED_MASK)) )
	{
		return_value = 99;
	}


	if (!mBitmap1.LoadBitmap(IDB_LAND)) 
	{
		return_value = 99;
	}
	if (!mBmSea.LoadBitmap(IDB_SEA))
	{
		return_value = 99;
	}

	if  (!mBmLand_terrain_types.LoadBitmap(IDB_LAND_TERRAIN_TYPES))
		return_value = 99;


	if ((!coast.LoadBitmap(IDB_COASTAL))
	||	(!coast_mask.LoadBitmap(IDB_COASTAL_MASK)) )
	{
		return_value = 99;
	}
	if ((!coast_river.LoadBitmap(IDB_COASTAL_RIVER))
	||	(!coast_river_mask.LoadBitmap(IDB_COASTAL_RIVER_MASK)) )
	{
		return_value = 99;
	}
	if ((!mountains.LoadBitmap(IDB_MOUNTAINS))
	||	(!mountains_mask.LoadBitmap(IDB_MOUNTAINS_MASK)) )
	{
		return_value = 99;
	}
	if ((!hills.LoadBitmap(IDB_HILLS))
	||	(!hills_mask.LoadBitmap(IDB_HILLS_MASK)) )
	{
		return_value = 99;
	}
	if ((!forests.LoadBitmap(IDB_FORESTS))
	||	(!forests_mask.LoadBitmap(IDB_FORESTS_MASK)) )
	{
		return_value = 99;
	}
	if ((!mnrivers.LoadBitmap(IDB_MNRIVERS))
	||  (!mnrivers_ocean.LoadBitmap(IDB_MNRIVERS_OCEAN))
	||	(!mnrivers_mask.LoadBitmap(IDB_MNRIVERS_MASK)) )
	{
		return_value = 99;
	}
	if (!Terr_blend_mask.LoadBitmap(IDB_TERR_BLEND_MASK))
	{
		return_value = 99;
	}
	if (!background.LoadBitmap(IDB_BACKGROUND))
	{
		return_value = 99;
	}
	if (!jansson_map.LoadBitmap(IDB_JANSSON_MAP))
	{
		return_value = 99;
	}
	if (!tag_text.LoadBitmap(IDB_TAG_TEXT))
	{
		return_value = 99;
	}
	//load artworks into array
	if (!artwork[0].LoadBitmap(IDB_NEW_COLONY))
	{
		return_value = 99;
	}

}


BitmapStore::~BitmapStore(void)
{
}

CPoint BitmapStore::getMapSquareSize(void)
{
	CPoint square_size;
	BITMAP bm1;
	mBitmap1.GetObject(sizeof(BITMAP), &bm1);
	
	square_size.x = bm1.bmWidth;
	square_size.y = bm1.bmHeight;

	return square_size;
}

int BitmapStore::getMapFrameSize(void)
{
	CPoint square_size = getMapSquareSize();
	return square_size.x + 2;
}

CPoint BitmapStore::getTagLoc(int piece_graphic_no, char direction)
{
	char LEFT  = 'L';
	char RIGHT = 'R';
	CPoint tagLoc;
	switch (piece_graphic_no)
	{
		case 0:
			if (direction == LEFT)
				tagLoc.SetPoint(9, 10);
			else
				tagLoc.SetPoint(29, 10);
		break;
		case 1:
			if (direction == LEFT)
				tagLoc.SetPoint(9, 10);
			else
				tagLoc.SetPoint(29, 10);
		break;
		case 2:
			if (direction == LEFT)
				tagLoc.SetPoint(2, 14);
			else
				tagLoc.SetPoint(36, 14);
		break;
		case 3:
			if (direction == LEFT)
				tagLoc.SetPoint(35, 11);
			else
				tagLoc.SetPoint(3, 11);
		break;
		case 4:
			if (direction == LEFT)
				tagLoc.SetPoint(36, 17);
			else
				tagLoc.SetPoint(2, 17);
		break;
		if (direction == LEFT)
			tagLoc.SetPoint(3, 7);
		else
			tagLoc.SetPoint(35, 7);
		break;
	}
	return tagLoc;
}

int BitmapStore::getColor(char *color_name)
{
	if (color_name == "LTGREY")
			return 400;
	else if (color_name == "BLACK")
			return 450;
	else if (color_name == "DKGREY")
			return 500;
	else if (color_name == "WHITE")
			return 550;	
	else    
			return 0;
}

int BitmapStore::getTagCharOffset(char tag_char)
{
	char * text_string = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*-~?/b+|^$:&";

	//substitute the letter O for 0
	if (tag_char == '0')
		tag_char = 'O';

	for (int i = 0; i < 48; i++)
	{
		if (*(text_string+i) == tag_char)
			return i * 12;
	}
	return 37 * 12;
}

int BitmapStore::getPieceDirOffset(char tag_char)
{
	switch (tag_char)
	{
	case 'l': case 'L': 
		return 0;
	case 'r': case 'R':
		return 1;
	}
	return 0;
}
