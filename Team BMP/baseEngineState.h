#pragma once
#include "baseEngine.h"
#include "srect.h"
//A very simple state struct.

//SDL includes and defines
#define SDLDELAY		17
#include "sdl/sdl.h"
#include <stdio.h>

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
public:
	void enter() {screen = SDL_SCREEN_STARTUP;}
	void execute() {} //Input Logic Draw
	void exit() {SDL_FreeSurface(screen);}
	static actualGameState* instance() {static actualGameState instance; return &instance;}
	void handleInput(UserInput obj) {}
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

	void enter() 
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
		screen = SDL_SCREEN_STARTUP;
		stateUI = NULL;
	}
	void execute() 
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

		titleScreen.draw(screen, 0, 0);
		SDL_Flip(screen);
		SDL_Delay(SDLDELAY);
	}//Title screen stuffs.
	void exit() 
	{
		//Use the choice variable here, that's set during execute!
		SDL_FreeSurface(screen);
		switch(choice)
		{
		case 1:
			baseEngine::cState = actualGameState::instance();
			baseEngine::cState->enter();
			break;
		case 2:
			baseEngine::cState = actualGameState::instance();
			baseEngine::cState->enter();
			break;
		case 3:
			baseEngine::cState = actualGameState::instance();
			baseEngine::cState->enter();
			break;
		default:
			break;
		}
		//IT SHOULD NEVER MAKE IT TO HERE. IF IT DOES, THEN SOMETHING IS WRONG.
		enter();
	}
	static titleScreenState* instance() {static titleScreenState instance; return &instance;}
	void handleInput(UserInput * obj) {stateUI = obj;}
};

class splashScreenState : public State
{
public:
	void enter() {}
	void execute() {} //Timer or something similar.
	void exit() {baseEngine::cState = titleScreenState::instance(); baseEngine::cState->enter(); }
	static splashScreenState* instance() {static splashScreenState instance; return &instance;}
	void handleInput(UserInput obj) {}
};