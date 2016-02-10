#include "EventHandler.h"

#pragma once

EventHandler::EventHandler()
{
	EventHandler(0);
}

EventHandler::EventHandler(int currentWorld)
{
	changeWorlds(currentWorld);
}

EventHandler::~EventHandler()
{

}

void EventHandler::changeWorlds(int currentWorld)
{
	//Load up the file for the world the player is going into.

}

void EventHandler::playEvent(int eventNum, World * currentWorld)
{
	//Pseudo coding this for now.

	//Check to see if the event has been completed already

	//If yes, skip and do nothing.
	//If no, continue.

	//Read the script file through and do exactly as it tells, following the gameplay pattern. (IE, move up, move down, etc.)

	//At the end, whatever it needs to do.
	//Records the player seeing the event
	//Kicks the player back out into normal gameplay.
}

