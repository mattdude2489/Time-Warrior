#include "baseEngine.h"
#include "baseEngineState.h"

void baseEngine::init()
{
	/*cWorld = new World();
	cWorld->setWorld("Maps/HubWorldMap.txt");
	cPlayer = new Player();
	cPlayer->setCamera
	cPlayer->setWorld(cWorld);*/
	//baseEngine::cState = titleScreenState::instance();
	//baseEngine::cState->enter();
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN);
	cState = titleScreenState::instance();
	cState->enter(this);
}

baseEngine::~baseEngine()
{
	/*delete cWorld;
	delete cPlayer;*/
//	SDL_FreeSurface(screen);
}

baseEngine::baseEngine()
{
	init();
}