#pragma once

#include "sdl/sdl.h"
#include "sdl/SDL_mixer.h"
#include "stdio.h"

#define NUM_SONGS				9

class audioHandler
{
private:
	Mix_Music * m_music;

public:
	audioHandler()
	{
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT,2,4096);
	//	m_music->Mix_LoadMUS("Track0.mp3");
	}
};