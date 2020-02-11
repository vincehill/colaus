#include "map.h"
#include "Paint1.h"
#include "StdAfx.h"
#include "BitmapStore.h"
#include "Paint1Doc.h"
#include "Paint1View.h"

#define DEST_vh           (DWORD)0x00AA0029  /* dest = dest               */

IMPLEMENT_SERIAL(Map, CObject, 0)
IMPLEMENT_SERIAL(Mapsquare, CObject, 0)

Map::Map(void)
{
	initMessages();

	initTables();
	//initCoastal_table;
	coast_table[0]  = 0;
	coast_table[1]  = 250;
	coast_table[2]  = 200;
	coast_table[3]  = 100;
	coast_table[4]  = 350;
	coast_table[5]  = 600;
	coast_table[6]  = 50;
	coast_table[7]  = 550;
	coast_table[8]  = 300;
	coast_table[9]  = 0;
	coast_table[10] = 650;
	coast_table[11] = 400;
	coast_table[12] = 150;
	coast_table[13] = 450;
	coast_table[14] = 500;
	coast_table[15] = 700;

	//initialise mount_table;
	mount_table[0]  = 0;
	mount_table[1]  = 100;
	mount_table[2]  = 150;
	mount_table[3]  = 350;
	mount_table[4]  = 50;
	mount_table[5]  = 450;
	mount_table[6]  = 300;
	mount_table[7]  = 550;
	mount_table[8]  = 200;
	mount_table[9]  = 250;
	mount_table[10] = 500;
	mount_table[11] = 600;
	mount_table[12] = 400;
	mount_table[13] = 650;
	mount_table[14] = 700;
	mount_table[15] = 750;

	//initialise forest_table;
	forest_table[0]  = 0;
	forest_table[1]  = 100;
	forest_table[2]  = 150;
	forest_table[3]  = 350;
	forest_table[4]  = 50;
	forest_table[5]  = 450;
	forest_table[6]  = 300;
	forest_table[7]  = 550;
	forest_table[8]  = 200;
	forest_table[9]  = 250;
	forest_table[10] = 500;
	forest_table[11] = 600;
	forest_table[12] = 400;
	forest_table[13] = 650;
	forest_table[14] = 700;
	forest_table[15] = 750;

	//map drawing indicator, might not belong here
	move_key_pressed = true;
	MAP_WIDTH  = 50;
	MAP_HEIGHT = 50;
	updateDisplaySize(11, 11);

	randomMap(50, 50, 50, 50, 80, 50, 75, 50, 50);
}

int Map::updateDisplaySize(int disp_x, int disp_y)
{
	//check parameters passed are even numbers
	if ( (disp_y %2 == 0)      && (disp_y % 2 == 0) 
	&&   (disp_x <= MAP_WIDTH) && (disp_y <= MAP_HEIGHT) )
	{
		MAP_DISP_WIDTH  = disp_x;
		MAP_DISP_HEIGHT = disp_y;
		return 1;
	}
	else
		return 0;
}

BOOL Map::createMap(char *inBuffer)
{
	int buff_idx = 6;

	this->MAP_WIDTH = 56;
	this->MAP_HEIGHT  = 70;

	for (int j = 0; j < MAP_HEIGHT; j++)
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			buff_idx = 6 + ((j + 1) * 58) + (i + 1);
			if (this->u_boundsCheck(i, j))
				if (!populateSquare(i, j, inBuffer[buff_idx]))
					return FALSE;
			//buff_idx++;
		}
	return TRUE;
}

BOOL Map::populateSquare(int i, int j, char hex_value)
{
	/*
	struct MapByte {
		unsigned			  : 4;
		unsigned forrested    : 1;
		unsigned base_terrain : 3;
	};

	const char TUNDRA       = 0x00;		//0000 0000
	const char DESERT       = 0x01;		//0000 0001
	const char PLAINS       = 0x02;     //0000 0010
	const char PRAIRIE      = 0x03;     //0000 0011		
	const char GRASSLAND    = 0x04;     //0000 0100
	const char SAVANNAH     = 0x05;     //0000 0101
	const char MARSH        = 0x06;     //0000 0110
	const char SWAMP        = 0x07;     //0000 0111

	const char TUN_F        = 0x08;     //0000 1000
	const char DES_F        = 0x09;     //0000 1001
	const char PLA_F        = 0x0A;     //0000 1010
	const char PRA_F        = 0x0B;     //0000 1011
	const char GRA_F        = 0x0C;     //0000 1100
	const char SAV_F        = 0x0D;     //0000 1101
	const char MAR_F        = 0x0E;     //0000 1110
	const char SWA_F        = 0x0F;     //0000 1111

	//special cases
	const char ARCTIC       = 0x18;     //0001 1000
	const char OCEAN        = 0x19;     //0001 1001
	const char SEA_LANE     = 0x1A;     //0001 1010

	const char TUN_H        = 0x20;     //0010 0000
	const char DES_H        = 0x21;     //0010 0001
	const char PLA_H        = 0x22;     //0010 0010
	const char PRA_H        = 0x23;     //0010 0011
	const char GRA_H        = 0x24;     //0010 0100
	const char SAV_H        = 0x25;     //0010 0101
	const char MAR_H        = 0x26;     //0010 0110
	const char SWA_H        = 0x27;     //0010 0111

	const char ARC_H        = 0x38;     //0011 1000

	const char TUN_H_F      = 0x28;     //0010 1000
	const char DES_H_F      = 0x29;     //0010 1001
	const char PLA_H_F      = 0x2A;     //0010 1010
	const char PRA_H_F      = 0x2B;     //0010 1011
	const char GRA_H_F      = 0x2C;     //0010 1100
	const char SAV_H_F      = 0x2D;     //0010 1101
	const char MAR_H_F      = 0x2E;     //0010 1110
	const char SWA_H_F      = 0x2F;     //0010 1111

	const char TUN_H_R      = 0x60;     //0110 0000
	const char DES_H_R      = 0x61;     //0110 0001
	const char PLA_H_R      = 0x62;     //0110 0010
	const char PRA_H_R      = 0x63;     //0110 0011
	const char GRA_H_R      = 0x64;     //0110 0100
	const char SAV_H_R      = 0x65;     //0110 0101
	const char MAR_H_R      = 0x66;     //0110 0110
	const char SWA_H_R      = 0x67;     //0110 0111

	const char TUN_MN       = 0x40;     //0100 0000
	const char DES_MN       = 0x41;     //0100 0001
	const char PLA_MN       = 0x42;     //0100 0010
	const char PRA_MN       = 0x43;     //0100 0011
	const char GRA_MN       = 0x44;     //0100 0100
	const char SAV_MN       = 0x45;     //0100 0101
	const char MAR_MN       = 0x46;     //0100 0110
	const char SWA_MN       = 0x47;     //0100 0111

	const char ARC_MN       = 0x58;     //0101 1000

	const char TUN_MN_F     = 0x48;     //0100 1000
	const char DES_MN_F     = 0x49;     //0100 1001
	const char PLA_MN_F     = 0x4A;     //0100 1010
	const char PRA_MN_F     = 0x4B;     //0100 1011
	const char GRA_MN_F     = 0x4C;     //0100 1100
	const char SAV_MN_F     = 0x4D;     //0100 1101
	const char MAR_MN_F     = 0x4E;     //0100 1110
	const char SWA_MN_F     = 0x4F;     //0100 1111

	const char TUN_MT       = 0xA0;     //1010 0000
	const char DES_MT       = 0xA1;     //1010 0001
	const char PLA_MT       = 0xA2;     //1010 0010
	const char PRA_MT       = 0xA3;     //1010 0011
	const char GRA_MT       = 0xA4;     //1010 0100
	const char SAV_MT       = 0xA5;     //1010 0101
	const char MAR_MT       = 0xA6;     //1010 0110
	const char SWA_MT       = 0xA7;     //1010 0111

	const char ARC_MT       = 0xB8;     //1011 1000

	const char TUN_MT_F     = 0xA8;     //1010 1000
	const char DES_MT_F     = 0xA9;     //1010 1001
	const char PLA_MT_F     = 0xAA;     //1010 1010
	const char PRA_MT_F     = 0xAB;     //1010 1011
	const char GRA_MT_F     = 0xAC;     //1010 1100
	const char SAV_MT_F     = 0xAD;     //1010 1101
	const char MAR_MT_F     = 0xAE;     //1010 1110
	const char SWA_MT_F     = 0xAF;     //1010 1111

	const char TUN_MJ       = 0xC0;     //1100 0000
	const char DES_MJ       = 0xC1;     //1100 0001
	const char PLA_MJ       = 0xC2;     //1100 0010
	const char PRA_MJ       = 0xC3;     //1100 0011
	const char GRA_MJ       = 0xC4;     //1100 0100
	const char SAV_MJ       = 0xC5;     //1100 0101
	const char MAR_MJ       = 0xC6;     //1100 0110
	const char SWA_MJ       = 0xC7;     //1100 0111

	const char ARC_MJ       = 0xD8;     //1101 1000

	const char TUN_MJ_F     = 0xC8;     //1100 1000
	const char DES_MJ_F     = 0xC9;     //1100 1001
	const char PLA_MJ_F     = 0xCA;     //1100 1010
	const char PRA_MJ_F     = 0xCB;     //1100 1011
	const char GRA_MJ_F     = 0xCC;     //1100 1100
	const char SAV_MJ_F     = 0xCD;     //1100 1101
	const char MAR_MJ_F     = 0xCE;     //1100 1110
	const char SWA_MJ_F     = 0xCF;     //1100 1111
*/
	unsigned ARCTIC_MASK    = 0x18;		//0001 1000
	unsigned BASE_TERR_MASK = 0x07;     //0000 0111
	unsigned FOREST_MASK    = 0x08;		//0000 1000
	unsigned MJ_RIVER_MASK  = 0xC0;		//1100 0000
	unsigned MN_RIVER_MASK  = 0x40;		//0100 0000
	//unsigned MT_MASK        = 0xA0;		//1000 0000
	unsigned MASK_8_7       = 0xC0;		//1100 0000
	unsigned MT_PRESENT     = 0x80;		//1000 0000
	unsigned HILL_MASK      = 0x20;		//0010 0000
	unsigned OCEAN_MASK     = 0x19;

	const char OCEAN        = 0x19;     //0001 1001
	const char SEA_LANE     = 0x1A;     //0001 1010
	const char ARCTIC       = 0x18;

	int mask_result = hex_value & OCEAN_MASK;
	if ((mask_result == OCEAN) || (hex_value == SEA_LANE))
	{
		Land[i][j].altitude = 0;
		Land[i][j].terrain = SEA;
		Land[i][j].river = NORIVER;
		mask_result = hex_value & MASK_8_7;
		if (mask_result == MJ_RIVER_MASK)
			Land[i][j].river = MJRIVER;
		else
			if (mask_result == MN_RIVER_MASK)
				Land[i][j].river = MNRIVER;
		return TRUE;
	}
	Land[i][j].altitude = SEALEVEL + 1;
	if (ARCTIC == (hex_value & ARCTIC_MASK))
	{
		Land[i][j].terrain = LAND;
		Land[i][j].land_terrain_type = ICE;
	}
	else
	{
		int mask_result = hex_value & BASE_TERR_MASK; 

		Land[i][j].land_terrain_type = mask_result;
		Land[i][j].terrain = LAND;

		if (hex_value & FOREST_MASK)
			Land[i][j].forest = true;
	}

	Land[i][j].river = NORIVER;
	mask_result = hex_value & MASK_8_7;
	if (mask_result == MJ_RIVER_MASK)
		Land[i][j].river = MJRIVER;
	else
		if (mask_result == MN_RIVER_MASK)
			Land[i][j].river = MNRIVER;

	if (mask_result == MT_PRESENT)
		Land[i][j].altitude = SNOWLINE + 1;
	else
	{
		mask_result = hex_value & HILL_MASK;

		if (mask_result == HILL_MASK)
			Land[i][j].altitude = HILLLINE + 1;
	}
	
	return TRUE;
}

int Map::randomMap(int size_x,
				   int size_y,
				   int temperature,
				   int rainfall,
				   int cont_size,
				   int altitude,
				   int land_points,
				   int forest,
				   int fertility)
{
	int i, j, cycles, count, cont_x, cont_y;
	int terr_type;

	//validate values of the paramaters passed
	if ((size_x > MAX_WIDTH) || (size_y > MAX_HEIGHT)
	||  (size_x < 0) || (size_y < 0))
		return 5;

	size_x = (size_x / 10) * 10;
	size_y = (size_y / 10) * 10;
	MAP_WIDTH   = size_x; //+ 100;
	MAP_HEIGHT  = size_y; //+ 100;

	if ((temperature > 100) || (temperature  < 0)
	||  (rainfall    > 100) || (rainfall     < 0)
    ||  (cont_size   > 100) || (cont_size    < 0)
    ||  (altitude    > 100) || (altitude     < 0)
	||  (forest      > 100) || (forest       < 0)
    ||  (fertility   > 100) || (fertility    < 0)
	||  (land_points > 100) || (land_points  < 0))
		return 6;

	mtemp        = temperature;
	mrainfall    = rainfall;
	mcont_size   = cont_size;
	maltitude    = altitude;
	mforest      = forest;
	mfertility   = fertility;
	mland_points = land_points * (MAP_WIDTH + MAP_HEIGHT) / 100;

	//make proportional to temperature...
	int tropics   = ((MAP_HEIGHT * 3) / 10);
	int temperate = ((MAP_HEIGHT * 8) / 10);  
	int antarctic  = ((MAP_HEIGHT * 9) / 10);

	//set initial values
	for (i=0; i < MAP_WIDTH; i++)
		for (j=0; j < MAP_HEIGHT; j++)
		{
			Land[i][j].init();
		}

	//seed map with inital land points
		/*
	for (cycles = 0; cycles < mland_points; )
	{
		i = rand() % MAP_WIDTH;
		j = rand() % MAP_HEIGHT;

		if((Land[i][j].terrain != LAND))
		{
			Land[i][j].terrain  = LAND;
			Land[i][j].altitude = 100;
			cycles++;
		}
	}
		*/

	
	//put random lines of land points instead
	/*
	for (cycles = 0; cycles < mland_points / 5; cycles++)
	{
		CPoint start_pt;
		CPoint end_pt;
		int max_length = 20;

		start_pt.x = rand() % MAP_WIDTH;
		start_pt.y = rand() % MAP_HEIGHT;

		end_pt.x = (rand() % (max_length * 2)) - max_length + start_pt.x;
		end_pt.y = (rand() % (max_length * 2)) - max_length + start_pt.y;

		u_boundsTrim(end_pt);

		i = start_pt.x;
		j = start_pt.y;
		bool line_finished = false;

		for (;!line_finished;)
		{
			if (rand() % 5)
			{
				int x_adj = (rand() % 3) - 1 + i;
				int y_adj = (rand() % 3) - 1 + j;
				Land[x_adj][y_adj].terrain  = LAND;
				Land[x_adj][y_adj].altitude = 100;
			}

			if ((start_pt.x < end_pt.x)
			&&  (i < end_pt.x))
				if (rand() % 2)
					i++;
			if ((start_pt.x > end_pt.x)
			&&  (i > end_pt.x))
				if (rand() % 2)
					i--;
			if ((start_pt.y < end_pt.y)
			&&  (j < end_pt.y))
				if (rand() % 2)
					j++;
			if ((start_pt.y >= end_pt.y)
			&&  (j > end_pt.y))
				if (rand() % 2)
					j--;	

			if ((abs(i - end_pt.x) < (rand() % 5))
			&&  (abs(j - end_pt.y) < (rand() % 5)))
				line_finished = true;
		}
	}
	*/

	CRect area_all(0,0,MAP_WIDTH,MAP_HEIGHT);
	seedlandArea(area_all, 3);

	CRect area1, area2, area3, area4, area5;
	CRect area6, area7, area8, area9, area10;
	CRect area11;

	//continent area
	area1.SetRect(MAP_WIDTH / 4, MAP_HEIGHT / 4, MAP_WIDTH * 7/12, MAP_HEIGHT * 2/3);
	area6.SetRect(MAP_WIDTH * 1/2, MAP_HEIGHT * 9/24, MAP_WIDTH * 2/3, MAP_HEIGHT * 55/100);
	area7.SetRect(MAP_WIDTH * 4/10, MAP_HEIGHT * 60/100, MAP_WIDTH * 1/2, MAP_HEIGHT * 75/100);
	//medium islands area (indonesia/new guinea)
	area2.SetRect(0, 0, MAP_WIDTH / 2, MAP_HEIGHT / 5);
	//small islands area (new caledonia/tahiti)
	area3.SetRect(MAP_WIDTH * 2/3, MAP_HEIGHT / 5, MAP_WIDTH * 4/5, MAP_HEIGHT * 2/3);
	//large islands area (new zealand)
	area4.SetRect(MAP_WIDTH * 7/10, MAP_HEIGHT * 2/3, MAP_WIDTH * 87/100, MAP_HEIGHT * 72/100);
	area8.SetRect(MAP_WIDTH * 72/100, MAP_HEIGHT * 74/100, MAP_WIDTH * 93/100, MAP_HEIGHT * 82/100);
	//(almost) no islands area (indian and southern oceans)
	area5.SetRect(0, MAP_HEIGHT / 4, MAP_WIDTH * 1/5, MAP_HEIGHT * 9/10);
	area9.SetRect(0, MAP_HEIGHT * 80/100, MAP_WIDTH * 60/100, MAP_HEIGHT * 92/100);
	//polynesia
	area10.SetRect(MAP_WIDTH / 2, 0, MAP_WIDTH, MAP_HEIGHT / 5);
	area11.SetRect(MAP_WIDTH * 0.8, MAP_HEIGHT * 0.2, MAP_WIDTH, MAP_HEIGHT * 0.66);
	
	seedlandArea(area1,3);
	seedlandArea(area6,2);
	seedlandArea(area7,2);

	createlandArea(area1, 60);
	createlandArea(area6, 50);
	createlandArea(area7, 54);

	createlandArea(area2, 32);
	createlandArea(area3,  8);

	createlandArea(area4, 45);
	createlandArea(area8, 48);
	
	//re-apply landforming to center of continent
	area1.DeflateRect(15, 15, 15, 15);
	createlandArea(area1, 50);

	//remove islands from designated open sea areas
	removeLand(area5, 95);
	removeLand(area9, 95);

	//further cycle to consolidate the core of the continent
	float temp = (((float)MAP_WIDTH * (float)mcont_size) / 100);
	cont_x = (int)(temp*0.7);
	temp = (((float)MAP_HEIGHT * (float)mcont_size) / 100);
	cont_y = (int)(temp*0.7);

	//make the edges variable by breaking up straight coastlines 
	u_varyEdges(3);

//--------above to be replaced with a 'zone' approach------------

	//create sea border, for east, north & west sides of the map only
	int boundary_width = 3;
	int c = 2;
	for (i=0; i < MAP_WIDTH; i++)
	{
		for(j=0; j < MAP_HEIGHT; j++)
		{
			count = 0;
			if ((i < boundary_width)
			||  (j < boundary_width)
			||  (i >= MAP_WIDTH - boundary_width) )
			//||  (j >= MAP_HEIGHT - boundary_width) )
			{
				Land[i][j].terrain  = SEA;
				Land[i][j].altitude = 25;
				Land[i][j].land_terrain_type = -1;
			}
		}
	}

	//make the edges variable by breaking up straight coastlines 
	u_varyEdges(3);

	//create antarctica (extra squares on bottom of map)
	boundary_width = 3;
	for (i=boundary_width; i < MAP_WIDTH - boundary_width; i++)
	{
		for(j=MAP_HEIGHT - boundary_width; j < MAP_HEIGHT; j++)
		{
			if (!(rand() % 6)
			|| (j == MAP_HEIGHT - 1))
			{
				Land[i][j].terrain  = LAND;
				Land[i][j].altitude = SEALEVEL + 1 +
									  ((rand() % (100 - SEALEVEL)/2) * 
									   (rand() % (100 - SEALEVEL)/2));
			}
		}
	}

	//kill off most lone hills/mountains, & some of the more isolated ones
	int hill_count;
	int sea_count;
	//proportional to altutude....
	for (cycles = 0; cycles < (25 - (altitude / 10)); cycles++)
		for (i = 0; i < MAP_WIDTH; i++)
		{
			for(j = 0; j < MAP_HEIGHT; j++)
			{
				if (Land[i][j].altitude >= HILLLINE)
				{
					hill_count = u_hillCount(i, j, 1);
					sea_count = u_seaCount(i, j, 3);
					if ((hill_count < 4)
					&&  (sea_count  < 30)
					&&  (rand() % 2))
						Land[i][j].altitude -= 1;

					if ((hill_count > 3)
					&&  (rand() % 6 < hill_count))
						Land[i][j].altitude += 1;
				}
			}
		}

	//remove all hills, and most lone mountains from antartic area
	for (i=boundary_width; i < MAP_WIDTH - boundary_width; i++)
	{
		for(j=antarctic; j < MAP_HEIGHT; j++)
		{
			hill_count = u_hillCount(i, j, 1);

			if ((hill_count <= 1)
			&&  (Land[i][j].altitude >= SNOWLINE)
			&&  (rand() % 10))
				Land[i][j].altitude = SNOWLINE - 1;

			if ((Land[i][j].altitude < SNOWLINE)
			&&  (Land[i][j].altitude >= HILLLINE))
				Land[i][j].altitude = HILLLINE - 1;
		}
	}

	//group mountains without regard for the proximity to the sea for the continental area
	for (cycles = 0; cycles < 5; cycles++)
		for (i = (MAP_WIDTH/2) - (cont_x/2); i < (MAP_WIDTH/2) + (cont_x/2); i++)
		{
			for(j = (MAP_HEIGHT/2) - (cont_y/2); j < (MAP_HEIGHT/2) + (cont_y/2); j++)
			{
				hill_count = u_hillCount(i, j, 1);

				if ((hill_count < 3)
				//&&  (hill_count > 0)
				&&  (Land[i][j].altitude >= HILLLINE)
				&&  (rand() % 6) > hill_count)
					Land[i][j].altitude -= 2;

				if ((hill_count > 1)
				&&  (hill_count < 5)
				&&  (rand() % 6 < hill_count)
				&&  (rand() % 6 < hill_count))
					Land[i][j].altitude += 1;			

			}
		}
	
	//increase where mountains already are, to extend length of ranges
	for (cycles = 0; cycles < (altitude / 10); cycles++)
		for (i = (MAP_WIDTH/2) - (cont_x/2); i < (MAP_WIDTH/2) + (cont_x/2); i++)
		{
			for(j = (MAP_HEIGHT/2) - (cont_y/2); j < (MAP_HEIGHT) + (cont_y/2); j++)
			{
				hill_count = u_hillCount(i, j, 1);

				if ((hill_count > 1)
				&&  (hill_count < 6)
				&&  (rand() % 6 < hill_count)
				&&  (rand() % 6 < hill_count))
					Land[i][j].altitude += 2;			

			}
		}

	//reduce all altitudes to a maximum of 100
	for (i = 0; i < MAP_WIDTH; i++)
		for (j = 0; j < MAP_HEIGHT; j++)
			if (Land[i][j].altitude > 100)
				Land[i][j].altitude = 100;


	for (i = 0; i < 3; i++)
	{
		increaseMtNumber(area2, 70, 70);
		//increaseMtNumber(area6, 50, 50);
		//increaseMtNumber(area7, 50, 50);
		//increaseMtNumber(area2, 70, 50);
	}

	//remove some sets of mountains where they are part of a square group of four
	CRect apply_area;
	apply_area.SetRect(0, 0, MAP_WIDTH, MAP_HEIGHT);
	reduceMtDensity(apply_area, 50);

	//reduce number of mountains in small islands areas
	for (i = 0; i < 8; i++)
	{
		reduceMtNumber(area3, 30);
		reduceMtNumber(area5, 30);
		reduceMtNumber(area9, 30);
		reduceMtNumber(area10, 30);
		reduceMtNumber(area11, 30);
	}

	//verify all squares with the appropriate altitude have terrain correctly
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for(j = 0; j < MAP_HEIGHT; j++)
		{
			if (Land[i][j].altitude > SEALEVEL)
				Land[i][j].terrain = LAND;
			else
			{
				Land[i][j].terrain = SEA;
				Land[i][j].land_terrain_type = -1;
			}
		}
	}

	//*below comments proably now redundant
	//seed start and end of 'great dividing range'
	//augment altitude of points between start and end point
	//cycle to cluster higher altitude areas

	//produce terrain types
	//based on temperature to decide latitude 
	//if temperature is 50 % then 
	//bottomm 10% is freezing
	//equator is at top 10 %
	//tropicss from 0 - 30 %
	//temperate from 30-60 %
	//cold from 60-90      %

	int r_cold[3] = {TUNDRA, PLAINS, ICE};
	int r_temp[5] = {PLAINS, DESERT, MARSH, GRASSLAND, PRAIRIE};
	int r_trop[4] = {SAVANNAH, SWAMP, GRASSLAND, MARSH};

	for (i=0; i < MAP_WIDTH; i++)
	{
		for(j=0; j < MAP_HEIGHT; j++)
		{
			if (Land[i][j].terrain == LAND)
			{
				if ( j >= antarctic)
					Land[i][j].land_terrain_type = ICE;
				else if ( j >= temperate )
					Land[i][j].land_terrain_type = r_cold[rand() % 3];
				else if ( j >= tropics )
					Land[i][j].land_terrain_type = r_temp[rand() % 5];
				else 
					Land[i][j].land_terrain_type = r_trop[rand() % 4];
			}
			else
				Land[i][j].land_terrain_type = -1;
		}
	}

	//cycle to cluster terrain_types
	for (cycles = 0; cycles < 4; cycles++)
	for (i=0+cycles; i < MAP_WIDTH; i+=2)
	{
		for(j=0+cycles; j < MAP_HEIGHT; j+=2)
		{
			if (Land[i][j].altitude > SEALEVEL)
			//need to count surrouding terrain types, and change to the most numerous
			for (terr_type = 0; terr_type < NUM_TERRAIN_TYPES; terr_type++)
				if ((u_terrCount(i, j, terr_type) > 2)
				&&  (rand() % 2))
				{
					Land[i][j].land_terrain_type = terr_type;
					terr_type = NUM_TERRAIN_TYPES;
				}
		}
	}

	//add extra deserts where distance from sea is great
	for (i=0+cycles; i < MAP_WIDTH; i+=2)
	{
		for(j=tropics + 1; j < temperate; j+=2)
		{
			if ((Land[i][j].altitude > SEALEVEL)
			&&  (Land[i][j].land_terrain_type != DESERT)	
		//	&&	(u_terrCount(i, j, DESERT) > 2)
			&&  (u_seaCount(i, j, 10) == 0)
			&&  (rand() % 10 == 0))
			{
				Land[i][j].land_terrain_type = DESERT;
			}
		}
	}

	//cycle to cluster deserts further 
	cycles = (temperature - rainfall);
	for (cycles = 0; cycles < ((temperature - rainfall) / 10); cycles++)
	for (i=0+(cycles%5); i < MAP_WIDTH; i+=2)
	{
		for(j=0+cycles+tropics; j < antarctic; j+=2)
		{
			if ((Land[i][j].altitude > SEALEVEL)
			&&  (Land[i][j].land_terrain_type != DESERT)	
			&&	(u_terrCount(i, j, DESERT) > 1)
			&&  (rand() % 3))
			{
				Land[i][j].land_terrain_type = DESERT;
			}
		}
	}

	//fill in gaps 
	cycles = 0;
	for (i=0+cycles; i < MAP_WIDTH; i+=1)
	{
		for(j=tropics + 1; j < temperate; j+=1)
		{
			if ((Land[i][j].altitude > SEALEVEL)
			&&  (Land[i][j].land_terrain_type != DESERT)	
			&&	(u_terrCount(i, j, DESERT) > 4))
		//	&&  (u_seaCount(i, j, 10) == 0)
		//	&&  (rand() % 10 == 0))
			{
				Land[i][j].land_terrain_type = DESERT;
			}
		}
	}

	//seed with forests
	initForests();

	//cycle to cluster forests..proportinal to rainfall
	for (cycles = 0; cycles < (forest / 10); cycles++)
	{
		for (i=0+(cycles % 5); i < MAP_WIDTH - 5; i+=5)
		{
			for(j=0+(cycles % 5); j < antarctic; j+=5)
			{
				if ((Land[i][j].altitude > SEALEVEL)
				&&  (Land[i][j].altitude < HILLLINE)
				&&  (Land[i][j].land_terrain_type != ICE)
				&&  (Land[i][j].land_terrain_type != DESERT)
				&&  (u_forestCount(i, j, 1) > 1)
				&&  (rand() % 2))
					Land[i][j].forest = true;
			}
		}
	}

	//seed rivers..proportional to rainfall
	int coast_score;
	for (i=0; i < MAP_WIDTH; i++)
		{
			for(j=0; j < antarctic; j++)
			{
				coast_score = 0;
				if (Land[i][j].altitude <= SEALEVEL)
					coast_score = coastalScore(i,j);
				//if this is a coastal square with 3 sides of land, 
				//and the potential river square is 
				//	*	not a hill/mountain
				//	*	not adjacent to the sea anywhere else
				//	*	not adjacent to an existing river square
				if ((coast_score == 7)
				||  ((coast_score == 2) && (rand() % 10 == 0)))
				{

					if ((u_boundsCheck(i,j+1))
					&&  (Land[i][j+1].altitude < HILLLINE)
					&&  (u_seaCount(i, j+1, 1) <= 3)
					&&  (u_riverCount(i, j+1, 0) <= 1)
					&&  (u_riverCount(i, j, 0) == 0))
					{
						Land[i][j].river = MNRIVER;
						Land[i][j+1].river = MNRIVER;
						//u_extendRiver2(i, j+1);
					}
				}
				if ((coast_score == 11)
				||  ((coast_score == 1) && (rand() % 10 == 0)))
				{
					if ((u_boundsCheck(i-1,j))
					&&  (Land[i-1][j].altitude < HILLLINE)
					&&  (u_seaCount(i-1, j, 1) <= 3)
					&&  (u_riverCount(i-1, j, 0) <= 1)
					&&  (u_riverCount(i, j, 0) == 0))
					{
						Land[i][j].river = MNRIVER;
						Land[i-1][j].river = MNRIVER;
						//u_extendRiver2(i-1, j);
					}
				}
				if ((coast_score == 13)
				||  ((coast_score == 8) && (rand() % 10 == 0)))
				{
					if ((u_boundsCheck(i,j-1))
					&&  (Land[i][j-1].altitude < HILLLINE)
					&&  (u_seaCount(i, j-1, 1) <= 3)
					&&  (u_riverCount(i, j-1, 0) <= 1)
					&&  (u_riverCount(i, j, 0) == 0))
					{
						Land[i][j].river = MNRIVER;
						Land[i][j-1].river = MNRIVER;
						//u_extendRiver2(i, j-1);
					}
				}
				if ((coast_score == 14)
				||  ((coast_score == 4) && (rand() % 10 == 0)))
				{
					if ((u_boundsCheck(i+1,j))
					&&  (Land[i+1][j].altitude < HILLLINE)
				//	&&  (u_seaCount(i+1, j, 1) <= 2)
					&&  (u_riverCount(i+1, j, 0) <= 1)
					&&  (u_riverCount(i, j, 0) == 0))
					{
						Land[i][j].river = MNRIVER;
						Land[i+1][j].river = MNRIVER;
						//u_extendRiver2(i+1, j);
					}
				}	
			}
		}

	int river_cycles = ((rainfall - (temperature/3))+10) / 10;
	river_cycles = 3;
	for (cycles = 0; cycles < river_cycles; cycles++)
	{
		for (i=0; i < MAP_WIDTH; i++)
		{
			for(j=0; j < antarctic; j++)
			{
				if ((Land[i][j].river == MNRIVER)
				&&  (Land[i][j].altitude > SEALEVEL)
			//	&&  (u_seaCount(i, j, 1) == 1)
				&&  (u_riverCount(i, j, 0) == 1))
					u_extendRiver3(i, j, 3);
			}
		}
	}

	return 0;
}

void Map::seedlandArea(CRect area, int density_pct)
{
	int area_size = area.Height() * area.Width();
	int num_cycles = area_size / 10 * density_pct / 100;

	//put random lines of land points instead
	for (int cycles = 0; cycles < num_cycles; cycles++)
	{
		CPoint start_pt;
		CPoint end_pt;
		int max_length = 20;

		start_pt.x = (rand() % (area.right - area.left)) + area.left;
		start_pt.y = (rand() % (area.bottom - area.top)) + area.top;

		end_pt.x = (rand() % (max_length * 2)) - max_length + start_pt.x;
		end_pt.y = (rand() % (max_length * 2)) - max_length + start_pt.y;

		u_boundsTrim(end_pt, area);

		int i = start_pt.x;
		int j = start_pt.y;
		bool line_finished = false;

		for (;!line_finished;)
		{
			if (rand() % 5)
			{
				int x_adj = (rand() % 3) - 1 + i;
				int y_adj = (rand() % 3) - 1 + j;
				Land[x_adj][y_adj].terrain  = LAND;
				Land[x_adj][y_adj].altitude = 100;
			}

			if ((start_pt.x < end_pt.x)
			&&  (i < end_pt.x))
				if (rand() % 2)
					i++;
			if ((start_pt.x > end_pt.x)
			&&  (i > end_pt.x))
				if (rand() % 2)
					i--;
			if ((start_pt.y < end_pt.y)
			&&  (j < end_pt.y))
				if (rand() % 2)
					j++;
			if ((start_pt.y >= end_pt.y)
			&&  (j > end_pt.y))
				if (rand() % 2)
					j--;	

			if ((abs(i - end_pt.x) < (rand() % 5))
			&&  (abs(j - end_pt.y) < (rand() % 5)))
				line_finished = true;
		}
	}

}

void Map::createlandArea(CRect area, int land_size)
{
	int count; 

	//need to have fewer cycles for the perimeter to minimize the possiblity
	//of a straight edge coastline

	for (int cycles = 0; cycles < land_size; cycles++)
		for (int i=area.left; i < area.right; i++)
			for (int j=area.top; (j < area.bottom); j++)
			{
				count = u_landcount(i, j, 1);

				//probability of adding land is greater in the center of the area
				bool add_land = false;
				int rand_factor;
				if  ((abs(j - area.top)    > (abs(area.top - area.bottom) * 0.85))
				||   (abs(j - area.bottom) > (abs(area.top - area.bottom) * 0.85))
				||   (abs(i - area.left)   > (abs(area.right - area.left) * 0.85))
				||   (abs(i - area.right)  > (abs(area.right - area.left) * 0.85)))
					rand_factor = 4;
				else
					rand_factor = 10;
				
				if (rand() % rand_factor)
					add_land = true;

				//only consider adding land if a probability is met
				if (add_land)
				{
					//only add land if there is some directly adjacent land
					if (count > 0)
					{
						if (  ((rand()  % 6) < count)
						&&   ((rand()  % 6) < count) 
						&&    (!(rand() % 3))  )
						{
							Land[i][j].terrain  = LAND;
							Land[i][j].altitude = SEALEVEL + (rand() % (100 - SEALEVEL)) + 1;
						}
					}
					//add land if there are many adjacent land squares
					count = u_landcount(i, j, 2);
					if (count > 20)
						if ((rand() % 25) < count)
						{
							Land[i][j].terrain  = LAND;
							Land[i][j].altitude = SEALEVEL + (rand() % (100 - SEALEVEL)) + 1;
						}
				}
			}
}

void Map::removeLand(CRect area, int probability)
{
	for (int i=area.left; i < area.right; i++)
		for (int j=area.top; (j < area.bottom); j++)
		{
			//remove land squares based on probability value
			if (rand() % 100 < probability)
			{
				Land[i][j].terrain  = SEA;
				Land[i][j].altitude = SEALEVEL - 5;
			}
		}
}

void Map::reduceMtDensity(CRect area, int probability)
{
	for (int i=area.left; i < area.right; i++)
		for (int j=area.top; (j < area.bottom); j++)
		{
			//remove land squares based on probability value
			int mt_score = u_mountSquare(i,j);
			if ((mt_score > 0)
			&&  (rand() % 100 < probability))
			{
				Land[i][j].altitude -= 8;
			}
		}
}

void Map::reduceMtNumber(CRect area, int probability)
{
	for (int i=area.left; i < area.right; i++)
		for (int j=area.top; (j < area.bottom); j++)
		{
			//reduce altitude of mountain squares based on probability value
			if (u_boundsCheck(i,j))
				if ((Land[i][j].altitude >= SNOWLINE)
				&&  (rand() % 100 < probability))
				{
					Land[i][j].altitude -= 8;
				}
		}
}

void Map::increaseMtNumber(CRect area, int probability, int cluster)
{
	//increase mountains, according to probabilty and cluster factors
	//the higher the cluster factor the more likely the altitude will increase
	//  in proportion to the number of surrounding hills/mountains
	for (int i=area.left; i < area.right; i++)
		for (int j=area.top; (j < area.bottom); j++)
		{
			if (u_boundsCheck(i,j))
			{
				int hill_count = u_hillCount(i, j, 1);
					if   ((rand() % cluster) < hill_count * 10)
						if (rand() % 100 < probability)
							Land[i][j].altitude += 3;
			}
		}
}

void Map::u_extendRiver3(int i, int j, int length)
{
	int i_loc, j_loc, direction, options, chance, count, new_length;
	
	if (length == 0)
		return;

	new_length = length - 1;

	//count the number of avaliable spaces for the river to go to
	//if none are available, this is the end
	//randomly select one to follow

	//occaisionally, select two to follow
	//very occaisionally, select all three

	CPoint directions[4];

	//left
	directions[0].x = -1;
	directions[0].y =  0;
	//right
	directions[1].x =  1;
	directions[1].y =  0;
	//top
	directions[2].x =  0;
	directions[2].y = -1;
	//bottom
	directions[3].x =  0;
	directions[3].y =  1;

	direction = rand() % 4;
	options = 0;

	for (count = 0; count < 4; count++)
	{
		i_loc = i;
		j_loc = j;
		i_loc += directions[count].x;
		j_loc += directions[count].y;

		if ((u_boundsCheck(i_loc,j_loc))
		&&  (Land[i_loc][j_loc].altitude < HILLLINE)
		&&  (Land[i_loc][j_loc].altitude > SEALEVEL)
		&&  (Land[i_loc][j_loc].river == NORIVER)
		&&  (Land[i_loc][j_loc].land_terrain_type != ICE)
		&&  (u_seaCount(i_loc, j_loc, 1) <= 1)
		&&  (u_riverCount(i_loc, j_loc, 2) <= 5))
			options++;
		else
			directions[count].x = directions[count].y = 0;
	}

	for (count = 0; count < options; count++)
	{
		chance = 0;
		if (count == 0)
		{
			if ((rand() % 8))
				chance = 1;
		}
		else
			 if (!(rand() % (count * 3)))
				 chance = 1;

		if (chance)
		{
			//pick directions at random, until an available one is found
			do
				direction = rand() % 4;
			while((directions[direction].x == 0) && 
				  (directions[direction].y == 0));

			i_loc = i;
			j_loc = j;
			i_loc += directions[direction].x;
			j_loc += directions[direction].y;

			if (u_riverCount(i_loc, j_loc, 0) <= 1)
			{
				Land[i_loc][j_loc].river = MNRIVER;
				u_extendRiver3(i_loc, j_loc, new_length);
			}
		}
	}

	return;
}


void Map::u_extendRiver2(int i, int j)
{
	int i_loc, j_loc, direction, options, chance, count;
	
	//count the number of avaliable spaces for the river to go to
	//if none are available, this is the end
	//randomly select one to follow

	//occaisionally, select two to follow
	//very occaisionally, select all three

	CPoint directions[4];

	//left
	directions[0].x = -1;
	directions[0].y =  0;
	//right
	directions[1].x =  1;
	directions[1].y =  0;
	//top
	directions[2].x =  0;
	directions[2].y = -1;
	//bottom
	directions[3].x =  0;
	directions[3].y =  1;

	direction = rand() % 4;
	options = 0;

	for (count = 0; count < 4; count++)
	{
		i_loc = i;
		j_loc = j;
		i_loc += directions[count].x;
		j_loc += directions[count].y;

		if ((u_boundsCheck(i_loc,j_loc))
		&&  (Land[i_loc][j_loc].altitude < HILLLINE)
		&&  (Land[i_loc][j_loc].altitude > SEALEVEL)
		&&  (Land[i_loc][j_loc].river == NORIVER)
		&&  (u_seaCount(i_loc, j_loc, 1) <= 1)
		&&  (u_riverCount(i_loc, j_loc, 0) <= 1))
			options++;
		else
			directions[count].x = directions[count].y = 0;
	}

	for (count = 0; count < options; count++)
	{
		chance = 0;
		if (count == 0)
			chance = 1;
		else
			 if (!(rand() % (count * 5)))
				 chance = 1;

		if (chance)
		{
			//pick directions at random, until an available one is found
			do
				direction = rand() % 4;
			while((directions[direction].x == 0) && 
				  (directions[direction].y == 0));

			i_loc = i;
			j_loc = j;
			i_loc += directions[direction].x;
			j_loc += directions[direction].y;

			if (u_riverCount(i_loc, j_loc, 0) <= 1)
			{
				Land[i_loc][j_loc].river = MNRIVER;
				u_extendRiver2(i_loc, j_loc);
			}
		}
	}

	return;
}

void Map::u_extendRiver(int i, int j)
{
	int x_dir, y_dir, i_loc, j_loc, tries, river_length;
	
	river_length = 4;
	i_loc = i;
	j_loc = j;

	for (tries = 0; tries < river_length; tries++)
	{
		i_loc = i;
		j_loc = j;
		do{
			x_dir = (rand() % 3) - 1;
			y_dir = (rand() % 3) - 1;
		}
		while(((abs(x_dir) + abs(y_dir))) != 1);

		i_loc += x_dir;
		j_loc += y_dir;

		if ((u_boundsCheck(i_loc,j_loc))
		&&  (Land[i_loc][j_loc].altitude < HILLLINE)
		&&  (Land[i_loc][j_loc].altitude > SEALEVEL)
		&&  (Land[i_loc][j_loc].river == NORIVER)
		&&  (u_seaCount(i_loc, j_loc, 1) <= 2)
		&&  (u_riverCount(i_loc, j_loc, 0) <= 1))
		{
			if ((rand() % 8 > tries))
			{
				Land[i_loc][j_loc].river = MNRIVER;
				u_extendRiver(i_loc, j_loc);
			}
			//return;
		}
	}
	return;
}

void Map::initTables(void)
{
	//initCoastal_table;
	coast_table[0]  = 0;
	coast_table[1]  = 250;
	coast_table[2]  = 200;
	coast_table[3]  = 100;
	coast_table[4]  = 350;
	coast_table[5]  = 600;
	coast_table[6]  = 50;
	coast_table[7]  = 550;
	coast_table[8]  = 300;
	coast_table[9]  = 0;
	coast_table[10] = 650;
	coast_table[11] = 400;
	coast_table[12] = 150;
	coast_table[13] = 450;
	coast_table[14] = 500;
	coast_table[15] = 700;

	//initialise mount_table;
	mount_table[0]  = 0;
	mount_table[1]  = 100;
	mount_table[2]  = 150;
	mount_table[3]  = 350;
	mount_table[4]  = 50;
	mount_table[5]  = 450;
	mount_table[6]  = 300;
	mount_table[7]  = 550;
	mount_table[8]  = 200;
	mount_table[9]  = 250;
	mount_table[10] = 500;
	mount_table[11] = 600;
	mount_table[12] = 400;
	mount_table[13] = 650;
	mount_table[14] = 700;
	mount_table[15] = 750;

	//initialise forest_table;
	forest_table[0]  = 0;
	forest_table[1]  = 100;
	forest_table[2]  = 150;
	forest_table[3]  = 350;
	forest_table[4]  = 50;
	forest_table[5]  = 450;
	forest_table[6]  = 300;
	forest_table[7]  = 550;
	forest_table[8]  = 200;
	forest_table[9]  = 250;
	forest_table[10] = 500;
	forest_table[11] = 600;
	forest_table[12] = 400;
	forest_table[13] = 650;
	forest_table[14] = 700;
	forest_table[15] = 750;

	//initialise rivers_table;
	rivers_table[0]  = 0;
	rivers_table[1]  = 100;
	rivers_table[2]  = 150;
	rivers_table[3]  = 350;
	rivers_table[4]  = 50;
	rivers_table[5]  = 450;
	rivers_table[6]  = 300;
	rivers_table[7]  = 550;
	rivers_table[8]  = 200;
	rivers_table[9]  = 250;
	rivers_table[10] = 500;
	rivers_table[11] = 600;
	rivers_table[12] = 400;
	rivers_table[13] = 650;
	rivers_table[14] = 700;
	rivers_table[15] = 750;
}

void Map::removeDiagonals(void)
{
	//remove all 'diagonally adjacent only' occurances
	int boundary_width = 1;
	BOOL landfill_flag = true;
	for ( ;landfill_flag; )
	{
		landfill_flag = false;
		for (int i=boundary_width; i<MAP_WIDTH - boundary_width; i++)
		{
			for(int j=boundary_width; j<MAP_HEIGHT - boundary_width; j++)
			{
				if (Land[i][j].terrain == LAND)
				{
					//check each diagonally adjacent boundary, and if it is land,
					//check the two adjacent lateral boundaries
					//if both are sea, randomly set one to land
					//update the landfill iteration counter
					if ((Land[i+1][j+1].terrain == LAND)
					&&  (Land[i+1][j].terrain == SEA)
					&&  (Land[i][j+1].terrain == SEA))
					{
						if (rand() % 2)
							Land[i+1][j].terrain = LAND;
						else
							Land[i][j+1].terrain = LAND;
						landfill_flag = true;
					}

					if ((Land[i+1][j-1].terrain == LAND)
					&&  (Land[i+1][j].terrain == SEA)
					&&  (Land[i][j-1].terrain == SEA))
					{
						if (rand() % 2)
							Land[i+1][j].terrain = LAND;
						else
							Land[i][j-1].terrain = LAND;
						landfill_flag = true;
					}				

					if ((Land[i-1][j+1].terrain == LAND)
					&&  (Land[i-1][j].terrain == SEA)
					&&  (Land[i][j+1].terrain == SEA))
					{
						if (rand() % 2)
							Land[i-1][j].terrain = LAND;
						else
							Land[i][j+1].terrain = LAND;
						landfill_flag = true;
					}

					if ((Land[i-1][j-1].terrain == LAND)
					&&  (Land[i-1][j].terrain == SEA)
					&&  (Land[i][j-1].terrain == SEA))
					{
						if (rand() % 2)
							Land[i-1][j].terrain = LAND;
						else
							Land[i][j-1].terrain = LAND;
						landfill_flag = true;
					}
				}
			}
		}
	}
}

Map::~Map(void)
{
}

void Map::initMessages(void)
{
	for (int i=0; i < 100; i++)
	{
		active_messages[i] = 0;
		Messages[i] = "Message not allocated";
	}
	Messages[0]  = "Successful return";
	Messages[1]  = "MaskBlt operation failed";
	Messages[2]  = "MaskBlt failed - caravel";
	Messages[3]  = "bltBackingSquare failed (MaskBlt)";
	Messages[99] = "Bitmap load failed";
}

void Map::initLandTerrainTypes(void)
{
	//should take climate as a paramater to vary the distribution of landscapes
	//currently just assigns terrain types randomly
	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			int count = 0;
			if (Land[i][j].terrain == LAND)
				Land[i][j].land_terrain_type = rand() % 9;
			else
				Land[i][j].land_terrain_type = -1;
		}
	}
}

void Map::initRecentlySeen(void)
{
	//should take climate as a paramater to vary the distribution of landscapes
	//currently just assigns terrain types randomly
	for (int i=0; i < MAP_WIDTH; i++)
		for(int j=0; j < MAP_HEIGHT; j++)
			Land[i][j].recently_seen = false;
}

void Map::initAltitudes(void)
{
	//should take climate as a paramater to vary the distribution of landscapes
	//currently just assigns terrain types randomly
	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			int count = 0;
			if (Land[i][j].terrain == SEA)
				Land[i][j].altitude = rand() % SEALEVEL;
			else
			{
				Land[i][j].altitude = (rand() % (100 - SEALEVEL)) + SEALEVEL + 1;
			}
		}
	}
}

void Map::initForests(void)
{
	//should take climate as a paramater to vary the distribution of landscapes
	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			if ( (Land[i][j].altitude > SEALEVEL)
			&&   (Land[i][j].altitude < HILLLINE)
			&&   (Land[i][j].land_terrain_type != ICE)  
			&&   (Land[i][j].land_terrain_type != DESERT) ) 
				if ((rand() % 5))
					Land[i][j].forest = true;
		}
	}
}

void Map::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << MAP_WIDTH;
		ar << MAP_HEIGHT;
	}
	else
	{
		ar >> MAP_WIDTH;
		ar >> MAP_HEIGHT;
	}
	for (int i = 0; i < MAP_WIDTH; i++)
		for (int j = 0; j < MAP_HEIGHT; j++)
			Land[i][j].Serialize(ar);
}

void Map::updateExplored(CPoint loc, int line_of_sight)
{
	//initialise a map area as explored for a given location
	for (int i = loc.x - line_of_sight; 
		     i <= loc.x + line_of_sight; 
			 i++)
		for (int j = loc.y - line_of_sight; 
			 j <= loc.y + line_of_sight; j++)
			 if ((i >= 0) && (j >=0) 
			 &&  (i < MAP_WIDTH) && (j < MAP_HEIGHT))
				Land[i][j].explored = true;
}

void Map::updateRecentlySeen(CPoint loc, int line_of_sight)
{
	//initialise a map area as 'recently seen' for a given location
	if (!u_boundsCheck(loc.x, loc.y))
		return;
	for (int i = loc.x - line_of_sight; 
		     i <= loc.x + line_of_sight; 
			 i++)
		for (int j = loc.y - line_of_sight; 
			 j <= loc.y + line_of_sight; j++)
			 if ((i >= 0) && (j >=0) 
			 &&  (i < MAP_WIDTH) && (j < MAP_HEIGHT))
				Land[i][j].recently_seen = true;
}

void Map::synchMapTerrain(Map *MasterMap, CPoint loc, int line_of_sight)
{
	//copies from the passed map object to 'this' all map details for a given radius
	if (!u_boundsCheck(loc.x, loc.y))
		return;
	for (int i = loc.x - line_of_sight; 
		     i <= loc.x + line_of_sight; 
			 i++)
		for (int j = loc.y - line_of_sight; 
			 j <= loc.y + line_of_sight; j++)
			 if ((i >= 0) && (j >=0) 
			 &&  (i < MAP_WIDTH) && (j < MAP_HEIGHT))
			 {
				 //copy all details from one square to another
				 //except piece details, as these should not be propagated since the MasterMap
				 //is out of date until end of turn
				 //int piece_data = this->Land[i][j].piece;
				 this->Land[i][j].copyTerrain(&(MasterMap->Land[i][j]));
				 //this->Land[i][j] = piece_data;
			 }
}

int Map::coastalScore(int x, int y)
{
	//evaluate coastal score
	int coastal_score = 0;
	if (y > 0)
		if (Land[x][y-1].terrain == LAND)
			coastal_score += 8;
	if (y< MAP_HEIGHT - 1)
		if (Land[x][y+1].terrain == LAND)
			coastal_score += 2;
	if (x > 0)
		if (Land[x-1][y].terrain == LAND)
			coastal_score += 1;
	if (x < MAP_WIDTH - 1)
		if (Land[x+1][y].terrain == LAND)
			coastal_score += 4;
	return coastal_score;
}

int Map::u_mountScore(int x, int y)
{
	//evaluate mount score
	int mount_score = 0;
	if (y > 0)
		if (Land[x][y-1].altitude >= SNOWLINE)
			mount_score += 8;
	if (y < MAP_HEIGHT - 1)
		if (Land[x][y+1].altitude >= SNOWLINE)
			mount_score += 2;
	if (x > 0)
		if (Land[x-1][y].altitude >= SNOWLINE)
			mount_score += 1;
	if (x < MAP_WIDTH - 1)
		if (Land[x+1][y].altitude >= SNOWLINE)
			mount_score += 4;
	return mount_score;
}

int Map::u_mountSquare(int x, int y)
{
	//evaluate mount score for all surrounding squares
	if (!((u_boundsCheck(x - 1, y - 1)) 
	&&   (u_boundsCheck(x + 1, y + 1))))
		return -1;

	int mount_score = 0;
	if ((Land[x-1][y-1].altitude >= SNOWLINE)
	&&  (Land[x][y-1].altitude >= SNOWLINE)
	&&  (Land[x-1][y].altitude >= SNOWLINE))
		mount_score += 1;

	if ((Land[x+1][y-1].altitude >= SNOWLINE)
	&&  (Land[x][y-1].altitude >= SNOWLINE)
	&&  (Land[x+1][y].altitude >= SNOWLINE))
		mount_score += 2;

	if ((Land[x+1][y+1].altitude >= SNOWLINE)
	&&  (Land[x][y+1].altitude >= SNOWLINE)
	&&  (Land[x+1][y].altitude >= SNOWLINE))
		mount_score += 4;

	if ((Land[x-1][y+1].altitude >= SNOWLINE)
	&&  (Land[x-1][y].altitude >= SNOWLINE)
	&&  (Land[x][y+1].altitude >= SNOWLINE))
		mount_score += 2;

	return mount_score;
}

int Map::u_mthillScore(int x, int y)
{
	//evaluate coastal score
	int mthill_score = 0;
	if (y > 0)
		if (Land[x][y-1].altitude >= HILLLINE)
			mthill_score += 8;
	if (y < MAP_HEIGHT - 1)
		if (Land[x][y+1].altitude >= HILLLINE)
			mthill_score += 2;
	if (x > 0)
		if (Land[x-1][y].altitude >= HILLLINE)
			mthill_score += 1;
	if (x < MAP_WIDTH - 1)
		if (Land[x+1][y].altitude >= HILLLINE)
			mthill_score += 4;
	return mthill_score;
}

int Map::u_terrCount(int x, int y, int terr)
{
	//count number of adjacent squares of the given terrain
	int count = 0;
	for (int i = x - 1; i < x + 2; i++)
		for (int j = y - 1; j < y + 2; j++)
			//check points are within map boundary
			if (u_boundsCheck(i, j))
			//if ((i >= 0) && (j >= 0) && (i < MAP_WIDTH) && (j < MAP_HEIGHT))
			if ((Land[i][j].land_terrain_type == terr)
				&&   ((i != x) || (j != y)))
					count++;
			
	return count;
}

int Map::u_seaCount(int x, int y, int radius)
{
	//count number of adjacent sea squares in the radius specified
	int count = 0;
	for (int i = x - radius; i < x + radius + 1; i++)
		for (int j = y - radius; j < y + radius + 1; j++)
			//check points are within map boundary
			if (u_boundsCheck(i, j))
				if ((Land[i][j].altitude <= SEALEVEL)
				&&  ((i != x) || (j != y)))
					count++;
			
	return count;
}

bool Map::isSeaSquare(int i, int j)
{
	if ((u_boundsCheck(i, j))
	&&  (Land[i][j].altitude <= SEALEVEL))
		return true;
	else		
		return false;
}

bool Map::isRecentlySeen(int x, int y)
{
	if ((u_boundsCheck(x, y))
	&&  (Land[x][y].recently_seen))
		return true;
	else
		return false;
}

int Map::getPiece(int i, int j)
{
	if (u_boundsCheck(i, j))
		return Land[i][j].piece;
	else
		return -1;
}

bool Map::hasRiver(int x, int y)
{
	if (Land[x][y].river == NORIVER)
		return false;
	else
		return true;
}

int Map::u_riverCount(int x, int y, int radius)
{
	//count number of adjacent river squares in the radius specified
	int count = 0;

	if (radius == 0)
	{
		if (Land[x-1][y].river != NORIVER)
			count++;
		if (Land[x+1][y].river != NORIVER)
			count++;
		if (Land[x][y-1].river != NORIVER)
			count++;
		if (Land[x][y+1].river != NORIVER)
			count++;
		return count;
	}

	for (int i = x - radius; i < x + radius + 1; i++)
		for (int j = y - radius; j < y + radius + 1; j++)
			//check points are within map boundary
			if (u_boundsCheck(i, j))
				if ((Land[i][j].river != NORIVER)
				&&  ((i != x) || (j != y)))
					count++;
			
	return count;
}

int Map::u_hillCount(int x, int y, int radius)
{
	//count number of adjacent sea squares in the radius specified
	int count = 0;
	for (int i = x - radius; i < x + radius + 1; i++)
		for (int j = y - radius; j < y + radius + 1; j++)
			//check points are within map boundary
			if (u_boundsCheck(i, j))
				if ((Land[i][j].altitude >= HILLLINE)
				&&  ((i != x) || (j != y)))
					count++;
			
	return count;
}

int Map::u_forestCount(int x, int y, int radius)
{
	//count number of adjacent squares of forest in the given radius
	int count = 0;
	for (int i = x - radius; i < x + radius + 1; i++)
		for (int j = y - radius; j < y + radius + 1; j++)
			//check points are within map boundary
			if (u_boundsCheck(i, j))
				if ((Land[i][j].forest)
				&&  ((i != x) || (j != y)))
					count++;
			
	return count;
}

void Map::u_varyEdges(int x)
{
	//now make the edge of the continent highly variable
	//break up long flat coastlines with an extra landsquare
	int left_side, top_side, bottom_side, right_side, count;
	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 0)
			&&  (j > 0)
			&&  (i < MAP_WIDTH - 1)
			&&  (j < MAP_HEIGHT - 1)
			&&  (Land[i][j].terrain == SEA))
			{
				left_side = top_side = bottom_side = right_side = 0;
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
				{
					count++;
					left_side++;
					top_side++;
				}
				if (Land[i-1][j].terrain == LAND)
				{
					count++;
					left_side++;
				}
				if (Land[i-1][j+1].terrain == LAND)
				{
					count++;
					left_side++;
					bottom_side++;
				}
				if (Land[i][j-1].terrain == LAND)
				{
					count++;
					top_side++;
				}
				if (Land[i+1][j+1].terrain == LAND)
				{
					count++;
					right_side++;
					bottom_side++;
				}
				if (Land[i][j+1].terrain == LAND)
				{
					count++;
					bottom_side++;
				}
				if (Land[i+1][j-1].terrain == LAND)
				{
					count++;
					right_side++;
					top_side++;
				}
				if (Land[i+1][j].terrain == LAND)
				{
					count++;
					right_side++;
				}
				if (count == 3)
				{
					if ((right_side == 3)
					||  (left_side == 3)
					||  (top_side == 3)
					||  (bottom_side == 3))
						if ((rand() % x) == 0)
						{
							int i_space = i;
							int j_space = j;
							if ((rand() % 2))
							{
								Land[i][j].terrain = LAND;
								Land[i][j].altitude = SEALEVEL + (rand() % (100 - SEALEVEL)) +1;
							}
							else 
							{
								if (right_side == 3)
								{
									Land[i+1][j].terrain = SEA;
									Land[i+1][j].altitude = SEALEVEL - 10;
								}
								if (left_side == 3)
								{
									Land[i-1][j].terrain = SEA;
									Land[i-1][j].altitude = SEALEVEL - 10;
								}
								if (bottom_side == 3)
								{
									Land[i][j+1].terrain = SEA;
									Land[i][j+1].altitude = SEALEVEL - 10;
								}
								if (top_side == 3)
								{
									Land[i][j-1].terrain = SEA;
									Land[i][j-1].altitude = SEALEVEL - 10;
								}
							}		
						}
				}
			}
		}
	}
}

bool Map::u_boundsCheck(int i, int j)
{
	if ((i >= 0) && (j >= 0) && (i < MAP_WIDTH) && (j < MAP_HEIGHT))
		return true;
	else
		return false;
}

void Map::u_boundsTrim(CPoint &pt)
{
	if (pt.x < 0) 
		pt.x = 0;
	if (pt.y < 0) 
		pt.y = 0;
	if (pt.x >= MAP_WIDTH) 
		pt.x = MAP_WIDTH - 1;
	if (pt.y >= MAP_HEIGHT)
		pt.y = MAP_HEIGHT - 1;

	return;
}

void Map::u_boundsTrim(CPoint &pt, CRect area)
{
	if (pt.x < area.left) 
		pt.x = area.left;
	if (pt.y < area.top) 
		pt.y = area.top;
	if (pt.x >= area.right) 
		pt.x = area.right - 1;
	if (pt.y >= area.bottom)
		pt.y = area.bottom - 1;

	return;
}

int Map::u_landcount(int x, int y, int radius)
{
	int count  = 0;

	for (int i = x - radius; 
		     i <= x + radius; 
			 i++)
		for (int j = y - radius; 
			 j <= y + radius; j++)
			 if (u_boundsCheck(i, j))
				if (Land[i][j].terrain == LAND)
					count++;

	return count;
}

int Map::u_hillScore(int x, int y)
{
	//evaluate coastal score
	int hill_score = 0;
	if (y > 0)
		if ((Land[x][y-1].altitude >= HILLLINE)
		&&  (Land[x][y-1].altitude < SNOWLINE))
			hill_score += 8;
	if (y< MAP_HEIGHT - 2)
		if ((Land[x][y+1].altitude >= HILLLINE)
		&&  (Land[x][y+1].altitude < SNOWLINE))
			hill_score += 2;
	if (x > 0)
		if ((Land[x-1][y].altitude >= HILLLINE)
		&&  (Land[x-1][y].altitude < SNOWLINE))
			hill_score += 1;
	if (x < MAP_WIDTH - 2)
		if ((Land[x+1][y].altitude >= HILLLINE)
		&&  (Land[x+1][y].altitude < SNOWLINE))
			hill_score += 4;
	return hill_score;
}

int Map::u_forestScore(int x, int y)
{
	//evaluate coastal score
	int forest_score = 0;
	if (y > 0)
		if (Land[x][y-1].forest)
			forest_score += 8;
	if (y < MAP_HEIGHT - 1)
		if (Land[x][y+1].forest)
			forest_score += 2;
	if (x > 0)
		if (Land[x-1][y].forest)
			forest_score += 1;
	if (x < MAP_WIDTH - 1)
		if (Land[x+1][y].forest)
			forest_score += 4;

	return forest_score;
}

int Map::u_riversScore(int x, int y)
{
	//evaluate coastal score
	int rivers_score = 0;
	if (y > 0)
		if (Land[x][y-1].river)
			rivers_score += 8;
	if (y < MAP_HEIGHT - 1)
		if (Land[x][y+1].river)
			rivers_score += 2;
	if (x > 0)
		if (Land[x-1][y].river)
			rivers_score += 1;
	if (x < MAP_WIDTH - 1)
		if (Land[x+1][y].river)
			rivers_score += 4;

	return rivers_score;
}

int Map::processMove(UINT nChar, int &x, int &y)
{
	//receives the "move from" location, and returns 
	//    the "move to" location in the same fields
	//the return value of the function indicates if the move was allowed

	int new_loc_x, new_loc_y;
	BOOL move_allowed = false;
	new_loc_x = x;
	new_loc_y = y;
	BOOL diagonal = false;
	BOOL topleft, topright, botleft, botright;
	topleft = topright  =  botleft = botright = false;

	//check move does not take the piece off the map boundary
	switch (nChar)
	{
		case VK_NUMPAD1:
		case VK_END:
			if ((x > 0)
			&&  (y < MAP_HEIGHT - 1))
			{
				new_loc_x--;
				new_loc_y++;
				move_allowed = true;
				botleft = true;
				diagonal = true;
			}
			break;
		case VK_NUMPAD2:
		case VK_DOWN:
			if ((y < MAP_HEIGHT - 1))
			{
				new_loc_y++;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD3:
		case VK_NEXT:
			if ((x < MAP_WIDTH - 1)
			&&  (y < MAP_HEIGHT - 1))
			{
				new_loc_x++;
				new_loc_y++;
				move_allowed = true;
				botright = true;
				diagonal = true;
			}
			break;
		case VK_NUMPAD4:
		case VK_LEFT:
			if (x > 0)
			{
				new_loc_x--;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD6:
		case VK_RIGHT:
			if (x < MAP_WIDTH - 1)
			{
				new_loc_x++;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD7:
		case VK_HOME:
			if ((x > 0)
			&&  (y > 0))
			{
				new_loc_x--;
				new_loc_y--;
				move_allowed = true;
				topleft = true;
				diagonal = true;
			}
			break;
		case VK_NUMPAD8:
		case VK_UP:
			if (y > 0)
			{
				new_loc_y--;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD9:
		case VK_PRIOR:
			if ((x < MAP_WIDTH - 1)
			&&  (y > 0))
			{
				new_loc_x++;
				new_loc_y--;
				move_allowed = true;
				topright = true;
				diagonal = true;
			}
			break;
	}

	CPoint old_loc(x, y);
	CPoint new_loc(new_loc_x, new_loc_y);
	
	//if (old_loc == new_loc)
	//	return 0;

	if (!move_allowed)
	{
		x = new_loc_x;
		y = new_loc_y;
		return 0;
	}

	if  (mapMoveOk(old_loc, new_loc))
	{
		x = new_loc_x;
		y = new_loc_y;
		return 1;
	}



	//disable diagonal sea moves where both other adjacent squares are land
	//bool seaMoveOk(CPoint old_loc, CPoint new_loc);
	if ( (Land[new_loc_x][new_loc_y].terrain == SEA)
	&&   (Land[x][y].terrain == SEA) )
	{	
		move_allowed = false;
		if ( (topleft)
		&&  ((Land[new_loc_x + 1][new_loc_y].terrain == SEA)
		||   (Land[new_loc_x][new_loc_y + 1].terrain == SEA)) )
			move_allowed = true;

		if ( (topright)
		&&  ((Land[new_loc_x - 1][new_loc_y].terrain == SEA)
		||   (Land[new_loc_x][new_loc_y + 1].terrain == SEA)) )
			move_allowed = true;

		if ( (botleft)
		&&  ((Land[new_loc_x + 1][new_loc_y].terrain == SEA)
		||   (Land[new_loc_x][new_loc_y - 1].terrain == SEA)) )
			move_allowed = true;

		if ( (botright)
		&&  ((Land[new_loc_x - 1][new_loc_y].terrain == SEA)
		||   (Land[new_loc_x][new_loc_y - 1].terrain == SEA)) )
			move_allowed = true;

		if (!diagonal)
			move_allowed = true;
	}

	if (!move_allowed)
		return 0;
	else
	{
		//return the new location
		x = new_loc_x;
		y = new_loc_y;
		//return "move is allowed" (based on map features only)
		return 1;
	}
}

bool Map::mapMoveOk(CPoint old_loc, CPoint new_loc)
{
	//applies map rules to verify the request is legal
	//receives the "move from" and to locations
	//the return value of the function indicates if the move was allowed

	CPoint offset;
	offset.x = old_loc.x - new_loc.x;
	offset.y = old_loc.y - new_loc.y;

	BOOL diagonal = false;

	//return false if the move takes the piece off the map boundary
	if ((new_loc.x < 0)
	&&  (new_loc.x > MAP_WIDTH - 1)
	&&  (new_loc.y < 0)
	&&  (new_loc.y > MAP_HEIGHT - 1))
		return false;

	//if moving from one sea square to another diagonally
	if ( (isSeaSquare(new_loc.x, new_loc.y))
	&&   (isSeaSquare(old_loc.x, old_loc.y)) 
	&&   (offset.x != 0)
	&&   (offset.y != 0) )
	{			
		//disallow diagonal sea moves where both other adjacent squares are land
		if  ( (!isSeaSquare(new_loc.x, old_loc.y))
		&&    (!isSeaSquare(old_loc.x, new_loc.y)) )
			return false;	
	}
	//return "move is allowed" (based on map features only)
		return true;
}

const Map &Map::operator=(const Map &right)
{
	if (&right == this)
		return *this;

	//map creation fields not copied
	//error handling/message fields not copied

	this->MAP_HEIGHT = right.MAP_HEIGHT;
	this->MAP_WIDTH = right.MAP_WIDTH;

	for (int i = 0; i < 16; i++)
	{
		this->coast_table[i]  = right.coast_table[i];
		this->forest_table[i] = right.forest_table[i];
		this->mount_table[i]  = right.mount_table[i];
		this->rivers_table[i] = right.rivers_table[i];
	}

	for (int i = 0; i < MAX_WIDTH; i++)
		for (int j = 0; j < MAX_HEIGHT; j++)
		{
			this->Land[i][j].altitude = 
					right.Land[i][j].altitude;
			this->Land[i][j].explored = 
					right.Land[i][j].explored;
			this->Land[i][j].forest = 
					right.Land[i][j].forest;
			this->Land[i][j].land_terrain_type = 
					right.Land[i][j].land_terrain_type;
			this->Land[i][j].random_number = 
				    right.Land[i][j].random_number;
			this->Land[i][j].resource = 
					right.Land[i][j].resource;
			this->Land[i][j].river = 
					right.Land[i][j].river;
			this->Land[i][j].terrain = 
				    right.Land[i][j].terrain;
			this->Land[i][j].piece = 
				    right.Land[i][j].piece;
		}

	return *this;
}

void Map::copyMoves(Map &right)
{
	//copy all piece locations from passed Map to this Map
	for (int i = 0; i < MAX_WIDTH; i++)
		for (int j = 0; j < MAX_HEIGHT; j++)
		{
			this->Land[i][j].piece = 
				    right.Land[i][j].piece;
		}
}

void Map::updatePiecePresence(int old_loc_x, int old_loc_y, int new_loc_x, int new_loc_y)
{
	//moves a piece on the map from one location to another
	Land[new_loc_x][new_loc_y].piece = Land[old_loc_x][old_loc_y].piece;
	Land[old_loc_x][old_loc_y].piece = NO_PIECE;
}

int Map::addPiecetoSquare(int new_piece, CPoint new_loc)
{
	int return_value = 0;
	//ensure destination square is empty
	if (getPiece(new_loc.x, new_loc.y) == EMPTY)
	{
		Land[new_loc.x][new_loc.y].piece = new_piece;
		return_value++; 
	}
	return return_value;
}


int Map::removePiecefromSquare(CPoint new_loc)
{
	//return non-zero only if square is not already empty
	if (getPiece(new_loc.x, new_loc.y) != EMPTY)
	{
		Land[new_loc.x][new_loc.y].piece = EMPTY;
		return 1; 
	}
	else
		return 0;
}

void Map::initMap(void)
{
	for (int i = 0; i < MAX_WIDTH; i++)
		for (int j = 0; j < MAX_HEIGHT; j++)
			Land[i][j].init();
}

/*---------------------------Mapsquare function definitions-----------------------------*/


void Mapsquare::init()
{
	altitude = 0;
	forest = false;
	land_terrain_type = -1;
	terrain = SEA;
	river = NORIVER;
	explored = false;
	resource = 0;
	piece = NO_PIECE;
}

Mapsquare::Mapsquare(void)
{
	terrain = rand() % NUM_TERRAIN_TYPES;
	random_number = rand() % NUM_RANDOM_TYPES;
	explored = true;
	forest = false;
	river = NORIVER;
	if (terrain == LAND)
	{
		altitude = rand() % 100;
		land_terrain_type  = rand() % 6;
	}
	else
		land_terrain_type = -1;
	resource = 0;
	piece = NO_PIECE;
	recently_seen = false;
}

Mapsquare::Mapsquare(int terra_type)
{
	terrain = terra_type;
	random_number = rand() % NUM_RANDOM_TYPES;
	explored = false;
	recently_seen = false;
	forest = false;
	if (terrain == LAND)
	{	
		altitude = rand() % 100;
		land_terrain_type  = rand() % 6;
	}
	else
		land_terrain_type = -1;
}

Mapsquare::~Mapsquare(void)
{
}

void Mapsquare::copyTerrain(Mapsquare *sourceSquare)
{
	//copy data members from source to current instance
	this->terrain = sourceSquare->terrain;
	this->land_terrain_type = sourceSquare->land_terrain_type;
	this->random_number = sourceSquare->random_number;
	this->altitude = sourceSquare->altitude;
	this->explored = sourceSquare->explored;
	this->recently_seen = sourceSquare->recently_seen;
	this->forest = sourceSquare->forest;
	this->river = sourceSquare->river;
	this->resource = sourceSquare->resource;
	//don't copy piece data, as this is a terrain related copy only
	//this->piece = sourceSquare->piece;
}

void Mapsquare::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << terrain << land_terrain_type << random_number << altitude << river
		   << resource << piece;
		ar << explored << recently_seen << forest;
	}
	else
	{
		ar >> terrain >> land_terrain_type >> random_number >> altitude >> river
		   >> resource >> piece;
		ar >> explored >> recently_seen >> forest;
	}
}
