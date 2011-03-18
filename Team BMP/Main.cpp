#include "sdl/sdl.h"
#include "srect.h"
#include "spoint.h"
#include "UserInput.h"
#include "Chip.h"
#include "Entity.h"
#include "Player.h"
#include "World.h"
#include "Hud.h"
#include "audiohandler.h"


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

	//loading a sprite to see if it works with the entity
	SDL_Sprite test("Sprites/spriteTest.bmp", 25, 32, 32, 4);
	test.setTransparency(COLOR_TRANSPARENT);
	//test.setHIndex(2,3);
	test.restart(2);
	Player eTest(15,1,39,100,100,100,5,9,&test);
	BasicFire chip;
	AdvancedFire chip2;
	Hud Ghud;
	World world;
	chip.setOwner(&eTest);
	chip.levelUp();
	chip2.setOwner(&eTest);
	chip2.levelUp();
	SDL_Sprite aTest("Sprites/greenguy.bmp", 32, 33, 32, 4);
	aTest.setTransparency(COLOR_TRANSPARENT);
	aTest.restart(2);
	Entity fightTest(0, 0, 0, 100, 100, 0, 0, 0, &aTest);

	eTest.setGauntletSlot(SLOT_ATK1, &chip);
	eTest.setGauntletSlot(SLOT_ATK2, &chip2);
	world.setAnEntity(&eTest);
	world.setAnEntity(&chip);
	world.setAnEntity(&chip2);
	world.setAnEntity(&fightTest);
	//music test stuff
	AudioHandler ah;
	ah.playMusic();
	if(!world.getSuccess())
	{
		printf("The map was loaded unsuccessfully. THERE IS A PROBLEM.");
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
				case SDL_MOUSEBUTTONUP:
					ui.setMouse(e.motion.x, e.motion.y);
					ui.setClick(0);
					break;
				case SDL_MOUSEMOTION: //If the mouse moves, then do this.
					ui.setMouse(e.motion.x, e.motion.y);
					break;
				case SDL_KEYDOWN: //This will work until we can get it without using a switch statement.
					ui.setKey(e.key.keysym.sym);
			//		test.zoom(200);
					ui.updateUI(false);
					break;
				case SDL_KEYUP:
					ui.setKey(e.key.keysym.sym);
					ui.updateUI(true);
					break;
			}
		}
		//printf("%d, %d, Button is: %d, Key is: %c \n", ui.getMouseX(), ui.getMouseY(), ui.getClick(), ui.getKey());
		eTest.handleInput(&ui);

		//update
		//test.update(passed);
		world.update(passed);

		Ghud.updateHud(&eTest, &ui);

		//draw
		//test.draw(screen, 100, 100);
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		world.draw(screen);
		if(fightTest.collide(&chip))
		{
			printf("Hi, collision has occurred between chip1 and the player.\n");
			fightTest.hit(chip.getDamage());
			chip.deactivate();
		}
		//test.draw(screen, 100, 100);
		Ghud.draw(screen);

		SDL_Flip(screen);
		SDL_Delay(100);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}