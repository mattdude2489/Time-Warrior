#pragma once
#include "baseEngine.h"
#include "srect.h"
//A very simple state struct.

class baseEngine;

class actualGameState : public State
{
public:
	void enter() {}
	void execute() {} //Input Logic Draw
	void exit() {}
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
	}
	void execute() 
	{
		
	}//Title screen stuffs.
	void exit() {baseEngine::cState = actualGameState::instance(); baseEngine::cState->enter(); }
	static titleScreenState* instance() {static titleScreenState instance; return &instance;}
	void handleInput(UserInput obj) {}
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