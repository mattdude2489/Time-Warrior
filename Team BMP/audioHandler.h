#pragma once

#include "sdl/sdl.h"
#include "sdl/SDL_mixer.h"
#include "stdio.h"
#include <time.h>


#define NUM_SONGS				9
#define NUM_SEFFECTS			8
enum e_seffects	{E_SLASH, E_BLUNT, E_FIRE, E_DIVINE, E_LIGHTNING, E_ICE, E_PIERCE, E_BOW};
enum e_music	{M_ENGLAND, M_DESERT, M_DUNGEON1, M_HUB, M_FINALBOSS, M_FOREST, M_CASTLE, M_DUNGEON2, M_MENU};
class AudioHandler
{
private:
	Mix_Music * m_music[NUM_SONGS];
	Mix_Chunk * m_seffects[NUM_SEFFECTS];
	int currentTrack, m_currentWorld;;
public:
	AudioHandler()
	{
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT,2,4096);
		char temp[32];
		currentTrack = m_currentWorld = 0;
		for(int i = 0; i < NUM_SONGS; i ++)
		{
			sprintf_s(temp, "Music/Track%i.wav", i);
			m_music[i] = Mix_LoadMUS(temp);
		}
		for(int i = 0; i < NUM_SEFFECTS; i++)
		{
			sprintf_s(temp, "Music/Effect%i.wav", i);
			m_seffects[i] = Mix_LoadWAV(temp);
		}
	}
	void playMusic()
	{
		Mix_PlayMusic(m_music[currentTrack], -1);
	}
	void playRandom()
	{
		srand(time_t(0));
		currentTrack = rand()%NUM_SONGS;
		Mix_PlayMusic(m_music[currentTrack], -1);
	}
	void nextTrack()
	{
		Mix_HaltMusic();
		currentTrack++; currentTrack %= NUM_SONGS;
		playMusic();
	}
	void previousTrack()
	{
		Mix_HaltMusic();
		currentTrack--;
		if(currentTrack<0)
			currentTrack = NUM_SONGS-1;
		playMusic();
	}
	void playThisTrack(int a_index)
	{
		Mix_HaltMusic();
		currentTrack = a_index;
		playMusic();
	}
	void update(int a_currentWorld, int castleNum)
	{
		if(a_currentWorld != m_currentWorld)
		{
			switch(a_currentWorld)
			{
			case 0: playThisTrack(M_HUB);		break;
			case 1:
			case 6:
			case 7: 
				playThisTrack(M_ENGLAND);	break;
			case 2: 
				switch(rand()%2)
				{
					case 0:playThisTrack(M_DUNGEON1);	break;
					case 1:playThisTrack(M_DUNGEON2);	break;
				}
				break;
			case 3: 
				if(castleNum == 3)
					playThisTrack(M_FINALBOSS);
				else
					playThisTrack(M_CASTLE);
					break;
			case 4: playThisTrack(M_FOREST);	break;
			case 5: playThisTrack(M_DESERT);	break;


			}
			m_currentWorld = a_currentWorld;
		}
	}
	void playEffect(int a_track)
	{
		Mix_PlayChannel(-1, m_seffects[a_track], 0);
	}
	~AudioHandler()
	{
		for(int i = 0; i < NUM_SONGS; i ++)
		{
			Mix_FreeMusic(m_music[i]);
		}
		for(int i =0 ; i < NUM_SEFFECTS; i++)
		{
			Mix_FreeChunk(m_seffects[i]);
		}
	}
};
