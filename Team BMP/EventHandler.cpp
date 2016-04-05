#include "EventHandler.h"

#pragma once

EventHandler::EventHandler()
{
	EventHandler(0);
}

EventHandler::EventHandler(int currentWorld)
{
	changeWorldEvents(currentWorld);
}

EventHandler::~EventHandler()
{
	
}

void EventHandler::changeWorldEvents(int currentWorld)
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
/*
The main Event item. It takes in the eventNum, taken when a player collides with the entity of the Event.

eventNum; the number, 1-0 (ten numbers total), of the event. This can be changed later on.
currentWorld; this will be needed to be able to alter the player, NPC's, or anything else.

Event trappings: The events will play out from a text file, labeled Event(currentWorld int).txt.
	Move(Int char, int dirX, int dirY, int facing): Move a certain character, 0 is player, 1 is an NPC, etc, to a place. Facing is determined by #'s 0 (up), 1 (left), 2 (right), and 3 (down).
	Talk(char * Name, char * Script): Make a character say a certain thing.
	Effect(Int effect, int x, int y, int dirX, int dirY)
		Name an effect, like a magic effect or sprite, at a certain place, that heads towards another one.
	Add(int chip): Adds a chip into the player's gauntlet.
	ChangeWorld(int world, int X, int Y): changes the World, and relocates player to X and Y.
	Remove(int chip): Removes a chip from the player's gauntlet. (We can do that, right?)

	All commands will be separated by a ;, all scripts will end with #. The first integer will be which event it is.
	Spaces are to be ignored.
*/
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

/*
Example Script: event0.txt
0 talk playerName "Ow, what am I doing here?"; talk "Guard" "Get up and go. No time to explain."; move 0 4 5 3; talk playerName "Fine...";#
*/