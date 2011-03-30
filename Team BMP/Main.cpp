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
#include "Minion.h"

//#define WITH_NETWORKING

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

#ifdef WITH_NETWORKING
	ServerModule s(9999);
	while(s.getState() != NetModule::LISTENING)
	{
		s.run();
		printf("Server state: %s \n", s.getStateText());
	}
	ClientModule c("127.0.1",9999);
	while(s.size() == 0)//allows player to connect
	{
		s.run();
		printf("Server state: %s \n", s.getStateText());
		c.run();
		printf("Client state: %s \n", c.getStateText());
	}
#endif
	//Timer stuff, for updateing sprites and anything else
	Uint32 then = SDL_GetTicks(), now, passed;

	//the user interface variable
	bool running = true;
	//loading a sprite to see if it works with the entity
	SDL_Sprite test("Sprites/skeleton.bmp", 24, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
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
	chip.levelUp();
	chip2.levelUp();
	chip3.levelUp();
	chip4.levelUp();
	SDL_Sprite aTest("Sprites/greenguy.bmp", SPRITE_SIZE, SPRITE_SIZE, SPRITE_SPEED, SPRITE_ROWS);
	aTest.setTransparency(COLOR_TRANSPARENT);
	aTest.restart(2);
	Entity fightTest(0, 0, 0, 100, 100, 0, 0, 0, &aTest);
	eTest.setGauntletSlot(SLOT_ATK1, &chip);
	eTest.setGauntletSlot(SLOT_ATK2, &chip2);
	world.add(&eTest);
	world.add(&chip);
	world.add(&chip2);
	world.add(&chip3);
	world.add(&chip4);
	world.add(&fightTest);
	world.setCamera(eTest.getCamera());
	//music test stuff
	
	AudioHandler ah;
	ah.playMusic();
#ifdef WITH_NETWORKING
	char send[30], old[30], *in;
	bool changeInInfoSoSend = false;
#endif
	if(!world.getSuccess()){printf("The map was loaded unsuccessfully. THERE IS A PROBLEM.");}
	UserInput aui;

	while(running) //While loop, can be easily used for testing.
	{
		//update the time
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
#ifdef WITH_NETWORKING
		if(c.getInbox()->getRawList() != NULL&& (unsigned char *)old != c.getInbox()->getRawList())
		{
			in = (char *)c.getInbox()->getRawList();
			c.getInbox()->clear();
			aui.convertServerInfo(in);
			printf("mouse state after server: %i\n", aui.getClick());
		//	eTest.handleServerInfo(in);
		//	printf(in);
		//	printf("server state: %s\n clientState: %s\n", s.getStateText(),c.getStateText()); 
		}

#endif
		//input
		while(SDL_PollEvent(&e)) //Polls the events of SDL
		{
			switch(e.type) //If there's an event, then what type?
			{
				case SDL_QUIT: running = false; break; //If The event is SDL quit, get outta there.
				case SDL_MOUSEBUTTONDOWN:
					ui.setMouse(e.motion.x, e.motion.y);
					ui.setClick(e.motion.state);
					printf("mouse state : %i \n", e.motion.state);
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
					printf("Key Pressed: %c \n", e.key.keysym.sym);
					ui.updateUI(false);
					break;
				case SDL_KEYUP:
					ui.setKey(e.key.keysym.sym);
					ui.updateUI(true);
					break;
			}
		}
		//printf("%d, %d, Button is: %d, Key is: %c \n", ui.getMouseX(), ui.getMouseY(), ui.getClick(), ui.getKey());
#ifdef WITH_NETWORKING
		eTest.handleInput(&aui, &world);
#else
		eTest.handleInput(&ui, &world);
#endif
		//update
		world.update(passed);
		Ghud.updateHud(&eTest, &ui);
		
		//draw
		//reset the screen.
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		world.draw(screen);
		Ghud.draw(screen);
	//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
#ifdef WITH_NETWORKING
		ui.sendUi2Server(send);
		for(int i = 0; i < strlen(send)&& changeInInfoSoSend == false; i++)
		{
			if(old[i] != send[i])
				changeInInfoSoSend = true;
		}
		if(changeInInfoSoSend)
		{	
			printf("sent: %i vs hit: %i\n", send[2], ui.getClick());
			c.getOutbox()->add(send);
			for(int i = 0; i < strlen(old); i++)
			{
				old[i] = send[i];
			}
			changeInInfoSoSend = false;
		//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
		}
#endif
		//reset the mouse input. Why was this so hard to figure out?
		ui.resetClick();
		s.run();
		c.run();
		SDL_Flip(screen);
		SDL_Delay(SPRITE_SPEED);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}