// Paint1Doc.h : interface of the CColAusDoc class
//


#pragma once

#include "map.h"
#include "piece.h"
#include "gameStats.h"
#include "GameRules.h"
#include "MoveList.h"
#include "cSound.h"

//const int MAX_PLAYERS = 6;

class CColAusDoc : public CDocument
{
protected: // create from serialization only
	CColAusDoc();
	DECLARE_DYNCREATE(CColAusDoc)

// Attributes
private:
	CString *m_csFileName;
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	BYTE *m_lpImage;
	int m_iCurrentPointer, m_iFileSize, m_iBlockSize;

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszFileName);
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CColAusDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//game creation
	void createGame(void);
	BOOL validMapFile(char *);
	BOOL createMap(char *, Map *);
	//flow of play control
	void endTurn(void);

	//utility functions
	void synchPlayerMaps(void);
	void synchMasterMap(int player);
	void updateAllExplored(int player);
	void refreshRecentlySeen(int player);
	int  addPiece(int player, int piece_type, CPoint new_loc);
	Map *getActiveMap(void);

	//access functions
	char *getPlayerAdj(int player_number);

	//data members specific to a given game
	Map PlayerMap[MAX_PLAYERS];
	Map MasterMap;
	PieceList MasterList;
	GameStats gameStats;
	PlayerStats playerStats[MAX_PLAYERS];
	CMoveList displayMoves;
	CMidiJukeBox midiJukeBox;

	//generic (constant) data members
	GameRules RuleBase;
	
	bool current_game;
	bool first_time;

	//How to store colonies?
	//Colony ColList[MAX_COLONYS];

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnTestSynch();
};