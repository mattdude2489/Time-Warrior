#include "sdl/sdl.h"
#include "srect.h"
#include "spoint.h"
#include "UserInput.h"
#include "Chip.h"

//Some debugging includes
#include <stdio.h>

#define WIDTH		800
#define HEIGHT		600
#define BPP			32

int main(int argc, char ** argv)//must be the header for sdl application and yes the arg stuff
{
	//INIT


	SDL_Init(SDL_INIT_EVERYTHING);
//	SDL_Surface * screen =
	SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE);
	SDL_Event e;
	UserInput ui; //This is the UserInput class.
	//the user interface variable
	bool running = true;

	//Some debugging for the UI.
	bool change = false;

	while(running) //While loop, can be easily used for testing.
	{
		while(SDL_PollEvent(&e)) //Polls the events of SDL
		{
			switch(e.type) //If there's an event, then what type?
			{
				case SDL_QUIT: running = false; break; //If The event is SDL quit, get outta there.
				case SDL_MOUSEBUTTONDOWN:
					ui.setMouse(e.motion.x, e.motion.y);
					ui.setClick(e.motion.state);
					change = true;
					break;
				case SDL_MOUSEMOTION:
					ui.setMouse(e.motion.x, e.motion.y);
					change = true;
					break;
				case SDL_KEYDOWN: //This will work until we can get it without using a switch statement.
					ui.setKey(e.key.keysym.sym);
					break;
			}
		}
		if(change)
		{
			printf("%d, %d, Button is: %d, Key is: %c \n", ui.getMouseX(), ui.getMouseY(), ui.getClick(), ui.getKey());
			change = false;
		}
	}
	SDL_Quit();//must do to free the momory that was used for init
	
	
	return 0;
}