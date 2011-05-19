#pragma once
#include "baseEngine.h"
#include "srect.h"
//A very simple state struct.

//SDL includes and defines
#define SDLDELAY		17
#include "sdl/sdl.h"
#include <stdio.h>
#include <conio.h>

//THIS NEXT LINE IS OMGWTFDANGEROUS, BE CAREFUL WHEN USING THIS LINE.
#define SDL_SCREEN_STARTUP			SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE)

class baseEngine;

//States:
/*
	actualGameState							-
		NetworkGameState				-
			ServerGameState					-
			ClientGameState					-
				-NewGameState			-
				-LoadGameState			-
	titleScreenState				-
		NewGameState					-
		LoadGameState					-
		NetworkGameState				-
	splashScreenState			-

	actualGameState will be the single-player. It won't worry about networking at all.
	titleScreenState will be the title screen. It has three states that it can go to.
		Each of those states are different ways to enter the actual game state.
		the Network game state has two other states, the server game and the client game. Both deal with networking.
	Splash screen state does...literally nothing.
*/

class actualGameState : public State
{
private:
	//INCLUDE SDL STUFF HERES
	SDL_Surface * screen;
	UserInput * stateUI;
	//Uint32 then = SDL_GetTicks(), now, passed, second = 0;
	Uint32 then, now, passed, second;
	TTtext fps;
	MyFont myfps;
	char cfps[10];
	int ifps;
public:
	void enter(baseEngine* be) 
	{
		screen = SDL_SCREEN_STARTUP;
		stateUI = NULL;
		then = SDL_GetTicks();
		now = passed = second = 0;
		myfps.setFont(FONTSIZE);
		fps.setFont(myfps.getFont());
		ifps = 0;
		fps.setMessage("0");
	}
	void execute(baseEngine* be) 
	{
		ifps++;
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
		if(stateUI != NULL)
		{
			be->getPlayer()->handleInput(stateUI, be->getWorld(), be->getAH());
			be->getWorld()->update(passed);
			be->getAH()->update(be->getWorld()->getCurrentWorld());
			be->getHUD()->updateHud(be->getPlayer(), stateUI, passed);
		}

		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
		be->getWorld()->draw(screen);
		be->getHUD()->draw(screen);
		fps.printMessage(screen, 0, 0);
		SDL_Flip(screen);
	} //Input Logic Draw
	void exit(baseEngine* be) {SDL_FreeSurface(screen);}
	static actualGameState* instance() {static actualGameState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class newGameState : public State
{
private:
	char playerName[21];
	char * enterNameMessage;
	UserInput * stateUI;
	SDL_Sprite newGameScreen;
	SDL_Surface * screen;
	bool typing, finished;
	SRect type;
	SDL_Rect newType;
	int num;
	MyFont hi;
	TTtext playerNewName;
	TTtext enterNewMessage;
public:
	void enter(baseEngine *be) 
	{
		for(int i = 0; i < 20; i++)
		{
			playerName[i] = ' ';
		}
		playerName[20] = 0; //Null terminator.
		enterNameMessage = "Enter new name below: ";
		stateUI = NULL;
		screen = SDL_SCREEN_STARTUP;
		newGameScreen.setSprite("Sprites/newGameScreen.bmp", 626, 470, 0, 1);
		typing = finished = false;
		type.x = newType.x = 219;
		type.y = newType.y = 224;
		type.h = newType.h = 43;
		type.w = newType.w = 204;
		num = 1;
		hi.setFont(FONTSIZE);
		playerNewName.setFont(hi.getFont());
		enterNewMessage.setFont(hi.getFont());
	}
	void execute(baseEngine *be) 
	{
		if(stateUI != NULL)
		{
			if(typing == true)
			{
				char c = stateUI->getLastKey();
				//char c = getch();
				if(c > 0)
				{
					if(c == 8 || c == 7) //Backspace, bell, tab, and newline.
						{
							num--;
							playerName[num] = ' ';
						}
					else if(c == 10)
					{
						//Do nothing.
					}
					else if(c == 13)
					{
						exit(be);
					}
					else if(num < 20)
					{
						playerName[num] = c;
						num++;
					}
				}
			}
			if(type.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI ->getClick() == 1)
			{
				typing = true;
			}
		}
		playerNewName.setMessage(playerName);
		enterNewMessage.setMessage(enterNameMessage);
		//Draw
		newGameScreen.draw(screen, 0, 0);
		enterNewMessage.printMessage(screen, 100, 100);
		SDL_FillRect(screen, &newType , 0xff0000);
		playerNewName.printMessage(screen, type.x, type.y+15);
		SDL_Flip(screen);
	}
	void exit(baseEngine *be) 
	{
		be->getPlayer()->newGame();
		be->getPlayer()->setName(playerName);
		be->changeState(actualGameState::instance());
	}
	void handleInput(UserInput * obj) {stateUI = obj;}
	static newGameState* instance() {static newGameState instance; return &instance;}
};

class titleScreenState : public State
{
private:
	SRect newGame;
	SRect networkGame;
	SRect loadGame;
	SDL_Sprite titleScreen;
	int choice;
	SDL_Surface * screen;
	UserInput * stateUI;
public:

	void enter(baseEngine* be) 
	{
		titleScreen.setSprite("Sprites/posTitleScreen.bmp", 626, 470, 0, 1); //MAGIC NUMBAHS = BAD
		choice = 0;
		newGame.setPosition(SPoint(68, 292));
		newGame.setDimension(SPoint(110, 137));
		networkGame.setDimension(SPoint(170, 117));
		networkGame.setPosition(SPoint(235, 58));
		loadGame.setDimension(SPoint(110, 137)); //These are debug numbers, btw. Not entirely accurate.
		loadGame.setPosition(SPoint(443, 306)); //Setting up the stuffs...
		//INCLUDE SDL STUFF HERE
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		stateUI = NULL;
	}
	void execute(baseEngine* be) 
	{
		//Input Logic Draw
		if(stateUI != NULL)
		{
			if(newGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY())) && stateUI->getClick() == 1)
				choice = 1;
			else if(loadGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY()))&& stateUI->getClick() == 1)
				choice = 2;
			else if(networkGame.contains(SPoint(stateUI->getMouseX(), stateUI->getMouseY()))&& stateUI->getClick() == 1)
				choice = 3;
		}
		if(choice != 0)
			exit(be);

		titleScreen.draw(screen, 0, 0);
		SDL_Flip(screen);
		SDL_Delay(SDLDELAY);
	}//Title screen stuffs.
	void exit(baseEngine* be) 
	{
		//Use the choice variable here, that's set during execute!
		SDL_FreeSurface(screen);
		switch(choice)
		{
		case 1:
			be->changeState(newGameState::instance());
			break;
		case 2:
			be->changeState(actualGameState::instance());
			break;
		case 3:
			be->changeState(actualGameState::instance());
			break;
		default:
			break;
		}
		//IT SHOULD NEVER MAKE IT TO HERE. IF IT DOES, THEN SOMETHING IS WRONG.
		enter(be);
	}
	static titleScreenState* instance() {static titleScreenState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class splashScreenState : public State
{
public:
	void enter(baseEngine* be) {}
	void execute(baseEngine* be) {} //Timer or something similar.
	void exit(baseEngine* be) {be->changeState(titleScreenState::instance()); }
	static splashScreenState* instance() {static splashScreenState instance; return &instance;}
	void handleInput(UserInput * obj) {}
};