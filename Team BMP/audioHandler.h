#pragma once

#include "sdl/sdl.h"
#include "sdl/SDL_mixer.h"
#include "stdio.h"
#include <time.h>

#define NUM_SONGS				9
#define NUM_EFFECTS				3
enum	e_effects	{E_SLASH, E_BLUNT, E_FIRE};

class AudioHandler
{
private:
	Mix_Music * m_music[NUM_SONGS];
	Mix_Chunk * m_effects[NUM_EFFECTS];
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
		for(int i = 0; i < NUM_EFFECTS; i++)
		{
			sprintf_s(temp, "Music/Effect%i.wav", i);
			m_effects[i] = Mix_LoadWAV(temp);
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
	void update(int a_currentWorld)
	{
		if(m_currentWorld!= a_currentWorld)
		{
			nextTrack();
			m_currentWorld = a_currentWorld;
		}
	}
	void playEffect(int a_track)
	{
		Mix_PlayChannel(-1, m_effects[a_track], 0);
	}
	~AudioHandler()
	{
		for(int i = 0; i < NUM_SONGS; i ++)
		{
			Mix_FreeMusic(m_music[i]);
		}
		for(int i =0 ; i < NUM_EFFECTS; i++)
		{
			Mix_FreeChunk(m_effects[i]);
		}
	}
};
