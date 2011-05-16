#include "baseEngine.h"
#include "baseEngineState.h"

void baseEngine::init()
{
	cWorld->setWorld("Maps/HubWorldMap.txt");
	cPlayer->init();
	//baseEngine::cState = titleScreenState::instance();
	//baseEngine::cState->enter();
}

baseEngine::~baseEngine()
{

}

baseEngine::baseEngine()
{
	init();
}