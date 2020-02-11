#include "StdAfx.h"
#include "map.h"
#include "Paint1.h"

#include "Paint1Doc.h"
#include "Paint1View.h"

#define DEST_vh           (DWORD)0x00AA0029  /* dest = dest               */


Map::Map(void)
{
	initMessages();

	//tempory code for the 'explorer' development/testing phase
	active_unit = 1;
	active_loc_x = 5;
	active_loc_y = 5;

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
	MAP_WIDTH  = 100;
	MAP_HEIGHT = 100;

	int count;

	display_x = 0;
	display_y = 0;

	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			Land[i][j].altitude = rand() % 100;

			if (Land[i][j].altitude > SEALEVEL)
				Land[i][j].terrain = LAND;
			else 
				Land[i][j].terrain = SEA;
			//Land[i][j].random_number = rand() % NUM_RANDOM_TYPES;
			Land[i][j].explored = false;

		}
	}
	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 1)
			&&  (j > 1)
			&&  (i < MAP_WIDTH - 2)
			&&  (j < MAP_HEIGHT - 2))
			{
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
					count++;
				if (Land[i-1][j].terrain == LAND)
					count++;
				if (Land[i-1][j+1].terrain == LAND)
					count++;
				if (Land[i][j-1].terrain == LAND)
					count++;
				if (Land[i][j].terrain == LAND)
					count++;
				if (Land[i+1][j+1].terrain == LAND)
					count++;
				if (Land[i+1][j-1].terrain == LAND)
					count++;
				if (Land[i+1][j].terrain == LAND)
					count++;
				if (count > 3)
					if (rand() % 2)
						Land[i][j].terrain = LAND;
				//if (count < 3)
				//	Land[i][j].terrain = SEA;
			}
			else
			{
				Land[i][j].terrain = SEA;
			}
		}
	}
	for (int i=2; i < MAP_WIDTH - 2; i++)
	{
		for(int j=2; j < MAP_HEIGHT - 2; j++)
		{
			count  = 0;
			if (Land[i-1][j].terrain == LAND)
				count++;
			if (Land[i+1][j].terrain == LAND)
				count++;
			if (Land[i][j+1].terrain == LAND)
				count++;
			if (Land[i][j-1].terrain == LAND)
				count++;
			if (count == 4)
				if (rand() % 15)
					Land[i][j].terrain = LAND;
		}
	}

	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 1)
			&&  (j > 1)
			&&  (i < MAP_WIDTH - 2)
			&&  (j < MAP_HEIGHT - 2))
			{
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
					count++;
				if (Land[i-1][j].terrain == LAND)
					count++;
				if (Land[i-1][j+1].terrain == LAND)
					count++;
				if (Land[i][j-1].terrain == LAND)
					count++;
				if (Land[i][j].terrain == LAND)
					count++;
				if (Land[i+1][j+1].terrain == LAND)
					count++;
				if (Land[i+1][j-1].terrain == LAND)
					count++;
				if (Land[i+1][j].terrain == LAND)
					count++;
				if (count > 3)
					if (rand() % 2)
						Land[i][j].terrain = LAND;
				//if (count < 3)
				//	Land[i][j].terrain = SEA;
			}
			else
			{
				Land[i][j].terrain = SEA;
			}
		}
	}

	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 1)
			&&  (j > 1)
			&&  (i < MAP_WIDTH - 2)
			&&  (j < MAP_HEIGHT - 2))
			{
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
					count++;
				if (Land[i-1][j].terrain == LAND)
					count++;
				if (Land[i-1][j+1].terrain == LAND)
					count++;
				if (Land[i][j-1].terrain == LAND)
					count++;
				if (Land[i][j].terrain == LAND)
					count++;
				if (Land[i+1][j+1].terrain == LAND)
					count++;
				if (Land[i+1][j-1].terrain == LAND)
					count++;
				if (Land[i+1][j].terrain == LAND)
					count++;
				if (count > 3)
					if (rand() % 2)
						Land[i][j].terrain = LAND;
				//if (count < 3)
				//	Land[i][j].terrain = SEA;
			}
			else
			{
				Land[i][j].terrain = SEA;
			}
		}
	}
	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 15)
			&&  (j > 10)
			&&  (i < MAP_WIDTH - 15)
			&&  (j < MAP_HEIGHT - 10))
			{
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
					count++;
				if (Land[i-1][j].terrain == LAND)
					count++;
				if (Land[i-1][j+1].terrain == LAND)
					count++;
				if (Land[i][j-1].terrain == LAND)
					count++;
				if (Land[i][j].terrain == LAND)
					count++;
				if (Land[i+1][j+1].terrain == LAND)
					count++;
				if (Land[i+1][j-1].terrain == LAND)
					count++;
				if (Land[i+1][j].terrain == LAND)
					count++;
				if (count > 2)
					if (rand() % 5)
						Land[i][j].terrain = LAND;
				//if (count < 3)
				//	Land[i][j].terrain = SEA;
			}
			else
			{
				if (!(rand() % 3))
					Land[i][j].terrain = SEA;
			}
		}
	}

	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 15)
			&&  (j > 10)
			&&  (i < MAP_WIDTH - 15)
			&&  (j < MAP_HEIGHT - 10))
			{
				count = 0;
				if (Land[i-1][j-1].terrain == LAND)
					count++;
				if (Land[i-1][j].terrain == LAND)
					count++;
				if (Land[i-1][j+1].terrain == LAND)
					count++;
				if (Land[i][j-1].terrain == LAND)
					count++;
				if (Land[i][j].terrain == LAND)
					count++;
				if (Land[i+1][j+1].terrain == LAND)
					count++;
				if (Land[i+1][j-1].terrain == LAND)
					count++;
				if (Land[i+1][j].terrain == LAND)
					count++;
				if (count > 2)
					if (rand() % 5)
						Land[i][j].terrain = LAND;
				if (count == 0)
					if (rand() % 5)
						Land[i][j].terrain = SEA;
			}
			else
			{
				if (rand() % 3)
					Land[i][j].terrain = SEA;
			}
		}
	}


	//break up long flat coastlines with an extra landsquare
	int left_side, top_side, bottom_side, right_side;
	for (int i=0; i<MAP_WIDTH; i++)
	{
		for(int j=0; j<MAP_HEIGHT; j++)
		{
			if ((i > 3)
			&&  (j > 3)
			&&  (i < MAP_WIDTH - 3)
			&&  (j < MAP_HEIGHT - 3)
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
						if ((rand() % 3) == 0)
							Land[i][j].terrain = LAND;
					//	else
					//		if (rand() % 3 == 0)

				}
			}
		}
	}

	//create a boundary of sea squares, 2 spaces wide, around the map
	int boundary_width = 2;
	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			count = 0;
			if ((i < boundary_width)
			||  (j < boundary_width)
			||  (i >= MAP_WIDTH - boundary_width)
			||  (j >= MAP_HEIGHT - boundary_width) )
				Land[i][j].terrain = SEA;
			else
			{
			if (Land[i-1][j].terrain == SEA)
				count++;
			if (Land[i+1][j].terrain == SEA)
				count++;
			if (Land[i][j+1].terrain == SEA)
				count++;
			if (Land[i][j-1].terrain == SEA)
				count++;
			if (count == 4)
				if (rand() % 15)
					Land[i][j].terrain = SEA;
			}
		}
	}
	//remove all 'diagonally adjacent only' occurances
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
	//update all sea squares to have an altitude of 25
	initAltitudes();
	//assign a terrain type to each LAND square
	initLandTerrainTypes();
	//assign forest values
	initForests();

	if (!(randomMap(100, 100, 50, 50, 80, 50, 60, 50, 50)))
		;//make some error messsage

	//tempory code for the 'explorer' development/testing phase
	active_unit = 1;
	active_loc_x = 0;
	active_loc_y = 0;

	Land[0][0].terrain = SEA;
	Land[0][0].altitude = 25;
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
	int i, j, cycles, count, x, y;

	if ((size_x > MAX_WIDTH) || (size_y > MAX_HEIGHT)
	||  (size_x < 0) || (size_y < 0))
		return 5;

	MAP_WIDTH   = size_x;
	MAP_HEIGHT  = size_y;

	if ((temperature > 100) || (temperature < 0)
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
	mland_points = land_points;

	//set initial values
	for (i=0; i < MAP_WIDTH; i++)
		for (j=0; j < MAP_HEIGHT; j++)
		{
			Land[i][j].altitude = 0;
			Land[i][j].forest = false;
			Land[i][j].land_terrain_type = 0;
			Land[i][j].terrain = SEA;
		}

	//seed map with inital land points
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

	//cycle to produce bigger land masses (land_size?)
	for (cycles = 0; cycles < 10; cycles++)
		for (i=0; i < MAP_WIDTH; i++)
			for (j=0; (j < MAP_HEIGHT); j++)
			{
				count = u_landcount(i, j);
				if (count > 0)
				{
					if (  ((rand() % 8) < count)
					 &&   ((rand() % 8) < count) )
					{
						Land[i][j].terrain  = LAND;
						Land[i][j].altitude = (rand() % 20) + 80;
					}
				}
			}
/*
	//cycle to produce continent
	x = (MAP_WIDTH * mcont_size)  / 100;
	y = (MAP_HEIGHT * mcont_size) / 100;
	for (cycles = (MAP_WIDTH - x); cycles < x; cycles++)
		for (i = MAP_WIDTH - x; i < x; i++)
		{
			for(j = MAP_HEIGHT - y; j < y; j++)
			{
					count = u_landcount(i, j);
					if   (count > 1)
					{
					    if  ( (i > (MAP_WIDTH - x) + 3)
					    &&	  (j > (MAP_HEIGHT - y) + 3)
					    &&    (i < x - 3)
					    &&    (j < y - 3) ) 
						{
							if (rand() % 8 < count)
							{
								Land[i][j].terrain = LAND;
								Land[i][j].altitude = (rand() % 50) + 50;
							}
						}
						else
							if (!(rand() % 5))
							{
								Land[i][j].terrain = LAND;
								Land[i][j].altitude = (rand() % 50) + 50;
							}
							if (!(rand() % 5))
							{
								Land[i][j].terrain = SEA;
								Land[i][j].altitude = rand() % 25;
							}
					}		
					else
					{
						if (!(rand() % 5)
						&&   (count < 7)
						&&   (cycles > 3))
						{
							Land[i][j].terrain = SEA;
							Land[i][j].altitude = (rand() % 25);
						}
					}
			}
		}
		*/
	// create an extra loop, where those close to the continent boundary are
	// exempted, to create a more variable coastline.

	x = (MAP_WIDTH * mcont_size)  / 100;
	y = (MAP_HEIGHT * mcont_size) / 100;
	for (cycles = (MAP_WIDTH - x); cycles < (x / 2); cycles++)
		for (i = MAP_WIDTH - x; i < x; i++)
		{
			for(j = MAP_HEIGHT - y; j < y; j++)
			{
				count = u_landcount(i, j);
				if (count > 0)
				{
					if (  ((rand() % 8) < count)
					 &&   ((rand() % 8) < count) )
					{
						Land[i][j].terrain  = LAND;
						Land[i][j].altitude = (rand() % 20) + 80;
					}
				}
			}
		}

	for (cycles = (MAP_WIDTH - x); cycles < (x / 2); cycles++)
		for (i = (MAP_WIDTH - x) + 6; i < x - 6; i++)
		{
			for(j = (MAP_HEIGHT - y) + 6; j < y - 6; j++)
			{
				count = u_landcount(i, j);
				if (count > 0)
				{
					if (  ((rand() % 8) < count)
					 &&   ((rand() % 8) < count) )
					{
						Land[i][j].terrain  = LAND;
						Land[i][j].altitude = (rand() % 20) + 80;
					}
				}
			}
		}

	//create sea border, for east, north west only....
	int boundary_width = 2;
	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			count = 0;
			if ((i < boundary_width)
			||  (j < boundary_width)
			||  (i >= MAP_WIDTH - boundary_width) )
			//||  (j >= MAP_HEIGHT - boundary_width) )
			{
				Land[i][j].terrain  = SEA;
				Land[i][j].altitude = 25;
			}
		}
	}

	//create antarctica (extra squares on bottom of map

	//remove diagonally adjacent land squares
	//void removeDiagonals();

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
	int tropics   = ((MAP_HEIGHT * 3) / 10);
	int temperate = ((MAP_HEIGHT * 6) / 10);  
	int antarctic  = ((MAP_HEIGHT * 9) / 10);

	int r_cold[3] = {1, 2, 7};
	int r_temp[6] = {0, 1, 2, 4, 5, 8};
	int r_trop[4] = {0, 5, 6, 8};

	for (int i=0; i < MAP_WIDTH; i++)
	{
		for(int j=0; j < MAP_HEIGHT; j++)
		{
			if (Land[i][j].terrain == LAND)
			{
				if ( j >= antarctic)
					Land[i][j].land_terrain_type = 3;
				else if ( j >= temperate )
					Land[i][j].land_terrain_type = r_cold[rand() % 3];
				else if ( j >= tropics )
					Land[i][j].land_terrain_type = r_temp[rand() % 6];
				else 
					Land[i][j].land_terrain_type = r_trop[rand() % 4];
			}
			else
				Land[i][j].land_terrain_type = -1;
		}
	}
	

	//seed with forests
	initForests();

	//cycle to cluster forests

	//add rivers
	return 0;
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
				Land[i][j].altitude = (rand() % (100 - SEALEVEL)) + 75;
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
			if ( (Land[i][j].terrain != SEA)
			&&   (Land[i][j].altitude < HILLLINE)
			&&   (Land[i][j].land_terrain_type != 3)  
			&&   (Land[i][j].land_terrain_type != 4) ) 
				if ((rand() % 5))
					Land[i][j].forest = true;
		}
	}
}

Mapsquare::Mapsquare(void)
{
	terrain = rand() % NUM_TERRAIN_TYPES;
	random_number = rand() % NUM_RANDOM_TYPES;
	explored = false;
	forest = false;
	if (terrain == LAND)
	{
		altitude = rand() % 100;
		land_terrain_type  = rand() % 6;
	}
	else
		land_terrain_type = -1;
}

Mapsquare::Mapsquare(int terra_type)
{
	terrain = terra_type;
	random_number = rand() % NUM_RANDOM_TYPES;
	explored = false;
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

int Map::ShowMap(CDC *pDC)
{
	int return_value = 0;

	BitmapStore col_graphics;
	BitmapStore* map_graphics;
	map_graphics = &col_graphics;
	//player1Map.map_graphics = &col_graphics;
	//map_graphics = &col_graphics;

	//set up a memory DC for the map area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, 48 * DISP_WIDTH, 48 * DISP_HEIGHT);
	memDC.SelectObject(&memBitmap);

	//set up a memory DC for a single square
	CDC memSQ;
	memSQ.CreateCompatibleDC(pDC);
	CBitmap memBackSq;
	memBackSq.CreateCompatibleBitmap(pDC, SQ_WIDTH, SQ_HEIGHT);
	memSQ.SelectObject(&memBackSq);
	BITMAP bmSquare;
	memBackSq.GetObject(sizeof(BITMAP), &bmSquare);
	
	CDC dcMem1 ,dcMem2;
	dcMem1.CreateCompatibleDC(pDC); 
	dcMem2.CreateCompatibleDC(pDC);

	//set up terrain square size paramaters (48x48)
	BITMAP bm1;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);


	pDC->SelectObject(&map_graphics->mBitmap1);

	//only if a move key has been pressed
	if (move_key_pressed)
	{
		updateExplored();
		reCenter_active();
	}

	int i, j, x_value, y_value;

	//map class ensures that display_x, display_y can never be out of bounds...
	int display_loc_x = display_x;
	int display_loc_y = display_y;

	for (i=0; i < DISP_WIDTH; i++)
	{
		x_value = (i)*48;
		for (j=0; j < DISP_HEIGHT; j++)										
		{
			y_value = (j)*48;
			switch(Land[i + display_loc_x][j + display_loc_y].terrain)
			{
				case LAND:
					//determine land terrain type to display
					int terr_type_loc;
					int terr_type_above, terr_type_below, terr_type_left, terr_type_right;
					 
					//select terrain types
					terr_type_loc = 
						(Land[i + display_loc_x][j + display_loc_y].land_terrain_type) * 50;
					if  ((i + display_loc_x ) > 0)
						terr_type_left = (Land[i + display_loc_x - 1][j + display_loc_y].land_terrain_type) * 50;
					else
						terr_type_left = terr_type_loc;
					if  ((j + display_loc_y) > 0)
						terr_type_above = (Land[i + display_loc_x][j + display_loc_y - 1].land_terrain_type) * 50;
					else
						terr_type_above = terr_type_loc;
					if  ((i + display_loc_x ) < (MAP_WIDTH  - 2))
						terr_type_right = (Land[i + display_loc_x + 1][j + display_loc_y].land_terrain_type) * 50;
					else
						terr_type_right = terr_type_loc;
					if  ((j + display_loc_y ) < (MAP_HEIGHT - 2))
						terr_type_below = (Land[i + display_loc_x][j + display_loc_y + 1].land_terrain_type) * 50;
					else
						terr_type_below = terr_type_loc;
					//if adjacent squares were sea squares, then reset 
					if (terr_type_left == -50)
						terr_type_left = terr_type_loc;
					if (terr_type_above == -50)
						terr_type_above = terr_type_loc;
					if (terr_type_right == -50)
						terr_type_right = terr_type_loc;
					if (terr_type_below == -50)
						terr_type_below = terr_type_loc;

					
					dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
					//dcMem2.SelectObject(&map_graphics->mBitmap1);

					//bitblt the locations terrain type first
					memDC.BitBlt(x_value, y_value, 
						         bm1.bmWidth, bm1.bmHeight, 
								 &dcMem1, 0, terr_type_loc, SRCCOPY);

					//maskblt the above terrain type
					if (!(memDC.MaskBlt(x_value, y_value, 
					                    bm1.bmWidth, bm1.bmHeight, 
					                    &dcMem1, 
										0, terr_type_above, 
										map_graphics->Terr_blend_mask, 
									    0, 0, 
										MAKEROP4(SRCCOPY, DEST_vh))))
					{
						return_value = 1;
						return return_value;
					}
					//maskblt the below terrain type
					if (!(memDC.MaskBlt(x_value, y_value, 
					                    bm1.bmWidth, bm1.bmHeight, 
					                    &dcMem1, 
										0, terr_type_below, 
										map_graphics->Terr_blend_mask, 
									    100, 0, 
										MAKEROP4(SRCCOPY, DEST_vh))))
					{
						return_value = 1;
						return return_value;
					}
					//maskblt the left terrain type
					if (!(memDC.MaskBlt(x_value, y_value, 
					                    bm1.bmWidth, bm1.bmHeight, 
					                    &dcMem1, 
										0, terr_type_left, 
										map_graphics->Terr_blend_mask, 
									    150, 0, 
										MAKEROP4(SRCCOPY, DEST_vh))))
					{
						return_value = 1;
						return return_value;
					}
					//maskblt the right terrain type
					if (!(memDC.MaskBlt(x_value, y_value, 
					                    bm1.bmWidth, bm1.bmHeight, 
					                    &dcMem1, 
										0, terr_type_right, 
										map_graphics->Terr_blend_mask, 
									    50, 0, 
										MAKEROP4(SRCCOPY, DEST_vh))))
					{
						return_value = 1;
						return return_value;
					}
/*
					if ((getBackingSquare(i + display_loc_x, 
						                  j + display_loc_y, 
										  &bmSquare, 
										  map_graphics,
										  pDC)) != 0)
					{
						//non zero, so a failure has occurred
						return 3;
					}
					memSQ.SelectObject(&memBackSq);
					//memBackSq = map_graphics->mBitmap1;
					//memSQ.BitBlt(0, 0, bm1.bmWidth, bm1.bmHeight, 
					//	             &dcMem1, 0, 0, SRCCOPY);
					memDC.BitBlt(x_value, y_value, 
						         bm1.bmWidth, bm1.bmHeight, 
								 &memSQ, 0, 0, SRCCOPY);
*/
					//maskblt the location square's terrain type over the top
					if (!(memDC.MaskBlt(x_value, y_value, 
					                    bm1.bmWidth, bm1.bmHeight, 
					                    &dcMem1, 
										0, terr_type_loc, 
										map_graphics->Terr_blend_mask, 
									    200, 0, 
										MAKEROP4(DEST_vh, SRCCOPY))))
					{
						return_value = 1;
						return return_value;
					}
					
					//memDC.BitBlt(x_value, y_value, 
					//	         bm1.bmWidth, bm1.bmHeight, 
					//			 &dcMem1, 0, terr_type_loc, SRCCOPY);

					//if the altitude is above the snowline, then blt a mountain
					
					if (Land[i + display_loc_x][j + display_loc_y].altitude > SNOWLINE)
					{
						dcMem1.SelectObject(&map_graphics->mountains);
						int mount_score = u_mountScore(i + display_loc_x, j + display_loc_y);
						/*
						if (!(memDC.MaskBlt(x_value, y_value, 
					                        bm1.bmWidth, bm1.bmHeight, 
					                        &dcMem1, 
								    		0, mount_table[mount_score], 
							    			map_graphics->mountains_mask, 
								    	    0, mount_table[mount_score], 
									    	MAKEROP4(SRCCOPY, DEST_vh))))
*/
						if (!(MyMaskBlt(    memDC.GetSafeHdc(),
							                x_value, y_value, 
					                        bm1.bmWidth, bm1.bmHeight, 
											dcMem1.GetSafeHdc(), 
								    		0, mount_table[mount_score], 
							    			map_graphics->mountains_mask, 
								    	    0, mount_table[mount_score], 
									    	MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
					}
					//if the square is forested, then blt a forest
					if (Land[i + display_loc_x][j + display_loc_y].forest)
					{
						dcMem1.SelectObject(&map_graphics->forests);
						int forest_score = u_forestScore(i + display_loc_x, j + display_loc_y);
						if (!(memDC.MaskBlt(x_value, y_value, 
					                        bm1.bmWidth, bm1.bmHeight, 
					                        &dcMem1, 
								    		0, forest_table[forest_score], 
							    			map_graphics->forests_mask, 
								    	    0, forest_table[forest_score], 
									    	MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
					}
					//if the altitude is sufficient, then blt a hill (if not forested)
					if ( (Land[i + display_loc_x][j + display_loc_y].altitude > HILLLINE)
					&&   (Land[i + display_loc_x][j + display_loc_y].altitude < SNOWLINE)
					&&   (!(Land[i + display_loc_x][j + display_loc_y].forest)))
					{
						dcMem1.SelectObject(&map_graphics->hills);
						int hill_score = u_hillScore(i + display_loc_x, j + display_loc_y);
						if (!(memDC.MaskBlt(x_value, y_value, 
					                        bm1.bmWidth, bm1.bmHeight, 
					                        &dcMem1, 
								    		0, mount_table[hill_score], 
							    			map_graphics->hills_mask, 
								    	    0, mount_table[hill_score], 
									    	MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
					}

					break;

				case SEA:
								
					int coastal_score;
					coastal_score = coastalScore(i + display_loc_x, j + display_loc_y);
					
					//no adjacent land squares, so just paint a plain sea square
					if (coastal_score == 0)
					{
						dcMem2.SelectObject(&map_graphics->mBmSea);
						memDC.BitBlt(x_value, y_value, bm1.bmWidth, bm1.bmHeight, &dcMem2, 0, 0, SRCCOPY);
					}
					//one or more adjacent land squares
					else
					{
						//bitblt the appropriate land square(s) first
						dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
						//memDC.BitBlt(x_value, y_value, bm1.bmWidth, bm1.bmHeight, &dcMem1, 0, 0, SRCCOPY);
						//use a four way mask to select terrain for each 'pie'
						int terr_type_above, terr_type_below, terr_type_left, terr_type_right;
					    int terr_type_first = 0;
						//select terrain types
						if  ((i + display_loc_x ) > 0)
							terr_type_left = (Land[i + display_loc_x - 1][j + display_loc_y].land_terrain_type) * 50;
						else
							terr_type_left = terr_type_loc;
						if  ((j + display_loc_y) > 0)
							terr_type_above = (Land[i + display_loc_x][j + display_loc_y - 1].land_terrain_type) * 50;
						else
							terr_type_above = terr_type_loc;
						if  ((i + display_loc_x ) < (MAP_WIDTH  - 2))
							terr_type_right = (Land[i + display_loc_x + 1][j + display_loc_y].land_terrain_type) * 50;
						else
							terr_type_right = terr_type_loc;
						if  ((j + display_loc_y ) < (MAP_HEIGHT - 2))
							terr_type_below = (Land[i + display_loc_x][j + display_loc_y + 1].land_terrain_type) * 50;
						else
							terr_type_below = terr_type_loc;
						//find a default terrain type for edges with sea squares adjacent 
						if (terr_type_left != -50)
							terr_type_first = terr_type_left;
						if (terr_type_above != -50)
							terr_type_first = terr_type_above;
						if (terr_type_right != -50)
							terr_type_first = terr_type_right;
						if (terr_type_below != -50)
							terr_type_first = terr_type_below;
						//if adjacent squares were sea squares, then reset 
						if (terr_type_left == -50)
							terr_type_left = terr_type_first;
						if (terr_type_above == -50)
							terr_type_above = terr_type_first;
						if (terr_type_right == -50)
							terr_type_right = terr_type_first;
						if (terr_type_below == -50)
							terr_type_below = terr_type_first;
			
						dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);


						//first Bitblt a default terrain type we know is adjacent to this square
						memDC.BitBlt(x_value, y_value, bm1.bmWidth, bm1.bmHeight, &dcMem1, 0, terr_type_first, SRCCOPY);

						//maskblt the above terrain type
						if (!(memDC.MaskBlt(x_value, y_value, 
							                bm1.bmWidth, bm1.bmHeight, 
								            &dcMem1, 
											0, terr_type_above, 
											map_graphics->Terr_blend_mask, 
											0, 0, 
											MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
						//maskblt the below terrain type
						if (!(memDC.MaskBlt(x_value, y_value, 
							                bm1.bmWidth, bm1.bmHeight, 
								            &dcMem1, 
											0, terr_type_below, 
											map_graphics->Terr_blend_mask, 
											100, 0, 
											MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
						//maskblt the left terrain type
						if (!(memDC.MaskBlt(x_value, y_value, 
							                bm1.bmWidth, bm1.bmHeight, 
								            &dcMem1, 
											0, terr_type_left, 
											map_graphics->Terr_blend_mask, 
											150, 0, 
											MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
						//maskblt the right terrain type
						if (!(memDC.MaskBlt(x_value, y_value, 
							                bm1.bmWidth, bm1.bmHeight, 
								            &dcMem1, 
											0, terr_type_right, 
											map_graphics->Terr_blend_mask, 
											50, 0, 
											MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
						//maskblt the location square's terrain type over the top
						//if (!(memDC.MaskBlt(x_value, y_value, 
					    //                bm1.bmWidth, bm1.bmHeight, 
					    //                &dcMem1, 
						//				0, terr_type_loc, 
						//				map_graphics->Terr_blend_mask, 
						//			    200, 0, 
						//				MAKEROP4(DEST_vh, SRCCOPY))))
						//{
						//	return_value = 1;
						//	return return_value;
						//}

						//now select the x offsets, based on the lookup table indexed by 'coastal score'
						int coast_offset_x = coast_table[coastal_score];
						//now select the y offsets, based on coastal score and random map value
						int coast_offset_y = ((Land[i + display_loc_x ][j + display_loc_y].random_number) % 4) * 50;
						//modify variation offset, as some coast types have less than 4
						if ((coastal_score == 11)
						||  (coastal_score == 13)
						||  (coastal_score == 14)
						||  (coastal_score == 7))
							coast_offset_y = 0;
						if ((coastal_score == 10)
						||  (coastal_score == 15))
							coast_offset_y = ((Land[i + display_loc_x ][j + display_loc_y].random_number) % 2) * 50;
						//now maskblt
						dcMem2.SelectObject(&map_graphics->coast);
						if (!(memDC.MaskBlt(x_value, y_value, 
						                    bm1.bmWidth, bm1.bmHeight, 
						                    &dcMem2, 
											coast_offset_x, coast_offset_y, 
											map_graphics->coast_mask, 
										    coast_offset_x, coast_offset_y, 
											MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
							return return_value;
						}
					}
				break;
			}

			//if the square has not been explored, then paint the undiscovered bitmap over it 
            BOOL above_explored, below_explored, left_explored, right_explored;
			above_explored = FALSE;
			below_explored = FALSE;
			left_explored = FALSE;
			right_explored = FALSE;

			if (!(Land[i + display_loc_x][j + display_loc_y].explored))
			{
				dcMem2.SelectObject(&map_graphics->undiscovered);

				//draw the central area unmasked
				memDC.BitBlt(x_value + 4, y_value + 4, bm1.bmWidth - 8, bm1.bmHeight - 8, &dcMem2, 4, 4, SRCCOPY);

				//for each adjacent square check to see it if is explored
				//if so draw with the mask, which will hint at the type of terrainn underneath
				//and record the status for future reference

				if ((j + display_loc_y > 0)
				&&	(Land[i + display_loc_x][j + display_loc_y - 1].explored) )
				{
					if (!(memDC.MaskBlt(x_value, y_value, 
						                bm1.bmWidth, bm1.bmHeight - 44, 
										&dcMem2, 0, 0, map_graphics->undisc_mask, 
										0, 0, MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					above_explored = TRUE;
				}
				
				if ((j + display_loc_y < MAP_HEIGHT - 1)
				&&	(Land[i + display_loc_x][j + display_loc_y + 1].explored) )
				{
					if (!(memDC.MaskBlt(x_value, y_value + 44, 
						                bm1.bmWidth, bm1.bmHeight - 44, 
										&dcMem2, 0, 0, map_graphics->undisc_mask, 
										100, 44, MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					below_explored = TRUE;
				}
		
				if ((i + display_loc_x > 0)
				&&	(Land[i + display_loc_x - 1][j + display_loc_y].explored) )
				{
					if (!(memDC.MaskBlt(x_value, y_value, 
						                bm1.bmWidth - 44, bm1.bmHeight, 
										&dcMem2, 0, 0, map_graphics->undisc_mask, 
										150, 0, MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					left_explored = TRUE;
				}			
				
				if ((i + display_loc_x < MAP_WIDTH - 1)
				&&	(Land[i + display_loc_x + 1][j + display_loc_y].explored) )
				{
					if (!(memDC.MaskBlt(x_value + 44, y_value, bm1.bmWidth - 44, 
						                bm1.bmHeight, &dcMem2, 0, 0, 
										map_graphics->undisc_mask, 
										50 + 44, 0, MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					right_explored = TRUE;
				}

				//where the adjacent square is not explored draw unmasked version
				int left_offset, right_offset;
				left_offset = right_offset = 0;
				if (!(above_explored))
				{
					if (left_explored)
						left_offset = 4;
					if (right_explored)
						right_offset = 4;
	                memDC.BitBlt(x_value + left_offset, y_value, 
						         bm1.bmWidth - (left_offset + right_offset), 
								 bm1.bmHeight - 44, &dcMem2, 0, 0, SRCCOPY);
				}
				
				left_offset = right_offset = 0;
				if (!(below_explored))
				{
					if (left_explored)
						left_offset = 4;
					if (right_explored)
						right_offset = 4;
	                memDC.BitBlt(x_value + left_offset, y_value + 44, 
						         bm1.bmWidth - (left_offset + right_offset), 
								 bm1.bmHeight - 44, &dcMem2, 0, 0, SRCCOPY);
				}

				int above_offset, below_offset;
				above_offset = below_offset = 0;
				if (!(left_explored))
				{
					if (above_explored)
						above_offset = 4;
					if (below_explored)
						below_offset = 4;
	                memDC.BitBlt(x_value, y_value + above_offset, 
						         bm1.bmWidth - 44, 
								 bm1.bmHeight - (above_offset + below_offset), 
								 &dcMem2, 0, 0, SRCCOPY);
				}

				above_offset = below_offset = 0;
				if (!(right_explored))
				{
					if (above_explored)
						above_offset = 4;
					if (below_explored)
						below_offset = 4;
	                memDC.BitBlt(x_value + 44, y_value + above_offset, 
						         bm1.bmWidth - 44, 
								 bm1.bmHeight - (above_offset + below_offset), 
								 &dcMem2, 0, 0, SRCCOPY);
				}
			}
		}
	}
	
	dcMem2.SelectObject(&map_graphics->caravel);
	
	if ((active_loc_x >= display_x)
	&&  (active_loc_x < (display_x + DISP_WIDTH))
	&&  (active_loc_y >= display_y)
	&&  (active_loc_y < (display_y + DISP_HEIGHT)))
	{
		if (!(memDC.MaskBlt((active_loc_x - display_x) * 48, 
			                (active_loc_y - display_y) * 48, 
			                bm1.bmWidth, bm1.bmHeight, 
							&dcMem2, 0, 0, 
							map_graphics->mask, 0, 0, MAKEROP4(DEST_vh, SRCCOPY))))
		{
			return_value = 2;
		}
	}

	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(0, 0, bm1.bmWidth * DISP_WIDTH, 
		        bm1.bmHeight * DISP_HEIGHT, 
				&memDC, 0, 0, SRCCOPY);

	//re-initialise Map level drawing paramaters
	move_key_pressed = false;

	return return_value;
}

void Map::updateExplored()
{
	//ensure the active unit has all squares within it's line of sight set to explored
	Land[active_loc_x][active_loc_y].explored = true;
	int line_of_sight = 3;

	for (int i = active_loc_x - line_of_sight; 
		     i <= active_loc_x + line_of_sight; 
			 i++)
		for (int j = active_loc_y - line_of_sight; 
			 j <= active_loc_y + line_of_sight; j++)
			 if ((i >= 0) && (j >=0) 
			 &&  (i < MAP_WIDTH) && (j < MAP_HEIGHT))
				Land[i][j].explored = true;
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
	//evaluate coastal score
	int mount_score = 0;
	if (y > 0)
		if (Land[x][y-1].altitude > SNOWLINE)
			mount_score += 8;
	if (y< MAP_HEIGHT - 2)
		if (Land[x][y+1].altitude > SNOWLINE)
			mount_score += 2;
	if (x > 0)
		if (Land[x-1][y].altitude > SNOWLINE)
			mount_score += 1;
	if (x < MAP_WIDTH - 2)
		if (Land[x+1][y].altitude > SNOWLINE)
			mount_score += 4;
	return mount_score;
}

int Map::u_landcount(int x, int y)
{

	int radius = 1;
	int count  = 0;

	for (int i = x - radius; 
		     i <= x + radius; 
			 i++)
		for (int j = y - radius; 
			 j <= y + radius; j++)
			 if ((i >= 0) && (j >=0) 
			 &&  (i < MAP_WIDTH) && (j < MAP_HEIGHT))
				if (Land[i][j].terrain == LAND)
					count++;

	return count;
}

int Map::u_hillScore(int x, int y)
{
	//evaluate coastal score
	int hill_score = 0;
	if (y > 0)
		if ((Land[x][y-1].altitude > HILLLINE)
		&&  (Land[x][y-1].altitude < SNOWLINE))
			hill_score += 8;
	if (y< MAP_HEIGHT - 2)
		if ((Land[x][y+1].altitude > HILLLINE)
		&&  (Land[x][y+1].altitude < SNOWLINE))
			hill_score += 2;
	if (x > 0)
		if ((Land[x-1][y].altitude > HILLLINE)
		&&  (Land[x-1][y].altitude < SNOWLINE))
			hill_score += 1;
	if (x < MAP_WIDTH - 2)
		if ((Land[x+1][y].altitude > HILLLINE)
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
	if (y< MAP_HEIGHT - 2)
		if (Land[x][y+1].forest)
			forest_score += 2;
	if (x > 0)
		if (Land[x-1][y].forest)
			forest_score += 1;
	if (x < MAP_WIDTH - 2)
		if (Land[x+1][y].forest)
			forest_score += 4;

	return forest_score;
}

int Map::getBackingSquare(int x, int y, BITMAP* bmSqPtr, 
						  BitmapStore* col_graphics, CDC* pDC)
{
	//paints a background terrain based on adjacent terrain types
	//it seems there is no way to pass a CBitmap.  Maybe just BITMAP????
	//forget about this, and keep going with other stuff....

	LONG bmWidth, bmHeight;

	//set up terrain square size paramaters (48x48)
	BITMAP bm1;
	BitmapStore the_graphics;
	BitmapStore* map_graphics;
	map_graphics = &the_graphics;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);
	bmWidth  = bm1.bmWidth;
	bmHeight = bm1.bmHeight;

	int x_value, y_value;
	x_value = y_value = 0;
	CBitmap bmCSq;
	//bmCSq.GetBitmap(bmSqPtr);

	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 
	dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC); 
	memDC.SelectObject(&bmCSq);

	memDC.BitBlt(x_value, y_value, bmWidth, bmHeight, &dcMem1, 0, 0, SRCCOPY);
	return 0;

	//use a four way mask to select terrain for each 'pie'
	int terr_type_above, terr_type_below, terr_type_left, terr_type_right;
    int terr_type_first;

	//select terrain types
	if  (x > 0)
		terr_type_left = (Land[x - 1][y].land_terrain_type) * 50;
	else
		terr_type_left = (Land[x][y].land_terrain_type) * 50;
	if  (y > 0)
		terr_type_above = (Land[x][y - 1].land_terrain_type) * 50;
	else
		terr_type_above = (Land[x][y].land_terrain_type) * 50;
	if  (x < (MAP_WIDTH  - 2))
		terr_type_right = (Land[x + 1][y].land_terrain_type) * 50;
	else
		terr_type_right = (Land[x][y].land_terrain_type) * 50;
	if  (y < (MAP_HEIGHT - 2))
		terr_type_below = (Land[x][y + 1].land_terrain_type) * 50;
	else
		terr_type_below = (Land[x][y].land_terrain_type) * 50;

	//find a default terrain type  
	if (terr_type_left != -50)
		terr_type_first = terr_type_left;
	if (terr_type_above != -50)
		terr_type_first = terr_type_above;
	if (terr_type_right != -50)
		terr_type_first = terr_type_right;
	if (terr_type_below != -50)
		terr_type_first = terr_type_below;

	//if adjacent squares were sea squares, then reset to the default terrain type
	if (terr_type_left == -50)
		terr_type_left = terr_type_first;
	if (terr_type_above == -50)
		terr_type_above = terr_type_first;
	if (terr_type_right == -50)
		terr_type_right = terr_type_first;
	if (terr_type_below == -50)
		terr_type_below = terr_type_first;

	terr_type_first = 0;
	//first Bitblt default terrain type to ensure no blank spots are left
	//(later this can be a full four way mask with no dithering?)
	memDC.BitBlt(x_value, y_value, bmWidth, bmHeight, &dcMem1, 0, terr_type_first, SRCCOPY);
/*
	//maskblt the above terrain type
	if (!(memDC->MaskBlt(x_value, y_value, 
		                 bmWidth, bmHeight, 
			             &dcMem1, 
						 0, terr_type_above, 
						 map_graphics->Terr_blend_mask, 
						 0, 0, 
						 MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return 1;
	}
	//maskblt the below terrain type
	if (!(memDC->MaskBlt(x_value, y_value, 
		                bmWidth, bmHeight, 
			            &dcMem1, 
						0, terr_type_below, 
						map_graphics->Terr_blend_mask, 
						100, 0, 
						MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return 1;
	}
	//maskblt the left terrain type
	if (!(memDC->MaskBlt(x_value, y_value, 
	                    bmWidth, bmHeight, 
			            &dcMem1, 
						0, terr_type_left, 
						map_graphics->Terr_blend_mask, 
						150, 0, 
						MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return 1;
	}
	//maskblt the right terrain type
	if (!(memDC->MaskBlt(x_value, y_value, 
		                bmWidth, bmHeight, 
			            &dcMem1, 
						0, terr_type_right, 
						map_graphics->Terr_blend_mask, 
						50, 0, 
						MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return 1;
	}*/
	return 0;
}

void Map::reCenter()
{
	//recenter to the right if needed
	if (active_loc_x >= display_x + DISP_WIDTH - 2)
	{
		display_x += DISP_WIDTH / 2;
		if ((display_x + DISP_WIDTH) >= MAP_WIDTH)
			display_x = MAP_WIDTH - DISP_WIDTH - 1;
	}

	//recenter to the left if needed
	if (active_loc_x < display_x)
		display_x -= DISP_WIDTH / 2;
		if (display_x < 0)
			display_x = 0 ;

	//recenter below if needed
	if (active_loc_y >= display_y + DISP_HEIGHT - 2)
		display_y += DISP_HEIGHT / 2;
		if (display_y + DISP_HEIGHT >= MAP_HEIGHT)
			display_y = MAP_HEIGHT - DISP_HEIGHT - 1;

	//recenter above if needed
	if (active_loc_y < display_y)
		display_y -= DISP_HEIGHT / 2;
		if (display_y < 0)
			display_y = 0 ;
}

void Map::reCenter_active()
{
	//recenter if active location is outside the display bounds 
	if ((active_loc_x >= display_x + DISP_WIDTH - 1)
	||  (active_loc_x < display_x + 1)
	||  (active_loc_y >= display_y + DISP_HEIGHT - 1)
	||  (active_loc_y < display_y + 1))
	{
		CPoint focus;
		focus.x = active_loc_x;
		focus.y = active_loc_y;
		reCenter_point(focus);
	}
}


void Map::reCenter_point(CPoint focus)
{
	//recenter the view on point provided
	display_x = focus.x - (DISP_WIDTH / 2);
	display_y = focus.y - (DISP_HEIGHT / 2);

	if (display_x < 0)
		display_x = 0;

	if ((display_x + DISP_WIDTH) >= MAP_WIDTH)
		display_x = MAP_WIDTH - DISP_WIDTH;

	if (display_y < 0)
		display_y = 0;

	if (display_y + DISP_HEIGHT >= MAP_HEIGHT)
		display_y = MAP_HEIGHT - DISP_HEIGHT;
}

void Map::processMove(UINT nChar)
{
	//calculate the new move position
	//check that it is eligible for the piece to move to
	//and that it is not moving off the map

	int new_loc_x, new_loc_y;
	BOOL move_allowed = false;
	new_loc_x = active_loc_x;
	new_loc_y = active_loc_y;
	BOOL diagonal = false;
	BOOL topleft, topright, botleft, botright;
	topleft = topright  =  botleft = botright = false;

	switch (nChar)
	{
		case VK_NUMPAD1:
		case VK_END:
			if ((active_loc_x > 0)
			&&  (active_loc_y < MAP_HEIGHT - 1))
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
			if ((active_loc_y < MAP_HEIGHT - 1))
			{
				new_loc_y++;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD3:
		case VK_NEXT:
			if ((active_loc_x < MAP_WIDTH - 1)
			&&  (active_loc_y < MAP_HEIGHT - 1))
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
			if (active_loc_x > 0)
			{
				new_loc_x--;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD6:
		case VK_RIGHT:
			if (active_loc_x < MAP_WIDTH - 1)
			{
				new_loc_x++;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD7:
		case VK_HOME:
			if ((active_loc_x > 0)
			&&  (active_loc_y > 0))
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
			if (active_loc_y > 0)
			{
				new_loc_y--;
				move_allowed = true;
			}
			break;
		case VK_NUMPAD9:
		case VK_PRIOR:
			if ((active_loc_x < MAP_WIDTH - 1)
			&&  (active_loc_y > 0))
			{
				new_loc_x++;
				new_loc_y--;
				move_allowed = true;
				topright = true;
				diagonal = true;
			}
			break;
	}

	if (!move_allowed)
		return;
	else
		move_allowed = false;

	//check that the active piece is able to move to the destination square
	//disable diagonal sea moves where both adjacent squares are land
	if (Land[new_loc_x][new_loc_y].terrain == SEA)
	{	
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
		return;
	else
	{
		active_loc_x = new_loc_x;
		active_loc_y = new_loc_y;
		move_key_pressed = true;
	}
}