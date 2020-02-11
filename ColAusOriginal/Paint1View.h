// Paint1View.h : interface of the CPaint1View class
//
#include "Map.h"
#include "BitmapStore.h"

#pragma once

const int MAP_DISPLAY = 1;
const int ENC_DISPLAY = 2;

const int PIECES = 1;

/*--------------------------------------------------------*/

class CColAusView : public CView
{
protected: // create from serialization only
	CColAusView();
	DECLARE_DYNCREATE(CColAusView)

// Attributes
public:
	CColAusDoc* GetDocument() const;

protected:
	enum {Fonts, Pens, Brushes, Bitmaps} m_Display;
	BitmapStore ColGraphics;
	BitmapStore* map_graphics;

	//map display paramaters
	int DISP_WIDTH;
	int DISP_HEIGHT;
	int MINI_MAP_WIDTH;
	int MINI_MAP_HEIGHT;
	CPoint mm_sqSize;
	CPoint mm_Size;
	CPoint display_start;
	int res_selection;
	int old_res;
	int animate_delay_time;
	CPoint active_click_pos;
	
	void InitResolution(void);
	void UpdateDisplaySize(Map *activeMap);
	void initMemDCs(CDC* pDC);

	void reCenter_point(Map *theMap, CPoint focus);
	//int  reCenter_active(Map *theMap, PieceList *thePieces);
	int  reCenter_active(Map *theMap, PieceList *thePieces, bool recenter_both = true);
	int  reCenter_location(CPoint new_loc, bool recenter_both = true);

	CPoint getMapSquare(CPoint screen_point);
	void activateClickedPiece(int piece_loc_x, int piece_loc_y);
	void updateActiveClickPos(int x, int y);
	CPoint getActiveClickPos(void);
	bool clickActivePieceDisplay(CPoint clicked_point);

	//members for display of map statistics
	enum {NONE, ALTITUDE, TERRAIN_TYPE} stat_display;
	int getStatValue(Map *theMap, CPoint display_point);
	int paintMapStats(CDC *pDC, 
		        	  Map *theMap, 
					  int disp_origin_x, int disp_origin_y,
					  int disp_width, int disp_height);
	int paintStat(CDC *pDC, int stat_value, int x, int y);

	//functions/data members
	Map* activeMap;
	PieceList* activeList;
	bool display_active;	//used to cycle flashing of active piece
	int animation_pending;
	BOOL t_hide_unexplored;
	UINT_PTR display_active_timer;
	UINT_PTR midi_song_timer;
	CDC memoryDC;
	CBitmap memoryBitmap;
	CSize disp_size;
	bool memoryDC_created;
	void copyDC(CDC* fromDC, CDC* toDC);
	CPoint getMMSqDim(Map *theMap);
	bool patrol_dest_pending;	//indicates a patrol destination is pending

	//signifies if the system is in a state to accept
	//another input to move a piece
	bool move_key_ok;
	//signifies if a move has just occured, and needs to
	//be animated to the screen
	bool piece_just_moved;
	LOGFONT FontLarge;
	//flag to switch display of tag_text on/off
	bool display_tag_text;

	bool isMoveKey(UINT nChar); 
	void processMoveKey(UINT nChar);
	void processSkipMove();
	void processWaitMove();
	void processDisbandMove();
	void processSleepMove();
	void processPatrolMove();
	int resolveConflict(int attacker, int defender);
	void allocatePatrol(CPoint clicked_map_square);

	//utility functions for movement processing
	bool movePiece(int piece, CPoint old_loc, CPoint new_loc);
	void updatePrevMove(int the_piece, CPoint old_loc, CPoint new_loc);

	//display types
	int current_display_type;
	bool IsMapDisplay(void);
	bool IsVisible(CPoint loc);

	//encylopedia display functions
	void ShowEnclopediaMenu(CDC* pDC, int menu_type);

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CColAusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int paintBackSquare(CDC *memDCPtr, Map *theMap, int i, int j);
	
	int paintMiniMap(CDC *pDC, Map *theMap, PieceList *thePieces, int x, int y);
	int paintPiece(CDC *pDC, PieceList *thePieces, int piece_number, int x, int y);

	//updated paint functions
	int paintUnexplored(CDC *memDCPtr, 
						 Map *theMap,
						 int disp_origin_x, int disp_origin_y,
						 int disp_width, int disp_height);

	int paintMap(CDC *pDC, 
		 	     Map *theMap, 
	             int disp_width, int disp_height);
	int paintTerrainSquares(CDC *pDC, 
		                    Map *theMap, 
			                int disp_origin_x, int dis_origin_y,
			                int disp_width, int disp_height, bool relative = true);
	int paintTerrainSquare(CDC *pDC, Map *theMap, 
		                   int map_x, int map_y, int cdc_x, int cdc_y);
	int paintBackSquare(CDC *memDCPtr, Map *theMap, 
		                int map_x, int map_y, int cdc_x, int cdc_y);
	int paintInactivePieces(CDC *pDC, 
		            Map *theMap, 
				    int disp_origin_x, int disp_origin_y,
				    int disp_width, int disp_height);
	int paintActivePiece(CDC *pDC, Map *theMap);
	void paintMapScreen(CDC *pDC, Map *theMap);


	int animateMove(CDC *pDC, Map *theMap,
				     int piece_number, 
				     CPoint move_from, CPoint move_to);
	int checkPieceMove(int piece_number, CPoint destination);
	int checkPieceMoveTerrain(int piece_number, CPoint dest);
	bool checkPieceCanMoveTo(int piece_number, CPoint dest);
	CPoint findClosestCompSquare(CPoint start, int piece_number, CRect search_area);
	CPoint getDestPieceMove(int piece_number);
	void processMoveToDest(int active_piece);
	void processToNextPiece(void);
	void ShowWoodCut(CDC* pDC, int);
	void displayPrevMoves(void);
	void processTurnStart(void);

	//utility functions
	CPoint getMapSquareSize(void);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void ShowColmap(CDC* pDC, Map* thePtr, PieceList* thePieces);
	void ShowSidePanel(CDC* pDC, Map* theMap, PieceList* thePieces);
	void ShowBackgroundPic(CDC* pDC);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	CSize ShowText(CDC* pDC, char* text, int x, int y);
public:
	afx_msg void OnTestHideUnexplored();
	afx_msg void OnUpdateTestHideUnexplored(CCmdUI *pCmdUI);
	afx_msg void OnTestNewmap();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFileGameoptions();
	afx_msg void OnActionsEndturn();
	afx_msg void OnTestAddpiece();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTestActivatetimer();
	afx_msg void OnViewActivepiece();
	afx_msg void OnEncyclopediaPieces();
	afx_msg void OnActionsSkip();
	afx_msg void OnActionsWait();
	afx_msg void OnPieceanimateIncrease();
	afx_msg void OnPieceanimateDecrease();
	afx_msg void OnActionsDisband();
	afx_msg void OnActionsSleep();
	afx_msg void OnTestDisplaymapstatistics();
};

#ifndef _DEBUG  // debug version in Paint1View.cpp
inline CColAusDoc* CColAusView::GetDocument() const
   { return reinterpret_cast<CColAusDoc*>(m_pDocument); }
#endif

#pragma once



