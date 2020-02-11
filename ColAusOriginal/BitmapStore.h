#pragma once

class BitmapStore
{
public:
	BitmapStore(void);
	~BitmapStore(void);

	//utility functions
	CPoint getMapSquareSize(void);
	int getMapFrameSize(void);
	CPoint getTagLoc(int piece_graphic_no, char direction);
	int getColor(char *color_name);
	int getTagCharOffset(char tag_char);
	int getPieceDirOffset(char tag_char);

	//status variables
	int return_value;

	//Map bitmaps	
	CBitmap mBitmap1;	//used to indicate the size of each map square
	CBitmap mBmSea;

	CBitmap mBmLand_terrain_types;
	CBitmap Terr_blend_mask;

	CBitmap mountains;
	CBitmap mountains_mask;

	CBitmap hills;
	CBitmap hills_mask;

	CBitmap forests;
	CBitmap forests_mask;

	CBitmap mnrivers;
	CBitmap mnrivers_ocean;
	CBitmap mnrivers_mask;

	CBitmap undiscovered;
	CBitmap undisc_mask;

	CBitmap coast;
	CBitmap coast_mask;

	CBitmap coast_river;
	CBitmap coast_river_mask;

	CBitmap pieces;
	CBitmap pieces_mask;
	CBitmap pieces_colors;
	CBitmap pieces_big;
	CBitmap pieces_big_mask;

	CBitmap tag_text;

	//background bitmaps
	CBitmap background;
	CBitmap artwork[10];
	CBitmap jansson_map;
};
