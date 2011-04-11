#pragma once
#include "sdl/sdl.h"
#include <iostream>
using namespace std;

SDL_Surface * load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface * desenation);