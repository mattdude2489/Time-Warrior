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
#include "NPC.h"

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
	ClientModule c("127.0.0.1",9999);
	while(c.getState() != NetModule::WAITING_FOR_DATA)//allows player to connect
	{
		s.run();
		printf("Server state: %s \n", s.getStateText());
		c.run();
		printf("Client state: %s \n", c.getStateText());
	}
#endif
	//Timer stuff, for updateing sprites and anything else
	Uint32 then = SDL_GetTicks(), now, passed, second = 0;

	//the user interface variable
	bool running = true;
	//loading a sprite to see if it works with the entity
	SDL_Sprite test("Sprites/SpriteTest.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Player eTest(15,1,39,100,100,100,5,9,&test);
	SDL_Sprite test2("Sprites/greenguy.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Entity fightTest(0, 0, 0, 100, 100, 0, 0, 0, &test2);
	NonPlayerChar diaTest(&test2);
	SDL_Sprite test3("Sprites/skeleton.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Minion minion(0, 0, 0, 100, 100, 0, 0, 0, &test3);
	SDL_Sprite slime("Sprites/slime.bmp", 31, 23, FRAME_RATE, NUM_ROWS);
	Minion m_slime(0 ,0, 0, 100, 100, 0, 0, 0, &slime);
	fightTest.move(100, -100);
	minion.move(-50, -50);
	m_slime.move(-100,-100);
	diaTest.move(100, 100);
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
	eTest.setGauntletSlot(SLOT_ATK1, &chip);
	eTest.setGauntletSlot(SLOT_ATK2, &chip2);
	world.add(&eTest);
	world.add(&chip);
	world.add(&chip2);
	world.add(&chip3);
	world.add(&chip4);
	world.add(&fightTest);
	world.add(&minion);
	world.add(&diaTest);
	world.add(&m_slime);
	world.setCamera(eTest.getCamera());
	//music test stuff
	
	AudioHandler ah;
	ah.playRandom();
#ifdef WITH_NETWORKING
	char send[1000], old[1000], *in;
	bool changeInInfoSoSend = false;
#endif
	if(!world.getSuccess()){printf("The map was loaded unsuccessfully. THERE IS A PROBLEM.");}
	UserInput aui;
	MyFont hi;
	TTtext fps;
	int ifps = 0;
	char cfps[10];
	fps.setFont(hi.getFont());
	fps.setMessage("0");
	while(running) //While loop, can be easily used for testing.
	{
		ifps++;
		//update the time
		now = SDL_GetTicks();
		passed = now - then;
		then = now;
		if(now - second > 1000)
		{
			sprintf_s(cfps, "%i", ifps);
			fps.setMessage(cfps);
			ifps = 0;
			second = now;
		}
#ifdef WITH_NETWORKING
		if(c.getInbox()->getRawList() != NULL&& (unsigned char *)old != c.getInbox()->getRawList())
		{
			in = (char *)c.getInbox()->getRawList();
			c.getInbox()->clear();
			printf("server info: %s\n",in);
			world.convertFromServer(in);
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
	//				printf("mouse state : %i \n", e.motion.state);
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
	//				printf("Key Pressed: %c \n", e.key.keysym.sym);
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
		eTest.handleInput(&ui, &world);
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
		fps.printMessage(screen, 0,0);
	//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
#ifdef WITH_NETWORKING
		strcpy(send, world.convertAllEntitiesToCharBuffer());
		for(int i = 0; i < strlen(send)&& changeInInfoSoSend == false; i++)
		{
			if(old[i] != send[i])
				changeInInfoSoSend = true;
		}
		if(changeInInfoSoSend)
		{	
	//		printf("sent: %i vs hit: %i\n", send[2], ui.getClick());
			c.getOutbox()->add(send);
			for(int i = 0; i < strlen(send); i++)
			{
				old[i] = send[i];
			}
			changeInInfoSoSend = false;
		//	printf("user in: %c %c\n", ui.getKeyLR(), ui.getKeyUD());
		}
#endif
		//reset the mouse input. Why was this so hard to figure out?
		ui.resetClick();
#ifdef WITH_NETWORKING
		
		for(int i = 0; i<3;i++){
			s.run();
			c.run();
		}
#endif
		SDL_Flip(screen);
		SDL_Delay(FRAME_RATE);
	}

	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}