#include "stdafx.h"
#include "cSound.h"

#ifndef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/****************************************
       Plays Wav or (Riff) Midi file
****************************************/

cSound::cSound()
{
}

cSound::~cSound()
{
}

DWORD cSound::PlaySoundFile(char *p_s8File)
{
    // It is important to check if the file exists !
    // On Windows NT PlaySound() returns TRUE 
    // even if the file does not exist !
    // Then PlaySound() makes the PC speaker beep !!!
    
    // mciSendString("open...") also gives an absolutely 
    // stupid error message
    // if the file does not exist !

    DWORD u32_Attr = ::GetFileAttributes(p_s8File);
    if (u32_Attr == 0xFFFFFFFF || (u32_Attr & 
            FILE_ATTRIBUTE_DIRECTORY)) 
        return ERROR_FILE_NOT_FOUND;


    // Get file extension
    char *p_s8Ext = strrchr(p_s8File, '.');
    if  (!p_s8Ext)
        return ERR_INVALID_FILETYPE;
    

    if (stricmp(p_s8Ext, ".wav") == 0)
    {
        StopSoundFile();
        
        // PlaySound() is very primitive: no Error Code available
        if (!PlaySound(p_s8File, 0, SND_FILENAME | SND_ASYNC))
            return ERR_PLAY_WAV;

        return 0;
    }
    

    DWORD u32_Err;
    if (!stricmp(p_s8Ext, ".mid") || !stricmp(p_s8Ext, ".midi")
           || !stricmp(p_s8Ext, ".rmi"))
    {
        StopSoundFile();

        static char s8_LastFile[MAX_PATH] = "";

        // the mciSendString("open...") command is slow 
        // (on Windows NT, 2000 and XP) 
        // so we call it only if necessary
        if (strcmp(s8_LastFile, p_s8File) != 0)
        {
            strcpy(s8_LastFile, p_s8File);

            mciSendString("close all", 0, 0, 0);

            char s8_Buf[300];
            sprintf(s8_Buf, 
             "open \"%s\" type sequencer alias MidiDemo", 
             p_s8File);

            if (u32_Err=mciSendString(s8_Buf, 0, 0, 0))
                return u32_Err;
        }

        if (u32_Err=mciSendString("play MidiDemo from 0", 0, 0, 0))
        {
            // replace stupid error messages
            if (u32_Err == 2) u32_Err = MCIERR_SEQ_NOMIDIPRESENT;
            return u32_Err;
        }

        return 0;
    }

    return ERR_INVALID_FILETYPE;
}


/**************************************************
       Stops the currently playing Wav and Midi
***************************************************/

void cSound::StopSoundFile()
{
    PlaySound(0, 0, SND_PURGE); // Stop Wav

    mciSendString("stop MidiDemo", 0, 0, 0); // Stop Midi
}

/*****************************************************
		CMidiJukeBox class
******************************************************/

CMidiJukeBox::CMidiJukeBox(void)
{
	InitJukeBox();
}

CMidiJukeBox::~CMidiJukeBox(void)
{
}

void CMidiJukeBox::InitJukeBox(void)
{
	GetCurrentDirectory(300, s_CurDir.GetBuffer(300));
	s_CurDir.ReleaseBuffer();

	SongTitle[0].SetString("Bound for Botany Bay");
	SongTitle[1].SetString("Wild Colonial Boy");
	SongTitle[2].SetString("The Irish Emigrant");
	SongTitle[3].SetString("Waltzing Matilda");
	SongTitle[4].SetString("Scarborough Fair");
	SongTitle[5].SetString("The Wild Rover");

	SongFileName[0].SetString("\\Sounds\\botany_bay.mid");
	SongFileName[1].SetString("\\Sounds\\wildcolonialboy.mid");
	SongFileName[2].SetString("\\Sounds\\irishemigrant.mid");
	SongFileName[3].SetString("\\Sounds\\waltzingmatilda.mid");
	SongFileName[4].SetString("\\Sounds\\scarboroughfair.mid");
	SongFileName[5].SetString("\\Sounds\\wildrover.mid");

	SongType[0].normal_play = true;
	SongType[1].normal_play = true;
	SongType[2].normal_play = true;
	SongType[3].normal_play = true;
	SongType[4].normal_play = true;
	SongType[5].normal_play = true;

	num_allocated_songs = 6;
	current_song = 0;
	shuffle_type = SEQUENTIAL;
}

int CMidiJukeBox::ShuffleJukeBox(int song_no)
{
	//start playing the song specified	
	//return non-zero only if there was no error
	if ( (song_no < 0)
	||   (song_no >= MAX_SONGS) )
		return 0;

	CString s_CurSongPath;
	s_CurSongPath.SetString(s_CurDir);
	s_CurSongPath.Append(SongFileName[song_no]);

	char *s8_Buf = s_CurSongPath.GetBuffer(300);

	DWORD u32_Err;
	if (u32_Err = midiPlayer.PlaySoundFile(s8_Buf))
	{
		// errors defined in cSound.h
		if      (u32_Err == ERR_INVALID_FILETYPE)     strcpy(s8_Buf, "This filetype is not supported !");
		else if (u32_Err == ERR_PLAY_WAV)             strcpy(s8_Buf, "Windows could not play the Wav file !");
		else if (u32_Err == MCIERR_SEQ_NOMIDIPRESENT) strcpy(s8_Buf, "There is no Midi device installed or it is used by another application!");
		else
		{
			// translate errors from WinError.h
			if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, u32_Err, 0, s8_Buf, sizeof(s8_Buf), 0))
			{
				// translate errors from MMsystem.h
				if (!mciGetErrorString(u32_Err, s8_Buf, sizeof(s8_Buf)))
				{
					sprintf(s8_Buf, "Error %d", u32_Err);
				}
			}
		}
		AfxMessageBox(s8_Buf);
		return 0;
	}
	return 1;
}

int CMidiJukeBox::ShuffleJukeBox(void)
{
	//play the next song on the jukebox, chosen depending on shuffle type
	bool song_chosen = false;

	if (shuffle_type == SEQUENTIAL)
	{
		for (current_song++; !song_chosen; current_song++)
		{
			if (current_song >= num_allocated_songs)
				current_song = 0;
			if (SongType[current_song].normal_play)
			{
				int shuffle_ok = ShuffleJukeBox(current_song);
				if (shuffle_ok)
					return current_song;
				else
					return -1;
			}
		}
	}
	else
		//shuffle_type == RANDOM
		for (int new_song = rand() % num_allocated_songs; 
			 !song_chosen; 
			 new_song = rand() % num_allocated_songs)
		{
			if ( (SongType[new_song].normal_play)
			&&   (new_song != current_song) )
			{
				current_song = new_song;
				ShuffleJukeBox(current_song);
				return current_song;
			}
		}
	return -1;
}

bool CMidiJukeBox::setShuffleType(int req_shuffle_type) 
{
	switch (req_shuffle_type)
	{
		case RANDOM:	
		case SEQUENTIAL:
			shuffle_type = req_shuffle_type;
			return true;
		default:
			return false;
	}
}

int CMidiJukeBox::getShuffleType(void)
{
	return shuffle_type;
}

char *CMidiJukeBox::getSongTitle(int song_num)
{
	return SongTitle[song_num].GetBuffer(50);
}

int CMidiJukeBox::getCurSongNum(void)
{
	return current_song;
}