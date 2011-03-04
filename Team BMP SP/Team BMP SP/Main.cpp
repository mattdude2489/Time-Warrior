#include "sdl/sdl.h"
#include "sdl/SDL_mixer.h"

#define WIDTH		800
#define HEIGHT		600
#define	BPP			32



int main(int argc, char ** argv)//must be the header for sdl application and yes the arg stuff
{
	//INIT
	
	//init the grid stuff
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface * screen =
	SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE);

	
	/*Mix_CloseAudio();*/ 
	SDL_Quit();//must do to free the momory that was used for init
	
	
	return 0;
}