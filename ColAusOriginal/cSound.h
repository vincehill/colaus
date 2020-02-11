//The cSound class
#if !defined(cSound_h_ElmueSoft)
#define cSound_H_ElmueSoft

#if _MSC_VER > 1000
#pragma once
#endif

#include "Mmsystem.h"

#define  ERR_INVALID_FILETYPE 50123
#define  ERR_PLAY_WAV         50124

class cSound
{
public:
    cSound();
    virtual   ~cSound();

    DWORD      PlaySoundFile(char *p_s8File);
    void       StopSoundFile();
};

#endif //!defined(cDound_H_ElmueSoft)

const int MAX_SONGS = 10;

const int RANDOM = 1;
const int SEQUENTIAL = 2;

struct sSongType
{
	bool normal_play;
};

class CMidiJukeBox
{
public:
	CMidiJukeBox();
	~CMidiJukeBox();

	void InitJukeBox(void);
	int ShuffleJukeBox(int);
	int ShuffleJukeBox(void);
	char *getSongTitle(int);
	bool setShuffleType(int);
	int getShuffleType(void);
	int getCurSongNum(void);

	int num_allocated_songs;
	int current_song;
	int shuffle_type;

	cSound midiPlayer;

	CString SongTitle[MAX_SONGS];
	CString SongFileName[MAX_SONGS];
	sSongType SongType[MAX_SONGS];

	CString s_CurDir;
};