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
#include "servermodule.h"


//Some debugging includes
#include <stdio.h>

int main(int argc, char ** argv)//must be the header for sdl application and yes the arg stuff
{
	//INIT


	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_Event e;
	UserInput ui; //This is the UserInput class.
	//networking
	ServerModule s(9999);
	while(s.getState() != NetModule::LISTENING)
	{
		s.run();
		printf("Server state: %s \n", s.getStateText());
	}
	ClientModule c("127.0.1",9999);
	while(s.size() == 0)
	{
		s.run();
		printf("Server state: %s \n", s.getStateText());
		c.run();
		printf("Client state: %s \n", c.getStateText());
	}

	//Timer stuff, for updateing sprites and anything else
	Uint32 then = SDL_GetTicks(), now, passed;

	//the user interface variable
	bool running = true;
	//loading a sprite to see if it works with the entity
	SDL_Sprite test("Sprites/spriteTest.bmp", 24, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
	test.setTransparency(COLOR_TRANSPARENT);
	//test.setHIndex(2,3);
	test.restart(2);
	Player eTest(15,1,39,100,100,100,5,9,&test);
	BasicSlash chip;
	BasicFire chip2;
	AdvancedFire chip3;
	ExpertFire chip4;
	eTest.addToAttackInventory(&chip);
	eTest.addToAttackInventory(&chip2);
	eTest.addToAttackInventory(&chip3);
	eTest.addToAttackInventory(&chip4);
	Hud Ghud;
	World world;
	chip.setWorld(&world);
	chip.levelUp();
	chip2.setWorld(&world);
	chip2.levelUp();
	chip3.setWorld(&world);
	chip3.levelUp();
	chip4.setWorld(&world);
	chip4.levelUp();
	SDL_Sprite aTest("Sprites/greenguy.bmp", SPRITE_SIZE, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
	aTest.setTransparency(COLOR_TRANSPARENT);
	aTest.restart(2);
	Entity fightTest(0, 0, 0, 100, 100, 0, 0, 0, &aTest);

	eTest.setGauntletSlot(SLOT_ATK1, &chip);
	eTest.setGauntletSlot(SLOT_ATK2, &chip2);
	world.setAnEntity(&eTest);
	world.setAnEntity(&chip);
	world.setAnEntity(&chip2);
	world.setAnEntity(&chip3);
	world.setAnEntity(&chip4);
	world.setAnEntity(&fightTest);
	//music test stuff
	
	AudioHandler ah;
	ah.playMusic();
	char send[10], old[10], *in;
	if(!world.getSuccess()){printf("The map was loaded unsuccessfully. THERE IS A PROBLEM.");}
	UserInput aui;

	while(running) //While loop, can be easily used for testing.
	{
		//update the time
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
		if(c.getInbox()->getRawList() != NULL)
		{
			in = (char *)c.getInbox()->getRawList();
			c.getInbox()->clear();
			aui.convertServerInfo(in);
			printf("converted data: %c %c\n", aui.getKeyLR(), aui.getKeyUD());
		//	printf("server state: %s\n clientState: %s\n", s.getStateText(),c.getStateText()); 
		}

		s.run();
		c.run();
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
					ui.setClick(CLICK_NONE);
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
		eTest.handleInput(&aui, &world);
		//update
		world.update(passed);
		Ghud.updateHud(&eTest, &ui);
		//reset the mouse input. Why was this so hard to figure out?
		ui.resetClick();

		//draw
		//test.draw(screen, 100, 100);
		//reset the screen.
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		world.draw(screen);
		/*if(fightTest.collide(&chip))
		{
			printf("Hi, collision has occurred between chip1 and the player.\n");
			fightTest.hit(chip.getDamage());
			chip.deactivate();
		}*/
		//test.draw(screen, 100, 100);
		Ghud.draw(screen);
	//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
		ui.sendUi2Server(send);
		if(aui.getKeyLR() != ui.getKeyLR() || aui.getKeyUD() != ui.getKeyUD())
		{
			
			c.getOutbox()->add(send);
		//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
		}
		
		SDL_Flip(screen);
		SDL_Delay(100);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}