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

class baseEngine;

struct State
{
	virtual void enter(baseEngine* be) {}
	virtual void execute(baseEngine* be){}
	virtual void exit(baseEngine* be) {}
	virtual void handleInput(UserInput * obj) {};
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
public:
	World* getWorld() {return cWorld;}
	Player* getPlayer() {return cPlayer;}
	AudioHandler * getAH() {return &cah;}
	void init();
	void changeState(State* newState) {cState = newState; cState->enter(this);}
	Hud * getHUD() {return &Ghud;}
	baseEngine();
	State* getState() {return cState;}
	~baseEngine();
	void setPlayer(Player * play) {cPlayer = play;}
	void updateState() {cState->execute(this);}
	void setWorld(World * omg) {cWorld = omg;} //My...god...ok, this isn't so bad.
	void handleInput(UserInput * obj) {cState->handleInput(obj);}
	baseEngine * instance() {static baseEngine be; return &be;}
};