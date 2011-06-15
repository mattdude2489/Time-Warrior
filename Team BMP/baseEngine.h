//the Base engine Finite State Machine
//Started by Matthew Morrill, perfected by John Parks (I can already tell John. I can already tell.)

#pragma once

//#include "baseEngineState.h"
#include "World.h"
#include "Player.h"
#include "UserInput.h"
#include "trueTextPrinter.h"
#include "audioHandler.h"
#include "SDL_Sprite.h"
#include "NPC.h"
#include "Hud.h"
#include "Server.h"
#include "Client.h"

class baseEngine;

struct State
{
	virtual void enter(baseEngine* be) {}
	virtual void execute(baseEngine* be){}
	virtual void exit(baseEngine* be) {}
	virtual void handleInput(UserInput * obj) {}
	//virtual static State* instance()=0; //I DEAR GOD TO HOPE THIS WORKS.
};

class baseEngine
{
private:
	Player * cPlayer;
	World * cWorld;
	AudioHandler cah;
	Hud Ghud;
	//c in front of any variables usually means "client."
	State* cState;
	SDL_Surface * screen;
	bool running;
public:
	World* getWorld() {return cWorld;}
	Player* getPlayer() {return cPlayer;}
	AudioHandler * getAH() {return &cah;}
	void init();
	void setRun(bool run) {running = run;}
	bool getRun() {return running;}
	void changeState(State* newState) {cState = newState; cState->enter(this);}
	void goToTitleScreen();
	SDL_Surface * getScreen() {return screen;}
	Hud * getHUD() {return &Ghud;}
	baseEngine();
	State* getState() {return cState;}
	~baseEngine();
	void setPlayer(Player * play) {cPlayer = play;}
	void updateState() 
	{
		SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); //Resets the color scheme back to black.
		cState->execute(this);
	}
	void setWorld(World * omg) {cWorld = omg;} //My...god...ok, this isn't so bad.
	void handleInput(UserInput * obj) {cState->handleInput(obj);}
	baseEngine * instance() {static baseEngine be; return &be;}
};