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
	Player eTest(100,100,0,0,9,0,0,0,&test);
	SDL_Sprite test2("Sprites/dummy.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, 1);
	SDL_Sprite green("Sprites/greenguy.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Entity fightTest(100,100,0,0,0,0,0,0,&test2);
	NonPlayerChar diaTest(&green);
	SDL_Sprite test3("Sprites/skeleton.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Minion minion(100,100,10,0,0,0,0,0,&test3);
	SDL_Sprite slime("Sprites/slime.bmp", FRAME_SIZE-1, 23, FRAME_RATE, NUM_ROWS);
	Minion m_slime(100,100,10,0,0,0,0,0,&slime);
	SDL_Sprite ghost("Sprites/ghost.bmp", FRAME_SIZE, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Minion m_ghost(100,100,10,0,0,0,0,0,&ghost);
	m_ghost.move(-200,200);
	fightTest.move(100, -100);
	minion.move(-50, -50);
	m_slime.move(-100,-100);
	diaTest.move(200, 200);
	BasicSlash chip;
	BasicDivine d1;
	AdvancedDivine d2;
	ExpertDivine d3;
	BasicFire f1;
	AdvancedFire f2;
	ExpertFire f3;
	eTest.addToAttackInventory(&chip);
	eTest.addToAttackInventory(&d1);
	eTest.addToAttackInventory(&d2);
	eTest.addToAttackInventory(&d3);
	eTest.addToAttackInventory(&f1);
	eTest.addToAttackInventory(&f2);
	eTest.addToAttackInventory(&f3);
	Hud Ghud;
	World world;
	chip.levelUp();
	d1.levelUp();
	d2.levelUp();
	d3.levelUp();
	f1.levelUp();
	f2.levelUp();
	f3.levelUp();
	eTest.setGauntletSlot(SLOT_ATK1, &chip);
	eTest.setGauntletSlot(SLOT_ATK2, &f1);
	world.add(&eTest);
	world.add(&chip);
	world.add(&d1);
	world.add(&d2);
	world.add(&d3);
	world.add(&f1);
	world.add(&f2);
	world.add(&f3);
	//world.add(&fightTest);
	//world.add(&minion);
	//world.add(&diaTest);
	//world.add(&m_slime);
	//world.add(&m_ghost);
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