// Paint1View.cpp : implementation of the CPaint1View class
//

#include "stdafx.h"
#include "Paint1.h"
#include "math.h"

#include "Paint1Doc.h"
#include "Paint1View.h"
#include "Map.h"
#include "afxwin.h"
#include "ColDialogs.h"
#include "GameRules.h"
#include "iostream.h"
#include "fstream.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEST_vh           (DWORD)0x00AA0029  /* dest = dest               */


ofstream testOutFile("Paint1View.txt", ios::out);

//utility function to convert integer to char string
char *Int2Char(int input)
{
	int max_digits = 4;
	int array_length = max_digits + 2;
	//length should be equal to array length
	static char output[6];

	int temp, count, decimal_place, first_non_zero; 
	decimal_place = first_non_zero = 0;
	if (input < 0)
		input *= -1;
	for (count = max_digits; count >= 0; count--)
	{
		int temp2 = (int)pow(10, count);
		temp = input / temp2;
		if ((temp > 0) || (count == 0) || (first_non_zero))
		{
			output[decimal_place] = (char)(48 + temp);
			first_non_zero = TRUE;
		}
		else
			output[decimal_place] = ' ';
		input = input - (temp * temp2);
		decimal_place++;
	}
	output[array_length - 1] = '\0';

	//now left shift everything, so there are no leading spaces
	int leading_spaces = 0;
	for (int i = 0; i < array_length; i++)
		if (output[i] == ' ')
			leading_spaces++;

	if (leading_spaces > 0)
		for (int i = 0 + leading_spaces; i < array_length; i++)
			output[i - leading_spaces] = output[i];

	return output;
}

ofstream testFile("pathFinder.txt", ios::out);

const int MAX_MATRIX_SIZE = 11;
const int MAX_RECORDED_PATH = 10;
const int MAX_SEARCH_DEPTH = 20;
const int MAX_INT = 32755;

struct mSquare {
	unsigned legal  : 1;
	unsigned x_diag : 1;
	unsigned rgn_no : 3;
	unsigned spare  : 3;
};

struct MatrixType {
	mSquare matrix[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
};

//class designed to find the shortest path between two points
class pathFinder
{

//utility function to find the shortest path between two points

//below is a matrix of the maximum initial size, 17x17.
//The shortest path is found from the starting point, initially 'x'
//to an abitrary destination point, 'd'
//'0', false, represents a space that cannot be on the path
//'1', true, represents a space that can be on the path

//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 d 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 0 0 0 0 1 1 1 1 1 1 1 1 1
//1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 x 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
//1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

//matrix, above, is populated by the caller, which shows which spaces the path can
//go to, and the start and end points of the path

//each path cannot use a square twice, so as the path is built each version of 
//the matrix is updated, so that squares which were free to be travelled to (1) are
//changed to be illegal (0).  This prevents loops and back-tracking.

//cross_diagonal indicates if the path can cross diagonally between two illegal spaces
//matrix radius species the distance from the origin (top left) that should be searched
//current_path_length keeps track of how long the path is at this point

//the first steps of the path are recorded in a global static variable

public:
//constructor/destructor
	pathFinder();

//data members
private:
	CPoint path[MAX_RECORDED_PATH];

	MatrixType startMatrix;
	MatrixType passedMatrix;
	
	int   min_path_length;
	float distance_to_dest;
	float tolerance;
	bool  short_path_found;
	int   recursive_call_count;
	int   max_call_limit;
	bool  reachable;
	CPoint start, finish;
	CPoint maze_dim;
	int   max_region_no;

//test members/functions
	void printMatrix(MatrixType);
	void printRegions(MatrixType);
	void printPath(void);
	bool display_diagnostics;

//functions
public:
	void set_matrix_point(int i, int j, bool legal_value, bool x_diag_value);
	int findMinPath(CPoint curr, CPoint dest, int matrix_size);
	CPoint getPathPoint(int point_number);
	bool pathExists(CPoint curr, CPoint dest, int matrix_size);

private:
	int minPathLength(CPoint from,
					  CPoint to,
					  MatrixType m,
					  int m_radius,
					  int curr_path_length);
	void fillRegion(CPoint square, int region_no);
};

void pathFinder::printMatrix(MatrixType m)
{
	testFile << endl << "Matrix is currently" << endl;
	for (int i = 0; i < MAX_MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
		{
			//indexes are reversed to ensure maze is output from top,left to bottom,right
			if ((j == start.x) && (i == start.y)) 
				testFile << "S ";
			else 
			if ((j == finish.x) && (i == finish.y)) 
				testFile << "D ";
			else 
			if (m.matrix[j][i].legal == 0)
				testFile << "X ";
			else
				testFile << "- ";
		}
		testFile << endl;
	}
}

void pathFinder::printRegions(MatrixType m)
{
	testFile << endl << max_region_no << " matrix regions, allocated as" << endl;
	for (int i = 0; i < MAX_MATRIX_SIZE; i++)
	{
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
		{
			if (m.matrix[j][i].rgn_no == 0)
				testFile << "X ";
			else 
			if (m.matrix[j][i].rgn_no < 10)
				testFile << m.matrix[j][i].rgn_no << ' ';
			else
				testFile << (m.matrix[j][i].rgn_no % 100);
		}
		testFile << endl;
	}
}

void pathFinder::printPath(void)
{
	testFile << "Path is currently :";
	for (int i = 0; i < MAX_RECORDED_PATH; i++)
		testFile << "(" << path[i].x << "," << path[i].y << "), ";
    testFile << endl;
}

int pathFinder::findMinPath(CPoint curr, CPoint dest, int matrix_size)
{
	//copy passed matrix to MatrixType struct
	for (int i = 0; i < MAX_MATRIX_SIZE; i++)
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
			startMatrix.matrix[i][j] = passedMatrix.matrix[i][j];

	if ((dest.x < 0) || (dest.x >= matrix_size) 
	||  (dest.y < 0) || (dest.y >= matrix_size))
		return MAX_INT;

	//copy curr & dest to class-wide data members
	start = curr;
	finish = dest;

	//length of shortest path found so far
	min_path_length = MAX_INT;

	//actually this is the sum of x & y differences, but not resolved to a hypotenuse
	distance_to_dest = max(abs(curr.x - dest.x),abs(curr.y - dest.y)) + 1;

	//test code to output the details of the maze and path requested
	testFile << "-------------------------------------" << endl;
	testFile << "---------New path search-------------" << endl;
	testFile << "-------------------------------------" << endl;
	printMatrix(startMatrix);
	testFile << "From (" << curr.x << "," << curr.y << ") to "
		     <<      "(" << dest.x << "," << dest.y << ")." << endl << endl;
	
	maze_dim.SetPoint(matrix_size,matrix_size);

	bool path_exists = pathExists(curr,dest,matrix_size);
	printRegions(startMatrix);

	if (path_exists)
	{	
		//call the recursive path finder function, and return the result
		return minPathLength(curr, dest, startMatrix, matrix_size, 0);
	}
	else
		return MAX_INT;
}

pathFinder::pathFinder()
{
	for (int i = 0; i < MAX_MATRIX_SIZE; i++)
		for (int j = 0; j < MAX_MATRIX_SIZE; j++)
		{
			passedMatrix.matrix[i][j].legal  = 0;
			passedMatrix.matrix[i][j].rgn_no = 0;
			passedMatrix.matrix[i][j].spare  = 0;
			passedMatrix.matrix[i][j].x_diag = 1;

			startMatrix.matrix[i][j].legal  = 0;
			startMatrix.matrix[i][j].rgn_no = 0;
			startMatrix.matrix[i][j].spare  = 0;
			startMatrix.matrix[i][j].x_diag = 1;
		}

	for (int count = 0; count < MAX_RECORDED_PATH; count++)
		path[count].SetPoint(-1,-1);

	min_path_length = MAX_INT;
	tolerance = 0.1;
	short_path_found = false;
	recursive_call_count = 0;
	max_call_limit = 25000;
	maze_dim.SetPoint(0,0);
	max_region_no = 0;

	display_diagnostics = false;
}

bool pathFinder::pathExists(CPoint curr, CPoint dest, int matrix_size)
{
	//returns true if a legal path exists
	//does this recursively calculating each navigable area, and assigning a number
	//to each square's 'regn_no' field

	//loop through all squares
	//call the recursive function if the square does not have a region yet
	//the recursive function will then populate rgn_no, and call itself for each
	//adjacent square that can be on the path

	//for region calculation the starting location is a legal move
	startMatrix.matrix[curr.x][curr.y].legal = 1;
	CPoint rgn_start;

	int curr_region = 0;
	for (int i = 0; i < maze_dim.x; i++)
		for (int j = 0; j < maze_dim.y; j++)
		{
			if ( (startMatrix.matrix[i][j].rgn_no == 0)
			&&   (startMatrix.matrix[i][j].legal)  )
			{
				curr_region++;
				rgn_start.SetPoint(i,j);
				fillRegion(rgn_start, curr_region);
			}
		}

	//reset status of starting location
	startMatrix.matrix[curr.x][curr.y].legal = 0;

	//save number of regions found in class-wide variable
	max_region_no = curr_region;

	//if the rgn_no for curr and dest are different then no possible path exists
	if (startMatrix.matrix[curr.x][curr.y].rgn_no !=
	    startMatrix.matrix[dest.x][dest.y].rgn_no)
		return false;
	else
		return true;
}

void pathFinder::fillRegion(CPoint from, int curr_region)
{
	startMatrix.matrix[from.x][from.y].rgn_no = curr_region;

	CPoint new_loc(0,0);

	//data members for neighbour traversal
	int offsets[8][2]= { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
	int i_offset, j_offset;
	int x = 0;
	int y = 1;

	//cycle through neighbour squares, and call fillRegion if they can be moved to
	for (int i=0; i < 8; i++)
	{
		i_offset = offsets[i][x];
		j_offset = offsets[i][y];
		new_loc.SetPoint(from.x + i_offset,from.y + j_offset);

		if ( (new_loc.x >= 0) && (new_loc.x < MAX_MATRIX_SIZE)
		&&   (new_loc.y >= 0) && (new_loc.y < MAX_MATRIX_SIZE) 
		&&   (new_loc.x <  maze_dim.x) && (new_loc.y <  maze_dim.y) )
		{
			if ( (startMatrix.matrix[new_loc.x][new_loc.y].legal)
			&&   (startMatrix.matrix[new_loc.x][new_loc.y].rgn_no == 0) )
			{
				//check diagonals
				if ( (abs(j_offset) == 1) 
				&&   (abs(i_offset) == 1) )
				{
					//check applicable adjacent squares, to see if a diagonal move is ok
					if (    (startMatrix.matrix[from.x + i_offset][from.y].x_diag)
						 && (startMatrix.matrix[from.x][from.y + j_offset].x_diag) )
						fillRegion(new_loc, curr_region);
				}
				else
					fillRegion(new_loc, curr_region);
			}
		}
	}
}

int pathFinder::minPathLength(CPoint from,
				  CPoint to,
				  MatrixType m,
				  int m_radius,
				  int curr_path_length)
{
	if (display_diagnostics)
	{
		for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
			testFile << "  ";
		testFile << "Searching from: (" << from.x << "," << from.y << ")"  
				 << "-Call count: " << recursive_call_count << endl;
	
	}

	//increment length of the current path (i.e. search depth)
	curr_path_length++;
	recursive_call_count++;

	if (from == to)
	{
		if (display_diagnostics)
		{
			for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
				testFile << "  ";
			testFile << "Destination found,";
			testFile << " exiting with path length: " << curr_path_length << endl;
			printMatrix(m);
		}

		//add destination point to path
		if ( (curr_path_length <= MAX_RECORDED_PATH)
		&&   (curr_path_length <= min_path_length) )
			path[curr_path_length - 1] = from;

		//store length of the shortest path found so far
		if (curr_path_length < min_path_length)
			min_path_length = curr_path_length;

		//check if a reasonably short path has been found
		if ((distance_to_dest + (distance_to_dest * tolerance)) 
				> curr_path_length)
			short_path_found = true;

		if (display_diagnostics)
			printPath();

		return curr_path_length;
	}

	//abandon further search if we have reached the maximum search depth
	if (curr_path_length > MAX_SEARCH_DEPTH)
	{
		if (display_diagnostics)
		{
			for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
				testFile << "  ";
			testFile << "Max search depth reached ,";
			testFile << " abandoning search at depth: " << curr_path_length << endl;
		}
		return MAX_INT;
	}

	//abandon furher search if an acceptably short path has been flagged as found
	if (short_path_found)
	{
		if (display_diagnostics)
		{
			for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
				testFile << "  ";
			testFile << "An acceptably short path has been found ,";
			testFile << " abandoning all search. Current depth: " << curr_path_length << endl;
		}
		return MAX_INT;
	}

	//if we have already exceeded the length of the shortest path found so far
	//(may want to add a flag to confirm a path to destination has been established)
	if (curr_path_length > min_path_length)
	{
		if (display_diagnostics)
		{
			for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
				testFile << "  ";
			testFile << "A shorter path has been found ,";
			testFile << " abandoning search at depth: " << curr_path_length << endl;
		}
		return MAX_INT;
	}

	//if the recursive calls exceed the maximum, then abandon further search
	if (recursive_call_count > max_call_limit)
	{
		if (display_diagnostics)
		{
			for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
				testFile << "  ";
			testFile << "Maximum call limit has been reached ,";
			testFile << " abandoning search at depth: " << curr_path_length << endl;
		}
		return MAX_INT;
	}

	//ensure this square cannot be visited again on subsequent calls
	m.matrix[from.x][from.y].legal = 0;
	
	int candidate[9] = {MAX_INT,MAX_INT,MAX_INT,
		                MAX_INT,MAX_INT,MAX_INT,
						MAX_INT,MAX_INT,MAX_INT};
	CPoint new_loc(0,0);
	int cand_no = 0;

	//data members for path search
	int order_matrix2[8][2]= { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
	int swap_element[2];
	int i_offset, j_offset;
	int x = 0;
	int y = 1;

	//sort order matrix based on proximity of result to destination
	//this is a heuristic to ensure we try each candidate square based on their
	//proximity to the destination
	for (int sort_pass = 1; sort_pass < 8; sort_pass++)
	{
		for (int index = 0; index < 7; index++)
		{
			int distance1, distance2;
			int distance1_x, distance1_y, distance2_x, distance2_y;
			CPoint new_point1(from.x + order_matrix2[index][x],
				              from.y + order_matrix2[index][y]);
			CPoint new_point2(from.x + order_matrix2[index+1][x],
				              from.y + order_matrix2[index+1][y]);
			distance1_x = abs(to.x - new_point1.x);
			distance1_y = abs(to.y - new_point1.y);
			distance2_x = abs(to.x - new_point2.x);
			distance2_y = abs(to.y - new_point2.y);
			distance1 = distance1_x + distance1_y;
			distance2 = distance2_x + distance2_y;
			if (distance1 > distance2)
			{
				swap_element[x]=order_matrix2[index][x];
				swap_element[y]=order_matrix2[index][y];
				order_matrix2[index][x]=order_matrix2[index+1][x];
				order_matrix2[index][y]=order_matrix2[index+1][y];
				order_matrix2[index+1][x]=swap_element[x];
				order_matrix2[index+1][y]=swap_element[y];
			}
			//if summed differences are the same, there may still be a preference
			if (distance1 == distance2)
			{
				int max_1 = max(distance1_x, distance1_y);
				int max_2 = max(distance2_x, distance2_y);

				if (max_1 > max_2)
				{
					swap_element[x]=order_matrix2[index][x];
					swap_element[y]=order_matrix2[index][y];
					order_matrix2[index][x]=order_matrix2[index+1][x];
					order_matrix2[index][y]=order_matrix2[index+1][y];
					order_matrix2[index+1][x]=swap_element[x];
					order_matrix2[index+1][y]=swap_element[y];
				}
			}
		}
	}

	//cycle through available squares, and evaluate path lengths if they can be moved to
	for (int i=0; i < 8; i++)
	{
		i_offset = order_matrix2[i][x];
		j_offset = order_matrix2[i][y];
		new_loc.SetPoint(from.x + i_offset,from.y + j_offset);

		if ( (new_loc.x >= 0) && (new_loc.x < MAX_MATRIX_SIZE)
		&&   (new_loc.y >= 0) && (new_loc.y < MAX_MATRIX_SIZE) 
		&&   (new_loc.x <  m_radius) && (new_loc.y <  m_radius) )
		{
			if (m.matrix[new_loc.x][new_loc.y].legal)
			{
				//check diagonals
				if ( (abs(j_offset) == 1) 
				&&   (abs(i_offset) == 1) )
				//&&   (!cross_diagonal) )
				{
					//check applicable adjacent squares, to see if a diagonal move is ok
					if (    (m.matrix[from.x + i_offset][from.y].x_diag)
						 && (m.matrix[from.x][from.y + j_offset].x_diag) )
					candidate[cand_no] = 
						minPathLength(new_loc,to,m,m_radius,curr_path_length);
				}
				else
					candidate[cand_no] = 
						minPathLength(new_loc,to,m,m_radius,curr_path_length);

			}
			//if destination found then don't search any more squares from this location
			if (new_loc == to)
				j_offset = i_offset = MAX_INT;
		}
		cand_no++;	
	}
	//-----------------------------------------------------------

	int minimum = MAX_INT;
	cand_no = 0;
	for (cand_no = 0; cand_no < 9; cand_no++)
	{
		if (candidate[cand_no] < minimum)  
			minimum = candidate[cand_no];
	}

	if ( (curr_path_length < MAX_RECORDED_PATH)
	&&   (minimum <= min_path_length)
	&&   (minimum < MAX_INT) )
		path[curr_path_length - 1] = from;

	if (minimum < min_path_length)
		min_path_length = minimum;

	if (display_diagnostics)
	{
		for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
			testFile << "  ";
		testFile << "Exiting ";
		testFile << "from: (" << from.x << "," << from.y << ")";
		testFile << ", shortest path had length: " << minimum << endl;
		for (int tab_level = 0; tab_level < curr_path_length; tab_level++)
			testFile << "  ";
		printPath();
	}

	return minimum;
}

void pathFinder::set_matrix_point(int i, int j, bool legal_value, bool x_diag_value)
{
	if ((i < MAX_MATRIX_SIZE) && (j < MAX_MATRIX_SIZE))
	{
		if (legal_value)
			passedMatrix.matrix[i][j].legal = 1;
		else
			passedMatrix.matrix[i][j].legal = 0;
		if (x_diag_value)
			passedMatrix.matrix[i][j].x_diag = 1;
		else
			passedMatrix.matrix[i][j].x_diag = 0;
	}
}

CPoint pathFinder::getPathPoint(int point_number)
{
	CPoint err_point(-1,-1);

	if (point_number < MAX_RECORDED_PATH)
		return path[point_number];
	else
		return err_point;
}


//---------------------------------------------------------------------------
// CRandMapDialog dialog
//---------------------------------------------------------------------------

class CRandMapDialog : public CDialog
{
	DECLARE_DYNAMIC(CRandMapDialog)

public:
	CRandMapDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRandMapDialog();

// Dialog Data
	enum { IDD = IDD_NEW_RAND_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Number of land points to seed new map with
	int land_points;
	// Scale for average temperature range
	int temperature;
	// Range to specify the level of rainfall for the new map
	int rainfall;
	// Width of new map
	int width;
	// Height of new map
	int height;
	// Specifies altitude range of new map
	int altitude;
	// Specifies size of the main continent
	int cont_size;
	// Indicates user has cancelled the dialog
	CButton cancelBtn;
	// Displays the width of the map to be created
	int width_display;
	// Display/set the height of the map
	int height_display;
	int land_points_display;
	int cont_size_display;
	int altitude_display;
	int temperature_display;
	int rainfall_display;
};

IMPLEMENT_DYNAMIC(CRandMapDialog, CDialog)
CRandMapDialog::CRandMapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRandMapDialog::IDD, pParent)
	, land_points(0)
	, temperature(0)
	, rainfall(0)
	, width(0)
	, height(0)
	, altitude(0)
	, cont_size(0)
	, width_display(0)
	, height_display(0)
	, land_points_display(0)
	, cont_size_display(0)
	, altitude_display(0)
	, temperature_display(0)
	, rainfall_display(0)
{
}

CRandMapDialog::~CRandMapDialog()
{
}

void CRandMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_NM_LANDPTS_SLIDER, land_points);
	DDX_Slider(pDX, IDC_NM_TEMP_SLIDER, temperature);
	DDX_Slider(pDX, IDC_NM_RAIN_SLIDER, rainfall);
	DDX_Slider(pDX, IDC_NM_WIDTH_SLIDER, width);
	DDX_Slider(pDX, IDC_NM_HEIGHT_SLIDER, height);
	DDX_Slider(pDX, IDC_NM_ALTITIDE_SLIDER, altitude);
	DDX_Slider(pDX, IDC_NM_CONTSIZE_SLIDER, cont_size);
	DDX_Control(pDX, IDCANCEL, cancelBtn);
	DDX_Text(pDX, IDC_NM_WIDTH_EDIT, width_display);
	DDX_Text(pDX, IDC_NM_HEIGHT_EDIT, height_display);
	DDX_Text(pDX, IDC_NM_LANDPTS_EDIT, land_points_display);
	DDX_Text(pDX, IDC_NM_CONTSIZE_EDIT, cont_size_display);
	DDX_Text(pDX, IDC_NM_ALTITUDE_EDIT, altitude_display);
	DDX_Text(pDX, IDC_NM_CLIMATE_TEMP_EDIT, temperature_display);
	DDX_Text(pDX, IDC_NM_WIDTH_EDIT4, rainfall_display);
}


BEGIN_MESSAGE_MAP(CRandMapDialog, CDialog)
END_MESSAGE_MAP()


// CRandMapDialog message handlers


/*-------------------------------------------------------*/

//-------------------------------------------------------------------------
// CColAusView
//-------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CColAusView, CView)

BEGIN_MESSAGE_MAP(CColAusView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Other commands
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_TEST_HIDE_UNEXPLORED, OnTestHideUnexplored)
	ON_UPDATE_COMMAND_UI(ID_TEST_HIDE_UNEXPLORED, OnUpdateTestHideUnexplored)
	ON_COMMAND(ID_TEST_NEWMAP, OnTestNewmap)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ACTIONS_ENDTURN, OnActionsEndturn)
	ON_COMMAND(ID_FILE_GAMEOPTIONS, OnFileGameoptions)
	ON_COMMAND(ID_TEST_ADDPIECE, OnTestAddpiece)
	ON_WM_TIMER()
	ON_COMMAND(ID_TEST_ACTIVATETIMER, OnTestActivatetimer)
	ON_COMMAND(ID_VIEW_ACTIVEPIECE, OnViewActivepiece)
	ON_COMMAND(ID_ENCYCLOPEDIA_PIECES, OnEncyclopediaPieces)
	ON_COMMAND(ID_ACTIONS_SKIP, OnActionsSkip)
	ON_COMMAND(ID_ACTIONS_WAIT, OnActionsWait)
	ON_COMMAND(ID_PIECEANIMATE_INCREASE, OnPieceanimateIncrease)
	ON_COMMAND(ID_PIECEANIMATE_DECREASE, OnPieceanimateDecrease)
	ON_COMMAND(ID_ACTIONS_DISBAND, OnActionsDisband)
	ON_COMMAND(ID_ACTIONS_SLEEP, OnActionsSleep)
	ON_COMMAND(ID_TEST_DISPLAYMAPSTATISTICS, OnTestDisplaymapstatistics)
END_MESSAGE_MAP()

// CPaint1View construction/destruction

CColAusView::CColAusView()
{
	m_Display = Fonts;
	t_hide_unexplored = TRUE;

	map_graphics = &ColGraphics;

	//set dynamically based on screen resolution and/or user updatable options
	DISP_WIDTH  = 11;
	DISP_HEIGHT = 11;

	//mini-map display paramaters
	MINI_MAP_WIDTH   = 100;
	MINI_MAP_HEIGHT  = 100;

	//number of pixels in the mini-map for each map square
	//Cmm_square_width = 2;

	display_start.SetPoint(0, 0);

	//screen resolution to use is currently unknown
	res_selection = 0;
	old_res = -1;
	display_active_timer = 0;
	midi_song_timer = 0;
	animate_delay_time = 10;

	display_active = true;
	animation_pending = 0;
	memoryDC_created = false;
	move_key_ok = true;
	piece_just_moved = false;
	display_tag_text = true;
	patrol_dest_pending = false;

	current_display_type = MAP_DISPLAY;

	activeMap = NULL;
	activeList = NULL;

	active_click_pos.SetPoint(-1, -1);

	//if (!midi_song_timer)
	//	midi_song_timer = SetTimer(2, 300000, NULL);

	stat_display = NONE;
}

CColAusView::~CColAusView()
{
	//causes an error.....
	//KillTimer(active_display_timer);
}

BOOL CColAusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	BOOL window_flag;

	window_flag = CView::PreCreateWindow(cs);

	return window_flag;
}

// CPaint1View drawing

void CColAusView::OnDraw(CDC* pDC)
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int active_player = pDoc->gameStats.active_player;
	activeMap = &pDoc->PlayerMap[active_player];

	//currently there is only one list of pieces for the doc
	activeList = &pDoc->MasterList;

	//initially set screen resolution & timer
	if (!res_selection)
		InitResolution();
	if (!display_active_timer)
		display_active_timer = SetTimer(1, 350, NULL);
	if (!midi_song_timer)
		midi_song_timer = SetTimer(2, 300000, NULL);
	if (!memoryDC_created)
		initMemDCs(pDC);

	//reset display paramaters if selected resolution has changed
	//if (old_res != res_selection)
	//always do to ensure MINI_MAP sizes (in map squares and pixels and pixels per map square) are reset
		UpdateDisplaySize(activeMap);

	//Encyclopedia view display processing
	if (current_display_type == ENC_DISPLAY)
	{
		ShowEnclopediaMenu(pDC, PIECES);
		return;
	}

	if (!pDoc->current_game)
	{
		ShowBackgroundPic(pDC);
		return;
	}
	//Map view display processing
	//Center map display on active piece if start of turn
	if (pDoc->gameStats.start_of_turn)
	{
		if (activeList->GetActive() > 0)
			reCenter_active(activeMap, activeList);

		pDoc->gameStats.start_of_turn = false;

		ShowColmap(pDC, activeMap, activeList);
		ShowSidePanel(pDC, activeMap, activeList);

		processTurnStart();
	}
	else
	{
		ShowColmap(pDC, activeMap, activeList);
		ShowSidePanel(pDC, activeMap, activeList);
	}
}

void CColAusView::displayPrevMoves(void)
{
	//Animates all unseen moves by other players 
	//currently updates the player's map also
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint dfrom, dto;
	int dpiece, dtype;

	int curr_move = pDoc->displayMoves.getNextMoveToDisplay(pDoc->gameStats.getActivePlayer(), 
		                                                    dfrom, dto, dpiece, dtype);
	//needed to ensure map has been drawn before animating
	reCenter_location(dto, false);
	paintMapScreen(GetDC(), activeMap);

	//don't display tag text to other players (may not be correct anyway)
	display_tag_text = false;

	for (;curr_move >= 0;)
	{
		//display some animation and/or sound to indicate the move to the player
		if  ( (activeMap->isRecentlySeen(dfrom.x, dfrom.y))
		||    (activeMap->isRecentlySeen(dto.x, dto.y))  )
		{
			CPoint old_disp_start;
			old_disp_start = display_start;
			reCenter_location(dto, false);

			if (old_disp_start != display_start)
				paintMapScreen(GetDC(), activeMap);
	
			switch (dtype)
			{
			case MOVE:
				activeList->UpdateActive(dpiece);
				animateMove(GetDC(), activeMap, dpiece, dfrom, dto);
				//activeList->UpdateActive(EMPTY);
				break;
			case ATTACK:
				//do attack sound (TODO: add animation)
				PlaySound("LILGUN.WAV", NULL, SND_FILENAME | SND_ASYNC); 
				break;
			}
		}

		//update Player's map with ramifications of the move
		switch (dtype)
		{
		case MOVE:
			//activeMap->updatePiecePresence(dfrom.x, dfrom.y, dto.x, dto.y);
			//need a function that only updates the map
			updatePrevMove(dpiece, dfrom, dto);
			//movePiece(dpiece, dfrom, dto);
			break;
		case ATTACK:
			//no changes to map required
			break;
		case DEFEAT:
			activeMap->removePiecefromSquare(dfrom);
			break;
		case CAPTURE:
			activeMap->removePiecefromSquare(dto);
			//for a capture dpiece is the captured piece
			activeMap->addPiecetoSquare(dpiece, dto);
			break;
		case VICTORY:
			activeMap->removePiecefromSquare(dto);
			break;
		case DISBAND:
			activeMap->removePiecefromSquare(dfrom);
			break;
		case EMERGE:
			activeMap->addPiecetoSquare(dpiece, dto);
			break;
		default:
			break;
		}
		//retrieve the next move to be displayed
		curr_move = pDoc->displayMoves.getNextMoveToDisplay(pDoc->gameStats.getActivePlayer(), dfrom, dto, dpiece, dtype);
	}
	display_tag_text = true;
	//add a function that sychronises a player's map with all piece data on the MasterMap
	//TODO: remove it when no longer required!
	activeMap->copyMoves(pDoc->MasterMap);
}

void CColAusView::processTurnStart(void)
{
	//Proccesses previous moves for a player
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	int curr_player = pDoc->gameStats.getActivePlayer();
	
	//processing varies depending on whether this is a computer or human player
	if (pDoc->playerStats[curr_player].isComputerPlayer())
	{
		//update PrevMoves as displayed
		pDoc->displayMoves.updateAllSeenBy(curr_player);
		//replace with "analysePrevMoves()", so AI can note changes

		//update the player's map with moves of other players
		activeMap->copyMoves(pDoc->MasterMap);

		//cycle through player's pieces and make appropriate moves,
		//  as determined by the applicable ai algorithm

		//end turn for this computer player
		pDoc->endTurn();
		Invalidate(FALSE);
			
	}
		else
		{
			displayPrevMoves();

			processToNextPiece();
		}
}

void CColAusView::paintMapScreen(CDC *pDC, Map *activeMap)
{
	//Fully paints the map and side panel direct to the DC
	//(may want to do both to memDC and then BitBlt that to actual DC)
	ShowColmap(pDC, activeMap, activeList);
	ShowSidePanel(pDC, activeMap, activeList);
}

void CColAusView::UpdateDisplaySize(Map *activeMap)
{
	//initialise size of Mini-map (in pixels)
	mm_Size.SetPoint(0,0);

	if (res_selection == 2)
	{
		activeMap->updateDisplaySize(16, 14);
		this->DISP_WIDTH = 16;
		this->DISP_HEIGHT = 14;

		mm_Size.SetPoint(200, 200); 
	}
	if (res_selection == 1)
	{
		activeMap->updateDisplaySize(11, 11);
		this->DISP_WIDTH = 11;
		this->DISP_HEIGHT = 11;
		//this->MINI_MAP_HEIGHT = 50;
		//this->MINI_MAP_WIDTH = 100;
		mm_Size.SetPoint(200, 100);
	}

	mm_sqSize = getMMSqDim(activeMap);
	//set so that Mini Map is the desired number of pixels for the resolution selected
	MINI_MAP_WIDTH = mm_Size.x / mm_sqSize.x;
	MINI_MAP_HEIGHT = mm_Size.y / mm_sqSize.y;

	old_res = res_selection;
}

void CColAusView::InitResolution(void)
{
	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	switch (screen_x)
	{
		case 800:
			res_selection = 1;
			break;
		case 1024:
			res_selection = 2;
			break;
		default:
			res_selection = 1;
			break;
	}
}

void CColAusView::initMemDCs(CDC* pDC)
{
	//initialise a memory DC that is the entire size of the view area
	CRect r;
	GetClientRect(&r);
    
	//create compatible objects
	//memoryDC.CreateCompatibleDC(pDC); 
	//memoryBitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
	//select memory Bitmap into the memory DC
	//CBitmap oldbm = memoryDC.SelectObject(&memoryBitmap);
	disp_size.SetSize(r.right, r.bottom);

	memoryDC_created = true;

}

void CColAusView::copyDC(CDC* fromDC, CDC* toDC)
{
	toDC->BitBlt(0, 0, 
		        disp_size.cx, 
		        disp_size.cy, 
				fromDC, 0, 0, SRCCOPY);
}


// CPaint1View printing

BOOL CColAusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CColAusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CColAusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPaint1View diagnostics

#ifdef _DEBUG
void CColAusView::AssertValid() const
{
	CView::AssertValid();
}

void CColAusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColAusDoc* CColAusView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColAusDoc)));
	return (CColAusDoc*)m_pDocument;
}
#endif //_DEBUG


// CColAusView message handlers

void CColAusView::OnFileGameoptions()
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GameOptsDialog gameoptsDlg;

	if (pDoc->gameStats.auto_end_turn)
		gameoptsDlg.auto_end_turn = TRUE;
	else
		gameoptsDlg.auto_end_turn = FALSE;

	if (pDoc->gameStats.animate_piece_moves)
		gameoptsDlg.animate_piece_moves = TRUE;
	else
		gameoptsDlg.animate_piece_moves = FALSE;

	if (gameoptsDlg.DoModal() == IDOK)
	{
		if (gameoptsDlg.auto_end_turn)
			pDoc->gameStats.auto_end_turn = true;
		else
			pDoc->gameStats.auto_end_turn = false;

		if (gameoptsDlg.res_selection > 0)
			res_selection = gameoptsDlg.res_selection;

		if (gameoptsDlg.animate_piece_moves)
			pDoc->gameStats.animate_piece_moves = true;
		else
			pDoc->gameStats.animate_piece_moves = false;

	}
	Invalidate(FALSE);
}

void CColAusView::ShowColmap(CDC* pDC, Map* theMap, PieceList* thePieces)
{
	int display_result;

	display_result = paintMap(pDC, theMap, 
							  DISP_WIDTH, 
							  DISP_HEIGHT);

	if (display_result > 0)
		ShowText(pDC, theMap->Messages[display_result], 50, 50);
	
	return;
}

void CColAusView::ShowEnclopediaMenu(CDC *pDC, int menu_type)
{
	//get access to document class
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//get current view dimensions
	CRect r;
	GetClientRect(&r);

	//set up a memory DC for the display area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
	CBitmap *oldbm1 = memDC.SelectObject(&memBitmap);

	//set up a memory DC for the background graphic
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 
	CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->background);

	BITMAP bm1;
	(map_graphics->background).GetObject(sizeof(BITMAP), &bm1);

	//calculate the number of times the background graphic will fit
	//accros the width and height of the viewing area
	int times_across = (r.right / bm1.bmWidth) + 1;
	int times_down = (r.bottom / bm1.bmHeight) + 1;

	for (int i = 0; i < times_across; i++)
		for (int j = 0; j < times_down; j++) 
			memDC.BitBlt(0 + (i * bm1.bmWidth), 
				         0 + (j * bm1.bmHeight), 
					     bm1.bmWidth, bm1.bmHeight, 
						 &dcMem1, 0, 0, 
						 SRCCOPY);

	ShowText(&memDC, "Encyclopedia of ColAusn-isation", 300, 25);

	if (menu_type == PIECES)
	{
		for (int j = 0; j <= pDoc->MasterList.allocated_pieces; j++)
			ShowText(&memDC, pDoc->MasterList.Piece_attributes[j].name, 
					 25, (j * 25) + 100);
	}

	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(0, 0, 
				r.right, r.bottom, 
				&memDC, 
				0, 0, SRCCOPY);

	memDC.SelectObject(oldbm1);
	dcMem1.SelectObject(oldbm2);
}

void CColAusView::ShowWoodCut(CDC* pDC, int artwork_number)
{
	//get current view dimensions
	CRect r;
	GetClientRect(&r);

	//set up a memory DC for the display area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
	CBitmap *oldbm1 = memDC.SelectObject(&memBitmap);

	//set up a memory DC for the background graphic
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 

	BITMAP bm1;
	(map_graphics->background).GetObject(sizeof(BITMAP), &bm1);
	CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->background);

	//calculate the number of times the background graphic will fit
	//accros the width and height of the viewing area
	int times_across = (r.right / bm1.bmWidth) + 1;
	int times_down = (r.bottom / bm1.bmHeight) + 1;

	for (int i = 0; i < times_across; i++)
		for (int j = 0; j < times_down; j++) 
			memDC.BitBlt(0 + (i * bm1.bmWidth), 
				         0 + (j * bm1.bmHeight), 
					     bm1.bmWidth, bm1.bmHeight, 
						 &dcMem1, 0, 0, 
						 SRCCOPY);

	//display the picture
	(map_graphics->artwork[artwork_number]).GetObject(sizeof(BITMAP), &bm1);
	dcMem1.SelectObject(&map_graphics->artwork[artwork_number]);

	int center_left = (r.right - bm1.bmWidth) / 2;
	int center_top = (r.bottom - bm1.bmHeight) / 2;

	memDC.BitBlt(center_left, 
		         center_top, 
			     bm1.bmWidth, bm1.bmHeight, 
				 &dcMem1, 0, 0, 
				 SRCCOPY);
	
	//draw a border around the picure
	int increment_amount = 1;
	int line_thickness = 3;
	int color_increment_amount = 3;
	int border_offset = -2;
	int frame_width = 15;
	int top_left_shade = 0;
	int bot_right_shade = frame_width * color_increment_amount;

	CPen dummy_pen(PS_SOLID, 1, RGB(127, 0, 0));
	CPen* oldPen = memDC.SelectObject(&dummy_pen);

	for (int i = 0; i < frame_width; i+= increment_amount)
	{
		if (i < frame_width)
		{
		CPen top_left_pen;
		CPen bottom_right_pen;
		top_left_pen.CreatePen(PS_SOLID, line_thickness,
			                   RGB(top_left_shade, top_left_shade, top_left_shade));
		bottom_right_pen.CreatePen(PS_SOLID, line_thickness,
			                       RGB(bot_right_shade, bot_right_shade, bot_right_shade));
		memDC.SelectObject(&top_left_pen);
		border_offset += increment_amount;
		memDC.MoveTo(center_left - border_offset, 
			         center_top - border_offset);
		memDC.LineTo(center_left + bm1.bmWidth + border_offset, 
					 center_top - border_offset);
		memDC.SelectObject(&bottom_right_pen);
		memDC.LineTo(center_left + bm1.bmWidth + border_offset, 
			         center_top + bm1.bmHeight + border_offset);
		memDC.LineTo(center_left - border_offset, 
			         center_top + bm1.bmHeight + border_offset);
		memDC.SelectObject(&top_left_pen);
		memDC.LineTo(center_left - border_offset, 
			         center_top - border_offset);
		top_left_shade += color_increment_amount;
		bot_right_shade -= color_increment_amount;
		memDC.SelectObject(&oldPen);
		border_offset += increment_amount;
		}
	}
	
	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(0, 0, 
				r.right, r.bottom, 
				&memDC, 
				0, 0, SRCCOPY);

	//reselect old objects
	memDC.SelectObject(oldbm1);
	dcMem1.SelectObject(oldbm2);
}

void CColAusView::ShowBackgroundPic(CDC* pDC)
{
	//get access to document class
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//get current view dimensions
	CRect r;
	GetClientRect(&r);

	//set up a memory DC for the display area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, r.right, r.bottom);
	CBitmap *oldbm1 = memDC.SelectObject(&memBitmap);

	//set up a memory DC for the background graphic
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 

	BITMAP bm1;
	(map_graphics->background).GetObject(sizeof(BITMAP), &bm1);
	CBitmap *old_object_ptr = dcMem1.SelectObject(&map_graphics->background);

	//calculate the number of times the background graphic will fit
	//accros the width and height of the viewing area
	int times_across = (r.right / bm1.bmWidth) + 1;
	int times_down = (r.bottom / bm1.bmHeight) + 1;

	for (int i = 0; i < times_across; i++)
		for (int j = 0; j < times_down; j++) 
			memDC.BitBlt(0 + (i * bm1.bmWidth), 
				         0 + (j * bm1.bmHeight), 
					     bm1.bmWidth, bm1.bmHeight, 
						 &dcMem1, 0, 0, 
						 SRCCOPY);

	//display the picture
	(map_graphics->jansson_map).GetObject(sizeof(BITMAP), &bm1);
	dcMem1.SelectObject(&map_graphics->jansson_map);

	int center_left = (r.right - bm1.bmWidth) / 2;
	int center_top = (r.bottom - bm1.bmHeight) / 2;

	memDC.BitBlt(center_left, 
		         center_top, 
			     bm1.bmWidth, bm1.bmHeight, 
				 &dcMem1, 0, 0, 
				 SRCCOPY);

	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(0, 0, 
				r.right, r.bottom, 
				&memDC, 
				0, 0, SRCCOPY);

	//select old object to eliminate memory leakage
	dcMem1.SelectObject(old_object_ptr);
	memDC.SelectObject(oldbm1);
}

CPoint CColAusView::getMMSqDim(Map *theMap)
{
	//Calculates the pixel width and height of the minimap
	//The default is 2x2
	CPoint return_value(2, 2);
	if ((theMap->MAP_WIDTH < (mm_Size.x / return_value.x))
	||  (theMap->MAP_HEIGHT < (mm_Size.y / return_value.y)))
		return_value.SetPoint(4, 4);

	return return_value;
}

void CColAusView::ShowSidePanel(CDC* pDC, Map* theMap, PieceList* thePieces)
{
	CSize win_size = pDC->GetViewportExt();

	int display_x, display_y;
	
	display_x = (DISP_WIDTH * SQ_WIDTH);
	display_y = 0;

	CRect r;
	GetClientRect(&r);
    
	//set up a memory DC for the map area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, r.right, 48 * DISP_HEIGHT);//view.sqsize
	CBitmap *oldbm1 = memDC.SelectObject(&memBitmap);

	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 
	CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->background);
	BITMAP bm1;
	(map_graphics->background).GetObject(sizeof(BITMAP), &bm1);

	for (int i = 0; i < 3; i++)
		memDC.BitBlt(display_x, display_y + (i * bm1.bmHeight), 
		             bm1.bmWidth, bm1.bmHeight, 
			         &dcMem1, 0, 0, 
					 SRCCOPY);

	//create a new pen and select it into the DC.
	CPen black_pen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* oldPen = memDC.SelectObject(&black_pen);

	CPoint mm_sqSize = getMMSqDim(theMap);
	//CPoint mm_Size = getMMDim(theMap);
	int mm_pxl_width = MINI_MAP_WIDTH * mm_sqSize.x;
	int mm_pxl_height = MINI_MAP_HEIGHT * mm_sqSize.y;
	int mm_offset = 25;
	
	//draw the line separating the map box from the rest of the panel
	memDC.MoveTo(display_x + 1, display_y);
	memDC.LineTo(display_x + 1, r.bottom);
	memDC.MoveTo(display_x, 
		         display_y + mm_pxl_height + (mm_offset * 2));
	memDC.LineTo(r.right, 
		         display_y + mm_pxl_height + (mm_offset * 2));

	CPen red_pen(PS_SOLID, 3, RGB(128, 0, 0));
	memDC.SelectObject(&red_pen);

	//draw a rectangle around the mini-map
	memDC.Rectangle(display_x + mm_offset - 1, 
		            display_y + mm_offset - 1,
		            display_x + mm_pxl_width + mm_offset + 1, 
					display_y + mm_pxl_height + mm_offset + 1);

	//restore the old pen to the DC
	memDC.SelectObject(oldPen);

	//display the minimap
	paintMiniMap(&memDC, theMap, thePieces, 
		         display_x + mm_offset, 
				 display_y + mm_offset);

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//display game/player statics
	//define some margins, spacers based on above, and use instead!!
	int left_margin = display_x + mm_offset - 1;
	int top_margin = display_y + mm_pxl_height + ((mm_offset + 1)*2) + 1;
	int line_spacer;
	int field_spacer = 10;
	//should obtain from view initialisation
	int sprite_size = 48;

	CPoint curr_loc(left_margin, top_margin);
	CSize move_loc;
	//return CPoint giving the size of the text written...?
	move_loc = ShowText(&memDC, pDoc->gameStats.getMonthtxt(), 
		                curr_loc.x, curr_loc.y);

	curr_loc.x += (move_loc.cx + field_spacer);
	line_spacer = (move_loc.cy / 8);
	
	int year = pDoc->gameStats.year;
	move_loc = ShowText(&memDC, Int2Char(year), 
		                curr_loc.x, curr_loc.y);

	curr_loc.x = left_margin;
	curr_loc.y += (move_loc.cy + line_spacer);

	move_loc = ShowText(&memDC, "Gold: ", 
		                curr_loc.x, curr_loc.y); 
	
	curr_loc.x += move_loc.cx;

	int gold = pDoc->playerStats[pDoc->gameStats.active_player].getGold();
	move_loc = ShowText(&memDC, Int2Char(gold),
			            curr_loc.x, curr_loc.y); 

	curr_loc.x = left_margin;
	curr_loc.y += (move_loc.cy + line_spacer);

	//display active piece/location information
	if (thePieces->active_piece >= 0)
	{
		paintPiece(&memDC, thePieces, 
			       thePieces->active_piece, 
				   curr_loc.x, curr_loc.y);
		
		//record location as part of view, so mouse clicks can be detected
		updateActiveClickPos(curr_loc.x, curr_loc.y);

		int active_piece = thePieces->GetActive();
		int piece_type = thePieces->List_of_pieces[active_piece].piece_type;

		curr_loc.x += (sprite_size + field_spacer);
		move_loc = ShowText(&memDC, "Moves:", 
			                curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + field_spacer);
		int moves = thePieces->GetMoves(active_piece);

		int whole_moves, part_moves;
		whole_moves = thePieces->GetMoves(active_piece) / 3;
		part_moves = thePieces->GetMoves(active_piece) % 3;
		move_loc = ShowText(&memDC, 
			                Int2Char(whole_moves), 
			                curr_loc.x, curr_loc.y);

		//display location of piece
		int small_field_spacer = 1;
		curr_loc.x = left_margin + sprite_size + field_spacer;
		curr_loc.y += (move_loc.cy + line_spacer);
		move_loc = ShowText(&memDC, "Locat: (", 
			                curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + small_field_spacer); 
		int piece_loc_x, piece_loc_y;
		activeList->GetPosition(active_piece, piece_loc_x, piece_loc_y);
		move_loc = ShowText(&memDC, Int2Char(piece_loc_x + 1), 
			                curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + small_field_spacer); 
		move_loc = ShowText(&memDC, ",", 
			                curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + small_field_spacer); 
		move_loc = ShowText(&memDC, Int2Char(piece_loc_y + 1), 
			                curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + small_field_spacer); 
		move_loc = ShowText(&memDC, ")", 
			                curr_loc.x, curr_loc.y);


		//Display the piece type name
		curr_loc.x = left_margin;
		curr_loc.y += (move_loc.cy + line_spacer);
		char* type_text = pDoc->getPlayerAdj(pDoc->gameStats.active_player);
		move_loc = ShowText(&memDC, type_text, 
							curr_loc.x, curr_loc.y);
		curr_loc.x += (move_loc.cx + field_spacer);
		type_text = thePieces->Piece_attributes[piece_type].name;
		move_loc = ShowText(&memDC, type_text, 
							curr_loc.x, curr_loc.y);

		//Display movement status of piece
		if (thePieces->hasDestination(active_piece))
		{
			curr_loc.x = left_margin;
			curr_loc.y += (move_loc.cy + line_spacer);
			CPoint dest = thePieces->getDestination(active_piece);
			move_loc = ShowText(&memDC, "Destination: (", curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, Int2Char(dest.x + 1), curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, ",", curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, Int2Char(dest.y + 1), curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, ")", curr_loc.x, curr_loc.y);
		}
		if (thePieces->onPatrol(active_piece))
		{
			curr_loc.x = left_margin;
			curr_loc.y += (move_loc.cy + line_spacer);
			CPoint dest = thePieces->getDestPatrolPoint(active_piece);
			move_loc = ShowText(&memDC, "On patrol to: (", curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, Int2Char(dest.x + 1), curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, ",", curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, Int2Char(dest.y + 1), curr_loc.x, curr_loc.y);
			curr_loc.x += move_loc.cx + small_field_spacer;
			move_loc = ShowText(&memDC, ")", curr_loc.x, curr_loc.y);
		}
			
		//paint cargo of active piece (if applicable)
		curr_loc.x = left_margin;
		curr_loc.y += (move_loc.cy + line_spacer);

		int max_holds = thePieces->Piece_attributes[piece_type].num_cargo_holds;

		if (max_holds > 0)
		{
			move_loc = ShowText(&memDC, "With:", curr_loc.x, curr_loc.y);
		    curr_loc.y += (move_loc.cy + line_spacer);

			if (thePieces->getAvailPieceHolds(active_piece) == max_holds)
				move_loc = ShowText(&memDC, "<no cargo>", curr_loc.x, curr_loc.y);
			else
			{
				for (int hold_nbr = 0; hold_nbr < max_holds; hold_nbr++)
				{
					int cargo_piece = thePieces->getPieceCargo(active_piece, hold_nbr);
					if (cargo_piece >= 0)
					{
						paintPiece(&memDC, thePieces, 
								   cargo_piece, 
								   curr_loc.x, curr_loc.y);
						curr_loc.x += (sprite_size + field_spacer);
						if (curr_loc.x > r.right - (sprite_size + field_spacer))
							curr_loc.SetPoint(left_margin, curr_loc.y + sprite_size + line_spacer);
					}
					//else if the cargo is a commodity (separate loop...)
				}
			}
		}
	}

	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(display_x, display_y, r.right - display_x, 
		        r.bottom - display_y, 
				&memDC, display_x, display_y, SRCCOPY);

	//reselect old objects
	memDC.SelectObject(oldbm1);
	dcMem1.SelectObject(oldbm2);
}

void CColAusView::updateActiveClickPos(int x, int y)
{
	active_click_pos.x = x;
	active_click_pos.y = y;
}

CPoint CColAusView::getActiveClickPos(void)
{
	return active_click_pos;
}

int CColAusView::paintMiniMap(CDC *pDC, Map *theMap, PieceList *thePieces, int x, int y)
{
	CDC dcMem1, dcMem2, dcMem3, dcMem4, dcMem5, dcMem6;
	dcMem1.CreateCompatibleDC(pDC); 
	dcMem2.CreateCompatibleDC(pDC); 
	dcMem3.CreateCompatibleDC(pDC); 
	dcMem4.CreateCompatibleDC(pDC); 
	dcMem5.CreateCompatibleDC(pDC);
	dcMem6.CreateCompatibleDC(pDC);

	CBitmap *oldbm1 = dcMem1.SelectObject(&map_graphics->undiscovered);
	CBitmap *oldbm2 = dcMem2.SelectObject(&map_graphics->mBmLand_terrain_types);
	CBitmap *oldbm3 = dcMem3.SelectObject(&map_graphics->mBmSea);
	CBitmap *oldbm4 = dcMem4.SelectObject(&map_graphics->pieces_colors);
	CBitmap *oldbm5 = dcMem5.SelectObject(&map_graphics->hills);
	CBitmap *oldbm6 = dcMem6.SelectObject(&map_graphics->mountains);

	CPoint mm_sqSize = getMMSqDim(theMap);
	CPoint lm_sqSize = getMapSquareSize();

	int sq_width = mm_sqSize.x; 
	int sq_height = mm_sqSize.y;

	//calculate the top left corner of each hills/map square to display
	int hill_x = (lm_sqSize.x - mm_sqSize.x) / 2;
	int hill_y = (lm_sqSize.y - mm_sqSize.y) / 2;

	//need to calculate the top left corner, and display next MINI_MAP_WIDTH squares
	//display will attempt to center the actual map display, and adjust
	//accordingly where this is outside the map bounds
	int graphic_offset = 50;
	int piece_counter;
	int start_x, start_y;
	start_x = display_start.x - (MINI_MAP_WIDTH / 2) + (DISP_WIDTH / 2);
	start_y = display_start.y - (MINI_MAP_HEIGHT / 2) + (DISP_HEIGHT / 2);
	if (start_x < 0)
		start_x = 0;
	if (start_y < 0)
		start_y = 0;
	if (start_x > (theMap->MAP_WIDTH - MINI_MAP_WIDTH))
		start_x = theMap->MAP_WIDTH - MINI_MAP_WIDTH;
	if (start_y > (theMap->MAP_HEIGHT - MINI_MAP_HEIGHT))
		start_y = theMap->MAP_HEIGHT - MINI_MAP_HEIGHT;

	if (start_x < 0)
		start_x = 0;
	if (start_y < 0)
		start_y = 0;
	int finish_x = start_x + MINI_MAP_WIDTH;
	int finish_y = start_y + MINI_MAP_HEIGHT;
	if (finish_x >= theMap->MAP_WIDTH)
		finish_x = theMap->MAP_WIDTH;
	if (finish_y >= theMap->MAP_HEIGHT)
		finish_y = theMap->MAP_HEIGHT;

	for (int i = start_x; i < finish_x; i ++)
	{
		for (int j = start_y; j < finish_y; j++)
		{

			if ((theMap->Land[i][j].explored == FALSE)
			&&  (t_hide_unexplored))
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height),  
						    sq_width, sq_height, 
							&dcMem1, 0, 0, 
							SRCCOPY);
			else if (theMap->Land[i][j].altitude <= SEALEVEL)
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height), 
						    sq_width, sq_height, 
							&dcMem3, 0, 0, 
							SRCCOPY);
			else if (theMap->Land[i][j].altitude >= SNOWLINE)
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height), 
						    sq_width, sq_height, 
							&dcMem6, hill_x, hill_y, 
							SRCCOPY);
			else if (theMap->Land[i][j].altitude >= HILLLINE)
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height), 
						    sq_width, sq_height, 
							&dcMem5, hill_x, hill_y, 
							SRCCOPY);
			else 
			{
				int terr_type = theMap->Land[i][j].land_terrain_type * 50;
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height),  
						    sq_width, sq_height, 
						    &dcMem2, 0, terr_type, 
						    SRCCOPY);
			}

			piece_counter = theMap->Land[i]
			                            [j].piece;
	        if ( (piece_counter != NO_PIECE)
			&&   (theMap->Land[i]
			                  [j].recently_seen == true) )
			{
				piece_counter = thePieces->GetOwner(piece_counter);
				pDC->BitBlt(x + ((i-start_x) * sq_width), 
							y + ((j-start_y) * sq_height),  
						    sq_width, sq_height, 
						    &dcMem4, 
							0, piece_counter * graphic_offset, 
						    SRCCOPY);				
			}
		}
	}
	CPen grey_pen(PS_SOLID, 2, RGB(16, 16, 0));
	CPen *oldpen1 = pDC->SelectObject(&grey_pen);

	int top_x, top_y, bot_x, bot_y;
	top_x = x + (((display_start.x) - start_x) * sq_width);
	top_y = y + (((display_start.y) - start_y) * sq_height);
	bot_x = top_x + (DISP_WIDTH * sq_width);
	bot_y = top_y + (DISP_HEIGHT * sq_height);

	pDC->MoveTo(top_x, top_y);
	pDC->LineTo(top_x, bot_y);
	pDC->LineTo(bot_x, bot_y);
	pDC->LineTo(bot_x, top_y);
	pDC->LineTo(top_x, top_y);

	//reselect old objects
	dcMem1.SelectObject(oldbm1);
	dcMem2.SelectObject(oldbm2);
	dcMem3.SelectObject(oldbm3);
	dcMem4.SelectObject(oldbm4);
	dcMem4.SelectObject(oldbm5);
	dcMem4.SelectObject(oldbm6);
	pDC->SelectObject(oldpen1);

	return 0;
}

int CColAusView::paintPiece(CDC *pDC, PieceList *thePieces, int piece_number, int x, int y)
{
	CDC dcMem1, dcMem2;
	dcMem1.CreateCompatibleDC(pDC); 
	dcMem2.CreateCompatibleDC(pDC); 

	CBitmap *oldbm1 = dcMem1.SelectObject(&map_graphics->pieces);
	CBitmap *oldbm2 = dcMem2.SelectObject(&map_graphics->pieces_colors);

	//get graphic square dimensions bitmapgraphics...
	int piece_graphic_offset = map_graphics->getMapFrameSize();
	CPoint map_sq_size = map_graphics->getMapSquareSize();
	int piece_graphic_height = map_sq_size.y;
	int piece_graphic_width = map_sq_size.x;
	int return_value;
	char piece_dir = thePieces->getPieceDir(piece_number);
	int piece_dir_offset = map_graphics->getPieceDirOffset(piece_dir);
	int the_piece_type = thePieces->List_of_pieces[piece_number].piece_type;

	if (!(MyMaskBlt(    pDC->GetSafeHdc(),
		                x, 
						y, 
	                    piece_graphic_width, piece_graphic_height, 
						dcMem1.GetSafeHdc(), 
			    		piece_dir_offset * piece_graphic_offset, 
						the_piece_type * piece_graphic_offset, 
		    			map_graphics->pieces_mask, 
			    	    piece_dir_offset * piece_graphic_offset, 
						the_piece_type * piece_graphic_offset, 
				    	MAKEROP4(DEST_vh, SRCCOPY))))
	{
		return_value = 2;
	}

	if (!(MyMaskBlt(    pDC->GetSafeHdc(),
						x, 
						y, 
						piece_graphic_width, piece_graphic_height, 
						dcMem2.GetSafeHdc(), 
						0, 
						(thePieces->List_of_pieces[piece_number].piece_owner) * piece_graphic_offset, 
	    				map_graphics->pieces_mask, 
		    			(piece_dir_offset + 2) * piece_graphic_offset, 
						the_piece_type * piece_graphic_offset,
			    		MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 2;
	}


	char tag_char = thePieces->getTagType(piece_number);
	if ( (tag_char)
	&&   (display_tag_text) )
	{
		int tag_text_offset = map_graphics->getTagCharOffset(tag_char);
		CPoint tag_offset = map_graphics->getTagLoc(the_piece_type, piece_dir);
		if (!(MyMaskBlt(pDC->GetSafeHdc(),
						x + tag_offset.x, y + tag_offset.y, 
						10, 14, //TODO:dimensions of tag should be derived from map_graphics
						dcMem2.GetSafeHdc(), 
						0, map_graphics->getColor("BLACK"), 
	    				map_graphics->tag_text, 
		    			tag_text_offset, 0,
			    		MAKEROP4(DEST_vh, SRCCOPY))))
			return_value = 2;
	}

	//select old objects to eliminate memory leakage
	dcMem1.SelectObject(oldbm1);
	dcMem2.SelectObject(oldbm2);

	return 0;
}

int CColAusView::paintStat(CDC *pDC, int stat_value, int x, int y)
{
	CDC dcMem1, dcMem2;

	dcMem1.CreateCompatibleDC(pDC); 
	dcMem2.CreateCompatibleDC(pDC); 
	CBitmap *oldbm1 = dcMem1.SelectObject(&map_graphics->tag_text);
	CBitmap *oldbm2 = dcMem2.SelectObject(&map_graphics->pieces_colors);

	//get graphic square dimensions 
	CPoint map_sq_size = getMapSquareSize();

	int return_value = 0;

	if (stat_value > 999)
		stat_value = 999;
	if (stat_value < -999)
		stat_value = -999;

	int digits = 0;
	if (abs(stat_value) < 10)
		digits = 1;
	else if (abs(stat_value) < 100)
		digits = 2;
	else digits = 3;

	char *numb_str = Int2Char(stat_value);
	char sign_str[4] = "-";
	char *text_str;

	if (stat_value < 0)
	{
		strcat(sign_str, numb_str);
		text_str = sign_str;
		digits++;
	}
	else
		text_str = numb_str;

	//TODO:dimensions of tag should be derived from map_graphics
	int text_height = 14;
	int text_width  = 10;

	//x & y give the top left pixel location of the current map square
	//need to adjust these to center the text horizontally & vertically
	int x_ctr = (map_sq_size.x / 2) - (digits * text_width / 2); 
	int y_ctr = (map_sq_size.y / 2) - (text_height / 2);

	for (int count = 0; count < digits; count++)
	{
		int tag_text_offset = map_graphics->getTagCharOffset(text_str[count]);
		CPoint tag_offset(x_ctr, y_ctr);

		/*BitBlt instead to show numbers more clearly
		if (!(MyMaskBlt(pDC->GetSafeHdc(),
						x + tag_offset.x, y + tag_offset.y, 
						10, 14, //width and height of a tag text element 
						dcMem2.GetSafeHdc(), 
						0, map_graphics->getColor("BLACK"), 
	    				map_graphics->tag_text, 
		    			tag_text_offset, 0,
			    		MAKEROP4(DEST_vh, SRCCOPY))))
			return_value = 2;
		*/
		pDC->BitBlt(x + tag_offset.x, y + tag_offset.y,
			        10, 14,
					&dcMem1,
					tag_text_offset, 0,
					SRCCOPY);

		x_ctr += text_width;
	}

	//select old objects to eliminate memory leakage
	dcMem1.SelectObject(oldbm1);
	dcMem2.SelectObject(oldbm2);

	return return_value;
}

void CColAusView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Possibly record the time the button went down.
	CView::OnLButtonDown(nFlags, point);
}

void CColAusView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	bool redraw_flag = false;

	//reset the patrol_dest_pending flag
	patrol_dest_pending = false;

	//decide what screen is being displayed...and call
	//appropriate function to process the key
	//currently this function deals with the map/main view only

	//int key_type = getKeyType(nChar);
	//switch(key_type)...

	testOutFile << "Key pressed, nChar :" << nChar 
		        << ", nRepCnt :" << nRepCnt
				<< ", nFlags :" << nFlags << endl;

	if  (current_display_type == ENC_DISPLAY)
	{
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	bool move_key_pressed = isMoveKey(nChar);

	//might not be needed.....
	if ( (activeMap == NULL)
	||   (activeList == NULL) )
	{
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		move_key_ok = true;
		return;
	}

	//need to ignore if the previous move/end of turn is still being processed

	if (move_key_pressed)
		if (move_key_ok)
		{
			move_key_ok = false;
			processMoveKey(nChar);
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
			//current display is map, so no need to redraw the whole view....?
			Invalidate(FALSE);
			//this gets set at the end of ondraw...
			move_key_ok = true;
		    return;
		}
		else
		{
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}

	if (!move_key_pressed)
	{
		if (nChar == VK_SPACE)
		{
			testOutFile << "Space key pressed." << endl;
			processSkipMove();
			Invalidate(FALSE);
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		    return;
		}
		//'D' or 'd'
		if ( (nChar == 0x44)
		|| 	 (nChar == 0x64) )
		{
			//testOutFile << "D key pressed." << endl;
			processDisbandMove();
			Invalidate(FALSE);
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		    return;
		}
		//'W' or 'w'
		if ( (nChar == 0x57)
		|| 	 (nChar == 0x77) ) 
		{
			//testOutFile << "W key pressed." << endl;
			processWaitMove();
			Invalidate(FALSE);
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		    return;
		}
		//'S' or 's'
		if ( (nChar == 0x53)
		|| 	 (nChar == 0x73) ) 
		{
			//testOutFile << "S/s key pressed." << endl;
			processSleepMove();
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		    return;
		}
		//'P' or 'p'
		if ( (nChar == 0x50)
		|| 	 (nChar == 0x70) ) 
		{
			//testOutFile << "P/p key pressed." << endl;
			processPatrolMove();
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		    return;
		}

	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	move_key_ok = true;
}

int  CColAusView::checkPieceMove(int piece_number, CPoint dest)
{
	//returns the number of moves points the move would cost
	//if the return value is negative then the move is not allowed
	//NB. maybe this routine should not check for opposition pieces...

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int return_value  = -1;

	int piece_type = activeList->getPieceType(piece_number);
	int dest_piece = activeMap->getPiece(dest.x, dest.y);

	if (!activeMap->mapMoveOk(activeList->GetPosition(piece_number), dest))
		return return_value;

	//validate destination square based on piece move type
	if (activeMap->isSeaSquare(dest.x, dest.y))
	{
		if  (!activeList->seaType(piece_type))
		{
			if (dest_piece == EMPTY)
				return return_value;
			int dest_owner = activeList->GetOwner(dest_piece);
			int curr_owner = activeList->GetOwner(piece_number);
			// unless moving to a ship of the current player, 
			if (dest_owner != curr_owner)
				return return_value;
			//..that has space to hold the piece
			if (activeList->getAvailPieceHolds(dest_piece) <= 0)
				return return_value;
			else
				return_value = 3;
		}
		else
			if (dest_piece != EMPTY)
				return return_value;
			else 
				return_value = 3;
		
		return return_value;
	}

	if (!activeMap->isSeaSquare(dest.x, dest.y))
	{
		if  (activeList->seaType(piece_type))
		// unless moving to a colony of the current player
			return -1;
		if (dest_piece == EMPTY)
			return 3;
		else
			return -1;
	}
	
	//all this is for the routine to actually make the move...
	//if colony in destination square....

	//if land piece type and currently on a ship
	//activeList->disembarkPiece(piece_type)

	//if piece is currently in a colony
	
	//we have now established that the move is to a vacant square
	if (activeMap->isSeaSquare(dest.x, dest.y))
	{	
		return_value = 3;
	}
	else
	{
		//int move_cost = activeMap->getMoveCost(dest);
		//if  (activeMap->hasRoad(dest))
		//	return_value.SetPoint(0,move_cost);
		//else
		//	return_value.SetPoint(move_cost,0);
		return_value = 3;
	}
	return return_value;
}

int  CColAusView::checkPieceMoveTerrain(int piece_number, CPoint dest)
{
	//returns the number of moves points the move would cost
	//if the return value is negative then the move is not allowed
	//NB. ignores whether destination square is occupied

	int piece_type = activeList->getPieceType(piece_number);

	//validate destination square based on piece move type
	if ( (activeMap->isSeaSquare(dest.x, dest.y))
	&&   (!activeList->seaType(piece_type)) )
		return -1;

	if ( (!activeMap->isSeaSquare(dest.x, dest.y))
	&&   (activeList->seaType(piece_type)) )
		return -1;
	
	//get move cost from terrain properties....to compute exact move cost
	return 3;
}

bool  CColAusView::checkPieceCanMoveTo(int piece_number, CPoint dest)
{
	//returns true if this square is of a terrani type that can be moved to by this
	//piece, and the destination square is vacant

	int piece_type = activeList->getPieceType(piece_number);

	//validate destination square based on piece move type
	if ( (activeMap->isSeaSquare(dest.x, dest.y))
	&&   (!activeList->seaType(piece_type)) )
		return false;

	if ( (!activeMap->isSeaSquare(dest.x, dest.y))
	&&   (activeList->seaType(piece_type)) )
		return false;
	
	if (activeMap->getPiece(dest.x, dest.y) != EMPTY)
		return false;
	else
		return true;
}

CPoint CColAusView::getDestPieceMove(int piece_number)
{
	//gets the next square to move to for a piece that has a destination
	//returns (-1, -1) to indicate no move was valid
	
	CPoint err_value(-1, -1);
	CPoint dest;

	//return an error if the piece does not have a valid goto/patrol status
	if ( (!activeList->hasDestination(piece_number))
	&&   (!activeList->onPatrol(piece_number)) )
		return err_value;

	//get the current location & destination for the piece
	if (activeList->hasDestination(piece_number))
		dest = activeList->getDestination(piece_number);
	if (activeList->onPatrol(piece_number))
		dest = activeList->getDestPatrolPoint(piece_number);

	CPoint curr = activeList->GetPosition(piece_number);

	//handle case where the piece has already arrived at it's destination
	if (dest == curr)
	{
		if (activeList->hasDestination(piece_number))
		{
			activeList->removeDestination(piece_number);
			return err_value;
		}
		else if (activeList->onPatrol(piece_number))
		{
			activeList->reverseDestPatrolPoint(piece_number);
			dest = activeList->getDestPatrolPoint(piece_number);
		}
		else
			return err_value;
	}

	//check if the piece has moves in it's route array, 
	//and if so return the next if that square is empty
	if (activeList->List_of_pieces[piece_number].hasRoute())
	{
		CPoint next_pt;
		next_pt = activeList->List_of_pieces[piece_number].getNextRoutePt();
		int route_pt_nbr = activeList->List_of_pieces[piece_number].getRoutePtr();
		//if ( (abs(next_pt.x - curr.x) > 1)
		//||   (abs(next_pt.y - curr.y) > 1) )
		//	bool stop_here = true;
		if ( (activeMap->getPiece(next_pt.x, next_pt.y) == EMPTY)
		&& ( (abs(next_pt.x - curr.x) <= 1) && (abs(next_pt.y - curr.y) <= 1) ) )
			return next_pt;
	}

	//otherwise the piece needs to have it's pathlist rebuilt
	//initialise both route/old_route only if piece is on patrol
	if (activeList->onPatrol(piece_number))
		activeList->List_of_pieces[piece_number].initRoute(true);
	else
		activeList->List_of_pieces[piece_number].initRoute(false);

	pathFinder router;

	//initialise matrix with values required, true means the square can be on the path
	bool sea_type;
	if (activeList->seaType(activeList->getPieceType(piece_number)))
		sea_type = true;
	else
		sea_type = false;

	int nav_radius = 5;
	int nav_diameter = (nav_radius * 2) + 1;
	for (int i = nav_radius * -1; i <= nav_radius; i++)
		for (int j = nav_radius * -1; j <= nav_radius; j++)
		{
			CPoint map_square(curr.x + i, curr.y + j);
			if (activeMap->getPiece(i + curr.x, j + curr.y) == EMPTY)
			{
				if (checkPieceCanMoveTo(piece_number, map_square))
					router.set_matrix_point(i + nav_radius, j + nav_radius, true, true);
				else
					if (sea_type)
						router.set_matrix_point(i + nav_radius, j + nav_radius, false, false);
					else
						router.set_matrix_point(i + nav_radius, j + nav_radius, false, true);
			}
			if ((i == 0) && (j == 0))
				//current location of piece cannot be on the path
				router.set_matrix_point(i + nav_radius, j + nav_radius, false, true);
		}
		
	//need to determine the closest square to the actual destination in the navigable 
	//area that the piece can actually move to
	CPoint interim_dest(dest.x, dest.y);

	if (dest.x < curr.x - nav_radius)
		interim_dest.x = curr.x - nav_radius;
	else if (dest.x > curr.x + nav_radius)
		interim_dest.x = curr.x + nav_radius;

	if (dest.y < curr.y - nav_radius)
		interim_dest.y = curr.y - nav_radius;
	else if (dest.y > curr.y + nav_radius)
		interim_dest.y = curr.y + nav_radius;

	if ( (interim_dest != dest)
	&&   (!checkPieceCanMoveTo(piece_number, interim_dest)) )
	{
		CRect search_area(curr.x - nav_radius, curr.y - nav_radius,
						  curr.x + nav_radius, curr.y + nav_radius);
		CRect map_area(0,0,activeMap->MAP_WIDTH - 1,activeMap->MAP_HEIGHT - 1);
		CRect clipped_area;
		clipped_area.IntersectRect(search_area, map_area);
		interim_dest = findClosestCompSquare(interim_dest,piece_number,clipped_area);
	}

	//TODO:
	//calculate if the piece is blocked?????
	//ie interim destination is further from the destination that the current location..

	//determine start and finish points in terms of the local pathFinder coordinates
	CPoint start(nav_radius,nav_radius);
	CPoint finish(interim_dest.x - curr.x + nav_radius, 
		          interim_dest.y - curr.y + nav_radius);

	int path_length = router.findMinPath(start, finish, nav_diameter);

	if (path_length < MAX_INT)
	{
		//activeList->List_of_pieces[piece_number].copyToOldRoute();
		//copy the route to the old_route, initialise the new one
		//activeList->List_of_pieces[piece_number].initRoute(false);
		//last point on path must be ignored
		for ( int i = 1; 
			((i < MAX_RECORDED_PATH - 1) && (i <= path_length));
			  i++)
		{
			CPoint next_pt;
			next_pt = router.getPathPoint(i);
			next_pt.x += curr.x - nav_radius;
			next_pt.y += curr.y - nav_radius;
			activeList->List_of_pieces[piece_number].setNextRoutePt(next_pt);
		}
		//return an error if the next part of the route overlaps with the old part,
		//i.e. a circular route has been created
		activeList->List_of_pieces[piece_number].setStartPoint(curr);

		if (activeList->List_of_pieces[piece_number].circRoute())
			return err_value;

		//if piece not on patrol
		//and piece has not just been manually allocated a new destination
		//CPoint prev_loc = activeList->List_of_pieces[piece_number].getPrevPos();
		//if (activeList->List_of_pieces[piece_number].posInRoute(prev_loc))
		//	return err_value;
	}
	else
		return err_value;

	CPoint nloc;
	nloc = activeList->List_of_pieces[piece_number].getNextRoutePt();

	//check move is legal, and get move cost
	//probably redundant, as this has already taken place.....
	int move_cost  = checkPieceMove(piece_number, nloc);
	int dest_piece = activeMap->getPiece(nloc.x, nloc.y);

	//check we are moving only one square at a time
	if ( (abs(nloc.x - curr.x) > 1)
	||   (abs(nloc.y - curr.y) > 1) )
			bool stop_here = true;

	if ((move_cost <= 0) || (dest_piece != EMPTY))
	{
		activeList->List_of_pieces[piece_number].initRoute();
		return err_value;
	}
	else
		return nloc;



	//decode the return path, and add to pieces 'current route' list

	//return move to square as next available from current route

	//if the route is still empty then return error point to wake piece

	//logic for determining the square the piece will move to 
	/*
	CPoint nav_from = curr;
	CPoint nav_to = dest;
	int nav_distance = 5;

	//CPoint navigatetoPoint(CPoint nav_from, CPoint nav_to, int nav_distance, int piece_number);
	//navigatetoPoint(curr, dest, 5, piece_number);

	int nav_terr_type;
	if (activeList->seaType(activeList->getPieceType(piece_number)))
		nav_terr_type = SEA;
	else
		nav_terr_type = LAND;

	//establish navigable area square
	CRect nav_rect(nav_from, nav_from);

	nav_rect.left  -= (nav_distance/2); 
	nav_rect.right += (nav_distance/2);
	//shift to ensure within map bounds
	if (nav_rect.right >= activeMap->MAP_WIDTH)
		nav_rect.right = activeMap->MAP_WIDTH - 1;
	if (nav_rect.left < 0)
		nav_rect.left = 0;

	nav_rect.top  -  = (nav_distance/2); 
	nav_rect.bottom += (nav_distance/2);
	//shift to ensure within map bounds
	if (nav_rect.bottom >= activeMap->MAP_HEIGHT)
		nav_rect.bottom = activeMap->MAP_HEIGHT - 1;
	if (nav_rect.top < 0)
		nav_rect.top = 0;

	//looping might start here

	//find the next move in the shortest navigable path between nav_from 
	//and nav_to, or the naviable square in the nav_rect closest to nav_to

	nloc = nav_from;
	n_loc.x--;
	n_loc.y--;
	if (nav_from.x >= activeMap->MAP_WIDTH)
		nav_from.x = active_map->MAP_WIDTH -1;
	if (nav_from.y >= activeMap->MAP_HEIGHT)
		nav_from.y = active_map->MAP_HEIGHT -1;

	CPoint dest_pt;
	if (nav_rect.PtInRect(nav_to))
		dest_pt = nav_to;
	else
		//decide closest point, to use as interim destination
	{
		if (nav_to.x > nav_rect.right)
			dest_pt.x = nav_rect.right;
		else if (nav_to.x < nav_rect.left)
			dest_pt.x = nav_rect.left;
		else
			dest_pt.x = nav_to.x;
		if (nav_to.y > nav_rect.bottom)
			dest_pt.y = nav_rect.bottom;
		else if (nav_to.y < nav_rect.top)
			dest_pt.y = nav_rect.top;
		else
			dest_pt.y = nav_to.y;
		
	//if there is no navigable path,then choose another
	if (dest_pt != nav_to)
		//need to confirm square is one which the piece can move to
		//and adjust to closest such square if not???
		//otherwise algorithm will be one that follows a general direction only
		//until the dest square is in the naviagtion area.
		//the piece will be 'blocked' if there is no navigable path at this point.
		;

	for (n_loc.x; n_loc.x <= (nav_from.x + 1);n_loc.x++)
		for (n_loc.y; n_loc.y <= (nav_from.y + 1);n_loc.y++)
			if ((checkPieceMove(piece_number, nloc))
			&&  (activeMap->getPiece(nloc.x, nloc.y)))
			{

			}
	
	*/
	//CPoint nloc;
	CPoint offset(0,0);
	
	if (curr.x > dest.x)
		offset.x--;
	else if (curr.x < dest.x)
		offset.x++;

	if (curr.y > dest.y)
		offset.y--;
	else if (curr.y < dest.y)
		offset.y++;

	nloc.x = curr.x + offset.x;
	nloc.y = curr.y + offset.y;

	//check move is legal, and get move cost
	move_cost  = checkPieceMove(piece_number, nloc);
	dest_piece = activeMap->getPiece(nloc.x, nloc.y);

	if ((move_cost < 0) || (dest_piece != EMPTY))
	{
		//if both are not zero, we can try a different move either vertically or horizontally
		if ((offset.x != 0) && (offset.y != 0))
		{
			if (rand() % 2)
			{
				nloc.x -= offset.x;
				move_cost = checkPieceMove(piece_number, nloc);
				dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			}
			if ((move_cost < 0) || (dest_piece != EMPTY))
			{
				nloc.y -= offset.y;
				move_cost = checkPieceMove(piece_number, nloc);
				dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			}
		}
		if ((offset.y == 0)
		&&  (offset.x != 0))
		{
			nloc = curr;
			nloc.y = curr.y - 1;
			move_cost = checkPieceMove(piece_number, nloc);
			dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			if ((move_cost < 0) || (dest_piece != EMPTY))
			{
				nloc.y += 2;
				move_cost = checkPieceMove(piece_number, nloc);
				dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			}
		}
		if ((offset.x == 0)
		&&  (offset.y != 0))
		{
			nloc = curr;
			nloc.x--;
			move_cost = checkPieceMove(piece_number, nloc);
			dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			if ((move_cost < 0) || (dest_piece != EMPTY))
			{
				nloc.x += 2;
			    move_cost = checkPieceMove(piece_number, nloc);
				dest_piece = activeMap->getPiece(nloc.x, nloc.y);
			}
		}

	}

	//returning the error value causes the piece to be 'woken' in calling code
	if ((move_cost < 0) || (dest_piece != EMPTY))
		return err_value;

	//note - possibly friendly pieces in square is acceptable.....
	return nloc;

	//return nav_from;

}

CPoint CColAusView::findClosestCompSquare(CPoint start, 
										  int piece_number, 
										  CRect search_area)
{
	//nb - eventually the second paramater would be a function, evaluating
	//if the given square is okay or not(paramater would just be the CPoint...?

	//if the current square is compatible, then return it 
	if (checkPieceCanMoveTo(piece_number, start))
		return start;

	//search all the surrounding squares, and return the first that is compatible
	int i, j; 
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++)
		{
			CPoint new_point(start);
			new_point.Offset(i,j);
			if ( ((abs(i) > 0) || (abs(j) > 0))
			&&   (search_area.PtInRect(new_point)) )
				if (checkPieceCanMoveTo(piece_number,new_point))
					return new_point;
		}

	i = j = -1;
	//if none of the surrounding squares are compatible, then search from each in turn
	for (i = -1; i <= 1 ; i++)
		for (j = -1; j <= 1; j++)
		{
			CPoint new_point(start);
			new_point.Offset(i,j);
			if ( ((abs(i) > 0) || (abs(j) > 0))
			&&   (search_area.PtInRect(new_point)) )
				return findClosestCompSquare(new_point,piece_number,search_area);
		}
}

void CColAusView::processSkipMove()
{
	//update piecelist to reflect skipped move
	int active_piece = activeList->GetActive();
	if (active_piece == EMPTY)
		return;
	activeList->ZeroMoves(active_piece);

	//then process to next piece ready to be moved
	processToNextPiece();
}

void CColAusView::processToNextPiece(void)
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int current_owner = pDoc->gameStats.getActivePlayer();
	int active_piece = activeList->GetNextPiece(current_owner);
		
	if (active_piece == EMPTY)
	{
		if (pDoc->gameStats.auto_end_turn) 
		{
			pDoc->endTurn();
			//current_owner = pDoc->gameStats.getActivePlayer();
			//active_piece = activeList->GetNextPiece(current_owner);
			active_piece = activeList->GetActive();
			//activeList->UpdateActive(active_piece);
			//reCenter_active(activeMap, activeList);
			Invalidate(FALSE);
		}	
	}
	else
	{
		activeList->UpdateActive(active_piece);

		if (reCenter_active(activeMap, activeList))
			paintMapScreen(GetDC(), activeMap);
			//paintMap(GetDC(), activeMap, DISP_WIDTH, DISP_HEIGHT);

		processMoveToDest(active_piece);
	}
}

void CColAusView::processWaitMove()
{
	//update piecelist & map to reflect skipped move
	//if there are no more pieces to move then the current piece will be re-activated

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	processToNextPiece();
}

void CColAusView::processDisbandMove()
{
	//update piecelist & map to reflect disbanding of the current piece
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//first confirm action with user


	int active_piece = activeList->GetActive();
	CPoint piece_loc = activeList->GetPosition(active_piece);

	if  (activeMap->getPiece(piece_loc.x, piece_loc.y) == active_piece)
	{
		activeMap->removePiecefromSquare(piece_loc);
		pDoc->displayMoves.addMoveInfo(piece_loc, piece_loc, active_piece, 
									   pDoc->gameStats.getActivePlayer(), DISBAND);

		activeList->killPiece(active_piece, pDoc->gameStats.getDateInt());
		//activeList->UpdateActive(EMPTY);
	}

	paintMapScreen(GetDC(), activeMap);
	processToNextPiece();
}

void CColAusView::processSleepMove()
{

	//update piecelist & map to reflect disbanding of the current piece
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int active_piece = activeList->GetActive();

	if (active_piece < 0)
		return;

	activeList->putToSleep(active_piece);
	//activeList->UpdateActive(EMPTY);

	paintMapScreen(GetDC(), activeMap);
	processToNextPiece();
}

void CColAusView::processPatrolMove()
{
	//set a flag to indicate the patrol destination is pending
	//only if the next action is a mouse-click to a valid square
	// will the patrol be set up - otherwise the flag will be reset
	patrol_dest_pending = true;

	//TODO:add text to status bar to help user set the patrol
}

void CColAusView::processMoveKey(UINT nChar)
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int current_owner = pDoc->gameStats.getActivePlayer();
	int active_piece = activeList->GetActive();
	
	int old_loc_x, old_loc_y;
	activeList->GetPosition(active_piece, old_loc_x, old_loc_y);
	int new_loc_x = old_loc_x;
	int new_loc_y = old_loc_y;
	CPoint new_loc(old_loc_x, old_loc_y);
	CPoint old_loc(old_loc_x, old_loc_y);

	if (active_piece >= 0)
		//find out if the move requested follows the rules of the map, and if so
		//get the new location
		if ((activeMap->processMove(nChar, new_loc_x, new_loc_y)))
		{
			new_loc.SetPoint(new_loc_x, new_loc_y);

			//update orientation of the piece
			activeList->updateDir(active_piece, old_loc.x, new_loc.x);

			//check move is legal for piece type, and get cost of move
			//nb. move cost is negative if dest square is occupied by an opponent
			int move_cost = checkPieceMove(active_piece, new_loc);
			int dest_piece = activeMap->getPiece(new_loc_x, new_loc_y);
			int owner =activeList->GetOwner(dest_piece);
	
			//if piece has sufficient moves, and the move is legal for the piece
			if ( (move_cost <= activeList->GetMoves(active_piece))
			&&   (move_cost > 0) )
			{
                //if the dest square is empty, or is occupied by a friendly piece
				//that can accept cargo
				if ( (dest_piece == EMPTY) 
				||   (   (current_owner == owner) 
                      && (activeList->getAvailPieceHolds(dest_piece) > 0) )  )
				//    need to add code for multiple player pieces in one location
				// ||(activeList->List_of_pieces[active_piece].piece_owner == owner) )
				{
					//activeList->SetPosition(active_piece, new_loc_x, new_loc_y);
					activeList->ReduceMoves(active_piece, move_cost);
					int oldlocpiecebefore = activeMap->getPiece(old_loc.x, old_loc.y);
					movePiece(active_piece, old_loc, new_loc);
					int oldlocpieceafter = activeMap->getPiece(old_loc.x, old_loc.y);

					
					//updated map square so that it has details of new piece
					//now update explored/line of sight
					activeMap->move_key_pressed = TRUE;

					//below looks like a separate function, as it happens all the time...
					int line_of_sight = activeList->GetLoS(active_piece);
					//to ensure revealed Terrain features are the latest available
					//MasterMap updated with player's piece position changes at end of turn
					activeMap->synchMapTerrain(&(pDoc->MasterMap), new_loc, line_of_sight);
					activeMap->updateExplored(new_loc, line_of_sight);
					activeMap->updateRecentlySeen(new_loc, line_of_sight);
					//TODO:update player's map using MasterMap, for each square that
					//needs to reveal all terrain/piece/colony data about the map area
					//activeMap->synchMapTerrain(&(pDoc->MasterMap), new_loc, line_of_sight);

					//recenters the view (only if required)
					//reCenter_active(activeMap, activeList);
					//this should be a list, which is applied sequentially to master map
					//and is also a record of moves to show each player at the beginning
					//of their turn......
					//pDoc->addMove(old_loc_x, old_loc_y, new_loc_x, new_loc_y, active_piece);

					//animate move to the DC, saves doing Invalidate.....
					CDC *pDC = GetDC();

					if (reCenter_active(activeMap, activeList, false))
					{
						animation_pending = active_piece;
						//need to avoid displaying currently active piece in new location
						paintMapScreen(pDC, activeMap);
						animation_pending = 0;
					}
		
					CPoint move_from(old_loc_x, old_loc_y);
					CPoint   move_to(new_loc_x, new_loc_y);
		
					//update the map of the player, as per line of sight
					int return_value = animateMove(pDC, activeMap,
									 		       active_piece, 
											       move_from, move_to);

					pDoc->displayMoves.addMoveInfo(move_from, move_to, active_piece,
					                              pDoc->gameStats.getActivePlayer());
					piece_just_moved = true;
					//if the piece now has zero moves, then processToNextPiece()
					if (activeList->GetMoves(active_piece) == 0)
						processToNextPiece();
				}
			}
			else  //move cost is less than zero, or piece has insufficient moves
			{
				//if move was disallowed due to an oppostion piece being in the square
				int could_move_to = checkPieceMoveTerrain(active_piece, new_loc);
				if ( (dest_piece != EMPTY) 
				&&   (owner != current_owner)
				&&   (could_move_to > 0) 
				&&   (could_move_to <= activeList->GetMoves(active_piece)) )
				{
					int conflict_result = resolveConflict(active_piece, dest_piece);
					if (conflict_result >= 0)
						processToNextPiece();
						//TO DO:add a move to the movelist for the conflict....
				}
				if  ( (dest_piece == EMPTY)
				&&    (could_move_to > 0) )
				{
					activeList->ZeroMoves(active_piece);
					this->processToNextPiece();
				}
				//if the move was a ship trying to move onto a land square...
				if ( (!activeMap->isSeaSquare(new_loc_x, new_loc_y))
				&&   (activeList->isShip(active_piece)) )
				{
					//...wake all pieces on board
					activeList->wakeOnBoard(active_piece);
				}


		    }

		}
}

int CColAusView::resolveConflict(int attacker, int defender)
{
	//Resolves a conflict - called when a piece moves to a square occuppied by an opposing piece
	//A negative value indicates no conflict took place

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int att_owner = pDoc->gameStats.getActivePlayer();
	int def_owner = activeList->GetOwner(defender);
	CPoint old_loc = activeList->GetPosition(attacker);
	CPoint new_loc = activeList->GetPosition(defender);

	//check if the attacker is able to attack
	if (activeList->List_of_pieces[attacker].getAttStr() <= 0)
	{
		ColMsgBox message_dlg;
		int piece_type = activeList->getPieceType(attacker);
		int attribute_strength = activeList->Piece_attributes[piece_type].att_strength;
		if  (attribute_strength <= 0)
			message_dlg.message_str = "That piece cannot attack.";
		else
			message_dlg.message_str = "That piece has insufficient attack strength, and must visit a friendly colony before it can launch an attack.";
		message_dlg.DoModal();
		return -1;
	}

	//check if the players are at war, and if not then ask attacker to confirm
	if  (!(pDoc->playerStats[att_owner].atWarWith(def_owner)))
	{
		ConfirmAttackDlg confirm_dlg;
		//int country = pDoc->playerStats[owner].country;
		//confirm_dlg.updateCountry(pDoc->RuleBase.getAdjCountry(country));
		confirm_dlg.updateCountry(pDoc->getPlayerAdj(def_owner));
		if (confirm_dlg.DoModal() == IDOK)
		{
			pDoc->playerStats[att_owner].declareWarWith(def_owner);
			pDoc->playerStats[def_owner].declareWarWith(att_owner);
		}
	}
	if  (pDoc->playerStats[att_owner].atWarWith(def_owner))
	{
		//players are at war, so do the conflict
		//add the attack to the list of moves/events to display to other players
		pDoc->displayMoves.addMoveInfo(old_loc, new_loc, attacker,
		                              pDoc->gameStats.getActivePlayer(), ATTACK);

		ResolveAttackDlg resolve_dlg;

		//populate Bitmaps and Strings for the dialog
		CBitmap attacking_piece, background, defending_piece;
		CDC memDC;
		CDC backgroundDC;
		CDC *pDC = GetDC();
		memDC.CreateCompatibleDC(pDC); 
		backgroundDC.CreateCompatibleDC(pDC);

		CPoint square_dim = map_graphics->getMapSquareSize();
		attacking_piece.CreateCompatibleBitmap(pDC, square_dim.x, square_dim.y);
		defending_piece.CreateCompatibleBitmap(pDC, square_dim.x, square_dim.y);
		background.CreateCompatibleBitmap(pDC, square_dim.x, square_dim.y);

		CBitmap *old1 = memDC.SelectObject(&attacking_piece);
		CBitmap *old2 = backgroundDC.SelectObject(&map_graphics->background);

		int result2 = paintTerrainSquare(&memDC, activeMap, 
										 old_loc.x, old_loc.y,
										 0, 0);
										
		this->paintPiece(&memDC, activeList, attacker, 0, 0);
		
		memDC.SelectObject(&defending_piece);
		int result3 = paintTerrainSquare(&memDC, activeMap, 
										 new_loc.x, new_loc.y,
										 0, 0);
										
		this->paintPiece(&memDC, activeList, defender, 0, 0);
		
		//need to select something else, so Dlg can use the CBitmap...
		memDC.SelectObject(old1);
		backgroundDC.SelectObject(old2);
				
		if (!(resolve_dlg.updateBitmaps(HBITMAP(attacking_piece), HBITMAP(defending_piece))))
			DisplayLastError();

		resolve_dlg.att_desc  =  pDoc->getPlayerAdj(att_owner);
		resolve_dlg.att_desc += " ";
		resolve_dlg.att_desc += activeList->getName(attacker);

		resolve_dlg.att_str  = "Strength: ";
		resolve_dlg.att_str += Int2Char(activeList->List_of_pieces[attacker].getAttStr());
		resolve_dlg.att_str += "\nAttack Bonus: 50%";

		resolve_dlg.def_desc  =  pDoc->getPlayerAdj(def_owner);
		resolve_dlg.def_desc += " ";
		resolve_dlg.def_desc += activeList->getName(defender);

		resolve_dlg.def_str  = "Strength: ";
		resolve_dlg.def_str += Int2Char(activeList->List_of_pieces[defender].getDefStr());

		//end populate Bitmaps & Strings for dialog

		resolve_dlg.DoModal();

		PlaySound("LILGUN.WAV", NULL, SND_FILENAME | SND_ASYNC); 
		
		//calculate the result of the combat
		//temporary code, eventually replace with full combat resolution logic 
		//int combat_result = pDoc->calcCombatResult(attacker, defender);
		//return value will include: 
		//     no_result, 
		//     attacker_lost (0..2), defender_lost
		//     attacker_strengh_reduced, defender_strength_reduced
		//     attacker_captured, defender_captured
		int combat_result = rand() % 10;
		
		if (combat_result < 3) 
		{
			//the attacker is killed only if the defender has some attack strength
			if  (activeList->List_of_pieces[defender].getAttStr() > 0)			
			{
				ColMsgBox msgDlg;
				msgDlg.message_str = "My lord, our ";
				msgDlg.message_str += activeList->getName(attacker);
				msgDlg.message_str += " was defeated!";
				msgDlg.DoModal();

				activeList->killPiece(attacker, pDoc->gameStats.getDateInt());
				activeMap->removePiecefromSquare(old_loc);

				pDoc->displayMoves.addMoveInfo(old_loc, new_loc, attacker,
		                              pDoc->gameStats.getActivePlayer(), DEFEAT);

				PlaySound("SHIPSINK.WAV", NULL, SND_FILENAME | SND_ASYNC); 
			}
			else
			{
				ColMsgBox msgDlg;
				msgDlg.message_str = "The battle was inconclusive.";
				msgDlg.DoModal();
			}
		}
		else if (combat_result >= 5)
		//the defender is defeated
		{
			int enemy_killed = rand() % 2;
			//int enemy_killed = 0;
			ColMsgBox msgDlg;
			if (enemy_killed)
				msgDlg.message_str = "My lord, the enemy were destroyed!";
			else
				msgDlg.message_str = "Excellent news, the enemy were captured!";
			msgDlg.DoModal();
			
			if (enemy_killed)
			{
				activeList->killPiece(defender, pDoc->gameStats.getDateInt());
				activeMap->removePiecefromSquare(new_loc);
				//play water sound only if defender was a ship....
				if (activeList->seaType(activeList->getPieceType(defender)))
					PlaySound("SHIPSINK.WAV", NULL, SND_FILENAME | SND_ASYNC); 

				//assumes attacker is not being carried, and dest square is now empty
				activeMap->updatePiecePresence(old_loc.x, old_loc.y, new_loc.x, new_loc.y);	
				activeList->SetPosition(attacker, new_loc.x, new_loc.y);
				int line_of_sight = activeList->GetLoS(attacker);
				activeMap->synchMapTerrain(&(pDoc->MasterMap), new_loc, line_of_sight);
				activeMap->updateExplored(new_loc, line_of_sight);
				activeMap->updateRecentlySeen(new_loc, line_of_sight);
				//pDoc->addMove(old_loc.x, old_loc.y, new_loc.x, new_loc.y, attacker);
				pDoc->displayMoves.addMoveInfo(old_loc, new_loc, attacker,
		                              pDoc->gameStats.getActivePlayer(), VICTORY);
				pDoc->displayMoves.addMoveInfo(old_loc, new_loc, attacker,
					                           pDoc->gameStats.getActivePlayer(), MOVE);
				//TODO:animation for sinking of defeated piece first
				animateMove(GetDC(), activeMap, attacker, old_loc, new_loc);
			}
			else //enemy was captured
			{
				int prisoner = activeList->capturePiece(att_owner, defender, pDoc->gameStats.getDateInt());
				int line_of_sight = activeList->GetLoS(prisoner);
				activeMap->synchMapTerrain(&(pDoc->MasterMap), new_loc, line_of_sight);
				activeMap->updateExplored(new_loc, line_of_sight);
				activeMap->updateRecentlySeen(new_loc, line_of_sight);
				activeMap->removePiecefromSquare(new_loc);
				activeMap->addPiecetoSquare(prisoner, new_loc);
				//int testvalue = activeMap->getPiece(new_loc.x, new_loc.y);
				//int testvalue2 = activeMap->getPiece(old_loc.x, old_loc.y);

				pDoc->displayMoves.addMoveInfo(old_loc, new_loc, defender,
		                              pDoc->gameStats.getActivePlayer(), CAPTURE);
			}
		}
		else
		{
			ColMsgBox msgDlg;
			msgDlg.message_str = "The battle was inconclusive.";
			msgDlg.DoModal();
			//TODO:add inconclusive battle to MoveList?
		}
		piece_just_moved = true;
		activeList->ZeroMoves(attacker);

		//processToNextPiece();
		//active_piece = activeList->GetNextPiece(current_owner);
		//activeList->UpdateActive(active_piece);
		return 0;
	}
	//should tell caller the result of the combat?
	return -1;
}

bool CColAusView::isMoveKey(UINT nChar)
{
	switch (nChar)
	{
		case VK_NUMPAD1:
		case VK_END:
		case VK_NUMPAD2:
		case VK_DOWN:
		case VK_NUMPAD3:
		case VK_NEXT:
		case VK_NUMPAD4:
		case VK_LEFT:
		case VK_NUMPAD6:
		case VK_RIGHT:
		case VK_NUMPAD7:
		case VK_HOME:
		case VK_NUMPAD8:
		case VK_UP:
		case VK_NUMPAD9:
		case VK_PRIOR:
			return true;
			break;
		default:
			return false;
	}
}

void CColAusView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//CColAusDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
		return;

}

CSize CColAusView::ShowText(CDC* pDC, char* text, int x, int y)
{
	CSize text_size(0, 0);

	if (text == NULL)
	{
		testOutFile << "Text string is NULL, at (" << x << "," << y << endl;
		return text_size;
	}

	//Initialise a LOGFONT structure for the fonts
	LOGFONT logFont;
	logFont.lfHeight = 24;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	strcpy(logFont.lfFaceName, "Ariel");

	//create a new font and select it into the DC
	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont* oldFont = pDC->SelectObject(&font);

	//print text with the new font
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(x, y, text);

	int text_length = 0;
	for (int i = 0; text[i] != '\0'; i++)
		text_length++;
	
	text_size = pDC->GetTextExtent(text, text_length);

	//restore the old font to the DC
	pDC->SelectObject(oldFont);

	return text_size;
}

int CColAusView::paintMap(CDC *pDC, 
						  Map *theMap, 
						  int disp_width, int disp_height)
{
	//updated routine to paint only a specific portion of the map
	int return_value = 0;

	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//set up terrain square size paramaters (initially 48x48 only)
	BITMAP bm1;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);
	//establish gaps between frames (eventually source from mapGraphics)
	int frame_gap = 2;
	int frame_offset = frame_gap + bm1.bmWidth;

	//set up a memory DC for the map area
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, 
		                             bm1.bmWidth * disp_width, 
									 bm1.bmHeight * disp_height);
	CBitmap *oldbm1 = memDC.SelectObject(&memBitmap);

	//memory DC for the map elements
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 

	return_value = paintTerrainSquares(&memDC, theMap, 				                       
						               0, 0,
						               disp_width, disp_height);
	
	//paint statistics on the screen for testing/display purposes
	return_value = paintMapStats(&memDC, theMap, 
						         0, 0,
						         disp_width, disp_height);

	//need to refrain from painting the currently moving piece
	return_value = paintInactivePieces(&memDC, theMap, 
		                               0, 0, 
							           disp_width, disp_height);

	//paint active piece here only if it has not moved
	//if (pDoc->active_move.piece_number == EMPTY) 
	//above no longer required, as animation done already
	//need to refrain from painting the currently moving piece
	if ( (pDoc->MasterList.GetActive() != EMPTY)
	&&   (!animation_pending) )
		return_value = paintActivePiece(&memDC, theMap);
 
	//hide unexplored areas not visible on this map
	if (t_hide_unexplored)
	{
		paintUnexplored(&memDC, theMap, 0, 0, disp_width, disp_height);
	}

	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(0, 0, 
			    bm1.bmWidth * disp_width, 
		        bm1.bmHeight * disp_height, 
				&memDC, 0, 0, SRCCOPY);

	//re-initialise Map level drawing paramaters
	//probably don't need this here now....
	theMap->move_key_pressed = false;

	//select old objects to eliminate memory leakage
	memDC.SelectObject(oldbm1);

	//return value > 0 indicates some errors were encountered
	return return_value;
}

CPoint CColAusView::getMapSquareSize(void)
{
	return map_graphics->getMapSquareSize();
}

int CColAusView::animateMove(CDC *pDC, Map *theMap,
							 int piece_number, 
							 CPoint move_from, CPoint move_to)
{	
	//Animates a move from one location to the next direct to the DC
	//Repaints a background of 7x7 squares, then uses this as a backdrop 
	//to perform the animation over
	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Access function to get size of map squares
	CPoint SQ_SIZE;
	SQ_SIZE = getMapSquareSize();

	int error_value  = -1;
	int null_value   = -2;
	int return_value = 0;

	//TODO:include bounds check/validation for piece_number against PieceList class
	if (piece_number == EMPTY)
		return error_value;

	if (!(pDoc->gameStats.animate_piece_moves)) 
		return null_value;

	//Derive x & y dimensions of move
	int x_offset = move_to.x - move_from.x;
	int y_offset = move_to.y - move_from.y;

	CPoint topleft_screen_pos, from_screen_pos, to_screen_pos;

	//Calculate view pixel locations of the 'from' map square
	from_screen_pos.x = (move_from.x - display_start.x) * SQ_SIZE.x;
	from_screen_pos.y = (move_from.y - display_start.y) * SQ_SIZE.y;
	to_screen_pos.x = (move_to.x - display_start.x) * SQ_SIZE.x;
	to_screen_pos.y = (move_to.y - display_start.y) * SQ_SIZE.y;

	//Calculate view pixel locations of top left corner of drawing area
	int draw_radius = 3;
	int draw_diameter = 7;
	CPoint topleft_sq;
	topleft_sq.x = move_to.x - display_start.x - draw_radius;
	topleft_sq.y = move_to.y - display_start.y - draw_radius;
	if (topleft_sq.x < 0)
		topleft_sq.x = 0;
	if (topleft_sq.y < 0)
		topleft_sq.y = 0;

	topleft_screen_pos.x = (topleft_sq.x) * SQ_SIZE.x;
	topleft_screen_pos.y = (topleft_sq.y) * SQ_SIZE.y;

	//prepare memory DC for storing the backdrop
	CDC backdropDC, dcTerrBackdrop;
	CBitmap bmBackdrop, bmTerrBackdrop;
	CBitmap *old1, *old2, *old3;
	backdropDC.CreateCompatibleDC(pDC); 
	bmBackdrop.CreateCompatibleBitmap(pDC, 
		                              SQ_SIZE.x * DISP_WIDTH, 
									  SQ_SIZE.y * DISP_HEIGHT);
	old1 = backdropDC.SelectObject(&bmBackdrop);
	dcTerrBackdrop.CreateCompatibleDC(pDC); 
	bmTerrBackdrop.CreateCompatibleBitmap(pDC, 
		                              SQ_SIZE.x * DISP_WIDTH, 
									  SQ_SIZE.y * DISP_HEIGHT);
	old2 = dcTerrBackdrop.SelectObject(&bmTerrBackdrop);


	//paint backdrop to the memory DC
	return_value = 
				paintTerrainSquares(&backdropDC, theMap, 				                       
				     			    topleft_sq.x, 
									topleft_sq.y,
					                draw_diameter, draw_diameter);

	//paint statistics on the screen for testing/display purposes
	return_value = paintMapStats(&backdropDC, theMap, 
				     			 topleft_sq.x, 
								 topleft_sq.y,
						         draw_diameter, draw_diameter);

	//hide unexplored areas not visible on this map
	if (t_hide_unexplored)
		paintUnexplored(&backdropDC, theMap, 
				        topleft_sq.x, 
						topleft_sq.y,
					    draw_diameter, draw_diameter);

	//save a copy of the terrain-only backdrop
	dcTerrBackdrop.BitBlt(topleft_screen_pos.x, topleft_screen_pos.y, 
			              SQ_SIZE.x * draw_diameter, SQ_SIZE.y * draw_diameter, 
				          &backdropDC, 
						  topleft_screen_pos.x, topleft_screen_pos.y, 
						  SRCCOPY);
	
	return_value = 
				paintInactivePieces(&backdropDC, theMap, 				                       
				     		        topleft_sq.x, 
							        topleft_sq.y,
									draw_diameter, draw_diameter);

	//repaint the terrain square of the 'from' location, as this should not be visible yet
	/* - not needed, as 'active' piece won't be displayed....
	backdropDC.BitBlt(from_screen_pos.x, from_screen_pos.y,
					  SQ_SIZE.x, SQ_SIZE.y,
					  &dcTerrBackdrop,
					  from_screen_pos.x, from_screen_pos.y,
					  SRCCOPY);
    */

	//create memDC for preparing the final picture to be displayed
	CDC finalDC;
	CBitmap bmFinal;
	finalDC.CreateCompatibleDC(pDC);
	bmFinal.CreateCompatibleBitmap(pDC, SQ_SIZE.x * DISP_WIDTH, SQ_SIZE.y * DISP_HEIGHT);
	old3 = finalDC.SelectObject(&bmFinal);

	for (int i = 0;
		 i <= SQ_SIZE.x;	//assumes SQ_SIZE.x == SQ_SIZE.y
		 i+=2)				
	{
			//TODO:bounds check on starting position against map class/view? 

			//redraw finalDC from backdropDC looping to create a delay only
			for (int delay = 0; delay < animate_delay_time; delay++)
				finalDC.BitBlt(topleft_screen_pos.x, 
							   topleft_screen_pos.y, 
							   draw_diameter * SQ_SIZE.x, 
							   draw_diameter * SQ_SIZE.y, 
							   &backdropDC, 
							   topleft_screen_pos.x, 
							   topleft_screen_pos.y, 
							   SRCCOPY);

			//now paint the piece at the 'in transit' location
			return_value = 
				paintPiece(&finalDC, activeList, piece_number,
				           from_screen_pos.x + (i * x_offset), 
						   from_screen_pos.y + (i * y_offset));

			//repaint from TerrBackdrop any squares that have not been seen recently
			for (int map_square_i = topleft_sq.x; 
				 map_square_i < topleft_sq.x + draw_diameter; 
				 map_square_i++)
			{
				for (int map_square_j = topleft_sq.y;
					 map_square_j < topleft_sq.y + draw_diameter;
					 map_square_j++)
				{
					if (!(activeMap->isRecentlySeen(map_square_i + display_start.x, 
						                            map_square_j + display_start.y))
					&&   (t_hide_unexplored) )
						finalDC.BitBlt(map_square_i * SQ_SIZE.x, map_square_j * SQ_SIZE.y,
									   SQ_SIZE.x, SQ_SIZE.y,
									   &dcTerrBackdrop, 
									   map_square_i * SQ_SIZE.x, map_square_j * SQ_SIZE.y,
							           SRCCOPY);

				}
			}

			//finally paint the updated area to the screen
			pDC->BitBlt(topleft_screen_pos.x, 
						topleft_screen_pos.y, 
			            SQ_SIZE.x * draw_diameter, 
		                SQ_SIZE.y * draw_diameter, 
				        &finalDC, 
						topleft_screen_pos.x, topleft_screen_pos.y, 
						SRCCOPY);		
	}
	backdropDC.SelectObject(old1);
	dcTerrBackdrop.SelectObject(old2);
	finalDC.SelectObject(old3);

	return return_value;
}

int CColAusView::paintActivePiece(CDC *pDC, Map *theMap)
{
	//displays the active piece in it's location on the map
	
	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//should be access function in view or map_graphics.....
	CPoint SQ_SIZE(48, 48);

	int active_piece = pDoc->MasterList.GetActive();
	CPoint active_pos = pDoc->MasterList.GetPosition(active_piece);
	int return_value = 0;

	testOutFile << "New: painting ActivePieces, " 
			    << "active_piece =" << active_piece 
		        << ", active_pos.x=" << active_pos.x
				<< ", active_pos.y=" << active_pos.x
				<< endl;

	if  (active_piece > 0)
	{
		if (display_active)
			return_value = 
					paintPiece(pDC, 
						       &(pDoc->MasterList), 
							   active_piece, 
							   (active_pos.x - display_start.x) * SQ_SIZE.x, 
							   (active_pos.y - display_start.y) * SQ_SIZE.y);
	}
	return return_value;
}	

int CColAusView::paintInactivePieces(CDC *pDC, 
						             Map *theMap, 
						             int disp_origin_x, int disp_origin_y,
						             int disp_width, int disp_height)
{
	//Paints pieces to the screen area specified in map square units
	//	- the active piece is not painted - this must be done separately
	//disp_origin is relative to the current display origin for the map view
	//(so should be zero to start from the top left of the visible map area

	//should be access function in view or map_graphics.....
	CPoint SQ_SIZE(48, 48);

	int piece_counter;
	int return_value = 0;

	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i=disp_origin_x; i < (disp_origin_x + disp_width); i++)
		for (int j=disp_origin_y; j < (disp_origin_y + disp_height); j++)	
		{
			piece_counter = theMap->Land[i + display_start.x]
			                            [j + display_start.y].piece;
			//testOutFile << "paintInactivePieces, " 
			//	        << "piece_counter=" << piece_counter 
			//			<< endl;
	        if  (piece_counter != NO_PIECE)
			{
				int piece_owner = pDoc->MasterList.GetOwner(piece_counter);
				int curr_player = pDoc->gameStats.getActivePlayer();
				int carrier = pDoc->MasterList.getCarrier(piece_counter);
				//testOutFile << "paintInactivePieces" 
				//            << ", piece_counter=" << piece_counter 
				//			<< ", piece_owner=" << piece_owner
				//			<< ", curr_player=" << curr_player
				//			<< ", carrier=" << carrier
				//	    	<< endl;

				if  ( ((curr_player != piece_owner)
			    &&    (theMap->Land[i + display_start.x]
			                       [j + display_start.y].recently_seen) 
		        &&    (theMap->Land[i + display_start.x]
				                   [j + display_start.y].explored))
			    ||    (curr_player == piece_owner) )
				{
					if ((piece_counter != pDoc->MasterList.GetActive()))
						if (carrier == NO_PIECE)
							return_value = 
								paintPiece(pDC, 
									       &(pDoc->MasterList), 
										   piece_counter, 
										   i * SQ_SIZE.x, 
										   j * SQ_SIZE.y);
				}
			}
	    }
	return return_value;
}

int CColAusView::paintMapStats(CDC *pDC, 
						       Map *theMap, 
						       int disp_origin_x, int disp_origin_y,
						       int disp_width, int disp_height)
{
	//Paints a number to the screen area specified in map square units
	//disp_origin is relative to the current display origin for the map view
	//(so should be zero to start from the top left of the visible map area

	if (stat_display == NONE)
		return -1;

	CPoint SQ_SIZE = getMapSquareSize();
	CPoint current_point(0,0);
	int stat_value = 0;
	int return_value = 0;

	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i=disp_origin_x; i < (disp_origin_x + disp_width); i++)
		for (int j=disp_origin_y; j < (disp_origin_y + disp_height); j++)	
		{
			current_point.SetPoint(i + display_start.x,j + display_start.y);
			stat_value = getStatValue(theMap, current_point);

			//now display the value to the current location, using in-built font

			return_value = paintStat(pDC, stat_value, 
								     i * SQ_SIZE.x, 
									 j * SQ_SIZE.y);
	    }

	return return_value;
}

int CColAusView::getStatValue(Map *theMap, CPoint display_point)
{
	//check if there is currently a statistics display type
	if  (stat_display == NONE)
		return -1;

	//depending on the stat_display value, call the appropriate map function
	switch (stat_display)
	{
		case ALTITUDE:
			return theMap->Land[display_point.x][display_point.y].altitude;
		case TERRAIN_TYPE:
			return theMap->Land[display_point.x][display_point.y].land_terrain_type;
		default:
			return -1;
	}
}

int CColAusView::paintTerrainSquares(CDC *pDC, 
						             Map *theMap, 
						             int disp_origin_x, int disp_origin_y,
						             int disp_width, int disp_height,
									 bool relative)
{
	//updated routine to paint only a specific portion of the map
	//paints the terrain only....
	int return_value = 0;

	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//set up terrain square size paramaters (initially 48x48 only)
	// & establish gaps between frames 
	// (eventually source from mapGraphics)
	BITMAP bm1;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);
	int frame_gap = 2;
	int frame_offset = frame_gap + bm1.bmWidth;

	//set up a memory DC for the map area (view area....)
	//done in initMemDCs()
	CRect r;
	GetClientRect(&r);
	
	CDC memoryDC;
	CBitmap memBitmap;
	memoryDC.CreateCompatibleDC(pDC); 
//	memBitmap.CreateCompatibleBitmap(pDC, 
//		                             bm1.bmWidth * disp_width, 
//									 bm1.bmHeight * disp_height);
	memBitmap.CreateCompatibleBitmap(pDC, 
		                             r.right, 
									 r.bottom);

	CBitmap *oldbm1 = memoryDC.SelectObject(&memBitmap);
	
	//copyDC(pDC, &memoryDC);
	//memory DC for the map elements
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 
	//CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
	CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->mBitmap1);

	int i, j, x_value, y_value;

	//top left hand point in map to begin display	
	
	int display_loc_x = display_start.x;
	int display_loc_y = display_start.y;

	if (!relative)
		display_loc_x = display_loc_y = 0;

	int terr_type_loc = 0;

	//loop through each square in the display area
	for (i=disp_origin_x; i < (disp_origin_x + disp_width); i++)
	{
		x_value = i * bm1.bmWidth;
		for (j=disp_origin_y; j < (disp_origin_y + disp_height); j++)										
		{
			y_value = j * bm1.bmHeight;
			switch(theMap->Land[i + display_loc_x]
			                   [j + display_loc_y].terrain)
			{
				case LAND:

					paintBackSquare(&memoryDC, theMap, i, j);
					
					terr_type_loc = (theMap->Land[i + display_loc_x][j + display_loc_y].land_terrain_type) * frame_offset;

					dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
					
					//maskblt the location square's terrain type over the top
					if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
									x_value, y_value, 
									bm1.bmWidth, bm1.bmHeight, 
									dcMem1.GetSafeHdc(), 0, terr_type_loc, 
			    					map_graphics->Terr_blend_mask, 
									frame_offset * 4, 0, 
					    			MAKEROP4(DEST_vh, SRCCOPY))))
					{
						return_value = 1;
					}
					//select another object, so that other functions can use
					//	the terrain type bitmap.
					dcMem1.SelectObject(&map_graphics->mountains);
					//if the altitude is above the snowline, then blt a mountain			
					//replace with access function  "int theMap->isMountain()"
					if (theMap->Land[i + display_loc_x][j + display_loc_y].altitude >= SNOWLINE)
					{
						dcMem1.SelectObject(&map_graphics->mountains);
						int mount_score = theMap->u_mountScore(i + display_loc_x, j + display_loc_y);
						if (!(MyMaskBlt(    memoryDC.GetSafeHdc(),
							                x_value, y_value, 
					                        bm1.bmWidth, bm1.bmHeight, 
											dcMem1.GetSafeHdc(), 
								    		0, theMap->mount_table[mount_score], 
							    			map_graphics->mountains_mask, 
								    	    0, theMap->mount_table[mount_score], 
									    	MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
						}
					}

					//if the square is forested, then blt a forest
					if (theMap->Land[i + display_loc_x][j + display_loc_y].forest)
					{
						dcMem1.SelectObject(&map_graphics->forests);
						int forest_score = theMap->u_forestScore(i + display_loc_x, j + display_loc_y);
						if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
										x_value, y_value, 
										bm1.bmWidth, bm1.bmHeight, 
										dcMem1.GetSafeHdc(), 
										0, theMap->forest_table[forest_score], 
			    						map_graphics->forests_mask, 
										0, theMap->forest_table[forest_score], 
					    				MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
						}
					}

					//if the altitude is sufficient, then blt a hill (if not forested)
					//replace with "int theMap->isHill()"
					if ( (theMap->Land[i + display_loc_x][j + display_loc_y].altitude >= HILLLINE)
					&&   (theMap->Land[i + display_loc_x][j + display_loc_y].altitude < SNOWLINE))
					{
						dcMem1.SelectObject(&map_graphics->hills);
						int hill_score = theMap->u_hillScore(i + display_loc_x, j + display_loc_y);
						if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
										x_value, y_value, 
										bm1.bmWidth, bm1.bmHeight, 
										dcMem1.GetSafeHdc(), 
										0, theMap->mount_table[hill_score], 
			    						map_graphics->hills_mask, 
										0, theMap->mount_table[hill_score], 
					    				MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
						}
					}
					
					//if the square has a river, then blt a river
					//if (theMap->Land[i + display_loc_x][j + display_loc_y].river == MNRIVER)
					if (theMap->hasRiver(i + display_loc_x, j + display_loc_y))
					{
						dcMem1.SelectObject(&map_graphics->mnrivers);
						int river_score = theMap->u_riversScore(i + display_loc_x, j + display_loc_y);
						if (river_score > 0)
							if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
											x_value, y_value, 
											bm1.bmWidth, bm1.bmHeight, 
											dcMem1.GetSafeHdc(), 0, theMap->rivers_table[river_score], 
			    							map_graphics->mnrivers_mask, 0, theMap->rivers_table[river_score], 
					    					MAKEROP4(SRCCOPY, DEST_vh))))
							{
								return_value = 1;
							}
						
					}
					break;

				case SEA:
								
					int coastal_score;
					coastal_score = theMap->coastalScore(i + display_loc_x, j + display_loc_y);
					dcMem1.SelectObject(&map_graphics->mBmSea);
					
					//no adjacent land squares, so just paint a plain sea square
					if (coastal_score == 0)
					{
						dcMem1.SelectObject(&map_graphics->mBmSea);
						memoryDC.BitBlt(x_value, y_value, bm1.bmWidth, bm1.bmHeight, &dcMem1, 0, 0, SRCCOPY);
					}
					//one or more adjacent land squares, so paint appropriate coastline
					else
					{						
						paintBackSquare(&memoryDC, theMap, i, j);

						//now select the x, y offsets, based on the lookup table indexed by 'coastal score'
						int coast_offset_x = theMap->coast_table[coastal_score];
						int coast_offset_y = 
							((theMap->Land[i + display_loc_x ]
						                  [j + display_loc_y].random_number) % 4) * frame_offset;
						//modify variation offset, as some coast types have less than 4
						if ((coastal_score == 11)
						||  (coastal_score == 13)
						||  (coastal_score == 14)
						||  (coastal_score == 7))
							coast_offset_y = 
							((theMap->Land[i + display_loc_x ]
							              [j + display_loc_y].random_number) % 2) * frame_offset;
						if ((coastal_score == 10)
						||  (coastal_score == 15))
							coast_offset_y = 
							((theMap->Land[i + display_loc_x ]
							              [j + display_loc_y].random_number) % 2) * frame_offset;
						//now maskblt
						dcMem1.SelectObject(&map_graphics->coast);
						if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
										x_value, y_value, 
										bm1.bmWidth, bm1.bmHeight, 
										dcMem1.GetSafeHdc(), 
										coast_offset_x, coast_offset_y, 
			    						map_graphics->coast_mask, 
										coast_offset_x, coast_offset_y, 
					    				MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
						}

						//if the square has a river, then blt a river
						//replace with "int theMap->isRiver(x, y)"
						//if (theMap->Land[i + display_loc_x][j + display_loc_y].river == MNRIVER)
						if (theMap->hasRiver(i + display_loc_x, j + display_loc_y))
						{
							dcMem1.SelectObject(&map_graphics->mnrivers_ocean);
							int river_score = theMap->u_riversScore(i + display_loc_x, j + display_loc_y);
							if (river_score > 0)
								if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
												x_value, y_value, 
												bm1.bmWidth, bm1.bmHeight, 
												dcMem1.GetSafeHdc(), 
												0, theMap->rivers_table[river_score], 
												map_graphics->mnrivers_mask, 
												0, theMap->rivers_table[river_score], 
					    						MAKEROP4(SRCCOPY, DEST_vh))))
								{
									return_value = 1;
								}
						}
					}
				break;
			}
		}
	}
	  
	//The memory DC is ready now, so BitBlt to the display DC
	pDC->BitBlt(disp_origin_x * bm1.bmWidth, 
				disp_origin_y * bm1.bmHeight, 
				bm1.bmWidth * disp_width, 
		        bm1.bmHeight * disp_height, 
				&memoryDC, 
				disp_origin_x * bm1.bmWidth, 
				disp_origin_y * bm1.bmHeight, SRCCOPY);

	//select old objects to eliminate memory leakage
	memoryDC.SelectObject(oldbm1);
	dcMem1.SelectObject(oldbm2);

	//return value > 0 indicates some errors were encountered
	return return_value;
}

int CColAusView::paintTerrainSquare(CDC *pDC, 
						            Map *theMap, 
						            int map_x, int map_y,
						            int cdc_x, int cdc_y)
{
	//This routine paints the terrain of a square only
	//updated routine to paint only a single square of the map:
	//	* map_x, map_y indicates the map square to paint
	//  * cdc_x, cdc_y indicates the location in the CDC to paint to

	int return_value = 0;

	CColAusDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDC == NULL)
		return 0;

	int frame_offset = map_graphics->getMapFrameSize();
	CPoint map_square = map_graphics->getMapSquareSize();
	
	CDC memoryDC;
	CBitmap memBitmap;
	memoryDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC, map_square.x, map_square.y);
	CBitmap *oldbm1 = memoryDC.SelectObject(&memBitmap);
	
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(pDC); 
	//CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
	CBitmap *oldbm2 = dcMem1.SelectObject(&map_graphics->mBitmap1);

	int i, j, x_value, y_value;

	int terr_type_loc = 0;

	i = map_x;
	j = map_y;
	x_value = i * map_square.x;
	y_value = j * map_square.y;
	x_value = y_value = 0;
	
	switch(theMap->Land[i][j].terrain)
	{
		case LAND:

			//need to update, and disentangle from 'display_location'-already done?
			paintBackSquare(&memoryDC, theMap, i, j, 0, 0);
					
			terr_type_loc = (theMap->Land[i][j].land_terrain_type) * frame_offset;

			dcMem1.SelectObject(&map_graphics->mBmLand_terrain_types);
					
			//maskblt the location square's terrain type over the top
			if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
							x_value, y_value, 
							map_square.x, map_square.y, 
							dcMem1.GetSafeHdc(), 0, terr_type_loc, 
							map_graphics->Terr_blend_mask, 
							frame_offset * 4, 0, 
				   			MAKEROP4(DEST_vh, SRCCOPY))))
			{
				return_value = 1;
			}
			//select another object, so that other functions can use
			//	the terrain type bitmap.
			dcMem1.SelectObject(&map_graphics->mountains);
			//if the altitude is above the snowline, then blt a mountain			
			//replace with access function  "int theMap->isMountain()"
			if (theMap->Land[i][j].altitude >= SNOWLINE)
			{
				dcMem1.SelectObject(&map_graphics->mountains);
				int mount_score = theMap->u_mountScore(i, j);
				if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
							    x_value, y_value, 
					            map_square.x, map_square.y, 
								dcMem1.GetSafeHdc(), 
								0, theMap->mount_table[mount_score], 
								map_graphics->mountains_mask, 
							    0, theMap->mount_table[mount_score], 
							   	MAKEROP4(SRCCOPY, DEST_vh))))
				{
					return_value = 1;
				}
			}

			//if the square is forested, then blt a forest
			if (theMap->Land[i][j].forest)
			{
				dcMem1.SelectObject(&map_graphics->forests);
				int forest_score = theMap->u_forestScore(i, j);
				if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
								x_value, y_value, 
								map_square.x, map_square.y,
								dcMem1.GetSafeHdc(), 
								0, theMap->forest_table[forest_score], 
								map_graphics->forests_mask, 
								0, theMap->forest_table[forest_score], 
				   				MAKEROP4(SRCCOPY, DEST_vh))))
				{
					return_value = 1;
				}
			}

			//if the altitude is sufficient, then blt a hill (if not forested)
			//replace with "int theMap->isHill()"
			if ( (theMap->Land[i][j].altitude >= HILLLINE)
			&&   (theMap->Land[i][j].altitude < SNOWLINE))
			{
				dcMem1.SelectObject(&map_graphics->hills);
				int hill_score = theMap->u_hillScore(i, j);
				if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
								x_value, y_value, 
								map_square.x, map_square.y,
								dcMem1.GetSafeHdc(), 
								0, theMap->mount_table[hill_score], 
	    						map_graphics->hills_mask, 
								0, theMap->mount_table[hill_score], 
			    				MAKEROP4(SRCCOPY, DEST_vh))))
				{
					return_value = 1;
				}
			}
					
			//if the square has a river, then blt a river
			if (theMap->hasRiver(i, j))
			{
				dcMem1.SelectObject(&map_graphics->mnrivers);
				int river_score = theMap->u_riversScore(i, j);
				if (river_score > 0)
					if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
									x_value, y_value, 
									map_square.x, map_square.y,
									dcMem1.GetSafeHdc(), 0, theMap->rivers_table[river_score], 
			    					map_graphics->mnrivers_mask, 0, theMap->rivers_table[river_score], 
					    			MAKEROP4(SRCCOPY, DEST_vh))))
					{
						return_value = 1;
					}
						
			}
			break;

		case SEA:
						
			int coastal_score;
			coastal_score = theMap->coastalScore(i, j);
			dcMem1.SelectObject(&map_graphics->mBmSea);
					
			//no adjacent land squares, so just paint a plain sea square
			if (coastal_score == 0)
			{
				dcMem1.SelectObject(&map_graphics->mBmSea);
				memoryDC.BitBlt(x_value, y_value, 
					            map_square.x, map_square.y, 
								&dcMem1, 0, 0, SRCCOPY);
			}
			//one or more adjacent land squares, so paint appropriate coastline
			else
			{	
				paintBackSquare(&memoryDC, theMap, i, j, 0, 0);
				//now select the x, y offsets, based on the lookup table indexed by 'coastal score'
				int coast_offset_x = theMap->coast_table[coastal_score];
				int coast_offset_y = 
							((theMap->Land[i][j].random_number) % 4) * frame_offset;
				//modify variation offset, as some coast types have less than 4
				if ((coastal_score == 11)
				||  (coastal_score == 13)
				||  (coastal_score == 14)
				||  (coastal_score == 7))
					coast_offset_y = 
					((theMap->Land[i][j].random_number) % 2) * frame_offset;
				if ((coastal_score == 10)
				||  (coastal_score == 15))
					coast_offset_y = 
					((theMap->Land[i][j].random_number) % 2) * frame_offset;
				//now maskblt
				dcMem1.SelectObject(&map_graphics->coast);
				if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
								x_value, y_value, 
								map_square.x, map_square.y,
								dcMem1.GetSafeHdc(), 
								coast_offset_x, coast_offset_y, 
	    						map_graphics->coast_mask, 
								coast_offset_x, coast_offset_y, 
			    				MAKEROP4(SRCCOPY, DEST_vh))))
				{
					return_value = 1;
				}

				//if the square has a river, then blt a river
				if (theMap->hasRiver(i, j))
				{
					dcMem1.SelectObject(&map_graphics->mnrivers_ocean);
					int river_score = theMap->u_riversScore(i, j);
					if (river_score > 0)
						if (!(MyMaskBlt(memoryDC.GetSafeHdc(),
										x_value, y_value, 
										map_square.x, map_square.y, 
										dcMem1.GetSafeHdc(), 
										0, theMap->rivers_table[river_score], 
										map_graphics->mnrivers_mask, 
										0, theMap->rivers_table[river_score], 
			    						MAKEROP4(SRCCOPY, DEST_vh))))
						{
							return_value = 1;
						}
				}
			}
			break;
	}
	  
	//BitBlt to the DC passed to the function
	pDC->BitBlt(cdc_x, 
				cdc_y, 
				map_square.x, 
		        map_square.y, 
				&memoryDC, 
				x_value, 
				y_value, 
				SRCCOPY);

	//select old objects to eliminate memory leakage
	memoryDC.SelectObject(oldbm1);
	dcMem1.SelectObject(oldbm2);

	//return value > 0 indicates some errors were encountered
	return return_value;
}

int CColAusView::paintBackSquare(CDC *memDCPtr, Map *theMap, int i, int j)
{
	//paints the backing square for blending adjacent terrain types
	int return_value = 0;

	CDC dcMem;
	dcMem.CreateCompatibleDC(memDCPtr); 

	//set up terrain square size paramaters (48x48)
	BITMAP bm1;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);

	CBitmap *oldbm1 = dcMem.SelectObject(&map_graphics->mBmLand_terrain_types);

	int display_loc_x, display_loc_y, x_value, y_value, loc_terr;

	display_loc_x = display_start.x;
	display_loc_y = display_start.y;
	x_value = (i)*48;
	y_value = (j)*48;

	//if this is a land square, then store the terrain type
	if (theMap->Land[i + display_loc_x][j + display_loc_y].terrain == LAND)
		loc_terr = (theMap->Land[i + display_loc_x][j + display_loc_y].land_terrain_type) * 50;
	else
		loc_terr = -1;

	//use a four way mask to select terrain for each 'pie'
	int terr_type_above, terr_type_below, terr_type_left, terr_type_right;
	terr_type_above = terr_type_below = terr_type_left = terr_type_right = -50;
	int terr_type_first = -1;

	//select each adjacent terrain types
	if  ((i + display_loc_x ) > 0)
		terr_type_left = (theMap->Land[i + display_loc_x - 1][j + display_loc_y].land_terrain_type) * 50;
	else
		terr_type_left = loc_terr;
	if  ((j + display_loc_y) > 0)
		terr_type_above = (theMap->Land[i + display_loc_x][j + display_loc_y - 1].land_terrain_type) * 50;
	else
		terr_type_above = loc_terr;
	if  ((i + display_loc_x ) < (theMap->MAP_WIDTH  - 1))
		terr_type_right = (theMap->Land[i + display_loc_x + 1][j + display_loc_y].land_terrain_type) * 50;
	else
		terr_type_right = loc_terr;
	if  ((j + display_loc_y ) < (theMap->MAP_HEIGHT - 1))
		terr_type_below = (theMap->Land[i + display_loc_x][j + display_loc_y + 1].land_terrain_type) * 50;
	else
		terr_type_below = loc_terr;

	//find a default terrain type for sea_squares 
	if (terr_type_left >= 0)
		terr_type_first = terr_type_left;
	if (terr_type_above >= 0)
		terr_type_first = terr_type_above;
	if (terr_type_right >= 0)
		terr_type_first = terr_type_right;
	if (terr_type_below >= 0)
		terr_type_first = terr_type_below;

	if (loc_terr >= 0)
		terr_type_first = loc_terr;
	else
		loc_terr = terr_type_first;
	
	//if edge did not get a terrain, then set to the first (default) type
	if (terr_type_left < 0)
		terr_type_left = terr_type_first;
	if (terr_type_above < 0)
		terr_type_above = terr_type_first;
	if (terr_type_right < 0)
		terr_type_right = terr_type_first;
	if (terr_type_below < 0)
		terr_type_below = terr_type_first;
			
	//first Bitblt a default terrain type we know is adjacent to this square
	//or in the case of a land square, bitblit its land terrain type

	if (!(memDCPtr->BitBlt(x_value, y_value, bm1.bmWidth, bm1.bmHeight, &dcMem, 0, loc_terr, SRCCOPY)))
		return_value = 5;
	
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					bm1.bmWidth, bm1.bmHeight, 
					dcMem.GetSafeHdc(), 0, terr_type_above, 
					map_graphics->Terr_blend_mask, 0, 0, 
	   				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 1;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					bm1.bmWidth, bm1.bmHeight, 
					dcMem.GetSafeHdc(), 0, terr_type_below, 
					map_graphics->Terr_blend_mask, 100, 0, 
					MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 2;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					bm1.bmWidth, bm1.bmHeight, 
					dcMem.GetSafeHdc(), 0, terr_type_left, 
					map_graphics->Terr_blend_mask, 150, 0, 
	  				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 3;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					bm1.bmWidth, bm1.bmHeight, 
					dcMem.GetSafeHdc(), 0, terr_type_right, 
					map_graphics->Terr_blend_mask, 50, 0, 
    				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 4;
	}

	//select old objects to eliminate memory leakage
	dcMem.SelectObject(oldbm1);

	if (return_value >= 1)
	{
		CHAR szBuf[80]; 
		DWORD dw = GetLastError(); 
 		sprintf(szBuf, "%s failed: GetLastError returned %u\n", 
        NULL, dw); 
 	    //MessageBox(NULL, szBuf, "Error", MB_OK); 
		return return_value;
	}
	return return_value;
}

int CColAusView::paintBackSquare(CDC *memDCPtr, Map *theMap, 
								 int map_x, int map_y,
								 int cdc_x, int cdc_y)
{
	//paints the backing square for blending adjacent terrain types
	//the map_x,y is the location the map, the cdcx,y is the top left corner of the square
	//on the DC
	int return_value = 0;

	CDC dcMem;
	dcMem.CreateCompatibleDC(memDCPtr); 
	CBitmap *oldbm1 = dcMem.SelectObject(&map_graphics->mBmLand_terrain_types);

	CPoint map_square = map_graphics->getMapSquareSize();
	int frame_size = map_graphics->getMapFrameSize();

	
	int x_value, y_value, loc_terr;

	x_value = cdc_x;
	y_value = cdc_y;
	int i = map_x;
	int j = map_y;

	//if this is a land square, then store the terrain type
	if (theMap->Land[i][j].terrain == LAND)
		loc_terr = (theMap->Land[i][j].land_terrain_type) * frame_size;
	else
		loc_terr = -1;

	//use a four way mask to select terrain for each 'pie'
	int terr_type_above, terr_type_below, terr_type_left, terr_type_right;
	terr_type_above = terr_type_below = terr_type_left = terr_type_right = frame_size * -1;
	int terr_type_first = -1;

	//select each adjacent terrain types
	if  (i > 0)
		terr_type_left = (theMap->Land[i - 1][j].land_terrain_type) * frame_size;
	else
		terr_type_left = loc_terr;
	if  (j > 0)
		terr_type_above = (theMap->Land[i][j - 1].land_terrain_type) * frame_size;
	else
		terr_type_above = loc_terr;
	if  (i < (theMap->MAP_WIDTH  - 1))
		terr_type_right = (theMap->Land[i + 1][j].land_terrain_type) * frame_size;
	else
		terr_type_right = loc_terr;
	if  (j < (theMap->MAP_HEIGHT - 1))
		terr_type_below = (theMap->Land[i][j + 1].land_terrain_type) * frame_size;
	else
		terr_type_below = loc_terr;

	//find a default terrain type for sea_squares 
	if (terr_type_left >= 0)
		terr_type_first = terr_type_left;
	if (terr_type_above >= 0)
		terr_type_first = terr_type_above;
	if (terr_type_right >= 0)
		terr_type_first = terr_type_right;
	if (terr_type_below >= 0)
		terr_type_first = terr_type_below;

	if (loc_terr >= 0)
		terr_type_first = loc_terr;
	else
		loc_terr = terr_type_first;
	
	//if edge did not get a terrain, then set to the first (default) type
	if (terr_type_left < 0)
		terr_type_left = terr_type_first;
	if (terr_type_above < 0)
		terr_type_above = terr_type_first;
	if (terr_type_right < 0)
		terr_type_right = terr_type_first;
	if (terr_type_below < 0)
		terr_type_below = terr_type_first;
			
	//first Bitblt a default terrain type we know is adjacent to this square
	//or in the case of a land square, bitblit its land terrain type

	if (!(memDCPtr->BitBlt(x_value, y_value, map_square.x, map_square.y, 
		                   &dcMem, 0, loc_terr, SRCCOPY)))
		return_value = 5;
	
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					map_square.x, map_square.y, 
					dcMem.GetSafeHdc(), 0, terr_type_above, 
					map_graphics->Terr_blend_mask, 0, 0, 
	   				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 1;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					map_square.x, map_square.y, 
					dcMem.GetSafeHdc(), 0, terr_type_below, 
					map_graphics->Terr_blend_mask, frame_size * 2, 0, 
					MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 2;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					map_square.x, map_square.y, 
					dcMem.GetSafeHdc(), 0, terr_type_left, 
					map_graphics->Terr_blend_mask, frame_size * 3, 0, 
	  				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 3;
	}
	if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
					x_value, y_value, 
					map_square.x, map_square.y,
					dcMem.GetSafeHdc(), 0, terr_type_right, 
					map_graphics->Terr_blend_mask, frame_size, 0, 
    				MAKEROP4(SRCCOPY, DEST_vh))))
	{
		return_value = 4;
	}

	//select old objects to eliminate memory leakage
	dcMem.SelectObject(oldbm1);

	if (return_value >= 1)
	{
		CHAR szBuf[80]; 
		DWORD dw = GetLastError(); 
 		sprintf(szBuf, "%s failed: GetLastError returned %u\n", 
        NULL, dw); 
 	    //MessageBox(NULL, szBuf, "Error", MB_OK); 
		return return_value;
	}
	return return_value;
}

int CColAusView::paintUnexplored(CDC *memDCPtr, 
								  Map *theMap,
								  int disp_origin_x, int disp_origin_y,
								  int disp_width, int disp_height)
{
	int return_value = 0;

	//set up local memory DC for map elements
	CDC dcMem1;
	dcMem1.CreateCompatibleDC(memDCPtr); 
	CBitmap *oldbm1 = dcMem1.SelectObject(&map_graphics->undiscovered);

	//set up terrain square size paramaters (48x48)
	BITMAP bm1;
	(map_graphics->mBitmap1).GetObject(sizeof(BITMAP), &bm1);

	int i, j, 
		display_loc_x, display_loc_y, 
		x_value, y_value;

    BOOL above_explored, below_explored, 
		 left_explored, right_explored;

	//map class ensures that display_x, display_y can never be out of bounds...
	display_loc_x = display_start.x;
	display_loc_y = display_start.y;

	//loop through each square in the display area
	for (i=disp_origin_x; i < (disp_origin_x + disp_width); i++)
	{
		x_value = i * bm1.bmWidth;
		for (j=disp_origin_y; j < (disp_origin_y + disp_height); j++)										
		{
			y_value = j * bm1.bmHeight;

 			above_explored = below_explored = left_explored = right_explored = FALSE;

			if (!(theMap->Land[i + display_loc_x]
			                  [j + display_loc_y].explored))
			{
				oldbm1 = dcMem1.SelectObject(&map_graphics->undiscovered);

				//draw the central area unmasked
				memDCPtr->BitBlt(x_value + 4, y_value + 4, bm1.bmWidth - 8, bm1.bmHeight - 8, &dcMem1, 4, 4, SRCCOPY);

				//for each adjacent square check to see it if is explored
				//if so draw with the mask, which will hint at the type of terrainn underneath
				//and record the status for future reference

				if ((j + display_loc_y > 0)
				&&	(theMap->Land[i + display_loc_x][j + display_loc_y - 1].explored) )
				{
					if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
									x_value, y_value, 
									bm1.bmWidth, bm1.bmHeight - 44, 
									dcMem1.GetSafeHdc(), 0, 0, 
			    					map_graphics->undisc_mask, 0, 0, 
					    			MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					above_explored = TRUE;
				}
				
				if ((j + display_loc_y < theMap->MAP_HEIGHT - 1)
				&&	(theMap->Land[i + display_loc_x][j + display_loc_y + 1].explored) )
				{
					if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
									x_value, y_value + 44, 
									bm1.bmWidth, bm1.bmHeight - 44, 
									dcMem1.GetSafeHdc(), 0, 0, 
			    					map_graphics->undisc_mask, 100, 44, 
					    			MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					below_explored = TRUE;
				}
		
				if ((i + display_loc_x > 0)
				&&	(theMap->Land[i + display_loc_x - 1]
				                 [j + display_loc_y].explored) )
				{
					if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
									x_value, y_value, 
									bm1.bmWidth - 44, bm1.bmHeight, 
									dcMem1.GetSafeHdc(), 0, 0, 
			    					map_graphics->undisc_mask, 150, 0, 
					    			MAKEROP4(DEST_vh, SRCCOPY))))
						return_value = 1;
					left_explored = TRUE;
				}			
				
				if ((i + display_loc_x < theMap->MAP_WIDTH - 1)
				&&	(theMap->Land[i + display_loc_x + 1]
				                 [j + display_loc_y].explored) )
				{
					if (!(MyMaskBlt(memDCPtr->GetSafeHdc(),
									x_value + 44, y_value, 
									bm1.bmWidth - 44, bm1.bmHeight, 
									dcMem1.GetSafeHdc(), 0, 0, 
			    					map_graphics->undisc_mask, 50 + 44, 0, 
					    			MAKEROP4(DEST_vh, SRCCOPY))))
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
	                memDCPtr->BitBlt(x_value + left_offset, y_value, 
						         bm1.bmWidth - (left_offset + right_offset), 
								 bm1.bmHeight - 44, &dcMem1, 0, 0, SRCCOPY);
				}
				
				left_offset = right_offset = 0;
				if (!(below_explored))
				{
					if (left_explored)
						left_offset = 4;
					if (right_explored)
						right_offset = 4;
	                memDCPtr->BitBlt(x_value + left_offset, y_value + 44, 
						         bm1.bmWidth - (left_offset + right_offset), 
								 bm1.bmHeight - 44, &dcMem1, 0, 0, SRCCOPY);
				}

				int above_offset, below_offset;
				above_offset = below_offset = 0;
				if (!(left_explored))
				{
					if (above_explored)
						above_offset = 4;
					if (below_explored)
						below_offset = 4;
	                memDCPtr->BitBlt(x_value, y_value + above_offset, 
						         bm1.bmWidth - 44, 
								 bm1.bmHeight - (above_offset + below_offset), 
								 &dcMem1, 0, 0, SRCCOPY);
				}

				above_offset = below_offset = 0;
				if (!(right_explored))
				{
					if (above_explored)
						above_offset = 4;
					if (below_explored)
						below_offset = 4;
	                memDCPtr->BitBlt(x_value + 44, y_value + above_offset, 
						         bm1.bmWidth - 44, 
								 bm1.bmHeight - (above_offset + below_offset), 
								 &dcMem1, 0, 0, SRCCOPY);
				}
			}
			else  if (!theMap->isRecentlySeen(i + display_loc_x, j + display_loc_y))
			{
				dcMem1.SelectObject(&map_graphics->undiscovered);

				//draw the central area unmasked
				//memDCPtr->BitBlt(x_value + 4, y_value + 4, bm1.bmWidth - 8, bm1.bmHeight - 8, &dcMem1, 4, 4, SRCCOPY);

				//for each adjacent square check to see it if is explored
				//if so draw with the mask, which will hint at the type of terrainn underneath
				//and record the status for future reference

				MyMaskBlt(	memDCPtr->GetSafeHdc(),
				        	x_value, y_value, 
							bm1.bmWidth, bm1.bmHeight, 
							dcMem1.GetSafeHdc(), 0, 0, 
			    			map_graphics->undisc_mask, 0, bm1.bmWidth + 2, 
					    	MAKEROP4(DEST_vh, SRCCOPY));
				}
		}
	}
	
	//select old object to eliminate memory leakage
	dcMem1.SelectObject(oldbm1);

	return return_value;
}

void CColAusView::OnTestHideUnexplored()
{
	// TODO: Add your command handler code here
	if (t_hide_unexplored)
		t_hide_unexplored = FALSE;
	else
		t_hide_unexplored = TRUE;

	Invalidate(FALSE);
}

void CColAusView::OnUpdateTestHideUnexplored(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//CColAusDoc *pDoc = GetDocument();

	if (t_hide_unexplored)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

/*
int CColAusView::reCenter_active(Map *theMap, PieceList *thePieces)
{
	int active_piece, this_piece_x, this_piece_y;
	active_piece = this_piece_x = this_piece_y = 0;
	int recentered_flag = 0;
	CPoint focus;

	//current focal point
	focus.x = display_start.x + (DISP_WIDTH / 2);
	focus.y = display_start.y + (DISP_HEIGHT / 2);

	active_piece = thePieces->GetActive();
	thePieces->GetPosition(active_piece, this_piece_x, this_piece_y);

	//recenter if active location is outside the display bounds 
	if ((this_piece_x >= display_start.x + DISP_WIDTH - 1)
	||  (this_piece_x < display_start.x + 1))
	{
		focus.x = this_piece_x;
		recentered_flag += 1;
	}
		
	if ((this_piece_y >= display_start.y + DISP_HEIGHT - 1)
	||  (this_piece_y < display_start.y + 1))
	{
		focus.y = this_piece_y;
		recentered_flag += 2;
	}
	reCenter_point(theMap, focus);
	return recentered_flag;
}
*/

int CColAusView::reCenter_active(Map *theMap, PieceList *thePieces, 
								 bool recenter_both)
{
	//recenters both dimensions, unless 'both' is false, in which case only
	//the out of bounds dimension is centered
	int active_piece, this_piece_x, this_piece_y;
	active_piece = this_piece_x = this_piece_y = 0;
	int recentered_flag = 0;
	CPoint focus;

	//current focal point
	focus.x = display_start.x + (DISP_WIDTH / 2);
	focus.y = display_start.y + (DISP_HEIGHT / 2);

	active_piece = thePieces->GetActive();
	thePieces->GetPosition(active_piece, this_piece_x, this_piece_y);

	//recenter if active location is outside the display bounds 
	if ((this_piece_x >= display_start.x + DISP_WIDTH - 1)
	||  (this_piece_x < display_start.x + 1)
	||  (recenter_both))
	{
		focus.x = this_piece_x;
		recentered_flag += 1;
	}
		
	if ((this_piece_y >= display_start.y + DISP_HEIGHT - 1)
	||  (this_piece_y < display_start.y + 1)
	||  (recenter_both))
	{
		focus.y = this_piece_y;
		recentered_flag += 2;
	}
	reCenter_point(theMap, focus);
	return recentered_flag;
}

int CColAusView::reCenter_location(CPoint new_loc, 
								   bool recenter_both)
{
	//recenters both dimensions, unless 'both' is false, in which case only
	//the out of bounds dimension is centered
	//uses the activeMap only
	int this_piece_x, this_piece_y;
	this_piece_x = this_piece_y = 0;
	int recentered_flag = 0;

	CPoint focus;

	//current focal point
	focus.x = display_start.x + (DISP_WIDTH / 2);
	focus.y = display_start.y + (DISP_HEIGHT / 2);

	this_piece_x = new_loc.x;
	this_piece_y = new_loc.y;

	//recenter if active location is outside the display bounds 
	if ((this_piece_x >= display_start.x + DISP_WIDTH - 1)
	||  (this_piece_x < display_start.x + 1)
	||  (recenter_both))
	{
		focus.x = this_piece_x;
		recentered_flag += 1;
	}
		
	if ((this_piece_y >= display_start.y + DISP_HEIGHT - 1)
	||  (this_piece_y < display_start.y + 1)
	||  (recenter_both))
	{
		focus.y = this_piece_y;
		recentered_flag += 2;
	}
	reCenter_point(activeMap, focus);
	return recentered_flag;
}

void CColAusView::reCenter_point(Map *theMap, CPoint focus)
{
	//recenter the view on point provided
	display_start.x = focus.x - (DISP_WIDTH / 2);
	display_start.y = focus.y - (DISP_HEIGHT / 2);

	if (display_start.x < 0)
		display_start.x = 0;

	if ((display_start.x + DISP_WIDTH) >= theMap->MAP_WIDTH)
		display_start.x = theMap->MAP_WIDTH - DISP_WIDTH;

	if (display_start.y < 0)
		display_start.y = 0;

	if (display_start.y + DISP_HEIGHT >= theMap->MAP_HEIGHT)
		display_start.y = theMap->MAP_HEIGHT - DISP_HEIGHT;
}

void CColAusView::OnTestNewmap()
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRandMapDialog randmapDlg;

	randmapDlg.width = pDoc->MasterMap.MAP_WIDTH - 100;
	randmapDlg.height = pDoc->MasterMap.MAP_HEIGHT - 100;
	randmapDlg.temperature = pDoc->MasterMap.mtemp;
	randmapDlg.rainfall = pDoc->MasterMap.mrainfall;
	randmapDlg.cont_size = pDoc->MasterMap.mcont_size;
	randmapDlg.altitude = pDoc->MasterMap.maltitude;
	randmapDlg.land_points = pDoc->MasterMap.mland_points;// / ((pDoc->PlayerMap.MAP_WIDTH + pDoc->PlayerMap.MAP_HEIGHT) * 100);
	//land_points * (MAP_WIDTH + MAP_HEIGHT) / 100

	randmapDlg.width_display = randmapDlg.width + 100;
	randmapDlg.height_display = randmapDlg.height + 100;
	randmapDlg.temperature_display = randmapDlg.temperature;
	randmapDlg.rainfall_display = randmapDlg.rainfall;
	randmapDlg.cont_size_display = randmapDlg.cont_size;
	randmapDlg.altitude_display = randmapDlg.altitude;
	randmapDlg.land_points_display = randmapDlg.land_points;// * (pDoc->PlayerMap.MAP_WIDTH + pDoc->PlayerMap.MAP_HEIGHT) /100;

	if (randmapDlg.DoModal() == IDOK)
	{
		int fertility_factor = ((randmapDlg.temperature * randmapDlg.rainfall) / 100);

		pDoc->MasterMap.randomMap(randmapDlg.width, 
								  randmapDlg.height, 
								  randmapDlg.temperature, 
								  randmapDlg.rainfall, 
								  randmapDlg.cont_size, 
								  randmapDlg.altitude, 
								  randmapDlg.land_points, 
								  fertility_factor, fertility_factor);
		Invalidate(FALSE);
	}
	//testing need to update PlayerMaps also.....
}


void CColAusView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (activeMap == NULL)
	{
		CView::OnLButtonUp(nFlags, point);
		return;
	}

	if  (current_display_type == ENC_DISPLAY)
	{
		current_display_type = MAP_DISPLAY;
		Invalidate(FALSE);
		CView::OnLButtonUp(nFlags, point);
		return;
	}

	//TODO:
	//check if button down was in the main map square
	//check if we are currently viewing the map first....
	//(e.g. europe view, reports, colony view etc....are different)
	if ((point.x < (SQ_WIDTH * DISP_WIDTH))
	&&  (point.y < (SQ_HEIGHT * DISP_HEIGHT)))
	{
		CPoint clicked_map_square = getMapSquare(point);

	//	if (patrol_dest_pending) 
	//	{
	//		allocatePatrol(clicked_map_square);
	//		processMoveToDest(activeList->GetActive());
	//	}
	//	else
	//	{
			reCenter_point(activeMap, clicked_map_square);
			activateClickedPiece(clicked_map_square.x, clicked_map_square.y);
	//	}
	}
	else 
	{
		//TODO:
		//two functions here...?
		//if (activeMap->miniMapClick(CPoint))
		//{
		//    new_loc = activeMap->updateMiniMap(CPoint)
		//    reCenter_point(activeMap, new_loc);
		//}
		int mini_map_x, mini_map_y;

		CPoint delta;
	
		mini_map_x = (DISP_WIDTH * SQ_WIDTH) + 24;
		mini_map_y = 0 + 24;
		CPoint mm_sqSize = getMMSqDim(activeMap);
		//int mm_square_width = 2;
		int mm_pixel_width = (this->MINI_MAP_WIDTH * mm_sqSize.x) + 2;
		int mm_pixel_height = (this->MINI_MAP_HEIGHT * mm_sqSize.y) + 2;

		//check if left button up was in the mini-map area
		if (   (point.x > mini_map_x) && (point.x < mini_map_x + mm_pixel_width)
			&& (point.y > mini_map_y) && (point.y < mini_map_y + mm_pixel_height) )
		{
			//calculate the top left corner map square on the minimap
			int start_x, start_y;
			start_x = display_start.x - (MINI_MAP_WIDTH / 2) + (DISP_WIDTH / 2);
			start_y = display_start.y - (MINI_MAP_HEIGHT / 2) + (DISP_HEIGHT / 2);
			if (start_x < 0)
				start_x = 0;
			if (start_y < 0)
				start_y = 0;
			if (start_x > (activeMap->MAP_WIDTH - MINI_MAP_WIDTH))
				start_x = activeMap->MAP_WIDTH - MINI_MAP_WIDTH;
			if (start_y > (activeMap->MAP_HEIGHT - MINI_MAP_HEIGHT))
				start_y = activeMap->MAP_HEIGHT - MINI_MAP_HEIGHT;

			int current_x, current_y;
			current_x = display_start.x - start_x;
			current_y = display_start.y - start_y;
			
			int new_x, new_y;
			new_x = ((point.x - mini_map_x) / mm_sqSize.x);//2 = width of mapsq in minimap
			new_y = ((point.y - mini_map_y) / mm_sqSize.y);

			delta.x = new_x - current_x;
			delta.y = new_y - current_y; 

			delta.x += display_start.x;
			delta.y += display_start.y;

			reCenter_point(activeMap, delta);
		}
		if (clickActivePieceDisplay(point))
			OnViewActivepiece();
	}

	Invalidate(FALSE);

	CView::OnLButtonUp(nFlags, point);
}

void CColAusView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (activeMap == NULL)
	{
		CView::OnRButtonUp(nFlags, point);
		return;
	}

	if  (current_display_type == ENC_DISPLAY)
	{
		current_display_type = MAP_DISPLAY;
		Invalidate(FALSE);
		CView::OnRButtonUp(nFlags, point);
		return;
	}

	//check if button down was in the main map square
	//eventually check if we are currently viewing the map first....
	//(e.g. europe view, reports, colony view etc....are different)
	if ((point.x < (SQ_WIDTH * DISP_WIDTH))
	&&  (point.y < (SQ_HEIGHT * DISP_HEIGHT)))
	{
		CPoint clicked_map_square = getMapSquare(point);

		//if the shift key is held down, then this is a request to give the 
		//clicked square as the destination for the currently active piece
		int active_piece = activeList->GetActive();
		if (active_piece >= 0)
		{
			if (patrol_dest_pending) 
			{
				allocatePatrol(clicked_map_square);
				//processMoveToDest(activeList->GetActive());
			}
			else
				//check that piece is allowed to move to this square....
				if (checkPieceMoveTerrain(active_piece, clicked_map_square) > 0)
					activeList->setDestination(active_piece, clicked_map_square);
			//recenter to active piece???

			//animate remaining moves towards destination
			processMoveToDest(active_piece);
		}
	}
}

void CColAusView::processMoveToDest(int active_piece)
{
	//TODO: add parameter to specify whether drawing to screen is required
	//or possibly copy this routine, as comp player processing may not be quite the same

	//animate remaining moves towards destination
	CPoint new_loc, old_loc;
	int move_cost, dest_piece;

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	//recenters the view (only if required)
	if (reCenter_active(activeMap, activeList, false))
		paintMapScreen(GetDC(), activeMap);

	new_loc = getDestPieceMove(active_piece);
	old_loc = activeList->GetPosition(active_piece);
	move_cost = checkPieceMove(active_piece, new_loc);
	dest_piece = activeMap->getPiece(new_loc.x, new_loc.y);

	//loop while new moves are possible, and the piece still has some moves
	while(  (new_loc.x >= 0)
	 &&     (move_cost <= activeList->GetMoves(active_piece))
	 &&     (dest_piece == EMPTY)
	 &&     (activeList->GetMoves(active_piece) > 0) )
	{
		if  ( (new_loc.x >= 0)
		&&    (move_cost <= activeList->GetMoves(active_piece)) )
		{
			activeList->updateDir(active_piece, old_loc.x, new_loc.x);

			//only move if there are no pieces in the target square
			if (dest_piece == EMPTY)
			{
				//activeList->SetPosition(active_piece, new_loc.x, new_loc.y);
				activeList->ReduceMoves(active_piece, move_cost);
				movePiece(active_piece, old_loc, new_loc);
				int line_of_sight = activeList->GetLoS(active_piece);
				activeMap->synchMapTerrain(&(pDoc->MasterMap), new_loc, line_of_sight);
				activeMap->updateExplored(new_loc, line_of_sight);
				activeMap->updateRecentlySeen(new_loc, line_of_sight);
				
				//recenters the view (only if required)
				if (reCenter_active(activeMap, activeList, false))
				{
					animation_pending = active_piece;
					//need to avoid displaying currently active piece
					paintMapScreen(GetDC(), activeMap);
					animation_pending = 0;
				}

				animateMove(GetDC(), activeMap,active_piece, old_loc, new_loc);
				//plus repaint side panel.....
				ShowSidePanel(GetDC(), activeMap, activeList);
				pDoc->displayMoves.addMoveInfo(old_loc, new_loc, active_piece,
					                           pDoc->gameStats.getActivePlayer());
		//later....
		//else if the piece is an enemy, then wake up the piece
		//else the piece is a friend, so just stop moving for this turn
			}
		}
		else
			int test_stop = 0;

		//else piece can't be moved, need to reset route start position if it was
		//due to lack of moves (otherwise reset route entirely)
		//{
		//}

		new_loc = getDestPieceMove(active_piece);
		old_loc = activeList->GetPosition(active_piece);
		move_cost = checkPieceMove(active_piece, new_loc);
		dest_piece = activeMap->getPiece(new_loc.x, new_loc.y);

	}

	if  (activeList->GetMoves(active_piece) == 0)
		processToNextPiece();
	else
		//the piece has remaining moves, and has arrived at it's destination, so take appropriate action

		//TODO: if the piece is blocked, i.e. remaining moves, but (-1,-1) returned,
		//then the piece should have destination removed
		//NB - the above seems to be in place already....
	{
		if (activeList->hasDestination(active_piece))
			activeList->removeDestination(active_piece);
		if (activeList->onPatrol(active_piece))
			activeList->reverseDestPatrolPoint(active_piece);
	}

}

bool CColAusView::movePiece(int the_piece, CPoint old_loc, CPoint new_loc)
{
	//moves a piece from one location to another
	//assumes that all move validation has already taken place
	//assumes that the destination square is empty
	//ensures that PieceList and Map are updated correctly, including if the 
	//piece is being carried by another piece

	//as such this is not suitable for displayPrevMoves, as the pieceList should
	//not be updated.....and validation

	//---------initialisation---------
	int dest_piece = activeMap->getPiece(new_loc.x, new_loc.y);
	int dest_owner = activeList->GetOwner(dest_piece);
	int thecarrier = activeList->getCarrier(the_piece);
	//---------validation-------------
	//if (dest_piece != EMPTY)
	//	return false;
	if ( (dest_owner != EMPTY)
	&&   (dest_owner != activeList->GetOwner(the_piece)) )
		return false;
	if ( (activeMap->getPiece(old_loc.x, old_loc.y) != the_piece)
	&&   (activeMap->getPiece(old_loc.x, old_loc.y) != thecarrier) )
		return false;
	//updated to new_loc already for displayPrevMove
	if ( (activeList->GetPosition(the_piece) != old_loc)
	&&   (thecarrier < 0) )
		return false;
	//--------processing--------------
	if (thecarrier >= 0)
	{
		activeList->disembarkPiece(the_piece);
		if (dest_piece == EMPTY)
		{
			activeMap->addPiecetoSquare(the_piece, new_loc);
		}
		else if (dest_owner == activeList->GetOwner(the_piece))
		{
			activeList->addPiecetoCargo(dest_piece, the_piece);
			//activeMap->removePiecefromSquare(old_loc);
		}
	}
	else
	{
		if (dest_piece == EMPTY)
		{
			activeMap->updatePiecePresence(old_loc.x, old_loc.y, new_loc.x, new_loc.y);	
		}
		else if (dest_owner == activeList->GetOwner(the_piece))
		{
			activeList->addPiecetoCargo(dest_piece, the_piece);
			activeMap->removePiecefromSquare(old_loc);
		}
	}
	//superfluous if the piece has embarked onto a carrier - essential otherwise
	activeList->SetPosition(the_piece, new_loc.x, new_loc.y);
	//-------return-data--------------
	return true;
}

void CColAusView::updatePrevMove(int the_piece, CPoint old_loc, CPoint new_loc)
{
	//updates the activeMap to move a piece from one square to another
	//does not update the PieceList at all
	//relies on information on the activeMap only!

	//as such this is suitable only for displayPrevMoves as the pieceList is
	//not be updated or referred to.....and validation is only on the map

	//---------initialisation---------
	int dest_piece = activeMap->getPiece(new_loc.x, new_loc.y);
	int origin_piece = activeMap->getPiece(old_loc.x, old_loc.y);
	//owner information should still be available, as old piece retained for one turn
	int dest_owner = activeList->GetOwner(dest_piece);
	//carrier information may be out of date, so can't be used
	//int thecarrier = activeList->getCarrier(the_piece);
	//---------validation-------------
	//move must have been illegal if owner of destination piece is not the same
	//as owner of the moving piece
	if ( (dest_owner != EMPTY)
	&&   (dest_owner != activeList->GetOwner(the_piece)) )
		return;
	//the old location should have the piece being moved as present - unless being carried
	//if (activeMap->getPiece(old_loc.x, old_loc.y) != the_piece)
	//	return;
	//--------processing--------------
	if (origin_piece != the_piece)
	{
		//must have been carried
		//activeList->disembarkPiece(the_piece);
		if (dest_piece == EMPTY)
		{
			activeMap->addPiecetoSquare(the_piece, new_loc);
		}
		else if (dest_owner = activeList->GetOwner(the_piece))
		{
			//activeList->addPiecetoCargo(dest_piece, the_piece);
			activeMap->removePiecefromSquare(old_loc);
		}
	}
	else
	{
		if (dest_piece == EMPTY)
		{
			activeMap->updatePiecePresence(old_loc.x, old_loc.y, new_loc.x, new_loc.y);	
		}
		else if (dest_owner = activeList->GetOwner(the_piece))
		{
			//activeList->addPiecetoCargo(dest_piece, the_piece);
			activeMap->removePiecefromSquare(old_loc);
		}
	}
	//activeList->SetPosition(the_piece, new_loc.x, new_loc.y);
	//-------return-data--------------
	return;
}

bool CColAusView::clickActivePieceDisplay(CPoint clicked_point)
{
	CPoint active_click_loc;
	active_click_loc = getActiveClickPos();
	
	if  ( (active_click_loc.x == -1)
	&&    (active_click_loc.y == -1) )
		return false;

	CPoint piece_disp_size = map_graphics->getMapSquareSize();

	if  ( (clicked_point.x >= active_click_pos.x) 
	&&    (clicked_point.x < active_click_pos.x + piece_disp_size.x)
	&&    (clicked_point.y >= active_click_pos.y)
	&&    (clicked_point.y < active_click_pos.y + piece_disp_size.y) )
		return true;
	else
		return false;
}

void CColAusView::activateClickedPiece(int piece_loc_x, int piece_loc_y)
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int active_player = pDoc->gameStats.getActivePlayer();
	Map *currentMap = &(pDoc->PlayerMap[active_player]);
	int piece_clicked = currentMap->getPiece(piece_loc_x,
											 piece_loc_y);
	if (piece_clicked >= 0)
	{
		int piece_owner = activeList->GetOwner(piece_clicked);
		if (piece_owner == active_player)
		{
			if  (activeList->GetMoves(piece_clicked) > 0)
				activeList->UpdateActive(piece_clicked);
			activeList->removeDestination(piece_clicked);
			activeList->removePatrol(piece_clicked);
		}
	}
}

void CColAusView::allocatePatrol(CPoint dest)
{
	int active_piece = activeList->GetActive();
	//check the destination is legal for the active piece
	//i.e. is the square empty, 
	//        correct terrain_type,
	//        reachable (i.e. same 'continent')

	if ( (checkPieceMoveTerrain(active_piece,dest) > 0)
	&&   (activeMap->getPiece(dest.x, dest.y) == EMPTY) )
	{
		//TODO - check for same 'continent' i.e. reachable
		activeList->List_of_pieces[active_piece].setPatrol(dest);

		//reset flag now that the requested patrol is established
		patrol_dest_pending = false;
	}
}

CPoint CColAusView::getMapSquare(CPoint point)
{
	CPoint returned_point(-1, -1);
	
	//only calculate point if it is in the map display area 
	//note:square width and disp width etc should come from access functions in CColAusView
	if ((point.x < (SQ_WIDTH * DISP_WIDTH))
	&&  (point.y < (SQ_HEIGHT * DISP_HEIGHT)))
	{
		returned_point.x = display_start.x + point.x / SQ_WIDTH;
		returned_point.y = display_start.y + point.y / SQ_HEIGHT;
	}
	return returned_point;
}

void CColAusView::OnActionsEndturn()
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->endTurn();



	//int active_piece = activeList->GetActive();
	//activeList->UpdateActive(active_piece);

	//recenters the view (only if required)
	//if (reCenter_active(activeMap, activeList))
	//	paintMap(GetDC(), activeMap, DISP_WIDTH, DISP_HEIGHT);

	//processMoveToDest(active_piece);

	Invalidate(FALSE);
}

void CColAusView::OnTestAddpiece()
{
	AddPieceDlg addpieceDlg;

	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//set default values for dialog box
	addpieceDlg.new_piece_owner = pDoc->gameStats.active_player;
			
	if (addpieceDlg.DoModal() == IDOK)
	{
		CPoint new_loc(addpieceDlg.new_loc_x, addpieceDlg.new_loc_y);
		int update_result = pDoc->addPiece(addpieceDlg.new_piece_owner,
										   addpieceDlg.new_piece_type,
										   new_loc);
		CString msg;
		if (update_result == -1)
			msg = "An error was detected, so the piece could not added.";
		else
            msg = "The piece was sucessfully added.";
		AfxMessageBox(msg);
	}

	Invalidate(FALSE);
}

void CColAusView::OnTimer(UINT nIDEvent)
{
	int active_piece;

	switch (nIDEvent)
	{
		case 1:
			//code to redraw the active piece, so it blinks
			active_piece = activeList->GetActive();
			if  ( (IsMapDisplay())
			&&    (active_piece >= 0) )
			{		
				if (display_active)
					display_active = false;
				else
					display_active = true;
		
				//paint active piece and terrain/inactive piece square (if visible)
				CPoint active_pos = activeList->GetPosition(active_piece);
				if (IsVisible(active_pos))
				{
					CPoint mapsq_size = getMapSquareSize();

					paintTerrainSquare(GetDC(), activeMap,
				               active_pos.x, active_pos.y,
							   (active_pos.x - display_start.x) * mapsq_size.x,
							   (active_pos.y - display_start.y) * mapsq_size.y);
					paintInactivePieces(GetDC(), activeMap,
				               active_pos.x - display_start.x, 
							   active_pos.y - display_start.y,
							   1, 1);
					paintActivePiece(GetDC(), activeMap);
				}
			}
			break;
		case 2:
			CColAusDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->midiJukeBox.ShuffleJukeBox();
			break;
	}
	CView::OnTimer(nIDEvent);
}

bool CColAusView::IsMapDisplay(void)
{
	if  (current_display_type == MAP_DISPLAY)
		return true;
	else
		return false;
}

bool CColAusView::IsVisible(CPoint loc)
{
	//returns true if the location specified is in the currently visible map area
	if  ( (loc.x >= display_start.x)
	&&    (loc.y >= display_start.y)
	&&    (loc.x <  display_start.x + DISP_WIDTH)
	&&    (loc.y <  display_start.y + DISP_HEIGHT) )
		return true;
	else
		return false;
}

void CColAusView::OnTestActivatetimer()
{
	display_active_timer = SetTimer(1, 350, NULL);

	//temp code to test Jukebox
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->midiJukeBox.ShuffleJukeBox();

	//DisplayLastError();
}

void CColAusView::OnViewActivepiece()
{
	CColAusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int active_piece = pDoc->MasterList.GetActive();
	if (active_piece >= 0)
	{
		reCenter_active(pDoc->getActiveMap(), &(pDoc->MasterList));
		Invalidate(FALSE);
	}
}


void CColAusView::OnEncyclopediaPieces()
{
	// TODO: Add your command handler code here
	//display encylopedia screen - pieces
	current_display_type = ENC_DISPLAY;

	Invalidate(FALSE);
	//might be better has a standard Windows help file....?
}

void CColAusView::OnActionsSkip()
{
	if (current_display_type == MAP_DISPLAY)
	{
		processSkipMove();
		Invalidate(FALSE);
	}
}

void CColAusView::OnActionsWait()
{
	if (current_display_type == MAP_DISPLAY)
	{
		processWaitMove();
		Invalidate(FALSE);
	}
}

void CColAusView::OnPieceanimateIncrease()
{
	animate_delay_time += 2;
}

void CColAusView::OnPieceanimateDecrease()
{
	animate_delay_time -= 2;
	if (animate_delay_time < 1)
		animate_delay_time = 1;
}

void CColAusView::OnActionsDisband()
{
	if (current_display_type == MAP_DISPLAY)
	{
		processDisbandMove();
		Invalidate(FALSE);
	}
}

void CColAusView::OnActionsSleep()
{
	if (current_display_type == MAP_DISPLAY)
	{
		processSleepMove();
		Invalidate(FALSE);
	}
}

void CColAusView::OnTestDisplaymapstatistics()
{
	//Sets up and displays the dialog for choosing a map statistic
	CMapStatDialog ChooseStatDlg;

	//set up initial value of radio buttons
	switch (stat_display)
	{
		case NONE:
			ChooseStatDlg.display_value = 0;
			break;
		case ALTITUDE:
			ChooseStatDlg.display_value = 1;
			break;
		case TERRAIN_TYPE:
			ChooseStatDlg.display_value = 2;
			break;
		default:
			ChooseStatDlg.display_value = 0;
	}

	//update display_stat with the value selected
	if (ChooseStatDlg.DoModal() == IDOK)
	{
		switch (ChooseStatDlg.display_value)
		{
			case 0:
				stat_display = NONE;
				break;
			case 1:
				stat_display = ALTITUDE;
				break;
			case 2:
				stat_display = TERRAIN_TYPE;
				break;
			default:
				stat_display = NONE;
		}
	}

	Invalidate(FALSE);
}
