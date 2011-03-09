#include "sdl/sdl.h"
#include "srect.h"
#include "spoint.h"
#include "UserInput.h"
#include "Chip.h"
#include "Entity.h"

//Some debugging includes
#include <stdio.h>

#define WIDTH		800
#define HEIGHT		600
#define BPP			32
#define FRAMEDELAY	20;

int main(int argc, char ** argv)//must be the header for sdl application and yes the arg stuff
{
	//INIT


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface * screen = 
		SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE);
	SDL_Event e;
	UserInput ui; //This is the UserInput class.

	//Timer stuff, for updateing sprites and anything else
	Uint32 then = SDL_GetTicks(), now, soon, passed;

	//the user interface variable
	bool running = true;

	//Some debugging for the UI.
	bool change = false;

	//loading asprite to see if it works with the entity
	Sprite test("spriteTest.bmp", 3, 32);
	test.setTransparency(255,0,255);
	test.setHIndex(4,3);
	Entity eTest(0,0,0,0,0,0,0,0,test);

	while(running) //While loop, can be easily used for testing.
	{
		//update the time
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
		soon = now + FRAMEDELAY;

		//update
		test.update(passed);




		//input
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
				case SDL_MOUSEMOTION: //If the mouse moves, then do this.
					ui.setMouse(e.motion.x, e.motion.y);
					change = true; //Makes the console update.
					break;
				case SDL_KEYDOWN: //This will work until we can get it without using a switch statement.
					ui.setKey(e.key.keysym.sym);
					change = true; //Makes the console update.
					break;
			}
		}
		if(change)
		{
			//printf("%d, %d, Button is: %d, Key is: %c \n", ui.getMouseX(), ui.getMouseY(), ui.getClick(), ui.getKey());
			change = false;
		}
				//draw
		test.draw(screen,100,100);
		SDL_Flip(screen);

		SDL_Delay(100);
	}
	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init
	
	
	return 0;
}