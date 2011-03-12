#pragma once
#include "sdl/sdl.h"
#include <iostream>
using namespace std;

SDL_Surface * load_image(std::string filename)
{
	SDL_Surface * loaded = NULL;
	SDL_Surface * optimized = NULL;
	loaded = SDL_LoadBMP(filename.c_str());
	if(loaded!=NULL)
	{
		optimized = SDL_DisplayFormat(loaded);
		SDL_FreeSurface(loaded);
	}
	return optimized;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface * desenation)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source, NULL, desenation, &offset);
}