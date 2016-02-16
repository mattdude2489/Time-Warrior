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
	FILE * infile;
	char * fileName;
	//For now this will work. Another possibility is to simply append the currentWorld onto the fileName. Like; Event0.txt or Event1.txt.
	switch (currentWorld)
	{
	case 0: //Hub World
		break;
	case 1: //England/Medieval World
		break;
	case 2: //Dungeons from Medieval World
		break;
	case 3: //Castle World
		break;
	case 4: //Forest from Medieval
		break;
	case 5: //Desert from Medieval
		break;
	case 6: //Unknown
		break;
	case 7: //Town from Medieval
		break;
	}
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

