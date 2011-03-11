#include "sdl/sdl.h"
#include "srect.h"
#include "spoint.h"
#include "UserInput.h"
#include "Chip.h"
#include "Entity.h"
#include "Player.h"
#include "Hud.h"

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
	SDL_Surface * screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_SWSURFACE);
	SDL_Event e;
	UserInput ui; //This is the UserInput class.

	//Timer stuff, for updateing sprites and anything else
	Uint32 then = SDL_GetTicks(), now, soon, passed;

	//the user interface variable
	bool running = true;

	//Some debugging for the UI.
	bool change = false;

	//loading a sprite to see if it works with the entity
	Sprite test("Sprites/spriteTest.bmp", 3, 32, 4);
	test.setTransparency(255,0,255);
	test.setHIndex(2,3);
	Player eTest(10,10,10,100,100,10,10,10,&test);
	Hud Ghud;

	if(test.isSprite())
	{
		printf("Heya! \n");
	}

	while(running) //While loop, can be easily used for testing.
	{
		//update the time
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
		soon = now + FRAMEDELAY;

		//input
		while(SDL_PollEvent(&e)) //Polls the events of SDL
		{
			switch(e.type) //If there's an event, then what type?
			{
				case SDL_QUIT: running = false; break; //If The event is SDL quit, get outta there.
				case SDL_MOUSEBUTTONDOWN:
					ui.setMouse(e.motion.x, e.motion.y);
					ui.setClick(e.motion.state);
					break;
				case SDL_MOUSEMOTION: //If the mouse moves, then do this.
					ui.setMouse(e.motion.x, e.motion.y);
					break;
				case SDL_KEYDOWN: //This will work until we can get it without using a switch statement.
					ui.setKey(e.key.keysym.sym);
					ui.updateUI(false);
					break;
				case SDL_KEYUP:
					ui.setKey(e.key.keysym.sym);
					ui.updateUI(true);
					break;
			}
		}
		  	//printf("%d, %d, Button is: %d, Key is: %c \n", ui.getMouseX(), ui.getMouseY(), ui.getClick(), ui.getKey());
			eTest.handleInput(ui);

		//update
		//test.update(passed);
		eTest.update(passed);

		Ghud.updateHud(eTest);


		//draw
		//test.draw(screen, 100, 100);
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		eTest.draw(screen);
		Ghud.draw(screen);

		SDL_Flip(screen);
		SDL_Delay(100);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}