#include "sdl/sdl.h"
#include "SDL_Sprite.h"
#include "srect.h"
#include "spoint.h"
//#include "UserInput.h"
#include "Chip.h"
#include "Entity.h"
//#include "Player.h"
//#include "World.h"
#include "Hud.h"
//#include "audiohandler.h"
#include "servermodule.h"
#include "Minion.h"
//#include "NPC.h"
#include "baseEngine.h"
//#define WITH_NETWORKING
#define SDLDELAY		17

//Some debugging includes
#include <stdio.h>

int main(int argc, char ** argv)//must be the header for sdl application and yes the arg stuff
{
	//INIT

	srand((unsigned int)time(NULL));
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
	baseEngine be;

	Hud Ghud;
	World world;
	SDL_Sprite test("Sprites/SpriteTest.bmp", 24, FRAME_SIZE, FRAME_RATE, NUM_ROWS);
	Player eTest(ONE_HUNDRED_PERCENT, ONE_HUNDRED_PERCENT, false, false, false, false, false, false, &test, &world);
	world.add(&eTest);
	world.setCamera(eTest.getCamera());
	printf("%d %d %d", sizeof(test), sizeof(world), sizeof(eTest));
	//music test stuff
	AudioHandler ah;
	ah.playRandom();
	
	be.setWorld(&world);
	be.setPlayer(&eTest);
#ifdef WITH_NETWORKING
	char send[1000], old[1000], *in;
	bool changeInInfoSoSend = false;
#endif
	if(!be.getWorld()->getSuccess()){printf("The map was loaded unsuccessfully. THERE IS A PROBLEM.");}
	UserInput aui;
	MyFont hi;
	TTtext fps;
	int ifps = 0;
	char cfps[10];
	hi.setFont(FONTSIZE);
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
		eTest.handleInput(&ui, &world, &ah);
#else
		//eTest.handleInput(&ui, &world, &ah);
#endif
		be.handleInput(&ui);
		//update
		be.updateState();
		//world.update(passed);
		//Ghud.updateHud(&eTest, &ui, passed);
		//ah.update(world.getCurrentWorld());//need to figure out to switch songs when entering a new area
		//draw
		//reset the screen.
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		//world.draw(screen);
		//Ghud.draw(screen);
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
//		SDL_Flip(screen);
		SDL_Delay(SDLDELAY);
		if(ui.getX())
			running = false;
	}
	SDL_FreeSurface(screen);
	SDL_Quit();//must do to free the memory that was used for init	
	return 0;
}