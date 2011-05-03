#include "baseEngine.h"
#include "baseEngineState.h"

void baseEngine::init()
{
	cWorld.setWorld("Maps/HubWorldMap.txt");
	cPlayer.init();
}

baseEngine::baseEngine()
{
	init();
}