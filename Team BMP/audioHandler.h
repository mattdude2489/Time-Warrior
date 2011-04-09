#pragma once

#include "sdl/sdl.h"
#include "sdl/SDL_mixer.h"
#include "stdio.h"
#include <time.h>

#define NUM_SONGS				9

class AudioHandler
{
private:
	Mix_Music * m_music[NUM_SONGS];
	int currentTrack;
public:
	AudioHandler()
	{
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT,2,4096);
		char temp[32];
		currentTrack = 0;
		for(int i = 0; i < NUM_SONGS; i ++)
		{
			sprintf_s(temp, "Music/Track%i.wav", i);
			m_music[i] = Mix_LoadMUS(temp);
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
	~AudioHandler()
	{
		for(int i = 0; i < NUM_SONGS; i ++)
		{
			Mix_FreeMusic(m_music[i]);
		}
	}
};