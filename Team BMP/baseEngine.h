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

struct State
{
	virtual void enter() = 0;
	virtual void execute() = 0;
	virtual void exit() = 0;
	//virtual static State* instance()=0; //I DEAR GOD TO HOPE THIS WORKS.
};

class baseEngine
{
private:
	Player cPlayer;
	World cWorld;
	AudioHandler cah;
	//c in front of any variables usually means "client."
public:
	static State* cState;
	World* getWorld() {return &cWorld;}
	Player* getPlayer() {return &cPlayer;}
	void init();
	baseEngine();
	~baseEngine();
	void setPlayer(Player play) {cPlayer = play;}
	void setWorld(World omg) {cWorld = omg;} //My...god...ok, this isn't so bad.
};